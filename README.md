# Teams BIC Sandbox

A cross-platform native application built with C++ and React, demonstrating seamless integration between native code and modern web technologies using the webview library.

## Overview

This project showcases a modern desktop application architecture that combines:
- **C++ Backend**: Native application logic with cross-platform window management
- **React Frontend**: Modern UI built with TypeScript and React
- **WebView Integration**: Platform-native webview components (WKWebView on macOS, WebView2 on Windows, WebKitGTK on Linux)
- **Bidirectional Communication**: TypeScript-safe bridge for C++ ↔ JavaScript communication

## Features

- ✨ Cross-platform support (macOS, Windows, Linux)
- ⚛️ Modern React UI with TypeScript
- 🔄 Bidirectional C++/JavaScript bridge
- 🎨 Custom window controls (minimize, maximize, close)
- 📦 Single executable with embedded UI
- 🚀 Native performance with web technologies

## Technology Stack

### Frontend
- **React 18** - UI framework
- **TypeScript** - Type-safe JavaScript
- **Webpack 5** - Module bundler
- **CSS3** - Styling

### Backend
- **C++17** - Application logic
- **webview/webview** - Cross-platform webview library
- **CMake** - Build system

### Platform-Specific WebViews
- **macOS**: WKWebView (Safari/WebKit engine)
- **Windows**: WebView2 (Chromium/Edge engine)
- **Linux**: WebKitGTK (WebKit engine)

## Prerequisites

### All Platforms
- **CMake 3.15+**
- **C++17 compatible compiler**
- **Node.js 18+** and npm

### macOS
- **Xcode Command Line Tools**
  ```bash
  xcode-select --install
  ```
- **macOS 10.13+** (for WKWebView support)

### Windows
- **Visual Studio 2019+** or **Build Tools for Visual Studio**
- **WebView2 Runtime** (pre-installed on Windows 11)
  - For Windows 10, download from: https://developer.microsoft.com/microsoft-edge/webview2/

### Linux (Ubuntu/Debian)
```bash
sudo apt update
sudo apt install build-essential cmake
sudo apt install libgtk-3-dev libwebkit2gtk-4.0-dev
```

### Linux (Fedora/RHEL)
```bash
sudo dnf install gcc-c++ cmake
sudo dnf install gtk3-devel webkit2gtk3-devel
```

## Project Structure

```
TeamsBICSandbox/
├── CMakeLists.txt              # CMake build configuration
├── src/                        # C++ source code
│   ├── main.cpp               # Application entry point
│   ├── app.h                  # Application class header
│   ├── app.cpp                # Application implementation
│   └── bindings.h             # C++ → JS function bindings
├── ui/                         # React application
│   ├── package.json           # npm dependencies
│   ├── webpack.config.js      # Webpack configuration
│   ├── tsconfig.json          # TypeScript configuration
│   ├── src/
│   │   ├── index.tsx         # React entry point
│   │   ├── App.tsx           # Main app component
│   │   ├── components/       # React components
│   │   ├── bridge/           # C++ ↔ JS bridge
│   │   └── styles/           # CSS styles
│   └── public/
│       └── index.html        # HTML template
├── vendor/                     # Third-party dependencies
│   └── webview/              # webview.h (auto-downloaded)
└── build/                      # Build output (gitignored)
```

## Building the Application

### Step 1: Install Node.js Dependencies

```bash
cd ui
npm install
cd ..
```

### Step 2: Configure with CMake

```bash
# Create build directory
mkdir build
cd build

# Configure (downloads webview.h automatically)
cmake ..

# On Windows with Visual Studio, you might need:
cmake .. -G "Visual Studio 17 2022"
```

### Step 3: Build

```bash
# Build everything (C++ app + React UI)
cmake --build .

# Or for multi-config generators (Visual Studio, Xcode):
cmake --build . --config Release
```

The build process will:
1. Download `webview.h` if not present
2. Run `npm install` in the `ui/` directory
3. Build the React app with Webpack
4. Compile the C++ application
5. Copy UI files to the build directory

### Step 4: Run

```bash
# From the build directory
./TeamsBICSandbox          # macOS/Linux
.\Debug\TeamsBICSandbox.exe   # Windows (Debug)
.\Release\TeamsBICSandbox.exe # Windows (Release)
```

## Development Workflow

### Frontend Development (React)

For faster React development with hot reload:

```bash
cd ui
npm start
```

This starts webpack-dev-server at http://localhost:3000 with live reloading.

### Backend Development (C++)

After modifying C++ code:

```bash
cd build
cmake --build .
```

### Full Rebuild

```bash
cd build
cmake --build . --clean-first
```

## Communication Bridge

### C++ to JavaScript

Bind C++ functions in `app.cpp`:

```cpp
webview_->bind("myFunction", [](const std::string& args) -> std::string {
    // Process arguments (JSON string)
    // Return result as JSON string
    return R"({"status": "ok"})";
});
```

### JavaScript to C++

Call from TypeScript:

```typescript
const result = await window.myFunction();
const data = JSON.parse(result);
```

Type declarations in `ui/src/bridge/types.ts`:

```typescript
declare global {
  interface Window {
    myFunction: () => Promise<string>;
  }
}
```

## Available Scripts

### UI (in `ui/` directory)

- `npm start` - Start development server with hot reload
- `npm run build` - Build production bundle
- `npm run dev` - Build development bundle
- `npm run watch` - Watch mode for development

### CMake Build

- `cmake -B build` - Configure build
- `cmake --build build` - Build application
- `cmake --build build --clean-first` - Clean and rebuild

## Platform-Specific Notes

### macOS

- Uses WKWebView (Safari engine)
- WebKit inspector available (Enable in Safari → Develop → Show Web Inspector)
- Requires code signing for distribution

### Windows

- Uses WebView2 (Chromium/Edge engine)
- F12 opens Edge DevTools for debugging
- Requires WebView2 Runtime (included in Windows 11)

### Linux

- Uses WebKitGTK (WebKit engine)
- Inspector can be enabled via GTK environment variables:
  ```bash
  GTK_DEBUG=interactive ./TeamsBICSandbox
  ```

## Debugging

### JavaScript/TypeScript Debugging

- **macOS**: Safari Web Inspector
- **Windows**: Edge DevTools (F12)
- **Linux**: WebKit Inspector (`GTK_DEBUG=interactive`)

### C++ Debugging

Use your platform's debugger:

```bash
# GDB (Linux/macOS)
gdb ./TeamsBICSandbox

# LLDB (macOS)
lldb ./TeamsBICSandbox

# Visual Studio (Windows)
# Open the .sln file in build/ directory
```

## Known Limitations

1. **Window Controls**: Native window manipulation (minimize/maximize) requires platform-specific code not exposed by the webview library. Current implementation provides the UI hooks but requires native extensions for full functionality.

2. **Custom Title Bar**: Fully custom title bars (frameless windows) require additional platform-specific code.

3. **Debugging**: Web inspector availability varies by platform and requires specific configuration.

## Troubleshooting

### Build fails with "webview.h not found"

The CMake script should auto-download it. If it fails:
```bash
mkdir -p vendor/webview
curl -o vendor/webview/webview.h https://raw.githubusercontent.com/webview/webview/master/webview.h
```

### npm install fails

Ensure Node.js 18+ is installed:
```bash
node --version
npm --version
```

### WebView2 not found (Windows)

Download and install from:
https://developer.microsoft.com/microsoft-edge/webview2/

### GTK/WebKit errors (Linux)

Ensure development packages are installed:
```bash
sudo apt install libgtk-3-dev libwebkit2gtk-4.0-dev
```

## Contributing

This is a sandbox project for experimentation. Feel free to fork and modify for your needs.

## License

This project is provided as-is for educational and development purposes.

## Resources

- [webview/webview](https://github.com/webview/webview) - Cross-platform webview library
- [React Documentation](https://react.dev)
- [CMake Documentation](https://cmake.org/documentation/)
- [TypeScript Documentation](https://www.typescriptlang.org/docs/)
