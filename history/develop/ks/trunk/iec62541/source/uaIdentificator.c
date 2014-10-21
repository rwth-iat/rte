
/******************************************************************************
*
*   FILE
*   ----
*   uaIdentificator.c
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
#include "libov/ov_macros.h"
#include "libov/ov_result.h"
#include "iec62541_config.h"
#include "ks_logfile.h"
#include "ua_transport_generated.h"
#include "ksbase_helper.h"


OV_DLLFNCEXPORT OV_RESULT iec62541_uaIdentificator_constructor(
	OV_INSTPTR_ov_object 	pobj
) {
    /*    
    *   local variables
    */
    OV_INSTPTR_iec62541_uaIdentificator pinst = Ov_StaticPtrCast(iec62541_uaIdentificator, pobj);
    OV_RESULT    result;

    /* do what the base class does first */
    result = ov_object_constructor(pobj);
    if(Ov_Fail(result))
         return result;

    /* do what */
    return ov_string_setvalue(&(pinst->v_protocolID), KSOPC_IDENTIFIER);
}

OV_DLLFNCEXPORT OV_BOOL iec62541_uaIdentificator_identify (
	OV_INSTPTR_ksbase_ProtocolIdentificator this,
	OV_INSTPTR_ksbase_Channel pchannel
) {
	OV_INSTPTR_iec62541_uaIdentificator thisId = Ov_StaticPtrCast(iec62541_uaIdentificator, this);
	UA_TcpHelloMessage	testMsg;
	UA_ByteString		msgSource = {.data = pchannel->v_inData.readPT, .length = ((pchannel->v_inData.data + pchannel->v_inData.length) - pchannel->v_inData.readPT)};
	UA_UInt32			offset = 4;
	UA_UInt32			msgLength;
	OV_UINT				iterator;
	OV_UINT				cmpLength;


	KS_logfile_debug(("%s: identify called with length: %u - data\n%s", thisId->v_identifier, msgSource.length, msgSource.data));

	cmpLength = ov_string_getlength(thisId->v_ident);
	for(iterator = 0; iterator < (cmpLength - 1) && iterator < msgSource.length; iterator++){
		if(msgSource.data[iterator] != thisId->v_ident[iterator]){
			KS_logfile_debug(("%s: first bytes in Message do not match ident", thisId->v_identifier));
			return FALSE;
		}
	}

	if(UA_UInt32_decodeBinary(&msgSource, &offset, &msgLength) != UA_STATUSCODE_GOOD){
		KS_logfile_debug(("%s: could not decode length in HelloMessage header", thisId->v_identifier));
		return FALSE;
	}

	if(msgLength != msgSource.length){
		KS_logfile_debug(("%s: length field in HelloMessage Header does not match length of Message", thisId->v_identifier));
		return FALSE;
	}

	if(UA_TcpHelloMessage_decodeBinary(&msgSource, &offset, &testMsg) == UA_STATUSCODE_GOOD){
		KS_logfile_debug(("%s: HelloMessage successfully decoded", thisId->v_identifier));
		return TRUE;
	} else {
		KS_logfile_debug(("%s: could not decode HelloMessage", thisId->v_identifier));
		return FALSE;
	}
}

static OV_UINT UAClientHandlerNamecounter = 0;

OV_DLLFNCEXPORT OV_RESULT iec62541_uaIdentificator_createClientHandler (
	OV_INSTPTR_ksbase_ProtocolIdentificator this,
	OV_INSTPTR_ksbase_Channel pchannel
) {
    /*    
    *   local variables
    */
	OV_INSTPTR_iec62541_uaClientHandler pClientHandler = NULL;

	char CHNameBuffer[32]; //"OPC_UAClientHandler + length MAXINT + '\0' + 1 Byte spare
	OV_RESULT result;
	KS_logfile_debug(("%s: creating uaClientHandler...", this->v_identifier));

	//get first free "OPC_UAClientHandler"-name
	do {
		pClientHandler = NULL;
		UAClientHandlerNamecounter++;
		sprintf(CHNameBuffer, "OPC_UAClientHandler%" OV_PRINT_UINT, UAClientHandlerNamecounter);
		pClientHandler	= (OV_INSTPTR_iec62541_uaClientHandler) Ov_SearchChild(ov_containment, Ov_StaticPtrCast(ov_domain, this), CHNameBuffer);
	} while (pClientHandler);

	result = Ov_CreateObject(iec62541_uaClientHandler, pClientHandler, this, CHNameBuffer);
	if(Ov_OK(result))
	{
		KS_logfile_debug(("%s: ClientHandler created: %s", this->v_identifier, pClientHandler->v_identifier));
		result = Ov_Link(ksbase_AssocChannelClientHandler, pchannel, pClientHandler);
		if(Ov_Fail(result))
		{
			KS_logfile_error(("%s: could not link ClientHandler %s to Channel %s", this->v_identifier, pClientHandler->v_identifier, pchannel->v_identifier));
			return result;
		}
		else
		{
			result = ov_string_setvalue(&(pClientHandler->v_sourceAdr), pchannel->v_address);
			if(Ov_Fail(result))
			{
				ov_memstack_lock();
				KS_logfile_error(("%s: could not set ClientHandler %s's sourceAdr. reason: %s", this->v_identifier, pClientHandler->v_identifier, ov_result_getresulttext(result)));
				ov_memstack_unlock();
			}
			return OV_ERR_OK;
		}
	}
	else
	{
		ov_memstack_lock();
		KS_logfile_error(("%s: could not create ClientHandler: %s", this->v_identifier, ov_result_getresulttext(result)));
		ov_memstack_unlock();
		return result;
	}
}

