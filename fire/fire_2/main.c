
//Using SDL and standard IO
#include <SDL.h>

#include <stdio.h>
#include <stdint.h>
#include "renderer.h"

//Screen dimension constants
#define SCREEN_WIDTH 1080
#define SCREEN_HEIGHT 720
#define VECTOR_BALL_WIDTH 16
#define VECTOR_BALL_HEIGHT 16
#define SIZE 8

int init(int width, int height);

SDL_Window* window = NULL;		//The window we'll be rendering to
SDL_Renderer *renderer;			//The renderer SDL will use to draw to the screen
SDL_Texture *colour_map;		//The texture representing the color pallet	
SDL_Texture *cool_map;			//The texture representing the cool	
SDL_Texture *screen;			//The texture representing the screen
struct pixel_buffer pb;			//the structure that holds the pixel buffer for screen	
struct pixel_buffer fire;		//the structure that holds the pixel buffer for fire	
struct pixel_buffer cm;			//the structure that holds the pixel buffer for color pallet	
struct pixel_buffer cool;		//the structure that holds the pixel buffer for cool map	

void smooth_buffer(struct pixel_buffer* pix) {

	int x,y;
	
	for (x = 0; x < pix->width; x++) {
		
		for (y = 0; y < pix->height; y++) {
			
			int left = (x - 1) % pix->width;
			int right = (x + 1) % pix->width;
			int up 	= (y - 1) % pix->height;
			int down = (y + 1) % pix->height;

			if (left < 0) {
				
				left = left + pix->width;
			}
			
			if (up < 0) {
				
				up = up + pix->height;
			}

			Uint32 val = (get_pixel(pix, x, up) + get_pixel(pix, x, down) + get_pixel(pix, left, y) + get_pixel(pix, right, y)) / 4;
			
			draw_pixel(pix, x, y, val);
		}
	}
}

int main (int argc, char* args[]) {
	
	//SDL Window setup
	if (init(SCREEN_WIDTH, SCREEN_HEIGHT) == 1) {
		
		return 0;
	}
	
	int sleep = 0;
	int quit = 0;
	Uint32 next_game_tick = SDL_GetTicks();
	Uint32 current_tick = next_game_tick;
	
	clear_pixels(&cm, 0);
	clear_pixels(&pb, 0);
	clear_pixels(&fire, 0);
	clear_pixels(&cool, 0);

	Uint8 r = 255, g = 255, b = 255;
	int i;

	//create fire colour pallet and store colour values in a pixel buffer
	for (i = 0; i < cm.width; i++) {
		
		Uint32 colour = r << 24 | g << 16 | b << 8;
		int p1 = 85;
		int p2 = 170;
		
		if (i <= p1) {
			
			float inverse = p1 - i;
			float interp = inverse / p1;
			b = interp * 255;
			
			colour = r << 24 | g << 16 | b << 8;
		
		} else if (i > p1 && i <= p2) {
			
			float inverse = p2 - i;
			float interp = inverse / p1;
			g = interp * 255;
			
			colour = r << 24 | g << 16 | b << 8;
		
		} else {
			
			float inverse = cm.width - i;
			float interp = inverse / p1;
			r = interp * 255;
			
			colour = r << 24 | g << 16 | b << 8;
		}
		
		draw_pixel(&cm, i, 0, colour);
	}
	
	SDL_UpdateTexture(colour_map, NULL, cm.pixels, cm.width * sizeof (Uint32));
	
	for (i = 0; i < 10000; i++) {
	
		int x = rand() % cool.width;
		int y = rand() % cool.height;

		draw_pixel(&cool, x, y, 255);
	}
	
	smooth_buffer(&cool);
	smooth_buffer(&cool);
	smooth_buffer(&cool);
	smooth_buffer(&cool);
	smooth_buffer(&cool);
	smooth_buffer(&cool);
	smooth_buffer(&cool);
	smooth_buffer(&cool);
	smooth_buffer(&cool);
	smooth_buffer(&cool);
	smooth_buffer(&cool);
	smooth_buffer(&cool);
	smooth_buffer(&cool);
	smooth_buffer(&cool);
	smooth_buffer(&cool);
	smooth_buffer(&cool);
	smooth_buffer(&cool);
	smooth_buffer(&cool);

	int offset = 0;

	//render loop
	while(quit == 0) {
	
		//check for new events every frame
		SDL_PumpEvents();

		const Uint8 *state = SDL_GetKeyboardState(NULL);
		
		if (state[SDL_SCANCODE_ESCAPE]) {
		
			quit = 1;
		}
		
		//init the fire array
		for (i = 0; i < 550; i++) {
			
			//random y and x values
			int x = rand() % fire.width;
			int y = rand() % 2 + fire.height - 2;

			draw_pixel(&fire, x, y, 255);
		}

		draw_circle(&fire, 100, 100, 5, 255);
		
		//heat spread smoothing
		smooth_buffer(&fire);
		
		int x,y;

		//cooling
		for (x = 0; x < fire.width; x++) {
			
			for (y = 0; y < fire.height; y++) {
				
				Uint32 val = get_pixel(&fire, x, y);
				Uint32 cool_val = get_pixel(&cool, x, (y + offset) % cool.height);

				if (cool_val == 0) {
					
					cool_val = rand() % 3;
				}

				int res = val - cool_val;

				if (res > 0) {
					
					draw_pixel(&fire, x, y - 1, res);
				}
				
			}

		}
		
		offset += 3;

		//draw to the screen buffer	
		for (x = 0; x < pb.width; x++) {
			
			for (y = 0; y < pb.height; y++) {
				
				Uint32 p = get_pixel(&fire, x, y);
					
				Uint32 colour = get_pixel(&cm, 255 - p, 0);

				draw_pixel(&pb, x, y, colour);
			}
		}
		
		SDL_UpdateTexture(screen, NULL, pb.pixels, pb.width * sizeof (Uint32));

		//draw to the screen
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, screen, NULL, NULL);
		SDL_RenderPresent(renderer);				//show final frame on the screen

		//time it takes to render 1 frame in milliseconds
		next_game_tick += 1000 / 60;
		current_tick = SDL_GetTicks();
		sleep = next_game_tick - current_tick;
		
		if( sleep >= 0 ) {
            				
			SDL_Delay(sleep);
		}
	}
	
	free(pb.pixels);
	free(cm.pixels);
	free(fire.pixels);

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
	screen = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
	
	//set up 
	colour_map = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, 256, 1);
	
	//set up 
	cool_map = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
	
	//set up colour_map ball pixel buffer
	create_renderer(&cm, 256, 1);
	
	//set up screen pixel buffer
	create_renderer(&pb, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
	
	//set up fire pixel buffer
	create_renderer(&fire, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
	
	//set up cool map pixel buffer
	create_renderer(&cool, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
	
	
	if (window == NULL) { 
		
		printf ("Window could not be created! SDL_Error: %s\n", SDL_GetError());
		
		return 1;
	}
	
	if (screen == NULL) { 
		
		printf ("texture could not be created! SDL_Error: %s\n", SDL_GetError());
		
		return 1;
	}
	
	if (colour_map == NULL) { 
		
		printf ("texture could not be created! SDL_Error: %s\n", SDL_GetError());
		
		return 1;
	}
	
	if (cool_map == NULL) { 
		
		printf ("texture could not be created! SDL_Error: %s\n", SDL_GetError());
		
		return 1;
	}

	if (cm.pixels == NULL) {
		
		printf ("pixel buffer could not be created!");

		return 1;
	}
	
	if (pb.pixels == NULL) {
		
		printf ("pixel buffer could not be created!");
		
		return 1;
	}
	
	if (fire.pixels == NULL) {
		
		printf ("pixel buffer could not be created!");
		
		return 1;
	}
	
	if (cool.pixels == NULL) {
		
		printf ("pixel buffer could not be created!");
		
		return 1;
	}

	//SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");  // make the scaled rendering look smoother.
	//SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
	
	return 0;
}

