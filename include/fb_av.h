#ifndef _FB_AV_INCLUDE_
#define _FB_AV_INCLUDE_

/******************************************************************************
***                                                                         ***
***   iFBSpro   -   Funktionsbaustein-Model                                 ***
***   #####################################                                 ***
***                                                                         ***
***   L T S o f t                                                           ***
***   Agentur f�r Leittechnik Software GmbH                                 ***
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
*   fb_av.h                                                                   *
*                                                                             *
*   Historie                                                                  *
*   --------                                                                  *
*   1998-02-22  Alexander Neugebauer: Erstellung, LTSoft, Kerpen              *
*   2001-06-12  Major revision.        A.Neugebauer                           *
*                                                                             *
*                                                                             *
*   Beschreibung                                                              *
*   ------------                                                              *
*   Funktions-Prototypen fuer Server-Identifizierung                          *
*                                                                             *
******************************************************************************/

#include "libov/ov_ov.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
*   Zugriffsrechte fuer Variablen pruefen
*   -------------------------------------
*/
OV_BOOL fb_ticket_canwrite(const OV_ELEMENT *pelem, const OV_TICKET *pticket);

/*
*	Set licence infos
*   -----------------
*/
OV_DLLFNCEXPORT void fb_setlicinfo(
     char*      filename
    ,OV_STRING* licinfo
    ,OV_STRING* version
);

/*
*	Get FB start state
*   ------------------
*/
int fb_started(void);


/*	----------------------------------------------------------------------	*/

#ifdef __cplusplus
}
#endif

#endif
