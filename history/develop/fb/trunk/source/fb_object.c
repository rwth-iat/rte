/******************************************************************************
***                                                                         ***
***   iFBSpro   -   Funktionsbaustein-Model                                 ***
***   #####################################                                 ***
***                                                                         ***
***   L T S o f t                                                           ***
***   Agentur für Leittechnik Software GmbH                                 ***
***   Brabanterstr. 13                                                      ***
***   D-50171 Kerpen                                                        ***
***   Tel : 02237/92869-2                                                   ***
***   Fax : 02237/92869-9                                                   ***
***   e-Mail   : ltsoft@ltsoft.de                                           ***
***   Internet : http://www.ltsoft.de                                       ***
***                                                                         ***
***   -------------------------------------------------------------------   ***
***                                                                         ***
***   Implementierung des Funktionsbausteinsystems IFBSpro                  ***
***   RWTH,   Aachen                                                        ***
***   LTSoft, Kerpen                                                        ***
***                                                                         ***
*******************************************************************************
*                                                                             *
*   Datei                                                                     *
*   -----                                                                     *
*   fb_object.c                                                            *
*                                                                             *
*                                                                             *
*   Historie                                                                  *
*   --------                                                                  *
*   2012-06-04 Alexander Neugebauer: Erstellung, LTSoft GmbH, Kerpen          *
*                                                                             *
*   Beschreibung                                                              *
*   ------------                                                              *
*   Implementierung der FB-BasisKlasse                                        *
*                                                                             *
******************************************************************************/

#ifndef OV_COMPILE_LIBRARY_fb
#define OV_COMPILE_LIBRARY_fb
#endif

#include "fb.h"
#include "fb_namedef.h"
#include "fb_macros.h"
#include "libov/ov_macros.h"
#include "libov/ov_logfile.h"
#include "fb_log.h"

/*	----------------------------------------------------------------------	*/
/* Hilfs-FNC: Prueft, ob Var-Typ von FB unterstuetz ist */
OV_DLLFNCEXPORT int fb_vartype_implemented(OV_VAR_TYPE typ) {

    switch(typ) {
        case OV_VT_ANY:
        case OV_VT_BYTE:
        case OV_VT_BOOL:
        case OV_VT_INT:
        case OV_VT_UINT:
        case OV_VT_SINGLE:
        case OV_VT_DOUBLE:
        case OV_VT_STRING:
        case OV_VT_TIME:
        case OV_VT_TIME_SPAN:
        
        case OV_VT_BYTE_VEC:
        case OV_VT_BOOL_VEC:
        case OV_VT_INT_VEC:
        case OV_VT_UINT_VEC:
        case OV_VT_SINGLE_VEC:
        case OV_VT_DOUBLE_VEC:
        case OV_VT_STRING_VEC:
        case OV_VT_TIME_VEC:
        case OV_VT_TIME_SPAN_VEC:

        case OV_VT_BOOL_PV:
        case OV_VT_INT_PV:
        case OV_VT_UINT_PV:
        case OV_VT_SINGLE_PV:
        case OV_VT_DOUBLE_PV:
        case OV_VT_STRING_PV:
        case OV_VT_TIME_PV:
        case OV_VT_TIME_SPAN_PV:

        case OV_VT_BOOL_PV_VEC:
        case OV_VT_INT_PV_VEC:
        case OV_VT_UINT_PV_VEC:
        case OV_VT_SINGLE_PV_VEC:
        case OV_VT_DOUBLE_PV_VEC:
        case OV_VT_STRING_PV_VEC:
        case OV_VT_TIME_PV_VEC:
        case OV_VT_TIME_SPAN_PV_VEC:

            return 1;
        default:
            break;
	}
    return 0;
}

/*	----------------------------------------------------------------------	*/
/*
*	Get the flags for the functionblock object or its variables
*/
OV_DLLFNCEXPORT OV_UINT fb_object_getflags(
	OV_INSTPTR_ov_object 	pobj,
	const OV_ELEMENT		*pelem
) {
	return ov_object_getflags(pobj, pelem );
}

/*	----------------------------------------------------------------------	*/
/*
*	Get the access rights for the functionblock object or its variables
*/
OV_DLLFNCEXPORT OV_ACCESS fb_object_getaccess(
	OV_INSTPTR_ov_object		pobj,
	const OV_ELEMENT			*pelem,
	const OV_TICKET				*pticket
) {
	/*
	*	local variables
	*/
	OV_INSTPTR_fb_object	    pfobj = Ov_StaticPtrCast(fb_object, pobj);
	OV_INSTPTR_fb_connection	pconn;

	/*
	*	switch based on the element's type
	*/
	switch(pelem->elemtype) {
		case OV_ET_VARIABLE:
			if(pelem->elemunion.pvar->v_offset >= offsetof(OV_INST_ov_object,__classinfo)) {
			    /* Typ unterstuetzt? */
			    if( !fb_vartype_implemented(pelem->elemunion.pvar->v_vartype)) {
                    return OV_AC_NONE;
				}
		        /* Rest der Variablen setzen, nur wenn Input-Flags gesetzt */
		        if(IsFlagSet(pelem->elemunion.pvar->v_flags, 'p') ||
				   IsFlagSet(pelem->elemunion.pvar->v_flags, 'i')) {
					return OV_AC_READWRITE;
				}
				/* Nicht FB? */
        		if(pelem->elemunion.pvar->v_varprops & OV_VP_SETACCESSOR) {
        			return OV_AC_READWRITE;
        		}
				return OV_AC_READ;
			}
			break;
		case OV_ET_OBJECT:
			pconn = Ov_GetFirstChild(fb_inputconnections, pfobj);
            if(pconn) {
                return (OV_AC_READWRITE | OV_AC_RENAMEABLE | OV_AC_LINKABLE | OV_AC_UNLINKABLE);
            }
            pconn = Ov_GetFirstChild(fb_outputconnections, pfobj);
            if(pconn) {
                return (OV_AC_READWRITE | OV_AC_RENAMEABLE | OV_AC_LINKABLE | OV_AC_UNLINKABLE);
            }
            /* Keine Verbindungen am Objekt. Pruefe OV-Zugriffsrechte */
            break;
		default:
			break;
	}
	
	return ov_object_getaccess(pobj, pelem, pticket);
}

/*	----------------------------------------------------------------------	*/
/*
*	Trigger input get connections
*/
OV_DLLFNCEXPORT void fb_object_triggerInpGetConnections(
	OV_INSTPTR_fb_object	pfb
) {
	OV_INSTPTR_fb_connection				pconn;
	OV_VTBLPTR_fb_connection				pconnvtable;

    /* Logging */
    FbSvcLog_printexecitem((OV_INSTPTR_ov_object)pfb, "trigger input get connections");

    /* Ueber alle Input-Verbindungen */
	Ov_ForEachChild(fb_inputconnections, pfb, pconn) {
		if(!pconn->v_sourcetrig) {
			Ov_GetVTablePtr(fb_connection, pconnvtable, pconn);
      
			if(pconnvtable) {
			    pconnvtable->m_trigger(pconn);
			} else {
			    pconn->v_on = FALSE;
			    
			    /* Logging */
                FbSvcLog_printexecitem((OV_INSTPTR_ov_object)pconn, "ERROR: method table not found");
			    ov_logfile_error("Connection %s: method table not found.", pconn->v_identifier);
			}
		}
	}

    return;
}

/*	----------------------------------------------------------------------	*/
/*
*	Trigger output send connections
*/
OV_DLLFNCEXPORT void fb_object_triggerOutSendConnections(
	OV_INSTPTR_fb_object	pfb
) {
	OV_INSTPTR_fb_connection				pconn;
	OV_VTBLPTR_fb_connection				pconnvtable;

    /* Logging */
    FbSvcLog_printexecitem((OV_INSTPTR_ov_object)pfb, "trigger output send connections");

    /* Ueber alle Output-Verbindungen */
	Ov_ForEachChild(fb_outputconnections, pfb, pconn) {
		if(pconn->v_sourcetrig) {
			Ov_GetVTablePtr(fb_connection, pconnvtable, pconn);
    
			if(pconnvtable) {
			    pconnvtable->m_trigger(pconn);
			} else {
			    pconn->v_on = FALSE;
			    ov_logfile_error("Connection %s: method table not found.", 
			        pconn->v_identifier);
			}
		}
	}
	
    return;
}


/*	----------------------------------------------------------------------	*/

OV_DLLFNCEXPORT OV_RESULT fb_object_getvar(
	OV_INSTPTR_ov_object 	pobj, 
	const OV_ELEMENT		*pelem,
	OV_ANY              	*pvarcurrprops
) {
    /* Typ unterstuetzt? */
    if( !fb_vartype_implemented(pelem->elemunion.pvar->v_vartype) ) {
        return OV_ERR_NOTIMPLEMENTED;
    }

    return ov_object_getvar(pobj, pelem, pvarcurrprops);
}


/*	----------------------------------------------------------------------	*/

OV_DLLFNCEXPORT OV_RESULT fb_object_setvar(
	OV_INSTPTR_ov_object 		pobj,
	const OV_ELEMENT			*pelem,
	const OV_ANY            	*pvarcurrprops
) {
	OV_INSTPTR_fb_functionblock	pfb;
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
                pfb = Ov_DynamicPtrCast(fb_functionblock, pobj);
                if(pfb) {
                    pfb->v_eexreq = setExtReq;
                }
            }
			return OV_ERR_OK;
		default:
			break;
	}
	return OV_ERR_BADPARAM;
}

/*
*	Constructor of object objects
*/
OV_DLLFNCEXPORT OV_RESULT fb_object_constructor(
	OV_INSTPTR_ov_object pobj
) {
	/*
	*	local variables
	*/
	OV_RESULT					result;
	OV_ELEMENT				    parent, child;

	result = ov_object_constructor(pobj);
	if(Ov_Fail(result)) {
		return result;
    }
    
    /*
    *   Set FB-Flags
    */    
	parent.elemtype = OV_ET_OBJECT;
	parent.pobj = pobj;
	child.elemtype = OV_ET_NONE;
	
	while(TRUE) {
		if(Ov_OK(ov_element_getnextpart(&parent, &child, OV_ET_VARIABLE))) {
            /* Finished? */
    		if(child.elemtype == OV_ET_NONE) {
    			break;
    		}
    		/* Is part a variable? */
    		if (child.elemtype == OV_ET_VARIABLE) {
    			if(child.elemunion.pvar->v_veclen == 0) {
    				SetNewFlag(child.elemunion.pvar->v_flags, 'd');
    			}
            	if(child.elemunion.pvar->v_vartype & OV_VT_HAS_STATE ) {
    				SetNewFlag(child.elemunion.pvar->v_flags, 's');
    			}
        	}
        } else {
            break;
        }
    }
	return OV_ERR_OK;
}


/*
*	End of file
*/
