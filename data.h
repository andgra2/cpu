#ifndef DATA_H
#define DATA_H

#include "util.h"

enum data_type {data_type_scalar, data_type_array};

struct data {
	char *label;
	size_t len;
};

bool data_scalar_create(struct data *d, const char *label);
bool data_array_create(struct data *d, const char *label, size_t len);
void data_destroy(struct data *d);
enum data_type data_type(struct data *d);
const char *data_label(struct data *d);
size_t data_array_len(struct data *d);

#endif

