PLATFORM = nt
ACPLTDIR = ../../../
PLTDIR = $(ACPLTDIR)plt/
KSDIR = $(ACPLTDIR)ks/
ONCDIR = $(ACPLTDIR)oncrpc/
LIBPLT = $(ACPLTDIR)lib/libplt.lib
LIBKS = $(ACPLTDIR)lib/libks.lib
LIBKSCLN = $(ACPLTDIR)lib/libkscln.lib
LIBONCRPC = $(ACPLTDIR)lib/oncrpc.lib
CXX = cl
LINK = link /MACHINE:I386 /NOLOGO

CXX_FLAGS = /GR /DNDEBUG /nologo /DFD_SETSIZE=128 /W3 /MT /GX /DPLT_USE_BUFFERED_STREAMS /DPLT_SYSTEM_NT=1 /DPLT_DEBUG_NEW=0 /I. /I$(PLTDIR)include /I$(KSDIR)include /I$(ONCDIR) /Int
CXX_EXTRA_FLAGS =

dbdump.exe:	dbdump.obj fnmatch.obj
	$(LINK) $^ $(LIBKSCLN) $(LIBKS) $(LIBPLT) $(LIBONCRPC) ADVAPI32.LIB USER32.LIB wsock32.lib /out:$@

dbdump.obj:	dbdump.cpp dbdump.h dbdump_inst.h
	$(CXX) /c $(CXX_FLAGS) $(CXX_EXTRA_FLAGS) dbdump.cpp

fnmatch.obj:	fnmatch.c fnmatch.h
	$(CXX) /c $(CXX_FLAGS) $(CXX_EXTRA_FLAGS) fnmatch.c

clean:
	del dbdump.obj fnmatch.obj dbdump.exe

