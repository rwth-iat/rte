/*
*   $Id: ov_ntservice.c,v 1.4 1999-08-25 13:15:59 dirk Exp $
*
*   Copyright (C) 1998-1999
*   Lehrstuhl fuer Prozessleittechnik,
*   RWTH Aachen, D-52056 Aachen, Germany.
*   All rights reserved.
*
*   Author: Dirk Meyer <dirk@plt.rwth-aachen.de>
*
*   This file is part of the ACPLT/OV Package which is licensed as open
*   source under the Artistic License; you can use, redistribute and/or
*   modify it under the terms of that license.
*
*   You should have received a copy of the Artistic License along with
*   this Package; see the file ARTISTIC-LICENSE. If not, write to the
*   Copyright Holder.
*
*   THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR IMPLIED
*   WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED WARRANTIES
*   OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE.
*/
/*
*	History:
*	--------
*	21-Jul-1999 Dirk Meyer <dirk@plt.rwth-aachen.de>: File created.
*/
/*
*	Below you'll find a full-blown ACPLT/OV server, which runs as a native NT
*	service. Twist the registry, then reboot, and off you go with your new
*	ACPLT/OV server service.
*
*	NOTE: remember to link against the multi-threaded run-time libraries!
*
*	REGISTRY:
*	add the following entries:
*	  HKEY_LOGCAL_MACHINE\SYSTEM\CurrentControlSet\Services
*	    ov_ntservice
*	      DisplayName     (SZ)        "ACPLT/OV NT-Service"
*	      ImagePath       (EXPAND_SZ) "%SystemRoot%\system32\ov_ntservice.exe [options]"
*	      ObjectName      (SZ)        "LocalSystem"
*	      ErrorControl    (DWORD)     0x00000001
*	      Start           (DWORD)     0x00000002 (or 0x00000003 for "by hand")
*	      Type            (DWORD)     0x00000110
*	      DependOnService (MULTI_SZ)  "tcpip" "portmap" "ks_manager"
*
*	By the way: The ONC/RPC Portmapper and the ACPLT/KS Manager then must have
*	been entered into the REGISTRY too:
*	  HKEY_LOGCAL_MACHINE\SYSTEM\CurrentControlSet\Services
*	    KS_manager
*	      DisplayName     (SZ)        "ACPLT/KS Manager"
*	      ImagePath       (EXPAND_SZ) "%SystemRoot%\system32\ntksmanager.exe"
*	      ObjectName      (SZ)        "LocalSystem"
*	      ErrorControl    (DWORD)     0x00000001
*	      Start           (DWORD)     0x00000002 (or 0x00000003 for "by hand")
*	      Type            (DWORD)     0x00000010
*	      DependOnService (MULTI_SZ)  "tcpip" "portmap"
*
*	  HKEY_LOGCAL_MACHINE\SYSTEM\CurrentControlSet\Services
*	    Portmap
*	      DisplayName     (SZ)        "ONC/RPC Portmap"
*	      ImagePath       (EXPAND_SZ) "%SystemRoot%\system32\portmap.exe"
*	      ObjectName      (SZ)        "LocalSystem"
*	      ErrorControl    (DWORD)     0x00000001
*	      Start           (DWORD)     0x00000002 (or 0x00000003 for "by hand")
*	      Type            (DWORD)     0x00000010
*	      DependOnService (MULTI_SZ)  "tcpip"
*/

/*
*	This application is multi-threaded. Remember to link to the multi-threaded
*	libraries!
*/
#define __MT__	/* Borland: we go multi-threaded... */

#include <ctype.h>

#include "libovks/ov_ksserver.h"
#include "libov/ov_database.h"
#include "libov/ov_result.h"
#include "libov/ov_logfile.h"

/*
*	Currently, Cygwin does not have the "shellapi" header file
*/
#if OV_COMPILER_CYGWIN

typedef struct {
	DWORD	cbSize;
	HWND	hWnd;
	UINT	uID;
	UINT	uFlags;
	UINT	uCallbackMessage;
	HICON	hIcon;
	char	szTip[64];
}	NOTIFYICONDATA;

#define NIM_ADD		0x00000000
#define NIM_MODIFY	0x00000001
#define NIM_DELETE	0x00000002

#define NIF_MESSAGE	0x00000001
#define NIF_ICON	0x00000002
#define NIF_TIP		0x00000004

BOOL WINAPI Shell_NotifyIcon(
	DWORD			dwMessage, 
	NOTIFYICONDATA	*pnid
);

#endif

/*	----------------------------------------------------------------------	*/

/*
*	Constants
*/
/*
*	Makro: Username (usually an environment variable)
*/
#define OV_USERNAME		getenv("USERNAME")
#define OV_SERVICE_NAME	"ov_ntservice"
#define OV_DISPLAY_NAME "ACPLT/OV NT-Service"
#define OV_EXEPATH		"%SystemRoot%\\system32\\ov_ntservice.exe"
#define OV_DEPENDENCIES "tcpip\0portmap\0ks_manager\0"
#define OV_TIMEOUT		7000				/* milliseconds */

/*	----------------------------------------------------------------------	*/

/*
*	Global variables
*/
static HWND						hwnd;
static HICON					hicon[2];
static OV_BOOL					initialized = FALSE;
static OV_BOOL					is_ok;
static OV_STRING				service_name;
static SERVICE_STATUS_HANDLE	status_handle;
static SERVICE_STATUS			status;
static HANDLE					done_event;
static HANDLE					one_shot_event;
static HANDLE					work_thread;
static DWORD					work_thread_id;
static OV_STRING				username;
static OV_STRING				filename = "database.ovd";
static OV_STRING				servername = NULL;
static OV_INT 					port = 0; /* KS_ANYPORT */
static OV_BOOL					startup = TRUE;
static OV_BOOL					help = FALSE;
static OV_BOOL					version = FALSE;

/*	----------------------------------------------------------------------	*/

/*
*	Open the service control manager
*/
SC_HANDLE ov_ntservice_opencontrolmanager(void) {
	/*
	*	local variables
	*/
	SC_HANDLE	manager;
	/*
	*	open service control manager
	*/
	manager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if(!manager) {
		fprintf(stderr, "Unable to open the Service Control Manager.\n"
			"Do you have appropriate privileges?\n");
	}
	return manager;
}

/*	----------------------------------------------------------------------	*/

/*
*	Install the service (registry)
*/
int ov_ntservice_install(
	OV_UINT		numargs,
	OV_STRING	*args
) {
	/*
	*	local variables
	*/
	SC_HANDLE	service;
	SC_HANDLE	manager;
	OV_STRING	exepath;
	OV_UINT		size;
	OV_UINT		i;
	/*
	*	open the service control manager
	*/
	manager = ov_ntservice_opencontrolmanager();
	if(manager) {
		/*
		*	create exe path
		*/
		size = strlen(OV_EXEPATH)+numargs+1;
		for(i=0; i<numargs; i++) {
			size += strlen(args[i]);
		}
		exepath = (OV_STRING)malloc(size);
		if(exepath) {
			strcpy(exepath, OV_EXEPATH);
			for(i=0; i<numargs; i++) {
				strcat(exepath, " ");
				strcat(exepath, args[i]);
			}
			/*
			*	create the service
			*/
			service = CreateService(manager, OV_SERVICE_NAME, OV_DISPLAY_NAME,
				SERVICE_ALL_ACCESS, SERVICE_WIN32_OWN_PROCESS | SERVICE_INTERACTIVE_PROCESS,
				SERVICE_DEMAND_START, SERVICE_ERROR_NORMAL, exepath, NULL, NULL,
				OV_DEPENDENCIES, NULL, NULL);
			/*
			*	fine, check the results and clean up
			*/
			free(exepath);
			CloseServiceHandle(manager);
			if(service) {
				printf("Service successfully created.\n");
				CloseServiceHandle(service);
				return EXIT_SUCCESS;
			}
			fprintf(stderr, "Could not create service.\n");
			return EXIT_FAILURE;
		}
		fprintf(stderr, "Unable to allocate memory.\n");
		CloseServiceHandle(manager);
	}
	return EXIT_FAILURE;
}

/*	----------------------------------------------------------------------	*/

/*
*	Uninstall the service (registry)
*/
int ov_ntservice_uninstall(void) {
	/*
	*	local variables
	*/
	SC_HANDLE	service;
	SC_HANDLE	manager;
	/*
	*	open the service control manager
	*/
	manager = ov_ntservice_opencontrolmanager();
	if(manager) {
		/*
		*	open the service
		*/
		service = OpenService(manager, OV_SERVICE_NAME, SERVICE_ALL_ACCESS);
		if(service) {
			if(DeleteService(service)) {
				printf("Service successfully deleted.\n");
				CloseServiceHandle(service);
				CloseServiceHandle(manager);
				return EXIT_SUCCESS;
			}
			printf("Could not delete service.\n");
			CloseServiceHandle(service);
			CloseServiceHandle(manager);
			return EXIT_FAILURE;
		}
		CloseServiceHandle(manager);
		fprintf(stderr, "Could not open service.\n");
	}
	return EXIT_FAILURE;
}

/*	----------------------------------------------------------------------	*/

/*
*	Parse the command line
*/
OV_BOOL ov_ntservice_parseargs(
	int		argc,
	char	**argv
) {
	/*
	*	local variables
	*/
	OV_UINT	i;
	/*
	*	parse command line arguments
	*/
	for(i=1; i<argc; i++) {
		/*
		*	set database filename option
		*/
		if(!strcmp(argv[i], "-f") || !strcmp(argv[i], "--file")) {
			i++;
			if(i<argc) {
				filename = argv[i];
			} else {
				return FALSE;
			}
		}
		/*
		*	set server name option
		*/
		else if(!strcmp(argv[i], "-s") || !strcmp(argv[i], "--server-name")) {
			i++;
			if(i<argc) {
				servername = argv[i];
			} else {
				return FALSE;
			}
		}
		/*
		*	set port number option
		*/
		else if(!strcmp(argv[i], "-p") || !strcmp(argv[i], "--port-number")) {
			i++;
			if(i<argc) {
				port = strtol(argv[i], NULL, 0);
			} else {
				return FALSE;
			}
		}
		/*
		*	no startup option
		*/
		else if(!strcmp(argv[i], "-n") || !strcmp(argv[i], "--no-startup")) {
			startup = FALSE;
		}
		/*
		*	display version option
		*/
		else if(!strcmp(argv[i], "-v") || !strcmp(argv[i], "--version")) {
			version = TRUE;
		}
		/*
		*	display help option
		*/
		else if(!strcmp(argv[i], "-h") || !strcmp(argv[i], "--help")) {
			help = TRUE;
		} else {
			return FALSE;
		}
	}
	return TRUE;
}

/*	----------------------------------------------------------------------	*/

/*
*	Add an icon to the task bar
*/
void ov_ntservice_addtaskbaricon(
	HICON		hicon,
	OV_STRING	tip
) {
	/*
	*	local variables
	*/
	NOTIFYICONDATA	icondata;
	/*
	*	initialize icon data structure
	*/
	icondata.cbSize = sizeof(NOTIFYICONDATA);
	icondata.hWnd = hwnd;
	icondata.uID = 0;
	icondata.uFlags = NIF_ICON | NIF_TIP;
	icondata.hIcon = hicon;
	lstrcpyn(icondata.szTip, tip, sizeof(icondata.szTip));
	Shell_NotifyIcon(NIM_ADD, &icondata);
}

/*	----------------------------------------------------------------------	*/

/*
*	Remove an icon from the task bar
*/
void ov_ntservice_deletetaskbaricon(void) {
	/*
	*	local variables
	*/
	NOTIFYICONDATA	icondata;
	/*
	*	initialize icon data structure
	*/
	icondata.cbSize = sizeof(NOTIFYICONDATA);
	icondata.hWnd = hwnd;
	icondata.uID = 0;
	Shell_NotifyIcon(NIM_DELETE, &icondata);
}

/*	----------------------------------------------------------------------	*/

/*
*	Modify an icon in the task bar
*/
void ov_ntservice_modifytaskbaricon(
	HICON		hicon,
	OV_STRING	tip
) {
	/*
	*	local variables
	*/
	NOTIFYICONDATA	icondata;
	/*
	*	initialize icon data structure
	*/
	icondata.cbSize = sizeof(NOTIFYICONDATA);
	icondata.hWnd = hwnd;
	icondata.uID = 0;
	icondata.uFlags = NIF_ICON | NIF_TIP;
	icondata.hIcon = hicon;
	lstrcpyn(icondata.szTip, tip, sizeof(icondata.szTip));
	Shell_NotifyIcon(NIM_MODIFY, &icondata);
}

/*	----------------------------------------------------------------------	*/

/*
*	Create the one and only service server object.
*/
OV_BOOL ov_ntservice_create(
	const OV_STRING	servicename
) {
	/*
	*	make sure the service is not already running
	*/
	if(initialized) {
		ov_logfile_error("There can only be one service");
		return FALSE;
	}
	/*
	*	initialize
	*/
	initialized = TRUE;
	if(servicename) {
		service_name = servicename;
	} else {
		service_name = OV_SERVICE_NAME;
	}
	is_ok = TRUE;
	status_handle = NULL;
	status.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
	status.dwCurrentState = SERVICE_START_PENDING;
	status.dwControlsAccepted = 0;
	status.dwWin32ExitCode = 0;
	status.dwServiceSpecificExitCode = 0;
	status.dwCheckPoint = 0;
	status.dwWaitHint = 0;
	done_event = NULL;
	one_shot_event = NULL;
	work_thread = NULL;
	work_thread_id = 0;
	return is_ok;
}

/*	----------------------------------------------------------------------	*/

/*
*	Delete the one and only service server object.
*/
OV_BOOL ov_ntservice_delete(void) {
	if(!initialized) {
		return FALSE;
	}
	if(done_event) {
		CloseHandle(done_event);
		done_event = NULL;
	}
	if(one_shot_event) {
		CloseHandle(one_shot_event);
		one_shot_event = NULL;
	}
	initialized = FALSE;
	return is_ok;
}

/*	----------------------------------------------------------------------	*/

/*
*	Print message with last error (in hex code) to the logfile
*/
void ov_ntservice_lasterror(
	const OV_STRING	msg
) {
	ov_logfile_error("%s (last error: %08X).", msg, GetLastError());
}

/*	----------------------------------------------------------------------	*/

/*
*	Report the state of the service process to the operating system.
*/

OV_BOOL ov_ntservice_reportservicestatus(
	DWORD	curr_stat,
	DWORD	exit_code,
	DWORD	user_code,
	DWORD	checkpoint,
	DWORD	wait_hint
) {
	status.dwCurrentState = curr_stat;
	if(curr_stat == SERVICE_START_PENDING) {
		status.dwControlsAccepted = 0;
	} else {
		status.dwControlsAccepted = SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_PAUSE_CONTINUE;
	}
	if(user_code == 0) {
		status.dwWin32ExitCode = exit_code;
	} else {
		status.dwWin32ExitCode = ERROR_SERVICE_SPECIFIC_ERROR;
		status.dwServiceSpecificExitCode = user_code;
	}
	status.dwCheckPoint	= checkpoint;
	status.dwWaitHint = wait_hint;
	return SetServiceStatus(status_handle, &status);
}

/*	----------------------------------------------------------------------	*/

/*
*	The service controller routine
*/
void ov_ntservice_servicecontroller(
	DWORD	control_code
) {
	/*
	*	what do we have to do?
	*/
	switch(control_code) {
		case SERVICE_CONTROL_STOP:
			/*
			*	spin down the service
			*/
			switch(status.dwCurrentState) {
				case SERVICE_RUNNING:
					ov_ntservice_reportservicestatus(SERVICE_STOP_PENDING,
						NO_ERROR, 0, 1, OV_TIMEOUT);
					ov_ksserver_downserver();
					return;
				case SERVICE_PAUSED:
					ov_ntservice_reportservicestatus(SERVICE_STOP_PENDING,
						NO_ERROR, 0, 1, OV_TIMEOUT);
					SetEvent(done_event);
					return;
				default:
					break;
			}
			break;

		case SERVICE_CONTROL_PAUSE:
			/*
			*	send the service to sleep
			*/
			if(status.dwCurrentState == SERVICE_RUNNING) {
				ov_ntservice_reportservicestatus(SERVICE_PAUSE_PENDING,
					NO_ERROR, 0, 1, OV_TIMEOUT);
				ov_ksserver_downserver();
				return;
			}
			break;

		case SERVICE_CONTROL_CONTINUE:
			/*
			*	back to work
			*/
			if(status.dwCurrentState == SERVICE_PAUSED) {
				ov_ntservice_reportservicestatus(SERVICE_CONTINUE_PENDING,
					NO_ERROR, 0, 1, OV_TIMEOUT);
				SetEvent(one_shot_event);
				return;
			}
			break;

		default:
			break;
	}
	/*
	*	otherwise report back the current state
	*/
	SetServiceStatus(status_handle, &status);
}

/*	----------------------------------------------------------------------	*/

/*
*	Main loop of the OV server thread
*/
int ov_ntservice_servicerun(void) {
	while(TRUE) {
		/*
		*	run one duty cycle of the server...
		*/
		ov_ntservice_reportservicestatus(status.dwCurrentState,
			NO_ERROR, 0, 2,	OV_TIMEOUT);
		ov_logfile_info("Starting server...");
		ov_ksserver_start();
		ov_logfile_info("Server started.");
		ov_ntservice_reportservicestatus(SERVICE_RUNNING,
			NO_ERROR, 0, 0, 0);
		ov_ntservice_modifytaskbaricon(hicon[0], "The ACPLT/OV server is running");
		ov_ksserver_run();
		ov_ntservice_modifytaskbaricon(hicon[1], "The ACPLT/OV server is stopped");
		ov_ntservice_reportservicestatus(status.dwCurrentState,
			NO_ERROR, 0, 2,	OV_TIMEOUT);
		ov_logfile_info("Stopping server...");
		ov_ksserver_stop();
		ov_logfile_info("Server stopped.");
		/*
		*	what do we have to do now?
		*/
		switch(status.dwCurrentState) {
			case SERVICE_STOP_PENDING:
				ov_ntservice_reportservicestatus(SERVICE_STOP_PENDING,
					NO_ERROR, 0, 3, OV_TIMEOUT);
				SetEvent(done_event);
				break;
			case SERVICE_PAUSE_PENDING:
				ov_ntservice_reportservicestatus(SERVICE_PAUSED,
					NO_ERROR, 0, 0, 0);
				break;
			default:
				/*
				*	the service was stopped successfully
				*/
				ov_ntservice_reportservicestatus(SERVICE_STOP,
					NO_ERROR, 0, 0, 0);
				break;
		}
		/*
		*	wait until we get woken up again...
		*/
		WaitForSingleObject(one_shot_event, INFINITE);
	}
}

/*	----------------------------------------------------------------------	*/

/*
*	Service main routine
*/
void ov_ntservice_servicemain(
	DWORD	argc,
	LPTSTR	*argv
) {
	/*
	*	local variables
	*/
	OV_RESULT	result;
	/*
	*	create signaling objects
	*/
	done_event = CreateEvent(0, FALSE, FALSE, 0);
	one_shot_event = CreateEvent(0, FALSE, FALSE, 0);
	if(!done_event || !one_shot_event) {
		ov_ntservice_lasterror("Can't initialize signaling mechanism");
		return;
	}
	/*
	*	register the service controller and report we are about to start
	*/
	status_handle = RegisterServiceCtrlHandler((LPCTSTR)service_name,
		(LPHANDLER_FUNCTION)ov_ntservice_servicecontroller);
	if(!status_handle) {
		ov_ntservice_lasterror("Can't register service control handler");
		return;
	}
	ov_ntservice_reportservicestatus(SERVICE_START_PENDING,
		NO_ERROR, 0, 1, OV_TIMEOUT);
	/*
	*	parse the command line
	*/
	if(!ov_ntservice_parseargs((int)argc, (char**)argv)) {
		ov_logfile_error("Error: bad argument in command line.");
		ov_ntservice_reportservicestatus(SERVICE_STOPPED,
			ERROR_SERVICE_SPECIFIC_ERROR, OV_ERR_BADPARAM, 0, 0);
		return;
	}
	/*
	*	determine the servername
	*/
	if(!servername) {
		if(username) {
			OV_STRING pc;
			servername = (OV_STRING)malloc(strlen("ov_server_")+strlen(username)+1);
			sprintf(servername, "ov_server_%s", username);
			for(pc=servername; *pc; pc++) {
				*pc = tolower(*pc);
			}
		} else {
			servername = "ov_server";
		}
	}
	/*
	*	map existing database
	*/
	ov_logfile_info("Mapping database \"%s\"...", filename);
	result = ov_database_map(filename);
	if(Ov_Fail(result)) {
ERRORMSG:
		ov_logfile_error("Error: %s (error code 0x%4.4x).",
			ov_result_getresulttext(result), result);
		ov_ntservice_reportservicestatus(SERVICE_STOPPED,
			ERROR_SERVICE_SPECIFIC_ERROR, result, 0, 0);
		return;
	}
	ov_logfile_info("Database mapped.");
	/*
	*	start up the database if appropriate
	*/
	if(startup) {
		ov_logfile_info("Starting up database...");
		result = ov_database_startup();
		if(Ov_Fail(result)) {
			goto ERRORMSG;
		}
		ov_logfile_info("Database started up.");
	}
	/*
	*	create the server object and add task bar icon
	*/
	ov_logfile_info("Creating server object...");
	result = ov_ksserver_create(servername, port, NULL);
	if(Ov_Fail(result)) {
		goto ERRORMSG;
	}
	ov_logfile_info("Server object created.");
	ov_ntservice_addtaskbaricon(hicon[1], "The ACPLT/OV server is stopped");
	/*
	*	we are about to start the server work thread...
	*/
	ov_ntservice_reportservicestatus(SERVICE_START_PENDING, NO_ERROR, 0, 2, OV_TIMEOUT);
	work_thread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ov_ntservice_servicerun,
		0, 0, &work_thread_id);
	if(work_thread <= 0) {
		is_ok = FALSE;
		ov_logfile_error("Can't start service thread");
		return;
	}
	/*
	*	we're up and running
	*/
	ov_ntservice_reportservicestatus(SERVICE_RUNNING, NO_ERROR, 0, 0, 0);
	if(WaitForSingleObject(done_event, INFINITE) != WAIT_OBJECT_0) {
		ov_logfile_warning("\"Service Done\" signaling failed");
	}
	if(!TerminateThread(work_thread, 0)) {
		ov_ntservice_lasterror("Can't terminate service thread");
	}
	/*
	*	shut down the database if appropriate
	*/
	if(startup) {
		ov_logfile_info("Shutting down database...");
		ov_database_shutdown();
		ov_logfile_info("Database shut down.");
	}
	/*
	*	delete the server object and remove the task bar icon
	*/
	ov_ksserver_delete();
	ov_ntservice_deletetaskbaricon();
	/*
	*   unmap the database
	*/
	ov_logfile_info("Unmapping database \"%s\"...", filename);
	ov_database_unmap();
	ov_logfile_info("Database unmapped.");
	/*
	*	close logfile
	*/
	ov_logfile_close();
	/*
	*	simply terminate, it will throw us back to the main thread
	*/
	ov_ntservice_reportservicestatus(SERVICE_STOPPED, status.dwWin32ExitCode,
		status.dwServiceSpecificExitCode, 0, 0);
}

/*	----------------------------------------------------------------------	*/

/*
*	Run the service
*/
void ov_ntservice_run(void) {
	/*
	*	local variables
	*/
	SERVICE_TABLE_ENTRY service_dispatch_table[2];
	/*
	*	initialize
	*/
	service_dispatch_table[0].lpServiceName = (LPTSTR)service_name;
	service_dispatch_table[0].lpServiceProc
		= (LPSERVICE_MAIN_FUNCTION)ov_ntservice_servicemain;
	service_dispatch_table[1].lpServiceName = 0;
	service_dispatch_table[1].lpServiceProc = 0;
	/*
	*	start the service control dispatcher
	*/
	if(!StartServiceCtrlDispatcher(service_dispatch_table)) {
		is_ok = FALSE;
		ov_ntservice_lasterror("Can't start service control dispatcher");
		return;
	}
}

/*	----------------------------------------------------------------------	*/

/*
*	Finally: the main function
*/
int main(int argc, char **argv) {
	/*
	*	local variables
	*/
	HINSTANCE hinst;
	/*
	*	test if "install" or "uninstall" option
	*/
	if(argc > 1) {
		if(!strcmp(argv[1], "install")) {
			return ov_ntservice_install(argc-2, &argv[2]);
		}
		if((argc == 2) && !strcmp(argv[1], "uninstall")) {
			return ov_ntservice_uninstall();
		}
	}
	/*
	*	load icons and set some global vars
	*/
	hinst = GetModuleHandle(NULL);
	hwnd = GetForegroundWindow();
	hicon[0] = LoadIcon(hinst, MAKEINTRESOURCE(1));
	hicon[1] = LoadIcon(hinst, MAKEINTRESOURCE(2));
	/*
	*	parse the command line
	*/
	if(!ov_ntservice_parseargs(argc, argv) || help) {
		fprintf(stderr, "Usage: ov_ntservice [install|uninstall] [arguments]\n"
				"\n"
				"ov_ntservice install [arguments] Install the service\n"
				"ov_ntservice uninstall           Uninstall the service\n"
				"\n"
				"The following optional arguments are available:\n"
				"-f FILE, --file FILE             Set database filename (*.ovd)\n"
				"-s SERVER, --server-name SERVER  Set server name\n"
				"-p PORT, --port-number PORT      Set server port number\n"
				"-n, --no-startup                 Do not startup the database\n"
				"-v, --version                    Display version information\n"
				"-h, --help                       Display this help message\n");
		return EXIT_FAILURE;
	}
	if(version) {
		printf(OV_DISPLAY_NAME " " OV_VER_NTSERVICE "\n");
	}
	/*
	*	set vendor name, server description and server version
	*/
	ov_vendortree_setname("Lehrstuhl fuer Prozessleittechnik, RWTH Aachen");
	ov_vendortree_setserverdescription("generic ACPLT/OV server (NT service)");
	ov_vendortree_setserverversion(OV_VER_SERVER);
	/*
	*	set the user name
	*/
	username = OV_USERNAME;
	/*
	*	log to the NT event logger
	*/
	ov_logfile_logtontlog(NULL);
	/*
	*   run server
	*/
	if(ov_ntservice_create(OV_SERVICE_NAME)) {
		ov_ntservice_run();
		if(!ov_ntservice_delete()) {
			return EXIT_FAILURE;
		}
	} else {
		ov_logfile_error("Failed to setup the service");
	}
	return EXIT_SUCCESS;
}

/*	----------------------------------------------------------------------	*/

/*
*	End of file
*/

