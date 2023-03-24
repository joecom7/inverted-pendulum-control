#ifndef TIMER_STATS_H
#define TIMER_STATS_H

#include <cmath>
#include <cstdint>

class TimerStats {
    private:
        const uint32_t BASE_CYCLE_TIME;
        double sum_cycle_times;
        double quadratic_sum_cycle_times;
        uint32_t max_cycle_time;
        uint32_t min_cycle_time;
        int cycles;
    public:
        TimerStats(uint32_t base_cycle_time);
        void set_last_cycle_time(uint32_t last_cycle_time_microseconds);
        float get_mean();
        float get_standard_deviation();
        uint32_t get_max();
        uint32_t get_min();
};

#endif