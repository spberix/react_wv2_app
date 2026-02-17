# WebView2 Integration for Multi-Window Support

## Problem Summary

The current application uses the `webview` library (github.com/webview/webview), which has a fundamental architectural limitation: its `run()` method is a blocking event loop designed for single-window applications. This causes the application to hang when dynamically creating or populating child windows, specifically:

**Failing Scenario:**
1. Add 3 participants ✓
2. Drag one participant outside window → child window created ✓
3. Drag another participant to existing child window → **APPLICATION HANGS** ✗

**Root Cause:**
- The webview library's event loop takes complete control of the main thread
- Multiple webview instances cannot coexist in the same process
- Any `webview->eval()` call during window operations blocks the UI
- The library was never designed for multi-window scenarios

## Solution: Microsoft WebView2 for macOS

Microsoft Teams uses an internal WebView2 SDK for macOS that provides proper multi-window support. This SDK:
- Integrates with the native NSRunLoop (non-blocking)
- Supports multiple independent WebView2 instances
- Provides the same API on Windows and macOS
- Includes full Chrome DevTools support
- Is production-tested in Microsoft Teams

**SDK Location:**
```
https://dev.azure.com/microsoft/Edge/_artifacts/feed/webview2-mac-stable/UPack/webview2_sdk/overview/0.3800.64
```

Requires Microsoft employee authentication (you have confirmed access).

## Architecture Change

### Before (webview library)
```
Application
├── webview::webview* (single instance)
│   └── run() - BLOCKING CALL
└── Child windows blocked by shared event loop
```

### After (WebView2)
```
Application
├── WebView2Manager (singleton environment)
├── MainWindow
│   └── WebView2Instance (independent)
└── ChildWindows[]
    └── WebView2Instance (independent, non-blocking)
```

## Integration Overview

### Created Files

**Documentation:**
- `/docs/WEBVIEW2_MIGRATION.md` - Complete migration guide
- `/docs/WEBVIEW2_CODE_CHANGES.md` - Specific code changes needed
- `/docs/WEBVIEW2_INTEGRATION_CHECKLIST.md` - Step-by-step checklist
- `/docs/README_WEBVIEW2.md` - This file

**Code Infrastructure:**
- `/src/webview/webview2_wrapper.h` - C++ abstraction layer
- `/src/webview/platform/macos/webview2_wrapper_macos.mm` - macOS implementation stub
- `/examples/webview2_minimal_test.mm` - Standalone test application

### Key Components

**1. WebView2Manager (Singleton)**
- Manages shared WebView2 environment
- Creates WebView2 instances for each window
- Handles SDK initialization and shutdown

**2. WebView2Instance (Per Window)**
- Wraps WebView2 controller for one window
- Provides API matching previous webview library
- Non-blocking operations
- Independent event handling

**3. Integration Points**
- `app.cpp` - Initialize WebView2Manager at startup
- `child_window_macos.mm` - Create WebView2 instances for child windows
- `video_grid_manager.cpp` - Use non-blocking executeScript()
- `CMakeLists.txt` - Link WebView2 framework

## Files Requiring Changes

| File | Change Type | Description |
|------|-------------|-------------|
| `CMakeLists.txt` | Add | WebView2 SDK paths and linking |
| `app.h` | Modify | Change webview member type |
| `app.cpp` | Modify | Initialize WebView2Manager, remove run() |
| `child_window.h` | Modify | Change webview member type |
| `child_window_macos.mm` | Modify | Create WebView2 instances |
| `video_grid_manager.cpp` | Modify | Use executeScript() instead of eval() |
| `videoBridge.ts` | Modify | Use message handlers instead of RPC |

**New files added:**
- `webview2_wrapper.h`
- `webview2_wrapper_macos.mm`

## Quick Start

### Step 1: Download SDK
```bash
# Access Azure DevOps (requires Microsoft login)
open "https://dev.azure.com/microsoft/Edge/_artifacts/feed/webview2-mac-stable"

# Download webview2_sdk version 0.3800.64 or later
# Save to Downloads folder
```

### Step 2: Extract SDK
```bash
cd /Users/elgasano/Sources/TeamsBICSandbox
mkdir -p external/webview2-macos
cd external/webview2-macos

# Extract downloaded package (adjust filename)
unzip ~/Downloads/webview2_sdk-0.3800.64.zip

# Verify structure
tree . > ../../docs/webview2_sdk_structure.txt
ls -la include/
ls -la lib/
```

### Step 3: Examine SDK
```bash
# Check for headers
cat include/WebView2.h | head -50

# Check for framework or library
ls -la lib/

# Read any documentation
cat README.md
cat docs/API_Reference.md
```

### Step 4: Update Wrapper Implementation
```bash
# Open wrapper implementation
open src/webview/platform/macos/webview2_wrapper_macos.mm

# Replace TODO sections with actual SDK API calls
# Use patterns from SDK documentation
# Follow examples from minimal test
```

### Step 5: Build and Test Minimal Example
```bash
cd examples

# Update webview2_minimal_test.mm with SDK API
# Build standalone test
clang++ -o webview2_test webview2_minimal_test.mm \
    -I../external/webview2-macos/include \
    -L../external/webview2-macos/lib \
    -framework Cocoa -framework WebView2 \
    -std=c++17 -ObjC++ \
    -DWEBVIEW2_SDK_AVAILABLE

# Run test
./webview2_test

# Verify multiple windows can be created without hanging
```

### Step 6: Integrate into Main Application
```bash
# Update CMakeLists.txt
# Follow changes from docs/WEBVIEW2_CODE_CHANGES.md

# Rebuild
cd build
rm -rf *
cmake ..
make

# Run
./TeamsBICSandbox

# Test the failing scenario:
# 1. Add 3 participants
# 2. Drag one outside
# 3. Drag another to existing child window
# Should NOT hang!
```

## Expected Results

### Before Integration
```
[LOG] Added participant 3: Participant 3
[LOG] Mouse up at screen: 2110.88, 757.613
[LOG] Drag state: none (notification disabled)
[LOG] Executing transfer IMMEDIATELY (no timer)
[LOG] Transferring participant 2: Participant 2
[LOG] Removed video tile for participant 2
[LOG] Transfer complete: removed 2 from source, added as 2 to target
[UI] ← FROZEN - no further events processed
```

### After Integration
```
[LOG] Added participant 3: Participant 3
[LOG] Mouse up at screen: 2110.88, 757.613
[LOG] Drag state: none (notification disabled)
[LOG] Executing transfer IMMEDIATELY
[LOG] Created WebView2 instance (total: 2)
[LOG] Transferring participant 2: Participant 2
[LOG] Removed video tile for participant 2
[LOG] Added video tile for participant 2
[LOG] Transfer complete: removed 2 from source, added as 2 to target
[LOG] Notified React with 1 participants
[UI] ← RESPONSIVE - continues processing events ✓
```

## Benefits of WebView2

### 1. Non-Blocking Architecture
- **Before:** `webview->run()` blocks main thread
- **After:** WebView2 integrates with NSRunLoop

### 2. Multi-Window Support
- **Before:** Single webview instance, shared event loop
- **After:** Independent WebView2 instance per window

### 3. Better Debugging
- **Before:** Limited devtools, unreliable in child windows
- **After:** Full Chrome DevTools in every window

### 4. Cross-Platform Consistency
- **Before:** Different behavior on different platforms
- **After:** Same WebView2 API on Windows and macOS

### 5. Production Quality
- **Before:** Third-party library with limitations
- **After:** Microsoft's production SDK used in Teams

## Testing Checklist

Once integrated, verify these scenarios:

- [ ] Main window loads with React UI
- [ ] Add 3 participants - all appear
- [ ] Drag participant outside - child window created ✓
- [ ] Drag another to existing child window - **NO HANG** ✓
- [ ] Drag participant between multiple windows ✓
- [ ] Create 5+ child windows - all responsive ✓
- [ ] Close child window - tiles return to main ✓
- [ ] Open devtools in child window (Cmd+Opt+I) ✓
- [ ] Inspect elements in all windows ✓
- [ ] JavaScript console works everywhere ✓
- [ ] Videos play smoothly during operations ✓
- [ ] No memory leaks after many operations ✓

## Support and Documentation

**Created documentation files:**
1. `WEBVIEW2_MIGRATION.md` - Overview and API mapping
2. `WEBVIEW2_CODE_CHANGES.md` - Specific file changes
3. `WEBVIEW2_INTEGRATION_CHECKLIST.md` - Step-by-step guide
4. `README_WEBVIEW2.md` - This overview

**Code infrastructure:**
- WebView2 wrapper classes ready for SDK integration
- Minimal test application for verifying SDK works
- All integration points identified and documented

**Next action required:**
Download the WebView2 SDK from Azure DevOps and examine its structure. Once the SDK API is understood, the wrapper implementation can be completed following the documented patterns.

## Timeline

**Estimated integration time: 5-8 hours**
- SDK acquisition and examination: 15 minutes
- Minimal test implementation: 1-2 hours
- Main application integration: 2-3 hours
- Testing and validation: 1-2 hours
- Documentation and cleanup: 30 minutes

## Contact

For SDK-specific questions:
- Check WebView2 SDK documentation in package
- Consult Edge WebView2 team on Microsoft Teams
- Reference Azure DevOps artifacts page for updates

## Summary

The webview library has served well for initial prototyping but cannot support the required multi-window functionality. Microsoft's internal WebView2 SDK for macOS is the correct solution - it's the same technology used by Microsoft Teams for exactly this use case.

The infrastructure is ready: wrapper classes are defined, integration points are documented, and a test application is prepared. The final step is downloading the SDK, examining its API, and filling in the wrapper implementation with actual SDK calls.

Once integrated, the application will have proper multi-window support without hanging, matching the functionality of Microsoft Teams.

---

**Current Status: Ready for SDK Integration**
**Blocking: Need to download and examine WebView2 SDK**
**Next Step: Access Azure DevOps and download SDK package**
