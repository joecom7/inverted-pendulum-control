OBJS	= src/encoder/rotary_encoder/rotary_encoder.o src/encoder/Encoder.o src/timer/timerStats/TimerStats.o src/timer/Timer.o stabilizer.o src/mecawrapper/mecawrapper.o
SOURCE	= src/encoder/rotary_encoder/rotary_encoder.cpp src/encoder/Encoder.cpp src/timer/timerStats/TimerStats.cpp src/timer/Timer.cpp stabilizer.cpp src/mecawrapper/mecawrapper.c

OUT	= stabilizer
CC	 = g++
FLAGS	 = -c -Wall
LFLAGS	 = -lpigpio -lrt `python3-config --cflags --ldflags --embed`

all: $(OBJS)
	$(CC) -g $(OBJS) -o $(OUT) $(LFLAGS)

src/mecawrapper/mecawrapper.c : src/mecawrapper/mecawrapper.pyx
	cythonize -3 src/mecawrapper/mecawrapper.pyx

src/encoder/rotary_encoder/rotary_encoder.o: src/encoder/rotary_encoder/rotary_encoder.cpp
	$(CC) -o src/encoder/rotary_encoder/rotary_encoder.o $(FLAGS) src/encoder/rotary_encoder/rotary_encoder.cpp

src/encoder/Encoder.o: src/encoder/Encoder.cpp
	$(CC) -o src/encoder/Encoder.o $(FLAGS) src/encoder/Encoder.cpp

src/timer/timerStats/TimerStats.o: src/timer/timerStats/TimerStats.cpp
	$(CC) -o src/timer/timerStats/TimerStats.o $(FLAGS) src/timer/timerStats/TimerStats.cpp

src/timer/Timer.o: src/timer/Timer.cpp
	$(CC) -o src/timer/Timer.o $(FLAGS) src/timer/Timer.cpp

stabilizer.o: stabilizer.cpp src/mecawrapper/mecawrapper.c constants.hpp
	$(CC) -o stabilizer.o $(FLAGS) stabilizer.cpp $(LFLAGS)

src/mecawrapper/mecawrapper.o: src/mecawrapper/mecawrapper.c
	$(CC) -o src/mecawrapper/mecawrapper.o $(FLAGS) src/mecawrapper/mecawrapper.c $(LFLAGS)