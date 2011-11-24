/*
*	Copyright (C) 2011
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
*	Element.c
*
*	Editors:
*	--------
*	Je							Holger Jeromin <jeromin@plt.rwth-aachen.de>
*	GQ							Gustavo Quiros <g.quiros@plt.rwth-aachen.de>
*	St							Stefan Schmitz <StefanS@plt.rwth-aachen.de>
*
*	SVN:
*	----
*	$Revision: 1.47 $
*	$Date: 2011-08-09 11:12:26 $
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


#include "cshmi.h"
#include "cshmilib.h"


OV_DLLFNCEXPORT OV_BOOL cshmi_Element_visible_get(
    OV_INSTPTR_cshmi_Element          pobj
) {
    return pobj->v_visible;
}

OV_DLLFNCEXPORT OV_RESULT cshmi_Element_visible_set(
    OV_INSTPTR_cshmi_Element          pobj,
    const OV_BOOL  value
) {
    pobj->v_visible = value;
    return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_UINT cshmi_Element_zindex_get(
    OV_INSTPTR_cshmi_Element          pobj
) {
    return pobj->v_zindex;
}

OV_DLLFNCEXPORT OV_RESULT cshmi_Element_zindex_set(
    OV_INSTPTR_cshmi_Element          pObj,
    const OV_UINT  DesValue
) {
	//	Local Pointer
	//
	OV_INSTPTR_ov_domain
		pDom = Ov_GetParent(ov_containment, pObj);
	OV_INSTPTR_cshmi_Element
		pSiblingObj = NULL;

	//	Local Variables
	//
	OV_BOOL		ConstructorCall	= FALSE;
	OV_UINT		SiblingObj			= 0;
	OV_RESULT	fr						= OV_ERR_OK;

	#ifdef cshmi_Element_DEBUG
		ov_logfile_debug("%d: %s - cshmi_Element_zindex_set - Set zindex from %u to %u", __LINE__,
			pObj->v_identifier,
			pObj->v_zindex,
			DesValue);
	#endif

	//	count sibling components (self is also counted)
	//
	Ov_ForEachChildEx(ov_containment, pDom, pSiblingObj, cshmi_Element)
	{
		if (	pObj->v_zindex			== 0
			&&	pSiblingObj->v_zindex	== 0
			&&	pObj							!= pSiblingObj)
		{
			ConstructorCall = TRUE;
		}
		SiblingObj = SiblingObj + 1;
	}

	//	test if constructor has called setAccessor-Method
	//
	if (	ConstructorCall	== TRUE
		||	SiblingObj			== 1)
	{
		if (DesValue == CSHMI_ZINDEX_TOP){
			pObj->v_zindex = SiblingObj - 1;
			return OV_ERR_OK;
		}
	} else {
		if (pObj->v_zindex == DesValue){
			//	actual zindex = desired zindex
			//
			return OV_ERR_OK;
		}
	}

	//	change zindex(es)
	//
	if (pObj->v_zindex > DesValue){
		//	actual zindex > desired zindex
		//	cshmi_Element_constructor() guarantees we have never actual zindex == desired zindex
		//
		Ov_ForEachChildEx(ov_containment, pDom, pSiblingObj, cshmi_Element){
			if (pSiblingObj != pObj){
				if (pSiblingObj->v_zindex == DesValue){
					#ifdef cshmi_Element_DEBUG
						ov_logfile_debug("%d: %s - cshmi_Element_zindex_set - changing containment position to before %s", __LINE__,
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
						#ifdef cshmi_Element_ERROR
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
					#ifdef cshmi_Element_DEBUG
						ov_logfile_debug("%d: %s - cshmi_Element_zindex_set - increasing %s zindex from %u to %u", __LINE__,
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
		//	cshmi_Element_constructor() guarantees we have never actual zindex == desired zindex
		//
		Ov_ForEachChildEx(ov_containment, pDom, pSiblingObj, cshmi_Element){
			if (pSiblingObj != pObj){
				if (	pSiblingObj->v_zindex	== DesValue
					||	DesValue						>= SiblingObj)
				{
					#ifdef cshmi_Element_DEBUG
						ov_logfile_debug("%d: %s - cshmi_Element_zindex_set - changing containment position to after %s", __LINE__,
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
						#ifdef cshmi_Element_ERROR
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
					#ifdef cshmi_Element_DEBUG
						ov_logfile_debug("%d: %s - cshmi_Element_zindex_set - decreasing %s zindex from %u to %u", __LINE__,
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
		#ifdef cshmi_Element_DEBUG
			ov_logfile_debug("%d: %s - cshmi_Element_zindex_set - found zindex == desvalue", __LINE__,
				pObj->v_identifier);
		#endif
	}

	if (SiblingObj - 1 > DesValue){
		pObj->v_zindex = DesValue;
	} else {
		pObj->v_zindex = SiblingObj - 1;
	}

	#ifdef cshmi_Element_DEBUG
		SiblingObj = 0;
		Ov_ForEachChildEx(ov_containment, pDom, pSiblingObj, cshmi_Element)
		{
			ov_logfile_debug("%d: %u: %s - %u", __LINE__, SiblingObj, pSiblingObj->v_identifier, pSiblingObj->v_zindex);
			SiblingObj++;
		}
	#endif

	return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_STRING cshmi_Element_stroke_get(
    OV_INSTPTR_cshmi_Element          pobj
) {
    return pobj->v_stroke;
}

OV_DLLFNCEXPORT OV_RESULT cshmi_Element_stroke_set(
    OV_INSTPTR_cshmi_Element          pobj,
    const OV_STRING  value
) {
    return ov_string_setvalue(&pobj->v_stroke,value);
}

OV_DLLFNCEXPORT OV_STRING cshmi_Element_fill_get(
    OV_INSTPTR_cshmi_Element          pobj
) {
    return pobj->v_fill;
}

OV_DLLFNCEXPORT OV_RESULT cshmi_Element_fill_set(
    OV_INSTPTR_cshmi_Element          pobj,
    const OV_STRING  value
) {
    return ov_string_setvalue(&pobj->v_fill,value);
}

OV_DLLFNCEXPORT OV_SINGLE cshmi_Element_opacity_get(
    OV_INSTPTR_cshmi_Element          pobj
) {
    return pobj->v_opacity;
}

OV_DLLFNCEXPORT OV_RESULT cshmi_Element_opacity_set(
    OV_INSTPTR_cshmi_Element          pobj,
    const OV_SINGLE  value
) {
    pobj->v_opacity = value;
    return OV_ERR_OK;
}


OV_DLLFNCEXPORT OV_RESULT cshmi_Element_constructor(
	OV_INSTPTR_ov_object 	pobj
) {
	// local variables
	//
	OV_INSTPTR_cshmi_Element pinst = Ov_StaticPtrCast(cshmi_Element, pobj);
	OV_RESULT    result;

	/* do what the base class does first */
	result = ov_object_constructor(pobj);
	if(Ov_Fail(result))
		return result;

	return cshmi_Element_zindex_set(pinst, CSHMI_ZINDEX_DEFAULT);
}
