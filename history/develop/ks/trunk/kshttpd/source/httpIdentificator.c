#ifndef OV_COMPILE_LIBRARY_kshttpd
#define OV_COMPILE_LIBRARY_kshttpd
#endif

#include "kshttpd.h"
#include "libov/ov_macros.h"

/**
 * in order to detect a http request, we check if a white list of HTTP methods are provided
 */
OV_DLLFNCEXPORT OV_BOOL kshttpd_httpIdentificator_identify (
	OV_INSTPTR_ksbase_ProtocolIdentificator this,
	OV_INSTPTR_ksbase_Channel pchannel
) {
	OV_INSTPTR_kshttpd_httpIdentificator
		pObj = Ov_StaticPtrCast(kshttpd_httpIdentificator, this);
	OV_UINT i = 0;
	OV_UINT length = 0;

	//minimal length of request is 14
	//GET / HTTP/1.1
	//we need to detect all HTTP methods
	if(pchannel->v_inData.length < 15){
		return FALSE;
	}
	//{"GET", "HEAD", "PUT", "PROPPATCH", "PROPFIND", "DELETE", "MOVE", "LINK", "UNLINK", "OPTIONS"}
	for(i = 0;pObj->v_AllowedMethods.veclen;i++){
		length = ov_string_getlength(pObj->v_AllowedMethods.value[i]);
		//ov_string_getlength returns 0 if pointer is NULL
		if(length == 0){
			continue;
		}
		if(strncmp(pObj->v_AllowedMethods.value[i], (char*)pchannel->v_inData.data, (int)length) == OV_STRCMP_EQUAL){
			return TRUE;
		}
	}
	return FALSE;
}

/**
*	functions to create the ClientHandler corresponding to the protocol. This is calles after a successfull identification. The function has to create the ClientHandler
*	and Link it to the Channel calling it with AssocChannelClientHandler.
*/
OV_DLLFNCEXPORT OV_RESULT kshttpd_httpIdentificator_createClientHandler (
	OV_INSTPTR_ksbase_ProtocolIdentificator this,
	OV_INSTPTR_ksbase_Channel pchannel
) {
	OV_INSTPTR_kshttpd_httpclienthandler
		pClient = NULL;
	OV_RESULT fr = OV_ERR_OK;

	fr = Ov_CreateObject(kshttpd_httpclienthandler, pClient, &pdb->root, "test");
	if(Ov_Fail(fr)){
		return fr;
	}
	//todo
	//fr = Ov_Link(ksbase_AssocChannelClientHandler, pchannel, pClient);
	return fr;
}

