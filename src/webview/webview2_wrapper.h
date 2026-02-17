#ifndef WEBVIEW2_WRAPPER_H
#define WEBVIEW2_WRAPPER_H

#include <string>
#include <memory>
#include <functional>
#include <map>

// Forward declarations to avoid including WebView2 headers directly
// These will be replaced with actual WebView2 types once SDK is integrated
#ifdef __APPLE__
typedef void* WebView2EnvironmentPtr;
typedef void* WebView2ControllerPtr;
typedef void* WebView2CorePtr;
#endif

namespace webview2 {

// Callback type for JavaScript message handlers
using MessageCallback = std::function<void(const std::string&)>;

// Callback type for async initialization
using InitCallback = std::function<void(bool success)>;

/**
 * WebView2Instance - Represents a single WebView2 control for one window
 *
 * Unlike the webview library which has a blocking run() method,
 * WebView2 integrates with the native event loop non-blocking.
 */
class WebView2Instance {
public:
    WebView2Instance(void* nativeWindow, bool debug);
    ~WebView2Instance();

    // Initialize the WebView2 controller for this window (blocking - use for first window only)
    bool initialize();

    // Initialize asynchronously with callback (use for additional windows to avoid deadlock)
    void initializeAsync(InitCallback callback);

    // Load HTML content
    void setHtml(const std::string& html);

    // Navigate to URL
    void navigate(const std::string& url);

    // Execute JavaScript
    void executeScript(const std::string& js);

    // Add script to execute on every document load
    void addInitScript(const std::string& js);

    // Add JavaScript message handler
    void addScriptHandler(const std::string& name, MessageCallback callback);

    // Enable/disable devtools
    void setDevToolsEnabled(bool enabled);

    // Configure transparency for the WebView
    void configureTransparency();

    // Close the WebView2 controller explicitly (call before window closes)
    void close();

    // Get native WebView2 controller
    void* getNativeController() const { return controller_; }

    // Handle incoming message from JavaScript (called by Objective-C delegate)
    void handleMessage(const std::string& handlerName, const std::string& data);

private:
    void* nativeWindow_;          // NSWindow* on macOS
    bool debug_;
    bool initialized_;

    // WebView2 objects (opaque pointers until SDK is integrated)
    WebView2ControllerPtr controller_;
    WebView2CorePtr webviewCore_;
    void* messageDelegate_;  // WebView2MessageDelegate* on macOS

    // Message handlers
    std::map<std::string, MessageCallback> messageHandlers_;
};

/**
 * WebView2Manager - Singleton manager for WebView2 environment
 *
 * Manages the shared WebView2 environment and creates WebView2 instances
 * for each window. Unlike webview library, this supports multiple instances.
 */
class WebView2Manager {
public:
    // Get singleton instance
    static WebView2Manager& getInstance();

    // Initialize the WebView2 environment (call once at app startup)
    bool initialize();

    // Create a new WebView2 instance for a window (blocking - use for first window only)
    std::shared_ptr<WebView2Instance> createWebView(void* nativeWindow, bool debug = false);

    // Create a new WebView2 instance asynchronously (use for additional windows)
    void createWebViewAsync(void* nativeWindow, bool debug, std::function<void(std::shared_ptr<WebView2Instance>)> callback);

    // Shutdown (call at app exit)
    void shutdown();

    // Get the shared WebView2 environment
    void* getEnvironment() const { return environment_; }

private:
    WebView2Manager();
    ~WebView2Manager();

    // Prevent copying
    WebView2Manager(const WebView2Manager&) = delete;
    WebView2Manager& operator=(const WebView2Manager&) = delete;

    bool initialized_;
    WebView2EnvironmentPtr environment_;
    std::vector<std::shared_ptr<WebView2Instance>> instances_;
};

} // namespace webview2

#endif // WEBVIEW2_WRAPPER_H
