#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <SDL2/SDL.h>

#define MEM_SIZE 4096
#define NUM_REGISTERS 16
#define SCREEN_W 640
#define SCREEN_H 320
#define SCREEN_BPP 32
#define W 64
#define H 32

typedef struct C8 {

  uint8_t V[NUM_REGISTERS];
  uint16_t stack[NUM_REGISTERS];
  short I;
  uint8_t sp;
  uint16_t pc;
  uint8_t delay;
  uint8_t timer;
  unsigned char *memory;
  unsigned char screen[64*32]; //starts at 0xF00

} C8;

typedef struct C8_display {
    SDL_Window *window;
    SDL_Renderer* renderer;
    SDL_Texture* texture;
} C8_display;

unsigned char fonts[80] =
{
  0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
  0x20, 0x60, 0x20, 0x20, 0x70,
  0xF0, 0x10, 0xF0, 0x80, 0xF0,
  0xF0, 0x10, 0xF0, 0x10, 0xF0,
  0x90, 0x90, 0xF0, 0x10, 0x10,
  0xF0, 0x80, 0xF0, 0x10, 0xF0,
  0xF0, 0x80, 0xF0, 0x90, 0xF0,
  0xF0, 0x10, 0x20, 0x40, 0x40,
  0xF0, 0x90, 0xF0, 0x90, 0xF0,
  0xF0, 0x90, 0xF0, 0x10, 0xF0,
  0xF0, 0x90, 0xF0, 0x90, 0x90,
  0xE0, 0x90, 0xE0, 0x90, 0xE0,
  0xF0, 0x80, 0x80, 0x80, 0xF0,
  0xE0, 0x90, 0x90, 0x90, 0xE0,
  0xF0, 0x80, 0xF0, 0x80, 0xF0,
  0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

static int key_map[0x10] =  {
  SDL_SCANCODE_X, SDL_SCANCODE_1, SDL_SCANCODE_2, SDL_SCANCODE_3,
  SDL_SCANCODE_Q, SDL_SCANCODE_W, SDL_SCANCODE_E, SDL_SCANCODE_A,
  SDL_SCANCODE_S, SDL_SCANCODE_D, SDL_SCANCODE_Z, SDL_SCANCODE_C,
  SDL_SCANCODE_4, SDL_SCANCODE_R, SDL_SCANCODE_F, SDL_SCANCODE_V
};

void instructionNotImplemented(uint16_t opcode, uint16_t pc);
void disassembleChip8Op(uint8_t *codebuffer, int pc);
int process_keypress(SDL_Event *e);
void executeOp(C8* c);
void init(FILE *f, C8 * c);
void sdl_draw(C8 *c, C8_display *display);
void dumpMem(C8 * c);
void dumpReg(C8 * c);
