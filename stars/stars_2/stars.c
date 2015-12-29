
//stars.c

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "stars.h"
#include "renderer.h"

void init_stars(struct star s[], int width, int height) {

	int i = 0;

	for (i = 0; i < NUM_STARS; i++) {
		
		s[i].location.x = (float) (rand() % 200 - 100) / 100;
		s[i].location.y = (float) (rand() % 200 - 100) / 100;
		s[i].location.z = (float) (rand() % 100 + 50) / 100;
		s[i].location.x *= width / 2;
		s[i].location.y *= height / 2;
		s[i].location.z *= 3;
		s[i].dest.x = 0;
		s[i].dest.y = 0;
		s[i].dest.w = 1;
		s[i].dest.h = 1;
	}
}

void draw_stars(SDL_Renderer* r, SDL_Texture* t, struct star s[]) {

	int i = 0;
	
	SDL_Rect src;

	src.x = 0;
	src.y = 0;
	src.w = 16;
	src.h = 16;

	for (i = 0; i < NUM_STARS; i++) {
		
		float inv = 2 - s[i].location.z;
		float inter = inv / 2;

		s[i].dest.w = inter * 5;
		s[i].dest.h = inter * 5;
		
		SDL_RenderCopy(r, t, &src, &s[i].dest);
	}
}

void update_stars(struct star s[], int width, int height, struct vector3d* v) {
	
	int i = 0;

	for (i = 0; i < NUM_STARS; i++) {

		//translate to screen space;
		struct vector3d translate = {width / 2, height / 2, 0};
		struct vector3d screen = s[i].location;
		struct vector3d velocity = {v->x, v->y , v->z};
		
		divide_vector(&screen, screen.z);		//calculate prespective projection
		add_vector(&screen, &translate); 		//convert world space to screen space
		
		s[i].dest.x = screen.x;
		s[i].dest.y = screen.y;
		
		add_vector(&s[i].location, &velocity);
	
		//star is behind camera, reset star
		if (s[i].location.z < 0) {
			
			s[i].location.x = (float) (rand() % 200 - 100) / 100;
			s[i].location.y = (float) (rand() % 200 - 100) / 100;
			s[i].location.z = (float) (rand() % 100 + 1) / 100;
			s[i].location.x *= width / 2;
			s[i].location.y *= height / 2;
			s[i].location.z *= 3;
		}
	}
}

