# generic unix makefile

PLT_DIR = ../../../plt

CXX = g++

CXX_EXTRA_FLAGS = -I . -I../../include -I$(PLT_DIR)/include -fno-implicit-templates

CXX_COMPILE = $(CXX) $(CXX_EXTRA_FLAGS) $(CXX_PLATFORM_FLAGS) $(CXX_FLAGS) -c

CXX_LINK = perl $(PLT_DIR)/build/templ.pl g++ -lstdc++

.SUFFIXES:

.SUFFIXES: .cpp .o .i

.cpp.o:
	$(CXX_COMPILE) -o $@ $<

.cpp.i:
	$(CXX_COMPILE) -E > $@ $<

CXX_LIBKS_SOURCES = \
	xdr.cpp \
	array.cpp \
	string.cpp \
	register.cpp \
	result.cpp \
	avticket.cpp \
	templates.cpp

CXX_LIBKS_OBJECTS = \
	xdr.o \
	array.o \
	string.o \
	avticket.o \
	result.o \
	templates.o

#	register.o \

CXX_TEST_SOURCES = \
	txdr.cpp \
	txdr2.cpp \
	tregister.cpp \
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

tvalue :  tvalue.o value.o
	$(CXX_LINK) -o $@ $^ $(LIBPLT)

txdr2 :  txdr2.o $(LIBKS) $(LIBPLT)
	$(CXX_LINK) -o $@ $^ 

tarray : tarray.o $(LIBKS) $(LIBPLT)
	$(CXX_LINK) -o $@ $^

txdr :  txdr.o $(LIBKS) $(LIBPLT)
	$(CXX_LINK) -o $@ $^

tregister :	tregister.o $(LIBKS) $(LIBPLT)
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










