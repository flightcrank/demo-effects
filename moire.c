#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "SDL/SDL.h"

#define SCREEN_WIDTH  512
#define SCREEN_HEIGHT 512
#define TEX_WIDTH 1024
#define TEX_HEIGHT 1024

struct vector {

	//position
	int x;
	int y;
	
	//delta/direction
	int dx;
	int dy;
};

//global variables, for convenience.
static SDL_Surface *screen;
static SDL_Surface *texture;

double distance[TEX_WIDTH][TEX_HEIGHT];
struct vector moire[2];

void init_moire() {

	int i;

	for(i = 0; i < 2; i++) {
		
		moire[i].x = rand() % screen->w / 2;
		moire[i].y = rand() % screen->h / 2;
		moire[i].dx = rand() % 5 + 2;
		moire[i].dy = rand() % 5 + 2;
	}
}

//populate look up tables
void pop_lut() {

	int i,j;
	
	//distance
	for (i = 0; i < TEX_HEIGHT; i++ ) {
		
		for (j = 0; j < TEX_WIDTH; j++ ) {
			
			int x = i - TEX_WIDTH / 2;
			int y = TEX_HEIGHT / 2 - j;

			double d =  sqrt(pow(x,2) + pow(y,2));
	
			if ((int)d % 15 < 7) {
			
				distance[i][j] = 255;
			}
		}
	}
}

//print look up table to stdout
void print_lut() {

	int i,j;
	
	for (i = 0; i < TEX_HEIGHT; i++ ) {
		
		for (j = 0; j < TEX_WIDTH; j++ ) {
		
			printf("%6.2f ", distance[i][j]);
			
		}
			printf("\n");
	}

}

//plot a pixel
void draw_pixel(SDL_Surface *surface, int x, int y, Uint32 colour) {

	//sometimes necessary to lock surface it before it can be modified
	SDL_LockSurface(surface);

	/* Get a pointer to the video surface memory. */
	Uint32 *raw_pixels;
	raw_pixels = (Uint32 *) surface->pixels;

	//calculate the location in memory where the disired pixel starts at
	//first value is the y location, second is the size of the screen width in memory, last value is the x location
	int pix_offset = y * (surface->pitch / 4) + x;

	//set the pixel to the desired colour
	raw_pixels[pix_offset] = colour;

	//finished drawing, so unlock the surface
	SDL_UnlockSurface(surface);
}

//get a pixel
Uint32 get_pixel(SDL_Surface *surface, int x, int y) {

	Uint32 pix;

	//sometimes necessary to lock surface it before it can be modified
	SDL_LockSurface(surface);

	/* Get a pointer to the video surface memory. */
	Uint32 *raw_pixels;
	raw_pixels = (Uint32 *) surface->pixels;

	//calculate the location in memory where the disired pixel starts at
	//first value is the y location, second is the size of the screen width in memory, last value is the x location
	int pix_offset = y * (surface->pitch / 4) + x;

	//set the pixel to the desired colour
	pix = raw_pixels[pix_offset];

	//finished drawing, so unlock the surface
	SDL_UnlockSurface(surface);

	return pix;
}

//Draw the background
void draw_background () {

	SDL_Rect src;

	src.x = 0;
	src.y = 0;
	src.w = screen->w;
	src.h = screen->h;
		
	Uint32 colour = SDL_MapRGBA(screen->format,0,0,0,255);
	SDL_FillRect(screen, &src, colour);
}

//draw grad texture
void draw_grad() {

	int width = TEX_WIDTH;
	int height = TEX_HEIGHT;
	int rmask = 0x000000ff;
	int gmask = 0x0000ff00;
	int bmask = 0x00ff0000;
	int amask = 0xff000000;

	texture = SDL_CreateRGBSurface(SDL_SWSURFACE, width, height, 32, rmask, gmask, bmask, amask);
	
	if (texture == NULL) {
        	
		fprintf(stderr, "CreateRGBSurface failed: %s\n", SDL_GetError());
	}

	//i = y co-ord, j = x co-ord
	int i,j;

	for (i = 0; i < TEX_HEIGHT; i++) {
		
		for (j = 0; j < TEX_WIDTH; j++) {
				
			Uint32 colour;
			int v = (int) distance[j][i];

			if (v == 255) {

				colour = SDL_MapRGBA(texture->format, v, v, v, v);
			
			} else {
			
				colour = SDL_MapRGBA(texture->format, 0, 0, 0, 0);
			}

			draw_pixel(texture, j, i, colour);
		}
	}
}

//draw texture to screen
static void draw_moire(struct vector m) {
	
	SDL_Rect src, dest;

	src.x = 0;
	src.y = 0;
	src.w = texture->w;
	src.h = texture->h;
	
	dest.x = m.x - screen->w / 2;
	dest.y = m.y - screen->h / 2;
	dest.w = texture->w;
	dest.h = texture->h;
		
	SDL_BlitSurface(texture, &src, screen, &dest);
}

//movie moire around screen
void move_moire() {
	
	int i;

	//check collisions	
	for (i = 0; i < 2; i++) {
	
		if (moire[i].x >= screen->w / 2) {
			
			moire[i].dx = -moire[i].dx;
		}
	
		if (moire[i].x <= -screen->w / 2) {
			
			moire[i].dx = -moire[i].dx;
		}
		
		if (moire[i].y >= screen->h / 2) {
			
			moire[i].dy = -moire[i].dy;
		}
		
		if (moire[i].y <= -screen->h / 2) {
			
			moire[i].dy = -moire[i].dy;
		}
		
		//move moire along its direction vector.
		moire[i].x += moire[i].dx;
		moire[i].y += moire[i].dy;
	}
}

//Main program
int main() {
	
	/* Initialize SDL's video system and check for errors */
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {

		printf("Unable to initialize SDL: %s\n", SDL_GetError());
		return 1;
	}
	
	/* Make sure SDL_Quit gets called when the program exits! */
	atexit(SDL_Quit);
	
	/*set window title*/
	SDL_WM_SetCaption("meta", "P");
	
	/* Attempt to set a 800x600 8 bit color video mode */
	screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, 32, SDL_DOUBLEBUF );
	
	if (screen == NULL) {
		
		printf("Unable to set video mode: %s\n", SDL_GetError());
		return 1;
	}

	SDL_SetAlpha(screen, SDL_SRCALPHA, SDL_ALPHA_OPAQUE);

	Uint32 next_game_tick = SDL_GetTicks();
	int sleep = 0;
	int quit = 0;
	SDL_Event event;
	
	init_moire();
	pop_lut();
	//print_lut();
	draw_grad();

	/* Animate */
	while (quit == 0) {
		
		/* Grab a snapshot of the keyboard. */
		
		while (SDL_PollEvent(&event)) {

			switch(event.type) {
				
				case SDL_KEYDOWN:
					
					switch( event.key.keysym.sym ) {
					
						//exit out of game loop if escape is pressed
						case SDLK_ESCAPE:
							
							quit = 1;
						break;
						
						default:
						break;
					}
				break;
			}
		}
	
		draw_background();
		draw_moire(moire[0]);
		draw_moire(moire[1]);
		move_moire();

		/* Ask SDL to update the entire screen. */
		SDL_Flip(screen);

		next_game_tick += 1000 / 30;
		sleep = next_game_tick - SDL_GetTicks();
	
		if( sleep >= 0 ) {

            		SDL_Delay(sleep);
        	}
	}
	
	return 0;
}
