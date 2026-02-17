#import "../../drag_overlay_view.h"
#import "../../../window/window_manager.h"
#import "../../../video/video_grid_manager.h"
#import "../../../video/video_participant.h"
#import <Cocoa/Cocoa.h>
#include <iostream>

// Helper function to call eval on webview without needing to include webview.h
// This avoids ARC compatibility issues with the webview library
extern "C" void callWebViewEval(void* webview, const char* js);

// Objective-C implementation of the drag overlay view
@interface DragOverlayViewImpl : NSView {
    WindowManager* windowManager_;
    VideoGridManager* gridManager_;
    void* webview_;
    int draggedParticipantId_;
    NSPoint dragStartLocation_;
    BOOL isDragging_;
    BOOL dragEnabled_;
}

@property (nonatomic, assign) WindowManager* windowManager;
@property (nonatomic, assign) VideoGridManager* gridManager;
@property (nonatomic, assign) void* webview; // Using void* to avoid template instantiation issues

@end

@implementation DragOverlayViewImpl

- (instancetype)initWithFrame:(NSRect)frame {
    self = [super initWithFrame:frame];
    if (self) {
        windowManager_ = nullptr;
        gridManager_ = nullptr;
        webview_ = nullptr;
        draggedParticipantId_ = -1;
        isDragging_ = NO;
        dragEnabled_ = YES;

        std::cout << "DragOverlayViewImpl initialized" << std::endl;
    }
    return self;
}

- (void)setWindowManager:(WindowManager*)manager {
    windowManager_ = manager;
}

- (void)setGridManager:(VideoGridManager*)manager {
    gridManager_ = manager;
}

- (void)setWebview:(void*)webview {
    webview_ = webview;
}

- (void)setDragEnabled:(BOOL)enabled {
    dragEnabled_ = enabled;
}

// Make view transparent
- (BOOL)wantsUpdateLayer {
    return YES;
}

- (void)updateLayer {
    self.layer.backgroundColor = [[NSColor clearColor] CGColor];
}

- (BOOL)acceptsFirstResponder {
    return YES;
}

// Override hit testing to only capture events on video tiles
- (NSView*)hitTest:(NSPoint)point {
    if (!dragEnabled_ || !gridManager_) {
        return nil; // Pass through if drag is disabled
    }

    // Convert point to this view's coordinates
    NSPoint localPoint = [self convertPoint:point fromView:self.superview];

    // Check if point is on a video tile
    int participantId = [self hitTestVideoTile:localPoint];

    if (participantId >= 0) {
        // Point is on a video tile, capture the event
        return self;
    }

    // Point is not on a video tile, pass through to views below
    return nil;
}

- (void)mouseDown:(NSEvent*)event {
    if (!dragEnabled_ || !gridManager_ || !windowManager_) {
        return;
    }

    NSPoint location = [self convertPoint:event.locationInWindow fromView:nil];
    std::cout << "Mouse down at: " << location.x << ", " << location.y << std::endl;

    // Hit test: find which video tile was clicked
    int participantId = [self hitTestVideoTile:location];
    if (participantId >= 0) {
        draggedParticipantId_ = participantId;
        dragStartLocation_ = location;
        isDragging_ = NO; // Not dragging yet, just pressed

        std::cout << "Hit participant: " << participantId << std::endl;
    }
}

- (void)mouseDragged:(NSEvent*)event {
    if (!dragEnabled_ || draggedParticipantId_ < 0 || !windowManager_) {
        return;
    }

    isDragging_ = YES;

    // Get screen coordinates
    NSPoint screenLocation = [NSEvent mouseLocation];
    std::cout << "Mouse dragged to screen: " << screenLocation.x << ", " << screenLocation.y << std::endl;

    // Check if outside all windows
    if (windowManager_->isPositionOutsideAllWindows(screenLocation.x, screenLocation.y)) {
        // Notify React for cursor change (show "create window" cursor)
        [self notifyReactDragState:@"outside"];
    } else {
        void* targetWindow = windowManager_->getWindowAtScreenPosition(screenLocation.x, screenLocation.y);

        // Get current window
        void* currentWindow = nullptr; // nullptr represents main window

        if (targetWindow == currentWindow) {
            [self notifyReactDragState:@"same_window"];
        } else {
            [self notifyReactDragState:@"other_window"];
        }
    }
}

- (void)mouseUp:(NSEvent*)event {
    if (!dragEnabled_ || draggedParticipantId_ < 0 || !windowManager_) {
        draggedParticipantId_ = -1;
        isDragging_ = NO;
        [self notifyReactDragState:@"none"];
        return;
    }

    // Only handle drop if we were actually dragging (not just a click)
    if (!isDragging_) {
        std::cout << "Mouse up without drag - ignoring" << std::endl;
        draggedParticipantId_ = -1;
        [self notifyReactDragState:@"none"];
        return;
    }

    NSPoint screenLocation = [NSEvent mouseLocation];
    std::cout << "Mouse up at screen: " << screenLocation.x << ", " << screenLocation.y << std::endl;

    // Capture values for deferred execution
    int participantId = draggedParticipantId_;
    WindowManager* wm = windowManager_;
    float screenX = screenLocation.x;
    float screenY = screenLocation.y;

    // Reset drag state immediately
    draggedParticipantId_ = -1;
    isDragging_ = NO;
    [self notifyReactDragState:@"none"];

    // Execute everything IMMEDIATELY and DIRECTLY - no timers, no delays, no async
    if (wm->isPositionOutsideAllWindows(screenX, screenY)) {
        std::cout << "Creating child window IMMEDIATELY" << std::endl;

        WindowRect rect;
        rect.x = screenX - 320;
        rect.y = screenY - 180;
        rect.width = 640;
        rect.height = 360;

        wm->createChildWindowWithParticipant(participantId, rect);
        std::cout << "Child window creation complete" << std::endl;
    } else {
        void* targetWindow = wm->getWindowAtScreenPosition(screenX, screenY);
        void* currentWindow = nullptr;

        if (targetWindow != currentWindow && targetWindow != nullptr) {
            std::cout << "Executing transfer IMMEDIATELY (no timer)" << std::endl;

            // Execute transfer directly - no timer, no delay, no indirection
            wm->transferParticipant(participantId, currentWindow, targetWindow);

            std::cout << "Immediate transfer done" << std::endl;
        } else {
            std::cout << "Dropped on same window - no action" << std::endl;
        }
    }
}

// Helper method - execute transfer immediately and synchronously
- (void)executeTransferWithParams:(NSTimer*)timer {
    NSDictionary* params = timer.userInfo;
    int participantId = [params[@"participantId"] intValue];
    void* targetWindow = [params[@"targetWindow"] pointerValue];
    void* currentWindow = [params[@"currentWindow"] pointerValue];
    WindowManager* wm = (WindowManager*)[params[@"windowManager"] pointerValue];

    std::cout << "Executing transfer SYNCHRONOUSLY (no delays, no threads)..." << std::endl;

    // Do the transfer directly on main thread - no async, no threads, no delays
    // Just get it done immediately
    wm->transferParticipant(participantId, currentWindow, targetWindow);

    std::cout << "Synchronous transfer complete" << std::endl;
}

- (int)hitTestVideoTile:(NSPoint)location {
    if (!gridManager_) {
        return -1;
    }

    // Query VideoGridManager for tile bounds
    std::vector<VideoParticipant> participants = gridManager_->getParticipants();

    // Get view height for coordinate conversion
    CGFloat viewHeight = self.bounds.size.height;

    for (const auto& p : participants) {
        VideoPosition pos = p.getPosition();

        // Convert from CALayer coordinates (top-left origin) to NSView coordinates (bottom-left origin)
        // CALayer: y increases downward from top
        // NSView: y increases upward from bottom
        CGFloat convertedY = viewHeight - pos.y - pos.height;

        CGRect tileBounds = CGRectMake(pos.x, convertedY, pos.width, pos.height);

        if (CGRectContainsPoint(tileBounds, CGPointMake(location.x, location.y))) {
            std::cout << "Hit test found participant " << p.getId()
                     << " at CALayer pos (" << pos.x << ", " << pos.y << ")"
                     << " converted to NSView (" << pos.x << ", " << convertedY << ")" << std::endl;
            return p.getId();
        }
    }

    return -1; // No tile hit
}

- (void)notifyReactDragState:(NSString*)state {
    // DISABLED: Completely skip all React notifications to prevent any potential blocking
    // Just log for debugging
    std::cout << "Drag state: " << [state UTF8String] << " (notification disabled)" << std::endl;
}

- (void)keyDown:(NSEvent*)event {
    // Handle Escape key to cancel drag
    if ([event keyCode] == 53) { // Escape key
        if (isDragging_ && draggedParticipantId_ >= 0) {
            std::cout << "Drag cancelled by Escape key" << std::endl;
            draggedParticipantId_ = -1;
            isDragging_ = NO;
            [self notifyReactDragState:@"none"];
        }
    }

    [super keyDown:event];
}

@end

// C++ wrapper implementation
DragOverlayView::DragOverlayView()
    : view_(nullptr)
    , impl_(nullptr)
    , initialized_(false) {
}

DragOverlayView::~DragOverlayView() {
    @autoreleasepool {
        if (impl_) {
            DragOverlayViewImpl* impl = (__bridge_transfer DragOverlayViewImpl*)impl_;
            [impl removeFromSuperview];
            impl_ = nullptr;
        }
        view_ = nullptr;
    }
}

bool DragOverlayView::initialize(void* window, WindowManager* windowManager,
                                VideoGridManager* gridManager, void* webview) {
    @autoreleasepool {
        if (initialized_) {
            std::cerr << "DragOverlayView already initialized" << std::endl;
            return false;
        }

        if (!window || !windowManager || !gridManager) {
            std::cerr << "Invalid parameters for DragOverlayView initialization" << std::endl;
            return false;
        }

        NSWindow* nsWindow = (__bridge NSWindow*)window;
        NSRect frame = nsWindow.contentView.bounds;

        // Create the Objective-C view
        DragOverlayViewImpl* impl = [[DragOverlayViewImpl alloc] initWithFrame:frame];
        impl.windowManager = windowManager;
        impl.gridManager = gridManager;
        impl.webview = webview;

        // Make view transparent and allow clicks to pass through for non-video areas
        impl.wantsLayer = YES;
        impl.layer.backgroundColor = [[NSColor clearColor] CGColor];
        impl.layer.opaque = NO;

        // Add to window's content view
        // Position it at the top to capture mouse events (it's transparent so clicks pass through)
        [nsWindow.contentView addSubview:impl positioned:NSWindowAbove relativeTo:nil];

        // Make sure it fills the window
        impl.autoresizingMask = NSViewWidthSizable | NSViewHeightSizable;

        // Store references
        // Cast DragOverlayViewImpl to NSView (PlatformView)
        view_ = (PlatformView)impl;
        // Retain the implementation
        impl_ = (__bridge_retained void*)impl;
        initialized_ = true;

        std::cout << "DragOverlayView initialized and added to window" << std::endl;
        return true;
    }
}

void DragOverlayView::setDragEnabled(bool enabled) {
    if (impl_) {
        DragOverlayViewImpl* impl = (__bridge DragOverlayViewImpl*)impl_;
        [impl setDragEnabled:enabled];
    }
}
