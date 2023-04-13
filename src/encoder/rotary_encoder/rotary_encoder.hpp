#ifndef ROTARY_ENCODER_HPP
#define ROTARY_ENCODER_HPP

#include <stdint.h>
#include <pigpio.h>

typedef void (*re_decoderCB_t)(int);

class re_decoder
{
   int mygpioA, mygpioB, levA, levB, lastGpio;

   re_decoderCB_t mycallback;

   void _pulse(int gpio, int level, uint32_t tick __attribute__((unused)));
   static void _pulseEx(int gpio, int level, uint32_t tick, void *user);


   public:

   /**
    * @brief Destroy the re decoder object. This ensures that Pigpio is uninitialized.
    * 
    */
   ~re_decoder();

   /**
    * @brief This function establishes a rotary encoder on gpioA and gpioB.

      When the encoder is turned the callback function is called.
    * 
    * @param gpioA GPIO to which the CLK pin of the encoder is connected.
    * @param gpioB GPIO to which the DT pin of the encoder is connected.
    * @param callback A function to be called when the encoder is turned.
    */
   re_decoder(int gpioA, int gpioB, re_decoderCB_t callback);

  /**
   * @brief Release the resources used by the decoder.
   * 
   */
   void re_cancel(void);
};

#endif
