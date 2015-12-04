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
*   fb_macros.h - Header-Datei fuer iFBSpro-Model                             *
*                                                                             *
*   Historie                                                                  *
*   --------                                                                  *
*   1998-02-22  Alexander Neugebauer: Erstellung, LTSoft, Kerpen              *
*                                                                             *
*   Beschreibung                                                              *
*   ------------                                                              *
*   Makro-Definitionen                                                        *
*                                                                             *
******************************************************************************/

#ifndef _FB_MACROS_H_
#define _FB_MACROS_H_

/*
*	Helper macro: is flag X set?
*/
#define IsFlagSet(flags, name)	(flags & (1L << (name-'a')))
/*
*	Helper macro: set new flag X.
*/
#define SetNewFlag(flags, name)	(flags = flags | (1L << (name-'a')))

#endif
