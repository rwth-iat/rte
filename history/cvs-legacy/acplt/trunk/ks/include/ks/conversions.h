/* -*-plt-c++-*- */

#ifndef KS_CONVERSIONS_INCLUDED
#define KS_CONVERSIONS_INCLUDED

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

#include "ks/ks.h"
#include "ks/string.h"

/////////////////////////////////////////////////////////////////////////////

inline bool
ksIsValidChar(char c)
{
    return isalnum(c) || c == '_';
}

/////////////////////////////////////////////////////////////////////////////

inline bool
ksIsDelim(char c)
{
    return c == '/' || c == '.';
}

const char *
ksToPercent(char c, char *s);

char
ksFromPercent(const char *s);

/////////////////////////////////////////////////////////////////////////////
// Converts all characters except alphanumerical characters and the
// underscore to percent representation.
//
KsString
ksStringToPercent(const KsString &org);

/////////////////////////////////////////////////////////////////////////////
// Converts all characters except alphanumerical characters, the underscore
// and valid path delimiters(currently slash and dot) to percent 
// representation.
//
KsString
ksPathToPercent(const KsString &org);

/////////////////////////////////////////////////////////////////////////////
// Converts all "%xx" to char(xx). All percents are expected to 
// be followed by a two char hexadecimal number.
//
KS_RESULT
ksStringFromPercent(const KsString &org,
                    KsString &retval);


/////////////////////////////////////////////////////////////////////////////
// Converts a string from human readable form to percent representation.
// If allowBackslash is true any character preceded by a backslash is
// converted to percent representation without further interpretation. 
// This option is meant to be used with '.', '/' and '%' which otherwise
// would be interpreted as special characters.
// If allowPercent is true any sequence "%xx" is just copied instead
// of encoding the percent.
//
KS_RESULT
ksEncodeString(const KsString &org,
               KsString &retval,
               bool allowBackslash = true,
               bool allowPercent = false);

/////////////////////////////////////////////////////////////////////////////
// Converts a string from percent encoding to human readable form.
// If useBackslash is true a backslash is used to distinguish between
// path delimiters(dot and slash without backslash) and not path related
// special characters(dot and slash with backslash).
//
KS_RESULT
ksDecodeString(const KsString &org,
               KsString &retval,
               bool useBackslash = true);

/////////////////////////////////////////////////////////////////////////////


#endif // KS_CONVERSIONS_INCLUDED



