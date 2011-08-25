#******************************************************************************
#*
#*   FILE
#*   ----
#*   generic.mk - Makefile fuer die Anwender-Bibliothek (generischer Teil)
#******************************************************************************
#*
#*   This file is generated by the 'fb_makmak' command
#*
#******************************************************************************

#	Directories
#	-----------

MODEL_DIR         = ../../model/
SOURCE_DIR        = ../../source/
INCLUDE_DIR       = ../../include/
USER_DIR          = ../../../
USERLIB_DIR       = $(USER_DIR)libs
ROOT_DIR          = ../../../../
BASE_DIR          = $(ROOT_DIR)base/
BASE_INC_DIR      = $(BASE_DIR)include/
FBS_INC_DIR       = $(BASE_INC_DIR)fb/
BASE_LIB_DIR      = $(BASE_DIR)lib/
BASE_MODEL_DIR    = $(BASE_DIR)model/
BIN_DIR           = $(ROOT_DIR)bin/
#   Rules
#   -----

.SUFFIXES:
.SUFFIXES: $(_C) .h .ovm $(_LIB) $(_DLL) $(_OBJ)

#	Paths and defines
#	-----------------

ifeq ($(COMPILER), MSVC)
INCLUDES  = /I$(BASE_INC_DIR) /I$(FBS_INC_DIR) /I$(BASE_MODEL_DIR) \
			/I$(MODEL_DIR) /I$(INCLUDE_DIR)
else
INCLUDES  = -I$(BASE_INC_DIR) -I$(FBS_INC_DIR) -I$(BASE_MODEL_DIR)\
			-I$(MODEL_DIR) -I$(INCLUDE_DIR)
endif

VPATH     = $(MODEL_DIR) $(SOURCE_DIR) $(INCLUDE_DIR) $(FBS_INC_DIR) \
			$(BASE_INC_DIR) $(BASE_MODEL_DIR)

ifeq ($(COMPILER), MSVC)
DEFINES    = /DOV_SYSTEM_$(SYSTEM)=1 /DPLT_SYSTEM_$(SYSTEM)=1
else
DEFINES    = -DOV_SYSTEM_$(SYSTEM)=1 -DPLT_SYSTEM_$(SYSTEM)=1
endif

#	User library
#	------------

LIBRARY        = cshmi
USERLIB_C      = $(LIBRARY)$(_C)
USERLIB_LIB    = $(LIBRARY)$(_LIB)
USERLIB_DLL    = $(LIBRARY)$(_DLL)

#	Targets
#	-------
USERLIB_SRC = $(USERLIB_C) $(wildcard $(SOURCE_DIR)*$(_C))
USERLIB_OBJ = $(foreach source, $(USERLIB_SRC), $(basename $(notdir $(source)))$(_OBJ))

TARGETS = \
	ov.h \
	fb.h \
	cshmi.h \
	$(USERLIB_LIB) \
	$(USERLIB_DLL)

SOURCES = \
	ov.h \
	fb.h \
	$(USERLIB_SRC)

