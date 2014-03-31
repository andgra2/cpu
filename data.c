#include "data.h"

static const size_t max_num_init_vals = 1;
static const size_t max_num_init_vals_auto_incr = 10;

bool data_scalar_create(struct data *d, const char *label)
{
	if ((d->label = str_clone(label)) == NULL) return false;
	
	if (!vector_create(
		&d->init_vals,
		sizeof(cell),
		max_num_init_vals,
		max_num_init_vals_auto_incr
	)) {
		free(d->label);
		return false;
	}

	d->type = data_type_scalar;
	d->num_vals = 1;

	return true;
}

bool data_array_create(struct data *d, const char *label)
{
	if ((d->label = str_clone(label)) == NULL) return false;

	if (!vector_create(
		&d->init_vals,
		sizeof(cell),
		max_num_init_vals,
		max_num_init_vals_auto_incr
	)) {
		free(d->label);
		return false;
	}

	d->type = data_type_array;

	return true;
}

void data_destroy(struct data *d)
{
	free(d->label);
	vector_destroy(&d->init_vals);
}

enum data_type data_get_type(struct data *d) { return d->type; }

const char *data_get_label(struct data *d) { return d->label; }

void data_scalar_set_init_val(struct data *d, cell init_val)
{
	vector_push_back(&d->init_vals, &init_val);
}

bool data_scalar_has_init_val(struct data *d) { return vector_get_num_vals(&d->init_vals) == 1; }

cell data_scalar_get_init_val(struct data *d)
{
	return * (cell *) vector_get_vals(&d->init_vals);
}

void data_array_set_num_vals(struct data *d, size_t num_vals) { d->num_vals = num_vals; }

size_t data_array_get_num_vals(struct data *d) { return d->num_vals; }

bool data_array_add_init_val(struct data *d, cell init_val)
{
	return vector_push_back(&d->init_vals, &init_val);
}

size_t data_array_get_num_init_vals(struct data *d)
{
	return vector_get_num_vals(&d->init_vals);
}

cell *data_array_get_init_vals(struct data *d) { return vector_get_vals(&d->init_vals); }

