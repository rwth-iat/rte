/* -*-plt-c++-*- */
/* 
 *  Copyright (c) 1996 PLT, RWTH-Aachen, Germany. See file copy_plt.txt!
 *  Author: Martin Kneissl <martin@plt.rwth-aachen.de>
 *
 */

#ifndef PLT_RTTI_INCLUDED
#define PLT_RTTI_INCLUDED

#include <plt/debug.h>

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

#endif // header


