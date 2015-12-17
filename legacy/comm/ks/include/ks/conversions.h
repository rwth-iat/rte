/* -*-plt-c++-*- */

#ifndef KS_CONVERSIONS_INCLUDED
#define KS_CONVERSIONS_INCLUDED

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



