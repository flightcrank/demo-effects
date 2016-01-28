
//renderer.c

#include "renderer.h"

void copy_buffer(struct pixel_buffer* source, struct pixel_buffer* dest) {
	
	int x, y;

	for(x = 0; x < source->width; x++){
		
		for(y = 0; y < source->height; y++){
			
			uint32_t colour = get_pixel(source, x,y);
			draw_pixel(dest, x, y, colour);
		}
	}
}

void draw_circle(struct pixel_buffer* pb, int x1, int y1, float radius, uint32_t colour) {
	
	int x, y;

	for (x = 0; x < pb->width; x++) {
		
		for (y = 0; y < pb->height; y++) {
			
			int a = x1 - x;
			int b = y1 - y;
			float c = sqrt(pow(a, 2) + pow(b, 2));
			
			if (c < radius) {
				
				draw_pixel(pb, x, y, colour);
			}
		}
	}
}

int draw_line(struct pixel_buffer* pb, int x1, int y1, int x2, int y2, uint32_t colour) {

	//plot the first point
	draw_pixel(pb, x1, y1, colour);

	//make sure we draw the line always from left to right
	if (x1 > x2) {

		int temp_x = x1;
		int temp_y = y1;

		x1 = x2;
		y1 = y2;

		x2 = temp_x;
		y2 = temp_y;
	}
	
	int dx = x2 - x1;
	int dy = y2 - y1;

	//the length of the line is greater along the X axis
	if (dx >= fabs(dy)) {
		
		float slope = (float) dy / dx;
	
		//line travels from top to bottom
		if (y1 <= y2) {

			float ideal_y = y1 + slope;
			int y = (int) round(ideal_y);
			float error = ideal_y - y;

			int i = 0;
			
			//loop through all the X values
			for(i = 1; i <= dx; i++) {
				
				int x = x1 + i;
				
				draw_pixel(pb, x, y, colour);
				
				error += slope;

				if (error  >= 0.5) {
				
					y++;
					error -= 1;
				}
			}
		}
		
		//line travels from bottom to top
		if (y1 > y2) {
			
			float ideal_y = y1 + slope;
			int y = (int) round(ideal_y);
			float error = ideal_y - y;

			int i = 0;
			
			//loop through all the x values
			for(i = 1; i <= dx; i++) {
				
				int x = x1 + i;
				
				draw_pixel(pb, x, y, colour);
				
				error += slope;

				if (error  <= -0.5) {
				
					y--;
					error += 1;
				}
			}
		}


	}

	//the length of the line is greater along the Y axis
	if (fabs(dy) > dx) {
		
		float slope = (float) dx / dy;
		
		//line travels from top to bottom
		if (y1 < y2) {
			
			float ideal_x = x1 + slope;
			int x = (int) round(ideal_x);
			float error = ideal_x - x;

			int i = 0;
			
			//loop through all the y values
			for(i = 1; i <= dy; i++) {
				
				int y = y1 + i;
				
				draw_pixel(pb, x, y, colour);
				
				error += slope;

				if (error  >= 0.5) {
				
					x++;
					error -= 1;
				}
			}
		}
		
		//draw line from bottom to top
		if (y1 > y2) {
			
			float ideal_x = x1 - slope;
			int x = (int) round(ideal_x);
			float error = ideal_x - x;

			int i = 0;
			
			//loop through all the y values
			for(i = 1; i <= fabs(dy); i++) {
				
				int y = y1 - i;
				
				draw_pixel(pb, x, y, colour);
				
				error += slope;

				if (error  <= -0.5) {
				
					x++;
					error += 1;
				}
			}
		}
	}

	return 0;	
}

int draw_pixel(struct pixel_buffer* pb, int x, int y, uint32_t colour) {
	
	//dont draw any pixels that are outside of the pixel buffer
	if (x < 0 || y < 0) {
			
		return 1;
	}
	
	//dont draw any pixels that are outside of the pixel buffer
	if (x >= pb->width || y >= pb->height) {
			
		return 1;
	}

	uint32_t position = y * pb->width + x;
	pb->pixels[position] = colour;

	return 0;
}

uint32_t get_pixel(struct pixel_buffer* pb, int x, int y) {
	
	uint32_t position = y * pb->width + x;
	
	return pb->pixels[position];
}

void clear_pixels(struct pixel_buffer* pb, uint32_t colour) {

	int i = 0;
	int buffer_size = pb->width * pb->height;

	for (i = 0; i < buffer_size; i++) {
		
		pb->pixels[i] = colour;
	}
}

void create_renderer(struct pixel_buffer* pb, int w, int h) {
	
	pb->pixels = NULL;
	pb->width = w;
	pb->height = h;
	
	//allocate pixel buffer
	pb->pixels = (uint32_t*) malloc((w * h) * sizeof(uint32_t));
}

