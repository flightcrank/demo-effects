
//font.h

#include <SDL.h>

SDL_Texture * load_font(char *file, SDL_Renderer *rend);

int get_char_width();

int get_char_height();

void font_set_scale(float v);

void font_set_v_gap(float v);

void font_set_h_gap(float v);

void print_str(char *str, int x, int y, SDL_Renderer *rend, SDL_Texture *tex);

void print_char(char c, int x, int y, SDL_Renderer *rend, SDL_Texture *tex);
