PLATFORM = hpux
PLT_DIR = ../../../libplt

CXX = g++

CXX_EXTRA_FLAGS = -I../../include -I$(PLT_DIR)/include

CXX_PLATFORM_FLAGS = -DPLT_HAS_BOOL=1

CXX_FLAGS = -g -Wall
#
#
#
CXX_COMPILE = $(CXX) $(CXX_EXTRA_FLAGS) $(CXX_PLATFORM_FLAGS) $(CXX_FLAGS) -c

CXX_LINK = g++ 

.SUFFIXES:

.SUFFIXES: .cpp .o

.cpp.o:
	$(CXX_COMPILE) -o $@ $<

CXX_SOURCES = tvalue.cpp value.cpp txdr2.cpp xdr.cpp

LIBPLT = ../../../libplt/build/hpux/libplt.a

VPATH = ../../src ../../tests

all:	tvalue txdr

value.o : value.cpp

value.s : value.cpp
	$(CXX_COMPILE) -S -o $@ $<

tvalue :  tvalue.o value.o
	$(CXX_LINK) -o $@ $^ $(LIBPLT)

txdr2 :  txdr2.o xdr.o
	$(CXX_LINK) -o $@ $^ $(LIBPLT)

depend : $(CXX_SOURCES)
	$(CXX_COMPILE) -M $^ >.depend

.depend :
	touch .depend

include .depend


