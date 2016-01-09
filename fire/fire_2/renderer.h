
//renderer.h

#ifndef RENDERER_H
#define RENDERER_H

#include <stdint.h>
#include <stdlib.h>
#include <math.h>

struct pixel_buffer {
	
	uint32_t* pixels;
	int width;
	int height;
};

//assing a colour to a single pixel
int draw_pixel(struct pixel_buffer* pb, int x, int y, uint32_t colour);

uint32_t get_pixel(struct pixel_buffer* pb, int x, int y);

int draw_line(struct pixel_buffer* pb, int x1, int y1, int x2, int y2, uint32_t colour);

void draw_circle(struct pixel_buffer* pb, int x1, int y1, float radius, uint32_t colour);

//assign a colour to all pixels
void clear_pixels(struct pixel_buffer* pb, uint32_t colour);

void create_renderer(struct pixel_buffer* pb, int w, int h);

#endif //RENDERER_H
