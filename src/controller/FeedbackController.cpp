#include "FeedbackController.hpp"

double FeedbackController::get_robot_input(uint64_t timestamp_microseconds, double encoder_angle) {
    return square_wave(timestamp_microseconds);
    //if(encoder_angle<-1||encoder_angle>1)
    //    return 0;
    //else if(encoder_angle>-0.08&&encoder_angle<0.08) {
    //    return 0;
    //}
    //else {
    //    return -0.5*encoder_angle;
    //}
}

double FeedbackController::square_wave(uint64_t timestamp_microseconds) {
    if((timestamp_microseconds%square_wave_period_microseconds)<(square_wave_period_microseconds/2)) {
        return square_wave_mean + square_wave_amplitude_pkpk/2;
    }
    return square_wave_mean - square_wave_amplitude_pkpk/2;
}

void FeedbackController::set_square_wave_param(double frequency_hz,double amplitude_pkpk_mps,double mean_mps) {
    square_wave_period_microseconds = (uint32_t)(1000000.0/frequency_hz);
    square_wave_amplitude_pkpk = amplitude_pkpk_mps;
    square_wave_mean = mean_mps;
}