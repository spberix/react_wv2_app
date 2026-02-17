#import "native_webview_macos.h"
#import <Cocoa/Cocoa.h>
#import <WebKit/WebKit.h>
#include <iostream>

namespace native_webview {

NativeWebView::NativeWebView(int width, int height, bool debug)
    : window_(nullptr)
    , webView_(nullptr)
    , width_(width)
    , height_(height)
    , debug_(debug)
    , initialized_(false) {
}

NativeWebView::~NativeWebView() {
    @autoreleasepool {
        if (webView_) {
            [webView_ removeFromSuperview];
            webView_ = nil;
        }
        if (window_) {
            [window_ close];
            window_ = nil;
        }
    }
}

bool NativeWebView::initialize(const std::string& title) {
    @autoreleasepool {
        if (initialized_) {
            return false;
        }

        // Create NSWindow
        NSRect frame = NSMakeRect(0, 0, width_, height_);
        NSWindow* window = [[NSWindow alloc]
            initWithContentRect:frame
                      styleMask:(NSWindowStyleMaskTitled |
                                NSWindowStyleMaskClosable |
                                NSWindowStyleMaskMiniaturizable |
                                NSWindowStyleMaskResizable)
                        backing:NSBackingStoreBuffered
                          defer:NO];

        if (!window) {
            std::cerr << "Failed to create NSWindow" << std::endl;
            return false;
        }

        // Set window properties
        NSString* titleString = [NSString stringWithUTF8String:title.c_str()];
        [window setTitle:titleString];
        [window center];
        [window setBackgroundColor:[NSColor blackColor]];

        // Create WKWebView configuration
        WKWebViewConfiguration* config = [[WKWebViewConfiguration alloc] init];

        // Enable developer extras if debug mode
        if (debug_) {
            [config.preferences setValue:@YES forKey:@"developerExtrasEnabled"];
        }

        // Create WKWebView
        WKWebView* webView = [[WKWebView alloc] initWithFrame:frame configuration:config];

        if (!webView) {
            std::cerr << "Failed to create WKWebView" << std::endl;
            return false;
        }

        // Configure webview
        webView.autoresizingMask = NSViewWidthSizable | NSViewHeightSizable;
        [webView setValue:@NO forKey:@"drawsBackground"]; // Transparent background

        // Add webview to window
        [window setContentView:webView];

        // Store references
        window_ = window;
        webView_ = webView;
        initialized_ = true;

        // Show window
        [window makeKeyAndOrderFront:nil];

        std::cout << "Native WKWebView initialized: " << width_ << "x" << height_ << std::endl;
        return true;
    }
}

void NativeWebView::setHtml(const std::string& html) {
    @autoreleasepool {
        if (!webView_) {
            std::cerr << "WebView not initialized" << std::endl;
            return;
        }

        NSString* htmlString = [NSString stringWithUTF8String:html.c_str()];
        [webView_ loadHTMLString:htmlString baseURL:nil];
    }
}

void NativeWebView::navigate(const std::string& url) {
    @autoreleasepool {
        if (!webView_) {
            std::cerr << "WebView not initialized" << std::endl;
            return;
        }

        NSString* urlString = [NSString stringWithUTF8String:url.c_str()];
        NSURL* nsUrl = [NSURL URLWithString:urlString];

        if (!nsUrl) {
            // Try as file URL
            nsUrl = [NSURL fileURLWithPath:urlString];
        }

        if (nsUrl) {
            NSURLRequest* request = [NSURLRequest requestWithURL:nsUrl];
            [webView_ loadRequest:request];
        } else {
            std::cerr << "Invalid URL: " << url << std::endl;
        }
    }
}

void NativeWebView::eval(const std::string& js) {
    @autoreleasepool {
        if (!webView_) {
            std::cerr << "WebView not initialized" << std::endl;
            return;
        }

        NSString* jsString = [NSString stringWithUTF8String:js.c_str()];

        [webView_ evaluateJavaScript:jsString completionHandler:^(id result, NSError* error) {
            if (error) {
                NSLog(@"JavaScript error: %@", error.localizedDescription);
            }
        }];
    }
}

void NativeWebView::bind(const std::string& name, void (*callback)(const std::string&)) {
    // TODO: Implement JavaScript to C++ binding using WKScriptMessageHandler
    // This requires creating a message handler and registering it with the webview
    std::cout << "Native binding not yet implemented: " << name << std::endl;
}

void* NativeWebView::getNativeWindow() {
    return (__bridge void*)window_;
}

void* NativeWebView::getNativeWebView() {
    return (__bridge void*)webView_;
}

void initializeNSApplication() {
    @autoreleasepool {
        [NSApplication sharedApplication];
        [NSApp setActivationPolicy:NSApplicationActivationPolicyRegular];
        [NSApp activateIgnoringOtherApps:YES];

        std::cout << "NSApplication initialized" << std::endl;
    }
}

void runNativeEventLoop() {
    @autoreleasepool {
        std::cout << "Starting native NSApplication event loop..." << std::endl;
        [NSApp run];
        std::cout << "NSApplication event loop exited" << std::endl;
    }
}

} // namespace native_webview
