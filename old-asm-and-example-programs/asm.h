
cell make_instruction(enum instr_num instr,
		enum instr_arg_type arg0,
		enum instr_arg_type arg1,
		enum instr_arg_type arg2,
		enum instr_arg_type arg3) {
	return (arg3 << 28) + (arg2 << 24) + (arg1 << 20) + (arg0 << 16) + instr;
}

#define ADD_IIM(in1, in2, out) \
	{\
		cell instr = make_instruction(instr_num_add, im, im, mem, 0);\
		memory_set(&memory, code+writepointer, instr); writepointer ++;\
		memory_set(&memory, code+writepointer, in1); writepointer ++;\
		memory_set(&memory, code+writepointer, in2); writepointer ++;\
		memory_set(&memory, code+writepointer, out); writepointer ++;\
	}

#define ADD_IMM(in1, in2, out) \
	{\
		cell instr = make_instruction(instr_num_add, im, mem, mem, 0);\
		memory_set(&memory, code+writepointer, instr); writepointer ++;\
		memory_set(&memory, code+writepointer, in1); writepointer ++;\
		memory_set(&memory, code+writepointer, in2); writepointer ++;\
		memory_set(&memory, code+writepointer, out); writepointer ++;\
	}

#define ADD_MMM(in1, in2, out) \
	{\
		cell instr = make_instruction(instr_num_add, mem, mem, mem, 0);\
		memory_set(&memory, code+writepointer, instr); writepointer ++;\
		memory_set(&memory, code+writepointer, in1); writepointer ++;\
		memory_set(&memory, code+writepointer, in2); writepointer ++;\
		memory_set(&memory, code+writepointer, out); writepointer ++;\
	}

#define ADD_MIM(in1, in2, out) \
	{\
		cell instr = make_instruction(instr_num_add, mem, im, mem, 0);\
		memory_set(&memory, code+writepointer, instr); writepointer ++;\
		memory_set(&memory, code+writepointer, in1); writepointer ++;\
		memory_set(&memory, code+writepointer, in2); writepointer ++;\
		memory_set(&memory, code+writepointer, out); writepointer ++;\
	}

#define AND_MIM(in1, in2, out) \
	{\
		cell instr = make_instruction(instr_num_and, mem, im, mem, 0);\
		memory_set(&memory, code+writepointer, instr); writepointer ++;\
		memory_set(&memory, code+writepointer, in1); writepointer ++;\
		memory_set(&memory, code+writepointer, in2); writepointer ++;\
		memory_set(&memory, code+writepointer, out); writepointer ++;\
	}

#define SUB_MIM(in1, in2, out) \
	{\
		cell instr = make_instruction(instr_num_sub, mem, im, mem, 0);\
		memory_set(&memory, code+writepointer, instr); writepointer ++;\
		memory_set(&memory, code+writepointer, in1); writepointer ++;\
		memory_set(&memory, code+writepointer, in2); writepointer ++;\
		memory_set(&memory, code+writepointer, out); writepointer ++;\
	}

#define SHR_MIM(in1, in2, out) \
	{\
		cell instr = make_instruction(instr_num_shr, mem, im, mem, 0);\
		memory_set(&memory, code+writepointer, instr); writepointer ++;\
		memory_set(&memory, code+writepointer, in1); writepointer ++;\
		memory_set(&memory, code+writepointer, in2); writepointer ++;\
		memory_set(&memory, code+writepointer, out); writepointer ++;\
	}

#define JUMP_I(address) \
	{\
		cell instr = make_instruction(instr_num_jump, im, 0, 0, 0);\
		memory_set(&memory, code+writepointer, instr); writepointer ++;\
		memory_set(&memory, code+writepointer, address); writepointer ++;\
	}

#define JUMP_M(addressptr) \
	{\
		cell instr = make_instruction(instr_num_jump, mem, 0, 0, 0);\
		memory_set(&memory, code+writepointer, instr); writepointer ++;\
		memory_set(&memory, code+writepointer, addressptr); writepointer ++;\
	}

#define JUMP_COND_MI(valueaddressptr, jumpaddress) \
	{\
		cell instr = make_instruction(instr_num_jump_cond, mem, im, 0, 0);\
		memory_set(&memory, code+writepointer, instr); writepointer ++;\
		memory_set(&memory, code+writepointer, valueaddressptr); writepointer ++;\
		memory_set(&memory, code+writepointer, jumpaddress); writepointer ++;\
	}

#define COPY_MM(valueptr, addressptr) \
	{\
		cell instr = make_instruction(instr_num_copy, mem, mem, 0, 0);\
		memory_set(&memory, code+writepointer, instr); writepointer ++;\
		memory_set(&memory, code+writepointer, valueptr); writepointer ++;\
		memory_set(&memory, code+writepointer, addressptr); writepointer ++;\
	}

#define COPY_IM(value, addressptr) \
	{\
		cell instr = make_instruction(instr_num_copy, im, mem, 0, 0);\
		memory_set(&memory, code+writepointer, instr); writepointer ++;\
		memory_set(&memory, code+writepointer, value); writepointer ++;\
		memory_set(&memory, code+writepointer, addressptr); writepointer ++;\
	}

#define AND_IMM(value, addressptr, address2ptr) \
	{\
		cell instr = make_instruction(instr_num_and, im, mem, 0, 0);\
		memory_set(&memory, code+writepointer, instr); writepointer ++;\
		memory_set(&memory, code+writepointer, value); writepointer ++;\
		memory_set(&memory, code+writepointer, addressptr); writepointer ++;\
		memory_set(&memory, code+writepointer, address2ptr); writepointer ++;\
	}

#define XOR_MMM(valueptr, addressptr, address2ptr) \
	{\
		cell instr = make_instruction(instr_num_xor, mem, mem, mem, 0);\
		memory_set(&memory, code+writepointer, instr); writepointer ++;\
		memory_set(&memory, code+writepointer, valueptr); writepointer ++;\
		memory_set(&memory, code+writepointer, addressptr); writepointer ++;\
		memory_set(&memory, code+writepointer, address2ptr); writepointer ++;\
	}

#define OR_IMM(value, addressptr, address2ptr) \
	{\
		cell instr = make_instruction(instr_num_or, im, mem, 0, 0);\
		memory_set(&memory, code+writepointer, instr); writepointer ++;\
		memory_set(&memory, code+writepointer, value); writepointer ++;\
		memory_set(&memory, code+writepointer, addressptr); writepointer ++;\
		memory_set(&memory, code+writepointer, address2ptr); writepointer ++;\
	}

#define JUMP_COND_MM(valueaddressptr, jumpaddressptr) \
	{\
		cell instr = make_instruction(instr_num_jump_cond, mem, mem, 0, 0);\
		memory_set(&memory, code+writepointer, instr); writepointer ++;\
		memory_set(&memory, code+writepointer, valueaddressptr); writepointer ++;\
		memory_set(&memory, code+writepointer, jumpaddressptr); writepointer ++;\
	}

#define PRINT_I(address) \
	{\
		cell instr = make_instruction(instr_num_print, im, 0, 0, 0);\
		memory_set(&memory, code+writepointer, instr); writepointer ++;\
		memory_set(&memory, code+writepointer, address); writepointer ++;\
	}

#define PRINT_M(addressptr) \
	{\
		cell instr = make_instruction(instr_num_print, mem, 0, 0, 0);\
		memory_set(&memory, code+writepointer, instr); writepointer ++;\
		memory_set(&memory, code+writepointer, addressptr); writepointer ++;\
	}

#define TERMINATE() \
	{\
		cell instr = make_instruction(instr_num_terminate, 0, 0, 0, 0);\
		memory_set(&memory, code+writepointer, instr); writepointer ++;\
	}

#define ASM_INIT() \
	struct memory memory; \
	memory_create(&memory); \
	cell code, code2; \
	code = memory_new_block(&memory); \
	int writepointer = 0;

#define ASM_RUN() \
	struct cpu cpu; \
	cpu_create(&cpu, &memory); \
	cpu_run_init(&cpu, code+0); \
	while(cpu_run_step(&cpu)) { \
	} \
	memory_destroy(&memory);

#define ASM_RUN_DEBUG() \
	struct cpu cpu; \
	cpu_create(&cpu, &memory); \
	cpu_run_init(&cpu, code+0); \
	while(cpu_run_step(&cpu)) { \
		printf("instruktionspekare @ %05X ", cpu.pc_ni-code); \
		for(int i=0; i<30; i++) { \
			printf(" %02X", memory_get(&memory, code+i)); \
		} \
		printf("\n"); \
		usleep(10000); \
	} \
	memory_destroy(&memory);
