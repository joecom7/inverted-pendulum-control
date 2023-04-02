#include "FeedbackController.hpp"

#define SQUARE_WAVE_PERIOD_MICROSECONDS 1000000
#define SQUARE_WAVE_AMPLITUDE_PKPK 0.02
#define SQUARE_WAVE_MEAN 0

double FeedbackController::get_robot_input(uint64_t timestamp_microseconds, double encoder_angle) {
    return square_wave(timestamp_microseconds);
}

double FeedbackController::square_wave(uint64_t timestamp_microseconds) {
    if((timestamp_microseconds%SQUARE_WAVE_PERIOD_MICROSECONDS)<(SQUARE_WAVE_PERIOD_MICROSECONDS/2)) {
        return SQUARE_WAVE_MEAN + SQUARE_WAVE_AMPLITUDE_PKPK/2;
    }
    return SQUARE_WAVE_MEAN - SQUARE_WAVE_AMPLITUDE_PKPK/2;
}