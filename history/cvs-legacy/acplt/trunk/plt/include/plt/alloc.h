/* -*-plt-c++-*- */
/* $Header: /home/david/cvs/acplt/plt/include/plt/alloc.h,v 1.1 1997-09-10 14:51:29 martin Exp $ */
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
v *    name of the Aachen University of Technology may be used to endorse or
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
#ifndef PLT_ALLOC_INCLUDED
#define PLT_ALLOC_INCLUDED

#include "plt/debug.h"

// private class

class PltAllocator_base
{
    class node
        {
        public:
            void * obj;
            node * next;
            node() : obj(0), next(0) { }
        };
    node * objlist;
    static node * nodelist;
    static size_t icount;
protected:
    PltAllocator_base() : objlist(0) { ++icount; }
    ~PltAllocator_base();

    PltAllocator_base(const PltAllocator_base &); // forbidden
    PltAllocator_base & operator = (const PltAllocator_base &); // forbidden

    void * do_alloc(size_t size);
    void do_free(void * ptr);
};

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
    
template <class T>
class PltAllocator
: private PltAllocator_base
{
public:
    PltAllocator() : PltAllocator_base() { }
    PltAllocator(const PltAllocator<T> &); // forbidden
    PltAllocator<T> & operator = (const PltAllocator<T> &); // forbidden

    void * alloc() { return do_alloc(sizeof (T)); }
    void free(void *ptr) { do_free(ptr); }
};

//////////////////////////////////////////////////////////////////////

#endif // PLT_ALLOC_INCLUDED
