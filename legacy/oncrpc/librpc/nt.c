/*********************************************************************
 * RPC for the Windows NT Operating System
 * 1993 by Martin F. Gergeleit
 *
 * RPC for the Windows NT Operating System COMES WITH ABSOLUTELY NO 
 * WARRANTY, NOR WILL I BE LIABLE FOR ANY DAMAGES INCURRED FROM THE 
 * USE OF. USE ENTIRELY AT YOUR OWN RISK!!!
 *********************************************************************/

#include <rpc/rpc.h>
#include <stdio.h>
#include <winsock.h>

WSADATA WSAData;

static int init = 0;

int rpc_nt_init(void)
{
    if (init++)
	return 0;
	
    if (WSAStartup(0x0101, &WSAData)) {
	init = 0;
	nt_rpc_report("WSAStartup failed\n");
	WSACleanup();
	return -1;
    }

    return 0;
}

int rpc_nt_exit(void)
{
    if (init == 0 || --init > 0)
	return 0;

    return WSACleanup();
}

VOID
nt_rpc_report(LPTSTR lpszMsg)
{
    CHAR    chMsg[256];
    HANDLE  hEventSource;
    LPTSTR  lpszStrings[2];

    // Use event logging to log the error.
    //
    hEventSource = RegisterEventSource(NULL,
                            TEXT("rpc.dll"));

    sprintf(chMsg, "sunrpc report: %d", GetLastError());
    lpszStrings[0] = chMsg;
    lpszStrings[1] = lpszMsg;

    if (hEventSource != NULL) {
        ReportEvent(hEventSource, // handle of event source
            EVENTLOG_WARNING_TYPE, // event type
            0,                    // event category
            0,                    // event ID
            NULL,                 // current user's SID
            2,                    // strings in lpszStrings
            0,                    // no bytes of raw data
            lpszStrings,          // array of error strings
            NULL);                // no raw data

        (VOID) DeregisterEventSource(hEventSource);
    }
}

