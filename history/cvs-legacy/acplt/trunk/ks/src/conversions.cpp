/* -*-plt-c++-*- */

/*
 * Copyright (c) 1996, 1997, 1998
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

/* Author : Markus Juergens <markusj@plt.rwth-aachen.de> */

/////////////////////////////////////////////////////////////////////////////

#include <ctype.h>

#include "ks/conversions.h"

/////////////////////////////////////////////////////////////////////////////

const char *
ksToPercent(char c, char *s)
{
    static char buf[3];

    if( !s ) s = buf;

    s[0] = '%';

    unsigned d = c;
    unsigned upper = d >> 4;
    unsigned lower = d % 16;

    s[1] = (char)upper + (upper < 10 ? '0' : '7');

    s[2] = (char)lower + (lower < 10 ? '0' : '7');

    return s;
}

/////////////////////////////////////////////////////////////////////////////

KsString
ksStringToPercent(const KsString &org)
{
    // Count characters that need to be converted
    //
    size_t convert = 0;
    for(size_t i = 0; i < org.len(); i++) {
        if( !ksIsValidChar(org[i]) ) convert++;
    }

    if( convert ) {
        char *res = new char[org.len() + 1 + convert*2];
        if( !res ) return KsString();
        //
        // Convert and copy data into new string
        //
        bool ok = true;
        for(size_t i = 0, j = 0; i < org.len() && ok; i++) {
            if( ksIsValidChar(org[i]) ) {
                res[j++] = org[i];
            } else {
                ok = ksToPercent(org[i], res + j) != 0;
                j += 3;
            }
        }

        res[org.len() + convert*2] = 0;

        if(ok) {
            return PltString::takeOwnership(res, org.len() + convert*2);
        } else {
            delete res;
            return KsString();
        }
    } else {
        return org;
    }
}

/////////////////////////////////////////////////////////////////////////////

KsString
ksPathToPercent(const KsString &org)
{
    // Count characters that need to be converted
    //
    size_t convert = 0;
    for(size_t i = 0; i < org.len(); i++) {
        if( !(ksIsValidChar(org[i]) || ksIsDelim(org[i])) ) convert++;
    }

    if( convert ) {
        char *res = new char[org.len() + 1 + convert*2];
        if( !res ) return KsString();
        //
        // Convert and copy data into new string
        //
        bool ok = true;
        for(size_t i = 0, j = 0; i < org.len() && ok; i++) {
            if( ksIsValidChar(org[i]) || ksIsDelim(org[i]) ) {
                res[j++] = org[i];
            } else {
                ok = ksToPercent(org[i], res + j) != 0;
                j += 3;
            }
        }

        res[org.len() + convert*2] = 0;

        if(ok) {
            return PltString::takeOwnership(res, org.len() + convert*2);
        } else {
            delete res;
            return KsString();
        }
    } else {
        return org;
    }
}

/////////////////////////////////////////////////////////////////////////////

char
ksFromPercent(const char *s)
{
    unsigned val = 0;
   
    if( isxdigit(s[0]) && isxdigit(s[1]) ) {
        if( s[0] <= '9' ) {
            val = s[0] - '0';
            val <<= 4; 
        } else if( s[0] <= 'F' ) {
            val = s[0] - '7';
            val <<= 4;
        } else {
            val = s[0] - 'W';
            val <<= 4;
        }
        if( s[1] <= '9' ) {
            val += s[1] - '0';
        } else if( s[1] <= 'F' ) {
            val += s[1] - '7';
        } else {
            val += s[1] - 'W';
        }
    }

    return val;
}
       
/////////////////////////////////////////////////////////////////////////////

KS_RESULT
ksStringFromPercent(const KsString &org,
                    KsString &retval)
{
    // Count characters to be converted
    //
    size_t convert = 0;
    for(size_t i = 0; i < org.len(); i++) {
        if( org[i] == '%' ) {
            convert++;
            i += 2;
        }
    }

    if( convert ) {
        // Allocate new string and copy data
        PLT_ASSERT(org.len() >= convert*3);

        const size_t szres = org.len() - convert*2; 
        char *res = new char[szres + 1];

        if( !res ) return KS_ERR_GENERIC;

#if PLT_DEBUG
        for(size_t i = 0; i <= szres; i++) {
            res[i] = '?';
        }
#endif

        bool ok = true;
        const char *p = org;

        for(size_t i = 0, j = 0; i < org.len() && ok; i++, j++) {
            if( p[i] == '%' ) {
                PLT_ASSERT( j < szres );
                res[j] = ksFromPercent(p + i + 1);
                ok = res[j] != 0;
                i+=2;
            } else {
                res[j] = p[i];
            }
        }

        res[org.len() - convert*2] = 0;

        if( ok ) {
            retval = PltString::takeOwnership(res, szres);
            return KS_ERR_OK;
        } else {
            delete [] res;
            return KS_ERR_BADNAME;
        }
    } else {
        retval = org;
        return KS_ERR_OK;
    }
}

/////////////////////////////////////////////////////////////////////////////

KS_RESULT
ksEncodeString(const KsString &org,
               KsString &retval,
               bool allowBackslash,
               bool allowPercent)
{
    // Allocate space that is big enough to hold
    // encoded string in any case.
    //
    const size_t maxsize = org.len() * 3;
    char *buf = new char[maxsize + 1];
    if( !buf ) return KS_ERR_GENERIC;

    // Iterate over characters of org and do conversions
    //
    size_t i = 0;        // position in org
    size_t j = 0;        // position in buf
    const char *p = org;
    bool ok = true;

    while( i < org.len() && ok ) {
        if( allowBackslash && p[i] == '\\' ) {
            // Check if we have a next char and encode it.
            // TODO: Should the following char be restricted to 
            //       '.', '/' and '%' ?
            //
            if( ++i < org.len() ) {
                ok = ksToPercent( p[i++], buf + j );
                j += 3;
            } else {
                ok = false;
            }
        } else if( allowPercent && p[i] == '%' ) {
            // Just copy percent and the following two characters
            // if exist and are valid.
            //
            buf[j++] = p[i++];

            if( i < org.len() && isxdigit(p[i]) ) {
                buf[j++] = p[i++];
                if( i < org.len() && isxdigit(p[i]) ) {
                    buf[j++] = p[i++];
                } else {
                    ok = false;
                }
            } else {
                ok = false;
            }
        } else if( ksIsValidChar(p[i]) || ksIsDelim(p[i]) ) {
            buf[j++] = p[i++];
        } else {
            ok = ksToPercent( p[i++], buf + j );
            j += 3;
        }   
    }

    buf[j] = 0;

    PLT_ASSERT( j <= maxsize ); 

    if( ok ) {
        retval = KsString(buf, j);
        delete [] buf;
        return retval.ok() ? KS_ERR_OK : KS_ERR_GENERIC;
    } else {
        delete [] buf;
        return KS_ERR_BADNAME;
    }
}

/////////////////////////////////////////////////////////////////////////////

KS_RESULT
ksDecodeString(const KsString &org,
               KsString &retval,
               bool useBackslash)
{
    // Decoded string has at most org.len() characters.
    //
    char *buf = new char[org.len() + 1];
    if( !buf ) return KS_ERR_GENERIC;

    // Iterate over characters of org and do conversions
    //
    size_t i = 0;
    size_t j = 0;
    const char *p  = org;
    bool ok = true;

    while( i < org.len() && ok ) {
        if( p[i] == '%' ) {
            char c = ksFromPercent(p + i + 1);
            if( c == 0 ) {
                ok = false;
            } else if( useBackslash && (ksIsDelim(c) || c == '%') ) {
                buf[j++] = '\\';
                buf[j++] = c;
            } else {
                buf[j++] = c;
            }
            i += 3;
        } else {
            buf[j++] = p[i++];
        }
    }

    if( ok ) {
        retval = KsString(buf, j);
        delete [] buf;
        return retval.ok() ? KS_ERR_OK : KS_ERR_GENERIC;
    } else {
        delete [] buf;
        return KS_ERR_BADNAME;
    }
}

/////////////////////////////////////////////////////////////////////////////
// EOF conversions.cpp
/////////////////////////////////////////////////////////////////////////////





