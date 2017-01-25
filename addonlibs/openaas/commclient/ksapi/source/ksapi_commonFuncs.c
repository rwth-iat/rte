/*
 * ksapi_commonFuncs.c
 *
 *  Created on: 14.05.2013
 *      Author: lars
 */


#ifndef OV_COMPILE_LIBRARY_ksapi
#define OV_COMPILE_LIBRARY_ksapi
#endif


#include "ksapi.h"
#include "libov/ov_macros.h"
#include "libov/ov_vendortree.h"
#include "libov/ov_class.h"

OV_RESULT ksapi_getClientPointers(OV_INSTPTR_ksapi_KSApiCommon pCommon,
				OV_INSTPTR_ksbase_ClientBase* pClient, OV_VTBLPTR_ksbase_ClientBase* pVtblClient)
{
	OV_RESULT result;
	OV_STRING tempstr = NULL;

	if(!pClient || !pVtblClient)
		return OV_ERR_BADPARAM;

	*pClient = NULL;
	*pVtblClient = NULL;

	Ov_ForEachChildEx(ov_containment, pCommon, *pClient, ksbase_ClientBase)
	{
		break;
	}
	if(!(*pClient))
	{/*	no client found --> create one	*/
		const char defaultClient [] = "xdrClient";
		OV_INSTPTR_ov_class pClassClient = NULL;
		ov_memstack_lock();
		tempstr = ov_vendortree_getcmdlineoption_value("KS_USECLIENT");
		if(!tempstr)
			tempstr = ov_vendortree_getcmdlineoption_value("KSAPI_USECLIENT");

		if(!tempstr)
			tempstr = (OV_STRING) defaultClient;
		else
			if(!(*tempstr))
				return OV_ERR_OK;

		Ov_ForEachChild(ov_inheritance, pclass_ksbase_ClientBase, pClassClient)
		{
			if(ov_string_compare(pClassClient->v_identifier, tempstr) == OV_STRCMP_EQUAL)
			{
				result = ov_class_createobject(pClassClient,Ov_StaticPtrCast(ov_domain, pCommon), "Client",
						OV_PMH_DEFAULT, NULL, NULL, NULL, (OV_INSTPTR_ov_object*) pClient);
				if(Ov_Fail(result))
				{
					ov_memstack_unlock();
					return result;
				}
				break;
			}
		}
		ov_memstack_unlock();
	}

	/*	here we definitely have a client object	*/
	Ov_GetVTablePtr(ksbase_ClientBase, *pVtblClient, *pClient);
	if(!pVtblClient)
		return OV_ERR_BADOBJTYPE;

	return OV_ERR_OK;
}

