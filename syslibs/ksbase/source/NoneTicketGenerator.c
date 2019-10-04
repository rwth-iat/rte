
/******************************************************************************
*
*   FILE
*   ----
*   NoneTicketGenerator.c
*
*   History
*   -------
*   2013-02-22   File created
*
*******************************************************************************
*
*   This file is generated by the 'acplt_builder' command
*
******************************************************************************/


#ifndef OV_COMPILE_LIBRARY_ksbase
#define OV_COMPILE_LIBRARY_ksbase
#endif


#include "ksbase.h"
#include "ov_macros.h"
#include "ov_ksserver_backend.h"

OV_DLLFNCEXPORT void ksbase_NoneTicketGenerator_startup(
	OV_INSTPTR_ov_object 	pobj
) {
    /*
    *   local variables
    */

    /* do what the base class does first */
    ov_object_startup(pobj);

    /* do what */
    ((OV_INSTPTR_ksbase_NoneTicketGenerator) pobj)->v_TicketType = OV_TT_NONE;

    return;
}

OV_DLLFNCEXPORT OV_RESULT ksbase_NoneTicketGenerator_encodeTicket(
	OV_INSTPTR_ksbase_ClientTicketGenerator this,
	KS_DATAPACKET* datapacket
) {
    return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_RESULT ksbase_NoneTicketGenerator_decodeReply(
	OV_INSTPTR_ksbase_ClientTicketGenerator this,
	KS_DATAPACKET* datapacket
) {
    return OV_ERR_OK;
}