#include "../../platform_helper.h"
#include <iostream>

namespace platform_helper {

void* getNativeWindowHandle(void* webviewWindow) {
    return webviewWindow;
}

void configureWebViewTransparency(void* webviewWindow) {
    std::cout << "WebView transparency not yet implemented (Linux)" << std::endl;
    // TODO: Implement WebKitGTK transparency
}

std::string selectVideoFile() {
    std::cerr << "File picker not implemented on Linux" << std::endl;
    // TODO: Implement Linux file picker
    return "";
}

} // namespace platform_helper
