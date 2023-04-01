#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <cstdint>

namespace Constants {
    extern const uint32_t TARGET_CYCLE_TIME_MICROSECONDS;
    extern const uint8_t ENCODER_CLK_PIN;
    extern const uint8_t ENCODER_DT_PIN;
    extern const uint16_t ENCODER_PPR;
    extern const uint8_t ENCODER_START_ANGLE_DEGREES;
    extern const float DELAY_FEEDBACK_GAIN;
    extern const float AGGRESSIVE_DELAY_FEEDBACK_GAIN;
    extern const bool BYPASS_ROBOT;
    extern const bool TIMER_AGGRESSIVE_MODE;
    extern const char* ROBOT_IP;
    extern const char* LOGFILE_NAME;
}

#endif