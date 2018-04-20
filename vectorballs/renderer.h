
//renderer.h

#include <stdint.h>
#include <math.h> 	//link option "-l"

struct pix_buff {

	int width;
	int height;
	uint32_t *pixels;
};

//assign a colour to a single pixel
int draw_pixel(struct pix_buff *pb, int x, int y, uint32_t colour);

//draws a line
int draw_line(struct pix_buff *pb, int x1, int y1, int x2, int y2, uint32_t colour);

//draws a circle
void draw_circle(struct pix_buff *pb, int x1, int y1, float radius, uint32_t colour);

//assign a colour to all pixels
void clear_pixels(struct pix_buff *pb, uint32_t colour);

//populates a pix_buff struct and mallocs memory
int create_pixel_buffer(struct pix_buff *pb, int width, int height);

//frees and malloc memory used by the pix_buff struct
void free_pixel_buffer(struct pix_buff *pb);

