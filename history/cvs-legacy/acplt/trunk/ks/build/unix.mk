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
	$(CXX_LINK) -o $@ $^ $(LIBPLT) $(CXX_PLATFORM_LIBS) $(CXX_LIBS)

.cpp.i:
	$(CXX_COMPILE) -E > $@ $<

LIBPLT = $(PLT_DIR)/build/$(PLATFORM)/libplt.a

LIBKS = libks.a

VPATH = ../../src ../../examples

include ../generic.mk

all: manager

examples: tmanager.exe tserver.exe tsclient.exe ttree.exe

$(LIBKS): $(LIBKS_OBJECTS)
	ar r $@ $?

../depend.nt : $(CXX_SOURCES) unix_manager.cpp
	$(CXX_COMPILE) -MM $^ > .depend
	perl $(PLT_DIR)/build/depend.pl .depend > $@

depend : ../depend.nt

.depend :
	touch .depend
	rm -f ../depend.nt

include .depend

manager:	unix_manager.o $(LIBKS)
	$(CXX_LINK) -o $@ $^ $(LIBPLT) $(CXX_PLATFORM_LIBS) $(CXX_LIBS)

clean :
	rm -f *.o core

mrproper :	clean
	rm -f libks.a .depend $(TESTS)
	for i in t*_inst.h ; do echo > $$i ; done
