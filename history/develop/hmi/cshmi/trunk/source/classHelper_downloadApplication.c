
/******************************************************************************
*
*   FILE
*   ----
*   downloadApplication.c
*
*   History
*   -------
*   2013-04-29   File created
*
*******************************************************************************
*
*   This file is generated by the 'acplt_builder' command
*
******************************************************************************/


#ifndef OV_COMPILE_LIBRARY_cshmi
#define OV_COMPILE_LIBRARY_cshmi
#endif

#include "cshmilib.h"

//#define CSHMI_TIMING_DEBUG 1
//#define CSHMI_CHILDLIST_DEBUG 1

#ifdef CSHMI_TIMING_DEBUG
	OV_UINT functioncounter = 0;
	OV_UINT iterationcounter = 0;
#endif

/**
 * returns a list of ov_containment
 * embedment/OV_PART is known to the visualisation system
 * @param pObj object to start childlist
 * @param strResult Pointer to the result String
 * @param isFirstEntry if FALSE the function adds an "," as a separator
 * @return
 */
static OV_RESULT cshmi_downloadApplication_buildChildList(OV_INSTPTR_ov_object pObj, OV_STRING*strResult, OV_BOOL isFirstEntry){
	OV_INSTPTR_ov_object pChildObj = NULL;
	OV_INSTPTR_cshmi_IfThenElse pIfThenElse = NULL;
	OV_INSTPTR_cshmi_ChildrenIterator pChildrenIterator = NULL;

	//local string to make ov_string functions fast
	OV_STRING strChildList = NULL;
	OV_RESULT result = OV_ERR_OK;

#ifdef CSHMI_TIMING_DEBUG
	functioncounter++;
#endif

	/* builds something like this (without the whitespaces):
	%22/TechUnits/cshmi%22:%7B
		%22ChildListParameters%22:%5B
			%22turbo /acplt/cshmi/downloadApplication%22,
			%22Templates /acplt/ov/domain%22
		%5D
	%7D,
	%22/TechUnits/cshmi/Templates%22:%7B
		%22ChildListParameters%22:%5B
		%5D
	%7D,
	%22/TechUnits/sheet%22:
	%7B
		%22ChildListParameters%22:%5B
		%5D
	%7D
	*/
	if(!Ov_CanCastTo(ov_domain, pObj)){
		return OV_ERR_OK;
	}
#ifdef CSHMI_CHILDLIST_DEBUG
	ov_logfile_debug("Childlist: listing children for %s", pObj->v_identifier);
#endif


	if(isFirstEntry == FALSE){
		ov_string_setvalue(&strChildList, ",%22");
	}else{
		ov_string_setvalue(&strChildList, "%22");
	}
	ov_memstack_lock();
	ov_string_append(&strChildList, ov_path_getcanonicalpath(pObj, 2));
	ov_memstack_unlock();
	ov_string_append(&strChildList, "%22:%7B");
	ov_string_append(&strChildList, "%22ChildListParameters%22:%5B");

	//iterate over all childrens and list them
	Ov_ForEachChild(ov_containment, Ov_StaticPtrCast(ov_domain, pObj), pChildObj){
		ov_string_append(&strChildList, "%22");
		ov_string_append(&strChildList, pChildObj->v_identifier);
		ov_string_append(&strChildList, " ");
		ov_memstack_lock();
		ov_string_append(&strChildList, ov_path_getcanonicalpath(Ov_PtrUpCast(ov_object, Ov_GetClassPtr(pChildObj)), 2));
		ov_memstack_unlock();
		ov_string_append(&strChildList, "%22");
		if(Ov_GetNextChild(ov_containment, pChildObj) != NULL){
			ov_string_append(&strChildList, ",");
		}
	};
	ov_string_append(&strChildList, "%5D%7D");

	//one single append to the huge string
	result = ov_string_append(strResult, strChildList);
	ov_string_setvalue(&strChildList, NULL);
	if(Ov_Fail(result)){
		return result;
	}

	//some of our children should list their children, too
	Ov_ForEachChild(ov_containment, Ov_StaticPtrCast(ov_domain, pObj), pChildObj){
		//interesting children are domains or some cshmi objects
		if(	Ov_GetParent(ov_instantiation, pChildObj) == pclass_ov_domain
			|| Ov_CanCastTo(cshmi_ContainerElement, pChildObj)
			|| Ov_CanCastTo(cshmi_Element, pChildObj)
			|| Ov_GetParent(ov_instantiation, pChildObj) == pclass_cshmi_SetConcatValue
			|| Ov_GetParent(ov_instantiation, pChildObj) == pclass_cshmi_SetMathValue
			|| Ov_CanCastTo(cshmi_Event, pChildObj)
			){
			result = cshmi_downloadApplication_buildChildList(pChildObj, strResult, FALSE);
			if(Ov_Fail(result)){
				return result;
			}
		}else
		//some children have no containment but their parts have interesting containments
		if(Ov_GetParent(ov_instantiation, pChildObj) == pclass_cshmi_IfThenElse){
			pIfThenElse = Ov_StaticPtrCast(cshmi_IfThenElse, pChildObj);
			result = cshmi_downloadApplication_buildChildList(Ov_PtrUpCast(ov_object, Ov_GetPartPtr(if, pIfThenElse)), strResult, FALSE);
			if(Ov_Fail(result)){
				return result;
			}
			result = cshmi_downloadApplication_buildChildList(Ov_PtrUpCast(ov_object, Ov_GetPartPtr(then, pIfThenElse)), strResult, FALSE);
			if(Ov_Fail(result)){
				return result;
			}
			result = cshmi_downloadApplication_buildChildList(Ov_PtrUpCast(ov_object, Ov_GetPartPtr(else, pIfThenElse)), strResult, FALSE);
			if(Ov_Fail(result)){
				return result;
			}
		}else if(Ov_GetParent(ov_instantiation, pChildObj) == pclass_cshmi_ChildrenIterator){
			pChildrenIterator = Ov_StaticPtrCast(cshmi_ChildrenIterator, pChildObj);
			result = cshmi_downloadApplication_buildChildList(Ov_PtrUpCast(ov_object, Ov_GetPartPtr(forEachChild, pChildrenIterator)), strResult, FALSE);
			if(Ov_Fail(result)){
				return result;
			}
#ifdef CSHMI_CHILDLIST_DEBUG
		}else{
			ov_logfile_debug("Childlist: skipped childrenlist for %s", pChildObj->v_identifier);
#endif
		}
	}

	return OV_ERR_OK;
}

/**
 *	builds the graphical elements into the cache
 * @param strResult
 * @return
 */
static OV_RESULT cshmi_downloadApplication_buildElementList(OV_STRING*strResult){
	OV_INSTPTR_ov_object pObj = NULL;

	//be careful to adjust the ov_string_print at the end of the function
	#define MAXELEMENTENTRIES 11
	OV_STRING ResultListVec[MAXELEMENTENTRIES] = {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL};
	//Note: we do not precache blackbox or image! To risky with the encoding of the Strings.
	OV_STRING strIterate = NULL;
	OV_UINT ResultListIndex = 0;
	OV_STRING ResultFormat = NULL;

	OV_STRING temp = NULL;
	OV_UINT i = 0;
	OV_BOOL elementInstantiated = FALSE;
	OV_BOOL elementIsfirst = TRUE;
	OV_RESULT result = OV_ERR_OK;
	ov_string_setvalue(&ResultFormat, "%s");

#ifdef CSHMI_TIMING_DEBUG
	functioncounter++;
#endif

	/* builds something like this (without the whitespaces):
	%22/TechUnits/sheet%22:%7B
		%22Parameters%22:%7B
			%22x%22:%220.000000%22,
			%22y%22:%220.000000%22,
			%22width%22:%221000.000000%22,
			%22height%22:%22900.000000%22,
			%22opacity%22:%221.000000%22,
			%22rotate%22:%220%22,
			%22hideable%22:%22FALSE%22,
			%22visible%22:%22TRUE%22,
			%22TemplateDefinition%22:%22%22,
			%22FBReference%22:%22%22,
			%22FBVariableReference%22:%22%22,
			%22ConfigValues%22:%22%22
		%7D
	%7D
	 */

	elementInstantiated = FALSE;
	Ov_ForEachChild(ov_instantiation, pclass_cshmi_Group, pObj){
#ifdef CSHMI_TIMING_DEBUG
		iterationcounter++;
#endif
		elementInstantiated = TRUE;
		ov_string_setvalue(&strIterate, cshmi_Object_getConfigAsJSON(Ov_StaticPtrCast(cshmi_Object, pObj))); //from heap mem
		if(strIterate && Ov_GetNextChild(ov_instantiation, pObj) != NULL){
			ov_string_append(&strIterate, ",");
		}
		//append does nothing when appending a NULL String
		ov_string_append(&ResultListVec[ResultListIndex], strIterate);
	}
	if(elementInstantiated == TRUE){
		ResultListIndex++;
		if(elementIsfirst == TRUE){
			ov_string_append(&ResultFormat, "%s");
		}else{
			ov_string_append(&ResultFormat, ",%s");
		}
		elementIsfirst = FALSE;
	}

	elementInstantiated = FALSE;
	Ov_ForEachChild(ov_instantiation, pclass_cshmi_TemplateDefinition, pObj){
#ifdef CSHMI_TIMING_DEBUG
		iterationcounter++;
#endif
		elementInstantiated = TRUE;
		ov_string_setvalue(&strIterate, cshmi_Object_getConfigAsJSON(Ov_StaticPtrCast(cshmi_Object, pObj))); //from heap mem
		if(strIterate && Ov_GetNextChild(ov_instantiation, pObj) != NULL){
			ov_string_append(&strIterate, ",");
		}
		//append does nothing when appending a NULL String
		ov_string_append(&ResultListVec[ResultListIndex], strIterate);
	}
	if(elementInstantiated == TRUE){
		ResultListIndex++;
		if(elementIsfirst == TRUE){
			ov_string_append(&ResultFormat, "%s");
		}else{
			ov_string_append(&ResultFormat, ",%s");
		}
		elementIsfirst = FALSE;
	}

	elementInstantiated = FALSE;
	Ov_ForEachChild(ov_instantiation, pclass_cshmi_InstantiateTemplate, pObj){
#ifdef CSHMI_TIMING_DEBUG
		iterationcounter++;
#endif
		elementInstantiated = TRUE;
		ov_string_setvalue(&strIterate, cshmi_Object_getConfigAsJSON(Ov_StaticPtrCast(cshmi_Object, pObj))); //from heap mem
		if(strIterate && Ov_GetNextChild(ov_instantiation, pObj) != NULL){
			ov_string_append(&strIterate, ",");
		}
		//append does nothing when appending a NULL String
		ov_string_append(&ResultListVec[ResultListIndex], strIterate);
	}
	if(elementInstantiated == TRUE){
		ResultListIndex++;
		if(elementIsfirst == TRUE){
			ov_string_append(&ResultFormat, "%s");
		}else{
			ov_string_append(&ResultFormat, ",%s");
		}
		elementIsfirst = FALSE;
	}

	elementInstantiated = FALSE;
	Ov_ForEachChild(ov_instantiation, pclass_cshmi_Rectangle, pObj){
#ifdef CSHMI_TIMING_DEBUG
		iterationcounter++;
#endif
		elementInstantiated = TRUE;
		ov_string_setvalue(&strIterate, cshmi_Object_getConfigAsJSON(Ov_StaticPtrCast(cshmi_Object, pObj))); //from heap mem
		if(strIterate && Ov_GetNextChild(ov_instantiation, pObj) != NULL){
			ov_string_append(&strIterate, ",");
		}
		//append does nothing when appending a NULL String
		ov_string_append(&ResultListVec[ResultListIndex], strIterate);
	}
	if(elementInstantiated == TRUE){
		ResultListIndex++;
		if(elementIsfirst == TRUE){
			ov_string_append(&ResultFormat, "%s");
		}else{
			ov_string_append(&ResultFormat, ",%s");
		}
		elementIsfirst = FALSE;
	}


	elementInstantiated = FALSE;
	Ov_ForEachChild(ov_instantiation, pclass_cshmi_Text, pObj){
#ifdef CSHMI_TIMING_DEBUG
		iterationcounter++;
#endif
		elementInstantiated = TRUE;
		ov_string_setvalue(&strIterate, cshmi_Object_getConfigAsJSON(Ov_StaticPtrCast(cshmi_Object, pObj))); //from heap mem
		if(strIterate && Ov_GetNextChild(ov_instantiation, pObj) != NULL){
			ov_string_append(&strIterate, ",");
		}
		//append does nothing when appending a NULL String
		ov_string_append(&ResultListVec[ResultListIndex], strIterate);
	}
	if(elementInstantiated == TRUE){
		ResultListIndex++;
		if(elementIsfirst == TRUE){
			ov_string_append(&ResultFormat, "%s");
		}else{
			ov_string_append(&ResultFormat, ",%s");
		}
		elementIsfirst = FALSE;
	}

	elementInstantiated = FALSE;
	Ov_ForEachChild(ov_instantiation, pclass_cshmi_Line, pObj){
#ifdef CSHMI_TIMING_DEBUG
		iterationcounter++;
#endif
		elementInstantiated = TRUE;
		ov_string_setvalue(&strIterate, cshmi_Object_getConfigAsJSON(Ov_StaticPtrCast(cshmi_Object, pObj))); //from heap mem
		if(strIterate && Ov_GetNextChild(ov_instantiation, pObj) != NULL){
			ov_string_append(&strIterate, ",");
		}
		//append does nothing when appending a NULL String
		ov_string_append(&ResultListVec[ResultListIndex], strIterate);
	}
	if(elementInstantiated == TRUE){
		ResultListIndex++;
		if(elementIsfirst == TRUE){
			ov_string_append(&ResultFormat, "%s");
		}else{
			ov_string_append(&ResultFormat, ",%s");
		}
		elementIsfirst = FALSE;
	}


	elementInstantiated = FALSE;
	Ov_ForEachChild(ov_instantiation, pclass_cshmi_Polyline, pObj){
#ifdef CSHMI_TIMING_DEBUG
		iterationcounter++;
#endif
		elementInstantiated = TRUE;
		ov_string_setvalue(&strIterate, cshmi_Object_getConfigAsJSON(Ov_StaticPtrCast(cshmi_Object, pObj))); //from heap mem
		if(strIterate && Ov_GetNextChild(ov_instantiation, pObj) != NULL){
			ov_string_append(&strIterate, ",");
		}
		//append does nothing when appending a NULL String
		ov_string_append(&ResultListVec[ResultListIndex], strIterate);
	}
	if(elementInstantiated == TRUE){
		ResultListIndex++;
		if(elementIsfirst == TRUE){
			ov_string_append(&ResultFormat, "%s");
		}else{
			ov_string_append(&ResultFormat, ",%s");
		}
		elementIsfirst = FALSE;
	}


	elementInstantiated = FALSE;
	Ov_ForEachChild(ov_instantiation, pclass_cshmi_Polygon, pObj){
#ifdef CSHMI_TIMING_DEBUG
		iterationcounter++;
#endif
		elementInstantiated = TRUE;
		ov_string_setvalue(&strIterate, cshmi_Object_getConfigAsJSON(Ov_StaticPtrCast(cshmi_Object, pObj))); //from heap mem
		if(strIterate && Ov_GetNextChild(ov_instantiation, pObj) != NULL){
			ov_string_append(&strIterate, ",");
		}
		//append does nothing when appending a NULL String
		ov_string_append(&ResultListVec[ResultListIndex], strIterate);
	}
	if(elementInstantiated == TRUE){
		ResultListIndex++;
		if(elementIsfirst == TRUE){
			ov_string_append(&ResultFormat, "%s");
		}else{
			ov_string_append(&ResultFormat, ",%s");
		}
		elementIsfirst = FALSE;
	}


	elementInstantiated = FALSE;
	Ov_ForEachChild(ov_instantiation, pclass_cshmi_Path, pObj){
#ifdef CSHMI_TIMING_DEBUG
		iterationcounter++;
#endif
		elementInstantiated = TRUE;
		ov_string_setvalue(&strIterate, cshmi_Object_getConfigAsJSON(Ov_StaticPtrCast(cshmi_Object, pObj))); //from heap mem
		if(strIterate && Ov_GetNextChild(ov_instantiation, pObj) != NULL){
			ov_string_append(&strIterate, ",");
		}
		//append does nothing when appending a NULL String
		ov_string_append(&ResultListVec[ResultListIndex], strIterate);
	}
	if(elementInstantiated == TRUE){
		ResultListIndex++;
		if(elementIsfirst == TRUE){
			ov_string_append(&ResultFormat, "%s");
		}else{
			ov_string_append(&ResultFormat, ",%s");
		}
		elementIsfirst = FALSE;
	}


	elementInstantiated = FALSE;
	Ov_ForEachChild(ov_instantiation, pclass_cshmi_Ellipse, pObj){
#ifdef CSHMI_TIMING_DEBUG
		iterationcounter++;
#endif
		elementInstantiated = TRUE;
		ov_string_setvalue(&strIterate, cshmi_Object_getConfigAsJSON(Ov_StaticPtrCast(cshmi_Object, pObj))); //from heap mem
		if(strIterate && Ov_GetNextChild(ov_instantiation, pObj) != NULL){
			ov_string_append(&strIterate, ",");
		}
		//append does nothing when appending a NULL String
		ov_string_append(&ResultListVec[ResultListIndex], strIterate);
	}
	if(elementInstantiated == TRUE){
		ResultListIndex++;
		if(elementIsfirst == TRUE){
			ov_string_append(&ResultFormat, "%s");
		}else{
			ov_string_append(&ResultFormat, ",%s");
		}
		elementIsfirst = FALSE;
	}


	//concat the result, in one batch for performance reasons (append is not cheap)
	result = ov_string_print(strResult, ResultFormat, *strResult, ResultListVec[0], ResultListVec[1], ResultListVec[2], ResultListVec[3], ResultListVec[4], ResultListVec[5], ResultListVec[6], ResultListVec[7], ResultListVec[8], ResultListVec[9], ResultListVec[10]);
	ov_string_setvalue(&ResultFormat, NULL);

	ov_string_setvalue(&temp, NULL);

	ov_string_setvalue(&strIterate, NULL);
	for(i = 0;i < MAXELEMENTENTRIES;i++){
		ov_string_setvalue(&ResultListVec[i], NULL);
	}

	return result;
}

/**
 *	builds the interactive elements (events/actions/conditions)
 * @param strResult
 * @return
 */
static OV_RESULT cshmi_downloadApplication_buildActionList(OV_STRING*strResult){
	OV_INSTPTR_ov_object pObj = NULL;

	//be careful to adjust the ov_string_print at the end of the function
	#define MAXACTIONENTRIES 13
	OV_STRING ResultListVec[MAXACTIONENTRIES] = {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL};
	OV_STRING strIterate = NULL;
	OV_UINT ResultListIndex = 0;
	OV_STRING ResultFormat = NULL;
	OV_STRING ParameterName = NULL;
	OV_STRING ParameterValue = NULL;
	OV_STRING temp = NULL;
	OV_BOOL elementInstantiated = FALSE;
	OV_BOOL elementIsfirst = TRUE;
	OV_UINT i = 0;
	OV_RESULT result = OV_ERR_OK;
	ov_string_setvalue(&ResultFormat, "%s");

#ifdef CSHMI_TIMING_DEBUG
	functioncounter++;
#endif
	/* builds something like this (without the whitespaces):
	%22/TechUnits/sheet/onload/set%22:%7B
		%22translationSource%22:%22%22,
		%22ParameterName%22:%22elemVar%22,
		%22ParameterValue%22:%22fill%22,
		%22translationSource%22:%22%22
	%7D,
	%22/TechUnits/sheet/onload/set.value%22:%7B
		%22ParameterName%22:%22value%22,
		%22ParameterValue%22:%22red%22%
	%7D
	%22/TechUnits/sheet/onload/if.if/conditionA%22:%7B
		%22Parameters%22:%7B
			%22comptype%22:%22==%22,
			%22ignoreError%22:%22FALSE%22
		%7D
	%7D
	*/

/*
	OV_VTBLPTR_cshmi_Object	pVTableObj;
	OV_STRING HeapConfigAsJSON = NULL;

    Ov_GetVTablePtr(cshmi_Object, pVTableObj, pObj);
	if(!pVTableObj) {
		HeapConfigAsJSON = pVTableObj->m_getConfigAsJSON(pObj);
	}
	*/


	elementInstantiated = FALSE;
	Ov_ForEachChild(ov_instantiation, pclass_cshmi_SetValue, pObj){
#ifdef CSHMI_TIMING_DEBUG
		iterationcounter++;
#endif
		elementInstantiated = TRUE;
		ov_string_setvalue(&strIterate, cshmi_Object_getConfigAsJSON(Ov_StaticPtrCast(cshmi_Object, pObj))); //from heap mem
		if(strIterate && Ov_GetNextChild(ov_instantiation, pObj) != NULL){
			ov_string_append(&strIterate, ",");
		}
		//append does nothing when appending a NULL String
		ov_string_append(&ResultListVec[ResultListIndex], strIterate);
	}
	if(elementInstantiated == TRUE){
		ResultListIndex++;
		if(elementIsfirst == TRUE){
			ov_string_append(&ResultFormat, "%s");
		}else{
			ov_string_append(&ResultFormat, ",%s");
		}
		elementIsfirst = FALSE;
	}

	elementInstantiated = FALSE;
	Ov_ForEachChild(ov_instantiation, pclass_cshmi_SetConcatValue, pObj){
#ifdef CSHMI_TIMING_DEBUG
		iterationcounter++;
#endif
		elementInstantiated = TRUE;
		ov_string_setvalue(&strIterate, cshmi_Object_getConfigAsJSON(Ov_StaticPtrCast(cshmi_Object, pObj))); //from heap mem
		if(strIterate && Ov_GetNextChild(ov_instantiation, pObj) != NULL){
			ov_string_append(&strIterate, ",");
		}
		//append does nothing when appending a NULL String
		ov_string_append(&ResultListVec[ResultListIndex], strIterate);
	}
	if(elementInstantiated == TRUE){
		ResultListIndex++;
		if(elementIsfirst == TRUE){
			ov_string_append(&ResultFormat, "%s");
		}else{
			ov_string_append(&ResultFormat, ",%s");
		}
		elementIsfirst = FALSE;
	}

	elementInstantiated = FALSE;
	Ov_ForEachChild(ov_instantiation, pclass_cshmi_SetMathValue, pObj){
#ifdef CSHMI_TIMING_DEBUG
		iterationcounter++;
#endif
		elementInstantiated = TRUE;
		ov_string_setvalue(&strIterate, cshmi_Object_getConfigAsJSON(Ov_StaticPtrCast(cshmi_Object, pObj))); //from heap mem
		if(strIterate && Ov_GetNextChild(ov_instantiation, pObj) != NULL){
			ov_string_append(&strIterate, ",");
		}
		//append does nothing when appending a NULL String
		ov_string_append(&ResultListVec[ResultListIndex], strIterate);
	}
	if(elementInstantiated == TRUE){
		ResultListIndex++;
		if(elementIsfirst == TRUE){
			ov_string_append(&ResultFormat, "%s");
		}else{
			ov_string_append(&ResultFormat, ",%s");
		}
		elementIsfirst = FALSE;
	}

	elementInstantiated = FALSE;
	Ov_ForEachChild(ov_instantiation, pclass_cshmi_GetValue, pObj){
#ifdef CSHMI_TIMING_DEBUG
		iterationcounter++;
#endif
		elementInstantiated = TRUE;
		ov_string_setvalue(&strIterate, cshmi_Object_getConfigAsJSON(Ov_StaticPtrCast(cshmi_Object, pObj))); //from heap mem
		if(strIterate && Ov_GetNextChild(ov_instantiation, pObj) != NULL){
			ov_string_append(&strIterate, ",");
		}
		//append does nothing when appending a NULL String
		ov_string_append(&ResultListVec[ResultListIndex], strIterate);
	}
	if(elementInstantiated == TRUE){
		ResultListIndex++;
		if(elementIsfirst == TRUE){
			ov_string_append(&ResultFormat, "%s");
		}else{
			ov_string_append(&ResultFormat, ",%s");
		}
		elementIsfirst = FALSE;
	}

	elementInstantiated = FALSE;
	Ov_ForEachChild(ov_instantiation, pclass_cshmi_ChildrenIterator, pObj){
#ifdef CSHMI_TIMING_DEBUG
		iterationcounter++;
#endif
		elementInstantiated = TRUE;
		ov_string_setvalue(&strIterate, cshmi_Object_getConfigAsJSON(Ov_StaticPtrCast(cshmi_Object, pObj))); //from heap mem
		if(strIterate && Ov_GetNextChild(ov_instantiation, pObj) != NULL){
			ov_string_append(&strIterate, ",");
		}
		//append does nothing when appending a NULL String
		ov_string_append(&ResultListVec[ResultListIndex], strIterate);
	}
	if(elementInstantiated == TRUE){
		ResultListIndex++;
		if(elementIsfirst == TRUE){
			ov_string_append(&ResultFormat, "%s");
		}else{
			ov_string_append(&ResultFormat, ",%s");
		}
		elementIsfirst = FALSE;
	}

	elementInstantiated = FALSE;
	Ov_ForEachChild(ov_instantiation, pclass_cshmi_IfThenElse, pObj){
#ifdef CSHMI_TIMING_DEBUG
		iterationcounter++;
#endif
		elementInstantiated = TRUE;
		ov_string_setvalue(&strIterate, cshmi_Object_getConfigAsJSON(Ov_StaticPtrCast(cshmi_Object, pObj))); //from heap mem
		if(strIterate && Ov_GetNextChild(ov_instantiation, pObj) != NULL){
			ov_string_append(&strIterate, ",");
		}
		//append does nothing when appending a NULL String
		ov_string_append(&ResultListVec[ResultListIndex], strIterate);
	}
	if(elementInstantiated == TRUE){
		ResultListIndex++;
		if(elementIsfirst == TRUE){
			ov_string_append(&ResultFormat, "%s");
		}else{
			ov_string_append(&ResultFormat, ",%s");
		}
		elementIsfirst = FALSE;
	}

	elementInstantiated = FALSE;
	Ov_ForEachChild(ov_instantiation, pclass_cshmi_Compare, pObj){
#ifdef CSHMI_TIMING_DEBUG
		iterationcounter++;
#endif
		elementInstantiated = TRUE;
		ov_string_setvalue(&strIterate, cshmi_Object_getConfigAsJSON(Ov_StaticPtrCast(cshmi_Object, pObj))); //from heap mem
		if(strIterate && Ov_GetNextChild(ov_instantiation, pObj) != NULL){
			ov_string_append(&strIterate, ",");
		}
		//append does nothing when appending a NULL String
		ov_string_append(&ResultListVec[ResultListIndex], strIterate);
	}
	if(elementInstantiated == TRUE){
		ResultListIndex++;
		if(elementIsfirst == TRUE){
			ov_string_append(&ResultFormat, "%s");
		}else{
			ov_string_append(&ResultFormat, ",%s");
		}
		elementIsfirst = FALSE;
	}

	elementInstantiated = FALSE;
	Ov_ForEachChild(ov_instantiation, pclass_cshmi_CompareIteratedChild, pObj){
#ifdef CSHMI_TIMING_DEBUG
		iterationcounter++;
#endif
		elementInstantiated = TRUE;
		ov_string_setvalue(&strIterate, cshmi_Object_getConfigAsJSON(Ov_StaticPtrCast(cshmi_Object, pObj))); //from heap mem
		if(strIterate && Ov_GetNextChild(ov_instantiation, pObj) != NULL){
			ov_string_append(&strIterate, ",");
		}
		//append does nothing when appending a NULL String
		ov_string_append(&ResultListVec[ResultListIndex], strIterate);
	}
	if(elementInstantiated == TRUE){
		ResultListIndex++;
		if(elementIsfirst == TRUE){
			ov_string_append(&ResultFormat, "%s");
		}else{
			ov_string_append(&ResultFormat, ",%s");
		}
		elementIsfirst = FALSE;
	}

	elementInstantiated = FALSE;
	Ov_ForEachChild(ov_instantiation, pclass_cshmi_TimeEvent, pObj){
#ifdef CSHMI_TIMING_DEBUG
		iterationcounter++;
#endif
		elementInstantiated = TRUE;
		ov_string_setvalue(&strIterate, cshmi_Object_getConfigAsJSON(Ov_StaticPtrCast(cshmi_Object, pObj))); //from heap mem
		if(strIterate && Ov_GetNextChild(ov_instantiation, pObj) != NULL){
			ov_string_append(&strIterate, ",");
		}
		//append does nothing when appending a NULL String
		ov_string_append(&ResultListVec[ResultListIndex], strIterate);
	}
	if(elementInstantiated == TRUE){
		ResultListIndex++;
		if(elementIsfirst == TRUE){
			ov_string_append(&ResultFormat, "%s");
		}else{
			ov_string_append(&ResultFormat, ",%s");
		}
		elementIsfirst = FALSE;
	}

	elementInstantiated = FALSE;
	Ov_ForEachChild(ov_instantiation, pclass_cshmi_RoutePolyline, pObj){
#ifdef CSHMI_TIMING_DEBUG
		iterationcounter++;
#endif
		elementInstantiated = TRUE;
		ov_string_setvalue(&strIterate, cshmi_Object_getConfigAsJSON(Ov_StaticPtrCast(cshmi_Object, pObj))); //from heap mem
		if(strIterate && Ov_GetNextChild(ov_instantiation, pObj) != NULL){
			ov_string_append(&strIterate, ",");
		}
		//append does nothing when appending a NULL String
		ov_string_append(&ResultListVec[ResultListIndex], strIterate);
	}
	if(elementInstantiated == TRUE){
		ResultListIndex++;
		if(elementIsfirst == TRUE){
			ov_string_append(&ResultFormat, "%s");
		}else{
			ov_string_append(&ResultFormat, ",%s");
		}
		elementIsfirst = FALSE;
	}

	elementInstantiated = FALSE;
	Ov_ForEachChild(ov_instantiation, pclass_cshmi_TranslationSource, pObj){
#ifdef CSHMI_TIMING_DEBUG
		iterationcounter++;
#endif
		elementInstantiated = TRUE;
		ov_string_setvalue(&strIterate, cshmi_Object_getConfigAsJSON(Ov_StaticPtrCast(cshmi_Object, pObj))); //from heap mem
		if(strIterate && Ov_GetNextChild(ov_instantiation, pObj) != NULL){
			ov_string_append(&strIterate, ",");
		}
		//append does nothing when appending a NULL String
		ov_string_append(&ResultListVec[ResultListIndex], strIterate);
	}
	if(elementInstantiated == TRUE){
		ResultListIndex++;
		if(elementIsfirst == TRUE){
			ov_string_append(&ResultFormat, "%s");
		}else{
			ov_string_append(&ResultFormat, ",%s");
		}
		elementIsfirst = FALSE;
	}

	elementInstantiated = FALSE;
	Ov_ForEachChild(ov_instantiation, pclass_cshmi_CreateObject, pObj){
#ifdef CSHMI_TIMING_DEBUG
		iterationcounter++;
#endif
		elementInstantiated = TRUE;
		ov_string_setvalue(&strIterate, cshmi_Object_getConfigAsJSON(Ov_StaticPtrCast(cshmi_Object, pObj))); //from heap mem
		if(strIterate && Ov_GetNextChild(ov_instantiation, pObj) != NULL){
			ov_string_append(&strIterate, ",");
		}
		//append does nothing when appending a NULL String
		ov_string_append(&ResultListVec[ResultListIndex], strIterate);
	}
	if(elementInstantiated == TRUE){
		ResultListIndex++;
		if(elementIsfirst == TRUE){
			ov_string_append(&ResultFormat, "%s");
		}else{
			ov_string_append(&ResultFormat, ",%s");
		}
		elementIsfirst = FALSE;
	}

	elementInstantiated = FALSE;
	Ov_ForEachChild(ov_instantiation, pclass_cshmi_Vibrate, pObj){
#ifdef CSHMI_TIMING_DEBUG
		iterationcounter++;
#endif
		elementInstantiated = TRUE;
		ov_string_setvalue(&strIterate, cshmi_Object_getConfigAsJSON(Ov_StaticPtrCast(cshmi_Object, pObj))); //from heap mem
		if(strIterate && Ov_GetNextChild(ov_instantiation, pObj) != NULL){
			ov_string_append(&strIterate, ",");
		}
		//append does nothing when appending a NULL String
		ov_string_append(&ResultListVec[ResultListIndex], strIterate);
	}
	if(elementInstantiated == TRUE){
		ResultListIndex++;
		if(elementIsfirst == TRUE){
			ov_string_append(&ResultFormat, "%s");
		}else{
			ov_string_append(&ResultFormat, ",%s");
		}
		elementIsfirst = FALSE;
	}

	//concat the result, manual for performance reasons (append is not cheap)
	result = ov_string_print(strResult, ResultFormat, *strResult, ResultListVec[0], ResultListVec[1], ResultListVec[2], ResultListVec[3], ResultListVec[4], ResultListVec[5], ResultListVec[6], ResultListVec[7], ResultListVec[8], ResultListVec[9], ResultListVec[10], ResultListVec[11], ResultListVec[12]);
	ov_string_setvalue(&ResultFormat, NULL);
	ov_string_setvalue(&ParameterName, NULL);
	ov_string_setvalue(&ParameterValue, NULL);
	ov_string_setvalue(&temp, NULL);

	ov_string_setvalue(&strIterate, NULL);
	for(i = 0;i < MAXACTIONENTRIES;i++){
		ov_string_setvalue(&ResultListVec[i], NULL);
	}

	return result;
}


/**
 * Tools:
 * http://meyerweb.com/eric/tools/dencoder/
 * http://jsonviewer.stack.hu/
 * http://jsonlint.com/
 */

/**
 *
 */
OV_DLLFNCEXPORT OV_STRING cshmi_downloadApplication_asJSON_get(
		OV_INSTPTR_cshmi_downloadApplication        UNUSED pThis
) {
	OV_INSTPTR_ov_object pTUcshmi = NULL;
	OV_INSTPTR_cshmi_Group pGroup = NULL;
	OV_INSTPTR_ov_object pGroupParent = NULL;
	OV_STRING returnString = NULL;
	OV_STRING strBaseKsPath = NULL;
	OV_STRING strElements = NULL;
	OV_STRING strActions = NULL;
	OV_STRING strChildList = NULL;
	OV_STRING pJSON = NULL;
	OV_UINT lenBaseKsPath = 0;
	OV_UINT lenElements = 0;
	OV_UINT lenActions = 0;
	OV_UINT lenChildList = 0;
	OV_RESULT result = OV_ERR_OK;

#ifdef CSHMI_TIMING_DEBUG
	OV_UINT lenSubChildList = 0;
	OV_TIME lasttime;
	OV_TIME thistime;
	OV_TIME_SPAN diff;

	functioncounter = 0;
	iterationcounter = 0;
#endif

	/**
	 * Umsetzungstabelle f�r decodeURI
	 * " %22
	 * { %7B
	 * } %7D
	 * [ %5B
	 * ] %5D
	 * ( (
	 * ) )
	 * + +
	 */

	pTUcshmi = ov_path_getobjectpointer("/TechUnits/cshmi", 2);
	if(pTUcshmi == NULL || !Ov_CanCastTo(ov_domain, pTUcshmi)){
		return (OV_STRING) 0;
	}

	//todo
	//some static baseKsPath settings
	result = ov_string_setvalue(&strBaseKsPath, "%22baseKsPath%22:%7B%22/TechUnits/cshmi%22:%5B%5D,%22/TechUnits/cshmi/Templates%22:%5B%5D%7D,");
	lenBaseKsPath = ov_string_getlength(strBaseKsPath);

#ifdef CSHMI_TIMING_DEBUG
	ov_time_gettime(&lasttime);
#endif

	//Elements is a list of configured elements
	ov_string_setvalue(&strElements, "%22Elements%22:%7B");
	result = cshmi_downloadApplication_buildElementList(&strElements);
	if(Ov_Fail(result)){
		ov_logfile_debug("%d:%s Error building Elementlist: %s", __LINE__, __FILE__, ov_result_getresulttext(result));
		ov_string_setvalue(&strBaseKsPath, NULL);
		ov_string_setvalue(&strElements, NULL);
		return (OV_STRING) 0;
	}else{
		ov_string_append(&strElements, "%7D,");
	}
	lenElements = ov_string_getlength(strElements);

#ifdef CSHMI_TIMING_DEBUG
	ov_time_gettime(&thistime);
	ov_time_diff(&diff, &thistime, &lasttime);
	ov_logfile_debug("Time: %s", ov_time_timetoascii_local(&thistime));
	ov_logfile_debug("Elements    : %s (%u Bytes, %u function calls, %u iterations)", ov_time_timespantoascii(&diff), lenElements, functioncounter, iterationcounter);
	ov_time_gettime(&lasttime);
	functioncounter = 0;
	iterationcounter = 0;
#endif

	//Actions is a list of events/actions/conditions, set/get stores the result only
	ov_string_setvalue(&strActions, "%22Actions%22:%7B");
	result = cshmi_downloadApplication_buildActionList(&strActions);

	if(Ov_Fail(result)){
		ov_logfile_debug("%d:%s Error building Actionlist: %s", __LINE__, __FILE__, ov_result_getresulttext(result));
		ov_string_setvalue(&strBaseKsPath, NULL);
		ov_string_setvalue(&strElements, NULL);
		ov_string_setvalue(&strActions, NULL);
		return (OV_STRING) 0;
	}else{
		ov_string_append(&strActions, "%7D,");
	}
	lenActions = ov_string_getlength(strActions);

#ifdef CSHMI_TIMING_DEBUG
	ov_time_gettime(&thistime);
	ov_time_diff(&diff, &thistime, &lasttime);
	ov_logfile_debug("Actions     : %s (%u Bytes, %u function calls, %u iterations)", ov_time_timespantoascii(&diff), lenActions, functioncounter, iterationcounter);
	ov_time_gettime(&lasttime);
	functioncounter = 0;
	iterationcounter = 0;
#endif

	//ChildList is a list of ov_containment
	ov_string_setvalue(&strChildList, "%22ChildList%22:%7B");
	result = cshmi_downloadApplication_buildChildList(pTUcshmi, &strChildList, TRUE);

#ifdef CSHMI_TIMING_DEBUG
	lenSubChildList = ov_string_getlength(strChildList);

	ov_time_gettime(&thistime);
	ov_time_diff(&diff, &thistime, &lasttime);
	ov_logfile_debug("Childlist CS: %s (%u Bytes, %u function calls, %u iterations)", ov_time_timespantoascii(&diff), lenSubChildList, functioncounter, iterationcounter);
	ov_time_gettime(&lasttime);
	functioncounter = 0;
	iterationcounter = 0;
#endif

	//todo 2mbyte mb scheint f�r den engineering testcase zu klein. also vergr��ern
	//�ndern in memstack, also heap
	if(Ov_Fail(result)){
		ov_logfile_debug("%d:%s Error building Childlist: %s", __LINE__, __FILE__, ov_result_getresulttext(result));
		ov_string_setvalue(&strBaseKsPath, NULL);
		ov_string_setvalue(&strElements, NULL);
		ov_string_setvalue(&strActions, NULL);
		ov_string_setvalue(&strChildList, NULL);
		return (OV_STRING) 0;
	}
	//check for other groups outside of /TechUnits/cshmi
	Ov_ForEachChildEx(ov_instantiation, pclass_cshmi_Group, pGroup, cshmi_Group){
		pGroupParent = Ov_PtrUpCast(ov_object, Ov_GetParent(ov_containment, pGroup));
		if(!Ov_CanCastTo(cshmi_Object, pGroupParent) && pGroupParent != pTUcshmi){
			result = cshmi_downloadApplication_buildChildList(Ov_PtrUpCast(ov_object, pGroup), &strChildList, FALSE);
			if(Ov_Fail(result)){
				ov_logfile_debug("%d:%s Error building Childlist: %s", __LINE__, __FILE__, ov_result_getresulttext(result));
				ov_string_setvalue(&strBaseKsPath, NULL);
				ov_string_setvalue(&strElements, NULL);
				ov_string_setvalue(&strActions, NULL);
				ov_string_setvalue(&strChildList, NULL);
				return (OV_STRING) 0;
			}
		}
	}
	ov_string_append(&strChildList, "%7D");

	lenChildList = ov_string_getlength(strChildList);

#ifdef CSHMI_TIMING_DEBUG
	ov_time_gettime(&thistime);
	ov_time_diff(&diff, &thistime, &lasttime);
	ov_logfile_debug("Childlist TU: %s (%u Bytes, %u function calls, %u iterations)", ov_time_timespantoascii(&diff), lenChildList-lenSubChildList, functioncounter, iterationcounter);
	ov_time_gettime(&lasttime);
	functioncounter = 0;
	iterationcounter = 0;
#endif

	//we need space for all 4 strings, two times 3 Bytes and a null byte terminator
	pJSON = (OV_STRING) ov_memstack_alloc(lenBaseKsPath+lenElements+lenActions+lenChildList+7);

	if (!pJSON){
		ov_logfile_debug("%d:%s Error reserving memory for concatting result: MEMSTACK full", __LINE__, __FILE__);
		ov_string_setvalue(&strBaseKsPath, NULL);
		ov_string_setvalue(&strActions, NULL);
		ov_string_setvalue(&strChildList, NULL);
		ov_string_setvalue(&strElements, NULL);
		return (OV_STRING) 0;
	}

	returnString = pJSON;
	//open JSON Element, concat Strings and close JSON Element
	strncpy(pJSON, "%7B", 3);
	pJSON = pJSON + 3;
	strncpy(pJSON, strBaseKsPath, lenBaseKsPath);
	pJSON = pJSON + lenBaseKsPath;
	strncpy(pJSON, strElements, lenElements);
	pJSON = pJSON + lenElements;
	strncpy(pJSON, strActions, lenActions);
	pJSON = pJSON + lenActions;
	strncpy(pJSON, strChildList, lenChildList);
	pJSON = pJSON + lenChildList;
	strncpy(pJSON, "%7D", 4);

	ov_string_setvalue(&strBaseKsPath, NULL);
	ov_string_setvalue(&strActions, NULL);
	ov_string_setvalue(&strChildList, NULL);
	ov_string_setvalue(&strElements, NULL);

#ifdef CSHMI_TIMING_DEBUG
	ov_time_gettime(&thistime);
	ov_time_diff(&diff, &thistime, &lasttime);
	ov_logfile_debug("strconcat   : %s", ov_time_timespantoascii(&diff));
	ov_time_gettime(&lasttime);
#endif

	return returnString;
}
