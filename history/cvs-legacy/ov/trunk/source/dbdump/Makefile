PLATFORM = linux
ACPLTDIR = ../../../
PLT_DIR = $(ACPLTDIR)plt/
KS_DIR = $(ACPLTDIR)ks/
LIBPLT = $(PLT_DIR)build/$(PLATFORM)/libplt.a
LIBKS = $(KS_DIR)build/$(PLATFORM)/libks.a
LIBKSCLN = $(KS_DIR)build/$(PLATFORM)/libkscln.a

CXX = g++

CXX_FLAGS = -g -Wall -DNDEBUG -DPLT_USE_BUFFERED_STREAMS=1
CXX_EXTRA_FLAGS = -I . -I$(KS_DIR)include -I$(PLT_DIR)include -fno-implicit-templates
CXX_PLATFORM_FLAGS = -DPLT_SYSTEM_LINUX=1
CXX_LIBS = -lstdc++

CXX_COMPILE = $(CXX) $(CXX_EXTRA_FLAGS) $(CXX_PLATFORM_FLAGS) $(CXX_FLAGS) -c
CXX_LINK = MAKE=$(MAKE) perl $(PLT_DIR)build/templ.pl g++ $(LD_FLAGS)

dbdump:		dbdump.o fnmatch.o
	$(CXX_LINK) -o $@ $^ $(LIBKSCLN) $(LIBKS) $(LIBPLT)

dbdump.o:	dbdump.cpp dbdump.h dbdump_inst.h
	$(CXX_COMPILE) dbdump.cpp

fnmatch.o:	fnmatch.c fnmatch.h
	$(CXX_COMPILE) fnmatch.c

clean:
	rm -f dbdump.o fnmatch.o core dbdump
