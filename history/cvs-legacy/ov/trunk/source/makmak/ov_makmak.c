#include <stdio.h>
#include <string.h>
#include "libov/ov_version.h"

#ifdef OV_SYSTEM_NT
char dupath[64] = "c:/acplt/user";
char dapath[64] = "c:/acplt/base";
char dbpath[64] = "c:/acplt/bin";
#endif
#ifdef OV_SYSTEM_LINUX
char dupath[64] = "~/acplt/user";
char dapath[64] = "~/acplt/base";
char dbpath[64] = "~/acplt/bin";
#endif

int main(int argc, char **argv) {
	char *libname=NULL;
	char *upath=dupath;
	char *apath=dapath;
	char *bpath=dbpath;
	char *libs[255];
	char *incpath[255];
	char *copath[255];
	char name[255];
	int j = 0,i,k,l;
	int m = 0;
	int n = 0;
        FILE *fp;

	for(i=1; i<argc; i++) {

		if(!strcmp(argv[i], "-l")) {
			i++;
			if(i<argc) {
				libname = argv[i];
			} else {
				goto HELP;
			}
		}
		else if(!strcmp(argv[i], "-d")) {
			i++;
			if(i<argc) {
				libs[j]=argv[i];
				j++;
			} else {
				goto HELP;
			}
		}
		else if(!strcmp(argv[i], "-pu")) {
			i++;
			if(i<argc) {
				upath=argv[i];
			} else {
				goto HELP;
			}
		}
		else if(!strcmp(argv[i], "-pa")) {
			i++;
			if(i<argc) {
				apath=argv[i];
			} else {
				goto HELP;
			}
		}
		else if(!strcmp(argv[i], "-pb")) {
			i++;
			if(i<argc) {
				bpath=argv[i];
			} else {
				goto HELP;
			}
		}
		else if(!strcmp(argv[i], "-i")) {
			i++;
			if(i<argc) {
				incpath[m]=argv[i];
				m++;
			} else {
				goto HELP;
			}
		}
		else if(!strcmp(argv[i], "-c")) {
			i++;
			if(i<argc) {
				copath[n]=argv[i];
				n++;
			} else {
				goto HELP;
			}
		}
		else if(!strcmp(argv[i], "-v")) {
                        fprintf(stderr, "ACPLT  directory: %s\n", dapath);
                        fprintf(stderr, "USER   directory: %s\n", dupath);
                        fprintf(stderr, "BINARY directory: %s\n", dbpath);
                        fprintf(stderr, "OV_MAKMAK Version: %s\n", OV_VER_MAKMAK);
                        return 0;
		}
		else if(!strcmp(argv[i], "-h")) {
HELP:			fprintf(stderr, "Makefile-Generator: creates generic.mk and makefile for compiling an OV-Library.\n");
                        fprintf(stderr, "An standard file structure of the acplt software is necessary:\n"
                                        "All ov libraries are stored in the USER-directory as an directory with the library name.\n"
                                        "The compiled ov libraries are stored inside the USER/libs-directory.\n"
                                        "Each library directory contains the 'build', 'source', 'model' and (optional) 'include' directory.\n"
                                        "The acplt base software is stored in the ACPLT-directory."
                                        "It's subdivided in the 'plt', 'ks' and 'ov' directory.\n"
                                        "The compiled acplt-software is stored in the BINARY directory.\n\n");
                        fprintf(stderr, "Usage: ov_makmak [arguments]\n"
			"\n"
			"-l  LIBNAME     Set name of the library whose makefile is to be generated\n"
			"-pu PATH        Set path to the USER directory\n"
			"-pa PATH        Set path to the ACPLT directory\n"
			"-pb PATH        Set path to the BINARY directory\n"
			"-i  PATH        Set additional include path for headers or libraries (absolute or relative to <LIBNAME>/build/<SYSTEM>)\n"
			"-c  PATH        Set additional (absolute) path, the compiled library is copied to\n"
			"-d  LIBNAME     Set name of base libraries (consider the correct order)\n"
			"-v              Lists the default settings and version informations\n");
			return 0;
		}
	}

	if(!libname) {
		goto HELP;
	}

	if (upath) sprintf(name,"%s/%s/build/generic.mk",upath,libname);
	else sprintf(name,"%s/build/generic.mk",libname);

	fp = fopen(name, "w");
	if(!fp) {
		fprintf(stderr, "unable to open file for writing: %s.\n", name);
		return -1;
	}


	//
	//	generic.mk
	//
	fprintf(fp,"#\n");
	fprintf(fp,"# automatisch erzeugtes generic.mk mit makmak\n");
	fprintf(fp,"# (c) 2002 Lehrstuhl für Prozessleittechnik\n");
	fprintf(fp,"#\n\n");

	fprintf(fp,"#\n");
	fprintf(fp,"#	Name der Bibliothek\n");
	fprintf(fp,"#	-------------------\n");
	fprintf(fp,"#\n\n");
	fprintf(fp,"MYLIB_NAME = %s\n",libname);
	k = 0;
	while (k < j) {
		fprintf(fp,"BASELIB%d_NAME = %s\n",k,libs[k]);
		k++;
	}
	fprintf(fp,"\n");
	fprintf(fp,"#\n");
	fprintf(fp,"#	Directories\n");
	fprintf(fp,"#	-----------\n\n");
	fprintf(fp,"USER_MAIN_DIR           = %s/\n",upath);
	fprintf(fp,"USER_LIBS_DIR           = $(USER_MAIN_DIR)libs/\n");
	k = 0;
	while (k < n) {
	      fprintf(fp,"COPY_DIR%d              = %s\n",k,copath[k]);
	      k++;
	}
	fprintf(fp,"\n");
	fprintf(fp,"ACPLT_DIR               = %s/\n",apath);
	fprintf(fp,"ACPLT_BIN_DIR           = %s/\n",bpath);
	fprintf(fp,"ACPLT_LIB_DIR           = $(ACPLT_DIR)lib/\n");
	fprintf(fp,"MYLIB_DIR               = ../../\n");
	fprintf(fp,"MYLIB_MODEL_DIR         = $(MYLIB_DIR)model/\n");
	fprintf(fp,"MYLIB_INCLUDE_DIR       = $(MYLIB_DIR)include/\n");
	fprintf(fp,"MYLIB_SOURCE_DIR        = $(MYLIB_DIR)source/\n\n");
	k = 0;
	while (k < j) {
		fprintf(fp,"BASELIB%d_DIR           = ../../../%s/\n",k,libs[k]);
		fprintf(fp,"BASELIB%d_MODEL_DIR     = $(BASELIB%d_DIR)model/\n",k,k);
		fprintf(fp,"BASELIB%d_INCLUDE_DIR   = $(BASELIB%d_DIR)include/\n",k,k);
		fprintf(fp,"BASELIB%d_SOURCE_DIR    = $(BASELIB%d_DIR)source/\n",k,k);
		fprintf(fp,"BASELIB%d_BUILD_DIR     = $(BASELIB%d_DIR)build/$(SYSDIR)/\n\n",k,k);
		k++;
	}
	fprintf(fp,"ACPLT_OV_DIR            = $(ACPLT_DIR)ov/\n");
	fprintf(fp,"ACPLT_OV_MODEL_DIR      = $(ACPLT_OV_DIR)model/\n");
	fprintf(fp,"ACPLT_OV_INCLUDE_DIR    = $(ACPLT_OV_DIR)include/\n\n");
	fprintf(fp,"ACPLT_PLT_DIR           = $(ACPLT_DIR)plt/\n");
	fprintf(fp,"ACPLT_PLT_INCLUDE_DIR   = $(ACPLT_PLT_DIR)include/\n\n");
	fprintf(fp,"ACPLT_KS_DIR            = $(ACPLT_DIR)ks/\n");
	fprintf(fp,"ACPLT_KS_INCLUDE_DIR    = $(ACPLT_KS_DIR)include/\n\n");
	fprintf(fp,"#	Includes and Defines\n\n");
	fprintf(fp,"MYLIB_INCLUDES = \\\n");
	k = 0;
	while (k < j) {
		fprintf(fp,"\t-I$(BASELIB%d_MODEL_DIR) \\\n",k);
		fprintf(fp,"\t-I$(BASELIB%d_INCLUDE_DIR) \\\n",k);
		k++;
	}
	k = 0;
	while (k < m) {
		fprintf(fp,"\t-I %s \\\n",incpath[k]);
		k++;
	}
	fprintf(fp,"\t-I$(MYLIB_MODEL_DIR) \\\n");
	fprintf(fp,"\t-I$(MYLIB_INCLUDE_DIR)\n\n");
	fprintf(fp,"MYLIB_DEFINES = \\\n");
	fprintf(fp,"\t-DOV_DEBUG \\\n");
	fprintf(fp,"\t-DOV_COMPILE_LIBRARY_$(MYLIB_NAME)\n\n");
	fprintf(fp,"OV_INCLUDES = \\\n");
	fprintf(fp,"\t-I$(ACPLT_OV_INCLUDE_DIR) \\\n");
	fprintf(fp,"\t-I$(ACPLT_OV_MODEL_DIR) \\\n");
	fprintf(fp,"\t-I$(ACPLT_PLT_INCLUDE_DIR) \\\n");
	fprintf(fp,"\t-I$(ACPLT_KS_INCLUDE_DIR)\n\n");
	fprintf(fp,"OV_DEFINES = \\\n");
	fprintf(fp,"\t-DOV_SYSTEM_$(SYSTEM)=1 \\\n");
	fprintf(fp,"\t-DPLT_SYSTEM_$(SYSTEM)=1\n\n");
	fprintf(fp,"INCLUDES = $(OV_INCLUDES) $(MYLIB_INCLUDES) -I.\n\n");
	fprintf(fp,"DEFINES = $(OV_DEFINES) $(MYLIB_DEFINES)\n\n");
	fprintf(fp,"#	Targets and their sources\n\n");
	fprintf(fp,"MYLIB_SRC = $(MYLIB_NAME).c $(wildcard $(MYLIB_SOURCE_DIR)*.c)\n");
	fprintf(fp,"MYLIB_OBJ = $(foreach source, $(MYLIB_SRC), $(basename $(notdir $(source)))$(OBJ))\n");
	fprintf(fp,"MYLIB_DLL = $(MYLIB_NAME)$(DLL)\n");
	fprintf(fp,"MYLIB_LIB = $(MYLIB_NAME)$(LIB)\n\n");
	fprintf(fp,"SOURCE_DIRS = $(MYLIB_SOURCE_DIR)\n\n");
	fprintf(fp,"#   Rules\n");
	fprintf(fp,"#   -----\n\n");
	fprintf(fp,".SUFFIXES:\n\n");
	fprintf(fp,".SUFFIXES: .c .h .ovm $(LIB) $(DLL) $(OBJ)\n\n");
	fprintf(fp,"VPATH = $(SOURCE_DIRS) $(ACPLT_OV_MODEL_DIR) $(MYLIB_MODEL_DIR) .\n\n");
	fprintf(fp,"TARGETS = \\\n");
	k = 0;
	while (k < j) {
		fprintf(fp,"\t%s.h \\\n",libs[k]);
		k++;
	}
	fprintf(fp,"\tov.h \\\n");
	fprintf(fp,"\t$(MYLIB_LIB) \\\n");
	fprintf(fp,"\t$(MYLIB_DLL)\n\n");
	fprintf(fp,"SOURCES = \\\n");
	fprintf(fp,"\tov.h \\\n");
	fprintf(fp,"\t$(MYLIB_SRC)\n\n");
	fprintf(fp,"all: $(TARGETS)\n");

	if(fclose(fp)) {
		fprintf(stderr, "error closing file.\n");
		return -1;
	}

	//
	//	Makefile
	//
#ifdef OV_SYSTEM_NT
	if (upath) sprintf(name,"%s/%s/build/nt/borland.mk",upath,libname);
	else sprintf(name,"%s/build/nt/borland.mk",libname);
#endif
#ifdef OV_SYSTEM_LINUX
	if (upath) sprintf(name,"%s/%s/build/linux/Makefile",upath,libname);
	else sprintf(name,"%s/build/linux/Makefile",libname);
#endif
	fp = fopen(name, "w");
	if(!fp) {
		fprintf(stderr, "unable to open file for writing: %s.\n", name);
		return -1;
	}

	fprintf(fp,"#\n");
	fprintf(fp,"# automatisch erzeugtes makefile mit makmak\n");
	fprintf(fp,"# (c) 2002 Lehrstuhl für Prozessleittechnik\n");
	fprintf(fp,"#\n\n");
	fprintf(fp,"#   Plattform\n");
	fprintf(fp,"#   ---------\n\n");
#ifdef OV_SYSTEM_NT
	fprintf(fp,"SYSTEM = NT\n");
	fprintf(fp,"SYSDIR = nt\n\n");
	fprintf(fp,"#   Filename conventions\n");
	fprintf(fp,"#   --------------------\n\n");
	fprintf(fp,"OBJ = .obj\n");
	fprintf(fp,"LIB = .lib\n");
	fprintf(fp,"DLL = .dll\n");
	fprintf(fp,"EXE = .exe\n");
	fprintf(fp,"RES = .res\n\n");
#endif
#ifdef OV_SYSTEM_LINUX
	fprintf(fp,"SYSTEM = LINUX\n");
	fprintf(fp,"SYSDIR = linux\n\n");
	fprintf(fp,"#   Filename conventions\n");
	fprintf(fp,"#   --------------------\n\n");
	fprintf(fp,"OBJ = .o\n");
	fprintf(fp,"LIB = .a\n");
	fprintf(fp,"DLL = .so\n");
	fprintf(fp,"EXE = \n\n");
#endif
	fprintf(fp,"#   Include generic part\n");
	fprintf(fp,"#   --------------------\n\n");
	fprintf(fp,"include ../generic.mk\n\n");
#ifdef OV_SYSTEM_NT
	//
	//	Makefile NT (Borland)
	//
	fprintf(fp,"#   Oncrpc\n");
	fprintf(fp,"#   ------\n\n");
	fprintf(fp,"ONCRPC_DIR              = $(ACPLT_DIR)oncrpc/\n\n");
	fprintf(fp,"#   Libraries\n");
	fprintf(fp,"#   ---------\n\n");
	fprintf(fp,"OVLIBS = $(ACPLT_LIB_DIR)libov$(LIB) $(ACPLT_LIB_DIR)libovks$(LIB)\n\n");
	fprintf(fp,"BASELIBS = ");
	k = 0;
	while (k < j) {
		fprintf(fp," \\\n");
		fprintf(fp,"\t$(BASELIB%d_BUILD_DIR)%s$(LIB)",k,libs[k]);
		k++;
	}
	fprintf(fp,"\n\n");
	fprintf(fp,"#   Compiler\n");
	fprintf(fp,"#   --------\n\n");
	fprintf(fp,"OV_CODEGEN_EXE  = $(ACPLT_BIN_DIR)ov_codegen$(EXE)\n\n");
	fprintf(fp,"MKIMPDEF        = mkimpdef$(EXE)\n");
	fprintf(fp,"MKEXPDEF        = mkexpdef$(EXE)\n");
	fprintf(fp,"MKDLLDEF        = mkdlldef$(EXE)\n\n");
	fprintf(fp,"CC              = bcc32\n");
	fprintf(fp,"CC_FLAGS        = -w -v -pc -wsig- -a8\n");
	fprintf(fp,"CC_DEFINES      = $(DEFINES)\n");
	fprintf(fp,"CC_INCLUDES     = $(INCLUDES) -I$(ONCRPC_DIR)\n");
	fprintf(fp,"COMPILE_C       = $(CC) $(CC_FLAGS) $(CC_DEFINES) $(CC_INCLUDES) -c\n\n");
	fprintf(fp,"IMPLIB          = implib\n");
	fprintf(fp,"IMPDEF          = impdef\n\n");
	fprintf(fp,"LD              = $(CC) $(CC_FLAGS) -tWDE");
        k = 0;
        while (k < m) {
        	fprintf(fp," -L%s",incpath[k]);
                k++;
        }
	fprintf(fp,"\n");
	fprintf(fp,"AR              = tlib /P64\n\n");
	fprintf(fp,"#   Implicit Rules\n");
	fprintf(fp,"#   --------------\n\n");
	fprintf(fp,".c$(OBJ):\n");
	fprintf(fp,"\t$(COMPILE_C) -o$@ $<\n\n");
	fprintf(fp,".ovm.c:\n");
	fprintf(fp,"\t$(OV_CODEGEN_EXE) -I $(ACPLT_OV_MODEL_DIR) ");
	k = 0;
	while (k < j) {
		fprintf(fp,"-I $(BASELIB%d_MODEL_DIR) ",k);
		k++;
	}
	fprintf(fp,"-f $(subst /,\\\\, $<) -l $(notdir $(basename $<))\n\n");
	fprintf(fp,".ovm.h:\n");
	fprintf(fp,"\t$(OV_CODEGEN_EXE) -I $(ACPLT_OV_MODEL_DIR) ");
	k = 0;
	while (k < j) {
		fprintf(fp,"-I $(BASELIB%d_MODEL_DIR) ",k);
		k++;
	}
	fprintf(fp,"-f $(subst /,\\\\, $<) -l $(notdir $(basename $<))\n\n");
	fprintf(fp,"#   Libraries\n");
	fprintf(fp,"#   ---------\n\n");
	k = 0;
	while (k < j) {
		fprintf(fp,"%s.h : $(BASELIB%d_MODEL_DIR)%s.ovm\n",libs[k],k,libs[k]);
		fprintf(fp,"\t$(OV_CODEGEN_EXE) -I $(ACPLT_OV_MODEL_DIR) ");
		l = k + 1;
		while (l < j) {
			fprintf(fp,"-I $(BASELIB%d_MODEL_DIR) ",l);
			l++;
		}
		fprintf(fp,"-f $(subst /,\\\\, $<)\n\n");
		k++;
	}
	fprintf(fp,"ov.h : $(ACPLT_OV_MODEL_DIR)ov.ovm\n");
	fprintf(fp,"\t$(OV_CODEGEN_EXE) -f $(subst /,\\\\, $<)\n\n");
	fprintf(fp,"$(MYLIB_LIB) : $(MYLIB_DLL)\n\n");
	fprintf(fp,"$(MYLIB_DLL) : $(MYLIB_OBJ) $(OVLIBS) $(BASELIBS)\n");
	fprintf(fp,"\t@-del $(basename $@).def\n");
	fprintf(fp,"\t$(LD) -e$@ $(filter-out %$(RES), $^)\n");
	fprintf(fp,"\t$(IMPDEF) $(basename $@)_tmp.def $@\n");
	fprintf(fp,"\t$(MKIMPDEF) $(basename $@)_tmp.def $(basename $@).def\n");
	fprintf(fp,"\t$(IMPLIB) $(basename $@)$(LIB) $(basename $@).def\n");
 	fprintf(fp,"\t$(MKEXPDEF) $(basename $@)_tmp.def $(basename $@).def\n");
	fprintf(fp,"\t$(LD) -e$@ $(filter-out %$(RES), $^)\n");
	fprintf(fp,"\t$(MKDLLDEF) $(basename $@)_tmp.def $(basename $@).def\n");
	fprintf(fp,"\t@-del $(basename $@)_tmp.def\n");
	fprintf(fp,"\tcopy $(MYLIB_DLL) $(subst /,\\\\, $(USER_LIBS_DIR))\n\n");
        k = 0;
	while (k < n) {
  	      fprintf(fp,"\t-cp $(MYLIB_DLL) $(subst /,\\\\, $(COPY_DIR%d))\n",k);
  	      k++;
	}
	fprintf(fp,"\n");
	fprintf(fp,"#\n");
	fprintf(fp,"#   Shared Lib\n");
	fprintf(fp,"#   ----------\n");
	fprintf(fp,"shared: $(TARGETS)\n");
	fprintf(fp,"\tcopy $(MYLIB_DLL) $(subst /,\\\\, $(ACPLT_SHAREDLIB_DIR))\n\n");
	fprintf(fp,"#   Executables\n");
	fprintf(fp,"#   -----------\n\n");
	fprintf(fp,"#   Aufraeumen\n");
	fprintf(fp,"#   ----------\n\n");
	fprintf(fp,"clean:\n");
	fprintf(fp,"\t@-del *.c\n");
	fprintf(fp,"\t@-del *.h\n");
	fprintf(fp,"\t@-del *.x\n");
	fprintf(fp,"\t@-del *.bak\n");
	fprintf(fp,"\t@-del *.map\n");
	fprintf(fp,"\t@-del *$(LIB)\n");
	fprintf(fp,"\t@-del *$(DLL)\n");
	fprintf(fp,"\t@-del *$(OBJ)\n");
	fprintf(fp,"\t@-del *$(RES)\n");
	fprintf(fp,"\t@-del respfile\n\n");
	if(fclose(fp)) {
		fprintf(stderr, "error closing file.\n");
		return -1;
	}
	//
	//	Makefile NT (VC)
	//
	if (upath) sprintf(name,"%s/%s/build/nt/msvc.mk",upath,libname);
	else sprintf(name,"%s/build/nt/msvc.mk",libname);
	fp = fopen(name, "w");
	if(!fp) {
		fprintf(stderr, "unable to open file for writing: %s.\n", name);
		return -1;
	}
	fprintf(fp,"#\n");
	fprintf(fp,"# automatisch erzeugtes makefile mit makmak\n");
	fprintf(fp,"# (c) 2002 Lehrstuhl für Prozessleittechnik\n");
	fprintf(fp,"#\n\n");
	fprintf(fp,"#   Plattform\n");
	fprintf(fp,"#   ---------\n\n");
	fprintf(fp,"SYSTEM = NT\n");
	fprintf(fp,"SYSDIR = nt\n\n");
	fprintf(fp,"#   Filename conventions\n");
	fprintf(fp,"#   --------------------\n\n");
	fprintf(fp,"OBJ = .obj\n");
	fprintf(fp,"LIB = .lib\n");
	fprintf(fp,"DLL = .dll\n");
	fprintf(fp,"EXE = .exe\n");
	fprintf(fp,"RES = .res\n\n");
	fprintf(fp,"#   Include generic part\n");
	fprintf(fp,"#   --------------------\n\n");
	fprintf(fp,"include ../generic.mk\n\n");
	fprintf(fp,"#   Oncrpc\n");
	fprintf(fp,"#   ------\n\n");
	fprintf(fp,"ONCRPC_DIR              = $(ACPLT_DIR)oncrpc/\n\n");
	fprintf(fp,"#   Libraries\n");
	fprintf(fp,"#   ---------\n\n");
	fprintf(fp,"OVLIBS = $(ACPLT_LIB_DIR)libov$(LIB) $(ACPLT_LIB_DIR)libovks$(LIB)\n\n");
	fprintf(fp,"BASELIBS = ");
	k = 0;
	while (k < j) {
		fprintf(fp," \\\n");
		fprintf(fp,"\t$(BASELIB%d_BUILD_DIR)%s$(LIB)",k,libs[k]);
		k++;
	}
	fprintf(fp,"\n\n");
	fprintf(fp,"#   Compiler\n");
	fprintf(fp,"#   --------\n\n");
	fprintf(fp,"OV_CODEGEN_EXE  = $(ACPLT_BIN_DIR)ov_codegen$(EXE)\n\n");
	fprintf(fp,"MKIMPDEF        = mkimpdef$(EXE)\n");
	fprintf(fp,"MKEXPDEF        = mkexpdef$(EXE)\n");
	fprintf(fp,"MKDLLDEF        = mkdlldef$(EXE)\n\n");
	fprintf(fp,"CC              = cl\n");
	fprintf(fp,"CC_FLAGS        = /W3 /c \n");
	fprintf(fp,"CC_DEFINES      = $(DEFINES)\n");
	fprintf(fp,"CC_INCLUDES     = $(INCLUDES) /I$(ONCRPC_DIR)\n");
	fprintf(fp,"COMPILE_C       = $(CC) $(CC_FLAGS) /TC $(CC_DEFINES) $(CC_INCLUDES) \n\n");
	fprintf(fp,"IMPLIB          = cl\n");
	fprintf(fp,"IMPDEF          = cl\n\n");
	fprintf(fp,"LD              = link /DLL");
        k = 0;
        while (k < m) {
        	fprintf(fp," /LIBPATH:%s",incpath[k]);
                k++;
        }
	fprintf(fp,"\n");
	fprintf(fp,"#   Implicit Rules\n");
	fprintf(fp,"#   --------------\n\n");
	fprintf(fp,".c$(OBJ):\n");
	fprintf(fp,"\t$(COMPILE_C) /out:$@ $<\n\n");
	fprintf(fp,".ovm.c:\n");
	fprintf(fp,"\t$(OV_CODEGEN_EXE) -I $(ACPLT_OV_MODEL_DIR) ");
	k = 0;
	while (k < j) {
		fprintf(fp,"-I $(BASELIB%d_MODEL_DIR) ",k);
		k++;
	}
	fprintf(fp,"-f $(subst /,\\\\, $<) -l $(notdir $(basename $<))\n\n");
	fprintf(fp,".ovm.h:\n");
	fprintf(fp,"\t$(OV_CODEGEN_EXE) -I $(ACPLT_OV_MODEL_DIR) ");
	k = 0;
	while (k < j) {
		fprintf(fp,"-I $(BASELIB%d_MODEL_DIR) ",k);
		k++;
	}
	fprintf(fp,"-f $(subst /,\\\\, $<) -l $(notdir $(basename $<))\n\n");
	fprintf(fp,"#   Libraries\n");
	fprintf(fp,"#   ---------\n\n");
	k = 0;
	while (k < j) {
		fprintf(fp,"%s.h : $(BASELIB%d_MODEL_DIR)%s.ovm\n",libs[k],k,libs[k]);
		fprintf(fp,"\t$(OV_CODEGEN_EXE) -I $(ACPLT_OV_MODEL_DIR) ");
		l = k + 1;
		while (l < j) {
			fprintf(fp,"-I $(BASELIB%d_MODEL_DIR) ",l);
			l++;
		}
		fprintf(fp,"-f $(subst /,\\\\, $<)\n\n");
		k++;
	}
	fprintf(fp,"ov.h : $(ACPLT_OV_MODEL_DIR)ov.ovm\n");
	fprintf(fp,"\t$(OV_CODEGEN_EXE) -f $(subst /,\\\\, $<)\n\n");
	fprintf(fp,"$(MYLIB_LIB) : $(MYLIB_DLL)\n\n");
	fprintf(fp,"$(MYLIB_DLL) : $(MYLIB_OBJ) $(OVLIBS) $(BASELIBS)\n");
	fprintf(fp,"\t$(LD) /out:$@ /implib:$(MYLIB_LIB) $(filter-out %$(RES), $^)\n");
	fprintf(fp,"\tcopy $(MYLIB_DLL) $(subst /,\\\\, $(USER_LIBS_DIR))\n\n");
	fprintf(fp,"#   Aufraeumen\n");
	fprintf(fp,"#   ----------\n\n");
	fprintf(fp,"clean:\n");
	fprintf(fp,"\t@-del *.c\n");
	fprintf(fp,"\t@-del *.h\n");
	fprintf(fp,"\t@-del *.x\n");
	fprintf(fp,"\t@-del *.bak\n");
	fprintf(fp,"\t@-del *.map\n");
	fprintf(fp,"\t@-del *.pdb\n");
	fprintf(fp,"\t@-del *.ilk\n");
	fprintf(fp,"\t@-del *$(LIB)\n");
	fprintf(fp,"\t@-del *$(DLL)\n");
	fprintf(fp,"\t@-del *$(OBJ)\n");
	fprintf(fp,"\t@-del *$(RES)\n");
	fprintf(fp,"\t@-del respfile\n\n");

#endif
#ifdef OV_SYSTEM_LINUX
	//
	//	Makefile LINUX
	//
	fprintf(fp,"#	Compiler\n");
	fprintf(fp,"#	--------\n\n");
	fprintf(fp,"OV_CODEGEN_EXE = $(ACPLT_BIN_DIR)ov_codegen$(EXE)\n\n");
	fprintf(fp,"CC             = gcc\n");
	fprintf(fp,"CC_FLAGS       = -g -Wall -O2 -shared\n");
	fprintf(fp,"COMPILE_C	   = $(CC) $(CC_FLAGS) $(DEFINES) $(INCLUDES) -c\n");
	fprintf(fp,"LD             = $(CC) -shared\n");
        fprintf(fp,"AR             = ar\n");
        fprintf(fp,"RANLIB         = ranlib\n\n");
	fprintf(fp,"#   Implicit Rules\n");
	fprintf(fp,"#   --------------\n\n");
	fprintf(fp,".c$(OBJ):\n");
	fprintf(fp,"\t$(COMPILE_C) -o$@ $<\n\n");
	fprintf(fp,".ovm.c:\n");
	fprintf(fp,"\t$(OV_CODEGEN_EXE) -I $(ACPLT_OV_MODEL_DIR) ");
	k = 0;
	while (k < j) {
		fprintf(fp,"-I $(BASELIB%d_MODEL_DIR) ",k);
		k++;
	}
	fprintf(fp,"-f $< -l $(notdir $(basename $<))\n\n");
	fprintf(fp,".ovm.h:\n");
	fprintf(fp,"\t$(OV_CODEGEN_EXE) -I $(ACPLT_OV_MODEL_DIR) ");
	k = 0;
	while (k < j) {
		fprintf(fp,"-I $(BASELIB%d_MODEL_DIR) ",k);
		k++;
	}
	fprintf(fp,"-f $< -l $(notdir $(basename $<))\n\n");
	fprintf(fp,"#   Dependencies\n");
	fprintf(fp,"#   ------------\n\n");
	fprintf(fp,".depend:\n");
	fprintf(fp,"\ttouch $@\n\n");
	fprintf(fp,"depend : $(SOURCES)\n");
	fprintf(fp,"\t$(COMPILE_C) -MM $^ > .depend\n\n");
	fprintf(fp,"#   Libraries\n");
	fprintf(fp,"#   ---------\n\n");
	k = 0;
	while (k < j) {
		fprintf(fp,"%s.h : $(BASELIB%d_MODEL_DIR)%s.ovm\n",libs[k],k,libs[k]);
		fprintf(fp,"\t$(OV_CODEGEN_EXE) -I $(ACPLT_OV_MODEL_DIR) ");
		l = k + 1;
		while (l < j) {
			fprintf(fp,"-I $(BASELIB%d_MODEL_DIR) ",l);
			l++;
		}
		fprintf(fp,"-f $<\n\n");
		k++;
	}
	fprintf(fp,"ov.h : $(ACPLT_OV_MODEL_DIR)ov.ovm\n");
	fprintf(fp,"\t$(OV_CODEGEN_EXE) -f $<\n\n");
        fprintf(fp,"$(MYLIB_LIB) : $(MYLIB_OBJ)\n");
	fprintf(fp,"\t$(AR) rv $@ $?\n");
	fprintf(fp,"\t$(RANLIB) $@\n\n");
	fprintf(fp,"$(MYLIB_DLL) : $(MYLIB_OBJ)\n");
	fprintf(fp,"\t$(LD) -o $@ $^\n");
	fprintf(fp,"\t-cp $(MYLIB_DLL) $(ACPLT_BIN_DIR)\n");
        k = 0;
	while (k < n) {
  	      fprintf(fp,"\t-cp $(MYLIB_DLL) $(COPY_DIR%d)\n",k);
  	      k++;
	}
	fprintf(fp,"\n");
	fprintf(fp,"#   Aufraeumen\n");
	fprintf(fp,"#   ----------\n\n");
	fprintf(fp,"clean:\n");
	fprintf(fp,"\t@-rm -f *.c *.h *.o *.so\n\n");
	fprintf(fp,"#    Include dependencies\n");
	fprintf(fp,"#    --------------------\n\n");
	fprintf(fp,"include .depend\n");

#endif
	if(fclose(fp)) {
		fprintf(stderr, "error closing file.\n");
		return -1;
	}

	return 0;
}

