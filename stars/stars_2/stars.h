
//stars.h

#include <stdint.h>
#include <SDL.h>
#include "vector.h"
#include "renderer.h"

#define NUM_STARS 1000
#define Z_DEPTH 3

struct star {
	
	struct vector3d location;	//in world space
	uint32_t colour;
	SDL_Rect dest;
};

void init_stars(struct star s[], int width, int height);

void draw_stars(SDL_Renderer* r, SDL_Texture* t, struct star s[]);

void update_stars(struct star s[], int width, int height, struct vector3d* v);

