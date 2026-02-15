#include "video_grid_manager.h"
#include <sstream>
#include <iostream>
#include <cmath>

VideoGridManager::VideoGridManager()
    : webview_(nullptr),
      windowWidth_(800),
      windowHeight_(600),
      nextParticipantId_(1) {
}

VideoGridManager::~VideoGridManager() {
    cleanup();
}

bool VideoGridManager::initialize(void* nativeWindowHandle, int width, int height) {
    std::lock_guard<std::mutex> lock(mutex_);

    windowWidth_ = width;
    windowHeight_ = height;

    // Create platform-specific renderer
    renderer_ = VideoRenderer::create();
    if (!renderer_) {
        std::cerr << "Failed to create video renderer" << std::endl;
        return false;
    }

    // Initialize renderer with native window
    if (!renderer_->initialize(nativeWindowHandle)) {
        std::cerr << "Failed to initialize video renderer" << std::endl;
        return false;
    }

    // Set resize callback
    renderer_->setResizeCallback([this](int width, int height) {
        this->updateWindowSize(width, height);
    });

    std::cout << "VideoGridManager initialized: " << width << "x" << height << std::endl;
    return true;
}

void VideoGridManager::setWebView(webview::webview* webview) {
    std::lock_guard<std::mutex> lock(mutex_);
    webview_ = webview;
    std::cout << "VideoGridManager: WebView set" << std::endl;
}

int VideoGridManager::addParticipant(const std::string& name, const std::string& videoPath) {
    std::lock_guard<std::mutex> lock(mutex_);

    // Check max capacity
    if (participants_.size() >= MAX_PARTICIPANTS) {
        std::cerr << "Cannot add participant: maximum capacity reached" << std::endl;
        return -1;
    }

    // Create new participant
    int participantId = nextParticipantId_++;
    VideoParticipant participant(participantId, name, videoPath);

    // Add to list
    participants_.push_back(participant);

    std::cout << "Added participant " << participantId << ": " << name << std::endl;

    // Recalculate layout
    calculateGridLayout();

    // Update positions of ALL existing tiles (before adding new one)
    if (renderer_ && participants_.size() > 1) {
        // Update all tiles except the newly added one
        std::vector<VideoParticipant> existingParticipants(participants_.begin(), participants_.end() - 1);
        renderer_->updateTilePositions(existingParticipants);
    }

    // Add new video tile to renderer
    if (renderer_) {
        renderer_->addVideoTile(participants_.back());
    }

    // Notify React
    notifyReact();

    return participantId;
}

bool VideoGridManager::removeParticipant(int participantId) {
    std::lock_guard<std::mutex> lock(mutex_);

    // Find participant
    auto it = std::find_if(participants_.begin(), participants_.end(),
                          [participantId](const VideoParticipant& p) {
                              return p.getId() == participantId;
                          });

    if (it == participants_.end()) {
        std::cerr << "Participant " << participantId << " not found" << std::endl;
        return false;
    }

    // Remove from renderer
    if (renderer_) {
        renderer_->removeVideoTile(participantId);
    }

    // Remove from list
    participants_.erase(it);

    std::cout << "Removed participant " << participantId << std::endl;

    // Recalculate layout for remaining participants
    calculateGridLayout();

    // Update positions of all remaining tiles
    if (renderer_ && !participants_.empty()) {
        renderer_->updateTilePositions(participants_);
    }

    // Notify React
    notifyReact();

    return true;
}

void VideoGridManager::updateWindowSize(int width, int height) {
    std::lock_guard<std::mutex> lock(mutex_);

    windowWidth_ = width;
    windowHeight_ = height;

    std::cout << "Window size updated: " << width << "x" << height << std::endl;

    // Recalculate layout
    calculateGridLayout();

    // Update renderer positions
    if (renderer_) {
        renderer_->updateTilePositions(participants_);
    }

    // Notify React
    notifyReact();
}

std::vector<VideoParticipant> VideoGridManager::getParticipants() const {
    std::lock_guard<std::mutex> lock(mutex_);
    return participants_;
}

size_t VideoGridManager::getParticipantCount() const {
    std::lock_guard<std::mutex> lock(mutex_);
    return participants_.size();
}

void VideoGridManager::cleanup() {
    std::lock_guard<std::mutex> lock(mutex_);

    if (renderer_) {
        renderer_->cleanup();
        renderer_.reset();
    }

    participants_.clear();
}

void VideoGridManager::calculateGridLayout() {
    // Calculate available space for grid (below controls)
    int gridTop = CONTROLS_HEIGHT;
    int gridHeight = windowHeight_ - CONTROLS_HEIGHT;
    int gridWidth = windowWidth_;

    if (participants_.empty()) {
        return;
    }

    // Calculate dynamic grid dimensions based on participant count
    int participantCount = participants_.size();
    int cols, rows;

    if (participantCount == 1) {
        cols = 1;
        rows = 1;
    } else if (participantCount == 2) {
        cols = 2;
        rows = 1;
    } else if (participantCount <= 4) {
        cols = 2;
        rows = 2;
    } else if (participantCount <= 6) {
        cols = 3;
        rows = 2;
    } else {
        cols = 3;
        rows = 3;
    }

    std::cout << "Dynamic grid layout: " << rows << "x" << cols << " for "
              << participantCount << " participants" << std::endl;

    // Calculate tile dimensions
    int tileWidth = (gridWidth - (cols + 1) * GRID_PADDING) / cols;
    int tileHeight = (gridHeight - (rows + 1) * GRID_PADDING) / rows;

    // Maintain 16:9 aspect ratio
    int aspectWidth = tileWidth;
    int aspectHeight = (aspectWidth * 9) / 16;

    if (aspectHeight > tileHeight) {
        aspectHeight = tileHeight;
        aspectWidth = (aspectHeight * 16) / 9;
    }

    // Position each participant in the grid
    for (size_t i = 0; i < participants_.size(); ++i) {
        int row = i / cols;
        int col = i % cols;

        // Calculate position with padding
        int x = GRID_PADDING + col * (tileWidth + GRID_PADDING);
        int y = gridTop + GRID_PADDING + row * (tileHeight + GRID_PADDING);

        // Center tile in cell
        x += (tileWidth - aspectWidth) / 2;
        y += (tileHeight - aspectHeight) / 2;

        VideoPosition pos(x, y, aspectWidth, aspectHeight);
        participants_[i].setPosition(pos);

        std::cout << "Participant " << participants_[i].getId()
                  << " positioned at (" << x << ", " << y << ") "
                  << aspectWidth << "x" << aspectHeight << std::endl;
    }
}

void VideoGridManager::notifyReact() {
    if (!webview_) {
        return;
    }

    std::string json = serializeParticipants();
    std::string js = "window.dispatchEvent(new CustomEvent('videoPositionsChanged', "
                    "{detail: " + json + "}))";

    // Execute JavaScript to dispatch event
    webview_->eval(js);

    std::cout << "Notified React with " << participants_.size() << " participants" << std::endl;
}

std::string VideoGridManager::serializeParticipants() const {
    std::ostringstream oss;
    oss << "{\"participants\": [";

    for (size_t i = 0; i < participants_.size(); ++i) {
        const auto& p = participants_[i];
        const auto& pos = p.getPosition();

        oss << "{\"id\": " << p.getId()
            << ", \"name\": \"" << escapeJson(p.getName()) << "\""
            << ", \"position\": {\"x\": " << pos.x
            << ", \"y\": " << pos.y
            << ", \"width\": " << pos.width
            << ", \"height\": " << pos.height << "}}";

        if (i < participants_.size() - 1) {
            oss << ", ";
        }
    }

    oss << "]}";
    return oss.str();
}

std::string VideoGridManager::escapeJson(const std::string& str) const {
    std::ostringstream oss;
    for (char c : str) {
        switch (c) {
            case '"':  oss << "\\\""; break;
            case '\\': oss << "\\\\"; break;
            case '\b': oss << "\\b"; break;
            case '\f': oss << "\\f"; break;
            case '\n': oss << "\\n"; break;
            case '\r': oss << "\\r"; break;
            case '\t': oss << "\\t"; break;
            default:   oss << c; break;
        }
    }
    return oss.str();
}
