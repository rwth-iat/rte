
#   $Id: generic.mk,v 1.21 2007-09-25 11:55:16 wolfram Exp $
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



#	Directories
#	-----------

#	generic part

ACPLT_DIR					= ../../../
ACPLT_BIN_DIR					= $(ACPLT_DIR)bin/
ACPLT_LIB_DIR					= $(ACPLT_DIR)lib/

ACPLT_OV_DIR				= ../../
OV_INCLUDE_DIR				= $(ACPLT_OV_DIR)include/
OV_MODEL_DIR				= $(ACPLT_OV_DIR)model/
OV_SOURCE_DIR				= $(ACPLT_OV_DIR)source/

OV_SOURCE_CODEGEN_DIR			= $(OV_SOURCE_DIR)codegen/
OV_SOURCE_BUILDER_DIR			= $(OV_SOURCE_DIR)builder/
OV_SOURCE_LIBOV_DIR			= $(OV_SOURCE_DIR)libov/
OV_SOURCE_DBUTIL_DIR			= $(OV_SOURCE_DIR)dbutil/
OV_SOURCE_LIBOVKS_DIR			= $(OV_SOURCE_DIR)libovks/
OV_SOURCE_SERVER_DIR			= $(OV_SOURCE_DIR)server/
OV_SOURCE_NTSERVICE_DIR			= $(OV_SOURCE_DIR)ntservice/
OV_SOURCE_EXAMPLE_DIR			= $(OV_SOURCE_DIR)example/
OV_SOURCE_KSHISTLIB_DIR			= $(OV_SOURCE_DIR)kshist/
OV_SOURCE_DYNOV_DIR			= $(OV_SOURCE_DIR)dynov/
OV_SOURCE_TASKLIB_DIR			= $(OV_SOURCE_DIR)tasklib/
OV_SOURCE_DBDUMP_DIR			= $(OV_SOURCE_DIR)dbdump/
OV_SOURCE_DBPARSE_DIR			= $(OV_SOURCE_DIR)dbparse/
OV_SOURCE_OVXIPARSE_DIR			= $(OV_SOURCE_DIR)ovxiparse/
OV_SOURCE_MAKMAK_DIR			= $(OV_SOURCE_DIR)makmak/
OV_SOURCE_DBINFO_DIR			= $(OV_SOURCE_DIR)dbinfo/
OV_SOURCE_LIBINFO_DIR			= $(OV_SOURCE_DIR)libinfo/

ACPLT_PLT_DIR				= ../../../plt/

ACPLT_KS_DIR				= ../../../ks/
ACPLT_PLT_INCLUDE_DIR			= $(ACPLT_PLT_DIR)include/
ACPLT_KS_INCLUDE_DIR			= $(ACPLT_KS_DIR)include/
ACPLT_KS_INCLUDE_KS_DIR			= $(ACPLT_KS_INCLUDE_DIR)ks/
ACPLT_KS_SOURCE_DIR			= $(ACPLT_KS_DIR)src/
ACPLT_PLT_BUILD_DIR			= $(ACPLT_LIB_DIR)

LIBMPM_DIR				= ../../../libmpm/

#	platforms requiring ONC/RPC

ONCRPC_DIR				= ../../../../oncrpc/
ONCRPC_INCLUDE_DIR			= $(ONCRPC_DIR)
ONCRPC_BIN_DIR				= $(ACPLT_LIB_DIR)

#	Cygwin/MinGW stuff

MINGWLIBPATH				= //C/usr/local/mingw/lib
MINGWINCLUDEPATH			= //C/usr/local/mingw/include

#	stuff for C16x compatible compilers using the Keil compiler

MC164_DIR				= ../../../../mc164/
MC164_INCLUDE_DIR			= $(MC164_DIR)include/
MC164_SOURCE_DIR			= $(MC164_DIR)source/

KEIL_INCLUDE_DIR			=  //c/c166/inc/

#	RMOS32 stuff

RMOS_DIR				= c:\\siemens\\step7\\m7sys4.00\\
RMOS_INCLUDE_DIR			= $(RMOS_DIR)inc\\
RMOS_LIBRARY_DIR            		= $(RMOS_DIR)lib\\

#	OpenVMS stuff

VMS_LIBRPC_DIR				= sys$$sysdevice:[vms$$common.ucx$$lib.rpc]

VMS_REPOSITORY				= acplt$$repository

SOURCE_DIRS	= \
	$(OV_SOURCE_CODEGEN_DIR) \
	$(OV_SOURCE_BUILDER_DIR) \
	$(OV_SOURCE_LIBOV_DIR) \
	$(OV_SOURCE_DBUTIL_DIR) \
	$(OV_SOURCE_LIBOVKS_DIR) \
	$(OV_SOURCE_SERVER_DIR) \
	$(OV_SOURCE_NTSERVICE_DIR) \
	$(OV_SOURCE_KSHISTLIB_DIR) \
	$(OV_SOURCE_DYNOV_DIR) \
	$(OV_SOURCE_TASKLIB_DIR) \
	$(OV_SOURCE_DBDUMP_DIR) \
	$(OV_SOURCE_DBPARSE_DIR) \
	$(OV_SOURCE_OVXIPARSE_DIR) \
	$(OV_SOURCE_MAKMAK_DIR) \
	$(OV_SOURCE_DBINFO_DIR) \
	$(OV_SOURCE_LIBINFO_DIR) \
	$(OV_SOURCE_EXAMPLE_DIR) \
	$(ACPLT_KS_SOURCE_DIR)

#	C-Includes
#	----------

ifeq ($(SYSTEM), RMOS)
C_INCLUDES = -I$(RMOS_INCLUDE_DIR)
else
C_INCLUDES =
endif

#	ONCRPC definitions and includes
#	-------------------------------

ifeq ($(SYSTEM), NT)
ifeq ($(COMPILER), MSVC)
	LIBRPC_DEFINES = \
		$(LIBRPC_PLATFORM_DEFINES) \
		/DWIN32
	LIBRPC_INCLUDES	= \
		/I$(ONCRPC_INCLUDE_DIR)
else
	LIBRPC_DEFINES = \
		$(LIBRPC_PLATFORM_DEFINES) \
		-DWIN32
	LIBRPC_INCLUDES	= \
		-I$(ONCRPC_INCLUDE_DIR)
endif
endif

ifeq ($(SYSTEM), RMOS)
LIBRPC_DEFINES = \
	$(LIBRPC_PLATFORM_DEFINES) \
	-DRMOS32
LIBRPC_INCLUDES	= \
	-I$(ONCRPC_INCLUDE_DIR)
endif

ifeq ($(SYSTEM), MC164)
LIBRPC_DEFINES = \
	$(LIBRPC_PLATFORM_DEFINES)
LIBRPC_INCLUDES	= \
	-I$(MC164_INCLUDE_DIR) \
	-I$(ONCRPC_INCLUDE_DIR)
endif

#	ACPLT/KS definitions and includes
#	---------------------------------

ifeq ($(COMPILER), MSVC)
	ACPLTKS_DEFINES = \
		$(ACPLTKS_PLATFORM_DEFINES) \
		/DPLT_SYSTEM_$(SYSTEM)=1 \
		/DPLT_USE_BUFFERED_STREAMS=1 \
		/DPLT_SERVER_TRUNC_ONLY=1 \
		/DNDEBUG
	ACPLTKS_INCLUDES = \
		/I$(ACPLT_PLT_INCLUDE_DIR) \
		/I$(ACPLT_KS_INCLUDE_DIR)
else
	ACPLTKS_DEFINES = \
		$(ACPLTKS_PLATFORM_DEFINES) \
		-DPLT_SYSTEM_$(SYSTEM)=1 \
		-DPLT_USE_BUFFERED_STREAMS=1 \
		-DPLT_SERVER_TRUNC_ONLY=1 \
		-DNDEBUG
	ACPLTKS_INCLUDES = \
		-I$(ACPLT_PLT_INCLUDE_DIR) \
		-I$(ACPLT_KS_INCLUDE_DIR)
endif
#	ACPLT/OV definitions and includes
#	---------------------------------

OV_DEFINES = \
	$(OV_PLATFORM_DEFINES) \
	-DOV_SYSTEM_$(SYSTEM)=1 \

ifeq ($(COMPILER), BORLAND)
OV_INCLUDES = \
	-I$(OV_INCLUDE_DIR) \
	-I$(OV_MODEL_DIR) \
	-I$(OV_SOURCE_CODEGEN_DIR) \
	-I$(OV_SOURCE_BUILDER_DIR) \
	-I$(OV_SOURCE_NTSERVICE_DIR) \
	-I$(OV_SOURCE_EXAMPLE_DIR) \
	-I$(OV_SOURCE_KSHISTLIB_DIR) \
	-I$(OV_SOURCE_DYNOV_DIR) \
	-I$(OV_SOURCE_TASKLIB_DIR) \
	-I$(OV_SOURCE_DBPARSE_DIR) \
	-I$(OV_SOURCE_DBDUMP_DIR) \
	-I$(LIBMPM_DIR) \
	-I.
else
ifeq ($(COMPILER), MSVC)
OV_INCLUDES = \
	/I$(OV_INCLUDE_DIR) \
	/I$(OV_MODEL_DIR) \
	/I$(OV_SOURCE_CODEGEN_DIR) \
	/I$(OV_SOURCE_BUILDER_DIR) \
	/I$(OV_SOURCE_NTSERVICE_DIR) \
	/I$(OV_SOURCE_EXAMPLE_DIR) \
	/I$(OV_SOURCE_KSHISTLIB_DIR) \
	/I$(OV_SOURCE_DYNOV_DIR) \
	/I$(OV_SOURCE_TASKLIB_DIR) \
	/I$(OV_SOURCE_DBPARSE_DIR) \
	/I$(OV_SOURCE_DBDUMP_DIR) \
	/I$(LIBMPM_DIR) \
	/I.
else
ifeq ($(COMPILER), KEIL)
OV_INCLUDES = \
	-I$(KEIL_INCLUDE_DIR) \
	-I$(OV_INCLUDE_DIR) \
	-I$(OV_MODEL_DIR) \
	-I$(LIBMPM_DIR) \
	-I.
else
OV_INCLUDES = \
	-I$(OV_INCLUDE_DIR) \
	-I$(OV_MODEL_DIR) \
	-I$(OV_SOURCE_CODEGEN_DIR) \
	-I$(OV_SOURCE_BUILDER_DIR) \
	-I$(OV_SOURCE_EXAMPLE_DIR) \
	-I$(OV_SOURCE_KSHISTLIB_DIR) \
	-I$(OV_SOURCE_DYNOV_DIR) \
	-I$(OV_SOURCE_TASKLIB_DIR) \
	-I$(OV_SOURCE_DBPARSE_DIR) \
	-I$(OV_SOURCE_DBDUMP_DIR) \
	-I$(LIBMPM_DIR) \
	-I.
endif
endif
endif
#	all defines and includes together
#	---------------------------------

DEFINES	 = $(LIBRPC_DEFINES) $(ACPLTKS_DEFINES) $(OV_DEFINES)
INCLUDES = $(C_INCLUDES) $(LIBRPC_INCLUDES) $(ACPLTKS_INCLUDES) $(OV_INCLUDES)

#	presupposed libraries
#	---------------------

LIBMPM_LIB			= $(ACPLT_LIB_DIR)libmpm$(_LIB)

ifeq ($(SYSTEM), NT)
LIBRPC_LIB			= $(ONCRPC_BIN_DIR)oncrpc$(_LIB)
endif

ifeq ($(SYSTEM), RMOS)
LIBRPC_LIB			= $(ONCRPC_BIN_DIR)oncrpc$(_LIB)
endif

ifeq ($SYSTEM), OPENVMS)
LIBRPC_LIB			= $(VMS_LIBRPC_DIR)ucx$$rpcxdr
endif

ACPLTKS_LIBS			= $(ACPLT_PLT_BUILD_DIR)libplt$(_LIB) $(LIBRPC_LIB)

LIBPLT_LIB			= $(ACPLT_PLT_BUILD_DIR)libplt$(_LIB)
LIBKS_LIB			= $(ACPLT_PLT_BUILD_DIR)libks$(_LIB)
LIBKSCLN_LIB			= $(ACPLT_PLT_BUILD_DIR)libkscln$(_LIB)

#   Rules
#   -----

.SUFFIXES:

.SUFFIXES: .c .cpp .h .y .lex .rc .ovm $(_RES) $(_LIB) $(_OBJ) $(_EXE) $(_DLL)

VPATH = $(SOURCE_DIRS) $(OV_MODEL_DIR) $(ACPLT_KS_INCLUDE_KS_DIR) .

#	ACPLT/OV C code generator
#	-------------------------

OV_CODEGEN_SRC := ov_ovmparser.c ov_ovmscanner.c \
	$(wildcard $(OV_SOURCE_CODEGEN_DIR)*.c)
OV_CODEGEN_OBJ  = $(foreach source, $(OV_CODEGEN_SRC), \
	$(basename $(notdir $(source)))$(_OBJ))
OV_CODEGEN_EXE  = ov_codegen$(_EXE)
OV_CODEGEN_RES	 = $(basename $(OV_CODEGEN_EXE))$(_RES)

#	ACPLT/OV framework builder
#	--------------------------

OV_BUILDER_SRC := ov_ovmparser.c ov_ovmscanner.c \
	$(wildcard $(OV_SOURCE_BUILDER_DIR)*.c)
OV_BUILDER_OBJ  = $(foreach source, $(OV_BUILDER_SRC), \
	$(basename $(notdir $(source)))$(_OBJ))
OV_BUILDER_EXE  = ov_builder$(_EXE)
OV_BUILDER_RES	= $(basename $(OV_BUILDER_EXE))$(_RES)

#	ACPLT/OV library
#	----------------

OV_LIBOV_SRC := ov.c $(wildcard $(OV_SOURCE_LIBOV_DIR)*.c)
OV_LIBOV_OBJ  = $(foreach source, $(OV_LIBOV_SRC), \
	$(basename $(notdir $(source)))$(_OBJ))
OV_LIBOV_LIB  = libov$(_LIB)
OV_LIBOV_DLL  = libov$(_DLL)
OV_LIBOV_RES  = libov$(_RES)

#	ACPLT/OV database utility
#	-------------------------

OV_DBUTIL_SRC := $(wildcard $(OV_SOURCE_DBUTIL_DIR)*.c)
OV_DBUTIL_OBJ  = $(foreach source, $(OV_DBUTIL_SRC), \
	$(basename $(notdir $(source)))$(_OBJ))
OV_DBUTIL_EXE  = ov_dbutil$(_EXE)
OV_DBUTIL_RES  = ov_dbutil$(_RES)

#	ACPLT/OV library for ACPLT/KS integration, KS part
#	--------------------------------------------------

KS_LIBOVKS_SRC = \
	$(ACPLT_KS_SOURCE_DIR)avticket.cpp \
	$(ACPLT_KS_SOURCE_DIR)connection.cpp \
	$(ACPLT_KS_SOURCE_DIR)connectionmgr.cpp \
	$(ACPLT_KS_SOURCE_DIR)event.cpp \
	$(ACPLT_KS_SOURCE_DIR)register.cpp \
	$(ACPLT_KS_SOURCE_DIR)result.cpp \
	$(ACPLT_KS_SOURCE_DIR)rpc.cpp \
	$(ACPLT_KS_SOURCE_DIR)rpcproto.cpp \
	$(ACPLT_KS_SOURCE_DIR)server.cpp \
	$(ACPLT_KS_SOURCE_DIR)interserver.cpp \
	$(ACPLT_KS_SOURCE_DIR)svrtransport.cpp \
	$(ACPLT_KS_SOURCE_DIR)string.cpp \
	$(ACPLT_KS_SOURCE_DIR)svrbase.cpp \
	$(ACPLT_KS_SOURCE_DIR)svrrpcctx.cpp \
	$(ACPLT_KS_SOURCE_DIR)time.cpp \
	$(ACPLT_KS_SOURCE_DIR)xdr.cpp \
	$(ACPLT_KS_SOURCE_DIR)xdrmemstream.cpp \
	$(ACPLT_KS_SOURCE_DIR)xdrtcpcon.cpp \
	$(ACPLT_KS_SOURCE_DIR)xdrudpcon.cpp
KS_LIBOVKS_OBJ = $(foreach source, $(KS_LIBOVKS_SRC), \
	$(basename $(notdir $(source)))$(_OBJ))

#	ACPLT/OV library for ACPLT/KS integration, OV part
#	--------------------------------------------------

OV_LIBOVKS_SRC := $(wildcard $(OV_SOURCE_LIBOVKS_DIR)*.c)
OV_LIBOVKS_OBJ  = $(foreach source, $(OV_LIBOVKS_SRC), \
	$(basename $(notdir $(source)))$(_OBJ))
OV_LIBOVKS_LIB  = libovks$(_LIB)
OV_LIBOVKS_DLL  = libovks$(_DLL)
OV_LIBOVKS_RES  = libovks$(_RES)

#	ACPLT/KS-Server for ACPLT/OV
#   	----------------------------

OV_SERVER_SRC := $(wildcard $(OV_SOURCE_SERVER_DIR)*.c)
OV_SERVER_OBJ  = $(foreach source, $(OV_SERVER_SRC), $(basename $(notdir $(source)))$(_OBJ))
OV_SERVER_EXE  = ov_server$(_EXE)
OV_SERVER_RES  = ov_server$(_RES)

#	ACPLT/KS-Server for ACPLT/OV as Windows NT service
#   	--------------------------------------------------

OV_NTSERVICE_SRC = $(OV_SOURCE_NTSERVICE_DIR)ov_ntservice.c
OV_NTSERVICE_OBJ = $(foreach source, $(OV_NTSERVICE_SRC), $(basename $(notdir $(source)))$(_OBJ))
OV_NTSERVICE_EXE = ov_ntservice$(_EXE)
OV_NTSERVICE_RES = ov_ntservice$(_RES)

#	ACPLT/OV Control Panel for the Windows NT service
#	-------------------------------------------------

OV_CONTROLPANEL_SRC = $(OV_SOURCE_NTSERVICE_DIR)ov_controlpanel.c
OV_CONTROLPANEL_OBJ = $(foreach source, $(OV_CONTROLPANEL_SRC), $(basename $(notdir $(source)))$(_OBJ))
OV_CONTROLPANEL_CPL = ov_controlpanel$(_CPL)
OV_CONTROLPANEL_RES = ov_controlpanel$(_RES)

#	Windows NT service installer
#   	----------------------------

OV_NTSERVICE_SRC = $(OV_SOURCE_NTSERVICE_DIR)install_ntservice.c
OV_NTSERVICE_OBJ = $(foreach source, $(OV_NTSERVICE_SRC), $(basename $(notdir $(source)))$(_OBJ))
OV_NTSERVICE_EXE = install_ntservice$(_EXE)

#	Table of statically linked ACPLT/OV libraries
#	---------------------------------------------

OV_LIBTABLE_SRC = ov_libtable.c
OV_LIBTABLE_OBJ = ov_libtable$(_OBJ)

#
#	ACPLT/OV example library
#	------------------------

EXAMPLE_SRC := ov_expression_parser.c ov_expression_scanner.c example.c $(wildcard $(OV_SOURCE_EXAMPLE_DIR)*.c)
EXAMPLE_OBJ  = $(foreach source, $(EXAMPLE_SRC), $(basename $(notdir $(source)))$(_OBJ))
EXAMPLE_LIB  = example$(_LIB)
EXAMPLE_DLL  = example$(_DLL)

#
#	ACPLT/OV KsHistory library
#	--------------------------

KSHISTLIB_SRC := kshist.c $(wildcard $(OV_SOURCE_KSHISTLIB_DIR)*.c)
KSHISTLIB_OBJ  = $(foreach source, $(KSHISTLIB_SRC), $(basename $(notdir $(source)))$(_OBJ))
KSHISTLIB_LIB  = kshist$(_LIB)
KSHISTLIB_DLL  = kshist$(_DLL)

#
#	ACPLT/OV dynov library
#	----------------------

DYNOV_SRC := dynov.c $(wildcard $(OV_SOURCE_DYNOV_DIR)*.c)
DYNOV_OBJ  = $(foreach source, $(DYNOV_SRC), $(basename $(notdir $(source)))$(_OBJ))
DYNOV_LIB  = dynov$(_LIB)
DYNOV_DLL  = dynov$(_DLL)

#
#	ACPLT/OV tasking library
#	------------------------

TASKLIB_SRC := tasklib.c $(wildcard $(OV_SOURCE_TASKLIB_DIR)*.c)
TASKLIB_OBJ  = $(foreach source, $(TASKLIB_SRC), $(basename $(notdir $(source)))$(_OBJ))
TASKLIB_LIB  = tasklib$(_LIB)
TASKLIB_DLL  = tasklib$(_DLL)

#	ACPLT/OV database dumper
#	------------------------

DBDUMP_SRC := $(wildcard $(OV_SOURCE_DBDUMP_DIR)*.cpp) $(wildcard $(OV_SOURCE_DBDUMP_DIR)*.c)
DBDUMP_OBJ  = $(foreach source, $(DBDUMP_SRC), $(basename $(notdir $(source)))$(_OBJ))
DBDUMP_EXE  = ov_dbdump$(_EXE)
DBDUMP_RES  = $(basename $(DBDUMP_EXE))$(_RES)

#	ACPLT/OV database parser
#	------------------------

DBPARSE_SRC := db_y.c db_lex.c dbparse.cpp dbparse1.cpp
DBPARSE_OBJ  = $(foreach source, $(DBPARSE_SRC), $(basename $(notdir $(source)))$(_OBJ))
DBPARSE_EXE  = ov_dbparse$(_EXE)
DBPARSE_RES  = $(basename $(DBPARSE_EXE))$(_RES)

#	ACPLT/OV OVXI parser
#	--------------------

OVXIPARSE_SRC := db_y.c db_lex.c dbparse.cpp ovxiparse.cpp
OVXIPARSE_OBJ  = $(foreach source, $(OVXIPARSE_SRC), $(basename $(notdir $(source)))$(_OBJ))
OVXIPARSE_EXE  = ov_ovxiparse$(_EXE)
OVXIPARSE_RES  = $(basename $(OVXIPARSE_EXE))$(_RES)

#	ACPLT/OV makmak
#	---------------

MAKMAK_SRC := $(wildcard $(OV_SOURCE_MAKMAK_DIR)*.c)
MAKMAK_OBJ  = $(foreach source, $(MAKMAK_SRC), $(basename $(notdir $(source)))$(_OBJ))
MAKMAK_EXE  = ov_makmak$(_EXE)
MAKMAK_RES  = $(basename $(MAKMAK_EXE))$(_RES)

#	ACPLT/OV library info
#	---------------------

LIBINFO_SRC := $(wildcard $(OV_SOURCE_LIBINFO_DIR)*.c)
LIBINFO_OBJ  = $(foreach source, $(LIBINFO_SRC), $(basename $(notdir $(source)))$(_OBJ))
LIBINFO_EXE  = ov_libinfo$(_EXE)
LIBINFO_RES  = $(basename $(LIBINFO_EXE))$(_RES)

#	Targets and their sources
#	-------------------------

TARGETS = \
	$(OV_CODEGEN_EXE) \
	$(OV_BUILDER_EXE) \
	$(OV_LIBOV_DLL) \
	$(OV_LIBOV_LIB) \
	$(OV_DBUTIL_EXE) \
	$(OV_LIBOVKS_DLL) \
	$(OV_LIBOVKS_LIB) \
	$(MAKMAK_EXE) \
	$(KSHISTLIB_DLL) \
	$(KSHISTLIB_LIB) \
	$(DYNOV_DLL) \
	$(DYNOV_LIB) \
	$(TASKLIB_DLL) \
	$(TASKLIB_LIB) \
	$(EXAMPLE_DLL) \
	$(EXAMPLE_LIB) \
	$(OV_SERVER_EXE) \
	$(DBDUMP_EXE) \
	$(DBPARSE_EXE) \
	$(OVXIPARSE_EXE) \
	$(LIBINFO_EXE)
	
ALL = \
	$(TARGETS)

SOURCES = \
	$(OV_CODEGEN_SRC) \
	$(OV_BUILDER_SRC) \
	$(OV_LIBOV_SRC) \
	$(OV_DBUTIL_SRC) \
	$(KS_LIBOVKS_SRC) \
	$(OV_LIBOVKS_SRC) \
	$(OV_SERVER_SRC) \
	$(KSHISTLIB_SRC) \
	$(DYNOV_SRC) \
	$(TASKLIB_SRC) \
	$(DBDUMP_SRC) \
	$(DBPARSE_SRC) \
	$(OVXIPARSE_SRC) \
	$(MAKMAK_SRC) \
	$(LIBINFO_SRC) \
	$(EXAMPLE_SRC)
