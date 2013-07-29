
#	Copyright (C) 1998, 1999
#	Chair of Process Control Engineering,
#	Aachen University of Technology.
#	All rights reserved.
#
#	Redistribution and use in source and binary forms, with or without
#	modification, are permitted provided that the following conditions
#	are met:
#	1. Redistributions of source code must retain the above copyright
#	   notice, this list of conditions and the following disclaimer.
#	2. Redistributions in binary form must print or display the above
#	   copyright notice either during startup or must have a means for
#	   the user to view the copyright notice.
#	3. Redistributions in binary form must reproduce the above copyright
#	   notice, this list of conditions and the following disclaimer in the
#	   documentation and/or other materials provided with the distribution.
#	4. Neither the name of the Chair of Process Control Engineering nor the
#	   name of the Aachen University of Technology may be used to endorse or
#	   promote products derived from this software without specific prior
#	   written permission.
#
#	THIS SOFTWARE IS PROVIDED BY THE CHAIR OF PROCESS CONTROL ENGINEERING
#	``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
#	TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
#	PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE CHAIR OF PROCESS CONTROL
#	ENGINEERING BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
#	EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
#	PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
#	OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
#	WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
#	OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
#	ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#	History:
#	--------
#	26-Nov-1998 Dirk Meyer <dirk@plt.rwth-aachen.de>: File created.
#	16-Apr-1999 Dirk Meyer <dirk@plt.rwth-aachen.de>: Major revision.



#	Plattform
#	---------

SYSTEM = MC164
SYSDIR = mc164

#	Filename conventions
#	--------------------

_OBJ = .obj
_LIB = .lib
_EXE =

#	Platform-specific definitions
#	-----------------------------

ACPLTKS_PLATFORM_DEFINES	= -D__MODEL__=6 -D__C166__
OV_DEBUG					=
ifdef OV_DEBUG
OV_DODEBUG	=	-DOV_DEBUG
else
OV_DODEBUG	=
endif
OV_PLATFORM_DEFINES			= $(OV_DODEBUG)

#	Compiler
#	--------

COMPILER	= KEIL

CODEGEN		= ../../../../bin/nt/$(OV_CODEGEN_EXE) 
MKLIBTAB	= ../mklibtab

CPP_PATH	= c:/tools/bin
CPP			= cpp
CC			= c166
#CC_FLAGS	= DB M167 F64 HLA NOPR
CC_FLAGS	= DB M167 HLA SB 
#EXTINS

AR	 		= lib166

#	Include generic part
#	--------------------

include ../generic.mk

#	C-Includes
#	----------

C_INCLUDES =

#	ONCRPC-Includes and defines
#	---------------------------

LIBRPC_DEFINES = \
	$(LIBRPC_PLATFORM_DEFINES)
LIBRPC_INCLUDES	= \
	-I$(MC164_INCLUDE_DIR) \
	-I$(ONCRPC_INCLUDE_DIR)


#	OV-Includes
#	-----------

OV_INCLUDES = $(OV_INCLUDES) \
	-I.\
	-I$(KEIL_INCLUDE_DIR) \
	-I$(OV_INCLUDE_DIR) \
	-I$(OV_MODEL_DIR) \
	-I$(LIBMPM_DIR)

#	all defines and includes together
#	---------------------------------

DEFINES	 = $(LIBRPC_DEFINES) $(ACPLTKS_DEFINES) $(OV_DEFINES)
INCLUDES = $(C_INCLUDES) $(LIBRPC_INCLUDES) $(ACPLTKS_INCLUDES) $(OV_INCLUDES)

#	presupposed libraries
#	---------------------

LIBMPM_LIB			= $(LIBMPM_DIR)libmpm$(LIB)

ACPLTKS_LIBS			= $(ACPLT_PLT_BUILD_DIR)libplt$(LIB) $(LIBRPC_LIB)

INCLUDES := $(INCLUDES) -I../../../../../

#	Targets
#	-------

all: $(OV_LIBOV_LIB) $(OV_LIBOVKS_LIB)

#	Create short filenames
#	----------------------

MC_COMM_SRC = \
	$(MC164_SOURCE_DIR)time.c    \
	$(MC164_SOURCE_DIR)strdup.c  \
	$(MC164_SOURCE_DIR)Kachel.c  \
	$(MC164_SOURCE_DIR)rpchead.c \
	$(MC164_SOURCE_DIR)CanKa.c   \
	$(MC164_SOURCE_DIR)mc_srvr.c \
	$(MC164_SOURCE_DIR)strtoul.c \
	$(MC164_SOURCE_DIR)mc_div.c  \
	$(MC164_SOURCE_DIR)mktime.c  \
	$(MC164_SOURCE_DIR)gmtime.c 
	
MC_COMM_OBJ = $(addsuffix $(_OBJ), $(shell ../shorten $(basename $(notdir $(MC_COMM_SRC)))))

OV_LIBOV_OBJ = $(addsuffix $(_OBJ), $(shell ../shorten $(basename $(notdir $(OV_LIBOV_SRC)))))

OV_LIBOVKS_SRC := $(wildcard $(OV_SOURCE_LIBOVKS_DIR)ov_ksserver*.c)
OV_LIBOVKS_OBJ = $(addsuffix $(_OBJ), $(shell ../shorten $(basename $(notdir $(OV_LIBOVKS_SRC)))))

#	Generate rules for short filenames
#	----------------------------------

ov_libov.in: ../generic.mk
	@echo Generating rules for libov...
	@../mkrules ov_libov.in $(OV_LIBOV_OBJ) $(OV_LIBOV_SRC)

ov_libovks.in: ../generic.mk
	@echo Generating rules for libovks...
	@../mkrules ov_libovks.in $(OV_LIBOVKS_OBJ) $(OV_LIBOVKS_SRC)

mc_comm.in: ../generic.mk
	@echo Generating rules for mc_comm...
	@../mkrules mc_comm.in $(MC_COMM_OBJ) $(MC_COMM_SRC)

#   Implicit Rules
#   --------------

.ovm.c:
	$(CODEGEN) -f $(subst /,\\, $<)

.ovm.h:
	$(CODEGEN) -f $(subst /,\\, $<)

#   Dependencies
#   ------------

depend:
	@echo Unable to generate dependencies under MC164

#	Table of statically linked ACPLT/OV libraries
#	---------------------------------------------

OV_LIBRARIES = $(EXAMPLE_LIB)

$(OV_LIBTABLE_SRC) : $(OV_LIBRARIES)
	@echo Generating static library table file '$@'
	$(MKLIBTAB) $@ $^	
	
#	Libraries
#	---------

#	ACPLT/OV-Library

include ov_libov.in

$(OV_LIBOV_LIB) : $(OV_LIBOV_OBJ) \
	$(addsuffix $(_OBJ), $(shell ../shorten time) $(shell ../shorten strdup) $(shell ../shorten strtoul) $(shell ../shorten gmtime) $(shell ../shorten mktime) $(shell ../shorten mc_div))
	@del $@
	$(AR) create $@
	@( $(foreach object, $^, echo $(AR) add $(object) to $@&) echo rem ) > $(basename $@).bat
	@cmd /q /c $(basename $@).bat
	@del $(basename $@).bat

#	ACPLT/OV library for ACPLT/KS integration

include ov_libovks.in

$(OV_LIBOVKS_LIB) : $(OV_LIBOVKS_OBJ) \
	$(addsuffix $(_OBJ), $(shell ../shorten Kachel) $(shell ../shorten rpchead)  $(shell ../shorten CanKa) $(shell ../shorten mc_srvr))
	@del $@
	$(AR) create $@
	@( $(foreach object, $^, echo $(AR) add $(object) to $@&) echo rem ) > $(basename $@).bat
	@cmd /q /c $(basename $@).bat
	@del $(basename $@).bat

#	supporting functions for MC164

include mc_comm.in

#	Aufraeumen
#	----------

clean:
	@del *.c
	@del *.h
	@del *.bak
	@del *.lst
	@del *$(_LIB)
	@del *$(_OBJ)

#	Include dependencies
#	--------------------

include depend.mc

