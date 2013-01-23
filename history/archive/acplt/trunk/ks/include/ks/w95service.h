/* -*-plt-c++-*- */
/* $Header: /home/david/cvs/acplt/ks/include/ks/w95service.h,v 1.5 2007-04-25 12:57:20 martin Exp $ */

#ifndef KS_W95SERVICE_INCLUDED
#define KS_W95SERVICE_INCLUDED

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

// -------------------------------------------------------------------------
// The NT service object class. It hides the "magic" (bad magic, huh?) of
// NT's service mechanism and eases the creation of such services. The only
// thing to do is to derive your own service class from KsNtServiceServer
// and to implement the createServer() method. That's it. Gosh! OOP: you get
// the ease and we get the trouble...
//

//// Marshalling stubs ////
extern "C" int ks_c_w95serviceRun();
extern "C" LRESULT CALLBACK HiddenWindowProc(HWND, UINT, WPARAM, LPARAM);


class KsW95ServiceServer {
public:
    KsW95ServiceServer(const char *service_name, int argc, char **argv);
    virtual ~KsW95ServiceServer();

    void run();

    bool isOk() const;
    bool isVerbose() const;

    static KsW95ServiceServer *getService();

protected:
    virtual KsServerBase *createServer(int argc, char **argv) = 0;

    void lastError(const char *msg);
    void verbose(const char *msg);

    bool                       _is_ok;
    bool                       _is_verbose;
    bool                       _is_w95;
    const char                *_service_name;
    int                        _argc;
    char                     **_argv;
    KsServerBase              *_my_server;

    static KsW95ServiceServer *_the_service;

    HWND                       _hidden_window;

    HANDLE                     _service_done_event;
    HANDLE                     _workhorse_thread;
    volatile DWORD             _workhorse_thread_id;

private:
    virtual int serviceRun();

    friend int ks_c_w95serviceRun();
    friend LRESULT CALLBACK HiddenWindowProc(HWND, UINT, WPARAM, LPARAM);
}; // KsW95ServiceServer


#endif /* #ifdef KS_W95SERVICE_INCLUDED */

/* End of w95service.h */
