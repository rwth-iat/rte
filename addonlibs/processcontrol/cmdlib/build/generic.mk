#/*****************************************************************************
#*                                                                            *
#*   i F B S p r o                                                            *
#*   #############                                                            *
#*                                                                            *
#*   © L T S o f t                                                            *
#*   Agentur für Leittechnik Software GmbH                                    *
#*   Heinrich-Hertz-Straße 10                                                 *
#*   50170 Kerpen                                                             *
#*   Tel      : 02273/9893-0                                                  *
#*   Fax      : 02273/52526                                                   *
#*   e-Mail   : ltsoft@ltsoft.de                                              *
#*   Internet : http://www.ltsoft.de                                          *
#*                                                                            *
#******************************************************************************
#*                                                                            *
#*   Datei                                                                    *
#*   -----                                                                    *
#*   generic.mk - Makefile fuer die Anwender-Bibliothek (generischer Teil)    *
#******************************************************************************
#*                                                                            *
#*   Historie                                                                 *
#*   --------                                                                 *
#*                                                                            *
#*****************************************************************************/


#	Directories
#	-----------

BIN_DIR					= ../../../../bin/

SOURCE_DIR				= ../../source/
INCLUDE_DIR				= ../../include/

FBS_DIR					= ../../../../base/fbs/
FBS_INCLUDE_DIR			        = $(FBS_DIR)include/

ACPLT_DIR				= ../../../../base/acplt/

ACPLT_KS_INCLUDE_DIR	= $(ACPLT_DIR)ks/include/
ACPLT_PLT_INCLUDE_DIR	= $(ACPLT_DIR)plt/include/
ACPLT_OV_INCLUDE_DIR	= $(ACPLT_DIR)ov/include/

OV_LIBS_DIR				= $(ACPLT_DIR)ov/lib/
FBS_LIBS_DIR			= $(FBS_DIR)lib/

#   Rules
#   -----

.SUFFIXES:

.SUFFIXES: .c .h .ovm $(LIB) $(DLL) $(OBJ)

#	Paths and defines
#	-----------------

INCLUDES	= -I$(ACPLT_OV_INCLUDE_DIR) -I$(ACPLT_PLT_INCLUDE_DIR) -I$(ACPLT_KS_INCLUDE_DIR) -I$(FBS_INCLUDE_DIR) -I$(INCLUDE_DIR)

VPATH 		= $(SOURCE_DIR) $(INCLUDE_DIR) $(FBS_INCLUDE_DIR) $(ACPLT_OV_INCLUDE_DIR) $(ACPLT_PLT_INCLUDE_DIR) $(ACPLT_KS_INCLUDE_DIR)

DEFINES 	= -DOV_SYSTEM_$(SYSTEM)=1 -DPLT_SYSTEM_$(SYSTEM)=1





###############################################################################
#	Anwender-Bibliothek                                                   #
#	-------------------                                                   #
###############################################################################
# (Nur Bibliothek-Name aendern, z.B. demolib in mylib)                        #
###############################################################################

LIBRARY = cmdlib

###############################################################################
# (ENDE)                                                                      #
###############################################################################




USERLIB_C	= $(LIBRARY)$(C)
USERLIB_LIB = $(LIBRARY)$(LIB)
USERLIB_DLL = $(LIBRARY)$(DLL)
