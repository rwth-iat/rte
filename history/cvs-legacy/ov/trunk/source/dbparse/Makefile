PLATFORM = linux
ACPLTDIR = ../../..
PLT_DIR = $(ACPLTDIR)/plt
KS_DIR = $(ACPLTDIR)/ks
LIBPLT = $(ACPLTDIR)/lib/libplt.a
LIBKS = $(ACPLTDIR)/lib/libks.a
LIBKSCLN = $(ACPLTDIR)/lib/libkscln.a

FLEX = flex
BISON = bison
CC = gcc
CXX = g++

CXX_FLAGS = -g -Wall -DNDEBUG -DPLT_USE_BUFFERED_STREAMS=1
CXX_EXTRA_FLAGS = -I . -I$(KS_DIR)/include -I$(PLT_DIR)/include -fno-implicit-templates
CXX_PLATFORM_FLAGS = -DPLT_SYSTEM_LINUX=1
CXX_LIBS = -lstdc++

CXX_COMPILE = $(CXX) $(CXX_EXTRA_FLAGS) $(CXX_PLATFORM_FLAGS) $(CXX_FLAGS) -c
CXX_LINK = MAKE=$(MAKE) perl templ.pl g++ $(LD_FLAGS)

dbparse:	dbparse.o dbparse1.o db.lex.o db.y.o
	$(CXX_LINK) -o $@ $^ $(LIBKSCLN) $(LIBKS) $(LIBPLT)

dbparse.o:		dbparse.cpp dbparse_inst.h
	$(CXX_COMPILE) $< -o $@

dbparse1.o:		dbparse1.cpp dbparse.h db.y.h
	$(CXX_COMPILE) $< -o $@

db.lex.o:	db.lex.c
	$(CXX_COMPILE) $< -o $@

db.y.o:		db.y.c dbparse.h
	$(CXX_COMPILE) $< -o $@

db.lex.c:	db.lex dbparse.h db.y.h
	$(FLEX) -o$@ $<

db.y.c:		db.y dbparse.h
	$(BISON) -t -d -v -o$@ $<

db.y.h:		db.y.c

clean:
	rm -f dbparse dbparse.o dbparse1.o db.lex.* db.y.* core *.bak
