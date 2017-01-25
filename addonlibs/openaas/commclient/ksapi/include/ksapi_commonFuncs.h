
#include "ksapi.h"

OV_RESULT ksapi_getClientPointers(OV_INSTPTR_ksapi_KSApiCommon pCommon,
				OV_INSTPTR_ksbase_ClientBase* pClient, OV_VTBLPTR_ksbase_ClientBase* pVtblClient);


#define KSAPI_COMMON_INTERNALERROR			128
#define KSAPI_COMMON_EXTERNALERROR			64
#define KSAPI_COMMON_INITIAL				0
#define KSAPI_COMMON_WAITINGFORANSWER		1
#define KSAPI_COMMON_REQUESTCOMPLETED		2
