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

        uint64_t old_timestamp;
        int old_pos;
        static void callback(int way);
    public:
        /**
         * @brief Construct a new Encoder object.
         * 
         * @param clk_gpio The GPIO to which the CLK pin of the encoder is connected. 
         * @param dt_gpio The GPIO to which the DT pin of the encoder is connected. 
         * @param ppr Number of pulses per revolution of the encoder.
         * @param start_angle_degrees Starting angle of the encoder.
         */
        Encoder(int clk_gpio, int dt_gpio, int ppr, int start_angle_degrees);
        /**
         * @brief Return the angle of the encoder in radians.
         * 
         * @return double The angle of the encoder in radians.
         */
        double get_angle();
        /**
         * @brief Return the angle of the encoder in degrees.
         * 
         * @return double The angle of the encoder in degrees.
         */
        double get_angle_degrees();
        /**
         * @brief Calibrate the encoder. This is optional but it helps
         * prevent the encoder from having a zero point that is not
         * then the pendulum is completely vertical.
         * 
         * @param timestamp Current timestamp in microseconds.
         */
        void calibrate(uint64_t timestamp);
        /**
         * @brief Destroy the Encoder object. This ensures that the callback
         * on the encoder rotation is cancelled.
         * 
         */
        ~Encoder();
};

#endif