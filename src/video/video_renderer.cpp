#include "video_renderer.h"

#ifdef __APPLE__
#include "platform/macos/video_renderer_macos.h"
#elif _WIN32
#include "platform/windows/video_renderer_windows.h"
#else
#include "platform/linux/video_renderer_linux.h"
#endif

std::unique_ptr<VideoRenderer> VideoRenderer::create() {
#ifdef __APPLE__
    return std::make_unique<VideoRendererMacOS>();
#elif _WIN32
    return std::make_unique<VideoRendererWindows>();
#else
    return std::make_unique<VideoRendererLinux>();
#endif
}
