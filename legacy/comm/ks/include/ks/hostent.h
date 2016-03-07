/* -*-plt-c++-*- */
/* $Header: /home/david/cvs/acplt/ks/include/ks/hostent.h,v 1.5 2007-04-25 12:57:20 martin Exp $ */

#ifndef KSC_HOSTENT_INCLUDED
#define KSC_HOSTENT_INCLUDED

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

/* Author: Markus Juergens <markusj@plt.rwth-aachen.de> */

//////////////////////////////////////////////////////////////////////

#include <plt/array.h>

#include "ks/rpc.h"

//////////////////////////////////////////////////////////////////////

typedef u_long KSC_IP_TYPE;

class KscHostEnt 
{
public: 
  KscHostEnt(struct hostent * = 0);
  KscHostEnt(const KscHostEnt &);
  
  KscHostEnt &operator = (const KscHostEnt &);

  // query functions
  size_t numIP() const;
  KSC_IP_TYPE getIP(size_t) const;

protected:
  void copy_ips(struct hostent *);

  PltArray<KSC_IP_TYPE> ips;
};

//////////////////////////////////////////////////////////////////////
// INLINE IMPLEMENTATION
//////////////////////////////////////////////////////////////////////

inline
KscHostEnt::KscHostEnt(struct hostent *he)
{
  copy_ips(he);
}

//////////////////////////////////////////////////////////////////////

inline
KscHostEnt::KscHostEnt(const KscHostEnt &other)
  : ips(other.ips)
{}

//////////////////////////////////////////////////////////////////////

inline
KscHostEnt &
KscHostEnt::operator = (const KscHostEnt &other)
{
  ips = other.ips;
  return *this;
}

//////////////////////////////////////////////////////////////////////

inline size_t
KscHostEnt::numIP() const
{
  return ips.size();
}

//////////////////////////////////////////////////////////////////////

inline KSC_IP_TYPE
KscHostEnt::getIP(size_t i) const
{
  return ips[i];
}

//////////////////////////////////////////////////////////////////////

#endif

