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
*	30-April-2004			St
*		-	Erstellung
*
*	30-August-2005			St
*		-	CVS-CheckIn
*
***********************************************************************/

#ifndef CMD_MACROS_H_INCLUDED
#define CMD_MACROS_H_INCLUDED

/***********************************************************************
	Cmd
***********************************************************************/

#define CMDLIB_CMD(target, command, value) \
	send_command( \
		(OV_INSTPTR_fb_functionblock) pfb, \
		NULL, \
		target, \
      command, \
      value);
      
/***********************************************************************
	sendCmd
***********************************************************************/
	
#define CMDLIB_SENDCMD(pCommander, pSender, Target, Command, Value) \
	send_command( \
		(OV_INSTPTR_fb_functionblock) pCommander, \
		(OV_INSTPTR_cmdlib_CmdSender) pSender, \
		Target, \
		Command, \
		Value);

#endif