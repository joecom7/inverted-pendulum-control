#ifndef TIMER_H
#define TIMER_H

#include <chrono>
#include <cstring>
#include <sched.h>
#include <unistd.h>
#include "timerStats/TimerStats.hpp"

/**
 * @brief Si occupa di tempificare il programma e far sì che start_cycle venga chiamata sempre
 *        allo stesso intervallo di tempo.
 * 
 */
class Timer {
    private:
        const uint32_t TARGET_CYCLE_TIME_MICROSECONDS;
        const float DELAY_FEEDBACK_GAIN;
        const bool AGGRESSIVE_MODE;
        uint64_t PROGRAM_START_MICROSECONDS;
        uint64_t current_cycle_start_microseconds;
        TimerStats time_stats;
        static uint64_t microseconds();
        uint32_t time_to_sleep();
    public:
        Timer(uint32_t target_cycle_time_microseconds , float delay_feedback_gain, bool aggressive_mode);
        void set_starting_timestamp();
        void start_cycle();
        void end_cycle();
        double get_mean_cycle_time();
        double get_standard_deviation_cycle_time();
        uint32_t get_max_cycle_time();
        uint32_t get_min_cycle_time();
        double get_seconds_from_program_start();
        uint64_t get_microseconds_from_program_start();
};

#endif