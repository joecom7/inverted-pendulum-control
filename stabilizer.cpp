#include <iostream>
#include "src/encoder/Encoder.hpp"
#include "src/timer/Timer.hpp"
#include "constants.h"

int main() {
    Timer timer(TARGET_CYCLE_TIME_MICROSECONDS);
    Encoder encoder(17, 18, 1024,0);
    while(true) {
        timer.start_cycle();
        std::cout << "pos=" << encoder.get_angle_degrees() << std::endl;
        timer.end_cycle();
    }
}