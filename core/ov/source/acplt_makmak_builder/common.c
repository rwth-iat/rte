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
#include <dirent.h>

/**

	Handling the path location business and consistency checks

**/

int sortCompare(const void* a, const void* b){

	return strcmp(*(char**)a, *(char**)b);

}

int locateLibrary(const char* libname, char* libPath, char *devModelPath, char* devBinPath,
		char* gitModelPath, char* sysModelPath, char* sysBinPath, int* newDirStructure){
	// Locate library
	char	cCurrentPath[FILENAME_MAX];
	char	help[512];
	char	*penv = NULL;
	char	*pgit = NULL;
	int		env = 0; // 0x1 ACPLT_HOME; 0x2 ACPLT_GIT

	if (!GetCurrentDir(cCurrentPath, sizeof(cCurrentPath))){
		return 1;
    }
	cCurrentPath[sizeof(cCurrentPath) - 1] = '\0'; /* not really required */

	/* Explore enviroment */
	if(getenv(ACPLT_HOME_ENVPATH) != NULL){
		penv = getenv(ACPLT_HOME_ENVPATH);
		compatiblePath(penv);
		if(stat(penv, &st) == 0)
			env = 1;
	}

	if(getenv(ACPLT_GIT_ENVPATH) != NULL){
		pgit = getenv(ACPLT_GIT_ENVPATH);
		compatiblePath(pgit);
		if(stat(pgit, &st) == 0)
			strcpy(gitModelPath, pgit);
	}

	//TRYING TO FIND THE LIBRARY
	//1ST TRY: try to follow the ACPLT_HOME environment variable: take $ACPLT_HOME/dev
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

	//2NDST TRY: try to follow the ACPLT_GIT environment variable
	if(strlen(libPath)==0 && strlen(gitModelPath)>0){
		searchGit(help, gitModelPath, libname);
		if(strlen(help)>0){
			sprintf(libPath, "%s/%s/%s", gitModelPath, help, libname);
			compatiblePath(libPath);
			if(stat(libPath, &st) == 0){
				*newDirStructure = 1;
				sprintf(devModelPath, "%s/dev/", penv);
				sprintf(devBinPath, "%s/addonlibs/", penv);
				sprintf(sysModelPath, "%s/system/sysdevbase/", penv);
				sprintf(sysBinPath, "%s/system/sysbin/", penv);
			} else {
				// searchGit has its own checks so this schould not happen
				libPath = "";
			}
		}
	}

	//3RD TRY: ARE WE IN libPath/build/platform/ ?
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

	//4TH TRY: try to follow the ACPLT_HOME environment variable (older structure): take $ACPLT_HOME/user
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

	//5TH TRY: search in current working dir
	if(strlen(libPath)==0){
		sprintf(help, "%s/%s/model/%s.ovm", cCurrentPath, libname, libname);
		compatiblePath(help);
		if(stat(help, &st) == 0){
			sprintf(libPath, "%s/%s", cCurrentPath, libname);
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
		if(strlen(gitModelPath)>0)
			fprintf(stderr,"GitModel %s\n", gitModelPath);
		else
			fprintf(stderr,"GitModel not set or invalid\n");
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

void makefilePath(char* string){
	char*	ph;
	ph = string;
	while(ph && (*ph)) {
		if(*ph == '\\')
			*ph = '/';
		ph++;
	}
}


void searchGit(char* path, const char* gitModelPath, const char* curlib){
	char	help[512] = "";
	char	tmpPath[512] = "";
	struct stat st;

	path[0] = '\0'; // set path to empty string

	strcpy(help, gitModelPath);
	compatiblePath(help);

	if( stat(gitModelPath, &st) || !S_ISDIR(st.st_mode))
		return;

	if(help[strlen(help)-1] == '\\' || help[strlen(help)-1] == '/')
		help[strlen(help)-1] = '\0';

	searchGit_worker(path, help, tmpPath, curlib, 0);
}

void searchGit_worker(char* path, const char* gitModelPath, char* relPath, const char* curlib, int depth){

	char		help[512] = "";
	const char*	blacklist[3] = { "build", "doc", "tools"};
	char**		dirList = NULL;

	struct dirent*	dp;
	DIR*			dir;
	struct stat 	st;

	unsigned int count = 0;
	unsigned int pos = 0;
	int i = 0;

	sprintf(help, "%s/%s", gitModelPath, relPath);
	compatiblePath(help);

	dir = opendir(help);

	while((dp = readdir(dir))){
		count++;
	}

	dirList = calloc(count, sizeof(char*));
	if(!dirList)
		goto cleanup;

	rewinddir(dir);

	while((dp = readdir(dir))){

		if(dp->d_name[0]=='.')
			continue;

		for(i = 0; i<sizeof(blacklist)/sizeof(char*); i++){ // skip unnecessary entries
			if(strcmp(blacklist[i], dp->d_name)==0)
				continue;
		}

		sprintf(help, "%s/%s/%s", gitModelPath, relPath, dp->d_name);
		stat(help, &st);
		if(!S_ISDIR(st.st_mode))
			continue;

		dirList[pos] = malloc(strlen(dp->d_name)+1);
		strcpy(dirList[pos], dp->d_name);
		pos++;
	}

	count = pos;
	qsort(dirList, count, sizeof(char*), sortCompare);

	for(pos = 0; pos<count; pos++){

		if(!dirList[pos])
			continue;

		if(strcmp(curlib, dirList[pos])==0){

			// check if library directory
			sprintf(help, "%s/%s/%s/model/%s.ovm", gitModelPath, relPath, curlib, curlib);

			compatiblePath(help);

			if(fileExists(help)){ // we found our library
				strcpy(path, relPath);

				compatiblePath(path);
				goto cleanup;
			}
		}
		if(depth < MAKMAK_MAX_RECURSION_DEPTH){ // if we reach this statement we have not found a suitable directory
			if(depth>0)
				sprintf(help, "%s/%s", relPath, dirList[pos]);
			else
				strcpy(help, dirList[pos]);

			compatiblePath(help);
			searchGit_worker(path, gitModelPath, help, curlib, depth+1);
			if(strlen(path)>0){
				goto cleanup;
			}
		}
	}

	cleanup:
	if(dirList){
		for(i=0; i < count; i++){
			free(dirList[i]);
		}
		free(dirList);
	}
	closedir(dir);
	return;



}

/*	----------------------------------------------------------------------	*/
/*
 *   Search base libraries
 */
/***

    Searching for lib dependencies located in recursively, first in devModelPath, then in sysModelPath, results are saved in arrays devLibs and sysLibs with counters

***/
void makmak_searchbaselibs(const char *lib, const char *devModelPath, const char* gitModelPath, const char *sysModelPath,
		char **devLibs, int* numDevLibs, char** gitLibs, char** gitRelPath, int* numGitLibs, char **sysLibs, int* numSysLibs) {
	char   originallib[512];
	strcpy(originallib, lib);
	makmak_searchbaselibs_worker(originallib, lib, devModelPath, gitModelPath, sysModelPath,
			devLibs, numDevLibs, gitLibs, gitRelPath, numGitLibs, sysLibs, numSysLibs);
}

void makmak_searchbaselibs_worker(const char *originallib, const char *curlib,
		const char *devModelPath, const char* gitModelPath, const char *sysModelPath,
		char **devLibs, int* numDevLibs, char** gitLibs, char** gitRelPath, int* numGitLibs, char **sysLibs, int* numSysLibs) {
	/* Local variables */
	char   help[512];
	char   gitPath[512] = "";
	FILE   *fd;
	char   *incFile;
	int    incFound;
	char   *ph;
	int    i;
	int    location = 0; // 0: dev; 1: sysbase; 2: git
	FILE   *fdreadtest;

	/* OV library? -- already included */
	if( !strcmp(curlib, "ov") ) {
		return;
	}

	/* if no system dir is set */
	//if(location == 0 && strlen(sysModelPath)==0){
	//	return;
	//}

	/* Exclude fb.dll from standard includes
	if( !strcmp(lib, "fb") ) {
		return;
	}
	*/

	/* prevent stack overflow */
	if(*numDevLibs >= MAX_INCLUDED_FILES || *numSysLibs >= MAX_INCLUDED_FILES || *numGitLibs >= MAX_INCLUDED_FILES) {
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
	/* inside of gitLibs */
	for(i=0; i<*numGitLibs; i++) {
		if( !strcmp(curlib, gitLibs[i]+(strlen(gitLibs[i])-1-strlen(curlib))) ) { // check for libname at end of gitLib
			return;
		}
	}

	//iterate at most three times for locations dev sys and git
	while(location < 3){
		/* locate library model */
		switch(location){
		case MAKMAK_LOCATION_DEV:
			sprintf(help, "%s/%s/model/%s.ovm", devModelPath, curlib, curlib);
			break;
		case MAKMAK_LOCATION_SYS:
			sprintf(help, "%s/%s/model/%s.ovm", sysModelPath, curlib, curlib);
			break;
		case MAKMAK_LOCATION_GIT:
			searchGit(help, gitModelPath, curlib);
			if(strlen(help)>0){
				strcpy(gitPath, help);
				sprintf(help, "%s/%s/%s/model/%s.ovm", gitModelPath, gitPath, curlib, curlib);
			}

			break;
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
						/** recursive deepening - search for ph **/
						makmak_searchbaselibs_worker(originallib, ph, devModelPath, gitModelPath, sysModelPath,
								devLibs, numDevLibs, gitLibs, gitRelPath, numGitLibs, sysLibs, numSysLibs);
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
			switch(location){
			case MAKMAK_LOCATION_DEV:
				devLibs[(*numDevLibs)++] = ph;
				break;
			case MAKMAK_LOCATION_SYS:
				sysLibs[(*numSysLibs)++] = ph;
				break;
			case MAKMAK_LOCATION_GIT:
				gitLibs[*numGitLibs] = ph;
				ph = malloc(strlen(gitPath)+1);
				if(!ph){
					fprintf(stderr, "Out of memory\n");
					exit(0);
				}
				makefilePath(gitPath);
				strcpy(ph, gitPath);
				gitRelPath[*numGitLibs] = ph;
				(*numGitLibs)++;
				break;
			default:
				fprintf(stderr, "invalid location %i\n", location);
				free(ph);
				return;
			}
			//jump out of the while queue
			break;
		}else{
			/* make sure we have no self-dependency */
			//if(0 == strcmp(curlib, originallib)) {
			//	return;
			//}

			/* model file not found and it was the second search pass */
			if(location == 2){
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
				/* inside of gitLibs */
				for(i=0; i<*numGitLibs; i++) {
					if( !strcmp(curlib, gitLibs[i]) ) {
						return;
					}
				}
				fprintf(stderr, "Error: .ovm model of included library %s could not be located\n", curlib);
				exit(0);
			}
		}
		location++;
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

