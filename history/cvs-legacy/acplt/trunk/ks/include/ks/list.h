/* -*-plt-c++-*- */
/* $Header: /home/david/cvs/acplt/ks/include/ks/list.h,v 1.4 1997-07-18 14:11:00 martin Exp $ */
/*
 * Copyright (c) 1996, 1997
 * Chair of Process Control Engineering,
 * Aachen University of Technology.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must print or display the above
 *    copyright notice either during startup or must have a means for
 *    the user to view the copyright notice.
 * 3. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 4. Neither the name of the Chair of Process Control Engineering nor the
 *    name of the Aachen University of Technology may be used to endorse or
 *    promote products derived from this software without specific prior
 *    written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE CHAIR OF PROCESS CONTROL ENGINEERING
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE CHAIR OF PROCESS CONTROL
 * ENGINEERING BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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











