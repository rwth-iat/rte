/* -*-plt-c++-*- */
#ifndef KS_MASK_INCLUDED
#define KS_MASK_INCLUDED
/* $Header: /home/david/cvs/acplt/ks/include/ks/mask.h,v 1.6 2007-04-25 12:57:20 martin Exp $ */
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
/*
 * Author: Nicole Boemeke
 */

#include "plt/string.h"

#ifndef PLT_IGNOR_UPCASE
#define PLT_IGNOR_UPCASE 0
#endif

///////////////////////////////////////////////////////////////////////

class KsMask
: public PltString
{
public:
    KsMask(const PltString & str) : PltString(str) { }
    int matches(const char *name) const;
private:
    int m_loop(const char *name, int n, int m) const;
    int set(const char *name, int &n, int &m) const;
    int joker(const char *name, int &n, int &m) const;
};

//////////////////////////////////////////////////////////////////
#endif // KS_MASK_INCLUDED
