#/******************************************************************************
#***
#***   L I B M L 
#***   #####################################
#***
#***   L T S o f t
#***   Agentur für Leittechnik Software GmbH
#***   Brabanterstr. 13
#***   D-50171 Kerpen
#***   Tel : 02237/92869-2
#***   Fax : 02237/92869-9
#***   e-Mail   : ltsoft@ltsoft.de 
#***   Internet : http://www.ltsoft.de
#*** 
#*******************************************************************************
#*
#*   Datei
#*   -----
#*   borland.mk
#*
#*   Historie
#*   --------
#*   2008-04-30 Alexander Neugebauer: Erstellung, LTSoft GmbH, Kerpen
#*
#*   Beschreibung
#*   ------------ 
#*   Makefile fuer libml unter Windows (Compiler Borland, GNU make)
#******************************************************************************/


CFLAGS = -w -pc -a8 -d -O2 -D__STDC__=1 -DML_SYSTEM_WIN -DDBG_TO_STDERR=0

all: libml.lib

HEADERS = ml_malloc.h

SOURCES_LIBML = ml_malloc.c ml_free.c ml_realloc.c

OBJECTS_LIBML = $(foreach src, $(SOURCES_LIBML), $(basename $(src)).obj)

libml.lib: $(OBJECTS_LIBML)
	tlib /P64 $@ $(foreach obj, $^, +-$(obj))


$(OBJECTS_LIBMPM): $(SOURCES_LIBMPM) $(HEADERS)

.SUFFIXES: .c .obj

.c.obj:
	bcc32 $(CFLAGS) -I. -c -o$@ $<

test.exe : test.obj
	bcc32 -e$@ $^ libml.lib
    
# Keep the world cleen.

clean:
	-del *.lib *.obj

