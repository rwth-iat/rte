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
*   fb_connectionlinkss.c                                                     *
*                                                                             *
*   Historie                                                                  *
*   --------                                                                  *
*   1999-07-30 Alexander Neugebauer: Erstellung, LTSoft GmbH, Kerpen          *
*   2006-09-09 Links ohne "Create"-Dienst zulassen              A. Neugebauer *
*                                                                             *
*   Beschreibung                                                              *
*   ------------                                                              *
*   Funktionen der Assoziation inputconnection/outputconnection               *
*                                                                             *
******************************************************************************/

#ifndef OV_COMPILE_LIBRARY_fb
#define OV_COMPILE_LIBRARY_fb
#endif

#include "fb.h"
#include "libov/ov_association.h"
#include "libov/ov_macros.h"
#include "fb_namedef.h"

/*	----------------------------------------------------------------------	*/
/*
*	Link parent and child in a inputconnections association
*/
OV_DLLFNCEXPORT OV_RESULT fb_inputconnections_link(
	const OV_PPT_fb_inputconnections	pparent,
	const OV_CPT_fb_inputconnections	pchild,
	const OV_PLACEMENT_HINT	            parenthint,
	const OV_CPT_fb_inputconnections	prelparent,
	const OV_PLACEMENT_HINT	            childhint,
	const OV_CPT_fb_inputconnections	prelchild
) {
    OV_INSTPTR_fb_connection pconn = Ov_StaticPtrCast(fb_connection, pchild);
    
    if( (!pparent) || (!pchild) ) {
        return OV_ERR_BADINITPARAM;
    }
    
    /*
    *   Initialisierung der Verbindung erzwingen
    */    
    pconn->v_sourceelem.elemtype = OV_ET_NONE;
    pconn->v_targetelem.elemtype = OV_ET_NONE;

    return ov_association_link(passoc_fb_inputconnections,
            Ov_PtrUpCast(ov_object,	pparent),
            Ov_PtrUpCast(ov_object, pchild),
			parenthint, Ov_PtrUpCast(ov_object, prelparent),
			childhint, Ov_PtrUpCast(ov_object, prelchild));
}

/*	----------------------------------------------------------------------	*/
/*
*	Link parent and child in a outputconnections association
*/
OV_DLLFNCEXPORT OV_RESULT fb_outputconnections_link(
	const OV_PPT_fb_outputconnections	pparent,
	const OV_CPT_fb_outputconnections	pchild,
	const OV_PLACEMENT_HINT	            parenthint,
	const OV_CPT_fb_outputconnections	prelparent,
	const OV_PLACEMENT_HINT	            childhint,
	const OV_CPT_fb_outputconnections	prelchild
) {
    OV_INSTPTR_fb_connection pconn = Ov_StaticPtrCast(fb_connection, pchild);
    
    if( (!pparent) || (!pchild) ) {
        return OV_ERR_BADINITPARAM;
    }
    
    /*
    *   Initialisierung der Verbindung erzwingen
    */    
    pconn->v_sourceelem.elemtype = OV_ET_NONE;
    pconn->v_targetelem.elemtype = OV_ET_NONE;

    return ov_association_link(passoc_fb_outputconnections,
            Ov_PtrUpCast(ov_object,	pparent),
            Ov_PtrUpCast(ov_object, pchild),
			parenthint, Ov_PtrUpCast(ov_object, prelparent),
			childhint, Ov_PtrUpCast(ov_object, prelchild));
}

/*	----------------------------------------------------------------------	*/

/*
*	Unlink parent and child in a inputconnections association
*   OV_IMPL_UNLINK(fb_inputconnections)
*/

OV_DLLFNCEXPORT void fb_inputconnections_unlink(
	const OV_PPT_fb_inputconnections	pparent,
	const OV_CPT_fb_inputconnections	pchild
) {
    OV_INSTPTR_fb_connection	pconn = Ov_StaticPtrCast(fb_connection, pchild);
    
    if( (!pparent) || (!pchild) ) {
        return;
    }
    /* Verbindung ausschalten */
    pconn->v_on = FALSE;
    
    pconn->v_sourceelem.elemtype = OV_ET_NONE;
    pconn->v_targetelem.elemtype = OV_ET_NONE;

    /* Unlinken */
	ov_association_unlink(
        passoc_fb_inputconnections,
        Ov_PtrUpCast(ov_object, pparent),
        Ov_PtrUpCast(ov_object, pchild));
}

/*
*	Unlink parent and child in a outputconnections association
*   OV_IMPL_UNLINK(fb_outputconnections)
*/

OV_DLLFNCEXPORT void fb_outputconnections_unlink(
	const OV_PPT_fb_outputconnections	pparent,
	const OV_CPT_fb_outputconnections	pchild
) {
    OV_INSTPTR_fb_connection	pconn = Ov_StaticPtrCast(fb_connection, pchild);
    
    if( (!pparent) || (!pchild) ) {
        return;
    }
    /* Verbindung ausschalten */
    pconn->v_on = FALSE;
    
    pconn->v_sourceelem.elemtype = OV_ET_NONE;
    pconn->v_targetelem.elemtype = OV_ET_NONE;

    /* Unlinken */
	ov_association_unlink(
        passoc_fb_outputconnections,
        Ov_PtrUpCast(ov_object, pparent),
        Ov_PtrUpCast(ov_object, pchild));
}


/*	----------------------------------------------------------------------	*/

/*
*	Get access rights for a parent and child in a inputconnections association
*/
OV_DLLFNCEXPORT OV_ACCESS fb_inputconnections_getaccess(
	const OV_PPT_fb_inputconnections	pparent,
	const OV_CPT_fb_inputconnections	pchild,
	const OV_TICKET						*pticket
) {
	OV_INSTPTR_ov_object 	pcon;
    OV_ACCESS               acc;
    
	/*
	*	check conditions
	*/
    if(pchild) {
        if(!Ov_Association_IsChildClass(passoc_fb_inputconnections, pchild)) {
            return OV_AC_READ;
        }
    }
    if(pparent) {
        if(!Ov_Association_IsParentClass(passoc_fb_inputconnections, pparent)) {
            return OV_AC_READ;
        }
    }
    if(pchild) {
    	/*
    	*   allready linked?
    	*/	
        if(Ov_Association_GetParent(passoc_fb_inputconnections, pchild)) {
            return (OV_AC_READ | OV_AC_UNLINKABLE);
        }
    }
    if(pparent) {
        acc = (OV_AC_READ | OV_AC_LINKABLE);
        pcon = Ov_Association_GetFirstChild(passoc_fb_inputconnections, pparent);
        if(pcon) {
            acc |= OV_AC_UNLINKABLE;
        }
	    return acc;
	}
	/*
	*   Not linked
	*/	
	return (OV_AC_READ | OV_AC_LINKABLE);
}

/*
*	Get access rights for a parent and child in a outputconnections association
*/
OV_DLLFNCEXPORT OV_ACCESS fb_outputconnections_getaccess(
	const OV_PPT_fb_outputconnections	pparent,
	const OV_CPT_fb_outputconnections	pchild,
	const OV_TICKET						*pticket
) {
	OV_INSTPTR_ov_object 	pcon;
    OV_ACCESS               acc;
    
	/*
	*	check conditions
	*/
    if(pchild) {
        if(!Ov_Association_IsChildClass(passoc_fb_outputconnections, pchild)) {
            return OV_AC_READ;
        }
    }
    if(pparent) {
        if(!Ov_Association_IsParentClass(passoc_fb_outputconnections, pparent)) {
            return OV_AC_READ;
        }
    }
    if(pchild) {
    	/*
    	*   allready linked?
    	*/	
        if(Ov_Association_GetParent(passoc_fb_outputconnections, pchild)) {
            return (OV_AC_READ | OV_AC_UNLINKABLE);
        }
    }
    if(pparent) {
        acc = (OV_AC_READ | OV_AC_LINKABLE);
        pcon = Ov_Association_GetFirstChild(passoc_fb_outputconnections, pparent);
        if(pcon) {
            acc |= OV_AC_UNLINKABLE;
        }
	    return acc;
	}
	/*
	*   Not linked
	*/	
	return (OV_AC_READ | OV_AC_LINKABLE);
}

/*	----------------------------------------------------------------------	*/

/*
*	End of file
*/
