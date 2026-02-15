#include "video_participant.h"

VideoParticipant::VideoParticipant(int id, const std::string& name, const std::string& contentPath, ParticipantType type)
    : id_(id), name_(name), contentPath_(contentPath), type_(type), position_() {
}
