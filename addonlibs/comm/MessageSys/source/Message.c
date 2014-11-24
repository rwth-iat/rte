#ifndef OV_COMPILE_LIBRARY_MessageSys
#define OV_COMPILE_LIBRARY_MessageSys
#endif


#include "MessageSys.h"
//#include "ServiceProviderConfig.h"

#include "libov/ov_macros.h"
#include "MessageSys_helpers.h"
#include <time.h>

static OV_INT LOCALMSGCOUNTER;

OV_DLLFNCEXPORT OV_ACCESS MessageSys_Message_getaccess(
	OV_INSTPTR_ov_object		pobj,
	const OV_ELEMENT			*pelem,
	const OV_TICKET				*pticket
) {
	/*
	*	local variables
	*/

	/*
	*	switch based on the element's type
	*/
	switch(pelem->elemtype) {
		case OV_ET_VARIABLE:
			if(pelem->elemunion.pvar->v_offset >= offsetof(OV_INST_ov_object,__classinfo)) {
			  if(pelem->elemunion.pvar->v_vartype == OV_VT_CTYPE)
				  return OV_AC_NONE;
			  else
				  return OV_AC_READWRITE;
			}
			break;
		default:
			break;
	}

	return ov_object_getaccess(pobj, pelem, pticket);
}

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


OV_DLLFNCEXPORT OV_STRING MessageSys_Message_msgID_get(
    OV_INSTPTR_MessageSys_Message          pobj
) {
    return pobj->v_msgID;
}

OV_DLLFNCEXPORT OV_RESULT MessageSys_Message_msgID_set(
    OV_INSTPTR_MessageSys_Message          pobj,
    const OV_STRING  value
) {
	return ov_string_setvalue(&pobj->v_msgID,value);
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
	OV_ANY srvnameprops;

	ov_object_constructor(pobj);

	srand ( time(NULL) );
	LOCALMSGCOUNTER = LOCALMSGCOUNTER + 1;
	while(ID>=4294967295){
		ID = (((rand() % 1625)+1) * ((rand() % 1625)+1) * ((rand() % 1625)+1)) + LOCALMSGCOUNTER;
	}
	ov_string_print(&this->v_msgID, "%lu", ID);

	ov_vendortree_getservername(&srvnameprops, NULL);
	ov_string_setvalue(&servername,srvnameprops.value.valueunion.val_string);
	MessageSys_Message_senderName_set(this,servername);

	//Collect Garbage
	ov_string_setvalue(&servername,NULL);
	this->v_sendBy = MSG_SEND_KSSETVAR;

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
    /*	delete an associated channel object	*/
    if(Ov_GetChild(MessageSys_Message2Channel, this)){
    	Ov_DeleteObject(Ov_GetChild(MessageSys_Message2Channel, this));
    }

    /* destroy object */
    ov_object_destructor(pobj);

    return;
}




