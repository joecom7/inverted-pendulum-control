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
        const unsigned int TARGET_CYCLE_TIME_MICROSECONDS;
        const float DELAY_FEEDBACK_GAIN;
        const bool AGGRESSIVE_MODE;
        uint64_t current_cycle_start_microseconds;
        TimerStats time_stats;
        static uint64_t microseconds();
    public:
        Timer(unsigned int target_cycle_time_microseconds , float delay_feedback_gain, bool aggressive_mode);
        void start_cycle();
        void end_cycle();
        double get_mean_cycle_time();
        double get_standard_deviation_cycle_time();
        uint32_t get_max_cycle_time();
        uint32_t get_min_cycle_time();
};

#endif