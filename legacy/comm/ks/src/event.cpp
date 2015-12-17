/* -*-plt-c++-*- */
/* $Header: /home/david/cvs/acplt/ks/src/event.cpp,v 1.8 2007-04-25 12:57:20 martin Exp $ */
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


#include "ks/event.h"

PLT_IMPL_RTTI0(KsEvent);
PLT_IMPL_RTTI1(KsTimerEvent,KsEvent);

//////////////////////////////////////////////////////////////////////


KsTime
KsTimerEvent::remainingTime() const
{
    KsTime now(KsTime::now());
    if (now < triggersAt()) {
        return triggersAt()-now;
    } else {
        return KsTime();
    }
}

/////////////////////////////////////////////////////////////////////////////
/* EOF ks/event.cpp */
