/* -*-plt-c++-*- */
#ifndef KS_ARRAY_INCLUDED
#define KS_ARRAY_INCLUDED
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

//////////////////////////////////////////////////////////////////////
// Author: Martin Kneissl <martin@plt.rwth-aachen.de>
//////////////////////////////////////////////////////////////////////

#include "plt/array.h"
#include "ks/xdr.h"

//////////////////////////////////////////////////////////////////////
#if 0
// just for reference
template <class T>
class KsArray
: public PltArray<T>,
  public KsXdrAble
{
 public:
    KsArray(size_t size = 0); // array size          // [1]
    KsArray(size_t size,      // array size 
             T * p,            // address of first element
             enum PltOwnership);                      // [1]
    
    // KsXdrAble interface
    KsArray(XDR *, bool & success);
    virtual bool xdrEncode(XDR *) const;  // serialize
    virtual bool xdrDecode(XDR *);        // deserialize
    static KsArray * xdrNew(XDR *);   // deserialization factory meth. 
};
#endif
//////////////////////////////////////////////////////////////////////
// IMPLEMENTATION
//////////////////////////////////////////////////////////////////////

#define KS_DECL_ARRAY_BODY(T)                                        \
: public PltArray<T>,                                                \
  public KsXdrAble                                                   \
{                                                                    \
 public:                                                             \
    KsArray(size_t size = 0);                                        \
    KsArray(size_t size,                                             \
             T * p,                                                  \
             enum PltOwnership);                                     \
                                                                     \
    KsArray(XDR *, bool & success);                                  \
    virtual bool xdrEncode(XDR *) const;                             \
    virtual bool xdrDecode(XDR *);                                   \
    static KsArray * xdrNew(XDR *);                                  \
}

//////////////////////////////////////////////////////////////////////
// Implementation
//////////////////////////////////////////////////////////////////////

template <class T>
class KsArray
KS_DECL_ARRAY_BODY(T);

//////////////////////////////////////////////////////////////////////

#define KS_DECL_ARRAY(T) \
     class KsArray<T> KS_DECL_ARRAY_BODY(T)

//////////////////////////////////////////////////////////////////////

template <class T>
inline
KsArray<T>::KsArray(size_t size, T *p, enum PltOwnership os)
: PltArray<T>(size, p, os)
{
}

//////////////////////////////////////////////////////////////////////

template <class T>
inline
KsArray<T>::KsArray(size_t size)
: PltArray<T>(size)
{
}

//////////////////////////////////////////////////////////////////////

template <class T>
inline bool
KsArray<T>::xdrDecode(XDR *xdr)
{
    PLT_PRECONDITION(xdr->x_op == XDR_DECODE);
    // retrieve size
    //
    u_long sz;
    if (! xdr_u_long(xdr, &sz) ) return false; // failed

    // adjust array size (possibly losing contents)
    //
    if (size() != sz) {
        // allocate sz elements
        PltArrayHandle<T> ha(new T[sz], PltOsArrayNew);
        if (!ha) return false; // failed
        a_array = ha;
        a_size = sz;
    }
    PLT_ASSERT(size() == sz);

    // now deserialize elements
    //
    for (size_t i=0; i < a_size; ++i) {
        if (! a_array[i].xdrDecode(xdr) ) return false; // failed
    }
    
    // success
    //
    return true;
}

//////////////////////////////////////////////////////////////////////

template <class T>
inline bool
KsArray<T>::xdrEncode(XDR *xdrs) const
{
    PLT_PRECONDITION(xdrs->x_op == XDR_ENCODE);
    // serialize size
    //
    u_long sz = a_size;
    if (! xdr_u_long(xdrs, &sz)) return false; // fail

    // serialize elements
    //
    for (size_t i = 0; i < a_size; ++i) {
        if (! a_array[i].xdrEncode(xdrs)) return false; // fail
    }

    // success
    //
    return true;
}
            
//////////////////////////////////////////////////////////////////////

template <class T>
inline
KsArray<T>::KsArray(XDR * xdr, bool & ok)
{
    PLT_PRECONDITION(xdr->x_op == XDR_DECODE);
    ok = xdrDecode(xdr);
}

//////////////////////////////////////////////////////////////////////

template <class T>
inline KsArray<T> *
KsArray<T>::xdrNew(XDR *xdrs)
{
    PLT_PRECONDITION(xdrs->x_op == XDR_DECODE);
    bool ok;                                         
    KsArray<T> * p = new KsArray<T>(xdrs, ok);     
    if ( !ok && p) {                             
        delete p;                                
        p = 0;                                   
    }                                            
    return p;                                    
}                                                

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
// MEGA-define follows

#define KS_IMPL_ARRAY_XDR(elem, xdr_elem)                               \
inline bool                                                             \
KsArray<elem>::xdrDecode(XDR *xdr)                                      \
{                                                                       \
    PLT_PRECONDITION(xdr->x_op == XDR_DECODE);                          \
    /* retrieve size */                                                 \
    u_long sz;                                                          \
    if (! xdr_u_long(xdr, &sz) ) return false;                          \
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
        if (! xdr_elem(xdr, & a_array[i] ) ) return false;              \
    }                                                                   \
                                                                        \
    /* success */                                                       \
                                                                        \
    return true;                                                        \
}                                                                       \
                                                                        \
/****************************************************************/      \
                                                                        \
inline bool                                                             \
KsArray<elem>::xdrEncode(XDR *xdrs) const                               \
{                                                                       \
    PLT_PRECONDITION(xdrs->x_op == XDR_ENCODE);                         \
    /* serialize size */                                                \
                                                                        \
    u_long sz = a_size;                                                 \
    if (! xdr_u_long(xdrs, &sz)) return false;                          \
                                                                        \
    /* serialize elements */                                            \
                                                                        \
    for (size_t i = 0; i < a_size; ++i) {                               \
        if (! xdr_elem(xdrs, & a_array[i]) ) return false;              \
    }                                                                   \
                                                                        \
    /* success */                                                       \
                                                                        \
    return true;                                                        \
    }                                                                   \
typedef void ks_dummy_typedef

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

#define KS_IMPL_ARRAY_BASICS(elem)                                      \
                                                                        \
inline                                                                  \
KsArray<elem>::KsArray(size_t size, elem *p, enum PltOwnership os)      \
: PltArray<elem>(size, p, os)                                           \
{                                                                       \
}                                                                       \
                                                                        \
/****************************************************************/      \
                                                                        \
inline                                                                  \
KsArray<elem>::KsArray(size_t size)                                     \
: PltArray<elem>(size)                                                  \
{                                                                       \
}                                                                       \
                                                                        \
/****************************************************************/      \
                                                                        \
inline                                                                  \
KsArray<elem>::KsArray(XDR * xdr, bool & ok)                            \
{                                                                       \
    PLT_PRECONDITION(xdr->x_op == XDR_DECODE);                          \
    ok = xdrDecode(xdr);                                                \
}                                                                       \
                                                                        \
/****************************************************************/      \
                                                                        \
inline KsArray<elem> *                                                  \
KsArray<elem>::xdrNew(XDR *xdrs)                                        \
{                                                                       \
    PLT_PRECONDITION(xdrs->x_op == XDR_DECODE);                         \
    bool ok;                                                            \
    KsArray<elem> * p = new KsArray<elem>(xdrs, ok);                    \
    if ( !ok && p) {                                                    \
        delete p;                                                       \
        p = 0;                                                          \
    }                                                                   \
    return p;                                                           \
    }                                                                   \
typedef void ks_dummy_typedef

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

#define KS_IMPL_ARRAY(elem)                     \
    KS_IMPL_ARRAY_XDR(elem, xdr_##elem);        \
    KS_IMPL_ARRAY_BASICS(elem);

#define KS_GEN_ARRAY(elem) \
    KS_DECL_ARRAY(elem);   \
    KS_IMPL_ARRAY(elem)

//////////////////////////////////////////////////////////////////////

KS_GEN_ARRAY(long);
KS_GEN_ARRAY(u_long);
KS_GEN_ARRAY(float);
KS_GEN_ARRAY(double);

//////////////////////////////////////////////////////////////////////

static inline bool_t
KsArray_xdr_bool(XDR *xdr, bool * pb)
// better save than sorry
{
    PLT_PRECONDITION(pb 
                     && 
                     (xdr->x_op == XDR_ENCODE || xdr->x_op == XDR_DECODE));
    switch (xdr->x_op) {
    case XDR_ENCODE: 
        {
            bool_t tmp = *pb;
            return xdr_bool(xdr, &tmp);
        } 
    case XDR_DECODE:
        {
            bool_t tmp;
            bool_t res = xdr_bool(xdr, &tmp);
            *pb = tmp;
            return res;
        }
    default:
        return true;
    }
}

//////////////////////////////////////////////////////////////////////

KS_DECL_ARRAY(bool);
KS_IMPL_ARRAY_XDR(bool, KsArray_xdr_bool);
KS_IMPL_ARRAY_BASICS(bool);


//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

KS_DECL_ARRAY(char);

//////////////////////////////////////////////////////////////////////

inline bool
KsArray<char>::xdrDecode(XDR *xdr)
{
    PLT_PRECONDITION(xdr->x_op == XDR_DECODE);
    /* retrieve size */
    u_long sz;
    if (! xdr_u_long(xdr, &sz) ) return false;

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

inline bool
KsArray<char>::xdrEncode(XDR *xdr) const
{
    PLT_PRECONDITION(xdr->x_op == XDR_ENCODE);
    
    char * p = a_array.getPtr();
    u_long sz = a_size;

    return xdr_u_long(xdr, &sz) 
        && xdr_opaque(xdr, p, sz);
}


//////////////////////////////////////////////////////////////////////

KS_IMPL_ARRAY_BASICS(char);

//////////////////////////////////////////////////////////////////////

#endif // KS_ARRAY_INCLUDED

