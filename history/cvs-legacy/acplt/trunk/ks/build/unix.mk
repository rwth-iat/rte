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

LIBKSSVR = libkssvr.a

LIBKSCLN = libkscln.a

VPATH = ../../src ../../examples

include ../generic.mk

all: $(LIBKS) $(LIBKSSVR) $(LIBKSCLN) manager

$(LIBKS): $(LIBKS_OBJECTS)
	ar r $@ $?

$(LIBKSSVR): $(LIBKSSVR_OBJECTS)
	ar r $@ $?

$(LIBKSCLN): $(LIBKSCLN_OBJECTS)
	ar r $@ $?

../depend.nt ../depend.vms : $(CXX_SOURCES) unix_manager.cpp
	$(CXX_COMPILE) -MM $^ > .depend
	perl $(PLT_DIR)/build/depend.pl .depend > ../depend.nt
	perl $(PLT_DIR)/build/dependvms.pl .depend > ../depend.vms

depend : ../depend.nt ../depend.vms

.depend :
	touch .depend
	rm -f ../depend.nt

include .depend

manager:	unix_manager.o $(LIBKSSVR) $(LIBKS)
	$(CXX_LINK) -o $@ $^ $(LIBPLT) $(CXX_PLATFORM_LIBS) $(CXX_LIBS)

clean :
	rm -f *.o core

mrproper :	clean
	rm -f \
		$(LIBKS) $(LIBKSSVR) $(LIBKSCLN) \
		.depend \
		manager $(TESTS)
	for i in t*_inst.h ; do echo > $$i ; done


