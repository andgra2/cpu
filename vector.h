#ifndef VECTOR_H
#define VECTOR_H

#include "include.h"

struct vector {
	void *vals;
	size_t num_vals;
	size_t val_size;
	size_t max_num_vals;
	size_t max_num_vals_auto_incr;
};

bool vector_create(
	struct vector *v,
	size_t val_size,
	size_t max_num_vals,
	size_t max_num_vals_auto_incr
);
void vector_destroy(struct vector *v);
void *vector_get_vals(struct vector *v);
size_t vector_get_num_vals(struct vector *v);
size_t vector_get_max_num_vals(struct vector *v);
bool vector_resize(struct vector *v, size_t max_num_vals_new);
void vector_shrink(struct vector *v);
bool vector_push_back(struct vector *v, const void *val);

#endif

