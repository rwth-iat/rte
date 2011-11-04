#Hauptmakefile enth�lt Definition der Form:
#SYSTEM = LINUX
#SYSDIR = linux

#	Filename conventions
#	--------------------

OBJ = .o
LIB = .a
DLL = .so
C   = .c

#   Include generic part
#   --------------------

include ../generic.mk

#ONCRPC_DIR		= -I ../../../../base/oncrpc/
USERLIB_DIR		= ../../../libs


USERLIB_SRC = $(USERLIB_C) $(wildcard $(SOURCE_DIR)*.c)
USERLIB_OBJ = $(foreach source, $(USERLIB_SRC), $(basename $(notdir $(source)))$(OBJ))

#	Targets und deren Quellen
#	-------------------------

TARGETS = \
	ov.h \
	fb.h \
	$(USERLIB_LIB) \
	$(USERLIB_DLL)

SOURCES = \
	ov.h \
	fb.h \
	$(USERLIB_SRC)

all: $(TARGETS)

#	Compiler
#	--------

OV_CODEGEN_EXE			= $(BIN_DIR)ov_codegen

CC			= gcc
CC_FLAGS	= -g -Wall -O2 -shared
CC_DEFINES	= $(DEFINES)
CC_INCLUDES	= $(INCLUDES) $(ONCRPC_DIR) -I.
COMPILE_C	= $(CC) $(CC_FLAGS) $(CC_DEFINES) $(CC_INCLUDES) -c

AR			= ar
RANLIB		= ranlib

LD			= ld -shared

#   Implicit Rules
#   --------------

ov.h : $(ACPLT_OV_INCLUDE_DIR)ov.ovm
	$(OV_CODEGEN_EXE) -f $<

fb.h : $(FBS_INCLUDE_DIR)fb.ovm
	$(OV_CODEGEN_EXE) -f $< -I $(ACPLT_OV_INCLUDE_DIR)

.ovm.c:
	$(OV_CODEGEN_EXE) -I $(FBS_INCLUDE_DIR) -I $(ACPLT_OV_INCLUDE_DIR) -f $< -l $(notdir $(basename $<))

.ovm.h:
	$(OV_CODEGEN_EXE) -I $(FBS_INCLUDE_DIR) -I $(ACPLT_OV_INCLUDE_DIR) -f $< -l $(notdir $(basename $<))

.c$(OBJ):
	$(COMPILE_C) $< -o $@



#   Dependencies
#   ------------

.depend:
	touch $@

depend : $(SOURCES)
	$(COMPILE_C) -MM $(USERLIB_SRC) > .depend

#	Libraries
#	---------

$(USERLIB_LIB) : $(USERLIB_OBJ)
	$(AR) rv $@ $^
	$(RANLIB) $@
	@strip --strip-debug $(USERLIB_LIB)

$(USERLIB_DLL) : $(USERLIB_OBJ)
	$(LD) -o $@ $^
	@strip --strip-debug $(USERLIB_DLL)
	cp $@ $(USERLIB_DIR)


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

