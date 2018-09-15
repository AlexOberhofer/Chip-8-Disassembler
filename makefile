CC=gcc
CFLAGS=-I /usr/include/SDL2
DEPS= chip8.h
TARGET= chip8

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

makechip8disassembler: src/chip8.o
		$(CC) -o ./bin/chip8 ./src/$(TARGET).c `sdl2-config --cflags --libs`

clean:
	rm ./bin/chip8
