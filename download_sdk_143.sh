#!/bin/bash

# Download WebView2 SDK version 143.0.3650.139 to match Teams runtime

SDK_VERSION="143.0.3650.139"
SDK_DIR="external/webview2-macos"

echo "========================================"
echo "  WebView2 SDK Download (v$SDK_VERSION)"
echo "========================================"
echo ""
echo "You need to download the SDK version that matches the installed runtime."
echo ""
echo "Step 1: Open this URL in your browser:"
echo "https://dev.azure.com/microsoft/Edge/_artifacts/feed/webview2-mac-stable/UPack/webview2_sdk/overview/0.3650.139"
echo ""
echo "Step 2: Click 'Download' button"
echo ""
echo "Step 3: Save the file to ~/Downloads/"
echo ""
read -p "Press Enter after downloading the SDK package... "
echo ""

# Look for downloaded SDK
SDK_FILE=$(ls ~/Downloads/webview2_sdk*3650* 2>/dev/null | head -1)

if [ -z "$SDK_FILE" ]; then
    echo "❌ SDK package not found in ~/Downloads/"
    echo "   Looking for: webview2_sdk*3650*"
    echo ""
    echo "Available files:"
    ls -1 ~/Downloads/webview2* 2>/dev/null || echo "  (no webview2 files found)"
    exit 1
fi

echo "Found SDK package: $SDK_FILE"
echo ""

# Backup current SDK
echo "Backing up current SDK..."
if [ -d "$SDK_DIR/MSWebView2Staging.framework" ]; then
    mv "$SDK_DIR/MSWebView2Staging.framework" "$SDK_DIR/MSWebView2Staging.framework.bak145"
    echo "✅ Backed up to: MSWebView2Staging.framework.bak145"
fi

# Extract new SDK
echo "Extracting SDK..."
cd "$SDK_DIR"

if [[ "$SDK_FILE" == *.zip ]]; then
    unzip -q "$SDK_FILE"
elif [[ "$SDK_FILE" == *.tar.gz ]]; then
    tar -xzf "$SDK_FILE"
else
    echo "❌ Unknown file format"
    exit 1
fi

# Verify extraction
if [ -d "MSWebView2Staging.framework" ]; then
    echo "✅ SDK extracted successfully"

    # Check version in headers
    if grep -q "143.0.3650.139" MSWebView2Staging.framework/Headers/*.h 2>/dev/null; then
        echo "✅ SDK version verified: 143.0.3650.139"
    else
        echo "⚠️  Could not verify SDK version in headers"
    fi
else
    echo "❌ Framework not found after extraction"
    echo "Contents of directory:"
    ls -la
    exit 1
fi

cd ../..

echo ""
echo "========================================"
echo "  SDK Downgrade Complete!"
echo "========================================"
echo ""
echo "Next step: Rebuild and test"
echo ""
echo "  clang++ -o test_webview2 test_webview2.mm src/webview/platform/macos/webview2_wrapper_macos.mm \\"
echo "    -I. -Isrc -Fexternal/webview2-macos -framework MSWebView2Staging -framework Cocoa \\"
echo "    -std=c++17 -ObjC++ -fobjc-arc -Wl,-rpath,@executable_path/external/webview2-macos"
echo ""
echo "  ./test_webview2"
echo ""
