from math import sin, sqrt
import time
import pigpio
import rotary_encoder
from datetime import datetime
import os

param = os.sched_param(os.sched_get_priority_max(os.SCHED_FIFO))
os.sched_setscheduler(0, os.SCHED_FIFO, param)
pi = pigpio.pi()
decoder = rotary_encoder.decoder(pi, 17, 18)

TEMPO_CICLO_DESIDERATO = 10e-3

current_time = time.time()
time.sleep(TEMPO_CICLO_DESIDERATO)
last_delay = 0
estimated_delay = 0
coefficiente_stima_delay = 0.2
somma_tempi_cicli = TEMPO_CICLO_DESIDERATO 
somma_quadratica_tempi_cicli = TEMPO_CICLO_DESIDERATO**2
media_tempo_ciclo = TEMPO_CICLO_DESIDERATO
deviazione_standard_tempi_cicli = 0
numero_iterazioni = 1

while True:

    start_time = time.time()

    # attività
    
    print(f"{decoder.get_angle_degrees():10.3f},{(TEMPO_CICLO_DESIDERATO + last_delay)*1000:10.3f},{deviazione_standard_tempi_cicli*1000:10.3f},{media_tempo_ciclo*1000:10.3f}")
    for i in range(1000):
        pass

    # metriche

    numero_iterazioni += 1
    tempo_esecuzione_ultimo_ciclo = TEMPO_CICLO_DESIDERATO + last_delay
    somma_tempi_cicli += tempo_esecuzione_ultimo_ciclo
    somma_quadratica_tempi_cicli += tempo_esecuzione_ultimo_ciclo**2
    media_tempo_ciclo = somma_tempi_cicli/numero_iterazioni
    deviazione_standard_tempi_cicli = sqrt(somma_quadratica_tempi_cicli/numero_iterazioni - media_tempo_ciclo**2)
    seno = sin(deviazione_standard_tempi_cicli)

    # algoritmo per tenere il tempo di esecuzione per ciclo costante
    last_cycle_time = current_time
    current_time = time.time()
    #print(f"{(current_time - last_cycle_time).total_seconds()},{TEMPO_CICLO_DESIDERATO}")
    last_delay = (current_time - last_cycle_time) - TEMPO_CICLO_DESIDERATO
    estimated_delay = coefficiente_stima_delay * (media_tempo_ciclo - TEMPO_CICLO_DESIDERATO) + (1 - coefficiente_stima_delay) * last_delay
    #print(f"{estimated_delay}")

    time.sleep(max(0,TEMPO_CICLO_DESIDERATO - (time.time() - start_time) - max(0,0.3*estimated_delay)))
