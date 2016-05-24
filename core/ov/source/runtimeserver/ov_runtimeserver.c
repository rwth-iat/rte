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


OV_DLLFNCEXPORT OV_RESULT ov_supervised_database_startup(void);

OV_DLLFNCEXPORT OV_RESULT ov_supervised_database_map(
	OV_STRING 	dbname
);

/*	----------------------------------------------------------------------	*/
/*
 * Helper functions to parse config file
 */

static int isWhiteSpace(const char* character)
{
	if(*character == ' ' || *character == '\t')
		return 1;
	else
		return 0;
}

static char* skipWhiteSpace(const char* line)
{
	while(*line != '\n' && *line!= '\r' && *line != '\0' && isWhiteSpace(line))
		line++;
	return ((char*)line);
}

static int isComment(const char* line)
{
	line = skipWhiteSpace(line);
	if(*line == '#')
		return 1;
	else
		return 0;
}

static void terminateLine(char* line)
{
	for(; *line !='\0' && *line!='\n' && *line!='\r' && *line!='#'; line++)
		;
	*line='\0';
	return;
}

static void stripSpaceAtEnd(char* line)
{
	char* temp = line;
	if(line)
	{
		line = line + strlen(line);
		while(line > temp && isWhiteSpace(--line))
		{
			*line='\0';
		}
	}
}

/*
 * returns string containing the value; 0 when out of memory; empty string when no value found
 * allocates memory for value on heap
 */
static char* readValue(char* line)
{
	char* temp;
	char* value = NULL;

	temp = line;
	/*	move after option name (next whitespace)	*/
	for(; *temp!='\0' && *temp!='\n' && *temp!='\r' && *temp!=' ' && *temp!='\t'; temp++)
		;
	temp = skipWhiteSpace(temp);
	stripSpaceAtEnd(temp);
	if(*temp)
	{
		value = malloc(strlen(temp) + 1);
		if(!value)	/*	out of memory	*/
			return NULL;

		strcpy(value, temp);
		return value;
	}
	else
	{
		value = calloc(1, sizeof(char));
		if(!value)	/*	out of memory	*/
			return NULL;
		return value;
	}
}


/**
 * checking if the string contains only A-Z, a-z, 0-9 and _
 * @param servername to check
 * @return Bool
 */
static OV_BOOL serverNameOk(const char* servername){
	const char* temp = NULL;
	if(!servername){
		return FALSE;
	}
	temp = servername;
	while((*temp>='0' && *temp<='9')
			|| (*temp>='A' && *temp<='Z')
			|| (*temp>='a' && *temp<='z')
			|| *temp=='_')
		temp++;
	if(*temp!='\0'){	/*	nothing may follow here	*/
		return FALSE;
	} else {
		return TRUE;
	}
}

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
         
        // ov_logfile_info("ov_ksserver_stripped_run ...sleeping: %ld,%ld", delay->secs, delay->usecs);
        //if( (delay->secs != 0) || (delay->usecs != 0) ) {
            // ov_logfile_info(" !!!! ov_ksserver_stripped_run ...sleeping: %ld,%ld", delay->secs, delay->usecs);
        //}

#if !OV_SYSTEM_NT
		 sleepTime.tv_sec = delay->secs;
		 sleepTime.tv_nsec = delay->usecs*1000;
		 nanosleep(&sleepTime, NULL);
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


/*
*	End of file
*/



/*	----------------------------------------------------------------------	*/

static void ov_server_usage(void)
{	
#if OV_SERVER_HELP
		fprintf(stderr, "Usage: ov_runtimeserver (-f DATABASE -s SERVERNAME):(-c CONFIGFILE) [arguments]\n"
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
				"-c CONFIGFILE, -cf CONFIGFILE    Set filename to read configuration from\n"
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
            	"-h, --help                       Display this help message\n");
#else
		fprintf(stderr, "No help output implemented for this server. Sorry.");
#endif

}

/*
*	Main program
*/

#define MAX_STARTUP_LIBRARIES		64

int main(int argc, char **argv) {
	/*
	*	local variables
	*/

	OV_STRING	            filename = NULL;	/*	malloced	*/
	OV_STRING	            servername = NULL;	/*	malloced	*/
	OV_STRING				configFile	=	NULL;	/*	not malloced	*/
	OV_STRING				configBasePath	=	NULL;	/*	obtained from the path of the configfile	*/ /*	malloced	*/
	OV_STRING	            password = NULL;	/*	malloced	*/
	OV_STRING               libraries[MAX_STARTUP_LIBRARIES];	/*	items malloced	*/
	OV_INT		            libcount;
	OV_STRING				helper = NULL;						/*	malloced and freed locally or copied to other variable	*/
	OV_STRING				execIdent = NULL;	/*	malloced	*/
	OV_STRING				execClass = NULL;	/*	malloced	*/
	OV_STRING				execLib = NULL;	/*	malloced	*/
	OV_STRING				commandline_options = NULL;	/*	malloced	*/
	OV_STRING				tempstr = NULL;	/*	malloced and freed locally or copied to other variable	*/
	FILE*					cfFile	=	NULL;
	OV_UINT					line = 0;
	OV_UINT					hlpindex = 0;
	OV_BOOL					logfileSpecified = FALSE;
	OV_BOOL					exec = FALSE;
	OV_INSTPTR_ov_library   plib;
	OV_INSTPTR_ov_domain    pdom;
	OV_INT                  i;
	OV_RESULT	            result;
	OV_INT 		            port = 0; /* KS_ANYPORT */
	OV_UINT					maxAllowedJitter = 0;
	OV_UINT					ks_maxItemsPerRequest = 0;
	OV_UINT					maxStringLength = 0;
	OV_UINT					maxVectorLength = 0;
	OV_UINT					maxNameLength = 0;
	OV_UINT					maxHierarchyDepth = 0;
	OV_ANY					tempAny = {{OV_VT_VOID, {0}}, 0, {0,0}};
	OV_BOOL		            startup = TRUE;
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
				if(argv[i])
				{
					filename = malloc(strlen(argv[i])+1);
					if(!filename){
						ov_logfile_free();
						return EXIT_FAILURE;
					}
					strcpy(filename, argv[i]);
				}
				else
				{
					goto HELP;
				}
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
				if(argv[i])
				{
					if(!serverNameOk(argv[i])){
						ov_logfile_error("Invalid servername given: only A-Z, a-z, 0-9 and _ allowed.");
						ov_logfile_free();
						return EXIT_FAILURE;
					}
					servername = malloc(strlen(argv[i])+1);
					if(!servername){
						ov_logfile_free();
						return EXIT_FAILURE;
					}
					strcpy(servername, argv[i]);
				}
				else
				{
					goto HELP;
				}
			} else {
				goto HELP;
			}
		}
		/*
		 * set config file option and read out file if set
		 */
		else if(!strcmp(argv[i], "-c") || !strcmp(argv[i], "-cf"))
		{
			i++;
			if(i<argc){
				configFile = argv[i];
			}
				else
					goto HELP;

			if(configFile && *configFile)
			{
				char lineStr[256];
				char* startRead = NULL;
				char* temp = NULL;
				char* temp2 = NULL;
				OV_UINT j;
				/*
				 * parse config file
				 * some memory is allocated on the heap. these variables are used while the server is running
				 * hence this is no memleak. they are freed by the operating system on termination of the server
				 *
				 *
				 * options from the commandline are NOT overwritten
				 * lines starting with # and empty lines are ignored
				 * Space and tab serve as whitespaces
				 * lines are parsed as a whole
				 * lines may be 256 characters long
				 * no multi-line entries
				 *
				 * lines have the pattern
				 * KEY VALUE
				 * e.g. SERVERNAME ov_server1
				 *
				 * recognized options are:
				 * DBFILE		path to database file
				 * SERVERNAME	name of this ov_server
				 * ID			Ticket Identification for server access
				 * PORT			server port number
				 * LIBRARY		Start server with library. one library per line
				 * LOGFILE		logfile name, you may use stdout, stderr or (on NT-systems) ntlog
				 * ACTIVITYLOCK	Locks OV activities (scheduler and accessorfnc). No argument
				 * OPTION text	Appends the option text to the
				 * 				cmdline_options variable in Vendortree
				 * 				text is mandatory
				 * NOSTARTUP	Do not startup the database. No argument
				 * EXIT			Exit immediately (test if database loads). No argument
				 * EXEC IDENTIFIER CLASS LIBRARY	Executes the first event in the schedulers
				 * 									queue that matches concerning IDENTIFIER
				 * 									CLASS and LIBRARY before starting the server.
				 * 									All parameters are mandatory. Use '/' as wildcard.
				 * ALLOWEDJITTER	number of microseconds the scheduler may jitter before incrementing numExceeds in vondortree (the latter documents the realtime behaviour of the system)
				 * KSMAXSTRLENGTH	maximum length of strings to process with ks
				 * KSMAXVECLENGTH	maximum length of vectors to process with ks
				 * KSMAXITEMSPERREQ	maximum number of items per ks-request
				 * DBSIZE		ignored. only for dbutil
				 * DBUTILLOG	ignored. only for dbutil
				 */

				cfFile = fopen(configFile, "r");
				if(!cfFile)
				{
					perror("Could not open config file");
					ov_logfile_free();
					return EXIT_FAILURE;
				}
				clearerr(cfFile);

				/*	get base path from path part of configFile	*/
#if OV_SYSTEM_NT
				for(j = strlen(configFile); configFile[j] != '\\' && configFile[j] != '/' && (j>0); j--);
#else
				for(j = strlen(configFile); (configFile[j] != '/') && (j>0); j--);
#endif
				if((j>0))
				{
					configBasePath = malloc(j+2);
					if(!configBasePath)
					{
						ov_logfile_error("Could not reserve memory for basePath. Aborting.");
						ov_logfile_free();
						return EXIT_FAILURE;
					}
					strncpy(configBasePath, configFile, j+1);
					configBasePath[j+1] = '\0';
					//ov_logfile_debug("BasePath: %s", configBasePath);
				}
				/*
				 * loop over lines
				 */
				while(fgets(lineStr, sizeof(lineStr), cfFile))
				{
					line++;
					/*      check if line complete   */
					if(!strchr(lineStr, '\n') && !feof(cfFile))
					{
						ov_logfile_error("Error reading config file: line %u too long", line);
						ov_logfile_free();
						return EXIT_FAILURE;
					}

					if(isComment(lineStr))
					{	/*	skip line if comment	*/
						continue;
					}

					startRead = skipWhiteSpace(lineStr);
					if(*startRead == '\0')
						break;	/*	probably EOF	*/

					if(*startRead == '\n' || *startRead == '\r')
						continue;	/*	empty line	*/
						/*	set terminating '\0' at occurance of newline or '#'	*/
					terminateLine(startRead);

					/**********************************************************************************
					 * parse parameters
					 *********************************************************************************/

					/*	DBFILE	*/
					if(strstr(startRead, "DBFILE")==startRead)
					{
						if(!filename)
							filename = readValue(startRead);
						if(!filename || !*filename){
							ov_logfile_free();
							return EXIT_FAILURE;
						}
					}
					/*	SERVERNAME	*/
					else if(strstr(startRead, "SERVERNAME")==startRead)
					{
						if(!servername)
							servername = readValue(startRead);
						if(!servername || !*servername){
							ov_logfile_free();
							return EXIT_FAILURE;
						}
						if(!serverNameOk(servername))
						{	ov_logfile_error("Error parsing SERVERNAME in line %u: only A-Z, a-z, 0-9 and _ allowed in servername.", line);
							ov_logfile_free();
							return EXIT_FAILURE;
						}
					}
					/*	ID	*/
					else if(strstr(startRead, "ID")==startRead)
					{
						if(!password)
							password = readValue(startRead);
						if(!password || !*password){
							ov_logfile_free();
							return EXIT_FAILURE;
						}
					}
					/*	PORT	*/
					else if(strstr(startRead, "PORT")==startRead)
					{
						if(!port)
							temp = readValue(startRead);
						if(!temp || !*temp){
							ov_logfile_free();
							return EXIT_FAILURE;
						}
						port = strtol(temp, &temp2, 0);
						if(*temp2)
						{
							ov_logfile_error("Error parsing line %u: too many arguments for PORT.", line);
							ov_logfile_free();
							return EXIT_FAILURE;
						}
						free(temp);
					}
					/*	LIBRARY	*/
					else if(strstr(startRead, "LIBRARY")==startRead)
					{
						if (libcount<MAX_STARTUP_LIBRARIES) {
							libraries[libcount] = readValue(startRead);
							if(!libraries[libcount] || !*libraries[libcount]){
								ov_logfile_free();
								return EXIT_FAILURE;
							}
							libcount++;
						}
						else 	ov_logfile_error("Too many libraries in start command and configfile.\n");
					}
					/*	LOGFILE	*/
					else if(strstr(startRead, "LOGFILE")==startRead)
					{
						if(logfileSpecified == FALSE)
						{
							temp = readValue(startRead);
							if(!temp || !*temp){
								ov_logfile_free();
								return EXIT_FAILURE;
							}
							if(!strcmp(temp, "stdout")) {
								ov_logfile_logtostdout(NULL);
							} else if(!strcmp(temp, "stderr")) {
								ov_logfile_logtostderr(NULL);
#if OV_SYSTEM_NT
							} else if(!strcmp(temp, "ntlog")) {
								ov_logfile_logtontlog(NULL);
#endif
							} else {
#if OV_SYSTEM_UNIX
								if(!(temp[0]=='/'))
#else
								if(!(temp[1]==':'|| temp[0]=='\\'))
#endif
								{/*	relative path --> prepend basePath	*/
									if(configBasePath && *configBasePath)
									{
										hlpindex = strlen(configBasePath);
										helper = calloc(hlpindex+strlen(temp)+2, sizeof(char));
										if(!helper)
										{
											ov_logfile_error("Could not reserve memory for logfile path. Aborting.");
											ov_logfile_free();
											return EXIT_FAILURE;
										}
										strcpy(helper, configBasePath);
										if(!(helper[hlpindex-1]=='\\' || helper[hlpindex-1]=='/'))
										{
#if OV_SYSTEM_NT
											helper[hlpindex] = '\\';
#else
											helper[hlpindex] = '/';
#endif
											hlpindex++;
										}
										strcpy(&(helper[hlpindex]), temp);
										free(temp);
										temp = helper;
									}
								}
								if(Ov_Fail(ov_logfile_open(NULL, temp, "w"))) {
									ov_logfile_error("Could not open log file: \"%s\".\n", temp);
									ov_logfile_free();
									return EXIT_FAILURE;
								}
							}
							free(temp);
						}
					}
					/*	ACTIVITYLOCK	*/
					else if(strstr(startRead, "ACTIVITYLOCK")==startRead)
					{
						ov_activitylock = TRUE;
						if(*(startRead+12))
						{
							ov_logfile_error("Error parsing line %u: ACTIVITYLOCK does not accept parameters.", line);
							ov_logfile_free();
							return EXIT_FAILURE;
						}
					}
					/*	OPTION	*/
					else if(strstr(startRead, "OPTION")==startRead)
					{
						temp = readValue(startRead);
						if(!temp || !*temp){
							ov_logfile_free();
							return EXIT_FAILURE;
						}
						temp2 = temp;
						while((!isWhiteSpace(temp2)) && *temp2)
							temp2++;
						if(*temp2!='\0')
						{	/*	whitespaces at line end are stripped: nothing may follow here	*/
							ov_logfile_error("Error parsing OPTION in line %u: no whitespaces allowed in options.", line);
						}
						if(commandline_options)
						{
							tempstr = realloc(commandline_options, strlen(commandline_options)+strlen(temp)+2); //oldlength + new option length + ' ' + '\0'
							if(tempstr)
							{
								sprintf(tempstr, "%s %s", tempstr, temp);
								commandline_options = tempstr;
							}
						}
						else	//first time commandline_options is a NULL-pointer --> strlen would crash
						{
							commandline_options = malloc(strlen(temp)+1);
							if(commandline_options)
								strcpy(commandline_options, temp);
						}
						free(temp);
					}
					/*	NOSTARTUP	*/
					else if(strstr(startRead, "NOSTARTUP")==startRead)
					{
						startup = FALSE;
						if(*(startRead+9))
						{
							ov_logfile_error("Error parsing line %u: NOSTARTUP does not accept parameters.", line);
							ov_logfile_free();
							return EXIT_FAILURE;
						}
					}
					/*	EXIT	*/
					else if(strstr(startRead, "EXIT")==startRead)
					{
						exit = TRUE;
						if(*(startRead+4))
						{
							ov_logfile_error("Error parsing line %u: EXIT does not accept parameters.", line);
							ov_logfile_free();
							return EXIT_FAILURE;
						}
					}
					/*	EXEC	*/
					else if(strstr(startRead, "EXEC")==startRead)
					{
						if(!exec)
						{
							temp = readValue(startRead);
							if(!temp || !*temp){
								ov_logfile_free();
								return EXIT_FAILURE;
							}
							temp2 = temp;
							while(!isWhiteSpace(temp2))
								temp2++;
							*temp2 = '\0';
							execIdent = temp;
							temp = ++temp2;
							while(!isWhiteSpace(temp2))
								temp2++;
							*temp2 = '\0';
							execClass = temp;
							temp = ++temp2;
							execLib = temp;
							temp2++;
							while(!isWhiteSpace(temp2))
								temp2++;
							if(*temp2)
							{
								ov_logfile_error("Error parsing line %u: too many arguments to EXEC option.", line);
								ov_logfile_free();
								return EXIT_FAILURE;
							}
							exec = TRUE;
						}
					}
					/*	ALLOWEDJITTER	*/
					else if(strstr(startRead, "ALLOWEDJITTER")==startRead)
					{
						temp = readValue(startRead);
						if(!temp || !*temp){
							ov_logfile_free();
							return EXIT_FAILURE;
						}
						maxAllowedJitter = strtoul(temp, &temp2, 0);
						if(*temp2)
						{
							ov_logfile_error("Error parsing line %u: too many arguments for ALLOWEDJITTER.", line);
							ov_logfile_free();
							return EXIT_FAILURE;
						}
						free(temp);
					}
					/*	MAXSTRLENGTH	*/
					else if(strstr(startRead, "MAXSTRLENGTH")==startRead)
					{
						temp = readValue(startRead);
						if(!temp || !*temp){
							ov_logfile_free();
							return EXIT_FAILURE;
						}
						maxStringLength = strtoul(temp, &temp2, 0);
						if(*temp2)
						{
							ov_logfile_error("Error parsing line %u: too many arguments for MAXSTRLENGTH.", line);
							ov_logfile_free();
							return EXIT_FAILURE;
						}
						free(temp);
					}
					/*	MAXVECLENGTH	*/
					else if(strstr(startRead, "MAXVECLENGTH")==startRead)
					{
						temp = readValue(startRead);
						if(!temp || !*temp){
							ov_logfile_free();
							return EXIT_FAILURE;
						}
						maxVectorLength = strtoul(temp, &temp2, 0);
						if(*temp2)
						{
							ov_logfile_error("Error parsing line %u: too many arguments for MAXVECLENGTH.", line);
							ov_logfile_free();
							return EXIT_FAILURE;
						}
						free(temp);
					}
					/*	MAXNAMELENGTH	*/
					else if(strstr(startRead, "MAXNAMELENGTH")==startRead)
					{
						temp = readValue(startRead);
						if(!temp || !*temp){
							ov_logfile_free();
							return EXIT_FAILURE;
						}
						maxNameLength = strtoul(temp, &temp2, 0);
						if(*temp2)
						{
							ov_logfile_error("Error parsing line %u: too many arguments for MAXNAMELENGTH.", line);
							ov_logfile_free();
							return EXIT_FAILURE;
						}
						free(temp);
					}
					/*	MAXHIERARCHYDEPTH	*/
					else if(strstr(startRead, "MAXHIERARCHYDEPTH")==startRead)
					{
						temp = readValue(startRead);
						if(!temp || !*temp){
							ov_logfile_free();
							return EXIT_FAILURE;
						}
						maxHierarchyDepth = strtoul(temp, &temp2, 0);
						if(*temp2)
						{
							ov_logfile_error("Error parsing line %u: too many arguments for MAXHIERARCHYDEPTH.", line);
							ov_logfile_free();
							return EXIT_FAILURE;
						}
						free(temp);
					}
					/*	KSMAXITEMSPERREQ	*/
					else if(strstr(startRead, "KSMAXITEMSPERREQ")==startRead)
					{
						temp = readValue(startRead);
						if(!temp || !*temp){
							ov_logfile_free();
							return EXIT_FAILURE;
						}
						ks_maxItemsPerRequest = strtoul(temp, &temp2, 0);
						if(*temp2)
						{
							ov_logfile_error("Error parsing line %u: too many arguments for KSMAXITEMSPERREQ.", line);
							ov_logfile_free();
							return EXIT_FAILURE;
						}
						free(temp);
					}
					/*	DBSIZE	*/
					else if(strstr(startRead, "DBSIZE")==startRead)
					{/*	ignored --> only for dbutil	*/
						;
					}
					/*	DBUTILLOG	*/
					else if(strstr(startRead, "DBUTILLOG")==startRead)
					{/*	ignored --> only for dbutil	*/
						;
					}
					/*
					 * default: option unknown
					 */
					else
					{
						ov_logfile_error("Error parsing line %u: unknown option", line);
						ov_logfile_free();
						return EXIT_FAILURE;
					}
				}
				/*	getline returns -1 on error or eof. eof is ok, error aborts program	*/
				if(ferror(cfFile))
				{
					perror("Error reading config file");
					ov_logfile_free();
					fclose(cfFile);
					return EXIT_FAILURE;
				}
			}
			fclose(cfFile);
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
				if(argv[i])
				{
					password = malloc(strlen(argv[i])+1);
					if(!password){
						ov_logfile_free();
						return EXIT_FAILURE;
					}
					strcpy(password, argv[i]);
				}
				else
				{
					goto HELP;
				}
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
			if (libcount<MAX_STARTUP_LIBRARIES) {
				if(argv[i])
				{
					libraries[libcount] = malloc(strlen(argv[i])+1);
					if(!libraries[libcount]){
						ov_logfile_free();
						return EXIT_FAILURE;
					}
					strcpy(libraries[libcount], argv[i]);
					libcount++;
				}
				else
				{
					goto HELP;
				}
			}
			else 	ov_logfile_error("Too many libraries in start command and configfile.\n");
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
			logfileSpecified = TRUE;
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
#if OV_SYSTEM_UNIX
					if(!(argv[i][0]=='/'))
#else
					if(!(argv[i][1]==':'|| argv[i][0]=='\\'))
#endif
					{/*	relative path --> prepend basePath	*/
						if(configBasePath && *configBasePath)
						{
							hlpindex = strlen(configBasePath);
							helper = calloc(hlpindex+strlen(argv[i])+2, sizeof(char));
							if(!helper)
							{
								ov_logfile_error("Could not reserve memory for logfile path. Aborting.");
								ov_logfile_free();
								return EXIT_FAILURE;
							}
							strcpy(helper, configBasePath);
							if(!(helper[hlpindex-1]=='\\' || helper[hlpindex-1]=='/'))
							{
#if OV_SYSTEM_NT
								helper[hlpindex] = '\\';
#else
								helper[hlpindex] = '/';
#endif
								hlpindex++;
							}
						strcpy((helper+hlpindex), argv[i]);
						}
					}
					else
					{	/*	absolute path --> just copy	*/
						if(argv[i])
						{
							helper = malloc(strlen(argv[i])+1);
							if(!helper){
								ov_logfile_free();
								return EXIT_FAILURE;
							}
							strcpy(helper, argv[i]);
						}
						else
						{
							goto HELP;
						}
					}

					if(Ov_Fail(ov_logfile_open(NULL, helper, "w"))) {
						ov_logfile_error("Could not open log file: \"%s\".\n", argv[i]);
						ov_logfile_free();
						return EXIT_FAILURE;
					}
				free(helper);
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
			ov_logfile_free();
			return EXIT_FAILURE;
		}
		/*
		 *	exec option
		 *	before the server is started up ths first event in the schedulers event queue that matchers identifier, class and / or library
		 *	is executed ONCE
		 *	This is useful to start the RootComTask on a database with set activitylock ("forensic mode" / dead database with communication)
		 */
		else if(!strcmp(argv[i], "--exec")) {
			exec = TRUE;
			i++;
			if(i<argc) {
				if(*(argv[i])!='/')			//get Identifier
				{
					execIdent = malloc(strlen(argv[i])+1);
					if(!execIdent){
						ov_logfile_free();
						return EXIT_FAILURE;
					}
					strcpy(execIdent, argv[i]);
				}
				i++;
				if(i<argc) {
					if(*(argv[i])!='/')		//get Class
					{
						execClass = malloc(strlen(argv[i])+1);
						if(!execClass){
							ov_logfile_free();
							return EXIT_FAILURE;
						}
						strcpy(execClass, argv[i]);
					}

					i++;
					if(i<argc) {
						if(*(argv[i])!='/')	//get Library
						{
							execLib = malloc(strlen(argv[i])+1);
							if(!execLib){
								ov_logfile_free();
								return EXIT_FAILURE;
							}
							strcpy(execLib, argv[i]);
						}

					} else {
						goto HELP;
					}
				} else {
					goto HELP;
				}
			} else {
				goto HELP;
			}
		}

		/*
		*	display help option
		*/
		else if(!strcmp(argv[i], "-h") || !strcmp(argv[i], "--help")) {
HELP:		ov_server_usage();			
			ov_logfile_free();
			return EXIT_FAILURE;
		} else {
			goto HELP;
		}
	}

	/**********************************************************************************************************************************************************
	 * End of commandline / logfile parsing
	 **********************************************************************************************************************************************************/

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
		ov_logfile_free();
		return EXIT_SUCCESS;
	}
#endif
	/*
	*	check servername
	*/
	if(!servername) {
		ov_logfile_error("No server name set. Aborting.");
		ov_server_usage();
		ov_logfile_free();
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
	ov_vendortree_setservername(servername);	//02.04.06 add by Senad

	/*
	 * set allowedJitter and ks-parameters
	 */
	tempAny.value.vartype = OV_VT_UINT;
	if(maxAllowedJitter){
		tempAny.value.valueunion.val_uint = maxAllowedJitter;
		ov_vendortree_setAllowedJitter(&tempAny, NULL);
	}
	if(maxStringLength){
		tempAny.value.valueunion.val_uint = maxStringLength;
		ov_vendortree_setMaxStringLength(&tempAny, NULL);
	}
	if(maxVectorLength){
		tempAny.value.valueunion.val_uint = maxVectorLength;
		ov_vendortree_setMaxVectorLength(&tempAny, NULL);
	}
	if(maxNameLength){
		tempAny.value.valueunion.val_uint = maxNameLength;
		ov_vendortree_setMaxNameLength(&tempAny, NULL);
	}
	if(maxHierarchyDepth){
		tempAny.value.valueunion.val_uint = maxHierarchyDepth;
		ov_vendortree_setMaxHierarchyDepth(&tempAny, NULL);
	}
	if(ks_maxItemsPerRequest){
		tempAny.value.valueunion.val_uint = ks_maxItemsPerRequest;
		ov_vendortree_setKsMaxItems(&tempAny, NULL);
	}
	tempAny.value.vartype = OV_VT_VOID;
	tempAny.value.valueunion.val_uint = 0;
	/*
	*	map existing database
	*	abort if no file set
	*/

	if(!filename || !(*filename))
	{
		ov_logfile_error("No database file set. Aborting.");
		ov_server_usage();
		ov_logfile_free();
		ov_vendortree_free();
		return EXIT_FAILURE;
	}
	/*
	 * check if filename is a relative path. if so put basePath in front of it
	 */
#if OV_SYSTEM_UNIX
	if(!(filename[0]=='/'))
#else
	if(!(filename[1]==':'|| filename[0]=='\\'))
#endif
	{/*	relative path --> prepend basePath	*/
		if(configBasePath && *configBasePath)
		{
			hlpindex = strlen(configBasePath);
			helper = calloc(hlpindex+strlen(filename)+2, sizeof(char));
			if(!helper)
			{
				ov_logfile_error("Could not reserve memory for filename path. Aborting.");
				ov_logfile_free();
				ov_vendortree_free();
				return EXIT_FAILURE;
			}
			strcpy(helper, configBasePath);
			if(!(helper[hlpindex-1]=='\\' || helper[hlpindex-1]=='/'))
			{
#if OV_SYSTEM_NT
				helper[hlpindex] = '\\';
#else
				helper[hlpindex] = '/';
#endif
				hlpindex++;
			}
			strcpy((helper+hlpindex), filename);
			free(filename);
			filename = helper;
		}
	}
	ov_logfile_info("Mapping database \"%s\"...", filename);

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
		ov_logfile_free();
		ov_vendortree_free();
		return EXIT_FAILURE;
	}
	ov_logfile_info("Database mapped.");

	/*
	 * set the commandline options
	 * if port is set put it in front as PORT
	 */
	if(port)
	{
		if(commandline_options)
		{
			tempstr = malloc(strlen(commandline_options)+16); //"PORT=" + max characters in INT + '\0'
			if(tempstr)
			{
				sprintf(tempstr, "PORT=%" OV_PRINT_UINT " %s", port, commandline_options);
				free(commandline_options);
				commandline_options = tempstr;
			}
		}
		else
		{
			commandline_options = malloc(16);
			if(commandline_options)
			{
				sprintf(commandline_options, "PORT=%" OV_PRINT_UINT, port);
			}
		}
	}
	/*	set config basPath	*/
	if(configBasePath && *configBasePath){
		if(commandline_options)
		{
			tempstr = malloc(8 + strlen(configBasePath) + 1 + strlen(commandline_options) + 1); // "CONFDIR=" + ... + " " + ... + '\0'
			if(tempstr)
			{
				sprintf(tempstr, "CONFDIR=%s %s", configBasePath, commandline_options);
				free(commandline_options);
				commandline_options = tempstr;
			}
		}
		else
		{
			commandline_options = malloc(8 + strlen(configBasePath) + 1); // "CONFDIR=" + ... + '\0'
			if(commandline_options)
			{
				sprintf(commandline_options, "CONFDIR=%s", configBasePath);
			}
		}
	}else{
		if(commandline_options)
		{
			tempstr = malloc(8 + 3 + strlen(commandline_options) + 1); // "CONFDIR=" + "./ " + ... + '\0'
			if(tempstr)
			{
#if OV_SYSTEM_UNIX
				sprintf(tempstr, "CONFDIR=./ %s", commandline_options);
#else
				sprintf(tempstr, "CONFDIR=.\\ %s", commandline_options);
#endif

				free(commandline_options);
				commandline_options = tempstr;
			}
		}
		else
		{
			commandline_options = malloc(8 + 2 + 1); // "CONFDIR=" + "./" + '\0'
			if(commandline_options)
			{
#if OV_SYSTEM_UNIX
				sprintf(commandline_options, "CONFDIR=./");
#else
				sprintf(commandline_options, "CONFDIR=.\\");
#endif
			}
		}
	}
	ov_vendortree_setcmdlineoptions(commandline_options);


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
	ov_vendortree_setServerPID();
	if(!exit){
		/*
		*   run server
		*/
		ov_logfile_info("Starting server...");
		result = ov_ksserver_stripped_create(servername, ov_server_sighandler);
		if(Ov_OK(result)) {
			ov_logfile_info("Server started.");
			ov_logfile_info("Servername: %s.",servername);
			ov_vendortree_setstartuptime(NULL);
			ov_server_run=TRUE;

			/*
			 * If exec option is set execute the demanded task here
			 */
			if(exec)
				if(!ov_scheduler_execnamedeventonce(execIdent, execClass, execLib))
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
	free(filename);
	ov_logfile_info("Database unmapped.");

	free(servername);
	free(commandline_options);
	free(configBasePath);
	free(password);
	for(i=0; i < libcount; i++)
		free(libraries[i]);
	free(execIdent);
	free(execClass);
	free(execLib);

	/*
	*	close the logfile, free its memory and return
	*/
	ov_logfile_close();
	ov_logfile_free();
	ov_vendortree_free();
	return exit_status;
}

/*	----------------------------------------------------------------------	*/

/*
*	End of file
*/



