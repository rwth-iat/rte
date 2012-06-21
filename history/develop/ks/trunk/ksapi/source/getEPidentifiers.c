#ifndef OV_COMPILE_LIBRARY_ksapi
#define OV_COMPILE_LIBRARY_ksapi
#endif


#include "ksapi.h"
#include "ksapi_h.h"
#include "libov/ov_scheduler.h"
#include "assert.h"
#if !OV_SYSTEM_NT
#include <unistd.h>
#endif

#include "xdrhandling.h"
#include "config.h"


/*	ksapi_getEPidentifiers_namemask_get
 *	returns the current value of the namemask
 */
OV_DLLFNCEXPORT OV_STRING ksapi_getEPidentifiers_namemask_get(
	OV_INSTPTR_ksapi_getEPidentifiers	pobj
) {
	return pobj->v_namemask;
}

/*	ksapi_getEPidentifiers_namemask_set
 *	sets the current receiveint to value
 */
OV_DLLFNCEXPORT OV_RESULT ksapi_getEPidentifiers_namemask_set(
	OV_INSTPTR_ksapi_getEPidentifiers	pobj,
	const OV_STRING							value
) {
	return ov_string_setvalue(&pobj->v_namemask, value);
	
}

/*	ksapi_getEPidentifiers_startup
 *	On startup set all values to default
 */
OV_DLLFNCEXPORT void ksapi_getEPidentifiers_startup(
	OV_INSTPTR_ov_object 	pobj
) {
	OV_INSTPTR_ksapi_getEPidentifiers sobj = Ov_StaticPtrCast(ksapi_getEPidentifiers, pobj);
	
	//To avoid a double Startup, which occures, because startup is executed while constructing 
	//the object and while starting the FB, this check is implemented
	if (pobj->v_objectstate & OV_OS_STARTED) {
		return;
	}
	ksapi_KSCommon_startup(pobj);
	//initialize Values
	ksapi_getEPidentifiers_namemask_set(sobj,"*");
	
	return;
}

/*	ksapi_getEPidentifiers_shutdown
 *	After closing the service and the connection, shutdown the object.
 */
OV_DLLFNCEXPORT void ksapi_getEPidentifiers_shutdown(
	OV_INSTPTR_ov_object	pobj
) {
	ksapi_KSCommon_shutdown(pobj);
	return;
}

/*	ksapi_getEPidentifiers_submit
 *	If all parameters were manually set, you can use this function to read the string
 */
OV_DLLFNCEXPORT void ksapi_getEPidentifiers_submit(
	OV_INSTPTR_ksapi_getEPidentifiers	pobj
) {
	OV_VTBLPTR_ksapi_getEPidentifiers   pvtableop; 
	OV_INSTPTR_ksapi_KSCommon pksc = Ov_StaticPtrCast(ksapi_KSCommon, pobj);

	OV_INSTPTR_ksapi_Channel channel = (OV_INSTPTR_ksapi_Channel)Ov_SearchChild(ov_containment, pobj, "channel");

	Ov_GetVTablePtr(ksapi_getEPidentifiers, pvtableop, pobj)

	if(channel == NULL) {
	    ov_logfile_error("GETEP object without Channel to use: %s", pobj->v_identifier);
	    pvtableop->m_returnMethod((OV_INSTPTR_ov_object) pobj, "no channel Object", 2);
	    return;
	}

	if((pobj->v_host) && (pobj->v_path)&& (pobj->v_namemask) && (ksapi_KSCommon_status_get(pksc) != STATUS_KSCOMMON_BUSY)){
		//all values are set
		char *xdr;
		char path[4096];
		int xdrlength;
		strcpy(path, pobj->v_path);
		
		generategetepxdr(&xdr, &xdrlength, path, (KS_OT_DOMAIN | KS_OT_LINK | KS_OT_VARIABLE), pobj->v_namemask, KS_EPF_DEFAULT);		//Don't ask for histories, since we can't parse the answer yet
	
		//print xdr
		//KSDEVEL int j;
		//KSDEVEL printf("\n\nxdr:\n");
		//KSDEVEL printf("%X %X %X %X %X %X   ", xdr[0], xdr[1], xdr[2], xdr[3], xdr[4], xdr[5]);
		//KSDEVEL for (j = 6; j < xdrlength; j=j+4)
			//KSDEVEL printf("%X %X %X %X     ", xdr[j], xdr[j+1], xdr[j+2], xdr[j+3]);
		//KSDEVEL printf("\n\n");
		//KSDEVEL for (j = 6; j < xdrlength; j=j+4)
			//KSDEVEL printf("%c %c %c %c     ", xdr[j], xdr[j+1], xdr[j+2], xdr[j+3]);
		//KSDEVEL printf("\n\n");
		//KSDEVEL printf("%s\n\n", xdr);	
	
		ksapi_Channel_sendxdr(channel, pksc, xdr, xdrlength);
	}
	else if(ksapi_KSCommon_status_get(pksc) == STATUS_KSCOMMON_BUSY)
	{
		log_error("getEPidentifiers: connection in use");
		pvtableop->m_returnMethod((OV_INSTPTR_ov_object) pobj, "connection in use", 5);
	}
	else {
		//not all values are set
		log_error("getEPidentifiers: Not all values are set. Reading cancelled");
		pvtableop->m_returnMethod((OV_INSTPTR_ov_object) pobj, "Not all values are set", 2);
	}
	return;
}


/*	getEPidentifiers_submitwrapper
 *	Wrapper to handle submit in scheduler
 */
void getEPidentifiers_submitwrapper(
	OV_INSTPTR_ov_object pobj
) {
	OV_INSTPTR_ksapi_getEPidentifiers ptmp = Ov_StaticPtrCast(ksapi_getEPidentifiers, pobj);
	ov_scheduler_unregister(pobj);
	ksapi_getEPidentifiers_submit(ptmp);

	return;
}

/*	ksapi_getEPidentifiers_setandsubmit
 *	this function sets the necessary values automatically and starts the reading
 */
OV_DLLFNCEXPORT void ksapi_getEPidentifiers_setandsubmit(
	OV_INSTPTR_ksapi_getEPidentifiers		pobj,
	OV_STRING										host,
	OV_STRING										server,
	OV_STRING										path,
	OV_STRING										namemask
) {
	OV_INSTPTR_ksapi_KSCommon pksc = Ov_StaticPtrCast(ksapi_KSCommon, pobj);
	//set values:
	if (Ov_Fail(ksapi_getEPidentifiers_namemask_set(pobj, namemask))) {
		log_error("getEPidentifiers: error setting sendstring");
		return;
	}
	if (Ov_Fail(ksapi_KSCommon_host_set(pksc, host))) {
		log_error("getEPidentifiers: error setting host");
		return;
	}
	if (Ov_Fail(ksapi_KSCommon_server_set(pksc, server))) {
		log_error("getEPidentifiers: error setting server");
		return;
	}
	if (Ov_Fail(ksapi_KSCommon_path_set(pksc, path))) {
		log_error("getEPidentifiers: error setting path");
		return;
	}
	//start reading
	ksapi_getEPidentifiers_submit(pobj);
	return;
}

OV_DLLFNCEXPORT void ksapi_getEPidentifiers_returnMethodxdr(
		OV_INSTPTR_ksapi_KSCommon			 	kscommon,
		OV_STRING									xdr,
		OV_INT										xdrlength
) {
	OV_INSTPTR_ksapi_getEPidentifiers pgEP = Ov_StaticPtrCast(ksapi_getEPidentifiers, kscommon);
	OV_VTBLPTR_ksapi_getEPidentifiers  pvtableop; 
	OV_UINT errorcode = 0;
	OV_UINT veclength, position, type;
	OV_UINT j, i;
	OV_UINT currstringlen;
	OV_STRING tempstr;
	Ov_GetVTablePtr(ksapi_getEPidentifiers, pvtableop, pgEP);



	//printxdr(xdr, xdrlength);


	errorcode = xdr[31];
	if(!errorcode)
	{
		for(j=0; j<4; j++) // read length of vec
			((char*) (&veclength))[3-j] = xdr[32+j];

		errorcode = Ov_SetDynamicVectorLength(&(pgEP->v_identifierList), veclength, STRING);
		if (Ov_Fail(errorcode))
		{
			ov_logfile_error("Error setting dynamic vector length: %s", ov_result_getresulttext(errorcode));
			Ov_SetDynamicVectorValue(&pgEP->v_identifierList, NULL, 0, STRING);
			pvtableop->m_returnMethod((OV_INSTPTR_ov_object)kscommon, "error", -1);
			return;
		}

		position = 36;			//begin of first element
		for(i=0; i<veclength; i++)	//iterate over elements
		{
			for(j=0; j<4; j++) // read type
				((char*) (&type))[3-j] = xdr[position+j];
			position +=4;		//ignore first 4 bytes
			switch(type)
			{

										//DOMAINS
			case KS_OT_DOMAIN:

				for(j=0; j<4; j++) // read length of class-string
					((char*) (&currstringlen))[3-j] = xdr[position+j];
				position += 4 + currstringlen;	//ignore class-string
				while(position%4)
					position++;

				for(j=0; j<4; j++) // read length of identifier-string
					((char*) (&currstringlen))[3-j] = xdr[position+j];
				ov_memstack_lock();
				tempstr = ov_memstack_alloc(currstringlen+1);		//use temporary string to copy since xdr is not 0-terminated
				if (!tempstr)
				{
					ov_logfile_error("Error setting tempstr: out of memory");
					Ov_SetDynamicVectorValue(&pgEP->v_identifierList, NULL, 0, STRING);
					pvtableop->m_returnMethod((OV_INSTPTR_ov_object)kscommon, "error", -1);
					ov_memstack_unlock();
					return;
				}
				for(j=0; j<currstringlen; j++)
					tempstr[j] = xdr[position+4+j];
				tempstr[currstringlen] = 0;
				errorcode = ov_string_setvalue(&(pgEP->v_identifierList.value[i]), tempstr);
				ov_memstack_unlock();
				if (Ov_Fail(errorcode))
				{
					ov_logfile_error("Error setting identifier string: %s", ov_result_getresulttext(errorcode));
					Ov_SetDynamicVectorValue(&pgEP->v_identifierList, NULL, 0, STRING);
					pvtableop->m_returnMethod((OV_INSTPTR_ov_object)kscommon, "error", -1);
					return;
				}
				position += 4 + currstringlen;
				while(position%4)
					position++;

				position += 8;		//ignore 2 xdr-fields (creation-time)

				for(j=0; j<4; j++) // read length of comment-string
					((char*) (&currstringlen))[3-j] = xdr[position+j];
				position += 4 + currstringlen;	//ignore comment-string
				while(position%4)
					position++;

				position += 4; //ignore one field (access-flags)

				position += 4; //ignore one more field (most probably semantics, but not tested since no domain with semantic flags found)
				break;

										//VARIABLES
			case KS_OT_VARIABLE:

				for(j=0; j<4; j++) // read length of unit-string
					((char*) (&currstringlen))[3-j] = xdr[position+j];
				position += 4 + currstringlen;	//ignore unit-string
				while(position%4)
					position++;

				position += 4;		//ignore vartype

				for(j=0; j<4; j++) // read length of identifier-string
					((char*) (&currstringlen))[3-j] = xdr[position+j];
				ov_memstack_lock();
				tempstr = ov_memstack_alloc(currstringlen+1);		//use temporary string to copy since xdr is not 0-terminated
				if (!tempstr)
				{
					ov_logfile_error("Error setting tempstr: out of memory");
					Ov_SetDynamicVectorValue(&pgEP->v_identifierList, NULL, 0, STRING);
					pvtableop->m_returnMethod((OV_INSTPTR_ov_object)kscommon, "error", -1);
					ov_memstack_unlock();
					return;
				}
				for(j=0; j<currstringlen; j++)
					tempstr[j] = xdr[position+4+j];
				tempstr[currstringlen] = 0;
				errorcode = ov_string_setvalue(&(pgEP->v_identifierList.value[i]), tempstr);
				ov_memstack_unlock();
				if (Ov_Fail(errorcode))
				{
					ov_logfile_error("Error setting identifier string: %s", ov_result_getresulttext(errorcode));
					Ov_SetDynamicVectorValue(&pgEP->v_identifierList, NULL, 0, STRING);
					pvtableop->m_returnMethod((OV_INSTPTR_ov_object)kscommon, "error", -1);
					return;
				}
				position += 4 + currstringlen;
				while(position%4)
					position++;

				position += 8;		//ignore 2 xdr-fields (creation-time)

				for(j=0; j<4; j++) // read length of comment-string
					((char*) (&currstringlen))[3-j] = xdr[position+j];
				position += 4 + currstringlen;	//ignore comment-string
				while(position%4)
					position++;

				position += 4; //ignore one field (access-flags)

				position += 4; //ignore one more field (semantics)
				break;

											//LINKS
			case KS_OT_LINK:

				position += 4;		//ignore linktype

				for(j=0; j<4; j++) // read length of anchor/role-string
					((char*) (&currstringlen))[3-j] = xdr[position+j];
				position += 4 + currstringlen;	//ignore anchor/role-string
				while(position%4)
					position++;

				for(j=0; j<4; j++) // read length of linkclass-string
					((char*) (&currstringlen))[3-j] = xdr[position+j];
				position += 4 + currstringlen;	//ignore linkclass-string
				while(position%4)
					position++;

				for(j=0; j<4; j++) // read length of identifier-string
					((char*) (&currstringlen))[3-j] = xdr[position+j];
				ov_memstack_lock();
				tempstr = ov_memstack_alloc(currstringlen+1);		//use temporary string to copy since xdr is not 0-terminated
				if (!tempstr)
				{
					ov_logfile_error("Error setting tempstr: out of memory");
					Ov_SetDynamicVectorValue(&pgEP->v_identifierList, NULL, 0, STRING);
					pvtableop->m_returnMethod((OV_INSTPTR_ov_object)kscommon, "error", -1);
					ov_memstack_unlock();
					return;
				}
				for(j=0; j<currstringlen; j++)
					tempstr[j] = xdr[position+4+j];
				tempstr[currstringlen] = 0;
				errorcode = ov_string_setvalue(&(pgEP->v_identifierList.value[i]), tempstr);
				ov_memstack_unlock();
				if (Ov_Fail(errorcode))
				{
					ov_logfile_error("Error setting identifier string: %s", ov_result_getresulttext(errorcode));
					Ov_SetDynamicVectorValue(&pgEP->v_identifierList, NULL, 0, STRING);
					pvtableop->m_returnMethod((OV_INSTPTR_ov_object)kscommon, "error", -1);
					return;
				}
				position += 4 + currstringlen;
				while(position%4)
					position++;

				position += 8;		//ignore 2 xdr-fields (creation-time)

				for(j=0; j<4; j++) // read length of comment-string
					((char*) (&currstringlen))[3-j] = xdr[position+j];
				position += 4 + currstringlen;	//ignore comment-string
				while(position%4)
					position++;

				position += 4; //ignore one field (access-flags)

				position += 4; //ignore one more field (most probably semantics but not verifiable right now semantics)

				break;

			default:
				pvtableop->m_returnMethod((OV_INSTPTR_ov_object)kscommon, "error", -1);
				ov_logfile_error("GetEP found other tyope than Domain, not yet implemented: %u", type);
				return;
			}
		}
		pvtableop->m_returnMethod((OV_INSTPTR_ov_object)kscommon, "Reading completed", 1);
	}
	else
	{
		pvtableop->m_returnMethod((OV_INSTPTR_ov_object)kscommon, "error", -1);
	}

	return;
}
