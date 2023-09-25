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
    //double K_GAIN[BUFFER_LENGTH+5] = {35.983071320941853343811089871451,7.530853343746892747390120348427,-1.682366834137376976343603018904,-2.661492918275244701220572096645,0.209200573985711429925160587118,0.055119987695542112249302846294,0.054293204348556417526694417575,0.053931584505303109544627204741,0.053931564348266347597160574878,0.054213923139789337024563309342,0.054718087143218969992108924316,0.055397766871932353771601498238};
    double K_GAIN[BUFFER_LENGTH+5] = {34.988004781008100962935714051127,7.322787928030317416983052680735,-1.539681400323275717312299093464,-2.493513437764796503159914209391,0.202997977745248164849201089055,0.051207233882430272520203118347,0.051049018296960145535390296345,0.051192320922144457506774983813,0.051571735760015564864477966012,0.052137265414870549717907266540,0.052850716268670459851986009880,0.053682937479911124745157735560};


public:
    void square_wave_w(float* w,uint64_t timestamp_microseconds);
    FeedbackController(control_type_t control_type_in,double start_control_angle_degrees,double stop_control_angle_degrees,uint32_t target_cycle_time_microseconds);
    void set_square_wave_param(double frequency_hz, double amplitude_pkpk_mps, double mean_mps);
    void set_chirp_param(double f0_hz, double k , double apkpk_mps);
    double get_robot_input(uint64_t timestamp_microseconds, double encoder_angle,double encoder_omega,double robot_pos,double robot_speed);
};

#endif