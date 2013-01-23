/* -*-plt-c++-*- */
/* $Header: /home/david/cvs/acplt/ks/include/ks/serviceparams.h,v 1.10 2007-04-25 12:57:20 martin Exp $ */
#ifndef KS_SERVICEPARAMS_INCLUDED
#define KS_SERVICEPARAMS_INCLUDED
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

/* Author : Markus Juergens <plt@rwth-aachen.de> */


#include "ks/xdr.h"
#include "ks/register.h"
#include "ks/props.h"
#include "ks/propsv1.h"
#include "ks/list.h"
#include "ks/array.h"


// ----------------------------------------------------------------------------
// Classes for the ACPLT/KS "GetPP" service. This service is now (as of proto-
// col version 2) depreciated. The "GetEP" service superceedes this service.
//   - class KsGetPPParams defines which parameters must be used for the
//     service call.
//   - class KsGetPPResult defines what is returned by the ACPLT/KS service.
//
class KsGetPPParams 
: public KsXdrAble
{
public:
    KsGetPPParams(XDR *, bool &);
    KsGetPPParams();

    bool xdrEncode(XDR *) const;
    bool xdrDecode(XDR *);
    static KsGetPPParams *xdrNew(XDR *);

    //
    // And now for the real service parameters...
    //
    KsString    path;
    KS_OBJ_TYPE type_mask;
    KsString    name_mask;
}; // class KsGetPPParams


class KsGetPPResult
: public KsResult
{
public:
    KsGetPPResult();
    KsGetPPResult(XDR *, bool &);

    bool xdrEncode(XDR *) const;
    bool xdrDecode(XDR *);
    static KsGetPPResult *xdrNew(XDR *);

    //
    // The things returned by the service reply...
    //
    KsList<KsEngPropsV1Handle> items;
}; // class KsGetPPResult


// ----------------------------------------------------------------------------
// Classes for the ACPLT/KS "GetEP" service. This service replaces the old
// GetPP service from the version 1 core protocol.
//   - class KsGetEPParams defines which parameters must be used for the
//     service call.
//   - class KsGetEPResult defines what is returned by the ACPLT/KS service.
//
class KsGetEPParams 
: public KsXdrAble
{
public:
    KsGetEPParams(XDR *, bool &);
    KsGetEPParams();

    bool xdrEncode(XDR *) const;
    bool xdrDecode(XDR *);
    static KsGetEPParams *xdrNew(XDR *);

    //
    // The service parameters...
    //
    KsString    path;
    KS_OBJ_TYPE type_mask;
    KsString    name_mask;
    KS_EP_FLAGS scope_flags;
}; // class KsGetEPParams


class KsGetEPResult
: public KsResult
{
public:
    KsGetEPResult();
    KsGetEPResult(XDR *, bool &);

    bool xdrEncode(XDR *) const;
    bool xdrDecode(XDR *);
    static KsGetEPResult *xdrNew(XDR *);

    KsList<KsEngPropsHandle> items;
}; // class KsGetEPResult


//////////////////////////////////////////////////////////////////////
// Classes for GetVar service
//   - class KsGetVarParams
//   - class KsGetVarItemResult
//   - class KsGetVarResult
//////////////////////////////////////////////////////////////////////

class KsGetVarParams
: public KsXdrAble
{
public:
    KsGetVarParams();
    KsGetVarParams(size_t num_ids);
    KsGetVarParams(XDR *, bool &);

    bool xdrEncode(XDR *) const;
    bool xdrDecode(XDR *);
    static KsGetVarParams *xdrNew(XDR *);

    KsArray<KsString> identifiers;
};

//////////////////////////////////////////////////////////////////////

class KsGetVarItemResult
: public KsResult
{
public:
    KsGetVarItemResult() { } // silence compiler
    KsGetVarItemResult(XDR *, bool &);

    bool xdrEncode(XDR *) const;
    bool xdrDecode(XDR *);
    static KsGetVarItemResult *xdrNew(XDR *);

    KsCurrPropsHandle item;
};

//////////////////////////////////////////////////////////////////////

class KsGetVarResult 
: public KsResult
{
public:
    KsGetVarResult();
    KsGetVarResult(size_t);
    KsGetVarResult(XDR *, bool &);

    bool xdrEncode(XDR *) const;
    bool xdrDecode(XDR *);
    static KsGetVarResult *xdrNew(XDR *);

    KsArray<KsGetVarItemResult> items;
};


//////////////////////////////////////////////////////////////////////
// Classes for SetVar Service
//   - class KsSetVarItem
//   - class KsSetVarParams
//   - class KsSetVarResult
//////////////////////////////////////////////////////////////////////

class KsSetVarItem
: public KsXdrAble
{
public:
    KsSetVarItem() { } // silence compiler
    KsSetVarItem(XDR *, bool &);

    bool xdrEncode(XDR *) const;
    bool xdrDecode(XDR *);
    static KsSetVarItem *xdrNew(XDR *);

    KsString path_and_name;
    KsCurrPropsHandle curr_props;
};

//////////////////////////////////////////////////////////////////////

class KsSetVarParams
: public KsXdrAble
{
public:
    KsSetVarParams(size_t);
    KsSetVarParams(XDR *, bool &);

    bool xdrEncode(XDR *) const;
    bool xdrDecode(XDR *);
    static KsSetVarParams *xdrNew(XDR *);

    KsArray<KsSetVarItem> items;
};

//////////////////////////////////////////////////////////////////////

class KsSetVarResult
: public KsResult
{
public:
    KsSetVarResult(size_t);
    KsSetVarResult(XDR *, bool &);

    bool xdrEncode(XDR *) const;
    bool xdrDecode(XDR *);
    static KsSetVarResult *xdrNew(XDR *);

    KsArray<KsResult> results;
};

//////////////////////////////////////////////////////////////////////
// Classes for ExgData Service
//   - class KsSetVarItem(see SetVar service beyond)
//   - class KsGetVarItemResult(see GetVar service beyond)
//   - class KsExgDataParams
//   - class KsExgDataResult
//////////////////////////////////////////////////////////////////////

class KsExgDataParams 
: public KsXdrAble
{
public:
    KsExgDataParams(size_t set_num_items, size_t get_num_items);
    KsExgDataParams(XDR *, bool &);

    bool xdrEncode(XDR *) const;
    bool xdrDecode(XDR *);
    static KsExgDataParams *xdrNew(XDR *);

    KsArray<KsSetVarItem> set_vars;
    KsArray<KsString> get_vars;
};

//////////////////////////////////////////////////////////////////////

class KsExgDataResult
: public KsResult
{
public:
    KsExgDataResult(size_t set_num_items, size_t get_num_items);
    KsExgDataResult(XDR *, bool &);

    bool xdrEncode(XDR *) const;
    bool xdrDecode(XDR *);
    static KsExgDataResult *xdrNew(XDR *);

    KsArray<KsResult> results;
    KsArray<KsGetVarItemResult> items;
};
    
//////////////////////////////////////////////////////////////////////
// Inline Implementation
//////////////////////////////////////////////////////////////////////

#if 0
inline
KsGetVarItemResult::KsGetVarItemResult()
{}

//////////////////////////////////////////////////////////////////////

inline
KsSetVarItem::KsSetVarItem()
{}

//////////////////////////////////////////////////////////////////////
#endif

inline
KsGetPPParams::KsGetPPParams()
{}

//////////////////////////////////////////////////////////////////////

inline
KsGetPPResult::KsGetPPResult()
{}

// ----------------------------------------------------------------------------

inline
KsGetEPParams::KsGetEPParams()
{}

inline
KsGetEPResult::KsGetEPResult()
{} // KsGetEPResult::KsGetEPResult

//////////////////////////////////////////////////////////////////////

inline
KsGetVarParams::KsGetVarParams(size_t num_ids)
: identifiers(num_ids)
{}

//////////////////////////////////////////////////////////////////////

inline
KsGetVarParams::KsGetVarParams()
{}

//////////////////////////////////////////////////////////////////////

inline bool
KsGetVarParams::xdrEncode(XDR *xdr) const 
{
    return identifiers.xdrEncode(xdr);
}

//////////////////////////////////////////////////////////////////////

inline bool
KsGetVarParams::xdrDecode(XDR *xdr)
{
    return identifiers.xdrDecode(xdr);
}

//////////////////////////////////////////////////////////////////////

inline
KsGetVarResult::KsGetVarResult(size_t nids)
: items(nids)
{}

//////////////////////////////////////////////////////////////////////

inline 
KsSetVarParams::KsSetVarParams(size_t nitems)
: items(nitems)
{}

//////////////////////////////////////////////////////////////////////

inline bool
KsSetVarParams::xdrEncode(XDR *xdr) const
{
    return items.xdrEncode(xdr);
}

//////////////////////////////////////////////////////////////////////

inline bool
KsSetVarParams::xdrDecode(XDR *xdr)
{
    return items.xdrDecode(xdr);
}

//////////////////////////////////////////////////////////////////////

inline
KsSetVarResult::KsSetVarResult(size_t size)
: results(size)
{}

//////////////////////////////////////////////////////////////////////

inline
KsExgDataParams::KsExgDataParams(size_t nset, size_t nget)
: set_vars(nset), get_vars(nget)
{}

//////////////////////////////////////////////////////////////////////

inline
KsExgDataResult::KsExgDataResult(size_t nset, size_t nget)
: results(nset), items(nget)
{}

#endif

/* End of ks/serviceparams.h */
