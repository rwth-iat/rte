CXX = g++

CXX_EXTRA_FLAGS = -I../../include

CXX_PLATFORM_FLAGS = -DPLT_HAS_BOOL=1

CXX_FLAGS = -g -Wall
#
#
#
CXX_COMPILE = $(CXX) $(CXX_EXTRA_FLAGS) $(CXX_PLATFORM_FLAGS) $(CXX_FLAGS) -c

VPATH = ../../tests ../../src

.SUFFIXES:

.SUFFIXES: .cpp .o

.cpp.o:
	$(CXX_COMPILE) -o $@ $<


CXX_SOURCES = \
	list.cpp tlist.cpp \
	handle.cpp \
	rtti.cpp trtti.cpp
		
all:	tlist thandle

thandle:	thandle.o
	$(CXX) -o thandle thandle.o

tlist:		tlist.o list.o
	$(CXX) -o tlist tlist.o list.o

trtti:		trtti.o rtti.o
	$(CXX) -o $@ $^

depend:	$(CXX_SOURCES)
	$(CXX_COMPILE) -M $^ >depend

include depend



