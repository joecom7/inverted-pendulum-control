#include "Robot.hpp"
#include "mecawrapper/mecawrapper.h"

Robot::Robot(const char* robot_ip , bool bypass_robot) : BYPASS_ROBOT(bypass_robot){
    PyImport_AppendInittab("mecawrapper", PyInit_mecawrapper);
    Py_Initialize();
    PyImport_ImportModule("mecawrapper");
    meca_init(bypass_robot, robot_ip);
}

void Robot::connect() {
    bool connected = meca_connect();
    while(!connected) {
        sleep(1);
        printf("robot non disponibile. ritento la connessione\n");
        connected = meca_connect();
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
    return meca_get_velocity();
}

double Robot::get_position() {
    return meca_get_velocity();
}

// N.B. La velocità è in metri al secondo
void Robot::move_lin_vel_trf(double velocity) {
    meca_move_lin_vel_trf(velocity*1e+3);
}

void Robot::set_conf(short c1, short c2, short c3) {
    meca_set_conf(c1,c2,c3);
}

void Robot::move_pose(double x, double y, double z, double alpha, double beta, double gamma) {
    meca_move_pose(x,y,z,alpha,beta,gamma);
    while(!block_ended()) {
        printf("attendo che il robot finisca di muoversi\n");
        sleep(1);
    }
}

bool Robot::block_ended() {
    return meca_block_ended();
}