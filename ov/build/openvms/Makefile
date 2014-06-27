
#   $Id: Makefile,v 1.6 2005-08-17 08:20:47 ansgar Exp $
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
#	08-Jul-1999 Dirk Meyer <dirk@plt.rwth-aachen.de>: File created.



#	Plattform
#	---------

SYSTEM = OPENVMS
SYSDIR = openvms

#	Filename conventions
#	--------------------

_OBJ = .obj
_LIB = .olb
_EXE = .exe

#	Directories
#	-----------

PLT_DIR						= [-.-.-.-]
PLT_BIN_DIR					= [-.-.-.-.bin.openvms]

ACPLT_OV_DIR				= [-.-]
OV_INCLUDE_DIR				= ../../include/
OV_MODEL_DIR				= [-.-.model]
OV_SOURCE_DIR				= [-.-.source]

OV_SOURCE_CODEGEN_DIR		= [-.-.source.codegen]
OV_SOURCE_LIBOV_DIR			= [-.-.source.libov]
OV_SOURCE_DBUTIL_DIR		= [-.-.source.dbutil]
OV_SOURCE_LIBOVKS_DIR		= [-.-.source.libovks]
OV_SOURCE_SERVER_DIR		= [-.-.source.server]
OV_SOURCE_EXAMPLE_DIR		= [-.-.source.example]

ACPLT_PLT_DIR				= [-.-.-.plt]

ACPLT_KS_DIR				= [-.-.-.ks]
ACPLT_PLT_INCLUDE_DIR		= ../../../plt/include/
ACPLT_KS_INCLUDE_DIR		= ../../../ks/include/
ACPLT_KS_SOURCE_DIR			= [-.-.-.ks.src]
ACPLT_PLT_BUILD_DIR			= [-.-.-.plt.build.openvms]

LIBMPM_DIR					= [-.-.-.-.libmpm]

LIBRPC_DIR					= sys$$sysdevice:[vms$$common.ucx$$lib.rpc]

REPOSITORY					= acplt$$repository

#	Platform-specific definitions
#	-----------------------------

ACPLTKS_PLATFORM_DEFINES	=
OV_DEBUG					=
ifdef OV_DEBUG
OV_DODEBUG	=	-DOV_DEBUG
else
OV_DODEBUG	=
endif
OV_PLATFORM_DEFINES			= $(OV_DODEBUG)

#	Special characters
#	------------------

COMMA = ,
SPACE = $(empty) $(empty)

#	Compiler
#	--------

COMPILER		= DEC

FLEX			= flex
BISON			= bison

MKLIBTAB		= mc []mklibtab.exe

#DEBUG			= /debug
DEBUG			= 

CC				= cc
CC_FLAGS		= /float=ieee_float/prefix=all /optimize $(DEBUG)
COMPILE_C		= $(CC) $(CC_FLAGS) \
					/define=($(patsubst -D%,"%"$(COMMA), $(DEFINES)) "") \
					/include=($(patsubst -I%,"%"$(COMMA), $(INCLUDES)) "[]")

LINK			= link
LINK_FLAGS		= $(DEBUG)

CXX				= cxx
CXX_FLAGS		= $(CC_FLAGS) /notemplate_define /repository=($(REPOSITORY))
CXX_COMPILE		= $(CXX) $(CXX_FLAGS) \
					/define=($(patsubst -D%,"%"$(COMMA), $(DEFINES)) "") \
					/include=($(patsubst -I%,"%"$(COMMA), $(INCLUDES)) "[]")

CXX_LINK		= link

AR	 			= library

#	C-Includes
#	----------
 
C_INCLUDES =
 
#	ACPLT/KS definitions and includes
#	---------------------------------

ACPLTKS_DEFINES = \
	$(ACPLTKS_PLATFORM_DEFINES) \
	-DPLT_SYSTEM_$(SYSTEM)=1 \
	-DNDEBUG \
	-DPLT_USE_BUFFERED_STREAMS=1 \
	-DPLT_SERVER_TRUNC_ONLY=1
ACPLTKS_INCLUDES = \
	-I$(ACPLT_PLT_INCLUDE_DIR) \
	-I$(ACPLT_KS_INCLUDE_DIR)

#	ACPLT/OV definitions and includes
#	---------------------------------

OV_DEFINES = \
	$(OV_PLATFORM_DEFINES) \
	-DOV_SYSTEM_$(SYSTEM)=1

OV_INCLUDES = \
	-I$(OV_INCLUDE_DIR) \
	-I$(OV_MODEL_DIR) \
	-I$(OV_SOURCE_CODEGEN_DIR) \
	-I$(OV_SOURCE_EXAMPLE_DIR) \
	-I$(LIBMPM_DIR)

#	all defines and includes together
#	---------------------------------

DEFINES	 = $(LIBRPC_DEFINES) $(ACPLTKS_DEFINES) $(OV_DEFINES)
INCLUDES = $(C_INCLUDES) $(LIBRPC_INCLUDES) $(ACPLTKS_INCLUDES) $(OV_INCLUDES) -I.

#	presupposed libraries
#	---------------------

LIBMPM_LIB			= $(LIBMPM_DIR)libmpm$(_LIB)
LIBRPC_LIB			= $(LIBRPC_DIR)ucx$$rpcxdr$(_LIB)
LIBPLT_LIB			= $(ACPLT_PLT_BUILD_DIR)libplt$(_LIB)

#	Rules
#	-----

.SUFFIXES:

.SUFFIXES: .c .cpp .h $(_LIB) $(_OBJ) $(_EXE)

#	ACPLT/OV C code generator
#	-------------------------

OV_CODEGEN_SRC = $(OV_SOURCE_CODEGEN_DIR)ov_codegen.c	
OV_CODEGEN_OBJ = $(foreach source, $(OV_CODEGEN_SRC), \
	$(basename $(notdir $(source)))$(_OBJ))
OV_CODEGEN_EXE = ov_codegen$(_EXE)

#	ACPLT/OV library
#	----------------

OV_LIBOV_SRC = \
	$(OV_SOURCE_LIBOV_DIR)ov_association.c \
	$(OV_SOURCE_LIBOV_DIR)ov_childrelationship.c \
	$(OV_SOURCE_LIBOV_DIR)ov_class.c \
	$(OV_SOURCE_LIBOV_DIR)ov_construction.c \
	$(OV_SOURCE_LIBOV_DIR)ov_containment.c \
	$(OV_SOURCE_LIBOV_DIR)ov_database.c \
	$(OV_SOURCE_LIBOV_DIR)ov_debug.c \
	$(OV_SOURCE_LIBOV_DIR)ov_element.c \
	$(OV_SOURCE_LIBOV_DIR)ov_embedment.c \
	$(OV_SOURCE_LIBOV_DIR)ov_inheritance.c \
	$(OV_SOURCE_LIBOV_DIR)ov_instantiation.c \
	$(OV_SOURCE_LIBOV_DIR)ov_library.c \
	$(OV_SOURCE_LIBOV_DIR)ov_logfile.c \
	$(OV_SOURCE_LIBOV_DIR)ov_malloc.c \
	$(OV_SOURCE_LIBOV_DIR)ov_memstack.c \
	$(OV_SOURCE_LIBOV_DIR)ov_object.c \
	$(OV_SOURCE_LIBOV_DIR)ov_operation.c \
	$(OV_SOURCE_LIBOV_DIR)ov_parentrelationship.c \
	$(OV_SOURCE_LIBOV_DIR)ov_part.c \
	$(OV_SOURCE_LIBOV_DIR)ov_path.c \
	$(OV_SOURCE_LIBOV_DIR)ov_result.c \
	$(OV_SOURCE_LIBOV_DIR)ov_scheduler.c \
	$(OV_SOURCE_LIBOV_DIR)ov_string.c \
	$(OV_SOURCE_LIBOV_DIR)ov_structure.c \
	$(OV_SOURCE_LIBOV_DIR)ov_supervise.c \
	$(OV_SOURCE_LIBOV_DIR)ov_time.c \
	$(OV_SOURCE_LIBOV_DIR)ov_variable.c \
	$(OV_SOURCE_LIBOV_DIR)ov_vector.c \
	$(OV_SOURCE_LIBOV_DIR)ov_vendortree.c
OV_LIBOV_OBJ = $(foreach source, $(OV_LIBOV_SRC), \
	$(basename $(notdir $(source)))$(_OBJ))
OV_LIBOV_LIB = libov$(_LIB)

#	ACPLT/OV database utility
#	-------------------------

OV_DBUTIL_SRC = $(OV_SOURCE_DBUTIL_DIR)ov_dbutil.c	
OV_DBUTIL_OBJ = $(foreach source, $(OV_DBUTIL_SRC), \
	$(basename $(notdir $(source)))$(_OBJ))
OV_DBUTIL_EXE = ov_dbutil$(_EXE)

#	ACPLT/OV library for ACPLT/KS integration, KS part
#	--------------------------------------------------

KS_LIBOVKS_SRC = \
	$(ACPLT_KS_SOURCE_DIR)avticket.cpp \
	$(ACPLT_KS_SOURCE_DIR)connection.cpp \
	$(ACPLT_KS_SOURCE_DIR)connectionmgr.cpp \
	$(ACPLT_KS_SOURCE_DIR)event.cpp \
	$(ACPLT_KS_SOURCE_DIR)interserver.cpp \
	$(ACPLT_KS_SOURCE_DIR)register.cpp \
	$(ACPLT_KS_SOURCE_DIR)result.cpp \
	$(ACPLT_KS_SOURCE_DIR)rpc.cpp \
	$(ACPLT_KS_SOURCE_DIR)rpcproto.cpp \
	$(ACPLT_KS_SOURCE_DIR)server.cpp \
	$(ACPLT_KS_SOURCE_DIR)string.cpp \
	$(ACPLT_KS_SOURCE_DIR)svrbase.cpp \
	$(ACPLT_KS_SOURCE_DIR)svrrpcctx.cpp \
	$(ACPLT_KS_SOURCE_DIR)svrtransport.cpp \
	$(ACPLT_KS_SOURCE_DIR)time.cpp \
	$(ACPLT_KS_SOURCE_DIR)xdr.cpp \
	$(ACPLT_KS_SOURCE_DIR)xdrmemstream.cpp \
	$(ACPLT_KS_SOURCE_DIR)xdrtcpcon.cpp \
	$(ACPLT_KS_SOURCE_DIR)xdrudpcon.cpp
KS_LIBOVKS_OBJ = $(foreach source, $(KS_LIBOVKS_SRC), \
	$(basename $(notdir $(source)))$(_OBJ))

#	ACPLT/OV library for ACPLT/KS integration, OV part
#	--------------------------------------------------

OV_LIBOVKS_SRC = \
	$(OV_SOURCE_LIBOVKS_DIR)ov_ksclient_xdr.c \
	$(OV_SOURCE_LIBOVKS_DIR)ov_ksserver_createobject.c \
	$(OV_SOURCE_LIBOVKS_DIR)ov_ksserver_deleteobject.c \
	$(OV_SOURCE_LIBOVKS_DIR)ov_ksserver_exgdata.c \
	$(OV_SOURCE_LIBOVKS_DIR)ov_ksserver_getcanonicalpath.c \
	$(OV_SOURCE_LIBOVKS_DIR)ov_ksserver_getep.c \
	$(OV_SOURCE_LIBOVKS_DIR)ov_ksserver_getpp.c \
	$(OV_SOURCE_LIBOVKS_DIR)ov_ksserver_getvar.c \
	$(OV_SOURCE_LIBOVKS_DIR)ov_ksserver_link.c \
	$(OV_SOURCE_LIBOVKS_DIR)ov_ksserver_renameobject.c \
	$(OV_SOURCE_LIBOVKS_DIR)ov_ksserver_setvar.c \
	$(OV_SOURCE_LIBOVKS_DIR)ov_ksserver_unlink.c \
	$(OV_SOURCE_LIBOVKS_DIR)ov_ksserver_xdr.c
OV_LIBOVKS_OBJ = $(foreach source, $(OV_LIBOVKS_SRC), \
	$(basename $(notdir $(source)))$(_OBJ))
OV_LIBOVKS_LIB = libovks$(_LIB)

#	ACPLT/KS-Server for ACPLT/OV
#	----------------------------

OV_SERVER_SRC = $(OV_SOURCE_SERVER_DIR)ov_server.c
OV_SERVER_OBJ = $(foreach source, $(OV_SERVER_SRC), $(basename $(notdir $(source)))$(_OBJ))
OV_SERVER_EXE = ov_server$(_EXE)

#	Table of statically linked ACPLT/OV libraries
#	---------------------------------------------

OV_LIBTABLE_SRC = ov_libtable.c
OV_LIBTABLE_OBJ = ov_libtable$(_OBJ)

#
#	ACPLT/OV example library
#	------------------------

EXAMPLE_SRC = \
	$(OV_SOURCE_EXAMPLE_DIR)example_gvc.c \
	$(OV_SOURCE_EXAMPLE_DIR)example_testassoc.c \
	$(OV_SOURCE_EXAMPLE_DIR)example_testclass.c				 
EXAMPLE_OBJ = $(foreach source, $(EXAMPLE_SRC), $(basename $(notdir $(source)))$(_OBJ))
EXAMPLE_LIB = example$(_LIB)

#	Targets
#	-------

TARGETS = \
	$(OV_CODEGEN_EXE) \
	$(OV_LIBOV_LIB) \
	$(OV_DBUTIL_EXE) \
	$(OV_LIBOVKS_LIB) \
	$(EXAMPLE_LIB) \
	$(OV_SERVER_EXE)

all: $(TARGETS) 

#	Implicit Rules
#	--------------

.c$(_OBJ):
	@if (f$$search("$@").nes."") then del $@;* /nolog
	$(COMPILE_C) $<

.cpp$(_OBJ):
	@if (f$$search("$@").nes."") then del $@;* /nolog
	$(CXX_COMPILE) $<

#	Dependencies
#	------------

depend:
	@echo Unable to generate dependencies under DEC C/CXX on OpenVMS

#	Table of statically linked ACPLT/OV libraries
#	---------------------------------------------

OV_LIBRARIES = $(EXAMPLE_LIB)

$(OV_LIBTABLE_SRC) : $(OV_LIBRARIES)
	!# Generating static library table file '$@'
	@if (f$$search("mklibtab$(_OBJ)").nes."") then del mklibtab$(_OBJ);* /nolog
	@if (f$$search("mklibtab$(_EXE)").nes."") then del mklibtab$(_EXE);* /nolog
	$(CC) [-]mklibtab.c
	$(LINK) mklibtab
	$(MKLIBTAB) $@ $(basename $^)	
	
#	Libraries
#	---------

#	ACPLT/OV library

$(OV_LIBOV_LIB) : ov$(_OBJ) $(OV_LIBOV_OBJ)
	@if (f$$search("$@").eqs."") then $(AR) /create $@
	$(AR) /repl $@ $?

ov.c : $(OV_MODEL_DIR)ov.ovm
	@if (f$$search("$@").nes."") then del $@;* /nolog
	mc []$(OV_CODEGEN_EXE) -f $^ 

$(OV_LIBOV_OBJ) : %$(_OBJ) : $(OV_SOURCE_LIBOV_DIR)%.c
	@if (f$$search("$@").nes."") then del $@;* /nolog
	$(COMPILE_C) $<

#	ACPLT/OV library for ACPLT/KS integration

$(OV_LIBOVKS_LIB) : $(KS_LIBOVKS_OBJ) $(OV_LIBOVKS_OBJ) ov_ksserver$(_OBJ) ov_ksclient$(_OBJ) ucx$$ioctl_routine$(_OBJ)
	@if (f$$search("$@").eqs."") then $(AR) /create $@
	$(AR) /repl $@ $?

$(KS_LIBOVKS_OBJ) : %$(_OBJ) : $(ACPLT_KS_SOURCE_DIR)%.cpp
	@if (f$$search("$@").nes."") then del $@;* /nolog
	$(CXX_COMPILE) $<

$(OV_LIBOVKS_OBJ) : %$(_OBJ) : $(OV_SOURCE_LIBOVKS_DIR)%.c
	@if (f$$search("$@").nes."") then del $@;* /nolog
	$(COMPILE_C) $<

ov_ksserver$(_OBJ) : $(OV_SOURCE_LIBOVKS_DIR)ov_ksserver.c
	@if (f$$search("$@").nes."") then del $@;* /nolog
	$(CXX_COMPILE) $<

ov_ksclient$(_OBJ) : $(OV_SOURCE_LIBOVKS_DIR)ov_ksclient.c
	@if (f$$search("$@").nes."") then del $@;* /nolog
	$(CXX_COMPILE) $<

ucx$$ioctl_routine$(_OBJ) :
	@if (f$$search("$@").nes."") then del $@;* /nolog
	$(CXX_COMPILE) ucx$$examples:ucx$$ioctl_routine.c

#	ACPLT/OV example library

$(EXAMPLE_LIB) : example$(_OBJ) $(EXAMPLE_OBJ)
	@if (f$$search("$@").eqs."") then $(AR) /create $@
	$(AR) /repl $@ $?

example.c : $(OV_SOURCE_EXAMPLE_DIR)example.ovm
	@if (f$$search("$@").nes."") then del $@;* /nolog
	mc []$(OV_CODEGEN_EXE) -f $^ --include-path $(OV_MODEL_DIR) -l example 

$(EXAMPLE_OBJ) : %$(_OBJ) : $(OV_SOURCE_EXAMPLE_DIR)%.c
	@if (f$$search("$@").nes."") then del $@;* /nolog
	$(COMPILE_C) $<

#	Executables
#	-----------

#	ACPLT/OV C code generator

$(OV_CODEGEN_EXE) : $(OV_CODEGEN_OBJ) ov_ovmscanner$(_OBJ) ov_ovmparser$(_OBJ)
	@if (f$$search("$@").nes."") then del $@;* /nolog
	$(LINK) $(LINK_FLAGS) /executable=$@ $^,sys$$library:vaxcrtl$(_LIB)/lib

ov_codegen.obj : $(OV_SOURCE_CODEGEN_DIR)ov_codegen.c
	@if (f$$search("$@").nes."") then del $@;* /nolog
	$(COMPILE_C) $^

ov_ovmparser.obj : ov_ovmparser.c
	@if (f$$search("$@").nes."") then del $@;* /nolog
	$(COMPILE_C) /define=("alloca=malloc") $^
 
ov_ovmscanner.c : $(OV_SOURCE_CODEGEN_DIR)ov_ovmscanner.lex
	@if (f$$search("$@").nes."") then del $@;* /nolog
	$(FLEX) -o$@ $<

ov_ovmparser.c : $(OV_SOURCE_CODEGEN_DIR)ov_ovmparser.y
	@if (f$$search("$@").nes."") then del $@;* /nolog
	$(BISON) /define /debug /out=$@ $^

ov_ovmscanner.c : ov_ovmparser.h	
ov_ovmparser.h : ov_ovmparser.c	

#	ACPLT/OV database utility

$(OV_DBUTIL_OBJ) : $(OV_LIBOV_LIB) $(LIBMPM_LIB)

$(OV_DBUTIL_EXE) : $(OV_DBUTIL_OBJ) 
	@if (f$$search("$@").nes."") then del $@;* /nolog
	$(LINK) $(LINK_FLAGS) /executable=$@ $^, $(OV_LIBOV_LIB)/lib, $(LIBMPM_LIB)/lib

$(OV_DBUTIL_OBJ) : %$(_OBJ) : $(OV_SOURCE_DBUTIL_DIR)%.c
	@if (f$$search("$@").nes."") then del $@;* /nolog
	$(COMPILE_C) $<

#	ACPLT/KS-Server for ACPLT/OV

$(OV_SERVER_OBJ) : $(OV_LIBRARIES) $(OV_LIBOVKS_LIB) $(ACPLTKS_LIBS) $(OV_LIBOV_LIB) $(LIBMPM_LIB)

$(OV_SERVER_EXE) : $(OV_SERVER_OBJ) $(OV_LIBTABLE_OBJ) 
	@if (f$$search("$@").nes."") then del $@;* /nolog
	$(LINK) $(LINK_FLAGS) /executable=$@ $^ \
	$(foreach lib, $(strip $(OV_LIBRARIES)), $(COMMA)$(lib)/lib), \
	[]$(OV_LIBOVKS_LIB)/lib, []$(OV_LIBOV_LIB)/lib, \
	$(LIBPLT_LIB)/lib, $(LIBMPM_LIB)/lib, $(LIBRPC_LIB)/lib

$(OV_SERVER_OBJ) : %$(_OBJ) : $(OV_SOURCE_SERVER_DIR)%.c
	@if (f$$search("$@").nes."") then del $@;* /nolog
	$(COMPILE_C) $<

#	Install
#	-------

#install : all
#	@echo Installing files to '$(ACPLT_BIN_DIR)'
#	@-for %i in ( $(TARGETS) ) do copy %i $(subst /,\, $(ACPLT_BIN_DIR))
#	@echo Done.

#	Clean up
#	--------

clean:
	if (f$$search("*.c").nes."") then del *.c;* /nolog
	if (f$$search("*.h").nes."") then del *.h;* /nolog
	if (f$$search("*.lis").nes."") then del *.lis;* /nolog
	if (f$$search("*$(_OBJ)").nes."") then del *$(_OBJ);* /nolog
	if (f$$search("*$(_LIB)").nes."") then del *$(_LIB);* /nolog
	if (f$$search("*$(_EXE)").nes."") then del *$(_EXE);* /nolog

#	Include dependencies
#	--------------------

#include depend.vms

