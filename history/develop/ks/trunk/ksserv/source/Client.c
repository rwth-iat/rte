/** @file 
* This class takes over the whole communication with a connected client.
* Objects of this class are created by the tcpconnection-object if a
* connection-request is comming on the at ks_manager registered port.
******************************************************************************/
#ifndef OV_COMPILE_LIBRARY_ksserv
#define OV_COMPILE_LIBRARY_ksserv
#endif


#include "ksserv.h"
#include "libov/ov_macros.h"
#include "libov/ov_path.h"
#include "ksserv_logfile.h"

/**
 * Returns the number of KS calls, which were received from this socket.
 */
OV_DLLFNCEXPORT OV_INT ksserv_Client_receivedCalls_get(
             OV_INSTPTR_ksserv_Client          pobj
) {
             return pobj->v_receivedCalls;
}

/**
 * Returns the Adr of the socket
 */
OV_DLLFNCEXPORT OV_STRING ksserv_Client_sourceAdr_get(
             OV_INSTPTR_ksserv_Client          pobj
) {
             return pobj->v_sourceAdr;
}

/**
 * Returns the OVTicket String of the current call 
 */
OV_DLLFNCEXPORT OV_STRING ksserv_Client_ovticket_get(
             OV_INSTPTR_ksserv_Client          pobj
) {
		if(pobj->v_ovticket == NULL) {
			return "";
		}
        return pobj->v_ovticket;
}

/** 
 * Inherited classes could simply call this method for setting themself as current client
 */
OV_DLLFNCEXPORT void ksserv_Client_setThisAsCurrent(
	     OV_INSTPTR_ksserv_Client	client
) {
	OV_INSTPTR_ksserv_comcontainer	communicationdomain = NULL;
	
	//ksserv_logfile_debug("ksserv/Client: %s from %s is current client.", client->v_identifier, client->v_sourceAdr);
	communicationdomain = (OV_INSTPTR_ksserv_comcontainer)ov_path_getobjectpointer("/communication", 2);
	Ov_Link(ksserv_AssocCurrentClient,communicationdomain,client);	
	return ;	
}
/** 
 * Inherited classes could simply call this method for unsetting themself as current client
 * Unsets also the ovticket variable as it is not important anymore
 */
OV_DLLFNCEXPORT void ksserv_Client_unsetThisAsCurrent(
	     OV_INSTPTR_ksserv_Client	client
) {
	OV_INSTPTR_ksserv_comcontainer	communicationdomain = NULL;
	
	//ksserv_logfile_debug("ksserv/Client: %s is NOT current client anymore.", client->v_identifier);
	communicationdomain = (OV_INSTPTR_ksserv_comcontainer)ov_path_getobjectpointer("/communication", 2);
	Ov_Unlink(ksserv_AssocCurrentClient,communicationdomain,client);	
	ov_string_setvalue(&client->v_ovticket, "");
	return ;	
}

