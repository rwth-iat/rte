
/******************************************************************************
*
*   FILE
*   ----
*   genericHttpClient.c
*
*   History
*   -------
*   2013-08-07   File created
*
*******************************************************************************
*
*   This file is generated by the 'acplt_builder' command
*
******************************************************************************/


#ifndef OV_COMPILE_LIBRARY_kshttp
#define OV_COMPILE_LIBRARY_kshttp
#endif


#include "kshttp.h"
#include "config.h"

void kshttp_genericHttpClient_Callback(OV_INSTPTR_ov_domain instanceCalled, OV_INSTPTR_ov_domain instanceCalling);


OV_DLLFNCEXPORT OV_RESULT kshttp_genericHttpClient_constructor(
	OV_INSTPTR_ov_object 	pobj
) {
	/*
	*   local variables
	*/
	OV_INSTPTR_kshttp_genericHttpClient thisCl = Ov_StaticPtrCast(kshttp_genericHttpClient, pobj);
	OV_RESULT    result;

	/* do what the base class does first */
	result = ksbase_ComTask_constructor(pobj);
	if(Ov_Fail(result)){
		return result;
	}

	ov_string_setvalue(&thisCl->v_serverPort, "80");
	ov_string_setvalue(&thisCl->v_serverName, "<unused>");

	return OV_ERR_OK;
}



OV_DLLFNCEXPORT OV_RESULT kshttp_genericHttpClient_beginCommunication_set(
	OV_INSTPTR_kshttp_genericHttpClient          thisCl,
	const OV_BOOL  value
) {
	return kshttp_generateAndSendHttpMessage(thisCl->v_method, thisCl->v_serverHost, thisCl->v_serverPort, thisCl->v_requestUri, 0, NULL, Ov_PtrUpCast(kshttp_httpClientBase, thisCl), Ov_PtrUpCast(ov_domain, thisCl), &kshttp_genericHttpClient_Callback);
}

void kshttp_genericHttpClient_Callback(OV_INSTPTR_ov_domain instanceCalled, OV_INSTPTR_ov_domain instanceCalling){
	OV_INSTPTR_kshttp_genericHttpClient	thisCl = Ov_DynamicPtrCast(kshttp_genericHttpClient, instanceCalling);
	OV_RESULT result = OV_ERR_OK;
	OV_INSTPTR_ksbase_Channel 	pChannel = NULL;
	OV_VTBLPTR_ksbase_Channel	pVtblChannel = NULL;

	result = getChannelPointer(Ov_PtrUpCast(kshttp_httpClientBase, thisCl), &pChannel, &pVtblChannel);
	if(Ov_Fail(result))
	{
		KS_logfile_error(("%s: Could not get Channel pointers.", this->v_identifier));
		return;
	}
	ov_string_setvalue(&thisCl->v_contentType, thisCl->v_ServerResponse.contentType);

	if(	ov_string_match(thisCl->v_contentType, "text/*") == TRUE ||
			ov_string_match(thisCl->v_contentType, "*application/xml*") == TRUE ||	//includes application/xml-dtd
			ov_string_match(thisCl->v_contentType, "*+xml*") == TRUE ||	//includes image/svg+xml
			ov_string_match(thisCl->v_contentType, "application/json*") == TRUE ||
			ov_string_match(thisCl->v_contentType, "application/javascript*") == TRUE ){
		if(thisCl->v_ServerResponse.contentLength != 0){
			thisCl->v_messageBody =  ov_database_malloc(thisCl->v_ServerResponse.contentLength+1);
			if(!thisCl->v_messageBody){
				//database too small
				thisCl->v_httpParseStatus = HTTP_MSG_DBOUTOFMEMORY;

				//this memory is from the ksdatapackage!
				thisCl->v_ServerResponse.messageBody = NULL;
				pVtblChannel->m_CloseConnection(pChannel);
				return;
			}
			memcpy(thisCl->v_messageBody, thisCl->v_ServerResponse.messageBody, thisCl->v_ServerResponse.contentLength);
			thisCl->v_messageBody[thisCl->v_ServerResponse.contentLength] = '\0';
		}
	}else{
		ov_string_setvalue(&thisCl->v_messageBody, "binary mimetype");
	}

	//this memory is from the ksdatapackage!
	thisCl->v_ServerResponse.messageBody = NULL;
	pVtblChannel->m_CloseConnection(pChannel);

	return;
}
