#ifndef VIDEO_PARTICIPANT_H
#define VIDEO_PARTICIPANT_H

#include <string>

struct VideoPosition {
    int x;
    int y;
    int width;
    int height;

    VideoPosition() : x(0), y(0), width(0), height(0) {}
    VideoPosition(int x_, int y_, int w, int h) : x(x_), y(y_), width(w), height(h) {}
};

class VideoParticipant {
public:
    VideoParticipant(int id, const std::string& name, const std::string& videoPath);

    int getId() const { return id_; }
    std::string getName() const { return name_; }
    std::string getVideoPath() const { return videoPath_; }
    VideoPosition getPosition() const { return position_; }

    void setPosition(const VideoPosition& pos) { position_ = pos; }
    void setName(const std::string& name) { name_ = name; }

private:
    int id_;
    std::string name_;
    std::string videoPath_;
    VideoPosition position_;
};

#endif // VIDEO_PARTICIPANT_H
