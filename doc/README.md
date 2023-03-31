
# Documentazione

```mermaid
    C4Context
      title Interazione componenti
        System(Main, "Main")
        Boundary(robot, "Gestione robot") {
            System(Robot, "Robot")
        System(mecawrapper, "mecawrapper")
        System(MecademicRobot, "MecademicRobot")
    }

    Boundary(encoder, "Gestione encoder") {
        System(Encoder, "Encoder")
        System(rotary_encoder, "rotary_encoder")
    }

    Boundary(timer, "Tempificazione") {
        System(Timer, "Timer")
        System(TimerStats, "TimerStats")
    }

        Rel(Main, Encoder, "usa")
        Rel(Main, Timer, "usa")
        Rel(Main, Robot, "usa")

        BiRel(Encoder,rotary_encoder,"riceve messaggi quando l'encoder gira")

        Rel(Robot, mecawrapper, "chiama l'interfaccia con python")
        Rel(mecawrapper, MecademicRobot, "utilizza le funzioni della libreria python")

        BiRel(Timer, TimerStats, "ottiene statistiche sulla tempificazione")

        UpdateRelStyle(Main, Encoder, $offsetY="-10")
        UpdateRelStyle(Main, Timer, $offsetY="-80",$offsetX="-250")
        UpdateRelStyle(Main, Robot, $offsetY="-10")

        UpdateLayoutConfig($c4ShapeInRow="6", $c4BoundaryInRow="4")
        
```



# Gestione encoder

Il funzionamento dell'encoder è astratto dalla classe Encoder, che dipende dalla classe re_decoder.

```mermaid
---
title: Gestione encoder
---
classDiagram
    class Encoder {
        - CLK_GPIO : const int
        - DT_GPIO : const int
        - RESOLUTION_DEGREES : const double
        - RESOLUTION_RADIANS : const double
        - PPR : static int
        - pos : static int
        - MAX_POS : static int
        - MIN_POS : static int
        - decoder : re_decoder
        - callback(way : int)$
        + Encoder(clk_gpio : int, dt_gpio : int, ppr : int, start_angle_degrees : int)
        + get_angle() double
        + get_angle_degrees() double
    }
    class re_decoder {
        - mygpioA : int
        - mygpioB : int
        - levA : int
        - levB : int
        - lastGpio : int
        - mycallback : re_decoderCB_t
        - _pulse(gpio : int, level : int, tick : uint32_t)
        - _pulseExc(gpio : int, level : int, tick : uint32_t, user : void*)$
        + re_decoder(gpioA : int, gpioB : int, callback : re_decoderCB_t)
        + re_cancel()
    }
    Encoder "1"o--"1" re_decoder
```

La classe re_decoder si interfaccia direttamente con la libreria pigpio. Il suo costruttore richiede, oltre ai pin, 
un puntatore a una funzione callback che verrà richiamata quando l'encoder verrà girato. Nel nostro caso
sarà la funzione callback() della classe Encoder.
Quando l'encoder viene girato viene richiamato il metodo statico _pulseExc della classe re_decoder che
richiama a sua volta il metodo _pulse. Il metodo _pulse decide quale valore richiamare il callback
attraverso la seguente tabella.

| new clk | new dt | old clk | old dt | result      |
|:-------:|:------:|---------|--------|-------------|
| 0       | 0      | 0       | 0      | no movement |
| 0       | 0      | 0       | 1      | +1          |
| 0       | 0      | 1       | 0      | -1          |
| 0       | 0      | 1       | 1      | +2          |
| 0       | 1      | 0       | 0      | -1          |
| 0       | 1      | 0       | 1      | no movement |
| 0       | 1      | 1       | 0      | -2          |
| 0       | 1      | 1       | 1      | +1          |
| 1       | 0      | 0       | 0      | +1          |
| 1       | 0      | 0       | 1      | -2          |
| 1       | 0      | 1       | 0      | no movement |
| 1       | 0      | 1       | 1      | -1          |
| 1       | 1      | 0       | 0      | +2          |
| 1       | 1      | 0       | 1      | -1          |
| 1       | 1      | 1       | 0      | +1          |
| 1       | 1      | 1       | 1      | no movement |

Il metodo callback incrementa il campo pos di Encoder assicurandosi che
resti in (-PPR/2,PPR/2]

```
void Encoder::callback(int way) {
    Encoder::pos += way;
    if(Encoder::pos > Encoder::MAX_POS) {
        Encoder::pos -= Encoder::PPR;
    }
    else if(Encoder::pos <= Encoder::MIN_POS) {
        Encoder::pos += Encoder::PPR;
    }
}
```

# Gestione robot

```mermaid
---
title: Gestione robot
---

classDiagram
    class Robot {
        - BYPASS_ROBOT : const bool
        - py_init(robot_ip : const char*, bypass_robot : bool)
        + Robot(robot_ip : const char*, bypass_robot : bool)
        + connect()
        + activate()
        + home()
        + deactivate()
        + disconnect()
        + reset_error()
        + print_number(number : double)
    }
```

# Tempificazione

```mermaid
---
title: Tempificazione
---
classDiagram
    class Timer {
        - TARGET_CYCLE_TIME_MICROSECONDS : const uint32_t
        - DELAY_FEEDBACK_GAIN : const float
        - AGGRESSIVE_MODE : const bool
        - current_cycle_start_microseconds : uint64_t
        - time_stats : TimerStats
        - microseconds()$ uint64_t
        + Timer(target_cycke_time_microseconds : uint32_t, delay_feedback_gain : float, aggressive_mode : bool)
        + start_cycle()
        + end_cycle()
        + get_mean_cycle_time() double
        + get_standard_deviation_cycle_time() double
        + get_max_cycle_time() uint32_t
        + get_min_cycle_time() uint32_t
    }

    class TimerStats {
        - BASE_CYCLE_TIME : const uint32_t
        - sum_cycle_times : double
        - quadratic_sum_cycle_times : double
        - max_cycle_time : uint32_t
        - min_cycle_time : uint32_t
        - cycles : uint32_t
        + TimerStats(base_cycle_time : uint32_t)
        + set_last_cycle_time(last_cycle_time_microseconds : uint32_t)
        + get_mean() : double
        + get_standard_deviation() double
        + get_max() uint32_t
        + get_min() uint32_t
    }
    Timer "1"o--"1" TimerStats
```