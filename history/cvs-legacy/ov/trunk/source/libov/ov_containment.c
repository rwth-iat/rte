/*
*   $Id: ov_containment.c,v 1.2 2003-08-25 09:16:03 ansgar Exp $
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
*	04-May-1999 Dirk Meyer <dirk@plt.rwth-aachen.de>: File created.
*/

#define OV_COMPILE_LIBOV

#include "libov/ov_association.h"
#include "libov/ov_macros.h"

/*	----------------------------------------------------------------------	*/

/*
*	Link parent and child in a containment association
*/
OV_IMPL_LINK(ov_containment)

/*	----------------------------------------------------------------------	*/

/*
*	Unlink parent and child in a containment association
*/
OV_IMPL_UNLINK(ov_containment)

/*	----------------------------------------------------------------------	*/

/*
*	Get access rights for a parent and child in a containment association
*/
OV_DECL_GETACCESS(ov_containment) {
	/*
	*	do not allow to link any parent/child couple or to read the links
	*	from the outside world
	*/
	if(pticket) {
#ifdef OV_EXPLAIN
		return OV_AC_READ;
#else
		return OV_AC_NONE;
#endif
	}
	return OV_AC_READ | OV_AC_LINKABLE | OV_AC_UNLINKABLE;
}

/*	----------------------------------------------------------------------	*/

/*
*	End of file
*/

