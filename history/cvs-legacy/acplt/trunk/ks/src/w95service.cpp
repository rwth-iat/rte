/* -*-plt-c++-*- */
/* $Header: /home/david/cvs/acplt/ks/src/w95service.cpp,v 1.1 1997-12-02 10:20:57 harald Exp $ */
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

// ATTENTION!!! We're now going multi-threaded!!!
// TODO: Why doesn't borland define this automatically when compiling for 
// multithreaded app?
#define __MT__

#include "ks/w95service.h"
#include "plt/log.h"
#include <stdio.h>
#if PLT_COMPILER_MSVC
#include <process.h>
#endif

// -------------------------------------------------------------------------
// We *do* need our instance handle but don't want to bother the programmer
// with it, so that's it why there is this extern declaration...
//
extern "C" HINSTANCE _hInstance;


// -------------------------------------------------------------------------
// This pointer points to the (only) instance of the service object. We do
// not support more than one service object. This would be too much trouble
// and surely not worth the effort. It's much easier and really much more
// stable to work with only one service per service executable.
//
KsW95ServiceServer *KsW95ServiceServer::_the_service = 0;


// -------------------------------------------------------------------------
// Construct a new service server object. Note that there can be only one
// such service object.
//
KsW95ServiceServer::KsW95ServiceServer(const char *service_name,
				       int argc, char **argv)
    : _is_ok(false), _is_verbose(false),
      _is_w95(false),
      _service_name(service_name),
      _argc(argc), _argv(argv),
      _hidden_window(0),
      _service_done_event(0),
      _workhorse_thread(0), _workhorse_thread_id(0),
      _my_server(0)
{
    if ( _the_service ) {
	PltLog::Error("Thou shall not have another KsW95ServiceServer");
	return;
    }
    //
    // Check whether we're running on Windooze '95 and register ourself
    // as a service process which will continue running even if the user
    // logs out.
    //
    OSVERSIONINFO osInfo;
    osInfo.dwOSVersionInfoSize = sizeof(osInfo);
    DWORD (*RegisterServiceProcess)(DWORD, DWORD) = 0;

    if ( !GetVersionEx(&osInfo) || 
        (osInfo.dwPlatformId != VER_PLATFORM_WIN32_WINDOWS) ||
        ((RegisterServiceProcess = (DWORD(*)(DWORD,DWORD)) GetProcAddress(
            GetModuleHandle("KERNEL32.DLL"), "RegisterServiceProcess")) == 0) ) {
        MessageBox(0,
                   "Note: this service is intended for Windows 95/96/97... only.\n"
                   "It will only run until you log out.",
                   _service_name ? _service_name : "W95 KS Service Server Object",
                   MB_ICONEXCLAMATION | MB_OK);
    } else {
#define RSP_SIMPLE_SERVICE 0x00000001l
        if ( RegisterServiceProcess(0, RSP_SIMPLE_SERVICE) ) {
            _is_w95 = true;
        } else {
            MessageBox(0,
                       "Can't register service process.\n"
                       "It will only run until you log out.",
                       _service_name ? _service_name : "W95 KS Service Server Object",
                       MB_ICONEXCLAMATION | MB_OK);
        }
    }
    //
    // As we need a dummy window to receive the WM_ENDSESSION, we now
    // register a new window class and then try to create a hidden window.
    //
    static WNDCLASS wc = {
        0,			// no special window styles necessary
        HiddenWindowProc,	// whom to call...
        0, 0,			// no extra class or window bytes needed
        _hInstance,
        0, 0,			// no icon and no cursor necessary
        0,			// no background
        0,			// no menu needed
        "W95ServiceProcessHiddenWindowClass"
    };

    if ( RegisterClass(&wc) == 0 ) {
        return;
    }
    _hidden_window = CreateWindow(
	"W95ServiceProcessHiddenWindowClass",
	"message for you, Sir!",
	WS_OVERLAPPED,		// simple top-level window
	0, 0, 1, 1,		// arbitrary window size and position
	0,			// no parent (well -- desktop)
	0,			// no menu
	_hInstance,
	0);			// no special pointer needed
    if ( _hidden_window == 0 ) {
        return;
    }
    //
    // See ntservice.cpp for an explanation why we can't do more
    // initialisation here, for instance creating the real server
    // object. Hint: think about virtual functions and vtables...
    //
    _the_service = this;
    _is_ok = true;
} // KsW95ServiceServer::KsW95ServiceServer

// -------------------------------------------------------------------------
// Clean up the service object.
//
KsW95ServiceServer::~KsW95ServiceServer()
{
    if ( _my_server ) { // should have already been done...
        delete _my_server; _my_server = 0;
    }
    if ( _hidden_window ) {
	DestroyWindow(_hidden_window); _hidden_window = 0;
    }
    if ( _service_done_event ) {
        CloseHandle(_service_done_event); _service_done_event = 0;
    }
    if ( _the_service == this ) {
        _the_service = 0;
    }
} // KsW95ServiceServer::~KsW95ServiceServer


// -------------------------------------------------------------------------
// Returns a pointer to the (one and only) service object, or 0, if there's
// currently no such object instantiated.
//
KsW95ServiceServer *KsW95ServiceServer::getService()
{
    return _the_service;
} // KsW95ServiceServer::getService


// -------------------------------------------------------------------------
// Returns, whether the service object is okay.
//
bool KsW95ServiceServer::isOk() const
{
    return _is_ok;
} // KsW95ServiceServer::isOk


// -------------------------------------------------------------------------
// Convenience function for reporting errors through the log mechanism. If
// we got stuck with a NT error, then this sucker spits out a message and
// appends the error code (in hex) too.
//
void KsW95ServiceServer::lastError(const char *msg)
{
    int error = (int) GetLastError();
    char buff[1024];

    strncpy(buff, msg, sizeof(buff) - 32);
    sprintf(buff + strlen(buff), " (Last Error: %08X)", error);
    PltLog::Error(buff);
} // KsW95ServiceServer::lastError


// -------------------------------------------------------------------------
// Spit out a debugging message only if the service object is today verbose.
//
void KsW95ServiceServer::verbose(const char *msg)
{
    if ( _is_verbose ) {
        PltLog::Debug(msg);
    }
} // KsW95ServiceServer::verbose


// -------------------------------------------------------------------------
//
//
void KsW95ServiceServer::run()
{
    if ( _is_ok ) {

	if ( _service_done_event == 0 ) {
	    _service_done_event = CreateEvent(0, FALSE, FALSE, 0);
	    if ( _service_done_event == 0 ) {
		_is_ok = false;
		PltLog::Error("Can't initialize signalling mechanism");
		return;
	    }
	}

	_my_server = createServer(_argc, _argv);
	if ( !_is_ok || (_my_server == 0) || !_my_server->isOk() ) {
	    _is_ok = false;
	    PltLog::Error("abnormal service termination");
	    return;
	}

	DWORD thread_id;
#if PLT_COMPILER_BORLAND
	_workhorse_thread = (HANDLE) _beginthreadNT(
           (void (*)(void *)) ks_c_w95serviceRun,
	   32768,
	   0,
	   0,
	   0,
	   &thread_id);
#else
# error "Don't know how to start threads"
#endif
	_workhorse_thread_id = thread_id;
        if ( _workhorse_thread <= 0 ) {
            _is_ok = false;
            PltLog::Error("Can't start service thread");
            return;
        }
        verbose("Service thread started");
	//
	// Now keep on going, dispatching messages until we'll notify
	// ourself to quit the game.
	//
	MSG messageForYouSir;
	while ( GetMessage(&messageForYouSir, 0, 0, 0) > 0 ) {
	    DispatchMessage(&messageForYouSir);
	}

        if ( _workhorse_thread > 0 ) {
            TerminateThread(_workhorse_thread, 0);
        }
        _workhorse_thread = 0;

	if ( _my_server ) {
            delete _my_server; _my_server = 0;
        }
    }
} // KsW95ServiceServer::run


// -------------------------------------------------------------------------
// Marshalling wrapper for the real service's main thread.
//
extern "C" ks_c_w95serviceRun()
{
    return KsW95ServiceServer::_the_service->serviceRun();
} // ks_c_w95serviceRun


// -------------------------------------------------------------------------
//
int KsW95ServiceServer::serviceRun()
{
    verbose("Server object spinning up");
    _my_server->startServer();
    verbose("Server object running");
    _my_server->run();
    verbose("Server object spinning down");
    _my_server->stopServer();
    verbose("Server object stopped");
    //
    // Signal that we've now through... By sending a WM_QUIT message
    // we make sure that in case we've arrived here too soon, the
    // run() method will terminate. The "done" event is used for
    // ordinarily shutdowns, where we must delay the WM_ENDSESSION
    // message until we're through at this place.
    //
    SetEvent(_service_done_event);
    PostQuitMessage(0);
    _workhorse_thread = 0; // we're dead very soon...
    return 0;
} // KsW95ServiceServer::serviceRun


// -------------------------------------------------------------------------
// As the messages come swirrling around, we just watch for WM_ENDSESSIONs
// to appear. If we see such one and the system is really going down, then
// we will first stop the server thread.
//
extern "C" LRESULT CALLBACK
HiddenWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch ( uMsg ) {
    case WM_ENDSESSION: {
	KsW95ServiceServer *service = KsW95ServiceServer::_the_service;
	if ( ((BOOL) wParam) && 
             (!(service && service->_is_w95) || !((BOOL) lParam)) ) {
	    //
	    // The system is going down. Run, run, run awaaayy!!!
	    //
	    // If we really have a server object, then we notify it to
	    // stop as soon as possible and wait for approx. 10 seconds
	    // to finish the job.
	    //
	    if ( service ) {
		if ( service->_my_server ) {
		    service->_my_server->downServer();
		    if ( WaitForSingleObject(
			     service->_service_done_event, 10 * 1000 ) !=
			 WAIT_OBJECT_0 ) {
			PltLog::Warning("\Service Done\" signalling failed");
		    }
		}
		service->verbose("Service thread now terminating");
	    }
	    //
	    // Switch the lights off when you go... We're just making
	    // here sure that the run() method terminates...
	    //
	    PostQuitMessage(0);
	}
	break;
    } // case WM_ENDSESSION
    }
    return DefWindowProc(hWnd, uMsg, wParam, lParam);
} // HiddenWindowProc


/* End of w95service.cpp */
