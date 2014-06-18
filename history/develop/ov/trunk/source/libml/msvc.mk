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
#*   msvc.mk
#*
#*   Historie
#*   --------
#*   2008-04-30 Alexander Neugebauer: Erstellung, LTSoft GmbH, Kerpen
#*
#*   Beschreibung
#*   ------------ 
#*   Makefile fuer libml unter Windows (Compiler VC++, GNU make)
#******************************************************************************/

CFLAGS = /W0 /c /O2 /D__STDC__=1 /DML_SYSTEM_WIN /DDBG_TO_STDERR=0

all: libml_vc.lib

HEADERS = ml_malloc.h

SOURCES_LIBML = ml_malloc.c ml_free.c ml_realloc.c

OBJECTS_LIBML = ml_malloc.obj ml_free.obj ml_realloc.obj

libml_vc.lib: $(OBJECTS_LIBML)
	lib /out:$@ $(OBJECTS_LIBML)

$(OBJECTS_LIBML): $(SOURCES_LIBML) $(HEADERS)

.SUFFIXES: .c .obj

.c.obj:
	cl $(CFLAGS) /I. /c /Fo$@ $<

test.exe : test.obj
	link $^ libml_vc.lib /out:test.exe

# Keep the world cleen.

clean:
	-del *.lib *.obj *.a *.o

