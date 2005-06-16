/* -*-c++-*- */
#ifndef PLT_LOG_INCLUDED
#define PLT_LOG_INCLUDED
/* $Header: /home/david/cvs/acplt/plt/include/plt/log.h,v 1.11 2005-06-16 11:38:31 harald Exp $ */
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

    const static int LOGFILTER_INFO    = 0x01;
    const static int LOGFILTER_DEBUG   = 0x02;
    const static int LOGFILTER_WARNING = 0x04;
    const static int LOGFILTER_ERROR   = 0x08;
    const static int LOGFILTER_ALERT   = 0x10;

    const static int LOG_UPTO_INFO = PltLog::LOGFILTER_INFO;
    const static int LOG_UPTO_DEBUG = PltLog::LOG_UPTO_INFO
                                        + PltLog::LOGFILTER_DEBUG;
    const static int LOG_UPTO_WARNING = PltLog::LOG_UPTO_DEBUG
                                          + PltLog::LOGFILTER_WARNING;
    const static int LOG_UPTO_ERROR = PltLog::LOG_UPTO_WARNING
                                        + PltLog::LOGFILTER_ERROR;
    const static int LOG_UPTO_ALERT = PltLog::LOG_UPTO_ERROR
                                        + PltLog::LOGFILTER_ALERT;

    const static int LOG_ALL = PltLog::LOGFILTER_INFO
                                 + PltLog::LOGFILTER_DEBUG
                                 + PltLog::LOGFILTER_WARNING
                                 + PltLog::LOGFILTER_ERROR
                                 + PltLog::LOGFILTER_ALERT;

    static void SetLog(PltLog & log); 
    static PltLog * GetLog();
    static void Info(const char *msg);
    static void Debug(const char *msg);
    static void Warning(const char *msg);
    static void Error(const char *msg);
    static void Alert(const char *msg);

    static int SetLogMode(int logMode);
    static int GetLogMode();

    PltLog(int logMode = PltLog::LOG_ALL);
    virtual ~PltLog();
    virtual void info(const char *msg) = 0;
    virtual void debug(const char *msg) = 0;
    virtual void warning(const char *msg) = 0;
    virtual void error(const char *msg) = 0;
    virtual void alert(const char *msg) = 0;

    virtual int setLogMode(int logMode);
    virtual int getLogMode();

protected:
    int _logMode;

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
              int facility = LOG_USER, // see openlog
	      int logMode = PltLog::LOG_ALL);
    virtual ~PltSyslog();
    virtual void info(const char *msg);
    virtual void debug(const char *msg);
    virtual void warning(const char *msg);
    virtual void error(const char *msg);
    virtual void alert(const char *msg);
private:
    PltSyslog(const PltSyslog &); // forbidden
    PltSyslog & operator = (const PltSyslog &); // forbidden

    void log(int logtype, int priority, const char *msg);

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
    PltCerrLog(const char * ident = 0, int logMode = LOG_ALL);
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
    PltNtLog(const char * ident = 0, int logMode = LOG_ALL);
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
