/*
*   $Id: ov_operation.c,v 1.3 1999-09-15 10:48:22 dirk Exp $
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

#include "libov/ov_operation.h"
#include "libov/ov_logfile.h"
#include "libov/ov_string.h"
#include "libov/ov_macros.h"

/*	----------------------------------------------------------------------	*/

/*
*	Load an operation into the database
*/
OV_RESULT ov_operation_load(
	OV_OPERATION_DEF*		popdef,
	OV_INSTPTR_ov_domain	pparent
) {
	/*
	*	local variables
	*/
	OV_RESULT				result;
	OV_INSTPTR_ov_operation	pop = NULL;
	/*
	*	create operation object
	*/
	result = Ov_CreateObject(ov_operation, pop, pparent, popdef->identifier);
	if(Ov_Fail(result)) {
		return result;
	}
	/*
	*	set parameters
	*/
	ov_string_setvalue(&pop->v_cfnctypename, popdef->cfnctypename);
	pop->v_opprops = popdef->opprops;
	return OV_ERR_OK;
}

/*	----------------------------------------------------------------------	*/

/*
*	Compare an operation with its definition
*/
OV_RESULT ov_operation_compare(
	OV_INSTPTR_ov_operation	pop,
	OV_OPERATION_DEF*		popdef
) {
	/*
	*	compare operation attributes
	*/
	if(strcmp(pop->v_identifier, popdef->identifier)
		|| strcmp(pop->v_cfnctypename, popdef->cfnctypename)
		|| (pop->v_opprops != popdef->opprops)
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
	ov_logfile_error("Error loading operation \"%s/%s/%s\".",
		Ov_GetParent(ov_containment, Ov_GetParent(ov_containment, pop))->v_identifier,
		Ov_GetParent(ov_containment, pop)->v_identifier,
		pop->v_identifier);
	return OV_ERR_OPDEFMISMATCH;
}

/*	----------------------------------------------------------------------	*/

/*
*	Test if we can unload an operation from the database
*/
OV_BOOL ov_operation_canunload(
	OV_INSTPTR_ov_operation	pop
) {
	return TRUE;
}

/*	----------------------------------------------------------------------	*/

/*
*	Accessor functions
*/
OV_DLLFNCEXPORT OV_INT ov_operation_opprops_get(
	OV_INSTPTR_ov_operation	pop
) {
	return pop->v_opprops;
}

OV_DLLFNCEXPORT OV_STRING ov_operation_cfnctypename_get(
	OV_INSTPTR_ov_operation	pop
) {
	return pop->v_cfnctypename;
}

/*	----------------------------------------------------------------------	*/

/*
*	End of file
*/

