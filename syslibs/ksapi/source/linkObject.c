
/******************************************************************************
*
*   FILE
*   ----
*   linkObject.c
*
*   History
*   -------
*   2013-05-14   File created
*
*******************************************************************************
*
*   This file is generated by the 'acplt_builder' command
*
******************************************************************************/


#ifndef OV_COMPILE_LIBRARY_ksapi
#define OV_COMPILE_LIBRARY_ksapi
#endif


#include "ksapi.h"
#include "ov_macros.h"
#include "ksapi_commonFuncs.h"
#include "ks_logfile.h"
#include "ksbase_helper.h"

void ksapi_linkObject_callback(const OV_INSTPTR_ov_domain this, const OV_INSTPTR_ov_domain that);



OV_DLLFNCEXPORT void ksapi_linkObject_startup(
	OV_INSTPTR_ov_object 	pobj
) {
    /*    
    *   local variables
    */

    /* do what the base class does first */
    ksapi_KSApiCommon_startup(pobj);

    /* do what */


    return;
}

OV_DLLFNCEXPORT void ksapi_linkObject_shutdown(
	OV_INSTPTR_ov_object 	pobj
) {
    /*    
    *   local variables
    */

    /* do what */

    /* set the object's state to "shut down" */
    ksapi_KSApiCommon_shutdown(pobj);

    return;
}

OV_DLLFNCEXPORT void ksapi_linkObject_submit(
	OV_INSTPTR_ksapi_KSApiCommon          pobj
) {
	OV_INSTPTR_ksapi_linkObject	pthis = Ov_StaticPtrCast(ksapi_linkObject, pobj);
	OV_INSTPTR_ksbase_ClientBase pClient = NULL;
	OV_VTBLPTR_ksbase_ClientBase pVtblClient = NULL;
	OV_LINK_ITEM item;
	OV_RESULT result;

	result = ksapi_KSApiCommon_prepareSubmit(pobj, &pClient, &pVtblClient);
	if(Ov_Fail(result))
		return;

	item.element_path = pthis->v_elementPath;
	item.link_path = pthis->v_path;
	item.place.hint = pthis->v_position;
	item.place.place_path = pthis->v_placePath;
	item.opposite_place.hint = OV_PMH_DEFAULT;
	item.opposite_place.place_path = NULL;
	/*	do the actual submit	*/
	pVtblClient->m_requestLink(pClient, NULL, 1, &item, (OV_INSTPTR_ov_domain) pthis, &ksapi_linkObject_callback);
	if(!(pClient->v_state & KSBASE_CLST_ERROR))
		pthis->v_status = KSAPI_COMMON_WAITINGFORANSWER;
	else
		pthis->v_status = KSAPI_COMMON_INTERNALERROR;

	return;
}

OV_DLLFNCEXPORT void ksapi_linkObject_setandsubmit(
    OV_INSTPTR_ksapi_linkObject          pobj,
	OV_STRING 									serverHost,
	OV_STRING 									serverName,
	OV_STRING 									path,
	OV_STRING 									elementpath,
	OV_INT										position,
	OV_STRING									place
) {
	OV_RESULT result;

	result = ksapi_KSApiCommon_genSetForSubmit(Ov_StaticPtrCast(ksapi_KSApiCommon, pobj), serverHost, serverName, path);
	if(Ov_Fail(result))
		return;

	result = ov_string_setvalue(&(pobj->v_elementPath), elementpath);
	if(Ov_Fail(result))
	{
		pobj->v_status = KSAPI_COMMON_INTERNALERROR;
		pobj->v_result = result;
		return;
	}

	result = ov_string_setvalue(&(pobj->v_placePath), place);
	if(Ov_Fail(result))
	{
		pobj->v_status = KSAPI_COMMON_INTERNALERROR;
		pobj->v_result = result;
		return;
	}

	pobj->v_position = position;

	ksapi_linkObject_submit(Ov_StaticPtrCast(ksapi_KSApiCommon, pobj));

	return;
}

void ksapi_linkObject_callback(const OV_INSTPTR_ov_domain this, const OV_INSTPTR_ov_domain that)
{
	OV_INSTPTR_ksapi_linkObject thisLinkObject = Ov_StaticPtrCast(ksapi_linkObject, this);
	OV_INSTPTR_ksbase_ClientBase pClient = Ov_StaticPtrCast(ksbase_ClientBase, that);
	OV_VTBLPTR_ksbase_ClientBase pVtblClient = NULL;
	OV_RESULT result;
	OV_UINT itemsLength;
	OV_RESULT* itemsVals;

	if(!this || !that)
	{
		KS_logfile_error(("callback issued with NULL pointers. aborting."));
		return;
	}

	Ov_GetVTablePtr(ksbase_ClientBase, pVtblClient, pClient);

	if(!pVtblClient)
	{
		KS_logfile_error(("%s callback: could not determine Vtable of Client %s. aborting",
				this->v_identifier, that->v_identifier));
		thisLinkObject->v_status = KSAPI_COMMON_INTERNALERROR;
		thisLinkObject->v_result = OV_ERR_BADOBJTYPE;
		return;
	}
	ov_memstack_lock();
	result = pVtblClient->m_processLink(pClient, NULL, (OV_RESULT*) &(thisLinkObject->v_result), &itemsLength, &itemsVals);
	if(Ov_Fail(result))
	{
		thisLinkObject->v_status = KSAPI_COMMON_INTERNALERROR;
		thisLinkObject->v_result = result;
		ov_memstack_unlock();
		return;
	}

	if(Ov_Fail(thisLinkObject->v_result))
	{
		thisLinkObject->v_status = KSAPI_COMMON_EXTERNALERROR;
		ov_memstack_unlock();
		return;
	}

	thisLinkObject->v_status = KSAPI_COMMON_REQUESTCOMPLETED;

	if(itemsLength)
	{
		thisLinkObject->v_result = *itemsVals;
	}
	ov_memstack_unlock();
	return;
}
