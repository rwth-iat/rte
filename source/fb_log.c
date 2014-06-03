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
*   fb_task.c                                                                 *
*                                                                             *
*                                                                             *
*   Historie                                                                  *
*   --------                                                                  *
*   1999-08-07 Alexander Neugebauer: Erstellung, LTSoft GmbH, Kerpen          *
*                                    Lehrstuhl fuer Prozessleittechnik, Aachen*
*                                                                             *
*   1999-10-11 V1.21 cyctime in Task-Objekt                                   *
*   1999-10-25 V1.21 Aufruf von Unterobjekten mit ltc, bei cyctime == 0       *
*   2001-06-12 Major revision.        A.Neugebauer                            *
*                                                                             *
*   Beschreibung                                                              *
*   ------------                                                              *
*   Implementierung der FB-Klasse task                                        *
*                                                                             *
******************************************************************************/

#include "fb.h"
#include "libov/ov_macros.h"
#include "libov/ov_memstack.h"
#include "libov/ov_ov.h"
#include "fb_log.h"

/*
*	Service log
*/
static FB_EXECLOG_TABLE_ENTRY	*plogentry = NULL;

/*
*	Get/Set entry for service logger
*/
OV_DLLFNCEXPORT FB_EXECLOG_TABLE_ENTRY* FbExecLog_getLogEntry(void) {
	return plogentry;
}
OV_DLLFNCEXPORT OV_RESULT FbExecLog_setLogEntry(
	FB_EXECLOG_TABLE_ENTRY	*pentry
) {
	plogentry = pentry;
	return OV_ERR_OK;
}

/*	----------------------------------------------------------------------	*/
/*
*   Start logger
*/
OV_DLLFNCEXPORT void FbSvcLog_startexeclog(OV_INSTPTR_ov_object pobj, OV_STRING msg) {
    /* Logger registriert? */
	if(!plogentry) {
        return;
    }
    plogentry->vtbl->startlog(pobj, msg);
}
/*	----------------------------------------------------------------------	*/
/*
*   Print info message
*/
OV_DLLFNCEXPORT void FbSvcLog_printexecitem(OV_INSTPTR_ov_object pobj, OV_STRING msg) {
    /* Logger registriert? */
	if(!plogentry) {
        return;
    }
    plogentry->vtbl->printexecmsg(pobj, msg);
}
OV_DLLFNCEXPORT void FbSvcLog_printsvc(OV_SVC svc, OV_STRING ip, OV_STRING msg, OV_RESULT res) {
    /* Logger registriert? */
	if(!plogentry) {
        return;
    }
    plogentry->vtbl->printsvcmsg(svc, ip, msg, res);

}

/*	----------------------------------------------------------------------	*/
/*
*   Incr/Decr indent
*/
OV_DLLFNCEXPORT void FbSvcLog_incrIndent(void) {
    /* Logger registriert? */
	if(!plogentry) {
        return;
    }
    plogentry->vtbl->incrindent();
}
OV_DLLFNCEXPORT void FbSvcLog_decrIndent(void) {
    /* Logger registriert? */
	if(!plogentry) {
        return;
    }
    plogentry->vtbl->decrindent();
}

