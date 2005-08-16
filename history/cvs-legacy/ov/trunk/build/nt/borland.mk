
#   $Id: borland.mk,v 1.7 2005-08-16 13:02:08 markus Exp $
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

_OBJ = .obj
_LIB = .lib
_DLL = .dll
_EXE = .exe
_RES = .res
_CPL = .cpl

#	Platform-specific definitions
#	-----------------------------

ACPLTKS_PLATFORM_DEFINES		= -DFD_SETSIZE=128
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

RC 			= brc32 -i$(OV_INCLUDE_DIR)

#	Include generic part
#	--------------------

include ../generic.mk

#	Targets
#	-------

targets: $(TARGETS) $(OV_NTSERVICE_EXE) $(OV_CONTROLPANEL_CPL)

all: targets

info:
	echo $(OV_LIBOV_OBJ) $(LIBMPM_LIB) $(OV_LIBOV_RES)


#   Implicit Rules
#   --------------

.c$(_OBJ):
	$(COMPILE_C) $< -o$@

.cpp$(_OBJ):
	$(CXX_COMPILE) -Jgx $< -o$@

.rc$(_RES):
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
	$(LD) -e$@ $(filter-out %$(_RES), $^)
	$(IMPDEF) $(basename $@)_tmp.def $@
	$(MKIMPDEF) $(basename $@)_tmp.def $(basename $@).def
	$(IMPLIB) $(basename $@)$(_LIB) $(basename $@).def
	$(MKEXPDEF) $(basename $@)_tmp.def $(basename $@).def
	$(LD) -e$@ $(filter-out %$(_RES), $^)
	$(RC) $(filter %$(_RES), $^) $@
	$(MKDLLDEF) $(basename $@)_tmp.def $(basename $@).def
	@del $(basename $@)_tmp.def

ov.c ov.h : $(OV_CODEGEN_EXE)

#	ACPLT/OV library for ACPLT/KS integration

$(OV_LIBOVKS_LIB) : $(OV_LIBOVKS_DLL)

$(OV_LIBOVKS_DLL) : $(KS_LIBOVKS_OBJ) $(OV_LIBOVKS_OBJ) $(OV_LIBOV_LIB) $(ACPLTKS_LIBS) $(OV_LIBOVKS_RES)
	$(LD) -e$@ $(filter-out %$(_RES), $^)
	$(IMPDEF) $(basename $@)_tmp.def $@
	$(MKIMPDEF) $(basename $@)_tmp.def $(basename $@).def
	$(IMPLIB) $(basename $@)$(_LIB) $(basename $@).def
	$(MKEXPDEF) $(basename $@)_tmp.def $(basename $@).def
	$(LD) -e$@ $(filter-out %$(_RES), $^)
	$(RC) $(filter %$(_RES), $^) $@
	$(MKDLLDEF) $(basename $@)_tmp.def $(basename $@).def
	@del $(basename $@)_tmp.def

ov_ksserver$(_OBJ) : $(OV_SOURCE_LIBOVKS_DIR)ov_ksserver.c
	$(CXX_COMPILE) -Jgd $< -o$@

ov_ksclient$(_OBJ) : $(OV_SOURCE_LIBOVKS_DIR)ov_ksclient.c
	$(CXX_COMPILE) -Jgx $< -o$@

#	Executables
#	-----------

#	ACPLT/OV C code generator

$(OV_CODEGEN_EXE) : $(OV_CODEGEN_OBJ) $(OV_CODEGEN_RES)
	$(LINK) -e$@ $(filter-out %$(_RES), $^) $(C_LIBS)
	$(RC) $(filter %$(_RES), $^) $@

#	ACPLT/OV framework builder

$(OV_BUILDER_EXE) : $(OV_BUILDER_OBJ)
	$(LINK) -e$@ $(filter-out %$(_RES), $^) $(C_LIBS)

#	ACPLT/OV database utility

$(OV_DBUTIL_EXE) : $(OV_DBUTIL_OBJ) $(OV_LIBOV_LIB) $(OV_DBUTIL_RES)
	$(LINK) -e$@ $(filter-out %$(_RES), $^) $(C_LIBS)
	$(RC) $(filter %$(_RES), $^) $@

#	ACPLT/KS-Server for ACPLT/OV

$(OV_SERVER_EXE) : $(OV_SERVER_OBJ) $(OV_LIBOVKS_LIB) $(OV_LIBOV_LIB) $(OV_SERVER_RES)
	$(CXX_LINK) -e$@ $(filter-out %$(_RES), $^) $(C_LIBS)
	$(RC) $(filter %$(_RES), $^) $@

#	ACPLT/KS-Server for ACPLT/OV (Demo version)

ovserver_demo.exe : quitmsgwnd.obj $(OV_SERVER_OBJ) $(OV_LIBOVKS_LIB) $(OV_LIBOV_LIB) $(OV_SERVER_RES)
	$(LINK) -e$@ $(filter-out %$(_RES), $^) $(C_LIBS)
	$(RC) $(filter %$(_RES), $^) $@

#	ACPLT/KS-Server for ACPLT/OV as Windows NT service

$(OV_NTSERVICE_EXE) : $(OV_NTSERVICE_OBJ) $(OV_LIBOVKS_LIB) $(OV_LIBOV_LIB) $(OV_NTSERVICE_RES)
	$(LINK) -e$@ $(filter-out %$(_RES), $^) $(C_LIBS)
	$(RC) $(filter %$(_RES), $^) $@

#	NT-Service Installer

$(NTSERVICE_INSTALL_EXE) : $(NTSERVICE_INSTALL_OBJ)
	$(LINK) -e$@ $(filter-out %$(_RES), $^) $(C_LIBS)
	$(RC) $(filter %$(_RES), $^) $@

#	ACPLT/OV Control Panel for the Windows NT service

$(OV_CONTROLPANEL_CPL) : $(OV_CONTROLPANEL_OBJ) $(OV_CONTROLPANEL_RES)
	$(LD) -e$@ $(filter-out %$(_RES), $^)
	$(RC) $(filter %$(_RES), $^) $@

#	ACPLT/OV database dumper

$(DBDUMP_EXE) : $(DBDUMP_OBJ) $(DBDUMP_RES)
	$(LINK) -e$@ $(filter-out %$(_RES), $^) $(C_LIBS)

#	ACPLT/OV database parser

$(DBPARSE_EXE) : $(DBPARSE_OBJ) $(DBPARSE_RES)
	$(LINK) -e$@ $(filter-out %$(_RES), $^) $(C_LIBS)

#	ACPLT/OV makmak

$(MAKMAK_EXE) : $(MAKMAK_OBJ) $(MAKMAK_RES)
	$(LINK) -e$@ $(filter-out %$(_RES), $^) $(C_LIBS)

#	ACPLT/OV library informations tool

$(LIBINFO_EXE) : $(LIBINFO_OBJ) $(OV_LIBOV_LIB) $(LIBINFO_RES)
	$(LINK) -e$@ $(filter-out %$(_RES), $^) $(C_LIBS)
	$(RC) $(filter %$(_RES), $^) $@

#	ACPLT/OV KsHistory library
#	--------------------------

$(KSHISTLIB_LIB) : $(KSHISTLIB_DLL)

$(KSHISTLIB_DLL) : $(KSHISTLIB_OBJ) $(OV_LIBOVKS_LIB) $(OV_LIBOV_LIB)
	$(LD) -e$@ $(filter-out %$(_RES), $^)
	$(IMPDEF) $(basename $@)_tmp.def $@
	$(MKIMPDEF) $(basename $@)_tmp.def $(basename $@).def
	$(IMPLIB) $(basename $@)$(_LIB) $(basename $@).def
	$(MKEXPDEF) $(basename $@)_tmp.def $(basename $@).def
	$(LD) -e$@ $(filter-out %$(_RES), $^)
	$(MKDLLDEF) $(basename $@)_tmp.def $(basename $@).def
	@del $(basename $@)_tmp.def

kshist.c kshist.h : $(OV_CODEGEN_EXE)

#	ACPLT/OV dynov library
#	----------------------

$(DYNOV_LIB) : $(DYNOV_DLL)

$(DYNOV_DLL) : $(DYNOV_OBJ) $(OV_LIBOVKS_LIB) $(OV_LIBOV_LIB)
	$(LD) -e$@ $(filter-out %$(_RES), $^)
	$(IMPDEF) $(basename $@)_tmp.def $@
	$(MKIMPDEF) $(basename $@)_tmp.def $(basename $@).def
	$(IMPLIB) $(basename $@)$(_LIB) $(basename $@).def
	$(MKEXPDEF) $(basename $@)_tmp.def $(basename $@).def
	$(LD) -e$@ $(filter-out %$(_RES), $^)
	$(MKDLLDEF) $(basename $@)_tmp.def $(basename $@).def
	@del $(basename $@)_tmp.def

dynov.c dynov.h : $(OV_CODEGEN_EXE)

#	ACPLT/OV tasking library
#	------------------------

$(TASKLIB_LIB) : $(TASKLIB_DLL)

$(TASKLIB_DLL) : $(TASKLIB_OBJ) $(OV_LIBOVKS_LIB) $(OV_LIBOV_LIB)
	$(LD) -e$@ $(filter-out %$(_RES), $^)
	$(IMPDEF) $(basename $@)_tmp.def $@
	$(MKIMPDEF) $(basename $@)_tmp.def $(basename $@).def
	$(IMPLIB) $(basename $@)$(_LIB) $(basename $@).def
	$(MKEXPDEF) $(basename $@)_tmp.def $(basename $@).def
	$(LD) -e$@ $(filter-out %$(_RES), $^)
	$(MKDLLDEF) $(basename $@)_tmp.def $(basename $@).def
	@del $(basename $@)_tmp.def

tasklib.c tasklib.h : $(OV_CODEGEN_EXE)

#	ACPLT/OV example library
#	------------------------

$(EXAMPLE_LIB) : $(EXAMPLE_DLL)

$(EXAMPLE_DLL) : $(EXAMPLE_OBJ) $(OV_LIBOVKS_LIB) $(OV_LIBOV_LIB)
	$(LD) -e$@ $(filter-out %$(_RES), $^)
	$(IMPDEF) $(basename $@)_tmp.def $@
	$(MKIMPDEF) $(basename $@)_tmp.def $(basename $@).def
	$(IMPLIB) $(basename $@)$(_LIB) $(basename $@).def
	$(MKEXPDEF) $(basename $@)_tmp.def $(basename $@).def
	$(LD) -e$@ $(filter-out %$(_RES), $^)
	$(MKDLLDEF) $(basename $@)_tmp.def $(basename $@).def
	@del $(basename $@)_tmp.def

example.c example.h : $(OV_CODEGEN_EXE)

#	Install
#	-------

install : all
	@echo Installing files to '$(ACPLT_BIN_DIR)'
	@-for %i in ($(filter-out %$(_LIB), $(ALL) $(OV_NTSERVICE_EXE) $(OV_CONTROLPANEL_CPL)) ) do copy %i $(subst /,\, $(ACPLT_BIN_DIR))
	@echo Installing files to '$(ACPLT_LIB_DIR)'
	@-for %i in ($(filter %$(_LIB), $(ALL) $(OV_NTSERVICE_EXE) $(OV_CONTROLPANEL_CPL)) ) do copy %i $(subst /,\, $(ACPLT_LIB_DIR))
	@echo Done.

#	Clean up
#	--------

clean:
	@del *.c *.h *.bak *.map *.def *$(_LIB) *$(_DLL) *$(_OBJ) *$(_EXE) *$(_RES) *$(_CPL)

#	Include dependencies
#	--------------------

include depend.nt

