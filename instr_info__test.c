#include "instr_info.h"

static void print_instr_infos()
{
	printf("Number of instructions: %u\n", (unsigned) num_instrs);

	printf("\n");

	for (int i = 0; i != num_instrs; ++i) {
		if (i != 0) printf("\n");

		const struct instr_info *x = &instr_infos[i];
		
		printf("%3u%15s", (unsigned) i, x->name);

		printf("(");
		for (int j = 0; j != x->num_args; ++j) {
			if (j != 0) printf(", ");
			
			printf( x->arg_types[j] == val ? "val" : "ref" );
		}
		printf(")");
	}

	printf("\n");
}

int main()
{
	print_instr_infos();

	return 0;
}

