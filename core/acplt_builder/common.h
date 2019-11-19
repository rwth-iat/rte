

#if OV_SYSTEM_NT
    #include <direct.h>
    #define GetCurrentDir _getcwd
#else
    #include <unistd.h>
    #define GetCurrentDir getcwd
#endif

#define OV_LIBRARY_PATH_ENVVAR      "OV_LIBRARY_PATH"
#define MAX_LIBNAME                 256
#define MAX_SEARCHPATHS             256
#define MAX_PATH_LENGTH             4096

/**
 * Find the relevant library include paths for ov_codegen's parser by searching included OVM files recursively.
 *
 * The function uses malloc() to allocate memory for the libNames and libPaths strings. It should be freed when not
 * needed anymore.
 *
 * @param lib The name of the processed base library
 * @param LibsNames Array to fill in libraries' names (must be at least of size MAX_INCLUDEPATHS)
 * @param LibsPaths Array to fill in libraries' model directory paths (must be at least of size MAX_INCLUDEPATHS)
 * @param numLibs Pointer to an int to be filled with number of found libraries
 * @param searchPaths The list of directories to search for OV libraries. The libraries must be placed as immediate
 *      subdirectories in one those search paths.
 * @param numSearchPaths Length of the `searchPaths` array
 */
void searchBaseLibs(const char *lib, char **LibsNames, char **LibsPaths, int* numLibs, const char *const *searchPaths,
                    const int numSearchPaths);

void compatiblePath(char* string);

int fileExists(const char* filename);
#define acplt_exists(file) (fileExists(file))
#define acplt_isFile(file) (fileExists(file)==1)
#define acplt_isDir(file) (fileExists(file)==2)

int acplt_mkdir(char* dir);

/**
 * A platform independent function to get the base path (directory path) of a file.
 *
 * Wraps dirname() on *NIX and _splitpath() on win32.
 *
 * @param dir Output buffer to write the directory path into. Must be at least `n` bytes long
 * @param path The input file path
 * @param n Maximum number of bytes to write to `dir`
 */
void getDirname(char *dir, const char *path, size_t n);

/**
 * A platform independent function to get a file's basename without extension from a path.
 *
 * Wraps basename() on *NIX and _splitpath() on win32.
 *
 * @param filename Output buffer to write the filename into. Must be at least `n` bytes long
 * @param path The input file path
 * @param n Maximum number of bytes to write to `filename`
 */
void getFilenameWoExt(char *filename, const char *path, size_t n);

/**
 * Generate array of library search paths from environment.
 *
 * The function reads the environment variable defined by OV_LIBRARY_PATH_ENVVAR and splits it into a list of paths.
 * Additionally it adds the parent directory of the processed library and the current working directory (and its parent)
 * to the list of searchPaths. It uses malloc() to allocate memory for all the strings. It should be freed when not
 * needed anymore.
 *
 * @param searchPaths Output array to fill with search paths
 * @param maxNumSearchPaths Maximum number of search paths to fill into searchPaths (should be the length of
 *      searchPaths)
 * @param currentLibPath Path to the directory of the processed library
 * @return The number of searchPaths filled into searchPaths
 */
size_t getSearchPaths(char **searchPaths, size_t maxNumSearchPaths, const char* currentLibPath);