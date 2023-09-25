#include <iostream>

#include <pigpio.h>

#include "rotary_encoder.hpp"

/*

             +---------+         +---------+      0
             |         |         |         |
   A         |         |         |         |
             |         |         |         |
   +---------+         +---------+         +----- 1

       +---------+         +---------+            0
       |         |         |         |
   B   |         |         |         |
       |         |         |         |
   ----+         +---------+         +---------+  1

*/

void re_decoder::_pulse(int gpio, int level, uint32_t tick __attribute__((unused)))
{
   int old_levA, old_levB ,switch_value;
   if (gpio == mygpioA) 
   {
      old_levA = levA;
      old_levB = levB;
      levA = level;
   }
   else 
   {
      old_levB = levB;
      old_levA = levA;
      levB = level;
   }

   switch_value = (levA<<3)+(levB<<2)+(old_levA<<1)+old_levB;
   switch(switch_value) {
      case 1: case 7: case 8: case 14:
         (mycallback)(1);
         break;
      case 2: case 4: case 11: case 13:
         (mycallback)(-1);
         break;
      case 3: case 12:
         (mycallback)(2);
         break;
      case 6: case 9:
         (mycallback)(2);
         break;
      default:
         break;
   }
}

void re_decoder::_pulseEx(int gpio, int level, uint32_t tick, void *user)
{
   /*
      Need a static callback to link with C.
   */

   re_decoder *mySelf = (re_decoder *) user;

   mySelf->_pulse(gpio, level, tick); /* Call the instance callback. */
}

re_decoder::re_decoder(int gpioA, int gpioB, re_decoderCB_t callback)
{
   gpioInitialise();
   mygpioA = gpioA;
   mygpioB = gpioB;

   mycallback = callback;

   levA=0;
   levB=0;

   lastGpio = -1;

   gpioSetMode(gpioA, PI_INPUT);
   gpioSetMode(gpioB, PI_INPUT);

   /* pull up is needed as encoder common is grounded */

   gpioSetPullUpDown(gpioA, PI_PUD_UP);
   gpioSetPullUpDown(gpioB, PI_PUD_UP);

   /* monitor encoder level changes */

   gpioSetAlertFuncEx(gpioA, _pulseEx, this);
   gpioSetAlertFuncEx(gpioB, _pulseEx, this);
}

void re_decoder::re_cancel(void)
{
   gpioSetAlertFuncEx(mygpioA, 0, this);
   gpioSetAlertFuncEx(mygpioB, 0, this);
}

re_decoder::~re_decoder() {
   printf("terminating pigpio\n");
   //gpioTerminate();
}