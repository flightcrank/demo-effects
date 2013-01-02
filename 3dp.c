#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "SDL/SDL.h"

#define SCREEN_WIDTH 800 
#define SCREEN_HEIGHT 600
#define CENTER_X SCREEN_WIDTH / 2 
#define CENTER_Y SCREEN_HEIGHT / 2
#define ZOOM 450
#define POINTS 8
#define EDGES 12

struct point3D {

	double x;
	double y;
	double z;
};

struct edge {

	int point_1;
	int point_2;
};

//global variables, for convenience.
static SDL_Surface *screen;
struct point3D cube[POINTS];
struct point3D rot_cube[POINTS];
struct point3D new_cube[POINTS];
struct edge edges[EDGES];
double matrix_x[3][3];
double matrix_y[3][3];
double matrix_z[3][3];
double matrix3[3][3];
double new_matrix3[3][3];

struct point3D multi_matrix_vector(struct point3D p) {

	int x,y;
	double temp[3];
	double total = 0;
	struct point3D new_point;

	temp[0] = p.x;
	temp[1] = p.y;
	temp[2] = p.z;

	for (y = 0; y < 3; y++) {
		
		for (x = 0; x < 3; x++) {
			
			total += matrix_y[y][x] * temp[x];		
		}
		
			switch (y) {
				
				case 0:
					new_point.x = total;
				break;
				
				case 1:
					new_point.y = total;
				break;
				
				case 2:
					new_point.z = total;
				break;
			}

		total = 0;
	}

	//printf("x = %f y = %f z = %f\n", new_point.x, new_point.y, new_point.z);
	return new_point;
}

void multi_matrix_matrix(double matrix1[3][3], double matrix2[3][3]) {

	int x,y,i;
	int count = 0;
	double matrix_list[9];
	double total = 0;

	for (x = 0; x < 3; x++) {
				
		for (i = 0; i < 3; i++) {
		
			for (y = 0; y < 3; y++) {
					
				total += matrix1[x][y] * matrix2[y][i];
			}
			
			matrix_list[count] = total;
			count++;
			total = 0;
		}
	}

	count = 0;

	for (y = 0; y < 3; y++) {
				
		for (x = 0; x < 3; x++) {

			new_matrix3[x][y] = matrix_list[count];
			count++;
		}
	}

}

//rotation matrix for x
void rotation_matrix_x (double angle) {

	double angle_radians = angle * 3.14 / 180;

	matrix_x[0][0] = 1;
	matrix_x[0][1] = 0;
	matrix_x[0][2] = 0;
	
	matrix_x[1][0] = 0;
	matrix_x[1][1] = cos(angle_radians);
	matrix_x[1][2] = -sin(angle_radians);
	
	matrix_x[2][0] = 0;
	matrix_x[2][1] = sin(angle_radians);
	matrix_x[2][2] = cos(angle_radians);
}

//rotation matrix for y
void rotation_matrix_y (double angle) {

	double angle_radians = angle * 3.14 / 180;

	matrix_y[0][0] = cos(angle_radians);
	matrix_y[0][1] = 0;
	matrix_y[0][2] = -sin(angle_radians);
	
	matrix_y[1][0] = 0;
	matrix_y[1][1] = 1;
	matrix_y[1][2] = 0;
	
	matrix_y[2][0] = sin(angle_radians);
	matrix_y[2][1] = 0;
	matrix_y[2][2] = cos(angle_radians);
}

//rotation matrix for z
void rotation_matrix_z (double angle) {

	double angle_radians = angle * 3.14 / 180;

	matrix_z[0][0] = cos(angle_radians);
	matrix_z[0][1] = -sin(angle_radians);
	matrix_z[0][2] = 0;
	
	matrix_z[1][0] = sin(angle_radians);
	matrix_z[1][1] = cos(angle_radians);
	matrix_z[1][2] = 0;

	matrix_z[2][0] = 0;
	matrix_z[2][1] = 0;
	matrix_z[2][2] = 1;
}

//init cube points
void init_cube() {

	//Define verts for a cube, two worldspace units in size
	//(-1 to 1 in each axis)

	cube[0].x = -1.0; cube[0].y = -1.0 ; cube[0].z = -1.0;
	cube[1].x = -1.0; cube[1].y = -1.0 ; cube[1].z = 1.0;
	cube[2].x = -1.0; cube[2].y = 1.0 ; cube[2].z = -1.0;
	cube[3].x = -1.0; cube[3].y = 1.0 ; cube[3].z = 1.0;
	cube[4].x = 1.0; cube[4].y = -1.0 ; cube[4].z = -1.0;
	cube[5].x = 1.0; cube[5].y = -1.0 ; cube[5].z = 1.0;
	cube[6].x = 1.0; cube[6].y = 1.0 ; cube[6].z = -1.0;
	cube[7].x = 1.0; cube[7].y = 1.0 ; cube[7].z = 1.0;

	edges[0].point_1 = 0; edges[0].point_2 = 1;		
	edges[1].point_1 = 0; edges[1].point_2 = 2;		
	edges[2].point_1 = 0; edges[2].point_2 = 4;		
	edges[3].point_1 = 1; edges[3].point_2 = 3;		
	edges[4].point_1 = 1; edges[4].point_2 = 5;		
	edges[5].point_1 = 2; edges[5].point_2 = 3;		
	edges[6].point_1 = 2; edges[6].point_2 = 6;		
	edges[7].point_1 = 3; edges[7].point_2 = 7;		
	edges[8].point_1 = 4; edges[8].point_2 = 5;		
	edges[9].point_1 = 4; edges[9].point_2 = 6;		
	edges[10].point_1 = 5; edges[10].point_2 = 7;		
	edges[11].point_1 = 6; edges[11].point_2 = 7;		
}

//plot a pixel
void draw_pixel(int x, int y, Uint32 colour) {

	//sometimes necessary to lock surface it before it can be modified
	SDL_LockSurface(screen);

	/* Get a pointer to the video surface memory. */
	Uint32 *raw_pixels;
	raw_pixels = (Uint32 *) screen->pixels;

	//calculate the location in memory where the disired pixel starts at
	//first value is the y location, second is the size of the screen width in memory, last value is the x location
	int pix_offset = y * (screen->pitch / 4) + x;

	//set the pixel to the desired colour
	raw_pixels[pix_offset] = colour;

	//finished drawing, so unlock the surface
	SDL_UnlockSurface(screen);
}

//Draw a line. ALL lines will be drawn from the left to the right
void draw_line(int x1, int y1, int x2, int y2, Uint32 colour) {
	
	//if points are given from left of the screen to right
	if (x2 < x1) {
		
		int tempx = x2;
		int tempy = y2;

		x2 = x1;
		y2 = y1;
		x1 = tempx;  
		y1 = tempy;  
	}
	

	int x = x1;
	int y = y1;
	int dx = x2 - x1; //change in x
	int dy = abs(y2 - y1); //change in y. Absolute values are needed due to cases where point 2 is above point 1 
	float y_exact = y1; //exact y coordinate
	float x_exact = x1; //exact x coordinate
	float slope = (float)dy / (float)dx; //slope of the line
	float error = slope; //distance away from the ideal x or y coordinate

	int i;
	
	//x axis is longer than the y axis
	if (dx > dy) {
		
		//line travels from top to bottom
		if (y2 > y1) {
			
			//printf("\ndx = %d dy = %d\n", dx, dy);
			//printf("slope = %f\n", slope);
		
			for (i = 0; i <= dx; i++) {
				
				draw_pixel(x + i, y, colour);
				
				//printf("\nPlotted line x = %d, y = %d\n", x + i , y);
				//printf("Ideal line x = %d, y = %f\n", x + i , y_exact);
				
				//ideal line y co-ord
				y_exact += slope;

				if (error >= 0.5) {
				
					y++;
				}
				
				error = y_exact - y; //plotted y coordinate distance from the ideal y coordinate
				
				//printf("Error distace = %f\n", error);
				
				error += slope;
				
				//printf("Error from ideal line = %f\n", error);
			}
			
			//printf("\n");
		
		//line travels from bottom to top
		} else {
			
			for (i = 0; i <= dx; i++) {
				
				draw_pixel(x + i, y, colour);
				
				//ideal line y co-ord
				y_exact -= slope;

				if (error >= 0.5) {
				
					y--;
				}
				
				// the calculation is the reverse of to to bottom case
				error = y - y_exact; //plotted y coordinate distance from the ideal y coordinate
				error += slope;
			}
		}

	//y axix is longer than the x axis
	} else {
	
		//line travels form top to bottom
		if (y2 > y1) {
		
			for (i = 0; i <= dy; i++) {
			
				draw_pixel(x, y + i, colour);
				
				//ideal line x co-ord
				x_exact += 1 / slope;

				if (error >= 0.5) {
				
					x++;
				}
				
				error = x_exact - x; //plotted x coordinate distance from the ideal x coordinate
				error += 1 / slope;
			}
		
		//line travels form bottom to top 
		} else {
			
			for (i = 0; i <= dy; i++) {
			
				draw_pixel(x, y - i, colour); //increment is subtracted from y
				
				//ideal line x co-ord
				x_exact += 1 / slope;

				if (error >= 0.5) {
				
					x++;
				}
				
				error = x_exact - x; //plotted x coordinate distance from the ideal x coordinate
				error += 1 / slope;
			}
		}
	}
}

//project point on 2d screen
struct point3D project_point(struct point3D p) {

	//puts the x positive diretion to the right and negative to the left
	p.x = -p.x;

	double screen_x = p.x / p.z;
	double screen_y = p.y / p.z; 
	
	screen_x *= ZOOM;
	screen_y *= ZOOM;
	screen_x += CENTER_X;
	screen_y += CENTER_Y;
	
	struct point3D projected;
	
	projected.x = screen_x;
	projected.y = screen_y;
	projected.z = 0;

	return projected;
}

//add points
struct point3D add_point3D(struct point3D p, int x, int y, int z) {
	
	struct point3D new_point;
	new_point.x = p.x + x;
	new_point.y = p.y + y;
	new_point.z = p.z + z;

	return new_point;
}

//Draw Points
void draw_points() {
	
	int i;
	int colour = SDL_MapRGB(screen->format, 0, 255, 0);
	struct point3D point_1;
	struct point3D point_2;

	for (i = 0; i < EDGES; i++) {
	
		int index_1 = edges[i].point_1;
		int index_2 = edges[i].point_2;
		
		//convert 3d point into 2d point
		point_1 = project_point(new_cube[index_1]); 
		point_2 = project_point(new_cube[index_2]); 
		draw_line(point_1.x, point_1.y,point_2.x, point_2.y, colour);
	}
}

//Draw the background
void draw_background() {

	SDL_Rect src;

	src.x = 0;
	src.y = 0;
	src.w = screen->w;
	src.h = screen->h;
	
	SDL_FillRect(screen,&src,0);
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
	screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, 32, SDL_DOUBLEBUF );
	
	if (screen == NULL) {
		
		printf("Unable to set video mode: %s\n", SDL_GetError());
		return 1;
	}

	Uint32 next_game_tick = SDL_GetTicks();
	int sleep = 0;
	int quit = 0;
	SDL_Event event;

	init_cube();
	double rot_y = 0;
	double rot_x = 0;
	double rot_z = 0;
	
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
	
		rot_y -= 3;
		rot_x += 1;
		rot_z += 1;
	
		if (rot_y > 360) {
			
			//rot_y -=360;
		}

		//create rotation
		rotation_matrix_y(rot_y); 
		rotation_matrix_x(rot_x); 
		rotation_matrix_z(rot_z); 
		
		//multi_matrix_matrix(matrix_z, matrix_y);	
		
		int i;

		//perform rotation to cubes original points
		//store them in a new array
		for (i = 0; i < POINTS; i++) {

			rot_cube[i] = multi_matrix_vector(cube[i]);
		}
		
		//transform points
		for (i = 0; i < POINTS; i++) {
				
			new_cube[i] = add_point3D(rot_cube[i], 0, 0, -6);
		}

		//draw_background 
		draw_background(); 

		//draw_points
		draw_points();

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
