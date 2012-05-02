#ifndef OV_COMPILE_LIBRARY_msgsys
#define OV_COMPILE_LIBRARY_msgsys
#endif


#include "msgsys.h"
#include "msgconfig.h"


OV_DLLFNCEXPORT OV_RESULT msgsys_ServiceRegistry_constructor(
	OV_INSTPTR_ov_object 	pobj
) {
    /*    
    *   local variables
    */
	//OV_INSTPTR_msgsys_ServiceRegistry this = Ov_StaticPtrCast(msgsys_ServiceRegistry, pobj);
	//OV_RESULT result = fb_functionblock_constructor(pobj);


    /* do what the base class does first */


    //if(Ov_Fail(result))
        //return result;
			//no fb functionblock any longer
			//this->v_iexreq = TRUE;
			//this->v_cyctime = MSGCYCTIME;
			//this->v_actimode = FALSE;

    /* do what */

    return OV_ERR_OK;
}
