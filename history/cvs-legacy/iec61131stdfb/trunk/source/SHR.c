/*
*	Copyright (C) 2010
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
/******************************************************************************
*
*   FILE
*   ----
*   SHR.c
*
*   History
*   -------
*   2010-12-15   File created
*
*******************************************************************************
*
*   This file is generated by the 'fb_builder' command
*
******************************************************************************/


#ifndef OV_COMPILE_LIBRARY_iec61131stdfb
#define OV_COMPILE_LIBRARY_iec61131stdfb
#endif


#include "iec61131stdfb.h"
#include "libov/ov_macros.h"


OV_DLLFNCEXPORT OV_RESULT iec61131stdfb_SHR_IN_set(
    OV_INSTPTR_iec61131stdfb_SHR          pobj,
    const OV_UINT  value
) {
    pobj->v_IN = value;
    return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_RESULT iec61131stdfb_SHR_N_set(
    OV_INSTPTR_iec61131stdfb_SHR          pobj,
    const OV_UINT  value
) {
    pobj->v_N = value;
    return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_UINT iec61131stdfb_SHR_OUT_get(
    OV_INSTPTR_iec61131stdfb_SHR          pobj
) {
    return pobj->v_OUT;
}

OV_DLLFNCEXPORT void iec61131stdfb_SHR_shutdown(
	OV_INSTPTR_ov_object 	pobj
) {
    /*    
    *   local variables
    */
    OV_INSTPTR_iec61131stdfb_SHR pinst = Ov_StaticPtrCast(iec61131stdfb_SHR, pobj);

    /* do what */

    /* set the object's state to "shut down" */
    fb_functionblock_shutdown(pobj);

    return;
}

OV_DLLFNCEXPORT void iec61131stdfb_SHR_typemethod(
	OV_INSTPTR_fb_functionblock	pfb,
	OV_TIME						*pltc
) {
    /*    
    *   local variables
    */
    OV_INSTPTR_iec61131stdfb_SHR pinst = Ov_StaticPtrCast(iec61131stdfb_SHR, pfb);
	if(pinst->v_N > 8 * sizeof(OV_UINT))
		pinst->v_OUT = 0;
	else
		pinst->v_OUT = pinst->v_IN >> pinst->v_N;
	
    return;
}

