all: sys_info

sys_info: sys_info.c
	gcc -c sys_info.c
	gcc -o sys_info sys_info.o

process_info: process_info.c
	gcc -c process_info.c
	gcc -o process_info process_info.o

memory: memory.c
	gcc -c memory.c
	gcc -o memory memory.o -Wl,-Map=memory.map,--cref

clean:
	rm -f *.o *.map process_info sys_info mul memory
