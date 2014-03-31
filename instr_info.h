#ifndef INSTR_INFO_H
#define INSTR_INFO_H

#include "include.h"

#define MAX_NUM_INSTR_ARGS 4

extern const size_t num_instrs;
extern const size_t max_num_instr_args;

enum instr_arg_type {val, ref};

struct instr_info {
	const char *name;
	size_t num_args;
	enum instr_arg_type arg_types[MAX_NUM_INSTR_ARGS];
};

extern const struct instr_info instr_infos[];

#endif

