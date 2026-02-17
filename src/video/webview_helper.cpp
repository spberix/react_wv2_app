#include <string>
#include <iostream>
#include "../webview/webview2_wrapper.h"

// Helper function to call eval on webview without including webview.h in Objective-C++ files
// Updated for WebView2 - uses executeScript() instead of eval()
extern "C" void callWebViewEval(void* webview, const char* js) {
    if (!webview || !js) {
        return;
    }

    // Cast to WebView2Instance and execute script
    webview2::WebView2Instance* webviewInstance = static_cast<webview2::WebView2Instance*>(webview);
    webviewInstance->executeScript(std::string(js));
}
