
//balls.h

#include <stdint.h>
#include <SDL.h>
#include "vector.h"
#include "renderer.h"


void init_vector_balls(struct vector3d ball[]);

void draw_vector_balls(SDL_Renderer* r, SDL_Texture* t, struct vector3d ball[], int size, int width, int height);

void update_balls(struct vector3d balls[], int size, float degrees);

