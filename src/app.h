#ifndef APP_H
#define APP_H

#include <string>
#include <memory>
#include "webview/webview2_wrapper.h"

// Forward declarations
class VideoGridManager;
class WindowManager;
class DragOverlayView;

class Application {
public:
    Application(const std::string& title, int width, int height, bool debug = false, bool useLocalhost = false);
    ~Application();

    // Initialize the application
    bool initialize();

    // Run the application event loop
    void run();

    // Terminate the application
    void terminate();

private:
    std::shared_ptr<webview2::WebView2Instance> webview_;
    void* nativeWindow_;  // Platform-specific window handle
    std::unique_ptr<VideoGridManager> videoGridManager_;
    std::unique_ptr<WindowManager> windowManager_;
    std::unique_ptr<DragOverlayView> dragOverlayView_;
    std::string title_;
    int width_;
    int height_;
    bool debug_;
    bool useLocalhost_;
    bool initialized_;

    // Load the UI
    void loadUI();

    // Set up C++ to JavaScript bindings
    void setupBindings();

    // Initialize video layer
    bool initializeVideoLayer();

    // Setup video bindings
    void setupVideoBindings();

    // Configure WebView transparency
    void configureWebViewTransparency();
};

#endif // APP_H
