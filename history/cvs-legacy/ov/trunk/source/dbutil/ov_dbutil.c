/*
*   $Id: ov_dbutil.c,v 1.1 1999-07-19 15:02:09 dirk Exp $
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
*	03-Jul-1998 Dirk Meyer <dirk@plt.rwth-aachen.de>: File created.
*	20-Apr-1999 Dirk Meyer <dirk@plt.rwth-aachen.de>: Major revision.
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "libov/ov_database.h"
#include "libov/ov_result.h"
#include "libov/ov_logfile.h"

/*	----------------------------------------------------------------------	*/

/*
*   Global variables: table of static libraries
*/

#if OV_STATIC_LIBRARIES
Ov_BeginStaticLibraryTable
Ov_EndStaticLibraryTable;
#endif

/*	----------------------------------------------------------------------	*/

/*
*	Main program
*/
int main(int argc, char **argv) {
	/*
	*	local variables
	*/
	OV_UINT 	i;
	OV_UINT		size = 0;
	OV_STRING	filename = "database.ovd";
	OV_RESULT	result;
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
						fprintf(stderr, "Could not open log file: \"%s\".\n", argv[i]);
						return EXIT_FAILURE;
					}
				}
			} else {
				goto HELP;
			}
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
				"-l LOGFILE, --logfile LOGFILE   Set logfile name, you may use stdout"
#if OV_SYSTEM_NT
				", stderr\n"
				"                                or ntlog (Windows NT only)\n"
#else
				" or stderr\n"
#endif
				"-v, --version                   Display version information\n"
				"-h, --help                      Display this help message\n");
			return EXIT_FAILURE;
		} else {
			goto HELP;
		}
	}
	/*
	*	create new or map existing database
	*/
	if(size) {
		ov_logfile_info("Creating database \"%s\"...", filename);
		result = ov_database_create(filename, size);
		if(Ov_Fail(result)) {
ERRORMSG:	ov_logfile_error("Error: %s (error code 0x%x).",
				ov_result_getresulttext(result), result);
			return EXIT_FAILURE;
		}
		ov_logfile_info("Database created.");
	} else {
		ov_logfile_info("Mapping database \"%s\"...", filename);
		result = ov_database_map(filename);
		if(Ov_Fail(result)) {
			goto ERRORMSG;
		}
		ov_logfile_info("Database mapped.");
	}
	/*
	*	Print some information
	*/
	ov_logfile_info("Database size is %ld Byte.", ov_database_getsize());
	ov_logfile_info("Used storage size is %ld Byte.", ov_database_getused());
	ov_logfile_info("Free storage size is %ld Byte.", ov_database_getfree());
	ov_logfile_info("Unmapping database \"%s\"...", filename);
	ov_database_unmap();
	ov_logfile_info("Database unmapped.");
	/*
	*	return
	*/
	return EXIT_SUCCESS;
}

/*	----------------------------------------------------------------------	*/

/*
*	End of file
*/

