#include "cpu.h"
#include <stdio.h>

typedef cell (*BINOP)(cell a, cell b);

static cell binop_add(cell a, cell b) { return a + b; }
static cell binop_sub(cell a, cell b) { return a - b; }
static cell binop_mul(cell a, cell b) { return a * b; }
static cell binop_div(cell a, cell b) { return a / b; }
static cell binop_and(cell a, cell b) { return a & b; }
static cell binop_or(cell a, cell b) { return a | b; }
static cell binop_xor(cell a, cell b) { return a ^ b; }
static cell binop_shl(cell a, cell b) { return a << b; }
static cell binop_shr(cell a, cell b) { return a >> b; }

static enum instr_arg_type iat_get(struct cpu *cpu, cell ian)
{
	cell c = memory_get(cpu->m, cpu->pc_ni);
	return (enum instr_arg_type) (c >> (16 + 4 * ian)) & 0x0000000F;
}

static cell arg_im_get(struct cpu *cpu, cell ian)
{
	return memory_get(cpu->m, cpu->pc_ni + ian + 1);
}

static cell arg_mem_get(struct cpu *cpu, cell ian)
{
	cell pc = arg_im_get(cpu, ian);
	return memory_get(cpu->m, pc);
}

static void arg_mem_set(struct cpu *cpu, cell ian, cell c)
{
	cell pc = arg_im_get(cpu, ian);
	memory_set(cpu->m, pc, c);
}

static cell arg_mem2_get(struct cpu *cpu, cell ian)
{
	cell pc = arg_mem_get(cpu, ian);
	return memory_get(cpu->m, pc);
}

static void arg_mem2_set(struct cpu *cpu, cell ian, cell c)
{
	cell pc = arg_mem_get(cpu, ian);
	memory_set(cpu->m, pc, c);
}

static cell arg_get(struct cpu *cpu, cell ian)
{
	switch (iat_get(cpu, ian))
	{
	case im:
		return arg_im_get(cpu, ian);
	case mem:
		return arg_mem_get(cpu, ian);
	case mem2:
		return arg_mem2_get(cpu, ian);
	}
}

static void arg_set(struct cpu *cpu, cell ian, cell c)
{
	switch (iat_get(cpu, ian))
	{
	case mem:
		arg_mem_set(cpu, ian, c);
		break;
	case mem2:
		arg_mem_set(cpu, ian, c);
		break;
	}
}

static void instr_terminate(struct cpu *cpu)
{
	cpu->pc_ni = pc_null;
}

static void instr_jump(struct cpu *cpu)
{
	cpu->pc_ni = arg_get(cpu, 0);
}

static void instr_jump_cond(struct cpu *cpu)
{
	if (arg_get(cpu, 0))
		cpu->pc_ni = arg_get(cpu, 1);
	cpu->pc_ni += 3;
}

static void instr_copy(struct cpu *cpu)
{
	arg_set(cpu, 1, arg_get(cpu, 0));
	cpu->pc_ni += 3;
}

static void instr_binop(struct cpu *cpu, BINOP binop)
{
	cell a = arg_get(cpu, 0);
	cell b = arg_get(cpu, 1);
	arg_set(cpu, 2, binop(a, b));
	cpu->pc_ni += 4;
}

static void instr_print(struct cpu *cpu)
{
	int value = arg_get(cpu, 0);
	printf("%c", value);
	fflush(stdout);
	cpu->pc_ni += 2;
}

static cell in_get(struct cpu *cpu)
{
	return memory_get(cpu->m, cpu->pc_ni) & 0x0000FFFF;
}

extern void cpu_create(struct cpu *cpu, struct memory *m)
{
	cpu->m = m;
	cpu->pc_ni = pc_null;
}

extern void cpu_run_init(struct cpu *cpu, cell pc_ni)
{
	cpu->pc_ni = pc_ni;
}

extern bool cpu_run_step(struct cpu *cpu)
{
	switch (in_get(cpu))
	{
	case instr_num_terminate: instr_terminate(cpu); break;
	case instr_num_jump: instr_jump(cpu); break;
	case instr_num_jump_cond: instr_jump_cond(cpu); break;
	case instr_num_copy: instr_copy(cpu); break;
	case instr_num_print: instr_print(cpu); break;
	case instr_num_add: instr_binop(cpu, binop_add); break;
	case instr_num_sub: instr_binop(cpu, binop_sub); break;
	case instr_num_mul: instr_binop(cpu, binop_mul); break;
	case instr_num_div: instr_binop(cpu, binop_div); break;
	case instr_num_and: instr_binop(cpu, binop_and); break;
	case instr_num_or: instr_binop(cpu, binop_or); break;
	case instr_num_xor: instr_binop(cpu, binop_xor); break;
	case instr_num_shl: instr_binop(cpu, binop_shl); break;
	case instr_num_shr: instr_binop(cpu, binop_shr); break;
	default: printf("Invalid instruction.\n"); break;
	}

	return cpu->pc_ni != pc_null;
}

extern bool cpu_is_run(struct cpu *cpu)
{
	return cpu->pc_ni != pc_null;
}
