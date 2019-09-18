/* -*-plt-c++-*- */
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
*   fb_loadlib.c                                                              *
*                                                                             *
*   Historie                                                                  *
*   --------                                                                  *
*   2000-01-30 Alexander Neugebauer: Erstellung, LTSoft GmbH, Kerpen          *
*                                                                             *
*   Beschreibung                                                              *
*   ------------                                                              *
*   Hilfsfunktionen : Laden der FB-DLL                                        *
*                                                                             *
******************************************************************************/

/*
*	Include-Dateien
*	---------------
*/

#include "fb.h"
#include "fb_av.h"
#include "fb_database.h"

#ifdef ov_library_open_fb
#undef ov_library_open_fb
#endif


/*	----------------------------------------------------------------------	*/
OV_RESULT ov_library_setglobalvars_fb_new(void) {

    /*
     *	local variables
     */
    OV_RESULT                   result;
    OV_INSTPTR_fb_dbinfoclass	pdbinfo;

    result = ov_library_setglobalvars_fb();
    if(result != OV_ERR_OK) {
        return result;
    }
    
    /* Create fb structure */
    result = fb_database_checkstruct();
    if(result != OV_ERR_OK) {
        return result;
    }
    
    /* Licence */
    pdbinfo = (OV_INSTPTR_fb_dbinfoclass)fb_database_getdbinfo();
    if(pdbinfo) {
        fb_setlicinfo(NULL, &pdbinfo->v_licinfo, &pdbinfo->v_version);
    }
    return OV_ERR_OK;
}

/*	----------------------------------------------------------------------	*/

/*
*	Replace the "setglobalvars" function of a library with this
*	previous one.
*	----------------------------------------------------------------------
*/
OV_DLLFNCEXPORT OV_LIBRARY_DEF *ov_library_open_fb(void) {
	/*
	*	local variables
	*/
	static OV_LIBRARY_DEF *OV_LIBRARY_DEF_fb_new;
	/*
	*	replace the "setglobalvars" function created by the code generator
	*	with a new one.
	*/
	OV_LIBRARY_DEF_fb_new = ov_library_open_fb_old();
	OV_LIBRARY_DEF_fb_new->setglobalvarsfnc = ov_library_setglobalvars_fb_new;
	return OV_LIBRARY_DEF_fb_new;
}
/*
 *	End of file
 */
