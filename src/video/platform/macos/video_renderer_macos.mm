#import "video_renderer_macos.h"
#import <Cocoa/Cocoa.h>
#import <AVFoundation/AVFoundation.h>
#import <QuartzCore/QuartzCore.h>
#include <iostream>

VideoRendererMacOS::VideoRendererMacOS()
    : window_(nullptr), videoContainerLayer_(nullptr), resizeObserver_(nullptr) {
}

VideoRendererMacOS::~VideoRendererMacOS() {
    cleanup();
}

bool VideoRendererMacOS::initialize(void* nativeWindowHandle) {
    if (!nativeWindowHandle) {
        std::cerr << "Invalid native window handle" << std::endl;
        return false;
    }

    // Cast to NSWindow
    window_ = (__bridge NSWindow*)nativeWindowHandle;

    // Make content view layer-backed
    [window_.contentView setWantsLayer:YES];

    // Make content view transparent
    window_.contentView.layer.backgroundColor = [[NSColor clearColor] CGColor];
    window_.contentView.layer.opaque = NO;

    // Create video container layer
    videoContainerLayer_ = [[CALayer alloc] init];
    videoContainerLayer_.frame = window_.contentView.bounds;
    videoContainerLayer_.autoresizingMask = kCALayerWidthSizable | kCALayerHeightSizable;
    videoContainerLayer_.backgroundColor = [[NSColor blackColor] CGColor]; // Black background
    videoContainerLayer_.opaque = YES;

    // Insert video layer at the bottom (index 0)
    [window_.contentView.layer insertSublayer:videoContainerLayer_ atIndex:0];

    std::cout << "VideoRendererMacOS initialized" << std::endl;
    std::cout << "Content view sublayers count: " << window_.contentView.layer.sublayers.count << std::endl;
    return true;
}

void VideoRendererMacOS::setResizeCallback(std::function<void(int, int)> callback) {
    resizeCallback_ = callback;

    // Remove existing observer if any
    if (resizeObserver_) {
        [[NSNotificationCenter defaultCenter] removeObserver:(__bridge id)resizeObserver_];
        resizeObserver_ = nullptr;
    }

    // Set up window resize observer
    if (window_ && resizeCallback_) {
        id observer = [[NSNotificationCenter defaultCenter]
            addObserverForName:NSWindowDidResizeNotification
                        object:window_
                         queue:[NSOperationQueue mainQueue]
                    usingBlock:^(NSNotification* note) {
            NSWindow* window = (NSWindow*)note.object;
            NSRect frame = window.contentView.bounds;

            std::cout << "Window resized to: " << frame.size.width << "x" << frame.size.height << std::endl;

            // Update video container frame
            videoContainerLayer_.frame = frame;

            // Call the resize callback
            if (resizeCallback_) {
                resizeCallback_(frame.size.width, frame.size.height);
            }
        }];

        resizeObserver_ = (__bridge_retained void*)observer;

        std::cout << "Window resize observer installed" << std::endl;
    }
}

bool VideoRendererMacOS::addVideoTile(const VideoParticipant& participant) {
    @autoreleasepool {
        // Check if already exists
        if (videoTiles_.find(participant.getId()) != videoTiles_.end()) {
            std::cerr << "Video tile already exists for participant " << participant.getId() << std::endl;
            return false;
        }

        // Create file URL from path
        NSString* pathString = [NSString stringWithUTF8String:participant.getVideoPath().c_str()];
        NSURL* videoURL = [NSURL fileURLWithPath:pathString];

        // Check if file exists
        if (![[NSFileManager defaultManager] fileExistsAtPath:pathString]) {
            std::cerr << "Video file not found: " << participant.getVideoPath() << std::endl;
            return false;
        }

        // Create AVPlayer
        AVPlayer* player = [AVPlayer playerWithURL:videoURL];
        if (!player) {
            std::cerr << "Failed to create AVPlayer" << std::endl;
            return false;
        }

        // Create AVPlayerLayer
        AVPlayerLayer* playerLayer = [AVPlayerLayer playerLayerWithPlayer:player];
        playerLayer.videoGravity = AVLayerVideoGravityResizeAspect;
        playerLayer.backgroundColor = [[NSColor redColor] CGColor]; // Debug: red background

        // Set position
        const VideoPosition& pos = participant.getPosition();
        playerLayer.frame = CGRectMake(pos.x, pos.y, pos.width, pos.height);

        std::cout << "Adding AVPlayerLayer at position: "
                  << pos.x << ", " << pos.y << " size: "
                  << pos.width << "x" << pos.height << std::endl;

        // Add to container layer
        [videoContainerLayer_ addSublayer:playerLayer];

        std::cout << "Video container sublayers count: " << videoContainerLayer_.sublayers.count << std::endl;

        // Setup looping
        setupLooping(player);

        // Store tile
        VideoTile tile;
        tile.player = player;
        tile.layer = playerLayer;
        tile.participantId = participant.getId();
        videoTiles_[participant.getId()] = tile;

        // Start playback
        [player play];

        std::cout << "Added video tile for participant " << participant.getId() << std::endl;
        return true;
    }
}

bool VideoRendererMacOS::removeVideoTile(int participantId) {
    @autoreleasepool {
        auto it = videoTiles_.find(participantId);
        if (it == videoTiles_.end()) {
            std::cerr << "Video tile not found for participant " << participantId << std::endl;
            return false;
        }

        VideoTile& tile = it->second;

        // Stop playback
        [tile.player pause];

        // Remove layer
        [tile.layer removeFromSuperlayer];

        // Remove from map
        videoTiles_.erase(it);

        std::cout << "Removed video tile for participant " << participantId << std::endl;
        return true;
    }
}

void VideoRendererMacOS::updateTilePositions(const std::vector<VideoParticipant>& participants) {
    @autoreleasepool {
        for (const auto& participant : participants) {
            auto it = videoTiles_.find(participant.getId());
            if (it != videoTiles_.end()) {
                const VideoPosition& pos = participant.getPosition();

                // Animate position change
                [CATransaction begin];
                [CATransaction setAnimationDuration:0.2];
                it->second.layer.frame = CGRectMake(pos.x, pos.y, pos.width, pos.height);
                [CATransaction commit];
            }
        }

        std::cout << "Updated positions for " << participants.size() << " video tiles" << std::endl;
    }
}

void VideoRendererMacOS::cleanup() {
    @autoreleasepool {
        // Remove resize observer
        if (resizeObserver_) {
            id observer = (__bridge_transfer id)resizeObserver_;
            [[NSNotificationCenter defaultCenter] removeObserver:observer];
            resizeObserver_ = nullptr;
        }

        // Stop and remove all video tiles
        for (auto& pair : videoTiles_) {
            [pair.second.player pause];
            [pair.second.layer removeFromSuperlayer];
        }
        videoTiles_.clear();

        // Remove container layer
        if (videoContainerLayer_) {
            [videoContainerLayer_ removeFromSuperlayer];
            videoContainerLayer_ = nullptr;
        }

        std::cout << "VideoRendererMacOS cleaned up" << std::endl;
    }
}

void VideoRendererMacOS::setupLooping(AVPlayer* player) {
    @autoreleasepool {
        // Add notification observer for when video ends
        [[NSNotificationCenter defaultCenter] addObserverForName:AVPlayerItemDidPlayToEndTimeNotification
                                                          object:player.currentItem
                                                           queue:[NSOperationQueue mainQueue]
                                                      usingBlock:^(NSNotification* note) {
            // Seek to beginning and play again
            [player seekToTime:kCMTimeZero];
            [player play];
        }];
    }
}
