
/******************************************************************************
*
*   FILE
*   ----
*   ua.c
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
#include "libov/ov_vendortree.h"
#include "libov/ov_memstack.h"
#include "libov/ov_result.h"
#include "iec62541_helpers.h"
#include "ks_logfile.h"
#include "open62541.h"
#include <errno.h>

#define ADDREFERENCE(NODEID, REFTYPE_NODEID, TARGET_EXPNODEID) do {     \
        UA_AddReferencesItem item;                                      \
        UA_AddReferencesItem_init(&item);                               \
        item.sourceNodeId = NODEID;                                     \
        item.referenceTypeId = REFTYPE_NODEID;                          \
        item.isForward = UA_TRUE;                                       \
        item.targetNodeId = TARGET_EXPNODEID;                           \
        UA_Server_addReference(server, &item);                          \
    } while(0)

//#include "/ua_nodestore_interface.h"

/*	function declaration needed as they are declared with __declspec(dllimport) in the header
 * which means they MUST come from a dll not an .a
*/
void UA_String_deleteMembers(UA_String *p);


OV_INSTPTR_iec62541_uaServer iec62541_pUaServer = NULL;
static UA_Boolean	UA_ServerRun = FALSE;




static UA_ByteString loadCertificate(void) {
    UA_ByteString certificate = UA_STRING_NULL;
	FILE *fp = NULL;
	OV_STRING tempstring = NULL;
	OV_STRING tempStackString = NULL;

	ov_memstack_lock();
	tempStackString = ov_vendortree_getcmdlineoption_value("UA_CertPath");
	if(tempStackString){
#if OV_SYSTEM_UNIX
		if(!(tempStackString[0]=='/'))
#else
		if(!(tempStackString[1]==':'|| tempStackString[0]=='\\'))
#endif
		{
			ov_string_print(&tempstring, "%s", tempStackString);
		} else {
			/*	relativePath --> prepend confdirpath	*/
#if OV_SYSTEM_UNIX
			ov_string_print(&tempstring, "%s/%s", ov_vendortree_getcmdlineoption_value("CONFDIR"), tempStackString);
#else
			ov_string_print(&tempstring, "%s\\%s", ov_vendortree_getcmdlineoption_value("CONFDIR"), tempStackString);
#endif
		}
	}
	ov_memstack_unlock();
	if(tempstring){
		fp=fopen(tempstring, "rb");
		ov_string_setvalue(&tempstring, NULL);
		if(!fp) {
			errno = 0; // we read errno also from the tcp layer...
			return certificate;
		}

		fseek(fp, 0, SEEK_END);
		certificate.length = ftell(fp);
		certificate.data = malloc(certificate.length*sizeof(UA_Byte));
		if(!certificate.data){
			fclose(fp);
			return certificate;
		}
		fseek(fp, 0, SEEK_SET);
		if(fread(certificate.data, sizeof(UA_Byte), certificate.length, fp) < (size_t)certificate.length){
			UA_ByteString_deleteMembers(&certificate); // error reading the cert
		}
		fclose(fp);
	}
    return certificate;
}

static UA_Int32 iec62541_uaServer_destroyNodeStore(void *ensHandle){
	return 0;
}

static void iec62541_uaServer_initServer(OV_INSTPTR_iec62541_uaServer pinst){
	UA_Logger logger;
	UA_ByteString certificate;
	UA_String url;
	OV_STRING tempStackString = NULL;
	UA_Int32 port;
	UA_Int32 ksPort;
	OV_INSTPTR_ov_library	pLibrary	=	NULL;
	OV_INSTPTR_ov_class		pClass		=	NULL;
	OV_INSTPTR_ov_object	pListener	=	NULL;
	OV_INSTPTR_ov_association	pAssoc	=	NULL;
	OV_RESULT		result;
	OV_ELEMENT		parent;
	OV_ELEMENT		child;


	pinst->v_serverData = UA_Server_new(pinst->v_serverConfig);
	logger = ov_UAlogger_new();
	UA_Server_setLogger(pinst->v_serverData, logger);
	certificate = loadCertificate();
	UA_Server_setServerCertificate(pinst->v_serverData, certificate);
	UA_ByteString_deleteMembers(&certificate);

	/*	determine port....	*/
	ov_memstack_lock();
	tempStackString = ov_vendortree_getcmdlineoption_value("UA_Port");
	ksPort = ov_vendortree_getport();
	if(!tempStackString){
		port = ksPort;
	} else {
		port = strtol(tempStackString, NULL, 10);
	}
	ov_memstack_unlock();
	if(port == -1){
		//	no port specified in either way --> use OPC-UA standard 16664
		port = 16664;
	}
	if(port != ksPort){
		Ov_ForEachChildEx(ov_instantiation, pclass_ov_library, pLibrary, ov_library){
			if(ov_string_compare(pLibrary->v_identifier, "TCPbind") == OV_STRCMP_EQUAL){
				break;
			}
		}
		if(!pLibrary){
			ov_logfile_error("%s: something went wrong while creating the specific Listener - could not find TCPbind library", pinst->v_identifier);
		}
		pClass = Ov_StaticPtrCast(ov_class, Ov_SearchChild(ov_containment, pLibrary, "TCPListener"));
		if(pClass){
			result = ov_class_createIDedObject(pClass, Ov_PtrUpCast(ov_domain, pinst), "TCPListener", OV_PMH_DEFAULT, NULL, NULL, NULL, &pListener);
			if(Ov_OK(result)){
				parent.elemtype = OV_ET_OBJECT;
				parent.pobj = pListener;
				child.elemtype = OV_ET_NONE;
				result = ov_element_searchpart(&parent, &child, OV_ET_VARIABLE, "port");
				if(Ov_OK(result)){
					*((OV_INT*)child.pvalue) = port;
				} else {
					ov_logfile_error("%s: something went wrong while creating the specific Listener - could not find port variable. reason: %s", pinst->v_identifier, ov_result_getresulttext(result));
				}
				parent.elemtype = OV_ET_OBJECT;
				parent.pobj = pListener;
				child.elemtype = OV_ET_NONE;
				result = ov_element_searchpart(&parent, &child, OV_ET_VARIABLE, "actimode");
				if(Ov_OK(result)){
					*((OV_INT*)child.pvalue) = 1;
				} else {
					ov_logfile_error("%s: something went wrong while creating the specific Listener - could not find actimode variable. reason: %s", pinst->v_identifier, ov_result_getresulttext(result));
				}
				pAssoc = Ov_StaticPtrCast(ov_association, Ov_SearchChild(ov_containment, pLibrary, "AssocSpecificClientHandler"));
				if(pAssoc){
					result = ov_association_link(pAssoc, pListener, Ov_GetFirstChild(ov_instantiation, pclass_iec62541_uaIdentificator), OV_PMH_DEFAULT, NULL, OV_PMH_DEFAULT, NULL);
				} else {
					ov_logfile_error("%s: something went wrong while creating the specific Listener - could not find association class");
				}
			} else {
				ov_logfile_error("%s: something went wrong while creating the specific Listener - could not create object. reason: %s", pinst->v_identifier, ov_result_getresulttext(result));
			}
		} else {
			ov_logfile_error("%s: something went wrong while creating the specific Listener - could not find TCPListener class", pinst->v_identifier);
		}
	}

	UA_Server_addNetworkLayer(pinst->v_serverData, ServerNetworkLayerOV_new(UA_ConnectionConfig_standard, port));

	url = UA_String_fromChars("iec62541-ov.acplt.org");

	//Services on Namespace OV (1)
	iec62541_pUaServer->v_nodeStoreNsOV.addNodes = ((OV_VTBLPTR_iec62541_nodeStoreFunctions)pclass_iec62541_nodeStoreFunctions->v_pvtable)->m_addNodes;
	iec62541_pUaServer->v_nodeStoreNsOV.addReferences = ((OV_VTBLPTR_iec62541_nodeStoreFunctions)pclass_iec62541_nodeStoreFunctions->v_pvtable)->m_addReferences;
	iec62541_pUaServer->v_nodeStoreNsOV.browseNodes = ((OV_VTBLPTR_iec62541_nodeStoreFunctions)pclass_iec62541_nodeStoreFunctions->v_pvtable)->m_browseNodes;
	iec62541_pUaServer->v_nodeStoreNsOV.deleteNodes = ((OV_VTBLPTR_iec62541_nodeStoreFunctions)pclass_iec62541_nodeStoreFunctions->v_pvtable)->m_deleteNodes;
	iec62541_pUaServer->v_nodeStoreNsOV.deleteReferences = ((OV_VTBLPTR_iec62541_nodeStoreFunctions)pclass_iec62541_nodeStoreFunctions->v_pvtable)->m_deleteReferences;
	iec62541_pUaServer->v_nodeStoreNsOV.readNodes = ((OV_VTBLPTR_iec62541_nodeStoreFunctions)pclass_iec62541_nodeStoreFunctions->v_pvtable)->m_readNodes;
	iec62541_pUaServer->v_nodeStoreNsOV.translateBrowsePathsToNodeIds = ((OV_VTBLPTR_iec62541_nodeStoreFunctions)pclass_iec62541_nodeStoreFunctions->v_pvtable)->m_translateBrowsePathsToNodeIDs;
	iec62541_pUaServer->v_nodeStoreNsOV.writeNodes = ((OV_VTBLPTR_iec62541_nodeStoreFunctions)pclass_iec62541_nodeStoreFunctions->v_pvtable)->m_writeNodes;
	iec62541_pUaServer->v_nodeStoreNsOV.destroy = iec62541_uaServer_destroyNodeStore;
	UA_Server_addExternalNamespace(iec62541_pUaServer->v_serverData,1,&url,&iec62541_pUaServer->v_nodeStoreNsOV);
	UA_String_deleteMembers(&url);
	/*	add reference to ov root	*/
	if(UA_Server_addReference(pinst->v_serverData, UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER),
			UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES), UA_EXPANDEDNODEID_NUMERIC(1, 0)) != UA_STATUSCODE_GOOD){
		ov_logfile_error("%s - init: could not create reference to ov-namespace", pinst->v_identifier);
	}
	UA_Server_run_startup(iec62541_pUaServer->v_serverData, 1, &UA_ServerRun);

	return;
}

static void iec62541_uaServer_stopServer(OV_INSTPTR_iec62541_uaServer pinst){
	UA_Server_run_shutdown(iec62541_pUaServer->v_serverData, 1);
	UA_Server_delete(pinst->v_serverData);
	return;
}

OV_DLLFNCEXPORT OV_BOOL iec62541_uaServer_LoginEnableAnonymous_get(
    OV_INSTPTR_iec62541_uaServer          pobj
) {
    return pobj->v_serverConfig.Login_enableAnonymous;
}

OV_DLLFNCEXPORT OV_RESULT iec62541_uaServer_LoginEnableAnonymous_set(
    OV_INSTPTR_iec62541_uaServer          pobj,
    const OV_BOOL  value
) {
    if(value != pobj->v_serverConfig.Login_enableAnonymous){
    	if(pobj->v_objectstate >= OV_OS_STARTED){
    		iec62541_uaServer_stopServer(pobj);
    	}
    	pobj->v_serverConfig.Login_enableAnonymous = value;
    	if(pobj->v_objectstate >= OV_OS_STARTED){
    		iec62541_uaServer_initServer(pobj);
    	}
    }
	return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_BOOL iec62541_uaServer_LoginEnableUsernamePassword_get(
    OV_INSTPTR_iec62541_uaServer          pobj
) {
    return pobj->v_serverConfig.Login_enableUsernamePassword;
}

OV_DLLFNCEXPORT OV_RESULT iec62541_uaServer_LoginEnableUsernamePassword_set(
    OV_INSTPTR_iec62541_uaServer          pobj,
    const OV_BOOL  value
) {
	if(value != pobj->v_serverConfig.Login_enableUsernamePassword){
		if(pobj->v_objectstate >= OV_OS_STARTED){
			iec62541_uaServer_stopServer(pobj);
		}
		pobj->v_serverConfig.Login_enableUsernamePassword = value;
		if(pobj->v_objectstate >= OV_OS_STARTED){
			iec62541_uaServer_initServer(pobj);
		}
	}
	return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_STRING* iec62541_uaServer_LoginUsernames_get(
    OV_INSTPTR_iec62541_uaServer          pobj,
    OV_UINT *pveclen
) {
    *pveclen = pobj->v_serverConfig.Login_loginsCount;
    return pobj->v_serverConfig.Login_usernames;
}

OV_DLLFNCEXPORT OV_RESULT iec62541_uaServer_LoginUsernames_set(
    OV_INSTPTR_iec62541_uaServer          pobj,
    const OV_STRING*  value,
    const OV_UINT veclen
) {
	OV_UINT iterator;
	if(pobj->v_objectstate >= OV_OS_STARTED){
		iec62541_uaServer_stopServer(pobj);
	}
	if(veclen != pobj->v_serverConfig.Login_loginsCount){
		if(veclen < pobj->v_serverConfig.Login_loginsCount){
			for(iterator = veclen; iterator < pobj->v_serverConfig.Login_loginsCount; iterator++){
				ov_string_setvalue(&(pobj->v_serverConfig.Login_usernames[iterator]), NULL);
				ov_string_setvalue(&(pobj->v_serverConfig.Login_passwords[iterator]), NULL);
			}
		}
		pobj->v_serverConfig.Login_usernames = ov_database_realloc(pobj->v_serverConfig.Login_usernames, veclen * sizeof(OV_STRING));
		pobj->v_serverConfig.Login_passwords = ov_database_realloc(pobj->v_serverConfig.Login_passwords, veclen * sizeof(OV_STRING));
		if(pobj->v_serverConfig.Login_usernames == NULL || pobj->v_serverConfig.Login_passwords){
			return OV_ERR_DBOUTOFMEMORY;
		}
	}
	for(iterator = 0; iterator < veclen; iterator++){
		if(Ov_Fail(ov_string_setvalue(&(pobj->v_serverConfig.Login_usernames[iterator]), value[iterator]))){
			return OV_ERR_DBOUTOFMEMORY;
		}
		ov_string_setvalue(&(pobj->v_serverConfig.Login_passwords[iterator]), NULL);
	}
	if(pobj->v_objectstate >= OV_OS_STARTED){
		iec62541_uaServer_initServer(pobj);
	}
    return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_STRING* iec62541_uaServer_LoginPasswords_get(
    OV_INSTPTR_iec62541_uaServer          pobj,
    OV_UINT *pveclen
) {
	*pveclen = pobj->v_serverConfig.Login_loginsCount;
	return pobj->v_serverConfig.Login_passwords;
}

OV_DLLFNCEXPORT OV_RESULT iec62541_uaServer_LoginPasswords_set(
    OV_INSTPTR_iec62541_uaServer          pobj,
    const OV_STRING*  value,
    const OV_UINT veclen
) {
	OV_UINT iterator;
	if(veclen != pobj->v_serverConfig.Login_loginsCount){
		return OV_ERR_BADVALUE;
	}
	if(pobj->v_objectstate >= OV_OS_STARTED){
		iec62541_uaServer_stopServer(pobj);
	}
	for(iterator = 0; iterator < veclen; iterator++){
		if(Ov_Fail(ov_string_setvalue(&(pobj->v_serverConfig.Login_passwords[iterator]), value[iterator]))){
			return OV_ERR_DBOUTOFMEMORY;
		}
	}
	if(pobj->v_objectstate >= OV_OS_STARTED){
		iec62541_uaServer_initServer(pobj);
	}
	return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_UINT iec62541_uaServer_LoginLoginsCount_get(
    OV_INSTPTR_iec62541_uaServer          pobj
) {
    return pobj->v_serverConfig.Login_loginsCount;
}

OV_DLLFNCEXPORT OV_STRING iec62541_uaServer_ApplicationURI_get(
    OV_INSTPTR_iec62541_uaServer          pobj
) {
    return pobj->v_serverConfig.Application_applicationURI;
}

OV_DLLFNCEXPORT OV_RESULT iec62541_uaServer_ApplicationURI_set(
    OV_INSTPTR_iec62541_uaServer          pobj,
    const OV_STRING  value
) {
	OV_RESULT result = OV_ERR_OK;
	if(ov_string_compare(pobj->v_serverConfig.Application_applicationURI, value) != OV_STRCMP_EQUAL){
		if(pobj->v_objectstate >= OV_OS_STARTED){
			iec62541_uaServer_stopServer(pobj);
		}
		result = ov_string_setvalue(&(pobj->v_serverConfig.Application_applicationURI), value);
		if(pobj->v_objectstate >= OV_OS_STARTED){
			iec62541_uaServer_initServer(pobj);
		}
	}
    return result;
}

OV_DLLFNCEXPORT OV_STRING iec62541_uaServer_ApplicationName_get(
    OV_INSTPTR_iec62541_uaServer          pobj
) {
	return pobj->v_serverConfig.Application_applicationName;
}

OV_DLLFNCEXPORT OV_RESULT iec62541_uaServer_ApplicationName_set(
    OV_INSTPTR_iec62541_uaServer          pobj,
    const OV_STRING  value
) {
	OV_RESULT result = OV_ERR_OK;
	if(ov_string_compare(pobj->v_serverConfig.Application_applicationName, value) != OV_STRCMP_EQUAL){
		if(pobj->v_objectstate >= OV_OS_STARTED){
			iec62541_uaServer_stopServer(pobj);
		}
		result = ov_string_setvalue(&(pobj->v_serverConfig.Application_applicationName), value);
		if(pobj->v_objectstate >= OV_OS_STARTED){
			iec62541_uaServer_initServer(pobj);
		}
	}
	return result;
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

    /* do what the base class does first */
    result = ksbase_ComTask_constructor(pobj);
    if(Ov_Fail(result))
         return result;

    /* do what */
    Ov_ForEachChild(ov_instantiation, pclass_iec62541_uaServer, pOtherObject){
    	if(pOtherObject != pobj){
    		KS_logfile_error(("%s: cannot instantiate - ua instance already exists", pinst->v_identifier));
    		return OV_ERR_ALREADYEXISTS;
    	}
    }
    pinst->v_serverConfig = UA_ServerConfig_standard;
    pinst->v_serverConfig.Application_applicationName = NULL;
    pinst->v_serverConfig.Application_applicationURI = NULL;
    ov_string_setvalue(&(pinst->v_serverConfig.Application_applicationName), "acplt-iec62541");
    ov_string_setvalue(&(pinst->v_serverConfig.Application_applicationURI), "urn:acplt:open62541:acplt-iec2541Server");
    pinst->v_serverConfig.Login_loginsCount = 2;
    pinst->v_serverConfig.Login_usernames = ov_database_malloc(2*sizeof(OV_STRING));
    pinst->v_serverConfig.Login_passwords = ov_database_malloc(2*sizeof(OV_STRING));
    if(!pinst->v_serverConfig.Login_usernames || !pinst->v_serverConfig.Login_passwords){
    	return OV_ERR_DBOUTOFMEMORY;
    }
    ov_string_setvalue(&(pinst->v_serverConfig.Login_usernames[0]), "operator");
    ov_string_setvalue(&(pinst->v_serverConfig.Login_usernames[1]), "programmer");
    ov_string_setvalue(&(pinst->v_serverConfig.Login_passwords[0]), "PWoperator");
    ov_string_setvalue(&(pinst->v_serverConfig.Login_passwords[1]), "PWprogrammer");
    return OV_ERR_OK;
}

OV_DLLFNCEXPORT void iec62541_uaServer_destructor(
	OV_INSTPTR_ov_object 	pobj
) {
    /*
    *   local variables
    */
	OV_INSTPTR_iec62541_uaServer pinst = Ov_StaticPtrCast(iec62541_uaServer, pobj);
	OV_UINT	iterator;
    /* do what */
	ov_string_setvalue(&(pinst->v_serverConfig.Application_applicationName), NULL);
	ov_string_setvalue(&(pinst->v_serverConfig.Application_applicationURI), NULL);
	for(iterator = 0; iterator < pinst->v_serverConfig.Login_loginsCount; iterator++){
		ov_string_setvalue(&(pinst->v_serverConfig.Login_usernames[iterator]), NULL);
		ov_string_setvalue(&(pinst->v_serverConfig.Login_passwords[iterator]), NULL);
	}
	ov_database_free(pinst->v_serverConfig.Login_usernames);
	pinst->v_serverConfig.Login_usernames = NULL;
	ov_database_free(pinst->v_serverConfig.Login_passwords);
	pinst->v_serverConfig.Login_passwords = NULL;
    /* destroy object */
	ksbase_ComTask_destructor(pobj);

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
    UA_ServerRun = TRUE;
    iec62541_uaServer_initServer(pinst);
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
    UA_ServerRun = FALSE;
    iec62541_uaServer_stopServer(pinst);
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

OV_DLLFNCEXPORT void iec62541_uaServer_typemethod (
	OV_INSTPTR_ksbase_ComTask	this
) {
	OV_INSTPTR_iec62541_uaServer thisServer = Ov_StaticPtrCast(iec62541_uaServer, this);
	UA_Server_run_mainloop(thisServer->v_serverData, &UA_ServerRun);
    return;
}
