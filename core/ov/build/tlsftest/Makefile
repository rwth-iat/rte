

INCLUDE := -I../../include/ -I/usr/include/valgrind
CFLAG := -DOV_SYSTEM_LINUX=1 -DCOMPILE=1

all: objects
	gcc -g tlsf.o test.o -o test

objects:
	gcc -g -c ${CFLAG} ${INCLUDE} ../../source/libov/tlsf.c -o tlsf.o
	gcc -g -c ${CFLAG} ${INCLUDE} test.c -o test.o
	
#%.o: %.c
#	gcc -I${INCLUDE} $< -o $@

clean:
	@rm *.o test