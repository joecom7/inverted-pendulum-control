#ifndef FEEDBACK_CONTROLLER_H
#define FEEDBACK_CONTROLLER_H

#define BUFFER_LENGTH 50

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
    double K_GAIN[BUFFER_LENGTH+5] = {52.904320109345825073887681355700,9.644100325322893851875960535835,-0.441963810061965722830734648596,-1.168768664398218026789777468366,1.256478888704065877135462869774,0.023607245959866407503113805433,0.023887175710113606880158343415,0.024170180573886645980508092180,0.024456294124274410156649750547,0.024745550300803526022619394098,0.025037983413361705742339324843,0.025333628146343878384527670278,0.025632519562646373378900221951,0.025934693107932192679410121627,0.026240184614485784903026299730,0.026549030305932412587832658346,0.026861266801176356122660138226,0.027176931118722366664819389825,0.027496060681249798879344226066,0.027818693319862763252414694648,0.028144867278446037556216552389,0.028474621218236011166435872610,0.028807994222756764385318106747,0.029145025802031675199454952008,0.029485755897204174447168156803,0.029830224885328542050455524759,0.030178473584220256303867202519,0.030530543258476882523266837666,0.030886475620389989421576970585,0.031246312839791675480816124377,0.031610097546165567827802789225,0.031977872834480156050140919888,0.032349682269263616007926032125,0.032725569892362867119306457653,0.033105580223743949486170379259,0.033489758271518899057639373495,0.033878149535410059800533133512,0.034270800009366435279201112962,0.034667756191533789789094299749,0.035069065086530883113002943219,0.035474774212622024738283954548,0.035884931606607363419048084552,0.036299585829829325578366194804,0.036718785973565815083929919638,0.037142581664756299519858373515,0.037571023072090495298347434527,0.038004160912674868344485190619,0.038442046456220270445047759722,0.038884731532624179639601180725,0.039332268537657075202496059774,0.039784710438750744976310613765,0.040242110781717549405911427129,0.040704523696905227370113067309,0.041172003905449332250565674940,0.041644606725835198179908047678};


public:
    void square_wave_w(float* w,uint64_t timestamp_microseconds);
    FeedbackController(control_type_t control_type_in,double start_control_angle_degrees,double stop_control_angle_degrees,uint32_t target_cycle_time_microseconds);
    void set_square_wave_param(double frequency_hz, double amplitude_pkpk_mps, double mean_mps);
    void set_chirp_param(double f0_hz, double k , double apkpk_mps);
    double get_robot_input(uint64_t timestamp_microseconds, double encoder_angle,double encoder_omega,double robot_pos,double robot_speed);
};

#endif