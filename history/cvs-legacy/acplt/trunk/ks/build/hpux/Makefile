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

.SUFFIXES:

.SUFFIXES: .cpp .o

.cpp.o:
	$(CXX_COMPILE) -o $@ $<

CXX_SOURCES = value.cpp tvalue.cpp

VPATH = ../../src ../../tests

value.o : value.cpp

value.s : value.cpp
	$(CXX_COMPILE) -S -o $@ $<

tvalue : value.o tvalue.o
	$(CXX) -o $@ $^

depend : $(CXX_SOURCES)
	$(CXX_COMPILE) -o depend -M $^

include depend