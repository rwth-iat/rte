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
#*   Makefile
#*
#*   Historie
#*   --------
#*   2008-04-30 Alexander Neugebauer: Erstellung, LTSoft GmbH, Kerpen
#*
#*   Beschreibung
#*   ------------ 
#*   Makefile fuer libml unter Unix
#******************************************************************************/

# Use this on System V.
#CPPFLAGS = -DUSG

UNAME_M := $(shell uname -m)
ifneq ($(filter arm%,$(UNAME_M)),)
OV_ARCH_BITWIDTH_CFLAGS         =       
OV_ARCH_BITWIDTH_LDFLAGS        =       
else ifdef OV_ARCH_BITWIDTH
OV_ARCH_BITWIDTH_CFLAGS		=	-m$(OV_ARCH_BITWIDTH)
OV_ARCH_BITWIDTH_LDFLAGS	=	-m$(OV_ARCH_BITWIDTH)
else
OV_ARCH_BITWIDTH_CFLAGS		=	-m32
OV_ARCH_BITWIDTH_LDFLAGS	=	-m32
endif

CFLAGS = $(OV_ARCH_BITWIDTH_CFLAGS) -Wall -O2 -DML_SYSTEM_UNIX -DDBG_TO_STDERR=0 -fPIC


ifneq "$(origin GCC_BIN_PREFIX)" "undefined"
CC = $(GCC_BIN_PREFIX)gcc
else
GCC_BIN_PREFIX=
endif

CC ?= $(GCC_BIN_PREFIX)gcc
AR = $(GCC_BIN_PREFIX)ar
RANLIB = $(GCC_BIN_PREFIX)ranlib



all: libml.a

HEADERS = ../../include/libml/ml_malloc.h

SOURCES_LIBML = ml_malloc.c ml_free.c ml_realloc.c

OBJECTS_LIBML = $(foreach src, $(SOURCES_LIBML), $(basename $(src)).o)

libml.a: $(OBJECTS_LIBML)
	$(AR) crv $@ $^
	$(RANLIB) $@


$(OBJECTS_LIBML): $(SOURCES_LIBML) $(HEADERS)

.c.o:
	$(CC) $(CFLAGS) $(CPPFLAGS) -I. -c $<

# Keep the world cleen.

clean:
	-rm -f *.a *.o core
