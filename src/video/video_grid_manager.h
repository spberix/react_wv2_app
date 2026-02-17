#ifndef VIDEO_GRID_MANAGER_H
#define VIDEO_GRID_MANAGER_H

#include "video_participant.h"
#include "video_renderer.h"
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
    // Using void* to avoid including webview.h header
    void setWebView(void* webview);

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

    // Participant transfer operations (for drag-and-drop between windows)
    VideoParticipant extractParticipant(int participantId);
    bool addExistingParticipant(const VideoParticipant& participant);

    // Extract and add video tiles (AVPlayerLayer transfer)
    bool extractVideoTile(int participantId, void** outPlayer, void** outLayer);
    bool addExistingVideoTile(int participantId, void* player, void* layer, const VideoParticipant& participant);

    // Force restart all videos
    void restartAllVideos();

    // Suppress React notifications during transfers (to avoid blocking webview->eval())
    void setSuppressReactNotifications(bool suppress);

    // Manually trigger React notification (used after suppressed operations complete)
    void notifyReact();

private:
    std::vector<VideoParticipant> participants_;
    std::unique_ptr<VideoRenderer> renderer_;
    void* webview_; // Using void* to avoid including webview.h
    int windowWidth_;
    int windowHeight_;
    int nextParticipantId_;
    mutable std::mutex mutex_;
    bool suppressReactNotifications_;

    // Layout constants
    static constexpr int MAX_PARTICIPANTS = 9;
    static constexpr int CONTROLS_HEIGHT = 60;
    static constexpr int GRID_PADDING = 10;

    // Calculate grid layout positions for all participants
    void calculateGridLayout();

    // Animate web app tiles via JavaScript injection
    void animateWebAppTiles(const std::vector<VideoParticipant>& participants);

    // Serialize participants to JSON
    std::string serializeParticipants() const;

    // Escape JSON string
    std::string escapeJson(const std::string& str) const;
};

#endif // VIDEO_GRID_MANAGER_H
