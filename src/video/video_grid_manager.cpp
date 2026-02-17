#include "video_grid_manager.h"
#include <sstream>
#include <iostream>
#include <cmath>

// External helper function to call webview eval without including webview.h
extern "C" void callWebViewEval(void* webview, const char* js);

VideoGridManager::VideoGridManager()
    : webview_(nullptr),
      windowWidth_(800),
      windowHeight_(600),
      nextParticipantId_(1),
      suppressReactNotifications_(false) {
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

void VideoGridManager::setWebView(void* webview) {
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
    VideoParticipant participant(participantId, name, videoPath, ParticipantType::VIDEO);

    // Add to list
    participants_.push_back(participant);

    std::cout << "Added participant " << participantId << ": " << name << std::endl;

    // Recalculate layout
    calculateGridLayout();

    // Update positions of ALL existing video tiles (before adding new one)
    if (renderer_ && participants_.size() > 1) {
        // Filter only video participants (exclude the newly added one)
        std::vector<VideoParticipant> existingVideoParticipants;
        for (size_t i = 0; i < participants_.size() - 1; ++i) {
            if (participants_[i].isVideo()) {
                existingVideoParticipants.push_back(participants_[i]);
            }
        }
        if (!existingVideoParticipants.empty()) {
            renderer_->updateTilePositions(existingVideoParticipants);
        }
    }

    // Add new video tile to renderer
    if (renderer_) {
        renderer_->addVideoTile(participants_.back());
    }

    // Animate web apps via JavaScript (synchronizes with video animations)
    animateWebAppTiles(participants_);

    // Notify React
    notifyReact();

    return participantId;
}

int VideoGridManager::addWebApp(const std::string& name, const std::string& url) {
    std::lock_guard<std::mutex> lock(mutex_);

    // Check max capacity
    if (participants_.size() >= MAX_PARTICIPANTS) {
        std::cerr << "Cannot add web app: maximum capacity reached" << std::endl;
        return -1;
    }

    // Create new web app participant
    int participantId = nextParticipantId_++;
    VideoParticipant participant(participantId, name, url, ParticipantType::WEB_APP);

    // Add to list
    participants_.push_back(participant);

    std::cout << "Added web app " << participantId << ": " << name << " (" << url << ")" << std::endl;

    // Recalculate layout
    calculateGridLayout();

    // Update positions of ALL existing video tiles (before adding new one)
    if (renderer_ && participants_.size() > 1) {
        // Filter only video participants for renderer update
        std::vector<VideoParticipant> videoParticipants;
        for (const auto& p : participants_) {
            if (p.isVideo()) {
                videoParticipants.push_back(p);
            }
        }
        if (!videoParticipants.empty()) {
            renderer_->updateTilePositions(videoParticipants);
        }
    }

    // Animate web apps via JavaScript (synchronizes with video animations)
    animateWebAppTiles(participants_);

    // Notify React (will include all participants, including web apps)
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

    // Update positions of all remaining video tiles
    if (renderer_ && !participants_.empty()) {
        std::vector<VideoParticipant> videoParticipants;
        for (const auto& p : participants_) {
            if (p.isVideo()) {
                videoParticipants.push_back(p);
            }
        }
        if (!videoParticipants.empty()) {
            renderer_->updateTilePositions(videoParticipants);
        }
    }

    // Animate web apps via JavaScript (synchronizes with video animations)
    animateWebAppTiles(participants_);

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

    // Update renderer positions for video participants only
    if (renderer_) {
        std::vector<VideoParticipant> videoParticipants;
        for (const auto& p : participants_) {
            if (p.isVideo()) {
                videoParticipants.push_back(p);
            }
        }
        if (!videoParticipants.empty()) {
            renderer_->updateTilePositions(videoParticipants);
        }
    }

    // Animate web apps via JavaScript (synchronizes with video animations)
    animateWebAppTiles(participants_);

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

void VideoGridManager::animateWebAppTiles(const std::vector<VideoParticipant>& participants) {
    std::cout << "animateWebAppTiles called with " << participants.size() << " participants" << std::endl;

    if (!webview_) {
        std::cout << "WARNING: webview_ is null, cannot animate" << std::endl;
        return;
    }

    // Count web apps
    int webAppCount = 0;
    for (const auto& p : participants) {
        if (p.isWebApp()) {
            webAppCount++;
        }
    }

    std::cout << "Found " << webAppCount << " web apps to animate" << std::endl;

    if (webAppCount == 0) {
        std::cout << "No web apps to animate, skipping" << std::endl;
        return;
    }

    // Build JavaScript to animate all web app iframes
    std::ostringstream js;
    js << "(function() {";
    js << "  const duration = 200;"; // Match CATransaction 0.2s
    js << "  const easing = 'ease-in-out';";

    // Get grid container offset (matches React's calculation)
    js << "  const gridContainer = document.querySelector('.video-grid');";
    js << "  const gridOffset = gridContainer ? gridContainer.getBoundingClientRect() : {left: 0, top: 0};";

    for (const auto& p : participants) {
        if (p.isWebApp()) {
            const auto& pos = p.getPosition();

            // Find iframe by participant ID and animate it using Web Animations API
            js << "  const tile" << p.getId() << " = document.querySelector('[data-participant-id=\""
               << p.getId() << "\"]');";
            js << "  if (tile" << p.getId() << ") {";
            js << "    const adjustedLeft = " << pos.x << " - gridOffset.left;";
            js << "    const adjustedTop = " << pos.y << " - gridOffset.top;";
            js << "    tile" << p.getId() << ".animate([";
            js << "      {},"; // Start from current position
            js << "      {";
            js << "        left: adjustedLeft + 'px',";
            js << "        top: adjustedTop + 'px',";
            js << "        width: '" << pos.width << "px',";
            js << "        height: '" << pos.height << "px'";
            js << "      }";
            js << "    ], { duration: duration, easing: easing, fill: 'forwards' });";
            js << "  }";
        }
    }

    js << "})();";

    // Execute JavaScript in WebView
    std::string jsCode = js.str();
    callWebViewEval(webview_, jsCode.c_str());

    std::cout << "Animated web app tiles via JavaScript" << std::endl;
}

void VideoGridManager::notifyReact() {
    if (!webview_ || suppressReactNotifications_) {
        if (suppressReactNotifications_) {
            std::cout << "Skipping React notification (suppressed during transfer)" << std::endl;
        }
        return;
    }

    std::string json = serializeParticipants();
    std::string js = "window.dispatchEvent(new CustomEvent('videoPositionsChanged', "
                    "{detail: " + json + "}))";

    // Execute JavaScript to dispatch event
    callWebViewEval(webview_, js.c_str());

    std::cout << "Notified React with " << participants_.size() << " participants" << std::endl;
}

void VideoGridManager::setSuppressReactNotifications(bool suppress) {
    std::lock_guard<std::mutex> lock(mutex_);
    suppressReactNotifications_ = suppress;
    std::cout << (suppress ? "Suppressing" : "Enabling") << " React notifications" << std::endl;
}

std::string VideoGridManager::serializeParticipants() const {
    std::ostringstream oss;
    oss << "{\"participants\": [";

    for (size_t i = 0; i < participants_.size(); ++i) {
        const auto& p = participants_[i];
        const auto& pos = p.getPosition();

        oss << "{\"id\": " << p.getId()
            << ", \"name\": \"" << escapeJson(p.getName()) << "\""
            << ", \"type\": \"" << (p.isVideo() ? "video" : "web_app") << "\""
            << ", \"contentUrl\": \"" << escapeJson(p.getContentPath()) << "\""
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

VideoParticipant VideoGridManager::extractParticipant(int participantId) {
    VideoParticipant participant(-1, "", "", ParticipantType::VIDEO);

    {
        std::lock_guard<std::mutex> lock(mutex_);

        // Suppress React notifications during transfer
        suppressReactNotifications_ = true;

        // Find participant
        auto it = std::find_if(participants_.begin(), participants_.end(),
                              [participantId](const VideoParticipant& p) {
                                  return p.getId() == participantId;
                              });

        if (it == participants_.end()) {
            std::cerr << "Participant " << participantId << " not found for extraction" << std::endl;
            suppressReactNotifications_ = false;
            // Return invalid participant
            return participant;
        }

        // Make a copy of the participant
        participant = *it;

        // Remove video tile from renderer (but keep the AVPlayer/Layer alive)
        // Note: For proper transfer, we need to extract the player/layer first
        if (renderer_ && participant.isVideo()) {
            renderer_->removeVideoTile(participantId);
        }

        // Remove from participants list
        participants_.erase(it);

        std::cout << "Extracted participant " << participantId << std::endl;

        // Recalculate layout for remaining participants
        if (!participants_.empty()) {
            calculateGridLayout();

            // Update positions of all remaining video tiles
            if (renderer_) {
                std::vector<VideoParticipant> videoParticipants;
                for (const auto& p : participants_) {
                    if (p.isVideo()) {
                        videoParticipants.push_back(p);
                    }
                }
                if (!videoParticipants.empty()) {
                    renderer_->updateTilePositions(videoParticipants);
                }
            }

            // Animate web apps (will be skipped due to suppression)
            animateWebAppTiles(participants_);
        }

        // Re-enable notifications flag
        suppressReactNotifications_ = false;
        // Re-enable notifications flag but DON'T call notifyReact yet
        suppressReactNotifications_ = false;
    } // Mutex is released here

    // Restart all videos to ensure they're still playing
    restartAllVideos();

    // DON'T notify React here - it will cause a hang
    // React will be notified on the next user interaction or we can trigger it later
    std::cout << "Transfer complete - skipping immediate React notification to avoid hang" << std::endl;

    return participant;
}

bool VideoGridManager::addExistingParticipant(const VideoParticipant& participant) {
    {
        std::lock_guard<std::mutex> lock(mutex_);

        // Suppress React notifications during transfer
        suppressReactNotifications_ = true;

        // Check if participant ID is valid
        if (participant.getId() < 0) {
            std::cerr << "Invalid participant for addition" << std::endl;
            suppressReactNotifications_ = false;
            return false;
        }

        // Check max capacity
        if (participants_.size() >= MAX_PARTICIPANTS) {
            std::cerr << "Cannot add participant: maximum capacity reached" << std::endl;
            suppressReactNotifications_ = false;
            return false;
        }

        // Add to list
        participants_.push_back(participant);

        std::cout << "Added existing participant " << participant.getId() << ": " << participant.getName() << std::endl;

        // Recalculate layout
        calculateGridLayout();

        // Update positions of ALL existing video tiles (before adding new one)
        if (renderer_ && participants_.size() > 1) {
            // Filter only video participants (exclude the newly added one)
            std::vector<VideoParticipant> existingVideoParticipants;
            for (size_t i = 0; i < participants_.size() - 1; ++i) {
                if (participants_[i].isVideo()) {
                    existingVideoParticipants.push_back(participants_[i]);
                }
            }
            if (!existingVideoParticipants.empty()) {
                renderer_->updateTilePositions(existingVideoParticipants);
            }
        }

        // Add new video tile to renderer
        if (renderer_ && participant.isVideo()) {
            renderer_->addVideoTile(participants_.back());
        }

        // Animate web apps via JavaScript (synchronizes with video animations) - will be skipped due to suppression
        animateWebAppTiles(participants_);

        // Keep suppressed for now
    } // Mutex is released here

    // Restart all videos to ensure they're still playing
    restartAllVideos();

    // DON'T notify React here - it will cause a hang
    // React will be notified on the next user interaction
    suppressReactNotifications_ = false;
    std::cout << "Transfer complete - skipping immediate React notification to avoid hang" << std::endl;

    return true;
}

bool VideoGridManager::extractVideoTile(int participantId, void** outPlayer, void** outLayer) {
    // This method would extract the AVPlayer and AVPlayerLayer for transfer
    // Implementation depends on VideoRenderer having access to these objects
    // For now, we'll use the simpler extractParticipant/addExistingParticipant flow
    std::cerr << "extractVideoTile not yet implemented - use extractParticipant instead" << std::endl;
    return false;
}

bool VideoGridManager::addExistingVideoTile(int participantId, void* player, void* layer, const VideoParticipant& participant) {
    // This method would add an existing AVPlayer/AVPlayerLayer
    // Implementation depends on VideoRenderer supporting this
    // For now, we'll use the simpler extractParticipant/addExistingParticipant flow
    std::cerr << "addExistingVideoTile not yet implemented - use addExistingParticipant instead" << std::endl;
    return false;
}

void VideoGridManager::restartAllVideos() {
    if (renderer_) {
        std::cout << "Restarting all videos..." << std::endl;
        // Get all video participants
        std::vector<VideoParticipant> videoParticipants;
        for (const auto& p : participants_) {
            if (p.isVideo()) {
                videoParticipants.push_back(p);
            }
        }

        if (!videoParticipants.empty()) {
            // Force position update which will also restart videos
            renderer_->updateTilePositions(videoParticipants);
        }
    }
}
