/*
*	Copyright (C) 2004
*	Chair of Process Control Engineering,
*	Aachen University of Technology.
*	All rights reserved.
*
*	Redistribution and use in source and binary forms, with or without
*	modification, are permitted provided that the following conditions
*	are met:
*	1. Redistributions of source code must retain the above copyright
*	   notice, this list of conditions and the following disclaimer.
*	2. Redistributions in binary form must print or display the above
*	   copyright notice either during startup or must have a means for
*	   the user to view the copyright notice.
*	3. Redistributions in binary form must reproduce the above copyright
*	   notice, this list of conditions and the following disclaimer in
*		the documentation and/or other materials provided with the
*		distribution.
*	4. Neither the name of the Chair of Process Control Engineering nor
*		the name of the Aachen University of Technology may be used to
*		endorse or promote products derived from this software without
*		specific prior written permission.
*
*	THIS SOFTWARE IS PROVIDED BY THE CHAIR OF PROCESS CONTROL ENGINEERING
*	``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
*	LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
*	A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE CHAIR OF
*	PROCESS CONTROL ENGINEERING BE LIABLE FOR ANY DIRECT, INDIRECT,
*	INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
*	BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
*	OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
*	AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
*	LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY
*	WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
*	POSSIBILITY OF SUCH DAMAGE.
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