/*****************************************
 *
 * When compiling runtimeserver with static libraries, the library_table has to be created
 * this is done here
 *
 * ov_runtimeserver has to be linked with "-Wl,--whole-archive -Wl,-z,muldefs"
 */

#include "libov/ov_config.h"

#if OV_STATIC_LIBRARIES

#include "libov/ov_database.h"
#include "libov/ov_result.h"
#include "libov/ov_logfile.h"
#include "libov/ov_macros.h"
#include "ov_ksserver_stripped.h"
#include "ksbase.h"
#include "TCPbind.h"
#include "ksxdr.h"
#include "kshttp.h"
#include "fb.h"

Ov_BeginStaticLibraryTable
Ov_DefineStaticLibrary(ksbase)
Ov_DefineStaticLibrary(TCPbind)
Ov_DefineStaticLibrary(ksxdr)
Ov_DefineStaticLibrary(kshttp)
Ov_DefineStaticLibrary(fb)
Ov_EndStaticLibraryTable;

#endif
