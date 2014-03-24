
#define A1 (*u->arg_ptr_1)
#define A2 (*u->arg_ptr_2)
#define A3 (*u->arg_ptr_3)
#define A4 (*u->arg_ptr_4)
#define CI (u->pc_curr_instr)
#define INSTR(NAME) static void instr_##NAME(struct cpu *u)

INSTR(terminate) { CI = pc_null; }
INSTR(jump) { CI = A1; }
INSTR(cond_jump) { if (A1) CI = A2; }
INSTR(cond_jump2) { if (A1) CI = A2; else CI = A3; }
INSTR(copy) { A2 = A1; }
INSTR(add) { A3 = A1 + A2; }
INSTR(sub) { A3 = A1 - A2; }
INSTR(mul) { A3 = A1 * A2; }
INSTR(div) { A3 = A1 / A2; }
INSTR(mod) { A3 = A1 % A2; }
INSTR(or) { A3 = A1 | A2; }
INSTR(and) { A3 = A1 & A2; }
INSTR(xor) { A3 = A1 ^ A2; }
INSTR(not) { A2 = ~ A1; }
INSTR(shl) { A3 = A1 << A2; }
INSTR(shr) { A3 = A1 >> A2; }
INSTR(print) { printf("%c", A1); fflush(stdout); }
INSTR(print_cell) { printf("%u", A1); fflush(stdout); }
INSTR(print_cell_hex) { printf("%x", A1); fflush(stdout); }

#undef A1
#undef A2
#undef A3
#undef A4
#undef CI
#undef INSTR

typedef void instr_func(struct cpu *u);

struct instr_info {
	instr_func *func;
	size_t num_args;
};

static const struct instr_info instructions[] =
{
	{instr_terminate, 0},
	{instr_jump, 1},
	{instr_cond_jump, 2},
	{instr_cond_jump2, 3},
	{instr_copy, 2},
	{instr_add, 3},
	{instr_sub, 3},
	{instr_mul, 3},
	{instr_div, 3},
	{instr_mod, 3},
	{instr_and, 3},
	{instr_or, 3},
	{instr_xor, 3},
	{instr_not, 2},
	{instr_shl, 3},
	{instr_shr, 3},
	{instr_print, 1},
	{instr_print_cell, 1},
	{instr_print_cell_hex, 1},
};

