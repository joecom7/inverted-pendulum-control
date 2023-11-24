#include <iostream>
#include "src/encoder/Encoder.hpp"
#include "src/timer/Timer.hpp"
#include "Constants.hpp"
#include <inttypes.h>
#include "meca500_ethercat_cpp/Robot.hpp"
#include "src/lcd/LCD.hpp"
#include "src/controller/FeedbackController.hpp"
#include "signal.h"
#include <sys/mman.h>
#include <pigpio.h>

bool activate_swingup;

#define START_BUTTON_GPIO 22
#define CALIBRATE_BUTTON_GPIO 23
#define SWINGUP_BUTTON_GPIO 24

bool control_terminated;
LCD *lcd;

typedef struct
{
    double *angle;
    double *omega;
    float *robot_x;
    double *robot_speed;
} lcd_print_param_t;

void pressed_start_button(int, int, unsigned int)
{
    control_terminated = true;
}

void lcd_printer(void *arg)
{
    lcd_print_param_t *param = (lcd_print_param_t *)arg;
    char buffer[100];
    struct sched_param sp;
    memset(&sp, 0, sizeof(sp));
    sp.sched_priority = 80;
    sched_setscheduler(0, SCHED_FIFO, &sp);
    while (!control_terminated)
    {
        lcd->clear();
        sprintf(buffer, "%8.2lf gradi\0", *(param->angle) * 180.0 / M_PI);
        (*lcd) << buffer;
        // lcd->setPosition(7, 0);
        // sprintf(buffer, "%4.1lf g/s\0", *(param->omega)*M_PI/180.0);
        //(*lcd) << buffer;
        lcd->setPosition(0, 1);
        sprintf(buffer, "%8.2f cm\0", *(param->robot_x) * 100.0);
        (*lcd) << buffer;
        // lcd->setPosition(7, 1);
        // sprintf(buffer, "%4.1lf cm/s\0", *(param->omega)*100.0);
        //(*lcd) << buffer;
        usleep(100000);
    }
}

void cleanup(int)
{
    control_terminated = true;
}

void control()
{
    bool robot_error = false;
    Robot robot(Constants::ROBOT_POS_LIMIT,
                Constants::TARGET_CYCLE_TIME_MICROSECONDS,
                Constants::NETWORK_INTERFACE,
                Constants::ROBOT_BLENDING_PERCENTAGE,
                Constants::ROBOT_ACCELERATION_LIMIT);

    float delay_feedback_gain;
    if (Constants::TIMER_AGGRESSIVE_MODE)
    {
        delay_feedback_gain = Constants::AGGRESSIVE_DELAY_FEEDBACK_GAIN;
    }
    else
    {
        delay_feedback_gain = Constants::DELAY_FEEDBACK_GAIN;
    }

    Timer timer(Constants::TARGET_CYCLE_TIME_MICROSECONDS, delay_feedback_gain,
                Constants::TIMER_AGGRESSIVE_MODE);

    double current_encoder_angle, current_robot_velocity, new_robot_input_velocity;
    double current_robot_position, current_robot_ctrl_speed, robot_ctrl_speed_timestamp, speed_timestamp, pos_timestamp;
    uint64_t timestamp_microseconds;

    signal(2, cleanup);
    signal(6, cleanup);

    FeedbackController feedbackController(Constants::CONTROL_TYPE, Constants::START_CONTROL_ANGLE_DEGREES, Constants::STOP_CONTROL_ANGLE_DEGREES, Constants::TARGET_CYCLE_TIME_MICROSECONDS, activate_swingup);
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

    double omega, robot_velocity;
    // float joint_omega[6] = {0,0,0,0,0,0};
    // float robot_joints[6];

    float pose[6];

    mlockall(MCL_CURRENT | MCL_FUTURE);
    Encoder encoder(Constants::ENCODER_CLK_PIN,
                    Constants::ENCODER_DT_PIN, Constants::ENCODER_PPR,
                    Constants::ENCODER_START_ANGLE_DEGREES);
    uint8_t low_omega_counter = 0;
    while (low_omega_counter < 10)
    {
        if (encoder.get_omega() < 1e-4)
        {
            low_omega_counter++;
        }
        else
        {
            low_omega_counter = 0;
        }
        usleep(200000);
    }
    lcd->clear();
    (*lcd) << "Premere il tasto";
    lcd->setPosition(0, 1);
    (*lcd) << "K1 per calibrare";
    while (gpioRead(CALIBRATE_BUTTON_GPIO) == 1)
    {
        usleep(200);
    }
    encoder.set_zero(Constants::ENCODER_START_ANGLE_DEGREES);
    lcd->clear();
    lcd_print_param_t param = {&current_encoder_angle, &omega, pose, &current_robot_velocity};
    std::thread lcd_thd(lcd_printer, &param);

    control_terminated = false;

    while (!control_terminated)
    {
        timer.start_cycle();

        /*
            tasks to execute in loop
        */

        timestamp_microseconds = timer.get_microseconds_from_program_start();
        current_encoder_angle = encoder.get_angle();
        omega = encoder.get_omega();
        robot.get_pose(pose);
        robot_velocity = robot.get_velocity();
        new_robot_input_velocity = feedbackController.get_robot_input(timestamp_microseconds, current_encoder_angle, omega, pose[0], robot_velocity);
        // std::cout << current_encoder_angle << '\n';//test
        robot.move_lin_vel_trf_x(new_robot_input_velocity);
        // joint_omega[JOINT_TO_MOVE] = new_robot_input_velocity;
        // robot.move_joints_vel(joint_omega)
        // robot.get_joints(robot_joints);

        // printf("\nenc_angle=%-10.3f mean time=%-10.3f sigma_time=%-10.3f max_time=%-10.3f robot_velocity=%-10.3f\n\n" ,
        //     current_encoder_angle , timer.get_mean_cycle_time(),
        //     timer.get_standard_deviation_cycle_time() , (float)timer.get_max_cycle_time() ,current_robot_velocity);

        /*
            end of loop
        */

       /*
        if (robot_error)
        {
            robot.reset_error();
            robot.move_pose(STARTING_ROBOT_POSITION, STARTING_ROBOT_ORIENTATION);
            feedbackController.reset();

            // double theta = encoder.get_angle();
            // usleep(Constants::TARGET_CYCLE_TIME_MICROSECONDS);
            // double theta_new = encoder.get_angle();
            // encoder.reset_derivatore((theta_new-theta)/Constants::TARGET_CYCLE_TIME_MICROSECONDS);
            // usleep(Constants::TARGET_CYCLE_TIME_MICROSECONDS);
            // uint8_t remaining_cycles = ((25e+3)/Constants::TARGET_CYCLE_TIME_MICROSECONDS) - 2;
            // for(int i=0;i<remaining_cycles;i++) {
            //     encoder.get_omega();
            // }
            // robot.reset_derivatore();
            robot_error = false;
            timer.reset();
            timer.start_cycle();
            for (int i = 0; i < 1000; i++)
            {
                timer.start_cycle();

                timestamp_microseconds = timer.get_microseconds_from_program_start();
                current_encoder_angle = encoder.get_angle();
                omega = encoder.get_omega();
                robot.get_pose(pose);
                robot_velocity = robot.get_velocity();
                new_robot_input_velocity = 0;
                // std::cout << current_encoder_angle << '\n';//test
                robot.move_lin_vel_trf_x(new_robot_input_velocity);
                // joint_omega[JOINT_TO_MOVE] = new_robot_input_velocity;
                // robot.move_joints_vel(joint_omega)
                // robot.get_joints(robot_joints);

                // printf("\nenc_angle=%-10.3f mean time=%-10.3f sigma_time=%-10.3f max_time=%-10.3f robot_velocity=%-10.3f\n\n" ,
                //     current_encoder_angle , timer.get_mean_cycle_time(),
                //     timer.get_standard_deviation_cycle_time() , (float)timer.get_max_cycle_time() ,current_robot_velocity);

                       timer.end_cycle();

            }
            timer.start_cycle();
        }
        */
        timer.end_cycle();
    }
    lcd_thd.join();
    printf("received termination signal. terminating...\n");
}

int main()
{
    struct sched_param sp;
    memset(&sp, 0, sizeof(sp));
    sp.sched_priority = 99;
    sched_setscheduler(0, SCHED_FIFO, &sp);
    if (gpioInitialise() < 0)
    {
        std::cout << "Failure..." << std::endl;
        exit(-1);
    }
    lcd = new LCD(1, 0x27);

    gpioSetMode(START_BUTTON_GPIO, PI_INPUT);
    gpioSetMode(CALIBRATE_BUTTON_GPIO, PI_INPUT);
    gpioSetMode(SWINGUP_BUTTON_GPIO, PI_INPUT);

    gpioSetPullUpDown(START_BUTTON_GPIO, PI_PUD_UP);
    gpioSetPullUpDown(CALIBRATE_BUTTON_GPIO, PI_PUD_UP);
    gpioSetPullUpDown(SWINGUP_BUTTON_GPIO, PI_PUD_UP);
    // gpioSetISRFunc(SWINGUP_BUTTON_GPIO, 0, 0, pressed_terminate_button);

    while (true)
    {
        usleep(1000000);
        lcd->clear();
        (*lcd) << "K4: no swingup";
        lcd->setPosition(0, 1);
        (*lcd) << "K3: swingup";
        while (gpioRead(START_BUTTON_GPIO) == 1 && gpioRead(SWINGUP_BUTTON_GPIO) == 1)
        {
            usleep(100);
        }
        if (gpioRead(START_BUTTON_GPIO) == 0)
        {
            Constants::START_CONTROL_ANGLE_DEGREES = 5.0;
            Constants::STOP_CONTROL_ANGLE_DEGREES = 5.0;
            activate_swingup = false;
        }
        else
        {
            activate_swingup = true;
        }
        gpioSetISRFunc(START_BUTTON_GPIO, 0, 0, pressed_start_button);
        lcd->clear();
        (*lcd) << "Attendere...";
        control();

        gpioSetISRFunc(START_BUTTON_GPIO, 0, 0, NULL);
    }
}