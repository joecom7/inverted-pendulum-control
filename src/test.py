import time
import pigpio
import rotary_encoder
pi = pigpio.pi()
decoder = rotary_encoder.decoder(pi, 17, 18)

while True:
    time.sleep(1)
    print(decoder.get_angle())