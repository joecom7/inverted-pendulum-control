#ifndef FEEDBACK_CONTROLLER_H
#define FEEDBACK_CONTROLLER_H

#define BUFFER_LENGTH 7

#include <cstdint>

typedef enum
{
    SQUARE,
    CHIRP,
    COSINE,
    CONTROLLER,
    NO_FEEDBACK,
} control_type_t;

typedef enum {
    CONTROL_RANGE,
    OUT_OF_RANGE,
} pendulum_state_t;

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
    const double START_CONTROL_ANGLE,STOP_CONTROL_ANGLE;
    uint32_t SQUARE_WAVE_RAISE_TIME_MICROSECONDS = 0;
    double SQUARE_WAVE_SLOPE;
    pendulum_state_t pendulum_state;
    double square_wave(uint64_t timestamp_microseconds);
    double controller(uint64_t timestamp_microseconds, double encoder_angle,double encoder_omega,double robot_pos,double robot_speed);
    double chirp(uint64_t timestamp_microseconds);
    double cosine(uint64_t timestamp_microseconds);
    void update_pendulum_state(double encoder_angle);
    double accel_control(uint64_t timestamp_microseconds, double encoder_angle,double encoder_omega,double robot_pos,double robot_speed);
    double vel_control(uint64_t timestamp_microseconds, double encoder_angle,double encoder_omega,double robot_pos,double robot_speed);
    double accel_buffer[BUFFER_LENGTH];
    uint32_t accel_buffer_index = 0;

    const double CYCLE_TIME;

    double last_vel = 0.0;
    double last_accel = 0.0;

    double accel_saturation = 7.0;
    double velocity_saturation = 0.3;
    double K_GAIN[BUFFER_LENGTH+5] = {34.497856064307448775707598542795,5.626635692536232191685030556982,-2.158461824680640184226376732113,-2.864100425476022504511774968705,0.247894084694377125899222846783,0.070654815434678136520929569997,0.073176767236236117675218793011,0.075776813957601482640846768390,0.078457435319047322264118804469,0.081221157590652542923592704938};


public:
    void square_wave_w(float* w,uint64_t timestamp_microseconds);
    FeedbackController(control_type_t control_type_in,double start_control_angle_degrees,double stop_control_angle_degrees,uint32_t target_cycle_time_microseconds);
    void set_square_wave_param(double frequency_hz, double amplitude_pkpk_mps, double mean_mps);
    void set_chirp_param(double f0_hz, double k , double apkpk_mps);
    double get_robot_input(uint64_t timestamp_microseconds, double encoder_angle,double encoder_omega,double robot_pos,double robot_speed);
};

#endif