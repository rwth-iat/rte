
#   $Id: Makefile,v 1.3 1999-08-28 13:45:55 dirk Exp $
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
#	18-Jun-1999 Dirk Meyer <dirk@plt.rwth-aachen.de>: File created.



#	Plattform
#	---------

SYSTEM = RMOS
SYSDIR = rmos

#	Filename conventions
#	--------------------

OBJ = .obj
LIB = .lib
EXE = .exe
DLL = .dll
RES = .res

#	Platform-specific definitions
#	-----------------------------

ACPLTKS_PLATFORM_DEFINES	= -DFD_SETSIZE=128
OV_PLATFORM_DEFINES			= -DOV_DEBUG

#	Platform-specific C libraries
#	-----------------------------

C_OBJS = $(RMOS_LIBRARY_DIR)cstrtbfp.obj
C_LIBS = rmfsk2ib.lib m7apibl.lib rm3bcc.lib rmfcrifb.lib rmfhlib.lib

#	Compiler
#	--------

COMPILER		= BORLAND

MKLIBTAB		= mklibtab
CODEGEN			= ../nt/$(OV_CODEGEN_EXE)
FLEX			= flex
BISON			= bison
CPP 			= cpp

CC				= bcc32
CC_FLAGS		= -w -v -pc -wsig- -a8 -O2 -d
COMPILE_C		= $(CC) $(CC_FLAGS) $(DEFINES) $(INCLUDES) -c

LINK			= tlink32 -L$(RMOS_LIBRARY_DIR) -Tpe -ap -c
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

#	Targets
#	-------

targets: $(OV_LIBOV_LIB) $(OV_DBUTIL_EXE) $(OV_LIBOVKS_LIB) $(OV_SERVER_EXE)


example: $(EXAMPLE)
	@

all: targets

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
	$(CODEGEN) -I $(OV_MODEL_DIR) -f $(subst /,\\, $<) -l $(notdir $(basename $<))

.ovm.h:
	$(CODEGEN) -I $(OV_MODEL_DIR) -f $(subst /,\\, $<) -l $(notdir $(basename $<))

#   Dependencies
#   ------------

depend:
	@echo Unable to generate dependencies under Borland C++/RMOS

#	Table of statically linked ACPLT/OV libraries
#	---------------------------------------------

OV_LIBRARIES = $(EXAMPLE_LIB)

$(OV_LIBTABLE_SRC) : $(OV_LIBRARIES)
	@echo Generating static library table file '$@'
	$(MKLIBTAB) $@ $(basename $(notdir $^))

#	Libraries
#	---------

#	ACPLT/OV library

$(OV_LIBOV_LIB) : $(OV_LIBOV_OBJ)
	@echo $(foreach object, $?, +-$(notdir $(object))) > respfile
	$(AR) $@ @respfile
	@del respfile


#	ACPLT/OV library for ACPLT/KS integration

$(OV_LIBOVKS_LIB) : $(KS_LIBOVKS_OBJ) $(OV_LIBOVKS_OBJ)
	@echo $(foreach object, $?, +-$(notdir $(object))) > respfile
	$(AR) $@ @respfile
	@del respfile

ov_ksserver$(OBJ) : $(OV_SOURCE_LIBOVKS_DIR)ov_ksserver.c
	$(CXX_COMPILE) -Jgd $< -o$@

ov_ksclient$(OBJ) : $(OV_SOURCE_LIBOVKS_DIR)ov_ksclient.c
	$(CXX_COMPILE) -Jgx $< -o$@

#	ACPLT/OV example library

$(EXAMPLE_LIB) : $(EXAMPLE_OBJ)
	@echo $(foreach object, $?, +-$(object)) > respfile
	$(AR) $@ @respfile
	@del respfile

#	Executables
#	-----------

#	ACPLT/OV database utility

$(OV_DBUTIL_EXE) : $(OV_DBUTIL_OBJ) $(OV_LIBOV_LIB) $(LIBMPM_LIB)
	@echo $(C_OBJS) $(filter %$(OBJ), $^), $@,, $(subst /,\, $(filter %$(LIB), $^ )) $(C_LIBS) > respfile
	$(LINK) @respfile
	@del respfile

#	ACPLT/KS-Server for ACPLT/OV

$(OV_SERVER_EXE) : $(OV_SERVER_OBJ) $(OV_LIBTABLE_OBJ) $(OV_LIBRARIES) $(OV_LIBOVKS_LIB) $(ACPLTKS_LIBS) $(OV_LIBOV_LIB) $(LIBMPM_LIB)
	@echo $(C_OBJS) $(filter %$(OBJ), $^), $@,, $(subst /,\, $(filter %$(LIB), $^ )) $(C_LIBS) > respfile
	$(LINK) @respfile
	@del respfile 

#	Install
#	-------

install : all
	@echo Installing files to '$(PLT_BIN_DIR)'
	@-for %i in ( $(OV_LIBOV_LIB) $(OV_LIBOVKS_LIB) ) do copy %i $(subst /,\, $(PLT_BIN_DIR))
	@echo Done.
	@echo Installing files to c:\siemens\step7\s7proj\test_m7\hrs
	@copy ov_dbutil.exe c:\siemens\step7\s7proj\test_m7\hrs\p0000010\OVdbutil.exe
	@copy ov_server.exe c:\siemens\step7\s7proj\test_m7\hrs\p0000011\OVserver.exe
	@echo Done.

#	Clean up
#	--------

clean:
	@del *.c *.h *.bak *.map *$(LIB) *$(OBJ) *$(EXE) respfile

#	Include dependencies
#	--------------------

include depend.rm

