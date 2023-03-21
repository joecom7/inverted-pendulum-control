#include <iostream>
#include <unistd.h>
#include <pigpio.h>

#include "encoder/Encoder.hpp"

/*

REQUIRES

A rotary encoder contacts A and B connected to separate gpios and
the common contact connected to Pi ground.

TO BUILD

g++ -o rot_enc_cpp test_rotary_encoder.cpp rotary_encoder.cpp -lpigpio -lrt

TO RUN

sudo ./rot_enc_cpp

*/

int main() {
    if (gpioInitialise() < 0) return 1;
    Encoder enc(17, 18, 1024,180);
    while(true) {
        std::cout << "pos=" << enc.get_angle() << std::endl;
        usleep(100);
    }
}