CXX = g++

CXX_EXTRA_FLAGS = -I../../include

CXX_PLATFORM_FLAGS = -DPLT_HAS_BOOL=1

CXX_FLAGS = -g -Wall
#
#
#
CXX_COMPILE = $(CXX) $(CXX_EXTRA_FLAGS) $(CXX_PLATFORM_FLAGS) $(CXX_FLAGS) -c

CXX_LINK = g++ 

VPATH = ../../tests ../../src

.SUFFIXES:

.SUFFIXES: .cpp .o

.cpp.o:
	$(CXX_COMPILE) -o $@ $<

CXX_SOURCES = rtti.cpp trtti.cpp
all : trtti

trtti :	rtti.o trtti.o
	$(CXX_LINK) -o $@ $^

trtti.i : trtti.cpp ../../include/plt/rtti.hpp
	$(CXX_COMPILE) -E -o $@ $<

depend : $(CXX_SOURCES)
	$(CXX_COMPILE) -M -o $@ $^


include depend
