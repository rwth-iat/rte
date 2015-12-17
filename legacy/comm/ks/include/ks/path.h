/* -*-plt-c++-*- */
#ifndef KS_PATH_INCLUDED
#define KS_PATH_INCLUDED
/* $Header: /home/david/cvs/acplt/ks/include/ks/path.h,v 1.13 2007-04-25 12:57:20 martin Exp $ */
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

/* Authors: 
 *   Harald Albrecht <harald@plt.rwth-aachen.de>
 *   Martin Kneissl <martin@plt.rwth-aachen.de>
 */

#include "ks/string.h"
#include "plt/array.h"
#include "ks/result.h"

//////////////////////////////////////////////////////////////////////

class KsPath 
{
public:
    KsPath();
    KsPath(const PltString &);
    KsPath(const KsPath & path, size_t first, size_t last);

    // accessors
    bool isValid() const;
    bool isSingle() const;
    bool isRelative() const;
    bool isAbsolute() const;

    PltString getHead() const;
    KsPath getTail() const;

    size_t size() const;
    PltString operator[] (size_t) const;
#if 0
    operator const char * () const;
#endif
    operator PltString () const;
    KsPath & operator = (const KsPath &);

    // Change path to be completely upper or lower case.
    void toUpper();
    void toLower();

    // Convert path from network representation using encoded special
    // characters ("%xx") to the natural decoded one.
    KS_RESULT decodePercents();

    KsPath resolve(const KsPath & rel);
    static void resolvePaths(const PltArray<KsString> & ids,
                             PltArray<KsPath> &paths,
                             PltArray<KS_RESULT> &res) ;
protected:
    KsPath(const KsPath &abs, const KsPath &rel);
    size_t checkAndCount();
    void findSlashes();
    PltString _str;
    bool _valid;
    PltArray<size_t> _slash;
    size_t _first;
    size_t _last;
    size_t _go_up; // How many domains do you have to go up before appending
                   // this path to get an absolute one?
};

//////////////////////////////////////////////////////////////////////


inline bool
KsPath::isValid() const 
{
    return _valid;
}


//////////////////////////////////////////////////////////////////////

inline bool
KsPath::isAbsolute() const
{
    PLT_PRECONDITION(isValid());
    return _go_up == 0;
}

//////////////////////////////////////////////////////////////////////

inline bool
KsPath::isRelative() const
{
    return !isAbsolute();
}

//////////////////////////////////////////////////////////////////////

inline size_t
KsPath::size() const
{
    return isValid() ? (_last - _first + 1) : 0;
}

//////////////////////////////////////////////////////////////////////

inline bool
KsPath::isSingle() const
{
    return size() == 1;
}

//////////////////////////////////////////////////////////////////////

inline void
KsPath::toUpper()
{
    _str.toUpper();
}

/////////////////////////////////////////////////////////////////////////////

inline void
KsPath::toLower()
{
    _str.toLower();
}

/////////////////////////////////////////////////////////////////////////////

#if 0
inline 
KsPath::operator const char *() const
{
    return operator PltString();
}
#endif

//////////////////////////////////////////////////////////////////////
#endif //KS_PATH_INCLUDED
