CXX = g++

CXX_EXTRA_FLAGS = -I../../include

CXX_PLATFORM_FLAGS = -DPLT_SIMULATE_BOOL=0 -DPLT_SIMULATE_RTTI=1

CXX_FLAGS = -g -Wall -DPLT_DEBUG_SLOW=0
#CXX_FLAGS = -O -Wall -DNDEBUG 
#
#
#
CXX_COMPILE = $(CXX) $(CXX_EXTRA_FLAGS) $(CXX_PLATFORM_FLAGS) $(CXX_FLAGS) -c

CXX_LINK = g++ -lstdc++

VPATH = ../../tests ../../src

.SUFFIXES:

.SUFFIXES: .cpp .o .a .s

.cpp.o:
	$(CXX_COMPILE) -o $@ $<

.cpp.s:
	$(CXX_COMPILE) -S -o $@ $<

CXX_LIBPLT_SOURCES = \
	debug.cpp \
	rtti.cpp \
	list.cpp \
	priorityqueue.cpp \
	string.cpp \
	hashtable.cpp \
	key.cpp
#	rbtreedict.cpp \

CXX_LIBPLT_OBJECTS = \
	debug.o \
	rtti.o \
	list.o \
	priorityqueue.o \
	string.o \
	hashtable.o \
	key.o
#	rbtreedict.o \

CXX_TEST_SOURCES = \
	thandle.cpp\
	tcomparable.cpp\
	trtti.cpp \
	tlist.cpp \
	ipriorityqueue.cpp tpriorityqueue.cpp \
	tstring.cpp \
	ttime.cpp \
	thashtable.cpp \
	thtname.cpp \
	tarray.cpp
#	trbtreedict.cpp

CXX_SOURCES = $(CXX_LIBPLT_SOURCES) $(CXX_TEST_SOURCES)

TESTS = trtti tlist tpriorityqueue tstring ttime thashtable tarray\
	thtname tcomparable thandle

all : libplt.a $(TESTS)


libplt.a: $(CXX_LIBPLT_OBJECTS)
	ar r $@ $?
	ranlib $@
	
thandle : thandle.o libplt.a
	$(CXX_LINK) -o $@ $^

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

ttime :	ttime.o libplt.a
	$(CXX_LINK) -o $@ $^

trbtreedict :	trbtreedict.o libplt.a
	$(CXX_LINK) -o $@ $^

thashtable :	thashtable.o libplt.a
	$(CXX_LINK) -o $@ $^

thtname :	thtname.o libplt.a
	$(CXX_LINK) -o $@ $^

tcomparable :	tcomparable.o
	$(CXX_LINK) -o $@ $^

tarray :	tarray.o libplt.a
	$(CXX_LINK) -o $@ $^

clean :
	rm -f *.o core

mrproper :	clean
	rm -f libplt.a .depend $(TESTS)

depend : $(CXX_SOURCES)
	$(CXX_COMPILE) -M $^ > .depend

.depend:
	touch .depend

include .depend
