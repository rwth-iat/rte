/* -*-plt-c++-*- */
// File: plt/rtti.cpp
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
/* Author: Martin Kneissl <martin@plt.rwth-aachen.de> */

//////////////////////////////////////////////////////////////////////

#include "plt/rtti.h"

#if PLT_SIMULATE_RTTI


PltRTTI::PltRTTI(const char *name, const PltRTTI* bases[])
: n(name), b(bases)
{
    PLT_ASSERT(n);
}

//////////////////////////////////////////////////////////////////////

bool
PltRTTI::isBaseOf(const PltRTTI *pd) const
{
    if (pd->b) {
        for (const PltRTTI **p = pd->b; *p; p++) {
            if ( isSame(*p) || isBaseOf(*p) ) {
                return true;
            }
        }
    }
    return false;
}

#endif

//////////////////////////////////////////////////////////////////////
// EOF plt/rtti.cpp
//////////////////////////////////////////////////////////////////////
