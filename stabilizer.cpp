#include <iostream>
#include "src/encoder/Encoder.hpp"
#include "src/timer/Timer.hpp"
#include "Constants.hpp"
#include <inttypes.h>
#include "src/robot/Robot.hpp"
#include "src/csvlogger/CsvLogger.hpp"
#include "signal.h"

Robot robot(Constants::ROBOT_IP , Constants::BYPASS_ROBOT);
CsvLogger csvLogger(Constants::LOGFILE_NAME);

void cleanup(int signum) {
    robot.deactivate();
    robot.disconnect();
    csvLogger.close();
    printf("ho ricevuto il segnale %d. termino...\n" , signum);
    exit(0);
}


int main() {
    float delay_feedback_gain;
    if(Constants::TIMER_AGGRESSIVE_MODE) {
        delay_feedback_gain = Constants::AGGRESSIVE_DELAY_FEEDBACK_GAIN;
    }
    else {
        delay_feedback_gain = Constants::DELAY_FEEDBACK_GAIN;
    }
    signal(2,cleanup);
    robot.connect();
    robot.activate();
    robot.home();
    Timer timer(Constants::TARGET_CYCLE_TIME_MICROSECONDS , delay_feedback_gain,
        Constants::TIMER_AGGRESSIVE_MODE);
    Encoder encoder(Constants::ENCODER_CLK_PIN, 
        Constants::ENCODER_DT_PIN, Constants::ENCODER_PPR,
        Constants::ENCODER_START_ANGLE_DEGREES);
    
    while(true) {
        timer.start_cycle();
        double current_encoder_angle = encoder.get_angle();
        /*
            Qui andranno le operazioni da eseguire in ciclo
        */

       csvLogger << current_encoder_angle;

        printf("\nenc_angle=%-10.3f mean time=%-10.3f sigma_time=%-10.3f max_time=%-10u min_time=%-10u\n\n" , 
            current_encoder_angle , timer.get_mean_cycle_time(),
            timer.get_standard_deviation_cycle_time() , timer.get_max_cycle_time(),
            timer.get_min_cycle_time());

        /*
            Fine operazioni
        */

        csvLogger.end_row();
        timer.end_cycle();
    }
}