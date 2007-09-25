/* -*-c++-*- */
/* $Header: /home/david/cvs/acplt/ks/include/ks/clnrequest.h,v 1.5 2007-09-25 12:07:06 wolfram Exp $ */
#ifndef KSC_CLNREQUEST_INCLUDED
#define KSC_CLNREQUEST_INCLUDED

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

#include "ks/serviceparams.h"
#include "ks/avmodule.h"
#include "ks/handle.h"

// ---------------------------------------------------------------------------
// Instances of KscServiceRequest are used to queue service requests with
// the KscServer proxies.
//
class KscServiceRequest {
public:
    KscServiceRequest(u_long service,
	              const KscAvModule *avm,
		      const KsXdrAble &params,
		      KsResult &result);
    KscServiceRequest(u_short service,
		      const KsString extension,
	              const KscAvModule *avm,
		      const KsXdrAble &params,
		      KsResult &result);
    virtual ~KscServiceRequest();

    u_long _service;           // KS service request number
    const KsString _extension; // optional extension name
    const KscAvModule *_avm;   // pointer to A/V module object
    const KsXdrAble &_params;  // the parameters
    KsResult &_result;

    KscNegotiator *_neg; // only temporarily used during REQ_BUSY phase.

    enum KscServiceRequestStatus {
	REQ_NONE,     // not even queued for later issue,
	REQ_WAITING,  // not yet sent, still waiting,
	REQ_BUSY,     // issued, awaiting reply from server,
	REQ_DONE      // got reply, finished.
    };

    KscServiceRequestStatus _status;

    enum KscServiceRequestSubStatus {
	SUBREQ_NORMAL,   // normal request
	SUBREQ_OPENONLY, // only open connection, don't send anything
	SUBREQ_QUERYEXT  // query extension's major opcode
    };

    KscServiceRequestSubStatus _sub_status;

    virtual void attention();

    virtual void reset();

}; // class KscServiceRequest


// ---------------------------------------------------------------------------
typedef PltPtrHandle<KscServiceRequest> KscServiceRequestHandle;


#endif /* KSC_CLNREQUEST_INCLUDED */

// End of ks/clnrequest.h
