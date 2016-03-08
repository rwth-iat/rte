### -*-makefile-*-
###
### unix generic part
###

### Filename conventions
O=.o
A=.a

# The following is not typically unix, but handy.
EXE=.exe

SRCDIR = ../../src/

### Compiler
CXX = $(PREFIX)g++

#CXX_EXTRA_FLAGS = -I. -I../../include 
CXX_EXTRA_FLAGS = -I. -I../../include  -fno-implicit-templates

#CXX_FLAGS = -g -Wall -DPLT_DEBUG_PEDANTIC=0

#CXX_FLAGS = -O -Wall -DNDEBUG 
#
#
#

CXX_COMPILE = $(CXX) $(CXX_EXTRA_FLAGS) $(CXX_PLATFORM_FLAGS) $(CXX_FLAGS) -c

CXX_LINK = perl ../templ.pl $(PREFIX)g++
CXX_LIBS = -lstdc++

.SUFFIXES:
.SUFFIXES: .cpp .a .o .s .exe

.cpp.o:
	$(CXX_COMPILE) -o $@ $<

.cpp.s:
	$(CXX_COMPILE) -S -o $@ $<

.o.exe:
	$(CXX) -o $@ $< $(LIBPLT) $(PLATFORM_LIBS) -lstdc++

VPATH = $(SRCDIR)

### Include generic part
include ../generic.mk

libplt.a: $(LIBPLT_OBJECTS)
	$(PREFIX)ar r $@ $?
	$(PREFIX)$(RANLIB) $@
	$(PREFIX)strip --strip-debug libplt.a

depend : $(CXX_SOURCES)
	$(CXX_COMPILE) -MM $^ > .depend
	perl ../depend.pl .depend > ../depend.nt
	perl ../dependvms.pl .depend > ../depend.vms

.depend:
	touch .depend



clean :
	rm -f *.o
	rm -f *.exe

mrproper :
	rm -f .depend *.a
#	for i in *_inst.h ; do echo rm $i ; touch $i ; done

### Include autodependencies

include .depend
