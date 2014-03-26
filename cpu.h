#ifndef CPU_H
#define CPU_H

#include "mem.h"

struct cpu {
	struct mem *m;
	cell pc_curr_instr;
	cell *arg_ptr_1;
	cell *arg_ptr_2;
	cell *arg_ptr_3;
	cell *arg_ptr_4;
};

void cpu_initialize(struct cpu *u, struct mem *m);
void cpu_run_init(struct cpu *u, cell pc_start_instr);
bool cpu_run_step(struct cpu *u);
bool cpu_is_run(struct cpu *u);

#endif

