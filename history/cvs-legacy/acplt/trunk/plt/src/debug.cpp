/* -*-plt-c++-*- */
/* File plt/debug.cpp */
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
/* Author: Martin Kneissl <martin@plt.rwth-aachen.de> */

#include "plt/debug.h"

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


//////////////////////////////////////////////////////////////////////
#if PLT_DEBUG_NEW
//////////////////////////////////////////////////////////////////////

#include <iomanip.h>

bool
PltDebugNewTracker::report_always = PLT_DEBUG_NEW_REPORT;

size_t 
PltDebugNewTracker::refcount = 0;

size_t 
PltDebugNewTracker::newcount = 0;

size_t 
PltDebugNewTracker::deletecount = 0;

size_t 
PltDebugNewTracker::newed = 0;

size_t 
PltDebugNewTracker::deleted = 0;

//////////////////////////////////////////////////////////////////////

PltDebugNewTracker::PltDebugNewTracker()
{
    if (refcount++ == 0) {
        newcount = 0;
        deletecount = 0;
        newed = 0;
        deleted = 0;
    }        
}

//////////////////////////////////////////////////////////////////////

PltDebugNewTracker::~PltDebugNewTracker()
{
    if (--refcount == 0) {
        // last instance being destroyed
        if (report_always || newed != deleted || newcount != deletecount) {
            cerr << endl;
            cerr << "Free store statistics:" << endl;
            cerr << endl;
            cerr << "Calls to ::new:    " << setw(10) << newcount << endl;
            if (deletecount != newcount) {
                cerr << "Calls to ::delete: " 
                     << setw(10) 
                     << deletecount 
                     << endl;
            }
            cerr << "Bytes newed:       " << setw(10) << newed << endl;
            if (newed != deleted) {
                cerr << "Bytes deleted:     " 
                     << setw(10) 
                     << deleted 
                     << endl;
            }
        }
    }
};

//////////////////////////////////////////////////////////////////////

#ifndef PLT_ALIGNMENT
#define PLT_ALIGNMENT 8
#endif

#define PLT_ALIGNTO(m,x)  ( ( ((x)-1)/(m) + 1 ) * (m) )

#define PLT_ALIGN(x)  PLT_ALIGNTO(PLT_ALIGNMENT,x)

//////////////////////////////////////////////////////////////////////

struct PltDebugNewHeader
{
    static const unsigned long magic_salt;
    unsigned long magic;
    size_t size;
};
const unsigned long 
PltDebugNewHeader::magic_salt = 0x471142;

//////////////////////////////////////////////////////////////////////

void * operator new(size_t sz) {
    PLT_ASSERT(sizeof(PltDebugNewHeader) % PLT_ALIGNMENT == 0);
    PltDebugNewHeader * p = 
        (PltDebugNewHeader *)
            malloc(sizeof(PltDebugNewHeader)+sz);
    if (p) {
         p->magic = PltDebugNewHeader::magic_salt;
         p->size  = sz;
         ++PltDebugNewTracker::newcount;
         PltDebugNewTracker::newed += sz;
         return p + 1;
    } else {
        return 0;
    }
}

//////////////////////////////////////////////////////////////////////

void operator delete(void * p) {
    if (p) {
        PltDebugNewHeader * pheader = (PltDebugNewHeader*)p - 1;
        PLT_ASSERT(pheader->magic == PltDebugNewHeader::magic_salt);
        PltDebugNewTracker::deleted += pheader->size;
        ++PltDebugNewTracker::deletecount;
        free(pheader);
    }
}

//////////////////////////////////////////////////////////////////////

void * operator new[](size_t sz) 
{
    return operator new(sz);
}

//////////////////////////////////////////////////////////////////////

void operator delete[](void * p)
{
    operator delete(p);
}

//////////////////////////////////////////////////////////////////////
#endif //PLT_DEBUG_NEW
//////////////////////////////////////////////////////////////////////

#endif // PLT_DEBUG

/* EOF plt/debug.cpp */
