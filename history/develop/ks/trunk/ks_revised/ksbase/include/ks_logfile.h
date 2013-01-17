/** @file 
  * ks Logging capability.
 * Currently, this is a wrapper around the ov_logfile logging facility or normal printf.
 * As defined by precompiler it might ignore to print anything, which 
 * results in an performance boost
 * 
*/

#if OV_SYSTEM_UNIX
#define OV_DLLFNCEXPORT
#endif

#if OV_SYSTEM_NT
#if OV_COMPILER_MSVC
#define OV_DLLFNCEXPORT 		_declspec(dllexport)
#else
#if OV_COMPILER_BORLAND || OV_COMPILER_CYGWIN
#define OV_DLLFNCEXPORT 		__declspec(dllexport)
#else
#define OV_DLLFNCEXPORT
#endif
#endif
#endif

#if OV_SYSTEM_OPENVMS
#define OV_DLLFNCEXPORT
#endif

#if OV_SYSTEM_RMOS
#define OV_DLLFNCEXPORT
#endif

#if OV_SYSTEM_MC164
#define OV_DLLFNCEXPORT
#endif


/**
*	Print info to logfile
*/
OV_DLLFNCEXPORT void ks_logfile_info(
	const char*	format,
	...
);

/**
*	Print debug info to logfile
*/
OV_DLLFNCEXPORT void ks_logfile_debug(
	const char*	format,
	...
);

/**
*	Print warning to logfile
*/
OV_DLLFNCEXPORT void ks_logfile_warning(
	const char*	format,
	...
);

/**
*	Print error to logfile
*/
OV_DLLFNCEXPORT void ks_logfile_error(
	const char*	format,
	...
);
