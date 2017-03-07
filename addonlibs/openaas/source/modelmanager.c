
/******************************************************************************
*
*   FILE
*   ----
*   modelmanager.c
*
*   History
*   -------
*   2016-12-22   File created
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
#include "libov/ov_macros.h"
#include "openaas_helpers.h"

extern OV_INSTPTR_openaas_nodeStoreFunctions pNodeStoreFunctions;

void openaas_modelmanager_AASConvertListAdd(IdentificationType aasId, OV_STRING aasName){
	OV_INSTPTR_openaas_modelmanager pmodelmanager = NULL;
	OV_INSTPTR_openaas_AASConvertListType pListElement = NULL;

	pmodelmanager = Ov_StaticPtrCast(openaas_modelmanager, Ov_GetFirstChild(ov_instantiation, pclass_openaas_modelmanager));

	Ov_ForEachChildEx(ov_containment, Ov_StaticPtrCast(ov_domain, pmodelmanager), pListElement, openaas_AASConvertListType){
		if (ov_string_compare(pListElement->v_AASIdString, aasId.IdSpec) == OV_STRCMP_EQUAL && pListElement->v_AASIdType == aasId.IdType)
			return;
	}

	OV_STRING tmpString = NULL;
	ov_string_print(&tmpString, "%i", pmodelmanager->v_ListCount+1);
	if (Ov_Fail(Ov_CreateObject(openaas_AASConvertListType, pListElement, Ov_StaticPtrCast(ov_domain, pmodelmanager), tmpString))){
		ov_database_free(tmpString);
		return;
	}
	ov_database_free(tmpString);
	pmodelmanager->v_ListSize += 1;
	pmodelmanager->v_ListCount += 1;
	ov_string_setvalue(&pListElement->v_AASIdString, aasId.IdSpec);
	pListElement->v_AASIdType = aasId.IdType;
	tmpString = NULL;
	ov_string_setvalue(&tmpString, "/TechUnits/openAAS/AASFolder/");
	ov_string_append(&tmpString, aasName);
	ov_string_setvalue(&pListElement->v_AASPath, tmpString);
	ov_database_free(tmpString);

	// Get the pointer to object for send the Message
	OV_INSTPTR_ksapi_setVar psendAASMessage = NULL;
	Ov_ForEachChildEx(ov_instantiation, pclass_ksapi_setVar, psendAASMessage, ksapi_setVar){
		if(ov_string_compare(psendAASMessage->v_identifier, "SendAASMessage") == OV_STRCMP_EQUAL){
			break;
		}
	}

	ov_string_setvalue(&psendAASMessage->v_serverHost, pNodeStoreFunctions->v_IPAddressAASDiscoveryServer);
	ov_string_setvalue(&psendAASMessage->v_serverName, pNodeStoreFunctions->v_ManagerNameAASDiscoveryServer);
	ov_string_setvalue(&psendAASMessage->v_path, pNodeStoreFunctions->v_PathToAASDiscoveryServer);
	ov_string_append(&psendAASMessage->v_path, ".AddOVDataForAAS");

	OV_STRING tmpHexString = NULL;
	ov_string_print(&tmpHexString, "%x", aasId.IdType);

	for (OV_UINT i = 0; i < ov_string_getlength(aasId.IdSpec); i++){
		OV_STRING tmpHexString2 = NULL;
		ov_string_print(&tmpHexString2, "%x", aasId.IdSpec[i]);
		ov_string_append(&tmpHexString, tmpHexString2);
		ov_database_free(tmpHexString2);
	}

	Ov_SetAnyValue(&psendAASMessage->v_varValue, NULL);
	psendAASMessage->v_varValue.value.vartype = OV_VT_STRING_VEC;
	psendAASMessage->v_varValue.value.valueunion.val_string_vec.veclen = 0;
	psendAASMessage->v_varValue.value.valueunion.val_string_vec.value = NULL;
	Ov_SetDynamicVectorLength(&psendAASMessage->v_varValue.value.valueunion.val_string_vec, 4, STRING);

	psendAASMessage->v_varValue.value.valueunion.val_string_vec.value[0] = NULL;
	ov_string_setvalue(&psendAASMessage->v_varValue.value.valueunion.val_string_vec.value[0], tmpHexString);
	psendAASMessage->v_varValue.value.valueunion.val_string_vec.value[1] = NULL;
	ov_string_setvalue(&psendAASMessage->v_varValue.value.valueunion.val_string_vec.value[1], pNodeStoreFunctions->v_IPAddressServer);
	OV_ANY tmpServername;
	ov_vendortree_getservername(&tmpServername, NULL);
	psendAASMessage->v_varValue.value.valueunion.val_string_vec.value[2] = NULL;
	ov_string_setvalue(&psendAASMessage->v_varValue.value.valueunion.val_string_vec.value[2], tmpServername.value.valueunion.val_string);
	psendAASMessage->v_varValue.value.valueunion.val_string_vec.value[3] = NULL;
	ov_string_setvalue(&psendAASMessage->v_varValue.value.valueunion.val_string_vec.value[3], "/TechUnits/openAAS/AASFolder/ComCo");
	ov_database_free(tmpHexString);

	OV_INSTPTR_ksapi_KSApiCommon pKSApiCommon = Ov_StaticPtrCast(ksapi_KSApiCommon, psendAASMessage);
	ksapi_KSApiCommon_Reset_set(pKSApiCommon, FALSE);
	ksapi_KSApiCommon_Reset_set(pKSApiCommon, TRUE);
	ksapi_KSApiCommon_Submit_set(pKSApiCommon, FALSE);
	ksapi_KSApiCommon_Submit_set(pKSApiCommon, TRUE);
	return;
}


void openaas_modelmanager_AASConvertListDelete(IdentificationType aasId){
	OV_INSTPTR_openaas_modelmanager pmodelmanager = NULL;
	OV_INSTPTR_openaas_AASConvertListType pListElement = NULL;

	pmodelmanager = Ov_StaticPtrCast(openaas_modelmanager, Ov_GetFirstChild(ov_instantiation, pclass_openaas_modelmanager));

	Ov_ForEachChildEx(ov_containment, Ov_StaticPtrCast(ov_domain, pmodelmanager), pListElement, openaas_AASConvertListType){
		if (ov_string_compare(pListElement->v_AASIdString, aasId.IdSpec) == OV_STRCMP_EQUAL && pListElement->v_AASIdType == aasId.IdType){
			if (Ov_Fail(Ov_DeleteObject(pListElement)))
				return;

			// Get the pointer to object for send the Message
			OV_INSTPTR_ksapi_setVar psendAASMessage = NULL;
			Ov_ForEachChildEx(ov_instantiation, pclass_ksapi_setVar, psendAASMessage, ksapi_setVar){
				if(ov_string_compare(psendAASMessage->v_identifier, "SendAASMessage") == OV_STRCMP_EQUAL){
					break;
				}
			}

			ov_string_setvalue(&psendAASMessage->v_serverHost, pNodeStoreFunctions->v_IPAddressAASDiscoveryServer);
			ov_string_setvalue(&psendAASMessage->v_serverName, pNodeStoreFunctions->v_ManagerNameAASDiscoveryServer);
			ov_string_setvalue(&psendAASMessage->v_path, pNodeStoreFunctions->v_PathToAASDiscoveryServer);
			ov_string_append(&psendAASMessage->v_path, ".DeleteOVDataForAAS");

			OV_STRING tmpHexString = NULL;
			ov_string_print(&tmpHexString, "%x", aasId.IdType);

			for (OV_UINT i = 0; i < ov_string_getlength(aasId.IdSpec); i++){
				OV_STRING tmpHexString2 = NULL;
				ov_string_print(&tmpHexString2, "%x", aasId.IdSpec[i]);
				ov_string_append(&tmpHexString, tmpHexString2);
				ov_database_free(tmpHexString2);
			}
			Ov_SetAnyValue(&psendAASMessage->v_varValue, NULL);
			ov_string_setvalue(&psendAASMessage->v_varValue.value.valueunion.val_string, tmpHexString);
			ov_database_free(tmpHexString);

			OV_INSTPTR_ksapi_KSApiCommon pKSApiCommon = Ov_StaticPtrCast(ksapi_KSApiCommon, psendAASMessage);
			ksapi_KSApiCommon_Submit_set(pKSApiCommon, FALSE);
			ksapi_KSApiCommon_Submit_set(pKSApiCommon, TRUE);
			pmodelmanager->v_ListSize -= 1;
			return;
		}
	}
	return;
}

OV_STRING openaas_modelmanager_AASConvertListGet(IdentificationType aasId){
	OV_INSTPTR_openaas_modelmanager pmodelmanager = NULL;
	OV_INSTPTR_openaas_AASConvertListType pListElement = NULL;
	pmodelmanager = Ov_StaticPtrCast(openaas_modelmanager, Ov_GetFirstChild(ov_instantiation, pclass_openaas_modelmanager));

	if (pmodelmanager->v_ListSize == 0){
		return "";
	}

	Ov_ForEachChildEx(ov_containment, Ov_StaticPtrCast(ov_domain, pmodelmanager), pListElement, openaas_AASConvertListType){
		if (ov_string_compare(pListElement->v_AASIdString, aasId.IdSpec) == OV_STRCMP_EQUAL && pListElement->v_AASIdType == aasId.IdType){
			return pListElement->v_AASPath;
		}
	}
	return "";
}




OV_DLLFNCEXPORT UA_NodeId openaas_modelmanager_getAASNodeId(IdentificationType aasId) {
    return UA_NODEID_STRING_ALLOC(pNodeStoreFunctions->v_interfacenamespace.index, openaas_modelmanager_AASConvertListGet(aasId));;
}


OV_DLLFNCEXPORT AASStatusCode openaas_modelmanager_ovresultToAASStatusCode(OV_RESULT result) {
	switch(result){
	case OV_ERR_OK:
		return AASSTATUSCODE_GOOD;
	default:
		return AASSTATUSCODE_BADUNEXPECTEDERROR;
	}
}




OV_DLLFNCEXPORT OV_ACCESS openaas_modelmanager_getaccess(
	OV_INSTPTR_ov_object	pobj,
	const OV_ELEMENT		*pelem,
	const OV_TICKET			*pticket
) {
    /*
    *   local variables
    */
    return (OV_ACCESS)(OV_AC_WRITE | OV_AC_READ | OV_AC_LINKABLE | OV_AC_UNLINKABLE | OV_AC_DELETEABLE | OV_AC_RENAMEABLE);
}


OV_DLLFNCEXPORT OV_RESULT openaas_modelmanager_constructor(
	OV_INSTPTR_ov_object 	pobj
) {
    /*
    *   local variables
    */
    OV_INSTPTR_openaas_modelmanager pinst = Ov_StaticPtrCast(openaas_modelmanager, pobj);
    OV_RESULT    result;
    OV_INSTPTR_ov_object pOtherObject = NULL;
    OV_INSTPTR_ov_domain pcommunication = NULL;
	OV_INSTPTR_ov_domain pDomOpenAAS = NULL;
	OV_INSTPTR_openaas_aas pComCo = NULL;
	OV_INSTPTR_openaas_ExternalPostOffice pExternalPost = NULL;
	OV_INSTPTR_ksapi_setVar psendAASMessage = NULL;
	OV_INSTPTR_ksapi_getVar pGetComCoAddressFromAASDiscoveryServer = NULL;
	OV_INSTPTR_ov_library pLibKSAPI = NULL;
	OV_INSTPTR_fb_task	purtask = NULL;
    /* do what the base class does first */
    result = ov_object_constructor(pobj);
    if(Ov_Fail(result))
         return result;

    /* do what */
    Ov_ForEachChild(ov_instantiation, pclass_openaas_modelmanager, pOtherObject){
		if(pOtherObject != pobj){
			ov_logfile_error("%s: cannot instantiate - modelmanager instance already exists", pinst->v_identifier);
			return OV_ERR_ALREADYEXISTS;
		}
	}

    Ov_ForEachChildEx(ov_instantiation, pclass_ov_library, pLibKSAPI, ov_library){
		if(ov_string_compare(pLibKSAPI->v_identifier, "ksapi") == OV_STRCMP_EQUAL){
			break;
		}
	 }
	 if(!pLibKSAPI){
		result = Ov_CreateObject(ov_library, pLibKSAPI, &(pdb->acplt), "ksapi");
		if(Ov_Fail(result)){
			ov_memstack_lock();
			ov_logfile_error("openaas: Fatal: Couldn't load dependency Library ksapi Reason: %s", ov_result_getresulttext(result));
			ov_memstack_unlock();
			return result;
		}
	 }

	pcommunication = Ov_StaticPtrCast(ov_domain, Ov_SearchChild(ov_containment, &(pdb->root), "communication"));
	if(!pcommunication) {
		result = Ov_CreateObject(ov_domain, pcommunication, &(pdb->root), "communication");
		if(Ov_Fail(result)) {
			ov_logfile_error("Fatal: Could not create Object 'communication'");
			return result;
		}
	}
	else if(!Ov_CanCastTo(ov_domain, (OV_INSTPTR_ov_object) pcommunication)){
		ov_logfile_error("Fatal: communication object found but not domain (or derived)");
		return OV_ERR_GENERIC;
	}

	pDomOpenAAS = Ov_StaticPtrCast(ov_domain, Ov_SearchChild(ov_containment, pcommunication, "OpenAAS"));
	if(!pDomOpenAAS) {
		result = Ov_CreateObject(ov_domain, pDomOpenAAS, pcommunication, "OpenAAS");
		if(Ov_Fail(result)){
			ov_logfile_error("Fatal: could not create OpenAAS domain");
			return result;
		}
	}
	else if(!Ov_CanCastTo(ov_domain, (OV_INSTPTR_ov_object) pDomOpenAAS)){
		ov_logfile_error("Fatal: OpenAAS object found but not domain (or derived)");
		return OV_ERR_GENERIC;
	}

	// create sendAASMessage
	Ov_ForEachChildEx(ov_instantiation, pclass_ksapi_setVar, psendAASMessage, ksapi_setVar){
		if(ov_string_compare(psendAASMessage->v_identifier, "SendAASMessage") == OV_STRCMP_EQUAL){
			break;
		}
	}
	if(!psendAASMessage){
		result = Ov_CreateObject(ksapi_setVar, psendAASMessage, pDomOpenAAS, "SendAASMessage");
		if(Ov_Fail(result)){
			ov_logfile_error("Fatal: could not create SendAASMessage object - reason: %s", ov_result_getresulttext(result));
			return result;
		}
	}

	// create getComCoAddressFromAASDiscoveryServer
	Ov_ForEachChildEx(ov_instantiation, pclass_ksapi_getVar, pGetComCoAddressFromAASDiscoveryServer, ksapi_getVar){
		if(ov_string_compare(pGetComCoAddressFromAASDiscoveryServer->v_identifier, "GetComCoAddressFromAASDiscoveryServer") == OV_STRCMP_EQUAL){
			break;
		}
	}
	if(!pGetComCoAddressFromAASDiscoveryServer){
		result = Ov_CreateObject(ksapi_getVar, pGetComCoAddressFromAASDiscoveryServer, pDomOpenAAS, "GetComCoAddressFromAASDiscoveryServer");
		if(Ov_Fail(result)){
			ov_logfile_error("Fatal: could not create GetComCoAddressFromAASDiscoveryServer object - reason: %s", ov_result_getresulttext(result));
			return result;
		}
	}

	// Create Folder for AAS
	OV_INSTPTR_ov_domain pTechUnits = NULL;
	pTechUnits = Ov_StaticPtrCast(ov_domain, Ov_SearchChild(ov_containment, &(pdb->root), "TechUnits"));
	if(!pTechUnits) {
		result = Ov_CreateObject(ov_domain, pTechUnits, &(pdb->root), "TechUnits");
		if(Ov_Fail(result)) {
			ov_logfile_error("Fatal: Could not create Object 'pTechUnits'");
			return result;
		}
	}
	else if(!Ov_CanCastTo(ov_domain, (OV_INSTPTR_ov_object) pTechUnits))	{
		ov_logfile_error("Fatal: pTechUnits object found but not domain (or derived)");
		return OV_ERR_GENERIC;
	}

	OV_INSTPTR_ov_domain popenAASFolder = NULL;
	popenAASFolder = Ov_StaticPtrCast(ov_domain, Ov_SearchChild(ov_containment, pTechUnits, "openAAS"));
	if(!popenAASFolder) {
		result = Ov_CreateObject(ov_domain, popenAASFolder, pTechUnits, "openAAS");
		if(Ov_Fail(result))	{
			ov_logfile_error("Fatal: could not create openAAS domain");
			return result;
		}
	}
	else if(!Ov_CanCastTo(ov_domain, (OV_INSTPTR_ov_object) popenAASFolder)){
		ov_logfile_error("Fatal: openAAS object found but not domain (or derived)");
		return OV_ERR_GENERIC;
	}

	OV_INSTPTR_ov_domain pAASFolder = NULL;
	pAASFolder = Ov_StaticPtrCast(ov_domain, Ov_SearchChild(ov_containment, popenAASFolder, "AASFolder"));
	if(!pAASFolder) {
		result = Ov_CreateObject(ov_domain, pAASFolder, popenAASFolder, "AASFolder");
		if(Ov_Fail(result))	{
			ov_logfile_error("Fatal: could not create AASFolder domain");
			return result;
		}
	}
	else if(!Ov_CanCastTo(ov_domain, (OV_INSTPTR_ov_object) pAASFolder)){
		ov_logfile_error("Fatal: AASFolder object found but not domain (or derived)");
		return OV_ERR_GENERIC;
	}

	// create ComCo
	Ov_ForEachChildEx(ov_instantiation, pclass_openaas_aas, pComCo, openaas_aas){
		if(ov_string_compare(pComCo->v_identifier, "ComCo") == OV_STRCMP_EQUAL){
			break;
		}
	}
	if(!pComCo){
		IdentificationType aasId;
		aasId.IdSpec = NULL;
		ov_string_setvalue(&aasId.IdSpec, "http://acplt.org/ComCo");
		aasId.IdType = 0;

		IdentificationType assetId;
		assetId.IdSpec = NULL;
		ov_string_setvalue(&assetId.IdSpec, "http://acplt.org/Manager");
		assetId.IdType = 0;

		if (AASSTATUSCODE_FAIL(openaas_modelmanager_createAAS(aasId, "ComCo", assetId))){
			ov_logfile_error("Fatal: Could not create Object 'ComCo'");
			return OV_ERR_GENERIC;
		}
	}

	// create ExternalPostOffice
	Ov_ForEachChildEx(ov_instantiation, pclass_openaas_ExternalPostOffice, pExternalPost, openaas_ExternalPostOffice){
		if(ov_string_compare(pExternalPost->v_identifier, "ExternalPostOffice") == OV_STRCMP_EQUAL){
			break;
		}
	}
	if(!pExternalPost){
		result = Ov_CreateObject(openaas_ExternalPostOffice, pExternalPost, pAASFolder, "ExternalPostOffice");
		if(Ov_Fail(result)){
			ov_logfile_error("Fatal: could not create externalPostOffice object - reason: %s", ov_result_getresulttext(result));
			return result;
		}
		purtask = (OV_INSTPTR_fb_task)ov_path_getobjectpointer("/Tasks/UrTask", 2);
		result = Ov_Link(fb_tasklist, purtask, pExternalPost);
		if (Ov_Fail(result)) {
			ov_logfile_error("Fatal: could not link externalPostOffice object - reason: %s", ov_result_getresulttext(result));
			return result;
		}
	}
    return OV_ERR_OK;
}

OV_DLLFNCEXPORT void openaas_modelmanager_destructor(
	OV_INSTPTR_ov_object 	pobj
) {
    /*
    *   local variables
    */
    //OV_INSTPTR_openaas_modelmanager pinst = Ov_StaticPtrCast(openaas_modelmanager, pobj);

    /* do what */

    /* destroy object */
    ov_object_destructor(pobj);

    return;
}



OV_DLLFNCEXPORT void openaas_modelmanager_startup(
	OV_INSTPTR_ov_object 	pobj
) {
    /*
    *   local variables
    */
    //OV_INSTPTR_openaas_modelmanager pinst = Ov_StaticPtrCast(openaas_modelmanager, pobj);

    /* do what the base class does first */
    ov_object_startup(pobj);

    /* do what */

    return;
}
