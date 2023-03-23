#ifndef ROBOT_H
#define ROBOT_H

#include "mecawrapper/mecawrapper.h"

class Robot {
    private:
        static void py_init();
    public:
        Robot();
        void connect();
        void activate();
        void home();
        void deactivate();
        void disconnect();
        void reset_error();
        void print_number(float number);
};

#endif