CXX = g++

CXX_EXTRA_FLAGS = -I../../include

CXX_PLATFORM_FLAGS = -DPLT_HAS_BOOL=1

CXX_FLAGS = -g -Wall -fno-access-control
#CXX_FLAGS = -O -Wall -DNDEBUG 
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

CXX_SOURCES = \
	rtti.cpp trtti.cpp \
	list.cpp tlist.cpp \
	priorityqueue.cpp ipriorityqueue.cpp tpriorityqueue.cpp \
	string.cpp tstring.cpp \
	ttime.cpp \
	rbtreedict.cpp trbtreedict.cpp

all : trtti tlist tpriorityqueue tstring ttime trbtreedict

tlist : tlist.o list.o
	$(CXX_LINK) -o $@ $^

trtti :	rtti.o trtti.o
	$(CXX_LINK) -o $@ $^

tpriorityqueue : ipriorityqueue.o tpriorityqueue.o
	$(CXX_LINK) -o $@ $^

tstring : string.o tstring.o
	$(CXX_LINK) -o $@ $^

trtti.i : trtti.cpp ../../include/plt/rtti.hpp
	$(CXX_COMPILE) -E -o $@ $<

ttime :	ttime.o
	$(CXX_LINK) -o $@ $^

trbtreedict :	trbtreedict.o rbtreedict.o 
	$(CXX_LINK) -o $@ $^



depend : $(CXX_SOURCES)
	$(CXX_COMPILE) -M $^ > .depend


include .depend
