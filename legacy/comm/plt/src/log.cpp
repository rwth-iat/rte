/* -*-plt-c++-*- */
/* $Header: /home/david/cvs/acplt/plt/src/log.cpp,v 1.16 2007-04-25 12:57:21 martin Exp $ */
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


#include "plt/log.h"
#include <string.h>

//////////////////////////////////////////////////////////////////////
PltLog *
PltLog::_pLog;

int PltLog::_logMode = PltLog::PLT_LOGMODE_INFO;

//////////////////////////////////////////////////////////////////////

PltLog::PltLog(const int logMode)
{
    _logMode = logMode;
    
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

int
PltLog::SetLogMode(const int logMode)
{
    if (logMode < PLT_LOGMODE_INFO) {
        _logMode = PLT_LOGMODE_INFO;
    } else if(logMode > PLT_LOGMODE_ALERT) {
        _logMode = PLT_LOGMODE_ALERT;
    } else {
        _logMode = logMode;
    }
    
    return _logMode;
}

//////////////////////////////////////////////////////////////////////

int
PltLog::GetLogMode()
{
    return _logMode;
}

//////////////////////////////////////////////////////////////////////

void
PltLog::Info(const char * msg)
{
    if (_pLog) {
        if(_logMode <= PLT_LOGMODE_INFO) {
            _pLog->info(msg);
        }
    }
}

//////////////////////////////////////////////////////////////////////

void
PltLog::Debug(const char * msg)
{
    if (_pLog) {
        if(_logMode <= PLT_LOGMODE_DEBUG) {
            _pLog->debug(msg);
        }
    }
}

//////////////////////////////////////////////////////////////////////

void
PltLog::Warning(const char * msg)
{
    if (_pLog) {
        if(_logMode <= PLT_LOGMODE_WARNING) {
            _pLog->warning(msg);
        }
    }
}

//////////////////////////////////////////////////////////////////////

void
PltLog::Error(const char * msg)
{
    if (_pLog) {
        if(_logMode <= PLT_LOGMODE_ERROR) {
            _pLog->error(msg);
        }
    }
}

//////////////////////////////////////////////////////////////////////

void
PltLog::Alert(const char * msg)
{
    if (_pLog) {
        if(_logMode <= PLT_LOGMODE_ALERT) {
            _pLog->alert(msg);
        }
    }
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

#if PLT_SYSTEM_HPUX && PLT_SYSTEM_HPUX_MAJOR<10
//
// Prototypes for syslog functions, copied from syslog(3C) page. This
// cludge is only necessary for old HP-UX 9.0.x systems, which don't
// have clean header files. The newer 10.20.x systems are finally more in
// line with header standards...
//
extern "C" {
int syslog(int priority, const char *message, ...);
int openlog(const char *ident, int logopt, int facility);
int closelog(void);
int setlogmask(int maskpri);
};
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

PltCerrLog::PltCerrLog(const char *ident, const int logMode)
:PltLog(logMode)
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
#if PLT_USE_DEPRECIATED_HEADER
    cerr
#else
    STDNS::cerr
#endif
       << (_ident?_ident:"") << " [Info]:" << msg << STDNS::endl;
}

/////////////////////////////////////////////////////////////////////

void
PltCerrLog::debug(const char *msg)
{
#if PLT_USE_DEPRECIATED_HEADER
    cerr
#else
    STDNS::cerr
#endif
       << (_ident?_ident:"") << " [Debug]:" << msg << STDNS::endl;
}

/////////////////////////////////////////////////////////////////////

void
PltCerrLog::warning(const char *msg)
{
#if PLT_USE_DEPRECIATED_HEADER
    cerr
#else
    STDNS::cerr
#endif
       << (_ident?_ident:"") << " [Warning]:" << msg << STDNS::endl;
}

/////////////////////////////////////////////////////////////////////

void
PltCerrLog::error(const char *msg)
{
#if PLT_USE_DEPRECIATED_HEADER
    cerr
#else
    STDNS::cerr
#endif
       << (_ident?_ident:"") << " [Error]:" << msg << STDNS::endl;
}

/////////////////////////////////////////////////////////////////////

void
PltCerrLog::alert(const char *msg)
{
#if PLT_USE_DEPRECIATED_HEADER
    cerr
#else
    STDNS::cerr
#endif
       << (_ident?_ident:"") << " [Alert]:" << msg << STDNS::endl;
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
