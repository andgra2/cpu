#include "cpu.h"
#include "memory.h"
#include "asm.h"

int main() {
	ASM_INIT();

	// 0 1 2
	COPY_IM('0', code+4);
	// 3 4
	PRINT_I('X');
	// 5 6
//	PRINT_I('\n');
	// 7 8 9
	ADD_IMM(1, code+4, code+4);
	// 10 11
	AND_IMM(0x7F, code+4, code+4);
	// 12 13
	JUMP_I(code+3);
	// 14
	TERMINATE();

	ASM_RUN()
}