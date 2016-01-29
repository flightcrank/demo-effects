
//Using SDL and standard IO
#include <SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include "renderer.h"
#include "vector.h"

//Screen dimension constants
#define SCREEN_WIDTH 1080
#define SCREEN_HEIGHT 720

#define CHAR_WIDTH 48
#define CHAR_HEIGHT 50

int init(int width, int height);

SDL_Window* window = NULL;	//The window we'll be rendering to
SDL_Renderer* renderer;		//The renderer SDL will use to draw to the screen
SDL_Texture* screen;		//The texture representing the screen	
SDL_Surface* char_map;		//A source image
SDL_Texture* c_map;

char* map[] = {"abcdef","ghijkl","mnopqr","stuvwx","yz0123","456789","?!().,"};

struct vector3d get_char(char c) {
	
	int i, j;

	struct vector3d pos = {0,0,0};

	for(i = 0; i < 7; i++) {
		
		for(j = 0; j < 6; j++) {
			
			if (c == map[i][j]) {
						
				return pos;
			}

			pos.x += CHAR_WIDTH;
		}
		
		pos.y += CHAR_HEIGHT;
		pos.x = 0;
	}

	pos.x = 0;
	pos.y = 0;

	return pos;
}

int main (int argc, char* args[]) {

	//SDL Window setup
	if (init(SCREEN_WIDTH, SCREEN_HEIGHT) == 1) {
		
		return 0;
	}

	char msg[] = "this is a test string";

	SDL_Rect src, dest;

	int sleep = 0;
	int quit = 0;
	Uint32 next_game_tick = SDL_GetTicks();
	
	int i = 0;
	float sin_t[360];
	
	for (i = 0; i < 360; i++) {
		
		sin_t[i] =  sin(i * M_PI / 180);
	}
	
	int c_offset = 0;
	int d = 1;

	//render loop
	while(quit == 0) {
	
		//check for new events every frame
		SDL_PumpEvents();

		const Uint8 *state = SDL_GetKeyboardState(NULL);
		
		if (state[SDL_SCANCODE_ESCAPE]) {
		
			quit = 1;
		}
		
		SDL_RenderClear(renderer);
		
		for (i = 0; i < strlen(msg); i++) {
			
			struct vector3d v = get_char(msg[i]);
			
			if (msg[i] == ' ') {
				
				c_offset += CHAR_WIDTH;
				continue;
			}

			src.x = v.x;
			src.y = v.y;
			src.w = CHAR_WIDTH;
			src.h = CHAR_WIDTH;
	
			dest.x = c_offset;
			dest.y = SCREEN_HEIGHT / 2 + sin_t[(int) fabs(c_offset) % 360] * 25;
			dest.w = CHAR_WIDTH;
			dest.h = CHAR_HEIGHT;
			
			SDL_RenderCopy(renderer, c_map, &src, &dest);
			
			c_offset += CHAR_WIDTH;
		}

		if (d > strlen(msg) * CHAR_WIDTH + SCREEN_WIDTH) {
			
			d = 0;
		}

		c_offset = SCREEN_WIDTH - d;
		d += 5;

		//draw to the screen
		SDL_RenderPresent(renderer);
				
		//time it takes to render 1 frame in milliseconds
		next_game_tick += 1000 / 60;
		sleep = next_game_tick - SDL_GetTicks();
	
		if( sleep >= 0 ) {
            				
			SDL_Delay(sleep);
		}
	}

	//free the source image
	SDL_FreeSurface(char_map);
	
	//Destroy window 
	SDL_DestroyWindow(window);

	//Quit SDL subsystems 
	SDL_Quit(); 
	 
	return 0;
}

int init(int width, int height) {

	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {

		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		
		return 1;
	} 
	
	//Create window	
	SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN, &window, &renderer);
	
	//set up screen texture
	screen = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, SCREEN_WIDTH, SCREEN_HEIGHT);
	
	//load image into surface
	SDL_Surface* tmp = SDL_LoadBMP("260.bmp");
	
	if (tmp == NULL) { 
		
		printf ("coild not load image file SDL_Error: %s\n", SDL_GetError());
		
		return 1;
	}

	char_map = SDL_ConvertSurfaceFormat(tmp, SDL_PIXELFORMAT_RGBA8888, 0);
	SDL_FreeSurface(tmp);

	c_map = SDL_CreateTextureFromSurface(renderer, char_map);

	if (window == NULL) { 
		
		printf ("Window could not be created! SDL_Error: %s\n", SDL_GetError());
		
		return 1;
	}

	if (c_map == NULL) { 
		
		printf ("Texture could not be created! SDL_Error: %s\n", SDL_GetError());
		
		return 1;
	}

	if (screen == NULL) { 
		
		printf ("Texture could not be created! SDL_Error: %s\n", SDL_GetError());
		
		return 1;
	}
	
	if (char_map == NULL) {
	
		printf ("Error allocating surface %s\n", SDL_GetError());
		
		return 1;
	}

	return 0;
}

