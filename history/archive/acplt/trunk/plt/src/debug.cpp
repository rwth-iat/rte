/* -*-plt-c++-*- */
/* File plt/debug.cpp */
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
#include "plt/log.h"

#if PLT_DEBUG
#include <iostream.h>
#include <stdlib.h>

void plt_canthappen(const char *what, const char *file, int line)
	// some programming error has occured.
    // report this and abort.
{
    PltLog::Alert("This is a bug! Please report it to your maintainer.");
    PltLog::Alert("I have to abort execution now.");
    PltLog::Alert(what);
    plt_dlog << file << ": " << line;
    plt_dlog.alert();
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

size_t 
PltDebugNewTracker::max = 0;


//////////////////////////////////////////////////////////////////////

inline size_t
PltDebugNewTracker::inuse()
{
    return newed-deleted;
}

//////////////////////////////////////////////////////////////////////

PltDebugNewTracker::PltDebugNewTracker()
{
    if (refcount++ == 0) {
        newcount = 0;
        deletecount = 0;
        newed = 0;
        deleted = 0;
        max = 0;
    }        
}

//////////////////////////////////////////////////////////////////////

PltDebugNewTracker::~PltDebugNewTracker()
{
    if (--refcount == 0) {
        // last instance being destroyed
        size_t max = PltDebugNewTracker::max;
        size_t newed = PltDebugNewTracker::newed;
        size_t deleted = PltDebugNewTracker::deleted;
        size_t newcount = PltDebugNewTracker::newcount;
        size_t deletecount = PltDebugNewTracker::deletecount;
        size_t inuse = PltDebugNewTracker::inuse();
#if 0
        // Under LINUX class ostrstream as base class of PltLogStream
        // returned always zero strings. Therefore the implementation
        // was changed.
        if (report_always || newed != deleted || newcount != deletecount) {
            plt_dlog << "Free store statistics:";
            plt_dlog.debug();
            plt_dlog << "Max. bytes used:   " << setw(10) << max;
            plt_dlog.debug();
            plt_dlog << "Calls to ::new:    " << setw(10) << newcount;
            plt_dlog.debug();
            if (deletecount != newcount) {
                plt_dlog << "Calls to ::delete: " 
                         << setw(10) 
                         << deletecount;
                plt_dlog.debug();
            }
            plt_dlog << "Bytes newed:       " << setw(10) << newed;
            plt_dlog.debug();
            if (newed != deleted) {
                plt_dlog << "Bytes deleted:     " 
                         << setw(10) 
                         << deleted;
                plt_dlog.debug();
                plt_dlog << "Lost bytes:        " 
                         << setw(10) 
                         << inuse;
                plt_dlog.debug();
            }
        }
#else
        if (report_always || newed != deleted || newcount != deletecount) {
            char buf[500]; 
            ostrstream str(buf, 500);
            str << endl << "Free store statistics:" << endl;
            str << "Max. bytes used:   " << setw(10) << max << endl;
            str << "Calls to ::new:    " << setw(10) << newcount << endl;
            if (deletecount != newcount) {
                str << "Calls to ::delete: " 
                    << setw(10) 
                    << deletecount << endl;
            }
            str << "Bytes newed:       " << setw(10) << newed << endl;
            if (newed != deleted) {
                str << "Bytes deleted:     " 
                    << setw(10) 
                    << deleted << endl;
                str << "Lost bytes:        " 
                    << setw(10) 
                    << inuse << endl;
            }
            str << ends;
            // PltLog::Debug(str.str());
            cerr << str.str() << endl;
        }
#endif
    }
}

/////////////////////////////////////////////////////////////////////////////

void 
PltDebugNewTracker::memState(PltDebugMemState &state)
{
    state.newcount = newcount;
    state.deletecount = deletecount;
    state.newed = newed;
    state.deleted = deleted;
}

//////////////////////////////////////////////////////////////////////

#ifndef PLT_ALIGNMENT
#define PLT_ALIGNMENT 8
#endif

#define PLT_ALIGNTO(m,x)  ( ( ((x)-1)/(m) + 1 ) * (m) )

#define PLT_ALIGN(x)  PLT_ALIGNTO(PLT_ALIGNMENT,x)

//////////////////////////////////////////////////////////////////////

#if PLT_AVOID_DELETE_BUG
static const char HEADERSTART[] = "g++drivesmenuts";
//---------------------------------0123456789ABCDEF
#endif

static const unsigned long 
magic = 0x471142;

//////////////////////////////////////////////////////////////////////

struct PltDebugNewHeader
{
#if PLT_AVOID_DELETE_BUG
    char headerstart[16];
#endif
    unsigned long magic;
    size_t size;
    double dummy;
};

 //////////////////////////////////////////////////////////////////////

void * operator new(size_t sz) {
    PLT_ASSERT(sizeof(PltDebugNewHeader) % PLT_ALIGNMENT == 0);

    if (sz == 0) sz = 1; // avoid malloc problems

    PltDebugNewHeader * p = 
        (PltDebugNewHeader *)
            malloc(sizeof(PltDebugNewHeader)+sz);
    if (p) {
#if PLT_AVOID_DELETE_BUG
        PLT_ASSERT(sizeof(p->headerstart) == sizeof HEADERSTART);
        memcpy(p->headerstart, HEADERSTART, sizeof HEADERSTART);
#endif
        p->magic = magic;
        p->size  = sz;
        ++PltDebugNewTracker::newcount;
        PltDebugNewTracker::newed += sz;
        if (PltDebugNewTracker::max < PltDebugNewTracker::inuse() ) {
            PltDebugNewTracker::max = PltDebugNewTracker::inuse();
        }
        return p + 1;
    } else {
        return 0;
    }
}

//////////////////////////////////////////////////////////////////////

void operator delete(void * p) {
    char *pc = (char*)p;
    if (pc) {
        pc -= sizeof (PltDebugNewHeader);
#if PLT_AVOID_DELETE_BUG
        // scan for start of header
        while (0 != memcmp(pc,HEADERSTART,sizeof HEADERSTART) ) {
            --pc;
        }
#endif
        p = pc;
        PltDebugNewHeader * pheader = (PltDebugNewHeader*)p;

        if (PltDebugNewTracker::refcount>0) {
            PLT_ASSERT(pheader->magic == magic);
            size_t sz = pheader->size;
#if 0
            PLT_ASSERT( PltDebugNewTracker::refcount == 0
                       || (      PltDebugNewTracker::deleted + sz
                              <= PltDebugNewTracker::newed
                           &&    PltDebugNewTracker::deletecount 
                              <  PltDebugNewTracker::newcount));
#endif
            ++PltDebugNewTracker::deletecount;
            PltDebugNewTracker::deleted += sz;
            free(pheader);
        }
    }
}

//////////////////////////////////////////////////////////////////////
#if PLT_ARRAY_NEW_OVERLOADABLE

void * operator new[](size_t sz) 
{
    return operator new(sz);
}

//////////////////////////////////////////////////////////////////////

void operator delete[](void * p)
{
    operator delete(p);
}

#endif
//////////////////////////////////////////////////////////////////////
#endif //PLT_DEBUG_NEW
//////////////////////////////////////////////////////////////////////

#if PLT_DEBUG
PltLogStream plt_dlog;
#endif

#endif // PLT_DEBUG

/* EOF plt/debug.cpp */
