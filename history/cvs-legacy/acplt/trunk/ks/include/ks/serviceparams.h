/* -*-plt-c++-*- */
#ifndef KS_SERVICEPARAMS_INCLUDED
#define KS_SERVICEPARAMS_INCLUDED
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

/* Author : Markus Juergens <plt@rwth-aachen.de> */

//////////////////////////////////////////////////////////////////////

#include <ks/xdr.h>
#include <ks/register.h>
#include <ks/props.h>
#include <ks/list.h>
#include <ks/array.h>

//////////////////////////////////////////////////////////////////////
// Classes for GetPP service
//   - class KsGetPPParams
//   - class KsGetPPResult
//////////////////////////////////////////////////////////////////////

class KsGetPPParams 
: public KsXdrAble
{
public:
    KsGetPPParams(XDR *, bool &);
    KsGetPPParams();

    bool xdrEncode(XDR *) const;
    bool xdrDecode(XDR *);
    static KsGetPPParams *xdrNew(XDR *);

    KsString path;
    KS_OBJ_TYPE type_mask;
    KsString name_mask;
};

//////////////////////////////////////////////////////////////////////

class KsGetPPResult
: public KsResult
{
public:
    KsGetPPResult();
    KsGetPPResult(XDR *, bool &);

    bool xdrEncode(XDR *) const;
    bool xdrDecode(XDR *);
    static KsGetPPResult *xdrNew(XDR *);

    KsList<KsProjPropsHandle> items;
};

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

//////////////////////////////////////////////////////////////////////

inline
KsGetVarParams::KsGetVarParams(size_t num_ids)
: identifiers(num_ids)
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








