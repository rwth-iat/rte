/*
 *   $Id: ov_options.c,v 1.0 2018-12-13 $
 *
 *   Copyright (C) 2018
 *   Lehrstuhl fuer Prozessleittechnik,
 *   D-52056 Aachen, Germany.
 *   All rights reserved.
 *
 *   Author: Lars Nothdurft <l.nothdurft@plt.rwth-aachen.de>
 *
 *   This file is part of the ACPLT/OV Package
 *   Licensed under the Apache License, Version 2.0 (the "License");
 *   you may not use this file except in compliance with the License.
 *   You may obtain a copy of the License at
 *
 *       http://www.apache.org/licenses/LICENSE-2.0
 *
 *   Unless required by applicable law or agreed to in writing, software
 *   distributed under the License is distributed on an "AS IS" BASIS,
 *   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *   See the License for the specific language governing permissions and
 *   limitations under the License.
 */
/*
 *	History:
 *	--------
 *	12-Dec-2018 Lars Nothdurft <l.nothdurft@plt.rwth-aachen.de>: File created.
 */

#include "libov/ov_database.h"
#include "libov/ov_ov.h"
#include "libov/ov_result.h"
#include "libov/ov_logfile.h"

#include "libov/ov_options.h"

/*
 * Initializes options struct with default values
 */
void ov_options_init(ov_options* opts){
	opts->activitylock = FALSE;
	opts->commandline_options = NULL;
	opts->configBasePath = NULL;
	opts->configFile = NULL;
	opts->dbBackupFilename = NULL;
	opts->dbFilename = NULL;
	opts->dbSize = 0;
	opts->dbflags = OV_DBOPT_VERBOSE;
	opts->dumpfilename = NULL;
	opts->exec = FALSE;
	opts->execClass = NULL;
	opts->execIdent = NULL;
	opts->execLib = NULL;
	opts->exit = FALSE;
	opts->extended = FALSE;
	opts->fbdDump = FALSE;
	opts->fbdDumpMode = 0;
	opts->fbdStart = NULL;
	opts->ks_maxItemsPerRequest = 0;
	opts->libcount = 0;
	opts->logfile = NULL;
	opts->maxAllowedJitter = 0;
	opts->maxHierarchyDepth = 0;
	opts->maxNameLength = 0;
	opts->maxStringLength = 0;
	opts->maxVectorLength = 0;
	opts->password = NULL;
	opts->port = 0;
	opts->servername = NULL;
	opts->startup = TRUE;
	memset(opts->libraries, 0, sizeof(opts->libraries));

#if TLSF
	opts->poolsize = 0;
#endif

#if OV_SYSTEM_RMOS
	opts->taskid = 0;
	opts->terminate = FALSE;
#endif

	opts->usage = NULL;
	opts->version = NULL;
}

/*
 * frees options struct
 */
void ov_options_free(ov_options* opts){
	int i;
	free(opts->commandline_options);
	opts->commandline_options = NULL;
	free(opts->configBasePath);
	opts->configBasePath = NULL;
	free(opts->execClass);
	opts->execClass = NULL;
	free(opts->execIdent);
	opts->execIdent = NULL;
	free(opts->execLib);
	opts->execLib = NULL;
	free(opts->password);
	opts->password = NULL;
	free(opts->servername);
	opts->servername = NULL;
	for(i=0; i<opts->libcount; i++){
		free(opts->libraries[i]);
		opts->libraries[i] = NULL;
	}
	opts->libcount = 0;
}

static OV_INLINE void opt_clear_string(char** var){
	if(*var){
		free(var);
		var = NULL;
	}
}

static OV_INLINE void opt_reassign_string(char** var, char* value){
	*var = realloc(*var, strlen(value));
	if(*var)
		strcpy(*var, value);
}


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

static int set_logfile(ov_options* opts, char* logfile){

	char* helper = NULL;
	int hlpindex = 0;

	if(!logfile && !*logfile)
		return EXIT_FAILURE;

	opt_reassign_string(&opts->logfile, logfile);

	if(!strcmp(logfile, "stdout")) {
		ov_logfile_logtostdout(NULL);
	} else if(!strcmp(logfile, "stderr")) {
		ov_logfile_logtostderr(NULL);
#if OV_SYSTEM_NT
	} else if(!strcmp(logfile, "ntlog")) {
		ov_logfile_logtontlog(NULL);
#endif
	} else {
#if OV_SYSTEM_UNIX
		if(!(logfile[0]=='/'))
#else
		if(!(logfile[1]==':'|| logfile[0]=='\\'))
#endif
			{/*	relative path --> prepend basePath	*/
				if(opts->configBasePath && *opts->configBasePath)
				{
					hlpindex = strlen(opts->configBasePath);
					helper = calloc(hlpindex+strlen(logfile)+2, sizeof(char));
					if(!helper)
					{
						ov_logfile_error("Could not reserve memory for logfile path. Aborting.");
						return EXIT_FAILURE;
					}
					strcpy(helper, opts->configBasePath);
					if(!(helper[hlpindex-1]=='\\' || helper[hlpindex-1]=='/'))
					{
#if OV_SYSTEM_NT
						helper[hlpindex] = '\\';
#else
						helper[hlpindex] = '/';
#endif
						hlpindex++;
					}
					strcpy(&(helper[hlpindex]), logfile);
					free(opts->logfile);// free old value
					opts->logfile = helper;
				}
			}
		if(Ov_Fail(ov_logfile_open(NULL, opts->logfile, "w"))) {
			ov_logfile_error("Could not open log file: \"%s\".\n", opts->logfile);
			return EXIT_FAILURE;
		}
	}

	return EXIT_SUCCESS;
}

OV_RESULT ov_readConfigFile(ov_options* opts, char* configFile){


	char lineStr[256];
	char* startRead = NULL;
	char* temp = NULL;
	char* temp2 = NULL;
	char* temp3 = NULL;
	FILE* cfFile;
	OV_UINT j;
	OV_UINT line = 0;
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
	 * DBFILE		path to database file. set to '-' to use no database
	 * SERVERNAME	name of this ov_server
	 * HEAPSIZE		size of TLSF heap pool
	 * ID			Ticket Identification for server access
	 * PORT			server port number
	 * LIBRARY		Start server with library. one library per line
	 * LOGFILE		logfile name for runtimeserver, you may use stdout, stderr or (on NT-systems) ntlog
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
	 * DBSIZE			db size when need to create
	 * DBUTILLOG		logfile name for dbutil, you may use stdout, stderr or (on NT-systems) ntlog
	 * DBNOMAP			read database but do not map
	 * DBFORCECREATE	force creation of a new database
	 * 					old database file is removed
	 */

	if(!configFile || !(*configFile)){

		return EXIT_FAILURE;
	}

	cfFile = fopen(configFile, "r");
	if(!cfFile)
	{
		fprintf(stderr, "Could not open config file");
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
		opts->configBasePath = malloc(j+2);
		if(!opts->configBasePath)
		{
			ov_logfile_error("Could not reserve memory for basePath. Aborting.");
			return EXIT_FAILURE;
		}
		strncpy(opts->configBasePath, configFile, j+1);
		opts->configBasePath[j+1] = '\0';
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
		/*	set terminating '\0' at occurrence of newline or '#'	*/
		terminateLine(startRead);

		/**********************************************************************************
		 * parse parameters
		 *********************************************************************************/

		/*	DBFILE	*/
		if(strstr(startRead, "DBFILE")==startRead)
		{
			opt_clear_string(&opts->dbFilename);
			opts->dbFilename = readValue(startRead);
			if(!opts->dbFilename || !*opts->dbFilename){
				return EXIT_FAILURE;
			}
		}
		/*	SERVERNAME	*/
		else if(strstr(startRead, "SERVERNAME")==startRead)
		{
			opt_clear_string(&opts->servername);
			opts->servername = readValue(startRead);
			if(!opts->servername || !*opts->servername){
				return EXIT_FAILURE;
			}
			if(!serverNameOk(opts->servername))
			{
				ov_logfile_error("Error parsing SERVERNAME in line %u: only A-Z, a-z, 0-9 and _ allowed in server name.", line);
				return EXIT_FAILURE;
			}
		}
#if TLSF
		/*	HEAPSIZE	*/
		else if(strstr(startRead, "HEAPSIZE")==startRead)
		{
			//ov_logfile_info("TLSF is activated");
			temp = readValue(startRead);
			if(!temp || !*temp)
				return EXIT_FAILURE;
			if(!opts->poolsize){
				opts->poolsize = strtoul(temp, NULL, 0);
			}
			else{
				return OV_ERR_HEAPOUTOFMEMORY;
			}
			free(temp);
		}
#endif
		/*	ID	*/
		else if(strstr(startRead, "ID")==startRead)
		{
			opt_clear_string(&opts->password);
			opts->password = readValue(startRead);
			if(!opts->password || !*opts->password){
				return EXIT_FAILURE;
			}
		}
		/*	PORT	*/
		else if(strstr(startRead, "PORT")==startRead)
		{
			temp = readValue(startRead);
			if(!temp || !*temp){
				return EXIT_FAILURE;
			}
			opts->port = strtol(temp, &temp2, 0);
			if(*temp2)
			{
				ov_logfile_error("Error parsing line %u: too many arguments for PORT.", line);
				free(temp);
				return EXIT_FAILURE;
			}
			free(temp);
		}
		/*	LIBRARY	*/
		else if(strstr(startRead, "LIBRARY")==startRead)
		{
			if (opts->libcount<MAX_STARTUP_LIBRARIES) {
				opts->libraries[opts->libcount] = readValue(startRead);
				if(!opts->libraries[opts->libcount] || !*opts->libraries[opts->libcount]){
					return EXIT_FAILURE;
				}
				opts->libcount++;
			}
			else 	ov_logfile_error("Too many libraries in start command and configfile.\n");
		}
		/*	LOGFILE	*/
		else if((opts->ctx==ov_runtimeserver && strstr(startRead, "LOGFILE")==startRead )||
				(opts->ctx==ov_dbutil && strstr(startRead, "DBUTILLOG")==startRead		)) {
				temp = readValue(startRead);
				if(!temp || !*temp){
					return EXIT_FAILURE;
				}
				set_logfile(opts, temp);
				free(temp);
		}
		/*	ACTIVITYLOCK	*/
		else if(strstr(startRead, "ACTIVITYLOCK")==startRead)
		{
			opts->activitylock = TRUE;
			if(*(startRead+12))
			{
				ov_logfile_error("Error parsing line %u: ACTIVITYLOCK does not accept parameters.", line);
				return EXIT_FAILURE;
			}
		}
		/*	OPTION	*/
		else if(strstr(startRead, "OPTION")==startRead)
		{
			temp = readValue(startRead);
			if(!temp || !*temp){
				return EXIT_FAILURE;
			}
			temp2 = temp;
			while((!isWhiteSpace(temp2)) && *temp2)
				temp2++;
			if(*temp2!='\0')
			{	/*	whitespaces at line end are stripped: nothing may follow here	*/
				ov_logfile_error("Error parsing OPTION in line %u: no whitespaces allowed in options.", line);
				free(temp);
				return EXIT_FAILURE;
			}
			if(opts->commandline_options)
			{
				temp2 = realloc(opts->commandline_options, strlen(opts->commandline_options)+strlen(temp)+2); //oldlength + new option length + ' ' + '\0'
				if(temp2)
				{
					strcat(temp2, " ");
					strcat(temp2, temp);
					opts->commandline_options = temp2;
				}
			}
			else	//first time commandline_options is a NULL-pointer --> strlen would crash
			{
				opts->commandline_options = malloc(strlen(temp)+1);
				if(opts->commandline_options)
					strcpy(opts->commandline_options, temp);
			}
			free(temp);
		}
		/*	NOSTARTUP	*/
		else if(strstr(startRead, "NOSTARTUP")==startRead)
		{
			opts->startup = FALSE;
			if(*(startRead+9))
			{
				ov_logfile_error("Error parsing line %u: NOSTARTUP does not accept parameters.", line);
				return EXIT_FAILURE;
			}
		}
		/*	EXIT	*/
		else if(strstr(startRead, "EXIT")==startRead)
		{
			opts->exit = TRUE;
			if(*(startRead+4))
			{
				ov_logfile_error("Error parsing line %u: EXIT does not accept parameters.", line);
				return EXIT_FAILURE;
			}
		}
		/*	EXEC	*/
		else if(strstr(startRead, "EXEC")==startRead)
		{
			if(!opts->exec)
			{
				temp = readValue(startRead);
				if(!temp || !*temp){
					return EXIT_FAILURE;
				}
				temp2 = temp;

				while(!isWhiteSpace(temp2)){
					if(!*temp2){
						EXECFEWARGUMENTSERROR:
						ov_logfile_error("Error parsing line %u: not enough arguments to EXEC option.", line);
						free(temp);
						return EXIT_FAILURE;
					}
					temp2++;
				}

				*temp2 = '\0';
				if(*temp!='/'){
					opt_reassign_string(&opts->execIdent, temp);
					if(!opts->execIdent){
						free(temp);
						return EXIT_FAILURE;
					}
				} else {
					opt_clear_string(&opts->execIdent);
				}

				temp3 = skipWhiteSpace(++temp2);

				temp2 = temp3;
				while(!isWhiteSpace(temp2)){
					if(!temp2)
						goto EXECFEWARGUMENTSERROR;
					temp2++;
				}

				*temp2 = '\0';
				if(*temp3!='/'){
					opt_reassign_string(&opts->execClass, temp3);
					if(!opts->execClass){
						free(temp);
						return EXIT_FAILURE;
					}
				} else {
					opt_clear_string(&opts->execClass);
				}

				temp3 = skipWhiteSpace(++temp2);

				temp2 = temp3;
				while(*temp2 && !isWhiteSpace(temp2))
					temp2++;

				if(*temp2){
					ov_logfile_error("Error parsing line %u: too many arguments to EXEC option.", line);
					free(temp);
					return EXIT_FAILURE;
				}

				if(*temp3!='/'){
					opt_reassign_string(&opts->execLib, temp3);
					if(!opts->execLib){
						free(temp);
						return EXIT_FAILURE;
					}
				} else {
					opt_clear_string(&opts->execLib);
				}

				opts->exec = TRUE;

				free(temp);
			}
		}
		/*	ALLOWEDJITTER	*/
		else if(strstr(startRead, "ALLOWEDJITTER")==startRead)
		{
			temp = readValue(startRead);
			if(!temp || !*temp){
				return EXIT_FAILURE;
			}
			opts->maxAllowedJitter = strtoul(temp, &temp2, 0);
			if(*temp2)
			{
				ov_logfile_error("Error parsing line %u: too many arguments for ALLOWEDJITTER.", line);
				free(temp);
				return EXIT_FAILURE;
			}
			free(temp);
		}
		/*	MAXSTRLENGTH	*/
		else if(strstr(startRead, "MAXSTRLENGTH")==startRead)
		{
			temp = readValue(startRead);
			if(!temp || !*temp){
				return EXIT_FAILURE;
			}
			opts->maxStringLength = strtoul(temp, &temp2, 0);
			if(*temp2)
			{
				ov_logfile_error("Error parsing line %u: too many arguments for MAXSTRLENGTH.", line);
				free(temp);
				return EXIT_FAILURE;
			}
			free(temp);
		}
		/*	MAXVECLENGTH	*/
		else if(strstr(startRead, "MAXVECLENGTH")==startRead)
		{
			temp = readValue(startRead);
			if(!temp || !*temp){
				return EXIT_FAILURE;
			}
			opts->maxVectorLength = strtoul(temp, &temp2, 0);
			if(*temp2)
			{
				ov_logfile_error("Error parsing line %u: too many arguments for MAXVECLENGTH.", line);
				free(temp);
				return EXIT_FAILURE;
			}
			free(temp);
		}
		/*	MAXNAMELENGTH	*/
		else if(strstr(startRead, "MAXNAMELENGTH")==startRead)
		{
			temp = readValue(startRead);
			if(!temp || !*temp){
				return EXIT_FAILURE;
			}
			opts->maxNameLength = strtoul(temp, &temp2, 0);
			if(*temp2)
			{
				ov_logfile_error("Error parsing line %u: too many arguments for MAXNAMELENGTH.", line);
				free(temp);
				return EXIT_FAILURE;
			}
			free(temp);
		}
		/*	MAXHIERARCHYDEPTH	*/
		else if(strstr(startRead, "MAXHIERARCHYDEPTH")==startRead)
		{
			temp = readValue(startRead);
			if(!temp || !*temp){
				return EXIT_FAILURE;
			}
			opts->maxHierarchyDepth = strtoul(temp, &temp2, 0);
			if(*temp2)
			{
				ov_logfile_error("Error parsing line %u: too many arguments for MAXHIERARCHYDEPTH.", line);
				free(temp);
				return EXIT_FAILURE;
			}
			free(temp);
		}
		/*	KSMAXITEMSPERREQ	*/
		else if(strstr(startRead, "KSMAXITEMSPERREQ")==startRead)
		{
			temp = readValue(startRead);
			if(!temp || !*temp){
				return EXIT_FAILURE;
			}
			opts->ks_maxItemsPerRequest = strtoul(temp, &temp2, 0);
			if(*temp2)
			{
				ov_logfile_error("Error parsing line %u: too many arguments for KSMAXITEMSPERREQ.", line);
				free(temp);
				return EXIT_FAILURE;
			}
			free(temp);
		}
		/*	DBSIZE	*/
		else if(strstr(startRead, "DBSIZE")==startRead)
		{
			temp = readValue(startRead);
			if(!temp || !*temp)
				return EXIT_FAILURE;
			opts->dbSize = strtoul(temp, NULL, 0);
			free(temp);
		}
		/*	DBNOMAP	*/
		else if(strstr(startRead, "DBNOMAP")==startRead)
		{
			opts->dbflags |= OV_DBOPT_NOMAP;
		}
		/*	DBFORCECREATE	*/
		else if(strstr(startRead, "DBFORCECREATE")==startRead)
		{
			opts->dbflags |= OV_DBOPT_FORCECREATE;
		}
		/*
		 * default: option unknown
		 */
		else
		{
			ov_logfile_error("Error parsing line %u: unknown option", line);
			return EXIT_FAILURE;
		}
	}
	/*	getline returns -1 on error or eof. eof is ok, error aborts program	*/
	if(ferror(cfFile))
	{
		fprintf(stderr, "Error reading config file");
		fclose(cfFile);
		return EXIT_FAILURE;
	}
	fclose(cfFile);


	return OV_ERR_OK;
}

OV_RESULT ov_readConfigFromArgs(ov_options* opts, int argc, char** argv){
	int i;

	for(i = 1; i < argc; i++){
		if(!strcmp(argv[i], "-cf") || !strcmp(argv[i], "--config") ||
				(opts->ctx==ov_runtimeserver && !strcmp(argv[i], "-c"))){
				/*	-c for legacy compatibility	*/
			i++;
			if(i<argc)
				opts->configFile = argv[i];
			else
				return EXIT_FAILURE;

			return ov_readConfigFile(opts, argv[i]);
		}
	}
	return EXIT_SUCCESS;
}

OV_RESULT ov_readArgs(ov_options* opts, int argc, char** argv){

	char* tempstr = NULL;
	int i;

	for(i = 1; i<argc; i++) {
		/*
		 *	set database filename option
		 */
		if(!strcmp(argv[i], "-f") || !strcmp(argv[i], "--file")) {
			i++;
			if(i<argc) {
				if(argv[i])
				{
					opts->dbFilename = realloc(opts->dbFilename, strlen(argv[i])+1);
					if(!opts->dbFilename){
						return EXIT_FAILURE;
					}
					strcpy(opts->dbFilename, argv[i]);
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
			if(opts->ctx!=ov_runtimeserver) goto HELP;
			i++;
			if(i<argc) {
				opts->dbBackupFilename = argv[i]; //TODO malloc?
			} else {
				goto HELP;
			}
		}
		/*
		 *	set server name option
		 */
		else if(!strcmp(argv[i], "-s") || !strcmp(argv[i], "--server-name")) {
			if(opts->ctx!=ov_runtimeserver) goto HELP;
			i++;
			if(i<argc) {
				if(argv[i])
				{
					if(!serverNameOk(argv[i])){
						ov_logfile_error("Invalid servername given: only A-Z, a-z, 0-9 and _ allowed.");
						return EXIT_FAILURE;
					}
					opts->servername = realloc(opts->servername, strlen(argv[i])+1);
					if(!opts->servername){
						return EXIT_FAILURE;
					}
					strcpy(opts->servername, argv[i]);
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
		 * set config file
		 * parsing of configfile should be done beforehand
		 */
		else if(!strcmp(argv[i], "-cf") || !strcmp(argv[i], "--config") ||
				(opts->ctx==ov_runtimeserver && !strcmp(argv[i], "-c"))) {
				/*	-c for legacy compatibility	*/
			i++;
			if(i<argc){
				opts->configFile = argv[i];
			}
			else
				goto HELP;
		}
		/*
		 *	set port number option
		 */
		else if(!strcmp(argv[i], "-p") || !strcmp(argv[i], "--port-number")) {
			if(opts->ctx!=ov_runtimeserver) goto HELP;
			i++;
			if(i<argc) {
				opts->port = strtol(argv[i], NULL, 0);
			} else {
				goto HELP;
			}
		}
		/*
		 *	set identification
		 */
		else if(!strcmp(argv[i], "-i") || !strcmp(argv[i], "--identifiy")) {
			if(opts->ctx!=ov_runtimeserver) goto HELP;
			i++;
			if(i<argc) {
				if(argv[i])
				{
					opts->password = realloc(opts->password, strlen(argv[i])+1);
					if(!opts->password){
						return EXIT_FAILURE;
					}
					strcpy(opts->password, argv[i]);
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
			if(opts->ctx!=ov_runtimeserver) goto HELP;
			opts->activitylock = TRUE;
		}
		/*
		 *	handle commandline options
		 */
		else if(!strcmp(argv[i], "-o") || !strcmp(argv[i], "--option")) {
			if(opts->ctx!=ov_runtimeserver) goto HELP;
			i++;
			if(i<argc) {
				if(opts->commandline_options)
				{
					tempstr = realloc(opts->commandline_options, strlen(opts->commandline_options)+strlen(argv[i])+2); //oldlength + new option length + ' ' + '\0'
					if(tempstr)
					{
						strcat(tempstr, " ");
						strcat(tempstr, argv[i]);
						opts->commandline_options = tempstr;
					}
				}
				else	//first time commandline_options is a NULL-pointer --> strlen would crash
				{
					opts->commandline_options = malloc(strlen(argv[i])+1);
					if(opts->commandline_options)
						strcpy(opts->commandline_options, argv[i]);
				}
			} else {
				goto HELP;
			}
		}
		/*
		 *	start with library
		 */
		else if(!strcmp(argv[i], "-w") || !strcmp(argv[i], "--start-with")) {
			if(opts->ctx!=ov_runtimeserver) goto HELP;
			i++;
			if (opts->libcount<MAX_STARTUP_LIBRARIES) {
				if(argv[i])
				{
					opts->libraries[opts->libcount] = malloc(strlen(argv[i])+1);
					if(!opts->libraries[opts->libcount]){
						return EXIT_FAILURE;
					}
					strcpy(opts->libraries[opts->libcount], argv[i]);
					opts->libcount++;
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
			if(opts->ctx!=ov_runtimeserver) goto HELP;
			opts->exit = TRUE;
		}
		/*
		 *	set logfile option
		 */
		else if(!strcmp(argv[i], "-l") || !strcmp(argv[i], "--logfile")) {
			i++;
			//logfileSpecified = TRUE;
			if(i<argc) {
				if(set_logfile(opts, argv[i])!=EXIT_SUCCESS)
					return EXIT_FAILURE;
			} else {
				goto HELP;
			}
		}
#if OV_SYSTEM_RMOS
		/*
		 *	terminate server option
		 */
		else if(!strcmp(argv[i], "-t") || !strcmp(argv[i], "--terminate")) {
			if(opts->ctx!=ov_runtimeserver) goto HELP;
			i++;
			if(i<argc) {
				opts->taskid = strtol(argv[i], NULL, 0);
				opts->terminate = TRUE;
			} else {
				goto HELP;
			}
		}
#endif
		/*
		 *	no startup option
		 */
		else if(!strcmp(argv[i], "-n") || !strcmp(argv[i], "--no-startup")) {
			if(opts->ctx!=ov_runtimeserver) goto HELP;
			opts->startup = FALSE;
		}
		/*
		 *	display version option
		 */
		else if(!strcmp(argv[i], "-v") || !strcmp(argv[i], "--version")) {
			if(opts->version)
				opts->version();
			else
				fprintf(stderr, "no version information available\n");

			ov_logfile_free();
			exit(EXIT_SUCCESS);
		}
		/*
		 *	exec option
		 *	before the server is started up ths first event in the schedulers event queue that matchers identifier, class and / or library
		 *	is executed ONCE
		 *	This is useful to start the RootComTask on a database with set activitylock ("forensic mode" / dead database with communication)
		 */
		else if(!strcmp(argv[i], "--exec")) {
			if(opts->ctx!=ov_runtimeserver) goto HELP;
			opts->exec = TRUE;
			i++;
			if(i<argc) {
				if(*(argv[i])!='/')			//get Identifier
				{
					opts->execIdent = realloc(opts->execIdent, strlen(argv[i])+1);
					if(!opts->execIdent){
						return EXIT_FAILURE;
					}
					strcpy(opts->execIdent, argv[i]);
				} else {
					opt_clear_string(&opts->execIdent);
				}

				i++;
				if(i<argc) {
					if(*(argv[i])!='/')		//get Class
					{
						opts->execClass = malloc(strlen(argv[i])+1);
						if(!opts->execClass){
							return EXIT_FAILURE;
						}
						strcpy(opts->execClass, argv[i]);
					} else {
						opt_clear_string(&opts->execClass);
					}

					i++;
					if(i<argc) {
						if(*(argv[i])!='/')	//get Library
						{
							opts->execLib = malloc(strlen(argv[i])+1);
							if(!opts->execLib){
								return EXIT_FAILURE;
							}
							strcpy(opts->execLib, argv[i]);
						} else {
							opt_clear_string(&opts->execLib);
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
		 *	set no map flag
		 */
		else if(!strcmp(argv[i], "--no-map")) {
			if(opts->ctx!=ov_runtimeserver) goto HELP;
			opts->dbflags |= OV_DBOPT_NOMAP;
		}
		/*
		 *	set force create flag
		 */
		else if(!strcmp(argv[i], "--force-create")) {
			opts->dbflags |= OV_DBOPT_FORCECREATE;
		}
		/*
		 *	create database option
		 */
		else if(!strcmp(argv[i], "-c") || !strcmp(argv[i], "--create")) {
			if(opts->ctx!=ov_dbutil) goto HELP;
			i++;
			if(i<argc) {
				opts->dbSize = strtoul(argv[i], NULL, 0);
			} else {
				goto HELP;
			}
		}
		/*
		 *	display extended information
		 */
		else if(!strcmp(argv[i], "-e") || !strcmp(argv[i], "--extended")) {
			if(opts->ctx!=ov_dbutil) goto HELP;
			opts->extended = TRUE;
		}
		/*
		 *	dump database
		 */
		else if(!strcmp(argv[i], "-d") || !strcmp(argv[i], "--dump")) {
			if(opts->ctx!=ov_dbutil) goto HELP;
			i++;
			if(i<argc) {
				opts->dumpfilename = argv[i];
			} else {
				goto HELP;
			}
		}
		/*
		 *	change dump format to fbd
		 */
		else if(!strcmp(argv[i], "--fbd")) {
			if(opts->ctx!=ov_dbutil) goto HELP;
			i++;
			opts->fbdDump = TRUE;
			if(i<argc) {
				if(*argv[i] != '-'){
					if(*argv[i] == '/'){
						opts->fbdStart = argv[i];
					} else {
						fprintf(stderr, "OV path needs to start with '/'");
						return EXIT_FAILURE;
					}
				} else { // next arg is an option
					i--;
				}
			}
		}
		/*
		 *	modify excluded folders for fbd dump
		 */
		else if(!strcmp(argv[i], "--dump-expert")) {
			if(opts->ctx!=ov_dbutil) goto HELP;
			opts->fbdDumpMode = 1;
		}

		/*
		 *	display help option
		 */
		else if(!strcmp(argv[i], "-h") || !strcmp(argv[i], "--help")) {
			HELP:		if(opts->usage) opts->usage(); else fprintf(stderr, "usage information not available");
			ov_logfile_free();
			exit(EXIT_FAILURE);

		} else {
			goto HELP;
		}
	}

	return OV_ERR_OK;
}
