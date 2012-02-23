/** @file 
  * ksservtcp Logging capability.
 * Currently, this is a wrapper around the ov_logfile logging facility. 
 * As defined by precompiler it might ignore to print anything, which 
 * results in an performance boost
 * 
 * This is a intermediate solution until Log4ACPLT exists,
 * which will use this to create Log-Message objects inside the OV Server
*/
#include "libov/ov_ov.h"
#include "libov/ov_logfile.h"
/**
*	Print info to logfile
*/
OV_DLLFNCEXPORT void ksserv_logfile_info(
	const OV_STRING	format,
	...
);

/**
*	Print debug info to logfile
*/
OV_DLLFNCEXPORT void ksserv_logfile_debug(
	const OV_STRING	format,
	...
);

/**
*	Print warning to logfile
*/
OV_DLLFNCEXPORT void ksserv_logfile_warning(
	const OV_STRING	format,
	...
);

/**
*	Print error to logfile
*/
OV_DLLFNCEXPORT void ksserv_logfile_error(
	const OV_STRING	format,
	...
);
