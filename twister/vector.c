
//vector.c

#include <stdio.h>
#include <math.h>
#include "vector.h"

void add_vector(struct vector2d* a, struct vector2d* b) {

	a->x += b->x;
	a->y += b->y;
}

struct vector2d add_vector_new(struct vector2d* a, struct vector2d* b) {

	struct vector2d new = {a->x + b->x, a->y + b->y};

	return new;
}

void multiply_vector(struct vector2d* v, float n) {

	v->x *= n;
	v->y *= n;
}

void divide_vector(struct vector2d* v, float n) {

	v->x /= n;
	v->y /= n;
}

void print_vector(struct vector2d* a) {
	
	printf("x = %f y = %f\n", a->x, a->y);
}

float magnitude_vector(struct vector2d* v) {
	
	float c2 = pow(v->x, 2) + pow(v->y, 2); 

	return sqrt(c2);
}

void normalise_vector(struct vector2d* v) {

	float mag = magnitude_vector(v);

	divide_vector(v, mag);
}

void limit_vector(struct vector2d* v, float limit) {
	
	float mag = magnitude_vector(v);

	if (mag > limit) {
		
		float ratio = limit / mag;
		v->x *= ratio;
		v->y *= ratio;
	}
}

void rotate_vector(struct vector2d* v, float degrees) {
	
	//calculate radians
	float angle = degrees * M_PI / 180;
	float sine = sin(angle);
	float cosine = cos(angle);
	
	//rotation matix
	float matrix[2][2] = {{cosine, -sine}, {sine, cosine}};

	float x = v->x;
	float y = v->y;

	v->x = matrix[0][0] * x + matrix[0][1] * y;
	v->y = matrix[1][0] * x + matrix[1][1] * y;
}

void scale_vector(struct vector2d *v, float x, float y) {
	
	//	scale identity matrix
	//	(1,0)
	//	(0,1)
	
	//scale matrix
	float matrix[2][2] = {{x, 0}, {0, y}};
	
	v->x = matrix[0][0] * v->x + matrix[0][1] * v->y;
	v->y = matrix[1][0] * v->x + matrix[1][1] * v->y;
}

void reflect_vector(struct vector2d *v, char axis) {

	//	reflection identity matrix
	//	y axis:	(-1,0)	x axis:	(1, 0)
	//			( 0,1)			(0,-1)
	
	if (axis == 'y') {
		
		float matrix[2][2] = {{-1, 0}, {0, 1}};
		
		v->x = matrix[0][0] * v->x + matrix[0][1] * v->y;
		v->y = matrix[1][0] * v->x + matrix[1][1] * v->y;
	}
	
	if (axis == 'x') {
		
		float matrix[2][2] = {{1, 0}, {0, -1}};
		
		v->x = matrix[0][0] * v->x + matrix[0][1] * v->y;
		v->y = matrix[1][0] * v->x + matrix[1][1] * v->y;
	}
}

void shear_vector(struct vector2d *v, float n, char axis) {
	
	//	shear identity matrix
	//	x axis:	(1, n)	u axis:	(1, 0)
	//			(0, 1)			(n, 1)
	
	if (axis == 'y') {
		
		float matrix[2][2] = {{-1, 0}, {n, 1}};
		
		v->x = matrix[0][0] * v->x + matrix[0][1] * v->y;
		v->y = matrix[1][0] * v->x + matrix[1][1] * v->y;
	}
	
	if (axis == 'x') {
		
		float matrix[2][2] = {{1, n}, {0, 1}};
		
		v->x = matrix[0][0] * v->x + matrix[0][1] * v->y;
		v->y = matrix[1][0] * v->x + matrix[1][1] * v->y;
	}
}
	
void add_vector3d(struct vector3d* a, struct vector3d* b) {

	a->x += b->x;
	a->y += b->y;
	a->z += b->z;
}

struct vector3d add_vector_new3d(struct vector3d* a, struct vector3d* b) {

	struct vector3d new = {a->x + b->x, a->y + b->y, a->z + b->z};

	return new;
}

void multiply_vector3d(struct vector3d* v, float n) {

	v->x *= n;
	v->y *= n;
	v->z *= n;
}

void divide_vector3d(struct vector3d* v, float n) {

	v->x /= n;
	v->y /= n;
	v->z /= n;
}

void rotate_vector3d(struct vector3d* v, float degrees, char axis) {
	
	//calculate radians
	float angle = degrees * M_PI / 180;
	float sine = sin(angle);
	float cosine = cos(angle);
	
	float x = v->x;
	float y = v->y;
	float z = v->z;
		
	if (axis == 'x') {
		
		//rotation matix around x axis
		float matrix[3][3] = {{1, 0, 0}, {0, cosine, -sine}, {0, sine, cosine}};
		
		v->x = matrix[0][0] * x + matrix[0][1] * y + matrix[0][2] * z;
		v->y = matrix[1][0] * x + matrix[1][1] * y + matrix[1][2] * z;
		v->z = matrix[2][0] * x + matrix[2][1] * y + matrix[2][2] * z;
	}
	
	if (axis == 'y') {
		
		//rotation matix around y axis
		float matrix[3][3] = {{cosine, 0, sine}, {0, 1, 0}, {-sine, 0, cosine}};
		
		v->x = matrix[0][0] * x + matrix[0][1] * y + matrix[0][2] * z;
		v->y = matrix[1][0] * x + matrix[1][1] * y + matrix[1][2] * z;
		v->z = matrix[2][0] * x + matrix[2][1] * y + matrix[2][2] * z;
	}

	if (axis == 'z') {
		
		//rotation matix around z axis
		float matrix[3][3] = {{cosine, -sine, 0}, {sine, cosine, 0}, {0, 0, 1}};
		
		v->x = matrix[0][0] * x + matrix[0][1] * y + matrix[0][2] * z;
		v->y = matrix[1][0] * x + matrix[1][1] * y + matrix[1][2] * z;
		v->z = matrix[2][0] * x + matrix[2][1] * y + matrix[2][2] * z;
	}
	


}
