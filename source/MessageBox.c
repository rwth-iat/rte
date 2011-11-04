#ifndef OV_COMPILE_LIBRARY_fbcomlib
#define OV_COMPILE_LIBRARY_fbcomlib
#endif


#include "fbcomlib.h"
#include "msgconfig.h"


OV_DLLFNCEXPORT OV_RESULT fbcomlib_MessageBox_constructor(
	OV_INSTPTR_ov_object 	pobj
) {
    /*    
    *   local variables
    */
	OV_INSTPTR_fbcomlib_MessageBox this = Ov_StaticPtrCast(fbcomlib_MessageBox, pobj);
	OV_RESULT result = fb_functionblock_constructor(pobj);


    /* do what the base class does first */


    if(Ov_Fail(result))
        return result;
			this->v_iexreq = TRUE;
			this->v_cyctime = MSGCYCTIME;
			this->v_actimode = FALSE;

    /* do what */

    return OV_ERR_OK;
}

OV_DLLFNCEXPORT void fbcomlib_MessageBox_typemethod(
	OV_INSTPTR_fb_functionblock	pfb,
	OV_TIME						*pltc
) {
    /*    
    *   local variables
    */
	//OV_INSTPTR_fbcomlib_MessageBox this = Ov_StaticPtrCast(fbcomlib_MessageBox, pfb);

	return;
}
