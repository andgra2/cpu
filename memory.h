#ifndef MEMORY_H
#define MEMORY_H

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

typedef unsigned int cell;

extern const cell pc_null;

struct memory {
	cell **bs;
	size_t bi_ff;
};

extern bool memory_create(struct memory *m);
extern void memory_destroy(struct memory *m);
extern cell memory_new_block(struct memory *m);
extern void memory_delete_block(struct memory *m, cell pc);
extern cell memory_get(struct memory *m, cell pc);
extern void memory_set(struct memory *m, cell pc, cell p);

#endif

