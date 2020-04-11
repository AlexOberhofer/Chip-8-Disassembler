CC=gcc
CFLAGS=-I /usr/include/SDL2
DEPS= chip8.h debug.h
TARGET= chip8

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

makechip8disassembler: src/debug.o src/chip8.o src/system.o
		@mkdir -p bin/linux
		$(CC) -Wall -std=c99 -o ./bin/linux/chip8 ./src/debug.c ./src/chip8.c  ./src/system.c `sdl2-config --cflags --libs`

clean:
	rm ./bin/linux/chip8
	rm ./src/chip8.o
	rm ./src/debug.o
	rm ./src/system.o
	rm -rf ./release/