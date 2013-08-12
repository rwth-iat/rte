#/******************************************************************************
#***                                                                         ***
#***   iFBSpro   -   Dienstbibliothek                                        ***
#***   #####################################                                 ***
#***                                                                         ***
#***   L T S o f t                                                           ***
#***   Agentur für Leittechnik Software GmbH                                 ***
#***   Heinrich-Hertz-Straße 9                                               ***
#***   50170 Kerpen                                                          ***
#***   Tel: 02273/9893-0                                                     ***
#***   Fax: 02273/9893-33                                                    ***
#***   e-Mail   : ltsoft@ltsoft.de                                           ***
#***   Internet : http://www.ltsoft.de                                       ***
#***                                                                         ***
#***   -------------------------------------------------------------------   ***
#***                                                                         ***
#***                                                                         ***
#*******************************************************************************
#*                                                                             *
#*   Datei                                                                     *
#*   -----                                                                     *
#*   generic.mk                                                                *
#*                                                                             *
#*   Historie                                                                  *
#*   --------                                                                  *
#*   1999-07-08 Alexander Neugebauer: Erstellung, LTSoft GmbH, Kerpen          *
#*                                                                             *
#*   Beschreibung                                                              *
#*   ------------                                                              *
#*   Makefile fuer iFBSpro-Bibliothek (generischer Teil)                       *
#******************************************************************************/

#	Directories
#	-----------

BASE_DIR				= ../../../../

FBSLIB_SRC_DIR          = ../../source/
FBSLIB_INC_DIR          = ../../include/

FB_DIR					= $(BASE_DIR)fbs/fb/
FB_INCLUDE_DIR			= $(FB_DIR)include/
FB_SRC_DIR              = $(FB_DIR)source/upload/

ACPLT_DIR				= $(BASE_DIR)acplt125/

ACPLT_OV_DIR			= $(ACPLT_DIR)ov171/
OV_INCLUDE_DIR			= $(ACPLT_OV_DIR)include/

ACPLT_PLT_DIR			= $(ACPLT_DIR)plt/
ACPLT_PLT_INCLUDE_DIR	= $(ACPLT_PLT_DIR)include/
ACPLT_KS_DIR			= $(ACPLT_DIR)ks/
ACPLT_KS_INCLUDE_DIR	= $(ACPLT_KS_DIR)include/

ONCRPC_DIR				= $(BASE_DIR)oncrpc/
ONCRPC_BIN_DIR          = $(ONCRPC_DIR)bin/

ONCRPC_INCLUDE_DIR		= $(ONCRPC_DIR)

SOURCE_DIRS	= \
	$(FBSLIB_SRC) \
	$(FB_SRC_DIR)


#	all defines and includes together
#	---------------------------------

DEFINES	 = /DPLT_SYSTEM_$(SYSTEM)=1 /DOV_SYSTEM_$(SYSTEM)=1 \
           /DNDEBUG /DPLT_USE_BUFFERED_STREAMS=1 /DFD_SETSIZE=128 /DPLT_DEBUG_NEW=0
INCLUDES = /I$(FBSLIB_INC_DIR) /I$(FB_INCLUDE_DIR) /I$(OV_INCLUDE_DIR) \
           /I$(ACPLT_PLT_INCLUDE_DIR) /I$(ACPLT_KS_INCLUDE_DIR) /I$(ONCRPC_DIR) /I.

#	presupposed libraries
#	---------------------

LIBRPC_LIB			= $(ONCRPC_BIN_DIR)oncrpcms$(LIB)

ifeq ($(SYSTEM), NT)
ACPLTKS_LIBS		= $(ACPLT_PLT_DIR)build/$(SYSDIR)/libplt$(LIB) \
                      $(ACPLT_KS_DIR)build/$(SYSDIR)/libks$(LIB) \
                      $(ACPLT_KS_DIR)build/$(SYSDIR)/libkscln$(LIB) \
                      $(LIBRPC_LIB) 
else
ACPLTKS_LIBS		= $(ACPLT_KS_DIR)build/$(SYSDIR)/libkscln$(LIB) \
                      $(ACPLT_KS_DIR)build/$(SYSDIR)/libks$(LIB) \
                      $(ACPLT_PLT_DIR)build/$(SYSDIR)/libplt$(LIB)
endif

#   Rules
#   -----

.SUFFIXES:

.SUFFIXES: .c .cpp .h .rc .lex .y $(RES) $(LIB) $(OBJ) $(DLL)

VPATH = $(FBSLIB_SRC_DIR) $(FB_SRC_DIR) .

#   Dienst-Lib
#   ----------
DIENST_LIB_SRC = \
	fb_parser.c \
	fb_scanner.c \
	$(FBSLIB_SRC_DIR)ifb_getserver.cpp \
	$(FBSLIB_SRC_DIR)ifb_logerror.cpp \
	$(FBSLIB_SRC_DIR)ifb_varprops.cpp \
	$(FBSLIB_SRC_DIR)ifb_readblockparam.cpp \
	$(FBSLIB_SRC_DIR)ifb_getcondata.cpp \
	$(FBSLIB_SRC_DIR)ifb_getportdata.cpp \
	$(FBSLIB_SRC_DIR)ifb_createcomcon.cpp \
	$(FBSLIB_SRC_DIR)ifb_setpar.cpp \
	$(FBSLIB_SRC_DIR)ifb_av.cpp \
	$(FBSLIB_SRC_DIR)ifb_delobj.cpp \
	$(FBSLIB_SRC_DIR)ifb_crobj.cpp \
	$(FBSLIB_SRC_DIR)ifb_tasklink.cpp \
	$(FBSLIB_SRC_DIR)ifb_rename.cpp \
	$(FBSLIB_SRC_DIR)ifb_delfulltu.cpp \
	$(FBSLIB_SRC_DIR)ifb_dbsaveinstream.cpp \
	$(FBSLIB_SRC_DIR)ifb_memfre.cpp \
	$(FBSLIB_SRC_DIR)ifb_importproject.cpp \
	$(FBSLIB_SRC_DIR)ifb_aufraeumen.cpp \
	$(FBSLIB_SRC_DIR)ifb_importeval.cpp \
	$(FBSLIB_SRC_DIR)ifb_compproject.cpp \
	$(FBSLIB_SRC_DIR)ifb_compeval.cpp \
	$(FBSLIB_SRC_DIR)ifb_updateproject.cpp \
	$(FBSLIB_SRC_DIR)ifb_updateeval.cpp \
	$(FBSLIB_SRC_DIR)ifb_dir.cpp \
	$(FBSLIB_SRC_DIR)ifb_selectsave.cpp \
	$(FBSLIB_SRC_DIR)ifb_dupl.cpp \
	$(FBSLIB_SRC_DIR)ifb_cleandb.cpp \
	$(FBSLIB_SRC_DIR)ifb_fileup.cpp \
	$(FB_SRC_DIR)lts_cfnc.cpp
	

DIENST_LIB_OBJ = $(foreach source, $(DIENST_LIB_SRC), $(basename $(notdir $(source)))$(OBJ))
DIENST_LIB = ifbslib$(LIB)

FB_DBCOMMANDS_SRC = $(FBSLIB_SRC_DIR)dbcommands.cpp
FB_DBCOMMANDS_OBJ = dbcommands$(OBJ)
FB_DBCOMMANDS_EXE = fb_dbcommands$(EXE)

FB_INIT_SRC = $(FBSLIB_SRC_DIR)fb_init.cpp
FB_INIT_OBJ = fb_init$(OBJ)
FB_INIT_EXE = fb_init$(EXE)


TARGETS = \
	$(DIENST_LIB) \
	$(FB_DBCOMMANDS_EXE) \
	$(FB_INIT_EXE)


SOURCES = \
	$(DIENST_LIB_SRC) \
	$(FB_DBCOMMANDS_SRC) \
	$(FB_INIT_SRC)

#	Targets
#	-------

all: $(TARGETS)
