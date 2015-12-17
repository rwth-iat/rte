/* -*-plt-c++-*- */
#ifndef PLT_LIST_IMPL_INCLUDED
#define PLT_LIST_IMPL_INCLUDED
/* $Header: /home/david/cvs/acplt/plt/include/plt/list_impl.h,v 1.11 2007-04-25 12:57:21 martin Exp $ */
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
    for (PltListNode<T> *p = (PltListNode<T> *)first; 
         p; 
         p = (PltListNode<T> *)(p->getNext()) ) 
        {
            if (p->info == t) {
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
PltIListIterator_THISTYPE(T) *
PltIList<T>::newIterator() const
{
    return new PltIListIterator<T>(*this);
}

//////////////////////////////////////////////////////////////////////
#endif
// plt/list_impl.h










