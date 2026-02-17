# WebView2 Integration - Quick Reference Card

## Problem
Application hangs when dragging video tiles to existing child windows.

**Root Cause:** webview library uses blocking event loop, incompatible with multi-window scenarios.

## Solution
Replace webview library with Microsoft's internal WebView2 SDK for macOS.

## SDK Location
```
https://dev.azure.com/microsoft/Edge/_artifacts/feed/webview2-mac-stable/UPack/webview2_sdk/overview/0.3800.64
```
(Requires Microsoft employee authentication - you have access)

## Quick Start
```bash
# 1. Download SDK from Azure DevOps
# 2. Extract to project
mkdir -p external/webview2-macos
cd external/webview2-macos
unzip ~/Downloads/webview2_sdk-*.zip

# 3. Verify structure
ls -la include/  # Should have WebView2.h
ls -la lib/      # Should have framework or .dylib

# 4. Test minimal example
cd examples
# Update webview2_minimal_test.mm with SDK API
clang++ -o test webview2_minimal_test.mm \
    -I../external/webview2-macos/include \
    -L../external/webview2-macos/lib \
    -framework Cocoa -framework WebView2 \
    -std=c++17 -ObjC++ -DWEBVIEW2_SDK_AVAILABLE
./test  # Verify multiple windows work

# 5. Integrate into main app
# Follow docs/WEBVIEW2_CODE_CHANGES.md
cd build && rm -rf * && cmake .. && make
./TeamsBICSandbox  # Test multi-window drag
```

## Files Created

### Documentation
- `/docs/README_WEBVIEW2.md` - Complete overview
- `/docs/WEBVIEW2_MIGRATION.md` - Migration guide
- `/docs/WEBVIEW2_CODE_CHANGES.md` - Specific code changes
- `/docs/WEBVIEW2_INTEGRATION_CHECKLIST.md` - Step-by-step checklist

### Code
- `/src/webview/webview2_wrapper.h` - Abstraction interface
- `/src/webview/platform/macos/webview2_wrapper_macos.mm` - macOS implementation
- `/examples/webview2_minimal_test.mm` - Standalone test

## Key Changes Required

| File | What to Change |
|------|----------------|
| `CMakeLists.txt` | Add WebView2 SDK paths and linking |
| `app.cpp` | Initialize WebView2Manager, remove run() |
| `child_window_macos.mm` | Use WebView2Manager to create instances |
| `video_grid_manager.cpp` | Use executeScript() not eval() |
| `videoBridge.ts` | Use message handlers not RPC |

## API Comparison

| webview library | WebView2 |
|----------------|----------|
| `new webview::webview(debug, window)` | `WebView2Manager::getInstance().createWebView(window, debug)` |
| `webview->set_html(html)` | `webview->setHtml(html)` |
| `webview->eval(js)` | `webview->executeScript(js)` ← non-blocking! |
| `webview->run()` ← blocking! | Native event loop (NSApp run) |
| Single instance only | Multiple independent instances ✓ |

## Testing Checklist

After integration, verify:
- [ ] Add 3 participants
- [ ] Drag one outside → child window created
- [ ] Drag another to child window → **NO HANG** ✓
- [ ] Drag between multiple windows
- [ ] Devtools work in all windows (Cmd+Opt+I)

## Expected Behavior

### Before (webview library)
```
Drag to child window → HANGS ✗
UI frozen, no events processed
```

### After (WebView2)
```
Drag to child window → WORKS ✓
All windows remain responsive
```

## Next Step

**Download WebView2 SDK from Azure DevOps and examine its structure.**

Then follow: `/docs/WEBVIEW2_INTEGRATION_CHECKLIST.md`

## Estimated Time: 5-8 hours

---

**Status:** Infrastructure ready, waiting for SDK download
**Blocking:** Need to access Azure DevOps and download package
**Success:** Multi-window drag-and-drop without hanging
