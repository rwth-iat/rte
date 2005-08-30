#
# automatisch erzeugtes makefile mit makmak
# (c) 2002 Lehrstuhl für Prozessleittechnik
#

#   Plattform
#   ---------

SYSTEM = NT
SYSDIR = nt

#   Filename conventions
#   --------------------

OBJ_ = .obj
LIB_ = .lib
DLL_ = .dll
EXE_ = .exe
RES_ = .res

#   Include generic part
#   --------------------

include ../generic.mk

#   Oncrpc
#   ------

ONCRPC_DIR              = $(ACPLT_DIR)oncrpc/

#   Libraries
#   ---------

OVLIBS = $(ACPLT_LIB_DIR)libov$(LIB_) $(ACPLT_LIB_DIR)libovks$(LIB_)

BASELIBS =  \
	$(BASELIB0_BUILD_DIR)fb$(LIB_)

#   Compiler
#   --------

OV_CODEGEN_EXE  = $(ACPLT_BIN_DIR)ov_codegen$(EXE_)

MKIMPDEF        = mkimpdef$(EXE_)
MKEXPDEF        = mkexpdef$(EXE_)
MKDLLDEF        = mkdlldef$(EXE_)

CC              = cl
CC_FLAGS        = /W3 /c 
CC_DEFINES      = $(DEFINES)
CC_INCLUDES     = $(INCLUDES) /I$(ONCRPC_DIR)
COMPILE_C       = $(CC) $(CC_FLAGS) /TC $(CC_DEFINES) $(CC_INCLUDES) 

IMPLIB          = cl
IMPDEF          = cl

LD              = link /DLL
#   Implicit Rules
#   --------------

.c$(OBJ_):
	$(COMPILE_C) /out:$@ $<

.ovm.c:
	$(OV_CODEGEN_EXE) -I $(ACPLT_OV_MODEL_DIR) -I $(BASELIB0_MODEL_DIR) -f $(subst /,\\, $<) -l $(notdir $(basename $<))

.ovm.h:
	$(OV_CODEGEN_EXE) -I $(ACPLT_OV_MODEL_DIR) -I $(BASELIB0_MODEL_DIR) -f $(subst /,\\, $<) -l $(notdir $(basename $<))

#   Libraries
#   ---------

fb.h : $(BASELIB0_MODEL_DIR)fb.ovm
	$(OV_CODEGEN_EXE) -I $(ACPLT_OV_MODEL_DIR) -f $(subst /,\\, $<)

ov.h : $(ACPLT_OV_MODEL_DIR)ov.ovm
	$(OV_CODEGEN_EXE) -f $(subst /,\\, $<)

$(MYLIB_LIB) : $(MYLIB_DLL)

$(MYLIB_DLL) : $(MYLIB_OBJ) $(OVLIBS) $(BASELIBS)
	$(LD) /out:$@ /implib:$(MYLIB_LIB) $(filter-out $(RES_), $^)
	copy $(MYLIB_DLL) $(subst /,\\, $(USER_LIBS_DIR))

#   Aufraeumen
#   ----------

clean:
	@-del *.c
	@-del *.h
	@-del *.x
	@-del *.bak
	@-del *.map
	@-del *.pdb
	@-del *.ilk
	@-del *$(LIB_)
	@-del *$(DLL_)
	@-del *$(OBJ_)
	@-del *$(RES_)
	@-del respfile

