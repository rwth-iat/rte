# unix generic part

CXX = g++

CXX_EXTRA_FLAGS = -I. -I../../include  -fno-implicit-templates

#CXX_FLAGS = -g -Wall -DPLT_DEBUG_PEDANTIC=0

#CXX_FLAGS = -O -Wall -DNDEBUG 
#
#
#

CXX_COMPILE = $(CXX) $(CXX_EXTRA_FLAGS) $(CXX_PLATFORM_FLAGS) $(CXX_FLAGS) -c

CXX_LINK = perl ../templ.pl g++ -lstdc++

VPATH = ../../tests ../../src

.SUFFIXES:

.SUFFIXES: .cpp .o .a .s

.cpp.o:
	$(CXX_COMPILE) -o $@ $<

.cpp.s:
	$(CXX_COMPILE) -S -o $@ $<

CXX_LIBPLT_SOURCES = \
	debug.cpp \
	handle.cpp \
	hashtable.cpp \
	list.cpp \
	log.cpp \
	priorityqueue.cpp \
	rtti.cpp \
	string.cpp

CXX_LIBPLT_OBJECTS = \
	debug.o \
	handle.o \
	hashtable.o \
	list.o \
	log.o \
	priorityqueue.o \
	rtti.o \
	string.o

CXX_TEST_SOURCES = \
	tarray.cpp \
	thandle.cpp\
	thashtable.cpp \
	thtname.cpp \
	tlist.cpp \
	tlog.cpp \
	tpriorityqueue.cpp \
	trtti.cpp \
	tstring.cpp \
	ttime.cpp

CXX_SOURCES = $(CXX_LIBPLT_SOURCES) $(CXX_TEST_SOURCES)

TESTS = trtti tlist tpriorityqueue tstring ttime thashtable tarray\
	thtname thandle tlog

all : libplt.a

tests : $(TESTS)

libplt.a: $(CXX_LIBPLT_OBJECTS)
	ar r $@ $?
	ranlib $@

thandle : thandle.o libplt.a
	$(CXX_LINK) -o $@ $^

tlist : tlist.o libplt.a
	$(CXX_LINK) -o $@ $^

tlog : tlog.o libplt.a
	$(CXX_LINK) -o $@ $^

trtti :	trtti.o libplt.a
	$(CXX_LINK) -o $@ $^

tpriorityqueue : tpriorityqueue.o libplt.a
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

tcomparable :	tcomparable.o libplt.a
	$(CXX_LINK) -o $@ $^

tarray :	tarray.o libplt.a
	$(CXX_LINK) -o $@ $^

clean :
	rm -f *.o core

mrproper :	clean
	rm -f libplt.a .depend $(TESTS)
	for i in t*_inst.h ; do echo > $$i ; done


depend : $(CXX_SOURCES)
	$(CXX_COMPILE) -M $^ > .depend

.depend:
	touch .depend

include .depend
