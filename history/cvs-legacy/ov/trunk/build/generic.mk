
#   $Id: generic.mk,v 1.5 2000-07-03 13:36:49 dirk Exp $
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



#	Directories
#	-----------

#	generic part

PLT_DIR						= ../../../../
PLT_BIN_DIR					= $(PLT_DIR)bin/$(SYSDIR)/

ACPLT_OV_DIR				= ../../
OV_INCLUDE_DIR				= $(ACPLT_OV_DIR)include/
OV_MODEL_DIR				= $(ACPLT_OV_DIR)model/
OV_SOURCE_DIR				= $(ACPLT_OV_DIR)source/

OV_SOURCE_CODEGEN_DIR		= $(OV_SOURCE_DIR)codegen/
OV_SOURCE_LIBOV_DIR			= $(OV_SOURCE_DIR)libov/
OV_SOURCE_DBUTIL_DIR		= $(OV_SOURCE_DIR)dbutil/
OV_SOURCE_LIBOVKS_DIR		= $(OV_SOURCE_DIR)libovks/
OV_SOURCE_SERVER_DIR		= $(OV_SOURCE_DIR)server/
OV_SOURCE_NTSERVICE_DIR		= $(OV_SOURCE_DIR)ntservice/
OV_SOURCE_EXAMPLE_DIR		= $(OV_SOURCE_DIR)example/

ACPLT_PLT_DIR				= ../../../plt/

ACPLT_KS_DIR				= ../../../ks/
ACPLT_PLT_INCLUDE_DIR		= $(ACPLT_PLT_DIR)include/
ACPLT_KS_INCLUDE_DIR		= $(ACPLT_KS_DIR)include/
ACPLT_KS_INCLUDE_KS_DIR		= $(ACPLT_KS_INCLUDE_DIR)ks/
ACPLT_KS_SOURCE_DIR			= $(ACPLT_KS_DIR)src/
ACPLT_PLT_BUILD_DIR			= $(ACPLT_PLT_DIR)build/$(SYSDIR)/

LIBMPM_DIR					= ../../../../libmpm/

#	platforms requiring ONC/RPC

ONCRPC_DIR					= ../../../../oncrpc/
ONCRPC_INCLUDE_DIR			= $(ONCRPC_DIR)
ONCRPC_BIN_DIR				= $(ONCRPC_DIR)bin/

#	Cygwin/MinGW stuff

MINGWLIBPATH				= //C/usr/local/mingw/lib
MINGWINCLUDEPATH			= //C/usr/local/mingw/include

#	stuff for C16x compatible compilers using the Keil compiler

MC164_DIR					= ../../../../mc164/
MC164_INCLUDE_DIR			= $(MC164_DIR)include/
MC164_SOURCE_DIR			= $(MC164_DIR)source/

KEIL_INCLUDE_DIR			=  //c/c166/inc/

#	RMOS32 stuff

RMOS_DIR					= c:\\siemens\\step7\\m7sys4.00\\
RMOS_INCLUDE_DIR			= $(RMOS_DIR)inc\\
RMOS_LIBRARY_DIR            = $(RMOS_DIR)lib\\

#	OpenVMS stuff

VMS_LIBRPC_DIR				= sys$$sysdevice:[vms$$common.ucx$$lib.rpc]

VMS_REPOSITORY				= acplt$$repository

SOURCE_DIRS	= \
	$(OV_SOURCE_CODEGEN_DIR) \
	$(OV_SOURCE_LIBOV_DIR) \
	$(OV_SOURCE_DBUTIL_DIR) \
	$(OV_SOURCE_LIBOVKS_DIR) \
	$(OV_SOURCE_SERVER_DIR) \
	$(OV_SOURCE_NTSERVICE_DIR) \
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
LIBRPC_DEFINES = \
	$(LIBRPC_PLATFORM_DEFINES) \
	-DWIN32
LIBRPC_INCLUDES	= \
	-I$(ONCRPC_INCLUDE_DIR)
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

ifeq ($(COMPILER), BORLAND)
OV_INCLUDES = \
	-I$(OV_INCLUDE_DIR) \
	-I$(OV_MODEL_DIR) \
	-I$(OV_SOURCE_CODEGEN_DIR) \
	-I$(OV_SOURCE_NTSERVICE_DIR) \
	-I$(OV_SOURCE_EXAMPLE_DIR) \
	-I$(LIBMPM_DIR)
else
ifeq ($(COMPILER), KEIL)
OV_INCLUDES = \
	-I$(KEIL_INCLUDE_DIR) \
	-I$(OV_INCLUDE_DIR) \
	-I$(OV_MODEL_DIR) \
	-I$(LIBMPM_DIR)
else
OV_INCLUDES = \
	-I$(OV_INCLUDE_DIR) \
	-I$(OV_MODEL_DIR) \
	-I$(OV_SOURCE_EXAMPLE_DIR) \
	-I$(LIBMPM_DIR)
endif
endif

#	all defines and includes together
#	---------------------------------

DEFINES	 = $(LIBRPC_DEFINES) $(ACPLTKS_DEFINES) $(OV_DEFINES)
INCLUDES = $(C_INCLUDES) $(LIBRPC_INCLUDES) $(ACPLTKS_INCLUDES) $(OV_INCLUDES) -I.

#	presupposed libraries
#	---------------------

LIBMPM_LIB			= $(LIBMPM_DIR)libmpm$(LIB)

ifeq ($(SYSTEM), NT)
LIBRPC_LIB			= $(ONCRPC_BIN_DIR)oncrpc$(LIB)
endif

ifeq ($(SYSTEM), RMOS)
LIBRPC_LIB			= $(ONCRPC_BIN_DIR)oncrpc$(LIB)
endif

ifeq ($SYSTEM), OPENVMS)
LIBRPC_LIB			= $(VMS_LIBRPC_DIR)ucx$$rpcxdr
endif

ACPLTKS_LIBS		= $(ACPLT_PLT_BUILD_DIR)libplt$(LIB) $(LIBRPC_LIB) 

#   Rules
#   -----

.SUFFIXES:

.SUFFIXES: .c .cpp .h .y .lex .rc .ovm $(RES) $(LIB) $(OBJ) $(EXE) $(DLL)

VPATH = $(SOURCE_DIRS) $(OV_MODEL_DIR) $(ACPLT_KS_INCLUDE_KS_DIR) .

#	ACPLT/OV C code generator
#	-------------------------

OV_CODEGEN_SRC := ov_ovmparser.c ov_ovmscanner.c \
	$(wildcard $(OV_SOURCE_CODEGEN_DIR)*.c)	
OV_CODEGEN_OBJ  = $(foreach source, $(OV_CODEGEN_SRC), \
	$(basename $(notdir $(source)))$(OBJ))
OV_CODEGEN_EXE  = ov_codegen$(EXE)
OV_CODEGEN_RES	 = $(basename $(OV_CODEGEN_EXE))$(RES)

#	ACPLT/OV library
#	----------------

OV_LIBOV_SRC := ov.c $(wildcard $(OV_SOURCE_LIBOV_DIR)*.c)
OV_LIBOV_OBJ  = $(foreach source, $(OV_LIBOV_SRC), \
	$(basename $(notdir $(source)))$(OBJ))
OV_LIBOV_LIB  = libov$(LIB)
OV_LIBOV_DLL  = libov$(DLL)
OV_LIBOV_RES  = libov$(RES)

#	ACPLT/OV database utility
#	-------------------------

OV_DBUTIL_SRC := $(wildcard $(OV_SOURCE_DBUTIL_DIR)*.c)	
OV_DBUTIL_OBJ  = $(foreach source, $(OV_DBUTIL_SRC), \
	$(basename $(notdir $(source)))$(OBJ))
OV_DBUTIL_EXE  = ov_dbutil$(EXE)
OV_DBUTIL_RES  = ov_dbutil$(RES)

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
	$(basename $(notdir $(source)))$(OBJ))

#	ACPLT/OV library for ACPLT/KS integration, OV part
#	--------------------------------------------------

OV_LIBOVKS_SRC := $(wildcard $(OV_SOURCE_LIBOVKS_DIR)*.c)
OV_LIBOVKS_OBJ  = $(foreach source, $(OV_LIBOVKS_SRC), \
	$(basename $(notdir $(source)))$(OBJ))
OV_LIBOVKS_LIB  = libovks$(LIB)
OV_LIBOVKS_DLL  = libovks$(DLL)
OV_LIBOVKS_RES  = libovks$(RES)

#	ACPLT/KS-Server for ACPLT/OV
#   ----------------------------

OV_SERVER_SRC := $(wildcard $(OV_SOURCE_SERVER_DIR)*.c)
OV_SERVER_OBJ  = $(foreach source, $(OV_SERVER_SRC), $(basename $(notdir $(source)))$(OBJ))
OV_SERVER_EXE  = ov_server$(EXE)
OV_SERVER_RES  = ov_server$(RES)

#	ACPLT/KS-Server for ACPLT/OV as Windows NT service
#   --------------------------------------------------

OV_NTSERVICE_SRC = $(OV_SOURCE_NTSERVICE_DIR)ov_ntservice.c
OV_NTSERVICE_OBJ = $(foreach source, $(OV_NTSERVICE_SRC), $(basename $(notdir $(source)))$(OBJ))
OV_NTSERVICE_EXE = ov_ntservice$(EXE)
OV_NTSERVICE_RES = ov_ntservice$(RES)

#	ACPLT/OV Control Panel for the Windows NT service
#	-------------------------------------------------

OV_CONTROLPANEL_SRC = $(OV_SOURCE_NTSERVICE_DIR)ov_controlpanel.c
OV_CONTROLPANEL_OBJ = $(foreach source, $(OV_CONTROLPANEL_SRC), $(basename $(notdir $(source)))$(OBJ))
OV_CONTROLPANEL_CPL = ov_controlpanel$(CPL)
OV_CONTROLPANEL_RES = ov_controlpanel$(RES)

#	Table of statically linked ACPLT/OV libraries
#	---------------------------------------------

OV_LIBTABLE_SRC = ov_libtable.c
OV_LIBTABLE_OBJ = ov_libtable$(OBJ)

#
#	ACPLT/OV example library
#	------------------------

EXAMPLE_SRC := example.c $(wildcard $(OV_SOURCE_EXAMPLE_DIR)*.c)
EXAMPLE_OBJ  = $(foreach source, $(EXAMPLE_SRC), $(basename $(notdir $(source)))$(OBJ))
EXAMPLE_LIB  = example$(LIB)
EXAMPLE_DLL  = example$(DLL)

#	Targets and their sources
#	-------------------------

TARGETS = \
	$(OV_CODEGEN_EXE) \
	$(OV_LIBOV_DLL) \
	$(OV_LIBOV_LIB) \
	$(OV_DBUTIL_EXE) \
	$(OV_LIBOVKS_DLL) \
	$(OV_LIBOVKS_LIB) \
	$(OV_SERVER_EXE)

EXAMPLE = \
	$(EXAMPLE_DLL) \
	$(EXAMPLE_LIB)

ALL = \
	$(TARGETS) \
	$(EXAMPLE)

SOURCES = \
	$(OV_CODEGEN_SRC) \
	$(OV_LIBOV_SRC) \
	$(OV_DBUTIL_SRC) \
	$(KS_LIBOVKS_SRC) \
	$(OV_LIBOVKS_SRC) \
	$(OV_SERVER_SRC) \
	$(EXAMPLE_SRC)
