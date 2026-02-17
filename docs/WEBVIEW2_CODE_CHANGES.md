# WebView2 Code Integration Changes

This document shows the specific code changes needed to migrate from webview library to WebView2.

## Overview of Changes

| File | Change Type | Description |
|------|-------------|-------------|
| `app.h` | Modify | Replace webview include, change member type |
| `app.cpp` | Modify | Replace initialization, remove run() call |
| `child_window.h` | Modify | Change webview member type |
| `child_window_macos.mm` | Modify | Replace webview creation |
| `CMakeLists.txt` | Modify | Add WebView2 SDK, remove webview library |

## File-by-File Changes

### 1. `/src/app.h`

**BEFORE:**
```cpp
#include "webview.h"

class Application {
private:
    webview::webview* webview_;
    void* nativeWindow_;
};
```

**AFTER:**
```cpp
#include "webview/webview2_wrapper.h"

class Application {
private:
    std::shared_ptr<webview2::WebView2Instance> webview_;
    void* nativeWindow_;
};
```

**Changes:**
- Replace `#include "webview.h"` with `#include "webview/webview2_wrapper.h"`
- Change `webview::webview*` to `std::shared_ptr<webview2::WebView2Instance>`

---

### 2. `/src/app.cpp`

**BEFORE:**
```cpp
bool Application::initialize(int width, int height, const std::string& title) {
    // ... platform window creation ...

    // Create webview
    webview_ = new webview::webview(true, nativeWindow_);
    if (!webview_) {
        return false;
    }

    // ... register bindings ...

    return true;
}

void Application::run() {
    // ... load UI ...

    // BLOCKING CALL - takes over main thread
    webview_->run();
}
```

**AFTER:**
```cpp
bool Application::initialize(int width, int height, const std::string& title) {
    // Initialize WebView2 environment (once at startup)
    if (!webview2::WebView2Manager::getInstance().initialize()) {
        std::cerr << "Failed to initialize WebView2 environment" << std::endl;
        return false;
    }

    // ... platform window creation ...

    // Create WebView2 instance for main window
    webview_ = webview2::WebView2Manager::getInstance().createWebView(nativeWindow_, true);
    if (!webview_) {
        std::cerr << "Failed to create WebView2 instance" << std::endl;
        return false;
    }

    // ... register bindings ...

    return true;
}

void Application::run() {
    // ... load UI ...

    // NO BLOCKING CALL - WebView2 integrates with native event loop
    // Just start the native event loop
    std::cout << "Starting application..." << std::endl;
    std::cout << "Close the window or press Ctrl+C to exit" << std::endl;

#ifdef __APPLE__
    @autoreleasepool {
        [NSApp run];
    }
#elif defined(_WIN32)
    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
#endif
}
```

**Key Changes:**
1. Initialize WebView2Manager at startup
2. Use WebView2Manager to create webview instances
3. Remove blocking `webview_->run()` call
4. Explicitly call native event loop (`[NSApp run]` on macOS)

---

### 3. `/src/window/child_window.h`

**BEFORE:**
```cpp
#include "webview.h"

class ChildWindow {
private:
    webview::webview* webview_;
};
```

**AFTER:**
```cpp
#include "../webview/webview2_wrapper.h"

class ChildWindow {
private:
    std::shared_ptr<webview2::WebView2Instance> webview_;
};
```

**Changes:**
- Replace webview include with webview2_wrapper
- Change member type to WebView2Instance shared_ptr

---

### 4. `/src/window/platform/macos/child_window_macos.mm`

**BEFORE:**
```cpp
bool ChildWindow::initialize(const WindowRect& rect, const std::string& title) {
    // ... create NSWindow ...

    // Create webview (causes hanging)
    try {
        webview_ = new webview::webview(debug_, platformWindow_);
        if (!webview_) {
            return false;
        }

        // Configure transparency
        // ... (complex WKWebView access code) ...

    } catch (const std::exception& e) {
        std::cerr << "Exception creating webview: " << e.what() << std::endl;
        return false;
    }

    // ... initialize video layer ...

    return true;
}
```

**AFTER:**
```cpp
bool ChildWindow::initialize(const WindowRect& rect, const std::string& title) {
    // ... create NSWindow ...

    // Create WebView2 instance (non-blocking)
    webview_ = webview2::WebView2Manager::getInstance().createWebView(platformWindow_, debug_);
    if (!webview_) {
        std::cerr << "Failed to create WebView2 for child window" << std::endl;
        return false;
    }

    // Enable devtools if debug mode
    if (debug_) {
        webview_->setDevToolsEnabled(true);
    }

    // ... initialize video layer ...

    return true;
}
```

**Key Changes:**
1. Use WebView2Manager to create instance
2. Much simpler initialization
3. No try-catch needed (WebView2 handles errors internally)
4. Built-in devtools support

---

### 5. Video Grid Manager - React Notifications

**BEFORE:**
```cpp
void VideoGridManager::notifyReact() {
    if (!webview_ || suppressReactNotifications_) {
        return;
    }

    // Cast void* back to webview
    auto* wv = static_cast<webview::webview*>(webview_);

    // BLOCKING CALL - freezes UI during multi-window operations
    std::string js = "window.dispatchEvent(...)";
    wv->eval(js);
}
```

**AFTER:**
```cpp
void VideoGridManager::notifyReact() {
    if (!webview_ || suppressReactNotifications_) {
        return;
    }

    // Cast void* back to WebView2Instance
    auto* wv = static_cast<webview2::WebView2Instance*>(webview_);

    // NON-BLOCKING - WebView2 executes asynchronously
    std::string js = "window.dispatchEvent(...)";
    wv->executeScript(js);
}
```

**Key Changes:**
- Cast to WebView2Instance instead of webview::webview
- Use `executeScript()` instead of `eval()`
- **Non-blocking** - doesn't freeze UI

---

### 6. JavaScript Bindings

**BEFORE:**
```cpp
// Register C++ function callable from JavaScript
webview_->bind("videoAddParticipant", [this](const std::string& req) -> std::string {
    // Parse request and call C++ function
    videoGridManager_->addParticipant(name, videoPath);
    return "{}";
});
```

**AFTER:**
```cpp
// Register C++ message handler
webview_->addScriptHandler("videoAddParticipant", [this](const std::string& message) {
    // Parse message and call C++ function
    videoGridManager_->addParticipant(name, videoPath);

    // No need to return string - WebView2 uses message passing
});

// In JavaScript (update UI code):
// BEFORE:
// const result = await window.videoAddParticipant(JSON.stringify({name: "..."}));

// AFTER:
// window.webkit.messageHandlers.videoAddParticipant.postMessage({name: "..."});
```

**Key Changes:**
- Use `addScriptHandler()` instead of `bind()`
- JavaScript uses message handlers (similar to iOS WKWebView)
- Asynchronous message passing instead of blocking RPC

---

### 7. CMakeLists.txt

**BEFORE:**
```cmake
# Include webview library
include_directories(${CMAKE_SOURCE_DIR}/external/webview)

# Sources
set(SOURCES
    src/app.cpp
    src/main.cpp
)
```

**AFTER:**
```cmake
# WebView2 SDK
set(WEBVIEW2_SDK_PATH "${CMAKE_SOURCE_DIR}/external/webview2-macos")
set(WEBVIEW2_INCLUDE_DIR "${WEBVIEW2_SDK_PATH}/include")
set(WEBVIEW2_LIBRARY_DIR "${WEBVIEW2_SDK_PATH}/lib")

# Include WebView2 headers
include_directories(${WEBVIEW2_INCLUDE_DIR})

# Sources
set(SOURCES
    src/app.cpp
    src/main.cpp
    src/webview/platform/macos/webview2_wrapper_macos.mm  # NEW
)

# Link WebView2 framework
if(APPLE)
    find_library(WEBVIEW2_FRAMEWORK WebView2 PATHS ${WEBVIEW2_LIBRARY_DIR} NO_DEFAULT_PATH)
    if(NOT WEBVIEW2_FRAMEWORK)
        message(FATAL_ERROR "WebView2 framework not found in ${WEBVIEW2_LIBRARY_DIR}")
    endif()
    target_link_libraries(TeamsBICSandbox ${WEBVIEW2_FRAMEWORK})
endif()
```

**Changes:**
1. Remove webview library include
2. Add WebView2 SDK paths
3. Add webview2_wrapper_macos.mm to sources
4. Link WebView2 framework

---

## React UI Changes

### Update JavaScript Bridge Calls

**File: `/ui/src/bridge/videoBridge.ts`**

**BEFORE:**
```typescript
export const addParticipant = async (name: string): Promise<void> => {
    if (window.videoAddParticipant) {
        const result = await window.videoAddParticipant(
            JSON.stringify({ name })
        );
    }
};
```

**AFTER:**
```typescript
export const addParticipant = async (name: string): Promise<void> => {
    // WebView2 uses message handlers (WebKit style)
    if (window.webkit?.messageHandlers?.videoAddParticipant) {
        window.webkit.messageHandlers.videoAddParticipant.postMessage({
            name
        });
    }
};
```

---

## Build Steps

1. **Download WebView2 SDK:**
   ```bash
   # Download from Azure DevOps
   # Extract to external/webview2-macos/
   ```

2. **Update CMake configuration:**
   ```bash
   cd build
   rm -rf *
   cmake ..
   ```

3. **Build:**
   ```bash
   make
   ```

4. **Run:**
   ```bash
   ./TeamsBICSandbox
   ```

---

## Testing Checklist

After integration, test these scenarios:

- [ ] Main window loads with React UI
- [ ] Add 3 participants - all appear
- [ ] Drag participant outside - child window created
- [ ] Drag another participant to existing child window - **NO HANG**
- [ ] Drag participant between multiple child windows
- [ ] Close child window - tiles return to main
- [ ] Open devtools in child window (Cmd+Opt+I)
- [ ] Inspect elements in child window
- [ ] JavaScript console works in child windows
- [ ] Video continues playing during drag operations
- [ ] No memory leaks after creating/closing multiple windows

---

## Expected Results

### Performance
- **Before (webview library):** Hangs when dragging to existing window
- **After (WebView2):** Smooth drag-and-drop, no freezing

### Multi-Window
- **Before:** Single event loop, blocking operations
- **After:** Independent WebView2 instances per window

### Debugging
- **Before:** Limited devtools, unreliable in child windows
- **After:** Full Chrome DevTools in all windows

### Code Simplicity
- **Before:** Complex workarounds, async hacks, notification suppression
- **After:** Clean initialization, natural multi-window support

---

## Rollback

If WebView2 integration fails, rollback with:

```bash
git checkout main
git checkout app.cpp app.h
git checkout src/window/child_window*
```

Keep this branch for future attempts:
```bash
git branch webview2-integration
```

---

## Next Steps

1. Download WebView2 macOS SDK from Azure DevOps
2. Extract to `external/webview2-macos/`
3. Examine SDK headers (`WebView2.h`, `WebView2Environment.h`)
4. Update `webview2_wrapper_macos.mm` with actual SDK API calls
5. Update CMakeLists.txt
6. Build and test
7. Document any SDK-specific quirks or issues
