/* -*-plt-c++-*- */
#ifndef KS_RESULT_INCLUDED
#define KS_RESULT_INCLUDED
/* $Header: /home/david/cvs/acplt/ks/include/ks/result.h,v 1.9 2007-04-25 12:57:20 martin Exp $ */
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

#include "ks/xdr.h"
#include "ks/ks.h"

////////////////////////////////////////////////////////////////////////////
// class KsResult
///////////////////////////////////////////////////////////////////////////

class KsResult 
: public KsXdrAble 
{
public:
    KsResult( KS_RESULT res = -1 ); // default is no result...
    KsResult( XDR *, bool &);

    bool xdrEncode(XDR *xdr) const;
    bool xdrDecode(XDR *xdr);
    static KsResult *xdrNew(XDR *);

    KS_RESULT result;
};
#endif /* ks/result.h */
