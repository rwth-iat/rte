# generic unix makefile

PLT_DIR = ../../../plt

CXX = g++

CXX_EXTRA_FLAGS = -I . -I../../include -I$(PLT_DIR)/include -fno-implicit-templates -DDEBUG

CXX_COMPILE = $(CXX) $(CXX_EXTRA_FLAGS) $(CXX_PLATFORM_FLAGS) $(CXX_FLAGS) -c

CXX_LINK = MAKE=$(MAKE) perl $(PLT_DIR)/build/templ.pl g++ -lstdc++

.SUFFIXES:

.SUFFIXES: .cpp .o .i

.cpp.o:
	$(CXX_COMPILE) -o $@ $<

.cpp.i:
	$(CXX_COMPILE) -E > $@ $<

CXX_LIBKS_SOURCES = \
	array.cpp \
	avticket.cpp \
	manager2.cpp \
	props.cpp \
	register.cpp \
	serviceparams.cpp \
	string.cpp \
	svrbase.cpp \
	templates.cpp \
	time.cpp \
	value.cpp \
	xdr.cpp

#	objecttree.cpp


CXX_LIBKS_OBJECTS = \
	array.o \
	avticket.o \
	manager2.o \
	props.o \
	register.o \
	serviceparams.o \
	string.o \
	svrbase.o \
	templates.o \
	time.o \
	value.o \
	xdr.o
#	objecttree.o \

CXX_TEST_SOURCES = \
	txdr.cpp \
	txdr2.cpp \
	tregister.cpp \
	tvalue.cpp \
	tvalue2.cpp \
	tvalue3.cpp \
	tserviceparams.cpp \
	tserviceparams2.cpp \
	tserviceparams3.cpp \
	tserviceparams4.cpp \
	tmanager2 \
	tarray.cpp

TESTS = txdr txdr2 tarray
#tregister

CXX_SOURCES = $(CXX_LIBKS_SOURCES) $(CXX_TEST_SOURCES)

LIBPLT = $(PLT_DIR)/build/$(PLATFORM)/libplt.a

LIBKS = libks.a

VPATH = ../../src ../../tests

all:	$(LIBKS)

tests:	$(TESTS)

libks.a :	$(CXX_LIBKS_OBJECTS)
	ar r $@ $?
	ranlib $@

value.s : value.cpp
	$(CXX_COMPILE) -S -o $@ $<

txdr2 :  txdr2.o $(LIBKS) $(LIBPLT)
	$(CXX_LINK) -o $@ $^ 

tarray : tarray.o $(LIBKS) $(LIBPLT)
	$(CXX_LINK) -o $@ $^

txdr :  txdr.o $(LIBKS) $(LIBPLT)
	$(CXX_LINK) -o $@ $^

tvalue :  tvalue.o $(LIBKS) $(LIBPLT)
	$(CXX_LINK) -o $@ $^

tvalue2 : tvalue2.o $(LIBKS) $(LIBPLT)
	$(CXX_LINK) -o $@ $^

tvalue3 : tvalue3.o $(LIBKS) $(LIBPLT)
	$(CXX_LINK) -o $@ $^

tregister :	tregister.o $(LIBKS) $(LIBPLT)
		$(CXX_LINK) -o $@ $^

tcurrprops : tcurrprops.o $(LIBKS) $(LIBPLT)
	$(CXX_LINK) -o $@ $^

tprojprops : tprojprops.o $(LIBKS) $(LIBPLT)
	$(CXX_LINK) -o $@ $^     

tserviceparams : tserviceparams.o $(LIBKS) $(LIBPLT)
	$(CXX_LINK) -o $@ $^

tserviceparams2 : tserviceparams2.o $(LIBKS) $(LIBPLT)
	$(CXX_LINK) -o $@ $^

tserviceparams3 : tserviceparams3.o $(LIBKS) $(LIBPLT)
	$(CXX_LINK) -o $@ $^

tserviceparams4 : tserviceparams4.o $(LIBKS) $(LIBPLT)
	$(CXX_LINK) -o $@ $^

tmanager2 : tmanager2.o $(LIBKS) $(LIBPLT)
	$(CXX_LINK) -o $@ $^

depend : $(CXX_SOURCES)
	$(CXX_COMPILE) -M $^ >.depend

.depend :
	touch .depend

include .depend

clean :
	rm -f *.o core

mrproper :	clean
	rm -f libks.a .depend $(TESTS)
	for i in t*_inst.h ; do echo > $$i ; done










