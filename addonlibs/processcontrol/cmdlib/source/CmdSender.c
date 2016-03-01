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
*	CmdSender.c
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
*	22-März-2004			St
*		-	Übernahme aus der comlib (V0.6.5)
*
*	30-August-2005			St
*		-	CVS-CheckIn
*
***********************************************************************/

#ifndef OV_COMPILE_LIBRARY_cmdlib
#define OV_COMPILE_LIBRARY_cmdlib
#endif

/***********************************************************************
	Includes
***********************************************************************/

#include "cmdlib.h"
#include "cmd.h"
#include "com.h"
#include "libov/ov_macros.h"

/***********************************************************************
	get Accessor ResultLocalTxt
***********************************************************************/

OV_STRING cmdlib_CmdSender_ResultLocalTxt_get(
	OV_INSTPTR_cmdlib_CmdSender	pSender
)	{			
	switch(pSender->v_ResultLocal)
	{
		case COM_ERR_OK:
			return COM_TEXT_OK;
			
		case COM_ERR_GENERIC:
			return COM_TEXT_GENERIC;
			
		case COM_ERR_NOACCESSMANAGER:
			return COM_TEXT_NOACCESSMANAGER;
			
		case COM_ERR_ALLREADYEXISTS:
			return COM_TEXT_ALLREADYEXISTS;
			
		case COM_ERR_BADPARAM:
			return COM_TEXT_BADPARAM;
			
		case COM_ERR_BADPATHMANAGER:
			return COM_TEXT_BADPATHMANAGER;
			
		case COM_ERR_BADOBJTYPEMANAGER:
			return COM_TEXT_BADOBJTYPEMANAGER;
			
		case COM_ERR_BADPATHLOCALINST:
			return COM_TEXT_BADPATHLOCALINST;
			
		case COM_ERR_BADOBJTYPELOCALINST:
			return COM_TEXT_BADOBJTYPELOCALINST;
			
		case COM_ERR_BADPATHSTATUS:
			return COM_TEXT_BADPATHSTATUS;
			
		case COM_ERR_BADDATATYPE:
			return COM_TEXT_BADDATATYPE;
			
		case COM_ERR_BADDATATYPESTATUS:
			return COM_TEXT_BADDATATYPESTATUS;
			
		case COM_ERR_BADOBJTYPESTATUS:
			return COM_TEXT_BADOBJTYPESTATUS;
			
		case COM_ERR_NEWSTATUS:
			return COM_TEXT_NEWSTATUS;
			
		default:
			return "generic ";	
	};
};

/***********************************************************************
	get Accessor ResultRemoteTxt
***********************************************************************/

OV_STRING cmdlib_CmdSender_ResultRemoteTxt_get(
	OV_INSTPTR_cmdlib_CmdSender	pSender
)	{			
	switch(pSender->v_ResultRemote)
	{
		case COM_STATUS_OK:
			return COM_TEXT_STATUS_OK;
		
		case COM_STATUS_BUSY:
			return COM_TEXT_STATUS_BUSY;
			
		case COM_STATUS_ERR:
			return COM_TEXT_STATUS_ERR;
		
		default:
			return "generic";
	};
};

/***********************************************************************
	Typemethod
***********************************************************************/

void OV_DLLFNCEXPORT cmdlib_CmdSender_typemethod(
	OV_INSTPTR_fb_functionblock	pfb,
	OV_TIME								*pltc
)	{
	//	Local Variables
	//
	OV_INSTPTR_cmdlib_CmdSender
		pSender = Ov_StaticPtrCast(cmdlib_CmdSender, pfb);
	OV_INSTPTR_fb_functionblock
		pCommander = NULL;
		
	//	Check Activity
	//
	if (pSender->v_Enabled == FALSE)
		return;

	//	Send Command, if Command and InstanceLocator set
	//
   if(	(ov_string_compare(pSender->v_Command, "")	!= 0) 
   	&& (ov_string_compare(pSender->v_Instance, "")	!= 0))
   {
   	//	Check & eventually set value
   	//
   	if (ov_string_compare(pSender->v_Value, "") == 0)
   		ov_string_setvalue(&pSender->v_Value, "0");
   	
   	//	get from CmdSender differing Commander
   	//
   	if (ov_string_compare(pSender->v_Commander, "") != 0)
   	{
   		//	get Commander
			//
			pCommander = Ov_DynamicPtrCast(fb_functionblock, 
								ov_path_getobjectpointer(pSender->v_Commander, 2));
			//	Commander is set, but no corresponding object found -> STOP
			//
			if (!pCommander)
				return;
   	};
   	
   	//	Send Command
   	//
   	if (pCommander)
   	{
   		//	from Sender differing Commander
   		//
   		pSender->v_ResultLocal = CMDLIB_SENDCMD(pCommander, pSender, pSender->v_Instance, pSender->v_Command, pSender->v_Value);
   	} else {
   		//	Commander == Sender
   		//
   		pSender->v_ResultLocal = CMDLIB_SENDCMD(pSender, pSender, pSender->v_Instance, pSender->v_Command, pSender->v_Value);
   	};
	};
  
	return;
};
