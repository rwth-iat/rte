/* -*-plt-c++-*- */
#ifndef PLT_LOGSTREAM_INCLUDED
#define PLT_LOGSTREAM_INCLUDED
/* $Header: /home/david/cvs/acplt/plt/include/plt/logstream.h,v 1.11 2007-04-25 12:57:21 martin Exp $ */
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
/* Written by Harald Albrecht, harald@plt.rwth-aachen.de */

#include "plt/log.h"
#if PLT_USE_DEPRECIATED_HEADER
#include <iostream.h>
#else
#include <iostream>
#endif
#if PLT_SYSTEM_NT && !PLT_COMPILER_CYGWIN
#include <strstrea.h>
#else
#if PLT_USE_DEPRECIATED_HEADER
#include <strstream.h>
#else
#include <sstream>
#endif
#endif

//////////////////////////////////////////////////////////////////////
// The PltLogStream class is a specialised ostream which works hand in
// hand with a PltLog object. You can simply instantiate such a
// PltLogStream object and feed it with a message using the familiar
// overloaded "<<" operators. When you're through with this step, then
// you can simply spit out the message to the logger with a logger
// operation: info, debug, warning, error, alert. This is as easy as
// a cake -- or was that "a pie"?!
//
class PltLogStream : 
#if PLT_USE_DEPRECIATED_HEADER
    public ostrstream {
#else
    public std::ostringstream {
#endif
public:
    PltLogStream();
    PltLogStream(PltLog &logger);
    void setLogger(PltLog &logger);
    PltLog *getLogger();

    void info();
    void debug();
    void warning();
    void error();
    void alert();

protected:
    void reset();
    const char *str();

    PltLog *_logger;
}; // class PltLogStream


//////////////////////////////////////////////////////////////////////
// Resets the whole logger stream, so that a fresh new logging line
// can be created.
// We're achieving this by seeking back to the beginning of the string
// buffer. The buffer is thereby unfroozen and gets ready to accept
// new data. This whole trick allows us to reuse the string stream,
// which we use as a convenient container for the logging text. The
// advantage of the string stream is, that it can grow dynamically
// and can be used just like any ostream.
//

inline void PltLogStream::reset()
{
// With the new sstream stuff, memory management issues of the old
// class(es) have been cleaned up a bit, so the freeze stuff is
// not required anymore. Instead, we simply install a new empty string
// to wipe out all the old content.
#if PLT_USE_DEPRECIATED_HEADER
    rdbuf()->freeze(0);
    seekp(0);
#else
    std::ostringstream::str("");
#endif
} /* PltLogStream::reset */


//////////////////////////////////////////////////////////////////////
// Freezes the string buffer and returns a pointer to its contents.
// Before you can use the string stream again, you must reset() it.
// This way, we hopefully encapsulate all brain-damaged things related
// to string streams...
//
inline const char *PltLogStream::str()
{
    *this << (char) 0; /* This sucker prevents us from printing old
                        * data, which still lurks around in the string
			* stream...
			*/
#if PLT_USE_DEPRECIATED_HEADER
    return ostrstream::str();
#else
    return std::ostringstream::str().c_str();
#endif
} /* PltLogStream::str */

#endif
/* End of logstream.h */





