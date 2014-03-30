#include "vm.h"

/*
 * Create virtual machine.
 */
bool vm_create(struct vm *vm)
{
	if (!mem_create(&vm->mem))
	{
		free(vm);
		return false;
	}

	return true;
}

/*
 * Destroy a virtual machine.
 */
bool vm_destroy(struct vm *vm)
{
	mem_destroy(&vm->mem);

	return true;
}

/*
 * Serialize a virtual machine to a file.
 */
bool vm_to_file(struct vm *vm, const char *filename)
{
	mem_to_file(&vm->mem, "mem.dmp");

	return true;
}

/*
 * Create a virtual machine from a serialized file.
 */
bool vm_create_from_file(struct vm *vm, const char *filename)
{
	return true;
}
