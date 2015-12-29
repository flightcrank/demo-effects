
//stars.c

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "stars.h"
#include "renderer.h"

void init_stars(struct star s[]) {

	int i = 0;

	for (i = 0; i < NUM_STARS; i++) {
		
		s[i].location.x = (float) (rand() % 200 - 100) / 100;
		s[i].location.y = (float) (rand() % 200 - 1000) / 100;
		s[i].location.z = (float) (rand() % 100 + 1) / 100;
		s[i].location.x *= SCREEN_WIDTH / 2;
		s[i].location.y *= SCREEN_HEIGHT / 2;
		s[i].location.z *= 2;
	}
}

void draw_stars(uint32_t* buffer, struct star s[]) {

	int i = 0;

	for (i = 0; i < NUM_STARS; i++) {
		
		//translate to screen space;
		struct vector3d translate = {SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0};
		struct vector3d screen = s[i].location;

		divide_vector(&screen, screen.z);		//calculate prespective projection
		add_vector(&screen, &translate); 		//convert world space to screen space

		if (s[i].location.z > 1.5) {

			s[i].colour = 0x55555500;
		
		} else if (s[i].location.z > 1 && s[i].location.z < 1.5) {
			
			s[i].colour = 0xaaaaaaff;
		
		} else {
		
			s[i].colour = 0xffffffff;
		}

		draw_pixel(buffer, screen.x, screen.y, s[i].colour);
	}
}

void update_stars(struct star s[]) {
	

	int i = 0;

	for (i = 0; i < NUM_STARS; i++) {

		struct vector3d velocity = {0, 0, -.007};
		add_vector(&s[i].location, &velocity);
	
		//star is behind camera, reset star
		if (s[i].location.z < 0 ) {
			
			s[i].location.x = (float) (rand() % 200 - 100) / 100;
			s[i].location.y = (float) (rand() % 200 - 100) / 100;
			s[i].location.z = (float) (rand() % 100 + 1) / 100;
			s[i].location.x *= SCREEN_WIDTH / 2;
			s[i].location.y *= SCREEN_HEIGHT / 2;
			s[i].location.z *= 3;
		}
	}
}

