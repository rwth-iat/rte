/* -*-plt-c++-*- */
/* $Header: /home/david/cvs/acplt/ks/src/ntservice.cpp,v 1.9 2007-04-25 12:57:20 martin Exp $ */
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

// ATTENTION!!! We're now going multi-threaded!!!
// TODO: Why doesn't borland define this automatically when compiling for 
// multithreaded app?
#ifndef __MT__
#define __MT__
#endif

#include "ks/ntservice.h"
#include "plt/log.h"
#include <stdio.h>
#if PLT_COMPILER_MSVC
#include <process.h>
#endif

// -------------------------------------------------------------------------
// This pointer points to the (only) instance of the service object. We do
// not support more than one service object. This would be too much trouble
// and surely not worth the effort. It's much easier and really much more
// stable to work with only one service per service executable.
//
KsNtServiceServer *KsNtServiceServer::_the_service = 0;

// -------------------------------------------------------------------------
// Construct a new service server object. Note that there can be only one
// such service object.
//
KsNtServiceServer::KsNtServiceServer(const char *service_name,
                                     unsigned int create_timeout,
                                     unsigned int spinup_timeout,
                                     unsigned int spindown_timeout,
                                     unsigned int destroy_timeout)
    : _is_ok(false), _is_verbose(false),
      _service_name(service_name),
      _service_status_handle(0),
      _service_done_event(0), _one_shot_event(0),
      _workhorse_thread(0), _workhorse_thread_id(0),
      _my_server(0),
      _create_to(create_timeout), _destroy_to(destroy_timeout),
      _spinup_to(spinup_timeout), _spindown_to(spindown_timeout)
{
    if ( _the_service ) {
        PltLog::Error("Thou shall not have another KsNtServiceServer");
        return;
    }
    _the_service = this;

    _service_status.dwServiceType             = SERVICE_WIN32_OWN_PROCESS;
    _service_status.dwCurrentState            = SERVICE_START_PENDING;
    _service_status.dwControlsAccepted        = 0;
    _service_status.dwWin32ExitCode           = 0;
    _service_status.dwServiceSpecificExitCode = 0;
    _service_status.dwCheckPoint              = 0;
    _service_status.dwWaitHint                = 0;
    //
    // That must be enough initialization mambo jumbo for now. Note that
    // we can't create here the server object, because createServer is
    // a virtual function and we're right here inside the constructor. So
    // calling virtual functions at this point is definetely a no-no.
    // Believe me, I've tried it... ;-)
    //
    _is_ok = true;
} // KsNtServiceServer::KsNtServiceServer

// -------------------------------------------------------------------------
// Clean up the service object.
//
KsNtServiceServer::~KsNtServiceServer()
{
    if ( _my_server ) { // should have already been done...
        delete _my_server;
        _my_server = 0;
    }
    if ( _service_done_event ) {
        CloseHandle(_service_done_event); _service_done_event = 0;
    }
    if ( _one_shot_event ) {
        CloseHandle(_one_shot_event); _one_shot_event = 0;
    }
    if ( _the_service == this ) {
        _the_service = 0;
    }
} // KsNtServiceServer::~KsNtServiceServer


// -------------------------------------------------------------------------
// Returns a pointer to the (one and only) service object, or 0, if there's
// currently no such object instantiated.
//
KsNtServiceServer *KsNtServiceServer::getService()
{
    return KsNtServiceServer::_the_service;
} // KsNtServiceServer::getService


// -------------------------------------------------------------------------
// Returns, whether the service object is okay.
//
bool KsNtServiceServer::isOk() const
{
    return _is_ok;
} // KsNtServiceServer::isOk


// -------------------------------------------------------------------------
// Convenience function for reporting errors through the log mechanism. If
// we got stuck with a NT error, then this sucker spits out a message and
// appends the error code (in hex) too.
//
void KsNtServiceServer::lastError(const char *msg)
{
    int error = (int) GetLastError();
    char buff[1024];

    strncpy(buff, msg, sizeof(buff) - 32);
    sprintf(buff + strlen(buff), " (Last Error: %08X)", error);
    PltLog::Error(buff);
} // KsNtServiceServer::lastError

// -------------------------------------------------------------------------
// Spit out a debugging message only if the service object is today verbose.
//
void KsNtServiceServer::verbose(const char *msg)
{
    if ( _is_verbose ) {
        PltLog::Debug(msg);
    }
} // KsNtServiceServer::verbose


// -------------------------------------------------------------------------
// Report the current health state of the service process to the "operating
// system". In addition, you can specify an exit code, as well as some
// information about how long to wait before the "operating system" can
// assume that this service just passed away...
//
// Note: if you pass in "user_code" a value not equal zero, then "exit_code"
// is automatically set to ERROR_SERVICE_SPECIFIC_ERROR, regardless of the
// value you pass in.
//
bool KsNtServiceServer::reportServiceStatus(DWORD curr_stat, DWORD exit_code,
                                            DWORD user_code, DWORD checkpoint,
                                            DWORD wait_hint)
{
    _service_status.dwCurrentState = curr_stat;
    if ( curr_stat == SERVICE_START_PENDING ) {
        _service_status.dwControlsAccepted = 0;
    } else {
        _service_status.dwControlsAccepted = SERVICE_ACCEPT_STOP |
                                             SERVICE_ACCEPT_PAUSE_CONTINUE;
    }
    if ( user_code == 0 ) {
        _service_status.dwWin32ExitCode = exit_code;
    } else {
        _service_status.dwWin32ExitCode = ERROR_SERVICE_SPECIFIC_ERROR;
        _service_status.dwServiceSpecificExitCode = user_code;
    }
    _service_status.dwCheckPoint    = checkpoint;
    _service_status.dwWaitHint      = wait_hint;
    return SetServiceStatus(_service_status_handle,
                            &_service_status);
} // KsNtServiceServer::reportServiceStatus

// -------------------------------------------------------------------------
// Convenience accessor for querying the current service status. This can
// be used if you want to call reportServiceStatus() but you can't be sure
// of the exact current state the service is in. If you don't understand
// what I said, then go off and look at serviceController().
//
DWORD KsNtServiceServer::currentServiceStatus()
{
    return _service_status.dwCurrentState;
} // KsNtServiceServer::currentServiceStatus()


// -------------------------------------------------------------------------
// Start the service, that is, register the control dispatcher, which will
// in turn create the server object. We can't do this in the constructor,
// as createServer() must be a virtual function, and we can't call a
// virtual function from within the ctor.
//
void KsNtServiceServer::run()
{
    SERVICE_TABLE_ENTRY service_dispatch_table[2];

    service_dispatch_table[0].lpServiceName = (LPTSTR) _service_name;
    service_dispatch_table[0].lpServiceProc = (LPSERVICE_MAIN_FUNCTION)
                                                  ks_c_serviceMain;
    service_dispatch_table[1].lpServiceName = 0;
    service_dispatch_table[1].lpServiceProc = 0;
    if ( !StartServiceCtrlDispatcher(service_dispatch_table) ) {
        _is_ok = false;
	lastError("Can't start service control dispatcher");
        return;
    }
} // KsNtServiceServer::run


// -------------------------------------------------------------------------
// Marshal NT's call for the main function of this service up to the
// service object.
//
extern "C" void ks_c_serviceMain(DWORD argc, LPTSTR *argv)
{
    KsNtServiceServer::_the_service->serviceMain((int) argc, (char **) argv);
} // ks_c_serviceMain

// -------------------------------------------------------------------------
//
void KsNtServiceServer::serviceMain(int argc, char **argv)
{
    //
    // First, create some signaling objects. The _one_shot_event signals
    // the serviceRun() task that it should wake up and iterate once again,
    // thus spinning up the KS server, and process requests. The next event,
    // _service_done_event, signals, that the whole service task (executable)
    // is now ready to cease.
    //
    _service_done_event = CreateEvent(0, FALSE, FALSE, 0);
    _one_shot_event     = CreateEvent(0, FALSE, FALSE, 0);
    if ( !_service_done_event || !_one_shot_event ) {
        lastError("Can't initialize signaling mechanism");
    }
    //
    // Next, register the service controller. Remember, that only after we've
    // done this, we can report the service status to the service manager.
    //
    _service_status_handle = RegisterServiceCtrlHandler(
                                 (LPCTSTR) _service_name,
                                 (LPHANDLER_FUNCTION) ks_c_serviceController);
    if ( !_service_status_handle ) {
        lastError("Can't register service control handler");
        return;
    }
    //
    // Next, we ask the developer for a KS server object, which will be
    // subsequently used within the real workhorse thread.
    //
    reportServiceStatus(SERVICE_START_PENDING, NO_ERROR, 0, 1, _create_to);
    _my_server = createServer((int) argc, (char **) argv);
    if ( !_is_ok || (_my_server == 0) || !_my_server->isOk() ) {
    	_is_ok = false;
        PltLog::Error("abnormal service termination");
        return;
    }
    verbose("Created server object successfully");
    reportServiceStatus(SERVICE_START_PENDING, NO_ERROR, 0, 2, _spinup_to);
    //
    // We must use the apropriate function from the C++ RTL here, as we will
    // otherwise get memory leaks. As if NT wouldn't be the biggest memory
    // leak ever created by menkind...
    //
#if PLT_COMPILER_MSVC
    _workhorse_thread = (HANDLE) _beginthread(
        (void (*)(void *)) ks_c_serviceRun,    // entry point of new thread
        0,                                     // stack size (default)
        0);                                    // arguments (none)
#else
# if PLT_COMPILER_BORLAND
    _workhorse_thread = (HANDLE) _beginthreadNT(
        (void (*)(void *)) ks_c_serviceRun, // entry point of new thread
        32768,                           // initial stack size
        0,                               // no arguments to pass to thread
        0,                               // no security, as NT hasn't one...
        0,                               // spin up thread immediately
        &_workhorse_thread_id);
# else
#  error "Don't know how to start threads"
# endif
#endif

    if ( _workhorse_thread <= 0) {
    	_is_ok = false;
        PltLog::Error("Can't start service thread");
        return;
    }
    verbose("Started service thread");
    //
    // Gosh! Finally, we're up and running. Yeah, I can't believe it!
    //
    reportServiceStatus(SERVICE_RUNNING, NO_ERROR, 0, 0, 0);
    if ( WaitForSingleObject(_service_done_event, INFINITE) !=
            WAIT_OBJECT_0 ) {
        PltLog::Warning("\"Service Done\" signaling failed");
    }
    verbose("Service thread woken up");
    if ( !TerminateThread(_workhorse_thread, 0) ) {
    	lastError("Can't terminate service thread");
    }
    if ( _my_server ) {
        delete _my_server; _my_server = 0;
    }
    //
    // Simply terminate. This will throw us back to the main thread.
    //
    reportServiceStatus(SERVICE_STOPPED,
                        _service_status.dwWin32ExitCode,
                        _service_status.dwServiceSpecificExitCode, 0, 0);
} // KsNtServiceServer::serviceMain


// -------------------------------------------------------------------------
// Marshal the service control request from Windooze NoThnx up to the
// service object.
//
extern "C" void WINAPI ks_c_serviceController(DWORD control_code)
{
    KsNtServiceServer::_the_service->serviceController(control_code);
} // ks_c_serviceController

// -------------------------------------------------------------------------
// Yuck. We've got a service control request and must now look for our real
// service task. We must now either control the startup or shutdown of this
// service task which is responsible for servicing KS requests.
//
void KsNtServiceServer::serviceController(DWORD control_code)
{
    DWORD current_state = currentServiceStatus();

    switch ( control_code ) {
    //
    // Spin down the service... We just notify the server object and then
    // return to the caller, because it may take some time to bring the
    // KS server down. The serviceRun() method is then responsible to
    // signal the main thread that the server is now ready to be burried.
    //
    case SERVICE_CONTROL_STOP:
        if ( current_state == SERVICE_RUNNING ) {
            verbose("About to stop the server object");
            reportServiceStatus(SERVICE_STOP_PENDING, NO_ERROR, 0, 1,
                                _spindown_to);
            _my_server->downServer();
            return;
        } else if ( current_state == SERVICE_PAUSED ) {
            verbose("Server object already stopped");
            reportServiceStatus(SERVICE_STOP_PENDING, NO_ERROR, 0, 1,
                                _destroy_to);
            SetEvent(_service_done_event);
            return;
        }
        break;
    //
    // Send the service to sleep. Once again, we just notify the real server
    // object that it should enter the sabbat.
    //
    case SERVICE_CONTROL_PAUSE:
        if ( current_state == SERVICE_RUNNING ) {
            verbose("About to pause the server object");
	    reportServiceStatus(SERVICE_PAUSE_PENDING, NO_ERROR, 0, 1,
                                _spindown_to);
            _my_server->downServer();
            return;
        }
        break;
    //
    // Sabbat is over, back to work.
    //
    case SERVICE_CONTROL_CONTINUE:
        if ( current_state == SERVICE_PAUSED ) {
            verbose("About to restart the server object");
	    reportServiceStatus(SERVICE_CONTINUE_PENDING, NO_ERROR, 0,
                                1, _spinup_to);
            SetEvent(_one_shot_event);
            return;
        }
        break;
    }
    //
    // Otherwise report back the current state.
    //
    SetServiceStatus(_service_status_handle, &_service_status);
} // KsNtServiceServer::serviceController


// -------------------------------------------------------------------------
// Marshalling wrapper for the real service's main thread.
//
extern "C" ks_c_serviceRun()
{
    return KsNtServiceServer::_the_service->serviceRun();
} // ks_c_serviceRun

// -------------------------------------------------------------------------
// This function is basically the main loop of the KS server task. It loops
// until it gets killed, and for each iteration, it first spins up the
// server object, let the server object service incomming requests, and
// eventually shuts it down. After this work has been done, the server
// task enters the sabbat, thereby waiting for a signal to arrive, which
// tells it that the new week has just begun and work is waiting...
//
int KsNtServiceServer::serviceRun()
{
    for ( ;; ) {
        //
        // Run one duty-cycle on the server object...
        //
        verbose("Server object spinning up");
	reportServiceStatus(currentServiceStatus(), NO_ERROR, 0, 2,
                            _spinup_to);
        _my_server->startServer();
        verbose("Server object running");
	reportServiceStatus(SERVICE_RUNNING, NO_ERROR, 0, 0, 0);
        _my_server->run();
        verbose("Server object spinning down");
        reportServiceStatus(currentServiceStatus(), NO_ERROR, 0, 2,
                            _spindown_to);
	_my_server->stopServer();
        switch ( currentServiceStatus() ) {
        case SERVICE_STOP_PENDING:
            //
            // As we're about to stop the service, we now signal to our main
            // thread that we're through. It then will kill us, indicate
            // the service manager that it has stopped the server and
            // finally return to someplace within the singularity of NT.
            //
            reportServiceStatus(SERVICE_STOP_PENDING, NO_ERROR, 0, 3,
                                _destroy_to);
            verbose("Server object stopped");
            SetEvent(_service_done_event);
            break;
        case SERVICE_PAUSE_PENDING:
            verbose("Server object paused");
            reportServiceStatus(SERVICE_PAUSED, NO_ERROR, 0, 0, 0);
            break;
        default:
            //
            // Otherwise we signal, that the service has been stopped
            // successfully.
            //
            verbose("Server object stopped");
            reportServiceStatus(SERVICE_STOP, NO_ERROR, 0, 0, 0);
        }
        //
        // Wait for someone other to wake us up when the new week has
        // started and the sabbat is over.
        //
        WaitForSingleObject(_the_service->_one_shot_event, INFINITE);
    }
    PltLog::Alert("Fallen off the serviceRun() loop!");
    return 0;
} // KsNtServiceServer::serviceRun

/* End of ntservice.cpp */

