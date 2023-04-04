#include "Robot.hpp"
#include "mecawrapper/mecawrapper.h"

Robot::Robot(const char* robot_ip , double pos_limit,bool bypass_robot) :
    BYPASS_ROBOT(bypass_robot) , POS_LIMIT(pos_limit){
    PyImport_AppendInittab("mecawrapper", PyInit_mecawrapper);
    Py_Initialize();
    PyImport_ImportModule("mecawrapper");
    meca_init(bypass_robot, robot_ip);
}

void Robot::connect(bool activate_feedback) {
    bool connected = meca_connect(activate_feedback);
    while(!connected) {
        sleep(1);
        printf("robot not available. retrying connection\n");
        connected = meca_connect(activate_feedback);
    }
}

void Robot::activate() {
    meca_activate();
}

void Robot::home() {
    meca_home();
}

void Robot::deactivate() {
    meca_deactivate();
}

void Robot::disconnect() {
    meca_disconnect();
    Py_Finalize();
}

void Robot::reset_error() {
    meca_reset_error();
}

void Robot::print_number(double number) {
    print_velocity(number);
}

double Robot::get_velocity() {
    return meca_get_velocity()*1e-3;
}

double Robot::get_position() {
    return meca_get_position()*1e-3;
}

// N.B. La velocità è in metri al secondo
void Robot::move_lin_vel_trf(double velocity) {
    bool out_of_range_right = ((get_position()>POS_LIMIT) && (velocity > 0));
    bool out_of_range_left = ((get_position()<(-POS_LIMIT)) && (velocity < 0));
    if((!out_of_range_right)&&(!out_of_range_left)) {
        meca_move_lin_vel_trf(velocity*1e+3);
    }
}

void Robot::set_conf(short c1, short c2, short c3) {
    meca_set_conf(c1,c2,c3);
}

void Robot::move_pose(double x, double y, double z, double alpha, double beta, double gamma) {
    meca_move_pose(x,y,z,alpha,beta,gamma);
    while(!block_ended()) {
        printf("waiting for robot to finish moving\n");
        sleep(1);
    }
}

Robot::~Robot() {
    meca_end_program();
    printf("deactivating robot\n");
    deactivate();
    usleep(1e+6);
    printf("disconnecting robot\n");
    disconnect();
}

bool Robot::block_ended() {
    return meca_block_ended();
}

void Robot::set_monitoring_interval(uint32_t monitoring_interval_microseconds) {
    meca_set_monitoring_interval(((double)monitoring_interval_microseconds)*1e-6);
}

double Robot::get_position_timestamp() {
    return meca_get_position_timestamp();
}

double Robot::get_speed_timestamp() {
    return meca_get_speed_timestamp();
}