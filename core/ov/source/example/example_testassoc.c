/*
*   $Id: example_testassoc.c,v 1.1 1999-07-19 15:02:11 dirk Exp $
*
*   Copyright (C) 1998
*   Lehrstuhl fuer Prozessleittechnik,
*   D-52056 Aachen, Germany.
*   All rights reserved.
*
*   Author: Dirk Meyer <dirk@plt.rwth-aachen.de>
*
*   This file is part of the ACPLT/OV Package 
*   
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

#ifndef OV_COMPILE_LIBRARY_example
#define OV_COMPILE_LIBRARY_example
#endif

#include <stdio.h>

#include "example.h"
#include "libov/ov_macros.h"

/*	----------------------------------------------------------------------	*/

/*
*	Link parent and child in a testassoc association
*/
OV_IMPL_LINK(example_testassoc)

/*	----------------------------------------------------------------------	*/

/*
*	Unlink parent and child in a testassoc association
*/
OV_IMPL_UNLINK(example_testassoc)

/*	----------------------------------------------------------------------	*/

/*
*	Get access rights for a parent and child in a testassoc association
*/
OV_IMPL_GETACCESS(example_testassoc)

/*	----------------------------------------------------------------------	*/

/*
*	End of file
*/

