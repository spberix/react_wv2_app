#ifndef WEBVIEW_FACTORY_H
#define WEBVIEW_FACTORY_H

// Factory functions to create/destroy webview instances
// These functions are in a .cpp file (not .mm) to avoid ARC compatibility issues

// Create a webview instance wrapping an existing native window
// Returns void* to avoid template instantiation issues
void* createWebViewForWindow(void* nativeWindow, bool debug);

// Destroy a webview instance
void destroyWebView(void* webview);

// Set HTML content for a webview
void webViewSetHtml(void* webview, const char* html);

#endif // WEBVIEW_FACTORY_H
