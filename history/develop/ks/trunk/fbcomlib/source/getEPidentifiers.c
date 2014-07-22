
/******************************************************************************
 *
 *   FILE
 *   ----
 *   getEPidentifiers.c
 *
 *   History
 *   -------
 *   2013-05-18   File created
 *
 *******************************************************************************
 *
 *   This file is generated by the 'acplt_builder' command
 *
 ******************************************************************************/


#ifndef OV_COMPILE_LIBRARY_fbcomlib
#define OV_COMPILE_LIBRARY_fbcomlib
#endif


#include "fbcomlib.h"
#include "libov/ov_macros.h"
#include "config.h"
#include "ksapi_commonFuncs.h"


OV_DLLFNCEXPORT OV_BOOL fbcomlib_getEPidentifiers_doReset_get(
		OV_INSTPTR_fbcomlib_getEPidentifiers          pobj
) {
	return pobj->v_doReset;
}

OV_DLLFNCEXPORT OV_RESULT fbcomlib_getEPidentifiers_doReset_set(
		OV_INSTPTR_fbcomlib_getEPidentifiers          pobj,
		const OV_BOOL  value
) {
	if(value)
	{
		if(pobj->p_apiGetEPid.v_Reset)
			ksapi_KSApiCommon_Reset_set((OV_INSTPTR_ksapi_KSApiCommon) &(pobj->p_apiGetEPid), FALSE);
		ksapi_KSApiCommon_Reset_set((OV_INSTPTR_ksapi_KSApiCommon) &(pobj->p_apiGetEPid), TRUE);
		fbcomlib_FBComCommon_resetAbstract(Ov_StaticPtrCast(fbcomlib_FBComCommon, pobj));
	}

	return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_RESULT fbcomlib_getEPidentifiers_constructor(
	OV_INSTPTR_ov_object 	pobj
) {
    /*
    *   local variables
    */
    OV_INSTPTR_fbcomlib_getEPidentifiers pinst = Ov_StaticPtrCast(fbcomlib_getEPidentifiers, pobj);
    OV_RESULT    result;

    /* do what the base class does first */
    result = fbcomlib_FBComCommon_constructor(pobj);
    if(Ov_Fail(result))
         return result;

    /* do what */

    pinst->v_identifierList.veclen = 0;
    pinst->v_identifierList.value = NULL;


    return OV_ERR_OK;
}

OV_DLLFNCEXPORT void fbcomlib_getEPidentifiers_startup(
		OV_INSTPTR_ov_object 	pobj
) {
	/*
	 *   local variables
	 */

	/* do what the base class does first */
	fb_functionblock_startup(pobj);

	/* do what */


	return;
}

OV_DLLFNCEXPORT void fbcomlib_getEPidentifiers_typemethod(
		OV_INSTPTR_fb_functionblock	pfb,
		OV_TIME						*pltc
) {
	/*
	 *   local variables
	 */
	OV_INSTPTR_fbcomlib_getEPidentifiers pinst = Ov_StaticPtrCast(fbcomlib_getEPidentifiers, pfb);
	OV_VTBLPTR_ksapi_getEPidentifiers pVtblgetEPidentifiers = (OV_VTBLPTR_ksapi_getEPidentifiers) pclass_ksapi_getEPidentifiers->v_pvtable;
	OV_RESULT result;

	if(pinst->v_state == FBCOMLIB_STATE_WAITING)
	{	/*	waiting: calculate timeouts and check ksapi-object for answer	*/

		if(pinst->p_apiGetEPid.v_status == KSAPI_COMMON_REQUESTCOMPLETED)
		{	/*	ksapi-request completed --> get answer	*/
			pinst->v_opResult = pinst->p_apiGetEPid.v_result;
			result = Ov_SetDynamicVectorValue(&(pinst->v_identifierList), &(pinst->p_apiGetEPid.v_identifierList.value), pinst->p_apiGetEPid.v_identifierList.veclen, STRING);
			if(Ov_Fail(result))
			{
				fbcomlib_FBComCommon_state_set(Ov_StaticPtrCast(fbcomlib_FBComCommon, pinst), FBCOMLIB_STATE_INTERNAL_ERROR);
				pinst->v_errorCode = result;
				return;
			}
			fbcomlib_FBComCommon_state_set(Ov_StaticPtrCast(fbcomlib_FBComCommon, pinst), FBCOMLIB_STATE_OK);
			pinst->v_errorCode = OV_ERR_OK;
		}
		else if(pinst->p_apiGetEPid.v_status == KSAPI_COMMON_INTERNALERROR)
		{
			fbcomlib_FBComCommon_state_set(Ov_StaticPtrCast(fbcomlib_FBComCommon, pinst), FBCOMLIB_STATE_KSAPI_ERROR);
			pinst->v_errorCode = pinst->p_apiGetEPid.v_result;
			return;
		}
		else if(pinst->p_apiGetEPid.v_status == KSAPI_COMMON_EXTERNALERROR)
		{
			fbcomlib_FBComCommon_state_set(Ov_StaticPtrCast(fbcomlib_FBComCommon, pinst), FBCOMLIB_STATE_EXTERNAL_ERROR);
			pinst->v_errorCode = OV_ERR_GENERIC;
			return;
		}
		else if(pinst->p_apiGetEPid.v_status == KSAPI_COMMON_WAITINGFORANSWER)
		{
			if(pltc->secs > (pinst->v_requestSendTime.secs + pinst->v_timeout))
			{
				fbcomlib_FBComCommon_state_set(Ov_StaticPtrCast(fbcomlib_FBComCommon, pinst), FBCOMLIB_STATE_TIMEOUT);
				return;
			}
		}

	}

	if((pinst->v_state == FBCOMLIB_STATE_OK) || (pinst->v_state == FBCOMLIB_STATE_INIT))
	{	/*	ready to start	*/
		/*	ksapi-object found	*/
		if(pinst->v_sendRequested)
		{	/*	single send requested	*/
			result = ov_string_setvalue(&(pinst->p_apiGetEPid.v_serverHost), pinst->v_host);
			if(Ov_Fail(result))
			{
				pinst->v_errorCode = result;
				fbcomlib_FBComCommon_state_set(Ov_StaticPtrCast(fbcomlib_FBComCommon, pinst), FBCOMLIB_STATE_INTERNAL_ERROR);
				return;
			}
			result = ov_string_setvalue(&(pinst->p_apiGetEPid.v_serverName), pinst->v_server);
			if(Ov_Fail(result))
			{
				pinst->v_errorCode = result;
				fbcomlib_FBComCommon_state_set(Ov_StaticPtrCast(fbcomlib_FBComCommon, pinst), FBCOMLIB_STATE_INTERNAL_ERROR);
				return;
			}
			result = ov_string_setvalue(&(pinst->p_apiGetEPid.v_path), pinst->v_path);
			if(Ov_Fail(result))
			{
				pinst->v_errorCode = result;
				fbcomlib_FBComCommon_state_set(Ov_StaticPtrCast(fbcomlib_FBComCommon, pinst), FBCOMLIB_STATE_INTERNAL_ERROR);
				return;
			}
			result = ov_string_setvalue(&(pinst->p_apiGetEPid.v_namemask), pinst->v_namemask);
			if(Ov_Fail(result))
			{
				pinst->v_errorCode = result;
				fbcomlib_FBComCommon_state_set(Ov_StaticPtrCast(fbcomlib_FBComCommon, pinst), FBCOMLIB_STATE_INTERNAL_ERROR);
				return;
			}
		}
		if(pinst->v_sendRequested || pinst->v_doCyclic)
		{	/*	send Requested or cyclic sending requested	*/

			if(pinst->v_doCyclic)
				pinst->p_apiGetEPid.v_holdConnection = TRUE;
			else
				pinst->p_apiGetEPid.v_holdConnection = FALSE;
			pVtblgetEPidentifiers->m_submit(Ov_StaticPtrCast(ksapi_KSApiCommon, &(pinst->p_apiGetEPid)));
			fbcomlib_FBComCommon_state_set(Ov_StaticPtrCast(fbcomlib_FBComCommon, pinst), FBCOMLIB_STATE_WAITING);
			pinst->v_requestSendTime = *pltc;
			pinst->v_sendRequested = FALSE;
			return;

		}
		return;
	}
	else if(pinst->v_state != FBCOMLIB_STATE_WRONGINPUT)
	{
		if(pinst->v_doCyclic && pinst->v_retryAfter)
		{
			if(pltc->secs > (pinst->v_requestSendTime.secs + pinst->v_retryAfter))
			{
				if(pinst->v_doReset)
					fbcomlib_getEPidentifiers_doReset_set(pinst, FALSE);
				fbcomlib_getEPidentifiers_doReset_set(pinst, TRUE);
				fbcomlib_getEPidentifiers_doReset_set(pinst, FALSE);
				fbcomlib_FBComCommon_doCyclic_set(Ov_StaticPtrCast(fbcomlib_FBComCommon, pinst), TRUE);
			}

		}
	}
	return;
}

