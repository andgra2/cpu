all: counter grid

clean:
	rm counter
	rm grid

counter: counter.c cpu.c cpu.h memory.c memory.h asm.h
	gcc -o counter counter.c memory.c cpu.c

grid: grid.c cpu.c cpu.h memory.c memory.h asm.h
	gcc -o grid grid.c memory.c cpu.c

