
/******************************************************************************
*
*   FILE
*   ----
*   uaConnection.c
*
*   History
*   -------
*   2015-06-02   File created
*
*******************************************************************************
*
*   This file is generated by the 'acplt_builder' command
*
******************************************************************************/


#ifndef OV_COMPILE_LIBRARY_iec62541
#define OV_COMPILE_LIBRARY_iec62541
#endif

#include <stddef.h>
#include "iec62541.h"
#include "libov/ov_macros.h"
#include "iec62541_helpers.h"
#include "ksbase_helper.h"

/*	callback functions	*/

static UA_StatusCode ov_ua_connection_write(UA_Connection *connection, const UA_ByteString *buf) {
    OV_INSTPTR_iec62541_uaConnection	pConnection = (OV_INSTPTR_iec62541_uaConnection)(connection->handle);
    OV_INSTPTR_ksbase_Channel			pChannel	=	Ov_GetParent(ksbase_AssocChannelClientHandler, pConnection);
    OV_VTBLPTR_ksbase_Channel			pVtblChannel	=	NULL;
    OV_RESULT	result;

    result = ksbase_KSDATAPACKET_append(&(pChannel->v_outData), buf->data, buf->length);
    if(Ov_Fail(result)){
    	return ov_resultToUaStatusCode(result);
    }
    Ov_GetVTablePtr(ksbase_Channel, pVtblChannel, pChannel);
    if(!pVtblChannel){
    	return UA_STATUSCODE_BADNOTFOUND;
    }
    pVtblChannel->m_SendData(pChannel);
    return UA_STATUSCODE_GOOD;
}

static void ov_ua_connection_closeConnection(UA_Connection *connection) {
	if(connection->state == UA_CONNECTION_CLOSED)
		return;
	connection->state = UA_CONNECTION_CLOSED;
	/*	delete the channel object	*/
	if(connection->handle){
		OV_INSTPTR_ksbase_Channel pChannel = Ov_GetParent(ksbase_AssocChannelClientHandler, Ov_StaticPtrCast(iec62541_uaConnection, connection->handle));
		Ov_DeleteObject(Ov_StaticPtrCast(iec62541_uaConnection, connection->handle));
		if(pChannel){
			Ov_DeleteObject(pChannel);
		}
	}
	Ov_HeapFree(connection);
	connection = NULL;
}

/*****************************/
/* Generic Buffer Management */
/*****************************/

static UA_StatusCode GetMallocedBuffer(UA_Connection *connection, UA_ByteString *buf, size_t minSize) {
    return UA_ByteString_newMembers(buf, minSize);
}

static void ReleaseMallocedBuffer(UA_Connection *connection, UA_ByteString *buf) {
    UA_ByteString_deleteMembers(buf);
}


OV_DLLFNCEXPORT OV_RESULT iec62541_uaConnection_constructor(
	OV_INSTPTR_ov_object 	pobj
) {
    /*    
    *   local variables
    */
    OV_INSTPTR_iec62541_uaConnection pinst = Ov_StaticPtrCast(iec62541_uaConnection, pobj);
    OV_RESULT    result;

    /* do what the base class does first */
    result = ksbase_ComTask_constructor(pobj);
    if(Ov_Fail(result))
         return result;

    /* do what */
    pinst->v_connection = Ov_HeapMalloc(sizeof(UA_Connection));
    if(!pinst->v_connection){
    	ov_logfile_error("%s: could not allocate memory for UA_Connection. Aborting");
    	return OV_ERR_HEAPOUTOFMEMORY;
    }
    UA_Connection_init(pinst->v_connection);
    pinst->v_connection->sockfd = 0;
    pinst->v_connection->handle = pobj;
    pinst->v_connection->localConf = getOvNetworkLayer()->v_localConfig;
    pinst->v_connection->write = ov_ua_connection_write;
    pinst->v_connection->close = ov_ua_connection_closeConnection;
    pinst->v_connection->getBuffer = GetMallocedBuffer;
    pinst->v_connection->releaseBuffer = ReleaseMallocedBuffer;
    pinst->v_connection->state = UA_CONNECTION_OPENING;

    return OV_ERR_OK;
}

OV_DLLFNCEXPORT void iec62541_uaConnection_destructor(
	OV_INSTPTR_ov_object 	pobj
) {
    /*    
    *   local variables
    */
//    OV_INSTPTR_iec62541_uaConnection pinst = Ov_StaticPtrCast(iec62541_uaConnection, pobj);

    /* do what */

    /* destroy object */
    ksbase_ComTask_destructor(pobj);

    return;
}

OV_DLLFNCEXPORT void iec62541_uaConnection_startup(
	OV_INSTPTR_ov_object 	pobj
) {
    /*    
    *   local variables
    */
//  OV_INSTPTR_iec62541_uaConnection pinst = Ov_StaticPtrCast(iec62541_uaConnection, pobj);

    /* do what the base class does first */
    ksbase_ClientHandler_startup(pobj);

    /* do what */

    return;
}

OV_DLLFNCEXPORT void iec62541_uaConnection_shutdown(
	OV_INSTPTR_ov_object 	pobj
) {
    /*    
    *   local variables
    */
    OV_INSTPTR_iec62541_uaConnection pinst = Ov_StaticPtrCast(iec62541_uaConnection, pobj);

    /* do what */
    pinst->v_connection->handle = NULL;
    /* set the object's state to "shut down" */
    ksbase_ClientHandler_shutdown(pobj);

    return;
}

OV_DLLFNCEXPORT OV_RESULT iec62541_uaConnection_HandleRequest(
	OV_INSTPTR_ksbase_ClientHandler this,
	OV_INSTPTR_ksbase_Channel pChannel,
	KS_DATAPACKET* dataReceived,
	KS_DATAPACKET* answer
) {
    /*    
    *   local variables
    */
	OV_INSTPTR_iec62541_uaConnection	pConnection	=	Ov_StaticPtrCast(iec62541_uaConnection, this);
	UA_ByteString						temp;
	UA_ByteString						*received	=	NULL;

	received = UA_ByteString_new();
	if(!received){
		return OV_ERR_HEAPOUTOFMEMORY;
	}
	temp.length = dataReceived->length;
	temp.data = dataReceived->data;
	UA_ByteString_copy(&temp, received);
	pConnection->v_buffer = UA_Connection_completeMessages(pConnection->v_connection, *received);
	pConnection->v_workNext = TRUE;
	ksbase_free_KSDATAPACKET(dataReceived);
    return OV_ERR_OK;
}

OV_DLLFNCEXPORT void iec62541_uaConnection_typemethod (
	OV_INSTPTR_ksbase_ComTask	this
) {
    /*
    *   local variables
    */

}
