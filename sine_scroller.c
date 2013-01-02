//known bugs
//some times the bullet wont go past through a base if it hits on the far left edge. cause unknowen
//its the same bug that i found in my invaders drawing code. using the objects own hitbox as the SDL_Rect
//as the destination rectangle to blit to. using a seperate local rect in the drawing code solves it.
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "SDL/SDL.h"

#define SCREEN_WIDTH 800 
#define SCREEN_HEIGHT 600

//global variables, for convenience.
static SDL_Surface *screen;
static SDL_Surface *cmap;

int sx = SCREEN_WIDTH;
int sy = SCREEN_HEIGHT / 2;

//Draw the background
void draw_background () {

	SDL_Rect src;

	src.x = 0;
	src.y = 0;
	src.w = screen->w;
	src.h = screen->h;
	
	SDL_FillRect(screen,&src,0);
}

//Draw a char
int draw_char(char c, int x, int y) {

	SDL_Rect src;
	SDL_Rect dest;
	int i,j;
	char *map[] = {"ABCDEFGHIJKLMNOPQRSTUVWXYZ",
			"abcdefghijklmnopqrstuvwxyz",
			"!@#$%^&*()_+{}|:\"<>?,.;'-=",
			"0123456789"};

	src.x = 0;
	src.y = 0;
	src.w = 20;
	src.h = 20;
	
	dest.x = x;
	dest.y = y;
	dest.w = 20;
	dest.h = 20;

	for (i = 0; i < 4; i++) {
	
		for(j = 0; j < strlen(map[i]); j++) {
			
			if (c == map[i][j]) {
			
				SDL_BlitSurface(cmap, &src, screen, &dest);
				return 0;
			}

			src.x += 20;
		}
	
		src.y += 20;//move down one line on the image file
		src.x = 0; //reset to start of line
	}

	return 0;
}

//Draw a string of chars
void draw_string(char s[], int x, int y) {

	int i;

	for (i = 0; i < strlen(s); i++) {
	
		int amp =  sin(.02 * x) * 30;

		draw_char(s[i],x,y + amp);
		x += 20;
	}
}

//Load image files
int load_image(char filename[], SDL_Surface **surface) {

	SDL_Surface *temp;
	
	//load image 
	temp = SDL_LoadBMP(filename);
	
	if (temp == NULL) {
	
		printf("Unable to load %s.\n", filename);
		return 1;
	}


	/* Set the image colorkey. */
	Uint32 colourkey = SDL_MapRGB(temp->format, 255, 0, 255);
	SDL_SetColorKey(temp, SDL_SRCCOLORKEY, colourkey);

	//convert the image surface to the same type as the screen
	(*surface) = SDL_DisplayFormat(temp);
	
	if ((*surface) == NULL) {
	
		printf("Unable to convert bitmap.\n");
		return 1;
	}
	
	SDL_FreeSurface(temp);

	return 0;
}

//move_string
void move_string(char s[]) {

	int l = strlen(s) * 20;
	sx -= 8;

	if (sx < -l) {
	
		sx = SCREEN_WIDTH - 1;
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
	SDL_WM_SetCaption("Space Invaders", "P");
	
	/* Attempt to set a 800x600 8 bit color video mode */
	screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, 8, SDL_DOUBLEBUF );
	
	if (screen == NULL) {
		
		printf("Unable to set video mode: %s\n", SDL_GetError());
		return 1;
	}

	//load images
	load_image("cmap.bmp", &cmap);

	Uint32 next_game_tick = SDL_GetTicks();
	int sleep = 0;
	int quit = 0;
	SDL_Event event;

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

		char s[] = "Well well well here it is finally a sine scroller, funny how easy it all is. and that its taken me so looooooooooooooooooooong";
		draw_string(s, sx, sy);
		move_string(s);
		
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
