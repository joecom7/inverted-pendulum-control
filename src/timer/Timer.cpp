#include "Timer.hpp"

Timer::Timer(unsigned int target_cycle_time_microseconds) : 
    TARGET_CYCLE_TIME_MICROSECONDS(target_cycle_time_microseconds){
        struct sched_param sp;
        memset( &sp, 0, sizeof(sp) );
        sp.sched_priority = 99;
        sched_setscheduler( 0, SCHED_FIFO, &sp );
        last_cycle_start_microseconds = Timer::microseconds();
}

uint64_t Timer::microseconds() {
    uint64_t us = std::chrono::duration_cast<std::chrono::microseconds>(
            std::chrono::high_resolution_clock::now().time_since_epoch())
            .count();
    return us; 
}

void Timer::start_cycle() {
    last_cycle_start_microseconds = Timer::microseconds();
}

void Timer::end_cycle() {
    usleep(TARGET_CYCLE_TIME_MICROSECONDS - (Timer::microseconds()-last_cycle_start_microseconds));
}