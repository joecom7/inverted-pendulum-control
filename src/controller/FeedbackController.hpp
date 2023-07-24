#ifndef FEEDBACK_CONTROLLER_H
#define FEEDBACK_CONTROLLER_H

#include <cstdint>

typedef enum
{
    SQUARE,
    CHIRP,
    CONTROLLER,
    NO_FEEDBACK,
} control_type_t;

class FeedbackController
{
private:
    control_type_t control_type;
    double square_wave_amplitude_pkpk = 0.002;
    uint32_t square_wave_period_microseconds = 1000000;
    double square_wave_mean = 0;
    double chirp_f0 = 0.1;
    double chirp_k = 1;
    double chirp_apkpk = 0.002;
    uint32_t SQUARE_WAVE_RAISE_TIME_MICROSECONDS = 0;
    double SQUARE_WAVE_SLOPE;
    double square_wave(uint64_t timestamp_microseconds);
    double controller(uint64_t timestamp_microseconds, double encoder_angle);
    double chirp(uint64_t timestamp_microseconds);

public:
    void square_wave_w(float* w,uint64_t timestamp_microseconds);
    FeedbackController(control_type_t control_type_in);
    void set_square_wave_param(double frequency_hz, double amplitude_pkpk_mps, double mean_mps);
    void set_chirp_param(double f0_hz, double k , double apkpk_mps);
    double get_robot_input(uint64_t timestamp_microseconds, double encoder_angle);
};

#endif