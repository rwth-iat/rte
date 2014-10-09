#include "definitions.h"
#include "common.h"

/*
 *	Include-Dateien
 *	---------------
 */
#include <stdio.h> /* defines FILENAME_MAX */
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/**

	Handling the path location business and consistency checks

**/
int locateLibrary(const char* libname, char* libPath, char *devModelPath, char* devBinPath, char* sysModelPath, char* sysBinPath, int* newDirStructure){
	// Locate library
	char	cCurrentPath[FILENAME_MAX];
	char	help[512];
	char	*penv;
	int		env;

	if (!GetCurrentDir(cCurrentPath, sizeof(cCurrentPath))){
		return 1;
    }
	cCurrentPath[sizeof(cCurrentPath) - 1] = '\0'; /* not really required */

	//TRYING TO FIND THE LIBRARY
	//1ST TRY: ARE WE IN libPath/build/platform/ ?
	if(strlen(libPath)==0){
		sprintf(help, "%s/../../model/%s.ovm", cCurrentPath, libname);
		compatiblePath(help);
		if(stat(help, &st) == 0){
			sprintf(libPath, "%s/../..", cCurrentPath);
			//newDirStructure or old structure?
			sprintf(help, "%s/../../user", libPath);
			compatiblePath(help);
			if(stat(help, &st) == 0){
				//old
				*newDirStructure = 0;
				sprintf(devModelPath, "%s/../../user/", libPath);
				sprintf(devBinPath, "%s/../../user/libs/", libPath);
				//sysModelPath and sysBinPath are empty
			}else{
				//newDirStructure
				*newDirStructure = 1;
				sprintf(devModelPath, "%s/../../dev/", libPath);
				sprintf(devBinPath, "%s/../../addonlibs/", libPath);
				sprintf(sysModelPath, "%s/../../system/sysdevbase/", libPath);
				sprintf(sysBinPath, "%s/../../system/sysbin/", libPath);
			}
		}
	}

	/* Explore enviroment */
	if(getenv(ACPLT_HOME_ENVPATH) != NULL){
		//fprintf(stdout,"ACPLT server detected\n");
		penv = getenv(ACPLT_HOME_ENVPATH);
		env = 1;
	}else{
		//fprintf(stderr,"No environment variable found, exporing current working dir\n");
		env = 0;
	}


	//2ND TRY: try to follow the ACPLT_HOME envionment variable (older structure): take $ACPLT_HOME/user
	if(strlen(libPath)==0 && env == 1){
		sprintf(help, "%s/user/%s/model/%s.ovm", penv, libname, libname);
		compatiblePath(help);
		if(stat(help, &st) == 0){
			*newDirStructure = 0;
			sprintf(libPath, "%s/user/%s", penv, libname);
			sprintf(devModelPath, "%s/user/", penv);
			sprintf(devBinPath, "%s/user/libs/", penv);
			//sysModelPath and sysBinPath are empty
		}
	}

	//3ND TRY: try to follow the ACPLT_HOME envionment variable: take $ACPLT_HOME/dev
	if(strlen(libPath)==0 && env == 1){
		sprintf(help, "%s/dev/%s/model/%s.ovm", penv, libname, libname);
		compatiblePath(help);
		if(stat(help, &st) == 0){
			*newDirStructure = 1;
			sprintf(libPath, "%s/dev/%s", penv, libname);
			sprintf(devModelPath, "%s/dev/", penv);
			sprintf(devBinPath, "%s/addonlibs/", penv);
			sprintf(sysModelPath, "%s/system/sysdevbase/", penv);
			sprintf(sysBinPath, "%s/system/sysbin/", penv);
		}
	}

	//get the right slashes
	compatiblePath(libPath);
	compatiblePath(devModelPath);
	compatiblePath(devBinPath);
	compatiblePath(sysModelPath);
	compatiblePath(sysBinPath);

	/* printf("The current working directory is %s", cCurrentPath); */
	if(strlen(libPath)==0){
		fprintf(stderr, "Error: Could not locate library %s\n", libname);
		return 1;
	}else{
		fprintf(stderr,"Working library %s located in %s\n", libname, libPath);
		fprintf(stderr,"Libpath %s\n", libPath);
		fprintf(stderr,"DevModel %s\n", devModelPath);
		fprintf(stderr,"DevBin %s\n", devBinPath);
		fprintf(stderr,"SysModel %s\n", sysModelPath);
		fprintf(stderr,"SysBin %s\n", sysBinPath);
	}

	//======= some more paranoid consistency checls ===============
	/* Check if lib dir is present */
	sprintf(help, "%s", libPath);
	compatiblePath(help);
	if(stat(help, &st) != 0){
			fprintf(stderr,"Error: Directory %s does not exist\n", help);
			return 1;
	}

	/* Check if model dir is present */
	sprintf(help, "%s/model", libPath);
	compatiblePath(help);
	if(stat(help, &st) != 0){
			fprintf(stderr,"Error: Model directory %s does not exist\n", help);
			return 1;
	}
	return 0;
}


/*	----------------------------------------------------------------------	*/ 
/*
 *  Update slashes to match windows plattform
 */
void compatiblePath(char* string){
#if OV_SYSTEM_NT
	char* ph;
	ph = string;
	while(ph && (*ph)) {
		if(*ph == '/') {
			*ph = '\\';
		}
		ph++;
	}
#endif
}

/*	----------------------------------------------------------------------	*/
/*
 *   Search base libraries
 */
/***

    Searching for lib dependencies located in recursively, first in devModelPath, then in sysModelPath, results are saved in arrays devLibs and sysLibs with counters

***/
void makmak_searchbaselibs(const char *lib, const char *devModelPath, const char *sysModelPath, char **devLibs, int* numDevLibs, char **sysLibs, int* numSysLibs) {
	char   originallib[512];
	strcpy(originallib, lib);
	makmak_searchbaselibs_worker(originallib, lib, devModelPath, sysModelPath, devLibs, numDevLibs, sysLibs, numSysLibs);
}

void makmak_searchbaselibs_worker(const char *originallib, const char *curlib, const char *devModelPath, const char *sysModelPath, char **devLibs, int* numDevLibs, char **sysLibs, int* numSysLibs) {
	/* Local variables */
	char   help[512];
	FILE   *fd;
	char   *incFile;
	int    incFound;
	char   *ph;
	int    i;
	int    dev = 1;
	FILE   *fdreadtest;

	/* OV library? -- already included */
	if( !strcmp(curlib, "ov") ) {
		return;
	}

	/* if no system dir is set */
	if(dev == 0 && strlen(sysModelPath)==0){
		return;
	}

	/* Exclude fb.dll from standard includes
	if( !strcmp(lib, "fb") ) {
		return;
	}
	*/

	/* prevent stack overflow */
	if(*numDevLibs >= MAX_INCLUDED_FILES || *numSysLibs >= MAX_INCLUDED_FILES) {
		fprintf(stderr, "To many included files.\n");
		exit(0);
	}

	/* recurion abort condition - Bereits vermerkt? */
	/* inside of devLibs */
	for(i=0; i<*numDevLibs; i++) {
		if( !strcmp(curlib, devLibs[i]) ) {
			return;
		}
	}
	/* inside of sysLibs */
	for(i=0; i<*numSysLibs; i++) {
		if( !strcmp(curlib, sysLibs[i]) ) {
			return;
		}
	}

	//iterate at most two times for dev=0 and dev=1
	while(dev >= 0){
		/* locate library model */
		if(dev==1){
			sprintf(help, "%s/%s/model/%s.ovm", devModelPath, curlib, curlib);
		}else{
			sprintf(help, "%s/%s/model/%s.ovm", sysModelPath, curlib, curlib);
		}
		compatiblePath(help);	

		fd = fopen(help, "r");
		/* If model exists - parse it */

		if(fd) {
			//read the file line by line with the maximum line length 511 (as our helper string is not much longer)
			while( fgets(help, 511, fd) ) {
				incFound = 0;
				ph = strstr(help, "#include");
				/* locate # and 'include' in a line, # must come before 'include' */
				/* make sure the line really starts with "#include" to allow commenting includes out */
				if(ph && ph==help) {
					/* skip include */
					ph += 8;
					incFile = NULL;
					while(ph && *ph && incFound == 0) {
						switch(*ph) {
						case '<':
						case '"':
							ph++;
							if(!incFile) {
								incFile = ph;
							} else {
								*ph = '\0';
								incFound = 1;
							}
							break;
						case '.':
							//remove the file extension
							*ph = '\0';
							incFound = 1;
							break;
						/* skip */					
						case '>':
						case ' ':
						case '\t':
						default:
							break;

						}

						if(incFound) {
							break;
						}

						ph++;
					}
					if(incFound) {
						ph = (char*)malloc(strlen(incFile) + 1);
						if(!ph) {
							fprintf(stderr, "Out of memory\n");
							exit(0);
						}
						strcpy(ph, incFile);

						//it is possible to have the variables in a separate OVM in the same directory (for example simblocks)
						sprintf(help, "%s/%s/model/%s.ovm", devModelPath, curlib, ph);
						compatiblePath(help);
						fdreadtest = fopen(help, "r");
						if(fdreadtest != NULL){
							fclose(fdreadtest);
							free(ph);
							//skip this ovm file
							continue;
						}
						/** recursive deepining - search for ph **/
						makmak_searchbaselibs_worker(originallib, ph, devModelPath, sysModelPath, devLibs, numDevLibs, sysLibs, numSysLibs);
						free(ph);
					}
				}
			}
			/* Close file */
			fclose(fd);

			/* make sure we have no self-dependency */
			if(0 == strcmp(curlib, originallib)) {
				return;
			}

			/* If we reached this line - the discovered library is found and new */
			ph = (char*)malloc(strlen(curlib) + 1);
			if(!ph) {
				fprintf(stderr, "Out of memory\n");
				exit(0);
			}
			strcpy(ph, curlib);
			if(dev == 1){
				devLibs[(*numDevLibs)++] = ph;
			}else{
				sysLibs[(*numSysLibs)++] = ph;
			}
			//jump out of the while queue
			break;
		}else{
			/* make sure we have no self-dependency */
			if(0 == strcmp(curlib, originallib)) {
				return;
			}

			/* model file not found and it was the second search pass */
			if(dev == 0){
				/* inside of devLibs */
				for(i=0; i<*numDevLibs; i++) {
					if( !strcmp(curlib, devLibs[i]) ) {
						return;
					}
				}
				/* inside of sysLibs */
				for(i=0; i<*numSysLibs; i++) {
					if( !strcmp(curlib, sysLibs[i]) ) {
						return;
					}
				}
				fprintf(stderr, "Error: .ovm model of included library %s could not be located\n", curlib);
				exit(0);
			}
		}
		dev--;																																
	}
}

/*	----------------------------------------------------------------------	*/
/*
 * Checks whether a file exists
 */
int fileExists(char* filename){
	FILE* file;
	file = fopen(filename, "r");
	if(file){
		fclose(file);
		return 1;
	}
	return 0;
}

/*	----------------------------------------------------------------------	*/
/*
 * Creates directory
 */
int acplt_mkdir(char* dir){
#if OV_SYSTEM_NT
			_mkdir(dir);
#else
			mkdir(dir, 0755);
#endif
	return 1;
}

