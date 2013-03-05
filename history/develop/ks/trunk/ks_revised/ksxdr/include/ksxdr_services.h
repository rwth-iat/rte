

#include "ksxdr.h"

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

/*	Client side	*/
OV_RESULT ksxdr_generateClientMessageHeader(OV_UINT procedureNumber, KS_DATAPACKET* datapacket, OV_UINT* xid);

OV_RESULT ksxdr_processServerReplyHeader(KS_DATAPACKET* datapacket, OV_UINT* xid, OV_INT* msgAccepted, OV_INT* msgStatus);

/*	some useful functions	*/
OV_BOOL bufferHoldsCompleteRequest(KS_DATAPACKET* dataReceived, OV_BYTE* BeginOfMessage, OV_RESULT* result);

OV_RESULT unfragmentXDRmessage(KS_DATAPACKET* dataReceived, OV_BYTE* BeginOfMessage);

