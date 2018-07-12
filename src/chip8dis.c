#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "chip8dis.h"
/*
CHIP 8 NOTES

- Each Instruction is 2 bytes represented by 4 HEXIDECIMAL Digits
How to access each bit -
0 - code[0] >> 4
1 - code[0] &0xf
2 - code[1] >> 4
3 - code[1] &0xf;

nnn or addr - A 12-bit value, the lowest 12 bits of the instruction
n or nibble - A 4-bit value, the lowest 4 bits of the instruction
x - A 4-bit value, the lower 4 bits of the high byte of the instruction
y - A 4-bit value, the upper 4 bits of the low byte of the instruction
kk or byte - An 8-bit value, the lowest 8 bits of the instruction

- When an instruction is presented containing non-hexadecimal characters, these
locations should be replaced in a program with valid hexadecimal digits depending upon the input data.
For example, valid uses of the CHIP-8 instruction 8XY1 include 8001, 81A1, 8F21, etc.

- Consider the 1NNN, instruction, which is used to jump to a certain address. A valid
use of this instruction would be 134A, which would reference the memory address 34A.

- CHIP-8 programs are normally stored in memory in big-endian fashion, with the most
 significant byte of a two-byte instruction being stored first.
*/



void executeOp(C8* c) {
  uint8_t *code = &c->memory[c->pc];
  uint16_t opcode = c->memory[c->pc] << 8 | c->memory[c->pc+1];
  uint8_t firstnib = code[0] >> 4;

  switch (firstnib) {

      case 0x0:
          switch (code[1]) {
            case 0xe0: //Clear the display.
              memset(c->screen, 0, sizeof(c->screen)); c->pc +=2; break;
            case 0xee: //Return from a subroutine.
              c->pc=c->stack[(--c->sp) & 0xf] + 2 ;
              break;
            default: instructionNotImplemented(opcode, c->pc); break;
          }
          break;

      case 0x01: //Jump to location nnn.
        c->pc = opcode & 0x0FFF;
        break;

      case 0x02: //Call subroutine at nnn.
        c->stack[(c->sp++)&0xF] = c->pc;
        c->pc = opcode & 0x0FFF; break;

      case 0x03: //Skip next instruction if Vx = kk.
        if(c->V[(opcode & 0x0F00) >> 8] == opcode & 0x00FF) {
          c->pc +=4;
        } else {
          c->pc +=2;
        } break;

      case 0x04: //Skip next instruction if Vx != kk.
        if(c->V[(opcode & 0x0F00) >> 8] != opcode & 0x00FF) {
          c->pc +=4;
        } else {
          c->pc +=2;
        } break;

      case 0x05: //Skip next instruction if Vx = Vy.
        if(c->V[(opcode & 0x0F00) >> 8] == c->V[opcode & 0x00F0 >> 4]) {
          c->pc +=4;
        } else {
          c->pc +=2;
        }  break;

      case 0x06: //Set Vx = kk.
        c->V[(opcode & 0x0F00) >> 8] = opcode & 0x00FF;
        c->pc += 2; break;

      case 0x07: //Set Vx = Vx + kk.
        c->V[(opcode & 0x0F00) >> 8] += (opcode & 0x00FF);
        c->pc += 2; break;

      case 0x08:{
        uint8_t lastnib = code[1] &0xf;
        switch(lastnib) {

          case 0x0: //Set Vx = Vy.
            c->V[(opcode & 0x0F00) >> 8] = c->V[(opcode & 0x00F0) >> 4];
            c->pc += 2; break;

          case 0x1: //Set Vx = Vx OR Vy
            c->V[(opcode & 0x0F00) >> 8] = c->V[(opcode & 0x0F00) >> 8] | c->V[(opcode & 0x00F0) >> 4];
            c->pc += 2; break;

          case 0x2: //Set Vx = Vx AND Vy.
            c->V[(opcode & 0x0F00) >> 8] = c->V[(opcode & 0x0F00) >> 8] & c->V[(opcode & 0x00F0) >> 4];
            c->pc += 2; break;

          case 0x3: //Set Vx = Vx XOR Vy
            c->V[(opcode & 0x0F00) >> 8] = c->V[(opcode & 0x0F00) >> 8] ^ c->V[(opcode & 0x00F0) >> 4];
            c->pc += 2; break;
          case 0x4: //Set Vx = Vx + Vy, set VF = carry.
            if(((int)c->V[(opcode & 0x0F00) >> 8] + (int) c->V[(opcode & 0x00F0) >> 4] < 256)) {
              c->V[0xf] &= 0;
            } else {
              c->V[0xf] = 1;
            }
            c->V[(opcode & 0x0F00) >> 8] += c->V[(opcode & 0x00F0) >> 4];
            c->pc += 2; break;
          case 0x5: //Set Vx = Vx - Vy, set VF = NOT borrow.
          if(((int)c->V[(opcode & 0x0F00) >> 8] - (int) c->V[(opcode & 0x00F0) >> 4] >= 0)) {
            c->V[0xf] &= 0;
          } else {
            c->V[0xf] = 1;
          }
          c->V[(opcode & 0x0F00) >> 8] -= c->V[(opcode & 0x00F0 >> 4)];
            c->pc +=2; break;
          case 0x6: instructionNotImplemented(opcode, c->pc); break;
          case 0x7: instructionNotImplemented(opcode, c->pc); break;
          case 0xe: instructionNotImplemented(opcode, c->pc); break;
          default: instructionNotImplemented(opcode, c->pc); break;
        }
      }
      break;
      case 0x09: instructionNotImplemented(opcode, c->pc); break;
      case 0x0a: instructionNotImplemented(opcode, c->pc); break;
      case 0x0b: instructionNotImplemented(opcode, c->pc); break;
      case 0x0c: instructionNotImplemented(opcode, c->pc); break;
      case 0x0d: instructionNotImplemented(opcode, c->pc); break;
      case 0x0e:
      switch(code[1]){
        case 0x9E: instructionNotImplemented(opcode, c->pc); break;
        case 0xA1: instructionNotImplemented(opcode, c->pc); break;
        default: printf("UNKNOWN E Instruction");break;
      }
      break;
      case 0x0f:
        switch(code[1]){
          case 0x07: instructionNotImplemented(opcode, c->pc); break;
          case 0x0a: instructionNotImplemented(opcode, c->pc); break;
          case 0x15: instructionNotImplemented(opcode, c->pc); break;
          case 0x18: instructionNotImplemented(opcode, c->pc); break;
          case 0x1e: instructionNotImplemented(opcode, c->pc); break;
          case 0x29: instructionNotImplemented(opcode, c->pc); break;
          case 0x33: instructionNotImplemented(opcode, c->pc);; break;
          case 0x55: instructionNotImplemented(opcode, c->pc); break;
          case 0x65: instructionNotImplemented(opcode, c->pc); break;
          default: printf("UNKNOWN F Instruction"); break;
        }
        break;
  }

  //TODO: Add some timer counting here
}

void instructionNotImplemented(uint16_t opcode, uint16_t pc) {
  printf("ERROR: OPCODE %04x AT PC: %04x\n", opcode, pc);
  exit(-1);
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

void init(FILE *f, C8 * c) {

  c-> memory = calloc(4096, 1);
  c-> sp = 0;
  c-> pc = 0x200;

  //Place fonts in portion of memory that  would normally be occupied
  //by the CH8 Interpreter
  for(int i = 0; i < 80; i++){
    c->memory[i] = fonts[i];
  }

  //initialize memory values
  memset(c->screen, 0, sizeof(c->screen));
  memset(c->V, 0, sizeof(c->V));
  memset(c->stack, 0, sizeof(c->stack));
  fread(c->memory+0x200, 1, MEM_SIZE-0x200, f);

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
    printf("STACK FRAME(%02d) %04x\n", i, c->stack[i]);
  }
  printf("\n");
}

void dumpMem(C8 * c){

  printf("\n-----------------------------------------MEMORY DUMP INITIATED-----------------------------------------\n");
  printf("\n\n");
  printf("BEGIN INTERPRETER MEMORY(0x000 - 0x01FF)---------------------------------------------------------------\n\n");

  int count = 0;

  for(int i = 0; i < MEM_SIZE; i+=2){
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

int main(int argc, char* argv[]) {


  //OPEN FILE
  FILE *f = fopen(argv[1], "rb");
  if (f == NULL) {
    printf("Error: Could not open %s\n", argv[1]);
    exit(1);
  }

  //create memory space for Chip 8
  C8 * c = calloc(sizeof(C8), 1);

  //initialize memory values
  init(f, c);

  //dumpMem(c);

  //dumpReg(c);


  //get size of file
  fseek(f, 0L, SEEK_END);
  int fsize = ftell(f);

  //CHIP 8 Program memory starts at 0x200
  //Thus we need to expect this
  //Read file into memory at 0x200 and close.
  /*
  unsigned char *buffer = malloc(fsize + 0x200); //create a buffer
  fread(buffer+0x200, fsize, 1, f); //read file into memory
  */


  while(c->pc < (fsize + 0x200)) {
    executeOp(c);
    //dumpReg(c);
    printf("\n");
  }



  fclose(f); // close FILE

  return 0;
}
