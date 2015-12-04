

#ifdef OV_SYSTEM_NT
    #include <direct.h>
    #define GetCurrentDir _getcwd
#else
    #include <unistd.h>
    #define GetCurrentDir getcwd
#endif

int locateLibrary(const char* libname, char* libPath, char *devModelPath, char* devBinPath, char* sysModelPath, char* sysBinPath, int* newDirStructure);
void compatiblePath(char* string);
void makmak_searchbaselibs(const char *lib, const char *devModelPath, const char *sysModelPath, char **devLibs, int* numDevLibs,
char **sysLibs, int* numSysLibs);
void makmak_searchbaselibs_worker(const char *originallib, const char *curlib, const char *devModelPath, const char *sysModelPath, char **devLibs, int* numDevLibs, char **sysLibs, int* numSysLibs);
int fileExists(char* filename);
int acplt_mkdir(char* dir);
