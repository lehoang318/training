all: sys_info

sys_info:
	gcc -c sys_info.c
	gcc -o sys_info sys_info.o

process_info:
	gcc -c process_info.c
	gcc -o process_info process_info.o

mul:
	gcc -c mul.c
	gcc -o mul mul.o -Wl,-Map=mul.map,--cref

clean:
	rm -f *.o *.map process_info sys_info mul
