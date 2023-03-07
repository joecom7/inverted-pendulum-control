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

      self.pi.set_mode(gpioA, pigpio.INPUT)
      self.pi.set_mode(gpioB, pigpio.INPUT)

      self.pi.set_pull_up_down(gpioA, pigpio.PUD_UP)
      self.pi.set_pull_up_down(gpioB, pigpio.PUD_UP)

      self.cbA = self.pi.callback(gpioA, pigpio.EITHER_EDGE, self._pulse)
      self.cbB = self.pi.callback(gpioB, pigpio.EITHER_EDGE, self._pulse)

      self.oldLevA = pi.read(gpioA)
      self.oldLevB = pi.read(gpioB)

      self.PPR=1024
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
         newLevA = level
         newLevB = self.oldLevB
      else:
         newLevB = level
         newLevA = self.oldLevA
      switchValue = int(newLevA)*8+int(newLevB)*4+int(self.oldLevA)*2+int(self.oldLevB)
      if(switchValue==1):
         self.callback(1)
      elif(switchValue==2):
         self.callback(-1)
      elif(switchValue==3):
         self.callback(2)
      elif(switchValue==4):
         self.callback(-1)
      elif(switchValue==6):
         self.callback(-2)
      elif(switchValue==7):
         self.callback(1)
      elif(switchValue==8):
         self.callback(1)
      elif(switchValue==9):
         self.callback(-2)
      elif(switchValue==11):
         self.callback(-1)
      elif(switchValue==12):
         self.callback(2)
      elif(switchValue==13):
         self.callback(-1)
      elif(switchValue==14):
         self.callback(1)
      self.oldLevA = newLevA
      self.oldLevB = newLevB

   def get_angle(self):
       return (self.pos*2*pi)/self.PPR
   def get_angle_degrees(self):
      return (self.pos*360)/self.PPR