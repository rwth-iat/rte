/*
*   $Id: ov_result.c,v 1.2 1999-07-26 16:14:14 dirk Exp $
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
*	19-Nov-1998 Dirk Meyer <dirk@plt.rwth-aachen.de>: File created.
*	13-Apr-1999 Dirk Meyer <dirk@plt.rwth-aachen.de>: Major revision.
*/

#define OV_COMPILE_LIBOV

#include "libov/ov_result.h"

/*	----------------------------------------------------------------------	*/

/*
*	Return error string associated with an error code
*/
OV_STRING OV_DLLFNCEXPORT ov_result_getresulttext(
	OV_RESULT	result
) {
	switch(result) {
		case OV_ERR_OK:					return "ok";
		case OV_ERR_GENERIC:			return "generic error";
		case OV_ERR_TARGETGENERIC:		return "generic error in target";
		case OV_ERR_BADAUTH:			return "bad authentification";
		case OV_ERR_UNKNOWNAUTH:		return "unknown authentification";
		case OV_ERR_NOTIMPLEMENTED:		return "not implemented";
		case OV_ERR_BADPARAM:			return "bad parameter";
		case OV_ERR_BADOBJTYPE:			return "bad object type";
   
		case OV_ERR_BADNAME:			return "bad name";
		case OV_ERR_BADPATH:			return "bad path";
		case OV_ERR_BADMASK:			return "bad mask";
		case OV_ERR_NOACCESS:			return "no access";
		case OV_ERR_BADTYPE:			return "bad type";
		case OV_ERR_BADSELECTOR:		return "bad selector";
		case OV_ERR_BADVALUE:			return "bad value";

		case OV_ERR_BADFACTORY:			return "bad factory";
		case OV_ERR_ALREADYEXISTS:		return "object already exists";
		case OV_ERR_BADINITPARAM:		return "bad initialization parameter";
		case OV_ERR_BADPLACEMENT:		return "bad placement";
		case OV_ERR_CANTMOVE:			return "can't move object";

		case OV_ERR_FILEALREADYEXISTS:	return "file already exists";
		case OV_ERR_CANTCREATEFILE:		return "can't create file";
		case OV_ERR_CANTOPENFILE:		return "can't open file";
		case OV_ERR_CANTLOCKFILE:		return "can't lock file";
		case OV_ERR_CANTREADFROMFILE:	return "can't read from file";
		case OV_ERR_CANTWRITETOFILE:	return "can't write to file";
		case OV_ERR_CANTMAPFILE:		return "can't map file to memory";
		case OV_ERR_BADDATABASE:		return "bad database";

		case OV_ERR_CANTOPENLIBRARY:	return "can't open library";

		case OV_ERR_LIBDEFMISMATCH:		return "mismatch of library definition";
		case OV_ERR_STRUCTDEFMISMATCH:	return "mismatch of structure definition";
		case OV_ERR_CLASSDEFMISMATCH:	return "mismatch of class definition";
		case OV_ERR_ASSOCDEFMISMATCH:	return "mismatch of association definition";
		case OV_ERR_VARDEFMISMATCH:		return "mismatch of variable definition";
		case OV_ERR_PARTDEFMISMATCH:	return "mismatch of part definition";
		case OV_ERR_OPDEFMISMATCH:		return "mismatch of operation definition";
		case OV_ERR_UNKNOWNSTRUCTDEF:	return "unknown structure definition";
		case OV_ERR_UNKNOWNCLASSDEF:	return "unknown class definition";
		case OV_ERR_UNKNOWNASSOCDEF:	return "unknown association definition";

		case OV_ERR_DBOUTOFMEMORY:		return "database is out of memory";
		case OV_ERR_HEAPOUTOFMEMORY:	return "heap is out of memory";
		
		default: break;
	}
	return "unknown error code";
}

/*	----------------------------------------------------------------------	*/

/*
*	End of file
*/

