
/******************************************************************************
*
*   FILE
*   ----
*   variable.c
*
*   History
*   -------
*   2011-06-10   File created
*
*******************************************************************************
*
*   This file is generated by the 'fb_builder' command
*
******************************************************************************/


#ifndef OV_COMPILE_LIBRARY_fb
#define OV_COMPILE_LIBRARY_fb
#endif


#include "fb.h"
#include "libov/ov_macros.h"
#include "fb_namedef.h"


OV_DLLFNCEXPORT OV_RESULT fb_port_flags_set(
    OV_INSTPTR_fb_port      pvar,
    const OV_UINT           value
) {
	OV_INSTPTR_fb_connection	pconn;

    OV_UINT  iFlag = 256;   /* SetNewFlag(iFlag, 'i') */
    OV_UINT  oFlag = 16384; /* SetNewFlag(oFlag, 'o') */
    
    if(pvar->v_flags == value) {
        return OV_ERR_OK;
    }
    if( (value != iFlag) && (value != oFlag) ) {
        return OV_ERR_BADVALUE;
    }
    
    /* Liegen Verbindungen am Port? */
	pconn = Ov_GetFirstChild(fb_inputconnections, pvar);
    if(pconn) {
        return OV_ERR_NOACCESS;
    }
    pconn = Ov_GetFirstChild(fb_outputconnections, pvar);
    if(pconn) {
        return OV_ERR_NOACCESS;
    }

    pvar->v_flags = value;
    return OV_ERR_OK;
}


OV_DLLFNCEXPORT OV_RESULT fb_port_setvar(
    OV_INSTPTR_ov_object  pobj,
    const OV_ELEMENT     *pelem,
    const OV_ANY         *pvarcurrprops
) {
	OV_INSTPTR_fb_functionchart pfc;
	OV_RESULT                   result;
	OV_BOOL                     setExtReq = FALSE;

	switch(pelem->elemtype) {
		case OV_ET_VARIABLE:
		
	    	/*
    		*	Check the variable type
		    */
		    if( !fb_vartype_implemented(pelem->elemunion.pvar->v_vartype) ) {
                return OV_ERR_BADTYPE;
			}
            
    		/* Compare values and set if changed */
    		result = fb_set_varvalue(pobj, pelem, pvarcurrprops, &setExtReq);
    		if(result != OV_ERR_OK) {
                return result;
            }
            if(setExtReq) {
                pfc = Ov_GetParent(fb_variables, (OV_INSTPTR_fb_port)pobj);
                if (pfc) {
                    pfc->v_eexreq = TRUE;
                }
            }
			return OV_ERR_OK;
		default:
			break;
	}
	return OV_ERR_BADPARAM;
}

OV_DLLFNCEXPORT OV_RESULT fb_port_constructor(
		OV_INSTPTR_ov_object 	pobj
) {
	/*
	 *   local variables
	 */
	OV_INSTPTR_fb_port pinst = Ov_StaticPtrCast(fb_port, pobj);
	OV_INSTPTR_ov_object pParent = Ov_PtrUpCast(ov_object, Ov_GetParent(ov_containment, pobj));
	OV_RESULT    result;

	/* do what the base class does first */
	result = fb_object_constructor(pobj);
	if(Ov_Fail(result)){
		return result;
	}

	if(!Ov_CanCastTo(fb_functionchart, pParent)){
		return OV_ERR_BADPLACEMENT;
	}

	return OV_ERR_OK;
}