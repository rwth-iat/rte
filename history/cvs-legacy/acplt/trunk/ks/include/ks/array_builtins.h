/* -*-plt-c++-*- */
#ifndef KS_ARRAY_BUILTINS_INCLUDED
#define KS_ARRAY_BUILTINS_INCLUDED
/* $Header: /home/david/cvs/acplt/ks/include/ks/array_builtins.h,v 1.1 1997-04-10 14:17:35 martin Exp $ */
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

#include "plt/debug.h"

#define KS_IMPL_ARRAY_XDR(elem,xdre_elem,xdrd_elem)                     \
bool                                                                    \
KsArray<elem>::xdrDecode(XDR *xdr)                                      \
{                                                                       \
    PLT_PRECONDITION(xdr->x_op == XDR_DECODE);                          \
    /* retrieve size */                                                 \
    u_long sz;                                                          \
    if (! ks_xdrd_u_long(xdr, &sz) ) return false;                      \
                                                                        \
    /* adjust array size (possibly losing contents) */                  \
                                                                        \
    if (size() != sz) {                                                 \
        /* allocate sz elements */                                      \
        PltArrayHandle<elem> ha(new elem[sz], PltOsArrayNew);           \
        if (!ha) return false; /* failed */                             \
        a_array = ha;                                                   \
        a_size = sz;                                                    \
    }                                                                   \
    PLT_ASSERT(size() == sz);                                           \
                                                                        \
    /* now deserialize elements */                                      \
                                                                        \
    for (size_t i=0; i < a_size; ++i) {                                 \
        if (! xdrd_elem(xdr, & a_array[i] ) ) return false;             \
    }                                                                   \
                                                                        \
    /* success */                                                       \
                                                                        \
    return true;                                                        \
}                                                                       \
                                                                        \
/****************************************************************/      \
                                                                        \
bool                                                                    \
KsArray<elem>::xdrEncode(XDR *xdrs) const                               \
{                                                                       \
    PLT_PRECONDITION(xdrs->x_op == XDR_ENCODE);                         \
    /* serialize size */                                                \
                                                                        \
    u_long sz = a_size;                                                 \
    if (! ks_xdre_u_long(xdrs, &sz)) return false;                      \
                                                                        \
    /* serialize elements */                                            \
                                                                        \
    for (size_t i = 0; i < a_size; ++i) {                               \
        if (! xdre_elem(xdrs, & a_array[i]) ) return false;             \
    }                                                                   \
                                                                        \
    /* success */                                                       \
                                                                        \
    return true;                                                        \
    }                                                                   \
typedef void ks_dummy_typedef

//////////////////////////////////////////////////////////////////////

#define KS_IMPL_ARRAY(elem)                                        \
    KS_IMPL_ARRAY_XDR(elem, ks_xdre_##elem,ks_xdrd_##elem);

#if 0
    bool KsArray<elem>::xdrEncode(XDR *) const;                    \
    bool KsArray<elem>::xdrDecode(XDR *)
#endif 

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

bool
KsArray<char>::xdrDecode(XDR *xdr)
{
    PLT_PRECONDITION(xdr->x_op == XDR_DECODE);
    /* retrieve size */
    u_long sz;
    if (! ks_xdrd_u_long(xdr, &sz) ) return false;

    /* adjust array size (possibly losing contents) */

    if (size() != sz) {
        /* allocate sz elements */
        PltArrayHandle<char> ha(new char[sz], PltOsArrayNew);
        if (!ha) return false; /* failed */
        a_array = ha;
        a_size = sz;
    }
    PLT_ASSERT(size() == sz);

    /* now deserialize elements */
    char * p = a_array.getPtr();
    return xdr_opaque(xdr, p, sz);
}


//////////////////////////////////////////////////////////////////////

bool
KsArray<char>::xdrEncode(XDR *xdr) const
{
    PLT_PRECONDITION(xdr->x_op == XDR_ENCODE);
    
    char * p = a_array.getPtr();
    u_long sz = a_size;

    return ks_xdre_u_long(xdr, &sz) 
        && xdr_opaque(xdr, p, sz);
}


/////////////////////////////////////////////////////////////////////

// define XDR types

KS_IMPL_ARRAY(long);
KS_IMPL_ARRAY(u_long);
KS_IMPL_ARRAY(int);
KS_IMPL_ARRAY(u_int);
KS_IMPL_ARRAY(short);
KS_IMPL_ARRAY(u_short);
KS_IMPL_ARRAY(float);
KS_IMPL_ARRAY(double);
#if !PLT_SIMULATE_BOOL
KS_IMPL_ARRAY(bool);
#endif

#endif // KS_ARRAY_BUILTINS_INCLUDED
