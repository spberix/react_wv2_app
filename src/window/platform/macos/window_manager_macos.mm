#include "../../window_manager.h"
#include "../../child_window.h"
#include "../../../video/video_grid_manager.h"
#include "../../../video/video_participant.h"
#include <algorithm>
#include <iostream>

#ifdef __APPLE__
#import <Cocoa/Cocoa.h>
#endif

WindowManager::WindowManager()
    : mainWindow_(nullptr)
    , mainGridManager_(nullptr) {
}

WindowManager::~WindowManager() {
    destroyAllChildWindows();
}

bool WindowManager::initialize(void* mainWindow, VideoGridManager* mainGridManager) {
    std::lock_guard<std::mutex> lock(mutex_);

    if (!mainWindow || !mainGridManager) {
        return false;
    }

    mainWindow_ = mainWindow;
    mainGridManager_ = mainGridManager;

    return true;
}

ChildWindow* WindowManager::createChildWindow(const WindowRect& rect, const std::string& title) {
    std::lock_guard<std::mutex> lock(mutex_);

    auto childWindow = std::make_unique<ChildWindow>();

    if (!childWindow->initialize(rect, title)) {
        return nullptr;
    }

    ChildWindow* ptr = childWindow.get();
    childWindows_.push_back(std::move(childWindow));

    return ptr;
}

void WindowManager::destroyChildWindow(ChildWindow* window) {
    if (!window) return;

    std::lock_guard<std::mutex> lock(mutex_);

    // Find and remove the window
    auto it = std::find_if(childWindows_.begin(), childWindows_.end(),
        [window](const std::unique_ptr<ChildWindow>& w) {
            return w.get() == window;
        });

    if (it != childWindows_.end()) {
        // Transfer all participants back to main window before destroying
        auto participants = (*it)->getGridManager()->getParticipants();
        for (const auto& participant : participants) {
            // Transfer back to main window
            transferParticipant(participant.getId(), window, nullptr);
        }

        childWindows_.erase(it);
    }
}

void WindowManager::destroyAllChildWindows() {
    std::lock_guard<std::mutex> lock(mutex_);

    // Transfer all participants back to main window
    for (auto& childWindow : childWindows_) {
        auto participants = childWindow->getGridManager()->getParticipants();
        for (const auto& participant : participants) {
            transferParticipant(participant.getId(), childWindow.get(), nullptr);
        }
    }

    childWindows_.clear();
}

bool WindowManager::transferParticipant(int participantId, void* sourceWindow, void* targetWindow) {
    // DON'T lock here - let individual operations handle their own locking
    // Holding a lock while calling other managers can cause deadlocks

    VideoGridManager* sourceGrid = nullptr;
    VideoGridManager* targetGrid = nullptr;

    {
        std::lock_guard<std::mutex> lock(mutex_);
        sourceGrid = getGridManagerForWindow(sourceWindow);
        targetGrid = getGridManagerForWindow(targetWindow);
    }

    if (!sourceGrid || !targetGrid) {
        return false;
    }

    // USE RECREATION APPROACH instead of extractParticipant/addExistingParticipant
    // This avoids the event loop blocking issue

    // 1. Get participant info BEFORE removing
    auto participants = sourceGrid->getParticipants();
    VideoParticipant targetParticipant(-1, "", "", ParticipantType::VIDEO);

    for (const auto& p : participants) {
        if (p.getId() == participantId) {
            targetParticipant = p;
            break;
        }
    }

    if (targetParticipant.getId() == -1) {
        std::cerr << "Participant " << participantId << " not found in source window" << std::endl;
        return false;
    }

    std::cout << "Transferring participant " << participantId << ": " << targetParticipant.getName() << std::endl;

    // CRITICAL: Suppress React notifications during transfer to avoid blocking webview->eval() calls
    sourceGrid->setSuppressReactNotifications(true);
    if (targetGrid != sourceGrid) {
        targetGrid->setSuppressReactNotifications(true);
    }

    // 2. Remove from source window (simple removal)
    sourceGrid->removeParticipant(participantId);

    // 3. Add NEW participant to target window (creates fresh video)
    int newId = targetGrid->addParticipant(targetParticipant.getName(),
                                           targetParticipant.getVideoPath());

    if (newId < 0) {
        std::cerr << "Failed to add participant to target window" << std::endl;
        // Re-enable notifications before returning
        sourceGrid->setSuppressReactNotifications(false);
        if (targetGrid != sourceGrid) {
            targetGrid->setSuppressReactNotifications(false);
        }
        // Try to add back to source
        sourceGrid->addParticipant(targetParticipant.getName(), targetParticipant.getVideoPath());
        return false;
    }

    // Re-enable notifications for future operations
    sourceGrid->setSuppressReactNotifications(false);
    if (targetGrid != sourceGrid) {
        targetGrid->setSuppressReactNotifications(false);
    }

    std::cout << "Transfer complete: removed " << participantId << " from source, added as " << newId << " to target" << std::endl;

    // NO React notification at all - this is what's causing the hang
    // Let the UI update on next user interaction
    std::cout << "Transfer complete - UI will update on next interaction" << std::endl;

    // Update participant registry (with its own lock)
    {
        std::lock_guard<std::mutex> lock(mutex_);
        registerParticipant(newId, targetWindow, isMainWindow(targetWindow));
    }

    return true;
}

bool WindowManager::createChildWindowWithParticipant(int participantId, const WindowRect& rect) {
    // Get participant info from main window BEFORE removing
    VideoGridManager* mainGrid = getGridManagerForWindow(nullptr);
    if (!mainGrid) {
        std::cerr << "Main grid manager not found" << std::endl;
        return false;
    }

    // Get ALL participant info first
    auto participants = mainGrid->getParticipants();
    VideoParticipant targetParticipant(-1, "", "", ParticipantType::VIDEO);

    for (const auto& p : participants) {
        if (p.getId() == participantId) {
            targetParticipant = p;
            break;
        }
    }

    if (targetParticipant.getId() == -1) {
        std::cerr << "Participant " << participantId << " not found" << std::endl;
        return false;
    }

    std::cout << "Found participant " << participantId << ": " << targetParticipant.getName()
              << " with video: " << targetParticipant.getVideoPath() << std::endl;

    // Create child window
    std::string title = "Video Window " + std::to_string(childWindows_.size() + 1);
    ChildWindow* childWindow = createChildWindow(rect, title);

    if (!childWindow) {
        return false;
    }

    std::cout << "Window created, now removing from main and adding to child..." << std::endl;

    // Remove from main window (simple removal, no transfer)
    mainGrid->removeParticipant(participantId);

    // Add NEW participant to child window (creates fresh video from file path)
    VideoGridManager* childGrid = childWindow->getGridManager();
    if (childGrid) {
        // Add as completely new participant (will create fresh AVPlayer)
        int newId = childGrid->addParticipant(targetParticipant.getName(),
                                               targetParticipant.getVideoPath());
        std::cout << "Added participant to child window with new ID: " << newId << std::endl;
    }

    return true;
}

void* WindowManager::getWindowAtScreenPosition(float x, float y) {
    std::lock_guard<std::mutex> lock(mutex_);

#ifdef __APPLE__
    NSPoint screenPoint = NSMakePoint(x, y);

    // Check child windows first (they're on top)
    for (const auto& childWindow : childWindows_) {
        NSWindow* nsWindow = childWindow->getNativeWindow();
        if (nsWindow) {
            NSRect frame = [nsWindow frame];
            if (NSPointInRect(screenPoint, frame)) {
                return childWindow.get();
            }
        }
    }

    // Check main window
    if (mainWindow_) {
        // Cast void* to NSWindow*
        NSWindow* nsWindow = (__bridge NSWindow*)mainWindow_;
        NSRect frame = [nsWindow frame];
        if (NSPointInRect(screenPoint, frame)) {
            return nullptr; // nullptr represents main window
        }
    }
#endif

    return nullptr;
}

bool WindowManager::isPositionOutsideAllWindows(float x, float y) {
    std::lock_guard<std::mutex> lock(mutex_);

#ifdef __APPLE__
    NSPoint screenPoint = NSMakePoint(x, y);

    // Check main window
    if (mainWindow_) {
        // Cast void* to NSWindow*
        NSWindow* nsWindow = (__bridge NSWindow*)mainWindow_;
        NSRect frame = [nsWindow frame];
        if (NSPointInRect(screenPoint, frame)) {
            return false;
        }
    }

    // Check all child windows
    for (const auto& childWindow : childWindows_) {
        NSWindow* nsWindow = childWindow->getNativeWindow();
        if (nsWindow) {
            NSRect frame = [nsWindow frame];
            if (NSPointInRect(screenPoint, frame)) {
                return false;
            }
        }
    }

    return true; // Not inside any window
#else
    return false;
#endif
}

void WindowManager::registerParticipant(int participantId, void* ownerWindow, bool isMainWindow) {
    std::lock_guard<std::mutex> lock(mutex_);

    ParticipantLocation location;
    location.participantId = participantId;
    location.ownerWindow = ownerWindow;
    location.isInMainWindow = isMainWindow;

    participantRegistry_[participantId] = location;
}

void WindowManager::unregisterParticipant(int participantId) {
    std::lock_guard<std::mutex> lock(mutex_);
    participantRegistry_.erase(participantId);
}

ParticipantLocation* WindowManager::getParticipantLocation(int participantId) {
    std::lock_guard<std::mutex> lock(mutex_);

    auto it = participantRegistry_.find(participantId);
    if (it != participantRegistry_.end()) {
        return &it->second;
    }

    return nullptr;
}

VideoGridManager* WindowManager::getGridManagerForWindow(void* window) {
    if (isMainWindow(window)) {
        return mainGridManager_;
    }

    // Check child windows
    for (const auto& childWindow : childWindows_) {
        if (childWindow.get() == window) {
            return childWindow->getGridManager();
        }
    }

    return nullptr;
}

bool WindowManager::isMainWindow(void* window) const {
    return window == nullptr; // nullptr represents main window
}
