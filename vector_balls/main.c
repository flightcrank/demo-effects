
//Using SDL and standard IO
#include <SDL.h>

#include <stdio.h>
#include <stdint.h>
#include "renderer.h"
#include "balls.h"

//Screen dimension constants
#define SCREEN_WIDTH 1080
#define SCREEN_HEIGHT 720
#define VECTOR_BALL_WIDTH 16
#define VECTOR_BALL_HEIGHT 16
#define SIZE 8

int init(int width, int height);

SDL_Window* window = NULL;		//The window we'll be rendering to
SDL_Renderer *renderer;			//The renderer SDL will use to draw to the screen
SDL_Texture *vector_ball;		//The texture representing the screen	
SDL_Texture *title;			//The texture representing the title message	
struct pixel_buffer vb;			//the structure that holds the pixel buffer for the snow texture	
struct vector3d balls[SIZE];

int main (int argc, char* args[]) {
	
	//SDL Window setup
	if (init(SCREEN_WIDTH, SCREEN_HEIGHT) == 1) {
		
		return 0;
	}
	
	//initilise vector_balls
	init_vector_balls(balls);

	//create circle image in pixel buffer and copy to a texture 
	clear_pixels(&vb, 0x00000000);
	draw_circle(&vb, vb.width / 2, vb.height / 2, 8, 0xffffffff);
	SDL_UpdateTexture(vector_ball, NULL, vb.pixels, vb.width * sizeof (uint32_t));

	int sleep = 0;
	int quit = 0;
	Uint32 next_game_tick = SDL_GetTicks();
	Uint32 current_tick = next_game_tick;

	float d = 1;

	//render loop
	while(quit == 0) {
	
		//check for new events every frame
		SDL_PumpEvents();

		const Uint8 *state = SDL_GetKeyboardState(NULL);
		
		if (state[SDL_SCANCODE_ESCAPE]) {
		
			quit = 1;
		}

		//draw to the screen
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);
		draw_vector_balls(renderer, vector_ball, balls, SIZE, SCREEN_WIDTH, SCREEN_HEIGHT);
		//SDL_RenderCopy(renderer, vector_ball, NULL, NULL);	//draw screen pixel_buffer
		SDL_RenderPresent(renderer);				//show final frame on the screen

		update_balls(balls, SIZE, d);

		//d+=.1;
		
		//time it takes to render 1 frame in milliseconds
		next_game_tick += 1000 / 60;
		current_tick = SDL_GetTicks();
		sleep = next_game_tick - current_tick;
		
		if( sleep >= 0 ) {
            				
			SDL_Delay(sleep);
		}
	}

	//free the vector_ball buffer
	free(vb.pixels);
	
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

	//set up vector ball pixel buffer
	create_renderer(&vb, VECTOR_BALL_WIDTH, VECTOR_BALL_HEIGHT);
	
	//set up vector_ball texture
	vector_ball = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STATIC, vb.width, vb.height);
	
	if (window == NULL) { 
		
		printf ("Window could not be created! SDL_Error: %s\n", SDL_GetError());
		
		return 1;
	}

	//SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");  // make the scaled rendering look smoother.
	//SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);

	if (vector_ball == NULL) { 
		
		printf ("Texture could not be created! SDL_Error: %s\n", SDL_GetError());
		
		return 1;
	}
	
	if (vb.pixels == NULL) {
	
		printf ("Error allocating pixel buffer");
		
		return 1;
	}

	return 0;
}

