#ifndef BINDINGS_H
#define BINDINGS_H

#include <string>
#include <functional>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <iostream>
// #include "webview/webview.h"  // Removed - using WebView2 now

namespace bindings {

// Window control functions exposed to JavaScript

// Minimize the window
inline std::string minimize(const std::string& /*args*/) {
#ifdef _WIN32
    // Windows-specific minimize
    // Note: webview library doesn't expose native window handle directly
    // This is a limitation - would need platform-specific code to access HWND
    return R"({"status": "not_implemented", "message": "Minimize not available on Windows"})";
#elif __APPLE__
    // macOS-specific minimize - would need Objective-C code
    return R"({"status": "not_implemented", "message": "Minimize requires native code"})";
#else
    // Linux GTK
    return R"({"status": "not_implemented", "message": "Minimize not available on Linux"})";
#endif
}

// Maximize or restore the window
inline std::string maximize(const std::string& /*args*/) {
#ifdef _WIN32
    return R"({"status": "not_implemented", "message": "Maximize not available on Windows"})";
#elif __APPLE__
    return R"({"status": "not_implemented", "message": "Maximize requires native code"})";
#else
    return R"({"status": "not_implemented", "message": "Maximize not available on Linux"})";
#endif
}

// Close the application
inline std::string closeApp(const std::string& /*args*/) {
    // This will be handled by terminating the webview
    return R"({"status": "ok"})";
}

// Check if window is maximized
inline std::string isMaximized(const std::string& /*args*/) {
    // Default implementation - returns false
    return R"({"maximized": false})";
}

// Example: Get platform information
inline std::string getPlatform(const std::string& /*args*/) {
#ifdef _WIN32
    return R"({"platform": "windows"})";
#elif __APPLE__
    return R"({"platform": "macos"})";
#else
    return R"({"platform": "linux"})";
#endif
}

// Demo: Echo function that reverses a string
inline std::string reverseString(const std::string& args) {
    // DEBUG: Print what we receive
    std::cout << "reverseString received: " << args << std::endl;

    // webview passes args as JSON array: ["Hello"]
    // We need to extract the first element
    std::string text = args;

    // Remove array brackets: ["Hello"] -> "Hello"
    if (text.length() >= 2 && text.front() == '[' && text.back() == ']') {
        text = text.substr(1, text.length() - 2);
    }

    // Remove quotes: "Hello" -> Hello
    if (text.length() >= 2 && text.front() == '"' && text.back() == '"') {
        text = text.substr(1, text.length() - 2);
    }

    std::cout << "Extracted text: " << text << std::endl;

    // Reverse the string
    std::reverse(text.begin(), text.end());

    std::cout << "Reversed text: " << text << std::endl;

    // Return as JSON
    return R"({"reversed": ")" + text + R"("})";
}

// Demo: Generate random number
inline std::string getRandomNumber(const std::string& args) {
    std::cout << "getRandomNumber called with args: " << args << std::endl;

    // Seed the random number generator with current time (only first call matters)
    static bool seeded = false;
    if (!seeded) {
        srand(static_cast<unsigned int>(time(nullptr)));
        seeded = true;
        std::cout << "Random number generator seeded" << std::endl;
    }

    int random = rand() % 100;
    std::cout << "Generated random number: " << random << std::endl;

    std::string result = R"({"number": )" + std::to_string(random) + R"(})";
    std::cout << "Returning: " << result << std::endl;
    return result;
}

} // namespace bindings

// Video bindings namespace
// These will be implemented in app.cpp with access to VideoGridManager
namespace video_bindings {

// Add participant with video file
// Args: {"name": "John Doe", "videoPath": "/path/to/video.mp4"}
// Returns: {"success": true, "participantId": 1}
std::string addParticipant(const std::string& args);

// Add web app participant
// Args: {"name": "Web App", "url": "https://example.com"}
// Returns: {"success": true, "participantId": 2}
std::string addWebApp(const std::string& args);

// Remove participant by ID
// Args: {"participantId": 1}
// Returns: {"success": true}
std::string removeParticipant(const std::string& args);

// Get current grid layout (called on initial load)
// Returns: {"participants": [{"id": 1, "name": "John", "position": {...}}, ...]}
std::string getGridLayout(const std::string& args);

// Open file picker to select video
// Returns: {"success": true, "path": "/selected/video.mp4"}
std::string selectVideoFile(const std::string& args);

} // namespace video_bindings

#endif // BINDINGS_H
