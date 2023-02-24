from RPi import GPIO
from time import sleep

GPIO_CLK = 17
GPIO_DT = 18
FRONTI_PER_GIRO = 100
SLEEP_TIME_MICROSECONDI = 10

GPIO.setmode(GPIO.BCM)
GPIO.setup(GPIO_CLK, GPIO.IN, pull_up_down=GPIO.PUD_DOWN)
GPIO.setup(GPIO_DT, GPIO.IN, pull_up_down=GPIO.PUD_DOWN)

counter = 0
clkLastState = GPIO.input(GPIO_CLK)

try:

        while True:
                clkState = GPIO.input(GPIO_CLK)
                dtState = GPIO.input(GPIO_DT)
                if clkState != clkLastState:
                        if dtState != clkState:
                                counter += 1
                        else:
                                counter -= 1
                        print(counter)
                clkLastState = clkState
                sleep(SLEEP_TIME_MICROSECONDI/1e6)
finally:
        GPIO.cleanup()
