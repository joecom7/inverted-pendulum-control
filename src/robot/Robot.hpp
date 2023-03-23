#ifndef ROBOT_H
#define ROBOT_H

#include "mecawrapper/mecawrapper.h"

class Robot {
    private:
        static void py_init();
    public:
        Robot();
        void print_number(float number);
};

#endif