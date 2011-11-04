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
*   fb_database.h                                                             *
*                                                                             *
*                                                                             *
*   Historie                                                                  *
*   --------                                                                  *
*   08-07-1999 Alexander Neugebauer: Erstellung, LTSoft GmbH, Kerpen          *
*                                    Lehrstuhl fuer Prozessleittechnik, Aachen*
*                                                                             *
*                                                                             *
*   Beschreibung                                                              *
*   ------------                                                              *
*   Funktions-Prototypen fuer Anlegen und Mappen der Database                 *
*                                                                             *
******************************************************************************/

#ifndef FB_DATBASE_H_INCLUDED
#define FB_DATBASE_H_INCLUDED

#include "libov/ov_ov.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
*	Create a new database
*/
OV_DLLFNCEXPORT OV_RESULT fb_database_create(
	OV_STRING	filename,
	OV_UINT		size
);

/*
*	Map an existing database
*/
OV_DLLFNCEXPORT OV_RESULT fb_database_map(
	OV_STRING	filename
);

/*
*	Unmap the database
*/
OV_DLLFNCEXPORT void fb_database_unmap(void);

/*
*	Flush the contents of a database
*/
OV_DLLFNCEXPORT void fb_database_flush(void);

/*
*	Start up the database
*/
OV_DLLFNCEXPORT OV_RESULT fb_database_startup(void);

/*
*	Shut down the database
*/
OV_DLLFNCEXPORT void fb_database_shutdown(void);

/*
*	Check (fb) database structure
*/
OV_DLLFNCEXPORT OV_RESULT fb_database_checkstruct(void);

/*
*	Get UrTask object
*/
OV_DLLFNCEXPORT void* fb_database_geturtask(void);

/*
*	Get libraries container object
*/
OV_DLLFNCEXPORT void* fb_database_getlibcontainer(void);

/*
*	Get dbinfo object
*/
OV_DLLFNCEXPORT void* fb_database_getdbinfo(void);
void  fb_database_nildbinfo(void);

/*
*	Test, if a string is a valid identifier
*/
OV_DLLFNCEXPORT OV_BOOL fb_database_identifierok(
	char*		identifier
);

#ifdef __cplusplus
}	/* extern "C" */
#endif

#endif
/*
*	End of file
*/
