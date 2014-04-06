#include "vector.h"

static size_t vals_size(size_t max_num_vals, size_t val_size)
{
	size_t num_vals_alloc = max_num_vals + 1;
	
	return num_vals_alloc * val_size;
}

bool vector_create(
	struct vector *v,
	size_t val_size,
	size_t max_num_vals,
	size_t max_num_vals_auto_incr
)
{
	if ((v->vals = malloc(vals_size(max_num_vals, val_size))) == NULL)
		return false;

	v->num_vals = 0;
	v->val_size = val_size;
	v->max_num_vals = max_num_vals;
	v->max_num_vals_auto_incr = max_num_vals_auto_incr;

	return true;
}

void *vector_move(struct vector *v, size_t *num_vals)
{
	*num_vals = v->num_vals;

	vector_shrink(v);

	return v->vals;
}

void vector_destroy(struct vector *v) { free(v->vals); }

void *vector_get_vals(struct vector *v) { return v->vals; }

size_t vector_get_num_vals(struct vector *v) { return v->num_vals; }

size_t vector_get_max_num_vals(struct vector *v) { return v->max_num_vals; }

bool vector_resize(struct vector *v, size_t max_num_vals)
{
	void *vals_new = realloc(v->vals, vals_size(max_num_vals, v->val_size));
	if (vals_new == NULL) return false;
	v->vals = vals_new;

	v->max_num_vals = max_num_vals;

	return true;
}

void vector_shrink(struct vector *v)
{
	vector_resize(v, v->num_vals);
}

bool vector_push_back(struct vector *v, const void *val)
{
	if (v->num_vals == v->max_num_vals) {
		v->max_num_vals += v->max_num_vals_auto_incr;
		if (!vector_resize(v, v->max_num_vals)) return false;
	}

	memcpy(v->vals + v->num_vals * v->val_size, val, v->val_size);

	++v->num_vals;

	return true;
}

