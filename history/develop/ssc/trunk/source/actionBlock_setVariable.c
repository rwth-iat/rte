/*
*   The Framework was generated by the ACPLT/OV Builder.
*
*   Builder version: 1.0.4
*   Date of file generation:  15-Jul-2011 (13:34:12)
*/

#ifndef OV_COMPILE_LIBRARY_ssc
#define OV_COMPILE_LIBRARY_ssc
#endif

#include "ssclib.h"

/**
 * splits ths inputstring at the last seperator and writes the parts in two strings
 */
static void extractBehindSeperator(const OV_STRING inputstring, OV_STRING *pathname, OV_STRING *varname, char seperator){
	OV_UINT len = 0;
	OV_UINT i = 0;

	if(!inputstring){
		return;
	}
	ov_string_setvalue(pathname, inputstring);
	len = ov_string_getlength(inputstring);	//todo iterate backwards, for speed :-)
	for(i = 0 ; i<len-1 ; i++){
		if(inputstring[len-i] == seperator){
			ov_string_setvalue(varname, &inputstring[len-i+1]);
			(*pathname)[len-i] = '\0';
			break;
		}
	}
	return;
}

/**
 * gets a object pointer from a relative path name and an anchor path
 * @param startPoint
 * @param pathname
 * @return
 */
OV_INSTPTR_ov_object getrelativeobjectpointer(
		const OV_STRING			startPoint,
		const OV_STRING			pathname
) {
	/*
	 *	local variables
	 */
	OV_PATH		path;
	OV_ELEMENT	element;
	OV_PATH     startPointPath;
	ov_memstack_lock();
	ov_path_resolve(&startPointPath, NULL, startPoint, 2);

	/*
	 *	instructions
	 */
	if(Ov_Fail(ov_path_resolve(&path, &startPointPath, pathname, 2))) {
		//	bad path
		ov_memstack_unlock();
		return NULL;
	}
	element = path.elements[path.size-1];

	ov_memstack_unlock();
	if(element.elemtype == OV_ET_OBJECT) {
		return element.pobj;
	}
	//	not an object path
	return NULL;
}

/**
 * gets an object pointer and a varname string from a setVariable object
 * @param pinst
 * @param pTargetObj will be NULL if object is not found
 * @param ptargetVarname will be NULL if variable not found on the object
 * @return
 */
static OV_RESULT ssc_getObjectAndVarnameFromSetVariable(
		const OV_INSTPTR_ssc_setVariable pinst,
		const OV_STRING nameToCheck,
		OV_INSTPTR_ov_object *pTargetObj,
		OV_STRING *ptargetVarname
) {
	OV_STRING targetPathname = NULL;
	OV_STRING pathRelativeobject = NULL;
	OV_INSTPTR_ssc_step pStep = Ov_DynamicPtrCast(ssc_step, Ov_GetParent(ov_containment, pinst));
	OV_INSTPTR_ssc_sscHeader activeHeader = Ov_DynamicPtrCast(ssc_sscHeader, Ov_GetParent(ov_containment, pStep));
	OV_INSTPTR_ov_domain containerDomain = NULL;

	*pTargetObj = NULL;
	//split the input at dot
	extractBehindSeperator(nameToCheck, &targetPathname, ptargetVarname, '.');

	if(targetPathname == NULL){
		return OV_ERR_BADPARAM;
	}else if(targetPathname[0] == '/'){
		//we have a full path
		*pTargetObj = ov_path_getobjectpointer(targetPathname, 2);
	}else{
		//we have a relative path
		ov_memstack_lock();
		//all path are relative to the activeHeader
		containerDomain = Ov_PtrUpCast(ov_domain, activeHeader);
		ov_string_setvalue(&pathRelativeobject, ov_path_getcanonicalpath(Ov_PtrUpCast(ov_object, containerDomain), 2));
		*pTargetObj = getrelativeobjectpointer(pathRelativeobject, targetPathname);
		if(*pTargetObj == NULL){
			//perhaps the target is in the actions PART of the header
			containerDomain = Ov_GetPartPtr(actions, activeHeader);
			ov_string_setvalue(&pathRelativeobject, ov_path_getcanonicalpath(Ov_PtrUpCast(ov_object, containerDomain), 2));
			*pTargetObj = getrelativeobjectpointer(pathRelativeobject, targetPathname);
		}
		ov_memstack_unlock();
	}
	ov_string_setvalue(&targetPathname, NULL);
	ov_string_setvalue(&pathRelativeobject,NULL);

	return OV_ERR_OK;
}

/**
 * checks and sets the new target path
 * an empty string is the only allowed not working parameter
 */
OV_DLLFNCEXPORT OV_RESULT ssc_setVariable_variable_set(
	OV_INSTPTR_ssc_setVariable          pinst,
	const OV_STRING  value
) {
	OV_INSTPTR_ov_object pTargetObj = NULL;
	OV_STRING targetVarname = NULL;

	// check input
	if(ov_string_compare(value, "") == OV_STRCMP_EQUAL){
		//allow INITIAL_VALUE for loading an fbd backup
		return ov_string_setvalue(&pinst->v_variable, value);
	}

	ssc_getObjectAndVarnameFromSetVariable(pinst, value, &pTargetObj, &targetVarname);
	if(pTargetObj == NULL || targetVarname == NULL){
		return OV_ERR_BADPARAM;
	}

	pinst->v_error = FALSE;
	ov_string_setvalue(&pinst->v_errorDetail, NULL);

	return ov_string_setvalue(&pinst->v_variable, value);
}

/**
 * setting the new value
 */
OV_DLLFNCEXPORT OV_RESULT ssc_setVariable_value_set(
		OV_INSTPTR_ssc_setVariable          pobj,
		const OV_ANY*  value
) {
	return ov_variable_setanyvalue(&pobj->v_value, value);
}

/**
 * set a variable with a pointer to the object and the name of the variable (correct handling for chart variables)
 * @param pTargetObj
 * @param targetVarname
 * @param value
 * @return
 */
OV_RESULT ssc_setNamedVariable(const OV_INSTPTR_ov_object pTargetObj, const OV_STRING targetVarname, OV_ANY *value){
	OV_RESULT result = OV_ERR_OK;
	OV_ELEMENT element;
	OV_ELEMENT varElement;
	OV_VTBLPTR_ov_object pVtblObj = NULL;

	if(pTargetObj == NULL){
			return OV_ERR_BADPARAM;
	}else if (Ov_CanCastTo(fb_functionchart, pTargetObj)){
		//set variable in a functionchart
		result = fb_functionchart_setport(Ov_StaticPtrCast(fb_functionchart, pTargetObj), targetVarname, value);
	}else{
		//set variable in a object
		varElement.elemtype = OV_ET_NONE;
		element.elemtype = OV_ET_OBJECT;
		element.pobj = pTargetObj;

		//search the variable for the set operation
		ov_element_searchpart(&element, &varElement, OV_ET_VARIABLE, targetVarname);
		if(varElement.elemtype == OV_ET_VARIABLE) {
			//port found, use the setter to write the value
			Ov_GetVTablePtr(ov_object, pVtblObj, pTargetObj);
			result = pVtblObj->m_setvar(varElement.pobj, &varElement, value);
		}
	}

	return result;
}


OV_DLLFNCEXPORT void ssc_setVariable_typemethod(
	OV_INSTPTR_fb_functionblock	pfb,
	OV_TIME						*pltc
) {
	// local variables
	OV_INSTPTR_ssc_setVariable pinst = Ov_StaticPtrCast(ssc_setVariable, pfb);
	OV_INSTPTR_ssc_step pStep = Ov_DynamicPtrCast(ssc_step, Ov_GetParent(ov_containment, pinst));
	OV_RESULT    result = OV_ERR_OK;
	OV_INSTPTR_ov_object pTargetObj = NULL;
	OV_STRING targetVarname = NULL;


	// init variables
	pinst->v_cyctime.secs = 0;
	pinst->v_cyctime.usecs = 0;
	pinst->v_iexreq = TRUE;
	pinst->v_error=FALSE;

	ov_string_setvalue(&pinst->v_errorDetail, NULL);

	// check location
	if (pStep == NULL){
		pinst->v_error = TRUE;
		ov_logfile_error("ssc_actionBlock_typemethod: action block must be encapsulated in a step.");
		return;
	}

	// check input
	if (ov_string_compare(pinst->v_variable, NULL) == OV_STRCMP_EQUAL){
		pinst->v_error=TRUE;
		ov_string_setvalue(&pinst->v_errorDetail, "variable is not defined.");
	};

	ssc_getObjectAndVarnameFromSetVariable(pinst, pinst->v_variable, &pTargetObj, &targetVarname);

	if(pTargetObj != NULL && targetVarname != NULL){
		result = ssc_setNamedVariable(pTargetObj, targetVarname, &(pinst->v_value));
	}else{
		pinst->v_error = TRUE;
		ov_string_setvalue(&pinst->v_errorDetail, "path not found");
	}

	if(Ov_Fail(result)){
		pinst->v_error=TRUE;
		ov_string_setvalue(&pinst->v_errorDetail, "bad parameter");
	}

	ov_string_setvalue(&targetVarname,NULL);

	return;
}
