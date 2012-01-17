/*	----------------------------------------------------------------------	*/ 
/*
 *  Update slashes to match windows plattform
 */
void compatiblePath(char* string){
#if OV_SYSTEM_NT
	char* ph;
	ph = string;
	while(ph && (*ph)) {
		if(*ph == '/') {
			*ph = '\\';
		}
		ph++;
	}
#endif
}

/*	----------------------------------------------------------------------	*/
/*
 *   Search base libraries
 */
void fb_makmak_searbaselibs(const char *userLibPath, const char *lib, const char *customLibPath) {

	/* Local variables */
	char   help[512];
	char   curlib[512];
	FILE   *fd;
	char   *incFile;
	int    incFound;
	char   *ph;
	int    i;

	/* Init */
	strcpy(curlib, lib);

	/* OV library? -- already included */
	if( !strcmp(lib, "ov") ) {
		return;
	}

	/* Exclude fb.dll from standard includes
	if( !strcmp(lib, "fb") ) {
		return;
	}
	*/

	/* Zuviele Dateien? */
	if(anzAddLibs >= MAX_INCLUDED_FILES) {
		fprintf(stderr, "To many included files.\n");
		exit(0);
	}

	/* Bereits vermerkt? */
	for(i=0; i<anzAddLibs; i++) {
		if( !strcmp(curlib, libs[i]) ) {
			return;
		}
	}

	/* Search all includes */
	if(customLibPath == NULL){
		sprintf(help, "%s/%s/model/%s.ovm", userLibPath, curlib, curlib);
	}else{
		sprintf(help, "%s/model/%s.ovm", customLibPath, curlib);
	}
	compatiblePath(help);	

	fd = fopen(help, "r");
	/* If library exists - parse it */

	if(fd) {

		while( fgets(help, 511, fd) ) {

			incFound = 0;
			ph = strstr(help, "#include");
			/* locate # and 'include' in a line, # must come before 'include' */
			if(ph) {
				/* skip include */
				ph += 8;
				incFile = NULL;
				while(ph && *ph && incFound == 0) {
					switch(*ph) {
					case '<':
					case '"':
						ph++;
						if(!incFile) {
							incFile = ph;
						} else {
							*ph = '\0';
							incFound = 1;
						}
						break;
					case '.':
						*ph = '\0';
						incFound = 1;
						break;
					/* skip */					
					case '>':
					case ' ':
					case '\t':
					default:
						break;

					}

					if(incFound) {
						break;
					}

					ph++;
				}
				if(incFound) {
					ph = (char*)malloc(strlen(incFile) + 1);
					if(!ph) {
						fprintf(stderr, "Out of memory\n");
						exit(0);
					}
					strcpy(ph, incFile);
					fb_makmak_searbaselibs(userLibPath, ph, NULL);
					free(ph);
				}
			}
		}
		/* Close file */
		fclose(fd);
	}


	/* Base library? */
	if( strcmp(libname, curlib) ) {
		/* Add base library */
		ph = (char*)malloc(strlen(curlib) + 1);
		if(!ph) {
			fprintf(stderr, "Out of memory\n");
			exit(0);
		}
		strcpy(ph, curlib);
		libs[anzAddLibs++] = ph;
	}

}

/*	----------------------------------------------------------------------	*/
/*
 * Checks whether a file exists
 */
int fileExists(char* filename){
	FILE* file;
	file = fopen(filename, "r");
	if(file){
		fclose(file);
		return 1;
	}
	return 0;
}

/*	----------------------------------------------------------------------	*/
/*
 * Creates directory
 */
int acplt_mkdir(char* dir){
#if OV_SYSTEM_NT
			_mkdir(dir);
#else
			mkdir(dir, 0755);
#endif
	return 1;
}
