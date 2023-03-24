#include <iostream>
#include "src/encoder/Encoder.hpp"
#include "src/timer/Timer.hpp"
#include "Constants.hpp"
#include <inttypes.h>
#include "src/robot/Robot.hpp"
#include "signal.h"

Robot robot(Constants::BYPASS_ROBOT);

void cleanup(int signum) {
    robot.deactivate();
    robot.disconnect();
    printf("ho ricevuto il segnale %d. chiudo...\n" , signum);
    exit(0);
}


int main() {
    Timer timer(Constants::TARGET_CYCLE_TIME_MICROSECONDS , Constants::DELAY_FEEDBACK_GAIN);
    Encoder encoder(Constants::ENCODER_CLK_PIN, 
        Constants::ENCODER_DT_PIN, Constants::ENCODER_PPR,
        Constants::ENCODER_START_ANGLE_DEGREES);
    signal(2,cleanup);
    robot.connect();
    robot.activate();
    robot.home();
    while(true) {
        timer.start_cycle();
        printf("enc_angle=%-10.3f mean time=%-10.3f sigma_time=%-10.3f max_time=%-10u min_time=%-10u\n" , 
            encoder.get_angle_degrees() , timer.get_mean_cycle_time(),
            timer.get_standard_deviation_cycle_time() , timer.get_max_cycle_time(),
            timer.get_min_cycle_time());
        robot.print_number(timer.get_mean_cycle_time());
        timer.end_cycle();
    }
}