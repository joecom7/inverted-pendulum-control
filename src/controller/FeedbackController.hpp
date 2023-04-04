#ifndef FEEDBACK_CONTROLLER_H
#define FEEDBACK_CONTROLLER_H

#include <cstdint>

class FeedbackController {
    private:
        double square_wave_amplitude_pkpk = 0.002;
        uint32_t square_wave_period_microseconds = 1000000;
        double square_wave_mean = 0;
        double square_wave(uint64_t timestamp_microseconds);
    public:
        void set_square_wave_param(double frequency_hz,double amplitude_pkpk_mps,double mean_mps);
        double get_robot_input(uint64_t timestamp_microseconds, double encoder_angle);
};

#endif