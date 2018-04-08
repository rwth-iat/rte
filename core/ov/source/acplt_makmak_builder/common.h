

#ifdef OV_SYSTEM_NT
    #include <direct.h>
    #define GetCurrentDir _getcwd
#else
    #include <unistd.h>
    #define GetCurrentDir getcwd
#endif

void searchGit_worker(char* path, const char* gitModelPath, char* relPath, const char* curlib, int depth);
void searchGit(char* path, const char* gitModelPath, const char* curlib);

int locateLibrary(const char* libname, char* libPath, char *devModelPath, char* devBinPath,
		char* gitModelPath, char* sysModelPath, char* sysBinPath, int* newDirStructure);
void compatiblePath(char* string);
void makefilePath(char* string);
void makmak_searchbaselibs(const char *lib, const char *devModelPath, const char* gitModelPath, const char *sysModelPath,
		char **devLibs, int* numDevLibs, char** gitLibs, char** gitRelPath, int* numGitLibs, char **sysLibs, int* numSysLibs);
void makmak_searchbaselibs_worker(const char *originallib, const char *curlib,
		const char *devModelPath, const char* gitModelPath, const char *sysModelPath,
		char **devLibs, int* numDevLibs, char** gitLibs, char** gitRelPath, int* numGitLibs, char **sysLibs, int* numSysLibs);
int fileExists(char* filename);
int acplt_mkdir(char* dir);
