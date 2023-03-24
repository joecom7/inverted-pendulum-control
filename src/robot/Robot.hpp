#ifndef ROBOT_H
#define ROBOT_H

#include "mecawrapper/mecawrapper.h"

class Robot {
    private:
        static void py_init(const char* robot_ip , bool bypass_robot);
        const bool BYPASS_ROBOT;
    public:
        Robot(const char* robot_ip , bool bypass_robot);
        void connect();
        void activate();
        void home();
        void deactivate();
        void disconnect();
        void reset_error();
        void print_number(float number);
};

#endif