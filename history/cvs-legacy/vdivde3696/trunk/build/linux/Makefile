#	Plattform
#	---------

SYSTEM = LINUX
SYSDIR = linux

#	Filename conventions
#	--------------------

OBJ = .o
LIB = .a
DLL = .so
EXE = 

#	Include generic part
#	--------------------

include ../generic.mk


#	Compiler
#	--------

OV_CODEGEN_EXE = $(ACPLT_BIN_DIR)ov_codegen$(EXE)

CC             = gcc
CC_FLAGS       = -g -Wall -O2 -shared
COMPILE_C		= $(CC) $(CC_FLAGS) $(DEFINES) $(INCLUDES) -c
LD             = $(CC) -shared


#   Implicit Rules
#   --------------

.c$(OBJ):
	$(COMPILE_C) -o$@ $<

.ovm.c:
	$(OV_CODEGEN_EXE) -I $(ACPLT_OV_MODEL_DIR) -I $(FB_INCLUDE_DIR) -f $< -l $(notdir $(basename $<))

.ovm.h:
	$(OV_CODEGEN_EXE) -I $(ACPLT_OV_MODEL_DIR) -I $(FB_INCLUDE_DIR) -f $< -l $(notdir $(basename $<))


#   Dependencies
#   ------------

.depend:
	touch $@

depend : $(SOURCES)
	$(COMPILE_C) -MM $^ > .depend


#	Libraries
#	---------

ov.h : $(ACPLT_OV_MODEL_DIR)ov.ovm
	$(OV_CODEGEN_EXE) -f $< -I $(ACPLT_OV_MODEL_DIR)

fb.h : $(FB_INCLUDE_DIR)fb.ovm
	$(OV_CODEGEN_EXE) -f $< -I $(ACPLT_OV_MODEL_DIR) -I $(FB_INCLUDE_DIR)

$(MYLIB_DLL) : $(MYLIB_OBJ)
	$(LD) -o $@ $^
	-cp $(MYLIB_DLL) $(USER_LIBS_DIR)

#	Aufraeumen
#	----------

clean:
	@-rm -f *.c *.h *.o *.so

#    Include dependencies
#    --------------------

include .depend
