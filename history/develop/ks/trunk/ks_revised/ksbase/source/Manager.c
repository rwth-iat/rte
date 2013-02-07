
/******************************************************************************
*
*   FILE
*   ----
*   Manager.c
*
*   History
*   -------
*   2013-01-15   File created
*
*******************************************************************************
*
*   This file is generated by the 'acplt_builder' command
*
******************************************************************************/


#ifndef OV_COMPILE_LIBRARY_ksbase
#define OV_COMPILE_LIBRARY_ksbase
#endif


#include "ksbase.h"
#include "libov/ov_macros.h"
#include "libov/ov_path.h"
#include "ksbase_helper.h"
#include "ks_logfile.h"


OV_DLLFNCEXPORT void ksbase_Manager_shutdown(
	OV_INSTPTR_ov_object 	pobj
) {
    /*
    *   local variables
    */
    OV_INSTPTR_ov_object pServerRep = NULL;
    OV_INSTPTR_ov_object pChildObject = NULL;
    OV_INSTPTR_ov_domain pServersDom = NULL;
    /* do what */

    pServersDom = (OV_INSTPTR_ov_domain)ov_path_getobjectpointer("/servers",2);
    if(pServersDom)
    {
    	/*	Iterate over all ServerReps in containment and delete them	*/
    	pChildObject = Ov_GetFirstChild(ov_containment, pServersDom);
    	while(pChildObject)
    	{
    		if(Ov_CanCastTo(ksbase_ServerRep, pChildObject))
    		{
    			pServerRep = pChildObject;
    			pChildObject = Ov_GetNextChild(ov_containment, pChildObject);
    			Ov_DeleteObject(pServerRep);
    			continue;
    		}
    		else
    		{
    			pChildObject = Ov_GetNextChild(ov_containment, pChildObject);
    			continue;
    		}
    	}
    }
    /* set the object's state to "shut down" */
    ov_object_shutdown(pobj);
    return;
}

OV_DLLFNCEXPORT OV_RESULT ksbase_Manager_register(
	OV_STRING name,
	OV_INT version,
	OV_STRING protocol,
	OV_STRING port,
	OV_UINT timetolive
) {
	OV_INSTPTR_ov_domain pNewSrvRepDomain=NULL;
	OV_INSTPTR_ov_domain pExistingSrvRepDomain=NULL;
	OV_INSTPTR_ov_domain pServersDomain = (OV_INSTPTR_ov_domain)ov_path_getobjectpointer("/servers",2);
	OV_INSTPTR_ksbase_ServerRep pNewSrvRep = NULL;
	OV_INSTPTR_ksbase_ServerRep pExistingSrvRep = NULL;
	OV_TIME timenow;
	char cntstr[11];
	OV_RESULT result;
	unsigned int i;

	
	KS_logfile_debug(("register:\n\tname: %s\n\tversion: %d\n\tttl: %d\n", name, version, timetolive));

	if(name && ks_isvalidname(name) && protocol && port && timetolive && pServersDomain)
	{//all values are set
		result = Ov_CreateObject(ov_domain, pNewSrvRepDomain, pServersDomain, name);
		if(Ov_OK(result))
		{//domain with servername created

			sprintf(cntstr, "%li", version);
			if(Ov_OK(Ov_CreateObject(ksbase_ServerRep, pNewSrvRep, pNewSrvRepDomain, cntstr)))
			{//object with name = version created (this structure is for compatibility with magellan and xfbspro)
					/*	at this point we only have one protocol	*/
				ksbase_ServerRep_name_set(pNewSrvRep, name);
				ksbase_ServerRep_version_set(pNewSrvRep, version);
				ksbase_ServerRep_port_set(pNewSrvRep, &port, 1);
				ksbase_ServerRep_protocols_set(pNewSrvRep, &protocol, 1);
				ksbase_ServerRep_timetolive_set(pNewSrvRep, timetolive);
				ov_time_gettime(&timenow);
				ksbase_ServerRep_regtime_set(pNewSrvRep, &timenow);
				ksbase_ServerRep_state_set(pNewSrvRep, 1);

			}
			else
			{
				KS_logfile_error(("could not create Server Representative"));
				return OV_ERR_GENERIC;
			}
		}
		else if(result == OV_ERR_ALREADYEXISTS)
		{//domain with servername already exists

			//get pointer to domain with servername
			pExistingSrvRepDomain = (OV_INSTPTR_ov_domain)Ov_SearchChild(ov_containment, pServersDomain, name);
			pExistingSrvRep = NULL;

			sprintf(cntstr, "%li", version);
			pExistingSrvRep = (OV_INSTPTR_ksbase_ServerRep)Ov_SearchChild(ov_containment, pExistingSrvRepDomain, cntstr);
			if(pExistingSrvRep)
			{
				//check if sever version matches to existing one
				if((version == pExistingSrvRep->v_version))
				{//version match --> update expiration time

					ksbase_ServerRep_timetolive_set(pExistingSrvRep, timetolive);
					ov_time_gettime(&timenow);
					for(i=0; i<pExistingSrvRep->v_protocols.veclen; i++)
						if(ov_string_compare(pExistingSrvRep->v_protocols.value[i], protocol) == OV_STRCMP_EQUAL)
							break;
					if(i>=pExistingSrvRep->v_protocols.veclen)
					{/*	protocol not found --> add it and reregister	*/
						result = Ov_SetDynamicVectorLength(&(pExistingSrvRep->v_protocols), pExistingSrvRep->v_protocols.veclen + 1, STRING);
						if(Ov_Fail(result))
							return result;
						result = ov_string_setvalue(&(pExistingSrvRep->v_protocols.value[pExistingSrvRep->v_protocols.veclen-1]), protocol);
						if(Ov_Fail(result))
							return result;
						result = Ov_SetDynamicVectorLength(&(pExistingSrvRep->v_port), pExistingSrvRep->v_port.veclen + 1, STRING);
						if(Ov_Fail(result))
							return result;
						result = ov_string_setvalue(&(pExistingSrvRep->v_port.value[pExistingSrvRep->v_port.veclen-1]), port);
						if(Ov_Fail(result))
							return result;
					}/*	protocol found --> just reregister	*/
					ksbase_ServerRep_regtime_set(pExistingSrvRep, &timenow);
					ksbase_ServerRep_state_set(pExistingSrvRep, 1);
					return OV_ERR_OK;
				}
				else
				{//version does not match --> error
					KS_logfile_info(("register: server with this name but different version already exits."));
					pExistingSrvRep = NULL;
					return KS_ERR_BADVALUE;
				}
			}
			else
			{
				KS_logfile_error(("register: unknown error."));
				return OV_ERR_GENERIC;
			}
		}
	}
	else
	{//not all values are set

		KS_logfile_warning(("register: Not all values are set."));
		return OV_ERR_GENERIC;
	}


	return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_RESULT ksbase_Manager_unregister(
	OV_STRING name,
	OV_INT version,
	OV_STRING protocol
) {
	OV_INSTPTR_ov_domain ServerRepDomain = (OV_INSTPTR_ov_domain)ov_path_getobjectpointer("/servers",2);
	OV_INSTPTR_ksbase_ServerRep pSrvRep = NULL;
	unsigned int i;
	OV_RESULT result;

	if((name) && (ServerRepDomain))
	{//all values are set
		pSrvRep = (OV_INSTPTR_ksbase_ServerRep)Ov_SearchChild(ov_containment, ServerRepDomain, name);
		if(pSrvRep)
		{
			if((pSrvRep->v_protocols.veclen == 1))
			{
				if((ov_string_compare(pSrvRep->v_protocols.value[0], protocol) == OV_STRCMP_EQUAL))
					Ov_DeleteObject(pSrvRep);	/*	it is the only protocol --> delete Server Representative	*/
				else
				{
					KS_logfile_info(("unregister: protocol not found"));
					return OV_ERR_BADVALUE;
				}
			}
			else
			{
				for(i=0; i < pSrvRep->v_protocols.veclen; i++)
					if((ov_string_compare(pSrvRep->v_protocols.value[i], protocol) == OV_STRCMP_EQUAL))
						break;
				if(i < pSrvRep->v_protocols.veclen)
				{/*	protocol found --> delete it from list	*/
					for(;i<pSrvRep->v_protocols.veclen-1; i++)
					{
						pSrvRep->v_protocols.value[i] = pSrvRep->v_protocols.value[i+1];
						pSrvRep->v_protocols.value[i] = pSrvRep->v_port.value[i+1];
					}
					result = Ov_SetDynamicVectorLength(&(pSrvRep->v_protocols), pSrvRep->v_protocols.veclen - 1, STRING);	/*	frees the last element and does not touch the rest	*/
					if(Ov_Fail(result))
						return result;
					result = Ov_SetDynamicVectorLength(&(pSrvRep->v_port), pSrvRep->v_port.veclen - 1, STRING);
					if(Ov_Fail(result))
						return result;
				}
				else
				{
					KS_logfile_info(("unregister: protocol not found"));
					return OV_ERR_BADVALUE;
				}
			}
		}
		else
		{
			KS_logfile_info(("unregister: entry not found"));
			return OV_ERR_BADVALUE;
		}
	}
	else
	{
		KS_logfile_warning(("unregister: not all values set."));
		return OV_ERR_BADPARAM;
	}

return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_RESULT ksbase_Manager_getserverdata(
	OV_STRING name,
	OV_INT version,
	OV_STRING_VEC* protocols,
	OV_STRING_VEC* ports,
	OV_UINT* TimeToLive,
	OV_TIME* ExpirationTime
) {
	OV_INSTPTR_ov_domain pServersDomain = (OV_INSTPTR_ov_domain)ov_path_getobjectpointer("/servers",2);
	OV_INSTPTR_ov_domain pSrvRepDomain = NULL;
	OV_INSTPTR_ksbase_ServerRep pSrvRep = NULL;

	if((name)
			&& ks_isvalidname(name)
			&& (pServersDomain)
			&& protocols
			&& ports
			&& TimeToLive
			&& ExpirationTime)
	{//all values are set
		pSrvRepDomain = (OV_INSTPTR_ov_domain)Ov_SearchChild(ov_containment, pServersDomain, name);
		if(pSrvRepDomain)
		{
			Ov_ForEachChildEx(ov_containment, pSrvRepDomain, pSrvRep, ksbase_ServerRep)
			{
				if(pSrvRep->v_version >= version && atoi(pSrvRep->v_identifier) >= version)
				{
					Ov_SetDynamicVectorValue(protocols, pSrvRep->v_protocols.value, pSrvRep->v_protocols.veclen, STRING);
					Ov_SetDynamicVectorValue(ports, pSrvRep->v_port.value, pSrvRep->v_port.veclen, STRING);
					*TimeToLive = pSrvRep->v_timetolive;
					*ExpirationTime = pSrvRep->v_expirationtime;
					return OV_ERR_OK;
				}
			}

			KS_logfile_info(("getserverdata: entry not found"));
			return KS_ERR_SERVERUNKNOWN;
		}
		KS_logfile_info(("getserverdata: entry not found"));
		return KS_ERR_SERVERUNKNOWN;
	}
	else
	{
		KS_logfile_warning(("getserverdata: not all values set."));
		return OV_ERR_BADPARAM;
	}

}

OV_DLLFNCEXPORT OV_ACCESS ksbase_Manager_getaccess(
	OV_INSTPTR_ov_object		pobj,
	const OV_ELEMENT			*pelem,
	const OV_TICKET				*pticket
) {
	/*
	*	local variables
	*/

	/*
	*	switch based on the element's type
	*/
	switch(pelem->elemtype) {
		case OV_ET_VARIABLE:
			if(pelem->elemunion.pvar->v_offset >= offsetof(OV_INST_ov_object,__classinfo)) {
			  if(pelem->elemunion.pvar->v_vartype == OV_VT_CTYPE)
				  return OV_AC_NONE;
			  else
				  return OV_AC_READWRITE;
			}
			break;
		default:
			break;
	}

	return ov_object_getaccess(pobj, pelem, pticket);
}

