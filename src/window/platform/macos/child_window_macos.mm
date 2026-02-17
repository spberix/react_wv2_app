#import "../../child_window.h"
#import "../../../video/video_grid_manager.h"
#import "../../../video/video_participant.h"
#import "../../../window/window_manager.h"
#import "../../webview_factory.h"
#import "../../../platform_helper.h"
#import <Cocoa/Cocoa.h>
#import <AVFoundation/AVFoundation.h>
#include <iostream>

// Window delegate to handle close events
@interface ChildWindowDelegate : NSObject <NSWindowDelegate>
@property (nonatomic, assign) ChildWindow* childWindow;
@end

@implementation ChildWindowDelegate

- (void)windowWillClose:(NSNotification*)notification {
    std::cout << "Child window closing" << std::endl;
    // Cleanup will be handled by WindowManager
}

- (BOOL)windowShouldClose:(NSWindow*)sender {
    // Allow window to close - cleanup will happen in WindowManager
    return YES;
}

@end

ChildWindow::ChildWindow()
    : window_(nullptr)
    , webview_(nullptr)
    , initialized_(false) {
}

ChildWindow::~ChildWindow() {
    @autoreleasepool {
        // Cleanup webview if present (currently child windows don't use webview)
        if (webview_) {
            // TODO: When WebView2 is added to child windows, cleanup here
            webview_ = nullptr;
        }

        if (gridManager_) {
            gridManager_->cleanup();
        }

        if (window_) {
            [window_ close];
            window_ = nullptr;
        }
    }
}

bool ChildWindow::initialize(const WindowRect& rect, const std::string& title) {
    @autoreleasepool {
        if (initialized_) {
            std::cerr << "ChildWindow already initialized" << std::endl;
            return false;
        }

        title_ = title;

        // Initialize platform window first
        if (!initializePlatformWindow(rect, title)) {
            std::cerr << "Failed to initialize platform window" << std::endl;
            return false;
        }

        // WebView is not enabled for child windows yet
        // Child windows currently only display video tiles without UI overlay
        std::cout << "Skipping webview initialization for child window (not yet implemented)" << std::endl;
        // if (!initializeWebView()) {
        //     std::cerr << "Failed to initialize webview for child window" << std::endl;
        //     return false;
        // }
        // std::cout << "Webview initialized successfully" << std::endl;

        // Initialize video layer
        std::cout << "Initializing video layer for child window..." << std::endl;
        if (!initializeVideoLayer()) {
            std::cerr << "Failed to initialize video layer for child window" << std::endl;
            return false;
        }
        std::cout << "Video layer initialized successfully" << std::endl;

        initialized_ = true;
        std::cout << "Child window initialized: " << title << std::endl;
        return true;
    }
}

bool ChildWindow::initializePlatformWindow(const WindowRect& rect, const std::string& title) {
    @autoreleasepool {
        // Create NSWindow
        NSRect windowRect = NSMakeRect(rect.x, rect.y, rect.width, rect.height);

        NSWindow* nsWindow = [[NSWindow alloc]
            initWithContentRect:windowRect
                      styleMask:(NSWindowStyleMaskTitled |
                                NSWindowStyleMaskClosable |
                                NSWindowStyleMaskMiniaturizable |
                                NSWindowStyleMaskResizable)
                        backing:NSBackingStoreBuffered
                          defer:YES];

        if (!nsWindow) {
            std::cerr << "Failed to create NSWindow" << std::endl;
            return false;
        }

        // Set window properties
        NSString* titleString = [NSString stringWithUTF8String:title.c_str()];
        [nsWindow setTitle:titleString];
        [nsWindow setBackgroundColor:[NSColor blackColor]];
        [nsWindow setOpaque:YES];

        // CRITICAL: Prevent this window from becoming key or main
        // This avoids interfering with the main window's webview event loop
        [nsWindow setLevel:NSNormalWindowLevel];
        [nsWindow setCollectionBehavior:NSWindowCollectionBehaviorCanJoinAllSpaces];

        // Make content view layer-backed
        [nsWindow.contentView setWantsLayer:YES];
        nsWindow.contentView.layer.backgroundColor = [[NSColor blackColor] CGColor];
        nsWindow.contentView.layer.opaque = YES;

        // Create and set window delegate
        ChildWindowDelegate* delegate = [[ChildWindowDelegate alloc] init];
        delegate.childWindow = this;
        [nsWindow setDelegate:delegate];

        // Show window without making it key (don't steal focus)
        // This is CRITICAL to avoid interfering with main window's event loop
        [nsWindow orderFront:nil];

        // Ensure we don't accept key focus
        [nsWindow setAcceptsMouseMovedEvents:NO];

        // Store window (PlatformWindow is NSWindow*, so no cast needed)
        window_ = nsWindow;

        std::cout << "Platform window created and shown" << std::endl;
        return true;
    }
}

bool ChildWindow::initializeVideoLayer() {
    @autoreleasepool {
        if (!window_) {
            std::cerr << "Window not initialized" << std::endl;
            return false;
        }

        // Create VideoGridManager for this window
        gridManager_ = std::make_unique<VideoGridManager>();

        // window_ is already NSWindow*, no cast needed
        NSRect contentRect = window_.contentView.bounds;

        // Initialize grid manager with this window's handle (as void*)
        if (!gridManager_->initialize((__bridge void*)window_, contentRect.size.width, contentRect.size.height)) {
            std::cerr << "Failed to initialize VideoGridManager for child window" << std::endl;
            return false;
        }

        // DO NOT set webview for child window's VideoGridManager
        // This prevents blocking webview->eval() calls during participant transfers
        // Child windows only need video rendering, not React notifications
        std::cout << "Skipping webview for VideoGridManager (child window doesn't need React notifications)" << std::endl;

        return true;
    }
}

bool ChildWindow::initializeWebView() {
    @autoreleasepool {
        if (!window_) {
            std::cerr << "Window must be initialized before webview" << std::endl;
            return false;
        }

        // TODO: Implement WebView2 for child windows when needed
        // For now, child windows only show video tiles without UI overlay
        // When implementing:
        // 1. Use WebView2Manager to create a new WebView2Instance
        // 2. Set controller.defaultBackgroundColor = [NSColor clearColor] for transparency
        // 3. Load minimal HTML with transparent background
        // 4. Add participant labels or controls as needed

        std::cout << "WebView initialization skipped for child window (not yet implemented with WebView2)" << std::endl;
        webview_ = nullptr;
        return true;
    }
}

bool ChildWindow::acceptParticipant(const VideoParticipant& participant, AVPlayer* player, AVPlayerLayer* layer) {
    @autoreleasepool {
        if (!gridManager_) {
            std::cerr << "Grid manager not initialized" << std::endl;
            return false;
        }

        // This method would be used for direct AVPlayerLayer transfer
        // For now, we'll use the extractParticipant/addExistingParticipant flow
        // This is a placeholder for future implementation

        std::cout << "Accept participant called for child window" << std::endl;
        return true;
    }
}

void ChildWindow::show() {
    @autoreleasepool {
        if (window_) {
            [window_ makeKeyAndOrderFront:nil];
        }
    }
}

void ChildWindow::hide() {
    @autoreleasepool {
        if (window_) {
            [window_ orderOut:nil];
        }
    }
}

void ChildWindow::setWindowDelegate(void* delegate) {
    @autoreleasepool {
        if (window_) {
            id<NSWindowDelegate> nsDelegate = (__bridge id<NSWindowDelegate>)delegate;
            [window_ setDelegate:nsDelegate];
        }
    }
}
