PLATFORM = hpux
PLT_DIR = ../../../plt

CXX = g++

CXX_EXTRA_FLAGS = -I../../include -I$(PLT_DIR)/include

CXX_PLATFORM_FLAGS = -DPLT_HAS_BOOL=1

CXX_FLAGS = -g -Wall

# -O3 -fno-inline -Wall
#
#
#
CXX_COMPILE = $(CXX) $(CXX_EXTRA_FLAGS) $(CXX_PLATFORM_FLAGS) $(CXX_FLAGS) -c

CXX_LINK = g++ 

.SUFFIXES:

.SUFFIXES: .cpp .o

.cpp.o:
	$(CXX_COMPILE) -o $@ $<

CXX_SOURCES = txdr2.cpp txdr.cpp string.cpp tregister.cpp register.cpp
#xdr.cpp
# tvalue.cpp value.cpp

LIBPLT = $(PLT_DIR)/build/hpux/libplt.a

VPATH = ../../src ../../tests

all:	tvalue txdr

value.s : value.cpp
	$(CXX_COMPILE) -S -o $@ $<

tvalue :  tvalue.o value.o
	$(CXX_LINK) -o $@ $^ $(LIBPLT)

txdr2 :  txdr2.o $(LIBPLT)
	$(CXX_LINK) -o $@ $^ 

txdr :  txdr.o string.o $(LIBPLT)
	$(CXX_LINK) -o $@ $^

tregister : 	tregister.o register.o string.o $(LIBPLT)	
		$(CXX_LINK) -o $@ $^     

depend : $(CXX_SOURCES)
	$(CXX_COMPILE) -M $^ >.depend

.depend :
	touch .depend

include .depend


