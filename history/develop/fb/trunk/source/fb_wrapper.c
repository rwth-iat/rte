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
*   fb_wraper.cpp                                                             *
*                                                                             *
*                                                                             *
*   Historie                                                                  *
*   --------                                                                  *
*   2010-10-14 Alexander Neugebauer: Erstellung, LTSoft GmbH, Kerpen          *
*                                                                             *
*   Beschreibung                                                              *
*   ------------                                                              *
*   C++/Try-Catch wrapper fuer FB-FNC                                         *
*                                                                             *
******************************************************************************/

#ifndef OV_COMPILE_LIBRARY_fb
#define OV_COMPILE_LIBRARY_fb
#endif

#include "libov/ov_database.h"
#include "libov/ov_logfile.h"
#include "libovks/ov_ksserver.h"


#ifdef __cplusplus
extern "C" {
#endif

/*	----------------------------------------------------------------------	*/
/*
*	Map/unmap an existing database
*/
void fb_database_wrapunmap(void) {

#ifdef FB_CATCH_EXCEPTIONS
    try {
        ov_database_unmap();
    } catch(...) {
    }
#else 
    ov_database_unmap();
#endif

    return;
}
/*	----------------------------------------------------------------------	*/
OV_RESULT fb_database_wrapmap(OV_STRING	filename) {
    OV_RESULT res;
        
#ifdef FB_CATCH_EXCEPTIONS
    try {
        res = ov_database_map(filename);
    } catch(...) {
        res = OV_ERR_TARGETGENERIC;
        
        fb_database_wrapunmap();
    }
#else 
    res = ov_database_map(filename);
#endif

    return res;
}

/*	----------------------------------------------------------------------	*/
/*
*	Start up the database
*/
OV_RESULT fb_database_wrapstartup(void) {
    OV_RESULT res;
    
#ifdef FB_CATCH_EXCEPTIONS
    try {
        //res = ov_supervised_database_startup();
        res = ov_database_startup();
    } catch(...) {
        res = OV_ERR_TARGETGENERIC;
    }
#else
    res = ov_database_startup();
#endif

    return res;
}


/*	----------------------------------------------------------------------	*/

#ifdef __cplusplus
}
#endif

/*
*	End of file
*/
