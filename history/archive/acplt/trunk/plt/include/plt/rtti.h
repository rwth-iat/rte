/* -*-plt-c++-*- */
#ifndef PLT_RTTI_INCLUDED
#define PLT_RTTI_INCLUDED
/* $Header: /home/david/cvs/acplt/plt/include/plt/rtti.h,v 1.6 2007-04-25 12:57:21 martin Exp $ */
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

#include "plt/debug.h"

#if PLT_SIMULATE_RTTI

//////////////////////////////////////////////////////////////////////
// Runtime type information is still not present in all 
// implementations of C++. Because the dynamic_cast feature
// is sometimes needed we provide some simulation of RTTI.
// This simulation needs help from class implementors.
//
// What will you have to do for every class containing new virtual 
// functions that might be referred to by pointers to base classes?
//
// You have to:
// - create a static null terminated array containing 
//   pointers to PltRTTI objects of all base classes.
// - Provide a static data member of class PltRTTI.
// - Bracket this code with #if PLT_SIMULATE_RTTI / #endif
// 
// TODO: Some example to make this clear.
//////////////////////////////////////////////////////////////////////


class PltRTTI {
public:
    PltRTTI(const char *name,const PltRTTI* bases[]);
    bool isSame(const PltRTTI *) const;
    bool isBaseOf(const PltRTTI *) const;
    bool canCastFrom(const PltRTTI *) const;
    const char *name() const;
private:
    const char *n;
    const PltRTTI **b;
};


//////////////////////////////////////////////////////////////////////
// INLINE IMPLEMENTATION
//////////////////////////////////////////////////////////////////////

inline bool
PltRTTI::isSame(const PltRTTI *p) const
{
    return this==p;
}

//////////////////////////////////////////////////////////////////////

inline bool
PltRTTI::canCastFrom(const PltRTTI *p) const
{
    return isSame(p) || isBaseOf(p);
}

//////////////////////////////////////////////////////////////////////
// MACROS
//////////////////////////////////////////////////////////////////////

#define PLT_DYNAMIC_PCAST(T,p)                                         \
           ((&T::RTTI)->canCastFrom((p)->getRTTI()) ? (T*)((p)) : 0)

#if 0
#define PLT_DYNAMIC_RCAST(T,r)                                         \
           (PLT_ASSERT((&T::RTTI)->canCastFrom((&(r))->getRTTI())),    \
           (T&)((r)))
#endif

#define PLT_DECL_RTTI                                                  \
     public:                                                           \
         virtual const PltRTTI* getRTTI() const                        \
             { return &RTTI; }                                         \
         static const PltRTTI RTTI

#define PLT_IMPL_RTTI0(clsname)                                        \
     const PltRTTI clsname::RTTI(#clsname,0)

#define PLT_IMPL_RTTI1(clsname,base1)                                  \
     static const PltRTTI * clsname##_bases [] = {                     \
         &base1::RTTI,                                                 \
         0 };                                                          \
     const PltRTTI clsname::RTTI(#clsname,clsname##_bases)

#define PLT_IMPL_RTTI2(clsname,base1,base2)                            \
     static const PltRTTI * clsname##_bases [] = {                     \
         &base1::RTTI,                                                 \
         &base2::RTTI,                                                 \
         0 };                                                          \
     const PltRTTI clsname::RTTI(#clsname,clsname##_bases)

#define PLT_IMPL_RTTI3(clsname,base1,base2,base3)                      \
     static const PltRTTI * clsname##_bases [] = {                     \
         &base1::RTTI,                                                 \
         &base2::RTTI,                                                 \
         &base3::RTTI,                                                 \
         0 };                                                          \
     const PltRTTI clsname::RTTI(#clsname,clsname##_bases)

#else // PLT_SIMULATE_RTTI

#define PLT_DECL_RTTI typedef void plt_rtti_dummy
#define PLT_IMPL_RTTI0(c) typedef void plt_rtti_dummy
#define PLT_IMPL_RTTI1(c,b1) typedef void plt_rtti_dummy
#define PLT_IMPL_RTTI2(c,b1,b2) typedef void plt_rtti_dummy
#define PLT_IMPL_RTTI3(c,b1,b2,b3) typedef void plt_rtti_dummy

#define PLT_DYNAMIC_PCAST(T,p) \
           (dynamic_cast<T*> (p))

#endif // PLT_SIMULATE_RTTI

#endif // PLT_RTTI_INCLUDED


