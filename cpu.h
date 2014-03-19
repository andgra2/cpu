#ifndef CPU_H
#define CPU_H

#include "memory.h"

struct cpu {
	struct memory *m;
	cell pc_ni;
};

void cpu_create(struct cpu *cpu, struct memory *m);
void cpu_run_init(struct cpu *cpu, cell pc_ni);
bool cpu_run_step(struct cpu *cpu);
bool cpu_is_run(struct cpu *cpu);

#endif

