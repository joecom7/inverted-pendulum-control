#ifndef ENCODER_H
#define ENCODER_H

#include "rotary_encoder/rotary_encoder.hpp"

class Encoder {
    private:
        const int CLK_GPIO;
        const int DT_GPIO;
        const double RESOLUTION_DEGREES;
        const double RESOLUTION_RADIANS;
        static int PPR;
        static int pos;
        static int MAX_POS;
        static int MIN_POS;
        re_decoder decoder;
        static void callback(int way);
    public:
        Encoder(int clk_gpio, int dt_gpio, int ppr, int start_angle_degrees);
        double get_angle();
        double get_angle_degrees();
};

#endif