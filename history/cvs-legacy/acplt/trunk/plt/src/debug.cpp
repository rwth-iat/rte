/* -*-plt-c++-*- */
/* 
 *  Copyright (c) 1996 PLT, RWTH-Aachen, Germany. See file copy_plt.txt!
 *  Author: Martin Kneissl <martin@plt.rwth-aachen.de>
 *
 */

#include <plt/debug.h>

#if PLT_DEBUG
#include <iostream.h>
#include <stdlib.h>

void plt_canthappen(const char *what, const char *file, int line)
	// some programming error has occured.
    // report this and abort.
{
    cerr << "This is a bug! Please report it to your maintainer." << endl;
    cerr << "I have to abort execution now." << endl;
    cerr << what << endl;
    cerr << file << ": " << line << endl;
    abort();
}

#endif
