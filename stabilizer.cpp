#include <iostream>
#include "src/encoder/Encoder.hpp"
#include "src/timer/Timer.hpp"
#include "constants.h"
#include <inttypes.h>


int main() {
    Timer timer(TARGET_CYCLE_TIME_MICROSECONDS);
    Encoder encoder(ENCODER_CLK_PIN, ENCODER_DT_PIN, ENCODER_PPR,ENCODER_START_ANGLE_DEGREES);
    while(true) {
        timer.start_cycle();
        printf("enc_angle=%-10.3f mean time=%-10.3f sigma_time=%-10.3f max_time=%-10u min_time=%-10u\n" , 
            encoder.get_angle_degrees() , timer.get_mean_cycle_time(),
            timer.get_standard_deviation_cycle_time() , timer.get_max_cycle_time(),
            timer.get_min_cycle_time() );
        timer.end_cycle();
    }
}