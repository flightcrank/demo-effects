//gcc -Wall -g -lSDL -lm shadebob.c -o app

#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include "SDL/SDL.h"

#define SCREEN_WIDTH 640 
#define SCREEN_HEIGHT 480
#define BLOB 20

//global variables, for convenience.
static SDL_Surface *screen;
int fire_array[SCREEN_WIDTH][SCREEN_HEIGHT];
SDL_Color colours[256];

int blobx;
int bloby;
int center_x = SCREEN_WIDTH / 2;
int center_y = SCREEN_HEIGHT / 2;
double current_angle = 0;
double radius = 160;

//init palette
void init_palette() {
	
	int i;
		
	for(i = 0; i < 64; i++) {
		
		colours[i].r = 4 * i;
		colours[i].g = 0;
		colours[i].b = 0;
	}
	
	for(i = 0; i < 64; i++) {
		
		colours[64 + i].r = 255;
		colours[64 + i].g = 4 * i;
		colours[64 + i].b = 0;
		
	}

	for(i = 0; i < 128; i++) {
		
		colours[128 + i].r = 255;
		colours[128 + i].g = 255;
		colours[128 + i].b = 2 * i;
	}

       	// Set palette 
       	SDL_SetPalette(screen, SDL_LOGPAL|SDL_PHYSPAL, colours, 0, 256);
}

//init fire array
void init_fire_array() {
	
	int x, y;

	for (y = 0; y < SCREEN_HEIGHT; y++) {
		
		for (x = 0; x < SCREEN_WIDTH; x++) {
			
			fire_array[x][y] = 0;
		}
	}
}

//plot a pixel
void draw_pixel(int x, int y, Uint32 colour) {

	//sometimes necessary to lock surface it before it can be modified
	SDL_LockSurface(screen);

	/* Get a pointer to the video surface memory. */
	Uint8 *raw_pixels;
	raw_pixels = (Uint8 *) screen->pixels;

	//calculate the location in memory where the disired pixel starts at
	//first value is the y location, second is the size of the screen width in memory, last value is the x location
	int pix_offset = y * screen->pitch + x;

	//set the pixel to the desired colour
	raw_pixels[pix_offset] = colour;

	//finished drawing, so unlock the surface
	SDL_UnlockSurface(screen);
}

//Draw a line. ALL lines will be drawn from the left to the right
void draw_line(int x1, int y1, int x2, int y2, Uint32 colour) {
	
	//if points are given from left of the screen to right
	if (x2 < x1) {
		
		int tempx = x2;
		int tempy = y2;

		x2 = x1;
		y2 = y1;
		x1 = tempx;  
		y1 = tempy;  
	}
	

	int x = x1;
	int y = y1;
	int dx = x2 - x1; //change in x
	int dy = abs(y2 - y1); //change in y. Absolute values are needed due to cases where point 2 is above point 1 
	float y_exact = y1; //exact y coordinate
	float x_exact = x1; //exact x coordinate
	float slope = (float)dy / (float)dx; //slope of the line
	float error = slope; //distance away from the ideal x or y coordinate

	int i;
	
	//x axis is longer than the y axis
	if (dx > dy) {
		
		//line travels from top to bottom
		if (y2 > y1) {
			
			//printf("\ndx = %d dy = %d\n", dx, dy);
			//printf("slope = %f\n", slope);
		
			for (i = 0; i <= dx; i++) {
				
				draw_pixel(x + i, y, colour);
				
				//printf("\nPlotted line x = %d, y = %d\n", x + i , y);
				//printf("Ideal line x = %d, y = %f\n", x + i , y_exact);
				
				//ideal line y co-ord
				y_exact += slope;

				if (error >= 0.5) {
				
					y++;
				}
				
				error = y_exact - y; //plotted y coordinate distance from the ideal y coordinate
				
				//printf("Error distace = %f\n", error);
				
				error += slope;
				
				//printf("Error from ideal line = %f\n", error);
			}
			
			//printf("\n");
		
		//line travels from bottom to top
		} else {
			
			for (i = 0; i <= dx; i++) {
				
				draw_pixel(x + i, y, colour);
				
				//ideal line y co-ord
				y_exact -= slope;

				if (error >= 0.5) {
				
					y--;
				}
				
				// the calculation is the reverse of to to bottom case
				error = y - y_exact; //plotted y coordinate distance from the ideal y coordinate
				error += slope;
			}
		}

	//y axix is longer than the x axis
	} else {
	
		//line travels form top to bottom
		if (y2 > y1) {
		
			for (i = 0; i <= dy; i++) {
			
				draw_pixel(x, y + i, colour);
				
				//ideal line x co-ord
				x_exact += 1 / slope;

				if (error >= 0.5) {
				
					x++;
				}
				
				error = x_exact - x; //plotted x coordinate distance from the ideal x coordinate
				error += 1 / slope;
			}
		
		//line travels form bottom to top 
		} else {
			
			for (i = 0; i <= dy; i++) {
			
				draw_pixel(x, y - i, colour); //increment is subtracted from y
				
				//ideal line x co-ord
				x_exact += 1 / slope;

				if (error >= 0.5) {
				
					x++;
				}
				
				error = x_exact - x; //plotted x coordinate distance from the ideal x coordinate
				error += 1 / slope;
			}
		}
	}
}

//Draw the background
void draw_background () {

	SDL_Rect src;

	src.x = 0;
	src.y = 0;
	src.w = screen->w;
	src.h = screen->h;
	
	SDL_FillRect(screen,&src,0);
}

//draw_blob
void draw_blob(int x, int y) {
	
	int xa,ya;
	
	for (ya = 0; ya < BLOB; ya++) {
		
		for (xa = 0; xa < BLOB; xa++) {
			
			fire_array[x + xa][y + ya] += 10;
		}
	}
}

//move blob
void move_blob() {
	
	double current_angle_radians = current_angle * 3.14 / 180;
	double x = radius * cos(current_angle_radians);
	double y = radius * sin(current_angle_radians); 
	
	x += center_x;
	y += center_y;

	blobx = x;
	bloby = y;

	current_angle++;

	if (current_angle == 360) {

		current_angle = 0;
	}
}

//draw_fire
void draw_fire() {
	
	int x, y;

	for (y = 0; y < SCREEN_HEIGHT; y++) {
		
		for (x = 0; x < SCREEN_WIDTH; x++) {
                
			int current_temp = fire_array[x][y];
			
			draw_pixel(x,y, current_temp);
		}
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
	SDL_WM_SetCaption("Skeleton", "P");
	
	/* Attempt to set a 800x600 8 bit color video mode */
	screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, 8, SDL_DOUBLEBUF);
	
	if (screen == NULL) {
		
		printf("Unable to set video mode: %s\n", SDL_GetError());
		return 1;
	}

	Uint32 next_game_tick = SDL_GetTicks();
	int sleep = 0;
	//Uint8 *keystate = 0;
	int quit = 0;
	SDL_Event event;

	init_fire_array();
	init_palette();

	/* Animate */
	while (quit == 0) {
		
		/* Grab a snapshot of the keyboard. */
		//keystate = SDL_GetKeyState(NULL);
		
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
	
		//draw_background();
		draw_blob(blobx,bloby);
		move_blob();
		draw_fire();

		/* Ask SDL to update the entire screen. */
		SDL_Flip(screen);

		next_game_tick += 1000 / 60;
		sleep = next_game_tick - SDL_GetTicks();
	
		if( sleep >= 0 ) {

            		SDL_Delay(sleep);
        	}
	}
	
	return 0;
}
