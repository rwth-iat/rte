#   $Id: Makefile,v 1.9 2004-10-27 12:21:11 ansgar Exp $
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

OBJ = .o
LIB = .a
DLL = .dll
EXE = .exe
RES = _res.o
CPL = .cpl

#	Platform-specific definitions
#	-----------------------------

LIBRPC_PLATFORM_DEFINES		=
ACPLTKS_PLATFORM_DEFINES	= -DFD_SETSIZE=128
OV_PLATFORM_DEFINES			= -DOV_DEBUG -DOV_V2SERVER

#	Compiler
#	--------

COMPILER		= GNU

CPP_PATH		= c:/tools/bin/

FLEX			= flex
BISON			= bison
CPP 			= cpp

CC				= gcc -mno-cygwin -I$(MINGWINCLUDEPATH)
CC_FLAGS		= -g -Wall -O2
COMPILE_C		= $(CC) $(CC_FLAGS) $(DEFINES) $(INCLUDES) -c

LINK			= gcc -mno-cygwin -L$(MINGWLIBPATH)
C_LIBS			= 

CXX				= g++ -mno-cygwin -I$(MINGWINCLUDEPATH)
CXX_FLAGS		= $(CC_FLAGS) -fno-implicit-templates
CXX_COMPILE		= $(CXX) $(CXX_FLAGS) $(DEFINES) $(INCLUDES) -c

CXX_LINK		= g++ -mno-cygwin -L$(MINGWLIBPATH)
CXX_LIBS		= $(C_LIBS) -lwsock32

AR				= ar
RANLIB			= ranlib

DLLTOOL			= dlltool
DLLWRAP			= dllwrap --entry _DllMainCRTStartup@12 --driver-flags -mno-cygwin -mdll -L$(MINGWLIBPATH)
CXX_DLLWRAP		= $(DLLWRAP) --driver-name g++

WINDRES			= windres --preprocessor $(CPP_PATH)cpp --define OV_USE_WINDRES \
				  $(foreach sourcedir, $(SOURCE_DIRS) $(OV_INCLUDE_DIR), --include-dir $(sourcedir))

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

LIBMPM_LIB			= $(LIBMPM_DIR)libmpm$(LIB)

LIBRPC_LIB			= $(ONCRPC_BIN_DIR)oncrpc$(LIB)

ACPLTKS_LIBS			= $(ACPLT_PLT_BUILD_DIR)libplt$(LIB) $(LIBRPC_LIB)

#	Targets
#	-------

targets: $(TARGETS) $(OV_NTSERVICE_EXE) $(OV_CONTROLPANEL_CPL)

example: $(EXAMPLE)
	@

all: targets example

#   Implicit Rules
#   --------------

.c$(OBJ):
	$(COMPILE_C) $< -o $@

.cpp$(OBJ):
	$(CXX_COMPILE) $< -o $@

.rc$(RES):
	$(WINDRES) -i $< -o $@

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

depend.cyg:
	touch $@

depend : $(SOURCES)
	$(CXX_COMPILE) -MM $^ > depend.cyg

#	Libraries
#	---------

#	ACPLT/OV library

$(OV_LIBOV_LIB) : $(OV_LIBOV_DLL)
	$(DLLTOOL) --def $(basename $@).def --dllname $^ --output-lib $@

$(OV_LIBOV_DLL) : $(OV_LIBOV_OBJ) $(LIBMPM_LIB) $(OV_LIBOV_RES)
	@echo EXPORTS > $(basename $@).def
	@nm $(filter %$(OBJ), $^) | grep ' [CDT] _' | sed 's/[[:xdigit:]]* [CDT] _/	/' >> $(basename $@).def
	$(DLLWRAP) -mwindows --def $(basename $@).def -o $@ $^

ov.c ov.h : $(OV_CODEGEN_EXE)

#	oncrpc library for ACPLT/KS integration
$(ONCRPC_LIB) : $(ONCRPC_DLL)
	$(DLLTOOL) --def $(basename $@).def --dllname $^ --output-lib $@

$(ONCRPC_DLL) : $(ONCRPC_OBJ)
	@echo EXPORTS > $(basename $@).def
	@nm $(filter %$(OBJ), $^) | grep ' [CDT] _' | sed 's/[[:xdigit:]]* [CDT] _/	/' >> $(basename $@).def
	$(DLLWRAP) -mwindows --def $(basename $@).def -o $@ $^

#	ACPLT/OV library for ACPLT/KS integration

$(OV_LIBOVKS_LIB) : $(OV_LIBOVKS_DLL)
	$(DLLTOOL) --def $(basename $@).def --dllname $^ --output-lib $@

$(OV_LIBOVKS_DLL) : $(KS_LIBOVKS_OBJ) $(OV_LIBOVKS_OBJ) $(OV_LIBOV_LIB) $(ACPLTKS_LIBS) $(OV_LIBOVKS_RES)
	@echo EXPORTS > $(basename $@).def
	@nm $(OV_LIBOVKS_OBJ) | grep ' [CDT] _' | sed 's/[[:xdigit:]]* [CDT] _/	/' >> $(basename $@).def
	$(CXX_DLLWRAP) --def $(basename $@).def -o $@ $^ $(CXX_LIBS)

ov_ksserver$(OBJ) : $(OV_SOURCE_LIBOVKS_DIR)ov_ksserver.c
	$(CXX_COMPILE) -o $@ $<

ov_ksclient$(OBJ) : $(OV_SOURCE_LIBOVKS_DIR)ov_ksclient.c
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

#	ACPLT/KS-Server for ACPLT/OV as Windows NT service

$(OV_NTSERVICE_EXE) : $(OV_NTSERVICE_OBJ) $(OV_LIBOVKS_LIB) $(OV_LIBOV_LIB) $(OV_NTSERVICE_RES)
	$(LINK) -mwindows -o $@ $^ $(C_LIBS)

#	ACPLT/OV Control Panel for the Windows NT service

$(OV_CONTROLPANEL_CPL) : $(OV_CONTROLPANEL_OBJ) $(OV_CONTROLPANEL_RES)
	$(DLLWRAP) -mwindows --def $(OV_SOURCE_NTSERVICE_DIR)$(basename $@).def -o $@ $^

#	ACPLT/OV database dumper

$(DBDUMP_EXE) : $(DBDUMP_OBJ)
	$(LINK) -o $@ $^ $(C_LIBS)

#	ACPLT/OV database parser

$(DBPARSE_EXE) : $(DBPASRE_OBJ)
	$(LINK) -o $@ $^ $(C_LIBS)

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
	@nm $(filter %$(OBJ), $^) | grep ' [CDT] _' | sed 's/[[:xdigit:]]* [CDT] _/	/' >> $(basename $@).def
	$(DLLWRAP) -mwindows --def $(basename $@).def -o $@ $^

kshist.c kshist.h : $(OV_CODEGEN_EXE)

#	ACPLT/OV example library
#	------------------------

$(EXAMPLE_LIB) : $(EXAMPLE_DLL)
	$(DLLTOOL) --def $(basename $@).def --dllname $^ --output-lib $@

$(EXAMPLE_DLL) : $(EXAMPLE_OBJ) $(OV_LIBOVKS_LIB) $(OV_LIBOV_LIB)
	@echo EXPORTS > $(basename $@).def
	@nm $(filter %$(OBJ), $^) | grep ' [CDT] _' | sed 's/[[:xdigit:]]* [CDT] _/	/' >> $(basename $@).def
	$(DLLWRAP) -mwindows --def $(basename $@).def -o $@ $^

example.c example.h : $(OV_CODEGEN_EXE)

#	ACPLT/OV dynov library
#	--------------------------

$(DYNOV_LIB) : $(DYNOV_DLL)
	$(DLLTOOL) --def $(basename $@).def --dllname $^ --output-lib $@

$(DYNOV_DLL) : $(DYNOV_OBJ) $(OV_LIBOVKS_LIB) $(OV_LIBOV_LIB)
	@echo EXPORTS > $(basename $@).def
	@nm $(filter %$(OBJ), $^) | grep ' [CDT] _' | sed 's/[[:xdigit:]]* [CDT] _/	/' >> $(basename $@).def
	$(DLLWRAP) -mwindows --def $(basename $@).def -o $@ $^

dynov.c dynov.h : $(OV_CODEGEN_EXE)

#	ACPLT/OV tasklib library
#	--------------------------

$(TASKLIB_LIB) : $(TASKLIB_DLL)
	$(DLLTOOL) --def $(basename $@).def --dllname $^ --output-lib $@

$(TASKLIB_DLL) : $(TASKLIB_OBJ)
	@echo EXPORTS > $(basename $@).def
	@nm $(filter %$(OBJ), $^) | grep ' [CDT] _' | sed 's/[[:xdigit:]]* [CDT] _/	/' >> $(basename $@).def
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
	@del *.c *.h *.bak *.map *.def *.s *.exp *$(LIB) *$(DLL) *$(OBJ) *$(EXE) *$(RES) *$(CPL)

#	Include dependencies
#	--------------------

include depend.cyg
	
=======

#   $Id: Makefile,v 1.9 2004-10-27 12:21:11 ansgar Exp $
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

OBJ = .o
LIB = .a
DLL = .dll
EXE = .exe
RES = _res.o
CPL = .cpl

#	Platform-specific definitions
#	-----------------------------

LIBRPC_PLATFORM_DEFINES		=
ACPLTKS_PLATFORM_DEFINES	= -DFD_SETSIZE=128
OV_PLATFORM_DEFINES			= -DOV_DEBUG -DOV_V2SERVER

#	Compiler
#	--------

COMPILER		= GNU

CPP_PATH		= c:/tools/bin/

FLEX			= flex
BISON			= bison
CPP 			= cpp

CC				= gcc -mno-cygwin -I$(MINGWINCLUDEPATH)
CC_FLAGS		= -g -Wall -O2
COMPILE_C		= $(CC) $(CC_FLAGS) $(DEFINES) $(INCLUDES) -c

LINK			= gcc -mno-cygwin -L$(MINGWLIBPATH)
C_LIBS			= 

CXX				= g++ -mno-cygwin -I$(MINGWINCLUDEPATH)
CXX_FLAGS		= $(CC_FLAGS) -fno-implicit-templates
CXX_COMPILE		= $(CXX) $(CXX_FLAGS) $(DEFINES) $(INCLUDES) -c

CXX_LINK		= g++ -mno-cygwin -L$(MINGWLIBPATH)
CXX_LIBS		= $(C_LIBS) -lwsock32

AR				= ar
RANLIB			= ranlib

DLLTOOL			= dlltool
DLLWRAP			= dllwrap --entry _DllMainCRTStartup@12 --driver-flags -mno-cygwin -mdll -L$(MINGWLIBPATH)
CXX_DLLWRAP		= $(DLLWRAP) --driver-name g++

WINDRES			= windres --preprocessor $(CPP_PATH)cpp --define OV_USE_WINDRES \
				  $(foreach sourcedir, $(SOURCE_DIRS) $(OV_INCLUDE_DIR), --include-dir $(sourcedir))

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

LIBMPM_LIB			= $(LIBMPM_DIR)libmpm$(LIB)

LIBRPC_LIB			= $(ONCRPC_BIN_DIR)oncrpc$(LIB)

ACPLTKS_LIBS			= $(ACPLT_PLT_BUILD_DIR)libplt$(LIB) $(LIBRPC_LIB)

#	Targets
#	-------

targets: $(TARGETS) $(OV_NTSERVICE_EXE) $(OV_CONTROLPANEL_CPL)

example: $(EXAMPLE)
	@

all: targets example

#   Implicit Rules
#   --------------

.c$(OBJ):
	$(COMPILE_C) $< -o $@

.cpp$(OBJ):
	$(CXX_COMPILE) $< -o $@

.rc$(RES):
	$(WINDRES) -i $< -o $@

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

depend.cyg:
	touch $@

depend : $(SOURCES)
	$(CXX_COMPILE) -MM $^ > depend.cyg

#	Libraries
#	---------

#	ACPLT/OV library

$(OV_LIBOV_LIB) : $(OV_LIBOV_DLL)
	$(DLLTOOL) --def $(basename $@).def --dllname $^ --output-lib $@

$(OV_LIBOV_DLL) : $(OV_LIBOV_OBJ) $(LIBMPM_LIB) $(OV_LIBOV_RES)
	@echo EXPORTS > $(basename $@).def
	@nm $(filter %$(OBJ), $^) | grep ' [CDT] _' | sed 's/[[:xdigit:]]* [CDT] _/	/' >> $(basename $@).def
	$(DLLWRAP) -mwindows --def $(basename $@).def -o $@ $^

ov.c ov.h : $(OV_CODEGEN_EXE)

#	oncrpc library for ACPLT/KS integration
$(ONCRPC_LIB) : $(ONCRPC_DLL)
	$(DLLTOOL) --def $(basename $@).def --dllname $^ --output-lib $@

$(ONCRPC_DLL) : $(ONCRPC_OBJ)
	@echo EXPORTS > $(basename $@).def
	@nm $(filter %$(OBJ), $^) | grep ' [CDT] _' | sed 's/[[:xdigit:]]* [CDT] _/	/' >> $(basename $@).def
	$(DLLWRAP) -mwindows --def $(basename $@).def -o $@ $^

#	ACPLT/OV library for ACPLT/KS integration

$(OV_LIBOVKS_LIB) : $(OV_LIBOVKS_DLL)
	$(DLLTOOL) --def $(basename $@).def --dllname $^ --output-lib $@

$(OV_LIBOVKS_DLL) : $(KS_LIBOVKS_OBJ) $(OV_LIBOVKS_OBJ) $(OV_LIBOV_LIB) $(ACPLTKS_LIBS) $(OV_LIBOVKS_RES)
	@echo EXPORTS > $(basename $@).def
	@nm $(OV_LIBOVKS_OBJ) | grep ' [CDT] _' | sed 's/[[:xdigit:]]* [CDT] _/	/' >> $(basename $@).def
	$(CXX_DLLWRAP) --def $(basename $@).def -o $@ $^ $(CXX_LIBS)

ov_ksserver$(OBJ) : $(OV_SOURCE_LIBOVKS_DIR)ov_ksserver.c
	$(CXX_COMPILE) -o $@ $<

ov_ksclient$(OBJ) : $(OV_SOURCE_LIBOVKS_DIR)ov_ksclient.c
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

#	ACPLT/KS-Server for ACPLT/OV as Windows NT service

$(OV_NTSERVICE_EXE) : $(OV_NTSERVICE_OBJ) $(OV_LIBOVKS_LIB) $(OV_LIBOV_LIB) $(OV_NTSERVICE_RES)
	$(LINK) -mwindows -o $@ $^ $(C_LIBS)

#	ACPLT/OV Control Panel for the Windows NT service

$(OV_CONTROLPANEL_CPL) : $(OV_CONTROLPANEL_OBJ) $(OV_CONTROLPANEL_RES)
	$(DLLWRAP) -mwindows --def $(OV_SOURCE_NTSERVICE_DIR)$(basename $@).def -o $@ $^

#	ACPLT/OV database dumper

$(DBDUMP_EXE) : $(DBDUMP_OBJ)
	$(LINK) -o $@ $^ $(C_LIBS)

#	ACPLT/OV database parser

$(DBPARSE_EXE) : $(DBPASRE_OBJ)
	$(LINK) -o $@ $^ $(C_LIBS)

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
	@nm $(filter %$(OBJ), $^) | grep ' [CDT] _' | sed 's/[[:xdigit:]]* [CDT] _/	/' >> $(basename $@).def
	$(DLLWRAP) -mwindows --def $(basename $@).def -o $@ $^

kshist.c kshist.h : $(OV_CODEGEN_EXE)

#	ACPLT/OV example library
#	------------------------

$(EXAMPLE_LIB) : $(EXAMPLE_DLL)
	$(DLLTOOL) --def $(basename $@).def --dllname $^ --output-lib $@

$(EXAMPLE_DLL) : $(EXAMPLE_OBJ) $(OV_LIBOVKS_LIB) $(OV_LIBOV_LIB)
	@echo EXPORTS > $(basename $@).def
	@nm $(filter %$(OBJ), $^) | grep ' [CDT] _' | sed 's/[[:xdigit:]]* [CDT] _/	/' >> $(basename $@).def
	$(DLLWRAP) -mwindows --def $(basename $@).def -o $@ $^

example.c example.h : $(OV_CODEGEN_EXE)

#	ACPLT/OV dynov library
#	--------------------------

$(DYNOV_LIB) : $(DYNOV_DLL)
	$(DLLTOOL) --def $(basename $@).def --dllname $^ --output-lib $@

$(DYNOV_DLL) : $(DYNOV_OBJ) $(OV_LIBOVKS_LIB) $(OV_LIBOV_LIB)
	@echo EXPORTS > $(basename $@).def
	@nm $(filter %$(OBJ), $^) | grep ' [CDT] _' | sed 's/[[:xdigit:]]* [CDT] _/	/' >> $(basename $@).def
	$(DLLWRAP) -mwindows --def $(basename $@).def -o $@ $^

dynov.c dynov.h : $(OV_CODEGEN_EXE)

#	ACPLT/OV tasklib library
#	--------------------------

$(TASKLIB_LIB) : $(TASKLIB_DLL)
	$(DLLTOOL) --def $(basename $@).def --dllname $^ --output-lib $@

$(TASKLIB_DLL) : $(TASKLIB_OBJ)
	@echo EXPORTS > $(basename $@).def
	@nm $(filter %$(OBJ), $^) | grep ' [CDT] _' | sed 's/[[:xdigit:]]* [CDT] _/	/' >> $(basename $@).def
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
	@del *.c *.h *.bak *.map *.def *.s *.exp *$(LIB) *$(DLL) *$(OBJ) *$(EXE) *$(RES) *$(CPL)

#	Include dependencies
#	--------------------

include depend.cyg
	
>>>>>>> 1.8
