#ifndef OV_COMPILE_LIBRARY_MessageSys
#define OV_COMPILE_LIBRARY_MessageSys
#endif


#include "MessageSys.h"
//#include "ServiceProviderConfig.h"

#include "libov/ov_macros.h"
#include "MessageSys_helpers.h"
#include <time.h>

static OV_INT LOCALMSGCOUNTER;

OV_DLLFNCEXPORT OV_STRING MessageSys_Message_senderAddress_get(
    OV_INSTPTR_MessageSys_Message          pobj
) {
    return pobj->v_senderAddress;
}

OV_DLLFNCEXPORT OV_RESULT MessageSys_Message_senderAddress_set(
    OV_INSTPTR_MessageSys_Message          pobj,
    const OV_STRING  value
) {
    return ov_string_setvalue(&pobj->v_senderAddress,value);
}

OV_DLLFNCEXPORT OV_STRING MessageSys_Message_senderName_get(
    OV_INSTPTR_MessageSys_Message          pobj
) {
    return pobj->v_senderName;
}

OV_DLLFNCEXPORT OV_RESULT MessageSys_Message_senderName_set(
    OV_INSTPTR_MessageSys_Message          pobj,
    const OV_STRING  value
) {
    return ov_string_setvalue(&pobj->v_senderName,value);
}

OV_DLLFNCEXPORT OV_STRING MessageSys_Message_receiverAddress_get(
    OV_INSTPTR_MessageSys_Message          pobj
) {
    return pobj->v_receiverAddress;
}

OV_DLLFNCEXPORT OV_RESULT MessageSys_Message_receiverAddress_set(
    OV_INSTPTR_MessageSys_Message          pobj,
    const OV_STRING  value
) {
    return ov_string_setvalue(&pobj->v_receiverAddress,value);
}

OV_DLLFNCEXPORT OV_STRING MessageSys_Message_receiverName_get(
    OV_INSTPTR_MessageSys_Message          pobj
) {
    return pobj->v_receiverName;
}

OV_DLLFNCEXPORT OV_RESULT MessageSys_Message_receiverName_set(
    OV_INSTPTR_MessageSys_Message          pobj,
    const OV_STRING  value
) {
    return ov_string_setvalue(&pobj->v_receiverName,value);
}

OV_DLLFNCEXPORT OV_STRING MessageSys_Message_senderComponent_get(
    OV_INSTPTR_MessageSys_Message          pobj
) {
    return pobj->v_senderComponent;
}

OV_DLLFNCEXPORT OV_RESULT MessageSys_Message_senderComponent_set(
    OV_INSTPTR_MessageSys_Message          pobj,
    const OV_STRING  value
) {
    return ov_string_setvalue(&pobj->v_senderComponent,value);
}

OV_DLLFNCEXPORT OV_STRING MessageSys_Message_receiverComponent_get(
    OV_INSTPTR_MessageSys_Message          pobj
) {
    return pobj->v_receiverComponent;
}

OV_DLLFNCEXPORT OV_RESULT MessageSys_Message_receiverComponent_set(
    OV_INSTPTR_MessageSys_Message          pobj,
    const OV_STRING  value
) {
    return ov_string_setvalue(&pobj->v_receiverComponent,value);
}


OV_DLLFNCEXPORT OV_UINT MessageSys_Message_msgID_get(
    OV_INSTPTR_MessageSys_Message          pobj
) {
    return pobj->v_msgID;
}

OV_DLLFNCEXPORT OV_RESULT MessageSys_Message_msgID_set(
    OV_INSTPTR_MessageSys_Message          pobj,
    const OV_UINT  value
) {
    pobj->v_msgID = value;
    return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_INT MessageSys_Message_msgStatus_get(
    OV_INSTPTR_MessageSys_Message          pobj
) {
    return pobj->v_msgStatus;
}

OV_DLLFNCEXPORT OV_RESULT MessageSys_Message_msgStatus_set(
    OV_INSTPTR_MessageSys_Message          pobj,
    const OV_INT  value
) {
    pobj->v_msgStatus = value;
    return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_STRING MessageSys_Message_msgBody_get(
    OV_INSTPTR_MessageSys_Message          pobj
) {
    return pobj->v_msgBody;
}

OV_DLLFNCEXPORT OV_RESULT MessageSys_Message_msgBody_set(
    OV_INSTPTR_MessageSys_Message          pobj,
    const OV_STRING  value
) {
    return ov_string_setvalue(&pobj->v_msgBody,value);
}

OV_DLLFNCEXPORT OV_RESULT MessageSys_Message_constructor(
	OV_INSTPTR_ov_object 	pobj
) {
	OV_INSTPTR_MessageSys_Message this = Ov_StaticPtrCast(MessageSys_Message, pobj);
	OV_UINT ID = 4294967295;
	OV_STRING servername = NULL;
	OV_VAR_CURRENT_PROPS srvnameprops;

	ov_object_constructor(pobj);

	srand ( time(NULL) );
	LOCALMSGCOUNTER = LOCALMSGCOUNTER + 1;
	while(ID>=4294967295){
		ID = (((rand() % 1625)+1) * ((rand() % 1625)+1) * ((rand() % 1625)+1)) + LOCALMSGCOUNTER;
	}
	MessageSys_Message_msgID_set(this,ID);

	ov_vendortree_getservername(&srvnameprops, NULL);
	ov_string_setvalue(&servername,srvnameprops.value.valueunion.val_string);
	MessageSys_Message_senderName_set(this,servername);

	//Collect Garbage
	ov_string_setvalue(&servername,NULL);

    return OV_ERR_OK;
}

OV_DLLFNCEXPORT void MessageSys_Message_destructor(
	OV_INSTPTR_ov_object 	pobj
) {
    /*
    *   local variables
    */
    OV_INSTPTR_MessageSys_Message this = Ov_StaticPtrCast(MessageSys_Message, pobj);

    /* do what */
    ov_string_setvalue(&this->v_msgBody, NULL);
    ov_string_setvalue(&this->v_sendString,NULL);

    /* destroy object */
    ov_object_destructor(pobj);

    return;
}

/**
 * Serializes the current message locally to v_sendString
 */
OV_DLLFNCEXPORT void MessageSys_Message_serializeMessage(
	OV_INSTPTR_MessageSys_Message pobj
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
															   pobj->v_senderComponent,
															   pobj->v_receiverAddress,
															   pobj->v_receiverName,
															   pobj->v_receiverComponent,
															   pobj->v_msgStatus,
															   pobj->v_msgID,
															   0,
															   data );
	//MessageSys_Message_sendString_set(pobj,value);
	ov_string_setvalue(&pobj->v_sendString,value);

	//Collecting Garbage
	ov_string_setvalue(&value,NULL);
	ov_string_setvalue(&status,NULL);
	ov_string_setvalue(&id,NULL);
	ov_string_setvalue(&data,NULL);
	return;
}

OV_DLLFNCEXPORT void MessageSys_Message_deserializeMessage(
	OV_INSTPTR_MessageSys_Message pobj
) {
	OV_STRING data = NULL;
	OV_STRING senderAddress = NULL;
	OV_STRING senderName = NULL;
	OV_STRING senderComponent = NULL;
	OV_STRING receiverAddress = NULL;
	OV_STRING receiverName = NULL;
	OV_STRING receiverComponent = NULL;
	OV_STRING msgStatus = NULL;
	OV_STRING msgID = NULL;

	ov_memstack_lock();

	senderAddress = MessageSys_Message_getSenderAddress(pobj->v_sendString);
	MessageSys_Message_senderAddress_set(pobj, senderAddress);

	senderName = MessageSys_Message_getSenderName(pobj->v_sendString);
	MessageSys_Message_senderName_set(pobj, senderName);

	senderComponent = MessageSys_Message_getSenderComponent(pobj->v_sendString);
	MessageSys_Message_senderComponent_set(pobj,senderComponent);

	receiverAddress = MessageSys_Message_getReceiverAddress(pobj->v_sendString);
	MessageSys_Message_receiverAddress_set(pobj, receiverAddress);

	receiverName = MessageSys_Message_getReceiverName(pobj->v_sendString);
	MessageSys_Message_receiverName_set(pobj, receiverName);

	receiverComponent = MessageSys_Message_getReceiverComponent(pobj->v_sendString);
	MessageSys_Message_receiverComponent_set(pobj, receiverComponent);

	msgStatus = MessageSys_Message_getMsgStatus(pobj->v_sendString);
	MessageSys_Message_msgStatus_set(pobj,atoi(msgStatus));

	msgID = MessageSys_Message_getMsgID(pobj->v_sendString);
	MessageSys_Message_msgID_set(pobj,(unsigned)atol(msgID));

	//ov_string_setvalue(&data,MessageSys_Message_getMsgData(MessageSys_Message_pobj->v_sendString_get(pobj)));
	//ov_string_setvalue(&data,MessageSys_Message_getMsgData(pobj->v_sendString));
	data = MessageSys_Message_getMsgData(pobj->v_sendString);

	if(data == NULL) MessageSys_Message_msgBody_set(pobj,"empty");
	else MessageSys_Message_msgBody_set(pobj,data);


	ov_memstack_unlock();
	return;
}

OV_DLLVAREXPORT OV_STRING MessageSys_Message_getSenderAddress(
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

OV_DLLVAREXPORT OV_STRING MessageSys_Message_getSenderName(
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

OV_DLLVAREXPORT OV_STRING MessageSys_Message_getSenderComponent(
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

OV_DLLVAREXPORT OV_STRING MessageSys_Message_getReceiverAddress(
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

OV_DLLVAREXPORT OV_STRING MessageSys_Message_getReceiverName(
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

OV_DLLVAREXPORT OV_STRING MessageSys_Message_getReceiverComponent(
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

OV_DLLVAREXPORT OV_STRING MessageSys_Message_getMsgStatus(
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

OV_DLLVAREXPORT OV_STRING MessageSys_Message_getMsgID(
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

OV_DLLVAREXPORT OV_STRING MessageSys_Message_getMsgType(
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

OV_DLLVAREXPORT OV_STRING MessageSys_Message_getMsgData(
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


OV_DLLFNCEXPORT OV_STRING MessageSys_Message_msgSTRING_get(
    OV_INSTPTR_MessageSys_Message          pobj
) {
    return pobj->v_msgSTRING;
}

OV_DLLFNCEXPORT OV_RESULT MessageSys_Message_msgSTRING_set(
    OV_INSTPTR_MessageSys_Message          pobj,
    const OV_STRING  value
) {
    return ov_string_setvalue(&pobj->v_msgSTRING,value);
}
