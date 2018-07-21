CC=gcc
CFLAGS=-I /usr/include/SDL2
DEPS= chip8diss.h
TARGET= chip8dis

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

makechip8disassembler: src/chip8dis.o
		$(CC) -o ./bin/chip8 ./src/$(TARGET).c `sdl2-config --cflags --libs`

clean:
	rm ./bin/chip8
