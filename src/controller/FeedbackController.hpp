#ifndef FEEDBACK_CONTROLLER_H
#define FEEDBACK_CONTROLLER_H

#include <cstdint>

class FeedbackController {
    private:
        double square_wave(uint64_t timestamp_microseconds);
    public:
        double get_robot_input(uint64_t timestamp_microseconds, double encoder_angle);
};

#endif