/* -*-plt-c++-*- */
/* $Header: /home/david/cvs/acplt/plt/src/alloc.cpp,v 1.1 1997-09-10 14:52:40 martin Exp $ */
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

//////////////////////////////////////////////////////////////////////

#include "plt/alloc.h"

//////////////////////////////////////////////////////////////////////

PltAllocator_base::node * 
PltAllocator_base::nodelist = 0;

size_t
PltAllocator_base::icount = 0;
//////////////////////////////////////////////////////////////////////

PltAllocator_base::~PltAllocator_base() 
{
    while (objlist) {
        node * p = objlist;
        objlist = objlist->next;
        ::operator delete(p->obj);
        delete p;
    }
    if ((--icount)==0) {
        // last instance
        while (nodelist) {
            node * p = nodelist;
            nodelist = nodelist->next;
            delete p;
        }
    }
}
    
//////////////////////////////////////////////////////////////////////

void *
PltAllocator_base::do_alloc(size_t size) 
{
    if (objlist) {
        // remove node from objlist
        node * p = objlist;
        objlist = objlist->next;
        
        // remember object space pointer
        void * retval = p->obj;
        
        // add node to nodelist
        p->next  = nodelist;
        nodelist = p;
        return retval;
    } else {
        return ::operator new(size);
    }
}

//////////////////////////////////////////////////////////////////////
            
void 
PltAllocator_base::do_free(void *obj)
{
    if (obj == 0) return;

    node * q;
    if (nodelist) {
        q = nodelist;
        nodelist = nodelist->next;
    } else {
        q = new node;
    }
    q->obj = obj;
    q->next = objlist;
    objlist = q;
} 

