
//Using SDL and standard IO
#include <SDL.h>

#include <stdio.h>
#include <stdint.h>
#include "renderer.h"
#include "stars.h"

//Screen dimension constants
#define SCREEN_WIDTH 1080
#define SCREEN_HEIGHT 720
#define VECTOR_BALL_WIDTH 16
#define VECTOR_BALL_HEIGHT 16

int init(int width, int height);

SDL_Window* window = NULL;		//The window we'll be rendering to
SDL_Renderer *renderer;			//The renderer SDL will use to draw to the screen
SDL_Texture *screen;			//The texture representing the screen	
SDL_Texture *vector_ball;		//The texture representing the screen	
SDL_Texture *title;			//The texture representing the title message	
SDL_Surface* source;			//A source image
struct pixel_buffer pb;			//The structure that holds the pixel buffer for the screen textue
struct pixel_buffer vb;			//the structure that holds the pixel buffer for the snow texture	
struct star snow[NUM_STARS];		//array of snowflakes
    
int main (int argc, char* args[]) {
	
	//SDL Window setup
	if (init(SCREEN_WIDTH, SCREEN_HEIGHT) == 1) {
		
		return 0;
	}
	
	init_stars(snow, SCREEN_WIDTH, SCREEN_HEIGHT);

	//create circle image in pixel buffer and copy to a texture 
	draw_circle(&vb, vb.width / 2, vb.height / 2, 8, 0xffffffff);
	SDL_UpdateTexture(vector_ball, NULL, vb.pixels, vb.width * sizeof (uint32_t));

	int sleep = 0;
	int quit = 0;
	struct vector3d vel = {0, 0, -.005};		//snow movment vector
	Uint32 next_game_tick = SDL_GetTicks();
	Uint32 current_tick = next_game_tick;

	//render loop
	while(quit == 0) {
	
		//check for new events every frame
		SDL_PumpEvents();

		const Uint8 *state = SDL_GetKeyboardState(NULL);
		
		if (state[SDL_SCANCODE_ESCAPE]) {
		
			quit = 1;
		}

		clear_pixels(&pb, 0x00000000);
		SDL_UpdateTexture(screen, NULL, pb.pixels, pb.width * sizeof (uint32_t));

		//draw to the screen
		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, screen, NULL, NULL);	//draw screen pixel_buffer

		draw_stars(renderer, vector_ball, snow);	//draw all snow flakes with vector ball texture
		SDL_RenderPresent(renderer);			//show fintal frame on the screen

		update_stars(snow, SCREEN_WIDTH, SCREEN_HEIGHT, &vel);
		
		//time it takes to render 1 frame in milliseconds
		next_game_tick += 1000 / 60;
		current_tick = SDL_GetTicks();
		sleep = next_game_tick - current_tick;
		
		if( sleep >= 0 ) {
            				
			SDL_Delay(sleep);
		}
	}

	//free the screen buffer
	free(pb.pixels);
	
	//free the vector_ball buffer
	free(vb.pixels);
	
	//free the source image
	SDL_FreeSurface(source);
	
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

	//set up screen pixel buffer
	create_renderer(&pb, SCREEN_WIDTH, SCREEN_HEIGHT);
	
	//set up vector ball pixel buffer
	create_renderer(&vb, VECTOR_BALL_WIDTH, VECTOR_BALL_HEIGHT);
	
	//set up screen texture
	screen = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, pb.width, pb.height);
	
	//set up vector_ball texture
	vector_ball = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STATIC, vb.width, vb.height);
	
	if (window == NULL) { 
		
		printf ("Window could not be created! SDL_Error: %s\n", SDL_GetError());
		
		return 1;
	}

	//SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");  // make the scaled rendering look smoother.
	//SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);

	if (screen == NULL) { 
		
		printf ("Texture could not be created! SDL_Error: %s\n", SDL_GetError());
		
		return 1;
	}
	
	if (pb.pixels == NULL) {
	
		printf ("Error allocating pixel buffer");
		
		return 1;
	}
	
	if (vb.pixels == NULL) {
	
		printf ("Error allocating pixel buffer");
		
		return 1;
	}

	return 0;
}

