/* -*-plt-c++-*- */
/* 
 *  Copyright (c) 1996 PLT, RWTH-Aachen, Germany. See file copy_plt.txt!
 *  Author: Martin Kneissl <martin@plt.rwth-aachen.de>
 *
 */

#include <plt/debug.h>
#include <ks/xdr.hpp>
#include <plt/string.hpp>
#define KS_XDR_MAX_PLT_STRING_SIZE 8192

//////////////////////////////////////////////////////////////////////

extern "C"                                    			
bool_t 
xdr_PltString (XDR *xdrs, PltString **pp)      
{                            
    char *p=0;
    const char *cp;

    PLT_PRECONDITION( xdrs && pp );        
    switch(xdrs->x_op) {
        
        case XDR_FREE:                            
            if (*pp) {                           
                delete *pp;                       
                *pp = 0;                          
                return TRUE;                     
            }                                    
        case XDR_ENCODE:                          
            if (! *pp) return FALSE;    
            cp = **pp;
            if ( !xdr_string(xdrs, &cp, ~0) ) {          
                return FALSE;                     
            }                                     
            break;                                
                                                  
        case XDR_DECODE:                          
            if (!xdr_string(xdrs, &p, ~0)) {         
                return FALSE;                     
            }                                     
            *pp = new PltString(p);
            xdr_free(xdr_string, &p);
            if (! *pp)
                return FALSE;
            break;
        default:
            return FALSE;
        }
    return TRUE;
}



