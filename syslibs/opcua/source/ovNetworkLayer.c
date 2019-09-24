
/******************************************************************************
*
*   FILE
*   ----
*   ovNetworkLayer.c
*
*   History
*   -------
*   2015-06-02   File created
*
*******************************************************************************
*
*   This file is generated by the 'acplt_builder' command
*
******************************************************************************/


#ifndef OV_COMPILE_LIBRARY_opcua
#define OV_COMPILE_LIBRARY_opcua
#endif

#ifndef _WIN32
//	for gethostname
#define _DEFAULT_SOURCE
#include <unistd.h>
#endif


#include "opcua.h"
#include "ov_macros.h"
#include "ks_logfile.h"
#include "ksbase_helper.h"

static OV_INSTPTR_opcua_ovNetworkLayer pOVNetworkLayer	=	NULL;

void opcua_ovNetworklayer_addConnToClose(UA_Connection* connection){
	UA_ConnectionPTRPTR tempPtr = NULL;
	if(pOVNetworkLayer){
		pOVNetworkLayer->v_connsToCloseCount++;
		tempPtr = Ov_HeapRealloc((pOVNetworkLayer->v_connsToClose), (pOVNetworkLayer->v_connsToCloseCount * sizeof(UA_ConnectionPTR)));
		if(!(tempPtr)){
			ov_logfile_error("addConnToClose: could not realloc close list -- this item will be lost");
			pOVNetworkLayer->v_connsToCloseCount--;
			return;
		}
		pOVNetworkLayer->v_connsToClose = tempPtr;
		pOVNetworkLayer->v_connsToClose[pOVNetworkLayer->v_connsToCloseCount - 1] = connection;
	}
}

static void FreeConnection(UA_Server *server, void *pConn) {
   	Ov_HeapFree(pConn);
}

OV_INSTPTR_opcua_ovNetworkLayer getOvNetworkLayer(){
	return pOVNetworkLayer;
}

UA_ServerNetworkLayer ServerNetworkLayerOV_new(UA_ConnectionConfig conf, UA_UInt32 port) {
    OV_INSTPTR_opcua_ovNetworkLayer	pNetworkLayer	=	NULL;
    OV_VTBLPTR_opcua_ovNetworkLayer	pVtblNetworkLayer	=	NULL;
    OV_RESULT							result;
	OV_STRING	tempstr = NULL;
    UA_ServerNetworkLayer nl;
    memset(&nl, 0, sizeof(UA_ServerNetworkLayer));

    result = Ov_CreateIDedObject(opcua_ovNetworkLayer, pNetworkLayer, Ov_StaticPtrCast(ov_domain, Ov_GetFirstChild(ov_instantiation, pclass_opcua_uaServer)), "ovNetworkLayer");
    if(Ov_Fail(result)){
    	ov_logfile_error("ovNetworkLayer - ServerNetworkLayerOV_New: could not Create ov_NetworkLayer instance.");
    	return nl;
    }

    char hostname[256];
    gethostname(hostname, 255);
    if(*hostname){
    	ov_string_print(&tempstr, "opc.tcp://%s:%d", hostname, port);
    } else {
    	ov_string_print(&tempstr, "opc.tcp://%s:%d", "localhost", port);
    }
    if(tempstr){
    	pNetworkLayer->v_discoveryUrlInternal = UA_String_fromChars(tempstr);
    	ov_string_setvalue(&tempstr, NULL);
    }
    //UA_String_copy(&(pNetworkLayer->v_discoveryUrlInternal), &nl.discoveryUrl);

//    pNetworkLayer->v_messageBuffer = (UA_ByteString){.length = conf.maxMessageSize, .data = Ov_HeapMalloc(conf.maxMessageSize)};
//    if(!(pNetworkLayer->v_messageBuffer.data)){
//    	ov_logfile_error("ovNetworkLayer - ServerNetworkLayerOV_New: could not allocate memory for message buffer.");
//    	return nl;
//    }
//    UA_ByteString_newMembers(&(pNetworkLayer->v_sendBuffer), conf.maxMessageSize);
//    if(!pNetworkLayer->v_sendBuffer.data){
//    	ov_logfile_error("ovNetworkLayer - ServerNetworkLayerOV_New: could not allocate memory for send buffer.");
//    	return nl;
//    }
    pNetworkLayer->v_localConfig = conf;
    Ov_GetVTablePtr(opcua_ovNetworkLayer, pVtblNetworkLayer, pNetworkLayer);
    nl.handle = pNetworkLayer;
    nl.start = pVtblNetworkLayer->m_start;
    nl.getJobs = pVtblNetworkLayer->m_getJobs;
    nl.stop = pVtblNetworkLayer->m_stop;
    nl.deleteMembers = pVtblNetworkLayer->m_delete;
    nl.discoveryUrl = pNetworkLayer->v_discoveryUrlInternal;
    return nl;
}


OV_DLLFNCEXPORT OV_STRING opcua_ovNetworkLayer_discoveryUrl_get(
    OV_INSTPTR_opcua_ovNetworkLayer          pobj
) {
	OV_STRING tempString = NULL;
    if(pobj->v_discoveryUrlInternal.length > 0){
    	tempString = ov_memstack_alloc((pobj->v_discoveryUrlInternal.length + 1) * sizeof(char));
    	if(!tempString){
    		return NULL;
    	} else {
    		memcpy(tempString, pobj->v_discoveryUrlInternal.data, pobj->v_discoveryUrlInternal.length);
    		tempString[pobj->v_discoveryUrlInternal.length] = '\0';
    	}
    }
	return tempString;
}

OV_DLLFNCEXPORT OV_RESULT opcua_ovNetworkLayer_constructor(
	OV_INSTPTR_ov_object 	pobj
) {
    /*    
    *   local variables
    */
    OV_INSTPTR_opcua_ovNetworkLayer pinst = Ov_StaticPtrCast(opcua_ovNetworkLayer, pobj);
    OV_INSTPTR_ov_object	pOtherObject	=	NULL;
    OV_RESULT    result;

    /* do what the base class does first */
    result = ov_object_constructor(pobj);
    if(Ov_Fail(result))
         return result;

    /* do what */
    Ov_ForEachChild(ov_instantiation, pclass_opcua_ovNetworkLayer, pOtherObject){
    	if(pOtherObject != pobj){
    		KS_logfile_error(("%s: cannot instantiate - ovNetworkLayer instance already exists", pinst->v_identifier));
    		return OV_ERR_ALREADYEXISTS;
    	}
    }
    return OV_ERR_OK;
}

OV_DLLFNCEXPORT void opcua_ovNetworkLayer_destructor(
	OV_INSTPTR_ov_object 	pobj
) {
    /*    
    *   local variables
    */
//    OV_INSTPTR_opcua_ovNetworkLayer pinst = Ov_StaticPtrCast(opcua_ovNetworkLayer, pobj);

    /* do what */

    /* destroy object */
    ov_object_destructor(pobj);

    return;
}

OV_DLLFNCEXPORT void opcua_ovNetworkLayer_startup(
	OV_INSTPTR_ov_object 	pobj
) {
    /*    
    *   local variables
    */
    OV_INSTPTR_opcua_ovNetworkLayer pinst = Ov_StaticPtrCast(opcua_ovNetworkLayer, pobj);

    /* do what the base class does first */
    ov_object_startup(pobj);

    /* do what */
    pOVNetworkLayer = pinst;
    pinst->v_connsToClose = NULL;
    pinst->v_connsToCloseCount = 0;
    return;
}

OV_DLLFNCEXPORT void opcua_ovNetworkLayer_shutdown(
	OV_INSTPTR_ov_object 	pobj
) {
    /*    
    *   local variables
    */
//    OV_INSTPTR_opcua_ovNetworkLayer pinst = Ov_StaticPtrCast(opcua_ovNetworkLayer, pobj);

    /* do what */
    pOVNetworkLayer = NULL;
    // UA_String_delete(&(pOVNetworkLayer->v_discoveryUrlInternal));
    /* set the object's state to "shut down" */
    ov_object_shutdown(pobj);

    return;
}

OV_DLLFNCEXPORT OV_ACCESS opcua_ovNetworkLayer_getaccess(
	OV_INSTPTR_ov_object	pobj,
	const OV_ELEMENT		*pelem,
	const OV_TICKET			*pticket
) {
	switch(pelem->elemtype) {
	case OV_ET_VARIABLE:
		if(pelem->elemunion.pvar->v_offset >= offsetof(OV_INST_ov_object,__classinfo)) {
			if(pelem->elemunion.pvar->v_vartype == OV_VT_CTYPE)
				return OV_AC_NONE;
			else{
				if((pelem->elemunion.pvar->v_varprops & OV_VP_DERIVED)){
					if((pelem->elemunion.pvar->v_varprops & OV_VP_SETACCESSOR)){
						return OV_AC_READWRITE;
					} else {
						return OV_AC_READ;
					}
				} else {
					return OV_AC_READWRITE;
				}
			}
		}
		break;
	default:
		break;
	}

	return ov_object_getaccess(pobj, pelem, pticket);
}

OV_DLLFNCEXPORT UA_StatusCode opcua_ovNetworkLayer_start(
	struct UA_ServerNetworkLayer *nl,
	UA_Logger logger
) {

    return (UA_StatusCode)0;
}

OV_DLLFNCEXPORT size_t opcua_ovNetworkLayer_getJobs(
	struct UA_ServerNetworkLayer *nl,
	UA_Job** jobs,
	UA_UInt16 timeout
) {

	OV_INSTPTR_opcua_uaConnection	pConnection	=	NULL;
	OV_UINT								counter		=	0;
	OV_UINT								closeConnCounter	=	0;
	UA_Job		 						*newJobs	=	NULL;
	OV_INSTPTR_opcua_ovNetworkLayer	this		=	Ov_StaticPtrCast(opcua_ovNetworkLayer, nl->handle);

	/*	count work items	*/
	Ov_ForEachChild(opcua_networkLayerToConnection, this, pConnection){
		if(pConnection->v_workNext == TRUE){
			counter++;
		}

	}

	counter += this->v_connsToCloseCount * 2;

	newJobs = UA_malloc(sizeof(UA_Job)*(counter+1));
	if(!newJobs){
		jobs = NULL;
		return 0;
	}

	/*	iterate and collect work	*/
	counter = 0;
	Ov_ForEachChild(opcua_networkLayerToConnection, this, pConnection){
		if(pConnection->v_workNext == TRUE){
			newJobs[counter].type = UA_JOBTYPE_BINARYMESSAGE_NETWORKLAYER;
			UA_ByteString_allocBuffer(&(newJobs[counter].job.binaryMessage.message), pConnection->v_buffer.length);
			memcpy(newJobs[counter].job.binaryMessage.message.data, pConnection->v_buffer.data, pConnection->v_buffer.length);
			ksbase_free_KSDATAPACKET(&(pConnection->v_buffer));
			newJobs[counter].job.binaryMessage.connection = pConnection->v_connection;
			pConnection->v_workNext = FALSE;
			counter++;
		}
	}

	for(closeConnCounter = 0; closeConnCounter < this->v_connsToCloseCount; closeConnCounter++){
		newJobs[counter].type = UA_JOBTYPE_DETACHCONNECTION;
		newJobs[counter].job.closeConnection = this->v_connsToClose[closeConnCounter];
		counter++;
		newJobs[counter].type = UA_JOBTYPE_METHODCALL_DELAYED;
		newJobs[counter].job.methodCall.method = FreeConnection;
		newJobs[counter].job.methodCall.data = this->v_connsToClose[closeConnCounter];
		counter++;
	}
	Ov_HeapFree(this->v_connsToClose);
	this->v_connsToClose = NULL;
	this->v_connsToCloseCount = 0;

	if(counter == 0) {
		UA_free(newJobs);
		*jobs = NULL;
	} else {
		*jobs = newJobs;
	}

	return counter;
}

OV_DLLFNCEXPORT size_t opcua_ovNetworkLayer_stop(
	struct UA_ServerNetworkLayer *nl,
	UA_Job** jobs
) {

    return (UA_Int32)0;
}

OV_DLLFNCEXPORT void opcua_ovNetworkLayer_delete(
	struct UA_ServerNetworkLayer *nl
) {
//	Ov_HeapFree(pOVNetworkLayer->v_messageBuffer.data);
//	UA_ByteString_deleteMembers(&(pOVNetworkLayer->v_sendBuffer));
	Ov_DeleteObject(pOVNetworkLayer);
	pOVNetworkLayer = NULL;
    return;
}
