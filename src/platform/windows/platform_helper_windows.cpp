#include "../../platform_helper.h"
#include <iostream>

namespace platform_helper {

void* getNativeWindowHandle(void* webviewWindow) {
    return webviewWindow;
}

void configureWebViewTransparency(void* webviewWindow) {
    std::cout << "WebView transparency not yet implemented (Windows)" << std::endl;
    // TODO: Implement WebView2 transparency
}

std::string selectVideoFile() {
    std::cerr << "File picker not implemented on Windows" << std::endl;
    // TODO: Implement Windows file picker
    return "";
}

} // namespace platform_helper
