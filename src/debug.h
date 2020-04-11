#include "chip8.h"

void dumpMem(C8 * c);
void dumpReg(C8 * c);
void instructionNotImplemented(uint16_t opcode, uint16_t pc);
void disassembleChip8Op(uint8_t *codebuffer, int pc);