#ifndef VIDEO_RENDERER_MACOS_H
#define VIDEO_RENDERER_MACOS_H

#include "../../video_renderer.h"
#include <map>

#ifdef __OBJC__
@class NSWindow;
@class CALayer;
@class AVPlayer;
@class AVPlayerLayer;
#else
typedef void NSWindow;
typedef void CALayer;
typedef void AVPlayer;
typedef void AVPlayerLayer;
#endif

class VideoRendererMacOS : public VideoRenderer {
public:
    VideoRendererMacOS();
    ~VideoRendererMacOS() override;

    bool initialize(void* nativeWindowHandle) override;
    bool addVideoTile(const VideoParticipant& participant) override;
    bool removeVideoTile(int participantId) override;
    void updateTilePositions(const std::vector<VideoParticipant>& participants) override;
    void cleanup() override;

    // Set resize callback
    void setResizeCallback(std::function<void(int, int)> callback) override;

private:
    struct VideoTile {
        AVPlayer* player;
        AVPlayerLayer* layer;
        int participantId;
    };

    NSWindow* window_;
    CALayer* videoContainerLayer_;
    std::map<int, VideoTile> videoTiles_;
    std::function<void(int, int)> resizeCallback_;
    void* resizeObserver_; // Will be cast to id in .mm file

    void setupLooping(AVPlayer* player);
};

#endif // VIDEO_RENDERER_MACOS_H
