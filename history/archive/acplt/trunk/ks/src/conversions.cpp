/* -*-plt-c++-*- */
/* $Header: /home/david/cvs/acplt/ks/src/conversions.cpp,v 1.8 2007-04-25 12:57:20 martin Exp $ */
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
            delete[] res;
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
            delete[] res;
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


// ---------------------------------------------------------------------------
// Encode a string (either a path like "/a/b/c" or a full-blown resource
// locator like "//host/server/a/b/c"). During encoding, all non-valid
// characters are represented by their hex value with a leading percent sign
// instead of their character.
//
// And now again for the impaired:
// "/A-Text" ==> "/A%2FText"
//
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
    if( !buf ) {
	return KS_ERR_GENERIC;
    }
    char *pbuf = buf;
    const char *p = org;
    //
    // In case of an absolute URL used on the client side, just skip
    // the //host/server part because they can contain all sorts of
    // silly characters...
    //
    if ( (*p == '/') && (p[1] == '/') ) {
	//
	// Just skip the "junk" at the beginning of the resource locator,
	// containing the host and server and proceed to the root slash.
	//
	size_t      len;
	const char *pSlash = strchr(p + 2, '/');
	if ( pSlash ) {
	    pSlash = strchr(pSlash + 1, '/');
	}
	if ( !pSlash ) {
	    //
	    // The resource locator is missing some parts. But as it is
	    // intended to be a resource locator, just copy it verbatim.
	    //
	    len = org.len();
	} else {
	    //
	    // Process only the host and server part without changing
	    // anything in it, that is, copy it verbatim into the
	    // encoded string.
	    //
	    len = pSlash - p + 1;
	}
	strncpy(pbuf, p, len);
	p    += len;
	pbuf += len;
    }
    //
    // Iterate over the characters in the org string and do
    // the conversions.
    //
    bool ok = true;

    while ( *p && ok ) {
        if ( allowBackslash && (*p == '\\') ) {
	    //
            // Check if we have a next char following the backslash
	    // escape and then encode it. But only "invalid" characters
	    // are encoded ("\." becomes "%2F"), valid characters like "\a"
	    // are still copied copied verbatim, that is "\a" becomes "a".
            //
            if ( *++p ) {
		if ( !ksIsValidChar(*p) ) {
		    ksToPercent(*p++, pbuf);
		    pbuf += 3; // skip the %xx stuff
		} else {
		    *pbuf++ = *p++;
		}
            } else {
                ok = false;
            }
        } else if ( allowPercent && (*p == '%') ) {
	    //
            // Just copy the percent and the following two characters
            // if they exist and are valid hex chars.
            //
            *pbuf++ = *p++;

            if ( isxdigit(*p) ) {
                *pbuf++ = *p++;
                if ( isxdigit(*p) ) {
                    *pbuf++ = *p++;
                } else {
                    ok = false;
                }
            } else {
                ok = false;
            }
        } else if ( ksIsValidChar(*p) || ksIsDelim(*p) ) {
	    //
	    // All valid characters or delimiters are always copied
	    // verbatim to the encoded string.
	    //
            *pbuf++ = *p++;
        } else {
	    //
	    // All other characters get encoded using the %xx syntax,
	    // like it is done with URLs in the WWW.
	    //
            ksToPercent(*p++, pbuf);
	    pbuf += 3; // skip the %xx stuff
        }   
    }
    //
    // Finally terminate the encoded string.
    //
    *pbuf = 0;

    PLT_ASSERT( (pbuf - buf) <= maxsize ); 

    if ( ok ) {
        retval = KsString(buf);
        delete [] buf;
        return retval.ok() ? KS_ERR_OK : KS_ERR_GENERIC;
    } else {
        delete [] buf;
        return KS_ERR_BADNAME;
    }
} // ksEncodeString


// ---------------------------------------------------------------------------
// Decode a string (either a path like "/a/b/c" or a full-blown resource
// locator like "//host/server/a/b/c"). During decoding, all "percentified"
// characters are converted back to their plan character, optionally escaped
// by a backslash.
//
// And now again for the impaired:
// "/A%2FText" ==> "/A-Text"
//
KS_RESULT
ksDecodeString(const KsString &org,
               KsString &retval,
               bool useBackslash)
{
    //
    // The decoded string can have at most org.len() characters.
    //
    char *buf = new char[org.len() + 1];
    if( !buf ) {
	return KS_ERR_GENERIC;
    }

    // Iterate over characters of org and do conversions
    //
    const char *p = org;
    char       *pbuf = buf;
    bool        ok = true;

    while ( *p && ok ) {
        if( *p == '%' ) {
	    //
	    // Convert characters back from their hex representation into
	    // their plain character and optionally escape them.
	    //
            char c = ksFromPercent(p + 1);
            if ( !c ) {
                ok = false;
            } else if ( useBackslash && 
                        ((c == '\\') || 
                         (ksIsDelim(c) || (c == '%'))) ) {
                *pbuf++ = '\\';
                *pbuf++ = c;
            } else {
		//
		// Just use the plain character instead of the encoded one.
		//
                *pbuf++ = c;
            }
            p += 3;
        } else {
	    //
	    // All other stuff is copied back verbatim.
	    //
            *pbuf++ = *p++;
        }
    }

    *pbuf = 0;

    if ( ok ) {
        retval = KsString(buf);
        delete [] buf;
        return retval.ok() ? KS_ERR_OK : KS_ERR_GENERIC;
    }
    delete [] buf;
    return KS_ERR_BADNAME;
} // ksDecodeString


// End of ks/conversions.cpp
