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
 *	12-Dec-2018 Lars Nothdurft <l.nothdurft@plt.rwth-aachen.de>:
 *							moved options parsing to shared function.
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
#include "libov/ov_options.h"
#include "ov_dbutil.h"

/*
 *   Global variables: table of static libraries
 */

#if OV_STATIC_LIBRARIES
Ov_BeginStaticLibraryTable
Ov_EndStaticLibraryTable;
#endif

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

static void ov_dbutil_usage(void){
	fprintf(stderr, "Usage: ov_dbutil [arguments]\n"
			"\n"
			"The following optional arguments are available:\n"
			"-f FILE, --file FILE            Set database filename (*.ovd)\n"
			"-c SIZE, --create SIZE          Create a new database\n"
			"--force-create                  Overwrite existing database file\n"
			"-cf CONFIGFILE, --config CONFIGFILE\n"
			"                                Create a new database using the specified\n"
			"                                configfile. The size of the new database must\n"
			"                                be specified in the configfile or with -c.\n"
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
			"--print-pointer                 Print pointer in database\n"
			"-v, --version                   Display version information\n"
			"-h, --help                      Display this help message\n");
}

static void ov_dbutil_version(void){
	fprintf(stderr, "ACPLT/OV Database Utility " OV_VER_DBUTIL "\n");
}

/*	----------------------------------------------------------------------	*/

/*
 *	Main program
 */
int main(int argc, char **argv) {
	/*
	 *	local variables
	 */

	OV_RESULT		result;
	OV_INSTPTR_ov_object	pobj;
	OV_INSTPTR_ov_object	pinst;
	OV_STRING		help = NULL;
	OV_STRING		helper = NULL;
	OV_UINT			hlpindex = 0;
	struct 			stat buffer;

	ov_options		opts;

	/*
	 *	if we are debugging, log to stderr (if not
	 *	specified by the command line options)
	 */
#ifdef OV_DEBUG
	ov_logfile_logtostderr(NULL);
#endif

	ov_options_init(&opts);
	opts.usage = &ov_dbutil_usage;
	opts.version = &ov_dbutil_version;
	opts.ctx = ov_dbutil;

	/*
	 *	parse command line arguments
	 */

	if(ov_readConfigFromArgs(&opts, argc, argv)){
		ov_logfile_free();
		ov_options_free(&opts);
		return EXIT_FAILURE;
	}

	if(ov_readArgs(&opts, argc, argv)){
		ov_logfile_free();
		ov_options_free(&opts);
		return EXIT_FAILURE;
	}

	/* disable nomap and nofile options */
	opts.dbflags &= ~(OV_DBOPT_NOMAP | OV_DBOPT_NOFILE);

	if(!opts.logfile){ // no logfile specified; log to stdout
		ov_logfile_logtostdout(NULL);
	}

#if TLSF_HEAP
#if !(USE_MMAP || USE_SBRK || USE_VIRTALLOC)
	if(!opts.poolsize){
		opts.poolsize = DBUTIL_DEFAULT_HEAP_SIZE;
	}
#endif
	ov_initHeap(opts.poolsize);
#endif

	/*
	 *	create new or map existing database
	 */

	if(!opts.dbFilename || !*opts.dbFilename)
	{
		ov_logfile_error("No database specified.");
		ov_dbutil_usage();
		ov_logfile_free();
		ov_options_free(&opts);
		ov_destroyHeap();
		return EXIT_FAILURE;
	}
#if OV_SYSTEM_UNIX
	if(!(opts.dbFilename[0]=='/'))
#else
	if(!(opts.dbFilename[1]==':'|| opts.dbFilename[0]=='\\'))
#endif
	{/*	relative path --> prepend basePath	*/
		if(opts.configBasePath && *opts.configBasePath)
		{
			hlpindex = strlen(opts.configBasePath);
			helper = calloc(hlpindex+strlen(opts.dbFilename)+2, sizeof(char));
			if(!helper)
			{
				ov_logfile_error("Could not reserve memory for filename path. Aborting.");
				ov_logfile_free();
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
	if(opts.configFile && !opts.dbSize)
	{
		ov_logfile_error("Error: no size specified for database-creation.");
		ov_dbutil_usage();
		ov_logfile_free();
		ov_options_free(&opts);
		ov_destroyHeap();
		return EXIT_FAILURE;
	}
	if(opts.dbSize) {
		//removing the eventually existing file
		if(stat(opts.dbFilename,&buffer)==0 && remove(opts.dbFilename)!=0){
			ov_logfile_error("Error: can not remove existing file \"%s\"", opts.dbFilename);
			ov_logfile_free();
			ov_options_free(&opts);
			ov_destroyHeap();
			return EXIT_FAILURE;
		}
		result = ov_database_create(opts.dbFilename, opts.dbSize, opts.dbflags);
		if(Ov_Fail(result)) {
			ERRORMSG:	ov_logfile_error("Error: %s (error code 0x%x).",
					ov_result_getresulttext(result), result);
			ov_logfile_free();
			ov_options_free(&opts);
			ov_destroyHeap();
			return EXIT_FAILURE;
		}
	} else {
		result = ov_database_load(opts.dbFilename, 0, opts.dbflags);
		if(Ov_Fail(result)) {
			goto ERRORMSG;
		}
		if (opts.extended) {
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
		if (opts.printPtr) {
			ov_freelist_print();
		}
		if (opts.dumpfilename) {
			/* Adding database path prefix */
			CONCATENATE_DATABASE_PATH(opts.dumpfilename, help);
			/* Potential memory leak with help pointer, but it's ok */

			/*
			 *	Create a text dump file of the ov database
			 */
			if(opts.fbdDump){
				result = ov_dumpFbd(opts.dumpfilename, opts.fbdStart, opts.fbdDumpMode);
			} else {
				result = ov_dump(opts.dumpfilename);
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

	ov_logfile_free();
	ov_options_free(&opts);
	ov_destroyHeap();

	/*
	 *	return
	 */
	return EXIT_SUCCESS;
}

/*	----------------------------------------------------------------------	*/

/*
 *	End of file
 */

