all: counter grid

clean:
	rm counter
	rm grid

counter: counter.c cpu.c cpu.h memory.c memory.h asm.h
	gcc -o counter counter.c memory.c cpu.c

grid: grid.c cpu.c cpu.h memory.c memory.h asm.h
	gcc -o grid grid.c memory.c cpu.c
	
rikardtest: rikard_test.c mem.h mem.c cpu.h cpu.c
	gcc --std=c99 -Wall -Werror -o rikard_test rikard_test.c mem.h mem.c cpu.h cpu.c vm.h vm.c mem_serialization.h mem_serialization.c