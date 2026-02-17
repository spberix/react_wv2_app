#!/bin/bash

# WebView2 Integration - Quick Start Script
# Run this after downloading the WebView2 SDK

clear
echo "=========================================="
echo "  WebView2 Integration - Quick Start"
echo "=========================================="
echo ""

# Step 1: Check if SDK is downloaded
echo "Step 1: Checking for WebView2 SDK..."
SDK_DIR="/Users/elgasano/Sources/TeamsBICSandbox/external/webview2-macos"

if [ ! -d "$SDK_DIR/include" ] || [ ! -d "$SDK_DIR/lib" ]; then
    echo "❌ WebView2 SDK not found or incomplete"
    echo ""
    echo "Please download the SDK first:"
    echo "1. The Azure DevOps page should be open in your browser"
    echo "2. Log in with your Microsoft credentials"
    echo "3. Click 'Download' to get the SDK package"
    echo "4. Extract the package to: $SDK_DIR"
    echo ""
    echo "Instructions: $SDK_DIR/DOWNLOAD_INSTRUCTIONS.md"
    echo ""
    echo "After downloading, run this script again:"
    echo "  ./start_webview2_integration.sh"
    exit 1
fi

echo "✅ SDK directory found!"
echo ""

# Step 2: Verify SDK
echo "Step 2: Verifying SDK structure..."
./verify_webview2_sdk.sh

# Check verification result
if [ $? -ne 0 ]; then
    echo ""
    echo "❌ SDK verification failed"
    echo "Please check the output above and fix any issues"
    exit 1
fi

echo ""
echo "=========================================="
echo "  SDK Verified - Ready to Integrate"
echo "=========================================="
echo ""

# Step 3: Ask user if they want to proceed
read -p "Do you want to start the integration now? (y/n) " -n 1 -r
echo ""

if [[ ! $REPLY =~ ^[Yy]$ ]]; then
    echo "Integration cancelled."
    echo ""
    echo "When ready, follow: docs/WEBVIEW2_INTEGRATION_CHECKLIST.md"
    exit 0
fi

echo ""
echo "Starting integration process..."
echo ""

# Step 4: Build minimal test first
echo "Step 4: Building minimal test application..."
cd examples

# Find the header file to determine exact include path
HEADER_FILE=$(find "$SDK_DIR/include" -name "*WebView2*.h" -o -name "*webview2*.h" 2>/dev/null | head -1)
if [ -z "$HEADER_FILE" ]; then
    echo "❌ Could not find WebView2 header file"
    exit 1
fi

HEADER_NAME=$(basename "$HEADER_FILE")
echo "Using header: $HEADER_NAME"
echo ""

# Try to build minimal test
echo "Attempting to build test application..."
echo "Note: This may fail if SDK API differs from expectations"
echo ""

# Detect if it's a framework or dylib
if [ -d "$SDK_DIR/lib/WebView2.framework" ]; then
    LIB_TYPE="framework"
    LIB_PATH="-F$SDK_DIR/lib -framework WebView2"
elif [ -f "$SDK_DIR/lib/libWebView2.dylib" ]; then
    LIB_TYPE="dylib"
    LIB_PATH="-L$SDK_DIR/lib -lWebView2"
else
    echo "❌ Could not determine library type"
    echo "Check $SDK_DIR/lib/ for WebView2 library"
    exit 1
fi

echo "Library type: $LIB_TYPE"
echo "Build command:"
echo "clang++ -o webview2_test webview2_minimal_test.mm \\"
echo "    -I$SDK_DIR/include \\"
echo "    $LIB_PATH \\"
echo "    -framework Cocoa -std=c++17 -ObjC++ -DWEBVIEW2_SDK_AVAILABLE"
echo ""

# Note: This will likely fail because we need to update the test with actual SDK API
# That's expected at this stage
echo "⚠️  Build will fail until webview2_minimal_test.mm is updated with actual SDK API"
echo ""
echo "Next steps:"
echo "1. Examine SDK headers: cat $HEADER_FILE | less"
echo "2. Review docs: cat $SDK_DIR/README.md"
echo "3. Update webview2_minimal_test.mm with correct SDK API"
echo "4. Update src/webview/platform/macos/webview2_wrapper_macos.mm"
echo "5. Try build again"
echo ""

read -p "Do you want to examine the SDK header now? (y/n) " -n 1 -r
echo ""

if [[ $REPLY =~ ^[Yy]$ ]]; then
    echo ""
    echo "=========================================="
    echo "  WebView2 SDK Header"
    echo "=========================================="
    head -100 "$HEADER_FILE"
    echo ""
    echo "... (truncated, full header at: $HEADER_FILE)"
    echo ""
fi

echo ""
echo "=========================================="
echo "  Integration Status"
echo "=========================================="
echo "✅ SDK downloaded and verified"
echo "✅ Project structure prepared"
echo "⏳ Need to update code with SDK API"
echo ""
echo "Follow: docs/WEBVIEW2_INTEGRATION_CHECKLIST.md"
echo "Start at: Phase 2, Step 2.1"
echo ""
