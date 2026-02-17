#import "../../platform_helper.h"
#import <Cocoa/Cocoa.h>
#import <UniformTypeIdentifiers/UniformTypeIdentifiers.h>
#include <iostream>

namespace platform_helper {

void* getNativeWindowHandle(void* webviewWindow) {
    if (!webviewWindow) {
        return nullptr;
    }
    // The webviewWindow is already an NSWindow pointer
    return webviewWindow;
}

// This function is no longer needed - WebView2 transparency is configured
// directly via controller.defaultBackgroundColor in WebView2Instance
void configureWebViewTransparency(void* webviewWindow) {
    // No-op: WebView2 transparency is now handled in webview2_wrapper_macos.mm
    // via controller.defaultBackgroundColor = [NSColor clearColor]
    std::cout << "configureWebViewTransparency called (deprecated - WebView2 handles this internally)" << std::endl;
}

std::string selectVideoFile() {
    @autoreleasepool {
        // Create open panel for file selection
        NSOpenPanel* openPanel = [NSOpenPanel openPanel];
        [openPanel setCanChooseFiles:YES];
        [openPanel setCanChooseDirectories:NO];
        [openPanel setAllowsMultipleSelection:NO];
        [openPanel setTitle:@"Select Video File"];

        // Set allowed content types (modern API)
        if (@available(macOS 11.0, *)) {
            NSArray* contentTypes = @[
                [UTType typeWithFilenameExtension:@"mp4"],
                [UTType typeWithFilenameExtension:@"mov"],
                [UTType typeWithFilenameExtension:@"m4v"],
                [UTType typeWithFilenameExtension:@"avi"],
                [UTType typeWithFilenameExtension:@"mkv"]
            ];
            [openPanel setAllowedContentTypes:contentTypes];
        } else {
            // Fallback for older macOS versions
            #pragma clang diagnostic push
            #pragma clang diagnostic ignored "-Wdeprecated-declarations"
            [openPanel setAllowedFileTypes:@[@"mp4", @"mov", @"m4v", @"avi", @"mkv"]];
            #pragma clang diagnostic pop
        }

        // Show the panel
        NSModalResponse response = [openPanel runModal];

        if (response == NSModalResponseOK) {
            NSURL* url = [[openPanel URLs] firstObject];
            NSString* path = [url path];
            return std::string([path UTF8String]);
        }
    }

    return "";  // User cancelled
}

} // namespace platform_helper
