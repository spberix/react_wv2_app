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

// Helper function to recursively find WKWebView
static NSView* findWKWebView(NSView* view) {
    if ([view isKindOfClass:NSClassFromString(@"WKWebView")]) {
        return view;
    }

    for (NSView* subview in view.subviews) {
        NSView* found = findWKWebView(subview);
        if (found) {
            return found;
        }
    }

    return nil;
}

void configureWebViewTransparency(void* webviewWindow) {
    if (!webviewWindow) {
        std::cerr << "No window handle for transparency configuration" << std::endl;
        return;
    }

    @autoreleasepool {
        NSWindow* window = (__bridge NSWindow*)webviewWindow;
        NSView* contentView = window.contentView;

        std::cout << "Configuring WebView transparency..." << std::endl;

        // Make the window itself transparent
        [window setOpaque:NO];
        [window setBackgroundColor:[NSColor clearColor]];

        // Make the content view transparent
        [contentView setWantsLayer:YES];
        contentView.layer.backgroundColor = [[NSColor clearColor] CGColor];

        // Recursively find the WKWebView
        NSView* wkWebView = findWKWebView(contentView);

        if (wkWebView) {
            std::cout << "Found WKWebView!" << std::endl;

            // Make the WKWebView transparent
            [wkWebView setValue:@NO forKey:@"drawsBackground"];

            // Set background color to clear
            if ([wkWebView respondsToSelector:@selector(setValue:forKey:)]) {
                [(id)wkWebView setValue:[NSColor clearColor] forKey:@"backgroundColor"];
            }

            // Also make its layer transparent
            [wkWebView setWantsLayer:YES];
            wkWebView.layer.backgroundColor = [[NSColor clearColor] CGColor];
            wkWebView.layer.opaque = NO;

            std::cout << "WebView transparency configured!" << std::endl;
        } else {
            std::cerr << "Warning: WKWebView not found in view hierarchy" << std::endl;
        }
    }
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
