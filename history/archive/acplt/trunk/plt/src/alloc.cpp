/* -*-plt-c++-*- */
/* $Header: /home/david/cvs/acplt/plt/src/alloc.cpp,v 1.7 2007-04-25 12:57:21 martin Exp $ */
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

//////////////////////////////////////////////////////////////////////

#include "plt/alloc.h"

//////////////////////////////////////////////////////////////////////

PltAllocator_base::node * 
PltAllocator_base::nodelist = 0;

size_t
PltAllocator_base::icount = 0;


// ----------------------------------------------------------------------------
//
PltAllocator_base::~PltAllocator_base() 
{
    while (objlist) {
        node * p = objlist;
        objlist = objlist->next;
#if 0 || PLT_DEBUG_PEDANTIC
	cerr << "Freeing 0x" << hex << (unsigned long) p->obj << endl;
#endif	
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
} // PltAllocator_base::~PltAllocator_base
    

// ----------------------------------------------------------------------------
//
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
#if 0 || PLT_DEBUG_PEDANTIC
	cerr << "Reusing 0x" << hex << (unsigned long) retval << endl;
#endif	
        return retval;
    } else {
	void * retval = ::operator new(size);
#if PLT_DEBUG_PEDANTIC
	cerr << "Allocating 0x" << hex << (unsigned long) retval << endl;
#endif	
        return retval;
    }
} // PltAllocator_base::do_alloc

            
// ----------------------------------------------------------------------------
//
void 
PltAllocator_base::do_free(void *obj)
{
    if (obj == 0) return;

#if 0 || PLT_DEBUG_PEDANTIC
	cerr << "Returning 0x" << hex << (unsigned long) obj << endl;
#endif	

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
} // PltAllocator_base::do_free

/* End of plt/alloc.cpp */
