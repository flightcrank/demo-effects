
//Using SDL and standard IO
#include <SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include "vector.h"

//Screen dimension constants
#define SCREEN_WIDTH 1080
#define SCREEN_HEIGHT 720

#define ROTO_WIDTH 256
#define ROTO_HEIGHT 256

int init(int width, int height);
void draw_pixel(int x, int y, Uint32);
Uint32 get_pixel(int x, int y);

SDL_Window* window = NULL;	//The window we'll be rendering to
SDL_Renderer* renderer;		//The renderer SDL will use to draw to the screen
SDL_Texture* screen;		//The texture representing the screen	
Uint32* pixels = NULL;		//The pixel buffer to draw to
SDL_Texture* roto;		//The texture representing the rotozoomed image	
SDL_Surface* source;		//A source image
    
int main (int argc, char* args[]) {

	//SDL Window setup
	if (init(SCREEN_WIDTH, SCREEN_HEIGHT) == 1) {
		
		return 0;
	}
	
	int sleep = 0;
	int quit = 0;
	Uint32 next_game_tick = SDL_GetTicks();
	
	int i = 0;
	int d = 0;
	float sin_t[360];
	
	for (i = 0; i < 360; i++) {
		
		sin_t[i] =  sin(i * M_PI / 180);
	}
	
	SDL_Rect src, dest;

	src.x = 0;
	src.y = 0;
	src.w = ROTO_WIDTH;
	src.h = ROTO_HEIGHT;
	
	dest.x = SCREEN_WIDTH / 2 - SCREEN_WIDTH / 2;
	dest.y = SCREEN_HEIGHT / 2 - SCREEN_WIDTH / 2;
	dest.w = SCREEN_WIDTH;
	dest.h = SCREEN_WIDTH;


	//render loop
	while(quit == 0) {
	
		//check for new events every frame
		SDL_PumpEvents();

		const Uint8 *state = SDL_GetKeyboardState(NULL);
		
		if (state[SDL_SCANCODE_ESCAPE]) {
		
			quit = 1;
		}
		
		struct vector2d obj_translate = {-ROTO_WIDTH / 2, -ROTO_HEIGHT / 2};
		struct vector2d screen_translate = {ROTO_WIDTH / 2, ROTO_HEIGHT / 2};
		
		d += 1;
		
		int x,y;

		for (x = 0; x < ROTO_WIDTH; x++) {
			
			for (y = 0; y < ROTO_HEIGHT; y++) {
				
				struct vector2d r = {x, y};
				
				add_vector(&r, &obj_translate);
				int index = (d % 360);
				rotate_vector(&r, sin_t[index] * 360);
				add_vector(&r, &screen_translate);
				
				if (r.y < 0) {
					
					r.y = fabs(r.y);
				}
				
				if (r.x < 0) {
					
					r.x = fabs(r.x);
				}

				float u = (float) r.x / ROTO_WIDTH  * fabs(sin_t[index]) * 5;
				float v = (float) r.y / ROTO_HEIGHT * fabs(sin_t[index]) * 5;
				u = (int) (u * source->w) % source->w;
				v = (int) (v * source->h) % source->h;
				
				Uint32 c = get_pixel(u, v);
				draw_pixel(x, y, c);
			}
		}

		//SDL_UpdateTexture(screen, NULL, pixels, SCREEN_WIDTH * sizeof (Uint32));
		
		SDL_UpdateTexture(roto, NULL, pixels, ROTO_WIDTH * sizeof (Uint32));

		//draw to the screen
		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, roto, &src, &dest);
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
	
	//set up screen texture
	screen = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, SCREEN_WIDTH, SCREEN_HEIGHT);
	
	//set up screen texture
	roto = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, ROTO_WIDTH, ROTO_HEIGHT);
	
	//allocate pixel buffer
	pixels = (Uint32*) malloc((ROTO_WIDTH * ROTO_HEIGHT) * sizeof(Uint32));
	
	//load image into surface
	SDL_Surface* tmp = SDL_LoadBMP("smile.bmp");
	
	if (tmp == NULL) { 
		
		printf ("coild not load image file SDL_Error: %s\n", SDL_GetError());
		
		return 1;
	}

	source = SDL_ConvertSurfaceFormat(tmp, SDL_PIXELFORMAT_RGBA8888, 0);
	SDL_FreeSurface(tmp);

	if (window == NULL) { 
		
		printf ("Window could not be created! SDL_Error: %s\n", SDL_GetError());
		
		return 1;
	}

	if (roto == NULL) { 
		
		printf ("Texture could not be created! SDL_Error: %s\n", SDL_GetError());
		
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

	if (source == NULL) {
	
		printf ("Error allocating surface %s\n", SDL_GetError());
		
		return 1;
	}

	return 0;
}

void draw_pixel(int x, int y, Uint32 c) {
	
	Uint32 position = y * ROTO_WIDTH + x;
	pixels[position] = c;
}

Uint32 get_pixel(int x, int y) {
	
	Uint32* p = (Uint32 *) source->pixels;
	Uint32 position = y * source->w + x;

	return p[position];
}

