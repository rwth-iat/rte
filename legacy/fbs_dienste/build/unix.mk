#/*****************************************************************************
#*                                                                            *
#*    i F B S p r o                                                           *
#*    #############                                                           *
#*                                                                            *
#*   L T S o f t                                                              *
#*   Agentur für Leittechnik Software GmbH                                    *
#*   Heinrich-Hertz-Straße 10                                                 *
#*   50170 Kerpen                                                             *
#*   Tel      : 02273/9893-0                                                  *
#*   Fax      : 02273/52526                                                   *
#*   e-Mail   : ltsoft@ltsoft.de                                              *
#*   Internet : http://www.ltsoft.de                                          *
#*                                                                            *
#******************************************************************************
#*                                                                            *
#*   iFBSpro - Client-Bibliothek                                              *
#*   IFBSpro/Client - iFBSpro  ACPLT/KS Dienste-Schnittstelle (C++)           *
#*   ==============================================================           *
#*                                                                            *
#*   Datei                                                                    *
#*   -----                                                                    *
#*   unix.mk                                                                  *
#*                                                                            *
#*   Historie                                                                 *
#*   --------                                                                 *
#*   1999-04-07  Alexander Neugebauer: Erstellung, LTSoft, Kerpen             *
#*                                                                            *
#*                                                                            *
#*   Beschreibung                                                             *
#*   ------------                                                             *
#*                                                                            *
#*                                                                            *
#*****************************************************************************/

#	Plattform
#	---------

#Hauptmakefile enthält Definition der Form:
#SYSTEM = LINUX
#SYSDIR = linux

#	Filename conventions
#	--------------------

OBJ = .o
LIB = .a
DLL = .so

#   Include generic part
#   --------------------

include ../generic.mk

#	Targets
#	-------

all: $(TARGETS)

# 32/64 bit architecture
UNAME_M := $(shell uname -m)
ifneq ($(filter arm%,$(UNAME_M)),)
OV_ARCH_BITWIDTH_CFLAGS         =
OV_ARCH_BITWIDTH_LDFLAGS	=	$(OV_ARCH_BITWIDTH_CFLAGS)
else ifdef OV_ARCH_BITWIDTH
OV_ARCH_BITWIDTH_CFLAGS		=	-m$(OV_ARCH_BITWIDTH)
OV_ARCH_BITWIDTH_LDFLAGS	=	$(OV_ARCH_BITWIDTH_CFLAGS)
else
OV_ARCH_BITWIDTH_CFLAGS		=	-m32
OV_ARCH_BITWIDTH_LDFLAGS	=	$(OV_ARCH_BITWIDTH_CFLAGS)
endif

#	Compiler
#	--------
GOV_PRECOMPILER_EXE	= $(BIN_DIR)ov_codegen$(EXE)

FLEX			= flex
BISON			= bison
RPCGEN			= rpcgen

GCC_BIN_PREFIX			= 

CC				= $(GCC_BIN_PREFIX)gcc
CC_FLAGS		= $(OV_ARCH_BITWIDTH_CFLAGS) -Wall -DOV_DEBUG -O2

CC_DEFINES		= $(DEFINES)
CC_INCLUDES		= $(INCLUDES)
COMPILE_C		= $(CC) $(CC_FLAGS) $(CC_DEFINES) $(CC_INCLUDES) -c

LINK			= $(CC)
C_LIBS			= $(GOV_GOVLIB_LIB)


CXX				= $(GCC_BIN_PREFIX)g++
CXX_FLAGS		= $(OV_ARCH_BITWIDTH_CFLAGS) -DNDEBUG -Wall -O2 -fno-implicit-templates
CXX_DEFINES		= $(CC_DEFINES)
CXX_INCLUDES	= $(CC_INCLUDES)
CXX_COMPILE		= $(CXX) $(CXX_FLAGS) $(CXX_DEFINES) $(CXX_INCLUDES) -c

CXX_LINK		= MAKE=$(MAKE) perl ../templ.pl $(CXX) $(OV_ARCH_BITWIDTH_CFLAGS)
CXX_LIBS		= $(C_LIBS) $(GOV_GOVLIB_LIB) $(ACPLTKSLIBS) -lstdc++

AR				= $(GCC_BIN_PREFIX)ar
RANLIB			= $(GCC_BIN_PREFIX)ranlib

LD				= ld $(OV_ARCH_BITWIDTH_LDFLAGS) -shared
LD_LIB			= -ldl


#   Implicit Rules
#   --------------

.c$(OBJ):
	$(COMPILE_C) $< -o $@

#.c$(OBJ):
#	$(CXX_COMPILE) -o $@ $<

.cpp$(OBJ):
	$(CXX_COMPILE) -o $@ $<

.lex.c:
	$(FLEX) -o$@ $<

.y.c:
	$(BISON) -t -d -o$@ $<

ov.h : $(GOV_MODEL_DIR)ov.ovm

#	./$(GOV_PRECOMPILER_EXE) -f $(GOV_MODEL_DIR)ov.ovm

#   Dependencies
#   ------------
depend.mk : $(SOURCES) ov.h
	$(CXX_COMPILE) -MM $(SOURCES) > .depend
	perl ../depend_nt.pl .depend > ../nt/depend.nt

#	touch fbs_dia_inst.h
#	perl ../generic/depend.pl .depend > $@
#	perl ../depend_vms.pl .depend > ../openvms/depend.vms
#	perl ../depend_mc.pl .depend > ../mc164/depend.mc

.depend:
	touch .depend

depend : depend.mk


#	Libraries
#	---------

#	Dienst-lib

$(DIENST_LIB) : $(DIENST_LIB_OBJ)
	$(AR) rv $@ $?
	$(RANLIB) $@
	$(GCC_BIN_PREFIX)strip --strip-debug $(DIENST_LIB)


$(DIENST_LIB_DLL) : $(DIENST_LIB_OBJ)

#	$(LD) -o $@ $^
#	strip --strip-debug $(DIENST_LIB_DLL)
#	cp $(DIENST_LIB_DLL) $(BIN_DIR)


testtmpl.o : $(SOURCE_DIR)testtmpl.cpp
	$(CXX_COMPILE) -o $@ $<


test_hist.o : $(SOURCE_DIR)test_hist.cpp
	$(CXX_COMPILE) -o $@ $<
	
test_hist_templates.o : $(SOURCE_DIR)test_hist_templates.cpp
	$(CXX_COMPILE) -o $@ $<

test_hist.exe : test_hist.o test_hist_templates.o
	$(CXX_LINK) -o $@ $^ $(DIENST_LIB) $(LIBKSCLN) $(LIBKS) $(LIBPLT) -lstdc++


#   Executes
#   --------

fb_dbsave.o : $(SOURCE_DIR)dbsave.cpp
	$(CXX_COMPILE) -o $@ $<

templ_for_exec.o : $(SOURCE_DIR)templ_for_exec.cpp
	$(CXX_COMPILE) -o $@ $<

fb_dbsave$(EXE) : fb_dbsave.o templ_for_exec.o
	$(CXX_LINK) -o $@ $^ $(DIENST_LIB) $(LIBKSCLN) $(LIBKS) $(LIBPLT) -lstdc++
	$(GCC_BIN_PREFIX)strip --strip-debug fb_dbsave$(EXE)

fb_dbload.o : $(SOURCE_DIR)dbload.cpp
	$(CXX_COMPILE) -o $@ $<

fb_dbload$(EXE) : fb_dbload.o templ_for_exec.o
	$(CXX_LINK) -o $@ $^ $(DIENST_LIB) $(LIBKSCLN) $(LIBKS) $(LIBPLT) -lstdc++
	$(GCC_BIN_PREFIX)strip --strip-debug fb_dbload$(EXE)

fb_dbcommands.o : $(SOURCE_DIR)dbcommands.cpp
	$(CXX_COMPILE) -o $@ $<

#static for windows build. Feed free to bugfix the static linking for unix, too.
fb_dbcommands$(EXE) : fb_dbcommands.o templ_for_exec.o
ifeq ($(SYSTEM), NT)
	$(CXX_LINK) -static -o $@ $^ $(DIENST_LIB) $(ACPLTKS_LIBS) -lstdc++ $(WINDOWS_LIBS)
else
	$(CXX_LINK) -o $@ $^ $(DIENST_LIB) $(ACPLTKS_LIBS) -lstdc++ $(WINDOWS_LIBS)
endif

	$(GCC_BIN_PREFIX)strip --strip-debug fb_dbcommands$(EXE)

fb_init.o : $(SOURCE_DIR)fb_init.cpp
	$(CXX_COMPILE) -o $@ $<

fb_init$(EXE) : fb_init.o templ_for_exec.o
	$(CXX_LINK) -o $@ $^ $(DIENST_LIB) $(ACPLTKS_LIBS) -lstdc++ $(WINDOWS_LIBS)
	$(GCC_BIN_PREFIX)strip --strip-debug fb_init$(EXE)


# $(LIBKSSVR)  -rdynamic

#	Aufraeumen
#	----------

clean :
	rm -f core
	rm -f *.c
	rm -f *.h
	rm -f *$(OBJ)

cleanall : clean
	rm -f *$(DLL)
	rm -f *$(LIB)
	rm -f fb_init$(EXE) fb_dbcommands$(EXE) fb_dbsave$(EXE) fb_dbload$(EXE)

#	Include dependencies
#	--------------------

include .depend
