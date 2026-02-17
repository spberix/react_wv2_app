#ifndef DRAG_OVERLAY_VIEW_H
#define DRAG_OVERLAY_VIEW_H

// Forward declarations
class WindowManager;
class VideoGridManager;

// Platform-specific view handle
#ifdef __APPLE__
#ifdef __OBJC__
@class NSView;
@class DragOverlayViewImpl;
#else
typedef void NSView;
typedef void DragOverlayViewImpl;
#endif
typedef NSView* PlatformView;
#else
typedef void* PlatformView;
#endif

// C++ wrapper for drag overlay view
class DragOverlayView {
public:
    DragOverlayView();
    ~DragOverlayView();

    // Initialize the drag overlay view
    // webview parameter is void* to avoid including webview header
    bool initialize(void* window, WindowManager* windowManager,
                   VideoGridManager* gridManager, void* webview);

    // Get native view handle
    PlatformView getNativeView() const { return view_; }

    // Enable/disable drag handling
    void setDragEnabled(bool enabled);

private:
    PlatformView view_;
    void* impl_; // Platform-specific implementation (stored as void* for ARC compatibility)
    bool initialized_;
};

#endif // DRAG_OVERLAY_VIEW_H
