/*******************************************************************************
 *                                                                             *
 *   Datei                                                                     *
 *   -----                                                                     *
 *   acplt_makmak.c                                                            *
 *                                                                             *
 *                                                                             *
 *   Historie                                                                  *
 *   --------                                                                  *
 *   2007-04-08 Alexander Neugebauer: Erstellung, LTSoft GmbH, Kerpen          *
 *   2011-06-21 Sten Gruener: Anpassung an MinGW, bugfixes, ACPLT	       *
 *   2013-04-24 Sten Gruener: Adoption to the new directory strucure           *
 *                            deep refactoring                                 *
 *                                                                             *
 *   Beschreibung                                                              *
 *   ------------                                                              *
 *   Generator der Makefiles fuer Benutzer-Bibliothek                          *
 *                                                                             *
 ******************************************************************************/

/*
 *	Definitionen
 *	------------
 */

#include "definitions.h"

/*
 *	Include-Dateien
 *	---------------
 */
#include <stdio.h> /* defines FILENAME_MAX */
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*
 *	Common Functions
 *	------------
 */

#include "common.h"


#define TARGETOS_WIN	1
#define TARGETOS_LINUX	2
#define TARGETOS_SOLARIS	3

/*	----------------------------------------------------------------------	*/
/*
 *	Bibliothek-Name in Grossbuchstaben
 */
char* getUpperLibName(char* str) {
	static char hStr[255];
	char *ph;

	if(!str) {
		return "";
	}

	strcpy(hStr, str);
	ph = hStr;
	while( ph && (*ph)) {
		if( ((*ph) >= 'a') && ((*ph) <= 'z') ) {
			*ph += ('A' - 'a');
		}
		ph++;
	}

	return hStr;
}

/*	----------------------------------------------------------------------	*/
/*
 *	Main program
 */
int main(int argc, char **argv) {
	/*
	 *	exGlobal variables
	 *	-----------------
	 */
	char        *devLibs[MAX_INCLUDED_FILES + 1];
	int         numDevLibs = 0;
	char        *sysLibs[MAX_INCLUDED_FILES + 1];
	int         numSysLibs = 0;
	char        *libname=NULL;

	/*
	 *	local variables
	 */
	FILE        *fd;
	char        help[512];
	char        libPath[512] = "";
	char        devModelPath[512] = "";
	char        devBinPath[512] = "";
	char        sysModelPath[512] = "";
	char        sysBinPath[512] = "";
	int			new = 0;
	int 	    i;
	int 	    addOpenLib = 0;
	int	    	force = 0;
	int			targetOs = 0;

	//default to the own bitwidth
#ifdef __x86_64
	int	    	archBitwidth = 64;
#else
	int	    	archBitwidth = 32;
#endif
	
	char        *builddir = NULL;
	char        *buildsys = NULL;

	//this is for the host system
#if OV_SYSTEM_NT
	targetOs	=	TARGETOS_WIN;
#else
#if OV_SYSTEM_LINUX
	targetOs	=	TARGETOS_LINUX;
#else
#if OV_SYSTEM_SOLARIS
	targetOs	=	TARGETOS_SOLARIS;
#else
#error --- Betriebssystem nicht unterstuetzt --- 
#endif  /* SOLARIS */
#endif  /* LINUX   */
#endif  /* NT      */

	/*
	 *	parse command line arguments
	 */
	for(i=1; i<argc; i++) {
		/*
		 *	set database filename option
		 */
		if( !strcmp(argv[i], "-l") ) {
			i++;
			if(i<argc) {
				libname = argv[i];
			} else {
				goto HELP;
			}
		}
		/*
		 *	'open lib' option
		 */
		else if( !strcmp(argv[i], "-o") || !strcmp(argv[i], "--userdefined-open") ) {
			addOpenLib = 1;
		}
		/*
		 *	'force' option
		 */
		else if( !strcmp(argv[i], "-f") || !strcmp(argv[i], "--force") ) {
			force = 1;
		}
		/*
		 *	'-m32' option
		 */
		else if( !strcmp(argv[i], "-m32")) {
			archBitwidth = 32;
		}
		/*
		 *	'-m32' option
		 */
		else if( !strcmp(argv[i], "-m64")) {
			archBitwidth = 64;
		}
		/*
		 *	'-mARM' option
		 */
		else if( !strcmp(argv[i], "-mARM")) {
			archBitwidth = 0;
		}
		/*
		 *	'targetLinux' option
		 */
		else if( !strcmp(argv[i], "--targetLinux")) {
			targetOs = TARGETOS_LINUX;
		}
		/*
		 *	'targetLinux' option
		 */
		else if( !strcmp(argv[i], "--targetWindows")) {
			targetOs = TARGETOS_WIN;
		}
		/*
		 *	'targetLinux' option
		 */
		else if( !strcmp(argv[i], "--targetSolaris")) {
			targetOs = TARGETOS_SOLARIS;
		}

		/*
		 *	display help option
		 */
		else if( !strcmp(argv[i], "-h") || !strcmp(argv[i], "--help") ) {
			HELP:
			fprintf(stderr, "Makefile generator: creates generic.mk and Makefile for compiling a user library.\n");
			fprintf(stderr, "Each library directory contains the 'build', 'source', 'model' and (optional) 'include' directory.\n");
			fprintf(stderr, "_ LIBNAME\n");
			fprintf(stderr, "  |_ build\n");
			fprintf(stderr, "  |  |_ %s\n", builddir);
			fprintf(stderr, "  |_ include\n");
			fprintf(stderr, "  |_ model\n");
			fprintf(stderr, "  |_ source\n");
			fprintf(stderr, "\n");
			fprintf(stderr, "Usage: acplt_makmak [arguments]\n"
					"-l   LIBNAME                Set name of the library whose makefile is to be generated\n"
					"The following optional arguments are available:\n"
					"-o   OR --userdefined-open  Add user defined 'openlib' option\n"
					"-h   OR --help              Display this help message and exit\n"
					"-f   OR --force             Force overwriting Makefile (project settings), note that .tcl scripts will never be overwritten\n"
					"-m32 OR -m64 OR -mARM       Force bit width of the library (default is the same as this executable: ");
#ifdef __x86_64
			fprintf(stderr, "64");
#else
			fprintf(stderr, "32");
#endif
			fprintf(stderr, " detected)\n"
					"							Use -mARM for ARM-Architecture Targets. (There -m32 or -m64 is not Accepted)\n"
					"--targetLinux OR --targetWindows OR --targetSolaris	Set a target OS for a Cross Makefile.\n"
					"\n");
			return 1;
		} else {
			goto HELP;
		}
	}

	if(targetOs == TARGETOS_WIN){
		builddir = "nt";
		buildsys = "NT";
	} else if(targetOs == TARGETOS_LINUX){
		builddir = "linux";
		buildsys = "LINUX";
	} else if(targetOs == TARGETOS_SOLARIS){
		builddir = "solaris";
		buildsys = "SOLARIS";
	}
	
	if(!libname) {
		goto HELP;
	}

	//locate library
	if(1 == locateLibrary(libname, libPath, devModelPath, devBinPath, sysModelPath, sysBinPath, &new)){
		return 1;
	}

	/*
	 *	one more sanity check
	 */
	sprintf(help, "%s/source/%s.c", libPath, libname);
	compatiblePath(help);
	if(1 == fileExists(help)){
		fprintf(stderr, "Error: %s.c in the /source/ dir will collide with generated file, please rename.\n", libname);
		return 1;
	}

	//library found for sure - create some more infrastructure

	/* Check if build dir is present */
	sprintf(help, "%s/build", libPath);
	compatiblePath(help);
	if(stat(help, &st) != 0){
			//fprintf(stdout,"Creating directory '%s'... \n", help);
			acplt_mkdir(help);
	}

	/* Check if platform dir is present */
	sprintf(help, "%s/build/%s", libPath, builddir);
	compatiblePath(help);
	if(stat(help, &st) != 0){
		acplt_mkdir(help);
	}

	/*
	 *   resolve dependencies
	 */
	makmak_searchbaselibs(libname, devModelPath, sysModelPath, devLibs, &numDevLibs, sysLibs, &numSysLibs);

	fprintf(stderr, "All model inclusions resolved:\n");
	for(i=0; i<numDevLibs; i++) {
		fprintf(stderr,"%s in %s\n"  , devLibs[i], devModelPath);
	}
	for(i=0; i<numSysLibs; i++) {
		fprintf(stderr,"%s in %s\n"  , sysLibs[i], sysModelPath);
	}

	/* ---------------------------- Generating files -------------------------  */
	/* generic.mk */
	sprintf(help, "%s/build/generic.mk", libPath);
	compatiblePath(help);

	//fprintf(stdout, "Creating file '%s' ...\n", help);

	fd = fopen(help, "w");
	if(!fd) {
		fprintf(stderr, "Error: Unable to open file '%s' for writing.\n", help);
		return 1;
	}

	fprintf(fd, "#******************************************************************************\n");
	fprintf(fd, "#*\n");
	fprintf(fd, "#*   FILE\n");
	fprintf(fd, "#*   ----\n");
	fprintf(fd, "#*   generic.mk - Makefile for user libs (generic part)\n");
	fprintf(fd, "#******************************************************************************\n");
	fprintf(fd, "#*\n");
	fprintf(fd, "#*   This file is generated by the 'acplt_makmak' command\n");
	fprintf(fd, "#*\n");
	fprintf(fd, "#******************************************************************************\n");

	fprintf(fd,"\n");
	fprintf(fd,"#	Directories\n");
	fprintf(fd,"#	-----------\n\n");

	fprintf(fd,"MODEL_DIR         = ../../model/\n");
	fprintf(fd,"SOURCE_DIR        = ../../source/\n");
	fprintf(fd,"INCLUDE_DIR       = ../../include/\n");

if(new == 0){
	/** older structure **/
	//server/user - dir
	fprintf(fd,"USER_DIR          = ../../../\n");
	fprintf(fd,"USERLIB_DIR       = $(USER_DIR)libs\n");
	//server - dir
	fprintf(fd,"ROOT_DIR          = ../../../../\n");
	fprintf(fd,"BASE_DIR          = $(ROOT_DIR)\n");
	fprintf(fd,"BASE_INC_DIR      = $(BASE_DIR)include/\n");
	fprintf(fd,"BASE_LIB_DIR      = $(BASE_DIR)lib/\n");
	fprintf(fd,"BASE_MODEL_DIR    = $(BASE_DIR)model/\n");
	fprintf(fd,"BIN_DIR           = $(ROOT_DIR)bin/\n");
}else{
	/** newer structure **/
	//server - dir
	fprintf(fd,"ROOT_DIR          = ../../../../\n");
	fprintf(fd,"BASE_DIR          = $(ROOT_DIR)\n");
	fprintf(fd,"BASE_INC_DIR      = $(BASE_DIR)system/sysdevbase/ov/include/\n");
	fprintf(fd,"BASE_LIB_DIR      = $(BASE_DIR)system/sysdevbase/ov/lib/\n");
	fprintf(fd,"BASE_MODEL_DIR    = $(BASE_DIR)system/sysdevbase/ov/model/\n");
	fprintf(fd,"BIN_DIR           = $(ROOT_DIR)system/sysbin/\n");

	//server/user - dir
	fprintf(fd,"USER_DIR          = ../../../\n");
	fprintf(fd,"USERLIB_DIR       = $(ROOT_DIR)system/addonlibs/\n");
	fprintf(fd,"SYS_DIR           = $(BASE_DIR)system/sysdevbase/\n");
	fprintf(fd,"SYSLIB_DIR        = $(ROOT_DIR)system/syslibs/\n");
}

	/* Basis-Bibliotheken? */
	for(i=0; i<numDevLibs; i++) {
		fprintf(fd,"%s_MODEL_DIR           = $(USER_DIR)%s/model/\n"  , getUpperLibName(devLibs[i]), devLibs[i]);
		fprintf(fd,"%s_INCLUDE_DIR         = $(USER_DIR)%s/include/\n", getUpperLibName(devLibs[i]), devLibs[i]);
	}
	/* System-Bibliotheken? */
	for(i=0; i<numSysLibs; i++) {
		fprintf(fd,"%s_MODEL_DIR           = $(SYS_DIR)%s/model/\n"  , getUpperLibName(sysLibs[i]), sysLibs[i]);
		fprintf(fd,"%s_INCLUDE_DIR         = $(SYS_DIR)%s/include/\n", getUpperLibName(sysLibs[i]), sysLibs[i]);
	}
	fprintf(fd,"#   Rules\n");
	fprintf(fd,"#   -----\n\n");

	fprintf(fd,".SUFFIXES:\n");
	fprintf(fd,".SUFFIXES: $(_C) .h .ovm $(_LIB) $(_DLL) $(_OBJ)\n\n");

	fprintf(fd,"#	Paths and defines\n");
	fprintf(fd,"#	-----------------\n\n");

	fprintf(fd,"ifeq ($(COMPILER), MSVC)\n");

	fprintf(fd,"INCLUDES  = /I$(BASE_INC_DIR) /I$(BASE_MODEL_DIR) \\\n");
	for(i=0; i<numDevLibs; i++) {
		fprintf(fd,"\t\t\t/I$(%s_MODEL_DIR) /I$(%s_INCLUDE_DIR) \\\n",
				getUpperLibName(devLibs[i]), getUpperLibName(devLibs[i]));
	}
	for(i=0; i<numSysLibs; i++) {
		fprintf(fd,"\t\t\t/I$(%s_MODEL_DIR) /I$(%s_INCLUDE_DIR) \\\n",
				getUpperLibName(sysLibs[i]), getUpperLibName(sysLibs[i]));
	}
	fprintf(fd,"\t\t\t/I$(MODEL_DIR) /I$(INCLUDE_DIR)\n");

	fprintf(fd,"else\n");

	fprintf(fd,"INCLUDES  = -I$(BASE_INC_DIR) -I$(BASE_MODEL_DIR)\\\n");
	for(i=0; i<numDevLibs; i++) {
		fprintf(fd,"\t\t\t-I$(%s_MODEL_DIR) -I$(%s_INCLUDE_DIR) \\\n",
				getUpperLibName(devLibs[i]), getUpperLibName(devLibs[i]) );
	}
	for(i=0; i<numSysLibs; i++) {
		fprintf(fd,"\t\t\t-I$(%s_MODEL_DIR) -I$(%s_INCLUDE_DIR) \\\n",
				getUpperLibName(sysLibs[i]), getUpperLibName(sysLibs[i]) );
	}
	fprintf(fd,"\t\t\t-I$(MODEL_DIR) -I$(INCLUDE_DIR)\n");

	fprintf(fd,"endif\n\n");

	fprintf(fd,"VPATH     = $(MODEL_DIR) $(SOURCE_DIR) $(INCLUDE_DIR) \\\n");
	for(i=0; i<numDevLibs; i++) {
		fprintf(fd,"\t\t\t$(%s_MODEL_DIR) $(%s_INCLUDE_DIR) \\\n",
				getUpperLibName(devLibs[i]), getUpperLibName(devLibs[i]) );
	}
	for(i=0; i<numSysLibs; i++) {
		fprintf(fd,"\t\t\t$(%s_MODEL_DIR) $(%s_INCLUDE_DIR) \\\n",
				getUpperLibName(sysLibs[i]), getUpperLibName(sysLibs[i]) );
	}
	fprintf(fd,"\t\t\t$(BASE_INC_DIR) $(BASE_MODEL_DIR)\n\n");

	fprintf(fd,"ifeq ($(COMPILER), MSVC)\n");
	fprintf(fd,"DEFINES    = /DOV_SYSTEM_$(SYSTEM)=1 /DPLT_SYSTEM_$(SYSTEM)=1");
	if(addOpenLib == 1) {
		fprintf(fd," /Dov_library_open_%s=ov_library_open_%s_old", libname, libname);
	}
	fprintf(fd,"\n");
	fprintf(fd,"else\n");
	fprintf(fd,"DEFINES    = -DOV_SYSTEM_$(SYSTEM)=1 -DPLT_SYSTEM_$(SYSTEM)=1");
	
	if(addOpenLib == 1) {
		fprintf(fd," -Dov_library_open_%s=ov_library_open_%s_old", libname, libname);
	}
	fprintf(fd,"\n");
	fprintf(fd,"endif\n\n");

	fprintf(fd,"#	User library\n");
	fprintf(fd,"#	------------\n\n");

	fprintf(fd,"LIBRARY        = %s\n", libname);
	fprintf(fd,"USERLIB_C      = $(LIBRARY)$(_C)\n");
	fprintf(fd,"USERLIB_LIB    = $(LIBRARY)$(_LIB)\n");
	fprintf(fd,"USERLIB_DLL    = $(LIBRARY)$(_DLL)\n\n");

	fprintf(fd,"#	Targets\n");
	fprintf(fd,"#	-------\n");

	fprintf(fd,"USERLIB_SRC = $(USERLIB_C) $(wildcard $(SOURCE_DIR)*$(_C))\n");
	fprintf(fd,"USERLIB_OBJ = $(foreach source, $(USERLIB_SRC), $(basename $(notdir $(source)))$(_OBJ))\n\n");

	fprintf(fd,"HEADERS = \\\n");
	fprintf(fd,"\tov.h \\\n");

	for(i=0; i<numDevLibs; i++) {
		fprintf(fd,"\t%s.h \\\n", devLibs[i]);
	}
	for(i=0; i<numSysLibs; i++) {
		fprintf(fd,"\t%s.h \\\n", sysLibs[i]);
	}
	fprintf(fd,"\t%s.h \n\n", libname);

	fprintf(fd,"ifndef STATIC_ONLY\n");
	fprintf(fd,"TARGETS = \\\n");
	fprintf(fd,"\t$(HEADERS) \\\n");
	fprintf(fd,"\t$(USERLIB_LIB) \\\n");
	fprintf(fd,"\t$(USERLIB_DLL)\n");
	fprintf(fd,"else\n");
	fprintf(fd,"TARGETS = \\\n");
	fprintf(fd,"\t$(HEADERS) \\\n");
	fprintf(fd,"\t$(USERLIB_LIB)\n");
	fprintf(fd,"endif\n\n");

	/* Added by Sten on 14.11.11 */

	/* Added by Sten on 21.06.11 */
	fprintf(fd,"SOURCES = \\\n");
	fprintf(fd,"\tov.h \\\n");
	for(i=0; i<numDevLibs; i++) {
		fprintf(fd,"\t%s.h \\\n", devLibs[i]);
	}
	for(i=0; i<numSysLibs; i++) {
		fprintf(fd,"\t%s.h \\\n", sysLibs[i]);
	}
	fprintf(fd,"\t$(USERLIB_SRC)\n\n");
	/* End add */

	fclose(fd);

	/* Generic postbuild script */
	/* postbuild.tcl */
	sprintf(help, "%s/build/postbuild.tcl", libPath);
	compatiblePath(help);

	if(fileExists(help)){
		//fprintf(stdout, "File '%s' exists already\n", help);
	}else{
		//fprintf(stdout, "Creating file '%s' ...\n", help);
		fd = fopen(help, "w");
		fclose(fd);
	}

	/* Generic prebuild script */
	/* prebuild.tcl */
	sprintf(help, "%s/build/prebuild.tcl", libPath);
	compatiblePath(help);

	if(fileExists(help)){
		//fprintf(stdout, "File '%s' exists already\n", help);
	}else{
		//fprintf(stdout, "Creating file '%s' ...\n", help);
		fd = fopen(help, "w");
		if(!fd) {
			fprintf(stderr, "unable to open file '%s' for writing.\n", help);
			return 1;
		}

		fclose(fd);
	}


	/* Meta makefile containing the configuration and the call of pre and postbuild scripts */
	/* Makefile */
	sprintf(help, "%s/build/%s/Makefile", libPath, builddir);
	compatiblePath(help);

	if(fileExists(help) && force == 0){
		//fprintf(stdout, "File '%s' exists already\n", help);
	}else{
		//fprintf(stdout, "Creating file '%s' ...\n", help);
		fd = fopen(help, "w");
		if(!fd) {
			fprintf(stderr, "unable to open file '%s' for writing.\n", help);
			return 1;
		}
		fprintf(fd, "#******************************************************************************#\n");
		fprintf(fd, "#* A lightweight Makefile to invoke the prebuild mechanism\n");
		fprintf(fd, "#******************************************************************************#\n");
		fprintf(fd,"\n");

		fprintf(fd,"#Lower case makro\n");
		fprintf(fd,"lc = $(subst A,a,$(subst B,b,$(subst C,c,$(subst D,d,$(subst E,e,$(subst F,f,$(subst G,g,$(subst H,h,$(subst I,i,$(subst J,j,$(subst K,k,$(subst L,l,$(subst M,m,$(subst N,n,$(subst O,o,$(subst P,p,$(subst Q,q,$(subst R,r,$(subst S,s,$(subst T,t,$(subst U,u,$(subst V,v,$(subst W,w,$(subst X,x,$(subst Y,y,$(subst Z,z,$1))))))))))))))))))))))))))\n\n");

		fprintf(fd,"#Project config parameters that are accessed by Eclipse\n");
		fprintf(fd,"SERVERSTARTKEYS = \n");
		fprintf(fd,"#/Project config parameters that are accessed by Eclipse\n");
		fprintf(fd,"MAKMAKOPTIONS = ");
		if(addOpenLib == 1){
			fprintf(fd,"-o");
		}
		fprintf(fd,"\n");
		fprintf(fd,"EXTRA_CC_FLAGS = \n");
		fprintf(fd,"EXTRA_LIBS = \n");
		fprintf(fd,"LD_FLAGS = \n");
		fprintf(fd,"COMPILER = GCC\n");
		fprintf(fd,"PREBUILD = ../prebuild.tcl\n");
		fprintf(fd,"POSTBUILD = ../postbuild.tcl\n");
		fprintf(fd,"COMPILER_LO = $(call lc,$(COMPILER))\n\n");
		fprintf(fd,"# Set TARGETOS to --targetWindows --targetLinux or --targetSolaris to cross-compile for the respective system.\n");
		fprintf(fd,"TARGETOS = \n\n");

		fprintf(fd, "all:\n\n");

		fprintf(fd, "%%:\n");
		fprintf(fd, "\tacplt_makmak -l %s $(MAKMAKOPTIONS) $(TARGETOS)\n", libname);
		sprintf(help, "\ttclsh $(PREBUILD) %s $(MAKMAKOPTIONS)\n", libname);
		compatiblePath(help);
		fprintf(fd, "%s", help);
		fprintf(fd, "\t$(MAKE) MAKMAKOPTIONS='$(MAKMAKOPTIONS)' EXTRA_CC_FLAGS='$(EXTRA_CC_FLAGS)' EXTRA_LIBS='$(EXTRA_LIBS)' LD_FLAGS='$(LD_FLAGS)' TARGET='$*' -f $(call lc,$(COMPILER)).mk $*\n");
		sprintf(help, "\ttclsh $(POSTBUILD) %s\n", libname);
		compatiblePath(help);
		fprintf(fd, "%s", help);
	}


	/* Makefile for GCC and MinGW */
	/* gcc.mk */
	sprintf(help, "%s/build/%s/gcc.mk", libPath, builddir);
	compatiblePath(help);

	//fprintf(stdout, "Creating file '%s' ...\n", help);

	fd = fopen(help, "w");
	if(!fd) {
		fprintf(stderr, "unable to open file '%s' for writing.\n", help);
		return 1;
	}

	fprintf(fd, "#******************************************************************************\n");
	fprintf(fd, "#*\n");
	fprintf(fd, "#*   FILE\n");
	fprintf(fd, "#*   ----\n");
	fprintf(fd, "#*   Makefile - Makefile fuer die Anwender-Bibliothek\n");
	fprintf(fd, "#******************************************************************************\n");
	fprintf(fd, "#*\n");
	fprintf(fd, "#*   This file is generated by the 'acplt_makmak' command\n");
	fprintf(fd, "#*\n");
	fprintf(fd, "#******************************************************************************\n");

	fprintf(fd,"\n");
	fprintf(fd,"#   Plattform\n");
	fprintf(fd,"#   ---------\n");
	fprintf(fd,"SYSTEM = %s\n", buildsys);
	fprintf(fd,"SYSDIR = %s\n\n", builddir);

	fprintf(fd,"COMPILER = GNU\n\n");

	fprintf(fd,"#	Filename conventions\n");
	fprintf(fd,"#	--------------------\n\n");

	fprintf(fd,"_C   = .c\n");
	fprintf(fd,"_OBJ = .o\n");
	fprintf(fd,"_LIB = .a\n");
	//	Executables are codegen and makmak --> host system
#if OV_SYSTEM_NT
	fprintf(fd,"_EXE = .exe\n\n");
#else
	fprintf(fd,"_EXE =\n\n");
#endif
	// Shared Libs are for the target
	if(targetOs == TARGETOS_WIN){
		fprintf(fd,"_DLL = .dll\n");
	} else {
		fprintf(fd,"_DLL = .so\n");
	}
	


	fprintf(fd,"#	Include generic part\n");
	fprintf(fd,"#	--------------------\n");

	fprintf(fd,"include ../generic.mk\n\n");

	fprintf(fd,"# Libraries\n");
	fprintf(fd,"# ---------\n\n");
	if(targetOs == TARGETOS_WIN){
		fprintf(fd,"OVLIBS = $(BASE_LIB_DIR)libov$(_LIB)\n");
		if(numDevLibs+numSysLibs > 0) {
			fprintf(fd,"ADD_LIBS =");
			for(i=0; i<numDevLibs; i++) {
				/* link directly using dll */
				fprintf(fd," $(USERLIB_DIR)/%s$(_DLL)", devLibs[i]);
			}
			for(i=0; i<numSysLibs; i++) {
				/* link directly using dll */
				fprintf(fd," $(SYSLIB_DIR)/%s$(_DLL)", sysLibs[i]);
			}
			fprintf(fd,"\n");
		}
	} else {
		fprintf(fd,"# Swithces for additional libraries needed for dynamic linkage in Linux\n");
		if(numDevLibs+numDevLibs > 0) {
			fprintf(fd,"ADD_LIBS_SWITCHES =");
			for(i=0; i<numDevLibs; i++) {
				/* fprintf(fd," $(USER_DIR)%s/build/%s/%s$(_LIB)", libs[i], builddir, libs[i]); */
				/* link against .a */
				fprintf(fd," %s$(_DLL)", devLibs[i]);
			}
			for(i=0; i<numSysLibs; i++) {
				/* fprintf(fd," $(USER_DIR)%s/build/%s/%s$(_LIB)", libs[i], builddir, libs[i]); */
				/* link against .a */
				fprintf(fd," %s$(_DLL)", sysLibs[i]);
			}
			fprintf(fd,"\n");
		}
	}
	fprintf(fd,"ADD_LIBS += $(foreach lib, $(EXTRA_LIBS),$(lib))\n\n");

	/* Compiler-Optionen */
	fprintf(fd,"#	Compiler\n");
	fprintf(fd,"#	--------\n");
	fprintf(fd,"OV_CODEGEN_EXE = $(BIN_DIR)ov_codegen$(_EXE)\n\n");
	fprintf(fd,"ifneq \"$(origin GCC_BIN_PREFIX)\" \"undefined\"\n");
	fprintf(fd,"CC = $(GCC_BIN_PREFIX)gcc\n");
	fprintf(fd,"else\n");
	fprintf(fd,"GCC_BIN_PREFIX		= \n");
	fprintf(fd,"endif\n\n");
	fprintf(fd,"CC		?= $(GCC_BIN_PREFIX)gcc\n");
	fprintf(fd,"ifneq ($(TARGET), debug)\n");
	fprintf(fd,"\tOPT = -O2 -fno-strict-aliasing\n");
	fprintf(fd,"endif\n");
	fprintf(fd,"CC_FLAGS	= -g -std=c99");
	// remove the following #if to enable cross compiling on an ARM host system
#if !__arm__
	if(archBitwidth == 32){
		fprintf(fd," -m32");
	}else if(archBitwidth == 64){
		fprintf(fd," -m64");
	}
#endif
	if(targetOs != TARGETOS_WIN){
		fprintf(fd," -fPIC");	// all code is position independent on windows
	}
	fprintf(fd," -Wdeclaration-after-statement -Wall -Wno-attributes $(OPT) $(EXTRA_CC_FLAGS)\n");

#if OV_SYSTEM_LINUX //assume that we compile on a linux host
	if(targetOs != TARGETOS_WIN){ 
		//gcc feature detection
		fprintf(fd,"GCCVERSIONGTEQ4 := $(shell expr `$(CC) -dumpversion | cut -f1 -d.` \\>= 4)\n");
		fprintf(fd,"ifeq \"$(GCCVERSIONGTEQ4)\" \"1\"\n");
    	fprintf(fd,"    CC_FLAGS += -fvisibility=hidden\n");
		fprintf(fd,"endif\n");
	}
#endif

	if(targetOs == TARGETOS_WIN){
		fprintf(fd,"CC_DEFINES	= $(DEFINES) -D__NT__=1 \n");
	} else {
		fprintf(fd,"CC_DEFINES	= $(DEFINES)\n");
	}

	fprintf(fd,"CC_INCLUDES	= $(INCLUDES) -I.\n");
	fprintf(fd,"COMPILE_C	= $(CC) $(CC_FLAGS) $(CC_DEFINES) $(CC_INCLUDES) -c\n");

	if(targetOs == TARGETOS_WIN){
		fprintf(fd,"LD		= $(CC) -shared");
		if(archBitwidth == 32){
			fprintf(fd," -m32");
		}else if(archBitwidth == 64){
			fprintf(fd," -m64");
		}
		fprintf(fd," -Wl,--output-def,%s.def,--out-implib,%s.a\n", libname, libname);
	} else {
		fprintf(fd,"LD		= $(CC) -shared");
		if(archBitwidth == 32){
			fprintf(fd," -m32");
		}else if(archBitwidth == 64){
			fprintf(fd," -m64");
		}
		fprintf(fd,"\n");
	}

	fprintf(fd,"AR			= $(GCC_BIN_PREFIX)ar\n");
	fprintf(fd,"RANLIB		= $(GCC_BIN_PREFIX)ranlib\n");
	fprintf(fd,"STRIP		= $(GCC_BIN_PREFIX)strip\n");

	fprintf(fd,"\n");

	/* Targets */
	fprintf(fd,"templates: \n");
		// this is for the host system
#if OV_SYSTEM_NT
	fprintf(fd,"\t-@cmd /c del ..\\..\\source\\sourcetemplates\\\\*$(_C)\n");
#else
	fprintf(fd,"\t-@rm ../../source/sourcetemplates/*$(_C)\n");
#endif
	fprintf(fd,"\tacplt_builder -l $(LIBRARY) $(MAKMAKOPTIONS)\n");
	//todo: refactor: join with with [1]
#if OV_SYSTEM_LINUX
	fprintf(fd,"\tsed -i -e 's/\r//' $(MODEL_DIR)$(LIBRARY).ovm\n");
#endif
	fprintf(fd,"\t$(OV_CODEGEN_EXE) -I $(BASE_MODEL_DIR)");
	for(i=0; i<numDevLibs; i++) {
		fprintf(fd," -I $(%s_MODEL_DIR)", getUpperLibName(devLibs[i]));
	}
	for(i=0; i<numSysLibs; i++) {
		fprintf(fd," -I $(%s_MODEL_DIR)", getUpperLibName(sysLibs[i]));
	}
	fprintf(fd," -f $(MODEL_DIR)$(LIBRARY).ovm -l $(LIBRARY)\n\n");
	fprintf(fd,"\t-@echo  ==== New templates have been created! ====\n");

	fprintf(fd,"\n");

	fprintf(fd,"all: $(TARGETS)\n");
	fprintf(fd,"ifndef STATIC_ONLY\n");
	fprintf(fd,"\t$(STRIP) --strip-debug $(USERLIB_LIB)\n");
	fprintf(fd,"\t$(STRIP) --strip-debug $(USERLIB_DLL)\n");
	// host system either
#if OV_SYSTEM_NT
	fprintf(fd,"\tcmd /c copy $(USERLIB_DLL) $(subst /,\\\\, $(USERLIB_DIR))\n");
	fprintf(fd,"endif\n\n");
#else
	fprintf(fd,"\tcp $(USERLIB_DLL) $(USERLIB_DIR)\n");
	fprintf(fd,"endif\n\n");
#endif
	fprintf(fd,"\n");

	fprintf(fd,"debug: $(TARGETS)\n");
	fprintf(fd,"ifndef STATIC_ONLY\n");
	// host system either
#if OV_SYSTEM_NT
	fprintf(fd,"\tcmd /c copy $(USERLIB_DLL) $(subst /,\\\\, $(USERLIB_DIR))\n");
	fprintf(fd,"endif\n\n");
#else
	fprintf(fd,"\tcp $(USERLIB_DLL) $(USERLIB_DIR)\n");
	fprintf(fd,"endif\n\n");
#endif
	fprintf(fd,"\n");


	fprintf(fd,"#   Rules\n");
	fprintf(fd,"#   -----\n");
	fprintf(fd,"$(LIBRARY).c $(LIBRARY).h: $(wildcard $(MODEL_DIR)$(LIBRARY).ov?) Makefile\n");
	//[1]
	// host system either
#if OV_SYSTEM_LINUX
	fprintf(fd,"\tsed -i -e 's/\r//' $(MODEL_DIR)$(LIBRARY).ovm\n");
#endif
	fprintf(fd,"\t$(OV_CODEGEN_EXE) -I $(BASE_MODEL_DIR)");
	for(i=0; i<numDevLibs; i++) {
		fprintf(fd," -I $(%s_MODEL_DIR)", getUpperLibName(devLibs[i]));
	}
	for(i=0; i<numSysLibs; i++) {
		fprintf(fd," -I $(%s_MODEL_DIR)", getUpperLibName(sysLibs[i]));
	}
	fprintf(fd," -I $(MODEL_DIR) -f $(MODEL_DIR)$(LIBRARY).ovm -l $(notdir $(basename $<))\n\n");

	fprintf(fd,"%%.c %%.h: %%.ovm Makefile\n");
	// host system either
#if OV_SYSTEM_LINUX
	fprintf(fd,"\tsed -i -e 's/\r//' $<\n");
#endif
	fprintf(fd,"\t$(OV_CODEGEN_EXE) -I $(BASE_MODEL_DIR)");
	for(i=0; i<numDevLibs; i++) {
		fprintf(fd," -I $(%s_MODEL_DIR)", getUpperLibName(devLibs[i]));
	}
	for(i=0; i<numSysLibs; i++) {
		fprintf(fd," -I $(%s_MODEL_DIR)", getUpperLibName(sysLibs[i]));
	}
	fprintf(fd," -I $(MODEL_DIR) -f $< -l $(notdir $(basename $<))\n\n");

	fprintf(fd,"\n");
	fprintf(fd,"%%.o: %%.c\n");
	fprintf(fd,"%%.o: %%.c $(HEADERS)\n");
	fprintf(fd,"\t$(COMPILE_C) -o $@ $<\n");
	fprintf(fd,"\n");
	if(targetOs == TARGETOS_WIN){
		fprintf(fd, "$(USERLIB_LIB) : $(USERLIB_OBJ) $(ADD_LIBS) $(OVLIBS)\n");
	} else {
		fprintf(fd, "$(USERLIB_LIB) : $(USERLIB_OBJ) $(ADD_LIBS)\n");
	}
	//fprintf(fd, "$(USERLIB_LIB) : $(USERLIB_DLL)\n");
	if(targetOs != TARGETOS_WIN){
		fprintf(fd,"\t$(AR) rv $@ $^\n");
		fprintf(fd,"\t$(RANLIB) $@\n");
	}
	fprintf(fd,"\n");
	if(targetOs == TARGETOS_WIN){
		fprintf(fd, "$(USERLIB_DLL) : $(USERLIB_OBJ) $(ADD_LIBS) $(OVLIBS)\n");
		fprintf(fd, "\t$(LD) -o $@ $^ $(LD_FLAGS)\n");
	} else {
		fprintf(fd, "$(USERLIB_DLL) : $(USERLIB_OBJ) $(ADD_LIBS)\n");
		if(numDevLibs+numSysLibs > 0) {
			//ugly method of copying dependencies, but it is the only one to work :(
			for(i=0; i<numDevLibs; i++) {
				fprintf(fd,"\tcp $(USERLIB_DIR)/%s$(_DLL) %s$(_DLL)\n", devLibs[i], devLibs[i]);
			}
			for(i=0; i<numSysLibs; i++) {
				fprintf(fd,"\tcp $(SYSLIB_DIR)/%s$(_DLL) %s$(_DLL)\n", sysLibs[i], sysLibs[i]);
			}
		}
		fprintf(fd, "\t$(LD) -o $@ $^ $(ADD_LIBS_SWITCHES) $(LD_FLAGS)\n");
	}
	fprintf(fd,"\n");

	if(targetOs == TARGETOS_WIN){
		fprintf(fd,"#   Dependencies\n");
		fprintf(fd,"#   ------------\n\n");

		fprintf(fd,".depend:\n");
		fprintf(fd,"\ttouch $@\n\n");

		fprintf(fd,"depend : $(SOURCES)\n");
		fprintf(fd,"\t$(COMPILE_C) -MM $(USERLIB_SRC) > .depend\n\n");
	}

	fprintf(fd,"#	Aufraeumen\n");
	fprintf(fd,"#	----------\n\n");

	fprintf(fd,"clean:\n");
	// host system
#if OV_SYSTEM_NT
	fprintf(fd,"\t-@cmd /c del *$(_C) *.h *$(_LIB) *$(_DLL) *$(_OBJ)\n");
#else
	fprintf(fd,"\t-@rm *$(_C) *.h *$(_LIB) *$(_DLL) *$(_OBJ)\n");
#endif
	fclose(fd);


/* ----------------------- Start of depricated makefiles ----------------------- */
/* deprications removed on 25.04.13 - check repository for the code! */


	return 0;
}

/*
 *	End of file
 */
