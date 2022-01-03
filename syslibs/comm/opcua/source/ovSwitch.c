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

#include "ovSwitch.h"
#include "libov/ov_macros.h"
#include "opcua.h"
#include "helpers.h"
#include "NoneTicketAuthenticator.h"
#include "ovTrafo.h"

static void opcua_ovSwitch_getObjData(const UA_NodeId *nodeId, OV_INSTPTR_ov_object *pobj, OV_STRING *virtualPath){
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


static void opcua_ovSwitch_deleteNodestore(void *context){
}

static void opcua_ovSwitch_deleteNode(void * context, UA_Node *node){
	OV_INSTPTR_opcua_server pServer = (OV_INSTPTR_opcua_server)context;
	if(pServer == NULL)
		return ;

	OV_INSTPTR_opcua_interface pInterfaceCheck = NULL;
	OV_VTBLPTR_opcua_interface pVtblInterface = NULL; //TODO use Call makro instead?
	OV_INSTPTR_ov_object pobj = NULL;
	OV_STRING virtualPath = NULL;

	opcua_ovSwitch_getObjData((const UA_NodeId*)&node->nodeId, &pobj, &virtualPath);

	Ov_ForEachChild(opcua_serverToInterfaces, pServer, pInterfaceCheck){
		Ov_GetVTablePtr(opcua_interface, pVtblInterface, pInterfaceCheck);
		if(pVtblInterface){
			if(pVtblInterface->m_checkNode(pInterfaceCheck, pobj, virtualPath)){
				ov_string_setvalue(&virtualPath, NULL);
				return pInterfaceCheck->v_trafo->deleteNode(pInterfaceCheck, node);
			}
		}
	}
	ov_string_setvalue(&virtualPath, NULL);
	pServer->v_ovTrafo->deleteNode(context, node);
}
static void opcua_ovSwitch_releaseNode(void *context, const UA_Node *node){
	OV_INSTPTR_opcua_server pServer = (OV_INSTPTR_opcua_server)context;
	if(pServer == NULL)
		return ;

	OV_INSTPTR_opcua_interface pInterfaceCheck = NULL;
	OV_VTBLPTR_opcua_interface pVtblInterface = NULL; //TODO use Call makro instead?
	OV_INSTPTR_ov_object pobj = NULL;
	OV_STRING virtualPath = NULL;

	opcua_ovSwitch_getObjData((const UA_NodeId*)&node->nodeId, &pobj, &virtualPath);

	Ov_ForEachChild(opcua_serverToInterfaces, pServer, pInterfaceCheck){
		Ov_GetVTablePtr(opcua_interface, pVtblInterface, pInterfaceCheck);
		if(pVtblInterface){
			if(pVtblInterface->m_checkNode(pInterfaceCheck, pobj, virtualPath)){
				ov_string_setvalue(&virtualPath, NULL);
				return pInterfaceCheck->v_trafo->releaseNode(pInterfaceCheck, node);
			}
		}
	}
	ov_string_setvalue(&virtualPath, NULL);
	pServer->v_ovTrafo->releaseNode(context, node);
}

static void opcua_ovSwitch_iterate(void *context, UA_NodestoreVisitor visitor, void* visitorHandle){
}
static UA_Node * opcua_ovSwitch_newNode(void * context, UA_NodeClass nodeClass){ // TODO: No NodeId
	OV_INSTPTR_opcua_server pServer = (OV_INSTPTR_opcua_server)context;
	if(pServer == NULL)
		return ;

	return pServer->v_ovTrafo->newNode(context, nodeClass);
}

static const UA_Node * opcua_ovSwitch_getNode(void * context, const UA_NodeId *nodeId){
	OV_INSTPTR_opcua_server pServer = (OV_INSTPTR_opcua_server)context;
	if(pServer == NULL)
		return ;

	OV_INSTPTR_opcua_interface pInterfaceCheck = NULL;
	OV_VTBLPTR_opcua_interface pVtblInterface = NULL; //TODO use Call makro instead?
	OV_INSTPTR_ov_object pobj = NULL;
	OV_STRING virtualPath = NULL;

	opcua_ovSwitch_getObjData(nodeId, &pobj, &virtualPath);

	Ov_ForEachChild(opcua_serverToInterfaces, pServer, pInterfaceCheck){
		Ov_GetVTablePtr(opcua_interface, pVtblInterface, pInterfaceCheck);
		if(pVtblInterface){
			if(pVtblInterface->m_checkNode(pInterfaceCheck, pobj, virtualPath)){
				ov_string_setvalue(&virtualPath, NULL);
				return pInterfaceCheck->v_trafo->getNode(pInterfaceCheck, nodeId);
			}
		}
	}
	ov_string_setvalue(&virtualPath, NULL);
	return pServer->v_ovTrafo->getNode(context, nodeId);
}
static UA_StatusCode opcua_ovSwitch_getNodeCopy(void *context, const UA_NodeId *nodeId, UA_Node ** nodeOut){
	OV_INSTPTR_opcua_server pServer = (OV_INSTPTR_opcua_server)context;
	if(pServer == NULL)
		return UA_STATUSCODE_BADNODEIDEXISTS;

	OV_INSTPTR_opcua_interface pInterfaceCheck = NULL;
	OV_VTBLPTR_opcua_interface pVtblInterface = NULL; //TODO use Call makro instead?
	OV_INSTPTR_ov_object pobj = NULL;
	OV_STRING virtualPath = NULL;

	opcua_ovSwitch_getObjData(nodeId, &pobj, &virtualPath);

	Ov_ForEachChild(opcua_serverToInterfaces, pServer, pInterfaceCheck){
		Ov_GetVTablePtr(opcua_interface, pVtblInterface, pInterfaceCheck);
		if(pVtblInterface){
			if(pVtblInterface->m_checkNode(pInterfaceCheck, pobj, virtualPath)){
				ov_string_setvalue(&virtualPath, NULL);
				return pInterfaceCheck->v_trafo->getNodeCopy(pInterfaceCheck, nodeId, nodeOut);
			}
		}
	}
	ov_string_setvalue(&virtualPath, NULL);
	return pServer->v_ovTrafo->getNodeCopy(context, nodeId, nodeOut);
}
static UA_StatusCode opcua_ovSwitch_removeNode(void *context, const UA_NodeId *nodeId){
	OV_INSTPTR_opcua_server pServer = (OV_INSTPTR_opcua_server)context;
	if(pServer == NULL)
		return UA_STATUSCODE_BADNODEIDEXISTS;

	OV_INSTPTR_opcua_interface pInterfaceCheck = NULL;
	OV_VTBLPTR_opcua_interface pVtblInterface = NULL; //TODO use Call makro instead?
	OV_INSTPTR_ov_object pobj = NULL;
	OV_STRING virtualPath = NULL;

	opcua_ovSwitch_getObjData(nodeId, &pobj, &virtualPath);

	Ov_ForEachChild(opcua_serverToInterfaces, pServer, pInterfaceCheck){
		Ov_GetVTablePtr(opcua_interface, pVtblInterface, pInterfaceCheck);
		if(pVtblInterface){
			if(pVtblInterface->m_checkNode(pInterfaceCheck, pobj, virtualPath)){
				ov_string_setvalue(&virtualPath, NULL);
				return pInterfaceCheck->v_trafo->removeNode(pInterfaceCheck, nodeId);
			}
		}
	}
	ov_string_setvalue(&virtualPath, NULL);

	return pServer->v_ovTrafo->removeNode(context, nodeId);
}
static UA_StatusCode opcua_ovSwitch_insertNode(void *context, UA_Node *node, UA_NodeId *parrentNode){
	OV_INSTPTR_opcua_server pServer = (OV_INSTPTR_opcua_server)context;
	if(pServer == NULL)
		return UA_STATUSCODE_BADNODEIDEXISTS;

	OV_INSTPTR_opcua_interface pInterfaceCheck = NULL;
	OV_VTBLPTR_opcua_interface pVtblInterface = NULL; //TODO use Call makro instead?
	OV_INSTPTR_ov_object pobj = NULL;
	OV_STRING virtualPath = NULL;

	opcua_ovSwitch_getObjData((const UA_NodeId*)&node->nodeId, &pobj, &virtualPath);

	Ov_ForEachChild(opcua_serverToInterfaces, pServer, pInterfaceCheck){
		Ov_GetVTablePtr(opcua_interface, pVtblInterface, pInterfaceCheck);
		if(pVtblInterface){
			if(pVtblInterface->m_checkNode(pInterfaceCheck, pobj, virtualPath)){
				ov_string_setvalue(&virtualPath, NULL);
				return pInterfaceCheck->v_trafo->insertNode(pInterfaceCheck, node, parrentNode);
			}
		}
	}
	ov_string_setvalue(&virtualPath, NULL);
	return pServer->v_ovTrafo->insertNode(context, node, parrentNode);
}
static UA_StatusCode opcua_ovSwitch_replaceNode(void *context, UA_Node *node){
	OV_INSTPTR_opcua_server pServer = (OV_INSTPTR_opcua_server)context;
	if(pServer == NULL)
		return UA_STATUSCODE_BADNODEIDEXISTS;
		
	OV_INSTPTR_opcua_interface pInterfaceCheck = NULL;
	OV_VTBLPTR_opcua_interface pVtblInterface = NULL; //TODO use Call makro instead?
	OV_INSTPTR_ov_object pobj = NULL;
	OV_STRING virtualPath = NULL;

	opcua_ovSwitch_getObjData((const UA_NodeId*)&node->nodeId, &pobj, &virtualPath);

	Ov_ForEachChild(opcua_serverToInterfaces, pServer, pInterfaceCheck){
		Ov_GetVTablePtr(opcua_interface, pVtblInterface, pInterfaceCheck);
		if(pVtblInterface){
			if(pVtblInterface->m_checkNode(pInterfaceCheck, pobj, virtualPath)){
				ov_string_setvalue(&virtualPath, NULL);
				return pInterfaceCheck->v_trafo->replaceNode(pInterfaceCheck, node);
			}
		}
	}
	ov_string_setvalue(&virtualPath, NULL);
	return pServer->v_ovTrafo->replaceNode(context, node);
}

UA_NodestoreInterface*
opcua_ovSwitch_new(OV_INSTPTR_opcua_server context) {
	UA_NodestoreInterface* nsi = (UA_NodestoreInterface*)UA_malloc(sizeof(UA_NodestoreInterface));
	if(nsi == NULL)
		return NULL;
    nsi->context =        	context;
    nsi->deleteNodestore =  opcua_ovSwitch_deleteNodestore;
    nsi->newNode =       	opcua_ovSwitch_newNode;
    nsi->deleteNode =    	opcua_ovSwitch_deleteNode;
    nsi->insertNode =       opcua_ovSwitch_insertNode;
    nsi->getNode =          opcua_ovSwitch_getNode;
    nsi->getNodeCopy =      opcua_ovSwitch_getNodeCopy;
    nsi->replaceNode =      opcua_ovSwitch_replaceNode;
    nsi->removeNode =       opcua_ovSwitch_removeNode;
    nsi->iterate =          opcua_ovSwitch_iterate;
    nsi->releaseNode =      opcua_ovSwitch_releaseNode;
    return nsi;
}
void
opcua_ovSwitch_delete(UA_NodestoreInterface * nodestoreInterface){
	if (nodestoreInterface){
		if (nodestoreInterface->context)
			nodestoreInterface->context = NULL;
		UA_free(nodestoreInterface);
		nodestoreInterface = NULL;
	}
}
