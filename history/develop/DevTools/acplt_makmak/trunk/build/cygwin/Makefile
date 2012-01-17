
#   Plattform
#   ---------

COMPILER = GNU 
_OBJ=.o
_EXE=.exe
PLATFORM = cygwin

DEFINES = -DOV_SYSTEM_NT -DPLT_SYSTEM_NT


OPT_INCLUDES = -I ../../../../oncrpc/

#	Compiler
#	--------

CC		= gcc
CC_FLAGS	= -g -std=c99 -Wall -O2 -shared -mno-cygwin -c
CC_DEFINES	= $(DEFINES)
LD 		= gcc -o

include ../gcc.mk

