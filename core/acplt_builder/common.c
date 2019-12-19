
#define _GNU_SOURCE

#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

#if OV_SYSTEM_NT
#include <direct.h>
#include <windows.h>
#else
#include <dirent.h>
#include <sys/stat.h>
#include <libgen.h>
#include <limits.h>
#endif

#include "ov_codegen.h"
#include "common.h"


/**

	Handling the path location business and consistency checks

**/

/*	----------------------------------------------------------------------	*/ 
/*
 *  Update slashes to match windows plattform
 */
void compatiblePath(char* string){
#if OV_SYSTEM_NT
	char* ph, *ph2;

	if(!string)
		return;

	ph = string;
	// convert cygwin path to windows path
	if(!strncmp(string, "/cygdrive/", 10)){
		ph2 = ph + 10;
		*ph = *ph2;
		ph++;
		*ph = ':';
		ph++;
		ph2++;
		while(*ph2){
			if(*ph2 == '/')
				*ph = '\\';
			else
				*ph = *ph2;
			ph++;
			ph2++;
		}
		*ph = 0;
	} else {
		while(ph && (*ph)) {
			if(*ph == '/') {
				*ph = '\\';
			}
			ph++;
		}
	}
#endif
}

/*	----------------------------------------------------------------------	*/
/*
 *   Search base libraries
 */

static void searchSingleLibrary(char *OVMPath, const char *name, const char *const *searchPaths, int numSearchPaths) {
    // Iterate searchPaths until we find the model file at the expected location within the searchPath
    for(int i=0; i < numSearchPaths; ++i) {
        char path[MAX_PATH_LENGTH];
        snprintf(path, MAX_PATH_LENGTH, "%s/%s/model/%s.ovm", searchPaths[i], name, name);
        compatiblePath(path);

        // If the model file exists at the expected location, copy the path to the destination pointer and return
        if (acplt_isFile(path)) {
            strcpy(OVMPath, path);
            return;
        }
    }
}

static void searchBaseLibsRecursive(const char *lib, char **libNames, char **libPaths, int *numLibs,
        const char *const *searchPaths, const int numSearchPaths) {
	/* check if we already found and processed this library */
	for(int i=0; i<*numLibs; i++) {
		if( !strcmp(lib, libNames[i]) ) {
			return;
		}
	}

    if(*numLibs >= MAX_INCLUDEPATHS) {
        fprintf(stderr, "To many included files.\n");
        exit(0);
    }

	// TODO check for circular dependencies using a stack of library names

	// Actually search the library
    char model_file[MAX_PATH_LENGTH] = "";
    searchSingleLibrary(model_file, lib, searchPaths, numSearchPaths);

    if (!model_file[0]) {
        fprintf(stderr, "Error: .ovm model of included library %s could not be located\n", lib);
        exit(1);
    }

    /* Try to open model file and parse the includes */
    FILE *fd = fopen(model_file, "r");
    if (!fd) {
        fprintf(stderr, "Error: Could not open .ovm model file %s\n", model_file);
        exit(1);
    }

    // First, let's add the library's model directory to the libPaths.
    // We just cut the model file's base path from `model_file`.
    char *newLibPath = (char*)malloc(strlen(model_file)+1);
    getDirname(newLibPath, model_file, strlen(model_file));
    libPaths[*numLibs] = newLibPath;
    char *libName = (char*)malloc((strlen(lib)+1));
    strcpy(libName, lib);
    libNames[*numLibs] = libName;
    (*numLibs)++;

    // Now, read the file line by line with the maximum line length 1023 and check for #include "…" lines
    char line[1024];
    while( fgets(line, 1023, fd) ) {
        int incFound = 0;
        char *ph = strstr(line, "#include");
        /* locate # and 'include' in a line, # must come before 'include' */
        /* make sure the line really starts with "#include" to allow commenting includes out */
        if(ph && ph==line) {
            /* skip include */
            ph += 8;
            char *includedLibName = NULL;
            while(ph && *ph && !incFound) {
                switch(*ph) {
                case '<':
                case '"':
                    ph++;
                    if(!includedLibName) {
                        includedLibName = ph;
                    } else {
                        *ph = '\0';
                        incFound = 1;
                    }
                    break;
                case '.':
                    //remove the file extension
                    if (includedLibName) {
                        *ph = '\0';
                        incFound = 1;
                        break;
                    }
                    /* skip */
                case '>':
                case ' ':
                case '\t':
                default:
                    break;

                }

                ph++;
            }

            if(incFound) {
                // It is possible to have the variables in a separate OVM in the same directory (for example simblocks).
                // In this case, we don't need to search the library
                sprintf(model_file, "%s/%s.ovm", newLibPath, includedLibName);
                compatiblePath(model_file);
                FILE *fdreadtest = fopen(model_file, "r");
                if(fdreadtest != NULL){
                    fclose(fdreadtest);
                    continue;
                }

                /*
                 * recursive deepening - search for includedLibName
                 */
                searchBaseLibsRecursive(includedLibName, libNames, libPaths, numLibs, searchPaths, numSearchPaths);
            }
        }
    }

    /* Close file */
    fclose(fd);
}


/**
 * Searching for lib dependencies recursively
 */
void searchBaseLibs(const char *lib, char **LibsNames, char **LibsPaths, int* numLibs, const char *const *searchPaths,
        const int numSearchPaths) {
    // TODO use stack of library names to detect circular dependencies
    searchBaseLibsRecursive(lib, LibsNames, LibsPaths, numLibs, searchPaths, numSearchPaths);
}

/*	----------------------------------------------------------------------	*/
/*
 * Checks whether a file exists
 */
int fileExists(const char* filename){
#if OV_SYSTEM_NT
    DWORD attr = GetFileAttributes(filename);
	if(attr==INVALID_FILE_ATTRIBUTES)
		return 0;
	if(attr&FILE_ATTRIBUTE_DIRECTORY)
		return 2;
	return 1;
#else
    struct stat st;
    if(stat(filename, &st)!=0)
        return 0;
    if(S_ISDIR(st.st_mode))
        return 2;
    return 1;
#endif
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

void getDirname(char *dir, const char *path, size_t n) {
#if OV_SYSTEM_NT
    char drive[_MAX_DRIVE];
    char dirpath[_MAX_DIR];
    _splitpath(path, drive, dirpath, NULL, NULL);
    snprintf(dir, n,"%s%s", drive, dirpath);
#else
    char *pathClone = (char*)malloc(strlen(path)+1);
    strcpy(pathClone, path);
    strncpy(dir, dirname(pathClone), n);
    dir[n-1] = '\0'; // For security reasons
    free(pathClone);
#endif
}


void getFilenameWoExt(char *filename, const char *path, size_t n) {
#if OV_SYSTEM_NT
    _splitpath_s(path, NULL, 0, NULL, 0, filename, n, NULL, 0);
#else
    // Clone path and get basename
    char *pathClone = (char*)malloc(strlen(path)+1);
    strcpy(pathClone, path);
    char *base = basename(pathClone);
    // Remove extension (replace last '.' with '\0')
    char *extPos = strrchr(base, '.');
    *extPos = '\0';
    // Return filename and clean up
    strncpy(filename, base, n);
    filename[n-1] = '\0'; // For security reasons
    free(pathClone);
#endif
}

size_t getSearchPaths(const char **searchPaths, size_t maxNumSearchPaths, const char* currentLibPath) {
    size_t numSearchPaths = 0;

    // First, add generic search paths
    const char *genericSearchPaths[] = {currentLibPath, ".", ".."};
    for (int i=0; i<3 && numSearchPaths<maxNumSearchPaths; i++) {
#if OV_SYSTEM_NT
        // TODO normalize path
        searchPaths[numSearchPaths] = genericSearchPaths[i];
#else
        char *path = realpath(genericSearchPaths[i], NULL);
        if (!path) {
            fprintf(stderr, "Warning: Could not resolve library search path %s\n", genericSearchPaths[i]);
            continue;
        }
        searchPaths[numSearchPaths] = path;
#endif
        numSearchPaths++;
    }

    // Now, process the OV_LIBRARY_PATH_ENVVAR environment variable
#if OV_SYSTEM_NT
    char delimiter = ';';
#else
    char delimiter = ':';
#endif

    const char* envPath = getenv(OV_LIBRARY_PATH_ENVVAR);
    if (!envPath) {
        return numSearchPaths;
    }

    const char* p = envPath;
    while (1) {
        // Search next delmiter (or end of string)
        char* pEnd = strchr(p, delimiter);
        if (!pEnd) {
            pEnd = strchr(p, '\0');
        }

        // Skip empty paths (we already added "." as generic path above)
        size_t len = pEnd - p;
        if (!len) {
            p = pEnd+1;
            continue;
        }

        // Clone single path and process path
        char *rawPath = (char*)malloc(len+1);
        strncpy(rawPath, p, len);
        rawPath[len] = '\0';
#if OV_SYSTEM_NT
        // TODO normalize path
        searchPaths[numSearchPaths] = rawPath;
#else
        char *path = realpath(rawPath, NULL);
        if (!path) {
            fprintf(stderr, "Warning: Could not resolve library search path %s\n", rawPath);
            free(rawPath);
            continue;
        }
        searchPaths[numSearchPaths] = path;
        free(rawPath);
#endif
        numSearchPaths++;

        // Break if we reached the end of the environment string (pEnd points to null terminator)
        if (!*pEnd) {
            break;
        }
        p = pEnd+1;
    }

    return numSearchPaths;
}