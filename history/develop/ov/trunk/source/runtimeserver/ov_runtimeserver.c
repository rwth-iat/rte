#include <ctype.h>
#include <signal.h>

//#include "libovks/ov_ksserver.h"
#include "libov/ov_database.h"
#include "libov/ov_object.h"
#include "libov/ov_result.h"
#include "libov/ov_logfile.h"
#include "libov/ov_scheduler.h"
#include "libov/ov_macros.h"
#include "libov/ov_malloc.h"
#include "ov_ksserver_stripped.h"
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


/*
*	Global variables: the ACPLT/KS server for ACPLT/OV and the logger object
*/

static OV_BOOL ov_ksserver_stripped_run_server=TRUE;


/*	----------------------------------------------------------------------	*/

/*
*	Signal handler for server shutdown
*/

static void ov_server_sighandler(
	int signal
) {
	ov_logfile_info("Stopping server...");
	ov_ksserver_stripped_sighandler(signal);
}

OV_DLLFNCEXPORT OV_RESULT ov_ksserver_stripped_create(
	OV_STRING			servername,
	OV_FNC_SIGHANDLER	*sighandler
) {
      //ov_logfile_info("ov_ksserver_stripped_create");
		if(!ov_object_identifierok(servername)) {
			return OV_ERR_BADNAME;
		}

     		if(sighandler) {
			signal(SIGTERM, sighandler);
			signal(SIGINT, sighandler);
#if !PLT_SYSTEM_NT
			signal(SIGHUP, sighandler);
#endif
         }
		ov_vendortree_setservername(servername);
		return OV_ERR_OK;
}

OV_DLLFNCEXPORT void ov_ksserver_stripped_delete(void) {
   //ov_logfile_info("ov_ksserver_stripped_delete");
   ov_vendortree_setservername(NULL);
}

OV_DLLFNCEXPORT void ov_ksserver_stripped_start(void) {
   //ov_logfile_info("ov_ksserver_stripped_start");
   ov_vendortree_setstartuptime(NULL);
   ov_ksserver_stripped_run_server=TRUE;
}

OV_DLLFNCEXPORT void ov_ksserver_stripped_run(void) {
    OV_TIME_SPAN  *delay;
    
//ov_logfile_info("ov_ksserver_stripped_run");
	while(ov_ksserver_stripped_run_server==TRUE) {
		 delay = ov_scheduler_schedulenextevent();
		 if( delay->secs > 1 ) {
            delay->secs = 0;
            delay->usecs = 1000;
         }
         
        // ov_logfile_info("ov_ksserver_stripped_run ...sleeping: %ld,%ld", delay->secs, delay->usecs);
        //if( (delay->secs != 0) || (delay->usecs != 0) ) {
            // ov_logfile_info(" !!!! ov_ksserver_stripped_run ...sleeping: %ld,%ld", delay->secs, delay->usecs);
        //}

#if !OV_SYSTEM_NT
	usleep(delay->secs * 1000 + delay->usecs / 1000);
        // Not work on LINUX: select(0,  0,0,0,  &delay);
#else
        if( (delay->secs == 0) && (delay->usecs == 0) ) {
            /* Windows does not sleep iff 0 is param, but linux usleep drops timslot of thread */
            delay->usecs = 1000;
        }
    	Sleep(delay->secs * 1000 + delay->usecs / 1000);
#endif
	}
}

OV_DLLFNCEXPORT void ov_ksserver_stripped_stop(void) {
   //ov_logfile_info("ov_ksserver_stripped_stop");
   ov_ksserver_stripped_run_server=FALSE;
}

OV_DLLFNCEXPORT void ov_ksserver_stripped_sighandler(int signum) {
   ov_logfile_info("ov_ksserver_stripped_sighandler");
   ov_ksserver_stripped_run_server=FALSE;
}
/*
*	End of file
*/



/*	----------------------------------------------------------------------	*/

static void ov_server_usage(void)
{	
            fprintf(stderr, "Usage: ov_server [arguments]\n"
				"\n"
				"The following optional arguments are available:\n"
				"-f FILE, --file FILE             Set database filename (*.ovd)\n"
				"-b FILE, --backup FILE           Set backup database filename (*.ovd)\n"
				"-s SERVER, --server-name SERVER  Set server name\n"
				"-i ID, --identify ID             Set Ticket Identification for server access\n"
				"-p PORT, --port-number PORT      Set server port number\n"
				"-w LIBRARY, --start-with LIBRARY Start server with library\n"
				"-l LOGFILE, --logfile LOGFILE    Set logfile name, you may use stdout"
#if OV_SYSTEM_NT
				", stderr\n"
				"                                 or ntlog (Windows NT only)\n"
#else
				" or stderr\n"
#endif
#if OV_SYSTEM_RMOS
				"-t TASKID, --terminate TASKID    Terminate server (RMOS option only)\n"
#endif
				"-a , --activity-lock             Locks OV activities (scheduler and accessorfnc)\n"
				"-o , --option [OPTION]           appends the option OPTION to the cmdline_options variable in Vendortree\n\t\tOPTION is mandatory"
				"-n, --no-startup                 Do not startup the database\n"
				"-v, --version                    Display version information\n"
				"-x, --exit                       Exit immediately (test if database loads)\n"
				"-h, --help                       Display this help message\n");


}

/*
*	Main program
*/

int main(int argc, char **argv) {
	/*
	*	local variables
	*/

	OV_STRING	        help = NULL;

	OV_STRING	            filename = "database.ovd";
	OV_STRING	            servername = NULL;
	OV_STRING	            password = NULL;
	OV_STRING               libraries[16];
	OV_STRING				commandline_options = NULL;
	OV_STRING				tempstr = NULL;
	OV_INSTPTR_ov_library   plib;
	OV_INSTPTR_ov_domain    pdom;
	OV_INT                  i;
	OV_INT		            libcount;
	OV_RESULT	            result;
	OV_INT 		            port = 0; /* KS_ANYPORT */
	OV_BOOL		            startup = TRUE;
	OV_BOOL		            reuse = FALSE;
	int	   	            exit_status = EXIT_SUCCESS;
	OV_BOOL		        exit = FALSE;

#if OV_SYSTEM_RMOS
	OV_UINT		            taskid;
	OV_BOOL		            terminate = FALSE;
#endif
	
	ov_backup = FALSE;
	db_backup_filename = NULL;
	libcount = 0;
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
				db_backup_filename = argv[i];
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
			ov_activitylock = TRUE;
		}
		/*
		*	handle commandline options
		*/
		else if(!strcmp(argv[i], "-o") || !strcmp(argv[i], "--option")) {
			i++;
			if(i<argc) {
				if(commandline_options)
				{
					tempstr = realloc(commandline_options, strlen(commandline_options)+strlen(argv[i])+2); //oldlength + new option length + ' ' + '\0'
					if(tempstr)
					{
						sprintf(tempstr, "%s %s", tempstr, argv[i]);
						commandline_options = tempstr;
					}
				}
				else	//first time commandline_options is a NULL-pointer --> strlen would crash
				{
					commandline_options = malloc(strlen(argv[i])+1);
					if(commandline_options)
						strcpy(commandline_options, argv[i]);
				}
			} else {
				goto HELP;
			}
		}
		/*
		*	start with library
		*/
		else if(!strcmp(argv[i], "-w") || !strcmp(argv[i], "--start-with")) {
			i++;
			if (libcount<16) {
			        libraries[libcount] = argv[i];
           			libcount++;
                        }
                        else 	ov_logfile_error("Too many libraries in start command.\n");
		}
		/*
		*	exit immideately
		*/
		else if(!strcmp(argv[i], "-x") || !strcmp(argv[i], "--exit")) {
			i++;
			exit = TRUE;
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
HELP:	   ov_server_usage();			
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
	ov_vendortree_setservername(servername);	//02.04.06 add by Senad
	/*
	*	map existing database
	*/
	ov_logfile_info("Mapping database \"%s\"...", filename);
	/* Adding database path prefix */
	CONCATENATE_DATABASE_PATH(filename, help);
#ifdef OV_CATCH_EXCEPTIONS
	result = ov_supervised_database_map(filename);
#else
	result = ov_database_map(filename);
#endif
	if (Ov_Fail(result) && db_backup_filename) {
MAPBACKUP:
		ov_backup = TRUE;
		ov_logfile_error("Error: %s (error code 0x%4.4x).",
		ov_result_getresulttext(result), result);
		ov_logfile_info("Mapping backup-database \"%s\"...", db_backup_filename);
		/* Adding database path prefix */
		CONCATENATE_DATABASE_PATH(db_backup_filename, help);
#ifdef OV_CATCH_EXCEPTIONS
		result = ov_supervised_database_map(db_backup_filename);
#else
		result = ov_database_map(db_backup_filename);
#endif
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
#ifdef OV_CATCH_EXCEPTIONS
		result = ov_supervised_database_startup();
#else
		result = ov_database_startup();
#endif
		if(Ov_Fail(result)) {
			if ((!ov_backup) && (db_backup_filename)) goto MAPBACKUP;
			goto ERRORMSG;
		}
		ov_logfile_info("Database started up.");
	}
	/*
	*	create startup libraries
	*/
        i = 0;
	while (i < libcount) {
	      pdom = Ov_DynamicPtrCast(ov_domain, Ov_SearchChild(ov_containment, &pdb->root, "acplt"));
              result = Ov_CreateObject(ov_library, plib, pdom, libraries[i]);
	      if (result == OV_ERR_ALREADYEXISTS) {
      		        ov_logfile_info("library %s exists.",libraries[i]);
	      }
              else if(Ov_Fail(result)) {
       		        ov_logfile_error("Could'nt create library %s: %s (error code 0x%4.4x).",
			        libraries[i], ov_result_getresulttext(result), result);
	      }
              i++;
	}
	/*
	*	set the serverpassword of the database
	*/
	if (!pdb->serverpassword) ov_vendortree_setserverpassword(password);

	/*
	 * set the commandline options
	 * if port is set put it in front as PORT
	 */
	if(port)
	{
		if(commandline_options)
		{
			tempstr = realloc(commandline_options, strlen(commandline_options)+16); //"PORT=" + max characters in INT + '\0'
			if(tempstr)
			{
				sprintf(tempstr, "PORT=%d %s", port, tempstr);
				commandline_options = tempstr;
			}
		}
		else
		{
			commandline_options = malloc(11);
			if(commandline_options)
			{
				sprintf(commandline_options, "PORT=%d", port);
			}
		}
	}
	if(commandline_options)
		ov_vendortree_setcmdlineoptions(commandline_options);

	if(!exit){
		/*
		*   run server
		*/
		ov_logfile_info("Starting server...");
		result = ov_ksserver_stripped_create(servername, ov_server_sighandler);
		if(Ov_OK(result)) {
			ov_logfile_info("Server started.");
			ov_logfile_info("Servername: %s.",servername);
			ov_ksserver_stripped_start();

			ov_ksserver_stripped_run();

			ov_ksserver_stripped_stop();
			ov_logfile_info("Server stopped.");
			if (Ov_Fail(result) && (!ov_backup) && db_backup_filename) {
				ov_ksserver_stripped_delete();
				goto MAPBACKUP;
			}
		} else {
			ov_logfile_error("Error: %s (error code 0x%4.4x).",
				ov_result_getresulttext(result), result);
			exit_status = EXIT_FAILURE;
		};

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
	*	delete the server object
	*/

	if(exit_status == EXIT_SUCCESS) {
		ov_ksserver_stripped_delete();
	}

	/*
	*   unmap the database
	*/

	ov_logfile_info("Unmapping database \"%s\"...", filename);
	ov_database_unmap();
	ov_logfile_info("Database unmapped.");

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



