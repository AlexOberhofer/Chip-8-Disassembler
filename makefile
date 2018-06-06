CC=gcc
CFLAGS=-I.
DEPS= chip8diss.h
TARGET= chip8dis

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

makechip8disassembler: src/chip8dis.o
		$(CC) -o bin/chip-8-disassembler src/$(TARGET).o

clean:
	rm bin/chip-8-disassembler
