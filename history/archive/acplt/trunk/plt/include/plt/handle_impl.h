/* -*-plt-c++-*- */
#ifndef PLT_HANDLE_IMPL_INCLUDED
#define PLT_HANDLE_IMPL_INCLUDED
/* $Header: /home/david/cvs/acplt/plt/include/plt/handle_impl.h,v 1.7 2007-04-25 12:57:21 martin Exp $ */
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

//////////////////////////////////////////////////////////////////////
// Handle template implementation
//////////////////////////////////////////////////////////////////////

#if !PLT_SEE_ALL_TEMPLATES
#include "plt/handle.h"
#endif

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

template <class T>
bool
PltHandle<T>::bindTo(T * p, enum PltOwnership t)
{
    PLT_PRECONDITION(t==PltOsUnmanaged || t==PltOsMalloc 
                     || t==PltOsNew || t==PltOsArrayNew);
    if (p == 0 || t == PltOsUnmanaged) {
        PltHandle_base::bindTo(p,0);
        return true;
    } else {
        Plt_AllocTracker *a;
        switch (t) {
        case PltOsMalloc:
            a = new Plt_AtMalloc;
            break;
        case PltOsNew:
            a = new Plt_AtNew<T>;
            break;
        case PltOsArrayNew:
            a = new Plt_AtArrayNew<T>;
            break;
        case PltOsUnmanaged:
        default:
            a = 0;
            PLT_ASSERT(0==1);
        }
        if (a) {
            PltHandle_base::bindTo(p,a);
            return true;
        } else {
            return false;
        }
    }
}

//////////////////////////////////////////////////////////////////////

template <class T>
PltHandle<T>::PltHandle(T *p, enum PltOwnership os) 
{
    PLT_PRECONDITION(os==PltOsUnmanaged || os==PltOsMalloc 
                     || os==PltOsNew || os==PltOsArrayNew);
    if (! bindTo(p, os)) {
        switch (os) {
        case PltOsMalloc:
            free(p);
            break;
        case PltOsNew:
            delete p;
            break;
        case PltOsArrayNew:
            delete [] p;
            break;
        case PltOsUnmanaged:
            // do nothing
            break;
        }
    }
    PLT_CHECK_INVARIANT();
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
#endif
// plt/handle_impl.h
