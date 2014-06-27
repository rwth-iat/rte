#   $Id: unix.mk,v 1.27 2011-07-15 13:36:46 gustavo Exp $
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
#	09-Jul-2001 Ansgar M�nnemann <ansgar@plt.rwth-aachen.de>: ov_builder included.
#	02-Jun-2013 Sten Gruener <s.gruener@plt.rwth-aachen.de>: Notes on valgrind.
#	10-Feb-2014 Lars Evertz <l.evertz@plt.rwth-aachen.de>: Notes on Cross compiling and corresponding enhancements, notes on static linking	

#===Notes on valgrind===#
# In case you want to use valgrind to search for memleaks, do the following:
# 1) recompile libov with -DOV_VALGRIND
# 2) start ov_server in valgrind's environment: 
#    $valgrind --leak-check=yes ov_runtimeserver [params]
# 3) while running, create an OV domain /acplt/malloc, starting from this moment all new ov_database_malloc/
#    ov_database_realloc/ov_database_free calls will be replaced through plain malloc/realloc/free
#    calls and hence logged in valgrind
# 4) load suspect libraries and let them operate in the RAM
# 5) terminate server with ctrl-c and inspect valgrind's output
#
# In case of segfaults, try to run suspect libraries without creating /acplt/malloc and watch for
# database miss output in the console. You may work on stack memory with ov_database_* functions

#===Notes on Cross compiling===#
#  In case you want to cross-compile ov (libov, as well as runtimeserver etc.) 
#  search this file for __CROSS__ and perform the specified modifications in the comments found.
#  Modifications concern build paths and targets for special architectures.

#===Notes on staticly linked servers===#
#  If you need a ststicly linked runtiemserver use the target ov_runtimeserver-static.
#  In this case you need to uncomment the rule for ov_serverlibs.o and fit it to the libraries of your choice.
#  Don't forget to fit the 'ov_serverlibs.c' file to the libraries of your choice then.
#  The target 'statics' builds a static libov, a staticly linked dbutil and a staticly linked runtimeserver.


#   Plattform
#   ---------

SYSTEM = LINUX
SYSDIR = linux

#	C-Includes
#	----------

C_INCLUDES =

#   Include unix generic part
#   -------------------------

#	Filename conventions
#	--------------------

_OBJ = .o
_LIB = .a
_DLL = .so
_EXE =

#	Platform-specific definitions
#	-----------------------------

ACPLTKS_PLATFORM_DEFINES	=
OV_DEBUG					=
ifdef OV_DEBUG
OV_DODEBUG	=	-DOV_DEBUG
else
OV_DODEBUG	=
endif
OV_PLATFORM_DEFINES			= $(OV_DODEBUG) #-DOV_VALGRIND

#	Compiler
#	--------

COMPILER		= GNU

FLEX			= flex
BISON			= bison


#__CROSS__ uncomment the following lines when cross compiling for a target without MMO, using uclibc
CC = ucfront-gcc gcc
CC_FLAGS = -g -Wall -O0 -fomit-frame-pointer -std=c99 -pipe -msoft-float -fno-builtin -DEMBED -fno-strict-aliasing
LD	= $(CC) -Wl,-elf2flt -msoft-float
LD_FLAGS = 

#__CROSS__ NoMMU-> to link to bflat format, the LINK command has to be altered to the commented line
LINK			= $(LD)

COMPILE_C		= $(CC) $(CC_FLAGS) $(DEFINES) $(INCLUDES) -c

C_LIBS			=
LD_LIB			= -ldl

CXX			= gcc -x c++
CXX_FLAGS		= $(CC_FLAGS) -fno-implicit-templates -Wno-deprecated
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
#LIBMPM_LIB			= $(LIBMPM_DIR)libmpm$(_LIB)

#ACPLTKS_LIBS			= $(ACPLT_PLT_BUILD_DIR)libplt$(_LIB) $(LIBRPC_LIB)

#	Targets
#	-------

TARGETS = \
	$(OV_CODEGEN_EXE) \
	$(OV_LIBOV_LIB) \
	$(OV_DBUTIL_EXE) \
	$(OV_RUNTIMESERVER_EXE) \
	$(ACPLT_BUILDER_EXE) \
    	$(ACPLT_MAKMAK_EXE)

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

.lex.c:
	$(FLEX) -o$@ $<

.y.c:
	$(BISON) -t -d -o$@ $<


#__CROSS__ when cross cimpiling you need to remove the './' before Codegen as you do not want to unse the codegen just cross-compiled on your local machine
.ovm.c:
	$(OV_CODEGEN_EXE) -I $(OV_MODEL_DIR) -f $< -l $(notdir $(basename $<))

.ovm.h:
	$(OV_CODEGEN_EXE) -I $(OV_MODEL_DIR) -f $< -l $(notdir $(basename $<))


#   Specialties for staticly linked servers
#   -----------------------------------------

#	Add the '-I' paths for the libraries of your choice, corresponding to the libraries in the 'ov_serverlibs.c' file

ov_serverlibs.o	:	ov_serverlibs.c
	$(COMPILE_C) $< -o $@ -I../../../../../../ksbase/include/	\
			-I../../../../../../ksbase/build/linux/	\
			-I../../../../../../ksxdr/build/linux/	\
			-I../../../../../../kshttp/build/linux/	\
			-I../../../../../../TCPbind/build/linux/	\
			-I../../../../../../fb/build/linux/	\
			-I../../../../../../kbuslib/build/linux/	\
			-I../../../../../../wagoFBKlib/build/linux/	\
			-I../../../../../../iec61131stdfb/build/linux/	\
			-I../../../../../../vdivde3696/build/linux/	\
			-I../../../../../../IOdriverlib/build/linux/	\
			-I../../../../../../dcdriverlib/build/linux/	\
			-I../../../../../../ksapi/build/linux/	\
			-I../../../../../../fbcomlib/build/linux/	\
			-I../../../../../../cmdlib/build/linux/	\
			-I../../../../../../pflib/build/linux/	\
			-I../../../../../../MessageSys/build/linux/	\
			-I../../../../../../PCMsgParser/build/linux/	\
			-I../../../../../../rtov/build/linux/	\
			-I../../../../../../m4pPCU/build/linux/	\
			-I../../../../../../ksbase/model/	\
			-I../../../../../../ksxdr/model/	\
			-I../../../../../../kshttp/model/	\
			-I../../../../../../TCPbind/model/	\
			-I../../../../../../fb/model/	\
			-I../../../../../../kbuslib/model/	\
			-I../../../../../../wagoFBKlib/model/	\
			-I../../../../../../iec61131stdfb/model/	\
			-I../../../../../../vdivde3696/model/	\
			-I../../../../../../rtov/model/	\
			-I../../../../../../dcdriverlib/model/	\
			-I../../../../../../ksapi/model/	\
			-I../../../../../../fbcomlib/model/	\
			-I../../../../../../cmdlib/model/	\
			-I../../../../../../pflib/model/	\
			-I../../../../../../MessageSys/model/	\
			-I../../../../../../PCMsgParser/model/	\
			-I../../../../../../m4pPCU/model/	\
			-I../../../../../../IOdriverlib/model/
			


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
	$(LD) -o $@ $^ $(LD_LIB)

ov.c ov.h : $(OV_CODEGEN_EXE)

#	ACPLT/OV library for ACPLT/KS integration

$(OV_LIBOVKS_LIB) : $(KS_LIBOVKS_OBJ) $(OV_LIBOVKS_OBJ)
	$(AR) rv $@ $?
	$(RANLIB) $@

$(OV_LIBOVKS_DLL) : $(KS_LIBOVKS_OBJ) $(OV_LIBOVKS_OBJ) $(ACPLTKS_LIBS)
	$(LD) -o $@ $^ $(CXX_LIBS)

ov_ksserver$(_OBJ) : $(OV_SOURCE_LIBOVKS_DIR)ov_ksserver.c
	$(CXX_COMPILE) -o $@ $<

ov_ksclient$(_OBJ) : $(OV_SOURCE_LIBOVKS_DIR)ov_ksclient.c
	$(CXX_COMPILE) -o $@ $<

#	Executables
#	-----------

#	ACPLT/OV C code generator

$(OV_CODEGEN_EXE) : $(OV_CODEGEN_OBJ)
	$(LINK) -o $@ $^ $(C_LIBS)

#	ACPLT/OV framework builder

$(OV_BUILDER_EXE) : $(OV_BUILDER_OBJ)
	$(LINK) -o $@ $^ $(C_LIBS)
	strip --strip-debug $(OV_BUILDER_EXE)

#	ACPLT/OV database utility

$(OV_DBUTIL_EXE) : $(OV_DBUTIL_OBJ) $(OV_LIBOV_LIB)
	$(LINK) -o $(OV_DBUTIL_EXE) $^ ../../../../libml/libml.a

#	ACPLT/KS-Server for ACPLT/OV

$(OV_SERVER_EXE) : $(OV_SERVER_OBJ) $(OV_LIBOVKS_DLL) $(OV_LIBOV_DLL)
	$(CXX_LINK) -rdynamic -o $@ $^ $(C_LIBS) $(LD_LIB)

#	ACPLT Runtime-Server for ACPLT/OV

$(OV_RUNTIMESERVER_EXE) : $(OV_RUNTIMESERVER_OBJ) $(OV_LIBOV_LIB)
	$(LINK) -Wl,--whole-archive -Wl,-z,muldefs -o $(OV_RUNTIMESERVER_EXE) $^	\
			libs/ksbase.a	\
			libs/ksxdr.a	\
			libs/kshttp.a	\
			libs/TCPbind.a	\
			libs/fb.a	\
			libs/kbuslib.a	\
			libs/wagoFBKlib.a	\
			libs/IOdriverlib.a	\
			libs/ksapi.a	\
			libs/fbcomlib.a	\
			libs/cmdlib.a	\
			libs/pflib.a	\
			libs/rtov.a	\
			libs/vdivde3696.a	\
			libs/MessageSys.a	\
			libs/PCMsgParser.a	\
			libs/m4pPCU.a	\
			../../../../libml/libml.a
			
#	'flthdr -s' sets the stack-size for the executable. Don't forget to leave the <TAB> there. This is still part of the rule
	flthdr -s 65535 $(OV_RUNTIMESERVER_EXE)



#	ACPLT/OV database dumper

dbdump.o : dbdump.cpp
	 $(CXX) $(CXX_FLAGS)  $(LIBRPC_DEFINES)	$(ACPLTKS_PLATFORM_DEFINES) -DPLT_SYSTEM_$(SYSTEM)=1 -DPLT_USE_BUFFERED_STREAMS=1 -DNDEBUG $(OV_DEFINES) $(INCLUDES) -c $< -o $@

fnmatch.o : fnmatch.c
	 $(CC) $(CC_FLAGS)  $(LIBRPC_DEFINES)	$(ACPLTKS_PLATFORM_DEFINES) -DPLT_SYSTEM_$(SYSTEM)=1 -DPLT_USE_BUFFERED_STREAMS=1 -DNDEBUG $(OV_DEFINES) $(INCLUDES) -c $< -o $@

$(DBDUMP_EXE) : $(DBDUMP_OBJ)
	$(CXX_LINK) -o $@ $^ $(LIBKSCLN_LIB) $(LIBKS_LIB) $(LIBPLT_LIB) $(CXX_LIBS)
	strip --strip-debug $(DBDUMP_EXE)

#	ACPLT/OV database parser

db_y.h: db_y.c


db_lex.o: db_lex.c
	 $(CXX) $(CXX_FLAGS)  $(LIBRPC_DEFINES)	$(ACPLTKS_PLATFORM_DEFINES) -DPLT_SYSTEM_$(SYSTEM)=1 -DPLT_USE_BUFFERED_STREAMS=1 -DNDEBUG $(OV_DEFINES) $(INCLUDES) -c $< -o $@


db_y.o: db_y.c
	 $(CXX) $(CXX_FLAGS)  $(LIBRPC_DEFINES)	$(ACPLTKS_PLATFORM_DEFINES) -DPLT_SYSTEM_$(SYSTEM)=1 -DPLT_USE_BUFFERED_STREAMS=1 -DNDEBUG $(OV_DEFINES) $(INCLUDES) -c $< -o $@

#	ACPLT/OV makmak

$(MAKMAK_EXE) : $(MAKMAK_OBJ)
	$(LINK) -o $@ $^ $(C_LIBS)



#	acplt_builder
#	------------------------
$(ACPLT_BUILDER_EXE) : $(ACPLT_BUILDER_OBJ)
	$(LINK) -o $@ $^ $(C_LIBS) ov_ovmparser$(_OBJ) ov_ovmscanner$(_OBJ)
	

#	acplt_makmak
#	------------------------
$(ACPLT_MAKMAK_EXE) : $(ACPLT_MAKMAK_OBJ)
	$(LINK) -o $@ $^ $(C_LIBS)
	


#	Install
#	-------

install : all
	@echo Installing files to '$(ACPLT_BIN_DIR)'
	@-cp *.so $(ACPLT_BIN_DIR)
	@-cp $(OV_CODEGEN_EXE) $(OV_BUILDER_EXE) $(OV_DBUTIL_EXE) $(OV_SERVER_EXE) $(OV_RUNTIMESERVER_EXE) \
		 $(DBDUMP_EXE) $(DBPARSE_EXE) $(OVXIPARSE_EXE) $(MAKMAK_EXE) $(LIBINFO_EXE) $(ACPLT_BIN_DIR)
	@echo Installing files to '$(ACPLT_LIB_DIR)'
	@-cp *.a $(ACPLT_LIB_DIR)
	@echo Done.

#	Clean up
#	--------

clean :
	@echo Cleaning up...
	@rm -f core *.c ov_ovmparser.h ov.h example.h dynov.h kshist.h db_y.h db_lex.h \
		*$(_LIB) *$(_DLL) *$(_OBJ) *.gdb $(OV_CODEGEN_EXE) $(OV_BUILDER_EXE) \
		$(OV_DBUTIL_EXE) $(OV_SERVER_EXE) $(OV_RUNTIMESERVER_EXE) $(DBDUMP_EXE) $(DBPARSE_EXE) $(MAKMAK_EXE) $(LIBINFO_EXE) $(ACPLT_MAKMAK_EXE) $(ACPLT_BUILDER_EXE)
	@for i in *_inst.h ; do echo > $$i ; done
	@echo Done.

#	Include dependencies
#	--------------------

include .depend

