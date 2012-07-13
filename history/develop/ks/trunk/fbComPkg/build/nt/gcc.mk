#******************************************************************************
#*
#*   FILE
#*   ----
#*   Makefile - Makefile fuer die Anwender-Bibliothek
#******************************************************************************
#*
#*   This file is generated by the 'acplt_makmak' command
#*
#******************************************************************************

#   Plattform
#   ---------
SYSTEM = NT
SYSDIR = nt

COMPILER = GNU

#	Filename conventions
#	--------------------

_C   = .c
_OBJ = .o
_LIB = .a
_DLL = .dll
_EXE = .exe

#	Include generic part
#	--------------------
include ../generic.mk

# Libraries
# ---------

OVLIBS = $(BASE_LIB_DIR)libov$(_LIB) $(BASE_LIB_DIR)libovks$(_LIB)
ADD_LIBS = $(USER_DIR)libs/fb$(_DLL) $(USER_DIR)libs/ksserv$(_DLL) $(USER_DIR)libs/ksapi$(_DLL)
ADD_LIBS += $(foreach lib, $(EXTRA_LIBS),$(lib))

#	Compiler
#	--------
OV_CODEGEN_EXE = $(BIN_DIR)ov_codegen$(_EXE)
GCC_BIN_PREFIX		= 
CC		= $(GCC_BIN_PREFIX)gcc
ifneq ($(TARGET), debug)
	OPT = -O2 -fno-strict-aliasing
endif
CC_FLAGS	= -g -std=c99 -m32 -Wdeclaration-after-statement -Wall -Wno-attributes $(OPT) -shared $(EXTRA_CC_FLAGS)
CC_DEFINES	= $(DEFINES) -D__NT__=1 
CC_INCLUDES	= $(INCLUDES) -I.
COMPILE_C	= $(CC) $(CC_FLAGS) $(CC_DEFINES) $(CC_INCLUDES) -c
LD		= $(CC) -shared -m32 -Wl,--output-def,fbComPkg.def,--out-implib,fbComPkg.a
AR			= $(GCC_BIN_PREFIX)ar
RANLIB		= $(GCC_BIN_PREFIX)ranlib
STRIP		= $(GCC_BIN_PREFIX)strip

all: $(TARGETS)
	$(STRIP) --strip-debug $(USERLIB_LIB)
	$(STRIP) --strip-debug $(USERLIB_DLL)
	cmd /c copy $(USERLIB_DLL) $(subst /,\\, $(USERLIB_DIR))


debug: $(TARGETS)
	cmd /c copy $(USERLIB_DLL) $(subst /,\\, $(USERLIB_DIR))


#   Rules
#   -----
%.c %.h: %.ovm
	$(OV_CODEGEN_EXE) -I $(BASE_MODEL_DIR) -I $(FB_MODEL_DIR) -I $(KSSERV_MODEL_DIR) -I $(KSAPI_MODEL_DIR) -f $< -l $(notdir $(basename $<))


%.o: %.c
%.o: %.c $(HEADERS)
	$(COMPILE_C) -o $@ $<

$(USERLIB_LIB) : $(USERLIB_DLL)

$(USERLIB_DLL) : $(USERLIB_OBJ) $(ADD_LIBS) $(OVLIBS)
	$(LD) -o $@ $^ $(LD_FLAGS)

#   Dependencies
#   ------------

.depend:
	touch $@

depend : $(SOURCES)
	$(COMPILE_C) -MM $(USERLIB_SRC) > .depend

#	Aufraeumen
#	----------

clean:
	-@cmd /c del *$(_C) *.h *$(_LIB) *$(_DLL) *$(_OBJ)
