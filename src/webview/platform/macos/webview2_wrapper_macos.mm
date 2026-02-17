#import "../../webview2_wrapper.h"
#import <Cocoa/Cocoa.h>
#import <MSWebView2Staging/MSWebView2.h>
#import <objc/runtime.h>
#include <iostream>

// Objective-C delegate to handle web messages
@interface WebView2MessageDelegate : NSObject <MSWebView2WebMessageDelegate>
@property (nonatomic, assign) webview2::WebView2Instance* instance;
@end

@implementation WebView2MessageDelegate
- (void)webView:(MSWebView2WebView*)webView
    webMessageReceived:(MSWebView2WebMessageReceivedInfo*)info {
    NSString* jsonMessage = info.webMessageAsJSON;
    std::cout << "Received web message: " << [jsonMessage UTF8String] << std::endl;

    // Parse JSON message
    NSData* data = [jsonMessage dataUsingEncoding:NSUTF8StringEncoding];
    NSError* error = nil;
    NSDictionary* json = [NSJSONSerialization JSONObjectWithData:data options:0 error:&error];

    if (error || !json) {
        std::cerr << "Failed to parse message JSON: " << [[error localizedDescription] UTF8String] << std::endl;
        return;
    }

    NSString* handlerName = json[@"handler"];
    id handlerDataObj = json[@"data"];

    if (!handlerName) {
        std::cerr << "Message missing handler name" << std::endl;
        return;
    }

    // Serialize data back to JSON string if it's not already a string
    NSString* handlerData = @"";
    if (handlerDataObj) {
        if ([handlerDataObj isKindOfClass:[NSString class]]) {
            handlerData = (NSString*)handlerDataObj;
        } else {
            // Serialize dictionary/array/object to JSON
            NSError* serializeError = nil;
            NSData* jsonData = [NSJSONSerialization dataWithJSONObject:handlerDataObj
                                                               options:0
                                                                 error:&serializeError];
            if (!serializeError && jsonData) {
                handlerData = [[NSString alloc] initWithData:jsonData encoding:NSUTF8StringEncoding];
            }
        }
    }

    // Call the C++ callback
    if (self.instance) {
        self.instance->handleMessage([handlerName UTF8String],
                                     handlerData ? [handlerData UTF8String] : "");
    }
}
@end

namespace webview2 {

// =============================================================================
// WebView2Instance Implementation
// =============================================================================

WebView2Instance::WebView2Instance(void* nativeWindow, bool debug)
    : nativeWindow_(nativeWindow)
    , debug_(debug)
    , initialized_(false)
    , controller_(nullptr)
    , webviewCore_(nullptr)
    , messageDelegate_(nullptr) {
}

void WebView2Instance::close() {
    @autoreleasepool {
        if (!initialized_ || !controller_) {
            std::cout << "WebView2Instance::close - already closed or not initialized" << std::endl;
            return;
        }

        std::cout << "WebView2Instance::close - starting explicit close" << std::endl;

        MSWebView2Controller* controller = (__bridge MSWebView2Controller*)controller_;
        MSWebView2WebView* webView = (__bridge MSWebView2WebView*)webviewCore_;

        // Clear delegate to prevent callbacks during shutdown
        if (webView && messageDelegate_) {
            std::cout << ">>> Clearing message delegate" << std::endl;
            webView.webMessageDelegate = nil;
        }

        // Close the controller - this is the critical step Teams must be doing
        // This should remove the webview from the window and clean up properly
        if (controller) {
            std::cout << ">>> Calling [MSWebView2Controller close]" << std::endl;
            [controller close];
        }

        // Now release our references
        if (messageDelegate_) {
            WebView2MessageDelegate* delegate = (__bridge_transfer WebView2MessageDelegate*)messageDelegate_;
            delegate = nil;
            messageDelegate_ = nullptr;
        }

        if (webviewCore_) {
            MSWebView2WebView* wv = (__bridge_transfer MSWebView2WebView*)webviewCore_;
            wv = nil;
            webviewCore_ = nullptr;
        }

        if (controller_) {
            MSWebView2Controller* ctrl = (__bridge_transfer MSWebView2Controller*)controller_;
            ctrl = nil;
            controller_ = nullptr;
        }

        initialized_ = false;
        std::cout << "WebView2Instance::close - completed" << std::endl;
    }
}

WebView2Instance::~WebView2Instance() {
    @autoreleasepool {
        std::cout << "WebView2Instance destroying..." << std::endl;

        // If close() wasn't called explicitly, do cleanup now
        if (initialized_ && controller_) {
            std::cout << ">>> Warning: close() not called before destructor, closing now" << std::endl;
            close();
        }

        std::cout << "WebView2Instance destroyed" << std::endl;
    }
}

bool WebView2Instance::initialize() {
    @autoreleasepool {
        if (initialized_) {
            std::cerr << "WebView2Instance already initialized" << std::endl;
            return false;
        }

        if (!nativeWindow_) {
            std::cerr << "Invalid native window" << std::endl;
            return false;
        }

        NSWindow* window = (__bridge NSWindow*)nativeWindow_;
        NSView* contentView = window.contentView;

        std::cout << "Initializing WebView2 for window: " << contentView.bounds.size.width
                  << "x" << contentView.bounds.size.height << std::endl;

        // Get shared environment from WebView2Manager
        MSWebView2Environment* environment = (__bridge MSWebView2Environment*)WebView2Manager::getInstance().getEnvironment();

        if (!environment) {
            std::cerr << "WebView2Environment not initialized" << std::endl;
            return false;
        }

        // Create controller options
        MSWebView2ControllerOptions* options = [[MSWebView2ControllerOptions alloc] init];
        // Use default profile for now

        // Create controller asynchronously
        __block bool creationComplete = false;
        __block bool creationSuccess = false;
        __block MSWebView2Controller* createdController = nil;

        [environment createWebViewController:contentView
                                     options:options
                           completionHandler:^(MSWebView2Controller* controller, NSError* error) {
            if (error) {
                std::cerr << "Failed to create WebView2 controller: "
                         << [[error localizedDescription] UTF8String] << std::endl;
                creationSuccess = false;
            } else {
                createdController = controller;
                creationSuccess = true;
                std::cout << "WebView2 controller created successfully" << std::endl;
            }
            creationComplete = true;
        }];

        // Wait for creation to complete (this is acceptable during initialization)
        while (!creationComplete) {
            [[NSRunLoop currentRunLoop] runMode:NSDefaultRunLoopMode
                                     beforeDate:[NSDate dateWithTimeIntervalSinceNow:0.01]];
        }

        if (!creationSuccess || !createdController) {
            std::cerr << "WebView2 controller creation failed" << std::endl;
            return false;
        }

        // Get the webview from controller
        MSWebView2WebView* webView = createdController.webview;

        // Configure settings
        MSWebView2Settings* settings = webView.settings;
        settings.devToolsEnabled = debug_;
        settings.zoomControlEnabled = NO;
        settings.builtInErrorPageEnabled = YES;
        settings.webMessageEnabled = YES;  // Enable web messages for JS-to-C++ communication

        // Set transparent background - try controller's defaultBackgroundColor
        if ([createdController respondsToSelector:@selector(setDefaultBackgroundColor:)]) {
            [createdController setDefaultBackgroundColor:[NSColor clearColor]];
            std::cout << "Set controller defaultBackgroundColor to clearColor" << std::endl;
        } else if ([createdController respondsToSelector:@selector(defaultBackgroundColor)]) {
            // Try as property
            createdController.defaultBackgroundColor = [NSColor clearColor];
            std::cout << "Set controller defaultBackgroundColor property to clearColor" << std::endl;
        } else {
            std::cout << "Controller does not have defaultBackgroundColor property" << std::endl;
        }

        // Store references
        controller_ = (__bridge_retained void*)createdController;
        webviewCore_ = (__bridge_retained void*)webView;

        // Inject transparency CSS that runs on every page load
        NSString* transparencyCss = @"(function() { \
            const style = document.createElement('style'); \
            style.id = 'webview2-transparency'; \
            style.textContent = 'html { background: transparent !important; } \
                                 body { background: transparent !important; margin: 0; padding: 0; overflow: hidden; } \
                                 #root { background: transparent !important; }'; \
            const applyStyle = function() { \
                if (document.head && !document.getElementById('webview2-transparency')) { \
                    document.head.insertBefore(style, document.head.firstChild); \
                    console.log('Transparency CSS applied'); \
                } \
            }; \
            if (document.readyState === 'loading') { \
                document.addEventListener('DOMContentLoaded', applyStyle); \
            } else { \
                applyStyle(); \
            } \
        })();";

        [webView addScriptToExecuteOnDocumentCreated:transparencyCss
                                   completionHandler:^(NSString* scriptID, NSError* error) {
            if (error) {
                std::cerr << "Failed to inject transparency CSS: "
                         << [[error localizedDescription] UTF8String] << std::endl;
            } else {
                std::cout << "Transparency CSS injected on document creation" << std::endl;
            }
        }];

        initialized_ = true;
        std::cout << "WebView2 instance initialized (debug: " << debug_ << ")" << std::endl;
        return true;
    }
}

void WebView2Instance::initializeAsync(InitCallback callback) {
    @autoreleasepool {
        if (initialized_) {
            std::cerr << "WebView2Instance already initialized" << std::endl;
            callback(false);
            return;
        }

        if (!nativeWindow_) {
            std::cerr << "Invalid native window" << std::endl;
            callback(false);
            return;
        }

        NSWindow* window = (__bridge NSWindow*)nativeWindow_;
        NSView* contentView = window.contentView;

        std::cout << "Initializing WebView2 async for window: " << contentView.bounds.size.width
                  << "x" << contentView.bounds.size.height << std::endl;

        // Get shared environment from WebView2Manager
        MSWebView2Environment* environment = (__bridge MSWebView2Environment*)WebView2Manager::getInstance().getEnvironment();

        if (!environment) {
            std::cerr << "WebView2Environment not initialized" << std::endl;
            callback(false);
            return;
        }

        // Create controller options
        MSWebView2ControllerOptions* options = [[MSWebView2ControllerOptions alloc] init];

        // Capture 'this' and callback for use in completion handler
        auto instancePtr = this;
        auto callbackCopy = callback;

        // Create controller asynchronously - NO BLOCKING
        [environment createWebViewController:contentView
                                     options:options
                           completionHandler:^(MSWebView2Controller* controller, NSError* error) {
            @autoreleasepool {
                if (error) {
                    std::cerr << "Failed to create WebView2 controller: "
                             << [[error localizedDescription] UTF8String] << std::endl;
                    callbackCopy(false);
                    return;
                }

                std::cout << "WebView2 controller created successfully (async)" << std::endl;

                // Get the webview from controller
                MSWebView2WebView* webView = controller.webview;

                // Configure settings
                MSWebView2Settings* settings = webView.settings;
                settings.devToolsEnabled = instancePtr->debug_;
                settings.zoomControlEnabled = NO;
                settings.builtInErrorPageEnabled = YES;
                settings.webMessageEnabled = YES;

                // Set transparent background - try controller's defaultBackgroundColor
                if ([controller respondsToSelector:@selector(setDefaultBackgroundColor:)]) {
                    [controller setDefaultBackgroundColor:[NSColor clearColor]];
                    std::cout << "Set controller defaultBackgroundColor to clearColor (async)" << std::endl;
                } else if ([controller respondsToSelector:@selector(defaultBackgroundColor)]) {
                    // Try as property
                    controller.defaultBackgroundColor = [NSColor clearColor];
                    std::cout << "Set controller defaultBackgroundColor property to clearColor (async)" << std::endl;
                } else {
                    std::cout << "Controller does not have defaultBackgroundColor property (async)" << std::endl;
                }

                // Store references
                instancePtr->controller_ = (__bridge_retained void*)controller;
                instancePtr->webviewCore_ = (__bridge_retained void*)webView;
                instancePtr->initialized_ = true;

                std::cout << "WebView2 instance initialized async (debug: " << instancePtr->debug_ << ")" << std::endl;
                callbackCopy(true);
            }
        }];

        std::cout << "WebView2 async initialization started (will complete in background)" << std::endl;
    }
}

void WebView2Instance::setHtml(const std::string& html) {
    @autoreleasepool {
        if (!initialized_ || !webviewCore_) {
            std::cerr << "WebView2Instance not initialized" << std::endl;
            return;
        }

        std::cout << "WebView2::setHtml - length: " << html.length() << std::endl;

        // WebView2 doesn't have loadHTMLString, use data URL instead
        NSString* htmlString = [NSString stringWithUTF8String:html.c_str()];

        // Encode HTML for data URL
        NSString* encoded = [htmlString stringByAddingPercentEncodingWithAllowedCharacters:
            [NSCharacterSet URLQueryAllowedCharacterSet]];
        NSString* dataURL = [NSString stringWithFormat:@"data:text/html;charset=utf-8,%@", encoded];

        MSWebView2WebView* webView = (__bridge MSWebView2WebView*)webviewCore_;
        [webView navigate:dataURL];
    }
}

void WebView2Instance::navigate(const std::string& url) {
    @autoreleasepool {
        if (!initialized_ || !webviewCore_) {
            std::cerr << "WebView2Instance not initialized" << std::endl;
            return;
        }

        std::cout << "WebView2::navigate - " << url << std::endl;

        NSString* urlString = [NSString stringWithUTF8String:url.c_str()];

        // Convert file paths to file:// URLs if needed
        if ([urlString hasPrefix:@"/"]) {
            NSURL* fileURL = [NSURL fileURLWithPath:urlString];
            urlString = [fileURL absoluteString];
        }

        MSWebView2WebView* webView = (__bridge MSWebView2WebView*)webviewCore_;
        [webView navigate:urlString];
    }
}

void WebView2Instance::addInitScript(const std::string& js) {
    @autoreleasepool {
        if (!initialized_ || !webviewCore_) {
            std::cerr << "WebView2Instance not initialized" << std::endl;
            return;
        }

        std::cout << "WebView2::addInitScript - length: " << js.length() << std::endl;

        NSString* jsString = [NSString stringWithUTF8String:js.c_str()];
        MSWebView2WebView* webView = (__bridge MSWebView2WebView*)webviewCore_;

        // Add script to run on every document load
        [webView addScriptToExecuteOnDocumentCreated:jsString
                                   completionHandler:^(NSString* scriptID, NSError* error) {
            if (error) {
                std::cerr << "Failed to add init script: "
                         << [[error localizedDescription] UTF8String] << std::endl;
            } else {
                std::cout << "Init script added with ID: " << [scriptID UTF8String] << std::endl;
            }
        }];
    }
}

void WebView2Instance::executeScript(const std::string& js) {
    @autoreleasepool {
        if (!initialized_ || !webviewCore_) {
            std::cerr << "WebView2Instance not initialized" << std::endl;
            return;
        }

        std::cout << "WebView2::executeScript - length: " << js.length() << std::endl;

        NSString* jsString = [NSString stringWithUTF8String:js.c_str()];
        MSWebView2WebView* webView = (__bridge MSWebView2WebView*)webviewCore_;

        // Execute asynchronously (non-blocking)
        [webView executeScript:jsString
             completionHandler:^(NSString* result, NSError* error) {
            if (error) {
                NSLog(@"JavaScript error: %@", error.localizedDescription);
            }
        }];
    }
}

void WebView2Instance::handleMessage(const std::string& handlerName, const std::string& data) {
    auto it = messageHandlers_.find(handlerName);
    if (it != messageHandlers_.end()) {
        std::cout << "Calling handler: " << handlerName << std::endl;
        it->second(data);
    } else {
        std::cerr << "No handler registered for: " << handlerName << std::endl;
    }
}

void WebView2Instance::addScriptHandler(const std::string& name, MessageCallback callback) {
    @autoreleasepool {
        if (!initialized_ || !webviewCore_) {
            std::cerr << "WebView2Instance not initialized" << std::endl;
            return;
        }

        std::cout << "WebView2::addScriptHandler - " << name << std::endl;

        // Store callback
        messageHandlers_[name] = callback;

        MSWebView2WebView* webView = (__bridge MSWebView2WebView*)webviewCore_;

        // Set web message delegate to receive messages from JavaScript
        if (!messageDelegate_) {
            // Create delegate
            WebView2MessageDelegate* delegate = [[WebView2MessageDelegate alloc] init];
            delegate.instance = this;
            messageDelegate_ = (__bridge_retained void*)delegate;

            // Set the delegate on the webview
            webView.webMessageDelegate = delegate;

            std::cout << "WebView2 message delegate created and set" << std::endl;

            // Inject setup script that wraps the native messaging
            std::string setupScript = R"(
                (function() {
                    window.webview2_handlers = window.webview2_handlers || {};
                    window.webview2_send = function(name, data) {
                        // postMessage accepts objects directly - no need to stringify
                        window.chrome.webview.postMessage({
                            handler: name,
                            data: data
                        });
                    };
                })();
            )";

            NSString* setupScriptNS = [NSString stringWithUTF8String:setupScript.c_str()];
            [webView addScriptToExecuteOnDocumentCreated:setupScriptNS
                                       completionHandler:^(NSString* scriptID, NSError* error) {
                if (error) {
                    NSLog(@"Script injection error: %@", error.localizedDescription);
                } else {
                    std::cout << "Setup script injected" << std::endl;
                }
            }];
        }

        std::cout << "WebView2 message handler registered: " << name << std::endl;
    }
}

// Helper function to recursively configure view transparency for WebView2
static void configureViewTransparency(NSView* view, int depth = 0) {
    @autoreleasepool {
        NSString* className = NSStringFromClass([view class]);
        NSString* indent = [@"" stringByPaddingToLength:depth * 2 withString:@" " startingAtIndex:0];

        // Log all views for debugging (only top levels)
        if (depth == 0 || depth <= 2) {
            std::cout << [indent UTF8String] << "View: " << [className UTF8String] << std::endl;
        }

        // Make ALL views transparent
        [view setWantsLayer:YES];
        if (view.layer) {
            view.layer.backgroundColor = [[NSColor clearColor] CGColor];
            view.layer.opaque = NO;
        }

        // Disable background drawing if supported
        if ([view respondsToSelector:@selector(setDrawsBackground:)]) {
            [(id)view setDrawsBackground:NO];
        }

        // Clear any compositing filters
        if ([view respondsToSelector:@selector(setCompositingFilter:)]) {
            [(id)view setCompositingFilter:nil];
        }

        // Special handling for WebView2 compositor views
        if ([className containsString:@"Compositor"] ||
            [className containsString:@"RenderWidget"] ||
            [className containsString:@"WebContents"]) {
            std::cout << [indent UTF8String] << "  -> Configuring " << [className UTF8String] << " for transparency" << std::endl;

            // Enable transparency in the compositor
            if ([view respondsToSelector:@selector(setBackgroundColor:)]) {
                [(id)view setBackgroundColor:[NSColor clearColor]];
            }

            // Allow layer content to be transparent
            if (view.layer) {
                view.layer.masksToBounds = NO;
            }
        }

        // Recursively configure subviews
        for (NSView* subview in view.subviews) {
            configureViewTransparency(subview, depth + 1);
        }
    }
}

void WebView2Instance::configureTransparency() {
    @autoreleasepool {
        if (!initialized_ || !nativeWindow_) {
            std::cerr << "WebView2Instance not initialized" << std::endl;
            return;
        }

        std::cout << "Configuring WebView2 view transparency..." << std::endl;

        // WebView2 adds its views to the window's contentView
        NSWindow* window = (__bridge NSWindow*)nativeWindow_;
        NSView* contentView = window.contentView;

        // Recursively configure all WebView2-related views for transparency
        configureViewTransparency(contentView);

        std::cout << "WebView2 view transparency configuration complete" << std::endl;
    }
}

void WebView2Instance::setDevToolsEnabled(bool enabled) {
    @autoreleasepool {
        if (!initialized_ || !webviewCore_) {
            std::cerr << "WebView2Instance not initialized" << std::endl;
            return;
        }

        std::cout << "WebView2::setDevToolsEnabled - " << enabled << std::endl;

        MSWebView2WebView* webView = (__bridge MSWebView2WebView*)webviewCore_;
        webView.settings.devToolsEnabled = enabled;
    }
}

// =============================================================================
// WebView2Manager Implementation
// =============================================================================

WebView2Manager::WebView2Manager()
    : initialized_(false)
    , environment_(nullptr) {
}

WebView2Manager::~WebView2Manager() {
    shutdown();
}

WebView2Manager& WebView2Manager::getInstance() {
    static WebView2Manager instance;
    return instance;
}

bool WebView2Manager::initialize() {
    @autoreleasepool {
        if (initialized_) {
            std::cout << "WebView2Manager already initialized" << std::endl;
            return true;
        }

        std::cout << "Initializing WebView2 environment..." << std::endl;

        // Create environment options
        MSWebView2EnvironmentOptions* options = [[MSWebView2EnvironmentOptions alloc] init];

        // Set user data folder
        NSString* userDataFolder = @"~/Library/Application Support/TeamsBICSandbox/WebView2";
        userDataFolder = [userDataFolder stringByExpandingTildeInPath];

        // Use system-installed WebView2 runtime (default location)
        // Default: ~/Library/Application Support/Microsoft WebView2/Microsoft Edge WebView2.app
        NSString* browserPath = nil;

        __block bool creationComplete = false;
        __block bool creationSuccess = false;
        __block MSWebView2Environment* createdEnvironment = nil;

        // Create environment with system runtime
        CreateMSWebView2Environment(
            browserPath,  // nil = use default system location
            userDataFolder,
            options,
            ^(MSWebView2Environment* environment, NSError* error) {
                if (error) {
                    std::cerr << "Failed to create WebView2 environment: "
                             << [[error localizedDescription] UTF8String] << std::endl;
                    creationSuccess = false;
                } else {
                    createdEnvironment = environment;
                    creationSuccess = true;
                    std::cout << "WebView2 environment created" << std::endl;
                    std::cout << "Browser version: " << [environment.browserVersionString UTF8String] << std::endl;
                }
                creationComplete = true;
            });

        // Wait for environment creation to complete
        while (!creationComplete) {
            [[NSRunLoop currentRunLoop] runMode:NSDefaultRunLoopMode
                                     beforeDate:[NSDate dateWithTimeIntervalSinceNow:0.01]];
        }

        if (!creationSuccess || !createdEnvironment) {
            std::cerr << "WebView2 environment creation failed" << std::endl;
            return false;
        }

        environment_ = (__bridge_retained void*)createdEnvironment;
        initialized_ = true;

        std::cout << "WebView2 environment initialized successfully" << std::endl;
        return true;
    }
}

std::shared_ptr<WebView2Instance> WebView2Manager::createWebView(void* nativeWindow, bool debug) {
    if (!initialized_) {
        std::cerr << "WebView2Manager not initialized" << std::endl;
        return nullptr;
    }

    auto instance = std::make_shared<WebView2Instance>(nativeWindow, debug);
    if (!instance->initialize()) {
        std::cerr << "Failed to initialize WebView2Instance" << std::endl;
        return nullptr;
    }

    instances_.push_back(instance);
    std::cout << "Created WebView2 instance (total: " << instances_.size() << ")" << std::endl;
    return instance;
}

void WebView2Manager::createWebViewAsync(void* nativeWindow, bool debug, std::function<void(std::shared_ptr<WebView2Instance>)> callback) {
    if (!initialized_) {
        std::cerr << "WebView2Manager not initialized" << std::endl;
        callback(nullptr);
        return;
    }

    auto instance = std::make_shared<WebView2Instance>(nativeWindow, debug);

    // Capture instance and callback for async initialization
    auto instanceCopy = instance;
    auto callbackCopy = callback;
    auto thisPtr = this;

    instance->initializeAsync([instanceCopy, callbackCopy, thisPtr](bool success) {
        if (!success) {
            std::cerr << "Failed to initialize WebView2Instance async" << std::endl;
            callbackCopy(nullptr);
            return;
        }

        thisPtr->instances_.push_back(instanceCopy);
        std::cout << "Created WebView2 instance async (total: " << thisPtr->instances_.size() << ")" << std::endl;
        callbackCopy(instanceCopy);
    });

    std::cout << "WebView2 instance creation started (async)" << std::endl;
}

void WebView2Manager::shutdown() {
    @autoreleasepool {
        if (!initialized_) {
            return;
        }

        std::cout << "Shutting down WebView2Manager..." << std::endl;

        // Clear all instances
        instances_.clear();

        // Release environment
        if (environment_) {
            MSWebView2Environment* env = (__bridge_transfer MSWebView2Environment*)environment_;
            env = nil;
            environment_ = nullptr;
        }

        initialized_ = false;
        std::cout << "WebView2Manager shutdown complete" << std::endl;
    }
}

} // namespace webview2
