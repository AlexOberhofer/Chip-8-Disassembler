CC=gcc
CFLAGS=-I.
DEPS= chip8diss.h
TARGET= chip8dis

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

makechip8disassembler: chip8dis.o
		$(CC) -o $(TARGET) $(TARGET).o 

clean:
	($RM) $(TARGET)
