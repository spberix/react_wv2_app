# WebView2 SDK Download Instructions

## Step 1: Access Azure DevOps

Open this URL in your browser:
```
https://dev.azure.com/microsoft/Edge/_artifacts/feed/webview2-mac-stable/UPack/webview2_sdk/overview/0.3800.64
```

You will be prompted to authenticate with your Microsoft credentials.

## Step 2: Download Package

1. Click the "Download" button on the artifact page
2. The file will likely be named something like:
   - `webview2_sdk.0.3800.64.zip`
   - `webview2_sdk-0.3800.64.tar.gz`
   - Or similar variant

3. Save to your Downloads folder

## Step 3: Extract Here

Once downloaded, extract the package to THIS directory:
```bash
cd /Users/elgasano/Sources/TeamsBICSandbox/external/webview2-macos

# If ZIP file:
unzip ~/Downloads/webview2_sdk*.zip

# If tar.gz file:
tar -xzf ~/Downloads/webview2_sdk*.tar.gz

# The contents should extract directly into this directory
```

## Step 4: Verify Structure

After extraction, run the verification script:
```bash
cd /Users/elgasano/Sources/TeamsBICSandbox
./verify_webview2_sdk.sh
```

## Expected Structure

After extraction, this directory should contain:
```
webview2-macos/
├── include/
│   ├── WebView2.h (or similar header files)
│   └── WebView2Environment.h
├── lib/
│   ├── WebView2.framework/ OR
│   └── libWebView2.dylib
├── docs/ (optional)
│   └── API documentation
└── samples/ (optional)
    └── Example code
```

## If Download Fails

If you cannot access the Azure DevOps link:
1. Verify you're logged into Microsoft with your work account
2. Check you have access to the Edge project
3. Try accessing the main feed first: https://dev.azure.com/microsoft/Edge/_artifacts
4. Contact Edge WebView2 team if access issues persist

## Next Steps

Once SDK is downloaded and extracted:
1. Run verification script: `./verify_webview2_sdk.sh`
2. Examine SDK structure and headers
3. Continue with integration following docs/WEBVIEW2_INTEGRATION_CHECKLIST.md
