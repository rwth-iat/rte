/* -*-c++-*- */
/* $Header: /home/david/cvs/acplt/ks/src/rpc.cpp,v 1.4 2003-10-13 12:05:55 harald Exp $ */
/*
 * Copyright (c) 1996, 1997, 1998, 1999
 * Lehrstuhl fuer Prozessleittechnik, RWTH Aachen
 * D-52064 Aachen, Germany.
 * All rights reserved.
 *
 * This file is part of the ACPLT/KS Package which is licensed as open
 * source under the Artistic License; you can use, redistribute and/or
 * modify it under the terms of that license.
 *
 * You should have received a copy of the Artistic License along with
 * this Package; see the file ARTISTIC-LICENSE. If not, write to the
 * Copyright Holder.
 *
 * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED WARRANTIES
 * OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 */

#include "plt/config.h"

#if PLT_SYSTEM_NT

#include "ks/rpc.h"

//////////////////////////////////////////////////////////////////////

int
KsNTOncRpc::refcount = 0;

//////////////////////////////////////////////////////////////////////

KsNTOncRpc::KsNTOncRpc()
{
    if (0 == refcount++) {
	// First time
	rpc_nt_init();
    }
}


//////////////////////////////////////////////////////////////////////

KsNTOncRpc::~KsNTOncRpc()
{
    if (0 == --refcount) {
	// Last time
	rpc_nt_exit();
    }
}

#endif // PLT_SYSTEM_NT
/////////////////////////////////////////////////////////////////////////////
/* EOF ks/rpc.cpp */
