/*********************************************************************
 * RPC for the Windows NT Operating System
 * 1993 by Martin F. Gergeleit
 *
 * RPC for the Windows NT Operating System COMES WITH ABSOLUTELY NO
 * WARRANTY, NOR WILL I BE LIABLE FOR ANY DAMAGES INCURRED FROM THE
 * USE OF. USE ENTIRELY AT YOUR OWN RISK!!!
 *********************************************************************/
/*
 *  Added on suggestion from Martin Johnson (MJJ)
 *
 *  The -v flag means VERBOSE, i.e. generate NT Application
 *  Event Log messages in Portmap.c/find_service() when
 *  clients try to connect.
 *
 *  To enable VERBOSE mode the service has to be started manually from
 *  Control Panel/Services, with something in the "Startup Parameters"
 *  field.  (Note: "NET START PORTMAP -V" won't work as the -V gets
 *  thrown away.)
 */
/*
 * Harald Albrecht: added self-registration and self-deregistration
 * to ease automatic installation of the portmapper (especially in
 * conjunction with installation programs).
 */

#include <rpc/rpc.h>
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <rpc/pmap_pro.h>

void reg_service();

HANDLE                  hServDoneEvent = NULL;
SERVICE_STATUS          ssStatus;
SERVICE_STATUS_HANDLE   sshStatusHandle;
DWORD                   dwGlobalErr;
DWORD                   TID = 0;
HANDLE                  threadHandle = NULL;
HANDLE                  pipeHandle;

VOID    service_main(DWORD dwArgc, LPTSTR *lpszArgv);
VOID    WINAPI service_ctrl(DWORD dwCtrlCode);
BOOL    ReportStatusToSCMgr(DWORD dwCurrentState,
                            DWORD dwWin32ExitCode,
                            DWORD dwCheckPoint,
                            DWORD dwWaitHint);
VOID    portmap_main(VOID *notUsed);
VOID    StopPortmapService(LPTSTR lpszMsg);
VOID	Report(LPTSTR lpszMsg);



/* --------------------------------------------------------------------------
 * The self-registration magic begins here...
 */
#include "servregi.h"


int main(int argc, char **argv)
{
    SERVICE_TABLE_ENTRY dispatchTable[] = {
        { TEXT("PortmapService"), (LPSERVICE_MAIN_FUNCTION)service_main },
        { NULL, NULL }
    };

    /*
     * First check for some options when run manually from the command
     * prompt...
     */
    if ( argc == 2 ) {
        if ( (strcmpi(argv[1], "/registerservice") == 0) ||
             (strcmpi(argv[1], "-registerservice") == 0) ) {
            return RegisterService(GetModuleHandle(0), NtServiceRegistration);
        }
        if ( (strcmpi(argv[1], "/unregisterservice") == 0) ||
             (strcmpi(argv[1], "-unregisterservice") == 0) ) {
            return UnregisterService(GetModuleHandle(0), NtServiceRegistration);
	    }
    }

    if (!StartServiceCtrlDispatcher(dispatchTable)) {
        StopPortmapService("StartServiceCtrlDispatcher failed.");
    }
    return 0;
}


int verbose = 0;              /* MJJ: 14-11-96 added this variable */

VOID
service_main(DWORD dwArgc, LPTSTR *lpszArgv)

{
    DWORD                   dwWait;
    int sock;
    struct sockaddr_in addr;
    SVCXPRT *xprt;
    int len = sizeof(struct sockaddr_in);

    if (dwArgc > 1)             /* MJJ: 14-11-96 added this 'if' block */
    {
      /*
       *  If we get here then the service was started manually from 
       *  Control Panel/Services, with something in the "Startup Parameters"
       *  field.  (Note: "NET START PORTMAP -V" won't work as the -V gets
       *  thrown away.)
       */
      DWORD i;
      for (i=1; i<dwArgc; i++)
        if ( (!strcmp(lpszArgv[i], "-v")) || (!strcmp(lpszArgv[i], "-V")) )
          verbose = 1;
    }

    sshStatusHandle = RegisterServiceCtrlHandler(
                                    TEXT("PortmapService"),
                                    (LPHANDLER_FUNCTION)service_ctrl);

    if (!sshStatusHandle)
        goto exit_portmap;

    ssStatus.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
    ssStatus.dwServiceSpecificExitCode = 0;

    if (!ReportStatusToSCMgr(SERVICE_START_PENDING, NO_ERROR, 1, 3000))
        goto exit_portmap;

    hServDoneEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

    if (hServDoneEvent == (HANDLE)NULL)
        goto exit_portmap;


    if (!ReportStatusToSCMgr(SERVICE_START_PENDING, NO_ERROR, 2, 3000))
        goto exit_portmap;

    if (rpc_nt_init() != 0) {
	goto exit_portmap;
	}

	if ((sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == INVALID_SOCKET) {
		StopPortmapService("cannot create socket");
		return;
	}

	addr.sin_addr.s_addr = 0;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(PMAPPORT);
	if (bind(sock, (struct sockaddr *)&addr, len) != 0) {
		StopPortmapService("cannot bind");
		return;
	}

	if ((xprt = svcudp_create(sock)) == (SVCXPRT *)NULL) {
		StopPortmapService("couldn't do udp_create");
		return;
	}

	if ((sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
		StopPortmapService("cannot create socket");
		return;
	}
	if (bind(sock, (struct sockaddr *)&addr, len) != 0) {
		StopPortmapService("cannot bind");
		return;
	}
	if ((xprt = svctcp_create(sock, 0, 0)) == (SVCXPRT *)NULL) {
		StopPortmapService("couldn't do tcp_create");
		return;
	}

        (void)svc_register(xprt, PMAPPROG, PMAPVERS, reg_service, FALSE);

    threadHandle = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)svc_run, NULL, 0, &TID);

    if (!threadHandle)
        goto exit_portmap;

    if (!ReportStatusToSCMgr(SERVICE_RUNNING, NO_ERROR, 0, 0))
        goto exit_portmap;

    dwWait = WaitForSingleObject(hServDoneEvent, INFINITE);

    TerminateThread(threadHandle, 0);

exit_portmap:

    Report("Portmap service terminates");

    rpc_nt_exit();

    if (hServDoneEvent != NULL)
        CloseHandle(hServDoneEvent);

    if (sshStatusHandle != 0)
        (VOID)ReportStatusToSCMgr(SERVICE_STOPPED, dwGlobalErr, 0, 0);

    return;
}


VOID
WINAPI service_ctrl(DWORD dwCtrlCode)
{
    DWORD  dwState = SERVICE_RUNNING;

    switch(dwCtrlCode) {

        case SERVICE_CONTROL_PAUSE:

            if (ssStatus.dwCurrentState == SERVICE_RUNNING) {
                SuspendThread(threadHandle);
                dwState = SERVICE_PAUSED;
            }
            break;

        case SERVICE_CONTROL_CONTINUE:

            if (ssStatus.dwCurrentState == SERVICE_PAUSED) {
                ResumeThread(threadHandle);
                dwState = SERVICE_RUNNING;
            }
            break;

        case SERVICE_CONTROL_STOP:

            dwState = SERVICE_STOP_PENDING;

            ReportStatusToSCMgr(SERVICE_STOP_PENDING, NO_ERROR, 1, 3000);

            SetEvent(hServDoneEvent);
            return;

        case SERVICE_CONTROL_INTERROGATE:
            break;

        default:
            break;

    }

    ReportStatusToSCMgr(dwState, NO_ERROR, 0, 0);
}


BOOL
ReportStatusToSCMgr(DWORD dwCurrentState,
                    DWORD dwWin32ExitCode,
                    DWORD dwCheckPoint,
                    DWORD dwWaitHint)
{
    BOOL fResult;

    if (dwCurrentState == SERVICE_START_PENDING)
        ssStatus.dwControlsAccepted = 0;
    else
        ssStatus.dwControlsAccepted = SERVICE_ACCEPT_STOP |
            SERVICE_ACCEPT_PAUSE_CONTINUE;

    ssStatus.dwCurrentState = dwCurrentState;
    ssStatus.dwWin32ExitCode = dwWin32ExitCode;
    ssStatus.dwCheckPoint = dwCheckPoint;

    ssStatus.dwWaitHint = dwWaitHint;

    if (!(fResult = SetServiceStatus(sshStatusHandle, &ssStatus)))
            StopPortmapService("SetServiceStatus");

    return fResult;
}


VOID
StopPortmapService(LPTSTR lpszMsg)
{
    CHAR    chMsg[256];
    HANDLE  hEventSource;
    LPTSTR  lpszStrings[2];

    dwGlobalErr = GetLastError();

    hEventSource = RegisterEventSource(NULL,
                            TEXT("Portmap"));

    sprintf(chMsg, "Portmap error: %d", dwGlobalErr);
    lpszStrings[0] = chMsg;
    lpszStrings[1] = lpszMsg;

    if (hEventSource != NULL) {
        ReportEvent(hEventSource,
            EVENTLOG_ERROR_TYPE,
            0,
            0,
            NULL,
            2,
            0,
            lpszStrings,
            NULL);

        (VOID) DeregisterEventSource(hEventSource);
    }

    SetEvent(hServDoneEvent);
}


VOID
Report(LPTSTR lpszMsg)
{
    CHAR    chMsg[256];
    HANDLE  hEventSource;
    LPTSTR  lpszStrings[2];

    dwGlobalErr = GetLastError();

    hEventSource = RegisterEventSource(NULL,
                            TEXT("Portmap"));

    sprintf(chMsg, "Portmap report: %d", dwGlobalErr);
    lpszStrings[0] = chMsg;
    lpszStrings[1] = lpszMsg;

    if (hEventSource != NULL) {
        ReportEvent(hEventSource,
            EVENTLOG_INFORMATION_TYPE,
            0,
            0,
            NULL,
            2,
            0,
            lpszStrings,
            NULL);

        (VOID) DeregisterEventSource(hEventSource);
    }
}
