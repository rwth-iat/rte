/* -*-plt-c++-*- */
#ifndef PLT_LIST_IMPL_INCLUDED
#define PLT_LIST_IMPL_INCLUDED
/* $Header: /home/david/cvs/acplt/plt/include/plt/list_impl.h,v 1.5 1997-04-10 14:09:26 martin Exp $ */
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


//////////////////////////////////////////////////////////////////////

#if !PLT_SEE_ALL_TEMPLATES
#include "plt/list.h"
#include "plt/container_impl.h"
#endif

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

template<class T>
PltList<T>::~PltList() 
{
    while (!isEmpty()) removeFirst();
}

//////////////////////////////////////////////////////////////////////

template<class T>
bool 
PltList<T>::addFirst(const T& t) 
{
    PltListNode<T> *p = new PltListNode<T>(t);
    if (p) {
        return PltList_base::addFirst(p); 
    } else {
        return false;
    }
}

//////////////////////////////////////////////////////////////////////

template<class T>
bool 
PltList<T>::addLast(const T & t) 
{ 
    PltListNode<T> *p = new PltListNode<T>(t);
    if (p) {
        return PltList_base::addLast(p); 
    } else {
        return false;
    }
}

//////////////////////////////////////////////////////////////////////

template<class T>
T 
PltList<T>::removeFirst() 
{
  PltListNode<T> * p = (PltListNode<T> *) PltList_base::removeFirst();
  T t(p->info);
  delete p;
  return t;
}

//////////////////////////////////////////////////////////////////////

template<class T>
T 
PltList<T>::removeLast() 
{
  PltListNode<T> * p = (PltListNode<T> *) PltList_base::removeLast();
  T t(p->info);
  delete p;
  return t;
}


//////////////////////////////////////////////////////////////////////

template<class T>
bool
PltList<T>::remove(const T & t) 
{
    for (PltListNode_base *p = first; p; ++p) {
        PltListNode<T> *q = (PltListNode<T> *) p;
        if (q->info == t) {
            delete PltList_base::remove(p);
            return true;
        }
    }
    return false;
}


//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

template<class T>
PltIList<T>::~PltIList() 
{
    while (!isEmpty()) removeFirst();
}

//////////////////////////////////////////////////////////////////////
template<class T>
PltIListIterator<T> *
PltIList<T>::newIterator() const
{
    return new PltIListIterator<T>(*this);
}

//////////////////////////////////////////////////////////////////////
#endif
// plt/list_impl.h










