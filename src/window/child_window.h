#ifndef CHILD_WINDOW_H
#define CHILD_WINDOW_H

#include <string>
#include <memory>

// Forward declarations
class VideoGridManager;
struct VideoParticipant;
struct WindowRect;

// Platform-specific window handle
#ifdef __APPLE__
#ifdef __OBJC__
@class NSWindow;
@class NSWindowDelegate;
#else
typedef void NSWindow;
typedef void NSWindowDelegate;
#endif
typedef NSWindow* PlatformWindow;
#else
typedef void* PlatformWindow;
#endif

// Platform-specific player and layer types
#ifdef __APPLE__
#ifdef __OBJC__
@class AVPlayer;
@class AVPlayerLayer;
#else
typedef void AVPlayer;
typedef void AVPlayerLayer;
#endif
#else
typedef void AVPlayer;
typedef void AVPlayerLayer;
#endif

class ChildWindow {
public:
    ChildWindow();
    ~ChildWindow();

    // Initialize the child window
    bool initialize(const WindowRect& rect, const std::string& title);

    // Get native window handle
    PlatformWindow getNativeWindow() const { return window_; }

    // Get video grid manager
    VideoGridManager* getGridManager() const { return gridManager_.get(); }

    // Accept a participant with its player and layer
    bool acceptParticipant(const VideoParticipant& participant, AVPlayer* player, AVPlayerLayer* layer);

    // Show/hide window
    void show();
    void hide();

    // Set window delegate (for handling close events)
    // delegate should be id<NSWindowDelegate> on macOS
    void setWindowDelegate(void* delegate);

private:
    PlatformWindow window_;
    std::unique_ptr<VideoGridManager> gridManager_;
    void* webview_;  // Using void* to avoid webview.h include (pointer to webview::webview)
    std::string title_;
    bool initialized_;

    // Platform-specific initialization
    bool initializePlatformWindow(const WindowRect& rect, const std::string& title);
    bool initializeVideoLayer();
    bool initializeWebView();
};

#endif // CHILD_WINDOW_H
