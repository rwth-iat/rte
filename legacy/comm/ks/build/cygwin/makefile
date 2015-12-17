RPCINCLUDEPATH = ../../../../oncrpc

MINGWLIBPATH = /cygdrive/c/MinGW32/lib

MINGWINCLUDEPATH = /cygdrive/c/MinGW32/include

LIBRPC = ../../../../oncrpc/bin/oncrpc.a

LD_FLAGS = -L$(MINGWLIBPATH)

#LD_FLAGS = 



# Platform specific part for cygwin



RANLIB = touch

#CXX_FLAGS = -g -Wall -DPLT_DEBUG_INVARIANTS=0 -DPLT_DEBUG_NEW=1 -DPLT_DEBUG_NEW_REPORT=1

CXX_FLAGS = -g -Wall -DNDEBUG -DPLT_USE_BUFFERED_STREAMS=1 -I$(RPCINCLUDEPATH)

#CXX_FLAGS = -O3 -Wall -DNDEBUG -I$(RPCINCLUDEPATH)



# The following set of flags compiles with optimization and no PLT debugging

# helpers (no alloc tracker, no debugging messages)

#CXX_FLAGS = -O2 -Wall -DNDEBUG



# Enable next set of flags when using the new server-side dynamic XDR

# streams, which buffer client i/o. (Prevents servers from blocking in i/o).

#CXX_FLAGS = -O2 -Wall -DNDEBUG -DPLT_USE_BUFFERED_STREAMS=1



# Enable next set for usual debugging turn-around.

#CXX_FLAGS = -g -Wall -DNDEBUG -DPLT_USE_BUFFERED_STREAMS=1



CXX_PLATFORM_FLAGS = -DPLT_SYSTEM_NT=1 -DFD_SETSIZE=128 -DWIN32 -I$(MINGWINCLUDEPATH)

#CXX_PLATFORM_FLAGS = -DPLT_SYSTEM_NT=1 -DFD_SETSIZE=128 -DWIN32

PLATFORM = cygwin



CXX_PLATFORM_LIBS = $(LIBRPC) -lwsock32



#don't: include ../unix.mk



LIBKS_NT_OBJECTS = ntservice$(O) w95service$(O)



# generic unix makefile



PLT_DIR = ../../../plt

O = .o

EXE = .exe

A = .a



CXX = g++



CXX_EXTRA_FLAGS = -I . -I../../include -I$(PLT_DIR)/include -fno-implicit-templates


CXX_COMPILE = $(CXX) $(CXX_EXTRA_FLAGS) $(CXX_PLATFORM_FLAGS) $(CXX_FLAGS) -c



CXX_LINK = MAKE=$(MAKE) perl $(PLT_DIR)/build/templ.pl g++ $(LD_FLAGS)



CXX_LIBS =

#CXX_LIBS = -lstdc++



.SUFFIXES:



.SUFFIXES: .cpp .o .exe .i



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



all: $(LIBKS) $(LIBKSSVR) $(LIBKSCLN)



$(LIBKS): $(LIBKS_OBJECTS)

	ar r $@ $?



$(LIBKSSVR): $(LIBKSSVR_OBJECTS)

	ar r $@ $?



$(LIBKSCLN): $(LIBKSCLN_OBJECTS)

	ar r $@ $?





depend : $(CXX_SOURCES)

	$(CXX_COMPILE) -MM $^ > .depend





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























