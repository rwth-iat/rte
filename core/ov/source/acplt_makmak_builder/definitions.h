

/* needed to check whether a dir exists */
#include <sys/stat.h>

/* this one is needed to use mkdir in windows */
#if OV_SYSTEM_NT
#include <direct.h>
#endif

//#include "fb_namedef.h"
#define IFBS_HOME_ENVPATH		"IFBS_HOME"
#define ACPLT_HOME_ENVPATH		"ACPLT_HOME"
#define ACPLT_GIT_ENVPATH		"ACPLT_GIT"

// defines for search location order in makmak
#define MAKMAK_LOCATION_DEV		0
#define MAKMAK_LOCATION_SYS		1
#define MAKMAK_LOCATION_GIT		2

#define MAKMAK_MAX_RECURSION_DEPTH	6

#define MAX_INCLUDED_FILES 255


//global variables
struct stat st;
