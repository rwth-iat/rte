
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


#ifndef OV_COMPILE_LIBRARY_opcua
#define OV_COMPILE_LIBRARY_opcua
#endif

#include <stddef.h>
#include "opcua.h"
#include "libov/ov_macros.h"
#include "libov/ov_result.h"
#include "ks_logfile.h"
#include "opcua_helpers.h"
#include "ksbase_helper.h"
#include "open62541.h"

/*****************************/
/* Generic Buffer Management */
/*****************************/

static UA_StatusCode GetMallocedBuffer(UA_Connection *connection, size_t length, UA_ByteString *buf) {
	if(length > connection->remoteConf.recvBufferSize)
		return UA_STATUSCODE_BADCOMMUNICATIONERROR;
	return UA_ByteString_allocBuffer(buf, length);
}

static void ReleaseMallocedBuffer(UA_Connection *connection, UA_ByteString *buf) {
	UA_ByteString_deleteMembers(buf);
}

static void ReleaseReceiveBuffer(UA_Connection *connection, UA_ByteString *buf) {
	if (connection->handle != NULL)
		ksbase_free_KSDATAPACKET(&((OV_INSTPTR_opcua_uaConnection)connection->handle)->v_buffer);
	UA_ByteString_deleteMembers(buf);
}

/*	callback functions	*/

static UA_StatusCode ov_ua_connection_write(UA_Connection *connection, UA_ByteString *buf) {
    OV_INSTPTR_opcua_uaConnection	pConnection = (OV_INSTPTR_opcua_uaConnection)(connection->handle);
    OV_INSTPTR_ksbase_Channel			pChannel	=	Ov_GetParent(ksbase_AssocChannelClientHandler, pConnection);
    OV_VTBLPTR_ksbase_Channel			pVtblChannel	=	NULL;
    OV_RESULT	result;

    result = ksbase_KSDATAPACKET_append(&(pChannel->v_outData), buf->data, buf->length);
    if(Ov_Fail(result)){
    	return ov_resultToUaStatusCode(result);
    }
    ReleaseMallocedBuffer(connection, buf);
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
		OV_INSTPTR_ksbase_Channel pChannel = Ov_GetParent(ksbase_AssocChannelClientHandler, Ov_StaticPtrCast(opcua_uaConnection, connection->handle));
		Ov_DeleteObject(Ov_StaticPtrCast(opcua_uaConnection, connection->handle));
		if(pChannel){
			Ov_DeleteObject(pChannel);
		}
	}
	Ov_HeapFree(connection);
	connection = NULL;
}



static OV_RESULT create_UA_Connection(OV_INSTPTR_opcua_uaConnection pinst){
		pinst->v_connection = Ov_HeapMalloc(sizeof(UA_Connection));
	    if(!pinst->v_connection){
	    	ov_logfile_error("%s: could not allocate memory for UA_Connection. Aborting", pinst->v_identifier);
	    	return OV_ERR_HEAPOUTOFMEMORY;
	    }
	    memset(pinst->v_connection, 0, sizeof(UA_Connection));
	    pinst->v_connection->sockfd = 0;
	    pinst->v_connection->handle = pinst;
	    pinst->v_connection->localConf = getOvNetworkLayer()->v_localConfig;
	    pinst->v_connection->send = ov_ua_connection_write;
	    pinst->v_connection->close = ov_ua_connection_closeConnection;
	    pinst->v_connection->getSendBuffer = GetMallocedBuffer;
	    pinst->v_connection->releaseSendBuffer = ReleaseMallocedBuffer;
	    pinst->v_connection->releaseRecvBuffer = ReleaseReceiveBuffer;
	    pinst->v_connection->state = UA_CONNECTION_OPENING;
	    return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_RESULT opcua_uaConnection_constructor(
	OV_INSTPTR_ov_object 	pobj
) {
    /*
    *   local variables
    */
    OV_INSTPTR_opcua_uaConnection pinst = Ov_StaticPtrCast(opcua_uaConnection, pobj);
    OV_RESULT    result;

    /* do what the base class does first */
    result = ksbase_ComTask_constructor(pobj);
    if(Ov_Fail(result))
         return result;

    /* do what */
    pinst->v_connection = NULL;
    return result;
}

OV_DLLFNCEXPORT void opcua_uaConnection_destructor(
	OV_INSTPTR_ov_object 	pobj
) {
    /*
    *   local variables
    */
//    OV_INSTPTR_opcua_uaConnection pinst = Ov_StaticPtrCast(opcua_uaConnection, pobj);

    /* do what */

    /* destroy object */
    ksbase_ComTask_destructor(pobj);

    return;
}

OV_DLLFNCEXPORT void opcua_uaConnection_startup(
	OV_INSTPTR_ov_object 	pobj
) {
    /*
    *   local variables
    */
	OV_INSTPTR_opcua_uaConnection pinst = Ov_StaticPtrCast(opcua_uaConnection, pobj);
	OV_RESULT result;

    /* do what the base class does first */
    ksbase_ClientHandler_startup(pobj);

    /* do what */
    result = create_UA_Connection(pinst);
    if(Ov_Fail(result)){
    	ov_logfile_error("%s - startup: failed to create UA_Connection. Reason: %s", pinst->v_identifier, ov_result_getresulttext(result));
    }
    return;
}

OV_DLLFNCEXPORT void opcua_uaConnection_shutdown(
	OV_INSTPTR_ov_object 	pobj
) {
    /*
    *   local variables
    */
    OV_INSTPTR_opcua_uaConnection pinst = Ov_StaticPtrCast(opcua_uaConnection, pobj);

    /* do what */
    pinst->v_connection->handle = NULL;
    opcua_ovNetworklayer_addConnToClose(pinst->v_connection);
    /* set the object's state to "shut down" */
    ksbase_ClientHandler_shutdown(pobj);
    ksbase_free_KSDATAPACKET(&(pinst->v_buffer));
    return;
}

OV_DLLFNCEXPORT OV_RESULT opcua_uaConnection_HandleRequest(
	OV_INSTPTR_ksbase_ClientHandler this,
	OV_INSTPTR_ksbase_Channel pChannel,
	KS_DATAPACKET* dataReceived,
	KS_DATAPACKET* answer
) {
    /*
    *   local variables
    */
	OV_RESULT result;
	OV_INSTPTR_opcua_uaConnection	pConnection	=	Ov_StaticPtrCast(opcua_uaConnection, this);
//KS_logfile_debug(("uaConnection %s: HandleRequest. Received %u bytes", this->v_identifier, dataReceived->length));
	result = ksbase_KSDATAPACKET_append(&(pConnection->v_buffer), dataReceived->data, dataReceived->length);
	if(Ov_Fail(result)){
		KS_logfile_error(("%s - HandleRequest: failed to copy dataPacket. Reason: %s", this->v_identifier, ov_result_getresulttext(result)));
	}
	ksbase_free_KSDATAPACKET(dataReceived);
	pConnection->v_workNext = true;
	return OV_ERR_OK;
}

OV_DLLFNCEXPORT void opcua_uaConnection_typemethod (
	OV_INSTPTR_ksbase_ComTask	this
) {
    /*
    *   local variables
    */

}
