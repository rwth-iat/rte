# generic unix makefile

PLT_DIR = ../../../plt
O = .o
EXE = .exe
A = .a

# 32/64 bit architecture
ifdef OV_ARCH_BITWIDTH
OV_ARCH_BITWIDTH_CFLAGS		=	-m$(OV_ARCH_BITWIDTH)
OV_ARCH_BITWIDTH_LDFLAGS	=	$(OV_ARCH_BITWIDTH_CFLAGS)
else
OV_ARCH_BITWIDTH_CFLAGS		=	-m32
OV_ARCH_BITWIDTH_LDFLAGS	=	$(OV_ARCH_BITWIDTH_CFLAGS)
endif

GCC_BIN_PREFIX	= 

CXX = $(GCC_BIN_PREFIX)g++ $(OV_ARCH_BITWIDTH_CFLAGS)

CXX_EXTRA_FLAGS = -I . -I../../include -I$(PLT_DIR)/include -fno-implicit-templates

CXX_COMPILE = $(CXX) $(CXX_EXTRA_FLAGS) $(CXX_PLATFORM_FLAGS) $(CXX_FLAGS) -c

CXX_LINK = MAKE=$(MAKE) perl $(PLT_DIR)/build/templ.pl $(GCC_BIN_PREFIX)g++ $(OV_ARCH_BITWIDTH_LDFLAGS) $(LD_FLAGS)

CXX_LIBS = -lstdc++

.SUFFIXES:
.SUFFIXES: .cpp .o .exe .i

.PRECIOUS: .o .a

.cpp.o:
	$(CXX_COMPILE) -o $@ $<

.o.exe:
	$(CXX_LINK) -o $@ $^ $(LIBPLT) $(CXX_PLATFORM_LIBS) $(CXX_LIBS)
	$(GCC_BIN_PREFIX)strip --strip-debug $@

.cpp.i:
	$(CXX_COMPILE) -E > $@ $<

LIBPLT = $(PLT_DIR)/build/$(PLATFORM)/libplt.a

LIBKS = libks.a

LIBKSSVR = libkssvr.a

LIBKSCLN = libkscln.a

VPATH = ../../src ../../examples

include ../generic.mk

$(LIBKS): $(LIBKS_OBJECTS)
	$(GCC_BIN_PREFIX)ar r $@ $?
	$(GCC_BIN_PREFIX)strip --strip-debug $(LIBKS)

$(LIBKSSVR): $(LIBKSSVR_OBJECTS)
	$(GCC_BIN_PREFIX)ar r $@ $?
	$(GCC_BIN_PREFIX)strip --strip-debug $(LIBKSSVR)

$(LIBKSCLN): $(LIBKSCLN_OBJECTS)
	$(GCC_BIN_PREFIX)ar r $@ $?
	$(GCC_BIN_PREFIX)strip --strip-debug $(LIBKSCLN)

depend : $(CXX_SOURCES) unix_manager.cpp
	$(CXX_COMPILE) -MM $^ > .depend
	perl $(PLT_DIR)/build/depend.pl .depend > ../depend.nt
	perl $(PLT_DIR)/build/dependvms.pl .depend > ../depend.vms


.depend :
	touch .depend

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

