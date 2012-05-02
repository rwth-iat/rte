#ifndef OV_COMPILE_LIBRARY_msgsys
#define OV_COMPILE_LIBRARY_msgsys
#endif


#include "msgsys.h"
#include "msgconfig.h"
#include "libov/ov_macros.h"


OV_DLLFNCEXPORT OV_RESULT msgsys_MessageBox_constructor(
	OV_INSTPTR_ov_object 	pobj
) {
    /*    
    *   local variables
    */
	OV_INSTPTR_msgsys_MessageBox this = Ov_StaticPtrCast(msgsys_MessageBox, pobj);
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

OV_DLLFNCEXPORT void msgsys_MessageBox_typemethod(
	OV_INSTPTR_fb_functionblock	pfb,
	OV_TIME						*pltc
) {
    /*    
    *   local variables
    */
	//OV_INSTPTR_msgsys_MessageBox this = Ov_StaticPtrCast(msgsys_MessageBox, pfb);

	return;
}

OV_DLLFNCEXPORT void msgsys_MessageBox_retMethod(
						   OV_INSTPTR_ov_object pobj,
						   OV_INSTPTR_ov_object message,
						   OV_STRING errorstring,
						   OV_INT errorcode
){

}

OV_DLLFNCEXPORT void msgsys_MessageBox_arrivalMethod(
						   OV_INSTPTR_msgsys_MessageBox mBox,
						   OV_INSTPTR_msgsys_Message message
){


	        //this is a workaround for calling the overwritten method in subclasses-instances
	        OV_VTBLPTR_msgsys_MessageBox pvtable = NULL;

	        Ov_GetVTablePtr(msgsys_MessageBox, pvtable,mBox);
	        pvtable->m_arrivalMethodImpl(mBox, message);
}

