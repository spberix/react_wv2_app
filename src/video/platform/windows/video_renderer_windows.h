#ifndef VIDEO_RENDERER_WINDOWS_H
#define VIDEO_RENDERER_WINDOWS_H

#include "../../video_renderer.h"
#include <map>
#include <functional>

class VideoRendererWindows : public VideoRenderer {
public:
    VideoRendererWindows();
    ~VideoRendererWindows() override;

    bool initialize(void* nativeWindowHandle) override;
    bool addVideoTile(const VideoParticipant& participant) override;
    bool removeVideoTile(int participantId) override;
    void updateTilePositions(const std::vector<VideoParticipant>& participants) override;
    void setResizeCallback(std::function<void(int, int)> callback) override;
    void cleanup() override;

private:
    void* windowHandle_;
    std::map<int, void*> videoTiles_;
};

#endif // VIDEO_RENDERER_WINDOWS_H
