#include "FeedbackController.hpp"
#include <iostream>
#include <string.h>
#include <cmath>

FeedbackController::FeedbackController(control_type_t control_type_in, double start_control_angle_degrees, double stop_control_angle_degrees, uint32_t target_cycle_time_microseconds) : START_CONTROL_ANGLE(start_control_angle_degrees * M_PI / 180.0), STOP_CONTROL_ANGLE(stop_control_angle_degrees * M_PI / 180.0),
                                                                                                                                                                                         CYCLE_TIME(target_cycle_time_microseconds * 1e-6)
{
    std::cout << "cycle time = " << CYCLE_TIME;
    control_type = control_type_in;
    SQUARE_WAVE_SLOPE = square_wave_amplitude_pkpk / SQUARE_WAVE_RAISE_TIME_MICROSECONDS;
    pendulum_state = OUT_OF_RANGE;
}

double FeedbackController::get_robot_input(uint64_t timestamp_microseconds, double encoder_angle, double encoder_omega, double robot_pos, double robot_speed)
{
    switch (control_type)
    {
    case SQUARE:
        return square_wave(timestamp_microseconds);
        break;
    case CONTROLLER:
        return controller(timestamp_microseconds, encoder_angle, encoder_omega, robot_pos, robot_speed);
        break;
    case CHIRP:
        return chirp(timestamp_microseconds);
        break;
    case COSINE:
        return cosine(timestamp_microseconds);
        break;
    case NO_FEEDBACK:
        return 0;
        break;
    default:
        return 0;
        break;
    }
}

double FeedbackController::controller(uint64_t timestamp_microseconds, double encoder_angle, double encoder_omega, double robot_pos, double robot_speed)
{

    update_pendulum_state(encoder_angle);

    switch (pendulum_state)
    {
    case CONTROL_RANGE:
        return vel_control(timestamp_microseconds, encoder_angle, encoder_omega, robot_pos, robot_speed);
        break;
    case OUT_OF_RANGE:
        return 0;
        break;
    default:
        return 0;
        break;
    }
}

double FeedbackController::vel_control(uint64_t timestamp_microseconds, double encoder_angle, double encoder_omega, double robot_pos, double robot_speed)
{
    double accel = accel_control(timestamp_microseconds, encoder_angle, encoder_omega, robot_pos, robot_speed);
    double velocity = 0;

    // velocity = last_vel + (CYCLE_TIME)*0.5*accel + (CYCLE_TIME)*0.5*last_accel;
    velocity = last_vel + (CYCLE_TIME)*last_accel;
    last_vel = velocity;
    last_accel = accel;

    if (velocity > velocity_saturation)
    {
        velocity = velocity_saturation;
    }
    else if (velocity < -velocity_saturation)
    {
        velocity = -velocity_saturation;
    }

    return velocity;
}

double FeedbackController::accel_control(uint64_t timestamp_microseconds, double encoder_angle, double encoder_omega, double robot_pos, double robot_speed)
{
    double accel = 0;

    accel -= K_GAIN[0] * encoder_angle;
    accel -= K_GAIN[1] * encoder_omega;

    accel -= K_GAIN[2] * robot_pos;

    accel -= K_GAIN[3] * robot_speed;

    accel -= K_GAIN[4] * robot_speed;

    for (int i = 0; i < BUFFER_LENGTH; i++)
    {
        accel -= K_GAIN[5 + i] * accel_buffer[(accel_buffer_index + 2 * BUFFER_LENGTH - 1 - i) % BUFFER_LENGTH];
    }

    accel_buffer[(accel_buffer_index++) % BUFFER_LENGTH] = accel;


    if (accel > accel_saturation)
    {
        accel = accel_saturation;
    }
    else if (accel < -accel_saturation)
    {
        accel = -accel_saturation;
    }

    return accel;
}

void FeedbackController::update_pendulum_state(double encoder_angle)
{
    switch (pendulum_state)
    {
    case CONTROL_RANGE:
        if (fabs(encoder_angle) > (STOP_CONTROL_ANGLE))
        {
            pendulum_state = OUT_OF_RANGE;
        }
        break;
    case OUT_OF_RANGE:
        if (fabs(encoder_angle) < START_CONTROL_ANGLE)
        {
            last_vel = 0.0;
            last_accel = 0.0;
            memset(accel_buffer, 0, BUFFER_LENGTH * sizeof(double));
            accel_buffer_index = 0;
            pendulum_state = CONTROL_RANGE;
        }
        break;
    default:
        break;
    }
}

double FeedbackController::chirp(uint64_t timestamp_microseconds)
{
    double t = (float)timestamp_microseconds * 1e-6;
    double f = chirp_f0 + (chirp_k / (2 * M_PI)) * (t);
    return (chirp_apkpk / 2) * cos(2 * M_PI * f * t);
}

double FeedbackController::cosine(uint64_t timestamp_microseconds)
{
    double t = (float)timestamp_microseconds * 1e-6;
    double T = 4;
    double A = 10;
    return -(A - A * cos(2 * M_PI * t / T));
}

double FeedbackController::square_wave(uint64_t timestamp_microseconds)
{
    uint32_t t = timestamp_microseconds % square_wave_period_microseconds;
    if (t < SQUARE_WAVE_RAISE_TIME_MICROSECONDS * 0.5)
    {
        return square_wave_mean + t * 1e-6 * (SQUARE_WAVE_SLOPE);
    }
    if (t < ((square_wave_period_microseconds / 2) - SQUARE_WAVE_RAISE_TIME_MICROSECONDS * 0.5))
    {
        return square_wave_mean + square_wave_amplitude_pkpk / 2;
    }
    if (t < ((square_wave_period_microseconds / 2) + SQUARE_WAVE_RAISE_TIME_MICROSECONDS * 0.5))
    {
        return square_wave_mean - (t - square_wave_period_microseconds * 0.5) * 1e-6 * (SQUARE_WAVE_SLOPE);
    }
    if (t < (square_wave_period_microseconds - SQUARE_WAVE_RAISE_TIME_MICROSECONDS * 0.5))
    {
        return square_wave_mean - square_wave_amplitude_pkpk / 2;
    }
    return 0;
    return square_wave_mean - (t - square_wave_period_microseconds) * 1e-6 * (SQUARE_WAVE_SLOPE);
}

void FeedbackController::square_wave_w(float *w, uint64_t timestamp_microseconds)
{
    if ((timestamp_microseconds % square_wave_period_microseconds) < (square_wave_period_microseconds / 2))
    {
        w[5] = square_wave_mean + square_wave_amplitude_pkpk / 2;
    }
    w[5] = square_wave_mean - square_wave_amplitude_pkpk / 2;
}

void FeedbackController::set_square_wave_param(double frequency_hz, double amplitude_pkpk_mps, double mean_mps)
{
    square_wave_period_microseconds = (uint32_t)(1000000.0 / frequency_hz);
    square_wave_amplitude_pkpk = amplitude_pkpk_mps;
    square_wave_mean = mean_mps;
    SQUARE_WAVE_SLOPE = square_wave_amplitude_pkpk / ((double)SQUARE_WAVE_RAISE_TIME_MICROSECONDS * 1e-6);
}

void FeedbackController::set_chirp_param(double f0_hz, double k, double apkpk_mps)
{
    chirp_apkpk = apkpk_mps;
    chirp_k = k;
    chirp_f0 = f0_hz;
}