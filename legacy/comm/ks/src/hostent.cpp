/* -*-plt-c++-*- */
/* $Header: /home/david/cvs/acplt/ks/src/hostent.cpp,v 1.6 2007-04-25 12:57:20 martin Exp $ */
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

/* Author: Markus Juergens <markusj@plt.rwth-aachen.de> */

//////////////////////////////////////////////////////////////////////

#include "ks/hostent.h"

//////////////////////////////////////////////////////////////////////

void
KscHostEnt::copy_ips(struct hostent *he)
{
    if ( he ) {
	size_t count = 0;
	while ( he->h_addr_list[count] ) count++;

	ips = PltArray<KSC_IP_TYPE>(count);

	for ( size_t i = 0; i < count; i++ ) {
	    ips[i] = *(KSC_IP_TYPE *)(he->h_addr_list[i]);
	}
    } else {
	ips = PltArray<KSC_IP_TYPE>(0);
    }
} // KscHostEnt::copy_ips

//////////////////////////////////////////////////////////////////////
// EOF hostent.cpp
//////////////////////////////////////////////////////////////////////


