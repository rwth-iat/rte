
#   $Id: borland.mk,v 1.1 2004-05-19 14:24:21 ansgar Exp $
#
#   Copyright (C) 1998-1999
#   Lehrstuhl fuer Prozessleittechnik,
#   RWTH Aachen, D-52056 Aachen, Germany.
#   All rights reserved.
#   
#   Author: Dirk Meyer <dirk@plt.rwth-aachen.de>
#   
#   This file is part of the ACPLT/OV Package which is licensed as open
#   source under the Artistic License; you can use, redistribute and/or
#   modify it under the terms of that license.
#   
#   You should have received a copy of the Artistic License along with
#   this Package; see the file ARTISTIC-LICENSE. If not, write to the
#   Copyright Holder.
#   
#   THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR IMPLIED
#   WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED WARRANTIES
#   OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE.

#	History:
#	--------
#	26-Nov-1998 Dirk Meyer <dirk@plt.rwth-aachen.de>: File created.
#	16-Apr-1999 Dirk Meyer <dirk@plt.rwth-aachen.de>: Major revision.
#	09-Jul-2001 Ansgar Münnemann <ansgar@plt.rwth-aachen.de>: ov_builder included.



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
CPL = .cpl

#	Platform-specific definitions
#	-----------------------------

ACPLTKS_PLATFORM_DEFINES	= -DFD_SETSIZE=128
OV_PLATFORM_DEFINES			= -DOV_DEBUG -DOV_CATCH_EXCEPTIONS

#	Compiler
#	--------

COMPILER		= BORLAND

MKIMPDEF		= mkimpdef
MKEXPDEF		= mkexpdef
MKDLLDEF		= mkdlldef

FLEX			= flex
BISON			= bison
CPP 			= cpp

CC				= bcc32
CC_FLAGS		= -w -v -pc -wsig- -a8 -O2 -d $(OV_PLATFORM_DEFINES)
COMPILE_C		= $(CC) $(CC_FLAGS) $(DEFINES) $(INCLUDES) -c 

LINK			= $(CC) $(CC_FLAGS)
LD				= $(CC) $(CC_FLAGS) -tWDE

CXX				= bcc32 -P
CXX_FLAGS		= $(CC_FLAGS)
CXX_COMPILE		= $(CXX) $(CXX_FLAGS) $(DEFINES) $(INCLUDES) -c

CXX_LINK		= $(CXX) $(CXX_FLAGS)

AR	 			= tlib /P64

IMPLIB			= implib
IMPDEF			= impdef

RC 				= brc32 -i$(OV_INCLUDE_DIR)

#	Include generic part
#	--------------------

include ../generic.mk

#	C-Includes
#	----------

C_INCLUDES =

#	ONCRPC-Includes and defines
#	---------------------------

LIBRPC_DEFINES = \
	$(LIBRPC_PLATFORM_DEFINES)\
	-DWIN32
LIBRPC_INCLUDES	= \
	-I$(ONCRPC_INCLUDE_DIR)

#	OV-Includes
#	-----------

OV_INCLUDES = $(OV_INCLUDES) \
	-I.\
	-I$(OV_SOURCE_CODEGEN_DIR) \
	-I$(OV_SOURCE_BUILDER_DIR) \
	-I$(OV_SOURCE_NTSERVICE_DIR)


#	all defines and includes together
#	---------------------------------

DEFINES	 = $(LIBRPC_DEFINES) $(ACPLTKS_DEFINES) $(OV_DEFINES)
INCLUDES = $(C_INCLUDES) $(LIBRPC_INCLUDES) $(ACPLTKS_INCLUDES) $(OV_INCLUDES)

#	presupposed libraries
#	---------------------

LIBMPM_LIB			= $(LIBMPM_DIR)libmpm$(LIB)

LIBRPC_LIB			= $(ONCRPC_BIN_DIR)oncrpc$(LIB)

ACPLTKS_LIBS			= $(ACPLT_PLT_BUILD_DIR)libplt$(LIB) $(LIBRPC_LIB)

#	Targets
#	-------

targets: $(TARGETS) $(OV_NTSERVICE_EXE) $(OV_CONTROLPANEL_CPL)

example: $(EXAMPLE)
	@

all: targets example

info: 
	echo $(OV_LIBOV_OBJ) $(LIBMPM_LIB) $(OV_LIBOV_RES)


#   Implicit Rules
#   --------------

.c$(OBJ):
	$(COMPILE_C) $< -o$@

.cpp$(OBJ):
	$(CXX_COMPILE) -Jgx $< -o$@

.rc$(RES):
	$(RC) -r -fo$@ $<

.lex.c:
	$(FLEX) -o$@ $<

.y.c:
	$(BISON) -t -d -o$@ $<

.ovm.c:
	./$(OV_CODEGEN_EXE) -I $(OV_MODEL_DIR) -f $(subst /,\\, $<) -l $(notdir $(basename $<))

.ovm.h:
	./$(OV_CODEGEN_EXE) -I $(OV_MODEL_DIR) -f $(subst /,\\, $<) -l $(notdir $(basename $<))

#   Dependencies
#   ------------

depend:
	@echo Unable to generate dependencies under Borland C++/NT

#	Libraries
#	---------

#	ACPLT/OV library

$(OV_LIBOV_LIB) : $(OV_LIBOV_DLL)

$(OV_LIBOV_DLL) : $(OV_LIBOV_OBJ) $(LIBMPM_LIB) $(OV_LIBOV_RES)
	$(LD) -e$@ $(filter-out %$(RES), $^)
	$(IMPDEF) $(basename $@)_tmp.def $@
	$(MKIMPDEF) $(basename $@)_tmp.def $(basename $@).def
	$(IMPLIB) $(basename $@)$(LIB) $(basename $@).def
	$(MKEXPDEF) $(basename $@)_tmp.def $(basename $@).def
	$(LD) -e$@ $(filter-out %$(RES), $^)
	$(RC) $(filter %$(RES), $^) $@
	$(MKDLLDEF) $(basename $@)_tmp.def $(basename $@).def
	@del $(basename $@)_tmp.def

ov.c ov.h : $(OV_CODEGEN_EXE)

#	ACPLT/OV library for ACPLT/KS integration

$(OV_LIBOVKS_LIB) : $(OV_LIBOVKS_DLL)

$(OV_LIBOVKS_DLL) : $(KS_LIBOVKS_OBJ) $(OV_LIBOVKS_OBJ) $(OV_LIBOV_LIB) $(ACPLTKS_LIBS) $(OV_LIBOVKS_RES)
	$(LD) -e$@ $(filter-out %$(RES), $^)
	$(IMPDEF) $(basename $@)_tmp.def $@
	$(MKIMPDEF) $(basename $@)_tmp.def $(basename $@).def
	$(IMPLIB) $(basename $@)$(LIB) $(basename $@).def
	$(MKEXPDEF) $(basename $@)_tmp.def $(basename $@).def
	$(LD) -e$@ $(filter-out %$(RES), $^)
	$(RC) $(filter %$(RES), $^) $@
	$(MKDLLDEF) $(basename $@)_tmp.def $(basename $@).def
	@del $(basename $@)_tmp.def

ov_ksserver$(OBJ) : $(OV_SOURCE_LIBOVKS_DIR)ov_ksserver.c
	$(CXX_COMPILE) -Jgd $< -o$@

ov_ksclient$(OBJ) : $(OV_SOURCE_LIBOVKS_DIR)ov_ksclient.c
	$(CXX_COMPILE) -Jgx $< -o$@

#	Executables
#	-----------

#	ACPLT/OV C code generator

$(OV_CODEGEN_EXE) : $(OV_CODEGEN_OBJ) $(OV_CODEGEN_RES)
	$(LINK) -e$@ $(filter-out %$(RES), $^) $(C_LIBS)
	$(RC) $(filter %$(RES), $^) $@

#	ACPLT/OV framework builder

$(OV_BUILDER_EXE) : $(OV_BUILDER_OBJ)
	$(LINK) -e$@ $(filter-out %$(RES), $^) $(C_LIBS)

#	ACPLT/OV database utility

$(OV_DBUTIL_EXE) : $(OV_DBUTIL_OBJ) $(OV_LIBOV_LIB) $(OV_DBUTIL_RES)
	$(LINK) -e$@ $(filter-out %$(RES), $^) $(C_LIBS)
	$(RC) $(filter %$(RES), $^) $@

#	ACPLT/KS-Server for ACPLT/OV

$(OV_SERVER_EXE) : $(OV_SERVER_OBJ) $(OV_LIBOVKS_LIB) $(OV_LIBOV_LIB) $(OV_SERVER_RES)
	$(CXX_LINK) -e$@ $(filter-out %$(RES), $^) $(C_LIBS)
	$(RC) $(filter %$(RES), $^) $@

#	ACPLT/KS-Server for ACPLT/OV (Demo version)

ovserver_demo.exe : quitmsgwnd.obj $(OV_SERVER_OBJ) $(OV_LIBOVKS_LIB) $(OV_LIBOV_LIB) $(OV_SERVER_RES)
	$(LINK) -e$@ $(filter-out %$(RES), $^) $(C_LIBS)
	$(RC) $(filter %$(RES), $^) $@

#	ACPLT/KS-Server for ACPLT/OV as Windows NT service

$(OV_NTSERVICE_EXE) : $(OV_NTSERVICE_OBJ) $(OV_LIBOVKS_LIB) $(OV_LIBOV_LIB) $(OV_NTSERVICE_RES)
	$(LINK) -e$@ $(filter-out %$(RES), $^) $(C_LIBS)
	$(RC) $(filter %$(RES), $^) $@

#	ACPLT/OV Control Panel for the Windows NT service

$(OV_CONTROLPANEL_CPL) : $(OV_CONTROLPANEL_OBJ) $(OV_CONTROLPANEL_RES)
	$(LD) -e$@ $(filter-out %$(RES), $^)
	$(RC) $(filter %$(RES), $^) $@

#	ACPLT/OV KsHistory library
#	--------------------------

$(KSHISTLIB_LIB) : $(KSHISTLIB_DLL)

$(KSHISTLIB_DLL) : $(KSHISTLIB_OBJ) $(OV_LIBOVKS_LIB) $(OV_LIBOV_LIB)
	$(LD) -e$@ $(filter-out %$(RES), $^)
	$(IMPDEF) $(basename $@)_tmp.def $@
	$(MKIMPDEF) $(basename $@)_tmp.def $(basename $@).def
	$(IMPLIB) $(basename $@)$(LIB) $(basename $@).def
	$(MKEXPDEF) $(basename $@)_tmp.def $(basename $@).def
	$(LD) -e$@ $(filter-out %$(RES), $^)
	$(MKDLLDEF) $(basename $@)_tmp.def $(basename $@).def
	@del $(basename $@)_tmp.def

kshist.c kshist.h : $(OV_CODEGEN_EXE)

#	ACPLT/OV example library
#	------------------------

$(EXAMPLE_LIB) : $(EXAMPLE_DLL)

$(EXAMPLE_DLL) : $(EXAMPLE_OBJ) $(OV_LIBOVKS_LIB) $(OV_LIBOV_LIB)
	$(LD) -e$@ $(filter-out %$(RES), $^)
	$(IMPDEF) $(basename $@)_tmp.def $@
	$(MKIMPDEF) $(basename $@)_tmp.def $(basename $@).def
	$(IMPLIB) $(basename $@)$(LIB) $(basename $@).def
	$(MKEXPDEF) $(basename $@)_tmp.def $(basename $@).def
	$(LD) -e$@ $(filter-out %$(RES), $^)
	$(MKDLLDEF) $(basename $@)_tmp.def $(basename $@).def
	@del $(basename $@)_tmp.def

example.c example.h : $(OV_CODEGEN_EXE)

#	Install
#	-------

install : all
	@echo Installing files to '$(PLT_BIN_DIR)'
	@-for %i in ( $(ALL) $(OV_NTSERVICE_EXE) $(OV_CONTROLPANEL_CPL) ) do copy %i $(subst /,\, $(PLT_BIN_DIR))
	@echo Done.

#	Clean up
#	--------

clean:
	@del *.c *.h *.bak *.map *.def *$(LIB) *$(DLL) *$(OBJ) *$(EXE) *$(RES) *$(CPL)

#	Include dependencies
#	--------------------

include depend.nt

