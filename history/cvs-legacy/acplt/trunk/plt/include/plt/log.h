/* -*-plt-c++-*- */
#ifndef PLT_LOG_INCLUDED
#define PLT_LOG_INCLUDED
/* $Header: /home/david/cvs/acplt/plt/include/plt/log.h,v 1.1 1997-03-07 12:27:52 martin Exp $ */
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

#include "plt/debug.h"

//////////////////////////////////////////////////////////////////////

class PltLog
{
public:
    static void SetLog(PltLog & log); 
    static PltLog * GetLog();
    static void Info(const char *msg);
    static void Debug(const char *msg);
    static void Warning(const char *msg);
    static void Error(const char *msg);
    static void Alert(const char *msg);

    PltLog();
    virtual ~PltLog();
    virtual void info(const char *msg) = 0;
    virtual void debug(const char *msg) = 0;
    virtual void warning(const char *msg) = 0;
    virtual void error(const char *msg) = 0;
    virtual void alert(const char *msg) = 0;
private:
    static PltLog * _pLog;
};

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
#ifdef PLT_USE_SYSLOG
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

#endif /* EOF plt/log.h */
