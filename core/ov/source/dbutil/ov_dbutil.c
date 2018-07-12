/*
 *   $Id: ov_dbutil.c,v 1.10 2007-04-25 13:59:03 martin Exp $
 *
 *   Copyright (C) 1998
 *   Lehrstuhl fuer Prozessleittechnik,
 *   D-52056 Aachen, Germany.
 *   All rights reserved.
 *
 *   Author: Dirk Meyer <dirk@plt.rwth-aachen.de>
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
 *
 */
/*
 *	History:
 *	--------
 *	03-Jul-1998 Dirk Meyer <dirk@plt.rwth-aachen.de>: File created.
 *	20-Apr-1999 Dirk Meyer <dirk@plt.rwth-aachen.de>: Major revision.
 *	11-Oct-2004 Ansgar M�nnemann <ansgar@plt.rwth-aachen.de>: integration of extended infos and dbdump functions.
 *	10-Jun-2018 Lars Nothdurft <l.nothdurft@plt.rwth-aachen.de>:
 *							moved dump function to separate file and added dump as fbd.
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/stat.h>

#include "libov/ov_database.h"
#include "libov/ov_result.h"
#include "libov/ov_logfile.h"
#include "libov/ov_macros.h"
#include "libov/ov_path.h"
#include "libov/ov_config.h"

#include "ov_dbutil.h"

/*	----------------------------------------------------------------------	*/
/*
 * Helper functions to parse config file
 */

int isWhiteSpace(const char* character)
{
	if(*character == ' ' || *character == '\t')
		return 1;
	else
		return 0;
}

char* skipWhiteSpace(const char* line)
{
	while(*line != '\n' && *line!= '\r' && *line != '\0' && isWhiteSpace(line))
		line++;
	return ((char*)line);
}

int isComment(const char* line)
{
	line = skipWhiteSpace(line);
	if(*line == '#')
		return 1;
	else
		return 0;
}

void terminateLine(char* line)
{
	for(; *line !='\0' && *line!='\n' && *line!='\r' && *line!='#'; line++)
		;
	*line='\0';
	return;
}

void stripSpaceAtEnd(char* line)
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
char* readValue(char* line)
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


/*
 *   Global variables: table of static libraries
 */

#if OV_STATIC_LIBRARIES
Ov_BeginStaticLibraryTable
Ov_EndStaticLibraryTable;
#endif

/*	----------------------------------------------------------------------	*/
/*
 * common functions for dumping
 */

/*
 *	calculate size of ov data types
 */
OV_UINT ov_sizeof(
		OV_UINT vartype
) {
	switch(vartype & 0x7f){
	case OV_VT_BOOL:
		return sizeof(OV_BOOL);
	case OV_VT_BYTE:
		return sizeof(OV_BYTE);
	case OV_VT_INT:
		return sizeof(OV_INT);
	case OV_VT_UINT:
		return sizeof(OV_UINT);
	case OV_VT_SINGLE:
		return sizeof(OV_SINGLE);
	case OV_VT_STRING:
		return sizeof(OV_STRING);
	case OV_VT_DOUBLE:
		return sizeof(OV_DOUBLE);
	case OV_VT_TIME:
		return sizeof(OV_TIME);
	case OV_VT_TIME_SPAN:
		return sizeof(OV_TIME_SPAN);
	}
	return 0;
}

/*
 *	new line in respect to rnum
 */
void ov_newline(
		FILE* handle,
		int rnum
) {
	int i;

	fprintf(handle, "\n");
	for (i=0;i<rnum;i++) fprintf(handle,"\t");
}

/*	----------------------------------------------------------------------	*/

/*
 *	Main program
 */
int main(int argc, char **argv) {
	/*
	 *	local variables
	 */
	OV_STRING	        help = NULL;

	OV_INT	 		i;
	OV_UINT			size = 0;
	OV_UINT			dbflags = OV_DBOPT_VERBOSE;
	OV_STRING	        filename = NULL;
	OV_STRING		dumpfilename = NULL;
	OV_STRING		fbdStart = NULL;
	OV_RESULT		result;
	OV_BOOL			fbdDump = FALSE;
	OV_INT			fbdDumpMode = 0;
	OV_BOOL			extended = FALSE;
	OV_INSTPTR_ov_object	pobj;
	OV_INSTPTR_ov_object	pinst;
	FILE*			cfFile = NULL;
	char* configFile = NULL;
	char* configBasePath = NULL;
	OV_BOOL logfileSpecified = FALSE;
	OV_UINT			line = 0;
	OV_STRING		helper = NULL;
	OV_UINT			hlpindex = 0;
 	struct 			stat buffer;

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
		 *	create database option
		 */
		else if(!strcmp(argv[i], "-c") || !strcmp(argv[i], "--create")) {
			i++;
			if(i<argc) {
				size = strtoul(argv[i], NULL, 0);
			} else {
				goto HELP;
			}
		}
		/*
		 *	create database with configfile option
		 */
		else if(!strcmp(argv[i], "-cf") || !strcmp(argv[i], "--create-from-config")) {
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
				 * e.g. DBFILE db.ovd
				 *
				 * recognized options are:
				 * DBFILE		path to database file
				 * DBSIZE		size of database
				 * DBUTILLOG	logfile name, you may use stdout, stderr or (on NT-systems) ntlog
				 * other options are ignored as they are only used in the runtimeserver
				 */

				cfFile = fopen(configFile, "r");
				if(!cfFile)
				{
					perror("Could not open config file");
					return EXIT_FAILURE;
				}
				clearerr(cfFile);

				/*	get base path from path part of configFile	*/
#if OV_SYSTEM_NT
				for(j = strlen(configFile); (configFile[j] != '\\') && (j>0); j--);
#else
				for(j = strlen(configFile); (configFile[j] != '/') && (j>0); j--);
#endif
				if((j>0))
				{

					configBasePath = malloc(j+2);
					if(!configBasePath)
					{
						ov_logfile_error("Could not reserve memory for basePath. Aborting.");
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
						if(!filename || !*filename)
							return EXIT_FAILURE;
					}
					/*	DBUTILLOG	*/
					else if(strstr(startRead, "DBUTILLOG")==startRead)
					{
						if(logfileSpecified == FALSE)
						{
							temp = readValue(startRead);
							if(!temp || !*temp)
								return EXIT_FAILURE;
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
											temp = helper;
										}

									}
								if(Ov_Fail(ov_logfile_open(NULL, temp, "w"))) {
									ov_logfile_error("Could not open log file: \"%s\".\n", temp);
									return EXIT_FAILURE;
								}
							}
							free(temp);
							logfileSpecified = TRUE;
						}
					}
					/*	DBSIZE	*/
					else if(strstr(startRead, "DBSIZE")==startRead)
					{
						temp = readValue(startRead);
						if(!temp || !*temp)
							return EXIT_FAILURE;
						if(!size)
							size = strtoul(temp, NULL, 0);
						free(temp);
					}
					/*
					 * default: option unknown
					 */
					else
					{	/*	ignore	*/
						;
					}
				}
				/*	fgets returns 0 on error or eof. eof is ok, error aborts program	*/
				if(ferror(cfFile))
				{
					perror("Error reading config file");
					return EXIT_FAILURE;
				}
			}
		}
		/*
		 *	set logfile option
		 */
		else if(!strcmp(argv[i], "-l") || !strcmp(argv[i], "--logfile")) {
			i++;
			if(i<argc) {
				logfileSpecified = TRUE;
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
						fprintf(stderr, "Could not open log file: \"%s\".\n", argv[i]);
						return EXIT_FAILURE;
					}
				}
			} else {
				goto HELP;
			}
		}
		/*
		 *	display extended information
		 */
		else if(!strcmp(argv[i], "-e") || !strcmp(argv[i], "--extended")) {
			extended = TRUE;
		}
		/*
		 *	dump database
		 */
		else if(!strcmp(argv[i], "-d") || !strcmp(argv[i], "--dump")) {
			i++;
			if(i<argc) {
				dumpfilename = argv[i];
			} else {
				goto HELP;
			}
		}
		/*
		 *	change dump format to fbd
		 */
		else if(!strcmp(argv[i], "--fbd")) {
			i++;
			fbdDump = TRUE;
			if(i<argc) {
				if(*argv[i] != '-'){
					if(*argv[i] == '/'){
						fbdStart = argv[i];
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
			fbdDumpMode = 1;
		}
		/*
		 *	display version option
		 */
		else if(!strcmp(argv[i], "-v") || !strcmp(argv[i], "--version")) {
			printf("ACPLT/OV Database Utility " OV_VER_DBUTIL "\n");
			return EXIT_FAILURE;
		}

		/*
		 *	display help option
		 */
		else if(!strcmp(argv[i], "-h") || !strcmp(argv[i], "--help")) {
			HELP:		fprintf(stderr, "Usage: ov_dbutil [arguments]\n"
					"\n"
					"The following optional arguments are available:\n"
					"-f FILE, --file FILE            Set database filename (*.ovd)\n"
					"-c SIZE, --create SIZE          Create a new database\n"
					"--force-create                  Overwrite existing database file\n"
					"-cf CONFIGFILE, --create-from-config CONFIGFILE\n"
					"\tCreate a new database using the specified configfile.\n"
					"\tThe size of the new database must be specified in the configfile or with -c.\n"
					"-l LOGFILE, --logfile LOGFILE   Set logfile name, you may use stdout"
#if OV_SYSTEM_NT
					", stderr\n"
					"                                or ntlog (Windows NT only)\n"
#else
					" or stderr\n"
#endif
					"-d DUMPFILE, --dump DUMPFILE    Create database text dump\n"
					"--fbd [STARTNODE]               Dump as fbd. optionally STARTNODE can be specified\n"
					"--dump-expert                   Include /communication and /data folder in fbd dump\n"
					"-e, --extended                  Display extended database information\n"
					"-v, --version                   Display version information\n"
					"-h, --help                      Display this help message\n");
			return EXIT_FAILURE;
		} else {
			fprintf(stderr, "unknown option %s\n\n", argv[i]);
			goto HELP;
		}
	}

	if(!logfileSpecified){ // no logfile specified; log to stdout
		ov_logfile_logtostdout(NULL);
	}

	/*
	 *	create new or map existing database
	 */

	if(!filename || !*filename)
	{
		ov_logfile_error("No database specified.");
		goto HELP;
	}
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
			filename = helper;
		}
	}
	if(configFile && !size)
	{
		ov_logfile_error("Error: no size specified for database-creation.");
		goto HELP;
	}
	if(size) {
		//removing the eventually existing file
		if(stat(filename,&buffer)==0 && remove(filename)!=0){
			ov_logfile_error("Error: can not remove existing file \"%s\"", filename);
			return EXIT_FAILURE;
		}
		result = ov_database_create(filename, size, dbflags);
		if(Ov_Fail(result)) {
			ERRORMSG:	ov_logfile_error("Error: %s (error code 0x%x).",
					ov_result_getresulttext(result), result);
			return EXIT_FAILURE;
		}
	} else {
		result = ov_database_load(filename, 0, dbflags);
		if(Ov_Fail(result)) {
			goto ERRORMSG;
		}
		if (extended) {
			/*
			 *	Print extended database information
			 */
			passoc_ov_containment = &pdb->containment;
			passoc_ov_instantiation = &pdb->instantiation;

			pobj = Ov_GetFirstChild(ov_containment, &(pdb->ov));
			while (pobj) {
				if (!strcmp(pobj->v_identifier, "library")) {
					Ov_ForEachChild(ov_instantiation, (OV_INSTPTR_ov_class) pobj, pinst) {
						ov_logfile_info("Defined library %s, version %s", pinst->v_identifier, ((OV_INSTPTR_ov_library) pinst)->v_version);
					}
				}
				pobj = Ov_GetNextChild(ov_containment, pobj);
			}
		}
		if (dumpfilename) {
			/* Adding database path prefix */
			CONCATENATE_DATABASE_PATH(dumpfilename, help);
			/* Potential memory leak with help pointer, but it's ok */

			/*
			 *	Create a text dump file of the ov database
			 */
			if(fbdDump){
				result = ov_dumpFbd(dumpfilename, fbdStart, fbdDumpMode);
			} else {
				result = ov_dump(dumpfilename);
			}
			if(Ov_Fail(result)) {
				goto ERRORMSG;
			}

		}
	}
	/*
	 *	Print some information
	 */
	ov_logfile_info("Database size is %ld Byte.", ov_database_getsize());
	ov_logfile_info("Used storage size is %ld Byte.", ov_database_getused());
	ov_logfile_info("Free storage size is %ld Byte.", ov_database_getfree());
	ov_database_unload();

	/*
	 *	return
	 */
	return EXIT_SUCCESS;
}

/*	----------------------------------------------------------------------	*/

/*
 *	End of file
 */

