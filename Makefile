.PHONY: main

main:
	g++ -Wall -o stabilizer stabilizer.cpp src/encoder/*.cpp src/encoder/rotary_encoder/*.cpp src/timer/*.cpp -lpigpio -lrt 