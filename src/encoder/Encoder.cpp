#include "Encoder.hpp"

int Encoder::PPR = 0;
int Encoder::pos = 0;
int Encoder::MAX_POS = 0;
int Encoder::MIN_POS = 0;

Encoder::Encoder(int clk_gpio, int dt_gpio, int ppr, int start_angle_degrees) :
    CLK_GPIO(clk_gpio) , DT_GPIO(dt_gpio) , decoder(clk_gpio, dt_gpio, callback){
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

float Encoder::get_angle() {
    return (pos*2*M_PI)/Encoder::PPR;
}

float Encoder::get_angle_degrees() {
    return (pos*360.0)/Encoder::PPR;;
}