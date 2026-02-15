#ifndef PLATFORM_HELPER_H
#define PLATFORM_HELPER_H

#include <string>

namespace platform_helper {

// Get native window handle from webview window pointer
void* getNativeWindowHandle(void* webviewWindow);

// Configure webview transparency for the platform
void configureWebViewTransparency(void* webviewWindow);

// Show native file picker and return selected path
std::string selectVideoFile();

} // namespace platform_helper

#endif // PLATFORM_HELPER_H
