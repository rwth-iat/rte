/*
*   $Id: ov_operation.h,v 1.1 1999-07-19 15:02:04 dirk Exp $
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

#ifndef OV_OPERATION_H_INCLUDED1
#define OV_OPERATION_H_INCLUDED1

#include "libov/ov_ov.h"

/*
*	OV_OPERATION_DEF:
*	-----------------
*	Operation definition, included in the library's code
*/
struct OV_OPERATION_DEF {
	struct OV_OPERATION_DEF	*pnext;
	OV_STRING				identifier;
	OV_OP_PROPS				opprops;
	OV_STRING				cfnctypename;
};
typedef struct OV_OPERATION_DEF OV_OPERATION_DEF;

#endif

#include "ov.h"

#ifdef OV_INSTBODY_ov_object

#ifndef OV_OPERATION_H_INCLUDED2
#define OV_OPERATION_H_INCLUDED2

#ifdef __cplusplus
extern "C" {
#endif

/*
*	Load an operation into the database
*/
OV_RESULT ov_operation_load(
	OV_OPERATION_DEF*		popdef,
	OV_INSTPTR_ov_domain	pparent
);

/*
*	Compare an operation with its definition
*/
OV_RESULT ov_operation_compare(
	OV_INSTPTR_ov_operation	pop,
	OV_OPERATION_DEF*		popdef
);

/*
*	Test if we can unload an operation from the database
*/
OV_BOOL ov_operation_canunload(
	OV_INSTPTR_ov_operation	pop
);

#ifdef __cplusplus
}	/* extern "C" */
#endif

#endif

#endif
/*
*	End of file
*/

