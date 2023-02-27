#!/usr/bin/env python

import pigpio
from math import pi
import math

class decoder:

   """Class to decode mechanical rotary encoder pulses."""

   def __init__(self, pi, gpioA, gpioB):

      self.pi = pi
      self.gpioA = gpioA
      self.gpioB = gpioB

      self.levA = 0
      self.levB = 0

      self.lastGpio = None

      self.pi.set_mode(gpioA, pigpio.INPUT)
      self.pi.set_mode(gpioB, pigpio.INPUT)

      self.pi.set_pull_up_down(gpioA, pigpio.PUD_UP)
      self.pi.set_pull_up_down(gpioB, pigpio.PUD_UP)

      self.cbA = self.pi.callback(gpioA, pigpio.EITHER_EDGE, self._pulse)
      self.cbB = self.pi.callback(gpioB, pigpio.EITHER_EDGE, self._pulse)

      self.PPR=3000
      self.INITIAL_ANGLE = 0
      #self.INITIAL_ANGLE = math.pi
      self.pos = (self.INITIAL_ANGLE/(2*math.pi))*self.PPR

   def callback(self,way):

      self.pos += way
      if self.pos > self.PPR/2:  # Normalizziamo l'angolo fra -pi e pi
         self.pos -= self.PPR
      elif self.pos <= -self.PPR/2:
         self.pos += self.PPR
   def _pulse(self, gpio, level, tick):

      """
      Decode the rotary encoder pulse.

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
      """

      if gpio == self.gpioA:
         self.levA = level
      else:
         self.levB = level

      if gpio != self.lastGpio: # debounce
         self.lastGpio = gpio

         if   gpio == self.gpioA and level == 1:
            if self.levB == 1:
               self.callback(1)
         elif gpio == self.gpioB and level == 1:
            if self.levA == 1:
               self.callback(-1)

   def get_angle(self):
       return (self.pos*2*pi)/self.PPR
