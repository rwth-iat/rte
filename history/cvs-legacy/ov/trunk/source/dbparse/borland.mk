PLATFORM = nt
ACPLTDIR = ../../plt/acplt
PLTDIR = $(ACPLTDIR)/plt
KSDIR = $(ACPLTDIR)/ks
ONCDIR = ../../plt/oncrpc
LIBPLT = $(PLTDIR)/build/$(PLATFORM)/libplt.lib
LIBKS = $(KSDIR)/build/$(PLATFORM)/libks.lib
LIBKSCLN = $(KSDIR)/build/$(PLATFORM)/libkscln.lib
TOOLS = c:/tools/bin

FLEX = $(TOOLS)/flex
BISON = $(TOOLS)/bison
CXX = bcc32

CXX_FLAGS = -DPLT_INSTANTIATE_TEMPLATES=1 -DPLT_COMPILER_BORLAND=__BORLANDC__ -w -DNDEBUG -DPLT_USE_BUFFERED_STREAMS -DPLT_SYSTEM_NT=1 -D_BORLANDC=1
CXX_EXTRA_FLAGS = -a8 -I. -pc -d -I$(PLTDIR)/include -I$(KSDIR)/include -I$(ONCDIR) -Int #-v -y

dbparse.exe:	dbparse.obj dbparse1.obj db_lex.obj db_y.obj
	$(CXX) -P $^ $(LIBKSCLN) $(LIBKS) $(LIBPLT) $(ONCDIR)/bin/oncrpc.lib

dbparse.obj:		dbparse.cpp
	$(CXX) -P -c $(CXX_FLAGS) $(CXX_EXTRA_FLAGS) dbparse.cpp 

dbparse1.obj:		dbparse1.cpp dbparse.h db_y.h
	$(CXX) -P -c $(CXX_FLAGS) $(CXX_EXTRA_FLAGS) dbparse1.cpp

db_lex.obj:	db_lex.c
	$(CXX) -P -c $(CXX_FLAGS) $(CXX_EXTRA_FLAGS) db_lex.c

db_y.obj:		db_y.c dbparse.h
	$(CXX) -P -c $(CXX_FLAGS) $(CXX_EXTRA_FLAGS) db_y.c

db_lex.c:	db.lex dbparse.h db_y.h
	$(FLEX) -o$@ db.lex

db_y.c:		db.y dbparse.h
	$(BISON) -t -d -v -o$@ db.y

db_y.h:		db_y.c

clean:
	del dbparse.exe dbparse.obj dbparse1.obj db_lex.c db_y.c db_lex.h db_y.h
