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
#include "ov_macros.h"
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
	OV_INSTPTR_opcua_server pServer = (OV_INSTPTR_opcua_server)context;
	pServer->v_ovTrafo->clear(pServer->v_ovTrafo->context);
	UA_free(pServer->v_ovTrafo);
	pServer->v_ovTrafo = NULL;

	OV_INSTPTR_opcua_interface pInterface = Ov_GetChild(opcua_serverToInterfaces, pServer);
	if(pInterface && pInterface->v_trafo){
		pInterface->v_trafo->clear(pInterface->v_trafo->context);
	}
}

static void opcua_ovSwitch_deleteNode(void * context, UA_Node *node){
	OV_INSTPTR_opcua_server pServer = (OV_INSTPTR_opcua_server)context;
	if(pServer == NULL)
		return ;

	OV_INSTPTR_ov_object pobj = NULL;
	OV_STRING virtualPath = NULL;

	opcua_ovSwitch_getObjData((const UA_NodeId*)&node->head.nodeId, &pobj, &virtualPath);

	OV_INSTPTR_opcua_interface pInterface = Ov_GetChild(opcua_serverToInterfaces, pServer);
	if(pInterface){
		if(Ov_Call2(opcua_interface, pInterface, checkNode, pobj, virtualPath)){
			ov_string_setvalue(&virtualPath, NULL);
			return pInterface->v_trafo->deleteNode(pInterface, node);
		}
		else
		{
			if(pInterface->v_useOvTrafo){
				ov_string_setvalue(&virtualPath, NULL);
				return pServer->v_ovTrafo->deleteNode(context, node);
			}
		}
	} else {
		ov_string_setvalue(&virtualPath, NULL);
		pServer->v_ovTrafo->deleteNode(context, node);
	}
}
static void opcua_ovSwitch_releaseNode(void *context, const UA_Node *node){
	OV_INSTPTR_opcua_server pServer = (OV_INSTPTR_opcua_server)context;
	if(pServer == NULL)
		return ;

	OV_INSTPTR_ov_object pobj = NULL;
	OV_STRING virtualPath = NULL;

	opcua_ovSwitch_getObjData((const UA_NodeId*)&node->head.nodeId, &pobj, &virtualPath);

	OV_INSTPTR_opcua_interface pInterface = Ov_GetChild(opcua_serverToInterfaces, pServer);
	if(pInterface){
		if(Ov_Call2(opcua_interface, pInterface, checkNode, pobj, virtualPath)){
			ov_string_setvalue(&virtualPath, NULL);
			return pInterface->v_trafo->releaseNode(pInterface, node);
		}
		else
		{
			if(pInterface->v_useOvTrafo){
				ov_string_setvalue(&virtualPath, NULL);
				return pServer->v_ovTrafo->releaseNode(context, node);
			}
		}
	} else {
		ov_string_setvalue(&virtualPath, NULL);
		pServer->v_ovTrafo->releaseNode(context, node);
	}
}

static void opcua_ovSwitch_iterate(void *context, UA_NodestoreVisitor visitor, void* visitorHandle){
}
static UA_Node * opcua_ovSwitch_newNode(void * context, UA_NodeClass nodeClass){ // TODO: No NodeId
	OV_INSTPTR_opcua_server pServer = (OV_INSTPTR_opcua_server)context;
	if(pServer == NULL)
		return NULL;

	return pServer->v_ovTrafo->newNode(context, nodeClass);
}

static const UA_Node * opcua_ovSwitch_getNode(void *context, const UA_NodeId *nodeId){
	OV_INSTPTR_opcua_server pServer = (OV_INSTPTR_opcua_server)context;
	if(pServer == NULL)
		return NULL;

	OV_INSTPTR_ov_object pobj = NULL;
	OV_STRING virtualPath = NULL;

	opcua_ovSwitch_getObjData(nodeId, &pobj, &virtualPath);

	OV_INSTPTR_opcua_interface pInterface = Ov_GetChild(opcua_serverToInterfaces, pServer);
	if(pInterface){
		if(Ov_Call2(opcua_interface, pInterface, checkNode, pobj, virtualPath)){
			ov_string_setvalue(&virtualPath, NULL);
			return pInterface->v_trafo->getNode(pInterface, nodeId);
		}
		else
		{
			if(pInterface->v_useOvTrafo){
				ov_string_setvalue(&virtualPath, NULL);
				return pServer->v_ovTrafo->getNode(context, nodeId);
			}
		}
		
	} else {
		ov_string_setvalue(&virtualPath, NULL);
		return pServer->v_ovTrafo->getNode(context, nodeId);
	}
	return NULL;
}
static UA_StatusCode opcua_ovSwitch_getNodeCopy(void *context, const UA_NodeId *nodeId, UA_Node ** nodeOut){
	OV_INSTPTR_opcua_server pServer = (OV_INSTPTR_opcua_server)context;
	if(pServer == NULL)
		return UA_STATUSCODE_BADNODEIDUNKNOWN;

	OV_INSTPTR_ov_object pobj = NULL;
	OV_STRING virtualPath = NULL;

	opcua_ovSwitch_getObjData(nodeId, &pobj, &virtualPath);

	OV_INSTPTR_opcua_interface pInterface = Ov_GetChild(opcua_serverToInterfaces, pServer);
	if(pInterface){
		if(Ov_Call2(opcua_interface, pInterface, checkNode, pobj, virtualPath)){
			ov_string_setvalue(&virtualPath, NULL);
			return pInterface->v_trafo->getNodeCopy(pInterface, nodeId, nodeOut);
		}
		else
		{
			if(pInterface->v_useOvTrafo){
				ov_string_setvalue(&virtualPath, NULL);
				return pServer->v_ovTrafo->getNodeCopy(context, nodeId, nodeOut);
			}
		}
	} else {
		ov_string_setvalue(&virtualPath, NULL);
		return pServer->v_ovTrafo->getNodeCopy(context, nodeId, nodeOut);
	}
	return UA_STATUSCODE_BADNODEIDUNKNOWN;
}
static UA_StatusCode opcua_ovSwitch_removeNode(void *context, const UA_NodeId *nodeId){
	OV_INSTPTR_opcua_server pServer = (OV_INSTPTR_opcua_server)context;
	if(pServer == NULL)
		return UA_STATUSCODE_BADNODEIDUNKNOWN;

	OV_INSTPTR_ov_object pobj = NULL;
	OV_STRING virtualPath = NULL;

	opcua_ovSwitch_getObjData(nodeId, &pobj, &virtualPath);

	OV_INSTPTR_opcua_interface pInterface = Ov_GetChild(opcua_serverToInterfaces, pServer);
	if(pInterface){
		if(Ov_Call2(opcua_interface, pInterface, checkNode, pobj, virtualPath)){
			ov_string_setvalue(&virtualPath, NULL);
			return pInterface->v_trafo->removeNode(pInterface, nodeId);
		}
		else
		{
			if(pInterface->v_useOvTrafo){
				ov_string_setvalue(&virtualPath, NULL);
				return pServer->v_ovTrafo->removeNode(context, nodeId);
			}
		}
	} else {
		ov_string_setvalue(&virtualPath, NULL);
		return pServer->v_ovTrafo->removeNode(context, nodeId);
	}
	return UA_STATUSCODE_BADNODEIDUNKNOWN;
}
static UA_StatusCode opcua_ovSwitch_insertNode(void *context, UA_Node *node, UA_NodeId *parrentNode){
	OV_INSTPTR_opcua_server pServer = (OV_INSTPTR_opcua_server)context;
	if(pServer == NULL)
		return UA_STATUSCODE_BADNODEIDUNKNOWN;

	OV_INSTPTR_ov_object pobj = NULL;
	OV_STRING virtualPath = NULL;

	opcua_ovSwitch_getObjData((const UA_NodeId*)&node->head.nodeId, &pobj, &virtualPath);

	OV_INSTPTR_opcua_interface pInterface = Ov_GetChild(opcua_serverToInterfaces, pServer);
	if(pInterface){
		if(Ov_Call2(opcua_interface, pInterface, checkNode, pobj, virtualPath)){
			ov_string_setvalue(&virtualPath, NULL);
			return pInterface->v_trafo->insertNode(pInterface, node, parrentNode);
		}
		else
		{
			if(pInterface->v_useOvTrafo){
				ov_string_setvalue(&virtualPath, NULL);
				return pServer->v_ovTrafo->insertNode(context, node, parrentNode);
			}
		}
	} else {
		ov_string_setvalue(&virtualPath, NULL);
		return pServer->v_ovTrafo->insertNode(context, node, parrentNode);
	}
	return UA_STATUSCODE_BADNODEIDUNKNOWN;
}
static UA_StatusCode opcua_ovSwitch_replaceNode(void *context, UA_Node *node){
	OV_INSTPTR_opcua_server pServer = (OV_INSTPTR_opcua_server)context;
	if(pServer == NULL)
		return UA_STATUSCODE_BADNODEIDUNKNOWN;
		
	OV_INSTPTR_ov_object pobj = NULL;
	OV_STRING virtualPath = NULL;

	opcua_ovSwitch_getObjData((const UA_NodeId*)&node->head.nodeId, &pobj, &virtualPath);

	OV_INSTPTR_opcua_interface pInterface = Ov_GetChild(opcua_serverToInterfaces, pServer);
	if(pInterface){
		if(Ov_Call2(opcua_interface, pInterface, checkNode, pobj, virtualPath)){
			ov_string_setvalue(&virtualPath, NULL);
			return pInterface->v_trafo->replaceNode(pInterface, node);
		}
		else
		{
			if(pInterface->v_useOvTrafo){
				ov_string_setvalue(&virtualPath, NULL);
				return pServer->v_ovTrafo->replaceNode(context, node);
			}
		}
	} else {
		ov_string_setvalue(&virtualPath, NULL);
		return pServer->v_ovTrafo->replaceNode(context, node);
	}
	return UA_STATUSCODE_BADNODEIDUNKNOWN;
}

UA_Nodestore*
opcua_ovSwitch_new(OV_INSTPTR_opcua_server context) {
	UA_Nodestore* nsi = (UA_Nodestore*)UA_malloc(sizeof(UA_Nodestore));
	if(nsi == NULL)
		return NULL;
    nsi->context =        	context;
    nsi->clear =  			opcua_ovSwitch_deleteNodestore;
    nsi->newNode =       	opcua_ovSwitch_newNode;
    nsi->deleteNode =    	opcua_ovSwitch_deleteNode;
    nsi->insertNode =       opcua_ovSwitch_insertNode;
    nsi->getNode =          opcua_ovSwitch_getNode;
	//nsi->getNodeFromPtr =   NULL; //use redirect to getNode in nsSwitch
    nsi->getNodeCopy =      opcua_ovSwitch_getNodeCopy;
    nsi->replaceNode =      opcua_ovSwitch_replaceNode;
    nsi->removeNode =       opcua_ovSwitch_removeNode;
    nsi->iterate =          opcua_ovSwitch_iterate;
    nsi->releaseNode =      opcua_ovSwitch_releaseNode;
    return nsi;
}
void
opcua_ovSwitch_delete(UA_Nodestore * nodestoreInterface){
	if (nodestoreInterface){
		if (nodestoreInterface->context)
			nodestoreInterface->context = NULL;
		UA_free(nodestoreInterface);
		nodestoreInterface = NULL;
	}
}
