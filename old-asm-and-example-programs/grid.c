#include "cpu.h"
#include "memory.h"
#include "asm.h"

int main() {
	ASM_INIT();

	#define Y code+125
	#define X code+126
	#define T1 code+130
	#define T2 code+131
	#define T3 code+132

	COPY_IM(40, Y);
	#define YLOOP code+3

	COPY_IM(130, X);
	#define XLOOP code+6

	AND_MIM(X, 255, T2);
	AND_MIM(Y, 255, T3);
	XOR_MMM(T2, T3, T1);
	AND_MIM(T1, 63, T1);
	ADD_MIM(T1, 'a', T1);

	PRINT_M(T1);

	SUB_MIM(X, 1, X);
	JUMP_COND_MI(X, XLOOP);

	PRINT_I('\n');

	SUB_MIM(Y, 1, Y);
	JUMP_COND_MI(Y, YLOOP);

	TERMINATE();

	ASM_RUN();
	// ASM_RUN_DEBUG();
}