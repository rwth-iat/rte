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
		
		generategetepxdr(&xdr, &xdrlength, path, 15, pobj->v_namemask, 192);
	
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
	int veclength;
	int *strlength;
	//KSDEVEL int j;
	char **result;
	
	Ov_GetVTablePtr(ksapi_getEPidentifiers, pvtableop, pgEP);
	veclength = xdr[39];
	strlength = (int*)malloc(veclength*sizeof(int));
	result = (char**)malloc(veclength*sizeof(char*));
	
	//print xdr
	//KSDEVEL printf("\n\nxdr:\n");
	//KSDEVEL for (j = 0; j < xdrlength; j=j+4)
		//KSDEVEL printf("%X %X %X %X     ", xdr[j], xdr[j+1], xdr[j+2], xdr[j+3]);
	//KSDEVEL printf("\n\n");
	//KSDEVEL for (j = 0; j < xdrlength; j=j+4)
		//KSDEVEL printf("%c %c %c %c     ", xdr[j], xdr[j+1], xdr[j+2], xdr[j+3]);
	//KSDEVEL printf("\n\n");
	//KSDEVEL printf("%s\n\n", xdr);

	/*KSDEVEL //get strings
	for (i=0; i<veclength; i++)
	{
		if (xdr[pos] == 0)
		{
			pos = pos+8;
			strlength[i] = xdr[pos];
			result[i] = (char*)malloc((strlength[i]+1)*sizeof(char));
			for (j=0; j<strlength[i]; j++)
			{
				result[i][j] = xdr[pos+1+j];
				result[i][j+1] = 0;
			}
			count = strlength[i];
			while ((count%4)!=0)
				count++;
			pos = pos+count+28;
		}
		else
		{
			count = xdr[pos];
			while ((count%4)!=0)
				count++;
			pos = pos+count+8;
			strlength[i] = xdr[pos];
			result[i] = (char*)malloc((strlength[i]+1)*sizeof(char));
			for (j=0; j<strlength[i]; j++)
			{
				result[i][j] = xdr[pos+1+j];
				result[i][j+1] = 0;
			}
			count = strlength[i];
			while ((count%4)!=0)
				count++;
			pos = pos+count+28;
		}
	}
	
	//set to object
	//	Set length of the string vector
	res = Ov_SetDynamicVectorLength(&pgEP->v_identifierList, veclength, STRING);
	if (Ov_Fail(res))
	{	
		pvtableop->m_returnMethod(pobj, ov_result_getresulttext(res), 9);
		return;
	}
	//	set Values
	for (i = 0; i < veclength; i++)
	{
		res = ov_string_setvalue(&(pgEP->v_identifierList.value[i]), result[i]);
		if (Ov_Fail(res))
		{
			Ov_SetDynamicVectorValue(&pgEP->v_identifierList, NULL, 0, STRING);
			pvtableop->m_returnMethod(pobj, ov_result_getresulttext(res),9);
			return;
		}
	}*/
	
	pvtableop->m_returnMethod((OV_INSTPTR_ov_object)kscommon, "Reading completed", 1);

	free(strlength);
	free (result);
	
	return;
}
