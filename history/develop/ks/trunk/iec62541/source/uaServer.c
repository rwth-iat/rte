
/******************************************************************************
*
*   FILE
*   ----
*   uaServer.c
*
*   History
*   -------
*   2014-10-14   File created
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
#include "ks_logfile.h"
#include "ua_types.h"
#include "ua_server.h"
//#include "server/ua_nodestore_interface.h"

/*	function declaration needed as they are declared with __declspec(dllimport) in the header
 * which means they MUST come from a dll not an .a
*/
UA_Int32 UA_String_copycstring(char const *src, UA_String *dst);
void UA_String_deleteMembers(UA_String *p);
//void UA_Server_init(UA_Server *server, UA_String *endpointUrl);
//UA_Int32 UA_Server_deleteMembers(UA_Server *server);

OV_INSTPTR_iec62541_uaServer iec62541_pUaServer = NULL;


void open62541_server_init(UA_Server *server){


    LIST_INIT(&server->timedWork);
#ifdef UA_MULTITHREADING
    rcu_init();
	cds_wfcq_init(&server->dispatchQueue_head, &server->dispatchQueue_tail);
    server->delayedWork = UA_NULL;
#endif

    // random seed
    server->random_seed = (UA_UInt32) UA_DateTime_now();

    // networklayers
    server->nls = UA_NULL;
    server->nlsSize = 0;

    UA_ByteString_init(&server->serverCertificate);

    // mockup application description
    UA_ApplicationDescription_init(&server->description);
    UA_String_copycstring("urn:unconfigured:open62541:application", &server->description.productUri);
    UA_String_copycstring("http://unconfigured.open62541/applications/", &server->description.applicationUri);
    UA_LocalizedText_copycstring("Unconfigured open62541 application", &server->description.applicationName);
    server->description.applicationType = UA_APPLICATIONTYPE_SERVER;
    server->externalNamespacesSize = 0;
    server->externalNamespaces = UA_NULL;

    // mockup endpoint description
    server->endpointDescriptionsSize = 1;
    UA_EndpointDescription *endpoint = UA_EndpointDescription_new(); // todo: check return code

    endpoint->securityMode = UA_MESSAGESECURITYMODE_NONE;
    UA_String_copycstring("http://opcfoundation.org/UA/SecurityPolicy#None", &endpoint->securityPolicyUri);
    UA_String_copycstring("http://opcfoundation.org/UA-Profile/Transport/uatcp-uasc-uabinary", &endpoint->transportProfileUri);

    endpoint->userIdentityTokensSize = 1;
    endpoint->userIdentityTokens = UA_malloc(sizeof(UA_UserTokenPolicy));
    UA_UserTokenPolicy_init(endpoint->userIdentityTokens);
    UA_String_copycstring("my-anonymous-policy", &endpoint->userIdentityTokens->policyId); // defined per server
    endpoint->userIdentityTokens->tokenType = UA_USERTOKENTYPE_ANONYMOUS;

    /* UA_String_copy(endpointUrl, &endpoint->endpointUrl); */
    /* /\* The standard says "the HostName specified in the Server Certificate is the */
    /*    same as the HostName contained in the endpointUrl provided in the */
    /*    EndpointDescription *\/ */
    /* UA_String_copy(&server->serverCertificate, &endpoint->serverCertificate); */
    UA_ApplicationDescription_copy(&server->description, &endpoint->server);
    server->endpointDescriptions = endpoint;

#define MAXCHANNELCOUNT 100
#define STARTCHANNELID 1
#define TOKENLIFETIME 10000
#define STARTTOKENID 1
    UA_SecureChannelManager_init(&server->secureChannelManager, MAXCHANNELCOUNT,
                                 TOKENLIFETIME, STARTCHANNELID, STARTTOKENID);

#define MAXSESSIONCOUNT 1000
#define SESSIONLIFETIME 10000
#define STARTSESSIONID 1
    UA_SessionManager_init(&server->sessionManager, MAXSESSIONCOUNT, SESSIONLIFETIME, STARTSESSIONID);
}

#define SERVER_MAX_ALLOWED_EXTERNAL_NAMESPACES 10
UA_StatusCode UA_Server_addExternalNamespace(UA_Server *server, UA_UInt16 namespaceIndex, const UA_String *url, UA_ExternalNodeStore *nodeStore){


	if(server->externalNamespacesSize > SERVER_MAX_ALLOWED_EXTERNAL_NAMESPACES)
	{
		return UA_STATUSCODE_BADOUTOFMEMORY;
	}

	server->externalNamespaces[server->externalNamespacesSize].index = namespaceIndex;
	UA_String_copy(url, &server->externalNamespaces[server->externalNamespacesSize].url);
	server->externalNamespaces[server->externalNamespacesSize].externalNodeStore = *nodeStore;
	server->externalNamespacesSize++;
	return UA_STATUSCODE_GOOD;

}

OV_DLLFNCEXPORT OV_RESULT iec62541_uaServer_endpointName_set(
    OV_INSTPTR_iec62541_uaServer          pobj,
    const OV_STRING  value
) {
	OV_RESULT result;
	result = ov_string_setvalue(&pobj->v_endpointName,value);
	if(Ov_Fail(result)){
		return result;
	}
	if(UA_String_copycstring(value, &(pobj->v_endpointUrl)) != UA_STATUSCODE_GOOD){
		return OV_ERR_GENERIC;
	}
	return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_RESULT iec62541_uaServer_constructor(
	OV_INSTPTR_ov_object 	pobj
) {
    /*
    *   local variables
    */
    OV_INSTPTR_iec62541_uaServer pinst = Ov_StaticPtrCast(iec62541_uaServer, pobj);
    OV_RESULT    result;
    OV_INSTPTR_ov_object pOtherObject = NULL;
    OV_INSTPTR_iec62541_uaNamespace0	pNs0 = NULL;

    /* do what the base class does first */
    result = ov_object_constructor(pobj);
    if(Ov_Fail(result))
         return result;

    /* do what */
    Ov_ForEachChild(ov_instantiation, pclass_iec62541_uaServer, pOtherObject){
    	if(pOtherObject != pobj){
    		KS_logfile_error(("%s: cannot instantiate - uaServer instance already exists", pinst->v_identifier));
    		return OV_ERR_ALREADYEXISTS;
    	}
    }

    result = Ov_CreateObject(iec62541_uaNamespace0, pNs0, pinst, "ns0");
    if(Ov_Fail(result) && (result != OV_ERR_ALREADYEXISTS))
    {
    	return result;
    }

    return OV_ERR_OK;
}

OV_DLLFNCEXPORT void iec62541_uaServer_destructor(
	OV_INSTPTR_ov_object 	pobj
) {
    /*
    *   local variables
    */
   // OV_INSTPTR_iec62541_uaServer pinst = Ov_StaticPtrCast(iec62541_uaServer, pobj);

    /* do what */

    /* destroy object */
    ov_object_destructor(pobj);

    return;
}

OV_DLLFNCEXPORT void iec62541_uaServer_startup(
	OV_INSTPTR_ov_object 	pobj
) {
    /*    
    *   local variables
    */
    OV_INSTPTR_iec62541_uaServer pinst = Ov_StaticPtrCast(iec62541_uaServer, pobj);
    /* do what the base class does first */
    ov_object_startup(pobj);

    /* do what */
    iec62541_pUaServer = pinst;
    UA_String_copycstring(pinst->v_endpointName, &(pinst->v_endpointUrl));

    open62541_server_init(&pinst->v_serverData);

    UA_String url;
    UA_String_copycstring("test1",&url);


    //Services on OV Namespace
    iec62541_pUaServer->v_serverData.externalNamespaces = UA_malloc(10 * sizeof(UA_ExternalNamespace));

    iec62541_pUaServer->v_nodeStoreNs0.readNodes = ((OV_VTBLPTR_iec62541_uaNamespace0)pclass_iec62541_uaNamespace0->v_pvtable)->m_readNodes;

    iec62541_pUaServer->v_nodeStoreNs0.browseNodes = ((OV_VTBLPTR_iec62541_uaNamespace0)pclass_iec62541_uaNamespace0->v_pvtable)->m_browseNodes;
    UA_Server_addExternalNamespace( &iec62541_pUaServer->v_serverData,0,&url,&iec62541_pUaServer->v_nodeStoreNs0);

    //Services on Namespace 0
    iec62541_pUaServer->v_nodeStoreNsOV.readNodes = ((OV_VTBLPTR_iec62541_uaNamespace0)pclass_iec62541_uaNamespace0->v_pvtable)->m_readNodes;
    iec62541_pUaServer->v_nodeStoreNsOV.browseNodes = ((OV_VTBLPTR_iec62541_uaNamespace0)pclass_iec62541_uaNamespace0->v_pvtable)->m_browseNodes;
    UA_Server_addExternalNamespace( &iec62541_pUaServer->v_serverData,1,&url,&iec62541_pUaServer->v_nodeStoreNsOV);


   // UA_Server_addNamespace(&(pinst->v_serverData),0,&(pinst->v_nodeStoreNs0));
    //UA_Server_addNamespace(&(pinst->v_serverData),42,&(pinst->v_nodeStoreNsOV));

   /* UA_NodeStore_registerReadNodesOperation(&(pinst->v_nodeStoreNs0),
    		((OV_VTBLPTR_iec62541_uaNamespace0)pclass_iec62541_uaNamespace0->v_pvtable)->m_readNodes);
    UA_NodeStore_registerBrowseNodesOperation(&(pinst->v_nodeStoreNs0),
       		((OV_VTBLPTR_iec62541_uaNamespace0)pclass_iec62541_uaNamespace0->v_pvtable)->m_browseNodes);
    UA_NodeStore_registerReadNodesOperation(&(pinst->v_nodeStoreNsOV),
    		((OV_VTBLPTR_iec62541_nodeStoreFunctions)pclass_iec62541_nodeStoreFunctions->v_pvtable)->m_readNodes);
    		*/
    return;
}

OV_DLLFNCEXPORT void iec62541_uaServer_shutdown(
	OV_INSTPTR_ov_object 	pobj
) {
    /*    
    *   local variables
    */
    OV_INSTPTR_iec62541_uaServer pinst = Ov_StaticPtrCast(iec62541_uaServer, pobj);

    /* do what */
    //UA_Server_deleteMembers(&(pinst->v_serverData));
    UA_String_deleteMembers(&(pinst->v_endpointUrl));
    iec62541_pUaServer = NULL;
    /* set the object's state to "shut down" */
    ov_object_shutdown(pobj);


    return;
}

OV_DLLFNCEXPORT OV_ACCESS iec62541_uaServer_getaccess(
	OV_INSTPTR_ov_object	pobj,
	const OV_ELEMENT		*pelem,
	const OV_TICKET			*pticket
) {
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
