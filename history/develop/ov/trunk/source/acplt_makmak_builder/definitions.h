

/* needed to check whether a dir exists */
#include <sys/stat.h>

/* this one is needed to use mkdir in windows */
#ifdef OV_SYSTEM_NT
#include <direct.h>
#endif

//#include "fb_namedef.h"
#define  IFBS_HOME_ENVPATH      "IFBS_HOME"
#define  ACPLT_HOME_ENVPATH      "ACPLT_HOME"

#define MAX_INCLUDED_FILES 255


//global variables
struct stat st;
