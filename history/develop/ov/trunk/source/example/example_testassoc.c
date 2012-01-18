/*
*   $Id: example_testassoc.c,v 1.1 1999-07-19 15:02:11 dirk Exp $
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

