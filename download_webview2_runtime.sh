#!/bin/bash

# WebView2 Runtime Download Script for macOS
# Version: 145.0.3800.64 (matches current SDK version)

set -e

RUNTIME_VERSION="145.0.3800.64"
INSTALL_DIR="$HOME/Library/Application Support/Microsoft WebView2"
TEMP_DIR="/tmp/webview2_runtime_download"

echo "========================================"
echo "  WebView2 Runtime Installation"
echo "========================================"
echo ""
echo "Runtime Version: $RUNTIME_VERSION"
echo "Install Location: $INSTALL_DIR"
echo ""

# Create temp directory
mkdir -p "$TEMP_DIR"
cd "$TEMP_DIR"

echo "MANUAL DOWNLOAD REQUIRED"
echo "========================"
echo ""
echo "The WebView2 runtime must be downloaded from Azure DevOps."
echo ""
echo "Step 1: Open this URL in your browser:"
echo "https://dev.azure.com/microsoft/Edge/_artifacts/feed/webview2-mac-stable/UPack/webview2_runtime/overview/$RUNTIME_VERSION"
echo ""
echo "  (Alternative feed to try if above doesn't work:)"
echo "  https://dev.azure.com/microsoft/Edge/_artifacts/feed/webview2-mac/UPack/webview2_runtime/overview/$RUNTIME_VERSION"
echo ""
echo "Step 2: Click the 'Download' button"
echo ""
echo "Step 3: Save the downloaded file to:"
echo "  $TEMP_DIR"
echo ""
echo "Step 4: The file will likely be named:"
echo "  - webview2_runtime.$RUNTIME_VERSION.zip"
echo "  - Microsoft Edge WebView2.app.zip"
echo "  - Or similar variant"
echo ""
echo "========================================="
echo ""
read -p "Press Enter once you've downloaded the runtime file to $TEMP_DIR... "
echo ""

# Look for downloaded file
RUNTIME_FILE=""
for pattern in "webview2_runtime*.zip" "Microsoft*WebView2*.zip" "*.zip"; do
    FILES=($pattern)
    if [ -f "${FILES[0]}" ]; then
        RUNTIME_FILE="${FILES[0]}"
        break
    fi
done

if [ -z "$RUNTIME_FILE" ]; then
    echo "ERROR: No runtime file found in $TEMP_DIR"
    echo "Please download the runtime and save it to this directory."
    exit 1
fi

echo "Found runtime file: $RUNTIME_FILE"
echo ""

# Extract runtime
echo "Extracting runtime..."
unzip -q "$RUNTIME_FILE"

# Find the .app bundle
APP_BUNDLE=$(find . -name "Microsoft Edge WebView2.app" -o -name "Microsoft*WebView*.app" | head -1)

if [ -z "$APP_BUNDLE" ]; then
    echo "ERROR: Could not find WebView2.app bundle after extraction"
    echo "Contents of temp directory:"
    ls -la
    exit 1
fi

echo "Found runtime bundle: $APP_BUNDLE"
echo ""

# Create install directory
echo "Creating install directory..."
mkdir -p "$INSTALL_DIR"

# Copy runtime to install location
echo "Installing runtime to: $INSTALL_DIR"
cp -R "$APP_BUNDLE" "$INSTALL_DIR/"

# Verify installation
INSTALLED_APP="$INSTALL_DIR/Microsoft Edge WebView2.app"
if [ -d "$INSTALLED_APP" ]; then
    echo ""
    echo "✅ Runtime installed successfully!"
    echo ""
    echo "Installed at: $INSTALLED_APP"

    # Check version
    VERSION=$(plutil -p "$INSTALLED_APP/Contents/Info.plist" | grep CFBundleShortVersionString | awk '{print $3}' | tr -d '"')
    echo "Version: $VERSION"

    # Verify library exists
    DYLIB_PATH="$INSTALLED_APP/Contents/Frameworks/Microsoft Edge Framework.framework/Libraries/libEmbeddedBrowserWebView.dylib"
    if [ -f "$DYLIB_PATH" ]; then
        echo "✅ Runtime library verified"
    else
        echo "⚠️  Warning: Runtime library not found at expected location"
        echo "   Expected: $DYLIB_PATH"
    fi
else
    echo "❌ Installation failed"
    exit 1
fi

# Cleanup
echo ""
echo "Cleaning up temporary files..."
cd ~
rm -rf "$TEMP_DIR"

echo ""
echo "========================================"
echo "  Installation Complete!"
echo "========================================"
echo ""
echo "Next steps:"
echo "1. Run the test: ./test_webview2"
echo "2. Verify WebView2 initializes without version mismatch errors"
echo ""
