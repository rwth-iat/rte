#define __MT__	/* Borland: we go multi-threaded... */

#include <ctype.h>
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*	----------------------------------------------------------------------	*/

/*
*	Open the service control manager
*/
static SC_HANDLE ntservice_opencontrolmanager(void) {
	/*
	*	local variables
	*/
	SC_HANDLE	manager;
	/*
	*	open service control manager
	*/
	manager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if(!manager) {
		fprintf(stderr, "Unable to open the Service Control Manager.\n"
			"Do you have appropriate privileges?\n");
	}
	return manager;
}

/*	----------------------------------------------------------------------	*/

/*
*	Install the service (registry)
*/
static int ntservice_install(
	unsigned int	numargs,
	char		**args
) {
	/*
	*	local variables
	*/
	SC_HANDLE	service;
	SC_HANDLE	manager;
	char*		exepath;
	char*		depend;
	unsigned int	size;
	unsigned int 	i;
	/*
	*	open the service control manager
	*/
	if (numargs > 3) {
		manager = ntservice_opencontrolmanager();
		if(manager) {
			/*
			*	create exe path
			*/
			size = strlen(args[3])+numargs-3;
			for(i=4; i<numargs; i++) {
				size += strlen(args[i]);
			}
			exepath = malloc(size);
			if(exepath) {
				strcpy(exepath, args[3]);
				for(i=4; i<numargs; i++) {
					strcat(exepath, " ");
					strcat(exepath, args[i]);
				}
				size = strlen(args[2])+2;
				depend = malloc(size);
				strcpy(depend, args[2]);
				depend[size-1] = 0;
				depend[size-2] = 0;
				for (i=0;i<size-2; i++) {
					if (depend[i] == ';') depend[i]=0;
				}
				/*
				*	create the service
				*/
				service = CreateService(manager, args[0], args[1],
					SERVICE_ALL_ACCESS, SERVICE_WIN32_OWN_PROCESS,
					SERVICE_AUTO_START, SERVICE_ERROR_NORMAL, exepath, NULL, NULL,
					depend, NULL, NULL);
				/*
				*	fine, check the results and clean up
				*/
				free(exepath);
				free(depend);
				CloseServiceHandle(manager);
				if(service) {
					printf("Service successfully created.\n");
					CloseServiceHandle(service);
					return -1;
				}
				fprintf(stderr, "Could not create service.\n");
				return -1;
			}
			fprintf(stderr, "Unable to allocate memory.\n");
			CloseServiceHandle(manager);
		}
	}
	return -1;
}

/*	----------------------------------------------------------------------	*/

/*
*	Uninstall the service (registry)
*/
static int ntservice_uninstall(char* servicename) {
	/*
	*	local variables
	*/
	SC_HANDLE	service;
	SC_HANDLE	manager;
	/*
	*	open the service control manager
	*/
	manager = ntservice_opencontrolmanager();
	if(manager) {
		/*
		*	open the service
		*/
		service = OpenService(manager, servicename, SERVICE_ALL_ACCESS);
		if(service) {
			if(DeleteService(service)) {
				printf("Service successfully deleted.\n");
				CloseServiceHandle(service);
				CloseServiceHandle(manager);
				return 0;
			}
			printf("Could not delete service.\n");
			CloseServiceHandle(service);
			CloseServiceHandle(manager);
			return -1;
		}
		CloseServiceHandle(manager);
		fprintf(stderr, "Could not open service.\n");
	}
	return -1;
}

/*	----------------------------------------------------------------------	*/

/*
*	the main function
*/
int main(int argc, char **argv) {

	/*
	*	test if "install" or "uninstall" option
	*/
	if(argc > 2) {
		if(!strcmp(argv[1], "install")) {
			return ntservice_install(argc-2, &argv[2]);
		}
		if((argc == 3) && !strcmp(argv[1], "uninstall")) {
			return ntservice_uninstall(argv[2]);
		}
	}
	fprintf(stderr, "Usage: %s install \"<Service Name>\" \"<Display Name>\" \"<Dependency;Dependency;...>\" <fullpath\\filename> [arguments]\n", argv[0]);
	fprintf(stderr, "or   : %s uninstall \"<Service Name>\"\n", argv[0]);
	return 0;
}

/*	----------------------------------------------------------------------	*/

/*
*	End of file
*/

