#include "video_participant.h"

VideoParticipant::VideoParticipant(int id, const std::string& name, const std::string& videoPath)
    : id_(id), name_(name), videoPath_(videoPath), position_() {
}
