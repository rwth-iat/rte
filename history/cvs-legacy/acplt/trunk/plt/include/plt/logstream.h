/* -*-plt-c++-*- */
#ifndef PLT_LOGSTREAM_INCLUDED
#define PLT_LOGSTREAM_INCLUDED
/* $Header: /home/david/cvs/acplt/plt/include/plt/logstream.h,v 1.5 1999-01-08 13:12:31 harald Exp $ */
/*
 * Copyright (c) 1996, 1997, 1998, 1999
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
/* Written by Harald Albrecht, harald@plt.rwth-aachen.de */

#include "plt/log.h"
#include <iostream.h>
#if PLT_SYSTEM_NT && !PLT_COMPILER_CYGWIN
#include <strstrea.h>
#else
#include <strstream.h>
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
class PltLogStream : public ostrstream {
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
    rdbuf()->freeze(0);
    seekp(0);
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
    return ostrstream::str();
} /* PltLogStream::freeze */

#endif
/* End of logstream.h */





