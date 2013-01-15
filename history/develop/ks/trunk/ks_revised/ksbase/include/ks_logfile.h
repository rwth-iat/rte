/** @file 
  * ks Logging capability.
 * Currently, this is a wrapper around the ov_logfile logging facility or normal printf.
 * As defined by precompiler it might ignore to print anything, which 
 * results in an performance boost
 * 
*/

#ifdef OV
#include "libov/ov_ov.h"
#endif
/**
*	Print info to logfile
*/
void ks_logfile_info(
	const char*	format,
	...
);

/**
*	Print debug info to logfile
*/
void ks_logfile_debug(
	const char*	format,
	...
);

/**
*	Print warning to logfile
*/
void ks_logfile_warning(
	const char*	format,
	...
);

/**
*	Print error to logfile
*/
void ks_logfile_error(
	const char*	format,
	...
);
