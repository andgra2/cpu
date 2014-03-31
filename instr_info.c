#include "instr_info.h"

const size_t max_num_instr_args = MAX_NUM_INSTR_ARGS;

const struct instr_info instr_infos[] = {
	{"EXIT", 0, {}},
	{"JMP", 1, {val}},
	{"JMP", 2, {val, val}},
	{"JMP", 3, {val, val, val}},
	{"MOV", 2, {ref, val}},
	{"ADD", 3, {ref, val, val}},
	{"SUB", 3, {ref, val, val}},
	{"MUL", 3, {ref, val, val}},
	{"DIV", 3, {ref, val, val}},
	{"MOD", 3, {ref, val, val}},
	{"AND", 3, {ref, val, val}},
	{"OR", 3, {ref, val, val}},
	{"XOR", 3, {ref, val, val}},
	{"NOT", 2, {ref, val}},
	{"SHL", 3, {ref, val, val}},
	{"SHR", 3, {ref, val, val}},
	{"PRINT", 1, {val}},
	{"PRINT_CELL", 1, {val}},
	{"PRINT_CELL_HEX", 1, {val}},
};

const size_t num_instrs = (sizeof(instr_infos) / sizeof(struct instr_info));

