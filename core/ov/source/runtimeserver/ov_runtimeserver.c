#if !OV_SYSTEM_NT
#define _POSIX_C_SOURCE	 199309L
#include <time.h>	//for timespec
#else
#include <windows.h>	//for Sleep
#endif

#include <ctype.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include "libov/ov_ov.h"
#include "libov/ov_database.h"
#include "libov/ov_object.h"
#include "libov/ov_result.h"
#include "libov/ov_logfile.h"
#include "libov/ov_scheduler.h"
#include "libov/ov_macros.h"
#include "libov/ov_malloc.h"
#include "libov/ov_options.h"

/*	----------------------------------------------------------------------	*/
/*
*	Constants
*/
OV_DLLVARIMPORT OV_BOOL ov_server_run;
/*	set this to 0 to remove the help output (reduces size for small systems)	*/
#define OV_SERVER_HELP 1

/*
*	OV_FNC_SIGHANDLER:
*	------------------
*	Signal hander function prototype for server shutdown
*/
typedef void OV_FNC_SIGHANDLER(
	int signal
);

/*
*	Flags for installing the signal handler
*/
#define OV_SIGHANDLER_DEFAULT	((OV_FNC_SIGHANDLER *)0)
#define OV_SIGHANDLER_NONE		((OV_FNC_SIGHANDLER *)-1)

#ifdef OV_CATCH_EXCEPTIONS
OV_DLLFNCEXPORT OV_RESULT ov_supervised_database_startup(void);

OV_DLLFNCEXPORT OV_RESULT ov_supervised_database_map(
	OV_STRING 	dbname
);
#endif

/*
*	Signal handler for server shutdown
*/

OV_DLLFNCEXPORT void ov_server_sighandler(
	int signal
) {
	ov_logfile_info("Received signal. Shutting down server...");
	ov_server_run=FALSE;
}

static void ov_ksserver_stripped_delete(void) {
	//ov_logfile_info("ov_ksserver_stripped_delete");
	ov_vendortree_setservername(NULL);
}


static OV_RESULT ov_ksserver_stripped_create(
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


static void ov_ksserver_stripped_run(void) {
	OV_TIME_SPAN  *delay;
#if !OV_SYSTEM_NT
	struct timespec sleepTime;
#endif
	//ov_logfile_info("ov_ksserver_stripped_run");
	while(ov_server_run==TRUE) {
		delay = ov_scheduler_schedulenextevent();
		if( delay->secs > 1 ) {
			delay->secs = 1;
			delay->usecs = 0;
		}

//		if( (delay->secs != 0) || (delay->usecs != 0) ) {
//			ov_logfile_info(" !!!! ov_ksserver_stripped_run ...sleeping: %ld,%06ld", delay->secs, delay->usecs);
//		}

#if !OV_SYSTEM_NT
		sleepTime.tv_sec = delay->secs;
		sleepTime.tv_nsec = delay->usecs*1000;
		nanosleep(&sleepTime, NULL);
#else
		if( (delay->secs == 0) || (delay->usecs/1000 == 0) ) {
			/* Windows does not sleep if 0 is param, but linux usleep drops time slot of thread */
			delay->usecs = 1000;
		}
		Sleep(delay->secs * 1000 + delay->usecs / 1000);
#endif
	}
}

/*	----------------------------------------------------------------------	*/

static void ov_server_usage(void)
{	
#if OV_SERVER_HELP
		fprintf( stderr,
				"Usage: ov_runtimeserver (-f DATABASE -s SERVERNAME):(-c CONFIGFILE) [arguments]\n"
				"\n"
				"Database file and servername must be set. This can be done either with\n"
				"\tthe -f and -s options or inside a config file (-c option)\n"
				"\tAbsolute paths are recommended. Relative paths may leed to\n"
				"\tunwanted results since the base path is not specified.\n"
				"\n"
				"The following optional arguments are available:\n"
				"-f FILE, --file FILE             Set database filename (*.ovd)\n"
				"-b FILE, --backup FILE           Set backup database filename (*.ovd)\n"
				"-s SERVER, --server-name SERVER  Set server name\n"
				"-cf CONFIGFILE, --config CONFIGFILE\n"
				"                                 Set filename to read configuration from\n"
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
				"-a, --activity-lock              Locks OV activities (scheduler and accessorfnc)\n"
				"-o OPTION, --option OPTION       Appends the option OPTION to the \n"
				"                                 cmdline_options variable in Vendortree.\n"
				"                                 OPTION is mandatory\n"
				"-n, --no-startup                 Do not startup the database\n"
				"-v, --version                    Display version information\n"
				"-x, --exit                       Exit immediately (test if database loads)\n"
				"--exec IDENTIFIER CLASS LIBRARY  Executes the first event in the schedulers\n"
				"                                 queue that matches concerning IDENTIFIER,\n"
				"                                 CLASS and LIBRARY before starting the server."
				"                                 All parameters are mandatory. Use '/' as wildcard.\n"
				"--no-map                         Do not map database.\n"
				"--force-create                   Force to create a new database\n"
				"-h, --help                       Display this help message\n");
#else
		fprintf(stderr, "No help output implemented for this server. Sorry.");
#endif

}

static void ov_server_version(void){
	perror("ACPLT/KS Server for ACPLT/OV " OV_VER_SERVER "\n");
}

/*
*	Main program
*/

int main(int argc, char **argv) {
	/*
	 *	local variables
	 */

	OV_STRING				helper = NULL;						/*	malloced and freed locally or copied to other variable	*/
	OV_UINT					hlpindex = 0;
	OV_INSTPTR_ov_library	plib;
	OV_INSTPTR_ov_domain	pdom;
	OV_INT					i;
	OV_RESULT				result;
	OV_ANY					tempAny = {{OV_VT_VOID, {0}}, 0, {0,0}};
	int						exit_status = EXIT_SUCCESS;

	ov_options	opts;

	ov_options_init(&opts);
	opts.usage = &ov_server_usage;
	opts.version = &ov_server_version;
	opts.ctx = ov_runtimeserver;

	ov_backup = FALSE;
	db_backup_filename = NULL;
	/*
	 *	log to stderr until specified otherwise
	 */
	ov_logfile_logtostderr(NULL);

	/*
	 *	parse command line arguments
	 */

	if(ov_readConfigFromArgs(&opts, argc, argv)!=EXIT_SUCCESS){
		ov_logfile_free();
		ov_options_free(&opts);
		return EXIT_FAILURE;
	}

	if(ov_readArgs(&opts, argc, argv)!=EXIT_SUCCESS){
		ov_logfile_free();
		ov_options_free(&opts);
		return EXIT_FAILURE;
	}

	if(opts.dbBackupFilename)
		db_backup_filename = opts.dbBackupFilename;

	/**********************************************************************************************************************************************************
	 * End of commandline / logfile parsing
	 **********************************************************************************************************************************************************/

	/*
	 *	terminate server if appropriate (RMOS only)
	 */
#if OV_SYSTEM_RMOS
	if(opts.terminate) {
		ov_logfile_info("Sending terminate message...");
		result = ov_ksserver_terminate(taskid);
		if(Ov_Fail(result)) {
			goto ERRORMSG;
		}
		ov_logfile_info("Message sent successfully.");
		ov_logfile_free();
		ov_options_free(&opts);
		return EXIT_SUCCESS;
	}
#endif
	/*
	 *
	 */
	if(!opts.poolsize){
		ov_logfile_error("No heap size specified");
		ov_logfile_free();
		ov_options_free(&opts);
		return EXIT_FAILURE;
	}
	ov_initHeap(opts.poolsize);

	/*
	 *	check servername
	 */
	if(!opts.servername) {
		ov_logfile_error("No server name set. Aborting.");
		ov_server_usage();
		ov_logfile_free();
		ov_options_free(&opts);
		ov_destroyHeap();
		return EXIT_FAILURE;
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
	ov_vendortree_setservername(opts.servername);	//02.04.06 add by Senad

	/*
	 * set allowedJitter and ks-parameters
	 */
	tempAny.value.vartype = OV_VT_UINT;
	if(opts.maxAllowedJitter){
		tempAny.value.valueunion.val_uint = opts.maxAllowedJitter;
		ov_vendortree_setAllowedJitter(&tempAny, NULL);
	}
	if(opts.maxStringLength){
		tempAny.value.valueunion.val_uint = opts.maxStringLength;
		ov_vendortree_setMaxStringLength(&tempAny, NULL);
	}
	if(opts.maxVectorLength){
		tempAny.value.valueunion.val_uint = opts.maxVectorLength;
		ov_vendortree_setMaxVectorLength(&tempAny, NULL);
	}
	if(opts.maxNameLength){
		tempAny.value.valueunion.val_uint = opts.maxNameLength;
		ov_vendortree_setMaxNameLength(&tempAny, NULL);
	}
	if(opts.maxHierarchyDepth){
		tempAny.value.valueunion.val_uint = opts.maxHierarchyDepth;
		ov_vendortree_setMaxHierarchyDepth(&tempAny, NULL);
	}
	if(opts.ks_maxItemsPerRequest){
		tempAny.value.valueunion.val_uint = opts.ks_maxItemsPerRequest;
		ov_vendortree_setKsMaxItems(&tempAny, NULL);
	}
	tempAny.value.vartype = OV_VT_VOID;
	tempAny.value.valueunion.val_uint = 0;

	/*
	 *	map existing database
	 *	abort if no file set
	 */
	if(!opts.dbFilename || !(*opts.dbFilename))
	{
		ov_logfile_error("No database file set. Aborting.");
		ov_server_usage();
		ov_logfile_free();
		ov_vendortree_free();
		ov_options_free(&opts);
		ov_destroyHeap();
		return EXIT_FAILURE;
	}
	/*
	 * check if filename is a relative path. if so put basePath in front of it
	 */
#if OV_SYSTEM_UNIX
	if(!(opts.dbFilename[0]=='/'))
#else
		if(!(opts.dbFilename[1]==':'|| opts.dbFilename[0]=='\\'))
#endif
		{/*	relative path --> prepend basePath	*/
			if(opts.configBasePath && *opts.configBasePath && strcmp(opts.dbFilename, "-")!=0)
			{
				hlpindex = strlen(opts.configBasePath);
				helper = calloc(hlpindex+strlen(opts.dbFilename)+2, sizeof(char));
				if(!helper)
				{
					ov_logfile_error("Could not reserve memory for filename path. Aborting.");
					ov_logfile_free();
					ov_vendortree_free();
					ov_options_free(&opts);
					return EXIT_FAILURE;
				}
				strcpy(helper, opts.configBasePath);
				if(!(helper[hlpindex-1]=='\\' || helper[hlpindex-1]=='/'))
				{
#if OV_SYSTEM_NT
					helper[hlpindex] = '\\';
#else
					helper[hlpindex] = '/';
#endif
					hlpindex++;
				}
				strcpy((helper+hlpindex), opts.dbFilename);
				free(opts.dbFilename);
				opts.dbFilename = helper;
			}
		}

#ifdef OV_CATCH_EXCEPTIONS
	result = ov_supervised_database_map(opts.dbFilename);
#else
	result = ov_database_load(opts.dbFilename, opts.dbSize, opts.dbflags);
#endif
	if (Ov_Fail(result) && db_backup_filename) {
		MAPBACKUP:
		ov_backup = TRUE;
		ov_logfile_error("Error: %s (error code 0x%4.4x).",
				ov_result_getresulttext(result), result);

#ifdef OV_CATCH_EXCEPTIONS
		result = ov_supervised_database_map(db_backup_filename);
#else
		result = ov_database_load(db_backup_filename, opts.dbSize, opts.dbflags|OV_DBOPT_BACKUP);
#endif
	}
	if(Ov_Fail(result)) {
		ERRORMSG:
		ov_logfile_error("Error: %s (error code 0x%4.4x).",
				ov_result_getresulttext(result), result);
		ov_logfile_free();
		ov_vendortree_free();
		ov_options_free(&opts);
		ov_destroyHeap();
		return EXIT_FAILURE;
	}

	/*
	 * set the commandline options
	 * if port is set put it in front as PORT
	 */
	if(opts.port)
	{
		if(opts.commandline_options)
		{
			helper = malloc(strlen(opts.commandline_options)+16); //"PORT=" + max characters in INT + '\0'
			if(helper)
			{
				sprintf(helper, "PORT=%" OV_PRINT_UINT " %s", opts.port, opts.commandline_options);
				free(opts.commandline_options);
				opts.commandline_options = helper;
			}
		}
		else
		{
			opts.commandline_options = malloc(16);
			if(opts.commandline_options)
			{
				sprintf(opts.commandline_options, "PORT=%" OV_PRINT_UINT, opts.port);
			}
		}
	}
	/*	set config basPath	*/
	if(opts.configBasePath && *opts.configBasePath){
		if(opts.commandline_options)
		{
			helper = malloc(8 + strlen(opts.configBasePath) + 1 + strlen(opts.commandline_options) + 1); // "CONFDIR=" + ... + " " + ... + '\0'
			if(helper)
			{
				sprintf(helper, "CONFDIR=%s %s", opts.configBasePath, opts.commandline_options);
				free(opts.commandline_options);
				opts.commandline_options = helper;
			}
		}
		else
		{
			opts.commandline_options = malloc(8 + strlen(opts.configBasePath) + 1); // "CONFDIR=" + ... + '\0'
			if(opts.commandline_options)
			{
				sprintf(opts.commandline_options, "CONFDIR=%s", opts.configBasePath);
			}
		}
	}else{
		if(opts.commandline_options)
		{
			helper = malloc(8 + 3 + strlen(opts.commandline_options) + 1); // "CONFDIR=" + "./ " + ... + '\0'
			if(helper)
			{
#if OV_SYSTEM_UNIX
				sprintf(helper, "CONFDIR=./ %s", opts.commandline_options);
#else
				sprintf(helper, "CONFDIR=.\\ %s", opts.commandline_options);
#endif

				free(opts.commandline_options);
				opts.commandline_options = helper;
			}
		}
		else
		{
			opts.commandline_options = malloc(8 + 2 + 1); // "CONFDIR=" + "./" + '\0'
			if(opts.commandline_options)
			{
#if OV_SYSTEM_UNIX
				sprintf(opts.commandline_options, "CONFDIR=./");
#else
				sprintf(opts.commandline_options, "CONFDIR=.\\");
#endif
			}
		}
	}
	ov_vendortree_setcmdlineoptions(opts.commandline_options);


	/*
	 *	start up the database if appropriate
	 */
	if(opts.startup) {
#ifdef OV_CATCH_EXCEPTIONS
		result = ov_supervised_database_startup();
#else
		result = ov_database_startup();
#endif
		if(Ov_Fail(result)) {
			if ((!ov_backup) && (db_backup_filename)) goto MAPBACKUP;
			goto ERRORMSG;
		}
	}
	/*
	 *	create startup libraries
	 */
	i = 0;
	while (i < opts.libcount) {
		pdom = Ov_DynamicPtrCast(ov_domain, Ov_SearchChild(ov_containment, &pdb->root, "acplt"));
		result = Ov_CreateObject(ov_library, plib, pdom, opts.libraries[i]);
		if (result == OV_ERR_ALREADYEXISTS) {
			ov_logfile_info("library %s exists.",opts.libraries[i]);
		}
		else if(Ov_Fail(result)) {
			ov_logfile_error("Could'nt create library %s: %s (error code 0x%4.4x).",
					opts.libraries[i], ov_result_getresulttext(result), result);
		}
		i++;
	}
	/*
	 *	set the serverpassword of the database
	 */
	if (!pdb->serverpassword) ov_vendortree_setserverpassword(opts.password);
	ov_vendortree_setServerPID();
	if(!opts.exit){
		/*
		 *   run server
		 */
		ov_logfile_info("Starting server...");
		result = ov_ksserver_stripped_create(opts.servername, ov_server_sighandler);
		if(Ov_OK(result)) {
			ov_logfile_info("Server started.");
			ov_logfile_info("Servername: %s.",opts.servername);
			ov_vendortree_setstartuptime(NULL);
			ov_server_run=TRUE;

			/*
			 * If exec option is set execute the demanded task here
			 */
			if(opts.exec)
				if(!ov_scheduler_execnamedeventonce(opts.execIdent, opts.execClass, opts.execLib))
					ov_logfile_error("Requested task not found in schedulers event queue");


			ov_ksserver_stripped_run();

			ov_server_run=FALSE;
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

	if(opts.startup) {
		ov_database_shutdown();
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

	ov_database_unload();

	/*
	 *	close the logfile, free its memory and return
	 */
	ov_logfile_close();
	ov_logfile_free();
	ov_vendortree_free();
	ov_options_free(&opts);
	ov_destroyHeap();
	return exit_status;
}

/*	----------------------------------------------------------------------	*/

/*
*	End of file
*/



