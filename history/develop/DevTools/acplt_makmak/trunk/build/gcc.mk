#   Paths
#   ---------

BASE_DIR = ../../..
INCLUDES = $(OPT_INCLUDES) -I $(BASE_DIR)/ov/source/codegen/ -I $(BASE_DIR)/ov/include/ -I $(BASE_DIR)/ks/include/ -I $(BASE_DIR)/plt/include/
OV_CODEGEN_O = $(BASE_DIR)/ov/build/$(PLATFORM)/ov_ovmparser$(_OBJ) $(BASE_DIR)/ov/build/$(PLATFORM)/ov_ovmscanner$(_OBJ)

SRCDIR = ../../source/
VPATH = $(SRCDIR)

all: acplt_makmak$(_EXE) acplt_builder$(_EXE)
 
acplt_builder$(_OBJ): acplt_builder.c common.h definitions.h 
	$(CC) $(CC_FLAGS) $(CC_DEFINES) -I . $< $(INCLUDES)

acplt_builder$(_EXE): acplt_builder$(_OBJ)
	$(LD) $@ $< $(OV_CODEGEN_O)

acplt_makmak$(_OBJ): acplt_makmak.c common.h definitions.h
	$(CC) $(CC_FLAGS) $(CC_DEFINES) -I . $<

acplt_makmak$(_EXE): acplt_makmak$(_OBJ)
	$(LD) $@ $<

clean:
	-@del acplt_makmak$(_EXE) core acplt_makmak$(_OBJ) acplt_builder$(_OBJ) acplt_builder$(_EXE)
	-@rm acplt_makmak$(_EXE) core acplt_makmak$(_OBJ) acplt_builder$(_OBJ) acplt_builder$(_EXE)
