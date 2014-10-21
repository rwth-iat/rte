
/******************************************************************************
*
*   FILE
*   ----
*   uaClientHandler.c
*
*   History
*   -------
*   2014-10-13   File created
*
*******************************************************************************
*
*   This file is generated by the 'acplt_builder' command
*
******************************************************************************/


#ifndef OV_COMPILE_LIBRARY_iec62541
#define OV_COMPILE_LIBRARY_iec62541
#endif


#include "iec62541.h"
#include "iec62541.ovt"
#include "iec62541_config.h"
#include "libov/ov_macros.h"
#include "ks_logfile.h"
#include "ksbase_helper.h"
#include "ua_server.h"

void UA_Server_processBinaryMessage(UA_Server *server, UA_Connection *connection, const UA_ByteString *msg);
UA_Int32 UA_Connection_init(UA_Connection *connection, UA_ConnectionConfig localConf, void *callbackHandle,
                            UA_Connection_closeCallback close, UA_Connection_writeCallback write);
extern OV_INSTPTR_iec62541_uaServer iec62541_pUaServer;

void UA_append (KS_DATAPACKET *dataPacket, const UA_ByteStringArray buf){
	OV_UINT iterator = 0;
	for(iterator = 0; iterator < buf.stringsSize; iterator++){
		ksbase_KSDATAPACKET_append(dataPacket, buf.strings[iterator].data, buf.strings[iterator].length);
	}
}
void UA_Free  (KS_DATAPACKET *dataPacket){
	ksbase_free_KSDATAPACKET(dataPacket);
}

OV_DLLFNCEXPORT OV_UINT iec62541_uaClientHandler_serverProtocolVersion_get(
    OV_INSTPTR_iec62541_uaClientHandler          pobj
) {
    return (OV_UINT)0;
}

OV_DLLFNCEXPORT OV_RESULT iec62541_uaClientHandler_serverProtocolVersion_set(
    OV_INSTPTR_iec62541_uaClientHandler          pobj,
    const OV_UINT  value
) {
    return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_UINT iec62541_uaClientHandler_serverReceiveBufferSize_get(
    OV_INSTPTR_iec62541_uaClientHandler          pobj
) {
    return (OV_UINT)0;
}

OV_DLLFNCEXPORT OV_RESULT iec62541_uaClientHandler_serverReceiveBufferSize_set(
    OV_INSTPTR_iec62541_uaClientHandler          pobj,
    const OV_UINT  value
) {
    return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_UINT iec62541_uaClientHandler_serverSendBufferSize_get(
    OV_INSTPTR_iec62541_uaClientHandler          pobj
) {
    return (OV_UINT)0;
}

OV_DLLFNCEXPORT OV_RESULT iec62541_uaClientHandler_serverSendBufferSize_set(
    OV_INSTPTR_iec62541_uaClientHandler          pobj,
    const OV_UINT  value
) {
    return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_UINT iec62541_uaClientHandler_serverMaxMessageSize_get(
    OV_INSTPTR_iec62541_uaClientHandler          pobj
) {
    return (OV_UINT)0;
}

OV_DLLFNCEXPORT OV_RESULT iec62541_uaClientHandler_serverMaxMessageSize_set(
    OV_INSTPTR_iec62541_uaClientHandler          pobj,
    const OV_UINT  value
) {
    return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_UINT iec62541_uaClientHandler_serverMaxChunkCount_get(
    OV_INSTPTR_iec62541_uaClientHandler          pobj
) {
    return (OV_UINT)0;
}

OV_DLLFNCEXPORT OV_RESULT iec62541_uaClientHandler_serverMaxChunkCount_set(
    OV_INSTPTR_iec62541_uaClientHandler          pobj,
    const OV_UINT  value
) {
    return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_UINT iec62541_uaClientHandler_clientProtocolVersion_get(
    OV_INSTPTR_iec62541_uaClientHandler          pobj
) {
    return (OV_UINT)0;
}

OV_DLLFNCEXPORT OV_RESULT iec62541_uaClientHandler_clientProtocolVersion_set(
    OV_INSTPTR_iec62541_uaClientHandler          pobj,
    const OV_UINT  value
) {
    return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_UINT iec62541_uaClientHandler_clientReceiveBufferSize_get(
    OV_INSTPTR_iec62541_uaClientHandler          pobj
) {
    return (OV_UINT)0;
}

OV_DLLFNCEXPORT OV_RESULT iec62541_uaClientHandler_clientReceiveBufferSize_set(
    OV_INSTPTR_iec62541_uaClientHandler          pobj,
    const OV_UINT  value
) {
    return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_UINT iec62541_uaClientHandler_clientSendBufferSize_get(
    OV_INSTPTR_iec62541_uaClientHandler          pobj
) {
    return (OV_UINT)0;
}

OV_DLLFNCEXPORT OV_RESULT iec62541_uaClientHandler_clientSendBufferSize_set(
    OV_INSTPTR_iec62541_uaClientHandler          pobj,
    const OV_UINT  value
) {
    return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_UINT iec62541_uaClientHandler_clientMaxMessageSize_get(
    OV_INSTPTR_iec62541_uaClientHandler          pobj
) {
    return (OV_UINT)0;
}

OV_DLLFNCEXPORT OV_RESULT iec62541_uaClientHandler_clientMaxMessageSize_set(
    OV_INSTPTR_iec62541_uaClientHandler          pobj,
    const OV_UINT  value
) {
    return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_UINT iec62541_uaClientHandler_clientMaxChunkCount_get(
    OV_INSTPTR_iec62541_uaClientHandler          pobj
) {
    return (OV_UINT)0;
}

OV_DLLFNCEXPORT OV_RESULT iec62541_uaClientHandler_clientMaxChunkCount_set(
    OV_INSTPTR_iec62541_uaClientHandler          pobj,
    const OV_UINT  value
) {
    return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_RESULT iec62541_uaClientHandler_constructor(
	OV_INSTPTR_ov_object 	pobj
) {
    /*    
    *   local variables
    */
    OV_INSTPTR_iec62541_uaClientHandler pinst = Ov_StaticPtrCast(iec62541_uaClientHandler, pobj);
    OV_RESULT    result;

    /* do what the base class does first */
    result = ksbase_ComTask_constructor(pobj);
    if(Ov_Fail(result))
         return result;

    /* do what */


    return OV_ERR_OK;
}

OV_DLLFNCEXPORT void iec62541_uaClientHandler_destructor(
	OV_INSTPTR_ov_object 	pobj
) {
    /*    
    *   local variables
    */
    OV_INSTPTR_iec62541_uaClientHandler pinst = Ov_StaticPtrCast(iec62541_uaClientHandler, pobj);

    /* do what */

    /* destroy object */
    ksbase_ComTask_destructor(pobj);

    return;
}

OV_DLLFNCEXPORT void iec62541_uaClientHandler_startup(
	OV_INSTPTR_ov_object 	pobj
) {
    /*    
    *   local variables
    */
    OV_INSTPTR_iec62541_uaClientHandler pinst = Ov_StaticPtrCast(iec62541_uaClientHandler, pobj);
    UA_ConnectionConfig UA_ConnectionConfig_default = { .protocolVersion = 0,    .sendBufferSize = 65536,
                                                         .recvBufferSize  = 65536, .maxMessageSize = 65536,
                                                         .maxChunkCount   = 1 };

    /* do what the base class does first */
    ksbase_ClientHandler_startup(pobj);

    /* do what */
    UA_Connection_init(&(pinst->v_connectionData), UA_ConnectionConfig_default, NULL, (UA_Connection_closeCallback)UA_Free, (UA_Connection_writeCallback)UA_append);

    return;
}

OV_DLLFNCEXPORT void iec62541_uaClientHandler_shutdown(
	OV_INSTPTR_ov_object 	pobj
) {
    /*    
    *   local variables
    */
    OV_INSTPTR_iec62541_uaClientHandler pinst = Ov_StaticPtrCast(iec62541_uaClientHandler, pobj);

    /* do what */

    /* set the object's state to "shut down" */
    ksbase_ClientHandler_shutdown(pobj);

    return;
}

OV_DLLFNCEXPORT OV_RESULT iec62541_uaClientHandler_HandleRequest(
	OV_INSTPTR_ksbase_ClientHandler this,
	KS_DATAPACKET* dataReceived,
	KS_DATAPACKET* answer
) {
    /*    
    *   local variables
    */
	OV_INSTPTR_iec62541_uaClientHandler		thisCl			=	Ov_StaticPtrCast(iec62541_uaClientHandler, this);
	UA_ByteString							tempMsg			=	{.data = dataReceived->readPT, .length = ((dataReceived->data + dataReceived->length) - dataReceived->readPT)};
	UA_TcpMessageHeader						tempMsgHeader;
	UA_UInt32								offset			=	0;

	if(!iec62541_pUaServer){
		return OV_ERR_BADFACTORY;
	}

	if(UA_TcpMessageHeader_decodeBinary(&tempMsg, &offset, &tempMsgHeader) != UA_STATUSCODE_GOOD){
		/*	error handling	*/
		KS_logfile_info(("%s: error decoding TcpHeader - message probably corrupted", thisCl->v_identifier));
		ksbase_free_KSDATAPACKET(dataReceived);
		return OV_ERR_GENERIC;
	}

	thisCl->v_connectionData.callbackHandle = answer;
	UA_Server_processBinaryMessage(&(iec62541_pUaServer->v_serverData), &(thisCl->v_connectionData), &tempMsg);


	return OV_ERR_OK;
}
