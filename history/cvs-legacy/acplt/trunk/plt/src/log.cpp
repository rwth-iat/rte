/* -*-plt-c++-*- */
/* $Header: /home/david/cvs/acplt/plt/src/log.cpp,v 1.1 1997-03-07 12:27:54 martin Exp $ */
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


#include "plt/log.h"
#include <string.h>

//////////////////////////////////////////////////////////////////////
PltLog *
PltLog::_pLog;
//////////////////////////////////////////////////////////////////////

PltLog::PltLog()
{
    if (! _pLog) {
        _pLog = this;
    }
}

//////////////////////////////////////////////////////////////////////

PltLog::~PltLog()
{
    if (_pLog == this) {
        _pLog = 0;
    }
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

void
PltLog::Info(const char * msg)
{
    if (_pLog) _pLog->info(msg);
}

//////////////////////////////////////////////////////////////////////

void
PltLog::Debug(const char * msg)
{
    if (_pLog) _pLog->debug(msg);
}

//////////////////////////////////////////////////////////////////////

void
PltLog::Warning(const char * msg)
{
    if (_pLog) _pLog->warning(msg);
}

//////////////////////////////////////////////////////////////////////

void
PltLog::Error(const char * msg)
{
    if (_pLog) _pLog->error(msg);
}

//////////////////////////////////////////////////////////////////////

void
PltLog::Alert(const char * msg)
{
    if (_pLog) _pLog->alert(msg);
}


//////////////////////////////////////////////////////////////////////

void
PltLog::SetLog(PltLog &log)
{
    _pLog = &log;
}

//////////////////////////////////////////////////////////////////////

PltLog *
PltLog::GetLog()
{
    return _pLog;
}

//////////////////////////////////////////////////////////////////////
#if PLT_USE_SYSLOG
//////////////////////////////////////////////////////////////////////

PltSyslog * 
PltSyslog::_currLog = 0;

//////////////////////////////////////////////////////////////////////

PltSyslog::PltSyslog(const char *ident, int logopt, int facility)
: _logopt(logopt), 
  _facility(facility)
{
    // make copy of ident just to be sure: 
    // The syslog facility stores only
    // the pointer.
    _ident = ident ? new char[strlen(ident)+1] : 0;

    if (_ident) {
        strcpy(_ident, ident);
    }
}

//////////////////////////////////////////////////////////////////////

PltSyslog::~PltSyslog()
{
    if (_currLog == this) {
        closelog();
        _currLog = 0;
    }
    if (_ident) {
        delete[] _ident;
    }
}

//////////////////////////////////////////////////////////////////////

void 
PltSyslog::log(int priority, const char * msg) 
{
    // switch log if necessary
    if (_currLog != this) {
        if (_currLog) {
            closelog();
        }
        openlog(_ident ? _ident : "<libplt>",
                _logopt, 
                _facility);
        _currLog = this;
    }
    
    // log message
    syslog(priority, msg);
}

//////////////////////////////////////////////////////////////////////

void
PltSyslog::info(const char *msg)
{
    log(LOG_INFO, msg);
}

/////////////////////////////////////////////////////////////////////

void
PltSyslog::debug(const char *msg)
{
    log(LOG_DEBUG, msg);
}

//////////////////////////////////////////////////////////////////////

void
PltSyslog::warning(const char *msg)
{
    log(LOG_WARNING, msg);
}

//////////////////////////////////////////////////////////////////////

void
PltSyslog::error(const char *msg)
{
    log(LOG_ERR, msg);
}

//////////////////////////////////////////////////////////////////////

void
PltSyslog::alert(const char *msg)
{
    log(LOG_ALERT, msg);
}

//////////////////////////////////////////////////////////////////////
#endif // PLT_USE_SYSLOG
//////////////////////////////////////////////////////////////////////

/* EOF blabla.cpp */
