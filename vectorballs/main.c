//main.c 

//Using libs SDL, glibc
#include <SDL.h>	//SDL version 2.0
#include <stdio.h>
#include <stdlib.h>
#include "renderer.h"
#include "vector.h"

#define SCREEN_WIDTH 1290	//window height
#define SCREEN_HEIGHT 720 	//window width
#define VBALLS 100
#define TWO_PI M_PI * 2

//function prototypes
int init(int w, int h, int argc, char *args[]);
void selection_sort(struct vector3d l[], int len);
void vector_cube(struct vector3d l[], int len);
void vector_plane(struct vector3d l[], int len);
void vector_sphere(struct vector3d l[], int len);

//globals
SDL_Window* window = NULL;	//The window we'll be rendering to
SDL_Renderer *renderer;		//The renderer SDL will use to draw to the screen
SDL_Texture *ball_t;
struct pix_buff ball_pb;
int width, height;			//used if fullscreen

struct vector3d v_balls[VBALLS]; //array of vectors used to display final shape
struct vector3d plane[VBALLS];
struct vector3d cube[VBALLS];
struct vector3d sphere[VBALLS];

int main (int argc, char *args[]) {
	
	//SDL Window setup
	if (init(SCREEN_WIDTH, SCREEN_HEIGHT, argc, args) == 1) {
		
		return 0;
	}
	
	SDL_GetWindowSize(window, &width, &height);
	
	draw_circle(&ball_pb, ball_pb.width / 2, ball_pb.height / 2, ball_pb.width / 2, 0xffffffff);
	SDL_UpdateTexture(ball_t, NULL, ball_pb.pixels, ball_pb.width * sizeof(uint32_t));
	
	SDL_RenderClear(renderer);
	vector_sphere(sphere, VBALLS);
	vector_cube(cube, VBALLS);	//generate a cube of vector balls
	vector_plane(plane, VBALLS); //generate a plane of vector balls
	
	int sleep = 0;
	int quit = 0;
	Uint32 next_game_tick = SDL_GetTicks();
	
	unsigned int inc = 0;	//used for interpolation if vector balls
	unsigned int frame = 1;	//used to count frames of animation
	unsigned int flip = 0;  //used to change vector balls between shapes
	int scale;				//used to change scale of vector ball shapes
	
	//render loop
	while(quit == 0) {
		
		//check for new events every frame
		SDL_PumpEvents();

		const Uint8 *state = SDL_GetKeyboardState(NULL);
		
		if (state[SDL_SCANCODE_ESCAPE]) {
		
			quit = 1;
		}
		
		//draw background
		SDL_RenderClear(renderer);
		
		float interp = (float) inc / 100;
		
		if (interp <= 1) {
			
			inc++;
		}	
		
		//reset interpolation increment
		if (frame % 500 == 0) {
			
			inc = 0;
			flip++;
		}
		
		int i;
		
		//interpolate vectors between shapes
		for (i = 0; i < VBALLS; i++) {
		
			if (flip % 3 == 0 && interp < 1) {
				
				v_balls[i].x = (cube[i].x * interp) + (sphere[i].x * (1 - interp));
				v_balls[i].y = (cube[i].y * interp) + (sphere[i].y * (1 - interp));
				v_balls[i].z = (cube[i].z * interp) + (sphere[i].z * (1 - interp));
				scale = (500 * interp) + (1000 * (1 - interp));
			}
			
			if (flip % 3 == 1 && interp < 1) {
		
				v_balls[i].x = (plane[i].x * interp) + (cube[i].x * (1 - interp));
				v_balls[i].y = (plane[i].y * interp) + (cube[i].y * (1 - interp));
				v_balls[i].z = (plane[i].z * interp) + (cube[i].z * (1 - interp));
				scale = (1000 * interp) + (500 * (1 - interp));
			}
			
			if (flip % 3 == 2 && interp < 1) {
		
				v_balls[i].x = (sphere[i].x * interp) + (plane[i].x * (1 - interp));
				v_balls[i].y = (sphere[i].y * interp) + (plane[i].y * (1 - interp));
				v_balls[i].z = (sphere[i].z * interp) + (plane[i].z * (1 - interp));
				//scale = (1000 * interp) + (500 * (1 - interp));
			}
		}
		
		//sort vectors based on there z value
		selection_sort(v_balls, VBALLS);
		
		for (i = 0; i < VBALLS; i++) {
			
			struct vector3d z_translate = {0, 0, 5};	
			struct vector3d world = {v_balls[i].x, v_balls[i].y, v_balls[i].z};
			struct vector2d screen = {world.x, world.y}; 
			struct vector2d screen_translate = {width / 2, height / 2};	

			//colour interpolation value
			float v = (2 + world.z) / 3.66; 
			int c = -v * 255 + 255;
			
			SDL_SetTextureColorMod(ball_t, c, c, c);
			
			//move 3d vectors away from the camera
			add_vector3d(&world, &z_translate);
			
			//calculate perspective for 2d screen
			divide_vector(&screen, world.z);
			
			//scale vectors for 2d screen
			multiply_vector(&screen, scale);
			
			//position vectors
			add_vector(&screen, &screen_translate);
			
			SDL_Rect dest;
			dest.w = 50;
			dest.h = 50;
			dest.x =  screen.x - dest.w / 2;
			dest.y =  screen.y - dest.h / 2;
			SDL_RenderCopy(renderer, ball_t, NULL, &dest);
			rotate_vector3d(&v_balls[i], 1, 'x');
			rotate_vector3d(&v_balls[i], 1, 'y');
			rotate_vector3d(&v_balls[i], 1, 'z');
			rotate_vector3d(&plane[i], 1, 'x');
			rotate_vector3d(&plane[i], 1, 'y');
			rotate_vector3d(&plane[i], 1, 'z');
			rotate_vector3d(&cube[i], 1, 'x');
			rotate_vector3d(&cube[i], 1, 'y');
			rotate_vector3d(&cube[i], 1, 'z');
			rotate_vector3d(&sphere[i], 1, 'x');
			rotate_vector3d(&sphere[i], 1, 'y');
			rotate_vector3d(&sphere[i], 1, 'z');
		}
		
		
		//draw to the screen
		SDL_RenderPresent(renderer);
				
		//time it takes to render 1 frame in milliseconds
		next_game_tick += 1000 / 60;
		sleep = next_game_tick - SDL_GetTicks();
	
		if( sleep >= 0 ) {
            				
			SDL_Delay(sleep);
		}
		
		frame++;
	}

	//free renderer and all textures used with it
	SDL_DestroyRenderer(renderer);
	
	//Destroy window 
	SDL_DestroyWindow(window);

	//Quit SDL subsystems 
	SDL_Quit(); 
	 
	return 0;
}

//generate vector plane
void vector_plane(struct vector3d plane[], int len) {

	int i, j, count = 0;
	
	int rows = 10;
	int cols = 10;
	
	for (i = 0; i < rows; i++) {
		
		//linearly interpolate between -1 and 1
		float v = ((float) i / (rows - 1) - 0.5) * 2;
	
		for (j = 0; j < cols; j++) {
		
			//linearly interpolate between -1 and 1
			float u = ((float) j / (cols - 1) - 0.5) * 2;
			
			plane[count].x = u;
			plane[count].y = v;
			plane[count].z = 0;
			count++;
		}
	}
}

//generate vector cube
void vector_cube(struct vector3d cube[], int len) {

	int i, j, k, count = 0;
	
	int rows = 5;
	int cols = 5;
	int slice = 4;
	
	for (i = 0; i < rows; i++) {
		
		//linearly interpolate between -1 and 1
		float v = ((float) i / (rows - 1) - 0.5) * 2;
	
		for (j = 0; j < cols; j++) {
			
			//linearly interpolate between -1 and 1
			float u = ((float) j / (cols - 1) - 0.5) * 2;
			
			for (k = 0; k < slice; k++) {
				
				//linearly interpolate between -1 and 1
				float w = ((float) k / (slice - 1) - 0.5) * 2;
				
				cube[count].x = u;
				cube[count].y = v;
				cube[count].z = w;
				count++;
			}
		}
	}
}

void vector_sphere(struct vector3d sphere[], int len) {

	int i, j, count = 0;
	int total_lon = 10;
	int total_lat = 10;
	float r = 1;
	
	for (i = 0; i < total_lat; i++) {
		
		float u = (float) i / (total_lat - 1);
		float lat = u * M_PI;
		
		for (j = 0; j < total_lon; j++) {
			
			float v = (float) j / (total_lon - 1);
			float lon = v * TWO_PI;
			
			sphere[count].x = r * sin(lat) * cos(lon);
			sphere[count].y = r * sin(lat) * sin(lon);
			sphere[count].z = r * cos(lat);
			count++;
		}
	}
}

void selection_sort(struct vector3d list[], int len) {

	int i, j;
	int lowest = 0;	
	
	for(i = 0; i < len; i++) {
		
		lowest = i;
		
		for(j = i; j < len; j++) {

			if (list[j].z >= list[lowest].z) {
				
				lowest = j;
			}
		}
		
		struct vector3d temp = list[i];
		list[i] = list[lowest];
		list[lowest] = temp;
	}
}

int init(int width, int height, int argc, char *args[]) {

	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {

		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		
		return 1;
	} 
	
	int i;
	
	for (i = 0; i < argc; i++) {
		
		//Create window	
		if(strcmp(args[i], "-f")) {
			
			SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN, &window, &renderer);
		
		} else {
		
			SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_FULLSCREEN_DESKTOP, &window, &renderer);
		}
	}
	
	if (window == NULL) { 
		
		printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
		
		return 1;
	}
	
	//create ball image
	create_pixel_buffer(&ball_pb, 100, 100);
	
	//texture to copy ball image to
	ball_t = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, ball_pb.width, ball_pb.height);
	SDL_SetTextureBlendMode(ball_t, SDL_BLENDMODE_BLEND);
	if (ball_pb.pixels == NULL) { 
		
		printf("could not create pixel buffer");
		
		return 1;
	}
	
	if (ball_t == NULL) { 
		
		printf("texture could not be created SDL_Error: %s\n", SDL_GetError());
		
		return 1;
	}

	return 0;
}
