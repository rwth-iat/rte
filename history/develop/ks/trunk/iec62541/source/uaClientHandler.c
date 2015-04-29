
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


static const UA_ConnectionConfig ov_UA_ConnectionConfig_standard =
    {.protocolVersion = 0, .sendBufferSize = 65536, .recvBufferSize  = 65536,
     .maxMessageSize = 65536, .maxChunkCount   = 1};

typedef  void (*UA_Connection_closeCallback)(void *connection);
typedef void (*UA_Connection_writeCallback)(void *connection, UA_ByteStringArray buf);

extern OV_INSTPTR_iec62541_uaServer iec62541_pUaServer;

void UA_Server_processBinaryMessage(UA_Server *server, UA_Connection *connection, const UA_ByteString *msg);

UA_Int32 UA_Connection_init(UA_Connection *connection, UA_ConnectionConfig localConf, void *callbackHandle,
                            UA_Connection_closeCallback close, UA_Connection_writeCallback write){

	connection->localConf = localConf;
	connection->channel = NULL;
	connection->state = UA_CONNECTION_OPENING;
	connection->close = close;
	connection->write = write;
	return UA_STATUSCODE_GOOD;
}


void UA_append (ov_UA_Connection *ovUAConn, const UA_ByteStringArray buf){
	OV_UINT iterator = 0;
	// KS_logfile_debug(("UA_append:\n\tdata:\t\t%p\n\tlength:\t\t%u", buf.strings, buf.stringsSize));
	for(iterator = 0; iterator < buf.stringsSize; iterator++){
		ksbase_KSDATAPACKET_append(ovUAConn->outData, buf.strings[iterator].data, buf.strings[iterator].length);
		// KS_logfile_debug(("\n\ti:\t\t\t%u\n\tstrings[i]:\t\t%p\n\tstrings[i].length:\t%u", iterator, buf.strings[iterator].data, buf.strings[iterator].length));
	}
}
void UA_Free  (ov_UA_Connection *ovUAConn){
	ksbase_free_KSDATAPACKET(ovUAConn->outData);
	ksbase_free_KSDATAPACKET(ovUAConn->inData);
}

OV_DLLFNCEXPORT OV_UINT iec62541_uaClientHandler_serverProtocolVersion_get(
    OV_INSTPTR_iec62541_uaClientHandler          pobj
) {
    return pobj->v_connectionData.connection.localConf.protocolVersion;
}

OV_DLLFNCEXPORT OV_RESULT iec62541_uaClientHandler_serverProtocolVersion_set(
    OV_INSTPTR_iec62541_uaClientHandler          pobj,
    const OV_UINT  value
) {
	pobj->v_connectionData.connection.localConf.protocolVersion = value;
	return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_UINT iec62541_uaClientHandler_serverReceiveBufferSize_get(
    OV_INSTPTR_iec62541_uaClientHandler          pobj
) {
    return pobj->v_connectionData.connection.localConf.recvBufferSize;;
}

OV_DLLFNCEXPORT OV_RESULT iec62541_uaClientHandler_serverReceiveBufferSize_set(
    OV_INSTPTR_iec62541_uaClientHandler          pobj,
    const OV_UINT  value
) {
	pobj->v_connectionData.connection.localConf.recvBufferSize = value;
    return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_UINT iec62541_uaClientHandler_serverSendBufferSize_get(
    OV_INSTPTR_iec62541_uaClientHandler          pobj
) {
	return pobj->v_connectionData.connection.localConf.sendBufferSize;
}

OV_DLLFNCEXPORT OV_RESULT iec62541_uaClientHandler_serverSendBufferSize_set(
    OV_INSTPTR_iec62541_uaClientHandler          pobj,
    const OV_UINT  value
) {
	pobj->v_connectionData.connection.localConf.sendBufferSize = value;
    return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_UINT iec62541_uaClientHandler_serverMaxMessageSize_get(
    OV_INSTPTR_iec62541_uaClientHandler          pobj
) {
    return pobj->v_connectionData.connection.localConf.maxMessageSize;;
}

OV_DLLFNCEXPORT OV_RESULT iec62541_uaClientHandler_serverMaxMessageSize_set(
    OV_INSTPTR_iec62541_uaClientHandler          pobj,
    const OV_UINT  value
) {
	pobj->v_connectionData.connection.localConf.maxMessageSize = value;
    return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_UINT iec62541_uaClientHandler_serverMaxChunkCount_get(
    OV_INSTPTR_iec62541_uaClientHandler          pobj
) {
    return pobj->v_connectionData.connection.localConf.maxChunkCount;
}

OV_DLLFNCEXPORT OV_RESULT iec62541_uaClientHandler_serverMaxChunkCount_set(
    OV_INSTPTR_iec62541_uaClientHandler          pobj,
    const OV_UINT  value
) {
	pobj->v_connectionData.connection.localConf.maxChunkCount = value;
    return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_UINT iec62541_uaClientHandler_clientProtocolVersion_get(
    OV_INSTPTR_iec62541_uaClientHandler          pobj
) {
    return pobj->v_connectionData.connection.remoteConf.protocolVersion;
}

OV_DLLFNCEXPORT OV_RESULT iec62541_uaClientHandler_clientProtocolVersion_set(
    OV_INSTPTR_iec62541_uaClientHandler          pobj,
    const OV_UINT  value
) {
    return OV_ERR_NOACCESS;
}

OV_DLLFNCEXPORT OV_UINT iec62541_uaClientHandler_clientReceiveBufferSize_get(
    OV_INSTPTR_iec62541_uaClientHandler          pobj
) {
	return pobj->v_connectionData.connection.remoteConf.recvBufferSize;
}

OV_DLLFNCEXPORT OV_RESULT iec62541_uaClientHandler_clientReceiveBufferSize_set(
    OV_INSTPTR_iec62541_uaClientHandler          pobj,
    const OV_UINT  value
) {
	return OV_ERR_NOACCESS;
}

OV_DLLFNCEXPORT OV_UINT iec62541_uaClientHandler_clientSendBufferSize_get(
    OV_INSTPTR_iec62541_uaClientHandler          pobj
) {
	return pobj->v_connectionData.connection.remoteConf.sendBufferSize;
}

OV_DLLFNCEXPORT OV_RESULT iec62541_uaClientHandler_clientSendBufferSize_set(
    OV_INSTPTR_iec62541_uaClientHandler          pobj,
    const OV_UINT  value
) {
    return OV_ERR_NOACCESS;
}

OV_DLLFNCEXPORT OV_UINT iec62541_uaClientHandler_clientMaxMessageSize_get(
    OV_INSTPTR_iec62541_uaClientHandler          pobj
) {
	return pobj->v_connectionData.connection.remoteConf.maxMessageSize;
}

OV_DLLFNCEXPORT OV_RESULT iec62541_uaClientHandler_clientMaxMessageSize_set(
    OV_INSTPTR_iec62541_uaClientHandler          pobj,
    const OV_UINT  value
) {
    return OV_ERR_NOACCESS;
}

OV_DLLFNCEXPORT OV_UINT iec62541_uaClientHandler_clientMaxChunkCount_get(
    OV_INSTPTR_iec62541_uaClientHandler          pobj
) {
	return pobj->v_connectionData.connection.remoteConf.maxChunkCount;
}

OV_DLLFNCEXPORT OV_RESULT iec62541_uaClientHandler_clientMaxChunkCount_set(
    OV_INSTPTR_iec62541_uaClientHandler          pobj,
    const OV_UINT  value
) {
    return OV_ERR_NOACCESS;
}

OV_DLLFNCEXPORT OV_RESULT iec62541_uaClientHandler_constructor(
	OV_INSTPTR_ov_object 	pobj
) {
    /*    
    *   local variables
    */
   // OV_INSTPTR_iec62541_uaClientHandler pinst = Ov_StaticPtrCast(iec62541_uaClientHandler, pobj);
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
   // OV_INSTPTR_iec62541_uaClientHandler pinst = Ov_StaticPtrCast(iec62541_uaClientHandler, pobj);

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
    UA_Connection_init(&(pinst->v_connectionData.connection), UA_ConnectionConfig_default, NULL, (UA_Connection_closeCallback)UA_Free, (UA_Connection_writeCallback)UA_append);

    return;
}

OV_DLLFNCEXPORT void iec62541_uaClientHandler_shutdown(
	OV_INSTPTR_ov_object 	pobj
) {
    /*    
    *   local variables
    */
    //OV_INSTPTR_iec62541_uaClientHandler pinst = Ov_StaticPtrCast(iec62541_uaClientHandler, pobj);

    /* do what */

    /* set the object's state to "shut down" */
    ksbase_ClientHandler_shutdown(pobj);

    return;
}
//OV_FNCEXTERN KSBASE_FNC_HANDLEREQUEST iec62541_uaClientHandler_HandleRequest(
OV_DLLFNCEXPORT OV_RESULT iec62541_uaClientHandler_HandleRequest(
		OV_INSTPTR_ksbase_ClientHandler this,
		OV_INSTPTR_ksbase_Channel pChannel,
		KS_DATAPACKET* dataReceived,
		KS_DATAPACKET* answer
) {
    /*    
    *   local variables
    */
	OV_INSTPTR_iec62541_uaClientHandler		thisCl			=	Ov_StaticPtrCast(iec62541_uaClientHandler, this);
	UA_ByteString							tempMsg			=	{.data = dataReceived->readPT, .length = ((dataReceived->data + dataReceived->length) - dataReceived->readPT)};
	UA_TcpMessageHeader						tempMsgHeader;
	size_t									offset			=	0;

	if(!iec62541_pUaServer){
		return OV_ERR_BADFACTORY;
	}

	if(UA_TcpMessageHeader_decodeBinary(&tempMsg, &offset, &tempMsgHeader) != UA_STATUSCODE_GOOD){
		/*	error handling	*/
		KS_logfile_info(("%s: error decoding TcpHeader - message probably corrupted", thisCl->v_identifier));
		ksbase_free_KSDATAPACKET(dataReceived);
		return OV_ERR_GENERIC;
	}

	thisCl->v_connectionData.clientHandler = Ov_StaticPtrCast(ksbase_ClientHandler, thisCl);
	thisCl->v_connectionData.outData = answer;
	thisCl->v_connectionData.inData = dataReceived;

	//	KS_logfile_debug(("-------------\nbefore response creation:\n\tdata:\t\t%p\n\tlength:\t\t%u\n\treadPt:\t\t%p\n\twritePt:\t%p", answer->data, answer->length, answer->readPT, answer->writePT));

	UA_Server_processBinaryMessage(&(iec62541_pUaServer->v_serverData), (UA_Connection*) &(thisCl->v_connectionData), &tempMsg);

	//	KS_logfile_debug(("after response creation:\n\tdata:\t\t%p\n\tlength:\t\t%u\n\treadPt:\t\t%p\n\twritePt:\t%p\n-------------", answer->data, answer->length, answer->readPT, answer->writePT));

	thisCl->v_connectionData.clientHandler = NULL;
	thisCl->v_connectionData.outData = NULL;
	thisCl->v_connectionData.inData = NULL;
	ksbase_free_KSDATAPACKET(dataReceived);
	return OV_ERR_OK;
}

