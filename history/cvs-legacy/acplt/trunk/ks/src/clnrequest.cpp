/* -*-c++-*- */
/* $Header: /home/david/cvs/acplt/ks/src/clnrequest.cpp,v 1.3 2003-10-17 13:32:11 harald Exp $ */

/*
 * Copyright (c) 2003
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

/* Written by Albrecht boshi */

#include "ks/clnrequest.h"


// ---------------------------------------------------------------------------
KscServiceRequest::KscServiceRequest(u_long service,
				     const KscAvModule *avm,
				     const KsXdrAble &params,
				     KsResult &result)
    : _service(service),
      _avm(avm),
      _params(params),
      _result(result),
      _neg(0),
      _status(REQ_NONE),
      _sub_status(SUBREQ_NORMAL)
{
} // KscServiceRequest::KscServiceRequest

KscServiceRequest::KscServiceRequest(u_short service,
				     const KsString extension,
				     const KscAvModule *avm,
				     const KsXdrAble &params,
				     KsResult &result)
    : _service(service),
      _extension(extension),
      _avm(avm),
      _params(params),
      _result(result),
      _neg(0),
      _status(REQ_NONE),
      _sub_status(SUBREQ_NORMAL)
{
} // KscServiceRequest::KscServiceRequest


// ---------------------------------------------------------------------------
// Not much really to destroy here. We just need the virtual destructor in the
// base class, so others can later clean up their mess properly.
//
KscServiceRequest::~KscServiceRequest()
{
} // KscServiceRequest::~KscServiceRequest


void
KscServiceRequest::attention()
{
} // KscServiceRequest::attention


// ---------------------------------------------------------------------------
// Reset the request, so it can be used another time.
//
void
KscServiceRequest::reset()
{
    _status = REQ_NONE;
} // KscServiceRequest::reset


// End of ks/clnrequest.cpp
