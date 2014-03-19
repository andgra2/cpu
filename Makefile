all: 	counter

counter: counter.c cpu.c cpu.h memory.c memory.h
	gcc -o counter counter.c memory.c cpu.c

