#include "debug.h"
#include <stdio.h>

void instructionNotImplemented(uint16_t opcode, uint16_t pc) {
  printf("ERROR: OPCODE %04x AT PC: %04x\n", opcode, pc);
  exit(1);
}

void disassembleChip8Op(uint8_t *codebuffer, int pc) {

    uint8_t *code = &codebuffer[pc];
    uint8_t firstnib = (code[0] >> 4);

    printf("%04x %02x %02x ", pc, code[0], code[1]);

    //DOT IN NAME SIGNIFIES INSTRUCTION MODIFIES VF Register
    switch (firstnib) {
        case 0x0:
            switch (code[1]) {
              case 0x00: printf("Empty Address"); break;
              case 0xe0: printf("%-10s", "CLS"); break;
              case 0xee: printf("%-10s", "RET"); break;
              default: printf("UNKNOWN 0 Instruction"); break;
            }
            break;
        case 0x01: printf("%-10s $%01x%02x", "JUMP", code[0]&0xf, code[1]); break;
        case 0x02: printf("%-10s V%01X, #$%02x", "CALL", code[0]&0xf, code[1]); break;
        case 0x03: printf("%-10s V%01X, #$%02x", "SKIP.EQ", code[0]&0xf, code[1]); break;
        case 0x04: printf("%-10s V%01X, #$%02x", "SKIP.NE", code[0]&0xf, code[1]); break;
        case 0x05: printf("%-10s V%01X, V%02X", "SKIP.EQ", code[0]&0xf, code[1]>>4); break;
        case 0x06: printf("%-10s V%01X, #$%02x", "MVI", code[0]&0xf, code[1]); break;
        case 0x07: printf("%-10s V%02X, #$%02x", "ADD", code[0]&0xf, code[1]); break;
        case 0x08:{
          uint8_t lastnib = code[1] &0xf;
          switch(lastnib) {
            case 0x0: printf("%-10s V%01X, V%01X", "LD.", code[0]&0xf, code[1]>>4); break;
            case 0x1: printf("%-10s V%01X, V%01X", "OR.", code[0]&0xf, code[1]>>4); break;
            case 0x2: printf("%-10s V%01X, V%01X", "AND.", code[0]&0xf, code[1]>>4); break;
            case 0x3: printf("%-10s V%01X, V%01X", "XOR.", code[0]&0xf, code[1]>>4); break;
            case 0x4: printf("%-10s V%01X, V%01X", "ADD.", code[0]&0xf, code[1]>>4); break;
            case 0x5: printf("%-10s V%01X, V%01X", "SUB.", code[0]&0xf, code[1]>>4); break;
            case 0x6: printf("%-10s V%01X, V%01X", "SHR.", code[0]&0xf, code[1]>>4); break;
            case 0x7: printf("%-10s V%01X, V%01X", "SUBN.", code[0]&0xf, code[1]>>4); break;
            case 0xe: printf("%-10s V%01X, V%01X", "SHL.", code[0]&0xf, code[1]>>4); break;
            default: printf("UNKNOWN 8 Instruction"); break;
          }
        }
          break;
        case 0x09: printf("%-10s V%01X, V%01X", "SNE", code[0]&0xf, code[1] >> 4); break;
        case 0x0a: printf("%-10s I, #$%01x%02x", "MVI", code[0]&0xf, code[1]); break;
        case 0x0b: printf("%-10s $%01x%02x(V0)", "JUMP", code[0]&0xf, code[1]); break;
        case 0x0c: printf("%-10s V%01X, #$%02X", "RND", code[0]%0xf, code[1]); break;
        case 0x0d: printf("%-10s V%01X, V%01X, #$%01x", "SPRITE", code[0]&0xf, code[1]>>4, code[1]&0xf); break;
        case 0x0e:
        switch(code[1]){
          case 0x9E: printf("%-10s V%01X", "SKIPKEY.Y", code[0]&0xf); break;
          case 0xA1: printf("%-10s V%01X", "SKIPKEY.N", code[0]&0xf); break;
          default: printf("UNKNOWN E Instruction");break;
        }
        break;
        case 0x0f:
          switch(code[1]){
            case 0x07: printf("%-10s V%01X, DT", "MOV", code[0]&0xf); break;
            case 0x0a: printf("%-10s V%01X", "WAIT KEY", code[0]&0xf); break;
            case 0x15: printf("%-10s DEL, V%01X", "MOV", code[0]&0xf); break;
            case 0x18: printf("%-10s SND, V%01X", "MOV", code[0]&0xf); break;
            case 0x1e: printf("%-10s I, V%01X", "ADD", code[0]&0xf); break;
            case 0x29: printf("%-10s I, V%01X", "LD", code[0]&0xf); break;
            case 0x33: printf("%-10s (I), V%01X", "MOV BCD", code[0]&0xf); break;
            case 0x55: printf("%-10s (I), V0-V%01X", "MEM", code[0]&0xf); break;
            case 0x65: printf("%-10s V0-V%01X, (I)", "MEM", code[0]&0xf); break;
            default: printf("UNKNOWN F Instruction"); break;
          }
          break;
    }
}



void dumpReg(C8 * c) {
  printf("\n-----------------------------------------REGISTER DUMP INITIATED-----------------------------------------\n\n");
  int count = 0;

  printf("GENERAL PURPOSE REGISTERS------------\n\n");


  for(int i = 0; i < NUM_REGISTERS; i++){
    printf("V%01X[%02x] ", i, c->V[i]);
    count++;

    if(count == 2) {
      count = 0;
      printf("\n");
    }
  }
  printf("\nSPECIAL REGISTERS--------------------\n\n");
  printf("I[%04X]\n", c->I);
  printf("PC[%04X]\n", c->pc);
  printf("SP[%02X]\n", c->sp);
  printf("DELAY[%02X]\n", c->delay);
  printf("TIMER[%02X]\n", c->timer);
  printf("\n");

  printf("\nSTACK--------------------------------\n\n");

  for(int i = 0; i < NUM_REGISTERS; i++){
    printf("STACK FRAME(%02d) %04x ", i, c->stack[i]);
    if((i-1)%2 == 0) {
      printf("\n");
    }
  }
  printf("\n");
}

void dumpMem(C8 * c){

  int i;
  int count = 0;

  printf("\n-----------------------------------------MEMORY DUMP INITIATED-----------------------------------------\n");
  printf("\n\n");
  printf("BEGIN INTERPRETER MEMORY(0x000 - 0x01FF)---------------------------------------------------------------\n\n");

  for(i = 0; i < MEM_SIZE; i+=2){
    if(i == 0x200){
      printf("\nBEGIN PROGRAM MEMORY(0x200 - 0xFFF)-------------------------------------------------------------------\n\n");
    }

    printf("A:%04X %02x%02x  ", i, c->memory[i], c->memory[i+1]);
    count++;

    if(count == 8) {
      printf("\n");
      count = 0;
    }
  }
  printf("\n");
}