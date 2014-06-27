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
*   fb_connection.c                                                           *
*                                                                             *
*                                                                             *
*   Historie                                                                  *
*   --------                                                                  *
*   08-07-1999 Alexander Neugebauer: Erstellung, LTSoft GmbH, Kerpen          *
*                                    Lehrstuhl fuer Prozessleittechnik, Aachen*
*                                                                             *
*   Beschreibung                                                              *
*   ------------                                                              *
*   Implementierung der FB-Klasse connection                                  *
*                                                                             *
******************************************************************************/

#ifndef OV_COMPILE_LIBRARY_fb
#define OV_COMPILE_LIBRARY_fb
#endif

#include "fb.h"
#include "libov/ov_string.h"
#include "libov/ov_memstack.h"
#include "libov/ov_logfile.h"
#include "libov/ov_macros.h"
#include "libov/ov_path.h"
#include "libov/ov_ov.h"
#include "fb_namedef.h"
#include "fb_macros.h"
#include "fb_database.h"
#include "fb_av.h"
#include "fb_log.h"


/*	----------------------------------------------------------------------	*/
int fb_connectiontype_implemented(OV_VAR_TYPE typ) {
    return fb_vartype_implemented(typ);
}


/*	----------------------------------------------------------------------	*/
/*
*	Get element infos and test, if connection is consistent (subroutine)
*/
OV_DLLFNCEXPORT OV_BOOL fb_connection_checkelements(
	OV_INSTPTR_fb_connection	pconn
) {
	/*
	*	local variables
	*/
	OV_ELEMENT					sourcefbelem, targetfbelem;
	OV_INSTPTR_fb_object    	psourcefb, ptargetfb;
	OV_VTBLPTR_fb_object    	psourcefbvtable, ptargetfbvtable;
	OV_TICKET                   dummyTicket;
	OV_INSTPTR_fb_functionblock	pfb;
	
	/* Init the ticket */
	dummyTicket.vtbl = NULL;
	dummyTicket.type = OV_TT_SIMPLE;
	dummyTicket.ticketunion.simpleticket.id = pdb->serverpassword;

	/*
	*	get source element info and test if it is a readable variable
	*/
	if(!ov_string_compare(pconn->v_sourceport, "")) {
		return FALSE;
	}
	psourcefb = Ov_GetParent(fb_outputconnections, pconn);
	if(!psourcefb) {
		return FALSE;
	}
	sourcefbelem.elemtype = OV_ET_OBJECT;
	sourcefbelem.pobj = Ov_PtrUpCast(ov_object, psourcefb);
	ov_element_searchpart(&sourcefbelem, &pconn->v_sourceelem, 
		OV_ET_VARIABLE, pconn->v_sourceport);
	if(pconn->v_sourceelem.elemtype != OV_ET_VARIABLE) {
		return FALSE;
	}

	Ov_GetVTablePtr(fb_object, psourcefbvtable, psourcefb);  
	if(!psourcefbvtable) {
	    pfb = Ov_DynamicPtrCast(fb_functionblock, psourcefb);
	    if(pfb) {
	    	pfb->v_actimode = FB_AM_OFF;	/* FIXME! */
    		pfb->v_ErrState = 1;	/* FIXME! */
    	}
		ov_logfile_error("Object %s: method table not found.", 
		    psourcefb->v_identifier);
		return FALSE;
    }
	if(!(psourcefbvtable->m_getaccess(
		Ov_PtrUpCast(ov_object, psourcefb),
		&pconn->v_sourceelem,
		&dummyTicket
	) & OV_AC_READ)) {
		return FALSE;
	}
    /* Typ unterstuetzt? */
    if( !fb_connectiontype_implemented(pconn->v_sourceelem.elemunion.pvar->v_vartype)) {
        return FALSE;
	}
	/*
	*	get target element info and test if it is a writeable variable
	*/
	if(!ov_string_compare(pconn->v_targetport, "")) {
		return FALSE;
	}
	ptargetfb = Ov_GetParent(fb_inputconnections, pconn);
	if(!ptargetfb) {
		return FALSE;
	}
	targetfbelem.elemtype = OV_ET_OBJECT;
	targetfbelem.pobj = Ov_PtrUpCast(ov_object, ptargetfb);
	ov_element_searchpart(&targetfbelem, &pconn->v_targetelem, 
		OV_ET_VARIABLE, pconn->v_targetport);
	if(pconn->v_targetelem.elemtype != OV_ET_VARIABLE) {
		return FALSE;
	}
	
	Ov_GetVTablePtr(fb_object, ptargetfbvtable, ptargetfb);  
	if(!ptargetfbvtable) {
	    pfb = Ov_DynamicPtrCast(fb_functionblock, ptargetfb);
	    if(pfb) {
	    	pfb->v_actimode = FB_AM_OFF;	/* FIXME! */
    		pfb->v_ErrState = 1;	/* FIXME! */
    	}
		ov_logfile_error("Object %s: method table not found.", 
		    ptargetfb->v_identifier);
		return FALSE;
    }
	if(!(ptargetfbvtable->m_getaccess(
		Ov_PtrUpCast(ov_object, ptargetfb),
		&pconn->v_targetelem,
		&dummyTicket
	) & OV_AC_WRITE)) {
		return FALSE;
	}
	if(IsFlagSet(pconn->v_targetelem.elemunion.pvar->v_flags, 'p') ) {
		return FALSE;
	}
    /* Typ unterstuetzt? */
    if( !fb_connectiontype_implemented(pconn->v_targetelem.elemunion.pvar->v_vartype)) {
        return FALSE;
	}
	
	return TRUE;
}
OV_BOOL fb_connection_getelements(
	OV_INSTPTR_fb_connection	pconn
) {
    OV_BOOL res;
	OV_VTBLPTR_fb_connection	pvtable;

    Ov_GetVTablePtr(fb_connection, pvtable, pconn);
	if(!pvtable) {
		pconn->v_on = 0;	/* FIXME! */

        /* Logging */
        FbSvcLog_printexecitem((OV_INSTPTR_ov_object)pconn, "ERROR: method table not found");

        pconn->v_sourceelem.elemtype = OV_ET_NONE;
        pconn->v_targetelem.elemtype = OV_ET_NONE;
        
        return FALSE;
    }
    
    res = pvtable->m_checkelements(pconn);
    if(res == FALSE) {
        /* Merke: Initialisierung invalid */
        pconn->v_sourceelem.elemtype = OV_ET_NONE;
        pconn->v_targetelem.elemtype = OV_ET_NONE;
    }
    
    return res;
}

/*	----------------------------------------------------------------------	*/
/*
*	Set accessor of variable "on"
*/
OV_DLLFNCEXPORT OV_RESULT fb_connection_on_set(
	OV_INSTPTR_fb_connection	pconn,
	const OV_BOOL				on
) {
	if(!pconn){
		return OV_ERR_BADPARAM;
	}
	//fixme, hier sollte eigentlich fb_connection_checkelements() aufgerufen werden, oder?
	pconn->v_on = on;
	return OV_ERR_OK;
}

/*	----------------------------------------------------------------------	*/
/*
*	Set accessor of variable "sourceport"
*/
OV_DLLFNCEXPORT OV_RESULT fb_connection_sourceport_set(
	OV_INSTPTR_fb_connection	pconn,
	const OV_STRING				value
) {
	if(!pconn){
		return OV_ERR_BADPARAM;
	}
    if( ov_string_compare(pconn->v_sourceport, value)) {
        /*
        *   Initialisierung der Verbindung erzwingen
        */    
        pconn->v_sourceelem.elemtype = OV_ET_NONE;
        pconn->v_targetelem.elemtype = OV_ET_NONE;
        
        return ov_string_setvalue(&pconn->v_sourceport, value);
    }
    return OV_ERR_OK;
}

/*	----------------------------------------------------------------------	*/
/*
*	Set accessor of variable "targetport"
*/
OV_DLLFNCEXPORT OV_RESULT fb_connection_targetport_set(
	OV_INSTPTR_fb_connection	pconn,
	const OV_STRING				value
) {
	if(!pconn){
		return OV_ERR_BADPARAM;
	}
    if( ov_string_compare(pconn->v_targetport, value)) {
        /*
        *   Initialisierung der Verbindung erzwingen
        */    
        pconn->v_sourceelem.elemtype = OV_ET_NONE;
        pconn->v_targetelem.elemtype = OV_ET_NONE;
        
        return ov_string_setvalue(&pconn->v_targetport, value);
    }
    return OV_ERR_OK;
}

/*	----------------------------------------------------------------------	*/
/*
*	Constructor of connection objects
*/
OV_DLLFNCEXPORT OV_RESULT fb_connection_constructor(
	OV_INSTPTR_ov_object pobj
) {
	/*
	*	local variables
	*/
	OV_RESULT					result;
	OV_INSTPTR_fb_dbinfoclass	pdbinfo ;
	OV_INSTPTR_fb_connection	pconn = Ov_StaticPtrCast(fb_connection, pobj);

	result = ov_object_constructor(pobj);
	if(Ov_Fail(result))
		return result;
    
    /*
    *   Verbindung noch nicht initialisiert
    */    
    pconn->v_sourceelem.elemtype = OV_ET_NONE;
    pconn->v_targetelem.elemtype = OV_ET_NONE;

    /*
    *   Eine Verbindung mehr
    */    
	pdbinfo = (OV_INSTPTR_fb_dbinfoclass)fb_database_getdbinfo();
	if(pdbinfo)
		pdbinfo->v_connnumber++;
	
	return OV_ERR_OK;
}

/*	----------------------------------------------------------------------	*/

OV_DLLFNCEXPORT OV_RESULT fb_connection_checkinit(
	OV_INSTPTR_ov_object pobj
) {
	OV_INSTPTR_fb_connection	pconn = Ov_StaticPtrCast(fb_connection, pobj);

    /*
    *   Verbindung noch nicht initialisiert
    */    
    pconn->v_sourceelem.elemtype = OV_ET_NONE;
    pconn->v_targetelem.elemtype = OV_ET_NONE;

    return OV_ERR_OK;
}

/*	----------------------------------------------------------------------	*/

/*
*	Start up the connection object
*/
OV_DLLFNCEXPORT void fb_connection_startup(
	OV_INSTPTR_ov_object pobj
) {
	/*
	*	local variables
	*/
	OV_INSTPTR_fb_connection	pconn = Ov_StaticPtrCast(fb_connection, pobj);
	/*
	*	get element info (pointer and offsets and stuff...)
	*/
	ov_object_startup(pobj);

    /*
    *   Neue-Initialisierung der Verbindung erzwingen
    */    
    pconn->v_sourceelem.elemtype = OV_ET_NONE;
    pconn->v_targetelem.elemtype = OV_ET_NONE;
}

/*	----------------------------------------------------------------------	*/

OV_DLLFNCEXPORT void fb_connection_destructor(
	OV_INSTPTR_ov_object pobj
) {
	OV_INSTPTR_fb_dbinfoclass	pdbinfo;

	ov_object_destructor(pobj);
	
	pdbinfo = (OV_INSTPTR_fb_dbinfoclass)fb_database_getdbinfo();
	if(pdbinfo){
        pdbinfo->v_connnumber = (pdbinfo->v_connnumber > 0) ? (pdbinfo->v_connnumber - 1) : 0 ;
	}
}

/*	----------------------------------------------------------------------	*/

/*
*	Get the access rights for the connection object or its variables
*/
OV_DLLFNCEXPORT OV_ACCESS fb_connection_getaccess(
	OV_INSTPTR_ov_object		pobj,
	const OV_ELEMENT			*pelem,
	const OV_TICKET				*pticket
) {
	/*
	*	switch based on the element's type
	*/
	switch(pelem->elemtype) {
		case OV_ET_VARIABLE:
			if(pelem->elemunion.pvar->v_offset >= offsetof(OV_INST_ov_object,__classinfo)) {
			    /* Typ unterstuetzt? */
			    if( !fb_connectiontype_implemented(pelem->elemunion.pvar->v_vartype)) {
                    return OV_AC_NONE;
				}
                return OV_AC_READWRITE;
			}
			break;
		case OV_ET_OBJECT:
		    /* Version 2: Auch Links zu spaeteren Zeitpunkten zulassen */
			return (OV_AC_READWRITE | OV_AC_RENAMEABLE | OV_AC_DELETEABLE | OV_AC_LINKABLE | OV_AC_UNLINKABLE);
		default:
			break;
	}
	return ov_object_getaccess(pobj, pelem, pticket);
}

/*	----------------------------------------------------------------------	*/

/*
*	Trigger the connection object
*/
OV_DLLFNCEXPORT void fb_connection_trigger(
	OV_INSTPTR_fb_connection	pconn
) {
	/*
	*	local variables
	*/
	OV_INSTPTR_fb_object    	psourcefb, ptargetfb;
	OV_VTBLPTR_fb_object    	psourcefbvtable, ptargetfbvtable;
	OV_INSTPTR_fb_functionblock	pfb;
	OV_ANY              	    varcurrprops;
	
	/*
	*	if we are "off", quit
	*/
	if(!pconn->v_on) {
		return;
	}
	
    /* Logging */
    FbSvcLog_incrIndent();

	/*
	*	get source and target fb pointer/vtable pointer
	*/
	psourcefb = Ov_GetParent(fb_outputconnections, pconn);
	ptargetfb = Ov_GetParent(fb_inputconnections, pconn);
	if(!psourcefb) {
        /* Logging */
        FbSvcLog_printexecitem((OV_INSTPTR_ov_object)pconn, "ERROR: source block missing");
        FbSvcLog_decrIndent();
        
	    return;
	}
	if(!ptargetfb) {
        /* Logging */
        FbSvcLog_printexecitem((OV_INSTPTR_ov_object)pconn, "ERROR: target block missing");
        FbSvcLog_decrIndent();
        
	    return;
	}
	/*
	*	Verbindung initialisert?
	*/
    if( (pconn->v_sourceelem.elemtype == OV_ET_NONE) ||
        (pconn->v_targetelem.elemtype == OV_ET_NONE) ) {
    	if(!fb_connection_getelements(pconn)) {
    	
    	    /* Turn connection off */
    		pconn->v_on = FALSE;
    		
    		pfb = Ov_DynamicPtrCast(fb_functionblock, psourcefb);
    		if(pfb) {
    		    pfb->v_ErrState = 1;	/* FIXME! */
    		}
    		pfb = Ov_DynamicPtrCast(fb_functionblock, ptargetfb);
    		if(pfb) {
		        pfb->v_ErrState = 1;	/* FIXME! */
		    }
    		
            /* Logging */
            FbSvcLog_printexecitem((OV_INSTPTR_ov_object)pconn, "ERROR: bad init parameter");
            FbSvcLog_decrIndent();
        
    		return;
    	}
    }
    

	Ov_GetVTablePtr(fb_object, psourcefbvtable, psourcefb);
	if(!psourcefbvtable) {
		pfb = Ov_DynamicPtrCast(fb_functionblock, psourcefb);
		if(pfb) {
			pfb->v_actimode = FB_AM_OFF;	/* FIXME! */
    		pfb->v_ErrState = 1;	/* FIXME! */
        }
		
        /* Logging */
        FbSvcLog_printexecitem((OV_INSTPTR_ov_object)psourcefb, "ERROR: method table not found");
        FbSvcLog_decrIndent();
    	ov_logfile_error("Object %s: method table not found.", 
    		    psourcefb->v_identifier);
		return;
    }
    
	Ov_GetVTablePtr(fb_object, ptargetfbvtable, ptargetfb);
	if(!ptargetfbvtable) {
		pfb = Ov_DynamicPtrCast(fb_functionblock, ptargetfb);
		if(pfb) {
			pfb->v_actimode = FB_AM_OFF;	/* FIXME! */
    		pfb->v_ErrState = 1;	/* FIXME! */
        }
        
        /* Logging */
        FbSvcLog_printexecitem((OV_INSTPTR_ov_object)ptargetfb, "ERROR: method table not found");
        FbSvcLog_decrIndent();

		ov_logfile_error("Object %s: method table not found.", 
		    ptargetfb->v_identifier);
		return;
    }
	/*
	*	get source value and set target value
	*/
	ov_memstack_lock();
	if(Ov_Fail(psourcefbvtable->m_getvar(
		Ov_PtrUpCast(ov_object, psourcefb),
		&pconn->v_sourceelem,
		&varcurrprops
	))) {
	    /* Logging */
        FbSvcLog_printexecitem((OV_INSTPTR_ov_object)pconn, "ERROR: can't get source value");
	} else {
		if(Ov_Fail(ptargetfbvtable->m_setvar(
			Ov_PtrUpCast(ov_object, ptargetfb),
			&pconn->v_targetelem,
			&varcurrprops
		))) {
    	    /* Logging */
            FbSvcLog_printexecitem((OV_INSTPTR_ov_object)pconn, "ERROR: can't set target value");
        }
    }
	ov_memstack_unlock();
	
    /* Logging */
    FbSvcLog_decrIndent();
}

/*	----------------------------------------------------------------------	*/
OV_RESULT fb_concreate_init_fnc(
    OV_INSTPTR_ov_object  pobj,
    OV_POINTER            userdata
) {
    /* Lokale Variablen */
	OV_RESULT                   result;
    OV_INSTPTR_fb_object     pSourceObj;
    OV_INSTPTR_fb_object     pTargetObj;
    OV_STRING                   *pUserdata;
	OV_INSTPTR_fb_connection    pconn = Ov_StaticPtrCast(fb_connection, pobj);
    
    pUserdata = (OV_STRING*)userdata;
    
    /* Source FB suchen */
    pSourceObj = (OV_INSTPTR_fb_object)ov_path_getobjectpointer(pUserdata[0], 2);
    if(!pSourceObj) {
        return OV_ERR_BADPATH;
    }
    /* Target FB suchen */
    pTargetObj = (OV_INSTPTR_fb_object)ov_path_getobjectpointer(pUserdata[2], 2);
    if(!pTargetObj) {
        return OV_ERR_BADPATH;
    }

    /* Verbundungs-Objekt mit Source- und Target- FB verlinken */
    result = Ov_Link(fb_outputconnections, pSourceObj, pconn);
    if(result != OV_ERR_OK) {
        return result;
    }
    result = Ov_Link(fb_inputconnections, pTargetObj, pconn);
    if(result != OV_ERR_OK) {
        Ov_Unlink(fb_outputconnections, pSourceObj, pconn);
        return result;
    }

    /* Target- und Source- Ports setzen */
    ov_string_setvalue(&pconn->v_sourceport, pUserdata[1]);
    ov_string_setvalue(&pconn->v_targetport, pUserdata[3]);
    
    /* Initialisierung OK ? */
    if( !fb_connection_getelements(pconn) ) {
        ov_string_setvalue(&pconn->v_sourceport, NULL);
        ov_string_setvalue(&pconn->v_targetport, NULL);
        Ov_Unlink(fb_inputconnections,  pTargetObj, pconn);
        Ov_Unlink(fb_outputconnections, pSourceObj, pconn);
        return OV_ERR_BADINITPARAM;
    }

    return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_RESULT fb_connection_create(
	OV_STRING				  identifier,
	OV_STRING				  sourceFB,
	OV_STRING				  sourcePort,
	OV_STRING				  targetFB,
	OV_STRING				  targetPort,
    OV_INSTPTR_fb_connection  *pcon
) {
    /* Lokale Variablen */
	OV_RESULT               result;
    OV_STRING               conIdent = NULL;
    OV_STRING               pIdent;
    OV_INSTPTR_ov_class     pClas;
    OV_INSTPTR_ov_domain    pdom;
    OV_INSTPTR_ov_object    pobj;
    OV_TIME                 t0;
    char                    help[128];
    char                   *ph;
    OV_STRING               userdata[4];
    
    /*  check parameters  */
	if(!pcon || !sourceFB || !sourcePort || !targetFB || !targetPort) {
		return OV_ERR_BADPARAM;
	}
	*pcon = NULL;
		
    /* Klass suchen */
    pClas = ov_class_search("fb/connection");
    if(!pClas) {
        /* Logging */
        FbSvcLog_printsvc(KS_CREATEOBJECT, "fb_server", "create connection object", OV_ERR_BADFACTORY);
        
        return OV_ERR_BADFACTORY;
    }
    
    /* Connection container */
    pdom = (OV_INSTPTR_ov_domain)Ov_SearchChild(ov_containment, &pdb->root, FB_CONN_CONTAINER);
	if(!pdom) {
        /* Logging */
        FbSvcLog_printsvc(KS_CREATEOBJECT, "fb_server", "create connection object", OV_ERR_BADPATH);

	    return OV_ERR_BADPATH;
	}
	
	/* Identifier gesetzt? */
	if(!identifier) {
        /* Create connection identifier */
        ov_time_gettime(&t0);
        strcpy(help, "v_a_");
        ph = help;
        ph += 4;
    	while(t0.secs > 0) {
            sprintf(ph,"%c", (int)((t0.secs % 26) + 97) );
            ph++;
            t0.secs /= 26;
    	}
    
    	while(t0.usecs > 0) {
            sprintf(ph,"%c", (int)((t0.usecs % 26) + 97) );
            ph++;
            t0.usecs /= 26;
    	}
   	    *ph = '\0';
   	       	    
        conIdent = (OV_STRING)malloc(strlen(help) + 1);
        if(!conIdent) {
            return OV_ERR_HEAPOUTOFMEMORY;
        }
        strcpy(conIdent, help);
        pIdent = conIdent;
        
    } else {
        pIdent = identifier;
    }
    
    /* Daten merken */
    userdata[0] = sourceFB;
    userdata[1] = sourcePort;
    userdata[2] = targetFB;
    userdata[3] = targetPort;
    
    /* Verbindung anlegen */
    pobj = NULL;
    result = ov_class_createobject(pClas, pdom, pIdent, OV_PMH_DEFAULT, NULL,
	         fb_concreate_init_fnc, (OV_POINTER)&userdata, &pobj);

    if(result == OV_ERR_OK) {
        *pcon = (OV_INSTPTR_fb_connection)pobj;
    }

    /* Speicher belegt? */
    if(conIdent) {
        free(conIdent);
    }
    
    /* Logging */
    FbSvcLog_printsvc(KS_CREATEOBJECT, "fb_server", "create connection object", result);
    
    return result;
}


/*	----------------------------------------------------------------------	*/
/*
*	End of file
*/
