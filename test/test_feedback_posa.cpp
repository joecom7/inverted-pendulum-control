#include <iostream>
#include "../src/encoder/Encoder.hpp"
#include "../src/timer/Timer.hpp"
#include "../Constants.hpp"
#include <inttypes.h>
#include "../meca500_ethercat_cpp/Robot.hpp"
#include "../src/csvlogger/CsvLogger.hpp"
#include "../src/lcd/LCD.hpp"
#include "../src/controller/FeedbackController.hpp"
#include "signal.h"
#include <sys/mman.h>
#include <pigpio.h>

#define START_BUTTON_GPIO 22
#define CALIBRATE_BUTTON_GPIO 23
#define EXIT_BUTTON_GPIO 24

bool control_terminated;
bool program_terminated = false;
LCD *lcd;

typedef struct {
    double* angle;
    double* omega;
    float* robot_x;
    double* robot_speed;
} lcd_print_param_t;

void pressed_start_button(int, int, unsigned int)
{
    control_terminated = true;
}

void pressed_terminate_button(int, int, unsigned int)
{
    control_terminated = true;
    program_terminated = true;
}

void lcd_printer(void *arg)
{
    lcd_print_param_t *param = (lcd_print_param_t*)arg;
    char buffer[100];
    struct sched_param sp;
    memset(&sp, 0, sizeof(sp));
    sp.sched_priority = 80;
    sched_setscheduler(0, SCHED_FIFO, &sp);
    while (!control_terminated)
    {
        lcd->clear();
        sprintf(buffer, "%8.2lf gradi\0", *(param->angle)*180.0/M_PI);
        (*lcd) << buffer;
        //lcd->setPosition(7, 0);
        //sprintf(buffer, "%4.1lf g/s\0", *(param->omega)*M_PI/180.0);
        //(*lcd) << buffer;
        lcd->setPosition(0, 1);
        sprintf(buffer, "%8.2f cm\0", *(param->robot_x)*100.0);
        (*lcd) << buffer;
        //lcd->setPosition(7, 1);
        //sprintf(buffer, "%4.1lf cm/s\0", *(param->omega)*100.0);
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
    Robot robot(Constants::ROBOT_POS_LIMIT,
                Constants::TARGET_CYCLE_TIME_MICROSECONDS,
                Constants::NETWORK_INTERFACE,
                Constants::ROBOT_BLENDING_PERCENTAGE,
                Constants::ROBOT_ACCELERATION_LIMIT);

    CsvLogger csvLogger("feedback_tempi.csv");
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

    FeedbackController feedbackController(SQUARE, Constants::START_CONTROL_ANGLE_DEGREES, Constants::STOP_CONTROL_ANGLE_DEGREES, Constants::TARGET_CYCLE_TIME_MICROSECONDS);
    feedbackController.set_square_wave_param(Constants::SQUARE_WAVE_FREQUENCY_HZ,
                                             0.0,
                                             Constants::SQUARE_WAVE_MEAN_MPS);
    feedbackController.set_chirp_param(Constants::CHIRP_F0_HZ,
                                       Constants::CHIRP_K,
                                       Constants::CHIRP_APKPK_MPS);
    // robot setup
    robot.reset_error();
    robot.set_conf(ROBOT_CONF);
    robot.move_pose(0, -240-18, 190+15, 90+3, 0-2, 0+4);
    timer.set_starting_timestamp();

    double omega, robot_velocity;
    // float joint_omega[6] = {0,0,0,0,0,0};
    // float robot_joints[6];

    float pose[6];

    mlockall(MCL_CURRENT | MCL_FUTURE);
    Encoder encoder(Constants::ENCODER_CLK_PIN,
                    Constants::ENCODER_DT_PIN, Constants::ENCODER_PPR,
                    0.0);
    lcd->clear();
    uint8_t low_omega_counter = 0;
    while(low_omega_counter<10) {
        if(encoder.get_omega()<1e-4) {
            low_omega_counter++;
        }
        else {
            low_omega_counter = 0;
        }
        usleep(200000);
    }
    encoder.set_zero(0.0);
    lcd->clear();
    lcd_print_param_t param = {&current_encoder_angle,&omega,pose,&current_robot_velocity};
    std::thread lcd_thd(lcd_printer, &param);

    control_terminated = false;

    while (!control_terminated)
    {
        timer.start_cycle();
        timestamp_microseconds = timer.get_microseconds_from_program_start();
        current_encoder_angle = encoder.get_angle();
        omega = encoder.get_omega();
        robot.get_pose(pose);
        robot_velocity = robot.get_velocity();
        new_robot_input_velocity = feedbackController.get_robot_input(timestamp_microseconds, current_encoder_angle, omega, pose[0], robot_velocity);
        robot.move_lin_vel_trf_x(new_robot_input_velocity);
        // robot.move_joints_vel(joint_omega)
        csvLogger << (double)timestamp_microseconds * 1e-6;
        csvLogger << current_encoder_angle;
        csvLogger << new_robot_input_velocity;
        csvLogger << pose[0];
        csvLogger << omega;
        csvLogger << robot_velocity;
        csvLogger.end_row();
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
    gpioSetMode(EXIT_BUTTON_GPIO, PI_INPUT);

    gpioSetPullUpDown(START_BUTTON_GPIO, PI_PUD_UP);
    gpioSetPullUpDown(CALIBRATE_BUTTON_GPIO, PI_PUD_UP);
    gpioSetPullUpDown(EXIT_BUTTON_GPIO, PI_PUD_UP);
    gpioSetISRFunc(EXIT_BUTTON_GPIO, 0, 0, pressed_terminate_button);

    while (!program_terminated)
    {
        usleep(1000000);
        lcd->clear();
        (*lcd) << "Premere il tasto";
        lcd->setPosition(0, 1);
        (*lcd) << "K4 per iniziare";
        while (gpioRead(START_BUTTON_GPIO) == 1)
        {
            usleep(200);
        }
        lcd->clear();
        (*lcd) << "Attendere...";
        gpioSetISRFunc(START_BUTTON_GPIO, 0, 0, pressed_start_button);
        control();
        gpioSetISRFunc(START_BUTTON_GPIO, 0, 0, NULL);
    }
    lcd->clear();
    gpioTerminate();
}