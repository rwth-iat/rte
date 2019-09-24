/*
*   $Id: ov_result.c,v 1.4 1999-09-15 10:48:22 dirk Exp $
*
*   Copyright (C) 1998
*   Lehrstuhl fuer Prozessleittechnik,
*   D-52056 Aachen, Germany.
*   All rights reserved.
*
*   Author: Dirk Meyer <dirk@plt.rwth-aachen.de>
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
*   limitations under the License..
*/
/*
*	History:
*	--------
*	19-Nov-1998 Dirk Meyer <dirk@plt.rwth-aachen.de>: File created.
*	13-Apr-1999 Dirk Meyer <dirk@plt.rwth-aachen.de>: Major revision.
*/

#define OV_COMPILE_LIBOV

#include "ov_result.h"

/*	----------------------------------------------------------------------	*/

/*
*	Return error string (as a static variable) associated with an error code
*	no need to worry about memstack allocation
*/
OV_DLLFNCEXPORT OV_STRING ov_result_getresulttext(
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
	
	default:
		break;
	}
	return "unknown error code";
}

/*	----------------------------------------------------------------------	*/

/*
*	End of file
*/

