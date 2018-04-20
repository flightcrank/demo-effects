
//vector.h

#ifndef VECTOR_H
#define VECTOR_H

struct vector2d {
	
	float x;
	float y;
};

struct vector3d {

	float x;
	float y;
	float z;
};

void add_vector(struct vector2d* a, struct vector2d* b);

struct vector2d add_vector_new(struct vector2d* a, struct vector2d* b);

void multiply_vector(struct vector2d* v, float n);

void divide_vector(struct vector2d* v, float n);

void print_vector(struct vector2d* a);

void rotate_vector(struct vector2d* v, float degrees);

float magnitude_vector(struct vector2d* v);

void normalise_vector(struct vector2d* v);

void limit_vector(struct vector2d* v, float limit);

void add_vector3d(struct vector3d* a, struct vector3d* b);

struct vector3d add_vector_new3d(struct vector3d* a, struct vector3d *b);

void multiply_vector3d(struct vector3d *v, float n);

void divide_vector3d(struct vector3d* v, float n);

void scale_vector3d(struct vector3d *v, float x, float y, float z);

void rotate_vector3d(struct vector3d* v, float degrees, char axis);
#endif //VECTOR_H
