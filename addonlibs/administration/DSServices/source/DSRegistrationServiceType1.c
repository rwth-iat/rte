
/******************************************************************************
*
*   FILE
*   ----
*   DSRegistrationServiceType1.c
*
*   History
*   -------
*   2018-05-14   File created
*
*******************************************************************************
*
*   This file is generated by the 'acplt_builder' command
*
******************************************************************************/


#ifndef OV_COMPILE_LIBRARY_DSServices
#define OV_COMPILE_LIBRARY_DSServices
#endif


#include "DSServices.h"
#include "json_helper.h"
#include "service_helper.h"

struct endpoint{
	OV_STRING protocolType;
	OV_STRING endpointString;
};

struct statement{
	OV_STRING carrierID;
	OV_STRING propertyID;
	OV_STRING expressionSemantic;
	OV_STRING relation;
	OV_STRING value;
	OV_STRING valueType;
	OV_STRING subModel;
};

OV_DLLFNCEXPORT OV_RESULT DSServices_DSRegistrationServiceType1_executeService(OV_INSTPTR_openAASDiscoveryServer_DSService pinst, const json_data JsonInput, OV_STRING *JsonOutput, OV_STRING *errorMessage) {
    /*    
    *   local variables
    */
	// Parsing Body
	OV_UINT endpointArraySize = 0;
	struct endpoint *endpoints = NULL;
	OV_UINT statementsArraySize = 0;
	struct statement *statements = NULL;
	OV_STRING_VEC jsonTags;
	jsonTags.value = NULL;
	jsonTags.veclen = 0;
	Ov_SetDynamicVectorLength(&jsonTags, 4, STRING);
	ov_string_setvalue(&jsonTags.value[0], "componentID");
	ov_string_setvalue(&jsonTags.value[1], "securityKey");
	ov_string_setvalue(&jsonTags.value[2], "endpoints");
	ov_string_setvalue(&jsonTags.value[3], "statements");
	OV_UINT_VEC tokenIndex;
	tokenIndex.value = NULL;
	tokenIndex.veclen = 0;
	Ov_SetDynamicVectorLength(&tokenIndex, 4, UINT);

	jsonGetTokenIndexByTags(jsonTags, JsonInput, 1, &tokenIndex);

	OV_STRING componentID = NULL;
	jsonGetValueByToken(JsonInput.js, &JsonInput.token[tokenIndex.value[0]+1], &componentID);
	OV_STRING securityKey = NULL;
	jsonGetValueByToken(JsonInput.js, &JsonInput.token[tokenIndex.value[1]+1], &securityKey);

	// check securityKey
	OV_RESULT resultOV = checkSecurityKey(pinst->v_DBWrapperUsed, componentID, securityKey);
	if (resultOV){
		ov_string_setvalue(errorMessage, "SecurityKey is not correct");
		goto FINALIZE;
	}

	// get registrationData
	// get endpoints from JSON
	endpointArraySize = JsonInput.token[tokenIndex.value[2]+1].size;
	endpoints = malloc(sizeof(struct endpoint)*endpointArraySize);
	for (OV_UINT i = 0; i < endpointArraySize; i++){
		endpoints[i].protocolType = NULL;
		// value + 2 start of objects + i*5 next object + 2/4 values of protocolType and endpoint
		jsonGetValueByToken(JsonInput.js, &JsonInput.token[tokenIndex.value[2]+2+i*5+2], &endpoints[i].protocolType);
		endpoints[i].endpointString = NULL;
		jsonGetValueByToken(JsonInput.js, &JsonInput.token[tokenIndex.value[2]+2+i*5+4], &endpoints[i].endpointString);
	}

	// get statements from JSON
	statementsArraySize = JsonInput.token[tokenIndex.value[3]+1].size;
	statements = malloc(sizeof(struct statement)*statementsArraySize);
	for (OV_UINT i = 0; i < statementsArraySize; i++){
		// value + 2 start of objects + i*15 next object + 2 values of carrierID
		statements[i].carrierID = NULL;
		jsonGetValueByToken(JsonInput.js, &JsonInput.token[tokenIndex.value[3]+2+i*15+2], &statements[i].carrierID);
		// value + 2 start of objects + i*15 next object + 4 values of propertyID
		statements[i].propertyID = NULL;
		jsonGetValueByToken(JsonInput.js, &JsonInput.token[tokenIndex.value[3]+2+i*15+4], &statements[i].propertyID);
		// value + 2 start of objects + i*15 next object + 6 values of expressionSemantic
		statements[i].expressionSemantic = NULL;
		jsonGetValueByToken(JsonInput.js, &JsonInput.token[tokenIndex.value[3]+2+i*15+6], &statements[i].expressionSemantic);
		// value + 2 start of objects + i*15 next object + 8 values of relation
		statements[i].relation = NULL;
		jsonGetValueByToken(JsonInput.js, &JsonInput.token[tokenIndex.value[3]+2+i*15+8], &statements[i].relation);
		// value + 2 start of objects + i*15 next object + 10 values of value
		statements[i].value = NULL;
		jsonGetValueByToken(JsonInput.js, &JsonInput.token[tokenIndex.value[3]+2+i*15+10], &statements[i].value);
		// value + 2 start of objects + i*15 next object + 10 values of value
		statements[i].valueType = NULL;
		jsonGetValueByToken(JsonInput.js, &JsonInput.token[tokenIndex.value[3]+2+i*15+12], &statements[i].valueType);
		// value + 2 start of objects + i*15 next object + 12 values of subModel
		statements[i].subModel = NULL;
		jsonGetValueByToken(JsonInput.js, &JsonInput.token[tokenIndex.value[3]+2+i*15+14], &statements[i].subModel);
	}

	// delete old registered Data
	// TODO: Extend to multiDBWrapper
	OV_INSTPTR_openAASDiscoveryServer_DBWrapper pDBWrapper = NULL;
	OV_VTBLPTR_openAASDiscoveryServer_DBWrapper pDBWrapperVTable = NULL;
	pDBWrapper = Ov_DynamicPtrCast(openAASDiscoveryServer_DBWrapper, ov_path_getobjectpointer(pinst->v_DBWrapperUsed.value[0], 2));
	if (!pDBWrapper){
		ov_string_setvalue(errorMessage, "Internel Error");
		ov_logfile_error("Could not find DBWrapper Object");
		goto FINALIZE;
	}
	Ov_GetVTablePtr(openAASDiscoveryServer_DBWrapper,pDBWrapperVTable, pDBWrapper);
	OV_STRING table  = NULL;
	OV_STRING tmpField = "ComponentID";
	OV_STRING tmpValue = NULL;
	ov_string_print(&tmpValue, "'%s'", componentID);
	table  = "Endpoints";
	resultOV = pDBWrapperVTable->m_deleteData(pDBWrapper, table, &tmpField, 1, &tmpValue, 1);
	table  = "statements_TextBoolean";
	resultOV = pDBWrapperVTable->m_deleteData(pDBWrapper, table, &tmpField, 1, &tmpValue, 1);
	table  = "statements_Numeric";
	resultOV = pDBWrapperVTable->m_deleteData(pDBWrapper, table, &tmpField, 1, &tmpValue, 1);
	table  = "PropertyID";
	resultOV = pDBWrapperVTable->m_deleteData(pDBWrapper, table, &tmpField, 1, &tmpValue, 1);
	table  = "CarrierID";
	resultOV = pDBWrapperVTable->m_deleteData(pDBWrapper, table, &tmpField, 1, &tmpValue, 1);
	table  = "ExpressionSemantic";
	resultOV = pDBWrapperVTable->m_deleteData(pDBWrapper, table, &tmpField, 1, &tmpValue, 1);
	table  = "Relation";
	resultOV = pDBWrapperVTable->m_deleteData(pDBWrapper, table, &tmpField, 1, &tmpValue, 1);
	table  = "Submodel";
	resultOV = pDBWrapperVTable->m_deleteData(pDBWrapper, table, &tmpField, 1, &tmpValue, 1);
	ov_string_setvalue(&tmpValue, NULL);

	// Insert endpoints in database
	// TODO: Extend to multiDBWrapper
	pDBWrapper = NULL;
	pDBWrapperVTable = NULL;
	pDBWrapper = Ov_DynamicPtrCast(openAASDiscoveryServer_DBWrapper, ov_path_getobjectpointer(pinst->v_DBWrapperUsed.value[0], 2));
	if (!pDBWrapper){
		ov_string_setvalue(errorMessage, "Internel Error");
		ov_logfile_error("Could not find DBWrapper Object");
		goto FINALIZE;
	}
	Ov_GetVTablePtr(openAASDiscoveryServer_DBWrapper,pDBWrapperVTable, pDBWrapper);
	table  = "Endpoints";
	OV_STRING tmpFields[3];
	OV_STRING tmpValues[3];
	tmpFields[0] = "ComponentID";
	tmpFields[1] = "ProtocolType";
	tmpFields[2] = "EndpointString";
	tmpValues[0] = NULL;
	ov_string_print(&tmpValues[0], "'%s'", componentID);
	for (OV_UINT i = 0; i < endpointArraySize; i++){
		tmpValues[1] = NULL;
		ov_string_print(&tmpValues[1], "'%s'", endpoints[i].protocolType);
		tmpValues[2] = NULL;
		ov_string_print(&tmpValues[2], "'%s'", endpoints[i].endpointString);
		resultOV = pDBWrapperVTable->m_insertData(pDBWrapper, table, tmpFields, 3, tmpValues, 3);
		ov_string_setvalue(&tmpValues[0], NULL);
		ov_string_setvalue(&tmpValues[1], NULL);
		ov_string_setvalue(&tmpValues[2], NULL);
		if (resultOV != OV_ERR_OK){
			ov_string_setvalue(errorMessage, "Internal Error");
			ov_logfile_error("Could not insert endpoints in database");
			goto FINALIZE;
		}
	}

	// Insert statements in database
	OV_STRING tmpFieldsStatements[7];
	tmpFieldsStatements[0] = "ComponentID";
	tmpFieldsStatements[1] = "CarrierID";
	tmpFieldsStatements[2] = "PropertyID";
	tmpFieldsStatements[3] = "ExpressionSemantic";
	tmpFieldsStatements[4] = "Relation";
	tmpFieldsStatements[5] = "Value";
	tmpFieldsStatements[6] = "SubModel";
	OV_STRING tmpValuesStatements[7];

	OV_STRING tmpFieldsList[2];
	tmpFieldsList[0] = "ComponentID";
	tmpFieldsList[1] = "Value";
	OV_STRING tmpValuesList[2];

	for (OV_UINT i = 0; i < statementsArraySize; i++){
		tmpValuesStatements[0] = NULL;
		tmpValuesList[0] = NULL;
		ov_string_print(&tmpValuesStatements[0], "'%s'", componentID);
		ov_string_print(&tmpValuesList[0], "'%s'", componentID);

		tmpValuesStatements[1] = NULL;
		ov_string_print(&tmpValuesStatements[1], "'%s'", statements[i].carrierID);
		tmpValuesList[1] = NULL;
		ov_string_print(&tmpValuesList[1], "'%s'", statements[i].carrierID);
		table = "CarrierID";
		resultOV = pDBWrapperVTable->m_insertData(pDBWrapper, table, tmpFieldsList, 2, tmpValuesList, 2);
		ov_string_setvalue(&tmpValuesList[1], NULL);

		tmpValuesStatements[2] = NULL;
		ov_string_print(&tmpValuesStatements[2], "'%s'", statements[i].propertyID);
		tmpValuesList[1] = NULL;
		ov_string_print(&tmpValuesList[1], "'%s'", statements[i].propertyID);
		table = "PropertyID";
		resultOV = pDBWrapperVTable->m_insertData(pDBWrapper, table, tmpFieldsList, 2, tmpValuesList, 2);
		ov_string_setvalue(&tmpValuesList[1], NULL);

		tmpValuesList[1] = NULL;
		tmpValuesStatements[3] = NULL;
		if (statements[i].expressionSemantic == NULL)
			ov_string_print(&tmpValuesStatements[3], "''");
		else
			ov_string_print(&tmpValuesStatements[3], "'%s'", statements[i].expressionSemantic);
		tmpValuesList[1] = NULL;
		if (statements[i].expressionSemantic == NULL)
			ov_string_print(&tmpValuesList[1], "''");
		else
			ov_string_print(&tmpValuesList[1], "'%s'", statements[i].expressionSemantic);
		table = "ExpressionSemantic";
		resultOV = pDBWrapperVTable->m_insertData(pDBWrapper, table, tmpFieldsList, 2, tmpValuesList, 2);
		ov_string_setvalue(&tmpValuesList[1], NULL);

		tmpValuesList[1] = NULL;
		tmpValuesStatements[4] = NULL;
		ov_string_print(&tmpValuesStatements[4], "'%s'", statements[i].relation);
		tmpValuesList[1] = NULL;
		ov_string_print(&tmpValuesList[1], "'%s'", statements[i].relation);
		table = "Relation";
		resultOV = pDBWrapperVTable->m_insertData(pDBWrapper, table, tmpFieldsList, 2, tmpValuesList, 2);
		ov_string_setvalue(&tmpValuesList[1], NULL);

		tmpValuesList[1] = NULL;
		tmpValuesStatements[5] = NULL;
		if (statements[i].value == NULL)
			ov_string_print(&tmpValuesStatements[5], "''");
		else
			ov_string_print(&tmpValuesStatements[5], "'%s'", statements[i].value);

		tmpValuesList[1] = NULL;
		tmpValuesStatements[6] = NULL;
		ov_string_print(&tmpValuesStatements[6], "'%s'", statements[i].subModel);
		tmpValuesList[1] = NULL;
		ov_string_print(&tmpValuesList[1], "'%s'", statements[i].subModel);
		table = "SubModel";
		resultOV = pDBWrapperVTable->m_insertData(pDBWrapper, table, tmpFieldsList, 2, tmpValuesList, 2);
		ov_string_setvalue(&tmpValuesList[1], NULL);
		ov_string_setvalue(&tmpValuesList[0], NULL);

		if (ov_string_compare(statements[i].valueType, "Numeric") == OV_STRCMP_EQUAL){
			table = "statements_Numeric";
		}else{
			table = "statements_TextBoolean";
		}

		resultOV = pDBWrapperVTable->m_insertData(pDBWrapper, table, tmpFieldsStatements, 7, tmpValuesStatements, 7);
		for (OV_UINT i = 0; i < 7; i++){
			ov_string_setvalue(&tmpValuesStatements[i], NULL);
		}
		if (resultOV != OV_ERR_OK){
			ov_string_setvalue(errorMessage, "Internal Error");
			ov_logfile_error("Could not insert statements in database");
			goto FINALIZE;
		}
	}

	FINALIZE:
	ov_string_print(JsonOutput, "\"body\":{}");
	Ov_SetDynamicVectorLength(&jsonTags, 0, STRING);
	Ov_SetDynamicVectorLength(&tokenIndex, 0, UINT);
	ov_string_setvalue(&componentID, NULL);
	ov_string_setvalue(&securityKey, NULL);
	for (OV_UINT i = 0; i < endpointArraySize; i++){
		ov_string_setvalue(&endpoints[i].protocolType, NULL);
		ov_string_setvalue(&endpoints[i].endpointString, NULL);
	}
	if (endpoints)
		free(endpoints);
	for (OV_UINT i = 0; i < statementsArraySize; i++){
		ov_string_setvalue(&statements[i].carrierID, NULL);
		ov_string_setvalue(&statements[i].propertyID, NULL);
		ov_string_setvalue(&statements[i].expressionSemantic, NULL);
		ov_string_setvalue(&statements[i].relation, NULL);
		ov_string_setvalue(&statements[i].value, NULL);
		ov_string_setvalue(&statements[i].valueType, NULL);
		ov_string_setvalue(&statements[i].subModel, NULL);
	}
	if (statements)
		free(statements);
    return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_ACCESS DSServices_DSRegistrationServiceType1_getaccess(
	OV_INSTPTR_ov_object	pobj,
	const OV_ELEMENT		*pelem,
	const OV_TICKET			*pticket
) {
    /*    
    *   local variables
    */

	switch(pelem->elemtype) {
		case OV_ET_VARIABLE:
			if(pelem->elemunion.pvar->v_offset >= offsetof(OV_INST_ov_object,__classinfo)) {
				if(pelem->elemunion.pvar->v_vartype == OV_VT_CTYPE)
					return OV_AC_NONE;
				else{
					if(pelem->elemunion.pvar->v_flags == 256) { // InputFlag is set
						return OV_AC_READWRITE;
					}
					/* Nicht FB? */
					if(pelem->elemunion.pvar->v_varprops & OV_VP_SETACCESSOR) {
						return OV_AC_READWRITE;
					}
					return OV_AC_READ;
				}
			}
		break;
		default:
		break;
	}

	return ov_object_getaccess(pobj, pelem, pticket);
}

