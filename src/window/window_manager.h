#ifndef WINDOW_MANAGER_H
#define WINDOW_MANAGER_H

#include <vector>
#include <map>
#include <memory>
#include <mutex>
#include <string>

// Forward declarations
class ChildWindow;
class VideoGridManager;
struct VideoParticipant;

// Platform-specific window handle
#ifdef __APPLE__
#ifdef __OBJC__
@class NSWindow;
#else
typedef void NSWindow;
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

// Structure to track participant location
struct ParticipantLocation {
    int participantId;
    void* ownerWindow; // Pointer to ChildWindow or null for main window
    bool isInMainWindow;
};

// Structure for window position
struct WindowRect {
    float x;
    float y;
    float width;
    float height;
};

class WindowManager {
public:
    WindowManager();
    ~WindowManager();

    // Initialize with main window
    // Using void* to avoid type mismatch between C++ and Objective-C++ compilation
    bool initialize(void* mainWindow, VideoGridManager* mainGridManager);

    // Window lifecycle
    ChildWindow* createChildWindow(const WindowRect& rect, const std::string& title);
    void destroyChildWindow(ChildWindow* window);
    void destroyAllChildWindows();

    // Tile transfer operations
    bool transferParticipant(int participantId, void* sourceWindow, void* targetWindow);
    bool createChildWindowWithParticipant(int participantId, const WindowRect& rect);

    // Window queries
    void* getWindowAtScreenPosition(float x, float y);
    bool isPositionOutsideAllWindows(float x, float y);
    void* getMainWindow() const { return mainWindow_; }
    VideoGridManager* getMainGridManager() const { return mainGridManager_; }

    // Participant registry
    void registerParticipant(int participantId, void* ownerWindow, bool isMainWindow);
    void unregisterParticipant(int participantId);
    ParticipantLocation* getParticipantLocation(int participantId);

    // Get all child windows
    const std::vector<std::unique_ptr<ChildWindow>>& getChildWindows() const {
        return childWindows_;
    }

private:
    void* mainWindow_; // NSWindow* on macOS, using void* for cross-compilation compatibility
    VideoGridManager* mainGridManager_;
    std::vector<std::unique_ptr<ChildWindow>> childWindows_;
    std::map<int, ParticipantLocation> participantRegistry_;
    mutable std::mutex mutex_;

    // Helper methods
    VideoGridManager* getGridManagerForWindow(void* window);
    bool isMainWindow(void* window) const;
};

#endif // WINDOW_MANAGER_H
