/* -*-plt-c++-*- */
#ifndef KS_PATH_INCLUDED
#define KS_PATH_INCLUDED
/* $Header: /home/david/cvs/acplt/ks/include/ks/path.h,v 1.1 1997-03-24 12:31:21 martin Exp $ */
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

/* Authors: 
 *   Harald Albrecht <harald@plt.rwth-aachen.de>
 *   Martin Kneissl <martin@plt.rwth-aachen.de>
 */

#include "plt/string.h"
#include "plt/array.h"

//////////////////////////////////////////////////////////////////////

class KsPath 
{
public:
    KsPath(const PltString &);
    KsPath(const KsPath & path, size_t first, size_t last);

    // accessors
    bool isValid() const;
    bool isSingle() const;
    bool isRelative() const;
    bool isAbsolute() const;

    PltString getHead() const;
    KsPath getTail() const;

    KsPath resolve(const KsPath & rel);
    size_t size() const;

    PltString operator[] (size_t) const;
    operator const char * () const;
    operator PltString () const;
    KsPath & operator = (const KsPath &);
    
private:
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

inline bool
KsPath::isSingle() const
{
    return _slash.size() == 1;
}

//////////////////////////////////////////////////////////////////////

inline size_t
KsPath::size() const
{
    return isValid() ? (_last - _first + 1) : 0;
}

//////////////////////////////////////////////////////////////////////

inline 
KsPath::operator const char *() const
{
    return operator PltString();
}

//////////////////////////////////////////////////////////////////////
#endif /KS_PATH_INCLUDED
