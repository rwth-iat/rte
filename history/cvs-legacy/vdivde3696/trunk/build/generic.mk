#
#	Name der Bibliothek
#	-------------------
#

MYLIB_NAME = vdivde3696

#
#	Directories
#	-----------

MAIN_DIR                = x:/apps_32/ov/
BC5_MAIN_DIR            = $(MAIN_DIR)bc5/
BC5_INCLUDE_DIR         = $(BC5_MAIN_DIR)include/
BC5_LIB_DIR             = $(BC5_MAIN_DIR)lib/

USER_MAIN_DIR           = g:/acplt/
USER_LIBS_DIR           = $(USER_MAIN_DIR)user/libs/

ACPLT_DIR               = $(MAIN_DIR)acplt/
ACPLT_BIN_DIR           = $(ACPLT_DIR)bin/
ACPLT_LIB_DIR           = $(ACPLT_DIR)base/lib/
ACPLT_USER_DIR          = $(ACPLT_DIR)user/
ACPLT_SHAREDLIB_DIR     = $(MAIN_DIR)sharedlibs

FB_DIR					= $(ACPLT_USER_DIR)fb/
FB_INCLUDE_DIR			= $(FB_DIR)include/
FB_LIB_DIR				= $(FB_DIR)lib/

MYLIB_DIR               = ../../
MYLIB_MODEL_DIR         = $(MYLIB_DIR)model/
MYLIB_INCLUDE_DIR       = $(MYLIB_DIR)include/
MYLIB_SOURCE_DIR        = $(MYLIB_DIR)source/

ACPLT_OV_DIR            = $(ACPLT_DIR)base/ov/
ACPLT_OV_MODEL_DIR      = $(ACPLT_OV_DIR)model/
ACPLT_OV_INCLUDE_DIR    = $(ACPLT_OV_DIR)include/

ACPLT_PLT_DIR           = $(ACPLT_DIR)base/plt/
ACPLT_PLT_INCLUDE_DIR   = $(ACPLT_PLT_DIR)include/

ACPLT_KS_DIR            = $(ACPLT_DIR)base/ks/
ACPLT_KS_INCLUDE_DIR    = $(ACPLT_KS_DIR)include/

ONCRPC_DIR 		= $(ACPLT_DIR)base/oncrpc/


#	Includes and Defines

MYLIB_INCLUDES = \
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

INCLUDES = $(OV_INCLUDES) $(MYLIB_INCLUDES) -I$(BC5_INCLUDE_DIR) -I$(FB_INCLUDE_DIR) -I.

DEFINES = $(OV_DEFINES) $(MYLIB_DEFINES)

#	Targets and their sources

MYLIB_SRC = $(MYLIB_NAME).c $(wildcard $(MYLIB_SOURCE_DIR)*.c)
MYLIB_OBJ = $(foreach source, $(MYLIB_SRC), $(basename $(notdir $(source)))$(OBJ))
MYLIB_DLL = $(MYLIB_NAME)$(DLL)

SOURCE_DIRS = $(MYLIB_SOURCE_DIR)

#   Rules
#   -----

.SUFFIXES:

.SUFFIXES: .c .h .ovm $(LIB) $(DLL) $(OBJ)

VPATH = $(SOURCE_DIRS) $(ACPLT_OV_MODEL_DIR) $(MYLIB_MODEL_DIR) .

TARGETS = \
	ov.h \
	fb.h \
	$(MYLIB_LIB) \
	$(MYLIB_DLL)

SOURCES = \
	ov.h \
	$(MYLIB_SRC)

all: $(TARGETS)
