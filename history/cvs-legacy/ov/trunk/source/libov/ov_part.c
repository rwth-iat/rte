/*
*   $Id: ov_part.c,v 1.1 1999-07-19 15:02:14 dirk Exp $
*
*   Copyright (C) 1998-1999
*   Lehrstuhl fuer Prozessleittechnik,
*   RWTH Aachen, D-52056 Aachen, Germany.
*   All rights reserved.
*
*   Author: Dirk Meyer <dirk@plt.rwth-aachen.de>
*
*   This file is part of the ACPLT/OV Package which is licensed as open
*   source under the Artistic License; you can use, redistribute and/or
*   modify it under the terms of that license.
*
*   You should have received a copy of the Artistic License along with
*   this Package; see the file ARTISTIC-LICENSE. If not, write to the
*   Copyright Holder.
*
*   THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR IMPLIED
*   WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED WARRANTIES
*   OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE.
*/
/*
*	History:
*	--------
*	09-Apr-1999 Dirk Meyer <dirk@plt.rwth-aachen.de>: File created.
*/

#define OV_COMPILE_LIBOV

#include "libov/ov_part.h"
#include "libov/ov_logfile.h"
#include "libov/ov_macros.h"

/*	----------------------------------------------------------------------	*/

/*
*	Load a part into the database
*/
OV_RESULT ov_part_load(
	OV_PART_DEF*			ppartdef,
	OV_INSTPTR_ov_domain	pparent
) {
	/*
	*	local variables
	*/
	OV_RESULT			result;
	OV_INSTPTR_ov_part	ppart;
	OV_INSTPTR_ov_class	ppartclass;
	/*
	*	look for the class object of the part's class
	*/
	ppartclass = ov_class_search(ppartdef->partclassname);
	if(!ppartclass) {
		return OV_ERR_UNKNOWNCLASSDEF;
	}
	/*
	*	create part object
	*/
	result = Ov_CreateObject(ov_part, ppart, pparent, ppartdef->identifier);
	if(Ov_Fail(result)) {
		return result;
	}
	/*
	*	set parameters
	*/
	ppart->v_offset = ppartdef->offset;
	/*
	*	link with class of the component
	*/
	Ov_WarnIfNot(Ov_OK(Ov_Link(ov_embedment, ppartclass, ppart)));
	return OV_ERR_OK;
}

/*	----------------------------------------------------------------------	*/

/*
*	Compare a part with its definition
*/
OV_RESULT ov_part_compare(
	OV_INSTPTR_ov_part	ppart,
	OV_PART_DEF*		ppartdef
) {
	/*
	*	local variables
	*/
	OV_INSTPTR_ov_class	pclass;
	OV_STRING			id, libid;
	/*
	*	compare class of the component
	*/
	pclass = Ov_GetParent(ov_embedment, ppart);
	if(!pclass) {
		goto ERRORMSG;
	}
	libid = Ov_GetParent(ov_containment, pclass)->v_identifier;
	id = (OV_STRING)Ov_HeapMalloc(strlen(libid)+1+strlen(pclass->v_identifier)+1);
	if(!id) {
		return OV_ERR_HEAPOUTOFMEMORY;
	}
	sprintf(id, "%s/%s", libid, pclass->v_identifier);
	if(strcmp(ppartdef->partclassname, id)) {
		Ov_HeapFree(id);
		goto ERRORMSG;
	}
	Ov_HeapFree(id);
	/*
	*	compare part attributes
	*/
	if(strcmp(ppart->v_identifier, ppartdef->identifier)
		|| (ppart->v_offset != ppartdef->offset)
	) {
		goto ERRORMSG;
	}
	/*
	*	all is fine
	*/
	return OV_ERR_OK;
	/*
	*	print error message and exit
	*/
ERRORMSG:
	ov_logfile_error("Error loading part \"%s/%s/%s\".",
		Ov_GetParent(ov_containment, Ov_GetParent(ov_containment, ppart))->v_identifier,
		Ov_GetParent(ov_containment, ppart)->v_identifier,
		ppart->v_identifier);
	return OV_ERR_PARTDEFMISMATCH;
}

/*	----------------------------------------------------------------------	*/

/*
*	Test if we can unload a part from the database
*/
OV_BOOL ov_part_canunload(
	OV_INSTPTR_ov_part	ppart
) {
	return TRUE;
}

/*	----------------------------------------------------------------------	*/

/*
*	End of file
*/

