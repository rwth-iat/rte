/*
*	Copyright (C) 2015
*	Chair of Process Control Engineering,
*	Aachen University of Technology.
*	All rights reserved.
*
*	Redistribution and use in source and binary forms, with or without
*	modification, are permitted provided that the following conditions
*	are met:
*	1. Redistributions of source code must retain the above copyright
*	   notice, this list of conditions and the following disclaimer.
*	2. Redistributions in binary form must print or display the above
*	   copyright notice either during startup or must have a means for
*	   the user to view the copyright notice.
*	3. Redistributions in binary form must reproduce the above copyright
*	   notice, this list of conditions and the following disclaimer in
*		the documentation and/or other materials provided with the
*		distribution.
*	4. Neither the name of the Chair of Process Control Engineering nor
*		the name of the Aachen University of Technology may be used to
*		endorse or promote products derived from this software without
*		specific prior written permission.
*
*	THIS SOFTWARE IS PROVIDED BY THE CHAIR OF PROCESS CONTROL ENGINEERING
*	``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
*	LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
*	A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE CHAIR OF
*	PROCESS CONTROL ENGINEERING BE LIABLE FOR ANY DIRECT, INDIRECT,
*	INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
*	BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
*	OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
*	AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
*	LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY
*	WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
*	POSSIBILITY OF SUCH DAMAGE.
*
***********************************************************************
*
* CSHMI - Client-Side Human-Machine Interface
*
*	File:
*	------
*	Object.c
*
*	Editors:
*	--------
*	Je							Holger Jeromin <jeromin@plt.rwth-aachen.de>
*	GQ							Gustavo Quiros <g.quiros@plt.rwth-aachen.de>
*
*	SVN:
*	----
*	$Revision$
*	$Date$
*
*	History:
*	--------
*	18-August-2011			GQ		V0.1.0
*		-	File created
*
***********************************************************************/

#ifndef OV_COMPILE_LIBRARY_cshmi
#define OV_COMPILE_LIBRARY_cshmi
#endif

//#define cshmi_Object_ERROR 1
//#define ZINDEX_DEBUG 1

#include "cshmilib.h"

/**
 * corrects ov_containment position to the desired zindex
 */
OV_DLLFNCEXPORT OV_RESULT cshmi_Object_zindex_set(
	OV_INSTPTR_cshmi_Object          pObj,
	const OV_UINT  Value
) {
	//	Local Pointer
	//
	OV_INSTPTR_ov_domain
		pDom = Ov_GetParent(ov_containment, pObj);
	OV_INSTPTR_cshmi_Object
		pSiblingObj = NULL;
	OV_INSTPTR_cshmi_downloadApplication pDownloadApplication = NULL;
	OV_UINT
		SiblingNumber			= 0;
	OV_RESULT	fr						= OV_ERR_OK;

	if (pDom == NULL){
		#ifdef ZINDEX_DEBUG
			ov_logfile_debug("%s:%d trying to set an zindex with the OV_PART %s. Skipping.", __FILE__, __LINE__,
				pObj->v_identifier);
		#endif
		return OV_ERR_OK;
	}
	cshmi_Object_resetCache(Ov_PtrUpCast(cshmi_Object, pObj));

	pDownloadApplication =  Ov_StaticPtrCast(cshmi_downloadApplication, Ov_GetFirstChild(ov_instantiation, pclass_cshmi_downloadApplication));
	if(pDownloadApplication != NULL){
		//throw away childlist from turbo
		CSHMI_EMPTYCLASSCACHEENTRY(Childlist);
	}

	//iterate over all siblings to find target position
	Ov_ForEachChildEx(ov_containment, pDom, pSiblingObj, cshmi_Object){
		#ifdef ZINDEX_DEBUG
			ov_logfile_debug("%d: %s Request %u, active Sibling: %s with (perhaps faked) index %u", __LINE__,
				pObj->v_identifier,
				Value,
				pSiblingObj->v_identifier,
				SiblingNumber);
		#endif
		if (pSiblingObj == pObj && SiblingNumber == Value){
			//zindex should not change
			return OV_ERR_OK;
		}else if (pSiblingObj == pObj && SiblingNumber < Value){
			//old zindex is smaller than the new one
			//move us back. This is done by skipping the incrementation of the SiblingNumber
		}else if (SiblingNumber == Value){
			//move us in front of the active sibling
			Ov_Unlink(ov_containment, pDom, pObj);
			fr = Ov_LinkRelativePlaced(
				ov_containment,
				pDom,
				pObj,
				OV_PMH_BEFORE,
				pSiblingObj);
			if (Ov_Fail(fr)){
				#ifdef cshmi_Object_ERROR
					ov_logfile_error("%s:%d - %s - Error during > re-link (%s)", __FILE__, __LINE__,
						pObj->v_identifier,
						ov_result_getresulttext(fr));
				#endif
				return fr;
			}
			return OV_ERR_OK;
		}else{
			//no action required with this sibling
			SiblingNumber++;
		}
	}

	//no hit till now => move us to the End
	Ov_Unlink(ov_containment, pDom, pObj);
	fr = Ov_LinkPlaced(ov_containment, pDom, pObj, OV_PMH_END);
	#ifdef cshmi_Object_ERROR
		if (Ov_Fail(fr)){
			ov_logfile_error("%s:%d - %s - Error during > re-link (%s)", __FILE__, __LINE__,
				pObj->v_identifier,
				ov_result_getresulttext(fr));
		}
	#endif
	return fr;
}

/*
 * returns the index in ov_containment association
 * returns 0, if the object is a PART
 */
OV_DLLFNCEXPORT OV_UINT cshmi_Object_zindex_get(
	OV_INSTPTR_cshmi_Object          pObj
) {
	OV_INSTPTR_ov_domain
		pDom = Ov_GetParent(ov_containment, pObj);
	OV_INSTPTR_cshmi_Object
		pSiblingObj = NULL;

	OV_UINT
		SiblingNumber			= 0;

	Ov_ForEachChildEx(ov_containment, pDom, pSiblingObj, cshmi_Object){
		#ifdef ZINDEX_DEBUG
			ov_logfile_debug("%d: zindex_get on %s, iteration number: %u", __LINE__,
				pObj->v_identifier,
				SiblingNumber);
		#endif
		if (pSiblingObj == pObj){
			//found our index
			break;
		}
		SiblingNumber++;
	}
	#ifdef ZINDEX_DEBUG
		ov_logfile_debug("%d: zindex_get on %s, result: %u", __LINE__,
			pObj->v_identifier,
			SiblingNumber);
	#endif
	return SiblingNumber;
}


/***********************************************************************
	getAccess
***********************************************************************/

OV_DLLFNCEXPORT OV_ACCESS cshmi_Object_getaccess(
	OV_INSTPTR_ov_object	pobj,
	const OV_ELEMENT		*pelem,
	const OV_TICKET		*pticket
)	{
	//we have nothing to hide, so all our normal variables can be read and written

	switch (pelem->elemtype){
		case OV_ET_VARIABLE :
			//prevent "special variables" from being read/write
			if (pelem->elemunion.pvar->v_offset >= offsetof(OV_INST_ov_object, __classinfo)){
				if (pelem->elemunion.pvar->v_vartype == OV_VT_CTYPE || pelem->elemunion.pvar->v_vartype == OV_VT_POINTER){
					return OV_AC_NONE;
				}else{
					return OV_AC_READWRITE;
				}
			}
			break;
		default:
			break;
	};
	return ov_object_getaccess(pobj, pelem, pticket);
};

/**
 * @param pObj Object to create
 */
OV_DLLFNCEXPORT OV_RESULT cshmi_Object_constructor(
	OV_INSTPTR_ov_object 	pObj
) {
	// local variables
	//
	OV_RESULT    result;
	OV_INSTPTR_cshmi_Object pinst = Ov_StaticPtrCast(cshmi_Object, pObj);
	OV_INSTPTR_cshmi_downloadApplication pDownloadApplication = NULL;

	/* do what the base class does first */
	result = ov_object_constructor(pObj);

	//reset own and mark class cache entry invalid
	cshmi_Object_resetCache(pinst);

	pDownloadApplication =  Ov_StaticPtrCast(cshmi_downloadApplication, Ov_GetFirstChild(ov_instantiation, pclass_cshmi_downloadApplication));
	if(pDownloadApplication != NULL){
		//throw away childlist from turbo
		CSHMI_EMPTYCLASSCACHEENTRY(Childlist);
	}

	return result;
}

OV_DLLFNCEXPORT void cshmi_Object_startup(
	OV_INSTPTR_ov_object 	pobj
) {
	/*
	 *   local variables
	 */
	OV_INSTPTR_cshmi_Object pinst = Ov_StaticPtrCast(cshmi_Object, pobj);

	/* do what the base class does first */
	ov_object_startup(pobj);

	//this is a pointer to heap memory, so clean
	pinst->v_ConfigCache.asJSON = NULL;
	pinst->v_ConfigCache.cacheDirty = TRUE;
	pinst->v_ConfigCache.parentObject = NULL;
	pinst->v_ConfigCache.identifier = NULL;

	return;
}


OV_DLLFNCEXPORT OV_RESULT cshmi_Object_rename(
		OV_INSTPTR_ov_object	pobj,
		OV_STRING				newid,
		OV_INSTPTR_ov_domain	pnewparent
) {
	/*
	 *   local variables
	 */
	OV_INSTPTR_cshmi_downloadApplication pDownloadApplication = NULL;
	OV_INSTPTR_cshmi_Object pinst = Ov_StaticPtrCast(cshmi_Object, pobj);

	//reset own and mark class cache entry invalid
	cshmi_Object_resetCache(pinst);

	pDownloadApplication =  Ov_StaticPtrCast(cshmi_downloadApplication, Ov_GetFirstChild(ov_instantiation, pclass_cshmi_downloadApplication));
	if(pDownloadApplication != NULL){
		//throw away all informations from turbo as they could be children of us. Each object has an additional cache which detects if it is really affected by this renaming
		CSHMI_EMPTYCLASSCACHEENTRY(Group);
		CSHMI_EMPTYCLASSCACHEENTRY(TemplateDefinition);
		CSHMI_EMPTYCLASSCACHEENTRY(InstantiateTemplate);
		CSHMI_EMPTYCLASSCACHEENTRY(Rectangle);
		CSHMI_EMPTYCLASSCACHEENTRY(Circle);
		CSHMI_EMPTYCLASSCACHEENTRY(Text);
		CSHMI_EMPTYCLASSCACHEENTRY(Line);
		CSHMI_EMPTYCLASSCACHEENTRY(Polyline);
		CSHMI_EMPTYCLASSCACHEENTRY(Polygon);
		CSHMI_EMPTYCLASSCACHEENTRY(Path);
		CSHMI_EMPTYCLASSCACHEENTRY(Ellipse);

		CSHMI_EMPTYCLASSCACHEENTRY(SetValue);
		CSHMI_EMPTYCLASSCACHEENTRY(SetConcatValue);
		CSHMI_EMPTYCLASSCACHEENTRY(SetMathValue);
		CSHMI_EMPTYCLASSCACHEENTRY(GetValue);
		CSHMI_EMPTYCLASSCACHEENTRY(ChildrenIterator);
		CSHMI_EMPTYCLASSCACHEENTRY(IfThenElse);
		CSHMI_EMPTYCLASSCACHEENTRY(Compare);
		CSHMI_EMPTYCLASSCACHEENTRY(CompareIteratedChild);
		CSHMI_EMPTYCLASSCACHEENTRY(TimeEvent);
		CSHMI_EMPTYCLASSCACHEENTRY(RoutePolyline);
		CSHMI_EMPTYCLASSCACHEENTRY(TranslationSource);
		CSHMI_EMPTYCLASSCACHEENTRY(CreateObject);
		CSHMI_EMPTYCLASSCACHEENTRY(Vibrate);

		CSHMI_EMPTYCLASSCACHEENTRY(Childlist);
	}

	return OV_ERR_OK;
}

OV_DLLFNCEXPORT void cshmi_Object_shutdown(
		OV_INSTPTR_ov_object 	pobj
) {
	/*
	 *   local variables
	 */
	OV_INSTPTR_cshmi_Object pinst = Ov_StaticPtrCast(cshmi_Object, pobj);

	//this is a pointer to heap memory, so free the memory
	Ov_HeapFree(pinst->v_ConfigCache.asJSON);
	pinst->v_ConfigCache.asJSON = NULL;
	pinst->v_ConfigCache.cacheDirty = TRUE;

	ov_string_setvalue(&pinst->v_ConfigCache.identifier, NULL);

	/* set the object's state to "shut down" */
	ov_object_shutdown(pobj);

	return;
}


OV_DLLFNCEXPORT void cshmi_Object_destructor(
		OV_INSTPTR_ov_object 	pobj
) {
	/*
	 *   local variables
	 */
	OV_INSTPTR_cshmi_Object pinst = Ov_StaticPtrCast(cshmi_Object, pobj);
	OV_INSTPTR_cshmi_downloadApplication pDownloadApplication = NULL;

	//reset own and mark class cache entry invalid
	cshmi_Object_resetCache(pinst);

	pDownloadApplication =  Ov_StaticPtrCast(cshmi_downloadApplication, Ov_GetFirstChild(ov_instantiation, pclass_cshmi_downloadApplication));
	if(pDownloadApplication != NULL){
		//throw away childlist from turbo
		CSHMI_EMPTYCLASSCACHEENTRY(Childlist);
	}

	/* destroy object */
	ov_object_destructor(pobj);

	return;
}

