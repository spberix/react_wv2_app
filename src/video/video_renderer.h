#ifndef VIDEO_RENDERER_H
#define VIDEO_RENDERER_H

#include "video_participant.h"
#include <vector>
#include <memory>
#include <functional>

class VideoRenderer {
public:
    virtual ~VideoRenderer() = default;

    // Initialize the renderer with the native window handle
    virtual bool initialize(void* nativeWindowHandle) = 0;

    // Add a video tile for a participant
    virtual bool addVideoTile(const VideoParticipant& participant) = 0;

    // Remove a video tile by participant ID
    virtual bool removeVideoTile(int participantId) = 0;

    // Update positions of all video tiles
    virtual void updateTilePositions(const std::vector<VideoParticipant>& participants) = 0;

    // Set resize callback (called when window is resized)
    virtual void setResizeCallback(std::function<void(int, int)> callback) = 0;

    // Cleanup resources
    virtual void cleanup() = 0;

    // Platform factory method
    static std::unique_ptr<VideoRenderer> create();
};

#endif // VIDEO_RENDERER_H
