
#   $Id: msvc.mk,v 1.5 2004-10-29 09:09:31 ansgar Exp $
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

ACPLTKS_PLATFORM_DEFINES	= /DFD_SETSIZE=128
OV_PLATFORM_DEFINES		= /DOV_DEBUG

#	Compiler
#	--------

COMPILER		= MSVC

MKIMPDEF		= mkimpdef
MKEXPDEF		= mkexpdef
MKDLLDEF		= mkdlldef

FLEX			= flex
BISON			= bison
CPP 			= cpp

CC			= cl
CC_FLAGS		= /W3 /c $(OV_PLATFORM_DEFINES)
COMPILE_C		= $(CC) $(CC_FLAGS) /TC $(DEFINES) $(INCLUDES)

LINK_FLAGS              = /nologo /debug
LINK			= link
LD			= link /DLL

CXX			= cl
CXX_FLAGS		= $(CC_FLAGS)
CXX_COMPILE		= $(CXX) $(CXX_FLAGS) /TP $(DEFINES) $(INCLUDES) /c /GX /GR /MT

CXX_LINK		= link

IMPLIB			= cl
IMPDEF			= cl

RC 			= rc /i$(OV_INCLUDE_DIR)

#	Include generic part
#	--------------------

include ../generic.mk

#	Targets
#	-------

targets: $(TARGETS) $(OV_NTSERVICE_EXE) $(OV_CONTROLPANEL_CPL)

all: targets

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

$(OV_LIBOV_LIB) : $(OV_LIBOV_OBJ)

$(OV_LIBOV_DLL) : $(OV_LIBOV_OBJ) $(LIBMPM_LIB) $(OV_LIBOV_RES)
	$(LD) $^ ADVAPI32.LIB USER32.LIB $(LINK_FLAGS) /OUT:$@

ov.c ov.h : $(OV_CODEGEN_EXE)

#	ACPLT/OV library for ACPLT/KS integration

$(OV_LIBOVKS_LIB) : $(OV_LIBOVKS_DLL)

$(OV_LIBOVKS_DLL) : $(KS_LIBOVKS_OBJ) $(OV_LIBOVKS_OBJ) $(OV_LIBOV_LIB) $(ACPLTKS_LIBS) $(LIBMPM_LIB) $(OV_LIBOVKS_RES)
	$(LD) $^ wsock32.lib ADVAPI32.LIB USER32.LIB $(LINK_FLAGS) /NODEFAULTLIB:libc /out:$@ /implib:$(OV_LIBOVKS_LIB)

ov_ksserver$(OBJ) : $(OV_SOURCE_LIBOVKS_DIR)ov_ksserver.c
	$(CXX_COMPILE) $(OV_SOURCE_LIBOVKS_DIR)ov_ksserver.c /out:ov_ksserver$(OBJ)

ov_ksclient$(OBJ) : $(OV_SOURCE_LIBOVKS_DIR)ov_ksclient.c
	$(CXX_COMPILE) $(OV_SOURCE_LIBOVKS_DIR)ov_ksclient.c /out:ov_ksclient$(OBJ)

#	Executables
#	-----------

#	ACPLT/OV C code generator

$(OV_CODEGEN_EXE) : $(OV_CODEGEN_OBJ) $(OV_CODEGEN_RES)
	$(LINK) $^ $(C_LIBS) $(LINK_FLAGS) /out:$@

#	ACPLT/OV framework builder

$(OV_BUILDER_EXE) : $(OV_BUILDER_OBJ) $(OV_BUILDER_RES)
	$(LINK) $^ $(C_LIBS) $(LINK_FLAGS) /out:$(OV_BUILDER_EXE)

#	ACPLT/OV database utility

$(OV_DBUTIL_EXE) : $(OV_DBUTIL_OBJ) $(OV_LIBOV_LIB) $(OV_DBUTIL_RES)
	$(LINK) $^ $(C_LIBS) $(LIBMPM_LIB) ADVAPI32.LIB USER32.LIB $(LINK_FLAGS) /out:$(OV_DBUTIL_EXE)

#	ACPLT/KS-Server for ACPLT/OV

$(OV_SERVER_EXE) : $(OV_SERVER_OBJ) $(OV_LIBOVKS_LIB) $(OV_LIBOV_LIB) $(OV_SERVER_RES)
	$(CXX_LINK) $^ $(C_LIBS) $(LIBMPM_LIB) ADVAPI32.LIB USER32.LIB $(LINK_FLAGS) /out:$(OV_SERVER_EXE)

#	ACPLT/KS-Server for ACPLT/OV (Demo version)

ovserver_demo.exe : quitmsgwnd.obj $(OV_SERVER_OBJ) $(OV_LIBOVKS_LIB) $(OV_LIBOV_LIB) $(OV_SERVER_RES)
	$(LINK)  $^ $(C_LIBS) $(LIBMPM_LIB) ADVAPI32.LIB USER32.LIB $(LINK_FLAGS) /out:ovserver_demo.exe

#	ACPLT/KS-Server for ACPLT/OV as Windows NT service

$(OV_NTSERVICE_EXE) : $(OV_NTSERVICE_OBJ) $(OV_LIBOVKS_LIB) $(OV_LIBOV_LIB) $(OV_NTSERVICE_RES)
	$(LINK)  $^ $(C_LIBS) $(LIBMPM_LIB) ADVAPI32.LIB USER32.LIB SHELL32.LIB $(LINK_FLAGS) /out:$(OV_NTSERVICE_EXE)

#	ACPLT/OV Control Panel for the Windows NT service

$(OV_CONTROLPANEL_CPL) : $(OV_CONTROLPANEL_OBJ)  $(OV_CONTROLPANEL_RES)
	$(LD) $^ ADVAPI32.LIB USER32.LIB COMDLG32.LIB /out:$(OV_CONTROLPANEL_CPL)

#	ACPLT/OV database dumper

fnmatch.obj: fnmatch.c
	cl /GR /DNDEBUG /nologo /DFD_SETSIZE=128 /W3 /MT /GX /DPLT_USE_BUFFERED_STREAMS /DPLT_SYSTEM_NT=1 /DPLT_DEBUG_NEW=0  /I$(INCLUDES) /I$(OV_SOURCE_DBPARSE_DIR)nt/ /TP /c $^

$(DBDUMP_EXE) : $(DBDUMP_OBJ) $(DBDUMP_RES)
	link /MACHINE:I386 /NOLOGO $^ $(LIBPLT_LIB) $(LIBKS_LIB) $(LIBKSCLN_LIB) $(LIBRPC_LIB) ADVAPI32.LIB USER32.LIB wsock32.lib /out:$@

#	ACPLT/OV database parser

db_y.h: db_y.c

db_y.obj: db_y.c
	cl /GR /DNDEBUG /nologo /DFD_SETSIZE=128 /W3 /MT /GX /DPLT_USE_BUFFERED_STREAMS /DPLT_SYSTEM_NT=1 /DPLT_DEBUG_NEW=0  /I$(INCLUDES) /I$(OV_SOURCE_DBPARSE_DIR)nt/ /TP /c $^

db_lex.obj: db_lex.c
	cl /GR /DNDEBUG /nologo /DFD_SETSIZE=128 /W3 /MT /GX /DPLT_USE_BUFFERED_STREAMS /DPLT_SYSTEM_NT=1 /DPLT_DEBUG_NEW=0  /I$(INCLUDES) /I$(OV_SOURCE_DBPARSE_DIR)nt/ /TP /c $^

$(DBPARSE_EXE) : $(DBPARSE_OBJ) $(DBPARSE_RES)
	link /MACHINE:I386 /NOLOGO $^ $(LIBPLT_LIB) $(LIBKS_LIB) $(LIBKSCLN_LIB) $(LIBRPC_LIB) ADVAPI32.LIB USER32.LIB wsock32.lib /out:$@

#	ACPLT/OV makmak

$(MAKMAK_EXE) : $(MAKMAK_OBJ) $(MAKMAK_RES)
	$(LINK) $^ $(C_LIBS) $(LINK_FLAGS) /out:$(MAKMAK_EXE)

#	ACPLT/OV library informations tool

$(LIBINFO_EXE) : $(LIBINFO_OBJ) $(OV_LIBOV_LIB) $(LIBINFO_RES)
	$(LINK) $^ $(C_LIBS) $(LIBMPM_LIB) $(LINK_FLAGS) /out:$(LIBINFO_EXE)

#	ACPLT/OV KsHistory library
#	--------------------------

$(KSHISTLIB_LIB) : $(KSHISTLIB_DLL)

$(KSHISTLIB_DLL) : $(KSHISTLIB_OBJ) $(OV_LIBOVKS_LIB) $(OV_LIBOV_LIB)
	$(LD) /out:$(KSHISTLIB_DLL) /implib:$(KSHISTLIB_LIB) $(KSHISTLIB_OBJ) $(OV_LIBOVKS_LIB) $(OV_LIBOV_LIB) $(LIBMPM_LIB) ADVAPI32.LIB USER32.LIB

kshist.c kshist.h : $(OV_CODEGEN_EXE)

#	ACPLT/OV dynov library
#	----------------------

$(DYNOV_LIB) : $(DYNOV_DLL)

$(DYNOV_DLL) : $(DYNOV_OBJ) $(OV_LIBOVKS_LIB) $(OV_LIBOV_LIB)
	$(LD) /out:$(DYNOV_DLL) /implib:$(DYNOV_LIB) $(DYNOV_OBJ) $(OV_LIBOVKS_LIB) $(OV_LIBOV_LIB) $(LIBMPM_LIB) ADVAPI32.LIB USER32.LIB

dynov.c dynov.h : $(OV_CODEGEN_EXE)

#	ACPLT/OV tasking library
#	------------------------

$(TASKLIB_LIB) : $(TASKLIB_DLL)

$(TASKLIB_DLL) : $(TASKLIB_OBJ) $(OV_LIBOVKS_LIB) $(OV_LIBOV_LIB)
	$(LD) /out:$(TASKLIB_DLL) /implib:$(TASKLIB_LIB) $(TASKLIB_OBJ) $(OV_LIBOVKS_LIB) $(OV_LIBOV_LIB) $(LIBMPM_LIB) ADVAPI32.LIB USER32.LIB

tasklib.c tasklib.h : $(OV_CODEGEN_EXE)

#	ACPLT/OV example library
#	------------------------

$(EXAMPLE_LIB) : $(EXAMPLE_DLL)

$(EXAMPLE_DLL) : $(EXAMPLE_OBJ) $(OV_LIBOVKS_LIB) $(OV_LIBOV_LIB)
	$(LD) $(LINK_FLAGS) /out:$(EXAMPLE_DLL) /implib:$(EXAMPLE_LIB) $(EXAMPLE_OBJ) $(OV_LIBOVKS_LIB) $(OV_LIBOV_LIB) $(LIBMPM_LIB) ADVAPI32.LIB USER32.LIB

example.c example.h : $(OV_CODEGEN_EXE)

#	Install
#	-------

install : all
	@echo Installing files to '$(PLT_BIN_DIR)'
	@cmd /C for %i in ($(filter-out %$(LIB), $(ALL) $(OV_NTSERVICE_EXE) $(OV_CONTROLPANEL_CPL)) ) do copy %i $(subst /,\, $(PLT_BIN_DIR))
	@echo Installing files to '$(PLT_LIB_DIR)'
	@cmd /C for %i in ($(filter %$(LIB), $(ALL) $(OV_NTSERVICE_EXE) $(OV_CONTROLPANEL_CPL)) ) do copy %i $(subst /,\, $(PLT_LIB_DIR))
	@echo Done.

#	Clean up
#	--------

clean:
	@del *.c *.h *.bak *.map *.def *.exp *$(LIB) *$(DLL) *$(OBJ) *$(EXE) *$(RES) *$(CPL) *.pdb *.ilk

#	Include dependencies
#	--------------------

include depend.nt

