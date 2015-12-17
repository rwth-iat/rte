/*
 * servreg.c -- utility functions for self-registering or unregistering
 *              NT services. This can also be used for manual registration
 *              of a service from the command line. Typically, this module
 *              should be compiled into its own DLL and accompany a NT
 *              service executable. For the registration magic to work with
 *              installation programs, this DLL needs to have the same name
 *              as the service executable: for example, the portmap.exe
 *              service is accompanied by portmap.dll.
 *
 * (c) 1999 Harald Albrecht <harald@plt.rwth-aachen.de>
 * Chair of Process Control Engineering,
 * Aachen University of Technology, Germany.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must print or display the above
 *    copyright notice either during startup or must have a means for
 *    the user to view the copyright notice.
 * 3. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 4. Neither the name of the Chair of Process Control Engineering nor the
 *    name of the Aachen University of Technology may be used to endorse or
 *    promote products derived from this software without specific prior
 *    written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE CHAIR OF PROCESS CONTROL ENGINEERING
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE CHAIR OF PROCESS CONTROL
 * ENGINEERING BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <windows.h>

#include "servreg.h"
#include "servregids.h"


/* --------------------------------------------------------------------------
 * In some cases we need to access the resources from this DLL and thus need
 * its module handle.
 */
static HMODULE hDLL;

/* --------------------------------------------------------------------------
 * Load a string resource from the module's executable or set a default, if
 * the resource does not exists. The module which supplies the resources is
 * indicated by the hModule parameter.
 */
static void
LoadStringResource(HMODULE hModule, UINT uID, char *buffer, int max, char *def)
{
    if ( !LoadString(hModule, uID, buffer, max) ) {
        strncpy(buffer, def, max);
    }
    buffer[max - 1] = 0; /* make sure string is properly terminated */
} /* LoadStringResource */


/* --------------------------------------------------------------------------
 * Following is just a very simple dialog procedure, which is only capable
 * of doing nothing. It is used when showing the self-registering splash
 * screen. Oh, the only real thing it does is centering the dialog in the
 * middle of the desktop. So why do we need to do such things by hand instead
 * of the Windows "os" taking care of this? Yeah, big deal: "new technology".
 */
static BOOL CALLBACK
SplashDialogProc(HWND hDialog, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    if ( uMsg == WM_INITDIALOG ) {
        RECT desktop, dialog;

        /*
         * Center the dialog within the screen estate...
         */
        GetWindowRect(GetDesktopWindow(), &desktop);
        GetWindowRect(hDialog, &dialog);
        OffsetRect(&dialog, -dialog.left, -dialog.top);
        SetWindowPos(hDialog, HWND_TOP,
                     (desktop.right - dialog.right) / 2,
                     (desktop.bottom - dialog.bottom) / 2,
                     dialog.right, dialog.bottom,
                     SWP_NOSIZE);

        return 0; /* set focus to ourself (okay...) */
    }
    return 0; /* didn't process message */
} /* SplashDialogProc */


/* --------------------------------------------------------------------------
 * Monitor the state of a NT service and wait for it to reach a particular
 * destination state. This uses the timeouts specified by the service itself
 * for monitoring progress.
 *
 * Parameters:
 *   hService           handle of service to wait for
 *   destination        destination mode of service to wait for
 *   hSplashDialog      handle of dialog which will receive status info
 *   templID            string resource id of info message
 *   service            clear-text name of service
 */
#define POLLINGDELAY 250
#define POLLINGMULTPLY (1000/POLLINGDELAY)

static DWORD
WaitForService(SC_HANDLE hService, DWORD destination,
               HWND hSplashDialog, 
               HMODULE hModule, UINT templID, char *service)
{
    SERVICE_STATUS serviceStatus;
    MSG            msg;
    char           templ[512];
    char           buff[512];
    HWND           text;
    int            counter;
    DWORD          checkpoint;
    DWORD          state;

    /*
     * Update the splash screen... that is, the single message line
     * (which is hard-coded to have a control id of 0).
     */
    if ( hSplashDialog ) {
        LoadStringResource(hModule, templID, templ, sizeof(templ),
                           destination == SERVICE_RUNNING ?
                             "Starting the %s service..." :
                             "Stopping the %s service...");
        wsprintf(buff, templ, service);
        text = GetDlgItem(hSplashDialog, 0);
        SetWindowText(text, buff);
    }

    /*
     * Wait for the service to enter its destination state. Check for
     * this every quarter of a second and redraw the splash screen, if
     * necessary. If the service doesn't reach its destination state,
     * then also return.
     */
    if ( !QueryServiceStatus(hService, &serviceStatus) ) {
	return SERVICE_STOPPED;
    }
    for ( ;; ) {
	switch ( state = serviceStatus.dwCurrentState ) {
	case SERVICE_START_PENDING:
	case SERVICE_STOP_PENDING:
	case SERVICE_CONTINUE_PENDING:
	case SERVICE_PAUSE_PENDING:
	    /* calculate max number of rounds to wait for progress */
	    counter = (serviceStatus.dwWaitHint / POLLINGDELAY) + 1;
	    checkpoint = serviceStatus.dwCheckPoint;
	    break;
	default:
	    counter = 1 * POLLINGMULTPLY; /* wait at least 1 second */
	    checkpoint = 0;
	}
	for ( ; counter >= 0; --counter ) {
	    Sleep(POLLINGDELAY);
	    if ( hSplashDialog ) {
		while ( PeekMessage(&msg, hSplashDialog, 0, 0, PM_REMOVE) ) {
		    DispatchMessage(&msg);
		}
	    }
	    if ( !QueryServiceStatus(hService, &serviceStatus) ) {
		return SERVICE_STOPPED;
	    }
	    if ( (serviceStatus.dwCurrentState == destination) ||
		 (serviceStatus.dwCurrentState == SERVICE_STOPPED) ) {
		return serviceStatus.dwCurrentState;
	    }
	    if ( (serviceStatus.dwCheckPoint != checkpoint) ||
                 (serviceStatus.dwCurrentState != state) ) {
		break;
	    }
	}
	if ( !counter ) {
	    /* well -- no progress made; failure. */
	    return SERVICE_STOPPED;
	}
	/* checkpoint value has changed, so take another round... */
    } /* forever... */
} /* WaitForStoppingService */


/* --------------------------------------------------------------------------
 * Register the new service. This is not as easy as it looks at the first
 * sight because there are many obstacles which can lay in our way. For
 * instance, we need to cope with the service being already installed and up
 * and running, but the user wants to replace the service...
 *
 * Parameters:
 *   serviceId          service name
 *   serviceDisplay     display name of service
 *   dependencies       String of services this service is depenent of. Each
 *                      item is zero-terminated with the list being zero-
 *                      terminated too (thus there must be two zero characters
 *                      at the end of the string).
 */
int
RegisterService(HMODULE hModule,
                char *serviceId, char *serviceDisplay, char *dependencies,
                NtServiceInstallationMode instMode)
{
    SC_HANDLE            hSCManager, hService;
    HWND                 hSplashDialog;
    char                 modulePath[MAX_PATH];
    char                *pExt;
    char                 title[128];
    char                 msg[512];
    BOOL                 ok;
    ENUM_SERVICE_STATUS *serviceStates = 0;
    DWORD                serviceStateSize;
    DWORD                serviceStateCount = 0;
    int                  i;
    int                  rebootPending;

    /*
     * We'll need the title for some dialogs lateron, so we'll just load
     * it now from the string resources.
     */
    LoadStringResource(hModule, IDS_INSTALLTITLE, title, sizeof(title),
                       "NT Service Installation");

    /*
     * Determine where this service executable is located in the file
     * system. This is used later to install the service. If this should
     * not be possible, then abort the registration process right here.
     * Note that hModule always indentifies this DLL module but never the
     * NT service executable.
     *
     * Remove the old file extension and always set the default extension
     * for a service executable -- that is, ".exe". This is necessary,
     * because the hModule handle always points to the selfregistration
     * DLL but not to the service executable. From this you can see, that
     * both files always need to have the same basename.
     */
    if ( GetModuleFileName(hModule, 
                           modulePath, sizeof(modulePath) - 1) == 0 ) {
        LoadStringResource(hModule, IDS_NOPATH, msg, sizeof(msg),
                           "Can not determine the path of the "
                           "service executable.");
        MessageBox(0, msg, title, MB_OK | MB_ICONERROR);
        return 10; /* checkpoint error code */
    }
    pExt = strrchr(modulePath, '.');
    if ( pExt ) {
	*pExt = 0;
    }
    strncat(modulePath, SERVICEFILENAMEEXTENSION, sizeof(modulePath));

    /*
     * Try to open the service control manager with full access, because
     * we need to configure and start/stop services. If something fails at
     * this stage, then we bail out with an error dialog box. In case of
     * insufficent access, we'll be a little bit more precise and say that
     * the user needs more privileges before aborting registration.
     */
    hSCManager = OpenSCManager(0, 0, SC_MANAGER_ALL_ACCESS);
    if ( hSCManager == 0 ) {
        DWORD err;

        if ( GetLastError() == ERROR_ACCESS_DENIED ) {
            LoadStringResource(hModule, IDS_INSUFFICENTACCESS, 
                               msg, sizeof(msg),
                               "You have insufficient privileges to\n"
                               "create and configure the new NT service.");
        } else {
            LoadStringResource(hModule, IDS_NOINSTALL, msg, sizeof(msg),
                               "The NT service could not be installed.");
        }
        MessageBox(0, msg, title, MB_OK | MB_ICONERROR);
        return 20; /* checkpoint error code */
    }

    /*
     * Now try to install the new service with the service control manager.
     * We have to cope with either the service already been installed on
     * this computer or the service to be newly created on this system. So
     * first find out whether this service already exists...
     */
    rebootPending = GlobalFindAtom(PENDINGREBOOTATOMNAME) != 0;
    hService = OpenService(hSCManager, serviceId, SERVICE_ALL_ACCESS);
    if ( hService != 0 ) {
        SERVICE_STATUS serviceStatus;

        if ( instMode != NtServiceAutomaticInstallation ) {
            /*
             * Oops. This service was already installed in the past.
             */
            LoadStringResource(hModule, IDS_REALLYINSTALL, msg, sizeof(msg),
                               "The service is already installed on this "
                               "computer\nDo you want to make this version "
                               "the current service?");
            if ( MessageBox(0, msg, title, MB_YESNO | MB_ICONINFORMATION)
                 != IDYES ) {
                CloseServiceHandle(hSCManager);
                CloseServiceHandle(hService);
                return 21; /* checkpoint error code */
            }
        }

        /*
    	 * Step One:
         * This is now getting really hairy... If we're in automatic install
    	 * mode, then we're not allowed to stop this service, if an old
    	 * version is currently running. In this case we are only allowed to
    	 * register it and mark a pending reboot.
    	 * In case we're in manual mode, then we first stop all dependent
    	 * services (of this service) and eventually this service. We store
    	 * this information, so we can start them lateron after we've
    	 * registered the service.
         */
        if ( instMode != NtServiceAutomaticInstallation ) {
            if ( (!EnumDependentServices(hService, SERVICE_ACTIVE,
                                        0, 0, &serviceStateSize,
                                        &serviceStateCount)
                 && (GetLastError() != ERROR_MORE_DATA) )
                 || (serviceStateSize &&
                     (serviceStates = malloc(serviceStateSize)) == 0) ) {
	        	/*
		         * Bail out if there's not enough memory to store the
        		 * dependencies.
	        	 */
                CloseServiceHandle(hSCManager);
                CloseServiceHandle(hService);
                LoadStringResource(hModule, IDS_DEPENDENTRETRERR,
                                   msg, sizeof(msg),
                                   "Can't retrieve list of services which "
                                   "depend on this service.");
                MessageBox(0, msg, title, MB_OK | MB_ICONERROR);
                return 30; /* checkpoint error code */
            }
        }
        if ( serviceStates ) {
            EnumDependentServices(hService, SERVICE_ACTIVE,
                                        serviceStates, serviceStateSize,
                                        &serviceStateSize,
                                        &serviceStateCount);

            hSplashDialog = CreateDialog(hModule,
                                         MAKEINTRESOURCE(DLG_STOPSERVICE),
                                         GetDesktopWindow(),
                                         SplashDialogProc);

            for ( i = 0; i < serviceStateCount; ++i ) {
                /*
                 * Stop a service. Do not complain if this doesn't work
                 * yet. Try to shut down ALL dependent services. During the
                 * shutdown process we need to wait for each service to
                 * stop individually, or otherwise dependecies might bite us.
                 */
                SC_HANDLE      hStopService;
                SERVICE_STATUS stopServiceStatus;

                /*
                 * Get access to the service to be stopped.
                 */
                hStopService = OpenService(hSCManager,
                                           serviceStates[i].lpServiceName,
                                           SERVICE_ALL_ACCESS);
                if ( hStopService ) {
                    if ( serviceStates[i].ServiceStatus.dwCurrentState
                         == SERVICE_RUNNING ) {
                        ControlService(hStopService, SERVICE_CONTROL_STOP,
                                       &stopServiceStatus);
                    }
                    WaitForService(hStopService,
                                   SERVICE_STOPPED,
                                   hSplashDialog, hModule,
                                   IDS_STOPSERVICE,
                                   serviceStates[i].lpDisplayName);
                    CloseServiceHandle(hStopService);
                }
            }
            if ( hSplashDialog ) {
                DestroyWindow(hSplashDialog);
            }
        }
	
	/*
	 * Okay. We've nearly completed the first step. If we're in auto
	 * install mode, then check the current status of the (old) service.
	 * If it's running, then there's a reboot pending due to file locking
	 * issues, so announce the reboot and don't touch the running service,
	 * only register it. If we're in manual mode, then we're stopping the
	 * service if it's currently running.
	 */
        serviceStatus.dwCurrentState = SERVICE_STOPPED; /*justincase...*/
        QueryServiceStatus(hService, &serviceStatus);
	if ( instMode == NtServiceAutomaticInstallation ) {
	    if ( serviceStatus.dwCurrentState != SERVICE_STOPPED ) {
		rebootPending = 1;
		GlobalAddAtom(PENDINGREBOOTATOMNAME);
	    }
	} else if ( serviceStatus.dwCurrentState == SERVICE_RUNNING ) {
            /*
             * Finally shut down the service we want to replace. Phew, quite
             * some things needed to be moved around before we reached this
             * point.
             */
            hSplashDialog = CreateDialog(hModule,
                                         MAKEINTRESOURCE(DLG_STOPSERVICE),
                                         GetDesktopWindow(),
                                         SplashDialogProc);

            ok = ControlService(hService, SERVICE_CONTROL_STOP,
                                &serviceStatus);
            if ( ok ) {
                if ( WaitForService(hService, SERVICE_STOPPED, hSplashDialog,
                                    hModule, IDS_STOPSERVICE, serviceDisplay)
                     != SERVICE_STOPPED ) {
                    ok = FALSE;
                    SetLastError(0); /* FIXME */
                }
            }
            if ( hSplashDialog ) {
                DestroyWindow(hSplashDialog);
            }
            if ( !ok ) {
                free(serviceStates);
                switch ( GetLastError() ) {
                case ERROR_SERVICE_NOT_ACTIVE:
                    /* This is just fine, the service wasn't running... */
                    break;
                case ERROR_DEPENDENT_SERVICES_RUNNING:
                    LoadStringResource(hModule, IDS_DEPENDENTRETRERR,
                                       msg, sizeof(msg),
                                       "The old NT service can not be shut down\n"
                                       "because dependent services are still running.");
                    MessageBox(0, msg, title, MB_OK | MB_ICONERROR);
                    CloseServiceHandle(hSCManager);
                    CloseServiceHandle(hService);
                    return 31; /* checkpoint error code */
                default:
                    LoadStringResource(hModule, IDS_SHUTERR,
                                       msg, sizeof(msg),
                                       "The old NT service could not be shut down.\n"
                                       "This new service is not installed.");
                    MessageBox(0, msg, title, MB_OK | MB_ICONERROR);
                    CloseServiceHandle(hSCManager);
                    CloseServiceHandle(hService);
                    return 32; /* checkpoint error code */
                }
                /* Fall through and continue */
            }
        }

        /*
         * Now set up the new service information. This mainly involves
         * (re)setting the service type, start mode, error reporting, and
         * -- most important -- the path of the service executable.
         */
        if ( !ChangeServiceConfig(hService,
                                  SERVICE_WIN32_OWN_PROCESS,
                                  SERVICE_AUTO_START,
                                  SERVICE_ERROR_NORMAL,
                                  modulePath,
                                  0, /* no load ordering group name */
                                  0, /* not interested in tag id */
                                  dependencies, /* dependencies see above*/
                                  0, /* default service start account */
                                  0, /* no passwords */
                                  serviceDisplay) ) {
            free(serviceStates);
            LoadStringResource(hModule, IDS_CANTREGISTER, msg, sizeof(msg),
                               "The NT service couldn't be reconfigured.\n"
                               "The service configuration wasn't updated.");
            MessageBox(0, msg, title, MB_OK | MB_ICONERROR);
            CloseServiceHandle(hSCManager);
            CloseServiceHandle(hService);
            return 40; /* checkpoint error code */
        }
    } else {
        /*
         * We need to create a new service with the service control manager,
         * because this service hasn't yet been registered.
         */
        hService = CreateService(hSCManager,
                                 serviceId,
                                 serviceDisplay,
                                 SERVICE_ALL_ACCESS,
                                 SERVICE_WIN32_OWN_PROCESS,
                                 SERVICE_AUTO_START,
                                 SERVICE_ERROR_NORMAL,
                                 modulePath,
                                 0, /* no load ordering group name */
                                 0, /* not interested in tag id */
                                 dependencies, /* dependencies see above*/
                                 0, /* default service start account */
                                 0 /* no passwords */ );

        if ( hService == 0 ) {
            CloseServiceHandle(hSCManager);
            CloseServiceHandle(hService);
            LoadStringResource(hModule, IDS_CANTREGISTER, msg, sizeof(msg),
                               "Can not register the NT service.");
            MessageBox(0, msg, title, MB_OK | MB_ICONERROR);
            return 50; /* checkpoint error code */
        }
    }

    /*
     * Now try to start the new service. If this fails, then pop up an
     * error message box. As the startup might take some time, we display
     * a splash screen during startup.
     *
     * Do not spin up the service if a reboot is pending.
     */
    if ( !rebootPending ) {
        hSplashDialog = CreateDialog(hModule,
                                     MAKEINTRESOURCE(DLG_STARTSERVICE),
                                     GetDesktopWindow(),
                                     SplashDialogProc);
        ok = StartService(hService, 0, 0);
        if ( ok ) {
            if ( WaitForService(hService, SERVICE_RUNNING, hSplashDialog,
                                hModule, IDS_STARTSERVICE, serviceDisplay)
                 != SERVICE_RUNNING ) {
                ok = FALSE;
            }
        }
        if ( !ok ) {
            if ( hSplashDialog ) {
                DestroyWindow(hSplashDialog);
            }
            CloseServiceHandle(hSCManager);
            CloseServiceHandle(hService);
            free(serviceStates);
            LoadStringResource(hModule, IDS_CANTSTART, msg, sizeof(msg),
                               "Can not start the NT service.");
            MessageBox(0, msg, title, MB_OK | MB_ICONERROR);
            return 60; /* checkpoint error code */
        }

        /*
         * Also restart all dependent services which were running before the
         * service was reinstalled. In case the service was newly installed,
         * then skip this section.
         */
        if ( serviceStates ) {
            SC_HANDLE             hStartService;
            QUERY_SERVICE_CONFIG *serviceConfig;
            DWORD                 serviceConfigSize;

            for ( i = serviceStateCount - 1; i >= 0; --i ) {
                /*
                 * Only restart services which were RUNNING in the past. We
                 * don't consider starting or stopping services, only running
                 * ones. In addition, the services must have been marked as
                 * auto starting.
                 */
                if ( serviceStates[i].ServiceStatus.dwCurrentState
                     != SERVICE_RUNNING ) {
                    continue;
                }
                hStartService = OpenService(hSCManager,
                                            serviceStates[i].lpServiceName,
                                            SERVICE_ALL_ACCESS);
                if ( hStartService ) {
                    QueryServiceConfig(hStartService,
                                       0, 0, &serviceConfigSize);
                    serviceConfig = malloc(serviceConfigSize);
                    if ( serviceConfig ) {
                        if ( !QueryServiceConfig(hStartService,
                                                 serviceConfig,
						                         serviceConfigSize,
                                                 &serviceConfigSize) ||
                             (serviceConfig->dwStartType
			                   != SERVICE_AUTO_START) ) {
                            free(serviceConfig);
                            continue; /* ignore this one... */
                        }
                        free(serviceConfig);
                        StartService(hStartService, 0, 0);
                        WaitForService(hStartService, SERVICE_RUNNING,
				                       hSplashDialog,
                                       hModule, IDS_STARTSERVICE,
                                       serviceStates[i].lpDisplayName);
                    }
                    CloseServiceHandle(hStartService);
                }
            }
            free(serviceStates);
        }
        /*
         * We're finally done. Pop down the splash screen.
         */
        if ( hSplashDialog ) {
            DestroyWindow(hSplashDialog);
        }
    }

    /*
     * Clean up any remaining bits.
     */
    CloseServiceHandle(hSCManager);
    CloseServiceHandle(hService);
    return 0; /* done and succeeded. */
} /* RegisterService */


/* --------------------------------------------------------------------------
 * Unregister this service.
 *
 * Parameters:
 *   serviceId          service name
 */
int
UnregisterService(HMODULE hModule, char *serviceId,
		          NtServiceInstallationMode instMode)
{
    SC_HANDLE             hSCManager, hService;
    HWND                  hSplashDialog;
    char                  modulePath[2048];
    char                  title[128];
    char                  msg[512];
    char                 *pExt;
    BOOL                  ok;
    QUERY_SERVICE_CONFIG *serviceConfig;
    DWORD                 serviceConfigSize;
    ENUM_SERVICE_STATUS *serviceStates = 0;
    DWORD                serviceStateSize;
    DWORD                serviceStateCount = 0;

    /*
     * We'll need the title for some dialogs lateron, so we'll just load
     * it now from the string resources.
     */
    LoadStringResource(hModule, IDS_DEINSTALLTITLE, title, sizeof(title),
                       "NT Service Deinstallation");
    /*
     * Determine where this service executable is located in the file
     * system. This is later used to install the service.
     */
    if ( GetModuleFileName(hModule,
			   modulePath, sizeof(modulePath) - 1) == 0 ) {
        LoadStringResource(hModule, IDS_NOPATH, msg, sizeof(msg),
                           "Can not determine the path of the "
                           "service executable.");
        MessageBox(0, msg, title, MB_OK | MB_ICONERROR);
        return 101; /* checkpoint error code */
    }
    pExt = strrchr(modulePath, '.');
    if ( pExt ) {
	*pExt = 0;
    }
    strncat(modulePath, SERVICEFILENAMEEXTENSION, sizeof(modulePath));

    /*
     * Try to open the service control manager with full access, because
     * we need to configure and start/stop services. If something fails at
     * this stage, then we bail out with an error dialog box.
     */
    hSCManager = OpenSCManager(0, 0, SC_MANAGER_ALL_ACCESS);
    if ( hSCManager == 0 ) {
        DWORD err;

        if ( GetLastError() == ERROR_ACCESS_DENIED ) {
            LoadStringResource(hModule, IDS_INSUFFICENTDEINSTACCESS,
                               msg, sizeof(msg),
                               "You have insufficient privileges to\n"
                               "deinstall the NT service.");
            MessageBox(0, msg, title, MB_OK | MB_ICONERROR);
        } else {
            LoadStringResource(hModule, IDS_NODEINSTALL, msg, sizeof(msg),
                               "The NT service could not be deinstalled.");
            MessageBox(0, msg, title, MB_OK | MB_ICONERROR);
        }
        return 102; /* checkpoint error code */
    }
    /*
     * Now try to open the service. If it can't be opened because it does
     * not exist, then silently ignore this faux pas. Otherwise bark loud.
     */
    hService = OpenService(hSCManager, serviceId, SERVICE_ALL_ACCESS);
    if ( hService == 0 ) {
        CloseServiceHandle(hSCManager);
        if ( GetLastError() == ERROR_SERVICE_DOES_NOT_EXIST	) {
            return 0;
        }
        LoadStringResource(hModule, IDS_NODEINSTALL, msg, sizeof(msg),
                           "The NT service could not be deinstalled.");
        MessageBox(0, msg, title, MB_OK | MB_ICONERROR);
        return 110; /* checkpoint error code */
    }
    /*
     * Now shut down the service and remove it. But only do this, if the
     * right executable is currently registered. If not, then we'll silently
     * ignore this and do not touch the service's configuration.
     */
    if ( !QueryServiceConfig(hService, 0, 0,
                             &serviceConfigSize) &&
         (GetLastError() != ERROR_INSUFFICIENT_BUFFER) ) {
        CloseServiceHandle(hSCManager);
        CloseServiceHandle(hService);
        LoadStringResource(hModule, IDS_NODEINSTALL, msg, sizeof(msg),
                           "The NT service could not be deinstalled.");
        MessageBox(0, msg, title, MB_OK | MB_ICONERROR);
        return 120; /* checkpoint error code */
    }
    serviceConfig = malloc(serviceConfigSize);
    if ( !serviceConfig ||
         !QueryServiceConfig(hService, serviceConfig, serviceConfigSize,
                             &serviceConfigSize)) {
        CloseServiceHandle(hSCManager);
        CloseServiceHandle(hService);
        LoadStringResource(hModule, IDS_NODEINSTALL, msg, sizeof(msg),
                           "The NT service could not be deinstalled.");
        MessageBox(0, msg, title, MB_OK | MB_ICONERROR);
        return 121; /* checkpoint error code */
    }
    /*
     * Shut down and remove the sucker.
     */
    if ( strcmpi(serviceConfig->lpBinaryPathName, modulePath) == 0 ) {
        SERVICE_STATUS serviceStatus;

        free(serviceConfig);

        if ( (!EnumDependentServices(hService, 
                                     SERVICE_ACTIVE | SERVICE_INACTIVE,
                                     0, 0, &serviceStateSize,
                                     &serviceStateCount)
              && (GetLastError() != ERROR_MORE_DATA) )
             || (serviceStateSize &&
                 (serviceStates = malloc(serviceStateSize)) == 0) ) {
	    /*
	     * Bail out if there's not enough memory to store the
             * dependencies.
	     */
            CloseServiceHandle(hSCManager);
            CloseServiceHandle(hService);
            LoadStringResource(hModule, IDS_DEPENDENTRETRERR,
                               msg, sizeof(msg),
                               "Can't retrieve list of services which "
                               "depend on this service.");
            MessageBox(0, msg, title, MB_OK | MB_ICONERROR);
            return 130; /* checkpoint error code */
	}
	/*
	 * We're just interested in the serviceStateCount but nothing else.
	 * We need to call EnumDependentServices to make sure the count
	 * variable will receive the correct count value.
	 */
        EnumDependentServices(hService, SERVICE_ACTIVE | SERVICE_INACTIVE,
                              serviceStates, serviceStateSize,
                              &serviceStateSize,
                              &serviceStateCount);
	free(serviceStates);
	if ( serviceStateCount != 0 ) {
            LoadStringResource(hModule, IDS_REALLYDEINSTALL,
                               msg, sizeof(msg),
                               "Other NT services are depending on this service.\n"
                               "Do you still want to deinstall it?");
            if ( MessageBox(0, msg, title, 
                            MB_YESNO | MB_DEFBUTTON2 | MB_ICONERROR) 
                 != IDYES ) {
	        CloseServiceHandle(hSCManager);
                CloseServiceHandle(hService);
                return 131; /* checkpoint error code */
	    }
	}

	/*
	 * Okay, looks like it would be safe to shut down that sucker.
	 */
        hSplashDialog = CreateDialog(GetModuleHandle(0),
                                     MAKEINTRESOURCE(DLG_STOPSERVICE),
                                     0, SplashDialogProc);
        ControlService(hService, SERVICE_CONTROL_STOP, &serviceStatus);
        ok = DeleteService(hService);
        if ( !ok && GetLastError() == ERROR_SERVICE_MARKED_FOR_DELETE ) {
            ok = TRUE;
        }
        if ( hSplashDialog ) {
             DestroyWindow(hSplashDialog);
        }
        CloseServiceHandle(hSCManager);
        CloseServiceHandle(hService);
        if ( !ok ) {
            LoadStringResource(hModule, IDS_NODEINSTALL, msg, sizeof(msg),
                               "The NT service could not be deinstalled.");
            MessageBox(0, msg, title, MB_OK | MB_ICONERROR);
            return 132; /* checkpoint error code */
        }
        return 0; /* okay -- was successfully marked for deletion */
    }
    /*
     * Another executable was registered, so we didn't touch it. Just bark
     * at the user, so she/he needs to take notice.
     */
    free(serviceConfig);
    CloseServiceHandle(hSCManager);
    CloseServiceHandle(hService);
    LoadStringResource(hModule, IDS_OTHERSERVICE, msg, sizeof(msg),
                       "This NT service was not deinstalled because another\n"
                       "service executable was installed as the active service.");
    MessageBox(0, msg, title, MB_OK | MB_ICONERROR);
    return 0; /* ok */
} /* UnregisterService */


/* --------------------------------------------------------------------------
 * And now for the selfregistration magic which is used for installation
 * programs like InstallShield Express: we just use the DllRegisterServer and
 * DllUnregisterServer entry points for service registration instead of OLE
 * server registration. Now finally a good use for registration...
 */
STDAPI DllRegisterServer()
{
    char serviceId[64];
    char serviceDisplay[128];
    char serviceDeps[1024];

    if ( !LoadString(hDLL, IDS_SERVICEID, serviceId, sizeof(serviceId)) ||
         !LoadString(hDLL, IDS_SERVICEDISPLAY, serviceDisplay,
                     sizeof(serviceDisplay)) ||
         !LoadString(hDLL, IDS_SERVICEDEPS, serviceDeps, sizeof(serviceDeps)) ) {
        return E_FAIL;
    }

    return RegisterService(hDLL,
                           serviceId, serviceDisplay, serviceDeps,
                           NtServiceAutomaticInstallation)
           ? E_FAIL : S_OK;
} /* DllRegisterServer */


STDAPI DllUnregisterServer()
{
    char serviceId[64];

    if ( !LoadString(hDLL, IDS_SERVICEID, serviceId, sizeof(serviceId)) ) {
        return E_FAIL;
    }
    return UnregisterService(hDLL, serviceId, NtServiceAutomaticInstallation)
           ? E_FAIL : S_OK;
} /* DllUnregisterServer */


/* --------------------------------------------------------------------------
 * Just a debugging helper...
 */
STDAPI Reset()
{
    GlobalDeleteAtom(GlobalFindAtom(PENDINGREBOOTATOMNAME));
    return S_OK;
}

/* --------------------------------------------------------------------------
 * For Borland C++ this is the entry point when the DLL gets loaded or thrown
 * out of the process space. As you can easily figure out yourself (c'mon,
 * just read the source below, lazy boy!) there isn't much to do here.
 */
BOOL APIENTRY
DllEntryPoint(HINSTANCE hInst, DWORD reason, LPVOID whatever)
{
    switch ( reason ) {
    case DLL_PROCESS_ATTACH:
        hDLL = hInst;
        break;
    case DLL_PROCESS_DETACH:
        hDLL = 0;
        break;
    }
    return TRUE;
} /* DllEntryPoint */


/* End of servreg.c */

