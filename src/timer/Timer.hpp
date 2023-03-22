#ifndef TIMER_H
#define TIMER_H

#include <chrono>
#include <cstring>
#include <sched.h>
#include <unistd.h>

/**
 * @brief Si occupa di tempificare il programma e far sì che start_cycle venga chiamata sempre
 *        allo stesso intervallo di tempo.
 * 
 */
class Timer {
    private:
        static uint64_t microseconds();
        const unsigned int TARGET_CYCLE_TIME_MICROSECONDS;
        uint64_t last_cycle_start_microseconds;
    public:
        Timer(unsigned int target_cycle_time_microseconds);
        void start_cycle();
        void end_cycle();
        float get_mean_cycle_time();
        float get_standard_deviation_cycle_time();
};

#endif