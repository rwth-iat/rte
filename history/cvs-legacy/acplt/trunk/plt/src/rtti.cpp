/* -*-plt-c++-*- */
/* 
 *  Copyright (c) 1996 PLT, RWTH-Aachen, Germany. See file copy_plt.txt!
 *  Author: Martin Kneissl <martin@plt.rwth-aachen.de>
 *
 */

#include <plt/debug.h>
#include <plt/rtti.hpp>

#if PLT_SIMULATE_RTTI
//////////////////////////////////////////////////////////////////////

PltRTTI::PltRTTI(const char *name, const PltRTTI* bases[])
: n(name), b(bases)
{
    PLT_ASSERT(n);
}

//////////////////////////////////////////////////////////////////////

bool
PltRTTI::isBaseOf(const PltRTTI *pd) const
{
    if (pd->b) {
        for (const PltRTTI **p = pd->b; *p; p++) {
            if ( isSame(*p) || isBaseOf(*p) ) {
                return true;
            }
        }
    }
    return false;
}
    
#endif
