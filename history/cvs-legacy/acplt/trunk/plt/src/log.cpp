/* -*-plt-c++-*- */
/* $Header: /home/david/cvs/acplt/plt/src/log.cpp,v 1.7 1999-01-08 13:12:50 harald Exp $ */
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

#if PLT_SYSTEM_HPUX
// prototypes for syslog functions, copied from syslog(3C) page.

#if 0 // HPUX 10.20
extern "C" {
int syslog(int priority, const char *message, ...);
int openlog(const char *ident, int logopt, int facility);
int closelog(void);
int setlogmask(int maskpri);
};
#endif

#endif // PLT_SYSTEM_HPUX

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
//////////////////////////////////////////////////////////////////////
#if PLT_USE_CERRLOG
//////////////////////////////////////////////////////////////////////

PltCerrLog::PltCerrLog(const char *ident)
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

PltCerrLog::~PltCerrLog()
{
    if (_ident) {
        delete[] _ident;
    }
}

//////////////////////////////////////////////////////////////////////

void
PltCerrLog::info(const char *msg)
{
    cerr << (_ident?_ident:"") << " [Info]:" << msg << endl;
}

/////////////////////////////////////////////////////////////////////

void
PltCerrLog::debug(const char *msg)
{
    cerr << (_ident?_ident:"") << " [Debug]:" << msg << endl;
}

/////////////////////////////////////////////////////////////////////

void
PltCerrLog::warning(const char *msg)
{
    cerr << (_ident?_ident:"") << " [Warning]:" << msg << endl;
}

/////////////////////////////////////////////////////////////////////

void
PltCerrLog::error(const char *msg)
{
    cerr << (_ident?_ident:"") << " [Error]:" << msg << endl;
}

/////////////////////////////////////////////////////////////////////

void
PltCerrLog::alert(const char *msg)
{
    cerr << (_ident?_ident:"") << " [Alert]:" << msg << endl;
}

//////////////////////////////////////////////////////////////////////
#endif // PLT_USE_CERRLOG
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
#if PLT_USE_NTLOG
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Constructing and destructing a NT logger object is fortunately
// almost straightforward. We register/deregister an event source,
// and then we can go on and bark at the user...
//
PltNtLog::PltNtLog(const char * ident)
{
    //
    // If the caller didn't supplied an identification, we'll fall back
    // on a generic one -- guess what...
    //
    if ( ident == 0 ) {
    	ident = "ACPLT/KS";
    }
    _event_source = RegisterEventSource(0, ident);
} // PltNtLog::PltNtLog

PltNtLog::~PltNtLog()
{
    //
    // If there's still an event source around, kill it! You know: only
    // a dead event source is a good event source...
    //
    if ( _event_source ) {
        DeregisterEventSource(_event_source);
    }
} // PltNtLog::~PltNtLog

//////////////////////////////////////////////////////////////////////
// This is the workhorse for barking at the user. Because NT doesn't
// know of all the log levels, we've defined (well -- stolen from the
// syslog(3) mechanism), we have to emulate some levels. We do this
// by specifying a roughly equivalent severity level and then prefixing
// the user msg by the sevMsg prefix text.
//
void PltNtLog::log(WORD severity, const char *sevMsg, const char *msg)
{
    LPCTSTR messages[2];
    int     msgCount;

    if ( _event_source ) {
    	msgCount = 1;
    	if ( sevMsg ) {
            messages[0] = (LPCTSTR) sevMsg;
            messages[1] = (LPCTSTR) msg;
            msgCount = 2;
        } else {
            messages[0] = (LPCTSTR) msg;
        }
    	ReportEvent(_event_source,
                    severity,
                    0, // there are no special event categories available
                    0, // no special event id
                    0, // no security identifier required
                    msgCount,
                    0, // no event-specific raw data
                    messages,
                    0);
    }
} // PltNtLog::log

void PltNtLog::info(const char *msg)
{
    log(EVENTLOG_INFORMATION_TYPE, 0, msg);
} // PltNtLog::info

void PltNtLog::debug(const char *msg)
{
    log(EVENTLOG_INFORMATION_TYPE, "[DEBUG] ", msg);
} // PltNtLog::debug

void PltNtLog::warning(const char *msg)
{
    log(EVENTLOG_WARNING_TYPE, 0, msg);
} // PltNtLog::warning

void PltNtLog::error(const char *msg)
{
    log(EVENTLOG_ERROR_TYPE, 0, msg);
} // PltNtLog::error

void PltNtLog::alert(const char *msg)
{
    log(EVENTLOG_INFORMATION_TYPE, "[ALERT] ", msg);
} // PltNtLog::alert

//////////////////////////////////////////////////////////////////////
#endif // PLT_USE_NTLOG
//////////////////////////////////////////////////////////////////////

/* EOF log.cpp */
