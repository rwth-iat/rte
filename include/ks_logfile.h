/** @file 
  * ks Logging capability.
 * Currently, this is a wrapper around the ov_logfile logging facility or normal printf.
 * As defined by precompiler it might ignore to print anything, which 
 * results in an performance boost
 * 
*/

/*
 * The following defines are needed for exporting the logging functions on different systems / with different compilers
 */

/*
*	Plattforms
*	----------
*/
#ifdef __NT__
#define OV_SYSTEM_NT 1
#endif

/*
*	Compiler
*	--------
*/

#if defined(__CYGWIN32__) || defined(__CYGWIN__) || defined(__MINGW32__)
#define OV_COMPILER_CYGWIN 1
#endif

#ifndef OV_COMPILER_CYGWIN
#define OV_COMPILER_CYGWIN 0
#endif

#ifdef _MSC_VER
#define OV_COMPILER_MSVC _MSC_VER
#endif

#ifndef OV_COMPILER_MSVC
#define OV_COMPILER_MSVC 0
#endif

#ifdef __BORLANDC__
#define OV_COMPILER_BORLAND __BORLANDC__
#endif

#ifndef OV_COMPILER_BORLAND
#define OV_COMPILER_BORLAND 0
#endif

/*
 * DLLEXPORT
 */
#if OV_SYSTEM_NT
#if OV_COMPILER_MSVC
#define DLLFNCEXPORT 		_declspec(dllexport)
#else
#if OV_COMPILER_BORLAND || OV_COMPILER_CYGWIN
#define DLLFNCEXPORT 		__declspec(dllexport)
#else
#define DLLFNCEXPORT
#endif
#endif
#else
#define DLLFNCEXPORT
#endif

/**
 * Macros for calling logging functions
 * if logging is disabled the functions do not get called
 * Usage: KS_logfile_xxx((parameters));
 */

#if LOG_KS || LOG_KS_DEBUG
#define KS_logfile_debug(_x_) ks_logfile_debug _x_
#else
#define KS_logfile_debug(_x_) ;
#endif

#if LOG_KS || LOG_KS_INFO
#define KS_logfile_info(_x_) ks_logfile_info _x_
#else
#define KS_logfile_info(_x_) ;
#endif

#if LOG_KS || LOG_KS_WARNING
#define KS_logfile_warning(_x_) ks_logfile_warning _x_
#else
#define KS_logfile_warning(_x_) ;
#endif

#if LOG_KS || LOG_KS_ERROR
#define KS_logfile_error(_x_) ks_logfile_error _x_
#else
#define KS_logfile_error(_x_) ;
#endif

/**
*	Print info to logfile
*/
DLLFNCEXPORT void ks_logfile_info(
	const char*	format,
	...
);

/**
*	Print debug info to logfile
*/
DLLFNCEXPORT void ks_logfile_debug(
	const char*	format,
	...
);

/**
*	Print warning to logfile
*/
DLLFNCEXPORT void ks_logfile_warning(
	const char*	format,
	...
);

/**
*	Print error to logfile
*/
DLLFNCEXPORT void ks_logfile_error(
	const char*	format,
	...
);
