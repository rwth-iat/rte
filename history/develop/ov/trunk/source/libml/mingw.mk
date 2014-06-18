#/******************************************************************************
#***
#***   L I B M L 
#***   #####################################
#***
#***   L T S o f t
#***   Agentur f�r Leittechnik Software GmbH
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
#*   Makefile
#*
#*   Historie
#*   --------
#*   2011-07-28 Sten Gr�ner
#*
#*   Beschreibung
#*   ------------ 
#*   Makefile fuer libml unter Mingw
#******************************************************************************/

# Use this on System V.
#CPPFLAGS = -DUSG

CFLAGS = -Wall -O2 -DML_SYSTEM_WIN -DDBG_TO_STDERR=0


CC = gcc

all: libml.a

HEADERS = ml_malloc.h

SOURCES_LIBML = ml_malloc.c ml_free.c ml_realloc.c

OBJECTS_LIBML = $(foreach src, $(SOURCES_LIBML), $(basename $(src)).o)

libml.a: $(OBJECTS_LIBML)
	ar crv $@ $^
	ranlib $@


$(OBJECTS_LIBML): $(SOURCES_LIBML) $(HEADERS)

.c.o:
	$(CC) $(CFLAGS) $(CPPFLAGS) -I. -c $<

# Keep the world clean.

clean:
	-rm -f *.a *.o core
