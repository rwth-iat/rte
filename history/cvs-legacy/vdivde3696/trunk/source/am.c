/*
*	Copyright (C) 1998, 1999
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
*	   notice, this list of conditions and the following disclaimer in the
*	   documentation and/or other materials provided with the distribution.
*	4. Neither the name of the Chair of Process Control Engineering nor the
*	   name of the Aachen University of Technology may be used to endorse or
*	   promote products derived from this software without specific prior
*	   written permission.
*
*	THIS SOFTWARE IS PROVIDED BY THE CHAIR OF PROCESS CONTROL ENGINEERING
*	``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
*	TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
*	PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE CHAIR OF PROCESS CONTROL
*	ENGINEERING BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*	EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
*	PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
*	OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
*	WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
*	OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*	ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
/****************************************************************************
*									    *
*    functionblock                      				    *
*                                                       		    *
*    alarm or message storing (VDI/VDE 3696)                                *
*                                                                           *
****************************************************************************/
/*
*   type: am
*   ========
*/
/*
*   history
*   -------
*   created : 21.5.1999
*   author  : Michael Klamann, Chair of Process Control Engineering, RWTH Aachen
*/
/*
*   description
*   -----------
*   The functionblock watches the input signal i and generates
*   alarms/messages on specific occurences.
* 
*   mode of operation (AMODE):
*     "A1" alarm if I=TRUE
*     "A0" alarm if I=FALSE
*     "MR" message if rising edge
*     "MF" message if falling edge
*     "MB" message if rising or falling edge   
*
*   note : VDI/VDE 3696 doesn´t specify any outputs.
*          The overall idea of the functionblock is to
*          create text messages which specify what occured, when it occured ,how 
*          important the occurance is and if the generated message is just a message 
*          or an alarm.
*
*          This version is limited to two BOOLEAN outputs.
*          Q indicates if there is an alarm/message as specified in AMODE.
*          QA determines if Q is an alarm- (TRUE) or a message- (FALSE) signal.
*          There is no extra priority output.
*/
/*
*   C-Includes
*   ----------
*/
#include <string.h>
/*  
*   defines
*   -------
*   none
*/
/*
*   modul-declaration
*   -----------------
*/
#include "vdivde3696.h"
#include "libov/ov_macros.h"

OV_RESULT OV_DLLFNCEXPORT vdivde3696_am_constructor(
	OV_INSTPTR_ov_object	pobj
) {
	OV_RESULT res;
	OV_INSTPTR_vdivde3696_am pam;

	res = fb_functionblock_constructor(pobj);
	if (Ov_Fail(res)) return res;
	pam = Ov_StaticPtrCast(vdivde3696_am, pobj);
	pam->v_en = TRUE;
	pam->v_aon = TRUE;
	ov_string_setvalue(&pam->v_amode, "A1" );
	pam->v_aprio = 0;
	return OV_ERR_OK;
}

void OV_DLLFNCEXPORT vdivde3696_am_typemethod(
	OV_INSTPTR_fb_functionblock	pfb,
	OV_TIME						*pltc
) {
	OV_INSTPTR_vdivde3696_am port = Ov_StaticPtrCast(vdivde3696_am, pfb);
	/*
	*   local variables
	*   ---------------
	*   none
	*/
	/*
	*   instructions
	*   ------------
	*/
        if (port->v_en) /* functionblock enable */
        {
	  
	  if (port->v_aon) /* alarm/message generating active */
	  {
	    /* preset output q to no alarm/message */
	    port->v_q = FALSE;

            /* check for existing alarm */
	    if (strcmp(port->v_amode,"A1") == 0)
	    {
	      /* alarm if i is TRUE */
	      if (port->v_i)
	      { 
		port->v_q = TRUE;
	      }
	      port->v_eno = TRUE;
	      port->v_qa = TRUE;
	    }
	    else if (strcmp(port->v_amode,"A0") == 0)
	         {
		   /* alarm if i is FALSE */
		   if (port->v_i == FALSE)
		   {
		     port->v_q = TRUE;
		   }  
		   port->v_qa = TRUE;
		   port->v_eno = TRUE;
		 }
	         else if (strcmp(port->v_amode,"MR") == 0)
		      {
			/* message if rising edge */
			if (port->v_i && (port->v_iold == FALSE))
		        {
			  port->v_q = TRUE;
			}
		        port->v_qa = FALSE;
			port->v_eno = TRUE;
		      }
	              else if (strcmp(port->v_amode,"MF") == 0)
		           {
			     /* message if falling edge */
			     if ((port->v_i == FALSE) && port->v_iold)
			     {
			       port->v_q = TRUE;
			     }
 			     port->v_qa = FALSE;
			     port->v_eno = TRUE;
			   }
	                   else if (strcmp(port->v_amode,"MB") == 0)
			        {
				  /* message on both edges */
				  if ((port->v_i - port->v_iold) != 0)
				  {
				    port->v_q = TRUE;
				  }
				  port->v_qa = FALSE;
				  port->v_eno = TRUE;
				}
	                        else /* not specified string in AMODE -> enable output to FALSE */
				{
				  port->v_eno = FALSE;
				}
	  }
	}
        else /* functionblock NOT enable*/
        {
	  port->v_eno = FALSE;
	}

	/* store input value for next cycle */
        port->v_iold = port->v_i;

        /* add 1 to counter */
	port->v_counter = (port->v_counter+1)%10000;

/*******  end  *******/
	return;
	}







