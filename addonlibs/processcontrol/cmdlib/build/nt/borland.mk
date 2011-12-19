#******************************************************************************
#*
#*   FILE
#*   ----
#*   borland.mk - Makefile fuer die Anwender-Bibliothek
#******************************************************************************
#*
#*   This file is generated by the 'fb_makmak' command
#*
#******************************************************************************

#   Plattform
#   ---------

SYSTEM = NT
SYSDIR = nt

COMPILER = BORLAND

#	Filename conventions
#	--------------------
_C   = .c
_OBJ = .obj
_LIB = .lib
_DLL = .dll
_EXE = .exe
_RES = .res

#	Include generic part
#	--------------------
include ../generic.mk

# Libraries
# ---------
OVLIBS = $(BASE_LIB_DIR)libov$(_LIB) $(BASE_LIB_DIR)libovks$(_LIB)
ADD_LIBS = $(USER_DIR)fb/build/nt/fb$(_LIB) $(USER_DIR)comlib/build/nt/comlib$(_LIB)

all: $(TARGETS)
	@tdstrp32 $(USERLIB_DLL)
	cmd /c copy $(USERLIB_DLL) $(subst /,\\, $(USERLIB_DIR))

debug: $(TARGETS)
	cmd /c copy $(USERLIB_DLL) $(subst /,\\, $(USERLIB_DIR))


#	Compiler
#	--------

OV_CODEGEN_EXE = $(BIN_DIR)ov_codegen$(_EXE)
CC              = bcc32
CC_FLAGS        = -w -v -pc -wsig- -a8 $(EXTRA_CC_FLAGS)
CC_DEFINES      = $(DEFINES) -DFD_SETSIZE=128 -DOV_COMPILE_LIBRARY_$(LIBRARY)
CC_INCLUDES     = $(INCLUDES) -I.
COMPILE_C		= $(CC) $(CC_FLAGS) $(CC_DEFINES) $(CC_INCLUDES) -c
LINK            = $(CC) $(CC_FLAGS)
MKIMPDEF        = $(BIN_DIR)mkimpdef$(_EXE)
MKEXPDEF        = $(BIN_DIR)mkexpdef$(_EXE)
MKDLLDEF        = $(BIN_DIR)mkdlldef$(_EXE)
IMPLIB          = implib
IMPDEF          = impdef
LD              = $(CC) $(CC_FLAGS) -tWDE
AR              = tlib /P64
RC              = brc32

#   Rules
#   -----
$(_C)$(_OBJ):
	$(COMPILE_C) -o$@ $<

.ovm$(_C):
	$(OV_CODEGEN_EXE) -I $(BASE_MODEL_DIR) -I $(FB_MODEL_DIR) -I $(COMLIB_MODEL_DIR) -f $(subst /,\\, $<) -l $(notdir $(basename $<))

.ovm.h:
	$(OV_CODEGEN_EXE) -I $(BASE_MODEL_DIR) -I $(FB_MODEL_DIR) -I $(COMLIB_MODEL_DIR) -f $(subst /,\\, $<) -l $(notdir $(basename $<))

ov.h : $(BASE_MODEL_DIR)ov.ovm
	$(OV_CODEGEN_EXE) -f $<

fb.h : $(FB_MODEL_DIR)fb.ovm
	$(OV_CODEGEN_EXE) -f $< -I $(BASE_MODEL_DIR) -I $(FB_MODEL_DIR) -I $(COMLIB_MODEL_DIR)

comlib.h : $(COMLIB_MODEL_DIR)comlib.ovm
	$(OV_CODEGEN_EXE) -f $< -I $(BASE_MODEL_DIR) -I $(FB_MODEL_DIR) -I $(COMLIB_MODEL_DIR)

cmdlib.h : $(MODEL_DIR)cmdlib.ovm
	$(OV_CODEGEN_EXE) -f $< -I $(BASE_MODEL_DIR) -I $(FB_MODEL_DIR) -I $(COMLIB_MODEL_DIR)

$(USERLIB_LIB) : $(USERLIB_DLL)

$(USERLIB_DLL) : $(USERLIB_OBJ) $(ADD_LIBS) $(OVLIBS)
	-@del $(basename $@).def
	$(LD) -e$@ $(filter-out $(_RES), $^)
	$(IMPDEF) $(basename $@)_tmp.def $@
	$(MKIMPDEF) $(basename $@)_tmp.def $(basename $@).def
	$(IMPLIB) $(basename $@)$(_LIB) $(basename $@).def
	$(MKEXPDEF) $(basename $@)_tmp.def $(basename $@).def
	$(LD) -e$@ $(filter-out $(_RES), $^)
	$(MKDLLDEF) $(basename $@)_tmp.def $(basename $@).def
	@del $(basename $@)_tmp.def

#	Aufraeumen
#	----------

clean:
	-@del *$(_C) *.h *.x *.bak *.map *$(_LIB) *$(_DLL) *$(_OBJ) *$(_RES) respfile
