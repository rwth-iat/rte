/* -*-plt-c++-*- */
/* $Header: /home/david/cvs/acplt/ks/include/ks/ntservice.h,v 1.8 2007-04-25 12:57:20 martin Exp $ */

#ifndef KS_NTSERVICE_INCLUDED
#define KS_NTSERVICE_INCLUDED

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

/* Author: Harald Albrecht <harald@plt.rwth-aachen.de> */

#include "ks/svrbase.h"

// Yuck! Some OS-specific stuff...
#if PLTCOMPILER_BORLAND > 0 && PLT_COMPILER_BORLAND < 0x0530
#include <win32/winsvc.h>
#else
#include <winsvc.h>
#endif

// -------------------------------------------------------------------------
// The NT service object class. It hides the "magic" (bad magic, huh?) of
// NT's service mechanism and eases the creation of such services. The only
// thing to do is to derive your own service class from KsNtServiceServer
// and to implement the createServer() method. That's it. Gosh! OOP: you get
// the ease and we get the trouble...
//

//// Marshalling stubs ////
extern "C" void ks_c_serviceMain(DWORD argc, LPTSTR *argv);
extern "C" void WINAPI ks_c_serviceController(DWORD control_code);
extern "C" int ks_c_serviceRun();

class KsNtServiceServer {
public:
    KsNtServiceServer(const char *service_name,
                      unsigned int create_timeout,
                      unsigned int spinup_timeout,
                      unsigned int spindown_timeout,
                      unsigned int destroy_timeout);
    virtual ~KsNtServiceServer();

    void run();

    bool isOk() const;
    bool isVerbose() const;
    bool reportServiceStatus(DWORD curr_stat, DWORD exit_code, DWORD user_code,
                             DWORD checkpoint, DWORD wait_hint);
    DWORD currentServiceStatus();

    static KsNtServiceServer *getService();

protected:
    virtual KsServerBase *createServer(int argc, char **argv) = 0;

    void lastError(const char *msg);
    void verbose(const char *msg);

    bool                      _is_ok;
    bool                      _is_verbose;
    const char               *_service_name;
    KsServerBase             *_my_server;

    static KsNtServiceServer *_the_service;

    unsigned int              _spinup_to; // timeouts
    unsigned int              _spindown_to;
    unsigned int              _create_to;
    unsigned int              _destroy_to;

    SERVICE_STATUS_HANDLE     _service_status_handle;
    SERVICE_STATUS            _service_status;
    HANDLE                    _service_done_event;
    HANDLE                    _one_shot_event;
    HANDLE                    _workhorse_thread;
    DWORD                     _workhorse_thread_id;

private:
    virtual void serviceMain(int argc, char **argv);
    virtual void serviceController(DWORD control_code);
    virtual int serviceRun();

friend void ks_c_serviceMain(DWORD argc, LPTSTR *argv);
friend void WINAPI ks_c_serviceController(DWORD control_code);
friend int ks_c_serviceRun();

}; // KsNtServiceServer

#endif /* #ifdef KS_NTSERVICE_INCLUDED */

/* End of ntservice.h */

