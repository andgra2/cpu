#ifndef DATA_H
#define DATA_H

#include "util.h"
#include "vector.h"
#include "mem.h"

enum data_type {data_type_scalar, data_type_array};

struct data {
	enum data_type type;
	char *label;
	size_t num_vals;
	struct vector init_vals;
};

bool data_scalar_create(struct data *d, const char *label);
bool data_array_create(struct data *d, const char *label);
void data_destroy(struct data *d);
enum data_type data_get_type(struct data *d);
const char *data_get_label(struct data *d);
void data_scalar_set_init_val(struct data *d, cell init_val);
bool data_scalar_has_init_val(struct data *d);
cell data_scalar_get_init_val(struct data *d);
void data_array_set_num_vals(struct data *d, size_t num_vals);
size_t data_array_get_num_vals(struct data *d);
bool data_array_add_init_val(struct data *d, cell init_val);
size_t data_array_get_num_init_vals(struct data *d);
cell *data_array_get_init_vals(struct data *d);

#endif

