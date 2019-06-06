/*
 * ovStore.c
 *
 *  Created on: 06.12.2016
 *      Author: Torben Deppe, Julian Grothoff
 *
 *  Collects functions that are necessary to transform ov object to ua nodes via UA_Nodestore as interface.
 */

#ifndef OV_COMPILE_LIBRARY_opcua
#define OV_COMPILE_LIBRARY_opcua
#endif

#include "opcua_ovStore.h"
#include "libov/ov_macros.h"
#include "opcua.h"
#include "opcua_helpers.h"
#include "NoneTicketAuthenticator.h"
#include "opcua_ovTrafo.h"

static void opcua_ovStore_getObjData(const UA_NodeId *nodeId, OV_INSTPTR_ov_object *pobj, OV_STRING *virtualPath){
	OV_STRING *plist = NULL;
	OV_STRING tmpString = NULL;
	OV_UINT len = 0;

	*pobj = NULL;
	*virtualPath = NULL;
	if (nodeId->identifierType != UA_NODEIDTYPE_STRING || nodeId->identifier.string.data == NULL || nodeId->identifier.string.length == 0)
		return ;
	opcua_helpers_copyUAStringToOV(nodeId->identifier.string, &tmpString);
	plist = ov_string_split(tmpString, OV_OPCUA_VIRTUALNODESEPERATOR, &len);
	*pobj = ov_path_getobjectpointer(plist[0], 2);
	if (len>1)
		ov_string_setvalue(virtualPath, plist[1]);
	ov_string_setvalue(&tmpString, NULL);
	ov_string_freelist(plist);
	return;
}


static void opcua_ovStore_deleteNodestore(void *context){
}

static void opcua_ovStore_deleteNode(void * context, UA_Node *node){
	OV_INSTPTR_opcua_interface pInterface = (OV_INSTPTR_opcua_interface)context;

	if(pInterface == NULL)
		return ;


	OV_INSTPTR_opcua_server pServer = Ov_GetParent(opcua_serverToInterfaces, pInterface);
	if(pServer == NULL)
		return ;
	OV_INSTPTR_opcua_interface pInterfaceCheck = NULL;
	OV_VTBLPTR_opcua_interface pVtblInterface = NULL; //TODO use Call makro instead?
	OV_INSTPTR_ov_object pobj = NULL;
	OV_STRING virtualPath = NULL;

	opcua_ovStore_getObjData((const UA_NodeId*)&node->nodeId, &pobj, &virtualPath);

	Ov_ForEachChild(opcua_serverToInterfaces, pServer, pInterfaceCheck){
		if(pInterface == pInterfaceCheck)
			continue;
		Ov_GetVTablePtr(opcua_interface, pVtblInterface, pInterfaceCheck);
		if(pVtblInterface){
			if(pVtblInterface->m_checkNode(pInterfaceCheck, pobj, virtualPath, context)){
				ov_string_setvalue(&virtualPath, NULL);
				return pInterfaceCheck->v_trafo->deleteNode(pInterfaceCheck, node);
			}
		}
	}
	ov_string_setvalue(&virtualPath, NULL);
	pInterface->v_trafo->deleteNode(context, node);
}
static void opcua_ovStore_releaseNode(void *context, const UA_Node *node){
	OV_INSTPTR_opcua_interface pInterface = (OV_INSTPTR_opcua_interface)context;

	if(pInterface == NULL)
		return ;


	OV_INSTPTR_opcua_server pServer = Ov_GetParent(opcua_serverToInterfaces, pInterface);
	if(pServer == NULL)
		return ;
	OV_INSTPTR_opcua_interface pInterfaceCheck = NULL;
	OV_VTBLPTR_opcua_interface pVtblInterface = NULL; //TODO use Call makro instead?
	OV_INSTPTR_ov_object pobj = NULL;
	OV_STRING virtualPath = NULL;

	opcua_ovStore_getObjData((const UA_NodeId*)&node->nodeId, &pobj, &virtualPath);

	Ov_ForEachChild(opcua_serverToInterfaces, pServer, pInterfaceCheck){
		if(pInterface == pInterfaceCheck)
			continue;
		Ov_GetVTablePtr(opcua_interface, pVtblInterface, pInterfaceCheck);
		if(pVtblInterface){
			if(pVtblInterface->m_checkNode(pInterfaceCheck, pobj, virtualPath, context)){
				ov_string_setvalue(&virtualPath, NULL);
				return pInterfaceCheck->v_trafo->releaseNode(pInterfaceCheck, node);
			}
		}
	}
	ov_string_setvalue(&virtualPath, NULL);
	pInterface->v_trafo->releaseNode(context, node);
}

static void opcua_ovStore_iterate(void *context, UA_NodestoreVisitor visitor, void* visitorHandle){
}
static UA_Node * opcua_ovStore_newNode(void * context, UA_NodeClass nodeClass){ // TODO: No NodeId
	OV_INSTPTR_opcua_interface pInterface = (OV_INSTPTR_opcua_interface)context;

	return pInterface->v_trafo->newNode(context, nodeClass);
}

static const UA_Node * opcua_ovStore_getNode(void * context, const UA_NodeId *nodeId){
	OV_INSTPTR_opcua_interface pInterface = (OV_INSTPTR_opcua_interface)context;

	if(pInterface == NULL)
		return NULL;


	OV_INSTPTR_opcua_server pServer = Ov_GetParent(opcua_serverToInterfaces, pInterface);
	if(pServer == NULL)
		return NULL;
	OV_INSTPTR_opcua_interface pInterfaceCheck = NULL;
	OV_VTBLPTR_opcua_interface pVtblInterface = NULL; //TODO use Call makro instead?
	OV_INSTPTR_ov_object pobj = NULL;
	OV_STRING virtualPath = NULL;

	opcua_ovStore_getObjData(nodeId, &pobj, &virtualPath);

	Ov_ForEachChild(opcua_serverToInterfaces, pServer, pInterfaceCheck){
		if(pInterface == pInterfaceCheck)
			continue;
		Ov_GetVTablePtr(opcua_interface, pVtblInterface, pInterfaceCheck);
		if(pVtblInterface){
			if(pVtblInterface->m_checkNode(pInterfaceCheck, pobj, virtualPath, context)){
				ov_string_setvalue(&virtualPath, NULL);
				return pInterfaceCheck->v_trafo->getNode(pInterfaceCheck, nodeId);
			}
		}
	}
	ov_string_setvalue(&virtualPath, NULL);
	return pInterface->v_trafo->getNode(context, nodeId);
}
static UA_StatusCode opcua_ovStore_getNodeCopy(void *context, const UA_NodeId *nodeId, UA_Node ** nodeOut){
	OV_INSTPTR_opcua_interface pInterface = (OV_INSTPTR_opcua_interface)context;

	if(pInterface == NULL)
		return UA_STATUSCODE_BADNODEIDEXISTS;


	OV_INSTPTR_opcua_server pServer = Ov_GetParent(opcua_serverToInterfaces, pInterface);
	if(pServer == NULL)
		return UA_STATUSCODE_BADNODEIDEXISTS;
	OV_INSTPTR_opcua_interface pInterfaceCheck = NULL;
	OV_VTBLPTR_opcua_interface pVtblInterface = NULL; //TODO use Call makro instead?
	OV_INSTPTR_ov_object pobj = NULL;
	OV_STRING virtualPath = NULL;

	opcua_ovStore_getObjData(nodeId, &pobj, &virtualPath);

	Ov_ForEachChild(opcua_serverToInterfaces, pServer, pInterfaceCheck){
		if(pInterface == pInterfaceCheck)
			continue;
		Ov_GetVTablePtr(opcua_interface, pVtblInterface, pInterfaceCheck);
		if(pVtblInterface){
			if(pVtblInterface->m_checkNode(pInterfaceCheck, pobj, virtualPath, context)){
				ov_string_setvalue(&virtualPath, NULL);
				return pInterfaceCheck->v_trafo->getNodeCopy(pInterfaceCheck, nodeId, nodeOut);
			}
		}
	}
	ov_string_setvalue(&virtualPath, NULL);
	return pInterface->v_trafo->getNodeCopy(context, nodeId, nodeOut);
}
static UA_StatusCode opcua_ovStore_removeNode(void *context, const UA_NodeId *nodeId){
	OV_INSTPTR_opcua_interface pInterface = (OV_INSTPTR_opcua_interface)context;

	if(pInterface == NULL)
		return UA_STATUSCODE_BADNODEIDEXISTS;


	OV_INSTPTR_opcua_server pServer = Ov_GetParent(opcua_serverToInterfaces, pInterface);
	if(pServer == NULL)
		return UA_STATUSCODE_BADNODEIDEXISTS;
	OV_INSTPTR_opcua_interface pInterfaceCheck = NULL;
	OV_VTBLPTR_opcua_interface pVtblInterface = NULL; //TODO use Call makro instead?
	OV_INSTPTR_ov_object pobj = NULL;
	OV_STRING virtualPath = NULL;

	opcua_ovStore_getObjData(nodeId, &pobj, &virtualPath);

	Ov_ForEachChild(opcua_serverToInterfaces, pServer, pInterfaceCheck){
		if(pInterface == pInterfaceCheck)
			continue;
		Ov_GetVTablePtr(opcua_interface, pVtblInterface, pInterfaceCheck);
		if(pVtblInterface){
			if(pVtblInterface->m_checkNode(pInterfaceCheck, pobj, virtualPath, context)){
				ov_string_setvalue(&virtualPath, NULL);
				return pInterfaceCheck->v_trafo->removeNode(pInterfaceCheck, nodeId);
			}
		}
	}
	ov_string_setvalue(&virtualPath, NULL);

	return pInterface->v_trafo->removeNode(context, nodeId);
}
static UA_StatusCode opcua_ovStore_insertNode(void *context, UA_Node *node, UA_NodeId *parrentNode){
	OV_INSTPTR_opcua_interface pInterface = (OV_INSTPTR_opcua_interface)context;

	if(pInterface == NULL)
		return UA_STATUSCODE_BADNODEIDEXISTS;


	OV_INSTPTR_opcua_server pServer = Ov_GetParent(opcua_serverToInterfaces, pInterface);
	if(pServer == NULL)
		return UA_STATUSCODE_BADNODEIDEXISTS;
	OV_INSTPTR_opcua_interface pInterfaceCheck = NULL;
	OV_VTBLPTR_opcua_interface pVtblInterface = NULL; //TODO use Call makro instead?
	OV_INSTPTR_ov_object pobj = NULL;
	OV_STRING virtualPath = NULL;

	opcua_ovStore_getObjData((const UA_NodeId*)&node->nodeId, &pobj, &virtualPath);

	Ov_ForEachChild(opcua_serverToInterfaces, pServer, pInterfaceCheck){
		if(pInterface == pInterfaceCheck)
			continue;
		Ov_GetVTablePtr(opcua_interface, pVtblInterface, pInterfaceCheck);
		if(pVtblInterface){
			if(pVtblInterface->m_checkNode(pInterfaceCheck, pobj, virtualPath, context)){
				ov_string_setvalue(&virtualPath, NULL);
				return pInterfaceCheck->v_trafo->insertNode(pInterfaceCheck, node, parrentNode);
			}
		}
	}
	ov_string_setvalue(&virtualPath, NULL);
	return pInterface->v_trafo->insertNode(context, node, parrentNode);
}
static UA_StatusCode opcua_ovStore_replaceNode(void *context, UA_Node *node){
	OV_INSTPTR_opcua_interface pInterface = (OV_INSTPTR_opcua_interface)context;

	if(pInterface == NULL)
		return UA_STATUSCODE_BADNODEIDEXISTS;


	OV_INSTPTR_opcua_server pServer = Ov_GetParent(opcua_serverToInterfaces, pInterface);
	if(pServer == NULL)
		return UA_STATUSCODE_BADNODEIDEXISTS;
	OV_INSTPTR_opcua_interface pInterfaceCheck = NULL;
	OV_VTBLPTR_opcua_interface pVtblInterface = NULL; //TODO use Call makro instead?
	OV_INSTPTR_ov_object pobj = NULL;
	OV_STRING virtualPath = NULL;

	opcua_ovStore_getObjData((const UA_NodeId*)&node->nodeId, &pobj, &virtualPath);

	Ov_ForEachChild(opcua_serverToInterfaces, pServer, pInterfaceCheck){
		if(pInterface == pInterfaceCheck)
			continue;
		Ov_GetVTablePtr(opcua_interface, pVtblInterface, pInterfaceCheck);
		if(pVtblInterface){
			if(pVtblInterface->m_checkNode(pInterfaceCheck, pobj, virtualPath, context)){
				ov_string_setvalue(&virtualPath, NULL);
				return pInterfaceCheck->v_trafo->replaceNode(pInterfaceCheck, node);
			}
		}
	}
	ov_string_setvalue(&virtualPath, NULL);
	return pInterface->v_trafo->replaceNode(context, node);
}

UA_NodestoreInterface*
opcua_ovStore_new(OV_INSTPTR_opcua_ovInterface context) {
	UA_NodestoreInterface* nsi = (UA_NodestoreInterface*)UA_malloc(sizeof(UA_NodestoreInterface));
	if(nsi == NULL)
		return NULL;
    nsi->context =        	context;
    nsi->deleteNodestore =  opcua_ovStore_deleteNodestore;
    nsi->newNode =       	opcua_ovStore_newNode;
    nsi->deleteNode =    	opcua_ovStore_deleteNode;
    nsi->insertNode =       opcua_ovStore_insertNode;
    nsi->getNode =          opcua_ovStore_getNode;
    nsi->getNodeCopy =      opcua_ovStore_getNodeCopy;
    nsi->replaceNode =      opcua_ovStore_replaceNode;
    nsi->removeNode =       opcua_ovStore_removeNode;
    nsi->iterate =          opcua_ovStore_iterate;
    nsi->releaseNode =      opcua_ovStore_releaseNode;
    return nsi;
}
void
opcua_ovStore_delete(UA_NodestoreInterface * nodestoreInterface){
	if (nodestoreInterface){
		if (nodestoreInterface->context)
			nodestoreInterface->context = NULL;
		UA_free(nodestoreInterface);
		nodestoreInterface = NULL;
	}
}
