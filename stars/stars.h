
//stars.h

#include <stdint.h>
#include "vector.h"

#define NUM_STARS 1000

struct star {
	
	struct vector3d location;	//in world space
	uint32_t colour;
};

void init_stars(struct star s[]);

void draw_stars(uint32_t* buffer, struct star s[]);

void update_stars(struct star s[]);

