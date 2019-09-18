/*
 *   $Id: fbddump.c, v1.0 2018-6-10$
 *
 *   Copyright (C) 1998
 *   Lehrstuhl fuer Prozessleittechnik,
 *   D-52056 Aachen, Germany.
 *   All rights reserved.
 *
 *   Author: Lars Nothdurft <l.nothdurft@plt.rwth-aachen.de>
 *
 *   This file is part of the ACPLT/OV Package
 *   Licensed under the Apache License, Version 2.0 (the "License");
 *   you may not use this file except in compliance with the License.
 *   You may obtain a copy of the License at
 *
 *       http://www.apache.org/licenses/LICENSE-2.0
 *
 *   Unless required by applicable law or agreed to in writing, software
 *   distributed under the License is distributed on an "AS IS" BASIS,
 *   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *   See the License for the specific language governing permissions and
 *   limitations under the License.
 *
 */
/*
 *	History:
 *	--------
 *	10-Jun-2018 Lars Nothdurft <l.nothdurft@plt.rwth-aachen.de>: File created.
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/stat.h>

#include "libov/ov_database.h"
#include "libov/ov_result.h"
#include "libov/ov_logfile.h"
#include "libov/ov_macros.h"
#include "libov/ov_path.h"
#include "libov/ov_config.h"
#include "libov/ov_variable.h"

#include "ov_dbutil.h"

OV_UINT ov_sizeof(OV_UINT vartype);
void ov_newline(FILE* handle,int rnum);

static void ov_instanceoutput(OV_INSTPTR_ov_object pobj, FILE *handle, int rnum);

static OV_INSTPTR_ov_domain pRoot = NULL;
static OV_INSTPTR_ov_object pTasks = NULL;
static OV_INSTPTR_ov_object pComm = NULL;
static OV_INSTPTR_ov_object pData = NULL;
static OV_INT includeMode = 0;
/*	----------------------------------------------------------------------	*/

/*
 * check if object is in path of another object
 */
static OV_BOOL ov_isPathParent(
	OV_INSTPTR_ov_object pPartent,
	OV_INSTPTR_ov_object pChild
){
	OV_INSTPTR_ov_domain pDom = NULL;
	OV_INSTPTR_ov_object pObj = pChild;

	if(!Ov_CanCastTo(ov_domain, pPartent))
		return FALSE;

	if(pPartent==pChild)
		return TRUE;

	while(pObj){
		if(pObj == pPartent)
			return TRUE;
		if(!pObj->v_pouterobject){
			pDom = Ov_GetParent(ov_containment, pObj);
			pObj = Ov_PtrUpCast(ov_object, pDom);
		} else { // is part; get outer object pointer
			pObj = pObj->v_pouterobject;
		}
	}
	return FALSE;
}

/*
 * filter function to export only needed links
 */
static OV_BOOL ov_linkFilterPass(OV_INSTPTR_ov_object pobj){
	if(!pobj)
		return FALSE;
	if(pRoot==&pdb->root && includeMode)
		return TRUE;
	if(!includeMode && (ov_isPathParent(pComm, pobj) || ov_isPathParent(pData, pobj)))
		return FALSE;
	if(!ov_isPathParent(Ov_PtrUpCast(ov_object, pRoot), pobj) &&
		!ov_isPathParent(pTasks, pobj)) // include /Tasks to preserve connections to UrTask
		return FALSE;
	return TRUE;
}

/*
 * determine if variable is INPUT or OUTPUT
 */
static void ov_flagoutput(
	OV_ELEMENT* pelement,
	FILE* handle
){
	OV_INSTPTR_ov_variable pvar = pelement->elemunion.pvar;
	const OV_UINT inputFlags=(1<<('i'-'a'))|(1<<('p'-'a'));

	if((pvar->v_varprops&OV_VP_SETACCESSOR) || (pvar->v_flags&inputFlags)){
		fprintf(handle, "INPUT");
	} else {
		fprintf(handle, "OUTPUT");
	}

}

/*
 * print type of variable
 */
static void ov_typeoutput(
	OV_BYTE* pvalue,
	OV_UINT vartype,
	OV_UINT veclen,
	FILE* handle
){

	if((vartype&OV_VT_KSMASK)==OV_VT_ANY)
		vartype = ((OV_ANY*)pvalue)->value.vartype;

	switch(vartype&OV_VT_KSMASK&~OV_VT_ISVECTOR){
	case OV_VT_BOOL:
		fprintf(handle, "BOOL");
		break;
	case OV_VT_BYTE:
		fprintf(handle, "BYTE");
		break;
	case OV_VT_INT:
		fprintf(handle, "INT");
		break;
	case OV_VT_UINT:
		fprintf(handle, "UINT");
		break;
	case OV_VT_SINGLE:
		fprintf(handle, "SINGLE");
		break;
	case OV_VT_STRING:
		fprintf(handle, "STRING");
		break;
	case OV_VT_DOUBLE:
		fprintf(handle, "DOUBLE");
		break;
	case OV_VT_TIME:
		fprintf(handle, "TIME");
		break;
	case OV_VT_TIME_SPAN:
		fprintf(handle, "TIME_SPAN");
		break;
	}
}

/*
 * escape '"' in string
 */
static OV_STRING ov_escapeString(const OV_STRING pStr){

	OV_STRING tmpStr = NULL;
	char *ph, *ph2;

	if(pStr){
		tmpStr = ov_memstack_alloc(2*strlen(pStr)+1);
		if(!tmpStr)
			return NULL;

		for(ph = pStr, ph2 = tmpStr; *ph; ph++, ph2++){
			switch(*ph){
			case '"':
				*ph2++ = '\\';
				*ph2 = '"';
				break;
			default:
				*ph2=*ph;
			}
		}
		*ph2 = *ph;
	}
	return tmpStr;
}

/*
 *	single variable value output
 */
static void ov_basevalueoutput(
		OV_UINT vartype,
		OV_BYTE* pvalue,
		FILE *handle
){

	OV_STRING		stringtime;
	OV_BOOL			b;
	OV_BYTE			byte;
	OV_INT			i;
	OV_UINT			u;
	OV_SINGLE		s;
	OV_DOUBLE		d;
	OV_STRING		st;
	OV_TIME			t;
	OV_TIME_SPAN	tsp;

	if(!pvalue){
		fprintf(handle, "(null)");
		return;
	}

	switch (vartype){
	case OV_VT_VOID:
		fprintf(handle, "<void>");
		break;
	case OV_VT_BOOL:
		b = *(OV_BOOL*)(pvalue);
		if (b == 0) fprintf(handle, "FALSE");
		else fprintf(handle, "TRUE");
		break;
	case OV_VT_INT:
		i = *(OV_INT*)(pvalue);
		fprintf(handle, "%" OV_PRINT_INT, i);
		break;
	case OV_VT_UINT:
		u = *(OV_UINT*)(pvalue);
		fprintf(handle, "%" OV_PRINT_UINT, u);
		break;
	case OV_VT_SINGLE:
		s = *(OV_SINGLE*)(pvalue);
		fprintf(handle, "%" OV_PRINT_SINGLE, s);
		break;
	case OV_VT_DOUBLE:
		d = *(OV_DOUBLE*)(pvalue);
		fprintf(handle, "%" OV_PRINT_DOUBLE, d);
		break;
	case OV_VT_BYTE:
		byte =*(OV_BYTE*)(pvalue);
		fprintf(handle, "%d", byte);
		break;
	case OV_VT_STRING:
		ov_memstack_lock();
		st = ov_escapeString(*(OV_STRING*)(pvalue));
		if(st){
			fprintf(handle, "\"%s\"", st);
		} else {
			fprintf(handle, "\"\"");
		}
		ov_memstack_unlock();
		break;
	case OV_VT_TIME:
		t = *(OV_TIME*)(pvalue);
		stringtime = ov_time_timetoascii(&t);
		fprintf(handle, "%s", stringtime);
		break;
	case OV_VT_TIME_SPAN:
		tsp = *(OV_TIME_SPAN*)(pvalue);
		Ov_TimeToDouble(tsp, d);
		fprintf(handle, "%.6" OV_PRINT_DOUBLE , d);
		break;
	}
}

/*
 *	variable value output
 */
static void ov_valueoutput(
		OV_ANY*		pany,
		FILE 		*handle
)
{
	OV_UINT 	veclength;
	OV_UINT		vartype = pany->value.vartype&OV_VT_KSMASK;
	OV_UINT 	z;
	OV_BYTE 	*pval;

	switch(vartype){
	case OV_VT_BOOL:
	case OV_VT_BYTE:
	case OV_VT_INT:
	case OV_VT_UINT:
	case OV_VT_SINGLE:
	case OV_VT_STRING:
	case OV_VT_DOUBLE:
	case OV_VT_TIME:
	case OV_VT_TIME_SPAN:
		ov_basevalueoutput(vartype, &pany->value.valueunion.val_byte, handle);
		fprintf(handle, ";");
		break;
	case OV_VT_BOOL_VEC:
	case OV_VT_BYTE_VEC:
	case OV_VT_INT_VEC:
	case OV_VT_UINT_VEC:
	case OV_VT_DOUBLE_VEC:
	case OV_VT_SINGLE_VEC:
	case OV_VT_STRING_VEC:
	case OV_VT_TIME_VEC:
	case OV_VT_TIME_SPAN_VEC:
		fprintf(handle, "{ ");
		/* dynamische Vektoren*/
			veclength = pany->value.valueunion.val_generic_vec.veclen;
			pval = pany->value.valueunion.val_generic_vec.value;
			for (z=0; z<veclength; z++){
				ov_basevalueoutput(vartype & 0x7f, pval, handle);
				pval = pval + ov_sizeof(vartype & 0x7f);
				if ((z+1) < veclength) fprintf(handle, " , ");
			}
		fprintf(handle, " };");
		break;

	}

}

/*
 *	variable output
 */
static void ov_processVariables(
		OV_INSTPTR_ov_object pobj,
		FILE *handle,
		int rnum
){
	OV_ELEMENT	parentelem;
	OV_ELEMENT	partelem;
	OV_UINT		arraysize = 1;
	OV_ANY		any = {.value.vartype=OV_VT_VOID};
	OV_BOOL		header = FALSE;

	parentelem.elemtype = OV_ET_OBJECT;
	parentelem.pobj = pobj;
	partelem.elemtype = OV_ET_NONE;
	partelem.pobj = NULL;

	ov_element_getnextpart(&parentelem, &partelem, OV_ET_VARIABLE);
	while (partelem.elemtype != OV_ET_NONE) {
		if (((partelem.elemunion.pvar->v_vartype & OV_VT_CTYPE) != OV_VT_CTYPE) &&
				strcmp(partelem.elemunion.pvar->v_identifier, "identifier") &&
				strcmp(partelem.elemunion.pvar->v_identifier, "creationtime") &&
				strcmp(partelem.elemunion.pvar->v_identifier, "objectstate") &&
				strcmp(partelem.elemunion.pvar->v_identifier, "idH") &&
				strcmp(partelem.elemunion.pvar->v_identifier, "idL")) {

			if(partelem.elemunion.pvar->v_varprops&OV_VP_DERIVED)
				goto SKIP; // accessor functions are not available so skip derived variables

			if(!header){
				header = TRUE;
				fprintf(handle, "\tVARIABLE_VALUES");
				ov_newline(handle, rnum);
			}

			fprintf(handle, "\t\t%s", partelem.elemunion.pvar->v_identifier);

			if(!partelem.pvalue)
				fprintf(handle, "(null)");

			ov_object_getvar(pobj, &partelem, &any);

			if(any.value.vartype&OV_VT_ISVECTOR){
				arraysize = any.value.valueunion.val_generic_vec.veclen;
			} else {
				arraysize = 1;
			}

			if(arraysize!=1){
				fprintf(handle, "[%i] : ", arraysize);
			} else {
				fprintf(handle, " : ");
			}

			ov_flagoutput(&partelem, handle);
			fprintf(handle, " ");
			ov_typeoutput(partelem.pvalue, partelem.elemunion.pvar->v_vartype, partelem.elemunion.pvar->v_veclen, handle);
			fprintf(handle, " = ");
			ov_valueoutput(&any, handle);
			ov_newline(handle, rnum);
		}
SKIP:
		ov_element_getnextpart(&parentelem, &partelem, OV_ET_VARIABLE);
	}

	if(header){
		fprintf(handle, "\tEND_VARIABLE_VALUES;");
		ov_newline(handle, rnum);
	}

	Ov_SetAnyValue(&any, NULL);
}

/*
 *	link output
 */
static void ov_linkoutput(
		OV_INSTPTR_ov_association passoc,
		OV_INSTPTR_ov_object pparent,
		FILE *handle
){
	OV_INSTPTR_ov_object	pchild;
	Ov_Association_DefineIteratorNM(pit);

	fprintf(handle, "LINK");
	ov_newline(handle, 1);
	fprintf(handle, "OF_ASSOCIATION %s;", passoc->v_identifier);
	ov_newline(handle, 1);
	fprintf(handle, "PARENT %s : CLASS unknown", passoc->v_parentrolename);
	ov_newline(handle, 2);
	// we already checked that the parent is a dumped / default object
	ov_memstack_lock();
	fprintf(handle, "= %s;", ov_path_getcanonicalpath(pparent, 2));
	ov_memstack_unlock();
	ov_newline(handle, 1);

	fprintf(handle, "CHILDREN %s : CLASS unknown", passoc->v_childrolename);
	ov_newline(handle, 2);
	fprintf(handle, "= { ");

	switch (passoc->v_assoctype) {
	case OV_AT_ONE_TO_ONE:
		pchild = Ov_Association_GetChild(passoc, pparent);
		if (pchild) { // already checked but to be sure
			ov_memstack_lock();
			fprintf(handle, "%s", ov_path_getcanonicalpath(pchild, 2));
			ov_memstack_unlock();
		}
		break;
	case OV_AT_ONE_TO_MANY:
		pchild = Ov_Association_GetFirstChild(passoc, pparent);
		while(pchild && !ov_linkFilterPass(pchild)){ // get first child to dump
			pchild = Ov_Association_GetNextChild(passoc, pchild);
		}
		while (pchild) {
			ov_memstack_lock();
			fprintf(handle, "%s", ov_path_getcanonicalpath(pchild, 2));
			ov_memstack_unlock();
			do { // get next child to dump
				pchild = Ov_Association_GetNextChild(passoc, pchild);
			} while (pchild && !ov_linkFilterPass(pchild));
			if (pchild) fprintf(handle, " , ");
		}
		break;
	case OV_AT_MANY_TO_MANY:
		pchild = Ov_Association_GetFirstChildNM(passoc, pit, pparent);
		while(pchild && !ov_linkFilterPass(pchild)){ // get first child to dump
			pchild = Ov_Association_GetNextChildNM(passoc, pit);
		}
		while (pchild) {
			ov_memstack_lock();
			fprintf(handle, " %s", ov_path_getcanonicalpath(pchild, 2));
			ov_memstack_unlock();
			do { // get next child to dump
				pchild = Ov_Association_GetNextChildNM(passoc, pit);
			} while (pchild && !ov_linkFilterPass(pchild));
			if (pchild) fprintf(handle, " , ");
		}
		break;
	}
	fprintf(handle, " };");
	ov_newline(handle, 0);
	fprintf(handle, "END_LINK;\n\n");
}

/*
 *	instance output
 */
static void ov_instanceoutput(
		OV_INSTPTR_ov_object pobj,
		FILE *handle,
		int rnum
){
	OV_INSTPTR_ov_class   	pclass;
	OV_STRING 		res;

	/*liefert den Zeiger der Klasse einer Instanz*/
	pclass = Ov_GetClassPtr(pobj);

	fprintf(handle, "INSTANCE ");

	ov_memstack_lock();
	res = ov_path_getcanonicalpath(pobj, 2);
	fprintf(handle, "%s :", res);
	ov_memstack_unlock();

	ov_newline(handle, 1);

	/* schreibt den Namen der zuge�rigen Bibliothek und Klasse aus */
	fprintf(handle, "CLASS ");
	ov_memstack_lock();
	res = ov_path_getcanonicalpath( Ov_StaticPtrCast(ov_object, pclass), 2);
	fprintf(handle,"%s;", res);
	ov_memstack_unlock();

	ov_newline(handle, rnum);

	/* Ausgabe der Objekt-Variablen */
	ov_processVariables(pobj, handle, rnum);

	fprintf(handle, "END_INSTANCE;\n\n");
}

/*
 *	iterate over association connections
 */
void ov_processAssociations(
	OV_INSTPTR_ov_association passoc,
	OV_INSTPTR_ov_class pclass,
	FILE *handle
){
	OV_INSTPTR_ov_class parentClass = Ov_GetParent(ov_parentrelationship, passoc); // get parent class of association
	OV_INSTPTR_ov_class derivedClass = NULL; 
	OV_INSTPTR_ov_object pobj = NULL;
	OV_INSTPTR_ov_object pchild = NULL;
	Ov_Association_DefineIteratorNM(pit);
	
	if(pclass)
		parentClass = pclass;

	// iterate over all instances of parentClass
	Ov_ForEachChild(ov_instantiation, parentClass, pobj){

		if(!ov_linkFilterPass(pobj))
			continue;

		switch (passoc->v_assoctype) {
			case OV_AT_ONE_TO_ONE:
				pchild = Ov_Association_GetChild(passoc, pobj);
				if(!ov_linkFilterPass(pchild))
					continue; // not connected to any dumped object
				break;
			case OV_AT_ONE_TO_MANY:
				pchild = Ov_Association_GetFirstChild(passoc, pobj);
				while(pchild && !ov_linkFilterPass(pchild)){
					pchild = Ov_Association_GetNextChild(passoc, pchild);
				}
				if(!pchild) // if NULL we have no links that need to be dumped
					continue;
				break;
			case OV_AT_MANY_TO_MANY:
				pchild = Ov_Association_GetFirstChildNM(passoc, pit, pobj);
				while(pchild && !ov_linkFilterPass(pchild)){
					pchild = Ov_Association_GetNextChildNM(passoc, pit);
				}
				if(!pchild) // if NULL we have no links that need to be dumped
					continue;
				break;
		}

		ov_linkoutput(passoc, pobj, handle);
	}
	
	// recursive call to handle derived classes
	Ov_ForEachChild(ov_inheritance, parentClass, derivedClass){
		ov_processAssociations(passoc, derivedClass, handle);
	}

}

/*
 * iterate over part objects
 */
void ov_processPartObjects(
	OV_INSTPTR_ov_object pobj,
	FILE *handle
){
	OV_ELEMENT	parentelem;
	OV_ELEMENT	partelem;

	parentelem.elemtype = OV_ET_OBJECT;
	parentelem.pobj = pobj;
	partelem.elemtype = OV_ET_NONE;
	partelem.pobj = NULL;

	ov_element_getnextpart(&parentelem, &partelem, OV_ET_OBJECT);
	while (partelem.elemtype != OV_ET_NONE) {
		ov_instanceoutput(partelem.pobj, handle, 0);
		ov_element_getnextpart(&parentelem, &partelem, OV_ET_OBJECT);
	}

}

/*
 *	iterate over database objects
 */
void ov_processDomains(
		FILE *handle,
		OV_INSTPTR_ov_domain pdom
) {
	OV_INSTPTR_ov_object 	pobj;
	OV_INSTPTR_ov_domain 	pdom2;

	Ov_ForEachChild(ov_containment, pdom, pobj) {

		if(	(pdom==&pdb->root) &&
			(	!strcmp(pobj->v_identifier, "acplt")		||
				!strcmp(pobj->v_identifier, "dbinfo")		||
				!strcmp(pobj->v_identifier, "vendor")		||
				!strcmp(pobj->v_identifier, "servers")		||
				!strcmp(pobj->v_identifier, "Libraries")	||
				(!includeMode &&
					(	!strcmp(pobj->v_identifier, "data")	||
						!strcmp(pobj->v_identifier, "communication")
					)
				)
			)
		){	continue;	}

		ov_instanceoutput(pobj, handle, 0);
		ov_processPartObjects(pobj, handle);

		pdom2 = Ov_DynamicPtrCast(ov_domain, pobj);
		if (pdom2) ov_processDomains(handle, pdom2);
	}
}

/*
 * dump loaded libraries
 */
void ov_processLibraries(
	FILE *handle
){
	OV_INSTPTR_ov_library pLib = NULL;

	ov_memstack_lock();
	Ov_ForEachChildEx(ov_instantiation, pclass_ov_library, pLib, ov_library){
		if(!strcmp(pLib->v_identifier, "ov"))
			continue;

		fprintf(handle, "LIBRARY\n");
		fprintf(handle, "\t%s\n", ov_path_getcanonicalpath(Ov_StaticPtrCast(ov_object, pLib), 2));
		fprintf(handle, "END_LIBRARY;\n\n");
	}
	ov_memstack_unlock();
}

/*
 * main fbd dumping function
 */
OV_RESULT ov_dumpFbd(
		OV_STRING name,
		OV_STRING startPath,
		OV_INT mode
) {
	FILE 			*handle;
	OV_INSTPTR_ov_object 	pobj;
	OV_INSTPTR_ov_association	passoc;

	includeMode = mode;

	handle = fopen(name, "w");
	if(!handle) {
		ov_logfile_error("unable to open file for writing: %s.\n", name);
		return EXIT_FAILURE;
	}
	passoc_ov_containment = &(pdb->containment);
	passoc_ov_instantiation = &(pdb->instantiation);
	if ((!passoc_ov_containment) || (!passoc_ov_instantiation)) {
		ov_logfile_error("unable to get associations ov_containment or ov_instantiation");
		fclose(handle);
		return EXIT_FAILURE;
	}

	// get passocs from database
	Ov_ForEachChild(ov_containment, &pdb->ov, pobj) {
		if (!strcmp(pobj->v_identifier, "containment")) passoc_ov_containment = (OV_INSTPTR_ov_association) pobj;
		if (!strcmp(pobj->v_identifier, "instantiation")) passoc_ov_instantiation = (OV_INSTPTR_ov_association) pobj;
		if (!strcmp(pobj->v_identifier, "inheritance")) passoc_ov_inheritance = (OV_INSTPTR_ov_association) pobj;
		if (!strcmp(pobj->v_identifier, "parentrelationship")) passoc_ov_parentrelationship = (OV_INSTPTR_ov_association) pobj;
		if (!strcmp(pobj->v_identifier, "childrelationship")) passoc_ov_childrelationship = (OV_INSTPTR_ov_association) pobj;

		if (!strcmp(pobj->v_identifier, "library")) pclass_ov_library = (OV_INSTPTR_ov_class) pobj;
		if (!strcmp(pobj->v_identifier, "object")) pclass_ov_object = (OV_INSTPTR_ov_class) pobj;
		if (!strcmp(pobj->v_identifier, "domain")) pclass_ov_domain = (OV_INSTPTR_ov_class) pobj;
		if (!strcmp(pobj->v_identifier, "class")) pclass_ov_class = (OV_INSTPTR_ov_class) pobj;
		if (!strcmp(pobj->v_identifier, "association")) pclass_ov_association = (OV_INSTPTR_ov_class) pobj;
		if (!strcmp(pobj->v_identifier, "variable")) pclass_ov_variable = (OV_INSTPTR_ov_class) pobj;
		if (!strcmp(pobj->v_identifier, "operation")) pclass_ov_operation = (OV_INSTPTR_ov_class) pobj;
		if (!strcmp(pobj->v_identifier, "part")) pclass_ov_part = (OV_INSTPTR_ov_class) pobj;
	}
	if ((!passoc_ov_inheritance) || (!passoc_ov_parentrelationship) || (!passoc_ov_childrelationship)) {
		ov_logfile_error("unable to get associations ov_inheritance or ov_parentrelationship or ov_childrelationship");
		fclose(handle);
		return EXIT_FAILURE;
	}

	if(startPath){
		ov_logfile_info("dumping %s as fbd to %s", startPath, name);
	} else {
		ov_logfile_info("dumping datbase as fbd to %s ", name);
	}

	if(startPath){
		pRoot = Ov_DynamicPtrCast(ov_domain, ov_path_getobjectpointer(startPath, 2));
		pTasks = ov_path_getobjectpointer("/Tasks", 2);
		if(!pRoot){
			ov_logfile_error("path %s not found or no of type ov_domain");
			fclose(handle);
			return EXIT_FAILURE;
		}
	} else {
		pRoot = &pdb->root;
	}

	pComm = ov_path_getobjectpointer("/communication", 2);
	pData = ov_path_getobjectpointer("/data", 2);

	ov_activitylock = TRUE; // set activity lock to avoid calling get functions which are not loaded

	// start recursive object iteration
	ov_processLibraries(handle);
	ov_processDomains(handle, pRoot);
	Ov_ForEachChildEx(ov_instantiation, pclass_ov_association, passoc, ov_association){
		if(Ov_GetParent(ov_containment, passoc)==Ov_PtrUpCast(ov_domain, &pdb->ov))
			continue; // skip associations in ov library

		ov_processAssociations(passoc, NULL, handle);
	}

	fclose(handle);
	return EXIT_SUCCESS;
}


