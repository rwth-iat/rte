/* -*-plt-c++-*- 
 * 
 *  Copyright (c) 1996 PLT, RWTH-Aachen, Germany. See file copy_plt.txt!
 *  Author: Martin Kneissl <martin@plt.rwth-aachen.de>
 *
 */

#ifndef PLT_CONFIG_INCLUDED
#define PLT_CONFIG_INCLUDED


#ifndef PLT_HAS_BOOL
#define PLT_HAS_BOOL 0			/* Is bool defined by the system? */
#endif

/* End of configuration section */

#if !PLT_HAS_BOOL
typedef int bool;
enum { false=0, true=1 };
#endif

#endif
