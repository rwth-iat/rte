/*
*	Copyright (C) 2014
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
*
***********************************************************************/

#ifndef OV_COMPILE_LIBRARY_kshttp
#define OV_COMPILE_LIBRARY_kshttp
#endif

#include "config.h"

OV_DLLFNCEXPORT OV_RESULT kshttp_httpSimpleTicketAuthenticator_constructor(
	OV_INSTPTR_ov_object 	pobj
) {
    /*    
    *   local variables
    */
//    OV_INSTPTR_kshttp_httpSimpleTicketAuthenticator pinst = Ov_StaticPtrCast(kshttp_httpSimpleTicketAuthenticator, pobj);
    OV_RESULT    result;

    /* do what the base class does first */
    result = ksbase_TicketAuthenticator_constructor(pobj);
    if(Ov_Fail(result))
         return result;

    /* do what */


    return OV_ERR_OK;
}

OV_DLLFNCEXPORT void kshttp_httpSimpleTicketAuthenticator_startup(
	OV_INSTPTR_ov_object 	pobj
) {
    /*    
    *   local variables
    */
    //OV_INSTPTR_kshttp_httpSimpleTicketAuthenticator pinst = Ov_StaticPtrCast(kshttp_httpSimpleTicketAuthenticator, pobj);

    /* do what the base class does first */
    ksbase_TicketAuthenticator_startup(pobj);

    /* do what */


    return;
}

OV_DLLFNCEXPORT void kshttp_httpSimpleTicketAuthenticator_shutdown(
	OV_INSTPTR_ov_object 	pobj
) {
    /*    
    *   local variables
    */
    //OV_INSTPTR_kshttp_httpSimpleTicketAuthenticator pinst = Ov_StaticPtrCast(kshttp_httpSimpleTicketAuthenticator, pobj);

    /* do what */

    /* set the object's state to "shut down" */
    ksbase_TicketAuthenticator_shutdown(pobj);

    return;
}

OV_DLLFNCEXPORT OV_TICKET* kshttp_httpSimpleTicketAuthenticator_createticket(
	void *data,
	OV_TICKET_TYPE type
) {
    /*    
    *   local variables
    */

    return (OV_TICKET*)0;
}

OV_DLLFNCEXPORT void kshttp_httpSimpleTicketAuthenticator_deleteticket(
	OV_TICKET *pticket
) {
    /*    
    *   local variables
    */

    return;
}

OV_DLLFNCEXPORT OV_BOOL kshttp_httpSimpleTicketAuthenticator_encodereply(
	void *data,
	OV_TICKET *pticket
) {
    /*    
    *   local variables
    */

    return (OV_BOOL)0;
}

OV_DLLFNCEXPORT OV_ACCESS kshttp_httpSimpleTicketAuthenticator_TicketGetaccess(
	const OV_TICKET *pticket
) {
    /*    
    *   local variables
    */

    return (OV_ACCESS)0;
}

