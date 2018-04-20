//main.c 

//Using libs SDL, glibc
#include <SDL.h>	//SDL version 2.0
#include <stdio.h>
#include <stdlib.h>
#include "renderer.h"
#include "vector.h"

#define SCREEN_WIDTH 640	//window height
#define SCREEN_HEIGHT 480	//window width

//function prototypes
int init(int w, int h, int argc, char *args[]);

//globals
SDL_Window* window = NULL;	//The window we'll be rendering to
SDL_Renderer *renderer;		//The renderer SDL will use to draw to the screen
int width, height;			//used if fullscreen
SDL_Texture *twist_t;		
struct pix_buff twist_pb;

int main (int argc, char *args[]) {
		
	//SDL Window setup
	if (init(SCREEN_WIDTH, SCREEN_HEIGHT, argc, args) == 1) {
		
		return 0;
	}
	
	SDL_GetWindowSize(window, &width, &height);
	
	int sleep = 0;
	int quit = 0;
	Uint32 next_game_tick = SDL_GetTicks();
	
	struct vector2d points[4];
	
	float angle = 0;
	
	//render loop
	while(quit == 0) {
	
		//check for new events every frame
		SDL_PumpEvents();

		const Uint8 *state = SDL_GetKeyboardState(NULL);
		
		if (state[SDL_SCANCODE_ESCAPE]) {
		
			quit = 1;
		}
		
		//draw background
		SDL_RenderClear(renderer);
		clear_pixels(&twist_pb, 0x000000ff);
		
		float a = angle;
		float b = a + M_PI / 2;
		float c = a +M_PI;
		float d = a +(M_PI / 2) * 3;
		
		int i;
		
		for (i = 0; i < twist_pb.height; i++) {
			
			float v = (float) i / twist_pb.height;
			v *= 2;
						
			points[0].x = 100 + (sin(a + v) * 60);
			points[0].y = 0 + i;
			points[1].x = 100 + (sin(b + v) * 60);
			points[1].y = 0 + i;
			points[2].x = 100 + (sin(c + v) * 60);
			points[2].y = 0 + i;
			points[3].x = 100 + (sin(d + v) * 60);
			points[3].y = 0 + i;

			if (points[0].x < points[1].x) {
				
				draw_line(&twist_pb, points[0].x, points[0].y, points[1].x, points[1].y, 0xff0000ff);
			}
			
			if (points[1].x < points[2].x) {
				
				draw_line(&twist_pb, points[1].x, points[1].y, points[2].x, points[2].y, 0x00ff00ff);
			}
		
			if (points[2].x < points[3].x) {
				
				draw_line(&twist_pb, points[2].x, points[2].y, points[3].x, points[3].y, 0x0000ffff);
			}
			
			if (points[3].x < points[0].x) {
				
				draw_line(&twist_pb, points[3].x, points[3].y, points[0].x, points[0].y, 0xffff00ff);
			}
		}
		
		angle += .02;
		
		
		SDL_UpdateTexture(twist_t, NULL, twist_pb.pixels, twist_pb.width * sizeof(uint32_t));
	
		SDL_Rect dest = {0, 0, twist_pb.width, height};
		
		SDL_RenderCopy(renderer, twist_t, NULL, &dest);
		
		//draw to the screen
		SDL_RenderPresent(renderer);
				
		//time it takes to render 1 frame in milliseconds
		next_game_tick += 1000 / 60;
		sleep = next_game_tick - SDL_GetTicks();
	
		if( sleep >= 0 ) {
            				
			SDL_Delay(sleep);
		}
	}

	//free renderer and all textures used with it
	SDL_DestroyRenderer(renderer);
	
	//Destroy window 
	SDL_DestroyWindow(window);

	//Quit SDL subsystems 
	SDL_Quit(); 
	 
	return 0;
}

int init(int width, int height, int argc, char *args[]) {

	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {

		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		
		return 1;
	} 
	
	int i;
	
	for (i = 0; i < argc; i++) {
		
		//Create window	
		if(strcmp(args[i], "-f")) {
			
			SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN, &window, &renderer);
		
		} else {
		
			SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_FULLSCREEN_DESKTOP, &window, &renderer);
		}
	}
	
	if (window == NULL) { 
		
		printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
		
		return 1;
	}
	
	//create and allocate custom pixel buffer
	create_pixel_buffer(&twist_pb, 256, 256);
	
	//set up screen texture
	twist_t = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, twist_pb.width, twist_pb.height);
	SDL_SetTextureBlendMode(twist_t, SDL_BLENDMODE_BLEND);

	return 0;
}
