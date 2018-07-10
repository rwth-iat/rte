/*
 * service_helper.c
 *
 *  Created on: 15.05.2018
 *      Author: ubuntu
 */

#include "service_helper.h"

OV_RESULT checkSecurityKey(OV_STRING_VEC DBWrapperPath, OV_STRING componentID, OV_STRING securityKey){
	// check SecurityKey in Database
	OV_RESULT resultOV = OV_ERR_OK;
	OV_STRING table  = "SecurityData";
	OV_STRING fields = "SecurityKey";
	OV_STRING whereFields = "ComponentID";
	OV_STRING whereValues = NULL;
	ov_string_print(&whereValues,"'%s'", componentID);
	OV_STRING_VEC result;
	result.value = NULL;
	result.veclen = 0;

	OV_BOOL securityCheckSuccessful = FALSE;
	OV_INSTPTR_openAASDiscoveryServer_DBWrapper pDBWrapper = NULL;
	OV_VTBLPTR_openAASDiscoveryServer_DBWrapper pDBWrapperVTable;
	for (OV_UINT i = 0; i < DBWrapperPath.veclen; i++){
		pDBWrapper = Ov_DynamicPtrCast(openAASDiscoveryServer_DBWrapper, ov_path_getobjectpointer(DBWrapperPath.value[i], 2));
		if (!pDBWrapper)
			break;

		Ov_GetVTablePtr(openAASDiscoveryServer_DBWrapper,pDBWrapperVTable, pDBWrapper);
		resultOV = pDBWrapperVTable->m_selectData(table, &fields, 1, &whereFields, 1, &whereValues, 1, &result);
		if (resultOV != OV_ERR_OK){
			pDBWrapper->v_ErrorFlag = TRUE;
			ov_string_setvalue(&pDBWrapper->v_ErrorMessage, "Internal Error: SQL error");
			ov_logfile_error("Internal Error: SQL error");
			return OV_ERR_GENERIC;
		}
		for (OV_UINT j = 0; j < result.veclen; j++){
			if (ov_string_compare(result.value[j], securityKey) == OV_STRCMP_EQUAL){
				securityCheckSuccessful = TRUE;
				break;
			}
		}
		if (securityCheckSuccessful == TRUE){
			Ov_SetDynamicVectorLength(&result, 0, STRING);
			break;
		}
	}
	ov_string_setvalue(&whereValues, NULL);

	if (securityCheckSuccessful == FALSE){
		return OV_ERR_GENERIC;
	}
	return OV_ERR_OK;
}
