#ifndef OV_COMPILE_LIBRARY_fbcomlib
#define OV_COMPILE_LIBRARY_fbcomlib
#endif

#include "fbcomlib.h"
#include "libov/ov_result.h"

#ifdef ov_library_open_fbcomlib
#undef ov_library_open_fbcomlib
#endif

/*
*       This function will be called, after the library is loaded.
*       ----------------------------------------------------------
*/
OV_RESULT ov_library_setglobalvars_fbcomlib_new(void) {
	return OV_ERR_OK;
}



/*
*       Replace the "setglobalvars" function of a library with this
*       previous one, which additionally creates instances.
*       ----------------------------------------------------------------------
*/
OV_DLLFNCEXPORT OV_LIBRARY_DEF *ov_library_open_fbcomlib(void) {
	/*
	*       local variables
	*/
	static OV_LIBRARY_DEF OV_LIBRARY_DEF_fbcomlib_new;
	/*
	*       replace the "setglobalvars" function created by the code generator
	*       with a new one.
	*/
	OV_LIBRARY_DEF_fbcomlib_new = *ov_library_open_fbcomlib_old();
	OV_LIBRARY_DEF_fbcomlib_new.setglobalvarsfnc = ov_library_setglobalvars_fbcomlib_new;
	return &OV_LIBRARY_DEF_fbcomlib_new;
}

