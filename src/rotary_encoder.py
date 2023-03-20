#!/usr/bin/env python

import pigpio
from math import pi
import math
import os

param = os.sched_param(os.sched_get_priority_max(os.SCHED_FIFO))
os.sched_setscheduler(0, os.SCHED_FIFO, param)

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

#                           _______         _______       
#               Pin1 ______|       |_______|       |______ Pin1
# negative <---         _______         _______         __      --> positive
#               Pin2 __|       |_______|       |_______|   Pin2

		#	new	new	old	old
		#	pin2	pin1	pin2	pin1	Result
		#	----	----	----	----	------
		#	0	0	0	0	no movement
		#	0	0	0	1	+1
		#	0	0	1	0	-1
		#	0	0	1	1	+2  (assume pin1 edges only)
		#	0	1	0	0	-1
		#	0	1	0	1	no movement
		#	0	1	1	0	-2  (assume pin1 edges only)
		#	0	1	1	1	+1
		#	1	0	0	0	+1
		#	1	0	0	1	-2  (assume pin1 edges only)
		#	1	0	1	0	no movement
		#	1	0	1	1	-1
		#	1	1	0	0	+2  (assume pin1 edges only)
		#	1	1	0	1	-1
		#	1	1	1	0	+1
		#	1	1	1	1	no movement

      if gpio == self.gpioA:
         newLevA = level
         newLevB = self.oldLevB
      else:
         newLevB = level
         newLevA = self.oldLevA
      switchValue = int(newLevA)*8+int(newLevB)*4+int(self.oldLevA)*2+int(self.oldLevB)
      if switchValue==1 or switchValue==7 or switchValue==8 or switchValue==14:
         self.callback(1)
      elif switchValue==2 or switchValue==4 or switchValue==11 or switchValue==13:
         self.callback(-1)
      elif switchValue==3 or switchValue==12:
         self.callback(2)
      elif switchValue==6 or switchValue==9:
         self.callback(-2)
      self.oldLevA = newLevA
      self.oldLevB = newLevB

   def get_angle(self):
       return (self.pos*2*pi)/self.PPR
   def get_angle_degrees(self):
      return (self.pos*360)/self.PPR