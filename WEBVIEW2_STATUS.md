# WebView2 Integration Status

## Completed
1. ✅ Downloaded WebView2 SDK from Azure DevOps
2. ✅ Extracted MSWebView2Staging.framework
3. ✅ Created webview2_wrapper.h abstraction layer  
4. ✅ Implemented webview2_wrapper_macos.mm with actual SDK API
5. ✅ Updated CMakeLists.txt to include WebView2 framework
6. ✅ CMake configuration succeeds

## In Progress
- ⏳ Updating app.h and app.cpp to use WebView2 API

## Required Changes

### app.cpp Major Changes
The webview library had a simple API where it created its own window:
```cpp
webview_ = std::make_unique<webview::webview>(debug_, nullptr);
webview_->set_title(title_);
webview_->set_size(width_, height_, WEBVIEW_HINT_NONE);
webview_->run();  // Blocking event loop
```

WebView2 requires:
1. **Create native window first** (NSWindow on macOS)
2. **Initialize WebView2Manager** (shared environment)
3. **Create WebView2Instance** for the window
4. **No blocking run()** - use native event loop

### API Mapping
| Old (webview library) | New (WebView2) |
|-----------------------|----------------|
| `webview::webview(debug, nullptr)` | Create NSWindow, then `WebView2Manager::getInstance().createWebView(window)` |
| `webview_->set_title(title)` | `[nsWindow setTitle:@"..."]` |
| `webview_->set_size(w, h, hint)` | Set NSWindow frame |
| `webview_->window()` | Store `nativeWindow_` handle |
| `webview_->navigate(url)` | `webview_->navigate(url)` (same) |
| `webview_->set_html(html)` | `webview_->setHtml(html)` |
| `webview_->eval(js)` | `webview_->executeScript(js)` (non-blocking) |
| `webview_->bind(name, fn)` | `webview_->addScriptHandler(name, fn)` |
| `webview_->run()` | `[NSApp run]` (native event loop) |
| `webview_->terminate()` | `[NSApp terminate:nil]` |

## Next Steps

Since app.cpp is complex (600+ lines), the best approach is:

**Option 1: Minimal Test First**
1. Create a minimal test app that just shows WebView2 works
2. Verify multi-window support
3. Then migrate full app.cpp

**Option 2: Create Platform Helper**  
1. Create `platform_helper::createNativeWindow()`
2. Update app.cpp initialization sequence
3. Handle native window creation per platform

Recommendation: Start with minimal test to verify SDK works end-to-end.
