/* -*-plt-c++-*- */
#ifndef KSC_AVMODULE_INCLUDED 
#define KSC_AVMODULE_INCLUDED
/* $Header: /home/david/cvs/acplt/ks/include/ks/avmodule.h,v 1.14 2007-04-25 12:57:20 martin Exp $ */
/*
 * Copyright (c) 1996, 1997, 1998, 1999, 2000
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

#include <plt/handle.h>
#include <plt/list.h>

#include "ks/ks.h"
#include "ks/xdr.h"
#include "ks/string.h"

//////////////////////////////////////////////////////////////////////
// forward declaration
//
class KscServer;

//////////////////////////////////////////////////////////////////////
// class KscNegotiator
//
class KscNegotiator
{
public:
    virtual ~KscNegotiator() {}
    virtual KS_AUTH_TYPE typeCode() const = 0;
    virtual bool xdrEncode(XDR *) = 0;
    virtual bool xdrDecode(XDR *) = 0;
};

typedef PltPtrHandle<KscNegotiator> KscNegotiatorHandle;

//////////////////////////////////////////////////////////////////////
// class KscAvModule
// 
class KscAvModule
{
public:
    KscAvModule();
    virtual ~KscAvModule();
    KscNegotiatorHandle getNegotiator(const KscServer *) const;

    virtual KS_AUTH_TYPE typeCode() const = 0;
    
    virtual unsigned long hash() const;
    bool operator == (const KscAvModule &) const;

protected:

    class ServernameNode :
    public PltString
    {
    public:
        ServernameNode(PltString name)
        : PltString(name), _next(0) { }
        ServernameNode *_next;
    };
    typedef ServernameNode *ServernameNodePtr;

    ServernameNodePtr *_server_namelist;

    virtual KscNegotiatorHandle _getNegotiator(const KscServer *) const = 0;

}; // class KscAvModule


//////////////////////////////////////////////////////////////////////
// class KscNoneNegotiator
//   handles an AUTH_NONE authentication. 
//
class KscNoneNegotiator
: public KscNegotiator
{
public:
    KS_AUTH_TYPE typeCode() const;
    bool xdrEncode(XDR *);
    bool xdrDecode(XDR *);

private:
    friend class KscAvNoneModule;
    KscNoneNegotiator() {}   // forbidden
};
    
//////////////////////////////////////////////////////////////////////
// class KscAvNoneModule
// This is a simple AvModule which give us a KscNegotiator object
// to handle an AUTH_NONE authentication.
//
class KscAvNoneModule
: public KscAvModule
{
public:
    virtual KS_AUTH_TYPE typeCode() const;

    static KscNegotiator *getStaticNegotiator();

protected:
    KscNegotiatorHandle _getNegotiator(const KscServer *) const;

private:
    static KscNoneNegotiator the_negotiator;
};

//////////////////////////////////////////////////////////////////////
// Inline Implementation
//////////////////////////////////////////////////////////////////////

inline
unsigned long
KscAvModule::hash() const
{
    return typeCode();
}

//////////////////////////////////////////////////////////////////////

inline
bool
KscAvModule::operator == (const KscAvModule &other) const
{
    return this == &other;
}

//////////////////////////////////////////////////////////////////////

inline
KS_AUTH_TYPE
KscAvNoneModule::typeCode() const
{
    return KS_AUTH_NONE;
}

//////////////////////////////////////////////////////////////////////

inline
KscNegotiator *
KscAvNoneModule::getStaticNegotiator()
{
    return &the_negotiator;
}

//////////////////////////////////////////////////////////////////////

inline
KS_AUTH_TYPE
KscNoneNegotiator::typeCode() const
{
    return KS_AUTH_NONE;
}

//////////////////////////////////////////////////////////////////////

#endif

// End of ks/avmodule.h
