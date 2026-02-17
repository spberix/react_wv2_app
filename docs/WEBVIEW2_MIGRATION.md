# WebView2 macOS Integration Guide

## Overview

This document guides the integration of Microsoft's internal WebView2 SDK for macOS to replace the webview library and fix multi-window hanging issues.

## Why WebView2?

The current webview library (github.com/webview/webview) has fundamental limitations:
- Single event loop design (blocking `run()` method)
- Cannot support multiple webview instances in same process
- UI freezes when dynamically creating/managing multiple windows
- Not designed for multi-window scenarios

WebView2 provides:
- True multi-window support
- Non-blocking event loop
- Same API across Windows and macOS
- Used by Microsoft Teams for multi-window video conferencing

## SDK Location

**Internal Microsoft WebView2 SDK for macOS:**
```
https://dev.azure.com/microsoft/Edge/_artifacts/feed/webview2-mac-stable/UPack/webview2_sdk/overview/0.3800.64
```

Version: 0.3800.64 (February 2026)
Access: Requires Microsoft employee authentication

## Integration Steps

### Step 1: Download and Extract SDK

1. Download the WebView2 SDK package from Azure DevOps
2. Extract to `/Users/elgasano/Sources/TeamsBICSandbox/external/webview2-macos/`
3. Expected structure:
   ```
   external/webview2-macos/
   ├── include/
   │   ├── WebView2.h
   │   └── WebView2Environment.h
   ├── lib/
   │   ├── libWebView2.dylib
   │   └── WebView2.framework/
   └── docs/
       └── API_Reference.md
   ```

### Step 2: Update CMakeLists.txt

Add WebView2 SDK to the build system:

```cmake
# WebView2 SDK paths
set(WEBVIEW2_SDK_PATH "${CMAKE_SOURCE_DIR}/external/webview2-macos")
set(WEBVIEW2_INCLUDE_DIR "${WEBVIEW2_SDK_PATH}/include")
set(WEBVIEW2_LIBRARY_DIR "${WEBVIEW2_SDK_PATH}/lib")

# Include WebView2 headers
include_directories(${WEBVIEW2_INCLUDE_DIR})

# Link WebView2 libraries
if(APPLE)
    find_library(WEBVIEW2_FRAMEWORK WebView2 PATHS ${WEBVIEW2_LIBRARY_DIR} NO_DEFAULT_PATH)
    target_link_libraries(TeamsBICSandbox ${WEBVIEW2_FRAMEWORK})
endif()
```

### Step 3: Replace webview Library References

**Files to modify:**
- `/src/app.h` - Remove webview.h include, add webview2_wrapper.h
- `/src/app.cpp` - Replace webview initialization with WebView2
- `/src/window/child_window.h` - Update webview type
- `/src/window/platform/macos/child_window_macos.mm` - Replace webview creation

### Step 4: Use WebView2 Wrapper

The codebase now includes `webview2_wrapper.h` which provides:
- `WebView2Manager` - Manages WebView2 environment and controllers
- `WebView2Instance` - Individual webview instance for each window
- Consistent API matching previous webview library

**Example usage:**
```cpp
// In app.cpp initialization
WebView2Manager::getInstance().initialize();

// Creating webview for main window
auto mainWebView = WebView2Manager::getInstance().createWebView(mainWindow);
mainWebView->setHtml(htmlContent);
mainWebView->navigate("file:///path/to/index.html");

// Creating webview for child window
auto childWebView = WebView2Manager::getInstance().createWebView(childWindow);
childWebView->setHtml(htmlContent);

// No blocking run() call needed - WebView2 integrates with native event loop
```

### Step 5: Testing

**Test scenarios:**
1. Create main window with 3 participants
2. Drag participant outside → new child window created ✓
3. Drag another participant to existing child window ✓ (should NOT hang)
4. Drag participant between multiple child windows ✓
5. Close child window → participants return to main ✓
6. Open devtools in child windows ✓

### Step 6: Deployment

**Runtime requirements:**
- WebView2 runtime must be bundled with application
- Copy `libWebView2.dylib` to app bundle
- Update `Info.plist` with WebView2 framework path

**App bundle structure:**
```
TeamsBICSandbox.app/
├── Contents/
│   ├── MacOS/
│   │   └── TeamsBICSandbox
│   ├── Frameworks/
│   │   └── WebView2.framework/
│   └── Info.plist
```

## API Mapping

### webview library → WebView2

| webview library | WebView2 Equivalent |
|----------------|---------------------|
| `webview::webview w(true, nullptr)` | `WebView2Manager::getInstance().createWebView(window)` |
| `w.set_html(html)` | `webView->setHtml(html)` |
| `w.navigate(url)` | `webView->navigate(url)` |
| `w.eval(js)` | `webView->executeScript(js)` |
| `w.bind(name, callback)` | `webView->addScriptHandler(name, callback)` |
| `w.run()` | NOT NEEDED - non-blocking |

## Key Differences

### Event Loop
- **webview library**: Blocking `run()` takes over main thread
- **WebView2**: Integrates with native NSRunLoop, non-blocking

### Multi-Window
- **webview library**: Single instance only, shared event loop
- **WebView2**: Multiple independent instances per window

### JavaScript Bridge
- **webview library**: `bind()` with string callbacks
- **WebView2**: Message handlers with structured data

### Debugging
- **webview library**: Limited devtools support
- **WebView2**: Full Chrome DevTools in all windows

## Expected Performance Improvements

1. **No more hanging** - Non-blocking event loop prevents UI freezes
2. **Smooth drag-and-drop** - Native multi-window support
3. **Better debugging** - DevTools in all child windows
4. **Consistent behavior** - Same API on Windows and macOS
5. **Teams-grade reliability** - Production-tested in Microsoft Teams

## Rollback Plan

If issues occur during integration:
1. Keep webview library code in git history
2. Use feature flag to toggle between implementations
3. Test WebView2 branch independently before merging

## Next Steps

1. ✅ Download SDK from Azure DevOps
2. ⏳ Extract and verify SDK contents
3. ⏳ Update CMakeLists.txt with WebView2 paths
4. ⏳ Integrate WebView2Wrapper into existing code
5. ⏳ Test multi-window scenarios
6. ⏳ Remove webview library dependency
7. ⏳ Update documentation

## Support

For SDK-specific questions:
- Check internal WebView2 documentation in SDK package
- Consult Edge WebView2 team on Microsoft Teams
- Reference Edge DevOps artifacts page for updates
