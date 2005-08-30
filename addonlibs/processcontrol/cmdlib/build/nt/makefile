#
# PLT-Vorlage
# makefile für iFBSpro-Bibliotheken
#

#	Plattform
#	---------

SYSTEM = NT
SYSDIR = nt

#	Filename conventions
#	--------------------

OBJ = .obj
LIB = .lib
DLL = .dll
EXE = .exe
RES = .res

#	Include generic part
#	--------------------

include ../generic.mk.switch


# 	Oncrpc
#	------

ONCRPC_DIR = $(ACPLT_DIR)base\oncrpc\

OVLIBS = $(ACPLT_LIB_DIR)libov$(LIB) $(ACPLT_LIB_DIR)libovks$(LIB)
MYLIBS = $(FB_LIB_DIR)fb$(LIB) $(COMLIB_LIB_DIR)comlib$(LIB)

#	Compiler
#	--------

OV_CODEGEN_EXE = $(ACPLT_BIN_DIR)ov_codegen$(EXE)

MKIMPDEF        = mkimpdef$(EXE)
MKEXPDEF        = mkexpdef$(EXE)
MKDLLDEF        = mkdlldef$(EXE)

CC              = bcc32
CC_FLAGS        = -w -v -pc -wsig- -a8
CC_DEFINES      = $(DEFINES)
CC_INCLUDES     = $(INCLUDES) -I$(ONCRPC_DIR)
COMPILE_C       = $(CC) $(CC_FLAGS) $(CC_DEFINES) $(CC_INCLUDES) -c

IMPLIB          = implib
IMPDEF          = impdef

LD              = $(CC) $(CC_FLAGS) -tWDE -L$(BC5_LIB_DIR)

AR              = tlib /P64


#   Implicit Rules
#   --------------

.c$(OBJ):
	$(COMPILE_C) -o$@ $<
	
.ovm.c:
	$(OV_CODEGEN_EXE) -f $(subst /,\\, $<) -l $(notdir $(basename $<)) -I $(ACPLT_OV_MODEL_DIR) -I $(FB_MODEL_DIR)

.ovm.h:
	$(OV_CODEGEN_EXE) -f $(subst /,\\, $<) -l $(notdir $(basename $<)) -I $(ACPLT_OV_MODEL_DIR) -I $(FB_MODEL_DIR)


#	Libraries
#	---------
info:
	@echo $(MYLIBS)

ov.h : $(ACPLT_OV_MODEL_DIR)ov.ovm
	$(OV_CODEGEN_EXE) -f $(subst /,\\, $<) -I $(ACPLT_OV_MODEL_DIR)

fb.h : $(FB_MODEL_DIR)fb.ovm
	$(OV_CODEGEN_EXE) -f $(subst /,\\, $<) -I $(ACPLT_OV_MODEL_DIR) -I $(FB_MODEL_DIR)
	
comlib.h : $(COMLIB_MODEL_DIR)comlib.ovm
	$(OV_CODEGEN_EXE) -f $(subst /,\\, $<) -I $(ACPLT_OV_MODEL_DIR) -I $(FB_MODEL_DIR) -I $(COMLIB_MODEL_DIR)

$(MYLIB_LIB) : $(MYLIB_DLL)

$(MYLIB_DLL) : $(MYLIB_OBJ) $(MYLIBS) $(OVLIBS)
	@-del $(basename $@).def
	$(LD) -e$@ $(filter-out %$(RES), $^)
	$(IMPDEF) $(basename $@)_tmp.def $@
	$(MKIMPDEF) $(basename $@)_tmp.def $(basename $@).def
	$(IMPLIB) $(basename $@)$(LIB) $(basename $@).def
	$(MKEXPDEF) $(basename $@)_tmp.def $(basename $@).def
	$(LD) -e$@ $(filter-out %$(RES), $^)
	$(MKDLLDEF) $(basename $@)_tmp.def $(basename $@).def
	@-del $(basename $@)_tmp.def
	copy $(MYLIB_DLL) $(subst /,\\, $(USER_LIBS_DIR))

#
#   Shared Lib
#   ------------
shared: $(TARGETS)
	copy $(MYLIB_DLL) $(subst /,\\, $(ACPLT_SHAREDLIB_DIR))


#	Executables
#	-----------


#	Aufraeumen
#	----------

clean:
	@-del *.c
	@-del *.h
	@-del *.x
	@-del *.bak
	@-del *.map
	@-del *$(LIB)
	@-del *$(DLL)
	@-del *$(OBJ)
	@-del *$(RES)
	@-del respfile
