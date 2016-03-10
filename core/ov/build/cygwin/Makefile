#   $Id: Makefile,v 1.17 2011-08-16 08:32:58 sten Exp $
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



#   Plattform
#   ---------

SYSTEM = NT
SYSDIR = cygwin

#	Filename conventions
#	--------------------

_OBJ = .o
_LIB = .a
_DLL = .dll
_EXE = .exe
_RES = _res.o
_CPL = .cpl

#	Platform-specific definitions
#	-----------------------------

LIBRPC_PLATFORM_DEFINES		=
ACPLTKS_PLATFORM_DEFINES	= -DFD_SETSIZE=128
OV_DEBUG					=
ifdef OV_DEBUG
OV_DODEBUG	=	-DOV_DEBUG
else
OV_DODEBUG	=
endif

ifdef OV_ARCH_BITWIDTH
OV_ARCH_BITWIDTH_CFLAGS		=	-m$(OV_ARCH_BITWIDTH)
OV_ARCH_BITWIDTH_LDFLAGS	=	-m$(OV_ARCH_BITWIDTH)
else
OV_ARCH_BITWIDTH_CFLAGS		=	-m32
OV_ARCH_BITWIDTH_LDFLAGS	=	-m32
endif

OV_PLATFORM_DEFINES			= $(OV_DODEBUG) -DOV_V2SERVER

#	Compiler
#	--------

COMPILER		= GNU
 
OV_CODEGEN_DIR	=	./

CPP_PATH		= 
#c:/tools/bin/

FLEX			= flex
BISON			= bison
CPP 			= cpp

#CC				= gcc -mno-cygwin -I$(MINGWINCLUDEPATH)
ifneq "$(origin GCC_BIN_PREFIX)" "undefined"
CC = $(GCC_BIN_PREFIX)gcc
else
GCC_BIN_PREFIX=
endif
CC				?= $(GCC_BIN_PREFIX)gcc 
#-I$(MINGWINCLUDEPATH)
CC_FLAGS		= -g $(OV_ARCH_BITWIDTH_CFLAGS) -Wall -Wno-attributes -Wstrict-aliasing=2 -fno-strict-aliasing -O0
#-fstrict-aliasing 
COMPILE_C		= $(CC) $(CC_FLAGS) $(DEFINES) $(INCLUDES) -c

#LINK			= gcc -mno-cygwin -L$(MINGWLIBPATH)
LINK			= $(GCC_BIN_PREFIX)gcc  
#-L$(MINGWLIBPATH)
C_LIBS			= 

#CXX				= g++ -mno-cygwin -I$(MINGWINCLUDEPATH)
CXX				= $(GCC_BIN_PREFIX)g++ 
#-I$(MINGWINCLUDEPATH)
CXX_FLAGS		= $(CC_FLAGS) -fno-implicit-templates
CXX_COMPILE		= $(CXX) $(CXX_FLAGS) $(DEFINES) $(INCLUDES) -c

#CXX_LINK		= g++ -mno-cygwin -L$(MINGWLIBPATH)
CXX_LINK		= $(GCC_BIN_PREFIX)g++ 
#-L$(MINGWLIBPATH)
#CXX_LINK		= MAKE=$(MAKE) perl ../../../plt/build/templ.pl g++ -mno-cygwin -L$(MINGWLIBPATH)
CXX_LIBS		= $(C_LIBS) -lwsock32

AR				= $(GCC_BIN_PREFIX)ar
RANLIB			= $(GCC_BIN_PREFIX)ranlib

LD				= $(CC) -shared
LD_LIB			= 

DLLTOOL			= $(GCC_BIN_PREFIX)dlltool
#DLLWRAP			= dllwrap --entry _DllMainCRTStartup@12 --driver-flags -mno-cygwin -mdll -L$(MINGWLIBPATH)
DLLWRAP			= $(GCC_BIN_PREFIX)dllwrap --entry _DllMainCRTStartup@12 --driver-flags -mdll 
#-L$(MINGWLIBPATH)
CXX_DLLWRAP		= $(DLLWRAP) --driver-name g++

WINDRES			= $(GCC_BIN_PREFIX)windres --preprocessor $(CPP_PATH)cpp $(WINDRESDEFS) --define OV_USE_WINDRES \
				  $(foreach sourcedir, $(SOURCE_DIRS) $(OV_INCLUDE_DIR), --include-dir $(sourcedir))
				  
				  #--include-dir $(MINGWINCLUDEPATH)

#	C-Includes
#	----------

C_INCLUDES =

#   Include generic part
#   --------------------

include ../generic.mk


#	all defines and includes together
#	---------------------------------

DEFINES	 = $(LIBRPC_DEFINES) $(ACPLTKS_DEFINES) $(OV_DEFINES)
INCLUDES = $(C_INCLUDES) $(LIBRPC_INCLUDES) $(ACPLTKS_INCLUDES) $(OV_INCLUDES)

#	presupposed libraries
#	---------------------

LIBMPM_LIB			= $(LIBMPM_SOURCE_DIR)libml$(_LIB)

LIBRPC_LIB			= $(ONCRPC_BIN_DIR)oncrpc$(_LIB)

#ACPLTKS_LIBS			= $(ACPLT_PLT_BUILD_DIR)libplt$(_LIB) $(LIBRPC_LIB)
ACPLTKS_LIBS			= ../../../plt/build/cygwin/libplt$(_LIB) $(LIBRPC_LIB)

#	Targets
#	-------

targets: $(TARGETS)

	
example: $(EXAMPLE)
	@

all: targets example


#   Implicit Rules
#   --------------

.c$(_OBJ):
	$(COMPILE_C) $< -o $@

.cpp$(_OBJ):
	$(CXX_COMPILE) $< -o $@

.rc$(_RES):
	$(WINDRES) -i $< -o $@

.lex.c:
	$(FLEX) -o$@ $<

.y.c:
	$(BISON) -t -d -o$@ $<

.ovm.c:
	$(OV_CODEGEN_DIR)$(OV_CODEGEN_EXE) -I $(OV_MODEL_DIR) -f $< -l $(notdir $(basename $<))

.ovm.h:
	$(OV_CODEGEN_DIR)$(OV_CODEGEN_EXE) -I $(OV_MODEL_DIR) -f $< -l $(notdir $(basename $<))

#   Dependencies
#   ------------

depend.cyg:
	touch $@

depend : $(SOURCES)
	$(CXX_COMPILE) -MM $^ > depend.cyg

#	Libraries
#	---------

#	ACPLT/OV library

$(OV_LIBOV_LIB) : $(OV_LIBOV_DLL)
	$(AR) rv $@ $?
	$(RANLIB) $@
	$(DLLTOOL) --def $(basename $@).def --dllname $^ --output-lib $@

#dllwrap have problems on 64bit, but holger is 2014 not brave enough to change 32 bit build
$(OV_LIBOV_DLL) : $(OV_LIBOV_OBJ) $(LIBMPM_LIB) $(OV_LIBOV_RES)
ifeq ($(OV_ARCH_BITWIDTH),64)
	$(LD) -o $@ -Wl,--output-def,$(basename $@).def $^ $(LD_LIB)
else
$(OV_LIBOV_DLL) : $(OV_LIBOV_OBJ) $(LIBMPM_LIB) $(OV_LIBOV_RES)
	@echo EXPORTS > $(basename $@).def
	@$(GCC_BIN_PREFIX)nm $(filter %$(_OBJ), $^) | grep ' [CDT] _' | sed 's/[[:xdigit:]]* [CDT] _/	/' >> $(basename $@).def
	$(DLLWRAP) -mwindows --def $(basename $@).def -o $@ $^
endif

ov.c ov.h : $(OV_CODEGEN_EXE)

#	oncrpc library for ACPLT/KS integration
$(ONCRPC_LIB) : $(ONCRPC_DLL)
	$(DLLTOOL) --def $(basename $@).def --dllname $^ --output-lib $@

$(ONCRPC_DLL) : $(ONCRPC_OBJ)
	@echo EXPORTS > $(basename $@).def
	@$(GCC_BIN_PREFIX)nm $(filter %$(_OBJ), $^) | grep ' [CDT] _' | sed 's/[[:xdigit:]]* [CDT] _/	/' >> $(basename $@).def
	$(DLLWRAP) -mwindows --def $(basename $@).def -o $@ $^

#	ACPLT/OV library for ACPLT/KS integration

$(OV_LIBOVKS_LIB) : $(OV_LIBOVKS_DLL)
	$(DLLTOOL) --def $(basename $@).def --dllname $^ --output-lib $@

$(OV_LIBOVKS_DLL) : $(KS_LIBOVKS_OBJ) $(OV_LIBOVKS_OBJ) $(OV_LIBOV_LIB) $(ACPLTKS_LIBS) $(OV_LIBOVKS_RES)
	@echo EXPORTS > $(basename $@).def
	@$(GCC_BIN_PREFIX)nm $(OV_LIBOVKS_OBJ) | grep ' [CDT] _' | sed 's/[[:xdigit:]]* [CDT] _/	/' >> $(basename $@).def
	$(CXX_DLLWRAP) --def $(basename $@).def -o $@ $^ $(CXX_LIBS)

ov_ksserver$(_OBJ) : $(OV_SOURCE_LIBOVKS_DIR)ov_ksserver.c
	$(CXX_COMPILE) -o $@ $<

ov_ksclient$(_OBJ) : $(OV_SOURCE_LIBOVKS_DIR)ov_ksclient.c
	$(CXX_COMPILE) -o $@ $<

#	Executables
#	-----------

#	ACPLT/OV C code generator

$(OV_CODEGEN_EXE) : $(OV_CODEGEN_OBJ) $(OV_CODEGEN_RES)
	$(LINK) -o $@ $^ $(C_LIBS)

#	ACPLT/OV framework builder

$(OV_BUILDER_EXE) : $(OV_BUILDER_OBJ)
	$(LINK) -o $@ $^ $(C_LIBS)

#	ACPLT/OV database utility

$(OV_DBUTIL_EXE) : $(OV_DBUTIL_OBJ) $(OV_LIBOV_LIB) $(OV_DBUTIL_RES)
	$(LINK) -o $@ $^ $(C_LIBS)

#	ACPLT/KS-Server for ACPLT/OV

$(OV_SERVER_EXE) : $(OV_SERVER_OBJ) $(OV_LIBOVKS_LIB) $(OV_LIBOV_LIB) $(OV_SERVER_RES)
	$(LINK) -o $@ $^ $(C_LIBS)

#	ACPLT Runtime-Server for ACPLT/OV

$(OV_RUNTIMESERVER_EXE) : $(OV_RUNTIMESERVER_OBJ) $(OV_LIBOV_DLL) $(OV_RUNTIMESERVER_RES)
	$(LINK) -o $@ $^ $(C_LIBS)

#	ACPLT/KS-Server for ACPLT/OV as Windows NT service

$(OV_NTSERVICE_EXE) : $(OV_NTSERVICE_OBJ) $(OV_LIBOVKS_LIB) $(OV_LIBOV_LIB) $(OV_NTSERVICE_RES)
	$(LINK) -mwindows -o $@ $^ $(C_LIBS)

#	ACPLT/OV Control Panel for the Windows NT service

$(OV_CONTROLPANEL_CPL) : $(OV_CONTROLPANEL_OBJ) $(OV_CONTROLPANEL_RES)
	$(DLLWRAP) -mwindows --def $(OV_SOURCE_NTSERVICE_DIR)$(basename $@).def -o $@ $^

#	ACPLT/OV database dumper

dbdump.o : dbdump.cpp
	 $(CXX) $(CXX_FLAGS) $(LIBRPC_DEFINES)	$(ACPLTKS_PLATFORM_DEFINES) -DPLT_SYSTEM_$(SYSTEM)=1 -DPLT_USE_BUFFERED_STREAMS=1 -DNDEBUG $(OV_DEFINES) $(INCLUDES) -c $< -o $@

fnmatch.o : fnmatch.c
	 $(CC) $(CC_FLAGS)  $(LIBRPC_DEFINES)	$(ACPLTKS_PLATFORM_DEFINES) -DPLT_SYSTEM_$(SYSTEM)=1 -DPLT_USE_BUFFERED_STREAMS=1 -DNDEBUG $(OV_DEFINES) $(INCLUDES) -c $< -o $@

$(DBDUMP_EXE) : $(DBDUMP_OBJ)
	$(CXX_LINK) -o $@ $^ $(LIBKSCLN_LIB) $(LIBKS_LIB) $(LIBPLT_LIB) $(LIBRPC_LIB) $(CXX_LIBS)

#	ACPLT/OV database parser

db_y.h: db_y.c


db_lex.o: db_lex.c
	 $(CXX) $(CXX_FLAGS)  $(LIBRPC_DEFINES)	$(ACPLTKS_PLATFORM_DEFINES) -DPLT_SYSTEM_$(SYSTEM)=1 -DPLT_USE_BUFFERED_STREAMS=1 -DNDEBUG $(OV_DEFINES) $(INCLUDES) -c $< -o $@


db_y.o: db_y.c
	 $(CXX) $(CXX_FLAGS)  $(LIBRPC_DEFINES)	$(ACPLTKS_PLATFORM_DEFINES) -DPLT_SYSTEM_$(SYSTEM)=1 -DPLT_USE_BUFFERED_STREAMS=1 -DNDEBUG $(OV_DEFINES) $(INCLUDES) -c $< -o $@

dbparse.o : dbparse.cpp
	 $(CXX) $(CXX_FLAGS)  $(LIBRPC_DEFINES)	$(ACPLTKS_PLATFORM_DEFINES) -DPLT_SYSTEM_$(SYSTEM)=1 -DPLT_USE_BUFFERED_STREAMS=1 -DNDEBUG $(OV_DEFINES) $(INCLUDES) -c $< -o $@

dbparse1.o : dbparse1.cpp
	 $(CXX) $(CXX_FLAGS)  $(LIBRPC_DEFINES)	$(ACPLTKS_PLATFORM_DEFINES) -DPLT_SYSTEM_$(SYSTEM)=1 -DOV_SYSTEM_$(SYSTEM)=1 -DPLT_USE_BUFFERED_STREAMS=1 -DNDEBUG $(OV_DEFINES) $(INCLUDES) -c $< -o $@

$(DBPARSE_EXE) : $(DBPARSE_OBJ)  
	$(CXX_LINK) -o $@ $^ $(LIBKSCLN_LIB) $(LIBKS_LIB) $(LIBPLT_LIB) $(LIBRPC_LIB) $(CXX_LIBS)

#	ACPLT/OV OVI/XML parser

ovxiparse.o : ovxiparse.cpp
	 $(CXX) $(CXX_FLAGS)  $(LIBRPC_DEFINES)	$(ACPLTKS_PLATFORM_DEFINES) -DPLT_SYSTEM_$(SYSTEM)=1 -DPLT_USE_BUFFERED_STREAMS=1 -DNDEBUG $(OV_DEFINES) $(INCLUDES) -c $< -o $@

$(OVXIPARSE_EXE) : $(OVXIPARSE_OBJ)  
	$(CXX_LINK) -o $@ $^ $(LIBKSCLN_LIB) $(LIBKS_LIB) $(LIBPLT_LIB) $(LIBRPC_LIB) $(CXX_LIBS)

#	ACPLT/OV makmak

$(MAKMAK_EXE) : $(MAKMAK_OBJ)
	$(LINK) -o $@ $^ $(C_LIBS)

#	ACPLT/OV library informations tool

$(LIBINFO_EXE) : $(LIBINFO_OBJ) $(OV_LIBOV_LIB) $(LIBMPM_LIB)
	$(LINK) -o $@ $^ $(C_LIBS)


#	ACPLT/OV KsHistory library
#	--------------------------

$(KSHISTLIB_LIB) : $(KSHISTLIB_DLL)
	$(DLLTOOL) --def $(basename $@).def --dllname $^ --output-lib $@

$(KSHISTLIB_DLL) : $(KSHISTLIB_OBJ) $(OV_LIBOVKS_LIB) $(OV_LIBOV_LIB)
	@echo EXPORTS > $(basename $@).def
	@$(GCC_BIN_PREFIX)nm $(filter %$(_OBJ), $^) | grep ' [CDT] _' | sed 's/[[:xdigit:]]* [CDT] _/	/' >> $(basename $@).def
	$(DLLWRAP) -mwindows --def $(basename $@).def -o $@ $^

kshist.c kshist.h : $(OV_CODEGEN_EXE)

#	ACPLT/OV example library
#	------------------------

$(EXAMPLE_LIB) : $(EXAMPLE_DLL)
	$(DLLTOOL) --def $(basename $@).def --dllname $^ --output-lib $@

$(EXAMPLE_DLL) : $(EXAMPLE_OBJ) $(OV_LIBOVKS_LIB) $(OV_LIBOV_LIB)
	@echo EXPORTS > $(basename $@).def
	@$(GCC_BIN_PREFIX)nm $(filter %$(_OBJ), $^) | grep ' [CDT] _' | sed 's/[[:xdigit:]]* [CDT] _/	/' >> $(basename $@).def
	$(DLLWRAP) -mwindows --def $(basename $@).def -o $@ $^

example.c example.h : $(OV_CODEGEN_EXE)

#	acplt_builder
#	------------------------
$(ACPLT_BUILDER_EXE) : $(ACPLT_BUILDER_OBJ)
	$(LINK) -o $@ $^ $(C_LIBS) ov_ovmparser$(_OBJ) ov_ovmscanner$(_OBJ)
	$(GCC_BIN_PREFIX)strip --strip-debug $(ACPLT_BUILDER_EXE)

#	acplt_makmak
#	------------------------
$(ACPLT_MAKMAK_EXE) : $(ACPLT_MAKMAK_OBJ)
	$(LINK) -o $@ $^ $(C_LIBS)
	$(GCC_BIN_PREFIX)strip --strip-debug $(ACPLT_MAKMAK_EXE)

#	ACPLT/OV dynov library
#	--------------------------

$(DYNOV_LIB) : $(DYNOV_DLL)
	$(DLLTOOL) --def $(basename $@).def --dllname $^ --output-lib $@

$(DYNOV_DLL) : $(DYNOV_OBJ) $(OV_LIBOVKS_LIB) $(OV_LIBOV_LIB)
	@echo EXPORTS > $(basename $@).def
	@$(GCC_BIN_PREFIX)nm $(filter %$(_OBJ), $^) | grep ' [CDT] _' | sed 's/[[:xdigit:]]* [CDT] _/	/' >> $(basename $@).def
	$(DLLWRAP) -mwindows --def $(basename $@).def -o $@ $^

dynov.c dynov.h : $(OV_CODEGEN_EXE)

#	ACPLT/OV tasklib library
#	--------------------------

$(TASKLIB_LIB) : $(TASKLIB_DLL)
	$(DLLTOOL) --def $(basename $@).def --dllname $^ --output-lib $@

$(TASKLIB_DLL) : $(TASKLIB_OBJ) $(OV_LIBOV_LIB)
	@echo EXPORTS > $(basename $@).def
	@$(GCC_BIN_PREFIX)nm $(filter %$(_OBJ), $^) | grep ' [CDT] _' | sed 's/[[:xdigit:]]* [CDT] _/	/' >> $(basename $@).def
	$(DLLWRAP) -mwindows --def $(basename $@).def -o $@ $^

tasklib.c tasklib.h : $(OV_CODEGEN_EXE)

#	Install
#	-------

install : all
	@echo Installing files to '$(PLT_BIN_DIR)'
	@-cp $(ALL) $(OV_NTSERVICE_EXE) $(OV_CONTROLPANEL_CPL) $(PLT_BIN_DIR)
	@echo Done.

#	Clean up
#	--------

clean :
	-@del *.c *.h *.bak *.map *.def *.s *.exp *$(_LIB) *$(_DLL) *$(_OBJ) *$(_EXE) *$(_RES) *$(_CPL)
	-@rm *.c *.h *.bak *.map *.def *.s *.exp *$(_LIB) *$(_DLL) *$(_OBJ) *$(_EXE) *$(_RES) *$(_CPL)

#	Include dependencies
#	--------------------

include depend.cyg
	
