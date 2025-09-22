all: sys_info

sys_info: sys_info.c
	gcc -Wall -c sys_info.c
	gcc -o sys_info.bin sys_info.o

process_info: process_info.c
	gcc -Wall -c process_info.c
	gcc -o process_info.bin process_info.o

memory: memory.c
	gcc -Wall -c memory.c
	gcc -o memory.bin memory.o

threads: tmain.c tchild.c common.c
	gcc -Wall -g -D _GNU_SOURCE -c tmain.c tchild.c common.c
	gcc -o threads.bin tmain.o tchild.o common.o -lpthread

clean:
	rm -f *.o *.map *.bin
