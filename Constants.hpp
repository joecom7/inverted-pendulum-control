#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <cstdint>

#define ROBOT_CONF 1,1,-1
#define STARTING_ROBOT_POSITION 0,-240,190
#define STARTING_ROBOT_ORIENTATION 90,0,0

namespace Constants {
    /**
     * @brief The desired time per cycle.
     * 
     */
    extern const uint32_t TARGET_CYCLE_TIME_MICROSECONDS;
    /**
     * @brief The GPIO the CLK pin of the encoder is connected to.
     * 
     */
    extern const uint8_t ENCODER_CLK_PIN;
    /**
     * @brief The GPIO the DT pin of the encoder is connected to.
     * 
     */
    extern const uint8_t ENCODER_DT_PIN;
    /**
     * @brief Number of pulses per revolution of the encoder.
     * 
     */
    extern const uint16_t ENCODER_PPR;
    /**
     * @brief The starting angle of the encoder in degrees.
     * 
     * It should be set to 0 if the pendulum starts in the unstable position,
     * to 180 if the pendulum starts in the stable position.
     * 
     */
    extern const uint8_t ENCODER_START_ANGLE_DEGREES;
    /**
     * @brief A gain used to keep the mean cycle time close to the desired cycle time.
     * 
     */
    extern const float DELAY_FEEDBACK_GAIN;
    /**
     * @brief A gain used to keep the mean cycle time close to the desired cycle time when 
     * a timer object is in aggressive mode.
     * 
     */
    extern const float AGGRESSIVE_DELAY_FEEDBACK_GAIN;
    /**
     * @brief Position limit in meters. The Meca500 will not distance itself more than
     * this limit from the starting position.
     * 
     */
    extern const double ROBOT_POS_LIMIT;
    /**
     * @brief The interval in microseconds between two feedbacks of the Meca500.
     * 
     */
    extern const uint32_t MONITORING_INTERVAL_MICROSECONDS;
    /**
     * @brief If set to true the program will not connect to the Meca500.
     * 
     */
    extern const bool BYPASS_ROBOT;
    /**
     * @brief If set to true the timer object will be in aggressive mode and
     * will wait much less than the desired cycle time and busy wait until the
     * desired cycle time has passed.
     * 
     */
    extern const bool TIMER_AGGRESSIVE_MODE;
    /**
     * @brief If set to true the Meca500 feedback of speed and position is collected.
     * 
     */
    extern const bool ACTIVATE_ROBOT_FEEDBACK;
    /**
     * @brief Meca500 IP address.
     * 
     */
    extern const char* ROBOT_IP;
    /**
     * @brief Name of the file in which the logs will be written.
     * 
     */
    extern const char* LOGFILE_NAME;
    /**
     * @brief Peak to peak amplitude in meters per second of the square wave used to collect data on
     * the frequency response of the Meca500.
     * 
     */
    extern const double SQUARE_WAVE_AMPLITUDE_PKPK_MPS;
    /**
     * @brief Frequency in hertz of the square wave used to collect data on
     * the frequency response of the Meca500.
     * 
     */
    extern const double SQUARE_WAVE_FREQUENCY_HZ;
    /**
     * @brief Mean in meters per second of the square wave used to collect data on
     * the frequency response of the Meca500.
     * 
     */
    extern const double SQUARE_WAVE_MEAN_MPS;
}

#endif