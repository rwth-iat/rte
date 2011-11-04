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

/*	----------------------------------------------------------------------	*/

/*
*	Link parent and child in a tasklist association
*/

OV_IMPL_LINK(fb_tasklist)

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
    char        help[32];
	OV_STRING   path;
	OV_ACCESS   acces;
	/*
	*	check parameters
	*/
	sprintf(help, "/%s/%s", FB_TASK_CONTAINER, FB_URTASK);
	ov_memstack_lock();
	path = ov_path_getcanonicalpath(Ov_PtrUpCast(ov_object, pchild),KS_VERSION);
	if(!ov_string_compare(path, help)) {
    	/*
    	*	"urtask" as child ?
    	*/
        acces = OV_AC_READ;
	} else {
	    acces = OV_AC_READ | OV_AC_LINKABLE | OV_AC_UNLINKABLE;
	}
	ov_memstack_unlock();
	
	return acces;
}

/*	----------------------------------------------------------------------	*/

/*
*	End of file
*/
