//main.c 

//Using libs SDL, glibc
#include <SDL.h>	//SDL version 2.0
#include <stdio.h>
#include <stdlib.h>
#include "renderer.h"
#include "vector.h"

//used to set the width and height of the window if not run in fullscreen
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define BALLS 6
#define THRESHOLD 50

struct metaball {

	struct vector2d pos;	//position
	struct vector2d vel;	//velocity
	float radius;
};

//function prototypes
int init(int w, int h, int argc, char *args[]);
void update();

//globals
SDL_Window* window = NULL;	//The window we'll be rendering to
SDL_Renderer *renderer;		//The renderer SDL will use to draw to the screen
SDL_Texture *screen_t;		//The texture representing the screen	
SDL_Surface *surface;		//this is simply used to get the PixelFormat struct so MapRGBA function works correctly 
struct pix_buff screen;		//custom pixel buffer to draw to
int width, height;
struct metaball mballs[BALLS];

int main (int argc, char *args[]) {
	
	//SDL Window setup
	if (init(SCREEN_WIDTH, SCREEN_HEIGHT, argc, args) == 1) {
		
		return 0;
	}
	
	//loop through all the balls and populate them
	int i;
	
	for(i = 0; i < BALLS; i++) {
	
		mballs[i].pos.x = rand() % screen.width;
		mballs[i].pos.y = rand() % screen.height;
		mballs[i].vel.x = (float) rand() / RAND_MAX * 5;
		mballs[i].vel.y = (float) rand() / RAND_MAX * 5;
		mballs[i].radius = rand() % 20 + 10;
	}
	
	int bpp;
	uint32_t r,g,b,a;
	uint32_t pitch = (screen.width * screen.height) * sizeof(uint32_t);
	
	SDL_PixelFormatEnumToMasks(SDL_PIXELFORMAT_RGBA8888, &bpp, &r,&g,&b,&a);
	SDL_Surface *surface = SDL_CreateRGBSurfaceFrom(screen.pixels, screen.width, screen.height, bpp, pitch, r, g, b, a);
	
	int sleep = 0;
	int quit = 0;
	Uint32 next_game_tick = SDL_GetTicks();
	
	//render loop
	while(quit == 0) {
	
		//check for new events every frame
		SDL_PumpEvents();

		const Uint8 *state = SDL_GetKeyboardState(NULL);
		
		if (state[SDL_SCANCODE_ESCAPE]) {
		
			quit = 1;
		}
		
		//draw a bg colour to screen_t
		clear_pixels(&screen, 0x51515100);
		
		int x,y;
		
		for(x = 0; x < screen.width; x++) {
			
			for(y = 0; y < screen.height; y++) {
				
				float sum = 0;
				
				//calculate iso-surface
				for (i = 0; i < BALLS; i++) {
				
					float a = x - mballs[i].pos.x;
					float b = y - mballs[i].pos.y;
					float d = sqrt(a * a + b * b); //calculate pixel distance from metaball position
					sum += 100 * mballs[i].radius / d;
				}
				
				//threshold
				if (sum >= THRESHOLD) {
				
					//uint32_t c = SDL_MapRGBA(surface->format, sum, 0, 0, 0);
					draw_pixel(&screen, x, y, 0xffffff00);
				}	
			}
		}
		
		update();
		
		//draw buffer to the texture representing the screen
		SDL_UpdateTexture(screen_t, NULL, screen.pixels, screen.width * sizeof(uint32_t));
		
		//draw background
		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, screen_t, NULL, NULL);
		
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
	
	//free sdl software surface
	SDL_FreeSurface(surface);
	
	//free pixel buffer/s
	free_pixel_buffer(&screen);

	//Quit SDL subsystems 
	SDL_Quit(); 
	 
	return 0;
}

//update metaball positions based on their velocity vector and check bounds
void update() {

	int i;
	
	for (i = 0; i < BALLS; i++) {
		
		if (mballs[i].pos.x < 0 || mballs[i].pos.x > screen.width) {
			
			mballs[i].vel.x *= -1;
		}
	
		if (mballs[i].pos.y < 0 || mballs[i].pos.y > screen.height) {
			
			mballs[i].vel.y *= -1;
		}
		
		mballs[i].pos.x += mballs[i].vel.x;
		mballs[i].pos.y += mballs[i].vel.y;
	}
}

int init(int width, int height, int argc, char *args[]) {

	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {

		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		
		return 1;
	} 
	
	int i;
	
	//loop through command line arguments
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

	//window has been created, populate global window width and height vars
	SDL_GetWindowSize(window, &width, &height);
	
	printf("window width = %d, height = %d\n", width, height);
	
	//create and allocate custom pixel buffer
	create_pixel_buffer(&screen, 640, 480);
	
	//set up screen texture
	screen_t = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, screen.width, screen.height);
	
	if (screen.pixels == NULL) { 
		
		printf ("Screen pixel buffer is NULL!");
		
		return 1;
	}
	
	if (screen_t == NULL) { 
		
		printf ("Texture could not be created! SDL_Error: %s\n", SDL_GetError());
		
		return 1;
	}
	
	return 0;
}
