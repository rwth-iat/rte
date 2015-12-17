#/******************************************************************************
#***                                                                         ***
#***   iFBSpro   -   Funktionsbaustein-Model                                 ***
#***   #####################################                                 ***
#***                                                                         ***
#***   L T S o f t                                                           ***
#***   Agentur für Leittechnik Software GmbH                                 ***
#***   Brabanterstr. 13                                                      ***
#***   D-50171 Kerpen                                                        ***
#***   Tel : 02237/92869-2                                                   ***
#***   Fax : 02237/92869-9                                                   ***
#***   e-Mail   : ltsoft@ltsoft.de                                           ***
#***   Internet : http://www.ltsoft.de                                       ***
#***                                                                         ***
#***   -------------------------------------------------------------------   ***
#***                                                                         ***
#***   Implementierung des Funktionsbausteinsystems IFBSpro                  ***
#***   RWTH,   Aachen                                                        ***
#***   LTSoft, Kerpen                                                        ***
#***                                                                         ***
#*******************************************************************************
#*                                                                             *
#*   Datei                                                                     *
#*   -----                                                                     *
#*   msvc.mk                                                                   *
#*                                                                             *
#*   Historie                                                                  *
#*   --------                                                                  *
#*   2006-10-31 Alexander Neugebauer: Erstellung, LTSoft GmbH, Kerpen          *
#*                                                                             *
#*   Beschreibung                                                              *
#*   ------------                                                              *
#*   Makefile fuer iFBSpro (Compiler MSVC)                                     *
#******************************************************************************/



#	Plattform
#	---------

SYSTEM = NT
SYSDIR = ntvc

#	Filename conventions
#	--------------------

OBJ = .obj
LIB = .lib
DLL = .dll
EXE = .exe
RES = .res

#	Platform-specific definitions
#	-----------------------------


#	Include generic part
#	--------------------

include ../generic_msvc.mk

#	Compiler
#	--------

COMPILER		= MSVC

FLEX			= flex
BISON			= bison
CPP 			= cpp

CC			= cl
CC_FLAGS		= /W3 /c
COMPILE_C		= $(CC) $(CC_FLAGS) /TC $(DEFINES) $(INCLUDES)

LINK_FLAGS      = /nologo
LINK			= link
LD			= link /DLL

CXX			= cl
CXX_FLAGS		= $(CC_FLAGS)
CXX_COMPILE		= $(CXX) $(CXX_FLAGS) /TP $(DEFINES) $(INCLUDES) /c /GX /GR /MT

CXX_LINK		= link /debug

RC 			= rc


#   Implicit Rules
#   --------------

.c$(OBJ):
	$(COMPILE_C) $< -o$@

.cpp$(OBJ):
	$(CXX_COMPILE) $< -o$@

.rc$(RES):
	$(RC) /fo$@ $<

.lex.c:
	$(FLEX) -o$@ $<

.y.c:
	$(BISON) -t -d -o$@ $<

cp : fb_parser.c
	@cop fb_parser.c xxx_parser.c

PLTSRV_LIBS = $(ACPLT_PLT_DIR)build/$(SYSDIR)/libplt$(LIB) \
              $(ACPLT_KS_DIR)build/$(SYSDIR)/libks$(LIB) \
              $(ACPLT_KS_DIR)build/$(SYSDIR)/libkssvr$(LIB) \
              $(LIBRPC_LIB) 


# Service library
# ---------------

$(DIENST_LIB) : $(DIENST_LIB_OBJ)
	lib /OUT:$@ $(DIENST_LIB_OBJ)

#	Executables
#	-----------

dbcommands.res: $(FBSLIB_SRC_DIR)dbcommands.rc
	$(RC) -r -fodbcommands.res $(FBSLIB_SRC_DIR)dbcommands.rc

$(FB_DBCOMMANDS_EXE) :  $(FB_DBCOMMANDS_OBJ) dbcommands.res
	$(CXX_LINK) $^ $(ACPLTKS_LIBS) $(DIENST_LIB) wsock32.lib ADVAPI32.LIB USER32.LIB $(LINK_FLAGS) /NODEFAULTLIB:libc /out:$(FB_DBCOMMANDS_EXE)


fb_init.res: $(FBSLIB_SRC_DIR)fb_init.rc
	$(RC) -r -fofb_init.res $(FBSLIB_SRC_DIR)fb_init.rc

$(FB_INIT_EXE) : $(FB_INIT_OBJ) fb_init.res
	$(CXX_LINK) $^ $(ACPLTKS_LIBS) $(DIENST_LIB) wsock32.lib ADVAPI32.LIB USER32.LIB $(LINK_FLAGS) /NODEFAULTLIB:libc /out:$(FB_INIT_EXE)

kstest.exe : kstest.obj
	$(CXX_LINK) $^ $(PLTSRV_LIBS) wsock32.lib ADVAPI32.LIB USER32.LIB $(LINK_FLAGS) /NODEFAULTLIB:libc /out:kstest.exe

tst.exe : tst.obj
	$(CXX_LINK) $^ $(PLTSRV_LIBS) wsock32.lib ADVAPI32.LIB USER32.LIB $(LINK_FLAGS) /NODEFAULTLIB:libc /out:tst.exe

pkg_test.exe : pkg_test.obj
	$(CXX_LINK) $^ $(ACPLTKS_LIBS) $(DIENST_LIB) wsock32.lib ADVAPI32.LIB USER32.LIB $(LINK_FLAGS) /NODEFAULTLIB:libc /out:pkg_test.exe





dbsavexml$(OBJ) : dbsavexml.cpp
	@echo Compiling $<
	$(CXX_COMPILE) -DXML_SIMPLE=1 -DNUR_TAG_ZIECHEN_CODIEREN=1 $< -o$@

ifb_dbsaveinxml$(OBJ) : ifb_dbsaveinxml.cpp
	@echo Compiling $<
	$(CXX_COMPILE) -DXML_SIMPLE=1 -DNUR_TAG_ZIECHEN_CODIEREN=1 $< -o$@

fb_dbsavexml.res : $(FBSLIB_SRC_DIR)fb_dbsavexml.rc
	$(RC) -r -fo$@ $<

ks_savexml$(EXE) :  dbsavexml$(OBJ) ifb_dbsaveinxml$(OBJ) fb_dbsavexml.res
	$(CXX_LINK) $^ $(ACPLTKS_LIBS) $(DIENST_LIB) wsock32.lib ADVAPI32.LIB USER32.LIB $(LINK_FLAGS) /NODEFAULTLIB:libc /out:ks_savexml$(EXE)


#	Clean up
#	--------

clean:
	@del *.c *.h *.bak *.map *.def *.exp *$(OBJ) *$(RES) *.pdb *.ilk

cleanall:
	@del *.c *.h *.bak *.map *.def *.exp *$(LIB) *$(DLL) *$(OBJ) *$(EXE) *$(RES) *.pdb *.ilk


