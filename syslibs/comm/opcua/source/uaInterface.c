
/******************************************************************************
*
*   FILE
*   ----
*   uaInterface.c
*
*   History
*   -------
*   2019-01-24   File created
*
*******************************************************************************
*
*   This file is generated by the 'acplt_builder' command
*
******************************************************************************/


#ifndef OV_COMPILE_LIBRARY_opcua
#define OV_COMPILE_LIBRARY_opcua
#endif


#include "opcua.h"
#include "libov/ov_macros.h"
#include "opcua_storeSwitch.h"

#include "opcua_helpers.h"


OV_DLLFNCEXPORT OV_ACCESS opcua_uaInterface_getaccess(
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

static UA_StatusCode addInformationModel(UA_Server * server, OV_UA_InformationModel * model, OV_BOOL forceLoad){
	if(!forceLoad){
		size_t index = 0;
		if(UA_Server_getNamespaceByName(server, model->uri, &index) == UA_STATUSCODE_GOOD){
			return OV_ERR_ALREADYEXISTS;
		}
	}
	UA_ServerConfig * config = UA_Server_getConfig(server);

	// add NamespaceUri
	OV_STRING uri = NULL;
	opcua_helpers_copyUAStringToOV(model->uri, &uri);
	model->index = UA_Server_addNamespace(server, uri);
	ov_database_free(uri);

	// add Datatypes
	if(model->dataTypes){
		model->dataTypes->next = config->customDataTypes;
		config->customDataTypes = model->dataTypes;
	}

	if(model->store){
		return UA_NodestoreSwitch_linkNodestoreToNamespace((UA_NodestoreSwitch*)config->nodestore.context, model->store, model->index);
	}
	return UA_STATUSCODE_GOOD;
}

static OV_RESULT removeInformationModel(UA_Server * server, OV_UA_InformationModel * model){
	// Mark namespace uri as deleted
	size_t index = 0;
	if(UA_Server_getNamespaceByName(server, model->uri, &index) == UA_STATUSCODE_BADNOTFOUND){
		return OV_ERR_GENERIC;
	}
	UA_Variant namespaceArray;
	UA_Server_readValue(server, UA_NODEID_NUMERIC(0, UA_NS0ID_SERVER_NAMESPACEARRAY), &namespaceArray);
	UA_String_deleteMembers(&(((UA_String*)namespaceArray.data)[index]));
	UA_String_init(&(((UA_String*)namespaceArray.data)[index]));
	UA_Server_writeValue(server, UA_NODEID_NUMERIC(0, UA_NS0ID_SERVER_NAMESPACEARRAY), namespaceArray);
	UA_Variant_deleteMembers(&namespaceArray);

	// remove Datatypes
	UA_ServerConfig * config = UA_Server_getConfig(server);
	UA_DataTypeArray * dataTypes = config->customDataTypes;
	UA_DataTypeArray * lastDataTypes = NULL;
	while(dataTypes != NULL){
		if(dataTypes == model->dataTypes){
			if(lastDataTypes == NULL){
				config->customDataTypes = dataTypes->next;
			}else{
				lastDataTypes->next = dataTypes->next;
			}
			break;
		}
		lastDataTypes = dataTypes;
		dataTypes = dataTypes->next;
	}

	UA_NodestoreSwitch_unlinkNodestoreFromNamespace((UA_NodestoreSwitch*)config->nodestore.context, model->store);
	return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_RESULT opcua_uaInterface_load(OV_INSTPTR_opcua_uaInterface pinst, OV_BOOL forceLoad) {

	OV_INSTPTR_opcua_uaServer uaServer = Ov_GetParent(opcua_uaServerToInterfaces, pinst);
	if(uaServer == NULL){
		return OV_ERR_GENERIC;
	}

	//Load all dependent interfaces first
	OV_INSTPTR_opcua_uaInterface dependentInterface = NULL;
	Ov_ForEachChild(opcua_uaInterfaceDependency, pinst, dependentInterface){
		opcua_uaInterface_load(dependentInterface, FALSE);
		//TODO error handling
	}

	// add Types
	if (pinst->v_types != NULL){
		addInformationModel(uaServer->v_server, pinst->v_types, forceLoad);
		//TODO error handling
	}

	// add Trafo
	if (pinst->v_trafo != NULL){
		addInformationModel(uaServer->v_server, pinst->v_trafo, forceLoad);
		//TODO error handling
	}
	//TODO add parent reference to first note
    return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_RESULT opcua_uaInterface_unload(OV_INSTPTR_opcua_uaInterface pinst) {
	OV_INSTPTR_opcua_uaServer uaServer = Ov_GetParent(opcua_uaServerToInterfaces, pinst);
	if(uaServer == NULL){
		return OV_ERR_GENERIC;
	}

	// unload all dependent interfaces
	OV_INSTPTR_opcua_uaInterface dependentInterface = Ov_GetParent(opcua_uaInterfaceDependency, pinst);
	if(dependentInterface != NULL){
		opcua_uaInterface_unload(dependentInterface);
		//TODO error handling
	}

	// unload Types
	if (pinst->v_types != NULL){
		removeInformationModel(uaServer->v_server, pinst->v_types);
		//TODO error handling
	}

	// unload Trafo
	if (pinst->v_trafo != NULL){
		removeInformationModel(uaServer->v_server, pinst->v_trafo);
		//TODO error handling
	}
    return OV_ERR_OK;
}

