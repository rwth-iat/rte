# generic unix makefile

PLT_DIR = ../../../plt
O = .o
EXE = .exe
A = .a

CXX = g++

CXX_EXTRA_FLAGS = -I . -I../../include -I$(PLT_DIR)/include -fno-implicit-templates

CXX_COMPILE = $(CXX) $(CXX_EXTRA_FLAGS) $(CXX_PLATFORM_FLAGS) $(CXX_FLAGS) -c

CXX_LINK = MAKE=$(MAKE) perl $(PLT_DIR)/build/templ.pl g++

CXX_LIBS = -lstdc++

.SUFFIXES:

.SUFFIXES: .cpp .o  .exe .i

.PRECIOUS: .o .a

.cpp.o:
	$(CXX_COMPILE) -o $@ $<

.o.exe:
	$(CXX_LINK) -o $@ $^ $(LIBPLT) -lstdc++

.cpp.i:
	$(CXX_COMPILE) -E > $@ $<

LIBPLT = $(PLT_DIR)/build/$(PLATFORM)/libplt.a

LIBKS = /tmp/$(USER)-libks.a

VPATH = ../../src ../../tests

include ../generic.mk

$(LIBKS): $(LIBKS_OBJECTS)
	ar r $@ $?

depend : $(CXX_SOURCES)
	$(CXX_COMPILE) -MM $^ >.depend

.depend :
	touch .depend

include .depend

clean :
	rm -f *.o core

mrproper :	clean
	rm -f libks.a .depend $(TESTS)
	for i in t*_inst.h ; do echo > $$i ; done










