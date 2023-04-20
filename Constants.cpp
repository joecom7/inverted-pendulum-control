#include "Constants.hpp"

// commentare per non usare localhost come ip del robot
//#define ROBOT_IP_IS_LOCALHOST

const uint32_t Constants::MONITORING_INTERVAL_MICROSECONDS = 1000;
const uint32_t Constants::TARGET_CYCLE_TIME_MICROSECONDS = 1000;
const uint8_t Constants::ENCODER_CLK_PIN = 17;
const uint8_t Constants::ENCODER_DT_PIN = 18;
const uint16_t Constants::ENCODER_PPR = 1024;
const uint8_t Constants::ENCODER_START_ANGLE_DEGREES = 0;
const float Constants::DELAY_FEEDBACK_GAIN =2;
const float Constants::AGGRESSIVE_DELAY_FEEDBACK_GAIN =20;
const bool Constants::BYPASS_ROBOT = true;
const bool Constants::ACTIVATE_ROBOT_FEEDBACK = true;
const bool Constants::TIMER_AGGRESSIVE_MODE = true;
const double Constants::ROBOT_POS_LIMIT = 0.5;
const double Constants::SQUARE_WAVE_AMPLITUDE_PKPK_MPS = 0.7;
const double Constants::SQUARE_WAVE_FREQUENCY_HZ = 2;
const double Constants::SQUARE_WAVE_MEAN_MPS = 0;

#ifdef ROBOT_IP_IS_LOCALHOST
const char* Constants::ROBOT_IP = "localhost";
#endif

#ifndef ROBOT_IP_IS_LOCALHOST
const char* Constants::ROBOT_IP = "192.168.2.103";
#endif

const char* Constants::LOGFILE_NAME = "pendulum.csv";
