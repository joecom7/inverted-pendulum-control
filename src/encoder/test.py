import encoder
from time import sleep

SLEEP_TIME_MICROSEC=1

encoder.setup()
while True:
	print(encoder.get_angle())
	sleep(SLEEP_TIME_MICROSEC*1e-9)
