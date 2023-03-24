#include "Robot.hpp"

void Robot::py_init(bool bypass_robot) {
    PyImport_AppendInittab("mecawrapper", PyInit_mecawrapper);
    Py_Initialize();
    PyImport_ImportModule("mecawrapper");
    meca_init(bypass_robot);
}

Robot::Robot(bool bypass_robot) : BYPASS_ROBOT(bypass_robot){
    py_init(bypass_robot);
}

void Robot::connect() {
    meca_connect();
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
}

void Robot::reset_error() {
    meca_reset_error();
}

void Robot::print_number(float number) {
    print_velocity(number);
}