/* -*-plt-c++-*- */
/* 
 *  Copyright (c) 1996 PLT, RWTH-Aachen, Germany. See file copy_plt.txt!
 *  Author: Martin Kneissl <martin@plt.rwth-aachen.de>
 *
 */

#ifndef PLT_CONFIG_INCLUDED
#define PLT_CONFIG_INCLUDED


#ifndef PLT_SIMULATE_BOOL
#define PLT_SIMULATE_BOOL 0			/* Is bool not defined by the system? */
#endif

#ifndef PLT_SIMULATE_RTTI           /* Do we have to provide RTTI? */
#define PLT_SIMULATE_RTTI 1
#endif

/* End of configuration section */

#if PLT_SIMULATE_BOOL
typedef int bool;
enum { false=0, true=1 };
#endif

#endif // header

