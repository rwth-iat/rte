PLATFORM = hpux
PLT_DIR = ../../../plt

CXX = g++

CXX_EXTRA_FLAGS = -I../../include -I$(PLT_DIR)/include

CXX_PLATFORM_FLAGS = -DPLT_SYSTEM_HPUX=1

CXX_FLAGS = -g -Wall

# -O3 -fno-inline -Wall
#
#
#
CXX_COMPILE = $(CXX) $(CXX_EXTRA_FLAGS) $(CXX_PLATFORM_FLAGS) $(CXX_FLAGS) -c

CXX_LINK = g++ 

.SUFFIXES:

.SUFFIXES: .cpp .o .i

.cpp.o:
	$(CXX_COMPILE) -o $@ $<

.cpp.i:
	$(CXX_COMPILE) -E > $@ $<

CXX_SOURCES = txdr2.cpp txdr.cpp xdr.cpp string.cpp tregister.cpp\
	avticket.cpp register.cpp

#xdr.cpp
# tvalue.cpp value.cpp

LIBPLT = $(PLT_DIR)/build/hpux/libplt.a

VPATH = ../../src ../../tests

all:	tvalue txdr

value.s : value.cpp
	$(CXX_COMPILE) -S -o $@ $<

tvalue :  tvalue.o value.o
	$(CXX_LINK) -o $@ $^ $(LIBPLT)

txdr2 :  txdr2.o xdr.o string.o $(LIBPLT)
	$(CXX_LINK) -o $@ $^ 

txdr :  txdr.o string.o $(LIBPLT)
	$(CXX_LINK) -o $@ $^

tavticket :  avticket.o $(LIBPLT)
	$(CXX_LINK) -o $@ $^

tregister :	register.o tregister.o string.o $(LIBPLT)
		$(CXX_LINK) -o $@ $^     

depend : $(CXX_SOURCES)
	$(CXX_COMPILE) -M $^ >.depend

.depend :
	touch .depend

include .depend











