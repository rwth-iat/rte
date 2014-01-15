#******************************************************************************
#*
#*   FILE
#*   ----
#*   generic.mk - Makefile for user libs (generic part)
#******************************************************************************
#*
#*   This file is generated by the 'acplt_makmak' command
#*
#******************************************************************************

#	Directories
#	-----------

MODEL_DIR         = ../../model/
SOURCE_DIR        = ../../source/
INCLUDE_DIR       = ../../include/
ROOT_DIR          = ../../../../
BASE_DIR          = $(ROOT_DIR)
BASE_INC_DIR      = $(BASE_DIR)system/sysdevbase/ov/include/
BASE_LIB_DIR      = $(BASE_DIR)system/sysdevbase/ov/lib/
BASE_MODEL_DIR    = $(BASE_DIR)system/sysdevbase/ov/model/
BIN_DIR           = $(ROOT_DIR)system/sysbin/
USER_DIR          = ../../../
USERLIB_DIR       = $(ROOT_DIR)system/addonlibs/
SYS_DIR           = $(BASE_DIR)system/sysdevbase/
SYSLIB_DIR        = $(ROOT_DIR)system/syslibs/
KSBASE_MODEL_DIR           = $(USER_DIR)ksbase/model/
KSBASE_INCLUDE_DIR         = $(USER_DIR)ksbase/include/
#   Rules
#   -----

.SUFFIXES:
.SUFFIXES: $(_C) .h .ovm $(_LIB) $(_DLL) $(_OBJ)

#	Paths and defines
#	-----------------

ifeq ($(COMPILER), MSVC)
INCLUDES  = /I$(BASE_INC_DIR) /I$(BASE_MODEL_DIR) \
			/I$(KSBASE_MODEL_DIR) /I$(KSBASE_INCLUDE_DIR) \
			/I$(MODEL_DIR) /I$(INCLUDE_DIR)
else
INCLUDES  = -I$(BASE_INC_DIR) -I$(BASE_MODEL_DIR)\
			-I$(KSBASE_MODEL_DIR) -I$(KSBASE_INCLUDE_DIR) \
			-I$(MODEL_DIR) -I$(INCLUDE_DIR)
endif

VPATH     = $(MODEL_DIR) $(SOURCE_DIR) $(INCLUDE_DIR) \
			$(KSBASE_MODEL_DIR) $(KSBASE_INCLUDE_DIR) \
			$(BASE_INC_DIR) $(BASE_MODEL_DIR)

ifeq ($(COMPILER), MSVC)
DEFINES    = /DOV_SYSTEM_$(SYSTEM)=1 /DPLT_SYSTEM_$(SYSTEM)=1 /Dov_library_open_UDPbind=ov_library_open_UDPbind_old
else
DEFINES    = -DOV_SYSTEM_$(SYSTEM)=1 -DPLT_SYSTEM_$(SYSTEM)=1 -Dov_library_open_UDPbind=ov_library_open_UDPbind_old
endif

#	User library
#	------------

LIBRARY        = UDPbind
USERLIB_C      = $(LIBRARY)$(_C)
USERLIB_LIB    = $(LIBRARY)$(_LIB)
USERLIB_DLL    = $(LIBRARY)$(_DLL)

#	Targets
#	-------
USERLIB_SRC = $(USERLIB_C) $(wildcard $(SOURCE_DIR)*$(_C))
USERLIB_OBJ = $(foreach source, $(USERLIB_SRC), $(basename $(notdir $(source)))$(_OBJ))

HEADERS = \
	ov.h \
	ksbase.h \
	UDPbind.h 

ifndef STATIC_ONLY
TARGETS = \
	$(HEADERS) \
	$(USERLIB_LIB) \
	$(USERLIB_DLL)
else
TARGETS = \
	$(HEADERS) \
	$(USERLIB_LIB)
endif

SOURCES = \
	ov.h \
	ksbase.h \
	$(USERLIB_SRC)

