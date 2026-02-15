#ifndef VIDEO_GRID_MANAGER_H
#define VIDEO_GRID_MANAGER_H

#include "video_participant.h"
#include "video_renderer.h"
#include "webview/webview.h"
#include <vector>
#include <memory>
#include <mutex>
#include <string>

class VideoGridManager {
public:
    VideoGridManager();
    ~VideoGridManager();

    // Initialize the video grid with native window handle
    bool initialize(void* nativeWindowHandle, int width, int height);

    // Set the webview instance for push notifications to React
    void setWebView(webview::webview* webview);

    // Add a participant with video file
    // Returns participant ID on success, -1 on failure
    int addParticipant(const std::string& name, const std::string& videoPath);

    // Add a web app participant
    // Returns participant ID on success, -1 on failure
    int addWebApp(const std::string& name, const std::string& url);

    // Remove a participant by ID
    bool removeParticipant(int participantId);

    // Update window size (triggers layout recalculation and React notification)
    void updateWindowSize(int width, int height);

    // Get all participants (for initial load)
    std::vector<VideoParticipant> getParticipants() const;

    // Get participant count
    size_t getParticipantCount() const;

    // Cleanup
    void cleanup();

private:
    std::vector<VideoParticipant> participants_;
    std::unique_ptr<VideoRenderer> renderer_;
    webview::webview* webview_;
    int windowWidth_;
    int windowHeight_;
    int nextParticipantId_;
    mutable std::mutex mutex_;

    // Layout constants
    static constexpr int MAX_PARTICIPANTS = 9;
    static constexpr int CONTROLS_HEIGHT = 60;
    static constexpr int GRID_PADDING = 10;

    // Calculate grid layout positions for all participants
    void calculateGridLayout();

    // Push notification to React when positions change
    void notifyReact();

    // Animate web app tiles via JavaScript injection
    void animateWebAppTiles(const std::vector<VideoParticipant>& participants);

    // Serialize participants to JSON
    std::string serializeParticipants() const;

    // Escape JSON string
    std::string escapeJson(const std::string& str) const;
};

#endif // VIDEO_GRID_MANAGER_H
