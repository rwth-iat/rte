/*
*   $Id: ov_object.h,v 1.1 1999-07-19 15:02:04 dirk Exp $
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
*	13-Apr-1999 Dirk Meyer <dirk@plt.rwth-aachen.de>: File created.
*/

#ifndef OV_OBJECT_H_INCLUDED
#define OV_OBJECT_H_INCLUDED

#include "ov.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
*	Test, if an object owns links (except to the parent domain and class)
*/
OV_BOOL OV_DLLFNCEXPORT ov_object_haslinks(
	OV_INSTPTR_ov_object 		pobj
);

/*
*	Test, if a string is a valid identifier for an object
*/
OV_BOOL OV_DLLFNCEXPORT ov_object_identifierok(
	OV_STRING		identifier
);

/*
*	Constructor of an object in case the database is not started up (subroutine)
*/
OV_RESULT ov_object_constructor_nostartup(
	OV_INSTPTR_ov_object 		pobj
);

/*
*	Method for getting the access rights of an object or one of its elements
*	in case the database is not started up (subroutine)
*/
OV_ACCESS ov_object_getaccess_nostartup(
	OV_INSTPTR_ov_object		pobj,
	const OV_ELEMENT			*pelem,
	const OV_TICKET				*pticket
);

/*
*	Move an object and its children/parts to a new address (subroutine)
*/
OV_RESULT ov_object_move(
	OV_INSTPTR_ov_object		pobj,
	const OV_INSTPTR_ov_object	pobjcopy,
	const OV_INT				distance
);

#ifdef __cplusplus
}	/* extern "C" */
#endif

#endif
/*
*	End of file
*/

