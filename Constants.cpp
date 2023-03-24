#include "Constants.hpp"

// commentare per non usare localhost come ip del robot
#define ROBOT_IP_IS_LOCALHOST

const uint32_t Constants::TARGET_CYCLE_TIME_MICROSECONDS = 5000;
const uint8_t Constants::ENCODER_CLK_PIN = 17;
const uint8_t Constants::ENCODER_DT_PIN = 18;
const uint16_t Constants::ENCODER_PPR = 1024;
const uint8_t Constants::ENCODER_START_ANGLE_DEGREES = 0;
const float Constants::DELAY_FEEDBACK_GAIN =1.1;
const float Constants::AGGRESSIVE_DELAY_FEEDBACK_GAIN =40;
const bool Constants::BYPASS_ROBOT = false;
const bool Constants::TIMER_AGGRESSIVE_MODE = false;

#ifdef ROBOT_IP_IS_LOCALHOST
const char* Constants::ROBOT_IP = "localhost";
#endif

#ifndef ROBOT_IP_IS_LOCALHOST
const char* Constants::ROBOT_IP = "192.168.0.100";
#endif