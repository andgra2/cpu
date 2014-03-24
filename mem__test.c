#include "mem.h"

int main(void)
{
	int ret = 0;

	struct mem m;
	if (!mem_create(&m)) {
		ret = -1;
		goto out_1;
	}

	cell pc;
	if ((pc = mem_new_block(&m)) == pc_null) {
		ret = -2;
		goto out_2;
	}

	*mem_ptr(&m, pc + 3) = 23;
	printf("\n%u\n", *mem_ptr(&m, pc + 3));
	mem_delete_block(&m, pc);

	if ((pc = mem_new_block(&m)) == pc_null) {
		ret = -3;
		goto out_2;
	}

	for (cell i = 0; i < 8; ++i)
		*mem_ptr(&m, pc + i) = 2 * i + 7;

	for (cell i = 0; i < 8; ++i)
		printf("\n%u\n", *mem_ptr(&m, pc + i));

out_2:
	mem_destroy(&m);
out_1:
	return ret;
}

