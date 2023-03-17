from math import sqrt
import time
import pigpio
import rotary_encoder
from datetime import datetime
pi = pigpio.pi()
decoder = rotary_encoder.decoder(pi, 17, 18)

now = datetime.now()
somma_delay = 0
somma_quadratica_delay = 0
iterazioni=0

while True:
    time.sleep(921e-5)
    iterazioni += 1
    delay = (datetime.now()-now).total_seconds() * 1000
    somma_delay += delay
    somma_quadratica_delay += delay**2
    media = somma_delay/iterazioni
    print(str(decoder.get_angle())+","+str(media)+","+str(sqrt((somma_quadratica_delay / iterazioni) - (media)**2 )))
    now = datetime.now()