#include "app.h"
#include <iostream>

#ifdef _WIN32
#include <windows.h>

// Windows entry point
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    (void)hInstance;
    (void)hPrevInstance;
    (void)lpCmdLine;
    (void)nCmdShow;

    // Redirect stdout/stderr to console if launched from console
    if (AttachConsole(ATTACH_PARENT_PROCESS)) {
        freopen("CONOUT$", "w", stdout);
        freopen("CONOUT$", "w", stderr);
    }

    // Enable debug mode (true = enable developer tools)
    Application app("Teams BIC Sandbox", 1200, 800, true);

    if (!app.initialize()) {
        std::cerr << "Failed to initialize application" << std::endl;
        return 1;
    }

    std::cout << "Starting Teams BIC Sandbox..." << std::endl;
    app.run();

    return 0;
}
#endif

// Cross-platform main entry point
int main(int argc, char* argv[]) {
    std::cout << "========================================" << std::endl;
    std::cout << "  Teams BIC Sandbox" << std::endl;
    std::cout << "  C++ WebView + React Application" << std::endl;
    std::cout << "========================================" << std::endl;

    // Check for command-line arguments
    bool useLocalhost = false;
    if (argc > 1) {
        std::string arg = argv[1];
        if (arg == "--localhost" || arg == "-l") {
            useLocalhost = true;
            std::cout << "Development mode: Loading from localhost:3000" << std::endl;
        } else if (arg == "--help" || arg == "-h") {
            std::cout << "\nUsage: " << argv[0] << " [OPTIONS]" << std::endl;
            std::cout << "\nOptions:" << std::endl;
            std::cout << "  --localhost, -l    Load UI from http://localhost:3000 (dev mode)" << std::endl;
            std::cout << "  --help, -h         Show this help message" << std::endl;
            std::cout << "\nDefault: Load from file system (ui/index.html)" << std::endl;
            return 0;
        }
    }

    // Enable debug mode (true = enable developer tools)
    Application app("Teams BIC Sandbox", 1200, 800, true, useLocalhost);

    if (!app.initialize()) {
        std::cerr << "Failed to initialize application" << std::endl;
        return 1;
    }

    std::cout << "Starting application..." << std::endl;
    std::cout << "Close the window or press Ctrl+C to exit" << std::endl;

    app.run();

    std::cout << "Application closed" << std::endl;
    return 0;
}
