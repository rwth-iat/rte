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
*   08-07-2011 Gustavo Quiros: Added support for function charts.             *
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
#include "fb_namedef.h"
#include "fb_macros.h"
#include "fb_database.h"
#include "fb_av.h"
#include "fb_log.h"
#include "ov_call_macros_10.h"

/* internal use */
int fb_vartype_implemented(OV_VAR_TYPE typ);

/*	----------------------------------------------------------------------	*/
int fb_connectiontype_implemented(OV_VAR_TYPE typ) {
    return fb_vartype_implemented(typ);
}

OV_INSTPTR_fb_variable fb_functionchart_searchvariable
(OV_INSTPTR_fb_functionchart pfc, OV_STRING id);

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
	OV_ELEMENT			sourceelem, targetelem;
	OV_INSTPTR_fb_functionblock	psourcefb, ptargetfb;
	OV_INSTPTR_fb_functionchart	psourcefc, ptargetfc;
	OV_INSTPTR_ov_object		psourceobj, ptargetobj;
	OV_INSTPTR_fb_variable		psourcevar, ptargetvar, plinkedvar;
	OV_VTBLPTR_ov_object	psourceobjvtable, ptargetobjvtable;
	OV_TICKET                   	dummyTicket;
        OV_STRING 			port = NULL;

	/* Init the ticket */
	dummyTicket.vtbl = NULL;
	dummyTicket.type = OV_TT_SIMPLE;
	dummyTicket.ticketunion.simpleticket.id = pdb->serverpassword;

	/*
	* get source object and source element info and test if it is a readable variable
	*/

        psourceobj = NULL;

	if(!ov_string_compare(pconn->v_sourceport, "")) {
		return FALSE;
	}
	psourcefb = Ov_GetParent(fb_outputconnections, pconn);
        
        if (psourcefb) {
          psourcefc = Ov_DynamicPtrCast (fb_functionchart, psourcefb);
        } else {
          psourcefc = Ov_GetParent (fb_initialconnections, pconn);  
        }
        
        psourcevar = NULL;
        if (psourcefc) {
          psourcevar = fb_functionchart_searchvariable (psourcefc, pconn->v_sourceport);
          if (psourcevar) {
            /* Link source variable */
            plinkedvar = Ov_GetParent (fb_sourcevariable, pconn);
            if (plinkedvar != psourcevar) {
              if (plinkedvar) {
                Ov_Unlink (fb_sourcevariable, plinkedvar, pconn);
                pconn->v_on = TRUE; /* Unlinking disables connection, enable it */
              }
              Ov_Link (fb_sourcevariable, psourcevar, pconn);
            }
            port = "value";
            psourceobj = Ov_PtrUpCast (ov_object, psourcevar);
          }
        }

        if (!psourceobj && psourcefb) {
          psourceobj = Ov_PtrUpCast (ov_object, psourcefb);
          port = pconn->v_sourceport;
        }

        if (!psourceobj) return FALSE;
  
        /* Get element */
  
	sourceelem.elemtype = OV_ET_OBJECT;
	sourceelem.pobj = psourceobj;
	ov_element_searchpart(&sourceelem, &pconn->v_sourceelem, 
		OV_ET_VARIABLE, port);
	if(pconn->v_sourceelem.elemtype != OV_ET_VARIABLE) {
		return FALSE;
	}
	Ov_GetVTablePtr(ov_object, psourceobjvtable, psourceobj);
  
	if(!psourceobjvtable) {
          if (psourcefb) {
		psourcefb->v_actimode = 0;	/* FIXME! */
		psourcefb->v_ErrState = 1;	/* FIXME! */
          }
          if (psourcefc) {
		psourcefc->v_actimode = 0;	/* FIXME! */
		psourcefc->v_ErrState = 1;	/* FIXME! */
          }
		ov_logfile_error("Object %s: method table not found.", 
		    psourceobj->v_identifier);

		return FALSE;
    }
	if(!(psourceobjvtable->m_getaccess(
		psourceobj,
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
	* get target object and target element info and test if it is a writeable variable
	*/

	ptargetobj = NULL;
    
	if(!ov_string_compare(pconn->v_targetport, "")) {
		return FALSE;
	}
	ptargetfb = Ov_GetParent(fb_inputconnections, pconn);

        if (ptargetfb) {
          ptargetfc = Ov_DynamicPtrCast (fb_functionchart, ptargetfb);
        } else {
          ptargetfc = Ov_GetParent (fb_finalconnections, pconn);  
        }

        ptargetvar = NULL;
        if (ptargetfc) {
          ptargetvar = fb_functionchart_searchvariable (ptargetfc, pconn->v_targetport);
          if (ptargetvar) {
            /* Link target variable */
            plinkedvar = Ov_GetChild (fb_targetvariable, pconn);
            if (plinkedvar != ptargetvar) {
              if (plinkedvar) {
                Ov_Unlink (fb_targetvariable, pconn, plinkedvar);
                pconn->v_on = TRUE; /* Unlinking disables connection, enable it */
              }
              Ov_Link (fb_targetvariable, pconn, ptargetvar);
            }
            port = "value";
            ptargetobj = Ov_PtrUpCast (ov_object, ptargetvar);
          }
        }
        
        if ((!ptargetobj) && ptargetfb) {
          ptargetobj = Ov_PtrUpCast (ov_object, ptargetfb);
          port = pconn->v_targetport;
        }

        if (!ptargetobj) return FALSE;
        
        /* Get element */
  
	targetelem.elemtype = OV_ET_OBJECT;
	targetelem.pobj = ptargetobj;
	ov_element_searchpart(&targetelem, &pconn->v_targetelem, 
		OV_ET_VARIABLE, port);
	if(pconn->v_targetelem.elemtype != OV_ET_VARIABLE) {
		return FALSE;
	}
	Ov_GetVTablePtr(ov_object, ptargetobjvtable, ptargetobj);
  
	if(!ptargetobjvtable) {
    if (ptargetfb) {
      ptargetfb->v_actimode = 0;	/* FIXME! */
      ptargetfb->v_ErrState = 1;	/* FIXME! */
    }
    if (ptargetfc) {
      ptargetfc->v_actimode = 0;	/* FIXME! */
      ptargetfc->v_ErrState = 1;	/* FIXME! */
    }
		ov_logfile_error("Object %s: method table not found.", 
		    ptargetobj->v_identifier);
		return FALSE;
    }
	if(!(ptargetobjvtable->m_getaccess(
		ptargetobj,
		&pconn->v_targetelem,
		&dummyTicket
	) & OV_AC_WRITE)) {
		return FALSE;
	}
  if (ptargetfc) {
    if (IsFlagSet (ptargetvar->v_flags, 'p')) {
      return FALSE;
    }
  } else if (IsFlagSet (pconn->v_targetelem.elemunion.pvar->v_flags, 'p')) {
    return FALSE;
  }

    /**********************************************************************/
	/*
	*	check if source and target port have the same type and vector length
	*/

  /* Allow implicit conversion to and from ANY (FIXME: is this safe?) */
  if (pconn->v_sourceelem.elemunion.pvar->v_vartype == OV_VT_ANY ||
      pconn->v_targetelem.elemunion.pvar->v_vartype == OV_VT_ANY) {
    return TRUE;
  }

	if(pconn->v_sourceelem.elemunion.pvar->v_vartype 
		!= pconn->v_targetelem.elemunion.pvar->v_vartype
	) {
		return FALSE;
	}
	if(pconn->v_sourceelem.elemunion.pvar->v_vartype == OV_VT_STRUCT) {
		if(Ov_GetParent(ov_construction, pconn->v_sourceelem.elemunion.pvar)
			!= Ov_GetParent(ov_construction, pconn->v_targetelem.elemunion.pvar)
		) {
			return FALSE;
		}
	}
	if(pconn->v_sourceelem.elemunion.pvar->v_vartype == OV_VT_BYTE_VEC) {
		if(strcmp(pconn->v_sourceelem.elemunion.pvar->v_ctypename, 
			pconn->v_targetelem.elemunion.pvar->v_ctypename)
		) {
			return FALSE;
		}
	}
	if(pconn->v_sourceelem.elemunion.pvar->v_veclen 
		!= pconn->v_targetelem.elemunion.pvar->v_veclen
	) {
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
			    if( !fb_vartype_implemented(pelem->elemunion.pvar->v_vartype)) {
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
	OV_INSTPTR_fb_functionblock	psourcefb, ptargetfb;
	OV_INSTPTR_fb_functionchart	psourcefc, ptargetfc;
	OV_INSTPTR_ov_object		psourceobj, ptargetobj;
	OV_INSTPTR_fb_variable		psourcevar, ptargetvar;
	OV_VTBLPTR_ov_object		psourceobjvtable, ptargetobjvtable;
	OV_ANY              		varcurrprops;
	
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
	psourcefc = Ov_GetParent(fb_initialconnections, pconn);
	ptargetfc = Ov_GetParent(fb_finalconnections, pconn);

	if(!psourcefb && !psourcefc) {
        /* Logging */
        FbSvcLog_printexecitem((OV_INSTPTR_ov_object)pconn, "ERROR: source functionblock missing");
        FbSvcLog_decrIndent();
        
	    return;
	}
	if(!ptargetfb && !ptargetfc) {
        /* Logging */
        FbSvcLog_printexecitem((OV_INSTPTR_ov_object)pconn, "ERROR: target functionblock missing");
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
    		
    		if (psourcefb) psourcefb->v_ErrState = 1;	/* FIXME! */
                if (ptargetfb) ptargetfb->v_ErrState = 1;	/* FIXME! */
                if (psourcefc) psourcefc->v_ErrState = 1;	/* FIXME! */
                if (ptargetfc) ptargetfc->v_ErrState = 1;	/* FIXME! */
    		
            /* Logging */
            FbSvcLog_printexecitem((OV_INSTPTR_ov_object)pconn, "ERROR: bad init parameter");
            FbSvcLog_decrIndent();
        
    		return;
    	}
    }

  /* Get source and target variables, if any */

  psourcevar = Ov_GetParent (fb_sourcevariable, pconn);
  ptargetvar = Ov_GetChild (fb_targetvariable, pconn);

  /* Determine source and target objects */

  psourceobj = NULL;
  ptargetobj = NULL;
  if (psourcefb) psourceobj = Ov_PtrUpCast (ov_object, psourcefb);
  if (ptargetfb) ptargetobj = Ov_PtrUpCast (ov_object, ptargetfb);
  if (psourcevar) psourceobj = Ov_PtrUpCast (ov_object, psourcevar);
  if (ptargetvar) ptargetobj = Ov_PtrUpCast (ov_object, ptargetvar);
    

	Ov_GetVTablePtr(ov_object, psourceobjvtable, psourceobj);
	if(!psourceobjvtable) {
		if (psourcefb) { 
      psourcefb->v_actimode = 0;	/* FIXME! */
      psourcefb->v_ErrState = 1;	/* FIXME! */
    }
		if (psourcefc) {
      psourcefc->v_actimode = 0;	/* FIXME! */
      psourcefc->v_ErrState = 1;	/* FIXME! */
    }	
        /* Logging */
        FbSvcLog_printexecitem(psourceobj, "ERROR: method table not found");
        FbSvcLog_decrIndent();
        
		ov_logfile_error("Object %s: method table not found.", 
		    psourceobj->v_identifier);
		return;
    }
    
	Ov_GetVTablePtr(ov_object, ptargetobjvtable, ptargetobj);
	if(!ptargetobjvtable) {
		if (ptargetfb) {
      ptargetfb->v_actimode = 0;	/* FIXME! */
      ptargetfb->v_ErrState = 1;	/* FIXME! */
    }
		if (ptargetfc) {
      ptargetfc->v_actimode = 0;	/* FIXME! */
      ptargetfc->v_ErrState = 1;	/* FIXME! */
    }

        /* Logging */
        FbSvcLog_printexecitem(ptargetobj, "ERROR: method table not found");
        FbSvcLog_decrIndent();

		ov_logfile_error("Object %s: method table not found.", 
		    ptargetobj->v_identifier);
		return;
    }
	/*
	*	get source value and set target value
	*/
	ov_memstack_lock();
	if(Ov_Fail(psourceobjvtable->m_getvar(
		psourceobj,
		&pconn->v_sourceelem,
		&varcurrprops
	))) {
	    /* Logging */
        FbSvcLog_printexecitem((OV_INSTPTR_ov_object)pconn, "ERROR: can't get source value");
	} else {
		if(Ov_Fail(ptargetobjvtable->m_setvar(
			ptargetobj,
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
    OV_INSTPTR_fb_functionblock pSourceFB;
    OV_INSTPTR_fb_functionblock pTargetFB;
    OV_STRING                   *pUserdata;
	OV_INSTPTR_fb_connection    pconn = Ov_StaticPtrCast(fb_connection, pobj);
    
    pUserdata = (OV_STRING*)userdata;
    
    /* Source FB suchen */
    pSourceFB = (OV_INSTPTR_fb_functionblock)ov_path_getobjectpointer(pUserdata[0], 2);
    if(!pSourceFB) {
        return OV_ERR_BADPATH;
    }
    /* Target FB suchen */
    pTargetFB = (OV_INSTPTR_fb_functionblock)ov_path_getobjectpointer(pUserdata[2], 2);
    if(!pTargetFB) {
        return OV_ERR_BADPATH;
    }

    /* Verbundungs-Objekt mit Source- und Target- FB verlinken */
    result = Ov_Link(fb_outputconnections, pSourceFB, pconn);
    if(result != OV_ERR_OK) {
        return result;
    }
    result = Ov_Link(fb_inputconnections, pTargetFB, pconn);
    if(result != OV_ERR_OK) {
        Ov_Unlink(fb_outputconnections, pSourceFB, pconn);
        return result;
    }

    /* Target- und Source- Ports setzen */
    ov_string_setvalue(&pconn->v_sourceport, pUserdata[1]);
    ov_string_setvalue(&pconn->v_targetport, pUserdata[3]);
    
    /* Initialisierung OK ? */
    if( !fb_connection_getelements(pconn) ) {
        ov_string_setvalue(&pconn->v_sourceport, NULL);
        ov_string_setvalue(&pconn->v_targetport, NULL);
        Ov_Unlink(fb_inputconnections,  pTargetFB, pconn);
        Ov_Unlink(fb_outputconnections, pSourceFB, pconn);
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

OV_DLLFNCEXPORT OV_STRING fb_connection_visuallayout_get(
    OV_INSTPTR_fb_connection          pobj
) {
    return pobj->v_visuallayout;
}

OV_DLLFNCEXPORT OV_RESULT fb_connection_visuallayout_set(
    OV_INSTPTR_fb_connection          pobj,
    const OV_STRING  value
) {
    return ov_string_setvalue(&pobj->v_visuallayout,value);
}


/*	----------------------------------------------------------------------	*/
/*
*	End of file
*/
