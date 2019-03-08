
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
#include "opcua_ovStore.h"
#include "opcua_helpers.h"

static UA_StatusCode opcua_interface_setNamespace(UA_Server* server, const UA_String uriOld, const UA_String uriNew, size_t * indexOut){
	size_t index = 0;
	// Get the namespace index of the old uri
	UA_StatusCode result = UA_STATUSCODE_GOOD;
	result = UA_Server_getNamespaceByName(server, uriOld, &index);
	if(result == UA_STATUSCODE_BADNOTFOUND){
		return result;
	}
	if(indexOut)
		*indexOut = index;

	// Read namespace array from server
	UA_Variant namespaceArray;
	result = UA_Server_readValue(server, UA_NODEID_NUMERIC(0, UA_NS0ID_SERVER_NAMESPACEARRAY), &namespaceArray);
	if(result != UA_STATUSCODE_GOOD){
		return result;
	}

	// Replace namespace URI
	UA_String_deleteMembers(&(((UA_String*)namespaceArray.data)[index]));
	if(uriNew.data == NULL)
		UA_String_init(&(((UA_String*)namespaceArray.data)[index]));
	else{
		result = UA_String_copy(&uriNew, &(((UA_String*)namespaceArray.data)[index]));
		if(result != UA_STATUSCODE_GOOD){
			return result;
		}
	}

	// Write back new namespace array
	result = UA_Server_writeValue(server, UA_NODEID_NUMERIC(0, UA_NS0ID_SERVER_NAMESPACEARRAY), namespaceArray);
	if(result != UA_STATUSCODE_GOOD){
		return result;
	}

	// Free resources
	UA_Variant_deleteMembers(&namespaceArray);
	return result;
}

OV_DLLFNCEXPORT OV_STRING opcua_interface_uri_get(
    OV_INSTPTR_opcua_interface          pobj
) {
	OV_STRING uri = NULL;
	opcua_helpers_copyUAStringToOV(pobj->v_trafo->uri, &uri);//TODO check for memleak
    return uri;
}

OV_DLLFNCEXPORT OV_RESULT opcua_interface_uri_set(
    OV_INSTPTR_opcua_interface          pobj,
    const OV_STRING  value
) {
	// Check for valid namespace uri
	if(ov_string_getlength(value) == 0){
		return OV_ERR_BADNAME;
	}
	OV_INSTPTR_opcua_server server = Ov_GetParent(opcua_serverToInterfaces, pobj);
	// Change trafo
	if(pobj->v_trafo){
		// Append types suffix and convert to UA string
		UA_String trafoUri = UA_String_fromChars(value);
		// Set namespace in server namespace array
		if(server != NULL && server->v_isRunning)
			opcua_interface_setNamespace(server->v_server, pobj->v_trafo->uri, trafoUri, NULL);
		// Change internal URIs
		UA_String_deleteMembers(&pobj->v_trafo->uri);
		pobj->v_trafo->uri = trafoUri;
	}
	// Change types
	if(pobj->v_types){
		// Append types suffix and convert to UA string
		OV_STRING ovTypesURI = NULL;
		ov_string_setvalue(&ovTypesURI, value);
		ov_string_append(&ovTypesURI, OPCUA_OVSTORE_DEFAULTNSURI_SUFFIX);
		UA_String typesUri = UA_String_fromChars(ovTypesURI);
		ov_string_setvalue(&ovTypesURI, NULL);
		// Set namespace in server namespace array
		if(server != NULL && server->v_isRunning)
			opcua_interface_setNamespace(server->v_server, pobj->v_types->uri, typesUri, NULL);
		// Change internal URIs
		UA_String_deleteMembers(&pobj->v_types->uri);
		pobj->v_types->uri = typesUri;
	}

    return OV_ERR_OK;
}

//TODO use macro
OV_DLLFNCEXPORT OV_ACCESS opcua_interface_getaccess(
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

static UA_StatusCode addInformationModel(UA_Server * server, OPCUA_InformationModel * model, OV_BOOL forceLoad){
	size_t index = 0;
	if(!forceLoad){
		if(UA_Server_getNamespaceByName(server, model->uri, &index) == UA_STATUSCODE_GOOD){
			return OV_ERR_ALREADYEXISTS;
		}
	}
	UA_ServerConfig * config = UA_Server_getConfig(server);

	// Add NamespaceUri
	// Try to replace empty namespace otherwise add to the end
	if(opcua_interface_setNamespace(server, UA_STRING_NULL, model->uri, &index) != UA_STATUSCODE_GOOD){
		OV_STRING uri = NULL;
		opcua_helpers_copyUAStringToOV(model->uri, &uri);
		model->index = UA_Server_addNamespace(server, uri);
		ov_string_setvalue(&uri, NULL);
	}

	// Add Datatypes
	if(model->dataTypes){
		model->dataTypes->next = config->customDataTypes;
		config->customDataTypes = model->dataTypes;
	}

	// Link the nodestore (transformation) to the namespace in the switch
	if(model->store){
		return UA_NodestoreSwitch_linkNodestoreToNamespace((UA_NodestoreSwitch*)config->nodestore.context, model->store, model->index);
	}

	// Load predefined nodeset
	if(model->nodeset){
		return model->nodeset(server);
	}
	return UA_STATUSCODE_GOOD;
}

//TODO delete all nodes from namespace (and all references)
static OV_RESULT removeInformationModel(UA_Server * server, OPCUA_InformationModel * model){
	// Mark namespace uri as deleted
	opcua_interface_setNamespace(server, model->uri, UA_STRING_NULL, NULL);

	// Remove Datatypes
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

	// Unlink the nodestore from the namespace
	UA_NodestoreSwitch_unlinkNodestoreFromNamespace((UA_NodestoreSwitch*)config->nodestore.context, model->store);
	return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_RESULT opcua_interface_load(OV_INSTPTR_opcua_interface pobj, OV_BOOL forceLoad) {

	OV_INSTPTR_opcua_server server = Ov_GetParent(opcua_serverToInterfaces, pobj);
	if(server == NULL){
		return OV_ERR_GENERIC;
	}

	//Load all dependent interfaces first
	OV_INSTPTR_opcua_interface dependentInterface = NULL;
	Ov_ForEachChild(opcua_interfaceDependency, pobj, dependentInterface){
		opcua_interface_load(dependentInterface, FALSE);
		//TODO error handling
	}

	// add Trafo
	if (pobj->v_trafo != NULL){
		addInformationModel(server->v_server, pobj->v_trafo, forceLoad);
		//TODO error handling
	}

	// add Types
	if (pobj->v_types != NULL){
		addInformationModel(server->v_server, pobj->v_types, forceLoad);
		//TODO error handling
	}
    return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_RESULT opcua_interface_unload(OV_INSTPTR_opcua_interface pobj) {

	OV_INSTPTR_opcua_server server = Ov_GetParent(opcua_serverToInterfaces, pobj);
	if(server == NULL){
		return OV_ERR_GENERIC;
	}

	// unload all dependent interfaces
	OV_INSTPTR_opcua_interface dependentInterface = Ov_GetParent(opcua_interfaceDependency, pobj);
	if(dependentInterface != NULL){
		opcua_interface_unload(dependentInterface);
		//TODO error handling
	}

	// unload Types
	if (pobj->v_types != NULL){
		removeInformationModel(server->v_server, pobj->v_types);
		//TODO error handling
	}

	// unload Trafo
	if (pobj->v_trafo != NULL){
		removeInformationModel(server->v_server, pobj->v_trafo);
		//TODO error handling
	}
    return OV_ERR_OK;
}
