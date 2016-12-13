/*
*   Copyright (C) 1998
*   Chair of Process Control Engineering,
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







