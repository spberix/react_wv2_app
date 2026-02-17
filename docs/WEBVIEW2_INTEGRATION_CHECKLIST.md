# WebView2 Integration Checklist

## Phase 1: SDK Acquisition and Verification

### 1.1 Download SDK
- [ ] Access Azure DevOps: https://dev.azure.com/microsoft/Edge/_artifacts/feed/webview2-mac-stable
- [ ] Download `webview2_sdk` version 0.3800.64 or later
- [ ] Save to desktop or downloads folder
- [ ] Verify file integrity (check file size is reasonable, e.g., > 10 MB)

### 1.2 Extract and Verify
```bash
# Create directory
mkdir -p /Users/elgasano/Sources/TeamsBICSandbox/external/webview2-macos

# Extract SDK (adjust path to downloaded file)
cd /Users/elgasano/Sources/TeamsBICSandbox/external/webview2-macos
unzip ~/Downloads/webview2_sdk-0.3800.64.zip

# OR if it's a tar.gz:
tar -xzf ~/Downloads/webview2_sdk-0.3800.64.tar.gz
```

- [ ] Verify directory structure created
- [ ] Check for `include/` directory
- [ ] Check for `lib/` directory
- [ ] List header files: `ls -la include/`
- [ ] List library files: `ls -la lib/`

### 1.3 Examine SDK Contents

**Expected files (verify these exist):**
- [ ] `include/WebView2.h` (or similar header)
- [ ] `include/WebView2Environment.h` (or similar)
- [ ] `lib/WebView2.framework/` OR `lib/libWebView2.dylib`
- [ ] Documentation: `README.md`, `CHANGELOG.md`, or similar
- [ ] Samples: check for `examples/` or `samples/` directory

**Document findings:**
```bash
# Save structure to file for reference
tree /Users/elgasano/Sources/TeamsBICSandbox/external/webview2-macos > sdk_structure.txt
```

- [ ] Save output to `/docs/webview2_sdk_structure.txt`
- [ ] Review any README or documentation files
- [ ] Check for API reference or header comments

---

## Phase 2: Minimal Test Application

### 2.1 Update Test Application
- [ ] Open `/examples/webview2_minimal_test.mm`
- [ ] Review SDK header files to understand API
- [ ] Update placeholder types with actual WebView2 types
- [ ] Uncomment `#import` statements
- [ ] Fill in TODO sections with actual SDK calls
- [ ] Define `-DWEBVIEW2_SDK_AVAILABLE` in build command

### 2.2 Build Test Application
```bash
cd /Users/elgasano/Sources/TeamsBICSandbox/examples

# Adjust framework path based on actual SDK structure
clang++ -o webview2_test webview2_minimal_test.mm \
    -I../external/webview2-macos/include \
    -L../external/webview2-macos/lib \
    -framework Cocoa -framework WebView2 \
    -std=c++17 -ObjC++ \
    -DWEBVIEW2_SDK_AVAILABLE
```

**Troubleshooting build errors:**
- [ ] If "framework not found": Check `-L` path matches actual lib location
- [ ] If "header not found": Check `-I` path matches actual include location
- [ ] If "undefined symbols": May need additional frameworks
- [ ] Document any additional flags needed

### 2.3 Run Test Application
```bash
./webview2_test
```

**Expected behavior:**
- [ ] Window opens with title "WebView2 Test Window 1"
- [ ] Window shows HTML content
- [ ] Clicking "Create New Window" button creates second window
- [ ] Both windows remain responsive (NO HANGING)
- [ ] Can create 3rd, 4th, 5th windows without issues
- [ ] Cmd+Opt+I opens devtools in each window
- [ ] JavaScript console works

**If test fails:**
- [ ] Check console output for error messages
- [ ] Verify WebView2 runtime is available: `ls /Library/Application\ Support/Microsoft/WebView2/`
- [ ] Try with debug flag: add `std::cout` statements to track execution
- [ ] Review WebView2 SDK documentation for initialization requirements

---

## Phase 3: Main Application Integration

### 3.1 Update CMakeLists.txt
- [ ] Open `/Users/elgasano/Sources/TeamsBICSandbox/CMakeLists.txt`
- [ ] Follow changes from `/docs/WEBVIEW2_CODE_CHANGES.md` section 7
- [ ] Add WebView2 SDK paths
- [ ] Add `webview2_wrapper_macos.mm` to sources
- [ ] Link WebView2 framework
- [ ] Remove old webview library references

### 3.2 Update webview2_wrapper Implementation
- [ ] Open `/src/webview/platform/macos/webview2_wrapper_macos.mm`
- [ ] Replace TODO sections with actual WebView2 SDK calls
- [ ] Use API patterns discovered in minimal test
- [ ] Ensure all methods are implemented
- [ ] Add error handling for SDK calls

### 3.3 Update Application Code
- [ ] Modify `/src/app.h` (see WEBVIEW2_CODE_CHANGES.md section 1)
- [ ] Modify `/src/app.cpp` (see section 2)
- [ ] Modify `/src/window/child_window.h` (see section 3)
- [ ] Modify `/src/window/platform/macos/child_window_macos.mm` (see section 4)
- [ ] Update `/src/video/video_grid_manager.cpp` notifyReact() (see section 5)

### 3.4 Update React Bridge
- [ ] Modify `/ui/src/bridge/videoBridge.ts`
- [ ] Replace `window.videoAddParticipant()` with message handlers
- [ ] Update all bridge functions to use WebKit message passing
- [ ] Test bridge communication works

### 3.5 Build Main Application
```bash
cd /Users/elgasano/Sources/TeamsBICSandbox/build
rm -rf *
cmake ..
make
```

**Build checklist:**
- [ ] CMake configuration succeeds
- [ ] WebView2 framework found
- [ ] All source files compile
- [ ] Linking succeeds
- [ ] No warnings about missing symbols
- [ ] Executable created: `./TeamsBICSandbox`

---

## Phase 4: Testing and Validation

### 4.1 Basic Functionality
```bash
./TeamsBICSandbox
```

- [ ] Main window opens
- [ ] React UI loads correctly
- [ ] No console errors
- [ ] Window is responsive
- [ ] Can interact with UI elements

### 4.2 Multi-Window Testing

**Test Scenario 1: Create child window**
- [ ] Click "Add Participant" 3 times
- [ ] Verify 3 video tiles appear
- [ ] Drag Participant 1 outside main window
- [ ] Child window appears at cursor location
- [ ] Participant 1 video plays in child window
- [ ] Main window shows 2 remaining participants
- [ ] Both windows remain responsive

**Test Scenario 2: Drag to existing child (THE CRITICAL TEST)**
- [ ] Start with 3 participants in main window
- [ ] Drag Participant 1 outside → child window created ✓
- [ ] Drag Participant 2 to existing child window
- [ ] **VERIFY: No hanging or freezing**
- [ ] Child window now shows 2 participants
- [ ] Main window shows 1 remaining participant
- [ ] All windows responsive

**Test Scenario 3: Multiple child windows**
- [ ] Add 5 participants
- [ ] Drag 3 participants outside → create 3 child windows
- [ ] Drag participants between windows
- [ ] Close one child window
- [ ] Verify tiles return to main window
- [ ] No crashes, no hangs

### 4.3 DevTools Testing
- [ ] Open main window devtools (Cmd+Opt+I)
- [ ] Verify console works
- [ ] Verify can inspect elements
- [ ] Open child window devtools
- [ ] Verify devtools work in child window
- [ ] Check for JavaScript errors in console

### 4.4 Performance Testing
- [ ] Create 10 child windows
- [ ] Drag tiles between multiple windows rapidly
- [ ] Check CPU usage (should be reasonable)
- [ ] Check memory usage (no leaks)
- [ ] Close all windows
- [ ] Verify clean shutdown

### 4.5 Edge Cases
- [ ] Drag tile and press Escape (should cancel)
- [ ] Drag tile back to original window
- [ ] Minimize/maximize windows during operation
- [ ] Full screen mode
- [ ] Multiple monitors (if available)

---

## Phase 5: Comparison and Validation

### 5.1 Before/After Comparison

**Problem: webview library (BEFORE):**
```
1. Add 3 participants ✓
2. Drag one outside → child window created ✓
3. Drag another to child window → HANGS ✗
```

**Solution: WebView2 SDK (AFTER):**
```
1. Add 3 participants ✓
2. Drag one outside → child window created ✓
3. Drag another to child window → WORKS! ✓
4. Continue dragging between any windows → WORKS! ✓
```

### 5.2 Success Criteria
- [ ] No hanging when creating child windows
- [ ] No freezing when dragging to existing windows
- [ ] All windows remain responsive during operations
- [ ] Video continues playing during drags
- [ ] React UI updates correctly
- [ ] DevTools work in all windows
- [ ] Memory usage is stable
- [ ] No crashes or errors

---

## Phase 6: Documentation and Cleanup

### 6.1 Document Changes
- [ ] Update main README.md with WebView2 requirements
- [ ] Document build steps with WebView2
- [ ] Note WebView2 SDK version used
- [ ] Document any SDK quirks or issues found

### 6.2 Code Cleanup
- [ ] Remove old webview library files
- [ ] Remove notification suppression code (no longer needed)
- [ ] Remove async workarounds
- [ ] Remove debugging logs added during investigation
- [ ] Update comments referencing old webview library

### 6.3 Git Commit
```bash
git add .
git commit -m "Replace webview library with WebView2 SDK for multi-window support

- Integrated Microsoft WebView2 macOS SDK (v0.3800.64)
- Created WebView2 wrapper abstraction layer
- Updated main application to use WebView2Manager
- Updated child windows to create WebView2 instances
- Fixed multi-window hanging issues
- Enabled devtools in all windows
- Removed notification suppression workarounds

Fixes: Multi-window drag-and-drop now works without hanging
Tested: All scenarios pass including drag-to-existing-window

Co-Authored-By: Claude Sonnet 4.5 <noreply@anthropic.com>"
```

---

## Troubleshooting Guide

### Issue: "WebView2 framework not found"
**Solution:**
1. Verify SDK extracted correctly: `ls external/webview2-macos/lib/`
2. Check CMakeLists.txt paths match actual structure
3. Try absolute path: `-L/Users/elgasano/Sources/TeamsBICSandbox/external/webview2-macos/lib`

### Issue: "Undefined symbols for WebView2Environment"
**Solution:**
1. Verify SDK is macOS version (not Windows ARM64)
2. Check architecture: `file external/webview2-macos/lib/libWebView2.dylib`
3. Should show: `Mach-O 64-bit dynamically linked shared library arm64`

### Issue: Test app compiles but crashes on launch
**Solution:**
1. Check WebView2 runtime installed: `ls /Library/Application\ Support/Microsoft/WebView2/`
2. SDK may require runtime bundle in app
3. Check SDK documentation for deployment requirements
4. Try copying .dylib to same directory as executable

### Issue: Window opens but shows blank/white
**Solution:**
1. Check HTML is loading: add console.log in JavaScript
2. Verify file:// URL is correct
3. Check WebView2 settings allow local file access
4. Review WebView2 console for errors

### Issue: Still hanging even with WebView2
**Solution:**
1. Verify actually using WebView2 (add log in wrapper)
2. Check not accidentally calling old webview code
3. Ensure all windows use WebView2Manager
4. Review event loop - ensure not blocking anywhere

---

## Success Indicators

You'll know the integration is successful when:

1. ✅ Test application creates multiple windows without hanging
2. ✅ Main application builds without errors
3. ✅ Can drag tiles to existing child windows smoothly
4. ✅ DevTools work in all windows
5. ✅ No more "the scenario still hangs out" reports
6. ✅ Logs show "WebView2 instance created" for each window
7. ✅ All videos continue playing during operations
8. ✅ Memory usage is stable over time

---

## Timeline Estimate

- Phase 1 (SDK acquisition): 15 minutes
- Phase 2 (Minimal test): 1-2 hours
- Phase 3 (Integration): 2-3 hours
- Phase 4 (Testing): 1-2 hours
- Phase 5 (Validation): 30 minutes
- Phase 6 (Cleanup): 30 minutes

**Total: 5-8 hours** depending on SDK complexity and issues encountered

---

## Next Steps

**Right now:**
1. Download WebView2 SDK from Azure DevOps
2. Extract and examine SDK structure
3. Share findings (header files, API structure)
4. Update minimal test application
5. Build and verify test works
6. Proceed with main integration

The critical first step is downloading the SDK and understanding its API structure. Once that's done, the integration follows the documented patterns.
