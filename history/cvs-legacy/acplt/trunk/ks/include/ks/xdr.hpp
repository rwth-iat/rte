/* -*-plt-c++-*- */
/* 
 *  Copyright (c) 1996 PLT, RWTH-Aachen, Germany. See file copy_plt.txt!
 *  Author: Martin Kneissl <martin@plt.rwth-aachen.de>
 *
 */

#ifndef KS_XDR_INCLUDED
#define KS_XDR_INCLUDED

#include <plt/debug.h>

extern "C" {
#include <rpc/xdr.h>
};


//////////////////////////////////////////////////////////////////////

class KsXdrUnion {
public:
    virtual enum_t xdrTypeCode() const = 0; // union-switch of XDR-unions
    virtual bool_t xdr(XDR *) = 0;
};

//////////////////////////////////////////////////////////////////////

#define KS_XDR_IF_BEGIN(base)                     \
    extern "C"                                    \
    bool_t xdr_##base (XDR *xdrs, base **pp)      \
    {                                             \
        PLT_PRECONDITION( xdrs && pp );           \
        enum_t type;                              \
        switch(xdrs->x_op) {                      \
                                                  \
        case XDR_FREE:                            \
            if (!*pp) {                           \
                delete *pp;                       \
                *pp = 0;                          \
                return TRUE;                      \
            }                                     \
        case XDR_ENCODE:                          \
            if (! *pp) return FALSE;              \
            type = (*pp)->xdrTypeCode();          \
            if (!xdr_enum(xdrs,&type)) {          \
                return FALSE;                     \
            }                                     \
            break;                                \
                                                  \
        case XDR_DECODE:                          \
            if (!xdr_enum(xdrs, &type)) {         \
                return FALSE;                     \
            }                                     \
            switch(type) {                        

#define KS_XDR_IF_MAP(constant,derived)           \
            case constant:                        \
                *pp = new derived;                \
                break                            

#define KS_XDR_IF_END                             \
            default:                              \
                return FALSE;                     \
            }                                     \
            if (!*pp) {                           \
                return FALSE;                     \
            }                                     \
            break;                                \
        default:                                  \
            return FALSE;                         \
        }                                         \
        assert((*pp)->xdrTypeCode() == type);     \
        return (*pp)->xdr(xdrs);                  \
    }                

#endif
