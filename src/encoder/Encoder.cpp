#include "Encoder.hpp"

#include <math.h>
#include <cstdio>

int Encoder::PPR = 0;
int Encoder::pos = 0;
int Encoder::MAX_POS = 0;
int Encoder::MIN_POS = 0;

Encoder::Encoder(int clk_gpio, int dt_gpio, int ppr, int start_angle_degrees) :
    CLK_GPIO(clk_gpio) , DT_GPIO(dt_gpio) ,
    RESOLUTION_DEGREES(360.0/(double)ppr) , RESOLUTION_RADIANS(2.0*M_PI/(double)ppr) ,
    decoder(clk_gpio, dt_gpio, callback){
        Encoder::PPR = ppr;
        pos = (start_angle_degrees*ppr)/360;
        Encoder::MAX_POS = ppr/2;
        Encoder::MIN_POS = -ppr/2;
    }

void Encoder::callback(int way) {
    Encoder::pos += way;
    if(Encoder::pos > Encoder::MAX_POS) {
        Encoder::pos -= Encoder::PPR;
    }
    else if(Encoder::pos <= Encoder::MIN_POS) {
        Encoder::pos += Encoder::PPR;
    }
}

double Encoder::get_angle() {
    return pos*RESOLUTION_RADIANS;
}

double Encoder::get_angle_degrees() {
    return pos*RESOLUTION_DEGREES;
}

void Encoder::calibrate(uint64_t timestamp) {
    if(old_pos==pos) {
        if((timestamp - old_timestamp )> 300e+3) {
            if(pos>-PPR/4&&pos<PPR/4) {
                pos = 0;
            }
            else {
                pos = MAX_POS;
            }
        }
    }
    else {
        old_pos = pos;
        old_timestamp = timestamp;
    }
}

Encoder::~Encoder() {
    printf("cancelling encoder callback\n");
    decoder.re_cancel();
}