# WebView2 Multi-Window Investigation - Findings and Recommendations

## Summary

After extensive investigation into creating multiple WebView2 windows on macOS, we discovered critical lifecycle and memory management issues when closing child windows containing WebView2 instances.

## Issues Discovered

### Issue 1: Window Animation Crash (SOLVED)
**Problem**: Closing a child window with WebView2 caused crash in `objc_release` during `_NSWindowTransformAnimation dealloc`.

**Root Cause**: macOS window close animations capture references to views in the window. When WebView2 objects are deallocated before the animation completes, the animation tries to release already-freed objects.

**Solution**: Disable ALL window animations by setting `NSWindowAnimationBehaviorNone` at window creation:
```objc
[childWindow setAnimationBehavior:NSWindowAnimationBehaviorNone];
```

**Status**: ✅ FIXED - No more animation crashes

### Issue 2: Autoreleased Object Crash (ONGOING)
**Problem**: After fixing animation crash, new crash occurs in `objc_autoreleasePoolPop` during `-[NSApplication run]` cleanup.

**Root Cause**: Over-release or use-after-free of WebView2-related objects in the autorelease pool.

**Status**: ⚠️ UNRESOLVED - Memory management conflict between WebView2 and Cocoa

## Technical Details

### What Works
1. ✅ WebView2 SDK v145.0.3800.64 integrates successfully on macOS
2. ✅ Main window with WebView2 works perfectly
3. ✅ Child windows can be created with WebView2 using async API
4. ✅ Multiple WebView2 instances can run concurrently
5. ✅ JavaScript-to-C++ message bridge works in all windows
6. ✅ Keeping WebView2Instance alive prevents early deallocation crashes
7. ✅ Disabling window animations prevents animation system crashes

### What Doesn't Work
1. ❌ Cleanly closing child WebView2 windows without crashes
2. ❌ Proper cleanup of WebView2 instances during window close
3. ❌ WebView2 lifecycle synchronization with NSWindow animations and autorelease pools

## Architecture Recommendations

### Option 1: Native Child Windows (RECOMMENDED)
**For the main application**, use this hybrid approach:
- Main window: WebView2 (for React UI)
- Child windows: Native AppKit views with AVPlayerLayer for video
- Benefits:
  - Avoids all WebView2 multi-window lifecycle issues
  - Better performance (no browser engine overhead per window)
  - Simpler memory management
  - Native macOS window behavior
  - More reliable

### Option 2: WebView2 Child Windows with Workarounds
If WebView2 is required in child windows:
1. Disable all window animations:
   ```objc
   [window setAnimationBehavior:NSWindowAnimationBehaviorNone];
   ```
2. Keep WebView2Instance alive even after window closes:
   ```cpp
   static std::vector<std::shared_ptr<WebView2Instance>> allWebviews;
   ```
3. Remove views before window close:
   ```objc
   - (BOOL)windowShouldClose:(NSWindow*)sender {
       // Remove all subviews
       for (NSView* subview in [[sender.contentView subviews] copy]) {
           [subview removeFromSuperview];
       }
       return YES;
   }
   ```
4. Accept that cleanup happens at app exit, not window close
5. **Known Limitation**: Still has autorelease pool crashes on window close

### Option 3: Single Window with Virtual Grid
- Keep all video tiles in the main window
- Use window management to resize/reposition the main window
- Avoid multi-window complexity entirely

## Code Changes Made

### 1. Async API for WebView2
Created non-blocking initialization to avoid deadlocks:
```cpp
// webview2_wrapper.h
using InitCallback = std::function<void(bool success)>;

class WebView2Instance {
public:
    void initializeAsync(InitCallback callback);
};

class WebView2Manager {
public:
    void createWebViewAsync(void* window, bool debug,
        std::function<void(std::shared_ptr<WebView2Instance>)> callback);
};
```

### 2. Disabled Window Animations
```objc
// test_webview2.mm - at window creation
[childWindow setAnimationBehavior:NSWindowAnimationBehaviorNone];
```

### 3. View Cleanup on Close
```objc
// test_webview2.mm - ChildWindowDelegate
- (BOOL)windowShouldClose:(NSWindow *)sender {
    // Remove all subviews before close
    for (NSView* subview in [[sender.contentView subviews] copy]) {
        [subview removeFromSuperview];
    }
    return YES;
}
```

### 4. Deferred WebView2 Cleanup
```cpp
// webview2_wrapper_macos.mm - destructor
WebView2Instance::~WebView2Instance() {
    // Do NOT call [controller close] - let ARC handle it
    // Prevents crashes during window animations
    if (controller_) {
        MSWebView2Controller* controller = (__bridge_transfer MSWebView2Controller*)controller_;
        controller = nil;  // Transfer to ARC
    }
}
```

### 5. ARC Compilation
```bash
# Build with Automatic Reference Counting
clang++ -fobjc-arc ...
```

## Testing Results

| Test Case | Result | Notes |
|-----------|--------|-------|
| Create main window with WebView2 | ✅ Pass | No issues |
| Create child window with WebView2 | ✅ Pass | Works with async API |
| Multiple concurrent WebView2 instances | ✅ Pass | All render correctly |
| JavaScript execution in child windows | ✅ Pass | Message bridge works |
| Close child window (with animations) | ❌ Crash | `_NSWindowTransformAnimation dealloc` |
| Close child window (no animations) | ❌ Crash | `objc_autoreleasePoolPop` |
| Keep app running after window close | ❌ Fail | Crashes during cleanup |

## Crash Reports

### Animation Crash (before fix)
```
#0  objc_release
#1  -[_NSWindowTransformAnimation dealloc]
#2  _Block_release
#3  -[NSConcretePointerArray dealloc]
#4  AutoreleasePoolPage::releaseUntil
#5  CA::Transaction::commit_transaction
```

### Autorelease Pool Crash (current)
```
#0  objc_release (crashed at 0x20 - null pointer)
#1  AutoreleasePoolPage::releaseUntil
#2  objc_autoreleasePoolPop
#3  _CFAutoreleasePoolPop
#4  -[NSAutoreleasePool drain]
#5  -[NSApplication run]
```

## Recommendation for Main Application

**Use Option 1: Native Child Windows**

For `/Users/elgasano/Sources/TeamsBICSandbox`, implement:
1. Main window: Keep WebView2 for React UI (already working)
2. Child windows: Use native `NSWindow` + `CALayer` + `AVPlayerLayer`
3. Video rendering: Use existing `VideoGridManager` with native rendering
4. No browser overhead in child windows
5. Reliable window lifecycle management

This avoids all WebView2 multi-window issues while maintaining the React UI in the main window.

## Files Modified

- `src/webview/webview2_wrapper.h` - Added async API
- `src/webview/platform/macos/webview2_wrapper_macos.mm` - Implemented async, fixed destructor
- `test_webview2.mm` - Created POC with window delegate, disabled animations
- `WEBVIEW2_POC_SUMMARY.md` - Initial findings document

## Build Instructions

```bash
# Build test with ARC and disabled animations
clang++ -o test_webview2 test_webview2.mm \
    src/webview/platform/macos/webview2_wrapper_macos.mm \
    -I. -Isrc \
    -F external/webview2-macos \
    -framework MSWebView2Staging \
    -framework Cocoa \
    -std=c++17 -ObjC++ \
    -fobjc-arc \
    -Wl,-rpath,@executable_path/external/webview2-macos \
    -Wl,-rpath,external/webview2-macos
```

## Next Steps

1. **For POC**: Accept current limitations, document known issues
2. **For Main App**: Implement Option 1 (native child windows)
3. **Future**: Monitor WebView2 SDK updates for improved lifecycle management

## Conclusion

WebView2 on macOS has complex interactions with Cocoa's window management and memory systems. While single-window WebView2 works perfectly, multi-window scenarios require careful lifecycle management and currently have unresolved memory issues.

**The recommended approach is to use WebView2 only for the main window and native views for child windows.**
