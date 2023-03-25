#include "Robot.hpp"

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