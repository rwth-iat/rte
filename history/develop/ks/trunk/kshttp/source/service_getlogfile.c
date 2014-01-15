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

#include "config.h"

#define EXEC_GETLOGFILE_RETURN	Ov_SetDynamicVectorLength(&frommatch,0,STRING);\
		Ov_SetDynamicVectorLength(&tomatch,0,STRING);\
		Ov_SetDynamicVectorLength(&maxentriesmatch,0,STRING);\
		ov_string_setvalue(&LoopEntry, NULL);\
		ov_string_setvalue(&LoopEntryXmlString, NULL);\
		ov_string_setvalue(&LoopEntryTimeString, NULL);\
				return

/**
 * Syntax: /getLogfile?from=2014-01-01T13:15:00.000000&to=2016-01-01T13:15:00.000000&maxentries=10
 *
 * extracts the command for the getlogfile and let do ov_logfile_getmessages the job
 * @param args arguments of the http get request
 * @param message pointer to the result string
 * @return resultcode of the operation
 */
OV_RESULT kshttp_exec_getlogfile(OV_STRING_VEC* const args, OV_STRING* message, KSHTTP_RESPONSEFORMAT response_format){
	/*
	*	parameter and result objects
	*/
	OV_STRING_VEC frommatch = {0,NULL};
	OV_STRING_VEC tomatch = {0,NULL};
	OV_STRING_VEC maxentriesmatch = {0,NULL};
	OV_TIME		from;
	OV_TIME		to;
	OV_UINT		max_no_messages;
	OV_STRING	*logmessages;
	OV_TIME		*times;
	OV_UINT		no_messages = 0;

	OV_UINT i = 0;
	OV_STRING LoopEntry = NULL;
	OV_STRING LoopEntryXmlString = NULL;
	OV_STRING LoopEntryTimeString = NULL;
	OV_RESULT fr = OV_ERR_OK;

	//process from
	Ov_SetDynamicVectorLength(&frommatch,0,STRING);
	kshttp_find_arguments(args, "from", &frommatch);
	if(frommatch.veclen == 1){
		kshttp_asciitotime(&from, frommatch.value[0], response_format);
	}else{
		from.secs = 0;
		from.usecs = 0;
	}

	//process to
	Ov_SetDynamicVectorLength(&tomatch,0,STRING);
	kshttp_find_arguments(args, "to", &tomatch);
	if(tomatch.veclen == 1){
		kshttp_asciitotime(&to, tomatch.value[0], response_format);
	}else{
		ov_time_gettime(&to);
	}

	//process maxnomessages
	Ov_SetDynamicVectorLength(&maxentriesmatch,0,STRING);
	kshttp_find_arguments(args, "maxentries", &maxentriesmatch);
	if(maxentriesmatch.veclen == 1){
		max_no_messages = atoi(maxentriesmatch.value[0]);
	}else{
		max_no_messages = OV_LOGFILE_MAXMSGS;
	}

	ov_memstack_lock();
	fr = ov_logfile_getmessages(&from, &to, max_no_messages, &logmessages, &times, &no_messages);

	/**
	 * Parse result from OV function
	 */
	if(Ov_Fail(fr)){
		//general problem like memory problem
		kshttp_print_result_array(message, response_format, &fr, 1, ": general problem");
		ov_memstack_unlock();
		EXEC_GETLOGFILE_RETURN fr;
	}
	ov_string_setvalue(&LoopEntryXmlString, "entry");
	for (i=0; i < no_messages; i++){
		if(response_format == KSX){
			kshttp_timetoascii(&LoopEntryXmlString, &times[i], response_format);
			ov_string_print(&LoopEntryXmlString, "entry time=\"%s\"", LoopEntryXmlString);
		}
		kshttp_response_part_init(&LoopEntry, response_format, LoopEntryXmlString);
		kshttp_escapeString(&LoopEntryTimeString, &logmessages[i], response_format);
		ov_string_append(&LoopEntry, LoopEntryTimeString);
		kshttp_response_part_finalize(&LoopEntry, response_format, "entry");
		ov_string_append(message, LoopEntry);
	}

	ov_memstack_unlock();
	EXEC_GETLOGFILE_RETURN fr;
}
