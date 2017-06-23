/******************************************************************************
 *
 *   FILE
 *   ----
 *   nodeStoreFunctions.c
 *
 *   History
 *   -------
 *   2014-10-21   File created
 *
 *******************************************************************************
 *
 *   This file is generated by the 'acplt_builder' command
 *
 ******************************************************************************/

#ifndef OV_COMPILE_LIBRARY_openaas
#define OV_COMPILE_LIBRARY_openaas
#endif

#include "openaas.h"

/*
OV_DLLFNCEXPORT OV_RESULT openaas_modelmanager_PVSLAASIdString_set(
    OV_INSTPTR_openaas_modelmanager          pobj,
    const OV_STRING  value
) {
    return ov_string_setvalue(&pobj->v_PVSLAASIdString,value);
}


OV_DLLFNCEXPORT OV_RESULT openaas_modelmanager_PVSLAASIdType_set(
    OV_INSTPTR_openaas_modelmanager          pobj,
    const OV_UINT  value
) {
    pobj->v_PVSLAASIdType = value;
    return OV_ERR_OK;
}



OV_DLLFNCEXPORT OV_RESULT openaas_modelmanager_PVSLSubModelIdString_set(
    OV_INSTPTR_openaas_modelmanager          pobj,
    const OV_STRING  value
) {
    return ov_string_setvalue(&pobj->v_PVSLSubModelIdString,value);
}

OV_DLLFNCEXPORT OV_RESULT openaas_modelmanager_PVSLSubModelIdType_set(
    OV_INSTPTR_openaas_modelmanager          pobj,
    const OV_UINT  value
) {
    pobj->v_PVSLSubModelIdType = value;
    return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_RESULT openaas_modelmanager_PVSLName_set(
    OV_INSTPTR_openaas_modelmanager          pobj,
    const OV_STRING  value
) {
    return ov_string_setvalue(&pobj->v_PVSLName,value);
}

OV_DLLFNCEXPORT OV_RESULT openaas_modelmanager_PVSLCarrierIdString_set(
    OV_INSTPTR_openaas_modelmanager          pobj,
    const OV_STRING  value
) {
    return ov_string_setvalue(&pobj->v_PVSLCarrierIdString,value);
}

OV_DLLFNCEXPORT OV_RESULT openaas_modelmanager_PVSLCarrierIdType_set(
    OV_INSTPTR_openaas_modelmanager          pobj,
    const OV_UINT  value
) {
    pobj->v_PVSLCarrierIdType = value;
    return OV_ERR_OK;
}


OV_DLLFNCEXPORT OV_RESULT openaas_modelmanager_PVSLCreatingInstanceIdString_set(
    OV_INSTPTR_openaas_modelmanager          pobj,
    const OV_STRING  value
) {
    return ov_string_setvalue(&pobj->v_PVSLCreatingInstanceIdString,value);
}

OV_DLLFNCEXPORT OV_RESULT openaas_modelmanager_PVSLCreatingInstanceIdType_set(
    OV_INSTPTR_openaas_modelmanager          pobj,
    const OV_UINT  value
) {
    pobj->v_PVSLCreatingInstanceIdType = value;
    return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_RESULT openaas_modelmanager_PVSLCreationTime_set(
    OV_INSTPTR_openaas_modelmanager          pobj,
    const OV_TIME*  value
) {
    pobj->v_PVSLCreationTime = *value;
    return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_RESULT openaas_modelmanager_PVSLCreate_set(
    OV_INSTPTR_openaas_modelmanager          pobj,
    const OV_BOOL  value
) {
	AASStatusCode result = AASSTATUSCODE_GOOD;
	pobj->v_PVSLCreate = value;
	if (pobj->v_PVSLCreate == TRUE){
		IdentificationType tmpAASId;
		tmpAASId.IdSpec = pobj->v_PVSLAASIdString;
		tmpAASId.IdType = pobj->v_PVSLAASIdType;

		IdentificationType tmpSubModelId;
		tmpSubModelId.IdSpec = pobj->v_PVSLSubModelIdString;
		tmpSubModelId.IdType = pobj->v_PVSSubModelIdType;

		IdentificationType tmpCarrier;
		tmpCarrier.IdSpec = pobj->v_PVSLCarrierIdString;
		tmpCarrier.IdType = pobj->v_PVSLCarrierIdType;

		IdentificationType tmpCreatingInstance;
		tmpCreatingInstance.IdSpec = pobj->v_PVSLCreatingInstanceIdString;
		tmpCreatingInstance.IdType = pobj->v_PVSLCreatingInstanceIdType;

		result = openaas_modelmanager_createPVSL(tmpAASId, tmpSubModelId, pobj->v_PVSLName, tmpCarrier, tmpCreatingInstance);
	}
	pobj->v_PVSLCreate = FALSE;
	pobj->v_PVSLStatus = result;
	return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_RESULT openaas_modelmanager_PVSLDelete_set(
    OV_INSTPTR_openaas_modelmanager          pobj,
    const OV_BOOL  value
) {
	AASStatusCode result = AASSTATUSCODE_GOOD;
	pobj->v_PVSLDelete = value;
	if (pobj->v_PVSLDelete == TRUE){
		IdentificationType tmpAASId;
		tmpAASId.IdSpec = pobj->v_PVSLAASIdString;
		tmpAASId.IdType = pobj->v_PVSLAASIdType;

		IdentificationType tmpSubModelId;
		tmpSubModelId.IdSpec = pobj->v_PVSLSubModelIdString;
		tmpSubModelId.IdType = pobj->v_PVSSubModelIdType;

		result = openaas_modelmanager_deletePVSL(tmpAASId, tmpSubModelId, pobj->v_PVSLName);
	}
	pobj->v_PVSLDelete = FALSE;
	pobj->v_PVSLStatus = result;
	return OV_ERR_OK;
}

OV_DLLFNCEXPORT AASStatusCode openaas_modelmanager_createPVSL(IdentificationType aasId, IdentificationType subModelId, OV_STRING pvslName, IdentificationType carrier, IdentificationType creatingInstance) {
	OV_RESULT result = OV_ERR_OK;
	OV_INSTPTR_openaas_aas paas = NULL;
	OV_INSTPTR_ov_object ptr = NULL;
	OV_INSTPTR_openaas_SubModel psm = NULL;
	OV_INSTPTR_propertyValueStatement_PropertyValueStatementList ppvsl = NULL;
	OV_INSTPTR_ov_domain pparrent = NULL;
	ptr = ov_path_getobjectpointer(openaas_modelmanager_AASConvertListGet(aasId), 2);
	if (!ptr)
		return AASSTATUSCODE_BADAASID;
	paas = Ov_StaticPtrCast(openaas_aas, ptr);
	if (paas){
		if (!subModelId.IdSpec){
			ppvsl = Ov_StaticPtrCast(propertyValueStatement_PropertyValueStatementList, Ov_SearchChild(ov_containment, Ov_StaticPtrCast(ov_domain, &paas->p_Body), pvslName));
			pparrent = Ov_StaticPtrCast(ov_domain, &paas->p_Body);
		}else{
			Ov_ForEachChild(ov_containment, Ov_StaticPtrCast(ov_domain, &paas->p_Body), ptr){
				if (ptr && Ov_CanCastTo(openaas_SubModel,ptr)){
					psm = Ov_StaticPtrCast(openaas_SubModel, ptr);
					if (ov_string_compare(subModelId.IdSpec, psm->v_IdString) == OV_STRCMP_EQUAL && subModelId.IdType == psm->v_IdType){
						break;
					}
				}
			}
			if (!ptr){
				return AASSTATUSCODE_BADSMID;
			}
			ppvsl = Ov_StaticPtrCast(propertyValueStatement_PropertyValueStatementList, Ov_SearchChild(ov_containment, Ov_StaticPtrCast(ov_domain, psm), pvslName));
			pparrent = Ov_StaticPtrCast(ov_domain, psm);
		}
		if(!ppvsl){
			result = Ov_CreateObject(propertyValueStatement_PropertyValueStatementList, ppvsl, pparrent, pvslName);
			if(Ov_Fail(result)){
				ov_logfile_error("Fatal: could not create PVSL object - reason: %s", ov_result_getresulttext(result));
				return openaas_modelmanager_ovresultToAASStatusCode(result);
			}
			ov_string_setvalue(&(ppvsl->v_CarrierIdString), carrier.IdSpec);
			ppvsl->v_CarrierIdType = carrier.IdType;
			ov_string_setvalue(&(ppvsl->v_CreatingInstanceIdString), creatingInstance.IdSpec);
			ppvsl->v_CreatingInstanceIdType = creatingInstance.IdType;
			ov_time_gettime(&ppvsl->v_CreationTime);
		}else{
			return AASSTATUSCODE_BADPVSLNAME;
		}
	}else{
		return AASSTATUSCODE_BADUNEXPECTEDERROR;
	}
	return AASSTATUSCODE_GOOD;
}

OV_DLLFNCEXPORT AASStatusCode openaas_modelmanager_createPVSLTime(IdentificationType aasId, IdentificationType subModelId, OV_STRING pvslName, IdentificationType carrier, IdentificationType creatingInstance, OV_TIME creatingTime) {
	OV_RESULT result = OV_ERR_OK;
	OV_INSTPTR_openaas_aas paas = NULL;
	OV_INSTPTR_ov_object ptr = NULL;
	OV_INSTPTR_openaas_SubModel psm = NULL;
	OV_INSTPTR_propertyValueStatement_PropertyValueStatementList ppvsl = NULL;
	OV_INSTPTR_ov_domain pparrent = NULL;
	ptr = ov_path_getobjectpointer(openaas_modelmanager_AASConvertListGet(aasId), 2);
	if (!ptr)
		return AASSTATUSCODE_BADAASID;
	paas = Ov_StaticPtrCast(openaas_aas, ptr);
	if (paas){
		if (!subModelId.IdSpec){
			ppvsl = Ov_StaticPtrCast(propertyValueStatement_PropertyValueStatementList, Ov_SearchChild(ov_containment, Ov_StaticPtrCast(ov_domain, &paas->p_Body), pvslName));
			pparrent = Ov_StaticPtrCast(ov_domain, &paas->p_Body);
		}else{
			Ov_ForEachChild(ov_containment, Ov_StaticPtrCast(ov_domain, &paas->p_Body), ptr){
				if (ptr && Ov_CanCastTo(openaas_SubModel,ptr)){
					psm = Ov_StaticPtrCast(openaas_SubModel, ptr);
					if (ov_string_compare(subModelId.IdSpec, psm->v_IdString) == OV_STRCMP_EQUAL && subModelId.IdType == psm->v_IdType){
						break;
					}
				}
			}
			if (!ptr){
				return AASSTATUSCODE_BADSMID;
			}
			ppvsl = Ov_StaticPtrCast(propertyValueStatement_PropertyValueStatementList, Ov_SearchChild(ov_containment, Ov_StaticPtrCast(ov_domain, psm), pvslName));
			pparrent = Ov_StaticPtrCast(ov_domain, psm);
		}
		if(!ppvsl){
			result = Ov_CreateObject(propertyValueStatement_PropertyValueStatementList, ppvsl, pparrent, pvslName);
			if(Ov_Fail(result)){
				ov_logfile_error("Fatal: could not create PVSL object - reason: %s", ov_result_getresulttext(result));
				return openaas_modelmanager_ovresultToAASStatusCode(result);
			}
			ov_string_setvalue(&(ppvsl->v_CarrierIdString), carrier.IdSpec);
			ppvsl->v_CarrierIdType = carrier.IdType;
			ov_string_setvalue(&(ppvsl->v_CreatingInstanceIdString), creatingInstance.IdSpec);
			ppvsl->v_CreatingInstanceIdType = creatingInstance.IdType;
			ppvsl->v_CreationTime = creatingTime;
		}else{
			return AASSTATUSCODE_BADPVSLNAME;
		}
	}else{
		return AASSTATUSCODE_BADUNEXPECTEDERROR;
	}
	return AASSTATUSCODE_GOOD;
}


OV_DLLFNCEXPORT AASStatusCode openaas_modelmanager_deletePVSL(IdentificationType aasId, IdentificationType subModelId, OV_STRING pvslName){
	OV_RESULT result = OV_ERR_OK;
	OV_INSTPTR_openaas_aas paas = NULL;
	OV_INSTPTR_ov_object ptr = NULL;
	OV_INSTPTR_openaas_SubModel psm = NULL;
	OV_INSTPTR_propertyValueStatement_PropertyValueStatementList ppvsl = NULL;
	ptr = ov_path_getobjectpointer(openaas_modelmanager_AASConvertListGet(aasId), 2);
	if (!ptr)
		return AASSTATUSCODE_BADAASID;
	paas = Ov_StaticPtrCast(openaas_aas, ptr);
	if (paas){
		if (!subModelId.IdSpec){
			ppvsl = Ov_StaticPtrCast(propertyValueStatement_PropertyValueStatementList, Ov_SearchChild(ov_containment, Ov_StaticPtrCast(ov_domain, &paas->p_Body), pvslName));
		}else{
			Ov_ForEachChild(ov_containment, Ov_StaticPtrCast(ov_domain, &paas->p_Body), ptr){
				if (ptr && Ov_CanCastTo(openaas_SubModel,ptr)){
					psm = Ov_StaticPtrCast(openaas_SubModel, ptr);
					if (ov_string_compare(subModelId.IdSpec, psm->v_IdString) == OV_STRCMP_EQUAL && subModelId.IdType == psm->v_IdType){
						break;
					}
				}
			}
			if (!ptr){
				return AASSTATUSCODE_BADSMID;
			}
			ppvsl = Ov_StaticPtrCast(propertyValueStatement_PropertyValueStatementList, Ov_SearchChild(ov_containment, Ov_StaticPtrCast(ov_domain, psm), pvslName));
		}
		if(ppvsl){
			result = Ov_DeleteObject(ppvsl);
			if(Ov_Fail(result)){
				ov_logfile_error("Fatal: could not delete PVSL object - reason: %s", ov_result_getresulttext(result));
				return openaas_modelmanager_ovresultToAASStatusCode(result);
			}
		}else{
			return AASSTATUSCODE_BADPVSLNAME;
		}
	}else{
		return AASSTATUSCODE_BADUNEXPECTEDERROR;
	}
	return AASSTATUSCODE_GOOD;
}




*/
