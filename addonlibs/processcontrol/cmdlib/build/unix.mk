#/******************************************************************************
#***                                                                         ***
#***   FB   -   Funktionsbaustein-Model                                      ***
#***   ###################################################################   ***
#***                                                                         ***
#***   Implementierung des Funktionsbausteinsystems IFBS V3.0	            ***
#***   RWTH,   Aachen                                                        ***
#***   LTSoft, Kerpen                                                        ***
#***                                                                         ***
#*******************************************************************************
#*                                                                             *
#*   Datei                                                                     *
#*   -----                                                                     *
#*   unix.mk - Makefile fuer die fb_model Bibliothek (allgemeiner Unix-Teil)   *
#*******************************************************************************
#*                                                                             *
#*   Historie                                                                  *
#*   --------                                                                  *
#*   1998-10-01  Alexander Neugebauer: Erstellung, LTSoft, Kerpen              *
#*   18-Apr-2000 Murat Fedai: Comlib in cmdlib umbenannt
#*                                                                             *
#******************************************************************************/

#	Plattform
#	---------

#Hauptmakefile enthält Definition der Form:
#SYSTEM = LINUX
#SYSDIR = linux

#	Filename conventions
#	--------------------

OBJ = .o
LIB = .a
DLL = .so

#   Include generic part
#   --------------------

include ../generic.mk

#	Targets
#	-------

all: $(TARGETS)

#	Compiler
#	--------

OV_PRECOMPILER_EXE			= $(BIN_DIR)ov_codegen$(EXE)

CC		= gcc
CC_FLAGS	= -g -Wall -ansi -O2
CC_DEFINES	= -DOV_SYSTEM_$(SYSTEM)=1 -DPLT_SYSTEM_$(SYSTEM)=1
CC_INCLUDES	= -I$(FB_MODEL_INCLUDE_DIR) -I$(FB_INCLUDE_DIR) -I$(INCLUDE_DIR) -I$(OV_MODEL_DIR) \
			  -I$(OV_INCLUDE_DIR) -I$(ACPLT_KS_INCLUDE_DIR) -I$(ACPLT_PLT_INCLUDE_DIR) -I.
COMPILE_C	= $(CC) $(CC_FLAGS) $(CC_DEFINES) $(CC_INCLUDES) -c

AR		= ar
RANLIB		= ranlib

LD		= ld -shared

#   Implicit Rules
#   --------------

fb.h : fb.ovm
	$(OV_PRECOMPILER_EXE) -f $< -I $(OV_MODEL_DIR)

ov.h : $(OV_MODEL_DIR)ov.ovm
	$(OV_PRECOMPILER_EXE) -f $<

.c$(OBJ):
	$(COMPILE_C) $< -o $@

.ovm.c:
	$(OV_PRECOMPILER_EXE) -f $< -I $(FB_MODEL_INCLUDE_DIR) -I $(OV_MODEL_DIR)

.ovm.h:
	$(OV_PRECOMPILER_EXE) -f $< -I $(FB_MODEL_INCLUDE_DIR) -I $(OV_MODEL_DIR)


#   Dependencies
#   ------------

.depend:
	touch $@

depend : $(SOURCES)
	$(COMPILE_C) -MM $^ > .depend
	perl ../depend_nt.pl .depend > ../nt/depend.nt
#	perl ../depend_vms.pl .depend > ../openvms/depend.vms
#	perl ../depend_mc.pl .depend > ../mc164/depend.mc

#	Libraries
#	---------

#	FBS-Bibliothek

$(CMDLIB_LIB) : $(cmdlib_OBJ)
	$(AR) rv $@ $?
	$(RANLIB) $@

$(CMDLIB_DLL) : $(cmdlib_OBJ)
	$(LD) -o $@ $^
	cp $@ $(BIN_DIR)


#	Aufraeumen
#	----------

clean :
	rm -f core
	rm -f *.c
	rm -f *.h
	rm -f *$(LIB)
	rm -f *$(DLL)
	rm -f *$(OBJ)

#	Include dependencies
#	--------------------

include .depend

