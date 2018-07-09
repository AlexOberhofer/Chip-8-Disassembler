#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef struct C8 {

  uint8_t V[0x10];
  uint16_t I;
  uint16_t sp;
  uint16_t pc;
  uint8_t delay;
  uint8_t timer;
  uint8_t *memory;
  uint8_t *screen; //starts at 0xF00

} C8;

void DisassembleChip8Op(uint8_t *codebuffer, int pc);
C8* init(void);
