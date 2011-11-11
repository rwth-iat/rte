#******************************************************************************
#*
#*   FILE
#*   ----
#*   msvc.mk - Makefile fuer die Anwender-Bibliothek
#******************************************************************************
#*
#*   This file is generated by the 'fb_makmak' command
#*
#******************************************************************************

#   Plattform
#   ---------

SYSTEM = NT
SYSDIR = nt

COMPILER = MSVC

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
ADD_LIBS += $(foreach lib, $(EXTRA_LIBS),$(lib))

ADD_LIBS = $(USER_DIR)fb/build/nt/fb$(_LIB) $(USER_DIR)comlib/build/nt/comlib$(_LIB)

all: $(TARGETS)

debug: $(TARGETS)

#	Compiler
#	--------

OV_CODEGEN_EXE = $(BIN_DIR)ov_codegen$(_EXE)
CC              = cl
ifeq ($(MAKECMDGOALS), debug)
CC_FLAGS        = /Zi /MTd /c $(EXTRA_CC_FLAGS)
else
CC_FLAGS        = /W3 /c
endif
CC_DEFINES      = $(DEFINES) /DOV_COMPILE_LIBRARY_$(LIBRARY)
CC_INCLUDES     = $(INCLUDES) /I.
COMPILE_C		= $(CC) $(CC_FLAGS) /TC $(CC_DEFINES) $(CC_INCLUDES) -c
LD              = link /DLL

#   Rules
#   -----
$(_C)$(_OBJ):
	$(COMPILE_C) -o$@ $<

.ovm$(_C):
	$(OV_CODEGEN_EXE) -I $(BASE_MODEL_DIR) -I $(fb_MODEL_DIR) -I $(comlib_MODEL_DIR) -f $(subst /,\\, $<) -l $(notdir $(basename $<))

.ovm.h:
	$(OV_CODEGEN_EXE) -I $(BASE_MODEL_DIR) -I $(fb_MODEL_DIR) -I $(comlib_MODEL_DIR) -f $(subst /,\\, $<) -l $(notdir $(basename $<))

ov.h : $(BASE_MODEL_DIR)ov.ovm
	$(OV_CODEGEN_EXE) -f $<

fb.h : $(FB_MODEL_DIR)fb.ovm
	$(OV_CODEGEN_EXE) -f $< -I $(BASE_MODEL_DIR) -I $(FB_MODEL_DIR) -I $(COMLIB_MODEL_DIR)

comlib.h : $(COMLIB_MODEL_DIR)comlib.ovm
	$(OV_CODEGEN_EXE) -f $< -I $(BASE_MODEL_DIR) -I $(FB_MODEL_DIR) -I $(COMLIB_MODEL_DIR)

cmdlib.h : $(MODEL_DIR)cmdlib.ovm
	$(OV_CODEGEN_EXE) -f $< -I $(BASE_MODEL_DIR) -I $(FB_MODEL_DIR) -I $(COMLIB_MODEL_DIR)

$(USERLIB_LIB) : $(USERLIB_DLL)

$(USERLIB_DLL) : $(USERLIB_OBJ)
	$(LD) $(USERLIB_OBJ) $(ADD_LIBS) $(OVLIBS) $(LINK_FLAGS) /OUT:$@
	cmd /c copy $(USERLIB_DLL) $(subst /,\\, $(USERLIB_DIR))

#	Aufraeumen
#	----------

clean:
	-@del *$(_C) *.h *.x *.bak *.map *$(_LIB) *$(_DLL) *$(_OBJ) *$(_RES) respfile
