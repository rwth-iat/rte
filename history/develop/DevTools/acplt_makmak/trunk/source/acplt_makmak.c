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

#ifdef OV_SYSTEM_NT
    #include <direct.h>
    #define GetCurrentDir _getcwd
#else
    #include <unistd.h>
    #define GetCurrentDir getcwd
 #endif


/*
 *	Globale Variablen
 *	-----------------
 */
char        *libs[MAX_INCLUDED_FILES + 1];
int         anzAddLibs = 0;
char        *libname=NULL;
char 	    cCurrentPath[FILENAME_MAX];

/*
 *	Common Functions
 *	------------
 */

#include "common.h"


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
	 *	local variables
	 */
	FILE        *fd;
	char        *ifbsEnvVar = IFBS_HOME_ENVPATH;
	char        *acpltEnvVar = ACPLT_HOME_ENVPATH;
	char        *penv;
	char        help[512];
	char        userLibPath[512];
	char        libPath[512];
	int 	    i;
	int 	    addOpenLib = 0;
	int	    force = 0;
	int	    acplt = 1; /* acplt or ifbspro server */

#if OV_SYSTEM_NT
	//char        *ph;
	char        *builddir = "nt";
	char        *buildsys = "NT";
	int 		j;
#else
#if OV_SYSTEM_LINUX
	char        *builddir = "linux";
	char        *buildsys = "LINUX";
#else
#if OV_SYSTEM_SOLARIS
	char        *builddir = "solaris";
	char        *buildsys = "SOLARIS";
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
					"\n\n");
			return 1;
		} else {
			goto HELP;
		}
	}

	if(!libname) {
		goto HELP;
	}


	/* Enviroment */
	//if ((penv=getenv(ifbsEnvVar)) == NULL) {
	//	fprintf(stderr,"No environment variable >%s<\n", pEnvVar);
	//	return 1;
	//}
	if(getenv(acpltEnvVar) != NULL && getenv(ifbsEnvVar) == NULL){
		fprintf(stdout,"ACPLT server detected\n");
		penv = getenv(acpltEnvVar);
		acplt = 1;
	}else if(getenv(ifbsEnvVar) != NULL && getenv(acpltEnvVar) == NULL){
		fprintf(stdout,"IFBS server detected\n");
		penv = getenv(ifbsEnvVar);
		acplt = 0;
	}else if(getenv(ifbsEnvVar) == NULL && getenv(acpltEnvVar) == NULL){
		fprintf(stderr,"Neither %s nor %s are set. No runtime server detected.\n", acpltEnvVar, ifbsEnvVar);
		return 1;
	}else{
		fprintf(stdout,"ACPLT and IFBS servers detected. ACPLT server selected.\n");
		penv = getenv(acpltEnvVar);
		acplt = 1;
	}

	if (!GetCurrentDir(cCurrentPath, sizeof(cCurrentPath))){
		return 1;
        }

	cCurrentPath[sizeof(cCurrentPath) - 1] = '\0'; /* not really required */

	/* printf("The current working directory is %s", cCurrentPath); */

	if(acplt == 1){
		sprintf(userLibPath, "%s/user", penv);	
	}else{
		sprintf(userLibPath, "%s/server/user", penv);
	}

	sprintf(help, "%s/../../model/%s.ovm", cCurrentPath, libname);
	compatiblePath(help);
	if(stat(help, &st) != 0){
			/* ovm does not exist */
			/* Difference between OV and IFBS */
			sprintf(libPath, "%s/%s", userLibPath, libname);
	}else{
			sprintf(libPath, "%s/../..", cCurrentPath);
	}



	/* Check if lib dir is present */
	sprintf(help, "%s", libPath);
	compatiblePath(help);
	if(stat(help, &st) != 0){
			fprintf(stderr,"Directory %s does not exist\n", help);
			return 1;
	}

	/* Check if model dir is present */
	sprintf(help, "%s/model", libPath);
	compatiblePath(help);
	if(stat(help, &st) != 0){
			fprintf(stderr,"Model directory %s does not exist\n", help);
			return 1;
	}

	/* Check if build dir is present */
	sprintf(help, "%s/build", libPath);
	compatiblePath(help);
	if(stat(help, &st) != 0){
			fprintf(stdout,"Creating directory '%s'... \n", help);
			acplt_mkdir(help);
	}

	/* Check if platform dir is present */
	sprintf(help, "%s/build/%s", libPath, builddir);
	compatiblePath(help);
	if(stat(help, &st) != 0){
		acplt_mkdir(help);
	}

	/*
	 *   Search base libraries
	 */
	fb_makmak_searbaselibs(userLibPath, libname, libPath);

	/* ---------------------------- Generating files -------------------------  */
	/* generic.mk */
	sprintf(help, "%s/build/generic.mk", libPath);
	compatiblePath(help);

	fprintf(stdout, "Creating file '%s' ...\n", help);

	fd = fopen(help, "w");
	if(!fd) {
		fprintf(stderr, "Unable to open file '%s' for writing.\n", help);
		return 1;
	}

	fprintf(fd, "#******************************************************************************\n");
	fprintf(fd, "#*\n");
	fprintf(fd, "#*   FILE\n");
	fprintf(fd, "#*   ----\n");
	fprintf(fd, "#*   generic.mk - Makefile fuer die Anwender-Bibliothek (generischer Teil)\n");
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

	//server/user - dir
	fprintf(fd,"USER_DIR          = ../../../\n");
	fprintf(fd,"USERLIB_DIR       = $(USER_DIR)libs\n");

	//server - dir
	fprintf(fd,"ROOT_DIR          = ../../../../\n");
	/* Difference between OV and IFBS */
	if(acplt == 1){
		fprintf(fd,"BASE_DIR          = $(ROOT_DIR)\n");
	}else{
		fprintf(fd,"BASE_DIR          = $(ROOT_DIR)base/\n");
	}
	fprintf(fd,"BASE_INC_DIR      = $(BASE_DIR)include/\n");
	fprintf(fd,"BASE_LIB_DIR      = $(BASE_DIR)lib/\n");
	fprintf(fd,"BASE_MODEL_DIR    = $(BASE_DIR)model/\n");
	fprintf(fd,"BIN_DIR           = $(ROOT_DIR)bin/\n");

	/* Basis-Bibliotheken? */
	for(i=0; i<anzAddLibs; i++) {
		fprintf(fd,"%s_MODEL_DIR           = $(USER_DIR)%s/model/\n"  , getUpperLibName(libs[i]), libs[i]);
		fprintf(fd,"%s_INCLUDE_DIR         = $(USER_DIR)%s/include/\n", getUpperLibName(libs[i]), libs[i]);
	}
	fprintf(fd,"#   Rules\n");
	fprintf(fd,"#   -----\n\n");

	fprintf(fd,".SUFFIXES:\n");
	fprintf(fd,".SUFFIXES: $(_C) .h .ovm $(_LIB) $(_DLL) $(_OBJ)\n\n");

	fprintf(fd,"#	Paths and defines\n");
	fprintf(fd,"#	-----------------\n\n");

	fprintf(fd,"ifeq ($(COMPILER), MSVC)\n");

	fprintf(fd,"INCLUDES  = /I$(BASE_INC_DIR) /I$(BASE_MODEL_DIR) \\\n");
	for(i=0; i<anzAddLibs; i++) {
		fprintf(fd,"\t\t\t/I$(%s_MODEL_DIR) /I$(%s_INCLUDE_DIR) \\\n",
				getUpperLibName(libs[i]), getUpperLibName(libs[i]) );
	}
	fprintf(fd,"\t\t\t/I$(MODEL_DIR) /I$(INCLUDE_DIR)\n");

	fprintf(fd,"else\n");

	fprintf(fd,"INCLUDES  = -I$(BASE_INC_DIR) -I$(BASE_MODEL_DIR)\\\n");
	for(i=0; i<anzAddLibs; i++) {
		fprintf(fd,"\t\t\t-I$(%s_MODEL_DIR) -I$(%s_INCLUDE_DIR) \\\n",
				getUpperLibName(libs[i]), getUpperLibName(libs[i]) );
	}
	fprintf(fd,"\t\t\t-I$(MODEL_DIR) -I$(INCLUDE_DIR)\n");

	fprintf(fd,"endif\n\n");

	fprintf(fd,"VPATH     = $(MODEL_DIR) $(SOURCE_DIR) $(INCLUDE_DIR) \\\n");
	for(i=0; i<anzAddLibs; i++) {
		fprintf(fd,"\t\t\t$(%s_MODEL_DIR) $(%s_INCLUDE_DIR) \\\n",
				getUpperLibName(libs[i]), getUpperLibName(libs[i]) );
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
	if(acplt != 1){
		fprintf(fd,"\tfb.h \\\n");
	}
	for(i=0; i<anzAddLibs; i++) {
		fprintf(fd,"\t%s.h \\\n", libs[i]);
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
	if(acplt != 1){
		fprintf(fd,"\tfb.h \\\n");
	}
	for(i=0; i<anzAddLibs; i++) {
		fprintf(fd,"\t%s.h \\\n", libs[i]);
	}
	fprintf(fd,"	$(USERLIB_SRC)\n\n");
	/* End add */

	fclose(fd);

	/* Generic postbuild script */
	/* postbuild.tcl */
	sprintf(help, "%s/build/postbuild.tcl", libPath);
	compatiblePath(help);

	if(fileExists(help)){
		fprintf(stdout, "File '%s' exists already\n", help);
	}else{
		fprintf(stdout, "Creating file '%s' ...\n", help);
		fd = fopen(help, "w");
		fclose(fd);
	}

	/* Generic prebuild script */
	/* prebuild.tcl */
	sprintf(help, "%s/build/prebuild.tcl", libPath);
	compatiblePath(help);

	if(fileExists(help)){
		fprintf(stdout, "File '%s' exists already\n", help);
	}else{
		fprintf(stdout, "Creating file '%s' ...\n", help);
		fd = fopen(help, "w");
		if(!fd) {
			fprintf(stderr, "unable to open file '%s' for writing.\n", help);
			return 1;
		}

		fclose(fd);
	}


	/* Meta makefile contating the configuration and the call of pre and postbuild scripts */
	/* Makefile */
	sprintf(help, "%s/build/%s/Makefile", libPath, builddir);
	compatiblePath(help);

	if(fileExists(help) && force == 0){
		fprintf(stdout, "File '%s' exists already\n", help);
	}else{
		fprintf(stdout, "Creating file '%s' ...\n", help);
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
		fprintf(fd,"DATABASE = database \n");
		fprintf(fd,"DATABASECREATEKEYS = \n");
		fprintf(fd,"MAKMAKOPTIONS = ");
		if(addOpenLib == 1){
			fprintf(fd,"-o");
		}
		fprintf(fd,"\n");
		fprintf(fd,"EXTRA_CC_FLAGS = \n");
		fprintf(fd,"EXTRA_LIBS = \n");
#if OV_SYSTEM_NT
		fprintf(fd,"LD_FLAGS = -lwsock32\n");
#else
		fprintf(fd,"LD_FLAGS = \n");
#endif
		fprintf(fd,"COMPILER = GCC\n");
		fprintf(fd,"PREBUILD = ../prebuild.tcl\n");
		fprintf(fd,"POSTBUILD = ../postbuild.tcl\n");
		fprintf(fd,"COMPILER_LO = $(call lc,$(COMPILER))\n\n");

		fprintf(fd, "all:\n\n");

		fprintf(fd, "%%:\n");
		fprintf(fd, "\tacplt_makmak -l %s $(MAKMAKOPTIONS)\n", libname);
		sprintf(help, "\ttclsh $(PREBUILD) %s $(MAKMAKOPTIONS)\n", libname);
		compatiblePath(help);
		fprintf(fd, "%s", help);
		fprintf(fd, "\t$(MAKE) EXTRA_CC_FLAGS='$(EXTRA_CC_FLAGS)' EXTRA_LIBS='$(EXTRA_LIBS)' LD_FLAGS='$(LD_FLAGS)' TARGET='$*' -f $(call lc,$(COMPILER)).mk $*\n");
		sprintf(help, "\ttclsh $(POSTBUILD) %s\n", libname);
		compatiblePath(help);
		fprintf(fd, "%s", help);
	}


	/* Makefile for GCC and MinGW */
	/* gcc.mk */
	sprintf(help, "%s/build/%s/gcc.mk", libPath, builddir);
	compatiblePath(help);

	fprintf(stdout, "Creating file '%s' ...\n", help);

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
#if OV_SYSTEM_NT
	fprintf(fd,"_DLL = .dll\n");
	fprintf(fd,"_EXE = .exe\n\n");
#else
	fprintf(fd,"_DLL = .so\n");
	fprintf(fd,"_EXE =\n\n");
#endif

	fprintf(fd,"#	Include generic part\n");
	fprintf(fd,"#	--------------------\n");

	fprintf(fd,"include ../generic.mk\n\n");

	fprintf(fd,"# Libraries\n");
	fprintf(fd,"# ---------\n\n");
#if OV_SYSTEM_NT
	fprintf(fd,"OVLIBS = $(BASE_LIB_DIR)libov$(_LIB) $(BASE_LIB_DIR)libovks$(_LIB)\n");
	if(anzAddLibs > 0) {
		fprintf(fd,"ADD_LIBS =");
		for(i=0; i<anzAddLibs; i++) {
			/* fprintf(fd," $(USER_DIR)%s/build/%s/%s$(_LIB)", libs[i], builddir, libs[i]); */
			/* link directly against dll */
			fprintf(fd," $(USER_DIR)libs/%s$(_DLL)", libs[i]);
		}
		fprintf(fd,"\n");
	}
#else
	fprintf(fd,"# Swithces for additional libraries needed for dynamic linkage in Linux\n");
	if(anzAddLibs > 0) {
		fprintf(fd,"ADD_LIBS_SWITCHES =");
		for(i=0; i<anzAddLibs; i++) {
			/* fprintf(fd," $(USER_DIR)%s/build/%s/%s$(_LIB)", libs[i], builddir, libs[i]); */
			/* link against .a */
			fprintf(fd," %s$(_DLL)", libs[i]);
		}
		fprintf(fd,"\n");
	}
#endif
	fprintf(fd,"ADD_LIBS += $(foreach lib, $(EXTRA_LIBS),$(lib))\n\n");

	/* Compiler-Optionen */
	fprintf(fd,"#	Compiler\n");
	fprintf(fd,"#	--------\n");
	fprintf(fd,"OV_CODEGEN_EXE = $(BIN_DIR)ov_codegen$(_EXE)\n");
	fprintf(fd,"GCC_BIN_PREFIX		= \n");
	fprintf(fd,"CC		= $(GCC_BIN_PREFIX)gcc\n");
	fprintf(fd,"ifneq ($(TARGET), debug)\n");
	fprintf(fd,"\tOPT = -O2 -fno-strict-aliasing\n");
	fprintf(fd,"endif\n");
	fprintf(fd,"CC_FLAGS	= -g -std=c99 -m32 -Wdeclaration-after-statement -Wall -Wno-attributes $(OPT) -shared $(EXTRA_CC_FLAGS)\n");
#if OV_SYSTEM_NT
	fprintf(fd,"CC_DEFINES	= $(DEFINES) -D__NT__=1 \n");
#else	
	fprintf(fd,"CC_DEFINES	= $(DEFINES)\n");
#endif

	fprintf(fd,"CC_INCLUDES	= $(INCLUDES) -I.\n");
	fprintf(fd,"COMPILE_C	= $(CC) $(CC_FLAGS) $(CC_DEFINES) $(CC_INCLUDES) -c\n");

#if OV_SYSTEM_NT
	fprintf(fd,"LD		= $(CC) -shared -m32 -Wl,--output-def,%s.def,--out-implib,%s.a\n", libname, libname);
#else
	fprintf(fd,"LD		= $(CC) -shared -m32\n");
#endif

	fprintf(fd,"AR			= $(GCC_BIN_PREFIX)ar\n");
	fprintf(fd,"RANLIB		= $(GCC_BIN_PREFIX)ranlib\n");
	fprintf(fd,"STRIP		= $(GCC_BIN_PREFIX)strip\n");

	fprintf(fd,"\n");

	/* Targets */
	fprintf(fd,"templates: \n");
#if OV_SYSTEM_NT
	fprintf(fd,"\t-@cmd /c del ./../source/sourcetemplates/*$(_C)\n");
#else
	fprintf(fd,"\t-@rm ../../source/sourcetemplates/*$(_C)\n");
#endif
	fprintf(fd,"\tacplt_builder -l %s $(MAKMAKOPTIONS)\n", libname);

	fprintf(fd,"\n");

	fprintf(fd,"all: $(TARGETS)\n");
	fprintf(fd,"ifndef STATIC_ONLY\n");
	fprintf(fd,"\t$(STRIP) --strip-debug $(USERLIB_LIB)\n");
	fprintf(fd,"\t$(STRIP) --strip-debug $(USERLIB_DLL)\n");
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
	fprintf(fd,"\t$(OV_CODEGEN_EXE) -I $(BASE_MODEL_DIR)");
	for(i=0; i<anzAddLibs; i++) {
		fprintf(fd," -I $(%s_MODEL_DIR)", getUpperLibName(libs[i]));
	}
	fprintf(fd," -f $(MODEL_DIR)$(LIBRARY).ovm -l $(notdir $(basename $<))\n\n");

	fprintf(fd,"%%.c %%.h: %%.ovm Makefile\n");
	fprintf(fd,"\t$(OV_CODEGEN_EXE) -I $(BASE_MODEL_DIR)");
	for(i=0; i<anzAddLibs; i++) {
		fprintf(fd," -I $(%s_MODEL_DIR)", getUpperLibName(libs[i]));
	}
	fprintf(fd," -f $< -l $(notdir $(basename $<))\n\n");

	fprintf(fd,"\n");
	fprintf(fd,"%%.o: %%.c\n");
	fprintf(fd,"%%.o: %%.c $(HEADERS)\n");
	fprintf(fd,"\t$(COMPILE_C) -o $@ $<\n");
	fprintf(fd,"\n");
#if OV_SYSTEM_NT
	fprintf(fd, "$(USERLIB_LIB) : $(USERLIB_OBJ) $(ADD_LIBS) $(OVLIBS)\n");
#else
	fprintf(fd, "$(USERLIB_LIB) : $(USERLIB_OBJ) $(ADD_LIBS)\n");
#endif
	//fprintf(fd, "$(USERLIB_LIB) : $(USERLIB_DLL)\n");
#ifndef OV_SYSTEM_NT
	fprintf(fd,"\t$(AR) rv $@ $^\n");
	fprintf(fd,"\t$(RANLIB) $@\n");
#endif
	fprintf(fd,"\n");
#if OV_SYSTEM_NT
	fprintf(fd, "$(USERLIB_DLL) : $(USERLIB_OBJ) $(ADD_LIBS) $(OVLIBS)\n");
	fprintf(fd, "\t$(LD) -o $@ $^ $(LD_FLAGS)\n");
#else
	fprintf(fd, "$(USERLIB_DLL) : $(USERLIB_OBJ) $(ADD_LIBS)\n");
	if(anzAddLibs > 0) {
		//ugly method of copying dependencies, but it is the only one to work :(
		for(i=0; i<anzAddLibs; i++) {
			fprintf(fd,"\tcp $(USERLIB_DIR)/%s$(_DLL) %s$(_DLL)\n", libs[i], libs[i]);
		}
	}
	fprintf(fd, "\t$(LD) -o $@ $^ $(ADD_LIBS_SWITCHES) $(LD_FLAGS)\n");
#endif
	fprintf(fd,"\n");

#if OV_SYSTEM_NT
	fprintf(fd,"#   Dependencies\n");
	fprintf(fd,"#   ------------\n\n");

	fprintf(fd,".depend:\n");
	fprintf(fd,"\ttouch $@\n\n");

	fprintf(fd,"depend : $(SOURCES)\n");
	fprintf(fd,"\t$(COMPILE_C) -MM $(USERLIB_SRC) > .depend\n\n");
#endif

	fprintf(fd,"#	Aufraeumen\n");
	fprintf(fd,"#	----------\n\n");

	fprintf(fd,"clean:\n");
#if OV_SYSTEM_NT
	fprintf(fd,"\t-@cmd /c del *$(_C) *.h *$(_LIB) *$(_DLL) *$(_OBJ)\n");
#else
	fprintf(fd,"\t-@rm *$(_C) *.h *$(_LIB) *$(_DLL) *$(_OBJ)\n");
#endif
	fclose(fd);


/* ----------------------- Start of depricated makefiles ----------------------- */

#if OV_SYSTEM_NT
	/* Add by Sten on 21.06.11 */
	/* Makefile fuer Borland (depricated) */
	sprintf(help, "%s/build/%s/borland.mk", libPath, builddir);
	compatiblePath(help);

	fprintf(stdout, "Creating file '%s' ...\n", help);

	fd = fopen(help, "w");
	if(!fd) {
		fprintf(stderr, "unable to open file '%s' for writing.\n", help);
		return 1;
	}

	fprintf(fd, "#******************************************************************************\n");
	fprintf(fd, "#*\n");
	fprintf(fd, "#*   FILE\n");
	fprintf(fd, "#*   ----\n");
	fprintf(fd, "#*   borland.mk - Makefile fuer die Anwender-Bibliothek\n");
	fprintf(fd, "#******************************************************************************\n");
	fprintf(fd, "#*\n");
	fprintf(fd, "#*   This file is generated by the 'fb_makmak' command\n");
	fprintf(fd, "#*\n");
	fprintf(fd, "#******************************************************************************\n");

	fprintf(fd,"\n");
	fprintf(fd,"#   Plattform\n");
	fprintf(fd,"#   ---------\n\n");
	fprintf(fd,"SYSTEM = %s\n", buildsys);
	fprintf(fd,"SYSDIR = %s\n\n", builddir);
	fprintf(fd,"COMPILER = BORLAND\n\n");
	fprintf(fd,"#	Filename conventions\n");
	fprintf(fd,"#	--------------------\n");
	fprintf(fd,"_C   = .c\n");
	fprintf(fd,"_OBJ = .obj\n");
	fprintf(fd,"_LIB = .lib\n");
	fprintf(fd,"_DLL = .dll\n");
	fprintf(fd,"_EXE = .exe\n");
	fprintf(fd,"_RES = .res\n\n");
	fprintf(fd,"#	Include generic part\n");
	fprintf(fd,"#	--------------------\n");

	fprintf(fd,"include ../generic.mk\n\n");



	fprintf(fd,"# Libraries\n");
	fprintf(fd,"# ---------\n");
	fprintf(fd,"OVLIBS = $(BASE_LIB_DIR)libov$(_LIB) $(BASE_LIB_DIR)libovks$(_LIB)\n");
	if(acplt != 1){
		fprintf(fd,"FBLIBS = $(BASE_LIB_DIR)fb$(_LIB)\n");
	}

	if(anzAddLibs > 0) {
		fprintf(fd,"ADD_LIBS =");
		for(i=0; i<anzAddLibs; i++) {
			fprintf(fd," $(USER_DIR)%s/build/%s/%s$(_LIB)"  , libs[i], builddir, libs[i]);
		}
		fprintf(fd,"\n");
	}
	fprintf(fd,"\n");


	/* Targets */
	fprintf(fd,"all: $(TARGETS)\n");

	fprintf(fd,"\t@tdstrp32 $(USERLIB_DLL)\n");
	fprintf(fd,"\tcmd /c copy $(USERLIB_DLL) $(subst /,\\\\, $(USERLIB_DIR))\n");

	fprintf(fd,"\n");

	fprintf(fd,"debug: $(TARGETS)\n");

	fprintf(fd,"\tcmd /c copy $(USERLIB_DLL) $(subst /,\\\\, $(USERLIB_DIR))\n\n");

	fprintf(fd,"\n");


	/* Compiler-Optionen */
	fprintf(fd,"#	Compiler\n");
	fprintf(fd,"#	--------\n\n");

	fprintf(fd,"OV_CODEGEN_EXE = $(BIN_DIR)ov_codegen$(_EXE)\n");

	fprintf(fd,"CC              = bcc32\n");
	fprintf(fd,"CC_FLAGS        = -w -v -pc -wsig- -a8 $(EXTRA_CC_FLAGS)\n");
	fprintf(fd,"CC_DEFINES      = $(DEFINES) -DFD_SETSIZE=128 -DOV_COMPILE_LIBRARY_$(LIBRARY)\n");
	fprintf(fd,"CC_INCLUDES     = $(INCLUDES) -I.\n");
	fprintf(fd,"COMPILE_C		= $(CC) $(CC_FLAGS) $(CC_DEFINES) $(CC_INCLUDES) -c\n");
	fprintf(fd,"LINK            = $(CC) $(CC_FLAGS)\n");
	fprintf(fd,"MKIMPDEF        = $(BIN_DIR)mkimpdef$(_EXE)\n");
	fprintf(fd,"MKEXPDEF        = $(BIN_DIR)mkexpdef$(_EXE)\n");
	fprintf(fd,"MKDLLDEF        = $(BIN_DIR)mkdlldef$(_EXE)\n");
	fprintf(fd,"IMPLIB          = implib\n");
	fprintf(fd,"IMPDEF          = impdef\n");
	fprintf(fd,"LD              = $(CC) $(CC_FLAGS) -tWDE\n");
	fprintf(fd,"AR              = tlib /P64\n");
	fprintf(fd,"RC              = brc32\n");

	fprintf(fd,"\n");
	fprintf(fd,"#   Rules\n");
	fprintf(fd,"#   -----\n");

	fprintf(fd,"$(_C)$(_OBJ):\n");
	fprintf(fd,"\t$(COMPILE_C) -o$@ $<\n\n");

	fprintf(fd,".ovm$(_C):\n");
	fprintf(fd,"\t$(OV_CODEGEN_EXE) -I $(BASE_MODEL_DIR)");
	for(i=0; i<anzAddLibs; i++) {
		/* BUG FIXED BY STEN on Jun 10*/
		/*	fprintf(fd," -I $(%s_MODEL_DIR)", libs[i]); */
		fprintf(fd," -I $(%s_MODEL_DIR)", getUpperLibName(libs[i]));

	}
	fprintf(fd," -f $(subst /,\\\\, $<) -l $(notdir $(basename $<))\n\n");

	fprintf(fd,".ovm.h:\n");
	fprintf(fd,"\t$(OV_CODEGEN_EXE) -I $(BASE_MODEL_DIR)");
	for(i=0; i<anzAddLibs; i++) {
		/* BUG FIXED BY STEN on Jun 10*/
		/*	   fprintf(fd," -I $(%s_MODEL_DIR)", libs[i]); */
		fprintf(fd," -I $(%s_MODEL_DIR)", getUpperLibName(libs[i]));
	}

	fprintf(fd," -f $(subst /,\\\\, $<) -l $(notdir $(basename $<))\n\n");    


	fprintf(fd,"ov.h : $(BASE_MODEL_DIR)ov.ovm\n");
	fprintf(fd,"\t$(OV_CODEGEN_EXE) -f $<\n\n");

	if(acplt != 1){
		fprintf(fd,"fb.h : $(BASE_MODEL_DIR)fb.ovm\n");
		fprintf(fd,"\t$(OV_CODEGEN_EXE) -f $< -I $(BASE_MODEL_DIR)\n\n");
	}

	for(i=0; i<anzAddLibs; i++) {
		fprintf(fd,"%s.h : $(%s_MODEL_DIR)%s.ovm\n", libs[i],
				getUpperLibName(libs[i]), libs[i]);
		fprintf(fd,"\t$(OV_CODEGEN_EXE) -f $< -I $(BASE_MODEL_DIR)");

		for(j=0; j<anzAddLibs; j++) {
			fprintf(fd," -I $(%s_MODEL_DIR)", getUpperLibName(libs[j]) );
		}
		fprintf(fd,"\n\n");
	}

	fprintf(fd,"%s.h : $(MODEL_DIR)%s.ovm\n", libname, libname);
	fprintf(fd,"\t$(OV_CODEGEN_EXE) -f $< -I $(BASE_MODEL_DIR)");
	for(j=0; j<anzAddLibs; j++) {
		fprintf(fd," -I $(%s_MODEL_DIR)", getUpperLibName(libs[j]) );
	}
	fprintf(fd,"\n\n");


	//fprintf(fd,"$(USERLIB_LIB) : $(USERLIB_DLL)\n\n");

	//if(acplt == 1){
	//	fprintf(fd,"$(USERLIB_DLL) : $(USERLIB_OBJ) $(ADD_LIBS) $(OVLIBS)\n");	
	//} else {
	//	fprintf(fd,"$(USERLIB_DLL) : $(USERLIB_OBJ) $(ADD_LIBS) $(FBLIBS) $(OVLIBS)\n");
	//}

	if(acplt == 1){
		fprintf(fd,"$(USERLIB_LIB) : $(USERLIB_OBJ) $(ADD_LIBS) $(OVLIBS)\n");	
	} else {
		fprintf(fd,"$(USERLIB_LIB) : $(USERLIB_OBJ) $(ADD_LIBS) $(FBLIBS) $(OVLIBS)\n");
	}

	if(acplt == 1){
		fprintf(fd,"$(USERLIB_DLL) : $(USERLIB_OBJ) $(ADD_LIBS) $(OVLIBS)\n");	
	} else {
		fprintf(fd,"$(USERLIB_DLL) : $(USERLIB_OBJ) $(ADD_LIBS) $(FBLIBS) $(OVLIBS)\n");
	}

	fprintf(fd,"\t-@del $(basename $@).def\n");
	fprintf(fd,"\t$(LD) -e$@ $(filter-out $(_RES), $^)\n");
	fprintf(fd,"\t$(IMPDEF) $(basename $@)_tmp.def $@\n");
	fprintf(fd,"\t$(MKIMPDEF) $(basename $@)_tmp.def $(basename $@).def\n");
	fprintf(fd,"\t$(IMPLIB) $(basename $@)$(_LIB) $(basename $@).def\n");
	fprintf(fd,"\t$(MKEXPDEF) $(basename $@)_tmp.def $(basename $@).def\n");
	fprintf(fd,"\t$(LD) -e$@ $(filter-out $(_RES), $^)\n");
	fprintf(fd,"\t$(MKDLLDEF) $(basename $@)_tmp.def $(basename $@).def\n");
	fprintf(fd,"\t@del $(basename $@)_tmp.def\n\n");



	fprintf(fd,"#	Aufraeumen\n");
	fprintf(fd,"#	----------\n\n");

	fprintf(fd,"clean:\n");
	fprintf(fd,"\t-@del *$(_C) *.h *.x *.bak *.map *$(_LIB) *$(_DLL) *$(_OBJ) *$(_RES) respfile\n");
	fclose(fd);

	/* End add by Sten */


	/* Makefile fuer MSVC (depricated) */
	sprintf(help, "%s/build/%s/msvc.mk", libPath, builddir);
	compatiblePath(help);

	fprintf(stdout, "Creating file '%s' ...\n", help);

	fd = fopen(help, "w");
	if(!fd) {
		fprintf(stderr, "unable to open file '%s' for writing.\n", help);
		return 1;
	}

	fprintf(fd, "#******************************************************************************\n");
	fprintf(fd, "#*\n");
	fprintf(fd, "#*   FILE\n");
	fprintf(fd, "#*   ----\n");
	fprintf(fd, "#*   msvc.mk - Makefile fuer die Anwender-Bibliothek\n");
	fprintf(fd, "#******************************************************************************\n");
	fprintf(fd, "#*\n");
	fprintf(fd, "#*   This file is generated by the 'fb_makmak' command\n");
	fprintf(fd, "#*\n");
	fprintf(fd, "#******************************************************************************\n");

	fprintf(fd,"\n");
	fprintf(fd,"#   Plattform\n");
	fprintf(fd,"#   ---------\n\n");
	fprintf(fd,"SYSTEM = %s\n", buildsys);
	fprintf(fd,"SYSDIR = %s\n\n", builddir);

	fprintf(fd,"COMPILER = MSVC\n\n");

	fprintf(fd,"#	Filename conventions\n");
	fprintf(fd,"#	--------------------\n");

	fprintf(fd,"_C   = .c\n");
	fprintf(fd,"_OBJ = .obj\n");
	fprintf(fd,"_LIB = .lib\n");
	fprintf(fd,"_DLL = .dll\n");
	fprintf(fd,"_EXE = .exe\n");
	fprintf(fd,"_RES = .res\n\n");

	fprintf(fd,"#	Include generic part\n");
	fprintf(fd,"#	--------------------\n");

	fprintf(fd,"include ../generic.mk\n\n");

	fprintf(fd,"# Libraries\n");
	fprintf(fd,"# ---------\n");
	fprintf(fd,"OVLIBS = $(BASE_LIB_DIR)libov$(_LIB) $(BASE_LIB_DIR)libovks$(_LIB)\n");
	fprintf(fd,"ADD_LIBS += $(foreach lib, $(EXTRA_LIBS),$(lib))\n\n");
	if(acplt != 1){
		fprintf(fd,"FBLIBS = $(BASE_LIB_DIR)fb_msvc$(_LIB)\n");
	}

	if(anzAddLibs > 0) {
		fprintf(fd,"ADD_LIBS =");
		for(i=0; i<anzAddLibs; i++) {
			fprintf(fd," $(USER_DIR)%s/build/%s/%s$(_LIB)"  , libs[i], builddir, libs[i]);
		}
		fprintf(fd,"\n");
	}
	fprintf(fd,"\n");

	fprintf(fd,"all: $(TARGETS)\n\n");
	fprintf(fd,"debug: $(TARGETS)\n\n");

	fprintf(fd,"#	Compiler\n");
	fprintf(fd,"#	--------\n\n");

	fprintf(fd,"OV_CODEGEN_EXE = $(BIN_DIR)ov_codegen$(_EXE)\n");

	fprintf(fd,"CC              = cl\n");

	fprintf(fd, "ifeq ($(MAKECMDGOALS), debug)\n");
	fprintf(fd, "CC_FLAGS        = /Zi /MTd /c $(EXTRA_CC_FLAGS)\n");
	fprintf(fd, "else\n");
	fprintf(fd, "CC_FLAGS        = /W3 /c\n");
	fprintf(fd, "endif\n");

	fprintf(fd,"CC_DEFINES      = $(DEFINES) /DOV_COMPILE_LIBRARY_$(LIBRARY)\n");
	fprintf(fd,"CC_INCLUDES     = $(INCLUDES) /I.\n");
	fprintf(fd,"COMPILE_C		= $(CC) $(CC_FLAGS) /TC $(CC_DEFINES) $(CC_INCLUDES) -c\n");
	fprintf(fd,"LD              = link /DLL\n");

	fprintf(fd,"\n");
	fprintf(fd,"#   Rules\n");
	fprintf(fd,"#   -----\n");

	fprintf(fd,"$(_C)$(_OBJ):\n");
	fprintf(fd,"\t$(COMPILE_C) -o$@ $<\n\n");

	fprintf(fd,".ovm$(_C):\n");
	fprintf(fd,"\t$(OV_CODEGEN_EXE) -I $(BASE_MODEL_DIR)");
	for(i=0; i<anzAddLibs; i++) {
		/* STEN: potential bug, since getUpperLibName is unused. I will not touch depricated msvc Makefiles*/
		fprintf(fd," -I $(%s_MODEL_DIR)", libs[i]);
	}
	fprintf(fd," -f $(subst /,\\\\, $<) -l $(notdir $(basename $<))\n\n");    

	fprintf(fd,".ovm.h:\n");
	fprintf(fd,"\t$(OV_CODEGEN_EXE) -I $(BASE_MODEL_DIR)");
	for(i=0; i<anzAddLibs; i++) {
		/* STEN: potential bug, since getUpperLibName is unused. I will not touch depricated msvc Makefiles*/
		fprintf(fd," -I $(%s_MODEL_DIR)", libs[i]);
	}
	fprintf(fd," -f $(subst /,\\\\, $<) -l $(notdir $(basename $<))\n\n");    


	fprintf(fd,"ov.h : $(BASE_MODEL_DIR)ov.ovm\n");
	fprintf(fd,"\t$(OV_CODEGEN_EXE) -f $<\n\n");

	if(acplt != 1){
		fprintf(fd,"fb.h : $(BASE_MODEL_DIR)fb.ovm\n");
		fprintf(fd,"\t$(OV_CODEGEN_EXE) -f $< -I $(BASE_MODEL_DIR)\n\n");
	}

	for(i=0; i<anzAddLibs; i++) {
		fprintf(fd,"%s.h : $(%s_MODEL_DIR)%s.ovm\n", libs[i],
				getUpperLibName(libs[i]), libs[i]);
		fprintf(fd,"\t$(OV_CODEGEN_EXE) -f $< -I $(BASE_MODEL_DIR)");

		for(j=0; j<anzAddLibs; j++) {
			fprintf(fd," -I $(%s_MODEL_DIR)", getUpperLibName(libs[j]) );
		}
		fprintf(fd,"\n\n");
	}

	fprintf(fd,"%s.h : $(MODEL_DIR)%s.ovm\n", libname, libname);
	fprintf(fd,"\t$(OV_CODEGEN_EXE) -f $< -I $(BASE_MODEL_DIR)");
	for(j=0; j<anzAddLibs; j++) {
		fprintf(fd," -I $(%s_MODEL_DIR)", getUpperLibName(libs[j]) );
	}
	fprintf(fd,"\n\n");


	fprintf(fd,"$(USERLIB_LIB) : $(USERLIB_OBJ)\n\n");

	fprintf(fd,"$(USERLIB_DLL) : $(USERLIB_OBJ)\n");

	if(acplt == 1){
		fprintf(fd,"\t$(LD) $(USERLIB_OBJ) $(ADD_LIBS) $(OVLIBS) $(LINK_FLAGS) /OUT:$@\n");
	}else{
		fprintf(fd,"\t$(LD) $(USERLIB_OBJ) $(ADD_LIBS) $(FBLIBS) $(OVLIBS) $(LINK_FLAGS) /OUT:$@\n");
	}
	fprintf(fd,"\tcmd /c copy $(USERLIB_DLL) $(subst /,\\\\, $(USERLIB_DIR))\n\n");


#if OV_SYSTEM_NT
#else
	fprintf(fd,"#   Dependencies\n");
	fprintf(fd,"#   ------------\n\n");

	fprintf(fd,".depend:\n");
	fprintf(fd,"\ttouch $@\n\n");

	fprintf(fd,"depend : $(SOURCES)\n");
	fprintf(fd,"\t$(COMPILE_C) -MM $(USERLIB_SRC) > .depend\n\n");
#endif


	fprintf(fd,"#	Aufraeumen\n");
	fprintf(fd,"#	----------\n\n");

	fprintf(fd,"clean:\n");
	fprintf(fd,"\t-@del *$(_C) *.h *.x *.bak *.map *$(_LIB) *$(_DLL) *$(_OBJ) *$(_RES) respfile\n");

	fclose(fd);

#endif


	return 0;
}

/*
 *	End of file
 */
