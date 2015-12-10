
//Using SDL and standard IO
#include <SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include "renderer.h"
#include "stars.h"

int init(int width, int height);

SDL_Window* window = NULL;	//The window we'll be rendering to
SDL_Renderer *renderer;		//The renderer SDL will use to draw to the screen
SDL_Texture *screen;		//The texture representing the screen	
Uint32* pixels = NULL;		//The pixel buffer to draw to
struct star stars[NUM_STARS];	//stars to show on screen
    
int main (int argc, char* args[]) {

	//SDL Window setup
	if (init(SCREEN_WIDTH, SCREEN_HEIGHT) == 1) {
		
		return 0;
	}
	
	init_stars(stars);

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

		//draw to the pixel buffer	
		clear_pixels(pixels, 0x00000000);
		draw_stars(pixels, stars);
		SDL_UpdateTexture(screen, NULL, pixels, SCREEN_WIDTH * sizeof (Uint32));

		update_stars(stars);

		//draw to the screen
		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, screen, NULL, NULL);
		SDL_RenderPresent(renderer);
				
		//time it takes to render 1 frame in milliseconds
		next_game_tick += 1000 / 60;
		sleep = next_game_tick - SDL_GetTicks();
	
		if( sleep >= 0 ) {
            				
			SDL_Delay(sleep);
		}
	}

	//free the screen buffer
	free(pixels);
	
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
	
	//allocate pixel buffer
	pixels = (Uint32*) malloc((SCREEN_WIDTH * SCREEN_HEIGHT) * sizeof(Uint32));


	if (window == NULL) { 
		
		printf ("Window could not be created! SDL_Error: %s\n", SDL_GetError());
		
		return 1;
	}

	if (screen == NULL) { 
		
		printf ("Texture could not be created! SDL_Error: %s\n", SDL_GetError());
		
		return 1;
	}
	
	if (pixels == NULL) {
	
		printf ("Error allocating pixel buffer");
		
		return 1;
	}

	return 0;
}


