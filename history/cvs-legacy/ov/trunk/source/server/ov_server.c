/*
*   $Id: ov_server.c,v 1.1 1999-07-19 15:02:17 dirk Exp $
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

#include <stdio.h>

#include "libovks/ov_ksserver.h"
#include "libov/ov_database.h"
#include "libov/ov_result.h"
#include "libov/ov_logfile.h"

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
*	Main program
*/
int main(int argc, char **argv) {
	/*
	*	local variables
	*/
	OV_STRING	filename = "database.ovd";
	OV_STRING	servername = "ov_server";
	OV_UINT		i;
	OV_RESULT	result;
	OV_INT 		port = 0; /* KS_ANYPORT */
	OV_BOOL		startup = TRUE;
	int			exit_status = EXIT_SUCCESS;
#if OV_SYSTEM_RMOS
	OV_UINT		taskid;
	OV_BOOL		terminate = FALSE;
#endif
	/*
	*	if we are debugging, log to stderr (if not 
	*	specified by the command line options)
	*/
#ifdef OV_DEBUG
	ov_logfile_logtostderr();
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
		*	set logfile option
		*/
		else if(!strcmp(argv[i], "-l") || !strcmp(argv[i], "--logfile")) {
			i++;
			if(i<argc) {
				if(!strcmp(argv[i], "stdout")) {
					ov_logfile_logtostdout();
				} else if(!strcmp(argv[i], "stderr")) {
					ov_logfile_logtostderr();
#if OV_SYSTEM_NT
				} else if(!strcmp(argv[i], "ntlog")) {
					ov_logfile_logtontlog(NULL);
#endif
				} else {
					if(Ov_Fail(ov_logfile_open(argv[i], "w"))) {
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
				"-s SERVER, --server-name SERVER Set server name\n"
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
	*	set vendor name, server description and server version
	*/
	ov_vendortree_setname("Chair of Process Control Engineering, "
		"Aachen University of Technology");
	ov_vendortree_setserverdescription
		("Server of the generic object management system ACPLT/OV");
	ov_vendortree_setserverversion(OV_VER_SERVER);
	/*
	*	map existing database
	*/
	ov_logfile_info("Mapping database \"%s\"...", filename);
	result = ov_database_map(filename);
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
		result = ov_database_startup();
		if(Ov_Fail(result)) {
			goto ERRORMSG;
		}
		ov_logfile_info("Database started up.");
	}
	/*
	*   run server
	*/
	ov_logfile_info("Starting server...");
	result = ov_ksserver_start(servername, port, ov_server_sighandler);
	if(Ov_OK(result)) {
		ov_logfile_info("Server started.");
		ov_ksserver_run();
		ov_logfile_info("Server stopped.");
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
		ov_ksserver_stop();
	}
	/*
	*   unmap the database
	*/
	ov_logfile_info("Unmapping database \"%s\"...", filename);
	ov_database_unmap();
	ov_logfile_info("Database unmapped.");
	/*
	*	close logfile and return
	*/
	ov_logfile_close();
	return exit_status;
}

/*	----------------------------------------------------------------------	*/

/*
*	End of file
*/

