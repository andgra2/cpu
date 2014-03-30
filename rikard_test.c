#include "vm.h"

int main (int argc, char *charv[])
{
	DEBUG("rikard test");

	/* VM */
	struct vm *vm = malloc(sizeof(struct vm));
	vm_create(vm);
	vm_destroy(vm);
	free(vm);

	/* MEM TEST */
	struct mem mt;
	mem_create(&mt);

	cell pc = mem_new_block(&mt);
	pc = mem_new_block(&mt);
	printf("cell pc: %d\n", pc);

	*mem_ptr(&mt, pc + 3) = 23;
	*mem_ptr(&mt, pc + 12) = 1337;

	pc = mem_new_block(&mt);
	pc = mem_new_block(&mt);
	*mem_ptr(&mt, pc + 12) = 1338;
	pc = mem_new_block(&mt);

	mem_state_debug(&mt);

	mem_to_file(&mt, "mem.dmp");
	mem_destroy(&mt);

	/* MEM FROM FILE */
	struct mem *m = malloc(sizeof(struct mem));
	mem_create_from_file(m, "mem.dmp");
	mem_state_debug(m);
	mem_destroy(m);

	free(m);

	return 0;
}
