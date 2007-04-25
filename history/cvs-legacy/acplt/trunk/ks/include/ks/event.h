/* -*-plt-c++-*- */
#ifndef KS_EVENT_INCLUDED
#define KS_EVENT_INCLUDED
/* $Header: /home/david/cvs/acplt/ks/include/ks/event.h,v 1.11 2007-04-25 12:57:20 martin Exp $ */
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

#include "ks/time.h"
#include "plt/rtti.h"

//////////////////////////////////////////////////////////////////////

class KsEvent
{
public:
    virtual void trigger() = 0;
    virtual ~KsEvent() { }
    PLT_DECL_RTTI;
};

//////////////////////////////////////////////////////////////////////

class KsTimerEvent
: public KsEvent
{
public:
    KsTimerEvent(const KsTime & at);
    virtual void trigger() = 0;
    KsTime remainingTime() const;

    friend bool operator == (const KsTimerEvent & lhs, const KsTimerEvent & rhs);
    friend bool operator != (const KsTimerEvent & lhs, const KsTimerEvent & rhs);
    friend bool operator <= (const KsTimerEvent & lhs, const KsTimerEvent & rhs);
    friend bool operator >= (const KsTimerEvent & lhs, const KsTimerEvent & rhs);
    friend bool operator <  (const KsTimerEvent & lhs, const KsTimerEvent & rhs);
    friend bool operator >  (const KsTimerEvent & lhs, const KsTimerEvent & rhs);

    KsTime triggersAt() const;
protected:
    KsTime _trigger_at;
    PLT_DECL_RTTI;
};


//////////////////////////////////////////////////////////////////////

inline
KsTimerEvent::KsTimerEvent(const KsTime & at)
: _trigger_at(at)
{
}


//////////////////////////////////////////////////////////////////////

inline KsTime
KsTimerEvent::triggersAt() const 
{ 
    return _trigger_at; 
} 

//////////////////////////////////////////////////////////////////////

inline bool 
operator == (const KsTimerEvent & lhs, const KsTimerEvent & rhs)
{
    return lhs.triggersAt() == rhs.triggersAt();
}


//////////////////////////////////////////////////////////////////////

inline bool 
operator != (const KsTimerEvent & lhs, const KsTimerEvent & rhs)
{
    return lhs.triggersAt() != rhs.triggersAt();
}


//////////////////////////////////////////////////////////////////////

inline bool 
operator <= (const KsTimerEvent & lhs, const KsTimerEvent & rhs)
{
    return lhs.triggersAt() <= rhs.triggersAt();
}


//////////////////////////////////////////////////////////////////////

inline bool 
operator >= (const KsTimerEvent & lhs, const KsTimerEvent & rhs)
{
    return lhs.triggersAt() >= rhs.triggersAt();
}


//////////////////////////////////////////////////////////////////////

inline bool 
operator < (const KsTimerEvent & lhs, const KsTimerEvent & rhs)
{
    return lhs.triggersAt() < rhs.triggersAt();
}


//////////////////////////////////////////////////////////////////////

inline bool 
operator > (const KsTimerEvent & lhs, const KsTimerEvent & rhs)
{
    return lhs.triggersAt() > rhs.triggersAt();
}


//////////////////////////////////////////////////////////////////////
#endif // KS_EVENT_INCLUDED
