#include "webview_factory.h"
#include "webview/webview.h"
#include <iostream>

void* createWebViewForWindow(void* nativeWindow, bool debug) {
    try {
        // Create a webview instance wrapping the existing window
        auto* wv = new webview::webview(debug, nativeWindow);

        if (!wv) {
            std::cerr << "Failed to create webview instance" << std::endl;
            return nullptr;
        }

        return static_cast<void*>(wv);
    } catch (const std::exception& e) {
        std::cerr << "Exception creating webview: " << e.what() << std::endl;
        return nullptr;
    }
}

void destroyWebView(void* webview) {
    if (webview) {
        auto* wv = static_cast<webview::webview*>(webview);
        delete wv;
    }
}

void webViewSetHtml(void* webview, const char* html) {
    if (webview) {
        auto* wv = static_cast<webview::webview*>(webview);
        wv->set_html(std::string(html));
    }
}
