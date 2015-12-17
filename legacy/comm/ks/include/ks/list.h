/* -*-plt-c++-*- */
/* $Header: /home/david/cvs/acplt/ks/include/ks/list.h,v 1.8 2007-04-25 12:57:20 martin Exp $ */
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
#ifndef KS_LIST_INCLUDED
#define KS_LIST_INCLUDED

#include "plt/list.h"
#include "ks/xdr.h"

//////////////////////////////////////////////////////////////////////
// Non-intrusive double linked list node with information type T
template <class T>
class KsList 
: public PltList<T>,
  public KsXdrAble
{
public:
    KsList() 
        : PltList<T>() { }
    KsList(XDR * xdr, bool & ok) 
        : PltList<T>() 
            {
                ok = xdrDecode(xdr);
            }

    virtual bool xdrEncode(XDR *) const;
    virtual bool xdrDecode(XDR *);

    static KsList<T> *xdrNew(XDR *);

private:
    KsList(const KsList &); // forbidden
    KsList & operator = (const KsList &); // forbidden
};


//////////////////////////////////////////////////////////////////////
// IMPLEMENTATION
//////////////////////////////////////////////////////////////////////
// Microsoft bug
#if 0
template <class T>
inline
KsList<T>::KsList()
: PltList<T>()
{
}

//////////////////////////////////////////////////////////////////////

template <class T>
inline
KsList<T>::KsList(XDR * xdr, bool & ok)
: PltList<T>()
{
    ok = xdrDecode(xdr);
}
#endif
//////////////////////////////////////////////////////////////////////

#if PLT_SEE_ALL_TEMPLATES
#include "ks/list_impl.h"
#endif

//////////////////////////////////////////////////////////////////////

#endif  // PLT_LIST_INCLUDED











