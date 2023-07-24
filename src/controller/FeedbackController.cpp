#include "FeedbackController.hpp"
#include <cmath>

FeedbackController::FeedbackController(control_type_t control_type_in)
{
    control_type = control_type_in;
    SQUARE_WAVE_SLOPE = square_wave_amplitude_pkpk/SQUARE_WAVE_RAISE_TIME_MICROSECONDS;
}

double FeedbackController::get_robot_input(uint64_t timestamp_microseconds, double encoder_angle)
{
    switch (control_type)
    {
    case SQUARE:
        return square_wave(timestamp_microseconds);
        break;
    case CONTROLLER:
        return controller(timestamp_microseconds, encoder_angle);
        break;
    case CHIRP:
        return chirp(timestamp_microseconds);
        break;
    case NO_FEEDBACK:
        return 0;
        break;
    }
}

double FeedbackController::controller(uint64_t timestamp_microseconds, double encoder_angle)
{
    if (encoder_angle < -1 || encoder_angle > 1)
        return 0;
    else
    {
        return -0.5 * encoder_angle;
    }
}

double FeedbackController::chirp(uint64_t timestamp_microseconds)
{
    double t = (float)timestamp_microseconds * 1e-6;
    double f = chirp_f0 + (chirp_k / (2 * M_PI)) * (t);
    return (chirp_apkpk / 2) * cos(2 * M_PI * f * t);
}

double FeedbackController::square_wave(uint64_t timestamp_microseconds)
{
    uint32_t t = timestamp_microseconds % square_wave_period_microseconds;
    if(t<SQUARE_WAVE_RAISE_TIME_MICROSECONDS*0.5) {
        return square_wave_mean + t*1e-6*(SQUARE_WAVE_SLOPE);
    }
    if (t < ((square_wave_period_microseconds / 2)-SQUARE_WAVE_RAISE_TIME_MICROSECONDS*0.5))
    {
        return square_wave_mean + square_wave_amplitude_pkpk / 2;
    }
    if(t < ((square_wave_period_microseconds / 2)+SQUARE_WAVE_RAISE_TIME_MICROSECONDS*0.5))
    {
        return square_wave_mean - (t-square_wave_period_microseconds*0.5)*1e-6*(SQUARE_WAVE_SLOPE);
    }
    if(t < (square_wave_period_microseconds - SQUARE_WAVE_RAISE_TIME_MICROSECONDS*0.5))
    {
        return square_wave_mean - square_wave_amplitude_pkpk / 2;
    }
    return 0;
    return square_wave_mean - (t-square_wave_period_microseconds)*1e-6*(SQUARE_WAVE_SLOPE);
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
    SQUARE_WAVE_SLOPE = square_wave_amplitude_pkpk/((double)SQUARE_WAVE_RAISE_TIME_MICROSECONDS*1e-6);
}

void FeedbackController::set_chirp_param(double f0_hz, double k, double apkpk_mps)
{
    chirp_apkpk = apkpk_mps;
    chirp_k = k;
    chirp_f0 = f0_hz;
}