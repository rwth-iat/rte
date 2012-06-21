/*
*	Copyright (C) 2012
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

#define cshmi_Object_ERROR 0
//#define ZINDEX_DEBUG 1

#include "cshmilib.h"

/***********************************************************************
	zindex magic
***********************************************************************/

OV_DLLFNCEXPORT OV_RESULT cshmi_Object_zindex_set(
	OV_INSTPTR_cshmi_Object          pObj,
	const OV_UINT  DesValue
) {
	//	Local Pointer
	//
	OV_INSTPTR_ov_domain
		pDom = Ov_GetParent(ov_containment, pObj);
	OV_INSTPTR_cshmi_Object
		pSiblingObj = NULL;

	//	Local Variables
	//
	OV_BOOL		ConstructorCall	= FALSE;
	OV_UINT		SiblingObj			= 0;
	OV_RESULT	fr						= OV_ERR_OK;

	#ifdef ZINDEX_DEBUG
		ov_logfile_debug("%d: %s/%s - Request to set zindex from %u to %u", __LINE__,
			Ov_GetParent(ov_containment, pObj)?Ov_GetParent(ov_containment, pObj)->v_identifier:"-",
			pObj->v_identifier,
			pObj->v_zindex,
			DesValue);
	#endif

	//	count sibling components (self is also counted)
	//
	Ov_ForEachChildEx(ov_containment, pDom, pSiblingObj, cshmi_Object)
	{
		if (	pObj->v_zindex			== 0
			&&	pSiblingObj->v_zindex	== 0
			&&	pObj							!= pSiblingObj)
		{
			//the other obj is zindex=0, we are zindex=0,
			//so it is a call directly after the constructor
			ConstructorCall = TRUE;
		}
		SiblingObj = SiblingObj + 1;
	}

	//	test if constructor has called setAccessor-Method
	//
	if (	ConstructorCall	== TRUE){
		pObj->v_zindex = SiblingObj - 1;
		#ifdef ZINDEX_DEBUG
			ov_logfile_debug("%d: %s - constructor, having at least one 'older' sibling, so setting zindex to last number: %u. Exiting.", __LINE__, pObj->v_identifier, pObj->v_zindex);
		#endif
		return OV_ERR_OK;
	}else if (SiblingObj == 1){
		#ifdef ZINDEX_DEBUG
			ov_logfile_debug("%d: %s - we are alone, so setting zindex to 0. Exiting.", __LINE__, pObj->v_identifier);
		#endif
		pObj->v_zindex = 0;
		return OV_ERR_OK;
	}else if (pObj->v_zindex == DesValue){
		//	actual zindex = desired zindex
		//
		#ifdef ZINDEX_DEBUG
			ov_logfile_debug("%d: %s - no change requested, so leaving at %u. Exiting.", __LINE__, pObj->v_identifier, pObj->v_zindex);
		#endif
		return OV_ERR_OK;
	}else{
		#ifdef ZINDEX_DEBUG
			ov_logfile_debug("%d: %s - let the magic begin!", __LINE__, pObj->v_identifier);
		#endif
	}

	//	change zindex(es)
	//
	if (pObj->v_zindex > DesValue){
		//	actual zindex > desired zindex
		//	cshmi_Object_constructor() guarantees we have never actual zindex == desired zindex
		//
		#ifdef ZINDEX_DEBUG
			ov_logfile_debug("%d: %s - actual zindex > desired zindex", __LINE__, pObj->v_identifier);
		#endif
		Ov_ForEachChildEx(ov_containment, pDom, pSiblingObj, cshmi_Object){
			if (pSiblingObj != pObj){
				if (pSiblingObj->v_zindex == DesValue){
					#ifdef ZINDEX_DEBUG
						ov_logfile_debug("%d: %s - changing containment position to before %s", __LINE__,
							pObj->v_identifier,
							pSiblingObj->v_identifier);
					#endif

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
				}

				if (	pSiblingObj->v_zindex >= DesValue
					&&	pSiblingObj->v_zindex <= pObj->v_zindex - 1)
				{
					#ifdef ZINDEX_DEBUG
						ov_logfile_debug("%d: %s - increasing %s zindex from %u to %u", __LINE__,
							pObj->v_identifier,
							pSiblingObj->v_identifier,
							pSiblingObj->v_zindex,
							pSiblingObj->v_zindex + 1);
					#endif

					pSiblingObj->v_zindex = pSiblingObj->v_zindex + 1;
				}
			}
		}
	} else if (pObj->v_zindex <= DesValue){
		//	actual zindex < desired zindex
		//	cshmi_Object_constructor() guarantees we have never actual zindex == desired zindex
		//
		#ifdef ZINDEX_DEBUG
			ov_logfile_debug("%d: %s - actual zindex < desired zindex", __LINE__, pObj->v_identifier);
		#endif
		Ov_ForEachChildEx(ov_containment, pDom, pSiblingObj, cshmi_Object){
			if (pSiblingObj != pObj){
				if (	pSiblingObj->v_zindex	== DesValue
					||	DesValue						>= SiblingObj)
				{
					#ifdef ZINDEX_DEBUG
						ov_logfile_debug("%d: %s - changing containment position to after %s", __LINE__,
							pObj->v_identifier,
							pSiblingObj->v_identifier);
					#endif

					Ov_Unlink(ov_containment, pDom, pObj);
					fr = Ov_LinkRelativePlaced(
						ov_containment,
						pDom,
						pObj,
						OV_PMH_AFTER,
						pSiblingObj);
					if (Ov_Fail(fr)){
						#ifdef cshmi_Object_ERROR
							ov_logfile_error("%s:%d - %s - Error during < re-link (%s)", __FILE__, __LINE__,
								pObj->v_identifier,
								ov_result_getresulttext(fr));
						#endif

						return fr;
					}
				}

				if (	pSiblingObj->v_zindex >= pObj->v_zindex + 1
					&&	pSiblingObj->v_zindex <= DesValue)
				{
					#ifdef ZINDEX_DEBUG
						ov_logfile_debug("%d: %s - decreasing %s zindex from %u to %u", __LINE__,
							pObj->v_identifier,
							pSiblingObj->v_identifier,
							pSiblingObj->v_zindex,
							pSiblingObj->v_zindex - 1);
					#endif
					pSiblingObj->v_zindex = pSiblingObj->v_zindex - 1;
				}
			}
		}
	}else{
		#ifdef ZINDEX_DEBUG
			ov_logfile_debug("%d: %s - found zindex == desvalue", __LINE__,
				pObj->v_identifier);
		#endif
	}

	if (SiblingObj == 0){
		//OV-Part should have an clean 0
		pObj->v_zindex = 0;
		#ifdef ZINDEX_DEBUG
			ov_logfile_debug("%d: %s - OV-Part set to zindex 0. Exiting.", __LINE__, pObj->v_identifier);
		#endif
	}else if (SiblingObj - 1 > DesValue){
		pObj->v_zindex = DesValue;
	} else {
		pObj->v_zindex = SiblingObj - 1;
	}

	#ifdef ZINDEX_DEBUG
		SiblingObj = 0;
		Ov_ForEachChildEx(ov_containment, pDom, pSiblingObj, cshmi_Object)
		{
			ov_logfile_debug("%d: check: Position %u has %u: %s ", __LINE__, SiblingObj, pSiblingObj->v_zindex, pSiblingObj->v_identifier);
			SiblingObj++;
		}
	#endif

	return OV_ERR_OK;
};

/**
 * renumber all zindex of all children to match ov_containment
 * @param pDom Domain to fix
 * @return void
 */

void cshmi_Object_zindexcorrection(
	OV_INSTPTR_ov_domain pDom
) {
	//	local variables
	//
	OV_INSTPTR_cshmi_Object
		pChildObj = NULL;

	OV_UINT newindex = 0;

	//test if we are handling an OV-PART
	if (pDom != NULL){
		//	renumber sibling components
		//
		Ov_ForEachChildEx(ov_containment, pDom, pChildObj, cshmi_Object){
			#ifdef ZINDEX_DEBUG
				ov_logfile_debug("%d: %s changed from %u to %u:  ", __LINE__, pSiblingObj->v_identifier, pChildObj->v_zindex, newindex);
			#endif
			//this is the truth, so no setaccessor needed
			pChildObj->v_zindex = newindex;
			newindex++;
		}
	}
	return;
}

/**
 * set zindex to top and
 * correct zindex of all siblings (could have been wrong from moving objects arround)
 * @param pObj Object to create
 */
OV_DLLFNCEXPORT OV_RESULT cshmi_Object_constructor(
	OV_INSTPTR_ov_object 	pObj
) {
	// local variables
	//
	OV_INSTPTR_ov_domain
		pDom = Ov_GetParent(ov_containment, pObj);
	OV_RESULT    result;


	/* do what the base class does first */
	result = ov_object_constructor(pObj);
	if(Ov_Fail(result))
		return result;

	#ifdef ZINDEX_DEBUG
		ov_logfile_debug("^ %d: constructor: %s/%s moving on top", __LINE__, Ov_GetParent(ov_containment, pObj)?Ov_GetParent(ov_containment, pObj)->v_identifier:"-", pObj->v_identifier);
	#endif
	//test if we are creating an OV-PART
	if (pDom != NULL){
		cshmi_Object_zindexcorrection(pDom);
	}
	#ifdef ZINDEX_DEBUG
		ov_logfile_debug("$ %d: constructor: %s constructed with zindex: %u.", __LINE__, pObj->v_identifier, Ov_StaticPtrCast(cshmi_Object, pObj)->v_zindex);
	#endif
	return OV_ERR_OK;
}

/**
 * correct zindex of all siblings (could have been wrong from moving objects arround)
 * @param pObj Object to delete
 */
OV_DLLFNCEXPORT void cshmi_Object_destructor(
	OV_INSTPTR_ov_object 	pObj
) {
	//	local variables
	//
	OV_INSTPTR_ov_domain
		pDom = Ov_GetParent(ov_containment, pObj);

	//test if we are deleting an OV-PART
	if (pDom != NULL){
		//unlink from own domain
		Ov_Unlink(ov_containment, pDom, pObj);

		//	renumber sibling components (self is removed from it)
		//
		cshmi_Object_zindexcorrection(pDom);
	}else{
		#ifdef ZINDEX_DEBUG
			ov_logfile_debug("%d: %s had no parent via containment", __LINE__, pObj->v_identifier);
		#endif
	}

	// destroy object
	ov_object_destructor(pObj);

	return;
}

/***********************************************************************
	getAccess
***********************************************************************/

OV_DLLFNCEXPORT OV_ACCESS cshmi_Object_getaccess(
	OV_INSTPTR_ov_object	pobj,
	const OV_ELEMENT		*pelem,
	const OV_TICKET		*pticket
)	{
	//we have nothing to hide, so all our variables can be read and written

	switch (pelem->elemtype){
		case OV_ET_VARIABLE :
			//prevent "special variables" from being read/write
			if (pelem->elemunion.pvar->v_offset >= offsetof(OV_INST_ov_object, __classinfo))
			{
				return OV_AC_READWRITE;
			}
			break;
		default:
			break;
	};
	return ov_object_getaccess(pobj, pelem, pticket);
};
