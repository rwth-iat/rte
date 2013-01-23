/* -*-plt-c++-*- */

#ifndef KS_EXT_SP_INCLUDED
#define KS_EXT_SP_INCLUDED

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

#include "ks/serviceparams.h"

/////////////////////////////////////////////////////////////////////////////
// This file contains service parameters for the sample protocol extension
// "ks_test" and its only service KS_CROBJ.
//

const u_long KS_CROBJ_MINOR_OPCODE = 0x5;

/////////////////////////////////////////////////////////////////////////////

class KsCreateParams 
: public KsXdrAble
{
public:
    KsCreateParams(XDR *, bool &);
    KsCreateParams() {}

    bool xdrEncode(XDR *) const;
    bool xdrDecode(XDR *);
    static KsCreateParams *xdrNew(XDR *);

    KsString name;
};

/////////////////////////////////////////////////////////////////////////////

typedef KsResult KsCreateResult;

/////////////////////////////////////////////////////////////////////////////

#endif
