#include "video_renderer_linux.h"
#include <iostream>

VideoRendererLinux::VideoRendererLinux() : windowHandle_(nullptr) {
}

VideoRendererLinux::~VideoRendererLinux() {
    cleanup();
}

bool VideoRendererLinux::initialize(void* nativeWindowHandle) {
    windowHandle_ = nativeWindowHandle;
    std::cout << "VideoRendererLinux initialized (stub)" << std::endl;
    // TODO: Initialize GStreamer
    return true;
}

bool VideoRendererLinux::addVideoTile(const VideoParticipant& participant) {
    std::cout << "VideoRendererLinux::addVideoTile (stub) - " << participant.getName() << std::endl;
    // TODO: Implement GStreamer video playback
    return true;
}

bool VideoRendererLinux::removeVideoTile(int participantId) {
    std::cout << "VideoRendererLinux::removeVideoTile (stub) - " << participantId << std::endl;
    // TODO: Implement tile removal
    return true;
}

void VideoRendererLinux::updateTilePositions(const std::vector<VideoParticipant>& participants) {
    std::cout << "VideoRendererLinux::updateTilePositions (stub)" << std::endl;
    // TODO: Implement position updates
}

void VideoRendererLinux::setResizeCallback(std::function<void(int, int)> callback) {
    std::cout << "VideoRendererLinux::setResizeCallback (stub)" << std::endl;
    // TODO: Implement resize callback
}

void VideoRendererLinux::cleanup() {
    std::cout << "VideoRendererLinux cleanup (stub)" << std::endl;
    videoTiles_.clear();
}
