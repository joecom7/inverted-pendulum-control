#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <cstdint>

#define ROBOT_CONF 1,1,-1
#define STARTING_ROBOT_POSITION 0,-240,190
#define STARTING_ROBOT_ORIENTATION 90,0,0

namespace Constants {
    extern const uint32_t TARGET_CYCLE_TIME_MICROSECONDS;
    extern const uint8_t ENCODER_CLK_PIN;
    extern const uint8_t ENCODER_DT_PIN;
    extern const uint16_t ENCODER_PPR;
    extern const uint8_t ENCODER_START_ANGLE_DEGREES;
    extern const float DELAY_FEEDBACK_GAIN;
    extern const float AGGRESSIVE_DELAY_FEEDBACK_GAIN;
    extern const double ROBOT_POS_LIMIT;
    extern const uint32_t MONITORING_INTERVAL_MICROSECONDS;
    extern const bool BYPASS_ROBOT;
    extern const bool TIMER_AGGRESSIVE_MODE;
    extern const bool ACTIVATE_ROBOT_FEEDBACK;
    extern const char* ROBOT_IP;
    extern const char* LOGFILE_NAME;
    extern const double SQUARE_WAVE_AMPLITUDE_PKPK_MPS;
    extern const double SQUARE_WAVE_FREQUENCY_HZ;
    extern const double SQUARE_WAVE_MEAN_MPS;
}

#endif