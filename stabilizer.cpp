#include <iostream>
#include "src/encoder/Encoder.hpp"
#include "src/timer/Timer.hpp"
#include "Constants.hpp"
#include <inttypes.h>
#include "meca500_ethercat_cpp/Robot.hpp"
#include "src/csvlogger/CsvLogger.hpp"
#include "src/controller/FeedbackController.hpp"
#include "signal.h"
#include <sys/mman.h>

#define JOINT_TO_MOVE 0

Robot robot(0.0,
            Constants::TARGET_CYCLE_TIME_MICROSECONDS,
            Constants::NETWORK_INTERFACE,
            Constants::ROBOT_BLENDING_PERCENTAGE,
            Constants::ROBOT_ACCELERATION_LIMIT);

CsvLogger csvLogger(Constants::LOGFILE_NAME);

bool program_terminated = false;

void cleanup(int)
{
    program_terminated = true;
}

int main()
{
    float delay_feedback_gain;
    if (Constants::TIMER_AGGRESSIVE_MODE)
    {
        delay_feedback_gain = Constants::AGGRESSIVE_DELAY_FEEDBACK_GAIN;
    }
    else
    {
        delay_feedback_gain = Constants::DELAY_FEEDBACK_GAIN;
    }

    Encoder encoder(Constants::ENCODER_CLK_PIN,
                    Constants::ENCODER_DT_PIN, Constants::ENCODER_PPR,
                    Constants::ENCODER_START_ANGLE_DEGREES);

    Timer timer(Constants::TARGET_CYCLE_TIME_MICROSECONDS, delay_feedback_gain,
                Constants::TIMER_AGGRESSIVE_MODE);

    double current_encoder_angle, current_robot_velocity, new_robot_input_velocity;
    double current_robot_position, current_robot_ctrl_speed, robot_ctrl_speed_timestamp, speed_timestamp, pos_timestamp;
    uint64_t timestamp_microseconds;

    signal(2, cleanup);
    signal(6, cleanup);

    FeedbackController feedbackController(Constants::CONTROL_TYPE);
    feedbackController.set_square_wave_param(Constants::SQUARE_WAVE_FREQUENCY_HZ,
                                             Constants::SQUARE_WAVE_AMPLITUDE_PKPK_MPS,
                                             Constants::SQUARE_WAVE_MEAN_MPS);
    feedbackController.set_chirp_param(Constants::CHIRP_F0_HZ,
                                       Constants::CHIRP_K,
                                       Constants::CHIRP_APKPK_MPS);
    // robot setup
    robot.reset_error();
    robot.set_conf(ROBOT_CONF);
    robot.move_pose(STARTING_ROBOT_POSITION, STARTING_ROBOT_ORIENTATION);
    timer.set_starting_timestamp();

    mlockall(MCL_CURRENT | MCL_FUTURE);

    while (!program_terminated)
    {
        timer.start_cycle();

        /*
            tasks to execute in loop
        */

        timestamp_microseconds = timer.get_microseconds_from_program_start();
        current_encoder_angle = encoder.get_angle();
        if(Constants::ACTIVATE_ROBOT_CALIBRATION) {
            encoder.calibrate(timestamp_microseconds);
        }
        new_robot_input_velocity = feedbackController.get_robot_input(timestamp_microseconds,current_encoder_angle);
            // std::cout << current_encoder_angle << '\n';//test
        robot.move_lin_vel_trf(new_robot_input_velocity);
        current_robot_position = robot.get_position();
        csvLogger << (double)timestamp_microseconds * 1e-6;
        csvLogger << current_encoder_angle;
        csvLogger << new_robot_input_velocity;
        //csvLogger << robot_joints[JOINT_TO_MOVE];
        csvLogger << current_robot_position;
        csvLogger << encoder.get_omega();

        // printf("\nenc_angle=%-10.3f mean time=%-10.3f sigma_time=%-10.3f max_time=%-10.3f robot_velocity=%-10.3f\n\n" ,
        //     current_encoder_angle , timer.get_mean_cycle_time(),
        //     timer.get_standard_deviation_cycle_time() , (float)timer.get_max_cycle_time() ,current_robot_velocity);

        /*
            end of tasks
        */

        csvLogger.end_row();
        timer.end_cycle();
    }

    printf("received termination signal. terminating...\n");
}
