
//vector.h

#ifndef VECTOR_H
#define VECTOR_H

struct vector3d {
	
	float x;
	float y;
	float z;
};

void add_vector(struct vector3d* a, struct vector3d* b);

struct vector3d add_vector_new(struct vector3d* a, struct vector3d* b);

void multiply_vector(struct vector3d* v, float n);

void divide_vector(struct vector3d* v, float n);

//void print_vector(struct vector2d* a);

void rotate_vector(struct vector3d* v, float degrees);

//float magnitude_vector(struct vector2d* v);

//void normalise_vector(struct vector2d* v);

//void limit_vector(struct vector2d* v, float limit);

#endif //VECTOR_H
