#ifndef MEM_H
#define MEM_H

#include "include.h"

typedef unsigned cell;

extern const cell pc_null;

struct mem {
	cell **blocks;
	size_t bi_first_free;
};

bool mem_create(struct mem *m);
void mem_destroy(struct mem *m);
cell mem_new_block(struct mem *m);
void mem_delete_block(struct mem *m, cell pc);
cell *mem_ptr(struct mem *m, cell pc);

#endif

