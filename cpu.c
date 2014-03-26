#include "cpu.h"

#include "cpu-instr.c"

enum arg_type {
	im,
	mem,
	mem2,
};

static enum arg_type at_from_an(struct cpu *u, size_t an)
{
	cell c = *mem_ptr(u->m, u->pc_curr_instr);
	
	return (enum arg_type) ((c >> (12 + 4 * an)) & 0x0000000F);
}

static cell *arg_ptr_im(struct cpu *u, size_t an)
{
	return mem_ptr(u->m, u->pc_curr_instr + an + 1);
}

static cell *arg_ptr_mem(struct cpu *u, size_t an)
{
	cell pc = *arg_ptr_im(u, an);
	
	return mem_ptr(u->m, pc);
}

static cell *arg_ptr_mem2(struct cpu *u, size_t an)
{
	cell pc = *arg_ptr_mem(u, an);

	return mem_ptr(u->m, pc);
}

static cell *arg_ptr(struct cpu *u, size_t an)
{
	cell *ap;
	switch (at_from_an(u, an))
	{
	case im:
		ap = arg_ptr_im(u, an);
		break;
	case mem:
		ap = arg_ptr_mem(u, an);
		break;
	case mem2:
		ap = arg_ptr_mem2(u, an);
		break;
	}

	return ap;
}

static size_t curr_ii(struct cpu *u)
{
	return *mem_ptr(u->m, u->pc_curr_instr) & 0x0000FFFF;
}

void cpu_initialize(struct cpu *u, struct mem *m)
{
	u->m = m;
	u->pc_curr_instr = pc_null;
}

void cpu_run_init(struct cpu *u, cell pc_start_instr)
{
	u->pc_curr_instr = pc_start_instr;
}

bool cpu_run_step(struct cpu *u)
{
	size_t ii = curr_ii(u);
	
	size_t na = instructions[ii].num_args;

	switch (na)
	{
	case 4:
		u->arg_ptr_1 = arg_ptr(u, 4);
	case 3:
		u->arg_ptr_2 = arg_ptr(u, 3);
	case 2:
		u->arg_ptr_3 = arg_ptr(u, 2);
	case 1:
		u->arg_ptr_4 = arg_ptr(u, 1);
	}

	u->pc_curr_instr += na + 1;

	instructions[ii].func(u);

	return u->pc_curr_instr != pc_null;
}

bool cpu_is_run(struct cpu *u)
{
	return u->pc_curr_instr != pc_null;
}

