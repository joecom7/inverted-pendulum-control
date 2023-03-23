#include <iostream>
#include "src/encoder/Encoder.hpp"
#include "src/timer/Timer.hpp"
#include "constants.hpp"
#include <inttypes.h>
#include "src/mecawrapper/mecawrapper.h"

void py_init() {
    PyImport_AppendInittab("mecawrapper", PyInit_mecawrapper);
    Py_Initialize();
    PyImport_ImportModule("mecawrapper");
}


int main() {
    py_init();
    PyImport_ImportModule("mecawrapper");
    Timer timer(TARGET_CYCLE_TIME_MICROSECONDS , DELAY_FEEDBACK_GAIN);
    Encoder encoder(ENCODER_CLK_PIN, ENCODER_DT_PIN, ENCODER_PPR,ENCODER_START_ANGLE_DEGREES);
    print_velocity(36e+4);
    print_velocity(2);
    while(true) {
        timer.start_cycle();
        printf("enc_angle=%-10.3f mean time=%-10.3f sigma_time=%-10.3f max_time=%-10u min_time=%-10u\n" , 
            encoder.get_angle_degrees() , timer.get_mean_cycle_time(),
            timer.get_standard_deviation_cycle_time() , timer.get_max_cycle_time(),
            timer.get_min_cycle_time());
        print_velocity(timer.get_mean_cycle_time());
        timer.end_cycle();
    }
}