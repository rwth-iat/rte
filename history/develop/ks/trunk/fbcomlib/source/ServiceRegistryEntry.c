#ifndef OV_COMPILE_LIBRARY_fbcomlib
#define OV_COMPILE_LIBRARY_fbcomlib
#endif

#include "fbcomlib.h"
#include "msgconfig.h"


OV_DLLFNCEXPORT OV_STRING fbcomlib_ServiceRegistryEntry_inboxPath_get(
    OV_INSTPTR_fbcomlib_ServiceRegistryEntry          pobj
) {
    return pobj->v_inboxPath;
}

OV_DLLFNCEXPORT OV_RESULT fbcomlib_ServiceRegistryEntry_inboxPath_set(
    OV_INSTPTR_fbcomlib_ServiceRegistryEntry          pobj,
    const OV_STRING  value
) {
    return ov_string_setvalue(&pobj->v_inboxPath,value);
}


OV_DLLFNCEXPORT OV_RESULT fbcomlib_ServiceRegistryEntry_constructor(
	OV_INSTPTR_ov_object 	pobj
) {
    /*    
    *   local variables
    */
	//OV_INSTPTR_fbcomlib_ServiceRegistryEntry this = Ov_StaticPtrCast(fbcomlib_ServiceRegistryEntry, pobj);
	//OV_RESULT result = fb_functionblock_constructor(pobj);


    /* do what the base class does first */


    //if(Ov_Fail(result))
      //  return result;
			//no fb functionblock any longer
			//this->v_iexreq = TRUE;
			//this->v_cyctime = MSGCYCTIME;

    /* do what */

    return OV_ERR_OK;
}

