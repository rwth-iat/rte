/* -*-plt-c++-*- */
/* $Header: /home/david/cvs/acplt/plt/src/logstream.cpp,v 1.2 1997-09-03 14:11:24 martin Exp $ */
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
/* Written by Harald Albrecht, harald@plt.rwth-aachen.de */

#include <plt/logstream.h>


//////////////////////////////////////////////////////////////////////
// Constructs a logger stream which uses the default logger object.
// In addition, you can associate a fixed logger object with this
// log stream, so the stream always uses that logger object.
//
PltLogStream::PltLogStream()
    : ostrstream(), _logger(0)
{
} /* PltLogStream::PltLogStream */

PltLogStream::PltLogStream(PltLog &logger)
    : ostrstream(), _logger(&logger)
{
} /* PltLogStream::PltLogStream */


//////////////////////////////////////////////////////////////////////
// Convenience functions, so the programmer might change the logging
// object which is associated with this logging stream.
//
void PltLogStream::setLogger(PltLog &logger)
{
    _logger = &logger;
} /* PltLogStream::setLogger */

PltLog *PltLogStream::getLogger()
{
    return _logger;
} /* PltLogStream::getLogger */


//////////////////////////////////////////////////////////////////////
// Below are the various logging functions, which send the current
// logging message from the string stream to the appropriate logger
// object.
//
void PltLogStream::info()
{
    const char *s = str();
    _logger ? _logger->info(s) : PltLog::Info(s);
    reset();
} /* PltLogStream::info */

void PltLogStream::debug()
{
    const char *s = str();
    _logger ? _logger->debug(s) : PltLog::Debug(s);
    reset();
} /* PltLogStream::debug */

void PltLogStream::warning()
{
    const char *s = str();
    _logger ? _logger->warning(s) : PltLog::Warning(s);
    reset();
} /* PltLogStream::warning */

void PltLogStream::error()
{
    const char *s = str();
    _logger ? _logger->error(s) : PltLog::Error(s);
    reset();
} /* PltLogStream::error */

void PltLogStream::alert()
{
    const char *s = str();
    _logger ? _logger->alert(s) : PltLog::Alert(s);
    reset();
} /* PltLogStream::alert */


/* End of logstream.cpp */
