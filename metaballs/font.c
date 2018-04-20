
//font.c

#include <SDL.h>
#include "font.h"

//The width and height of each char in the font image
#define CHAR_WIDTH 16
#define CHAR_HEIGHT 25

float scale = 1;
float v_gap = 1;
float h_gap = 1;

SDL_Texture * load_font(char *file, SDL_Renderer *rend) {

	SDL_Surface *temp = SDL_LoadBMP(file);
	SDL_Texture *tex = SDL_CreateTextureFromSurface(rend, temp);
	SDL_FreeSurface(temp);
	
	if (temp == NULL) {
	
		printf("Could not load BMP file %s:%s", file, SDL_GetError());
		return NULL;
	}
	
	if (tex == NULL) {
	
		printf("Could not create font texture :%s", SDL_GetError());
		return NULL;
	}
	
	return tex;
}

int get_char_width() {
	
	return CHAR_WIDTH * scale * h_gap;	
}

int get_char_height() {

	return CHAR_HEIGHT * scale * v_gap;
}

void font_set_scale(float v) {
	
	scale = v;	
}

void font_set_v_gap(float v){

	v_gap = v;
}

void font_set_h_gap(float v) {
	
	h_gap = v;
}

void print_str(char *str, int x, int y, SDL_Renderer *rend, SDL_Texture *tex) {

    while (*str != '\0') {
		print_char(*str++, x, y, rend, tex);
		x += CHAR_WIDTH * scale;
	}
}

//assumes the bitmap'd font is all on the same y axis (all on the same line)
void print_char(char c, int x, int y, SDL_Renderer *rend, SDL_Texture *tex) {
	
	//string representation of the layout of the bitmap font
    const char cmap[] = " abcdefghijklmnopqrstuvwxyz"
                        "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!@#$%^&*()[];':\",./\\?";
	
	int i;
	
	SDL_Rect src;
	src.w = CHAR_WIDTH; //each char is 17px wide
	src.h = CHAR_HEIGHT; //each char is 24 pix tall
	src.x = 0;
	src.y = 0;
	
	for (i = 0; i < sizeof cmap; i++) {
	
		if (c == cmap[i]) {
		
			break;
		}
		
		src.x += src.w;
	}

	if (i < sizeof cmap) { // if character match found
        SDL_Rect dest;
        dest.w = src.w * scale;
        dest.h = src.h * scale;
        dest.x = x;
        dest.y = y;

        SDL_RenderCopy(rend, tex, &src, &dest);
    }
}
