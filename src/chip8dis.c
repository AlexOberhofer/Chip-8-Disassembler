#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "chip8dis.h"

void DisassembleChip8Op(uint8_t *codebuffer, int pc) {

    uint8_t *code = &codebuffer[pc];
    uint8_t firstnib = (code[0] >> 4);

    printf("%04x %02x %02x ", pc, code[0], code[1]);

    //DOT IN NAME SIGNIFIES INSTRUCTION MODIFIES VF
    switch (firstnib)
    {
        case 0x0:
            switch (code[1]) {
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
        case 0x09: printf("9 not handled yet"); break;
        case 0x0a:
            {
                uint8_t addresshi = code[0] & 0x0f;
                printf("%-10s I,#$%01x%02x", "MVI", addresshi, code[1]);
            }
            break;
        case 0x0b: printf("b not handled yet"); break;
        case 0x0c: printf("c not handled yet"); break;
        case 0x0d: printf("d not handled yet"); break;
        case 0x0e: printf("e not handled yet"); break;
        case 0x0f: printf("f not handled yet"); break;
    }
}

int main(int argc, char** argv[]) {

  //OPEN FILE
  FILE *f = fopen(argv[1], "rb");
  if (f == NULL) {
    printf("Error: Could not open %s\n", argv[1]);
  }

  //get size of file
  fseek(f, 0L, SEEK_END);
  int fsize = ftell(f);
  fseek(f, 0L, SEEK_SET);

  //CHIP 8 Program memory starts at 0x200
  //Thus we need to expect this
  //Read file into memory at 0x200 and close.
  unsigned char *buffer = malloc(fsize + 0x200); //create a buffer
  fread(buffer+0x200, fsize, 1, f); //read file into memory
  fclose(f); // close FILE

  int pc = 0x200; // start PC at 0x200 address

  while(pc < (fsize + 0x200)) {
    DisassembleChip8Op(buffer, pc);
    pc += 2;
    printf("\n");
  }

  return 0;
}
