/*
*   $Id: ov_controlpanel.c,v 1.2 2004-10-29 09:08:24 ansgar Exp $
*
*   Copyright (C) 2000
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
*	21-Jun-2000 Dirk Meyer <dirk@plt.rwth-aachen.de>: File created.
*/

#include "libov/ov_ov.h"
#include "ov_controlpanel.h"

#include <windows.h>
#if OV_COMPILER_CYGWIN
/* missing under cygwin */
#define CPL_INIT		1
#define CPL_GETCOUNT	2
#define CPL_INQUIRE		3
#define CPL_SELECT		4
#define CPL_DBLCLK		5
#define CPL_STOP		6
#define CPL_EXIT		7
#define CPL_NEWINQUIRE	8
typedef NEWCPLINFO		*LPNEWCPLINFO;
#else
#include <cpl.h>
#endif
#include <stdio.h>
#include <stdlib.h>

/*
*	Registry keys
*/
#define KEY_OVNTSERVICE		"SYSTEM\\CurrentControlSet\\Services\\ov_ntservice"
#define SUBKEY_IMPAGEPATH	"ImagePath"

/*
*	Global variables
*/
static HMODULE	hinst = 0;
static char		*svcpath = 0;
static char		*filename = 0;
static char		*servername = 0;
static int		port = 0;
static int		startup = TRUE;
static HKEY		hkeysvc_query = 0;
static HKEY		hkeysvc_set = 0;
static int		dlginitialized = FALSE;

/*
*	Check if the parameter settings are OK
*/
static int checkParams(void) {
	/*
	*	local variables
	*/
	char	*pc;
	/*
	*	check if we can write to the registry
	*/
	if(!svcpath || !hkeysvc_set) {
		return FALSE;
	}
	/*
	*	check the server port
	*/
	if((port<0) || (port > 65535)) {
		return FALSE;
	}
	/*
	*	check the server name
	*/
	if(servername) {
		for(pc=servername; *pc; pc++) {
			if((!((*pc >= 'a') && (*pc <= 'z')))		/* no lower char */
				&& (!((*pc >= 'A') && (*pc <= 'Z')))	/* no upper char */
				&& (!((*pc >= '0') && (*pc <= '9')))	/* no digit */
				&& (*pc != '_')							/* and no underscore */
			) {
				return FALSE;
			}
		}
	}
	/*
	*	parameters are OK
	*/
	return TRUE;
}

/*
*	Fill the dialog window with the parameters
*/
static void fillDialogWithParams(HWND hwnd) {
	/*
	*	local variables
	*/
	char portno[16];	/* must be big enough to hold a 32 bit integer number string */
	/*
	*	instructions
	*/
	SendDlgItemMessage(hwnd, IDC_EDIT_SERVERNAME, WM_SETTEXT, 0, (LPARAM)(servername?servername:""));
	sprintf(portno, "%d", port);
	SendDlgItemMessage(hwnd, IDC_EDIT_SERVERPORT, WM_SETTEXT, 0, (LPARAM)portno);
	EnableWindow(GetDlgItem(hwnd, IDC_EDIT_SERVERPORT), port?TRUE:FALSE);
	CheckDlgButton(hwnd, IDC_RADIOBUTTON_ANYPORT, port?0:1);
	CheckDlgButton(hwnd, IDC_RADIOBUTTON_SPECIFICPORT, port?1:0);
	SendDlgItemMessage(hwnd, IDC_EDIT_DATABASEFILE, WM_SETTEXT, 0, (LPARAM)(filename?filename:""));
	CheckDlgButton(hwnd, IDC_RADIOBUTTON_STARTUP, startup?1:0);
	CheckDlgButton(hwnd, IDC_RADIOBUTTON_NOSTARTUP, startup?0:1);
	EnableWindow(GetDlgItem(hwnd, IDOK), checkParams()?TRUE:FALSE);
}

/*
*	Get the parameters form the dialog window
*/
static void getParamsFromDialog(HWND hwnd) {
	/*
	*	local variables
	*/
	DWORD	size;
	char	*portno;
	/*
	*	get the database file name
	*/
	if(filename) {
		free(filename);
		filename = 0;
	}
	size = SendDlgItemMessage(hwnd, IDC_EDIT_DATABASEFILE, WM_GETTEXTLENGTH, 0, 0)+1;
	if(size > 1) {
		filename = (char *)malloc(size);
		SendDlgItemMessage(hwnd, IDC_EDIT_DATABASEFILE, WM_GETTEXT, size, (LPARAM)filename);
	}
	/*
	*	get the server name
	*/
	if(servername) {
		free(servername);
		servername = 0;
	}
	size = SendDlgItemMessage(hwnd, IDC_EDIT_SERVERNAME, WM_GETTEXTLENGTH, 0, 0)+1;
	if(size > 1) {
		servername = (char *)malloc(size);
		SendDlgItemMessage(hwnd, IDC_EDIT_SERVERNAME, WM_GETTEXT, size, (LPARAM)servername);
	}
	/*
	*	get the port number
	*/
	if(IsDlgButtonChecked(hwnd, IDC_RADIOBUTTON_SPECIFICPORT)) {
		size = SendDlgItemMessage(hwnd, IDC_EDIT_SERVERPORT, WM_GETTEXTLENGTH, 0, 0)+1;
		portno = (char *)malloc(size);
		SendDlgItemMessage(hwnd, IDC_EDIT_SERVERPORT, WM_GETTEXT, size, (LPARAM)portno);
		port = atoi(portno);
		free(portno);
	} else {
		port = 0;
	}
	/*
	*	get the startup option
	*/
	startup = IsDlgButtonChecked(hwnd, IDC_RADIOBUTTON_STARTUP)?TRUE:FALSE;
}

/*
*	Open the registry
*/
static void openRegistry(void) {
	if(!hkeysvc_query) {
		if(RegOpenKeyEx(HKEY_LOCAL_MACHINE, KEY_OVNTSERVICE,
			0, KEY_QUERY_VALUE, &hkeysvc_query) != ERROR_SUCCESS
		) {
			hkeysvc_query = 0;
		}
	}
	if(!hkeysvc_set) {
		if(RegOpenKeyEx(HKEY_LOCAL_MACHINE, KEY_OVNTSERVICE,
			0, KEY_SET_VALUE, &hkeysvc_set) != ERROR_SUCCESS
		) {
			hkeysvc_set = 0;
		}
	}
}

/*
*	Close the registry
*/
static void closeRegistry(void) {
	if(hkeysvc_query) {
		RegCloseKey(hkeysvc_query);
		hkeysvc_query = 0;
	}
	if(hkeysvc_set) {
		RegCloseKey(hkeysvc_set);
		hkeysvc_set = 0;
	}
}

/*
*	Get the service parameters from the registry
*/
static void queryParamsFromRegistry(void) {
	/*
	*	local variables
	*/
	char	*imagepath;
	char	*pc;
	int		argc;
	char	**argv;
	int		i;
	DWORD	size;
	/*
	*	initialize with default values
	*/
	if(svcpath) {
		free(svcpath);
		svcpath = 0;
	}
	if(filename) {
		free(filename);
		filename = 0;
	}
	if(servername) {
		free(servername);
		servername = 0;
	}
	port = 0;		/* default: any port */
	startup = TRUE;	/* default: startup */
	/*
	*	try to get the parameters from the registry
	*/
	if(hkeysvc_query) {
		if(RegQueryValueEx(hkeysvc_query, SUBKEY_IMPAGEPATH, NULL, NULL, NULL, &size) == ERROR_SUCCESS) {
			imagepath = (char *)malloc(size*sizeof(char));
			RegQueryValueEx(hkeysvc_query, SUBKEY_IMPAGEPATH, NULL, NULL, (LPBYTE)imagepath, &size);
			/*
			*	convert into argc/argv format
			*/
			for(argc=0, pc=imagepath; *pc; ) {
				for(; *pc && (*pc != ' '); pc++) {}
				for(; *pc == ' '; pc++) {}
				argc++;
			}
			argv = (char **)malloc(argc*sizeof(char *));
			for(argc=0, pc=imagepath; *pc; ) {
				argv[argc] = pc;
				for(; *pc && (*pc != ' '); pc++) {}
				for(; *pc == ' '; pc++) { *pc = 0; }
				argc++;
			}
			/*
			*	parse the arguments
			*/
			if(argc) {
				if(svcpath) {
					free(svcpath);
				}
				svcpath = strdup(argv[0]);
			}
			for(i=1; i<argc; i++) {
				/*
				*	set database filename option
				*/
				if(!strcmp(argv[i], "-f") || !strcmp(argv[i], "--file")) {
					i++;
					if(i<argc) {
						if(filename) {
							free(filename);
						}
						filename = strdup(argv[i]);
					}
				}
				/*
				*	set server name option
				*/
				else if(!strcmp(argv[i], "-s") || !strcmp(argv[i], "--server-name")) {
					i++;
					if(i<argc) {
						if(servername) {
							free(servername);
						}
						servername = strdup(argv[i]);
					}
				}
				/*
				*	set port number option
				*/
				else if(!strcmp(argv[i], "-p") || !strcmp(argv[i], "--port-number")) {
					i++;
					if(i<argc) {
						port = strtol(argv[i], NULL, 0);
					}
				}
				/*
				*	no startup option
				*/
				else if(!strcmp(argv[i], "-n") || !strcmp(argv[i], "--no-startup")) {
					startup = FALSE;
				}
			}
			/*
			*	free the memory allocated
			*/
			free(argv);
			free(imagepath);
		}
	}
}

/*
*	Write the parameters to the registry
*/
static void writeParamsToRegistry(HWND hwnd) {
	/*
	*	local variables
	*/
	char	*imagepath;
	char	portno[16];	/* must be big enough to hold a 32 bit integer number string */
	/*
	*	try to set the parameters in the registry
	*/
	getParamsFromDialog(hwnd);
	if(hkeysvc_set && svcpath) {
		sprintf(portno, "%d", port);
		imagepath = (char *)malloc((
			strlen(svcpath)							// "<svcpath>"
			+(filename?strlen(filename)+4:0)		// " -f <filename>"
			+(servername?strlen(servername)+4:0)	// " -s <servername>"
			+strlen(portno)+4						// " -p <port>"
			+3										// " -n"
			+1
			)*sizeof(char));
		sprintf(imagepath, "%s%s%s%s%s%s%s%s",
			svcpath,
			filename?" -f ":"", filename?filename:"",
			servername?" -s ":"", servername?servername:"",
			port?" -p ":"", port?portno:"",
			startup?"":" -n"
		);
		RegSetValueEx(hkeysvc_set, SUBKEY_IMPAGEPATH, 0, REG_EXPAND_SZ,
			(CONST BYTE *)imagepath, strlen(imagepath)+1);
		free(imagepath);
	}
}

/*
*	Free memory allocated for the parameters
*/
static void deleteParams(void) {
	if(svcpath) {
		free(svcpath);
		svcpath = 0;
	}
	if(servername) {
		free(servername);
		servername = 0;
	}
	if(filename) {
		free(filename);
		filename = 0;
	}
}

/*
*	Display a dialog letting the user select the database file
*/
static void selectDatabaseFile(HWND hwnd) {
	/*
	*	local variabales
	*/
	OPENFILENAME	ofn;
	char			file[1024];
	DWORD			size;
	/*
	*	get the filename from the dialog box
	*/
	size = SendDlgItemMessage(hwnd, IDC_EDIT_DATABASEFILE, WM_GETTEXTLENGTH, 0, 0)+1;
	if((size>1) && (size<sizeof(file))) {
		SendDlgItemMessage(hwnd, IDC_EDIT_DATABASEFILE, WM_GETTEXT, size, (LPARAM)file);
	} else {
		file[0] = 0;
	}
	/*
	*	prepare the open dialog
	*/
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = hwnd;
	ofn.hInstance = 0;
	ofn.lpstrFilter = "All Files (*.*)\0*.*\0ACPLT/OV Database (*.ovd)\0*.ovd\0";
	ofn.lpstrCustomFilter = NULL;
	ofn.nMaxCustFilter = 0;
	ofn.nFilterIndex = 2;
	ofn.lpstrFile = (LPTSTR)file;
	ofn.nMaxFile = sizeof(file);
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.lpstrTitle = "Select ACPLT/OV Database";
	ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
	ofn.nFileOffset = 0;
	ofn.nFileExtension = 0;
	ofn.lpstrDefExt = "ovd";
	ofn.lCustData = 0;
	ofn.lpfnHook = NULL;
	ofn.lpTemplateName = NULL;
	/*
	*	display the open file dialog
	*/
   	if(GetOpenFileName(&ofn)) {
   		/*
   		*	set the selected file name in the options dialog
   		*/
		SendDlgItemMessage(hwnd, IDC_EDIT_DATABASEFILE, WM_SETTEXT, 0, (LPARAM)file);
	}
}

/*
*	About window procedure
*/
static BOOL CALLBACK AboutDialogProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
	switch(msg) {
	case WM_INITDIALOG:
		return TRUE;

	case WM_COMMAND:
		switch(LOWORD(wparam)) {
		case IDOK:
			EndDialog(hwnd, TRUE);
			return TRUE;

		default:
			break;
		}
		break;
	
	case WM_CLOSE:
		EndDialog(hwnd, FALSE);
		return TRUE;

	default:
		break;
	}
	return FALSE;
}

/*
*	Dialog window procedure
*/
static BOOL CALLBACK OptionDialogProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
	switch(msg) {
	case WM_INITDIALOG:
		fillDialogWithParams(hwnd);
		dlginitialized = TRUE;
		return TRUE;

	case WM_COMMAND:
		switch(LOWORD(wparam)) {
		case IDOK:
			writeParamsToRegistry(hwnd);
			EndDialog(hwnd, TRUE);
			dlginitialized = FALSE;
			return TRUE;

		case IDCANCEL:
			EndDialog(hwnd, FALSE);
			dlginitialized = FALSE;
			return TRUE;

		case IDC_BUTTON_ABOUT:
			DialogBox(hinst, (LPCTSTR)MAKEINTRESOURCE(DIALOG_ABOUT), hwnd, (DLGPROC)AboutDialogProc); 
			return TRUE;

		case IDC_BUTTON_SELECTFILE:
			selectDatabaseFile(hwnd);
			return TRUE;

		case IDC_RADIOBUTTON_ANYPORT:
		case IDC_RADIOBUTTON_SPECIFICPORT:
		case IDC_EDIT_SERVERNAME:
		case IDC_EDIT_SERVERPORT:
			if(dlginitialized) {
				getParamsFromDialog(hwnd);
				EnableWindow(GetDlgItem(hwnd, IDOK), checkParams()?TRUE:FALSE);
				EnableWindow(GetDlgItem(hwnd, IDC_EDIT_SERVERPORT),
					IsDlgButtonChecked(hwnd, IDC_RADIOBUTTON_SPECIFICPORT)?TRUE:FALSE);
			}
			return TRUE;

		default:
			break;
		}
		break;
	
	case WM_CLOSE:
		EndDialog(hwnd, FALSE);
		return TRUE;

	default:
		break;
	}
	return FALSE;
}

OV_DLLFNCEXPORT LONG APIENTRY CPlApplet(HWND hwnd, UINT msg, LPARAM lparam1, LPARAM lparam2) { 
	LPNEWCPLINFO newcplinfo; 
	switch(msg) { 
	case CPL_INIT:
		hinst = GetModuleHandle("ov_controlpanel.cpl");
		openRegistry();
		queryParamsFromRegistry();
		return TRUE; 

	case CPL_GETCOUNT:
		return 1; 

	case CPL_NEWINQUIRE:
		newcplinfo = (LPNEWCPLINFO)lparam2; 
		newcplinfo->dwSize = sizeof(NEWCPLINFO); 
		newcplinfo->dwFlags = 0; 
		newcplinfo->dwHelpContext = 0; 
		newcplinfo->lData = 0; 
		newcplinfo->hIcon = LoadIcon(hinst, (LPCTSTR)MAKEINTRESOURCE(ICON_OV)); 
		newcplinfo->szHelpFile[0] = '\0';
		LoadString(hinst, STRINGTABLE_101, newcplinfo->szName, 32); 
		LoadString(hinst, STRINGTABLE_102, newcplinfo->szInfo, 64); 
		break; 

	case CPL_SELECT:
		break; 

	case CPL_DBLCLK:
		DialogBox(hinst, (LPCTSTR)MAKEINTRESOURCE(DIALOG_OPTIONS), hwnd, (DLGPROC)OptionDialogProc); 
		break; 

	case CPL_STOP:
		break; 

	case CPL_EXIT:
		deleteParams();
		closeRegistry();
		break; 

	default: 
		break; 
	} 
	return FALSE;
} 
 
