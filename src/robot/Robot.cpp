#include "Robot.hpp"

void Robot::py_init() {
    PyImport_AppendInittab("mecawrapper", PyInit_mecawrapper);
    Py_Initialize();
    PyImport_ImportModule("mecawrapper");
}

Robot::Robot() {
    py_init();
}

void Robot::print_number(float number) {
    print_velocity(number);
}