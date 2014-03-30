#include "data.h"

static const size_t data_len_if_scalar = 0;

bool data_scalar_create(struct data *d, const char *label)
{
	if ((d->label = str_clone(label)) == NULL) return false;

	d->len = data_len_if_scalar;

	return true;
}

bool data_array_create(struct data *d, const char *label, size_t len)
{
	if ((d->label = str_clone(label)) == NULL) return false;

	d->len = len;

	return true;
}

void data_destroy(struct data *d) { free(d->label); }

enum data_type data_type(struct data *d)
{
	return d->len == data_len_if_scalar ? data_type_scalar : data_type_array;
}

const char *data_label(struct data *d) { return d->label; }

size_t data_array_len(struct data *d) { return d->len; }

