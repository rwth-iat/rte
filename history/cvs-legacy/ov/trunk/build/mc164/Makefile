
#   $Id: Makefile,v 1.1 1999-07-19 15:01:56 dirk Exp $
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



#	Plattform
#	---------

SYSTEM = MC164
SYSDIR = mc164

#	Filename conventions
#	--------------------

OBJ = .obj
LIB = .lib
EXE =

#	Platform-specific definitions
#	-----------------------------

ACPLTKS_PLATFORM_DEFINES	= -D__MODEL__=6 -D__C166__
OV_PLATFORM_DEFINES			=

#	Compiler
#	--------

COMPILER	= KEIL

CODEGEN		= ../nt/$(OV_CODEGEN_EXE) 
MKLIBTAB	= ../mklibtab

CPP_PATH	= c:/tools/bin
CPP			= cpp
CC			= c166
CC_FLAGS	= DB M167 F64 HLA NOPR

AR	 		= lib166

#	Include generic part
#	--------------------

include ../generic.mk

#	Targets
#	-------

all: $(OV_LIBOV_LIB) $(OV_LIBOVKS_LIB) ovserver.obj

#	Create short filenames
#	----------------------

MC_COMM_SRC = \
	$(MC164_SOURCE_DIR)time.c \
	$(MC164_SOURCE_DIR)strdup.c \
	$(MC164_SOURCE_DIR)Kachel.c \
	$(MC164_SOURCE_DIR)rpchead.c
MC_COMM_OBJ = $(addsuffix $(OBJ), $(shell ../shorten $(basename $(notdir $(MC_COMM_SRC)))))

OV_LIBOV_OBJ = $(addsuffix $(OBJ), $(shell ../shorten $(basename $(notdir $(OV_LIBOV_SRC)))))

OV_LIBOVKS_SRC := $(wildcard $(OV_SOURCE_LIBOVKS_DIR)ov_ksserver*.c)
OV_LIBOVKS_OBJ = $(addsuffix $(OBJ), $(shell ../shorten $(basename $(notdir $(OV_LIBOVKS_SRC)))))

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
	$(addsuffix $(OBJ), $(shell ../shorten time) $(shell ../shorten strdup))
	@del $@
	$(AR) create $@
	@( $(foreach object, $^, echo $(AR) add $(object) to $@&) echo rem ) > $(basename $@).bat
	@cmd /q /c $(basename $@).bat
	@del $(basename $@).bat

#	ACPLT/OV library for ACPLT/KS integration

include ov_libovks.in

$(OV_LIBOVKS_LIB) : $(OV_LIBOVKS_OBJ) \
	$(addsuffix $(OBJ), $(shell ../shorten Kachel) $(shell ../shorten rpchead))
	@del $@
	$(AR) create $@
	@( $(foreach object, $^, echo $(AR) add $(object) to $@&) echo rem ) > $(basename $@).bat
	@cmd /q /c $(basename $@).bat
	@del $(basename $@).bat

#	supporting functions for MC164

include mc_comm.in

#	ACPLT/OV server

ovserver.obj: $(MC164_SOURCE_DIR)ovserver.c
	$(CPP) -Wall -P -C $(DEFINES) $(INCLUDES) $^ > ovserver.c
	$(CC) ovserver.c $(CC_FLAGS) OJ($@)	

#	Aufraeumen
#	----------

clean:
	@del *.c
	@del *.h
	@del *.bak
	@del *$(LIB)
	@del *$(OBJ)

#	Include dependencies
#	--------------------

include depend.mc

