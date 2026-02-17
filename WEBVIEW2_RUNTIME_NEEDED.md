# WebView2 Runtime Required

## Current Status

✅ **SDK Integration Complete**
- WebView2 SDK framework is downloaded and integrated
- C++ wrapper (`webview2_wrapper_macos.mm`) implemented with actual API
- CMakeLists.txt updated to link framework
- Minimal test compiles successfully

❌ **Runtime Not Installed**
The test fails with:
```
The libEmbeddedBrowserWebView.dylib doesn't exist at 
/Users/elgasano/Library/Application Support/Microsoft WebView2/Microsoft Edge WebView2.app/
Contents/Frameworks/Microsoft Edge Framework.framework/Libraries/libEmbeddedBrowserWebView.dylib
```

## What's Missing

The WebView2 SDK is just the headers and loader. The actual runtime (browser engine) needs to be installed separately.

**Expected Location:**
```
~/Library/Application Support/Microsoft WebView2/Microsoft Edge WebView2.app
```

## Solutions

### Option 1: Install System Runtime (Recommended for Development)
The WebView2 runtime needs to be installed system-wide. This is likely available through:
- Internal Microsoft distribution
- Microsoft Teams installation (if Teams uses it, runtime might already be there)
- Separate WebView2 runtime installer

**Check if already installed:**
```bash
ls -la ~/Library/Application\ Support/Microsoft\ WebView2/
ls -la /Library/Application\ Support/Microsoft\ WebView2/
```

### Option 2: Bundle Runtime with Application (For Distribution)
For distributing the application:
1. Bundle the WebView2 runtime inside the app bundle
2. Set `WEBVIEW2_BROWSER_EXECUTABLE_FOLDER` environment variable
3. Or use `browserExecutableFolder` parameter in `CreateMSWebView2Environment()`

## Next Steps

1. **Locate WebView2 Runtime:**
   - Check if Microsoft Teams installation includes it
   - Ask internal Teams/WebView2 team for runtime installer
   - Check Azure DevOps for runtime package (separate from SDK)

2. **Install Runtime:**
   - Install to: `~/Library/Application Support/Microsoft WebView2/`
   - Or bundle with application

3. **Re-run Test:**
   ```bash
   ./test_webview2
   ```

4. **Once Runtime Works:**
   - Complete app.cpp migration
   - Test multi-window drag-and-drop
   - Verify hanging issue is fixed

## Teams Question

Since Microsoft Teams uses WebView2 on macOS, the runtime should either:
- Be bundled within Teams.app
- Be installed as a shared system component

**Check Teams installation:**
```bash
ls -la /Applications/Microsoft\ Teams.app/Contents/Frameworks/ | grep -i webview
ls -la /Applications/Microsoft\ Teams\ \(work\ or\ school\).app/Contents/Frameworks/ | grep -i webview
```

If Teams bundles it, we might be able to use that path for development/testing.

## Contact

For runtime distribution questions:
- Edge WebView2 team (internal)
- Check #webview2 or similar internal channels
- Azure DevOps artifacts might have runtime package separate from SDK

