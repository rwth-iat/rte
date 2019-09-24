/*
*   $Id: ov_embedment.c,v 1.1 1999-07-19 15:02:12 dirk Exp $
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
*   limitations under the License.
*/
/*
*	History:
*	--------
*	04-May-1999 Dirk Meyer <dirk@plt.rwth-aachen.de>: File created.
*/

#define OV_COMPILE_LIBOV

#include "ov_association.h"
#include "ov_macros.h"

/*	----------------------------------------------------------------------	*/

/*
*	Link parent and child in a embedment association
*/
OV_IMPL_LINK(ov_embedment)

/*	----------------------------------------------------------------------	*/

/*
*	Unlink parent and child in a embedment association
*/
OV_IMPL_UNLINK(ov_embedment)

/*	----------------------------------------------------------------------	*/

/*
*	Get access rights for a parent and child in a embedment association
*/
OV_DECL_GETACCESS(ov_embedment) {
	/*
	*	do not allow to link any parent/child couple from the outside world
	*/
	if(pticket) {
		return OV_AC_READ;
	}
	return OV_AC_READ | OV_AC_LINKABLE | OV_AC_UNLINKABLE;
}

/*	----------------------------------------------------------------------	*/

/*
*	End of file
*/

