#include "mem.h"
#include "cpu.h"
#include "mem_serialization.h"

#define DEBUG(x) { printf(x); printf("\n"); }

struct vm {
	struct mem mem;
	struct cpu cpu;
};

extern bool vm_create(struct vm *vm);
extern bool vm_destroy(struct vm *vm);
extern bool vm_to_file(struct vm *vm, const char *filename);
extern bool vm_create_from_file(struct vm *vm, const char *filename);
