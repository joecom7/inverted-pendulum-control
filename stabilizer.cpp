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

bool program_terminated = false;

void cleanup(int) {
    program_terminated = true;
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
    double current_robot_position,current_robot_ctrl_speed,robot_ctrl_speed_timestamp,speed_timestamp,pos_timestamp;
    uint64_t timestamp_microseconds;

    signal(2,cleanup);
    signal(6,cleanup);

    FeedbackController feedbackController;
    feedbackController.set_square_wave_param(Constants::SQUARE_WAVE_FREQUENCY_HZ,
                                            Constants::SQUARE_WAVE_AMPLITUDE_PKPK_MPS,
                                            Constants::SQUARE_WAVE_MEAN_MPS);

    // robot setup
    robot.connect(Constants::ACTIVATE_ROBOT_FEEDBACK);
    robot.activate();
    robot.home();
    robot.set_conf(ROBOT_CONF);
    robot.move_pose(STARTING_ROBOT_POSITION,STARTING_ROBOT_ORIENTATION);
    robot.set_monitoring_interval(Constants::MONITORING_INTERVAL_MICROSECONDS);

    timer.set_starting_timestamp();
    
    while(!program_terminated) {
        timer.start_cycle();

        /*
            tasks to execute in loop
        */

        timestamp_microseconds = timer.get_microseconds_from_program_start();
        current_encoder_angle = encoder.get_angle();
        new_robot_input_velocity = feedbackController.get_robot_input(timestamp_microseconds,current_encoder_angle);

        robot.move_lin_vel_trf(new_robot_input_velocity);

        current_robot_ctrl_speed = robot.get_target_velocity();
        robot_ctrl_speed_timestamp = robot.get_target_speed_timestamp();
        current_robot_velocity = robot.get_velocity();
        speed_timestamp = robot.get_speed_timestamp();
        current_robot_position = robot.get_position();
        pos_timestamp = robot.get_position_timestamp();

        if(robot_ctrl_speed_timestamp>1e-5) { //this is needed because first feedbacks will have a timestamp of 0
            static double robot_zero_timestamp = robot_ctrl_speed_timestamp;
            csvLogger << (double)timestamp_microseconds*1e-6;
            csvLogger << current_encoder_angle;
            csvLogger << new_robot_input_velocity;
            csvLogger << robot_ctrl_speed_timestamp - robot_zero_timestamp;
            csvLogger << current_robot_ctrl_speed;
            csvLogger << speed_timestamp - robot_zero_timestamp;
            csvLogger << current_robot_velocity;
            csvLogger << pos_timestamp - robot_zero_timestamp;
            csvLogger << current_robot_position;
        }
        else {
            csvLogger << (double)timestamp_microseconds*1e-6;
            csvLogger << current_encoder_angle;
            csvLogger << new_robot_input_velocity;
            csvLogger << robot_ctrl_speed_timestamp;
            csvLogger << current_robot_ctrl_speed;
            csvLogger << speed_timestamp;
            csvLogger << current_robot_velocity;
            csvLogger << pos_timestamp;
            csvLogger << current_robot_position;
        }

        //printf("\nenc_angle=%-10.3f mean time=%-10.3f sigma_time=%-10.3f robot_velocity=%-10.3f\n\n" , 
        //    current_encoder_angle , timer.get_mean_cycle_time(),
        //    timer.get_standard_deviation_cycle_time() , current_robot_velocity);

        /*
            end of tasks
        */

        csvLogger.end_row();
        timer.end_cycle();
    }

    printf("received termination signal. terminating...\n");
}