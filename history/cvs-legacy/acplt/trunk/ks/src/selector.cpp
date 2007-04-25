/* -*-plt-c++-*- */

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

/* Author : Markus Juergens <markusj@plt.rwth-aachen.de> */

/////////////////////////////////////////////////////////////////////////////

#include <stdlib.h>

#include "ks/selector.h"

/////////////////////////////////////////////////////////////////////////////

PLT_IMPL_RTTI0(KsSelector);
PLT_IMPL_RTTI1(KsNoneSel, KsSelector);
PLT_IMPL_RTTI1(KsTimeSel, KsSelector);
PLT_IMPL_RTTI1(KsStringSel, KsSelector);

PLT_IMPL_RTTI1(KsAbsRelTime, KsXdrUnion);

/////////////////////////////////////////////////////////////////////////////

KS_BEGIN_IMPL_XDRUNION(KsSelector);
KS_XDR_MAP(KS_HSELT_NONE, KsNoneSel);
KS_XDR_MAP(KS_HSELT_TIME, KsTimeSel);
KS_XDR_MAP(KS_HSELT_STRING, KsStringSel);
KS_END_IMPL_XDRUNION;

/////////////////////////////////////////////////////////////////////////////

KS_IMPL_XDRCTOR(KsAbsRelTime);
KS_IMPL_XDRNEW(KsAbsRelTime);

/////////////////////////////////////////////////////////////////////////////

bool 
KsAbsRelTime::xdrEncode(XDR *xdr) const
{
    PLT_PRECONDITION(xdr->x_op == XDR_ENCODE);
    
    switch(type) {
    case KS_TT_ABSOLUTE: {
        return ks_xdre_enum(xdr, &type)
            && ks_xdre_u_long(xdr, &time.abs.sec)
            && ks_xdre_u_long(xdr, &time.abs.usec);
    } 
    case KS_TT_RELATIVE: {
        return ks_xdre_enum(xdr, &type)
            && ks_xdre_long(xdr, &time.rel.sec)
            && ks_xdre_long(xdr, &time.rel.usec);
    }
    default: {
        return false;
    }
    } // switch
}

/////////////////////////////////////////////////////////////////////////////

bool 
KsAbsRelTime::xdrDecode(XDR *xdr)
{
    PLT_PRECONDITION(xdr->x_op == XDR_DECODE);

    if( ks_xdrd_enum(xdr, &type) ) {
        switch(type) {
        case KS_TT_ABSOLUTE: {
            return ks_xdrd_u_long(xdr, &time.abs.sec) && 
                ks_xdrd_u_long(xdr, &time.abs.usec);
        }
        case KS_TT_RELATIVE: {
            return ks_xdrd_long(xdr, &time.rel.sec)
                && ks_xdrd_long(xdr, &time.rel.usec);
        }
        default: {
            return false;
        }
        } // switch
    }

    return false;
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

bool 
KsTimeSel::xdrEncodeVariant(XDR *xdr) const
{
    PLT_PRECONDITION(xdr->x_op == XDR_ENCODE);
    
    return ks_xdre_enum(xdr, &ip_mode)
        && from.xdrEncode(xdr)
        && to.xdrEncode(xdr)
        && delta.xdrEncode(xdr);
}

//////////////////////////////////////////////////////////////////////

bool 
KsTimeSel::xdrDecodeVariant(XDR *xdr) 
{
    PLT_PRECONDITION(xdr->x_op == XDR_DECODE);

    return ks_xdrd_enum(xdr, &ip_mode)
        && from.xdrDecode(xdr)
        && to.xdrDecode(xdr)
        && delta.xdrDecode(xdr);
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

#if PLT_DEBUG

void
KsAbsRelTime::print(ostream &os) const
{
    os << "KsAbsRelTime {";

    switch( xdrTypeCode() ) {
    case KS_TT_ABSOLUTE: {
        char buf[200];
        time_t temp = time.abs.sec;
        struct tm *broken = gmtime(&temp);

        strftime(buf, sizeof(buf), "UTC %c", broken);
        os << buf << "or ";

        broken = localtime(&temp);
        strftime(buf, sizeof(buf), "%Z %c", broken);
        os << buf;
    } break;
    case KS_TT_RELATIVE: {
        u_long t = (u_long)labs( time.rel.sec );
        long h = t / 3600,
            m = (t % 3600) / 60,
            s = t % 60;

        os << "Time span ";
        if( time.rel.sec < 0 ) {
            os << "-";
        }
        if( h ) os << h << ":";
        if( m ) os << m << ":";
        os << s;
    } break;
    default: {
        os << "Unknown typecode";
    }
    }

    os << "}";
}

/////////////////////////////////////////////////////////////////////////////

void
KsNoneSel::print(ostream &os) const
{
    os << "KsNoneSel";
}

/////////////////////////////////////////////////////////////////////////////

void
KsStringSel::print(ostream &os) const
{
    os << "KsStringSel {"
       << mask
       << "}";
}

/////////////////////////////////////////////////////////////////////////////

void
KsTimeSel::print(ostream &os) const
{
    os << "KsTimeSel {"
       << ip_mode << endl;

    from.print(os); 
    os << endl;
    to.print(os);
    os << endl;
    KsAbsRelTime(delta).print(os);
    os << "}";
}

#endif

/////////////////////////////////////////////////////////////////////////////
// EOF selector.cpp
/////////////////////////////////////////////////////////////////////////////






