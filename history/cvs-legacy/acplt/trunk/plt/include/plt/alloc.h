/* -*-plt-c++-*- */
/* $Header: /home/david/cvs/acplt/plt/include/plt/alloc.h,v 1.5 2007-04-25 12:57:21 martin Exp $ */
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
