#include <iostream>
#include "src/encoder/Encoder.hpp"
#include "src/timer/Timer.hpp"
#include "Constants.hpp"
#include <inttypes.h>
#include "src/robot/Robot.hpp"
#include "src/csvlogger/CsvLogger.hpp"
#include "src/controller/FeedbackController.hpp"
#include "signal.h"

Robot robot(Constants::ROBOT_IP , Constants::ROBOT_POS_LIMIT, Constants::BYPASS_ROBOT);
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

    Encoder encoder(Constants::ENCODER_CLK_PIN, 
        Constants::ENCODER_DT_PIN, Constants::ENCODER_PPR,
        Constants::ENCODER_START_ANGLE_DEGREES);

    Timer timer(Constants::TARGET_CYCLE_TIME_MICROSECONDS , delay_feedback_gain,
        Constants::TIMER_AGGRESSIVE_MODE);

    double current_encoder_angle, current_robot_velocity , new_robot_input_velocity;
    double current_robot_position;
    uint64_t timestamp_microseconds;

    signal(2,cleanup);
    signal(6,cleanup);

    FeedbackController feedbackController;

    // Comandi per il setup del robot
    robot.connect();
    robot.activate();
    robot.home();
    robot.set_conf(ROBOT_CONF);
    robot.move_pose(STARTING_ROBOT_POSITION,STARTING_ROBOT_ORIENTATION);
    robot.set_monitoring_interval(Constants::MONITORING_INTERVAL_MICROSECONDS);
    timestamp_microseconds = timer.get_microseconds_from_program_start();
    current_encoder_angle = encoder.get_angle();
    current_robot_velocity = robot.get_velocity();
    current_robot_position = robot.get_position();
    new_robot_input_velocity = feedbackController.get_robot_input(timestamp_microseconds,current_encoder_angle);
    robot.move_lin_vel_trf(0.0);

    timer.set_starting_timestamp();
    
    while(true) {
        timer.start_cycle();

        /*
            Qui andranno le operazioni da eseguire in ciclo
        */

        timestamp_microseconds = timer.get_microseconds_from_program_start();
        current_encoder_angle = encoder.get_angle();
        current_robot_velocity = robot.get_velocity();
        current_robot_position = robot.get_position();
        new_robot_input_velocity = feedbackController.get_robot_input(timestamp_microseconds,current_encoder_angle);

        robot.move_lin_vel_trf(new_robot_input_velocity);

        csvLogger << (double)timestamp_microseconds*1e-6;
        csvLogger << current_encoder_angle;
        csvLogger << new_robot_input_velocity;
        csvLogger << current_robot_velocity;
        csvLogger << current_robot_position;

        //printf("\nenc_angle=%-10.3f mean time=%-10.3f sigma_time=%-10.3f robot_velocity=%-10.3f\n\n" , 
        //    current_encoder_angle , timer.get_mean_cycle_time(),
        //    timer.get_standard_deviation_cycle_time() , current_robot_velocity);

        /*
            Fine operazioni
        */

        csvLogger.end_row();
        timer.end_cycle();
    }
}