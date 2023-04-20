OBJS	= src/encoder/rotary_encoder/rotary_encoder.o src/encoder/Encoder.o src/timer/timerStats/TimerStats.o src/timer/Timer.o stabilizer.o src/robot/mecawrapper/mecawrapper.o src/robot/Robot.o Constants.o src/csvlogger/CsvLogger.o src/controller/FeedbackController.o
SOURCE	= src/encoder/rotary_encoder/rotary_encoder.cpp src/encoder/Encoder.cpp src/timer/timerStats/TimerStats.cpp src/timer/Timer.cpp stabilizer.cpp src/robot/mecawrapper/mecawrapper.c src/robot/Robot.cpp Constants.cpp src/csvlogger/CsvLogger.cpp src/controller/FeedbackController.cpp

OUT	= stabilizer
CC	 = chrt -r 99 g++
FLAGS	 = -c -W -Wall -Wextra
LFLAGS	 = -lpigpio -lrt `python3-config --cflags --ldflags --embed`

all: $(OBJS)
	$(CC) -g $(OBJS) -o $(OUT) $(LFLAGS)

src/robot/mecawrapper/mecawrapper.c : src/robot/mecawrapper/mecawrapper.pyx
	cythonize -3 src/robot/mecawrapper/mecawrapper.pyx

src/encoder/rotary_encoder/rotary_encoder.o: src/encoder/rotary_encoder/rotary_encoder.cpp
	$(CC) -o src/encoder/rotary_encoder/rotary_encoder.o $(FLAGS) src/encoder/rotary_encoder/rotary_encoder.cpp

src/encoder/Encoder.o: src/encoder/Encoder.cpp
	$(CC) -o src/encoder/Encoder.o $(FLAGS) src/encoder/Encoder.cpp

Constants.o: Constants.cpp Constants.hpp
	$(CC) -o Constants.o $(FLAGS) Constants.cpp

src/timer/timerStats/TimerStats.o: src/timer/timerStats/TimerStats.cpp src/timer/timerStats/TimerStats.hpp
	$(CC) -o src/timer/timerStats/TimerStats.o $(FLAGS) src/timer/timerStats/TimerStats.cpp

src/timer/Timer.o: src/timer/Timer.cpp
	$(CC) -o src/timer/Timer.o $(FLAGS) src/timer/Timer.cpp

src/robot/Robot.o: src/robot/Robot.cpp src/robot/mecawrapper/mecawrapper.c
	$(CC) -o src/robot/Robot.o $(FLAGS) src/robot/Robot.cpp $(LFLAGS)

stabilizer.o: stabilizer.cpp src/robot/mecawrapper/mecawrapper.c Constants.hpp
	$(CC) -o stabilizer.o $(FLAGS) stabilizer.cpp $(LFLAGS)

src/robot/mecawrapper/mecawrapper.o: src/robot/mecawrapper/mecawrapper.c
	$(CC) -o src/robot/mecawrapper/mecawrapper.o $(FLAGS) src/robot/mecawrapper/mecawrapper.c $(LFLAGS)

src/csvlogger/CsvLogger.o: src/csvlogger/CsvLogger.cpp
	$(CC) -o src/csvlogger/CsvLogger.o $(FLAGS) src/csvlogger/CsvLogger.cpp

src/controller/FeedbackController.o: src/controller/FeedbackController.cpp
	$(CC) -o src/controller/FeedbackController.o $(FLAGS) src/controller/FeedbackController.cpp
