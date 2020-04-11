#include "debug.h"
#include "chip8.h"

static int key_map[0x10] =  {
  SDL_SCANCODE_X, SDL_SCANCODE_1, SDL_SCANCODE_2, SDL_SCANCODE_3,
  SDL_SCANCODE_Q, SDL_SCANCODE_W, SDL_SCANCODE_E, SDL_SCANCODE_A,
  SDL_SCANCODE_S, SDL_SCANCODE_D, SDL_SCANCODE_Z, SDL_SCANCODE_C,
  SDL_SCANCODE_4, SDL_SCANCODE_R, SDL_SCANCODE_F, SDL_SCANCODE_V
};

void sdl_draw(C8 *c, C8_display *display) {
  unsigned int pixels[W * H];

  for(int i = 0; i < (W * H); i++){
    unsigned short pixel = c->screen[i];
    pixels[i] = (0x00FFFFFF * pixel) | 0xFF000000;
  }

  SDL_UpdateTexture(display->texture, NULL, pixels, 64*sizeof(Uint32));
  SDL_RenderClear(display->renderer);
  SDL_RenderCopy(display->renderer, display->texture, NULL, NULL);
  SDL_RenderPresent(display->renderer);
  SDL_Delay(2);

}

void process_timers(C8 *c){
    if(c->delay > 0)
      c->delay--;
    if(c->timer > 0)
      c->timer--;
    if(c->timer != 0)
      printf("BEEP\n");
}

int process_keypress(SDL_Event *e){

  const Uint8 *keys = SDL_GetKeyboardState(NULL);
    if(keys[SDL_SCANCODE_ESCAPE])
      return  0;
    if(keys[SDL_SCANCODE_P]) {
       while(1){
         if(SDL_PollEvent(e)){
           if(keys[SDL_SCANCODE_ESCAPE]){
             return 0;
           } else if(keys[SDL_SCANCODE_R]){
             break;
           }
         }

       }
    }
    return 1;
}

int main(int argc, char* argv[]) {

  int run = 1;
  short ticks = 0;
  int debug_flag = 0;
  FILE *f = NULL;
  SDL_Event event;
  const Uint8 *keys;

  //create memory space for Chip 8
  C8 * c = calloc(sizeof(C8), 1);
  C8_display *display = malloc(sizeof(C8_display));

  //OPEN FILE
  if(argc == 2){
      f = fopen(argv[1], "rb");
  } else if (argc == 3) {
      f = fopen(argv[2], "rb");
      if(strcmp(argv[1], "--debug") == 0){
        debug_flag = 1;
        printf("debug_flag set.");
      } else {
        printf("Flags include --debug.\n");
        exit(1);
      }
  }

  if (f == NULL) {
    printf("Error: Could not open %s\n", argv[1]);
    printf("Usage: chip8 <flag> <path/to/rom>");
    exit(1);
  }

  //initialize memory values
  init(f, c);

  if(SDL_Init(SDL_INIT_EVERYTHING) < 0){
      printf("SDL_Init failed: %s\n", SDL_GetError());
      exit(1);
  } else {
      display->window = SDL_CreateWindow("Chip 8 Emulator", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_W, SCREEN_H, SDL_WINDOW_OPENGL);
      display->renderer = SDL_CreateRenderer(display->window, -1, SDL_RENDERER_ACCELERATED);
      display->texture = SDL_CreateTexture(display->renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, W, H);
  }

  while(run) {
    if(SDL_PollEvent(&event)){
      if(event.type == SDL_QUIT)
        exit(1);
    }

    if(debug_flag){
      dumpReg(c);
      //dumpMem(c);  //<--Remove comment and recompile to enable memory debug dump
    }

    //Keyboard vars
    uint8_t *code = &c->memory[c->pc];
    uint16_t opcode = c->memory[c->pc] << 8 | c->memory[c->pc+1];
    uint8_t byte = code[0] >> 4;

    /**
    * This is hacky but this is how I will have to abstract the keyboard functions
    * in the interpreter for each specific platform. To port this code a developer will
    * be required to provide their own platform specific implementation for these 3 opcodes
    * */
    if(byte == 0x0e) 
    {

      if(code[1] == 0x9e) //Skip next instruction if key with the value of Vx is pressed.
      {
        SDL_PumpEvents();
        keys = SDL_GetKeyboardState(NULL);

          if(keys[key_map[c->V[(opcode & 0x0F00) >> 8]]]) 
          {
            c->pc += 4;
          } else {
            c->pc += 2;
          }
      } else if (code[1] == 0xa1) //Skip next instruction if key with the value of Vx is not pressed.
      {
        SDL_PumpEvents();
        keys = SDL_GetKeyboardState(NULL);
          if(keys[key_map[c->V[(opcode & 0x0F00) >> 8]]]) {
            c->pc += 4;
          } else {
            c->pc += 2;
          }
      }
    } else if (byte == 0x0f && code[1] == 0x0a) //Wait for a key press, store the value of the key in Vx.
    {
      keys = SDL_GetKeyboardState(NULL);
            int i = 0;
            for(i=0; i <0x10; i++){
              if(keys[key_map[i]]){
                c->V[(opcode & 0x0F00) >> 8] = i;
                c->pc +=2;
              }
            }
    } else {
      executeOp(c);
    }

    

    if(ticks == 10){
      process_timers(c);
      ticks = 0;
    }

    ticks++;
    sdl_draw(c, display);
    run = process_keypress(&event);
  }


  fclose(f); // close FILE
  free(c);
  free(display);
  return 0;
}

int WinMain(int argc, char* argv[]){return main(argc, argv);}