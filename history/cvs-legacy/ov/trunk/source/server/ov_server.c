/*
*   $Id: ov_server.c,v 1.11 2002-06-18 10:15:58 ansgar Exp $
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
*	23-Jul-1998 Dirk Meyer <dirk@plt.rwth-aachen.de>: File created.
*	21-Apr-1999 Dirk Meyer <dirk@plt.rwth-aachen.de>: Major revision.
*/

#include <ctype.h>

#include "libovks/ov_ksserver.h"
#include "libov/ov_database.h"
#include "libov/ov_result.h"
#include "libov/ov_logfile.h"

/*	----------------------------------------------------------------------	*/

/*
*	Constants
*/
#if OV_SYSTEM_UNIX || OV_SYSTEM_OPENVMS
#define OV_USERNAME getenv("USER")
#elif OV_SYSTEM_NT
#define OV_USERNAME getenv("USERNAME")
#else
#define OV_USERNAME NULL
#endif

/*	----------------------------------------------------------------------	*/

/*
*	Signal handler for server shutdown
*/
static void ov_server_sighandler(
	int signal
) {
	ov_logfile_info("Stopping server...");
	ov_ksserver_sighandler(signal);
}

/*	----------------------------------------------------------------------	*/

/*
*	A ticket demonstration using simple tickets
*	-------------------------------------------
*/

#ifdef OV_TICKET_DEMO
/*
*	access rights granted by the demo ticket
*/
static OV_ACCESS demoticket_access;
/*
*	function prototypes for ticket methods
*/
static OV_TICKET *demoticket_createticket(XDR *xdr, OV_TICKET_TYPE type);
static void demoticket_deleteticket(OV_TICKET *pticket);
static OV_BOOL demoticket_encodereply(XDR *xdr, OV_TICKET *pticket);
static OV_ACCESS demoticket_getaccess(const OV_TICKET *pticket);

/*
*	VTable of our ticket object
*/
static OV_TICKET_VTBL demoticketvtbl = {
	demoticket_createticket,
	demoticket_deleteticket,
	demoticket_encodereply,
	demoticket_getaccess
};

/*
*	Create a ticket object from the XDR stream
*/
static OV_TICKET *demoticket_createticket(XDR *xdr, OV_TICKET_TYPE type) {
	/*
	*	local variables: we use a static object
	*/
	static OV_TICKET ticket = { &demoticketvtbl, OV_TT_SIMPLE };
	/*
	*	make sure we get a simple ticket
	*/
	if(type != OV_TT_SIMPLE) {
		return NULL;
	}
	/*
	*	decode the simple ticket id, a string, from the XDR stream
	*/
	if(!xdr_string(xdr, &ticket.ticketunion.simpleticket.id, KS_SIMPLEID_MAXLEN)) {
		return NULL;
	}
	/*
	*	only grant read access to anyone who does not have the "supervisor" ticket
	*/
	if(strcmp(ticket.ticketunion.simpleticket.id, "supervisor")) {
		demoticket_access = OV_AC_READ;
	} else {
		demoticket_access = OV_AC_READWRITE | OV_AC_INSTANTIABLE
			| OV_AC_DELETEABLE | OV_AC_RENAMEABLE | OV_AC_LINKABLE
			| OV_AC_UNLINKABLE; 
	}
	return &ticket;
}

/*
*	Delete a ticket object, i.e. its id in this case
*/
static void demoticket_deleteticket(OV_TICKET *pticket) {
	/*
	*	local variables
	*/
	static XDR xdr;
	/*
	*	just let XDR free the memory it allocated during construction of the id
	*/
	xdr.x_op = XDR_FREE;
	xdr_string(&xdr, &pticket->ticketunion.simpleticket.id, KS_SIMPLEID_MAXLEN);
}

/*
*	we do not encode further ticket information in the reply in a simple ticket
*/
static OV_BOOL demoticket_encodereply(XDR *xdr, OV_TICKET *pticket) {
	return TRUE;
}

/*
*	determine which access rights the ticket holder gets at most
*/
static OV_ACCESS demoticket_getaccess(const OV_TICKET *pticket) {
	return demoticket_access;
}

#endif

/*	----------------------------------------------------------------------	*/

/*
*	Main program
*/
int main(int argc, char **argv) {
	/*
	*	local variables
	*/
	OV_STRING	filename = "database.ovd";
	OV_STRING	backupfilename = NULL;
	OV_STRING	servername = NULL;
	OV_STRING	password = NULL;
	OV_UINT		i;
	OV_RESULT	result;
	OV_INT 		port = 0; /* KS_ANYPORT */
	OV_BOOL		startup = TRUE;
	OV_BOOL		reuse = FALSE;
	int			exit_status = EXIT_SUCCESS;
#if OV_SYSTEM_RMOS
	OV_UINT		taskid;
	OV_BOOL		terminate = FALSE;
#endif

	backup = FALSE;
	/*
	*	if we are debugging, log to stderr (if not 
	*	specified by the command line options)
	*/
#ifdef OV_DEBUG
	ov_logfile_logtostderr(NULL);
#endif
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
				goto HELP;
			}
		}
		/*
		*	set database backup filename option
		*/
		else if(!strcmp(argv[i], "-b") || !strcmp(argv[i], "--backup")) {
			i++;
			if(i<argc) {
				backupfilename = argv[i];
			} else {
				goto HELP;
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
				goto HELP;
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
				goto HELP;
			}
		}
		/*
		*	set identification
		*/
		else if(!strcmp(argv[i], "-i") || !strcmp(argv[i], "--identifiy")) {
			i++;
			if(i<argc) {
				password = argv[i];
			} else {
				goto HELP;
			}
		}
		/*
		*	set activity lock
		*/
		else if(!strcmp(argv[i], "-a") || !strcmp(argv[i], "--activity-lock")) {
			i++;
			activitylock = TRUE;
		}
		/*
		*	set reuse of socket address/port
		*/
		else if(!strcmp(argv[i], "-r") || !strcmp(argv[i], "--reuse-address")) {
			i++;
			reuse = TRUE;
		}
		/*
		*	set logfile option
		*/
		else if(!strcmp(argv[i], "-l") || !strcmp(argv[i], "--logfile")) {
			i++;
			if(i<argc) {
				if(!strcmp(argv[i], "stdout")) {
					ov_logfile_logtostdout(NULL);
				} else if(!strcmp(argv[i], "stderr")) {
					ov_logfile_logtostderr(NULL);
#if OV_SYSTEM_NT
				} else if(!strcmp(argv[i], "ntlog")) {
					ov_logfile_logtontlog(NULL);
#endif
				} else {
					if(Ov_Fail(ov_logfile_open(NULL, argv[i], "w"))) {
						ov_logfile_error("Could not open log file: \"%s\".\n", argv[i]);
						return EXIT_FAILURE;
					}
				}
			} else {
				goto HELP;
			}
		}
#if OV_SYSTEM_RMOS
		/*
		*	terminate server option
		*/
		else if(!strcmp(argv[i], "-t") || !strcmp(argv[i], "--terminate")) {
			i++;
			if(i<argc) {
				taskid = strtol(argv[i], NULL, 0);
				terminate = TRUE;
			} else {
				goto HELP;
			}
		}
#endif
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
			printf("ACPLT/KS Server for ACPLT/OV " OV_VER_SERVER "\n");
			return EXIT_FAILURE;
		}
		/*
		*	display help option
		*/
		else if(!strcmp(argv[i], "-h") || !strcmp(argv[i], "--help")) {
HELP:		fprintf(stderr, "Usage: ov_server [arguments]\n"
				"\n"
				"The following optional arguments are available:\n"
				"-f FILE, --file FILE            Set database filename (*.ovd)\n"
				"-b FILE, --backup FILE          Set backup database filename (*.ovd)\n"
				"-s SERVER, --server-name SERVER Set server name\n"
				"-i ID, --identify ID            Set Ticket Identification for server access\n"
				"-p PORT, --port-number PORT     Set server port number\n"
				"-l LOGFILE, --logfile LOGFILE   Set logfile name, you may use stdout"
#if OV_SYSTEM_NT
				", stderr\n"
				"                                or ntlog (Windows NT only)\n"
#else
				" or stderr\n"
#endif
#if OV_SYSTEM_RMOS
				"-t TASKID, --terminate TASKID   Terminate server (RMOS option only)\n"
#endif
				"-a , --activity-lock            Locks OV activities (scheduler and accessorfnc)\n"
				"-r , --reuse-address            Reuses the socket address/port\n"
				"-n, --no-startup                Do not startup the database\n"
				"-v, --version                   Display version information\n"
				"-h, --help                      Display this help message\n");
			return EXIT_FAILURE;
		} else {
			goto HELP;
		}
	}
	/*
	*	terminate server if appropriate (RMOS only)
	*/
#if OV_SYSTEM_RMOS
	if(terminate) {
		ov_logfile_info("Sending terminate message...");
		result = ov_ksserver_terminate(taskid);
		if(Ov_Fail(result)) {
			goto ERRORMSG;
		}
		ov_logfile_info("Message sent successfully.");
		return EXIT_SUCCESS;
	}
#endif
	/*
	*	determine the servername
	*/
	if(!servername) {
		if(OV_USERNAME) {
			OV_STRING pc;
			servername = (OV_STRING)malloc(strlen("ov_server_")+strlen(OV_USERNAME)+1);
			sprintf(servername, "ov_server_%s", OV_USERNAME);
			for(pc=servername; *pc; pc++) {
				*pc = tolower(*pc);
			}
		} else {
			servername = "ov_server";
		}
	}
#ifdef OV_TICKET_DEMO
	/*
	*	register our demo ticket
	*/
	ov_ksserver_ticket_setdefaultaccess(OV_AC_READ);
	Ov_WarnIfNot(Ov_OK(ov_ksserver_ticket_register(OV_TT_SIMPLE, &demoticketvtbl)));
#endif
	/*
	*	set vendor name, server description and server version
	*/
	ov_vendortree_setname("Lehrstuhl fuer Prozessleittechnik, RWTH Aachen");
	ov_vendortree_setserverdescription("generic ACPLT/OV server");
	ov_vendortree_setserverversion(OV_VER_SERVER);
	/*
	*	map existing database
	*/
	ov_logfile_info("Mapping database \"%s\"...", filename);
	result = ov_supervised_database_map(filename);
	if (Ov_Fail(result) && backupfilename) {
MAPBACKUP:
		backup = TRUE;
		ov_logfile_error("Error: %s (error code 0x%4.4x).",
			ov_result_getresulttext(result), result);
		ov_logfile_info("Mapping backup-database \"%s\"...", backupfilename);
		result = ov_supervised_database_map(backupfilename);
	}
	if(Ov_Fail(result)) {
ERRORMSG:
		ov_logfile_error("Error: %s (error code 0x%4.4x).",
			ov_result_getresulttext(result), result);
		return EXIT_FAILURE;
	}
	ov_logfile_info("Database mapped.");
	/*
	*	start up the database if appropriate
	*/
	if(startup) {
		ov_logfile_info("Starting up database...");
		result = ov_supervised_database_startup();
		if(Ov_Fail(result)) {
			if ((!backup) && (backupfilename)) goto MAPBACKUP;
			goto ERRORMSG;
		}
		ov_logfile_info("Database started up.");
	}
	/*
	*	set the serverpassword of the database
	*/
	if (!pdb->serverpassword) ov_vendortree_setserverpassword(password);
	/*
	*   run server
	*/
	ov_logfile_info("Starting server...");
	result = ov_ksserver_create(servername, port, ov_server_sighandler, reuse);
	if(Ov_OK(result)) {
		ov_logfile_info("Server started.");
		ov_ksserver_start();
		result = ov_supervised_server_run();
		ov_ksserver_stop();
		ov_logfile_info("Server stopped.");
		if (Ov_Fail(result) && (!backup) && backupfilename) {
			ov_ksserver_delete();
			goto MAPBACKUP;
		}
	} else {
		ov_logfile_error("Error: %s (error code 0x%4.4x).",
			ov_result_getresulttext(result), result);
		exit_status = EXIT_FAILURE;
	};
	/*
	*	shut down the database if appropriate
	*/
	if(startup) {
		ov_logfile_info("Shutting down database...");
		ov_database_shutdown();
		ov_logfile_info("Database shut down.");
	}
	/*
	*	delete the server object
	*/
	if(exit_status == EXIT_SUCCESS) {
		ov_ksserver_delete();
	}
	/*
	*   unmap the database
	*/
	ov_logfile_info("Unmapping database \"%s\"...", filename);
	ov_database_unmap();
	ov_logfile_info("Database unmapped.");
#ifdef OV_TICKET_DEMO
	/*
	*	unregister our demo ticket
	*/
	Ov_WarnIfNot(Ov_OK(ov_ksserver_ticket_unregister(OV_TT_SIMPLE)));
#endif
	/*
	*	close the logfile and return
	*/
	ov_logfile_close();
	return exit_status;
}

/*	----------------------------------------------------------------------	*/

/*
*	End of file
*/

