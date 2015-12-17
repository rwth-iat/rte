/* -*-plt-c++-*- */
/* $Header: /home/david/cvs/acplt/plt/src/logstream.cpp,v 1.4 2003-09-23 08:28:10 harald Exp $ */
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

#include <plt/logstream.h>


//////////////////////////////////////////////////////////////////////
// Constructs a logger stream which uses the default logger object.
// In addition, you can associate a fixed logger object with this
// log stream, so the stream always uses that logger object.
//
PltLogStream::PltLogStream()
#if PLT_USE_DEPRECIATED_HEADER
    : ostrstream(), 
#else
    : std::ostringstream(),
#endif
      _logger(0)
{
} /* PltLogStream::PltLogStream */

PltLogStream::PltLogStream(PltLog &logger)
#if PLT_USE_DEPRECIATED_HEADER
    : ostrstream(), 
#else
    : std::ostringstream(),
#endif
      _logger(&logger)
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
