#/*****************************************************************************
#*                                                                            *
#*   i F B S p r o                                                            *
#*   #############                                                            *
#*                                                                            *
#*   © L T S o f t                                                            *
#*   Agentur für Leittechnik Software GmbH                                    *
#*   Heinrich-Hertz-Straße 10                                                 *
#*   50170 Kerpen                                                             *
#*   Tel      : 02273/9893-0                                                  *
#*   Fax      : 02273/9893-33                                                 *
#*   e-Mail   : ltsoft@ltsoft.de                                              *
#*   Internet : http://www.ltsoft.de                                          *
#*                                                                            *
#******************************************************************************
#*                                                                            *
#*   Datei                                                                    *
#*   -----                                                                    *
#*   makefile                                                                 *
#******************************************************************************
#*                                                                            *
#*   Historie                                                                 *
#*   --------                                                                 *
#*                                                                            *
#*****************************************************************************/


#	COMPILER  GNU
#	-------------


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

#	Include generic part
#	--------------------

include ../generic.mk

# Oncrpc
# ------

ONCRPC_DIR	= ..\\..\\..\\..\\base\\oncrpc\\
USERLIB_DIR	= ../../../libs
COM_DIR      = ../../../cmdlib/
cmdlib_DIR      = $(COM_DIR)include/

OVLIBS = $(OV_LIBS_DIR)libov$(LIB) $(OV_LIBS_DIR)libovks$(LIB)
FBLIBS = $(FBS_LIBS_DIR)fb$(LIB)
cmdlib = $(COM_DIR)build/nt/cmdlib$(LIB)

#	Targets und deren Quellen
#	-------------------------

USERLIB_SRC = $(USERLIB_C) $(wildcard $(SOURCE_DIR)*.c)
USERLIB_OBJ = $(foreach source, $(USERLIB_SRC), $(basename $(notdir $(source)))$(OBJ))

TARGETS = \
	ov.h \
	fb.h \
	cmdlib.h \
	$(USERLIB_LIB) \
	$(USERLIB_DLL)

SOURCES = \
	ov.h \
	fb.h \
	cmdlib.h \
	$(USERLIB_SRC)

all: $(TARGETS)

#	Compiler
#	--------

OV_CODEGEN_EXE = $(BIN_DIR)ov_codegen$(EXE)

MKIMPDEF        = $(BIN_DIR)mkimpdef$(EXE)
MKEXPDEF        = $(BIN_DIR)mkexpdef$(EXE)
MKDLLDEF        = $(BIN_DIR)mkdlldef$(EXE)

CC              = bcc32
CC_FLAGS        = -w -v -pc -wsig- -a8
CC_DEFINES      = $(DEFINES) -DFD_SETSIZE=128 -DOV_COMPILE_LIBRARY_$(LIBRARY)
CC_INCLUDES     = $(INCLUDES) -I$(ONCRPC_DIR) -I$(cmdlib_DIR)
COMPILE_C		= $(CC) $(CC_FLAGS) $(CC_DEFINES) $(CC_INCLUDES) -c

LINK            = $(CC) $(CC_FLAGS)

IMPLIB          = implib
IMPDEF          = impdef

LD              = $(CC) $(CC_FLAGS) -tWDE

AR              = tlib /P64

RC              = brc32

#   Implicit Rules
#   --------------

.c$(OBJ):
	$(COMPILE_C) -o$@ $<

.ovm.c:
	cmd /c $(OV_CODEGEN_EXE) -I $(ACPLT_OV_INCLUDE_DIR) -I $(FBS_INCLUDE_DIR) -I $(cmdlib_DIR) -f $(subst /,\\, $<) -l $(notdir $(basename $<))

.ovm.h:
	cmd /c $(OV_CODEGEN_EXE) -I $(ACPLT_OV_INCLUDE_DIR) -I $(FBS_INCLUDE_DIR) -I $(cmdlib_DIR) -f $(subst /,\\, $<) -l $(notdir $(basename $<))

#   Dependencies
#   ------------

depend:
	@echo Unable to generate dependencies under NT

#	Libraries
#	---------

ov.h : ov.ovm
	./$(OV_CODEGEN_EXE) -f $(subst /,\\, $<)

fb.h : fb.ovm
	./$(OV_CODEGEN_EXE) -f $(subst /,\\, $<) -I $(ACPLT_OV_INCLUDE_DIR)

cmdlib.h : $(cmdlib_DIR)cmdlib.ovm
	./$(OV_CODEGEN_EXE) -f $(subst /,\\, $<) -I $(FBS_INCLUDE_DIR) -I $(cmdlib_DIR) -I $(ACPLT_OV_INCLUDE_DIR)

$(USERLIB_LIB) : $(USERLIB_DLL)

$(USERLIB_DLL) : $(USERLIB_OBJ) $(FBLIBS) $(OVLIBS)
	@del $(basename $@).def
	$(LD) -e$@ $(filter-out %$(RES), $^)
	$(IMPDEF) $(basename $@)_tmp.def $@
	$(MKIMPDEF) $(basename $@)_tmp.def $(basename $@).def
	$(IMPLIB) $(basename $@)$(LIB) $(basename $@).def
	$(MKEXPDEF) $(basename $@)_tmp.def $(basename $@).def
	$(LD) -e$@ $(filter-out %$(RES), $^)
	$(MKDLLDEF) $(basename $@)_tmp.def $(basename $@).def
	@del $(basename $@)_tmp.def
	copy $(USERLIB_DLL) $(subst /,\\, $(USERLIB_DIR))

#	Executables
#	-----------

#	Install
#	-------

install : $(TARGETS)
	copy $(USERLIB_DLL) $(subst /,\\, $(USERLIB_DIR))

#	Aufraeumen
#	----------

clean:
	@del *.c
	@del *.h
	@del *.x
	@del *.bak
	@del *.map
	@del *$(LIB)
	@del *$(DLL)
	@del *$(OBJ)
	@del *$(RES)
	@del respfile
#	Include dependencies
#	--------------------

include depend.nt

