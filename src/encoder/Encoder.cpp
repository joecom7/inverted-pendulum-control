#include "Encoder.hpp"

#include <math.h>
#include <cstdio>

int Encoder::PPR = 0;
int Encoder::pos = 0;
int Encoder::MAX_POS = 0;
int Encoder::MIN_POS = 0;

Encoder::Encoder(int clk_gpio, int dt_gpio, int ppr, int start_angle_degrees) : CLK_GPIO(clk_gpio), DT_GPIO(dt_gpio),
                                                                                RESOLUTION_DEGREES(360.0 / (double)ppr), RESOLUTION_RADIANS(2.0 * M_PI / (double)ppr),
                                                                                decoder(clk_gpio, dt_gpio, callback)
{
    Encoder::PPR = ppr;
    pos = (start_angle_degrees * ppr) / 360;
    u_prev = pos * RESOLUTION_RADIANS;
    Encoder::MAX_POS = ppr / 2;
    Encoder::MIN_POS = -ppr / 2;
}
void Encoder::set_zero(int start_angle_degrees)
{
    pos = (start_angle_degrees * PPR) / 360;
    u_prev = pos * RESOLUTION_RADIANS;
}
void Encoder::callback(int way)
{
    Encoder::pos += way;
    if (Encoder::pos > Encoder::MAX_POS)
    {
        Encoder::pos -= Encoder::PPR;
    }
    else if (Encoder::pos <= Encoder::MIN_POS)
    {
        Encoder::pos += Encoder::PPR;
    }
}

double Encoder::get_angle()
{
    return pos * RESOLUTION_RADIANS;
}

double Encoder::get_omega()
{
    double theta = get_angle();
    if (theta * u_prev < 0 && fabs(theta - u_prev) > M_PI_2)
    {
        if (u_prev > 0)
        {
            u_prev -= 2 * M_PI;
        }
        else
        {
            u_prev += 2 * M_PI;
        }
    }
    double tau = costante_tempo_filtro;
    omega = -(T - 2 * tau) / (T + 2 * tau) * omega_prev + 2 / (T + 2 * tau) * theta - 2 / (T + 2 * tau) * u_prev;
    // vel = (pos-last_pos)/T;
    u_prev = theta;
    omega_prev = omega;
    return omega;
}

double Encoder::get_angle_degrees()
{
    return pos * RESOLUTION_DEGREES;
}

void Encoder::calibrate(uint64_t timestamp)
{
    if (old_pos == pos && pos > -0.0061 && pos < 0.0061)
    {
        if ((timestamp - old_timestamp) > 300e+3)
        {
            if (pos > -PPR / 4 && pos < PPR / 4)
            {
                pos = 0;
            }
            else
            {
                pos = MAX_POS;
            }
        }
    }
    else
    {
        old_pos = pos;
        old_timestamp = timestamp;
    }
}

Encoder::~Encoder()
{
    printf("cancelling encoder callback\n");
    decoder.re_cancel();
}