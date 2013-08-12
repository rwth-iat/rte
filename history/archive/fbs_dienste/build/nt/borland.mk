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
#*   borland.mk                                                               *
#*                                                                            *
#*   Historie                                                                 *
#*   --------                                                                 *
#*   1999-04-07  Alexander Neugebauer: Erstellung, LTSoft, Kerpen             *
#*                                                                            *
#*                                                                            *
#*   Beschreibung                                                             *
#*   ------------                                                             *
#*   Makefile fuer die iFBSpro-Dienste unter NT (Borland C++ 5.0)             *
#*                                                                            *
#*****************************************************************************/


#	Plattform
#	---------

SYSTEM = NT
SYSDIR = nt

#	Filename conventions
#	--------------------

OBJ = .obj
LIB = .lib
DLL = .dll
EXE = .exe
RES = .res

#	Directories
#	-----------

BORLAND_DIR					= c:\\bc5
#BORLAND_DIR					= c:\Programme\bc5

BORLAND_BIN_DIR				= $(BORLAND_DIR)\bin
BORLAND_LIB_DIR				= $(BORLAND_DIR)\lib
BORLAND_INCLUDE_DIR			= $(BORLAND_DIR)\include

FLEX				= flex
BISON				= bison

#	Include generic part
#	--------------------

include ..\generic.mk


#	Compiler
#	--------


RC 					= brc32
FLEX			= flex
BISON			= bison
CPP 			= cpp

CC				= bcc32 -v-
CC_FLAGS		= -w -pc -wsig- -a8 -O2 -d $(OV_PLATFORM_DEFINES)
COMPILE_C		= $(CC) $(CC_FLAGS) $(DEFINES) $(INCLUDES) -c 

LINK			= $(CC) $(CC_FLAGS)
LD				= $(CC) $(CC_FLAGS) -tWDE

CXX				= bcc32 -P -v-
CXX_FLAGS		= $(CC_FLAGS)
CXX_COMPILE		= $(CXX) $(CXX_FLAGS) $(DEFINES) $(INCLUDES) -c

CXX_LINK		= $(CXX) $(CXX_FLAGS)

AR	 			= tlib /P64

IMPLIB			= implib
IMPDEF			= impdef



#   Implicit Rules
#   --------------

.c$(OBJ):
	$(COMPILE_C) $< -o$@

.cpp$(OBJ):
	@echo Compiling $<
	$(CXX_COMPILE) -Jgx $< -o$@

.rc$(RES):
	$(RC) -r -fo$@ $<

.lex.c:
	$(FLEX) -o$@ $<

.y.c:
	$(BISON) -t -d -o$@ $<



$(DIENST_LIB) : $(DIENST_LIB_OBJ)
	..\plt_ar tlib /P256 $@ $(DIENST_LIB_OBJ)

#	Executables
#	-----------


templ_for_exec.obj :  $(SOURCE_DIR)templ_for_exec.cpp
	$(CXX) -Jgd $(CXX_FLAGS) $(DEFINES) $(INCLUDES) -c -o$@ $^


dbcommands.res : $(FBSLIB_SRC_DIR)dbcommands.rc
	$(RC) -r -fo$@ $<
	
fb_dbcommands$(EXE) :  dbcommands.obj templ_for_exec.obj $(DIENST_LIB) dbcommands.res
	$(LINK) -e$@ dbcommands.obj templ_for_exec.obj $(DIENST_LIB) $(ACPLTKS_LIBS)
	$(RC) dbcommands.res $@
	tdstrp32 fb_dbcommands$(EXE)

fb_init.res : $(FBSLIB_SRC_DIR)fb_init.rc
	$(RC) -r -fo$@ $<

fb_init$(EXE) : fb_init.obj templ_for_exec.obj $(DIENST_LIB) fb_init.res
	$(LINK) -e$@ fb_init.obj templ_for_exec.obj $(DIENST_LIB) $(ACPLTKS_LIBS)
	$(RC) fb_init.res $@
	tdstrp32 fb_init$(EXE)



dbsavexml$(OBJ) : dbsavexml.cpp
	@echo Compiling $<
	$(CXX_COMPILE) -DXML_SIMPLE=1 -DNUR_TAG_ZIECHEN_CODIEREN=1 -Jgx $< -o$@

ifb_dbsaveinxml$(OBJ) : ifb_dbsaveinxml.cpp
	@echo Compiling $<
	$(CXX_COMPILE) -DXML_SIMPLE=1 -DNUR_TAG_ZIECHEN_CODIEREN=1 -Jgx $< -o$@

fb_dbsavexml.res : $(FBSLIB_SRC_DIR)fb_dbsavexml.rc
	$(RC) -r -fo$@ $<

ks_savexml$(EXE) :  dbsavexml$(OBJ) ifb_dbsaveinxml$(OBJ) templ_for_exec.obj $(DIENST_LIB) fb_dbsavexml.res
	$(LINK) -e$@ dbsavexml$(OBJ) ifb_dbsaveinxml.obj templ_for_exec.obj $(DIENST_LIB) $(ACPLTKS_LIBS)
	$(RC) fb_dbsavexml.res $@
	tdstrp32 ks_savexml$(EXE)


#	Aufraeumen
#	----------

clean :
	-del *$(OBJ)
	-del *.res
	-del *.bak
	-del *.map
	-del fb_parser.c
	-del fb_parser.h
	-del fb_scanner.c

cleanall : clean
	-del *$(LIB)
	-del *.exe



#BccW32.cfg : 
#   Copy &&|
#-w
#-R
#-v
#-vi
#-H
#-H=xks-lib.csm
#| $@
