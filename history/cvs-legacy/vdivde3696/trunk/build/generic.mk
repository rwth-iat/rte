#
# automatisch erzeugtes generic.mk mit makmak
# (c) 2002 Lehrstuhl für Prozessleittechnik
#

#
#	Name der Bibliothek
#	-------------------
#

MYLIB_NAME = vdivde3696
BASELIB0_NAME = fb

#
#	Directories
#	-----------

USER_MAIN_DIR           = D:\acplt\user/
USER_LIBS_DIR           = $(USER_MAIN_DIR)libs/

ACPLT_DIR               = x:\apps_32\ov\acplt/base/
ACPLT_BIN_DIR           = x:\apps_32\ov\acplt/bin/
ACPLT_LIB_DIR           = $(ACPLT_DIR)lib/
MYLIB_DIR               = ../../
MYLIB_MODEL_DIR         = $(MYLIB_DIR)model/
MYLIB_INCLUDE_DIR       = $(MYLIB_DIR)include/
MYLIB_SOURCE_DIR        = $(MYLIB_DIR)source/

BASELIB0_DIR           = ../../../fb/
BASELIB0_MODEL_DIR     = $(BASELIB0_DIR)model/
BASELIB0_INCLUDE_DIR   = $(BASELIB0_DIR)include/
BASELIB0_SOURCE_DIR    = $(BASELIB0_DIR)source/
BASELIB0_BUILD_DIR     = $(BASELIB0_DIR)build/$(SYSDIR)/

ACPLT_OV_DIR            = $(ACPLT_DIR)ov/
ACPLT_OV_MODEL_DIR      = $(ACPLT_OV_DIR)model/
ACPLT_OV_INCLUDE_DIR    = $(ACPLT_OV_DIR)include/

ACPLT_PLT_DIR           = $(ACPLT_DIR)plt/
ACPLT_PLT_INCLUDE_DIR   = $(ACPLT_PLT_DIR)include/

ACPLT_KS_DIR            = $(ACPLT_DIR)ks/
ACPLT_KS_INCLUDE_DIR    = $(ACPLT_KS_DIR)include/

#	Includes and Defines

MYLIB_INCLUDES = \
	-I$(BASELIB0_MODEL_DIR) \
	-I$(BASELIB0_INCLUDE_DIR) \
	-I$(MYLIB_MODEL_DIR) \
	-I$(MYLIB_INCLUDE_DIR)

MYLIB_DEFINES = \
	-DOV_DEBUG \
	-DOV_COMPILE_LIBRARY_$(MYLIB_NAME)

OV_INCLUDES = \
	-I$(ACPLT_OV_INCLUDE_DIR) \
	-I$(ACPLT_OV_MODEL_DIR) \
	-I$(ACPLT_PLT_INCLUDE_DIR) \
	-I$(ACPLT_KS_INCLUDE_DIR)

OV_DEFINES = \
	-DOV_SYSTEM_$(SYSTEM)=1 \
	-DPLT_SYSTEM_$(SYSTEM)=1

INCLUDES = $(OV_INCLUDES) $(MYLIB_INCLUDES) -I.

DEFINES = $(OV_DEFINES) $(MYLIB_DEFINES)

#	Targets and their sources

MYLIB_SRC = $(MYLIB_NAME).c $(wildcard $(MYLIB_SOURCE_DIR)*.c)
MYLIB_OBJ = $(foreach source, $(MYLIB_SRC), $(basename $(notdir $(source)))$(OBJ_))
MYLIB_DLL = $(MYLIB_NAME)$(DLL_)
MYLIB_LIB = $(MYLIB_NAME)$(LIB_)

SOURCE_DIRS = $(MYLIB_SOURCE_DIR)

#   Rules
#   -----

.SUFFIXES:

.SUFFIXES: .c .h .ovm $(LIB_) $(DLL_) $(OBJ_)

VPATH = $(SOURCE_DIRS) $(ACPLT_OV_MODEL_DIR) $(MYLIB_MODEL_DIR) .

TARGETS = \
	fb.h \
	ov.h \
	$(MYLIB_LIB) \
	$(MYLIB_DLL)

SOURCES = \
	ov.h \
	$(MYLIB_SRC)

all: $(TARGETS)
