#include <iostream>
#include <unistd.h>
#include <pigpio.h>
#include <cstring>

#include "encoder/Encoder.hpp"
#include <chrono>
#include "constants.h"

uint64_t micros()
{
    uint64_t us = std::chrono::duration_cast<std::chrono::microseconds>(
            std::chrono::high_resolution_clock::now().time_since_epoch())
            .count();
    return us; 
}

int main() {
    uint64_t start_time , end_time, microseconds_last_cycle, microseconds_current_cycle;
    struct sched_param sp;
    microseconds_last_cycle = micros();
    usleep(TEMPO_CICLO_DESIDERATO_MICROSECONDI);
    microseconds_current_cycle = micros();
    memset( &sp, 0, sizeof(sp) );
    sp.sched_priority = 99;
    sched_setscheduler( 0, SCHED_FIFO, &sp );
    if (gpioInitialise() < 0) return 1;
    Encoder enc(17, 18, 1024,0);
    while(true) {
        start_time = micros();
        std::cout << "pos=" << enc.get_angle_degrees() << "," << microseconds_current_cycle - microseconds_last_cycle << std::endl;
        microseconds_last_cycle = microseconds_current_cycle;
        microseconds_current_cycle = micros();
        end_time = micros();
        usleep(TEMPO_CICLO_DESIDERATO_MICROSECONDI - (end_time-start_time));
    }
}