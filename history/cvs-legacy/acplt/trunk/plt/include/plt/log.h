/* -*-c++-*- */
#ifndef PLT_LOG_INCLUDED
#define PLT_LOG_INCLUDED
/* $Header: /home/david/cvs/acplt/plt/include/plt/log.h,v 1.15 2007-04-25 12:57:21 martin Exp $ */
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

#include "plt/config.h"

//////////////////////////////////////////////////////////////////////

class PltLog
{
public:

    enum {
            PLT_LOGMODE_INFO       = 0,
            PLT_LOGMODE_DEBUG      = 1,
            PLT_LOGMODE_WARNING    = 2,
            PLT_LOGMODE_ERROR      = 3,
            PLT_LOGMODE_ALERT      = 4
    };

    static int  SetLogMode(const int logMode); 
    static int  GetLogMode(); 
    static void SetLog(PltLog & log); 
    static PltLog * GetLog();

    static void Info(const char *msg);
    static void Debug(const char *msg);
    static void Warning(const char *msg);
    static void Error(const char *msg);
    static void Alert(const char *msg);

    PltLog(const int logMode=PltLog::PLT_LOGMODE_INFO);
    virtual ~PltLog();
    virtual void info(const char *msg) = 0;
    virtual void debug(const char *msg) = 0;
    virtual void warning(const char *msg) = 0;
    virtual void error(const char *msg) = 0;
    virtual void alert(const char *msg) = 0;
    
protected:
    static int      _logMode;
    
private:
    static PltLog * _pLog;
};

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
#if PLT_USE_SYSLOG
//////////////////////////////////////////////////////////////////////
#include <syslog.h>

class PltSyslog
: public PltLog
{
public:
    PltSyslog(const char * ident = 0, 
              int logopt = LOG_PID, 
              int facility = LOG_USER); // see openlog
    virtual ~PltSyslog();
    virtual void info(const char *msg);
    virtual void debug(const char *msg);
    virtual void warning(const char *msg);
    virtual void error(const char *msg);
    virtual void alert(const char *msg);
private:
    PltSyslog(const PltSyslog &); // forbidden
    PltSyslog & operator = (const PltSyslog &); // forbidden

    void log(int priority, const char *msg);

    char * _ident;
    int _logopt;
    int _facility;
    static PltSyslog * _currLog; // the open log
};

//////////////////////////////////////////////////////////////////////
#endif // PLT_USE_SYSLOG
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
#if PLT_USE_CERRLOG
//////////////////////////////////////////////////////////////////////
#if PLT_USE_DEPRECIATED_HEADER
#include <iostream.h>
#else
#include <iostream>
#endif

class PltCerrLog
: public PltLog
{
public:
    PltCerrLog(const char * ident = 0, const int logMode=PltLog::PLT_LOGMODE_INFO);
    ~PltCerrLog();
    virtual void info(const char *msg);
    virtual void debug(const char *msg);
    virtual void warning(const char *msg);
    virtual void error(const char *msg);
    virtual void alert(const char *msg);
private:
    PltCerrLog(const PltCerrLog &); // forbidden
    PltCerrLog & operator = (const PltCerrLog &); // forbidden

    char * _ident;
};

//////////////////////////////////////////////////////////////////////
#endif // PLT_USE_CERRLOG
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
#if PLT_USE_NTLOG
//////////////////////////////////////////////////////////////////////

#include <windows.h>

//
// This is a specialised log object, which works only on NT. Yeah, big deal!
// Of course it only works on NT, because NT isn't portable at all. And what
// is NT at least? The biggest collection of programming mistakes. But now
// enough quarrel...
//
class PltNtLog : public PltLog {
public:
    PltNtLog(const char * ident = 0);
    virtual ~PltNtLog();
    virtual void info(const char *msg);
    virtual void debug(const char *msg);
    virtual void warning(const char *msg);
    virtual void error(const char *msg);
    virtual void alert(const char *msg);

private:
    void log(WORD severity, const char *sevMsg, const char *msg);

    HANDLE _event_source;
}; // class PltNtLog

//////////////////////////////////////////////////////////////////////
#endif // PLT_USE_NTLOG
//////////////////////////////////////////////////////////////////////

#endif /* EOF plt/log.h */
