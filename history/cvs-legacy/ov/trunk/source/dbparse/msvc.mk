PLATFORM = nt

ACPLTDIR = ../../../

PLTDIR = $(ACPLTDIR)plt/

KSDIR = $(ACPLTDIR)ks/

ONCDIR = $(ACPLTDIR)oncrpc/

LIBPLT = $(ACPLTDIR)lib/libplt.lib

LIBKS = $(ACPLTDIR)lib/libks.lib

LIBKSCLN = $(ACPLTDIR)lib/libkscln.lib

LIBONCRPC = $(ACPLTDIR)lib/oncrpc.lib

TOOLS = c:/tools/bin/



FLEX = $(TOOLS)flex

BISON = $(TOOLS)bison

CXX = cl

LINK = link /MACHINE:I386 /NOLOGO



CXX_FLAGS = /GR /DNDEBUG /nologo /DFD_SETSIZE=128 /W3 /MT /GX /DPLT_USE_BUFFERED_STREAMS /DPLT_SYSTEM_NT=1 /DPLT_DEBUG_NEW=0 /I. /I$(PLTDIR)include /I$(KSDIR)include /I$(ONCDIR) /Int

CXX_EXTRA_FLAGS =



dbparse.exe:	dbparse.obj dbparse1.obj db_lex.obj db_y.obj

	$(LINK) $^ $(LIBKSCLN) $(LIBKS) $(LIBPLT) $(LIBONCRPC) ADVAPI32.LIB USER32.LIB wsock32.lib /out:$@



dbparse.obj:	dbparse.cpp

	$(CXX) /c $(CXX_FLAGS) $(CXX_EXTRA_FLAGS) dbparse.cpp



dbparse1.obj:	dbparse1.cpp dbparse.h db_y.h

	$(CXX) /c $(CXX_FLAGS) $(CXX_EXTRA_FLAGS) dbparse1.cpp



db_lex.obj:	db_lex.c

	$(CXX) /c /TP $(CXX_FLAGS) db_lex.c



db_y.obj:	db_y.c dbparse.h

	$(CXX) /c /TP $(CXX_FLAGS) db_y.c



db_lex.c:	db.lex dbparse.h db_y.h

	$(FLEX) -o$@ db.lex



db_y.c:		db.y dbparse.h

	$(BISON) -t -d -v -o$@ db.y



db_y.h:		db_y.c



clean:

	del dbparse.exe dbparse.obj dbparse1.obj db_lex.c db_y.c db_lex.h db_y.h

