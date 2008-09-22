/* -*-plt-c++-*- */
#ifndef KS_ARRAY_BUILTINS_INCLUDED
#define KS_ARRAY_BUILTINS_INCLUDED
/* $Header: /home/david/cvs/acplt/ks/include/ks/array_builtins.h,v 1.9 2008-09-22 08:26:09 henning Exp $ */
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

#include "plt/debug.h"

#if PLT_COMPILER_MSVC || PLT_COMPILER_CYGWIN
#define PLT_PSEUDO_INLINE inline
#else
#if PLT_COMPILER_GCC && PLT_COMPILER_GCC >= 0x40000
#define PLT_PSEUDO_INLINE inline
#else
#define PLT_PSEUDO_INLINE
#endif
#endif

#define KS_IMPL_ARRAY_XDR(elem,xdre_elem,xdrd_elem)                     \
FEATURE_TEMPL_SPEC                                              \
PLT_PSEUDO_INLINE bool                                                  \
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
FEATURE_TEMPL_SPEC                                              \
PLT_PSEUDO_INLINE bool                                                  \
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

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
FEATURE_TEMPL_SPEC
PLT_PSEUDO_INLINE bool
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
    return xdr_opaque(xdr, p, sz) ? true : false;
}


//////////////////////////////////////////////////////////////////////

FEATURE_TEMPL_SPEC
PLT_PSEUDO_INLINE bool
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
