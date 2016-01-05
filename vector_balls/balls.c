
//balls.c

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <SDL.h>
#include "balls.h"
#include "renderer.h"

void init_vector_balls(struct vector3d ball[]) {
	
	ball[0].x = -1;
	ball[0].y = -1;
	ball[0].z = -1;
	ball[1].x = -1;
	ball[1].y = -1;
	ball[1].z = 1;
	ball[2].x = 1;
	ball[2].y = -1;
	ball[2].z = 1;
	ball[3].x = 1;
	ball[3].y = -1;
	ball[3].z = -1;
	ball[4].x = -1;
	ball[4].y = 1;
	ball[4].z = -1;
	ball[5].x = -1;
	ball[5].y = 1;
	ball[5].z = 1;
	ball[6].x = 1;
	ball[6].y = 1;
	ball[6].z = 1;
	ball[7].x = 1;
	ball[7].y = 1;
	ball[7].z = -1;
}

void draw_vector_balls(SDL_Renderer* r, SDL_Texture* t, struct vector3d ball[], int size, int width, int height) {
	
	SDL_Rect  dest;

	dest.w = 30;
	dest.h = 30;

	int i;

	for (i = 0; i < size; i++) {

		struct vector3d z_translate = {0,0,2};	
		struct vector3d screen_translate = {width / 2 , height / 2, 0};	

		struct vector3d world = {ball[i].x, ball[i].y, ball[i].z};
		
		add_vector(&world, &z_translate);
		multiply_vector(&world, 100);
		add_vector(&world, &screen_translate);
		
		dest.x = world.x;
		dest.y = world.y;
		
		SDL_RenderCopy(r, t, NULL, &dest);	//draw screen pixel_buffer
	}

}

void update_balls(struct vector3d balls[], int size, float degrees) {

	
	int i;

	for (i = 0; i < size; i++) {
		
		rotate_vector(&balls[i], degrees);
	}
}

