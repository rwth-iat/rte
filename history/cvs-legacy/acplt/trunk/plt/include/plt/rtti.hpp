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

#define PLT_DYNAMIC_PCAST(T,p) \
           ((&T::RTTI)->canCastFrom(p->getRTTI()) ? (T*)((p)) : 0)
#else // PLT_SIMULATE_RTTI
#define PLT_DYNAMIC_PCAST(T,p) \
           (dynamic_cast<T*> (p))
#endif // PLT_SIMULATE_RTTI

#endif // header


