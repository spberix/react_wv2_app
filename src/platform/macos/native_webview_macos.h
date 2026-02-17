#ifndef NATIVE_WEBVIEW_MACOS_H
#define NATIVE_WEBVIEW_MACOS_H

#ifdef __APPLE__

#include <string>

// Forward declaration for Objective-C types
#ifdef __OBJC__
@class NSWindow;
@class WKWebView;
@class NSApplicationDelegate;
#else
typedef void NSWindow;
typedef void WKWebView;
typedef void NSApplicationDelegate;
#endif

namespace native_webview {

// Native WKWebView wrapper for macOS
class NativeWebView {
public:
    NativeWebView(int width, int height, bool debug);
    ~NativeWebView();

    // Initialize the webview and window
    bool initialize(const std::string& title);

    // Load HTML content
    void setHtml(const std::string& html);

    // Navigate to URL
    void navigate(const std::string& url);

    // Execute JavaScript
    void eval(const std::string& js);

    // Bind a C++ function to JavaScript
    void bind(const std::string& name, void (*callback)(const std::string&));

    // Get native window handle
    void* getNativeWindow();

    // Get native WKWebView handle
    void* getNativeWebView();

private:
    NSWindow* window_;
    WKWebView* webView_;
    int width_;
    int height_;
    bool debug_;
    bool initialized_;
};

// Run the native NSApplication event loop
void runNativeEventLoop();

// Initialize NSApplication (call once at startup)
void initializeNSApplication();

} // namespace native_webview

#endif // __APPLE__

#endif // NATIVE_WEBVIEW_MACOS_H
