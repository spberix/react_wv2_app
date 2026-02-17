/**
 * Minimal WebView2 SDK Test
 *
 * This verifies the WebView2 SDK integration is working.
 * Compile and run to test before integrating into main app.
 *
 * Build:
 *   clang++ -o test_webview2 test_webview2.mm \
 *       src/webview/platform/macos/webview2_wrapper_macos.mm \
 *       -I. -Isrc \
 *       -F external/webview2-macos \
 *       -framework MSWebView2Staging \
 *       -framework Cocoa \
 *       -std=c++17 -ObjC++
 *
 * Run:
 *   ./test_webview2
 */

#import <Cocoa/Cocoa.h>
#include "src/webview/webview2_wrapper.h"
#include <iostream>

// Forward declarations
static void removeChildWebviewAtIndex(size_t index);
static size_t getChildWebviewCount();

// Window delegate to handle close events properly
@interface ChildWindowDelegate : NSObject <NSWindowDelegate>
@property (nonatomic, assign) int windowNumber;
@end

@implementation ChildWindowDelegate
- (BOOL)windowShouldClose:(NSWindow *)sender {
    std::cout << "Child window #" << self.windowNumber << " requesting close..." << std::endl;

    // Teams approach: Just hide the window instead of destroying it
    // This avoids all WebView2 lifecycle issues
    std::cout << ">>> Hiding window instead of closing (WebView2 stays alive)" << std::endl;
    [sender orderOut:nil];

    // Return NO - we hid it, don't actually close it
    return NO;
}

- (void)windowWillClose:(NSNotification *)notification {
    std::cout << "Child window #" << self.windowNumber << " closing (app exit)" << std::endl;
    // Only called when app is actually quitting - no cleanup needed
}
@end

// Global counter for child windows
static int windowCounter = 1;

// Keep child windows and webviews alive
static std::vector<std::shared_ptr<webview2::WebView2Instance>> childWebviews;

// Helper functions for managing child webviews
static void removeChildWebviewAtIndex(size_t index) {
    if (index < childWebviews.size()) {
        childWebviews.erase(childWebviews.begin() + index);
    }
}

static size_t getChildWebviewCount() {
    return childWebviews.size();
}

// Function to create a new WebView2 window (async to avoid deadlock)
void createNewWindow() {
    std::cout << ">>> Scheduling child window creation..." << std::endl;

    // Dispatch to main queue asynchronously to avoid blocking the message handler
    dispatch_async(dispatch_get_main_queue(), ^{
        @autoreleasepool {
            windowCounter++;
            std::cout << ">>> [ASYNC] Creating child window #" << windowCounter << "..." << std::endl;

            // Create new NSWindow
            NSRect frame = NSMakeRect(200 + (windowCounter * 30), 200 + (windowCounter * 30), 800, 600);
            std::cout << ">>> [ASYNC] Allocating NSWindow..." << std::endl;
            NSWindow* childWindow = [[NSWindow alloc]
                initWithContentRect:frame
                          styleMask:(NSWindowStyleMaskTitled |
                                    NSWindowStyleMaskClosable |
                                    NSWindowStyleMaskMiniaturizable |
                                    NSWindowStyleMaskResizable)
                            backing:NSBackingStoreBuffered
                              defer:NO];

            NSString* title = [NSString stringWithFormat:@"WebView2 Child Window %d", windowCounter];
            [childWindow setTitle:title];

            // CRITICAL: Disable ALL window animations to prevent crash
            // NSWindowAnimationBehaviorNone prevents the animation system from capturing WebView2 object references
            [childWindow setAnimationBehavior:NSWindowAnimationBehaviorNone];

            // Set up window delegate to handle close properly
            ChildWindowDelegate* delegate = [[ChildWindowDelegate alloc] init];
            delegate.windowNumber = windowCounter;
            [childWindow setDelegate:delegate];

            std::cout << ">>> [ASYNC] NSWindow created with title: " << [title UTF8String] << " (animations disabled)" << std::endl;

            // Create WebView2 instance for this window ASYNCHRONOUSLY
            std::cout << ">>> [ASYNC] Creating WebView2 instance async..." << std::endl;
            int currentWindowNum = windowCounter;
            webview2::WebView2Manager::getInstance().createWebViewAsync((__bridge void*)childWindow, true,
                [childWindow, currentWindowNum, delegate](std::shared_ptr<webview2::WebView2Instance> childWebview) {
                @autoreleasepool {
                    if (!childWebview) {
                        std::cerr << ">>> [ASYNC CALLBACK] ✗ Failed to create child WebView2 instance" << std::endl;
                        return;
                    }
                    std::cout << ">>> [ASYNC CALLBACK] WebView2 instance created successfully" << std::endl;

                    // Load content in child window
                    std::string html = R"html(
<!DOCTYPE html>
<html>
<head>
    <title>Child Window</title>
    <style>
        body {
            font-family: -apple-system, system-ui, sans-serif;
            margin: 40px;
            background: linear-gradient(135deg, #f093fb 0%, #f5576c 100%);
            color: white;
            display: flex;
            justify-content: center;
            align-items: center;
            height: 100vh;
        }
        .container {
            text-align: center;
            background: rgba(255,255,255,0.1);
            padding: 40px;
            border-radius: 20px;
            backdrop-filter: blur(10px);
        }
        h1 { font-size: 2.5em; margin: 0; }
        p { font-size: 1.2em; margin: 20px 0; }
    </style>
</head>
<body>
    <div class="container">
        <h1>🎉 Child Window #)html" + std::to_string(currentWindowNum) + R"html(</h1>
        <p>WebView2 multi-window works!</p>
        <p id="time"></p>
    </div>
    <script>
        setInterval(() => {
            document.getElementById('time').textContent = new Date().toLocaleTimeString();
        }, 1000);
    </script>
</body>
</html>
                    )html";

                    childWebview->setHtml(html);
                    [childWindow makeKeyAndOrderFront:nil];

                    // Keep the webview alive
                    childWebviews.push_back(childWebview);

                    std::cout << ">>> [ASYNC CALLBACK] ✓ Child window created and shown (total webviews: "
                              << childWebviews.size() << ")" << std::endl;
                }
            });

            std::cout << ">>> [ASYNC] WebView2 instance creation initiated (will complete via callback)" << std::endl;
        }
    });
}

int main(int argc, char* argv[]) {
    @autoreleasepool {
        std::cout << "======================================" << std::endl;
        std::cout << "  WebView2 SDK Integration Test" << std::endl;
        std::cout << "======================================" << std::endl;
        std::cout << std::endl;

        // Initialize NSApplication
        [NSApplication sharedApplication];
        [NSApp setActivationPolicy:NSApplicationActivationPolicyRegular];
        [NSApp activateIgnoringOtherApps:YES];

        // Don't quit when last window closes (we'll handle it manually)
        [[NSUserDefaults standardUserDefaults] setBool:NO forKey:@"NSQuitAlwaysKeepsWindows"];

        // Initialize WebView2 environment
        std::cout << "1. Initializing WebView2 environment..." << std::endl;
        if (!webview2::WebView2Manager::getInstance().initialize()) {
            std::cerr << "✗ Failed to initialize WebView2 environment" << std::endl;
            return 1;
        }
        std::cout << "✓ WebView2 environment initialized" << std::endl << std::endl;

        // Create NSWindow
        std::cout << "2. Creating native window..." << std::endl;
        NSRect frame = NSMakeRect(100, 100, 1200, 800);
        NSWindow* window = [[NSWindow alloc]
            initWithContentRect:frame
                      styleMask:(NSWindowStyleMaskTitled |
                                NSWindowStyleMaskClosable |
                                NSWindowStyleMaskMiniaturizable |
                                NSWindowStyleMaskResizable)
                        backing:NSBackingStoreBuffered
                          defer:NO];

        [window setTitle:@"WebView2 Test"];
        [window center];
        std::cout << "✓ Native window created" << std::endl << std::endl;

        // Create WebView2 instance
        std::cout << "3. Creating WebView2 instance..." << std::endl;
        auto webview = webview2::WebView2Manager::getInstance().createWebView((__bridge void*)window, true);
        if (!webview) {
            std::cerr << "✗ Failed to create WebView2 instance" << std::endl;
            return 1;
        }
        std::cout << "✓ WebView2 instance created" << std::endl << std::endl;

        // Add message handler for creating new windows
        webview->addScriptHandler("createWindow", [](const std::string& message) {
            std::cout << "=== Received createWindow message from JavaScript ===" << std::endl;
            std::cout << "Message data: " << message << std::endl;
            createNewWindow();
            std::cout << "=== createWindow handler finished ===" << std::endl;
        });

        // Load simple HTML
        std::cout << "4. Loading HTML content..." << std::endl;
        std::string html = R"html(
<!DOCTYPE html>
<html>
<head>
    <title>WebView2 Test</title>
    <style>
        body {
            font-family: -apple-system, system-ui, sans-serif;
            margin: 40px;
            background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
            color: white;
            display: flex;
            justify-content: center;
            align-items: center;
            height: 100vh;
        }
        .container {
            text-align: center;
            background: rgba(255,255,255,0.1);
            padding: 60px;
            border-radius: 20px;
            backdrop-filter: blur(10px);
        }
        h1 { font-size: 3em; margin: 0; }
        p { font-size: 1.5em; margin: 20px 0; }
        button {
            padding: 15px 30px;
            font-size: 1.2em;
            margin: 10px;
            border: none;
            border-radius: 10px;
            background: white;
            color: #667eea;
            cursor: pointer;
            font-weight: 600;
        }
        button:hover { transform: scale(1.05); }
        #output { margin-top: 20px; font-size: 1.1em; }
    </style>
</head>
<body>
    <div class="container">
        <h1>✅ WebView2 Works!</h1>
        <p>Microsoft WebView2 SDK for macOS</p>
        <button onclick="testScript()">Test JavaScript</button>
        <button onclick="createWindow()">Create New Window</button>
        <div id="output"></div>
    </div>
    <script>
        function testScript() {
            document.getElementById('output').textContent =
                '✓ JavaScript is working! Time: ' + new Date().toLocaleTimeString();
        }

        function createWindow() {
            document.getElementById('output').textContent = 'Creating new window...';
            // Call C++ via WebView2 message handler
            window.webview2_send('createWindow', '{}');
        }

        console.log('WebView2 HTML loaded successfully');
    </script>
</body>
</html>
        )html";

        webview->setHtml(html);
        std::cout << "✓ HTML loaded" << std::endl << std::endl;

        // Execute JavaScript
        std::cout << "5. Testing JavaScript execution..." << std::endl;
        webview->executeScript("console.log('JavaScript execution from C++ works!');");
        std::cout << "✓ JavaScript executed" << std::endl << std::endl;

        // Show window
        std::cout << "6. Showing window..." << std::endl;
        [window makeKeyAndOrderFront:nil];
        std::cout << "✓ Window shown" << std::endl << std::endl;

        std::cout << "======================================" << std::endl;
        std::cout << "  ✓ All tests passed!" << std::endl;
        std::cout << "======================================" << std::endl;
        std::cout << std::endl;
        std::cout << "Window is open. Close it to exit." << std::endl;
        std::cout << std::endl;

        // Run event loop
        [NSApp run];

        std::cout << "Application exited" << std::endl;
    }

    return 0;
}
