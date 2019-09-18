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
*   fb_tasklist.c                                                             *
*                                                                             *
*                                                                             *
*   Historie                                                                  *
*   --------                                                                  *
*   1999-07-08 Alexander Neugebauer: Erstellung, LTSoft GmbH, Kerpen          *
*                                    Lehrstuhl fuer Prozessleittechnik, Aachen*
*                                                                             *
*   Beschreibung                                                              *
*   ------------                                                              *
*   Funktionen der Asociation tasklist                                        *
*                                                                             *
******************************************************************************/

#ifndef OV_COMPILE_LIBRARY_fb
#define OV_COMPILE_LIBRARY_fb
#endif

#include "fb.h"
#include "libov/ov_association.h"
#include "libov/ov_macros.h"
#include "libov/ov_path.h"
#include "fb_namedef.h"
#include "fb_database.h"

/*	----------------------------------------------------------------------	*/

/*
*	Link parent and child in a tasklist association
*/

OV_DLLFNCEXPORT OV_RESULT fb_tasklist_link(
	const struct OV_INST_fb_task* pparent,
	const struct OV_INST_fb_task* pchild,
	const OV_PLACEMENT_HINT parenthint,
	const struct OV_INST_fb_task* prelparent,
	const OV_PLACEMENT_HINT childhint,
	const struct OV_INST_fb_task* prelchild) {

	OV_INSTPTR_fb_task pTask = (OV_INSTPTR_fb_task)pparent;

	// is pchild task parent of pparent?
	while(pTask && pTask!=pchild){
		pTask = Ov_GetParent(fb_tasklist, pTask);
	}
	if(pTask)
		return OV_ERR_BADPLACEMENT;

	return ov_association_link(passoc_fb_tasklist,
			Ov_PtrUpCast(ov_object, pparent), Ov_PtrUpCast(ov_object, pchild),
			parenthint, Ov_PtrUpCast(ov_object, prelparent), childhint,
			Ov_PtrUpCast(ov_object, prelchild));
}

/*	----------------------------------------------------------------------	*/

/*
*	Unlink parent and child in a tasklist association
*/

OV_IMPL_UNLINK(fb_tasklist)

/*	----------------------------------------------------------------------	*/

/*
*	Get access rights for a parent and child in a tasklist association
*/
OV_DLLFNCEXPORT OV_ACCESS fb_tasklist_getaccess(
	const OV_PPT_fb_tasklist	pparent,
	const OV_CPT_fb_tasklist	pchild,
	const OV_TICKET				*pticket
) {
	OV_ACCESS   acces;
	/*
	*	check parameters
	*/

	if(pchild==(OV_CPT_fb_tasklist)fb_database_geturtask()) {
    	/*
    	*	"urtask" as child ?
    	*/
        acces = OV_AC_READ;
	} else {
	    acces = OV_AC_READ | OV_AC_LINKABLE | OV_AC_UNLINKABLE;
	}

	return acces;
}

/*	----------------------------------------------------------------------	*/

/*
*	End of file
*/
