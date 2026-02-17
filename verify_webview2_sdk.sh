#!/bin/bash

# WebView2 SDK Verification Script
# Run this after downloading and extracting the SDK

echo "=========================================="
echo "  WebView2 SDK Verification"
echo "=========================================="
echo ""

SDK_DIR="/Users/elgasano/Sources/TeamsBICSandbox/external/webview2-macos"

# Color codes
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# Check if SDK directory exists
if [ ! -d "$SDK_DIR" ]; then
    echo -e "${RED}✗ SDK directory not found: $SDK_DIR${NC}"
    exit 1
fi

echo "Checking SDK directory: $SDK_DIR"
echo ""

# Check for include directory
echo "1. Checking for include directory..."
if [ -d "$SDK_DIR/include" ]; then
    echo -e "${GREEN}✓ include/ directory found${NC}"
    echo "   Headers found:"
    ls -1 "$SDK_DIR/include/" | while read file; do
        echo "   - $file"
    done

    # Check for specific headers
    if [ -f "$SDK_DIR/include/WebView2.h" ] || ls "$SDK_DIR/include/"*WebView2*.h 2>/dev/null; then
        echo -e "${GREEN}✓ WebView2 header files found${NC}"
    else
        echo -e "${YELLOW}⚠ WebView2.h not found - check for alternate header names${NC}"
    fi
else
    echo -e "${RED}✗ include/ directory not found${NC}"
fi
echo ""

# Check for lib directory
echo "2. Checking for lib directory..."
if [ -d "$SDK_DIR/lib" ]; then
    echo -e "${GREEN}✓ lib/ directory found${NC}"
    echo "   Libraries found:"
    ls -1 "$SDK_DIR/lib/" | while read file; do
        echo "   - $file"

        # Check architecture if it's a dylib or framework
        if [[ $file == *.dylib ]]; then
            echo "     Architecture: $(file "$SDK_DIR/lib/$file")"
        elif [[ $file == *.framework ]]; then
            if [ -f "$SDK_DIR/lib/$file/WebView2" ]; then
                echo "     Architecture: $(file "$SDK_DIR/lib/$file/WebView2")"
            fi
        fi
    done

    # Check for specific library
    if [ -d "$SDK_DIR/lib/WebView2.framework" ] || [ -f "$SDK_DIR/lib/libWebView2.dylib" ]; then
        echo -e "${GREEN}✓ WebView2 library found${NC}"
    else
        echo -e "${YELLOW}⚠ WebView2 library not found - check for alternate library names${NC}"
    fi
else
    echo -e "${RED}✗ lib/ directory not found${NC}"
fi
echo ""

# Check for documentation
echo "3. Checking for documentation..."
if [ -d "$SDK_DIR/docs" ] || [ -f "$SDK_DIR/README.md" ]; then
    echo -e "${GREEN}✓ Documentation found${NC}"
    [ -f "$SDK_DIR/README.md" ] && echo "   - README.md"
    [ -d "$SDK_DIR/docs" ] && echo "   - docs/ directory"
else
    echo -e "${YELLOW}⚠ No documentation found${NC}"
fi
echo ""

# Check for samples
echo "4. Checking for samples..."
if [ -d "$SDK_DIR/samples" ] || [ -d "$SDK_DIR/examples" ]; then
    echo -e "${GREEN}✓ Samples found${NC}"
    [ -d "$SDK_DIR/samples" ] && echo "   - samples/ directory"
    [ -d "$SDK_DIR/examples" ] && echo "   - examples/ directory"
else
    echo -e "${YELLOW}⚠ No samples found${NC}"
fi
echo ""

# Generate full structure report
echo "5. Full SDK structure:"
echo "========================================"
tree -L 3 "$SDK_DIR" 2>/dev/null || find "$SDK_DIR" -maxdepth 3 -print | sed 's|[^/]*/| |g'
echo ""

# Save structure to file
STRUCTURE_FILE="/Users/elgasano/Sources/TeamsBICSandbox/docs/webview2_sdk_structure.txt"
echo "Saving full structure to: $STRUCTURE_FILE"
tree "$SDK_DIR" > "$STRUCTURE_FILE" 2>/dev/null || find "$SDK_DIR" -print > "$STRUCTURE_FILE"
echo -e "${GREEN}✓ Structure saved${NC}"
echo ""

# Check if main headers can be read
echo "6. Examining main header file..."
HEADER_FILE=$(find "$SDK_DIR/include" -name "*WebView2*.h" -o -name "*webview2*.h" 2>/dev/null | head -1)
if [ -n "$HEADER_FILE" ]; then
    echo -e "${GREEN}✓ Found header: $(basename $HEADER_FILE)${NC}"
    echo ""
    echo "First 50 lines of header:"
    echo "========================================"
    head -50 "$HEADER_FILE"
    echo "========================================"
    echo ""

    # Save header preview
    HEADER_PREVIEW="/Users/elgasano/Sources/TeamsBICSandbox/docs/webview2_header_preview.txt"
    head -100 "$HEADER_FILE" > "$HEADER_PREVIEW"
    echo "Full header preview saved to: $HEADER_PREVIEW"
else
    echo -e "${RED}✗ Could not find main WebView2 header${NC}"
fi
echo ""

# Summary and next steps
echo "=========================================="
echo "  Verification Summary"
echo "=========================================="

PASS_COUNT=0
FAIL_COUNT=0

[ -d "$SDK_DIR/include" ] && ((PASS_COUNT++)) || ((FAIL_COUNT++))
[ -d "$SDK_DIR/lib" ] && ((PASS_COUNT++)) || ((FAIL_COUNT++))

echo -e "Checks passed: ${GREEN}$PASS_COUNT${NC}"
echo -e "Checks failed: ${RED}$FAIL_COUNT${NC}"
echo ""

if [ $FAIL_COUNT -eq 0 ]; then
    echo -e "${GREEN}✓ SDK appears to be properly installed!${NC}"
    echo ""
    echo "Next steps:"
    echo "1. Review SDK structure: docs/webview2_sdk_structure.txt"
    echo "2. Review header file: docs/webview2_header_preview.txt"
    echo "3. Update webview2_wrapper_macos.mm with SDK API"
    echo "4. Build minimal test: cd examples && clang++ ..."
    echo "5. Follow docs/WEBVIEW2_INTEGRATION_CHECKLIST.md"
    echo ""
    echo "Ready to proceed with integration!"
else
    echo -e "${RED}✗ SDK installation incomplete${NC}"
    echo ""
    echo "Please check:"
    echo "1. SDK was extracted to correct directory"
    echo "2. All files were extracted (not just a subdirectory)"
    echo "3. Review external/webview2-macos/DOWNLOAD_INSTRUCTIONS.md"
fi

echo ""
echo "=========================================="
