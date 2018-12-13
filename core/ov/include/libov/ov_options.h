#ifndef _OV_OPTIONS_H_
#define _OV_OPTIONS_H_

#define MAX_STARTUP_LIBRARIES 64

enum options_context{
	ov_runtimeserver,
	ov_dbutil
};

typedef struct struct_ov_options {

	enum options_context ctx;

	char*		dbFilename;
	char*		dbBackupFilename; // not malloc'ed
	char*		servername;
	char*		configFile; // not malloc'ed
	char*		configBasePath;
	char*		password;
	int			port;
	OV_UINT		dbSize;
	char*		libraries[MAX_STARTUP_LIBRARIES];
	int			libcount;
	char*		logfile;
	char*		commandline_options;
	OV_BOOL		activitylock;
	OV_BOOL		startup;
	OV_BOOL		exit;
	OV_BOOL		exec;
	char*		execIdent;
	char*		execClass;
	char*		execLib;
	OV_UINT		maxAllowedJitter;

	OV_UINT		ks_maxItemsPerRequest;
	OV_UINT		maxStringLength;
	OV_UINT		maxVectorLength;
	OV_UINT		maxNameLength;
	OV_UINT		maxHierarchyDepth;

	OV_UINT		dbflags;

#if OV_SYSTEM_RMOS
	OV_UINT		taskid;
	OV_BOOL		terminate;
#endif

	/*
	 * dbutil options
	 */
	char*		dumpfilename; // not malloced
	OV_BOOL		extended;
	OV_BOOL		fbdDump;
	char*		fbdStart;	// not malloced
	int			fbdDumpMode; // 1 = expert

	/*
	 * callback functions for usage and version
	 */
	void (*usage)(void);
	void (*version)(void);

} ov_options;

OV_DLLFNCEXPORT void ov_options_init(ov_options* opts);
OV_DLLFNCEXPORT void ov_options_free(ov_options* opts);
OV_DLLFNCEXPORT OV_RESULT ov_readConfigFile(ov_options* opts, char* configFile);
OV_DLLFNCEXPORT OV_RESULT ov_readConfigFromArgs(ov_options* opts, int argc, char** argv);
OV_DLLFNCEXPORT OV_RESULT ov_readArgs(ov_options* opts, int argc, char** argv);

#endif
