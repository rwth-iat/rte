CXX = g++

CXX_EXTRA_FLAGS = -I../../include

CXX_PLATFORM_FLAGS = -DPLT_HAS_BOOL=1

CXX_FLAGS = -g -Wall -DPLT_DEBUG_SLOW=0
#CXX_FLAGS = -O -Wall -DNDEBUG 
#
#
#
CXX_COMPILE = $(CXX) $(CXX_EXTRA_FLAGS) $(CXX_PLATFORM_FLAGS) $(CXX_FLAGS) -c

CXX_LINK = g++ 

VPATH = ../../tests ../../src

.SUFFIXES:

.SUFFIXES: .cpp .o .a

.cpp.o:
	$(CXX_COMPILE) -o $@ $<


CXX_LIBPLT_SOURCES = \
	debug.cpp \
	rtti.cpp \
	list.cpp \
	priorityqueue.cpp \
	string.cpp \
	rbtreedict.cpp \
	hashtable.cpp

CXX_LIBPLT_OBJECTS = \
	debug.o \
	rtti.o \
	list.o \
	priorityqueue.o \
	string.o \
	rbtreedict.o \
	hashtable.o

CXX_TEST_SOURCES = \
	trtti.cpp \
	tlist.cpp \
	ipriorityqueue.cpp tpriorityqueue.cpp \
	tstring.cpp \
	ttime.cpp \
	trbtreedict.cpp \
	thashtable.cpp

CXX_SOURCE = $(CXX_LIBPLT_SOURCES) $(CXX_TEST_SOURCES)

all : libplt.a trtti tlist tpriorityqueue tstring ttime thashtable


libplt.a: $(CXX_LIBPLT_OBJECTS)
	ar r $@ $?
	ranlib $@
	
tlist : tlist.o libplt.a
	$(CXX_LINK) -o $@ $^

trtti :	trtti.o libplt.a
	$(CXX_LINK) -o $@ $^

tpriorityqueue : ipriorityqueue.o tpriorityqueue.o libplt.a
	$(CXX_LINK) -o $@ $^

tstring : tstring.o libplt.a
	$(CXX_LINK) -o $@ $^

trtti.i : trtti.cpp ../../include/plt/rtti.hpp
	$(CXX_COMPILE) -E -o $@ $<

ttime :	ttime.o
	$(CXX_LINK) -o $@ $^

trbtreedict :	trbtreedict.o libplt.a

thashtable :	thashtable.o libplt.a
	$(CXX_LINK) -o $@ $^


clean :
	rm -f *.o

depend : $(CXX_SOURCES)
	$(CXX_COMPILE) -M $^ > .depend


include .depend
