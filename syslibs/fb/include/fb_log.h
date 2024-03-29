#ifndef _FB_LOG_INCLUDE_
#define _FB_LOG_INCLUDE_

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
*   fb_log.h                                                                  *
*                                                                             *
*   Historie                                                                  *
*   --------                                                                  *
*   2010-11-19  Alexander Neugebauer: Erstellung, LTSoft, Kerpen              *
*                                                                             *
*                                                                             *
*   Beschreibung                                                              *
*   ------------                                                              *
*   Funktions-Prototypen fuer Server-Logging                                  *
*                                                                             *
******************************************************************************/

#include "ov_ov.h"


#include "ov_ksserver_backend.h"


#ifdef __cplusplus
extern "C" {
#endif



/*
*	FB_SVCLOG_VTBL:
*	---------------
*	VTable for service logging
*/
struct FB_EXECLOG_VTBL {
    void        (* startlog)(OV_INSTPTR_ov_object pobj, OV_STRING msg);
    void        (* incrindent)(void);
    void        (* decrindent)(void);
    void        (* printexecmsg)(OV_INSTPTR_ov_object pobj, OV_STRING msg);
    void        (* printsvcmsg)(OV_SVC svc, OV_STRING ip, OV_STRING msg, OV_RESULT res);
};
typedef struct FB_EXECLOG_VTBL	FB_EXECLOG_VTBL;

struct FB_EXECLOG_TABLE_ENTRY {
	const FB_EXECLOG_VTBL *vtbl;
};
typedef struct FB_EXECLOG_TABLE_ENTRY FB_EXECLOG_TABLE_ENTRY;

/*
*	Get/Set entry for service logger
*/
OV_DLLFNCEXPORT FB_EXECLOG_TABLE_ENTRY* FbExecLog_getLogEntry(void);
OV_DLLFNCEXPORT OV_RESULT FbExecLog_setLogEntry(FB_EXECLOG_TABLE_ENTRY	*plogentry);
/* Start logger */
OV_DLLFNCEXPORT void FbSvcLog_startexeclog(OV_INSTPTR_ov_object pobj, OV_STRING msg);
/* Print info message */
OV_DLLFNCEXPORT void FbSvcLog_printexecitem(OV_INSTPTR_ov_object pobj, OV_STRING msg);
/* Print service message */
OV_DLLFNCEXPORT void FbSvcLog_printsvc(OV_SVC svc, OV_STRING ip, OV_STRING msg, OV_RESULT res);
/* Incr/Decr indent */
OV_DLLFNCEXPORT void FbSvcLog_incrIndent(void);
OV_DLLFNCEXPORT void FbSvcLog_decrIndent(void);

/*	----------------------------------------------------------------------	*/
#ifdef __cplusplus
}
#endif

#endif
