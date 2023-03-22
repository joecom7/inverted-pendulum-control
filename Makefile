OBJS	= src/encoder/rotary_encoder/rotary_encoder.o src/encoder/Encoder.o src/timer/timerStats/TimerStats.o src/timer/Timer.o stabilizer.o src/cythontest/cythontest.o
SOURCE	= src/encoder/rotary_encoder/rotary_encoder.cpp src/encoder/Encoder.cpp src/timer/timerStats/TimerStats.cpp src/timer/Timer.cpp stabilizer.cpp src/cythontest/cythontest.c

OUT	= stabilizer
CC	 = g++
FLAGS	 = -c -Wall
LFLAGS	 = -lpigpio -lrt `python3-config --cflags --ldflags --embed`

all: $(OBJS)
	$(CC) -g $(OBJS) -o $(OUT) $(LFLAGS)

src/cythontest/cythontest.c : src/cythontest/cythontest.pyx
	cythonize -3 src/cythontest/cythontest.pyx

src/encoder/rotary_encoder/rotary_encoder.o: src/encoder/rotary_encoder/rotary_encoder.cpp
	$(CC) -o src/encoder/rotary_encoder/rotary_encoder.o $(FLAGS) src/encoder/rotary_encoder/rotary_encoder.cpp

src/encoder/Encoder.o: src/encoder/Encoder.cpp
	$(CC) -o src/encoder/Encoder.o $(FLAGS) src/encoder/Encoder.cpp

src/timer/timerStats/TimerStats.o: src/timer/timerStats/TimerStats.cpp
	$(CC) -o src/timer/timerStats/TimerStats.o $(FLAGS) src/timer/timerStats/TimerStats.cpp

src/timer/Timer.o: src/timer/Timer.cpp
	$(CC) -o src/timer/Timer.o $(FLAGS) src/timer/Timer.cpp

stabilizer.o: stabilizer.cpp src/cythontest/cythontest.c constants.h
	$(CC) -o stabilizer.o $(FLAGS) stabilizer.cpp $(LFLAGS)

src/cythontest/cythontest.o: src/cythontest/cythontest.c
	$(CC) -o src/cythontest/cythontest.o $(FLAGS) src/cythontest/cythontest.c $(LFLAGS)