/*****************************************
 *
 * When compiling runtimeserver with static libraries, the library_table has to be created
 * this is done here
 *
 * ov_runtimeserver has to be linke with -z,muldefs and --whole-archive
 */



#if OV_STATIC_LIBRARIES

#include "libov/ov_database.h"
#include "libov/ov_result.h"
#include "libov/ov_logfile.h"
#include "libov/ov_macros.h"
#include "ov_ksserver_stripped.h"
#include "ksserv.h"
#include "ksservtcp.h"
#include "ksservhttp.h"
#include "fb.h"
#include "iec61131stdfb.h"
#include "controlblocks.h"
#include "kbuslib.h"
#include "wagoFBKlib.h"


Ov_BeginStaticLibraryTable
Ov_DefineStaticLibrary(ksserv)
Ov_DefineStaticLibrary(ksservtcp)
Ov_DefineStaticLibrary(fb)
Ov_DefineStaticLibrary(iec61131stdfb)
Ov_DefineStaticLibrary(controlblocks)
Ov_EndStaticLibraryTable;

#endif
