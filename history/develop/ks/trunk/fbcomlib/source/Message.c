#ifndef OV_COMPILE_LIBRARY_fbcomlib
#define OV_COMPILE_LIBRARY_fbcomlib
#endif


#include "fbcomlib.h"
#include "msgconfig.h"

OV_DLLFNCEXPORT OV_STRING fbcomlib_Message_senderAddress_get(
    OV_INSTPTR_fbcomlib_Message          pobj
) {
    return pobj->v_senderAddress;
}

OV_DLLFNCEXPORT OV_RESULT fbcomlib_Message_senderAddress_set(
    OV_INSTPTR_fbcomlib_Message          pobj,
    const OV_STRING  value
) {
    return ov_string_setvalue(&pobj->v_senderAddress,value);
}

OV_DLLFNCEXPORT OV_STRING fbcomlib_Message_senderName_get(
    OV_INSTPTR_fbcomlib_Message          pobj
) {
    return pobj->v_senderName;
}

OV_DLLFNCEXPORT OV_RESULT fbcomlib_Message_senderName_set(
    OV_INSTPTR_fbcomlib_Message          pobj,
    const OV_STRING  value
) {
    return ov_string_setvalue(&pobj->v_senderName,value);
}

OV_DLLFNCEXPORT OV_STRING fbcomlib_Message_receiverAddress_get(
    OV_INSTPTR_fbcomlib_Message          pobj
) {
    return pobj->v_receiverAddress;
}

OV_DLLFNCEXPORT OV_RESULT fbcomlib_Message_receiverAddress_set(
    OV_INSTPTR_fbcomlib_Message          pobj,
    const OV_STRING  value
) {
    return ov_string_setvalue(&pobj->v_receiverAddress,value);
}

OV_DLLFNCEXPORT OV_STRING fbcomlib_Message_receiverName_get(
    OV_INSTPTR_fbcomlib_Message          pobj
) {
    return pobj->v_receiverName;
}

OV_DLLFNCEXPORT OV_RESULT fbcomlib_Message_receiverName_set(
    OV_INSTPTR_fbcomlib_Message          pobj,
    const OV_STRING  value
) {
    return ov_string_setvalue(&pobj->v_receiverName,value);
}

OV_DLLFNCEXPORT OV_STRING fbcomlib_Message_senderService_get(
    OV_INSTPTR_fbcomlib_Message          pobj
) {
    return pobj->v_senderService;
}

OV_DLLFNCEXPORT OV_RESULT fbcomlib_Message_senderService_set(
    OV_INSTPTR_fbcomlib_Message          pobj,
    const OV_STRING  value
) {
    return ov_string_setvalue(&pobj->v_senderService,value);
}

OV_DLLFNCEXPORT OV_STRING fbcomlib_Message_receiverService_get(
    OV_INSTPTR_fbcomlib_Message          pobj
) {
    return pobj->v_receiverService;
}

OV_DLLFNCEXPORT OV_RESULT fbcomlib_Message_receiverService_set(
    OV_INSTPTR_fbcomlib_Message          pobj,
    const OV_STRING  value
) {
    return ov_string_setvalue(&pobj->v_receiverService,value);
}


OV_DLLFNCEXPORT OV_INT fbcomlib_Message_msgType_get(
    OV_INSTPTR_fbcomlib_Message          pobj
) {
    return pobj->v_msgType;
}

OV_DLLFNCEXPORT OV_RESULT fbcomlib_Message_msgType_set(
    OV_INSTPTR_fbcomlib_Message          pobj,
    const OV_INT  value
) {
	pobj->v_msgType = value;
    return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_UINT fbcomlib_Message_msgID_get(
    OV_INSTPTR_fbcomlib_Message          pobj
) {
    return pobj->v_msgID;
}

OV_DLLFNCEXPORT OV_RESULT fbcomlib_Message_msgID_set(
    OV_INSTPTR_fbcomlib_Message          pobj,
    const OV_UINT  value
) {
    pobj->v_msgID = value;
    return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_INT fbcomlib_Message_msgStatus_get(
    OV_INSTPTR_fbcomlib_Message          pobj
) {
    return pobj->v_msgStatus;
}

OV_DLLFNCEXPORT OV_RESULT fbcomlib_Message_msgStatus_set(
    OV_INSTPTR_fbcomlib_Message          pobj,
    const OV_INT  value
) {
    pobj->v_msgStatus = value;
    return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_STRING fbcomlib_Message_msgBody_get(
    OV_INSTPTR_fbcomlib_Message          pobj
) {
    return pobj->v_msgBody;
}

OV_DLLFNCEXPORT OV_RESULT fbcomlib_Message_msgBody_set(
    OV_INSTPTR_fbcomlib_Message          pobj,
    const OV_STRING  value
) {
    return ov_string_setvalue(&pobj->v_msgBody,value);
}

OV_DLLFNCEXPORT OV_RESULT fbcomlib_Message_constructor(
	OV_INSTPTR_ov_object 	pobj
) {
	OV_INSTPTR_fbcomlib_Message this = Ov_StaticPtrCast(fbcomlib_Message, pobj);
	OV_UINT ID = 4294967295;
	OV_STRING servername = NULL;
	OV_VAR_CURRENT_PROPS srvnameprops;

	ov_object_constructor(pobj);

	srand ( time(NULL) );
	LOCALMSGCOUNTER = LOCALMSGCOUNTER + 1;
	while(ID>=4294967295){
		ID = (((rand() % 1625)+1) * ((rand() % 1625)+1) * ((rand() % 1625)+1)) + LOCALMSGCOUNTER;
	}
	fbcomlib_Message_msgID_set(this,ID);

	ov_vendortree_getservername(&srvnameprops, NULL);
	ov_string_setvalue(&servername,srvnameprops.value.valueunion.val_string);
	fbcomlib_Message_senderName_set(this,servername);

	//Collect Garbage
	ov_string_setvalue(&servername,NULL);

    return OV_ERR_OK;
}

/**
 * Serializes the current message locally to v_sendString
 */
OV_DLLFNCEXPORT void fbcomlib_Message_serializeMessage(
	OV_INSTPTR_fbcomlib_Message pobj
) {
	OV_STRING value = NULL;
	OV_STRING status = NULL;
	OV_STRING id = NULL;
	OV_STRING data = NULL;

	if(pobj->v_msgBody == NULL) ov_string_setvalue(&data,"empty");
	else ov_string_setvalue(&data,pobj->v_msgBody);
	ov_string_print(&value,"%s##%s##%s##%s##%s##%s##%d##%lu##%d##%s",
															   pobj->v_senderAddress,
															   pobj->v_senderName,
															   pobj->v_senderService,
															   pobj->v_receiverAddress,
															   pobj->v_receiverName,
															   pobj->v_receiverService,
															   pobj->v_msgStatus,
															   pobj->v_msgID,
															   pobj->v_msgType,
															   data );
	//fbcomlib_Message_sendString_set(pobj,value);
	ov_string_setvalue(&pobj->v_sendString,value);

	ov_logfile_debug("MsgSys/serializeMessage: Generated sendString '%s'",pobj->v_sendString);

	//Collecting Garbage
	ov_string_setvalue(&value,NULL);
	ov_string_setvalue(&status,NULL);
	ov_string_setvalue(&id,NULL);
	ov_string_setvalue(&data,NULL);
	return;
}

OV_DLLFNCEXPORT void fbcomlib_Message_deserializeMessage(
	OV_INSTPTR_fbcomlib_Message pobj
) {
	OV_STRING sendString = NULL;
	OV_STRING data = NULL;

	//ov_string_setvalue(&sendString,fbcomlib_Message_sendString_get(pobj));
	ov_string_setvalue(&sendString,pobj->v_sendString);
	ov_logfile_debug("MsgSys/deserializeMessage: Called with sendString '%s'",pobj->v_sendString);

	fbcomlib_Message_senderAddress_set(pobj,fbcomlib_Message_getSenderAddress(sendString));
	fbcomlib_Message_senderName_set(pobj,fbcomlib_Message_getSenderName(sendString));
	fbcomlib_Message_senderService_set(pobj,fbcomlib_Message_getSenderService(sendString));
	fbcomlib_Message_receiverAddress_set(pobj,fbcomlib_Message_getReceiverAddress(sendString));
	fbcomlib_Message_receiverName_set(pobj,fbcomlib_Message_getReceiverName(sendString));
	fbcomlib_Message_receiverService_set(pobj,fbcomlib_Message_getReceiverService(sendString));

	fbcomlib_Message_msgStatus_set(pobj,atoi(fbcomlib_Message_getMsgStatus(sendString)));
	fbcomlib_Message_msgID_set(pobj,(unsigned)atol(fbcomlib_Message_getMsgID(sendString)));
	fbcomlib_Message_msgType_set(pobj,atoi(fbcomlib_Message_getMsgType(sendString)));

	//ov_string_setvalue(&data,fbcomlib_Message_getMsgData(fbcomlib_Message_sendString_get(pobj)));
	ov_string_setvalue(&data,fbcomlib_Message_getMsgData(sendString));

	if(data == NULL) fbcomlib_Message_msgBody_set(pobj,"empty");
	else fbcomlib_Message_msgBody_set(pobj,data);
	return;
}



OV_DLLVAREXPORT OV_STRING fbcomlib_Message_getSenderAddress(
	const OV_STRING value
) {	
	OV_INT position = 0;
	OV_INT cnt = 0;
	OV_STRING token = NULL;

	for(;cnt <= 0; cnt++){
		MSG_split(value,"##",&position,&token);
	}

	return token;
}

OV_DLLVAREXPORT OV_STRING fbcomlib_Message_getSenderName(
	const OV_STRING value
) {	
	OV_INT position = 0;
	OV_INT cnt = 0;
	OV_STRING token = NULL;

	for(;cnt <= 1; cnt++){
		MSG_split(value,"##",&position,&token);
	}

	return token;
}

OV_DLLVAREXPORT OV_STRING fbcomlib_Message_getSenderService(
	const OV_STRING value
) {	
	OV_INT position = 0;
	OV_INT cnt = 0;
	OV_STRING token = NULL;
	
	for(;cnt <= 2; cnt++){
		MSG_split(value,"##",&position,&token);
	}

	return token;
}

OV_DLLVAREXPORT OV_STRING fbcomlib_Message_getReceiverAddress(
	const OV_STRING value
) {	
	OV_INT position = 0;
	OV_INT cnt = 0;
	OV_STRING token = NULL;

	for(;cnt <= 3; cnt++){
		MSG_split(value,"##",&position,&token);
	}

	return token;
}

OV_DLLVAREXPORT OV_STRING fbcomlib_Message_getReceiverName(
	const OV_STRING value
) {	
	OV_INT position = 0;
	OV_INT cnt = 0;
	OV_STRING token = NULL;

	for(;cnt <= 4; cnt++){
		MSG_split(value,"##",&position,&token);
	}

	return token;
}

OV_DLLVAREXPORT OV_STRING fbcomlib_Message_getReceiverService(
	const OV_STRING value
) {	
	OV_INT position = 0;
	OV_INT cnt = 0;
	OV_STRING token = NULL;

	for(;cnt <= 5; cnt++){
		MSG_split(value,"##",&position,&token);
	}

	return token;
}

OV_DLLVAREXPORT OV_STRING fbcomlib_Message_getMsgStatus(
	const OV_STRING value
) {	
	OV_INT position = 0;
	OV_INT cnt = 0;
	OV_STRING token = NULL;

	for(;cnt <= 6; cnt++){
		MSG_split(value,"##",&position,&token);
	}

	return token;
}

OV_DLLVAREXPORT OV_STRING fbcomlib_Message_getMsgID(
	const OV_STRING value
) {	
	OV_INT position = 0;
	OV_INT cnt = 0;
	OV_STRING token = NULL;

	for(;cnt <= 7; cnt++){
		MSG_split(value,"##",&position,&token);
	}

	return token;
}

OV_DLLVAREXPORT OV_STRING fbcomlib_Message_getMsgType(
	const OV_STRING value
) {	
	OV_INT position = 0;
	OV_INT cnt = 0;
	OV_STRING token = NULL;

	for(;cnt <= 8; cnt++){
		MSG_split(value,"##",&position,&token);
	}

	return token;
}

OV_DLLVAREXPORT OV_STRING fbcomlib_Message_getMsgData(
	const OV_STRING value
) {	
	OV_INT position = 0;
	OV_INT cnt = 0;
	OV_STRING token = NULL;

	for(;cnt <= 9; cnt++){
		MSG_split(value,"##",&position,&token);
	}

	return token;
}
