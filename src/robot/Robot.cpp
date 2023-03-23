#include "Robot.hpp"

void Robot::py_init() {
    PyImport_AppendInittab("mecawrapper", PyInit_mecawrapper);
    Py_Initialize();
    PyImport_ImportModule("mecawrapper");
    meca_init();
}

Robot::Robot() {
    py_init();
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