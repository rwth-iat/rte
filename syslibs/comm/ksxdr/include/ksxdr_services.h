

#include "ksxdr.h"

/****************************************************************************************************************************************************************************
 * 			Server Side
 ****************************************************************************************************************************************************************************/
OV_RESULT ksxdr_getserver(const OV_UINT version, const OV_TICKET* pticket, KS_DATAPACKET* dataReceived, KS_DATAPACKET* serviceAnswer, OV_UINT* msgState, OV_UINT* ksErrCode);

OV_RESULT ksxdr_register(const OV_UINT version, const OV_TICKET* pticket, KS_DATAPACKET* dataReceived, KS_DATAPACKET* serviceAnswer, OV_UINT* msgState, OV_UINT* ksErrCode);

OV_RESULT ksxdr_unregister(const OV_UINT version, const OV_TICKET* pticket, KS_DATAPACKET* dataReceived, KS_DATAPACKET* serviceAnswer, OV_UINT* msgState, OV_UINT* ksErrCode);

OV_RESULT ksxdr_getEP(const OV_UINT version, const OV_TICKET* pticket, KS_DATAPACKET* dataReceived, KS_DATAPACKET* serviceAnswer, OV_UINT* msgState, OV_UINT* ksErrCode);

OV_RESULT ksxdr_getPP(const OV_UINT version, const OV_TICKET* pticket, KS_DATAPACKET* dataReceived, KS_DATAPACKET* serviceAnswer, OV_UINT* msgState, OV_UINT* ksErrCode);

OV_RESULT ksxdr_getVar(const OV_UINT version, const OV_TICKET* pticket, KS_DATAPACKET* dataReceived, KS_DATAPACKET* serviceAnswer, OV_UINT* msgState, OV_UINT* ksErrCode);

OV_RESULT ksxdr_setVar(const OV_UINT version, const OV_TICKET* pticket, KS_DATAPACKET* dataReceived, KS_DATAPACKET* serviceAnswer, OV_UINT* msgState, OV_UINT* ksErrCode);

OV_RESULT ksxdr_createObject(const OV_UINT version, const OV_TICKET* pticket, KS_DATAPACKET* dataReceived, KS_DATAPACKET* serviceAnswer, OV_UINT* msgState, OV_UINT* ksErrCode);

OV_RESULT ksxdr_link(const OV_UINT version, const OV_TICKET* pticket, KS_DATAPACKET* dataReceived, KS_DATAPACKET* serviceAnswer, OV_UINT* msgState, OV_UINT* ksErrCode);

OV_RESULT ksxdr_deleteObject(const OV_UINT version, const OV_TICKET* pticket, KS_DATAPACKET* dataReceived, KS_DATAPACKET* serviceAnswer, OV_UINT* msgState, OV_UINT* ksErrCode);

OV_RESULT ksxdr_unlink(const OV_UINT version, const OV_TICKET* pticket, KS_DATAPACKET* dataReceived, KS_DATAPACKET* serviceAnswer, OV_UINT* msgState, OV_UINT* ksErrCode);

OV_RESULT ksxdr_getCanonicalPath(const OV_UINT version, const OV_TICKET* pticket, KS_DATAPACKET* dataReceived, KS_DATAPACKET* serviceAnswer, OV_UINT* msgState, OV_UINT* ksErrCode);

OV_RESULT ksxdr_renameObject(const OV_UINT version, const OV_TICKET* pticket, KS_DATAPACKET* dataReceived, KS_DATAPACKET* serviceAnswer, OV_UINT* msgState, OV_UINT* ksErrCode);

/****************************************************************************************************************************************************************************
 * 			Client Side
 ****************************************************************************************************************************************************************************/

OV_RESULT ksxdr_generateClientMessageHeader(OV_UINT procedureNumber, OV_BOOL usesStreamProtocol, KS_DATAPACKET* datapacket, OV_UINT* xid, OV_UINT* pindex);

OV_RESULT ksxdr_processServerReplyHeader(KS_DATAPACKET* datapacket, const OV_INSTPTR_ksbase_ClientTicketGenerator TicketGenerator,
		OV_UINT expectedXID, OV_BOOL usesStreamProtocol, OV_UINT* xid, OV_INT* msgAccepted, OV_INT* msgStatus, OV_UINT* functionStatus);

OV_RESULT ksxdr_getChannelPointer(OV_INSTPTR_ksbase_DataHandler this, OV_INSTPTR_ksbase_Channel* ppChannel, OV_VTBLPTR_ksbase_Channel* ppVtblChannel);

OV_RESULT ksxdr_initiateConnection(OV_INSTPTR_ksxdr_xdrClient this, OV_INSTPTR_ksbase_Channel pChannel, OV_VTBLPTR_ksbase_Channel pVtblChannel,
		OV_BOOL isLocal, OV_STRING host, OV_STRING port);

OV_RESULT ksxdr_trySend(OV_INSTPTR_ksxdr_xdrClient thisCl, OV_INSTPTR_ksbase_Channel pChannel, OV_VTBLPTR_ksbase_Channel pVtblChannel);

OV_RESULT ksxdr_insertTicket(KS_DATAPACKET* datapacket, const OV_INSTPTR_ksbase_ClientTicketGenerator TicketGenerator);

OV_RESULT ksxdr_readBackTicket(KS_DATAPACKET* datapacket, const OV_INSTPTR_ksbase_ClientTicketGenerator TicketGenerator);

OV_RESULT ksxdr_checkPMAskList(OV_INSTPTR_ksxdr_xdrClient thisCl);

/****************************************************************************************************************************************************************************
 * 			some useful functions
 ****************************************************************************************************************************************************************************/

OV_BOOL bufferHoldsCompleteRequest(KS_DATAPACKET* dataReceived, OV_BYTE* BeginOfMessage, OV_RESULT* result);

OV_RESULT unfragmentXDRmessage(KS_DATAPACKET* dataReceived, OV_BYTE* BeginOfMessage);

void ksxdr_prepend_length(KS_DATAPACKET* answer, OV_UINT begin);

/*	callbackfunction for processing the getserver at the beginning of a ks-service call	*/
void GetServerCallback(const OV_INSTPTR_ov_domain this, const OV_INSTPTR_ov_domain that);


OV_RESULT generate_PMAPPROC(KS_DATAPACKET* datapacket, OV_BOOL usesStreamProtocol, OV_UINT command, OV_UINT protocol, OV_UINT port);

OV_RESULT process_getPort(KS_DATAPACKET* datapacket, OV_BOOL usesStreamProtocol, OV_STRING* port);
