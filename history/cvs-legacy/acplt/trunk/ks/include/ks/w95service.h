/* -*-plt-c++-*- */
/* $Header: /home/david/cvs/acplt/ks/include/ks/w95service.h,v 1.1 1997-12-02 10:16:08 harald Exp $ */

#ifndef KS_W95SERVICE_INCLUDED
#define KS_W95SERVICE_INCLUDED

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
