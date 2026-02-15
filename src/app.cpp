#include "app.h"
#include "bindings.h"
#include "video/video_grid_manager.h"
#include "platform_helper.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <climits>
#include <cstdlib>
#include <vector>

// Global pointer to video grid manager for bindings
static VideoGridManager* g_videoGridManager = nullptr;

// Pre-generated test video directory
static const std::string VIDEO_POOL_DIR = "/tmp/video_grid_test";

Application::Application(const std::string& title, int width, int height, bool debug, bool useLocalhost)
    : title_(title), width_(width), height_(height), debug_(debug), useLocalhost_(useLocalhost), initialized_(false) {
}

Application::~Application() {
    if (webview_) {
        webview_->terminate();
    }
}

bool Application::initialize() {
    try {
        // Create webview instance (debug mode, no parent window)
        webview_ = std::make_unique<webview::webview>(debug_, nullptr);

        if (!webview_) {
            std::cerr << "Failed to create webview" << std::endl;
            return false;
        }

        // Set window title and size
        webview_->set_title(title_);
        webview_->set_size(width_, height_, WEBVIEW_HINT_NONE);

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

        initialized_ = true;
        return true;

    } catch (const webview::exception& e) {
        std::cerr << "Webview exception during initialization: " << e.what() << std::endl;
        return false;
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

    // Apply transparency one more time right before running
    // This ensures it happens after everything is loaded
    auto windowResult = webview_->window();
    if (windowResult.ok()) {
        void* webviewWindow = windowResult.value();
        platform_helper::configureWebViewTransparency(webviewWindow);
    }

    // Inject CSS to ensure HTML/body are transparent
    webview_->eval(R"(
        (function() {
            const style = document.createElement('style');
            style.textContent = 'html, body { background: transparent !important; }';
            document.head.appendChild(style);
            console.log('Transparency CSS injected');
        })();
    )");

    try {
        // Run the webview event loop
        webview_->run();
    } catch (const webview::exception& e) {
        std::cerr << "Webview exception: " << e.what() << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }
}

void Application::terminate() {
    if (webview_) {
        webview_->terminate();
    }
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
            // Fall back to using set_html
            std::ifstream file(uiPath);
            std::stringstream buffer;
            buffer << file.rdbuf();
            webview_->set_html(buffer.str());
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

        webview_->set_html(fallbackHtml);
        std::cout << "Loading fallback UI (React app not built yet)" << std::endl;
    }
}

void Application::setupBindings() {
    // Bind C++ functions to JavaScript
    webview_->bind("minimize", bindings::minimize);
    webview_->bind("maximize", bindings::maximize);
    webview_->bind("closeApp", [this](const std::string& args) -> std::string {
        // Terminate the webview on close
        this->terminate();
        return bindings::closeApp(args);
    });
    webview_->bind("isMaximized", bindings::isMaximized);
    webview_->bind("getPlatform", bindings::getPlatform);

    // Demo bindings for showing bridge communication
    webview_->bind("reverseString", bindings::reverseString);
    webview_->bind("getRandomNumber", bindings::getRandomNumber);

    std::cout << "C++ bindings registered" << std::endl;
}

bool Application::initializeVideoLayer() {
    // Create video grid manager
    videoGridManager_ = std::make_unique<VideoGridManager>();

    // Get native window handle using platform helper
    auto windowResult = webview_->window();
    void* webviewWindow = nullptr;

    if (windowResult.ok()) {
        webviewWindow = windowResult.value();
    }

    if (!webviewWindow) {
        std::cerr << "Failed to get webview window" << std::endl;
        return false;
    }

    void* nativeWindowHandle = platform_helper::getNativeWindowHandle(webviewWindow);

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

    std::cout << "Video layer initialized" << std::endl;
    return true;
}

void Application::configureWebViewTransparency() {
    auto windowResult = webview_->window();
    if (windowResult.ok()) {
        void* webviewWindow = windowResult.value();
        platform_helper::configureWebViewTransparency(webviewWindow);
    } else {
        std::cerr << "Failed to get window for transparency configuration" << std::endl;
    }
}

void Application::setupVideoBindings() {
    // Capture 'this' to access videoGridManager_
    auto* manager = videoGridManager_.get();

    // Add participant
    webview_->bind("videoAddParticipant", [manager](const std::string& args) -> std::string {
        return video_bindings::addParticipant(args);
    });

    // Add web app
    webview_->bind("videoAddWebApp", [manager](const std::string& args) -> std::string {
        return video_bindings::addWebApp(args);
    });

    // Remove participant
    webview_->bind("videoRemoveParticipant", [manager](const std::string& args) -> std::string {
        return video_bindings::removeParticipant(args);
    });

    // Get grid layout
    webview_->bind("videoGetGridLayout", [manager](const std::string& args) -> std::string {
        return video_bindings::getGridLayout(args);
    });

    // Select video file
    webview_->bind("videoSelectFile", [manager](const std::string& args) -> std::string {
        return video_bindings::selectVideoFile(args);
    });

    std::cout << "Video bindings registered" << std::endl;
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
