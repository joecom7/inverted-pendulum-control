from cmath import pi
from RPi import GPIO

GPIO_CLK = 17
GPIO_DT = 18
FRONTI_PER_GIRO = 1000

def setup():
	global clkLastState,counter
	GPIO.setmode(GPIO.BCM)
	GPIO.setup(GPIO_CLK, GPIO.IN, pull_up_down=GPIO.PUD_DOWN)
	GPIO.setup(GPIO_DT, GPIO.IN, pull_up_down=GPIO.PUD_DOWN)
	counter=0
	clkLastState = GPIO.input(GPIO_CLK)

def get_angle():
    global clkLastState,counter
    clkState = GPIO.input(GPIO_CLK)
    dtState = GPIO.input(GPIO_DT)
    if clkState != clkLastState:
            if dtState != clkState:
                    counter += 1
            else:
                    counter -= 1
    clkLastState = clkState
    return 2*pi*counter/FRONTI_PER_GIRO
