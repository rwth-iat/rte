
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


OV_DLLFNCEXPORT OV_RESULT ksbase_Manager_register(
	OV_STRING name,
	OV_INT version,
	OV_STRING_VEC protocols,
	OV_STRING_VEC ports,
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
	
	KS_logfile_debug(("register:\n\tname: %s\n\tversion: %d\n\tttl: %d\n", name, version, timetolive));

	if(name && ks_isvalidname(name) && ports.veclen && (protocols.veclen == ports.veclen) && timetolive && pServersDomain)
	{//all values are set
		result = Ov_CreateObject(ov_domain, pNewSrvRepDomain, pServersDomain, name);
		if(Ov_OK(result))
		{//domain with servername created

			sprintf(cntstr, "%li", version);
			if(Ov_OK(Ov_CreateObject(ksbase_ServerRep, pNewSrvRep, pNewSrvRepDomain, cntstr)))
			{//object with name = version created (this structure is for compatibility with magellan and xfbspro)

				ksbase_ServerRep_name_set(pNewSrvRep, name);
				ksbase_ServerRep_version_set(pNewSrvRep, version);
				ksbase_ServerRep_port_set(pNewSrvRep, ports.value, ports.veclen);
				ksbase_ServerRep_protocols_set(pNewSrvRep, protocols.value, protocols.veclen);
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
					ksbase_ServerRep_regtime_set(pExistingSrvRep, &timenow);
					ksbase_ServerRep_port_set(pExistingSrvRep, ports.value, ports.veclen);
					ksbase_ServerRep_protocols_set(pExistingSrvRep, protocols.value, protocols.veclen);
					ksbase_ServerRep_state_set(pExistingSrvRep, 1);
					return OV_ERR_OK;
				}
				else
				{//version does not match --> error
					KS_logfile_error(("register: server with this name but different version already exits."));
					pExistingSrvRep = NULL;
					return OV_ERR_BADPARAM;
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

		KS_logfile_error(("register: Not all values are set."));
		return OV_ERR_GENERIC;
	}


	return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_RESULT ksbase_Manager_unregister(
	OV_STRING name,
	OV_INT version
) {
	OV_INSTPTR_ov_domain ServerRepDomain = (OV_INSTPTR_ov_domain)ov_path_getobjectpointer("/servers",2);
	OV_INSTPTR_ksbase_ServerRep pSrvRep = NULL;

	if((name) && (ServerRepDomain))
	{//all values are set
		pSrvRep = (OV_INSTPTR_ksbase_ServerRep)Ov_SearchChild(ov_containment, ServerRepDomain, name);
		if(pSrvRep)
		{
			Ov_DeleteObject(pSrvRep);
		}
		else
		{
			KS_logfile_error(("unregister:	entry not found"));
			return OV_ERR_GENERIC;
		}
	}
	else
	{
		KS_logfile_error(("unregister: not all values set."));
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
	char verstr[12];
	unsigned int veriterator = 0;

	if((name) && ks_isvalidname(name) && (pServersDomain))
	{//all values are set
		pSrvRepDomain = (OV_INSTPTR_ov_domain)Ov_SearchChild(ov_containment, pServersDomain, name);
		do
		{
			sprintf(verstr, "%i", veriterator);
			pSrvRep = (OV_INSTPTR_ksbase_ServerRep)Ov_SearchChild(ov_containment, pSrvRepDomain, verstr);
			if(pSrvRep)
			{
				Ov_SetDynamicVectorValue(protocols, pSrvRep->v_protocols.value, pSrvRep->v_protocols.veclen, STRING);
				Ov_SetDynamicVectorValue(ports, pSrvRep->v_port.value, pSrvRep->v_port.veclen, STRING);
				*TimeToLive = pSrvRep->v_timetolive;
				*ExpirationTime = pSrvRep->v_expirationtime;
				return OV_ERR_OK;
			}
			else
			{
				KS_logfile_error(("getserverdata: entry not found"));
				return OV_ERR_GENERIC;
			}
		}while(veriterator <= pSrvRep->v_version);
		KS_logfile_error(("getserverdata: version conflict"));
		return OV_ERR_BADVALUE;
	}
	else
	{
		KS_logfile_error(("getserverdata: not all values set."));
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

