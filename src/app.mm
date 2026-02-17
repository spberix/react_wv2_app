#include "app.h"
#include "bindings.h"
#include "video/video_grid_manager.h"
#include "window/window_manager.h"
#include "video/drag_overlay_view.h"
#include "platform_helper.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <climits>
#include <cstdlib>
#include <vector>

#ifdef __APPLE__
#import <Cocoa/Cocoa.h>
#endif

// Global pointer to video grid manager for bindings
static VideoGridManager* g_videoGridManager = nullptr;

// Pre-generated test video directory
static const std::string VIDEO_POOL_DIR = "/tmp/video_grid_test";

Application::Application(const std::string& title, int width, int height, bool debug, bool useLocalhost)
    : title_(title), width_(width), height_(height), debug_(debug), useLocalhost_(useLocalhost), initialized_(false) {
}

Application::~Application() {
    if (webview_) {
        webview_->close();
    }
}

bool Application::initialize() {
    try {
        // Initialize WebView2 environment
        std::cout << "Initializing WebView2 environment..." << std::endl;
        if (!webview2::WebView2Manager::getInstance().initialize()) {
            std::cerr << "Failed to initialize WebView2 environment" << std::endl;
            return false;
        }

#ifdef __APPLE__
        // Create native NSWindow on macOS
        @autoreleasepool {
            NSRect frame = NSMakeRect(100, 100, width_, height_);
            NSWindow* window = [[NSWindow alloc]
                initWithContentRect:frame
                          styleMask:(NSWindowStyleMaskTitled |
                                    NSWindowStyleMaskClosable |
                                    NSWindowStyleMaskMiniaturizable |
                                    NSWindowStyleMaskResizable)
                            backing:NSBackingStoreBuffered
                              defer:NO];

            [window setTitle:[NSString stringWithUTF8String:title_.c_str()]];
            [window center];

            nativeWindow_ = (__bridge_retained void*)window;
        }
#endif

        // Create WebView2 instance for this window
        std::cout << "Creating WebView2 instance..." << std::endl;
        webview_ = webview2::WebView2Manager::getInstance().createWebView(nativeWindow_, debug_);

        if (!webview_) {
            std::cerr << "Failed to create WebView2 instance" << std::endl;
            return false;
        }

        // Initialize video layer (must be before loading UI)
        if (!initializeVideoLayer()) {
            std::cerr << "Warning: Failed to initialize video layer" << std::endl;
        }

        // Set up bindings (including video bindings)
        setupBindings();
        setupVideoBindings();

        // Load UI
        loadUI();

        // Configure WebView transparency AFTER UI is loaded
        configureWebViewTransparency();

#ifdef __APPLE__
        // Show window
        @autoreleasepool {
            NSWindow* window = (__bridge NSWindow*)nativeWindow_;
            [window makeKeyAndOrderFront:nil];
        }
#endif

        initialized_ = true;
        return true;

    } catch (const std::exception& e) {
        std::cerr << "Exception during initialization: " << e.what() << std::endl;
        return false;
    }
}

void Application::run() {
    if (!initialized_) {
        std::cerr << "Application not initialized" << std::endl;
        return;
    }

    std::cout << "Starting application event loop..." << std::endl;

    // Apply transparency immediately
    if (webview_) {
        webview_->configureTransparency();
    }

    // Apply transparency again after a delay to catch views added by WebView2
    #ifdef __APPLE__
    dispatch_after(dispatch_time(DISPATCH_TIME_NOW, (int64_t)(1.0 * NSEC_PER_SEC)),
                   dispatch_get_main_queue(), ^{
        std::cout << "Applying delayed transparency configuration..." << std::endl;
        if (webview_) {
            webview_->configureTransparency();
        }
    });
    #endif

    // Inject CSS to ensure HTML/body are transparent
    webview_->executeScript(R"(
        (function() {
            const style = document.createElement('style');
            style.textContent = 'html, body { background: transparent !important; }';
            document.head.appendChild(style);
            console.log('Transparency CSS injected');
        })();
    )");

    try {
#ifdef __APPLE__
        // Run native macOS event loop
        @autoreleasepool {
            [NSApp run];
        }
#elif _WIN32
        // Windows message loop would go here
        MSG msg;
        while (GetMessage(&msg, NULL, 0, 0)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
#endif
    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }
}

void Application::terminate() {
    if (webview_) {
        webview_->close();
    }

#ifdef __APPLE__
    @autoreleasepool {
        [NSApp terminate:nil];
    }
#endif
}

void Application::loadUI() {
    // Check if we should load from localhost (development mode)
    if (useLocalhost_) {
        std::string localhostUrl = "http://localhost:3000";
        webview_->navigate(localhostUrl);
        std::cout << "Loading UI from: " << localhostUrl << std::endl;
        std::cout << "Make sure webpack dev server is running: cd ui && npm start" << std::endl;
        return;
    }

    // Load from file system (production mode)
    // Try multiple possible paths for the UI
    std::string uiPath;
    std::vector<std::string> possiblePaths = {
        "ui/dist/index.html",  // Running from project root
        "ui/index.html",       // Copied to build/ui
        "../ui/dist/index.html" // Running from build directory
    };

    std::ifstream uiFile;
    for (const auto& path : possiblePaths) {
        uiFile.open(path);
        if (uiFile.good()) {
            uiPath = path;
            break;
        }
        uiFile.close();
    }
    if (uiFile.good()) {
        uiFile.close();

        // Get absolute path
        char absolutePath[PATH_MAX];
        if (realpath(uiPath.c_str(), absolutePath) != nullptr) {
            // Construct proper file:// URL with three slashes
            std::string fileUrl = std::string("file://") + absolutePath;
            webview_->navigate(fileUrl);
            std::cout << "Loading UI from: " << fileUrl << std::endl;
        } else {
            std::cerr << "Failed to resolve absolute path for: " << uiPath << std::endl;
            // Fall back to using setHtml
            std::ifstream file(uiPath);
            std::stringstream buffer;
            buffer << file.rdbuf();
            webview_->setHtml(buffer.str());
        }
    } else {
        // Fallback to inline HTML with a loading message
        std::string fallbackHtml = R"html(
<!DOCTYPE html>
<html>
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Teams BIC Sandbox</title>
    <style>
        body {
            margin: 0;
            padding: 0;
            font-family: -apple-system, BlinkMacSystemFont, 'Segoe UI', Roboto, Oxygen, Ubuntu, Cantarell, sans-serif;
            background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
            display: flex;
            justify-content: center;
            align-items: center;
            height: 100vh;
            color: white;
        }
        .container {
            text-align: center;
            padding: 40px;
            background: rgba(255, 255, 255, 0.1);
            border-radius: 20px;
            backdrop-filter: blur(10px);
        }
        h1 {
            margin: 0 0 20px 0;
            font-size: 2.5em;
        }
        p {
            font-size: 1.2em;
            margin: 10px 0;
        }
        .platform {
            margin-top: 20px;
            padding: 10px 20px;
            background: rgba(255, 255, 255, 0.2);
            border-radius: 10px;
            display: inline-block;
        }
        button {
            margin: 10px;
            padding: 12px 24px;
            font-size: 1em;
            border: none;
            border-radius: 8px;
            background: white;
            color: #667eea;
            cursor: pointer;
            font-weight: 600;
            transition: transform 0.2s, box-shadow 0.2s;
        }
        button:hover {
            transform: translateY(-2px);
            box-shadow: 0 4px 12px rgba(0, 0, 0, 0.2);
        }
        button:active {
            transform: translateY(0);
        }
    </style>
</head>
<body>
    <div class="container">
        <h1>🚀 Teams BIC Sandbox</h1>
        <p>C++ WebView + React Application</p>
        <div class="platform" id="platform">Loading platform...</div>
        <div style="margin-top: 30px;">
            <button onclick="testBridge()">Test Bridge</button>
            <button onclick="closeApp()">Close App</button>
        </div>
        <p style="font-size: 0.9em; margin-top: 20px; opacity: 0.8;">
            UI files not found. Build the React app first.
        </p>
    </div>
    <script>
        // Test the bridge
        async function testBridge() {
            try {
                const result = await window.getPlatform();
                const data = JSON.parse(result);
                document.getElementById('platform').textContent = 'Platform: ' + data.platform;
                alert('Bridge is working! Platform: ' + data.platform);
            } catch (error) {
                alert('Error: ' + error.message);
            }
        }

        async function closeApp() {
            try {
                await window.closeApp();
            } catch (error) {
                console.error('Error closing app:', error);
            }
        }

        // Load platform on startup
        testBridge();
    </script>
</body>
</html>
        )html";

        webview_->setHtml(fallbackHtml);
        std::cout << "Loading fallback UI (React app not built yet)" << std::endl;
    }
}

void Application::setupBindings() {
    if (!webview_) {
        std::cerr << "Cannot setup bindings - webview not initialized" << std::endl;
        return;
    }

    std::cout << "Setting up WebView2 bindings..." << std::endl;

    // Register message handlers for window control functions
    webview_->addScriptHandler("minimize", [](const std::string& data) {
        std::string result = bindings::minimize(data);
        std::cout << "minimize result: " << result << std::endl;
    });

    webview_->addScriptHandler("maximize", [](const std::string& data) {
        std::string result = bindings::maximize(data);
        std::cout << "maximize result: " << result << std::endl;
    });

    webview_->addScriptHandler("closeApp", [](const std::string& data) {
        std::string result = bindings::closeApp(data);
        std::cout << "closeApp result: " << result << std::endl;
        // Trigger app termination
        #ifdef __APPLE__
        dispatch_async(dispatch_get_main_queue(), ^{
            [NSApp terminate:nil];
        });
        #endif
    });

    webview_->addScriptHandler("isMaximized", [](const std::string& data) {
        std::string result = bindings::isMaximized(data);
        std::cout << "isMaximized result: " << result << std::endl;
    });

    webview_->addScriptHandler("getPlatform", [](const std::string& data) {
        std::string result = bindings::getPlatform(data);
        std::cout << "getPlatform result: " << result << std::endl;
    });

    // Inject JavaScript to create window.minimize(), etc. that wrap webview2_send
    // Use addInitScript so it runs on every page load (including React app)
    std::string bindingsScript = R"(
        (function() {
            // Create window functions that send messages to C++
            window.minimize = function() {
                window.webview2_send('minimize', {});
            };

            window.maximize = function() {
                window.webview2_send('maximize', {});
            };

            window.closeApp = function() {
                window.webview2_send('closeApp', {});
            };

            window.isMaximized = function() {
                window.webview2_send('isMaximized', {});
            };

            window.getPlatform = function() {
                window.webview2_send('getPlatform', {});
            };

            console.log('Window control bindings initialized');
        })();
    )";

    webview_->addInitScript(bindingsScript);
    std::cout << "WebView2 bindings setup complete" << std::endl;
}

bool Application::initializeVideoLayer() {
    // Create video grid manager
    videoGridManager_ = std::make_unique<VideoGridManager>();

    // Use the native window we already have
    void* nativeWindowHandle = nativeWindow_;

    if (!nativeWindowHandle) {
        std::cerr << "Failed to get native window handle" << std::endl;
        return false;
    }

    // Initialize video grid
    if (!videoGridManager_->initialize(nativeWindowHandle, width_, height_)) {
        std::cerr << "Failed to initialize video grid manager" << std::endl;
        return false;
    }

    // Set webview for push notifications
    videoGridManager_->setWebView(webview_.get());

    // Set global pointer for bindings
    g_videoGridManager = videoGridManager_.get();

    // Initialize window manager
    windowManager_ = std::make_unique<WindowManager>();
    if (!windowManager_->initialize(nativeWindowHandle, videoGridManager_.get())) {
        std::cerr << "Failed to initialize window manager" << std::endl;
        return false;
    }
    std::cout << "Window manager initialized" << std::endl;

    // Initialize drag overlay view
    dragOverlayView_ = std::make_unique<DragOverlayView>();
    if (!dragOverlayView_->initialize(nativeWindowHandle, windowManager_.get(),
                                     videoGridManager_.get(), webview_.get())) {
        std::cerr << "Failed to initialize drag overlay view" << std::endl;
        // Not a fatal error - continue without drag support
    } else {
        std::cout << "Drag overlay view initialized" << std::endl;
    }

    std::cout << "Video layer initialized" << std::endl;
    return true;
}

void Application::configureWebViewTransparency() {
    if (webview_) {
        webview_->configureTransparency();
    } else {
        std::cerr << "Failed to configure transparency - webview not initialized" << std::endl;
    }
}

void Application::setupVideoBindings() {
    if (!webview_) {
        std::cerr << "Cannot setup video bindings - webview not initialized" << std::endl;
        return;
    }

    std::cout << "Setting up WebView2 video bindings..." << std::endl;

    // Register message handlers for video functions
    // Each handler receives the message, calls the C++ function, and sends result back to JS

    webview_->addScriptHandler("videoAddParticipant", [this](const std::string& data) {
        std::cout << "videoAddParticipant handler called with: " << data << std::endl;

        // Parse the message to get requestId and args
        // Format: {"requestId":"req_3","args":"{\"name\":\"Participant 1\"}"}

        // Helper lambda to extract JSON string value, handling escaped quotes
        auto extractJsonValue = [](const std::string& json, const std::string& key) -> std::string {
            std::string searchKey = "\"" + key + "\":\"";
            size_t keyPos = json.find(searchKey);
            if (keyPos == std::string::npos) {
                return "";
            }

            size_t valueStart = keyPos + searchKey.length();
            size_t pos = valueStart;
            std::string result;

            // Parse until we find an unescaped quote
            while (pos < json.length()) {
                if (json[pos] == '\\' && pos + 1 < json.length()) {
                    // Escaped character - add both the backslash and next char
                    result += json[pos];
                    result += json[pos + 1];
                    pos += 2;
                } else if (json[pos] == '"') {
                    // Unescaped quote - end of value
                    break;
                } else {
                    result += json[pos];
                    pos++;
                }
            }

            return result;
        };

        std::string requestId = extractJsonValue(data, "requestId");
        std::string args = extractJsonValue(data, "args");

        std::cout << "Parsed requestId: " << requestId << std::endl;
        std::cout << "Parsed args: " << args << std::endl;

        // Call the C++ function
        std::string result = video_bindings::addParticipant(args);

        // Send result back to JavaScript - escape quotes in result
        std::string escapedResult = result;
        size_t pos = 0;
        while ((pos = escapedResult.find("\"", pos)) != std::string::npos) {
            escapedResult.replace(pos, 1, "\\\"");
            pos += 2;
        }

        std::string responseScript = "window.__webview2_resolveRequest('" + requestId + "', '" + escapedResult + "');";
        webview_->executeScript(responseScript);
    });

    webview_->addScriptHandler("videoAddWebApp", [this](const std::string& data) {
        std::cout << "videoAddWebApp handler called" << std::endl;

        auto extractJsonValue = [](const std::string& json, const std::string& key) -> std::string {
            std::string searchKey = "\"" + key + "\":\"";
            size_t keyPos = json.find(searchKey);
            if (keyPos == std::string::npos) return "";
            size_t valueStart = keyPos + searchKey.length();
            size_t pos = valueStart;
            std::string result;
            while (pos < json.length()) {
                if (json[pos] == '\\' && pos + 1 < json.length()) {
                    result += json[pos];
                    result += json[pos + 1];
                    pos += 2;
                } else if (json[pos] == '"') {
                    break;
                } else {
                    result += json[pos];
                    pos++;
                }
            }
            return result;
        };

        std::string requestId = extractJsonValue(data, "requestId");
        std::string args = extractJsonValue(data, "args");

        std::string result = video_bindings::addWebApp(args);

        std::string escapedResult = result;
        size_t pos = 0;
        while ((pos = escapedResult.find("\"", pos)) != std::string::npos) {
            escapedResult.replace(pos, 1, "\\\"");
            pos += 2;
        }

        std::string responseScript = "window.__webview2_resolveRequest('" + requestId + "', '" + escapedResult + "');";
        webview_->executeScript(responseScript);
    });

    webview_->addScriptHandler("videoRemoveParticipant", [this](const std::string& data) {
        std::cout << "videoRemoveParticipant handler called" << std::endl;

        auto extractJsonValue = [](const std::string& json, const std::string& key) -> std::string {
            std::string searchKey = "\"" + key + "\":\"";
            size_t keyPos = json.find(searchKey);
            if (keyPos == std::string::npos) return "";
            size_t valueStart = keyPos + searchKey.length();
            size_t pos = valueStart;
            std::string result;
            while (pos < json.length()) {
                if (json[pos] == '\\' && pos + 1 < json.length()) {
                    result += json[pos];
                    result += json[pos + 1];
                    pos += 2;
                } else if (json[pos] == '"') {
                    break;
                } else {
                    result += json[pos];
                    pos++;
                }
            }
            return result;
        };

        std::string requestId = extractJsonValue(data, "requestId");
        std::string args = extractJsonValue(data, "args");

        std::string result = video_bindings::removeParticipant(args);

        std::string escapedResult = result;
        size_t pos = 0;
        while ((pos = escapedResult.find("\"", pos)) != std::string::npos) {
            escapedResult.replace(pos, 1, "\\\"");
            pos += 2;
        }

        std::string responseScript = "window.__webview2_resolveRequest('" + requestId + "', '" + escapedResult + "');";
        webview_->executeScript(responseScript);
    });

    webview_->addScriptHandler("videoGetGridLayout", [this](const std::string& data) {
        std::cout << "videoGetGridLayout handler called" << std::endl;

        auto extractJsonValue = [](const std::string& json, const std::string& key) -> std::string {
            std::string searchKey = "\"" + key + "\":\"";
            size_t keyPos = json.find(searchKey);
            if (keyPos == std::string::npos) return "";
            size_t valueStart = keyPos + searchKey.length();
            size_t pos = valueStart;
            std::string result;
            while (pos < json.length()) {
                if (json[pos] == '\\' && pos + 1 < json.length()) {
                    result += json[pos];
                    result += json[pos + 1];
                    pos += 2;
                } else if (json[pos] == '"') {
                    break;
                } else {
                    result += json[pos];
                    pos++;
                }
            }
            return result;
        };

        std::string requestId = extractJsonValue(data, "requestId");
        std::string args = extractJsonValue(data, "args");

        std::string result = video_bindings::getGridLayout(args);

        std::string escapedResult = result;
        size_t pos = 0;
        while ((pos = escapedResult.find("\"", pos)) != std::string::npos) {
            escapedResult.replace(pos, 1, "\\\"");
            pos += 2;
        }

        std::string responseScript = "window.__webview2_resolveRequest('" + requestId + "', '" + escapedResult + "');";
        webview_->executeScript(responseScript);
    });

    webview_->addScriptHandler("videoSelectFile", [this](const std::string& data) {
        std::cout << "videoSelectFile handler called" << std::endl;

        auto extractJsonValue = [](const std::string& json, const std::string& key) -> std::string {
            std::string searchKey = "\"" + key + "\":\"";
            size_t keyPos = json.find(searchKey);
            if (keyPos == std::string::npos) return "";
            size_t valueStart = keyPos + searchKey.length();
            size_t pos = valueStart;
            std::string result;
            while (pos < json.length()) {
                if (json[pos] == '\\' && pos + 1 < json.length()) {
                    result += json[pos];
                    result += json[pos + 1];
                    pos += 2;
                } else if (json[pos] == '"') {
                    break;
                } else {
                    result += json[pos];
                    pos++;
                }
            }
            return result;
        };

        std::string requestId = extractJsonValue(data, "requestId");
        std::string args = extractJsonValue(data, "args");

        std::string result = video_bindings::selectVideoFile(args);

        std::string escapedResult = result;
        size_t pos = 0;
        while ((pos = escapedResult.find("\"", pos)) != std::string::npos) {
            escapedResult.replace(pos, 1, "\\\"");
            pos += 2;
        }

        std::string responseScript = "window.__webview2_resolveRequest('" + requestId + "', '" + escapedResult + "');";
        webview_->executeScript(responseScript);
    });

    // Inject JavaScript to create promise-based window.videoAddParticipant() etc.
    // Use addInitScript so it runs on every page load (including React app)
    std::string videoBindingsScript = R"(
        (function() {
            // Request/response management
            window.__webview2_pendingRequests = {};
            window.__webview2_requestCounter = 0;

            window.__webview2_resolveRequest = function(requestId, result) {
                console.log('Resolving request:', requestId, 'with result:', result);
                const pending = window.__webview2_pendingRequests[requestId];
                if (pending) {
                    pending.resolve(result);
                    delete window.__webview2_pendingRequests[requestId];
                }
            };

            // Helper to create async request function
            function createRequestFunction(handlerName) {
                return function(args) {
                    return new Promise((resolve, reject) => {
                        const requestId = 'req_' + (++window.__webview2_requestCounter);
                        window.__webview2_pendingRequests[requestId] = { resolve, reject };

                        // Send message to C++
                        window.webview2_send(handlerName, {
                            requestId: requestId,
                            args: args
                        });

                        // Timeout after 30 seconds
                        setTimeout(() => {
                            if (window.__webview2_pendingRequests[requestId]) {
                                delete window.__webview2_pendingRequests[requestId];
                                reject(new Error('Request timeout'));
                            }
                        }, 30000);
                    });
                };
            }

            // Create video functions that return promises
            window.videoAddParticipant = createRequestFunction('videoAddParticipant');
            window.videoAddWebApp = createRequestFunction('videoAddWebApp');
            window.videoRemoveParticipant = createRequestFunction('videoRemoveParticipant');
            window.videoGetGridLayout = createRequestFunction('videoGetGridLayout');
            window.videoSelectFile = createRequestFunction('videoSelectFile');

            console.log('Video bindings initialized');
        })();
    )";

    webview_->addInitScript(videoBindingsScript);
    std::cout << "WebView2 video bindings setup complete" << std::endl;
}

// Video bindings implementation
namespace video_bindings {

// Helper function to extract JSON from webview array format: ["..."]
std::string extractJsonFromArray(const std::string& args) {
    // Webview passes args as array: ["actual json"]
    std::string json = args;

    // Remove array brackets if present
    if (json.length() >= 2 && json.front() == '[' && json.back() == ']') {
        json = json.substr(1, json.length() - 2);
    }

    // Remove quotes around the JSON string
    if (json.length() >= 2 && json.front() == '"' && json.back() == '"') {
        json = json.substr(1, json.length() - 2);
    }

    // Unescape internal quotes
    size_t pos = 0;
    while ((pos = json.find("\\\"", pos)) != std::string::npos) {
        json.replace(pos, 2, "\"");
        pos += 1;
    }

    return json;
}

// Helper function to parse JSON string value
std::string parseJsonString(const std::string& json, const std::string& key) {
    size_t keyPos = json.find("\"" + key + "\"");
    if (keyPos == std::string::npos) return "";

    size_t colonPos = json.find(":", keyPos);
    if (colonPos == std::string::npos) return "";

    size_t firstQuote = json.find("\"", colonPos);
    if (firstQuote == std::string::npos) return "";

    size_t secondQuote = json.find("\"", firstQuote + 1);
    if (secondQuote == std::string::npos) return "";

    return json.substr(firstQuote + 1, secondQuote - firstQuote - 1);
}

// Helper function to parse JSON int value
int parseJsonInt(const std::string& json, const std::string& key) {
    size_t keyPos = json.find("\"" + key + "\"");
    if (keyPos == std::string::npos) return -1;

    size_t colonPos = json.find(":", keyPos);
    if (colonPos == std::string::npos) return -1;

    size_t start = colonPos + 1;
    while (start < json.length() && std::isspace(json[start])) start++;

    size_t end = start;
    while (end < json.length() && std::isdigit(json[end])) end++;

    if (end > start) {
        return std::stoi(json.substr(start, end - start));
    }

    return -1;
}

std::string addParticipant(const std::string& args) {
    std::cout << "videoAddParticipant called with: " << args << std::endl;

    if (!g_videoGridManager) {
        return R"({"success": false, "error": "Video grid manager not initialized"})";
    }

    // Extract JSON from webview array format
    std::string json = extractJsonFromArray(args);
    std::cout << "Extracted JSON: " << json << std::endl;

    // Parse args: {"name": "John Doe"}
    std::string name = parseJsonString(json, "name");

    if (name.empty()) {
        return R"({"success": false, "error": "Missing name"})";
    }

    // Auto-select video from pre-generated pool
    int currentCount = g_videoGridManager->getParticipantCount();
    if (currentCount >= 9) {
        return R"({"success": false, "error": "Maximum participants reached"})";
    }

    // Build path to pre-generated video
    std::string videoPath = VIDEO_POOL_DIR + "/test_video_" + std::to_string(currentCount) + ".mp4";

    // Check if video exists
    std::ifstream videoFile(videoPath);
    if (!videoFile.good()) {
        std::cerr << "Video file not found: " << videoPath << std::endl;
        std::cerr << "Please run ./generate_test_videos.sh first" << std::endl;
        return R"({"success": false, "error": "Test videos not found. Run ./generate_test_videos.sh first"})";
    }
    videoFile.close();

    std::cout << "Using video: " << videoPath << std::endl;

    int participantId = g_videoGridManager->addParticipant(name, videoPath);

    if (participantId < 0) {
        return R"({"success": false, "error": "Failed to add participant"})";
    }

    return "{\"success\": true, \"participantId\": " + std::to_string(participantId) + "}";
}

std::string addWebApp(const std::string& args) {
    std::cout << "videoAddWebApp called with: " << args << std::endl;

    if (!g_videoGridManager) {
        return R"({"success": false, "error": "Video grid manager not initialized"})";
    }

    // Extract JSON from webview array format
    std::string json = extractJsonFromArray(args);
    std::cout << "Extracted JSON: " << json << std::endl;

    // Parse args: {"name": "Web App", "url": "https://example.com"}
    std::string name = parseJsonString(json, "name");
    std::string url = parseJsonString(json, "url");

    if (name.empty()) {
        return R"({"success": false, "error": "Missing name"})";
    }

    if (url.empty()) {
        return R"({"success": false, "error": "Missing url"})";
    }

    // Check capacity
    int currentCount = g_videoGridManager->getParticipantCount();
    if (currentCount >= 9) {
        return R"({"success": false, "error": "Maximum participants reached"})";
    }

    std::cout << "Adding web app: " << name << " (" << url << ")" << std::endl;

    int participantId = g_videoGridManager->addWebApp(name, url);

    if (participantId < 0) {
        return R"({"success": false, "error": "Failed to add web app"})";
    }

    return "{\"success\": true, \"participantId\": " + std::to_string(participantId) + "}";
}

std::string removeParticipant(const std::string& args) {
    std::cout << "videoRemoveParticipant called with: " << args << std::endl;

    if (!g_videoGridManager) {
        return R"({"success": false, "error": "Video grid manager not initialized"})";
    }

    // Extract JSON from webview array format
    std::string json = extractJsonFromArray(args);

    // Parse args: {"participantId": 1}
    int participantId = parseJsonInt(json, "participantId");

    if (participantId < 0) {
        return R"({"success": false, "error": "Invalid participantId"})";
    }

    bool success = g_videoGridManager->removeParticipant(participantId);

    return success ? R"({"success": true})" : R"({"success": false, "error": "Participant not found"})";
}

std::string getGridLayout(const std::string& args) {
    std::cout << "videoGetGridLayout called" << std::endl;

    if (!g_videoGridManager) {
        return R"({"participants": []})";
    }

    auto participants = g_videoGridManager->getParticipants();

    std::ostringstream oss;
    oss << "{\"participants\": [";

    for (size_t i = 0; i < participants.size(); ++i) {
        const auto& p = participants[i];
        const auto& pos = p.getPosition();

        oss << "{\"id\": " << p.getId()
            << ", \"name\": \"" << p.getName() << "\""
            << ", \"position\": {\"x\": " << pos.x
            << ", \"y\": " << pos.y
            << ", \"width\": " << pos.width
            << ", \"height\": " << pos.height << "}}";

        if (i < participants.size() - 1) {
            oss << ", ";
        }
    }

    oss << "]}";
    return oss.str();
}

std::string selectVideoFile(const std::string& args) {
    std::cout << "videoSelectFile called" << std::endl;

    std::string path = platform_helper::selectVideoFile();

    if (!path.empty()) {
        return "{\"success\": true, \"path\": \"" + path + "\"}";
    }

    return R"({"success": false, "error": "User cancelled or not implemented"})";
}

} // namespace video_bindings
