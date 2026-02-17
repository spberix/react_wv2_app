/**
 * Minimal WebView2 Test Application
 *
 * This standalone test verifies WebView2 SDK integration on macOS.
 * Use this to test the SDK before integrating into the main application.
 *
 * Build:
 *   clang++ -o webview2_test webview2_minimal_test.mm \
 *       -I../external/webview2-macos/include \
 *       -L../external/webview2-macos/lib \
 *       -framework Cocoa -framework WebView2 \
 *       -std=c++17 -ObjC++
 *
 * Run:
 *   ./webview2_test
 *
 * Expected result:
 *   - Window opens with HTML content
 *   - Clicking button triggers C++ callback
 *   - Multiple windows can be created without hanging
 */

#import <Cocoa/Cocoa.h>
#include <iostream>
#include <memory>

// TODO: Include WebView2 SDK headers once downloaded
// #import <WebView2/WebView2.h>
// #import <WebView2/WebView2Environment.h>

// Placeholder WebView2 types (replace with actual SDK types)
#ifdef WEBVIEW2_SDK_AVAILABLE
// Use real SDK types
#else
// Placeholders for testing compilation
typedef void* WebView2Environment;
typedef void* WebView2Controller;
typedef void* WebView2Core;
#endif

/**
 * TestWindow - Simple window with WebView2
 */
@interface TestWindow : NSObject {
    NSWindow* window_;
    WebView2Controller controller_;
    int windowNumber_;
}
- (instancetype)initWithNumber:(int)number;
- (bool)initialize;
- (void)show;
- (void)loadHTML;
@end

@implementation TestWindow

- (instancetype)initWithNumber:(int)number {
    self = [super init];
    if (self) {
        windowNumber_ = number;
        window_ = nil;
        controller_ = nil;
    }
    return self;
}

- (bool)initialize {
    // Create NSWindow
    NSRect frame = NSMakeRect(100 + (windowNumber_ * 50),
                             100 + (windowNumber_ * 50),
                             800, 600);

    window_ = [[NSWindow alloc]
        initWithContentRect:frame
                  styleMask:(NSWindowStyleMaskTitled |
                            NSWindowStyleMaskClosable |
                            NSWindowStyleMaskResizable)
                    backing:NSBackingStoreBuffered
                      defer:NO];

    if (!window_) {
        std::cerr << "Failed to create NSWindow" << std::endl;
        return false;
    }

    NSString* title = [NSString stringWithFormat:@"WebView2 Test Window %d", windowNumber_];
    [window_ setTitle:title];
    [window_ setBackgroundColor:[NSColor blackColor]];

#ifdef WEBVIEW2_SDK_AVAILABLE
    // TODO: Create WebView2 controller
    /*
    // Get shared environment
    WebView2Environment* environment = ...; // from manager

    // Create controller options
    auto options = [WebView2ControllerOptions new];
    options.isInPrivateModeEnabled = NO;

    // Create WebView2 controller
    NSError* error = nil;
    controller_ = [[WebView2Controller alloc]
        initWithEnvironment:environment
                     bounds:window_.contentView.bounds
                    options:options
                      error:&error];

    if (!controller_) {
        std::cerr << "Failed to create WebView2 controller: "
                 << [[error localizedDescription] UTF8String] << std::endl;
        return false;
    }

    // Configure settings
    WebView2Core* core = controller_.coreWebView2;
    core.settings.areDevToolsEnabled = YES;
    core.settings.isZoomControlEnabled = YES;

    // Add view to window
    NSView* webView = controller_.view;
    webView.autoresizingMask = NSViewWidthSizable | NSViewHeightSizable;
    [window_.contentView addSubview:webView];

    std::cout << "WebView2 controller created for window " << windowNumber_ << std::endl;
    */
#else
    std::cout << "WebView2 SDK not available - placeholder mode" << std::endl;
    std::cout << "Window " << windowNumber_ << " created (would contain WebView2)" << std::endl;
#endif

    return true;
}

- (void)show {
    [window_ makeKeyAndOrderFront:nil];
}

- (void)loadHTML {
#ifdef WEBVIEW2_SDK_AVAILABLE
    /*
    WebView2Core* core = controller_.coreWebView2;

    NSString* html = [NSString stringWithFormat:@
        "<!DOCTYPE html>"
        "<html>"
        "<head><title>Test Window %d</title></head>"
        "<body style='background: #2b2b2b; color: white; font-family: sans-serif;'>"
        "  <h1>WebView2 Test Window %d</h1>"
        "  <button onclick='window.webkit.messageHandlers.testHandler.postMessage({action: \"clicked\", window: %d})'>Click Me</button>"
        "  <button onclick='window.webkit.messageHandlers.createWindow.postMessage({})'>Create New Window</button>"
        "  <p>This window should not block when creating/interacting with other windows.</p>"
        "</body>"
        "</html>",
        windowNumber_, windowNumber_, windowNumber_];

    [core navigateToString:html];
    */
#else
    std::cout << "Would load HTML in window " << windowNumber_ << std::endl;
#endif
}

@end

/**
 * Main application coordinator
 */
class WebView2TestApp {
public:
    static WebView2TestApp& getInstance() {
        static WebView2TestApp instance;
        return instance;
    }

    bool initialize() {
        std::cout << "Initializing WebView2 test application..." << std::endl;

#ifdef WEBVIEW2_SDK_AVAILABLE
        /*
        // TODO: Create WebView2 environment
        auto options = [WebView2EnvironmentOptions new];
        options.userDataFolder = @"~/Library/Application Support/WebView2Test";

        NSError* error = nil;
        environment_ = [WebView2Environment createWithOptions:options error:&error];

        if (!environment_) {
            std::cerr << "Failed to create WebView2 environment: "
                     << [[error localizedDescription] UTF8String] << std::endl;
            return false;
        }

        std::cout << "WebView2 environment created" << std::endl;
        */
#else
        std::cout << "WebView2 SDK not available - running in placeholder mode" << std::endl;
#endif

        // Initialize Cocoa
        [NSApplication sharedApplication];
        [NSApp setActivationPolicy:NSApplicationActivationPolicyRegular];
        [NSApp activateIgnoringOtherApps:YES];

        return true;
    }

    void createWindow() {
        TestWindow* window = [[TestWindow alloc] initWithNumber:windowCount_++];
        if ([window initialize]) {
            [window show];
            [window loadHTML];
            windows_.push_back(window);
            std::cout << "Created window " << windowCount_ - 1
                     << " (total: " << windows_.size() << ")" << std::endl;
        }
    }

    void run() {
        std::cout << "\n========================================" << std::endl;
        std::cout << "  WebView2 Multi-Window Test" << std::endl;
        std::cout << "========================================" << std::endl;
        std::cout << "\nThis test verifies:" << std::endl;
        std::cout << "1. Multiple WebView2 windows can be created" << std::endl;
        std::cout << "2. Windows remain responsive when creating new windows" << std::endl;
        std::cout << "3. JavaScript can communicate with C++" << std::endl;
        std::cout << "\nPress Cmd+Q to exit\n" << std::endl;

        // Create initial window
        createWindow();

        // Run event loop
        [NSApp run];
    }

private:
    WebView2TestApp() : windowCount_(1) {}
    ~WebView2TestApp() {}

    WebView2Environment environment_;
    std::vector<TestWindow*> windows_;
    int windowCount_;
};

/**
 * Entry point
 */
int main(int argc, char* argv[]) {
    @autoreleasepool {
        auto& app = WebView2TestApp::getInstance();

        if (!app.initialize()) {
            std::cerr << "Failed to initialize application" << std::endl;
            return 1;
        }

        app.run();
    }

    return 0;
}

/**
 * USAGE INSTRUCTIONS:
 *
 * 1. Download WebView2 SDK from Azure DevOps:
 *    https://dev.azure.com/microsoft/Edge/_artifacts/feed/webview2-mac-stable/UPack/webview2_sdk/overview/0.3800.64
 *
 * 2. Extract to: ../external/webview2-macos/
 *
 * 3. Examine SDK structure:
 *    - Check include/ directory for WebView2.h
 *    - Check lib/ directory for framework or .dylib
 *    - Review any documentation or samples
 *
 * 4. Update this file:
 *    - Replace placeholder types with actual SDK types
 *    - Uncomment #import statements
 *    - Fill in TODO sections with SDK API calls
 *    - Define WEBVIEW2_SDK_AVAILABLE
 *
 * 5. Build:
 *    clang++ -o webview2_test webview2_minimal_test.mm \
 *        -I../external/webview2-macos/include \
 *        -L../external/webview2-macos/lib \
 *        -framework Cocoa -framework WebView2 \
 *        -std=c++17 -ObjC++ \
 *        -DWEBVIEW2_SDK_AVAILABLE
 *
 * 6. Run:
 *    ./webview2_test
 *
 * 7. Test:
 *    - Verify window opens
 *    - Click "Create New Window" button multiple times
 *    - Verify NO HANGING when creating multiple windows
 *    - Verify each window remains responsive
 *    - Check devtools work (Cmd+Opt+I)
 *
 * EXPECTED BEHAVIOR:
 * - Window 1 opens immediately
 * - Clicking "Create New Window" creates Window 2, 3, etc.
 * - ALL windows remain responsive
 * - No blocking, no hanging
 * - Devtools accessible in all windows
 *
 * If this test passes, WebView2 SDK integration is working correctly
 * and can be integrated into the main TeamsBICSandbox application.
 */
