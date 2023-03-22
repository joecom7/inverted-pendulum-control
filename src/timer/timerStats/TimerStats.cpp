#include "TimerStats.hpp"

TimerStats::TimerStats(uint32_t base_cycle_time) : BASE_CYCLE_TIME(base_cycle_time){
    sum_cycle_times = 0;
    quadratic_sum_cycle_times = 0;
    max_cycle_time = 0;
    min_cycle_time = UINT32_MAX;
    cycles = 0;
}

void TimerStats::set_last_cycle_time(uint32_t last_cycle_time_microseconds) {
    sum_cycle_times += last_cycle_time_microseconds;
    quadratic_sum_cycle_times += pow((float)last_cycle_time_microseconds,2);
    if(last_cycle_time_microseconds > max_cycle_time)
        max_cycle_time = last_cycle_time_microseconds;
    if(last_cycle_time_microseconds < min_cycle_time)
        min_cycle_time = last_cycle_time_microseconds;
    cycles++;
}

float TimerStats::get_mean() {
    if(cycles == 0)
        return BASE_CYCLE_TIME;
    return sum_cycle_times / cycles;
}

float TimerStats::get_standard_deviation() {
    if(cycles == 0)
        return 0;
    return sqrt(quadratic_sum_cycle_times/cycles - pow(get_mean(),2));
}

uint32_t TimerStats::get_max() {
    return max_cycle_time;
}

uint32_t TimerStats::get_min() {
    return min_cycle_time;
}