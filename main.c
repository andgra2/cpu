#include <stdio.h>
#include "memory.h"

int main()
{
	struct memory m;
	if (!memory_create(&m))
		goto out_1;

	cell pc;
	if ((pc = memory_new_block(&m)) == pc_null)
		goto out_2;
	memory_set(&m, pc + 3, 23);
	printf("\n%u\n", memory_get(&m, pc + 3));
	memory_delete_block(&m, pc);

	if ((pc = memory_new_block(&m)) == pc_null)
		goto out_3;
	for (int i = 0; i < 8; ++i)
		memory_set(&m, pc + i, i * 2 + 7);
	for (int i = 0; i < 8; ++i)
		printf("\n%u\n", memory_get(&m, pc + i));

	printf("Success!\n");
	return 0;		
out_3:
out_2:
	memory_destroy(&m);
out_1:
	printf("Error!\n");
	return -1;
}

