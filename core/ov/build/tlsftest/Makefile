

INCLUDE := -I../../include/ -I/usr/include/valgrind
CFLAG := -DOV_SYSTEM_LINUX=1 

all: objects
	gcc tlsf.o test.o -o test

objects:
	gcc -c ${CFLAG} ${INCLUDE} ../../source/libov/tlsf.c -o tlsf.o
	gcc -c ${CFLAG} ${INCLUDE} test.c -o test.o
	
#%.o: %.c
#	gcc -I${INCLUDE} $< -o $@

clean:
	@rm *.o test