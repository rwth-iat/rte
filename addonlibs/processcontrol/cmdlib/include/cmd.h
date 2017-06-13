/*
*   Copyright (C) 2004
*   Lehrstuhl fuer Prozessleittechnik,
*   D-52056 Aachen, Germany.
*   All rights reserved.
*
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
/***********************************************************************
*
*	File
*	-----
*	cmd_lib.h
*
*	Autoren:
*	--------
*	St							Stefan Schmitz <StefanS@plt.rwth-aachen.de>
*
*	CVS:
*	----
*	$Revision: 1.2 $
*	$Date: 2005-08-30 14:06:27 $
*
*	Historie:
*	--------
*	23-März-2004			St
*		-	Erstellung
*
*	18-Juli-2005			St
*		-	Belegungskommandos aus comlib (com.h) überführt
*		-	FB_LOCAL hinzugefügt
*
*	30-August-2005			St
*		-	CVS-CheckIn
*
***********************************************************************/

#ifndef _CMD_LIB_H
#define _CMD_LIB_H

/***********************************************************************
	Includes
***********************************************************************/

#include "fb.h"
#include "fb_namedef.h"
#include "libov/ov_path.h"
#include "libov/ov_malloc.h"
#include "cmd_macros.h"

/***********************************************************************
	Der Modul sendet ein Kommando von einer Quellinstanz (pinst) an eine
	Zielinstanz (target). Vor der Versendung wird die Belegung der
	Einheit und des Kommandoeinganges überprüft.
***********************************************************************/

OV_RESULT OV_DLLFNCEXPORT send_command(
	OV_INSTPTR_fb_functionblock	pCommander,
	OV_INSTPTR_cmdlib_CmdSender	pSender,
	const char  *target, 	           		/* >|  Zielinstanz	        */
	const char  *command,						/* >|  Kommandoname          */
	const char  *value							/* >|  Kommandowert          */
);

OV_STRING OV_DLLFNCEXPORT cmdlib_processcontrol_generic_commands(void);

/***********************************************************************
	Defines
***********************************************************************/

#define CMD(target, command, value) \
	send_command( \
		(OV_INSTPTR_fb_functionblock) pfb, \
		NULL, \
		target, \
      command, \
      value);
	
#define CMDLIB_SENDCMD(pCommander, pSender, Target, Command, Value) \
	send_command( \
		(OV_INSTPTR_fb_functionblock) pCommander, \
		(OV_INSTPTR_cmdlib_CmdSender) pSender, \
		Target, \
		Command, \
		Value);

/***********************************************************************
	Belegungskommandos
***********************************************************************/

#define FB_OPERATOR  "OP"
#define FB_LOCAL		"LOCAL"
#define FB_OCCUPY    "OCCUPY"
#define FB_FREE      "FREE"

#endif
