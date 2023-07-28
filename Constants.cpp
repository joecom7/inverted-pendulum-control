#include "Constants.hpp"

// commentare per non usare localhost come ip del robot
//#define ROBOT_IP_IS_LOCALHOST

const uint32_t Constants::MONITORING_INTERVAL_MICROSECONDS = 2000;
const uint32_t Constants::TARGET_CYCLE_TIME_MICROSECONDS = 2000;
const uint8_t Constants::ENCODER_CLK_PIN = 17;
const uint8_t Constants::ENCODER_DT_PIN = 18;
const uint16_t Constants::ENCODER_PPR = 1024;
const uint8_t Constants::ENCODER_START_ANGLE_DEGREES = 180;
const float Constants::DELAY_FEEDBACK_GAIN =2;
const float Constants::AGGRESSIVE_DELAY_FEEDBACK_GAIN =20;
const bool Constants::BYPASS_ROBOT = true;
const bool Constants::ACTIVATE_ROBOT_FEEDBACK = true;
const bool Constants::TIMER_AGGRESSIVE_MODE = false;
const bool Constants::ACTIVATE_ROBOT_CALIBRATION = false;
const double Constants::ROBOT_POS_LIMIT = 0.2;
const double Constants::SQUARE_WAVE_AMPLITUDE_PKPK_MPS = 0.05;
const double Constants::SQUARE_WAVE_FREQUENCY_HZ = 2;
const double Constants::SQUARE_WAVE_MEAN_MPS = 0;

#ifdef ROBOT_IP_IS_LOCALHOST
const char* Constants::ROBOT_IP = "localhost";
#endif

#ifndef ROBOT_IP_IS_LOCALHOST
const char* Constants::ROBOT_IP = "192.168.2.103";
#endif

const char* Constants::LOGFILE_NAME = "pendulum.csv";
const control_type_t Constants::CONTROL_TYPE = SQUARE;
const double Constants::CHIRP_F0_HZ = 0.01;
const double Constants::CHIRP_K = 0.4;
const double Constants::CHIRP_APKPK_MPS = 0.2;
char Constants::NETWORK_INTERFACE[] = "eth0";

const float Constants::ROBOT_BLENDING_PERCENTAGE = 0.0;
const float Constants::ROBOT_ACCELERATION_LIMIT = 100.0;

const double Constants::START_CONTROL_ANGLE_DEGREES = 2.0;
const double Constants::STOP_CONTROL_ANGLE_DEGREES = 60.0;
