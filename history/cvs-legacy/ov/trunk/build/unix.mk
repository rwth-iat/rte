
#   $Id: unix.mk,v 1.12 2004-10-27 12:09:05 ansgar Exp $
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
#	17-Jun-1998 Dirk Meyer <dirk@plt.rwth-aachen.de>: File created.
#	16-Apr-1999 Dirk Meyer <dirk@plt.rwth-aachen.de>: Major revision.
#	09-Jul-2001 Ansgar Münnemann <ansgar@plt.rwth-aachen.de>: ov_builder included.



#	Filename conventions
#	--------------------

OBJ = .o
LIB = .a
DLL = .so
EXE =

#	Platform-specific definitions
#	-----------------------------

ACPLTKS_PLATFORM_DEFINES	=
OV_PLATFORM_DEFINES			= -DOV_DEBUG

#	Compiler
#	--------

COMPILER		= GNU

FLEX			= flex
BISON			= bison

CC				= gcc
CC_FLAGS		= -d -g -Wall -O2 -shared
COMPILE_C		= $(CC) $(CC_FLAGS) $(DEFINES) $(INCLUDES) -c

LINK			= $(CC)
C_LIBS			=

LD				= $(CC) -shared
LD_LIB			= -ldl

CXX			= gcc -x c++
CXX_FLAGS		= $(CC_FLAGS) -fno-implicit-templates
CXX_COMPILE		= $(CXX) $(CXX_FLAGS) $(DEFINES) $(INCLUDES) -c

# CXX_LINK		= $(CXX)
CXX_LINK 		= MAKE=$(MAKE) perl ../templ.pl gcc
CXX_LIBS		= $(C_LIBS) -lstdc++

AR				= ar
RANLIB			= ranlib

#   Include generic part
#   --------------------

include ../generic.mk

#	all defines and includes together
#	---------------------------------

DEFINES	 = $(LIBRPC_DEFINES) $(ACPLTKS_DEFINES) $(OV_DEFINES)
INCLUDES = $(C_INCLUDES) $(LIBRPC_INCLUDES) $(ACPLTKS_INCLUDES) $(OV_INCLUDES)

#	presupposed libraries
#	---------------------

LIBMPM_LIB			= $(LIBMPM_DIR)libmpm$(LIB)

ACPLTKS_LIBS			= $(ACPLT_PLT_BUILD_DIR)libplt$(LIB) $(LIBRPC_LIB)

#	Targets
#	-------

targets: $(TARGETS)

example: $(EXAMPLE)
	@

all: targets example

#   Implicit Rules
#   --------------

.c$(OBJ):
	$(COMPILE_C) $< -o $@

.cpp$(OBJ):
	$(CXX_COMPILE) $< -o $@

.lex.c:
	$(FLEX) -o$@ $<

.y.c:
	$(BISON) -t -d -o$@ $<

.ovm.c:
	./$(OV_CODEGEN_EXE) -I $(OV_MODEL_DIR) -f $< -l $(notdir $(basename $<))

.ovm.h:
	./$(OV_CODEGEN_EXE) -I $(OV_MODEL_DIR) -f $< -l $(notdir $(basename $<))

#   Dependencies
#   ------------

.depend:
	touch $@

depend : $(SOURCES)
	$(CXX_COMPILE) -MM $^ > .depend
	perl ../depend_nt.pl .depend > ../nt/depend.nt
	perl ../depend_rm.pl .depend > ../rmos/depend.rm
	perl ../depend_mc.pl .depend > ../mc164/depend.mc
	perl ../depend_vms.pl .depend > ../openvms/depend.vms

#	Libraries
#	---------

#	ACPLT/OV library

$(OV_LIBOV_LIB) : $(OV_LIBOV_OBJ)
	$(AR) rv $@ $?
	$(RANLIB) $@

$(OV_LIBOV_DLL) : $(OV_LIBOV_OBJ) $(LIBMPM_LIB)
	$(LD) -o $@ $^

ov.c ov.h : $(OV_CODEGEN_EXE)

#	ACPLT/OV library for ACPLT/KS integration

$(OV_LIBOVKS_LIB) : $(KS_LIBOVKS_OBJ) $(OV_LIBOVKS_OBJ)
	$(AR) rv $@ $?
	$(RANLIB) $@

$(OV_LIBOVKS_DLL) : $(KS_LIBOVKS_OBJ) $(OV_LIBOVKS_OBJ) $(ACPLTKS_LIBS)
	$(LD) -o $@ $^ $(CXX_LIBS)

ov_ksserver$(OBJ) : $(OV_SOURCE_LIBOVKS_DIR)ov_ksserver.c
	$(CXX_COMPILE) -o $@ $<

ov_ksclient$(OBJ) : $(OV_SOURCE_LIBOVKS_DIR)ov_ksclient.c
	$(CXX_COMPILE) -o $@ $<

#	Executables
#	-----------

#	ACPLT/OV C code generator

$(OV_CODEGEN_EXE) : $(OV_CODEGEN_OBJ)
	$(LINK) -o $@ $^ $(C_LIBS)

#	ACPLT/OV framework builder

$(OV_BUILDER_EXE) : $(OV_BUILDER_OBJ)
	$(LINK) -o $@ $^ $(C_LIBS)

#	ACPLT/OV database utility

$(OV_DBUTIL_EXE) : $(OV_DBUTIL_OBJ) $(OV_LIBOV_DLL)
	$(LINK) -rdynamic -o $@ $^ $(C_LIBS) $(LD_LIB)

#	ACPLT/KS-Server for ACPLT/OV

$(OV_SERVER_EXE) : $(OV_SERVER_OBJ) $(OV_LIBOVKS_DLL) $(OV_LIBOV_DLL)
	$(CXX_LINK) -rdynamic -o $@ $^ $(C_LIBS) $(LD_LIB)

#	ACPLT/OV database dumper

$(DBDUMP_EXE) : $(DBDUMP_OBJ)
	$(CXX_LINK) -o $@ $^ $(LIBKSCLN_LIB) $(LIBKS_LIB) $(LIBPLT_LIB) $(CXX_LIBS)

#	ACPLT/OV database parser

db_y.h: db_y.c


db_lex.o: db_lex.c
	$(CXX_COMPILE) -o $@ $<

db_y.o: db_y.c
	$(CXX_COMPILE) -o $@ $<

$(DBPARSE_EXE) : $(DBPARSE_OBJ)  
	$(CXX_LINK) -o $@ $^ $(LIBKSCLN_LIB) $(LIBKS_LIB) $(LIBPLT_LIB) $(CXX_LIBS)

#	ACPLT/OV makmak

$(MAKMAK_EXE) : $(MAKMAK_OBJ)
	$(LINK) -o $@ $^ $(C_LIBS)

#	ACPLT/OV library informations tool

$(LIBINFO_EXE) : $(LIBINFO_OBJ) $(OV_LIBOV_DLL)
	$(LINK) -rdynamic -o $@ $^ $(C_LIBS) $(LIBMPM_LIB) $(LD_LIB)


#	ACPLT/OV KsHistory library
#	--------------------------

$(KSHISTLIB_LIB) : $(KSHISTLIB_OBJ)
	$(AR) rv $@ $?
	$(RANLIB) $@

$(KSHISTLIB_DLL) : $(KSHISTLIB_OBJ)
	$(LD) -o $@ $^

kshist.c kshist.h : $(OV_CODEGEN_EXE)

#	ACPLT/OV dynov library
#	--------------------------

$(DYNOV_LIB) : $(DYNOV_OBJ)
	$(AR) rv $@ $?
	$(RANLIB) $@

$(DYNOV_DLL) : $(DYNOV_OBJ)
	$(LD) -o $@ $^

dynov.c dynov.h : $(OV_CODEGEN_EXE)

#	ACPLT/OV tasklib library
#	--------------------------

$(TASKLIB_LIB) : $(TASKLIB_OBJ)
	$(AR) rv $@ $?
	$(RANLIB) $@

$(TASKLIB_DLL) : $(TASKLIB_OBJ)
	$(LD) -o $@ $^

tasklib.c tasklib.h : $(OV_CODEGEN_EXE)

#	ACPLT/OV example library
#	------------------------

$(EXAMPLE_LIB) : $(EXAMPLE_OBJ)
	$(AR) rv $@ $?
	$(RANLIB) $@

$(EXAMPLE_DLL) : $(EXAMPLE_OBJ)
	$(LD) -o $@ $^

example.c example.h : $(OV_CODEGEN_EXE)

#	Install
#	-------

install : all
	@echo Installing files to '$(PLT_BIN_DIR)'
	@-cp *.so $(PLT_BIN_DIR)
	@-cp $(OV_CODEGEN_EXE) $(OV_BUILDER_EXE) $(OV_DBUTIL_EXE) $(OV_SERVER_EXE) \
		 $(DBDUMP_EXE) $(DBPARSE_EXE) $(MAKMAK_EXE) $(LIBINFO_EXE) $(PLT_BIN_DIR)
	@echo Installing files to '$(PLT_LIB_DIR)'
	@-cp *.a $(PLT_LIB_DIR)
	@echo Done.

#	Clean up
#	--------

clean :
	@echo Cleaning up...
	@rm -f core *.c ov.h example.h dynov.h kshist.h db_y.h db_lex.h \
		*$(LIB) *$(DLL) *$(OBJ) $(OV_CODEGEN_EXE) $(OV_BUILDER_EXE) \
		$(OV_DBUTIL_EXE) $(OV_SERVER_EXE) $(DBDUMP_EXE) $(DBPARSE_EXE) $(MAKMAK_EXE) $(LIBINFO_EXE)
	@for i in *_inst.h ; do echo > $$i ; done
	@echo Done.

#	Include dependencies
#	--------------------

include .depend

