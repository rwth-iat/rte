
/******************************************************************************
*
*   FILE
*   ----
*   xdrManagerCom.c
*
*   History
*   -------
*   2013-02-07   File created
*
*******************************************************************************
*
*   This file is generated by the 'acplt_builder' command
*
******************************************************************************/


#ifndef OV_COMPILE_LIBRARY_ksxdr
#define OV_COMPILE_LIBRARY_ksxdr
#endif


#include "ksxdr.h"
#include "libov/ov_macros.h"
#include "libov/ov_vendortree.h"
#include "libov/ov_result.h"
#include "ks_logfile.h"
#include "ksxdr_config.h"
#include "ksbase_helper.h"
#include "KSDATAPACKET_xdrhandling.h"
#include "ksxdr_services.h"





static OV_UINT pmXID = 0x4359f4a3;



void ksxdr_xdrManagercom_Callback(OV_INSTPTR_ov_domain instanceCalled, OV_INSTPTR_ov_domain instanceCalling);

OV_RESULT generate_PMAPPROC(KS_DATAPACKET* datapacket, OV_BOOL usesStreamProtocol, OV_UINT command, OV_UINT protocol, OV_UINT port){
	OV_RESULT result = OV_ERR_OK;
	OV_UINT valToSet;

	do{
		if(usesStreamProtocol) {
			valToSet = 0x80000038;
			result = KS_DATAPACKET_write_xdr_uint(datapacket, &valToSet);
			if(Ov_Fail(result))
				break;
		}
		//	xid
		result = KS_DATAPACKET_write_xdr_uint(datapacket, &pmXID);
		pmXID++;
		if(Ov_Fail(result))
			break;
		//rpc Call 0
		valToSet = 0;
		result = KS_DATAPACKET_write_xdr_uint(datapacket, &valToSet);
		if(Ov_Fail(result))
			break;
		//rpc Version
		valToSet = PM_VERSION;
		result = KS_DATAPACKET_write_xdr_uint(datapacket, &valToSet);
		if(Ov_Fail(result))
			break;
		//program number of portmapper (Dez 100000)
		valToSet = PM_PROGNUMBER;
		result = KS_DATAPACKET_write_xdr_uint(datapacket, &valToSet);
		if(Ov_Fail(result))
			break;
		//portmap protocol version
		valToSet = PM_VERSION;
		result = KS_DATAPACKET_write_xdr_uint(datapacket, &valToSet);
		if(Ov_Fail(result))
			break;
		//portmap procedure number
		result = KS_DATAPACKET_write_xdr_uint(datapacket, &command);
		if(Ov_Fail(result))
			break;
		//24 - 39: 16byte auth data (twice flavour (0) and 0 length data)
		valToSet = 0;
		result = KS_DATAPACKET_write_xdr_uint(datapacket, &valToSet);
		if(Ov_Fail(result))
			break;
		result = KS_DATAPACKET_write_xdr_opaque(datapacket, NULL, 0);
		if(Ov_Fail(result))
			break;
		valToSet = 0;
		result = KS_DATAPACKET_write_xdr_uint(datapacket, &valToSet);
		if(Ov_Fail(result))
			break;
		result = KS_DATAPACKET_write_xdr_opaque(datapacket, NULL, 0);
		if(Ov_Fail(result))
			break;
		//ks program number
		valToSet = KSXDR_PROGRAMNUMBER;
		result = KS_DATAPACKET_write_xdr_uint(datapacket, &valToSet);
		if(Ov_Fail(result))
			break;
		//ks version number
		valToSet = KSXDR_PROGRAMVERSION;
		result = KS_DATAPACKET_write_xdr_uint(datapacket, &valToSet);
		if(Ov_Fail(result))
			break;
		//protocol ident
		result = KS_DATAPACKET_write_xdr_uint(datapacket, &protocol);
		if(Ov_Fail(result))
			break;
		//port - ignored at unset
		result = KS_DATAPACKET_write_xdr_uint(datapacket, &port);
	}while(0);
	return result;
}

OV_RESULT process_getPort(KS_DATAPACKET* datapacket, OV_BOOL usesStreamProtocol, OV_STRING* port){
	OV_RESULT result;
	OV_UINT readVar;
	OV_STRING	OptValTemp = NULL;

	if(usesStreamProtocol == TRUE){
		//	discard header
		KS_DATAPACKET_read_xdr_uint(datapacket, &readVar);
	}
	result = KS_DATAPACKET_read_xdr_uint(datapacket, &readVar);
	if(readVar == (pmXID - 1)){
		//	got a valid answer
		//	discard 5 entries (reply, accept, auth, auth, result)
		result |= KS_DATAPACKET_read_xdr_uint(datapacket, &readVar);
		result |= KS_DATAPACKET_read_xdr_uint(datapacket, &readVar);
		result |= KS_DATAPACKET_read_xdr_uint(datapacket, &readVar);
		result |= KS_DATAPACKET_read_xdr_uint(datapacket, &readVar);
		result |= KS_DATAPACKET_read_xdr_uint(datapacket, &readVar);
		//	read port
		if(Ov_OK(result)){
			if(Ov_Fail(KS_DATAPACKET_read_xdr_uint(datapacket, &readVar))){
				readVar = 0;
				return OV_ERR_GENERIC;
			}
		} else {
			KS_logfile_error(("error parsing portmapper answer"));
			return result;
		}
		if(port){
			if(!readVar){
				readVar = 7509;
			}
			ov_memstack_lock();
			OptValTemp = ov_memstack_alloc(12);
			sprintf(OptValTemp, "%" OV_PRINT_UINT, readVar);
			ov_string_setvalue(port, OptValTemp);
			ov_memstack_unlock();
		}
	}
	return OV_ERR_OK;
}

static OV_RESULT  ksxdr_xdrManagerCom_getOwnPort(OV_INSTPTR_ksxdr_xdrManagerCom thisMngCom){
	OV_INT							port = -1;
	OV_STRING						OptValTemp = NULL;
	OV_INSTPTR_ksbase_ComTask		pListener = NULL;
	OV_INSTPTR_ov_class				pClIterator = NULL;
	OV_ELEMENT						elemPort;
	OV_ELEMENT						ListenerElement;

	/*	if the OwnPort is not set check commandline options and if they are not set check for a TCPListener (default binding)	*/
	if(!thisMngCom->v_OwnPort || !(*thisMngCom->v_OwnPort) || ov_string_compare(thisMngCom->v_OwnPort, "-1") == OV_STRCMP_EQUAL) {
		ov_memstack_lock();
		port = ov_vendortree_getport();
		if(port < 0 )// PORT not set
		{
			//check environment Variable OWNPORT (for compatibility reasons)
			if (getenv("OWNPORT")) //OWNPORT set
			{
				OptValTemp = getenv("OWNPORT");
				ov_string_setvalue(&thisMngCom->v_OwnPort, OptValTemp);
			} else {	//check option KS_PORT
				OptValTemp = ov_vendortree_getcmdlineoption_value("KS_PORT");
				if(OptValTemp && *OptValTemp) {
					ov_string_setvalue(&thisMngCom->v_OwnPort, OptValTemp);
				} else	/*	check the TCPListener and UDPListener (default binding) WITHOUT creating a dependency on TCPBind or UDPbind	*/
				{	/*	get Listener class	*/
					/*	ov_searchchild does not work here, as ov_instanciation lacks the property OV_AP_LOCAL	*/
					pClIterator = Ov_GetFirstChild(ov_inheritance, pclass_ksbase_ComTask);
					while(pClIterator) {
						if((ov_string_compare(pClIterator->v_identifier, "TCPListener") == OV_STRCMP_EQUAL)
								|| (ov_string_compare(pClIterator->v_identifier, "UDPListener") == OV_STRCMP_EQUAL))
							break;
						pClIterator = Ov_GetNextChild(ov_inheritance, pClIterator);
					} if(!pClIterator) {
						KS_logfile_error(("%s: typemethod: OwnPort not set and Listener class (TCP or UDP) not found. retry in 2 minutes.", thisMngCom->v_identifier));
						thisMngCom->v_cycInterval = 120000;	/*	retry in 2 minutes	*/
						ov_memstack_unlock();
						return OV_ERR_GENERIC;
					}
					/*	get Listener instance	*/
					pListener = Ov_StaticPtrCast(ksbase_ComTask, Ov_GetFirstChild(ov_instantiation, pClIterator));
					if(!pListener) {
						KS_logfile_error(("%s: typemethod: OwnPort not set and Listener instance (TCP or UDP) not found. retry in 2 minutes.", thisMngCom->v_identifier));
						thisMngCom->v_cycInterval = 120000;	/*	retry in 2 minutes	*/
						ov_memstack_unlock();
						return OV_ERR_GENERIC;
					}

					/*	read out port variable	*/
					ListenerElement.elemtype = OV_ET_OBJECT;
					ListenerElement.pobj = Ov_StaticPtrCast(ov_object, pListener);

					if(Ov_Fail(ov_element_searchpart(&ListenerElement, &elemPort, OV_ET_VARIABLE, "port"))) {
						KS_logfile_error(("%s: typemethod: element port not found. deactivating registration.", thisMngCom->v_identifier));
						ov_memstack_unlock();
						thisMngCom->v_actimode = 0;
						return OV_ERR_GENERIC;
					}
					if(elemPort.elemtype == OV_ET_VARIABLE) {
						if(elemPort.pvalue) {
							port = *((OV_INT*) elemPort.pvalue);
						} else {
							KS_logfile_error(("element value* is NULL"));
						}
						OptValTemp = ov_memstack_alloc(12);	/*	this way prevent us from NULL-pointer exceptions in ov_string_print	*/
						if(OptValTemp) {
							sprintf(OptValTemp, "%" OV_PRINT_INT, port);
							ov_string_setvalue(&thisMngCom->v_OwnPort, OptValTemp);
						} else {
							KS_logfile_error(("%s typemethod: could not set port: HEAPOUTOFMEMORY", thisMngCom->v_identifier));
						}
					} else {
						KS_logfile_error(("%s: typemethod: could not obtain port from Listener. deactivating registration.", thisMngCom->v_identifier));
						ov_memstack_unlock();
						thisMngCom->v_actimode = 0;
						return OV_ERR_GENERIC;
					}
				}
			}
		} else {
			OptValTemp = ov_memstack_alloc(12);
			sprintf(OptValTemp, "%" OV_PRINT_INT, port);
			ov_string_setvalue(&thisMngCom->v_OwnPort, OptValTemp);
		}
		ov_memstack_unlock();
	}
	return OV_ERR_OK;
}

OV_DLLFNCEXPORT void ksxdr_xdrManagerCom_startup(
	OV_INSTPTR_ov_object 	pobj
) {
    /*    
    *   local variables
    */
    OV_INSTPTR_ksxdr_xdrManagerCom	pinst = Ov_StaticPtrCast(ksxdr_xdrManagerCom, pobj);
    OV_INSTPTR_ksbase_Channel		pChannel = NULL;
    OV_VTBLPTR_ksbase_Channel		pVtblChannel = NULL;
    OV_INSTPTR_ov_class				pClIterator = NULL;
    OV_RESULT						result;
    OV_TIME							now;
    OV_TIME							endTime;
    /* do what the base class does first */
    ov_object_startup(pobj);

    /* do what */
    pinst->v_cycInterval = 5000; /*	cycle every 5 seconds at first	*/
    pinst->v_actimode = 1;
    pinst->v_Tries = 0;
    pinst->v_OwnPort = 0;
    pinst->v_PmReqSent = FALSE;
    if(Ov_Fail(ksxdr_xdrManagerCom_getOwnPort(pinst))){
    	return;
    }
    if(!Ov_GetFirstChild(ov_instantiation, pclass_ksbase_Manager)){
    	if((pinst->v_PmReqSent == FALSE) && (pinst->v_PmAsk == TRUE)){
    		Ov_ForEachChildEx(ov_containment, pinst, pChannel, ksbase_Channel){
    			if((ov_string_compare(Ov_GetParent(ov_instantiation, pChannel)->v_identifier, "UDPChannel") == OV_STRCMP_EQUAL)
    					|| ov_string_compare(Ov_GetParent(ov_instantiation, pChannel)->v_identifier, "TCPChannel") == OV_STRCMP_EQUAL){
    				break;
    			}
    		}
    		if(!pChannel){
    			pClIterator = Ov_GetFirstChild(ov_inheritance, pclass_ksbase_Channel);
    			while(pClIterator) {
    				if((ov_string_compare(pClIterator->v_identifier, "UDPChannel") == OV_STRCMP_EQUAL))
    					break;
    				pClIterator = Ov_GetNextChild(ov_inheritance, pClIterator);
    			}
    			if(!pClIterator) {
    				pClIterator = Ov_GetFirstChild(ov_inheritance, pclass_ksbase_Channel);
    				while(pClIterator) {
    					if((ov_string_compare(pClIterator->v_identifier, "TCPChannel") == OV_STRCMP_EQUAL))
    						break;
    					pClIterator = Ov_GetNextChild(ov_inheritance, pClIterator);
    				}
    				if(!pClIterator) {
    					KS_logfile_info(("%s: startup: Neither UDPChannel nor TCPChannel found. Not asking portmapper", pinst->v_identifier));
    					pinst->v_PmAsk = FALSE;
    				}
    			}
    			if(pClIterator){
    				result = ov_class_createIDedObject(pClIterator, Ov_PtrUpCast(ov_domain, pinst),
    						"Channel", OV_PMH_DEFAULT, NULL, NULL, NULL, (OV_INSTPTR_ov_object*) &pChannel);
    				if(Ov_Fail(result)){
    					ov_memstack_lock();
    					KS_logfile_error(("%s: startup: Could not create channel. Reason: %s", pinst->v_identifier, ov_result_getresulttext(result)));
    					ov_memstack_unlock();
    				}
    			}
    		}
    		if(pChannel){
    			Ov_GetVTablePtr(ksbase_Channel, pVtblChannel, pChannel);
    			if(!pVtblChannel){
    				KS_logfile_error(("%s: startup: Could not get vtable of channel", pinst->v_identifier));
    				pinst->v_PmAsk = FALSE;
    			} else {
    				//	Do NOT use "localhost" as this may open an IPV6 connection which the portmapper probably can't handle
    				if(Ov_OK(pVtblChannel->m_OpenConnection(pChannel, "127.0.0.1", PM_PORT))){
    					//	empty output buffer
    					ksbase_free_KSDATAPACKET(&(pChannel->v_outData));
    					//	create getport
    					result = generate_PMAPPROC(&(pChannel->v_outData), pChannel->v_usesStreamProtocol, PM_FUNC_GETPORT, PM_PROT_TCP, 0);
    					if(Ov_Fail(result)){
    						KS_logfile_error(("%s: startup: Generation of getport call failed.", pinst->v_identifier));
    					} else if(Ov_Fail(pVtblChannel->m_SendData(pChannel))){
    						ov_memstack_lock();
    						KS_logfile_error(("%s: startup: Sending getport to portmapper failed. Reason: %s", pinst->v_identifier, ov_result_getresulttext(result)));
    						ov_memstack_unlock();
    					} else {
    						//	call the channels typemethod in a loopto receive data
    						//	do this 3 seconds tops
    						ov_time_gettime(&endTime);
    						endTime.secs += 3;
    						do{
    							pVtblChannel->m_typemethod(Ov_PtrUpCast(ksbase_ComTask, pChannel));
    							if(pChannel->v_inData.length){
    								//	we received something
    								process_getPort(&pChannel->v_inData, pChannel->v_usesStreamProtocol, &pinst->v_ManagerPort);
    								break;
    							}
    							ov_time_gettime(&now);
    						}while(ov_time_compare(&now, &endTime) != OV_TIMECMP_AFTER);
    					}
    					pVtblChannel->m_CloseConnection(pChannel);
    					ksbase_free_KSDATAPACKET(&(pChannel->v_inData));
    					ksbase_free_KSDATAPACKET(&(pChannel->v_outData));
    					pinst->v_PmReqSent = TRUE;
    				}else{
    					pinst->v_PmAsk = FALSE;
    				}
    			}
    		}
    	}
    }
    return;
}

OV_DLLFNCEXPORT void ksxdr_xdrManagerCom_shutdown(
	OV_INSTPTR_ov_object 	pobj
) {
    /*    
    *   local variables
    */
	OV_INSTPTR_ksxdr_xdrManagerCom	thisMngCom = Ov_StaticPtrCast(ksxdr_xdrManagerCom, pobj);
	OV_INSTPTR_ksbase_Manager		pManager = NULL;
	OV_INSTPTR_ksxdr_xdrClient		xdrClient = NULL;
	OV_ANY							servername;
	OV_INSTPTR_ksbase_Channel		pChannel = NULL;
	OV_VTBLPTR_ksbase_Channel		pVtblChannel = NULL;
	OV_RESULT result;

	if(thisMngCom->v_UseShortCut == TRUE) {

		KS_logfile_debug(("%s: unregister: using shortCut", thisMngCom->v_identifier));
		pManager = Ov_StaticPtrCast(ksbase_Manager, Ov_GetFirstChild(ov_instantiation, pclass_ksbase_Manager));
		if(!pManager) {
			KS_logfile_error(("%s: shutdown: no Manager here. Cannot unregister", thisMngCom->v_identifier));
			return;
		}

		KS_logfile_debug(("%s: unregistering MANAGER ksxdr protocol", thisMngCom->v_identifier));
		if(Ov_Fail(ksbase_Manager_unregister("MANAGER", 2, KSXDR_IDENTIFIER))) {
			thisMngCom->v_RegisterState = XDR_MNGRCOM_REGISTERSTATE_ERROR;	/*	set register error	*/
			thisMngCom->v_cycInterval = 5000;	/*	5 seconds cyctime for next start up	*/
			thisMngCom->v_Tries = 0;		/*	for next start up reset counter	*/
		} else {
			thisMngCom->v_RegisterState = XDR_MNGRCOM_REGISTERSTATE_NOTREGISTERED;	/*	set state to unregistered	*/
			thisMngCom->v_cycInterval = 5000;	/*	5 seconds cyctime for next start up	*/
			thisMngCom->v_Tries = 0;
		}
		if(thisMngCom->v_PmRegister == TRUE){
			/*
			 * register at portmapper
			 * create a UDPChannel (if not there already)
			 * send an unset to portmapper
			 * send a set for tcp (if there is a listener)
			 * send a set for udp
			 */
			Ov_ForEachChildEx(ov_containment, thisMngCom, pChannel, ksbase_Channel){
				if(ov_string_compare(Ov_GetParent(ov_instantiation, pChannel)->v_identifier, "UDPChannel") == OV_STRCMP_EQUAL){
					break;
				}
			}
			if(pChannel){
				Ov_GetVTablePtr(ksbase_Channel, pVtblChannel, pChannel);
				if(!pVtblChannel){
					KS_logfile_error(("%s: typemethod: Could not get vtable of channel", thisMngCom->v_identifier));
					thisMngCom->v_PmRegister = FALSE;
				} else {
					//	Do NOT use "localhost" as this may open an IPV6 connection which the portmapper probably can't handle
					if(Ov_OK(pVtblChannel->m_OpenConnection(pChannel, "127.0.0.1", PM_PORT))){
						//	empty output buffer
						ksbase_free_KSDATAPACKET(&(pChannel->v_outData));
						//	create unset
						result = generate_PMAPPROC(&(pChannel->v_outData), pChannel->v_usesStreamProtocol, PM_FUNC_UNSET, 0, 0);
						if(Ov_Fail(result)){
							KS_logfile_error(("%s: typemethod: Generation of unset call failed.", thisMngCom->v_identifier));
						} else if(Ov_Fail(pVtblChannel->m_SendData(pChannel))){
							ov_memstack_lock();
							KS_logfile_error(("%s: typemethod: Sending unset to portmapper failed. Reason: %s", thisMngCom->v_identifier, ov_result_getresulttext(result)));
							ov_memstack_unlock();
						}
						pVtblChannel->m_CloseConnection(pChannel);
						ksbase_free_KSDATAPACKET(&(pChannel->v_inData));
						ksbase_free_KSDATAPACKET(&(pChannel->v_outData));
					}else{
						//errors are not interesting in shutdown
					}
				}
			}
		}
		return;
	} else
	{/*	find the first xdrClient in containment to request unregister	*/
		xdrClient = Ov_StaticPtrCast(ksxdr_xdrClient, Ov_GetFirstChild(ov_containment, thisMngCom));
		if(xdrClient) {
			while(xdrClient && (Ov_GetClassPtr(xdrClient) != pclass_ksxdr_xdrClient))
				xdrClient = Ov_StaticPtrCast(ksxdr_xdrClient, Ov_GetNextChild(ov_containment, xdrClient));
		}

		servername.value.vartype = OV_VT_VOID;
		servername.value.valueunion.val_string = NULL;
		ov_vendortree_getservername(&servername, NULL);

		if(xdrClient)
			ksxdr_xdrClient_requestUnRegister(Ov_StaticPtrCast(ksbase_ClientBase, xdrClient), NULL, servername.value.valueunion.val_string, 2, NULL, NULL);

	}
	thisMngCom->v_RegisterState = XDR_MNGRCOM_REGISTERSTATE_NOTREGISTERED;	/*	set state to unregistered	*/
	thisMngCom->v_cycInterval = 5000;	/*	5 seconds cyctime for next start up	*/
	thisMngCom->v_Tries = 0;


    /* set the object's state to "shut down" */
    ov_object_shutdown(pobj);

    return;
}



OV_DLLFNCEXPORT void ksxdr_xdrManagerCom_typemethod (
	OV_INSTPTR_ksbase_ComTask	this
) {
    /*
    *   local variables
    */
	OV_INSTPTR_ksxdr_xdrManagerCom	thisMngCom = Ov_StaticPtrCast(ksxdr_xdrManagerCom, this);
	OV_INSTPTR_ksbase_Manager 		pManager = NULL;
	OV_INSTPTR_ov_class				pClIterator = NULL;
	OV_ANY							servername;
	OV_INSTPTR_ksxdr_xdrClient		xdrClient = NULL;
	OV_RESULT						result;
	OV_INSTPTR_ksbase_Channel		pChannel = NULL;
	OV_VTBLPTR_ksbase_Channel		pVtblChannel = NULL;

	switch(thisMngCom->v_RegisterState)
	{

	case XDR_MNGRCOM_REGISTERSTATE_NOTREGISTERED:	/*	if not registered or registered successfully (in latter case typemethod is slowed down to TTL * 0.9) or error (in this case typemethod is slowed down for retries)	*/
	case XDR_MNGRCOM_REGISTERSTATE_REGISTERED:
	case XDR_MNGRCOM_REGISTERSTATE_ERROR:
		if(thisMngCom->v_Tries < 3) {
			if(Ov_Fail(ksxdr_xdrManagerCom_getOwnPort(thisMngCom))){
				return;
			}
			/*************************************************************************************************************************************************************
			 * here we definitely know the port
			 ************************************************************************************************************************************************************/

			if(thisMngCom->v_UseShortCut == TRUE) {
				KS_logfile_debug(("%s: typemethod: using shortCut", this->v_identifier));
				pManager = Ov_StaticPtrCast(ksbase_Manager, Ov_GetFirstChild(ov_instantiation, pclass_ksbase_Manager));
				if(!pManager) {
					KS_logfile_error(("%s: typemethod: no Manager here. Cannot register", this->v_identifier));
					return;
				}

				KS_logfile_debug(("%s: registering MANAGER ksxdr protocol on port %s", thisMngCom->v_identifier, thisMngCom->v_OwnPort));
				if(Ov_Fail(ksbase_Manager_register("MANAGER", 2, KSXDR_IDENTIFIER, thisMngCom->v_OwnPort, 30))) {
					thisMngCom->v_RegisterState = XDR_MNGRCOM_REGISTERSTATE_ERROR;	/*	set register error	*/
					thisMngCom->v_Tries++;
				} else {
					thisMngCom->v_RegisterState = XDR_MNGRCOM_REGISTERSTATE_REGISTERED;	/*	set state to registered and slow down typemethod	*/
					thisMngCom->v_cycInterval = 30000;	/*	re-register every 30 seconds (assuming rootcomtask runs at 1 msec)	*/
					thisMngCom->v_Tries = 0;
				}
				if((thisMngCom->v_PmReqSent == FALSE) && (thisMngCom->v_PmRegister == TRUE)){
					/*
					 * register at portmapper
					 * create a UDPChannel (if not there already)
					 * send an unset to portmapper
					 * send a set for tcp (if there is a listener)
					 * send a set for udp
					 */
					Ov_ForEachChildEx(ov_containment, this, pChannel, ksbase_Channel){
						if((ov_string_compare(Ov_GetParent(ov_instantiation, pChannel)->v_identifier, "UDPChannel") == OV_STRCMP_EQUAL)
								|| ov_string_compare(Ov_GetParent(ov_instantiation, pChannel)->v_identifier, "TCPChannel") == OV_STRCMP_EQUAL){
							break;
						}
					}
					if(!pChannel){
						pClIterator = Ov_GetFirstChild(ov_inheritance, pclass_ksbase_Channel);
						while(pClIterator) {
							if((ov_string_compare(pClIterator->v_identifier, "UDPChannel") == OV_STRCMP_EQUAL))
								break;
							pClIterator = Ov_GetNextChild(ov_inheritance, pClIterator);
						} if(!pClIterator) {
							pClIterator = Ov_GetFirstChild(ov_inheritance, pclass_ksbase_Channel);
							while(pClIterator) {
								if((ov_string_compare(pClIterator->v_identifier, "TCPChannel") == OV_STRCMP_EQUAL))
									break;
								pClIterator = Ov_GetNextChild(ov_inheritance, pClIterator);
							} if(!pClIterator) {
								KS_logfile_info(("%s: typemethod: Neither UDPChannel nor TCPChannel found. Not registering at portmapper", this->v_identifier));
								thisMngCom->v_PmRegister = FALSE;
							}
						}
						if(pClIterator){

							result = ov_class_createIDedObject(pClIterator, Ov_PtrUpCast(ov_domain, this),
									"Channel", OV_PMH_DEFAULT, NULL, NULL, NULL, (OV_INSTPTR_ov_object*) &pChannel);
							if(Ov_Fail(result)){
								ov_memstack_lock();
								KS_logfile_error(("%s: typemethod: Could not create channel. Reason: %s", this->v_identifier, ov_result_getresulttext(result)));
								ov_memstack_unlock();
							}
						}
					}
					if(pChannel){
						Ov_GetVTablePtr(ksbase_Channel, pVtblChannel, pChannel);
						if(!pVtblChannel){
							KS_logfile_error(("%s: typemethod: Could not get vtable of channel", this->v_identifier));
							thisMngCom->v_PmRegister = FALSE;
						} else {
							//	Do NOT use "localhost" as this may open an IPV6 connection which the portmapper probably can't handle
							if(Ov_OK(pVtblChannel->m_OpenConnection(pChannel, "127.0.0.1", PM_PORT))){
								//	empty output buffer
								ksbase_free_KSDATAPACKET(&(pChannel->v_outData));
								//	create unset
								result = generate_PMAPPROC(&(pChannel->v_outData), pChannel->v_usesStreamProtocol, PM_FUNC_UNSET, 0, 0);
								if(Ov_Fail(result)){
									KS_logfile_error(("%s: typemethod: Generation of unset call failed.", this->v_identifier));
								} else if(Ov_Fail(pVtblChannel->m_SendData(pChannel))){
									ov_memstack_lock();
									KS_logfile_error(("%s: typemethod: Sending unset to portmapper failed. Reason: %s", this->v_identifier, ov_result_getresulttext(result)));
									ov_memstack_unlock();
								}
								//check for UDP
								pClIterator = Ov_GetFirstChild(ov_inheritance, pclass_ksbase_Channel);
								while(pClIterator) {
									if((ov_string_compare(pClIterator->v_identifier, "TCPChannel") == OV_STRCMP_EQUAL))
										break;
									pClIterator = Ov_GetNextChild(ov_inheritance, pClIterator);
								}
								if(pClIterator){
									//set for UDP
									result = generate_PMAPPROC(&(pChannel->v_outData), pChannel->v_usesStreamProtocol, PM_FUNC_SET, PM_PROT_UDP, strtol(thisMngCom->v_OwnPort, NULL, 10));
									if(Ov_Fail(result)){
										KS_logfile_error(("%s: typemethod: Generation of set call for UDP failed.", this->v_identifier));
									} else if(Ov_Fail(pVtblChannel->m_SendData(pChannel))){
										ov_memstack_lock();
										KS_logfile_error(("%s: typemethod: Sending register for UDP to portmapper failed. Reason: %s", this->v_identifier, ov_result_getresulttext(result)));
										ov_memstack_unlock();
									}
								}
								// check for TCPbind
								pClIterator = Ov_GetFirstChild(ov_inheritance, pclass_ksbase_Channel);
								while(pClIterator) {
									if((ov_string_compare(pClIterator->v_identifier, "TCPChannel") == OV_STRCMP_EQUAL))
										break;
									pClIterator = Ov_GetNextChild(ov_inheritance, pClIterator);
								}
								if(pClIterator){
									//set for TCP
									result = generate_PMAPPROC(&(pChannel->v_outData), pChannel->v_usesStreamProtocol, PM_FUNC_SET, PM_PROT_TCP, strtol(thisMngCom->v_OwnPort, NULL, 10));
									if(Ov_Fail(result)){
										KS_logfile_error(("%s: typemethod: Generation ofnset call for TCP failed.", this->v_identifier));
									} else if(Ov_Fail(pVtblChannel->m_SendData(pChannel))){
										ov_memstack_lock();
										KS_logfile_error(("%s: typemethod: Sending register for TCP to portmapper failed. Reason: %s", this->v_identifier, ov_result_getresulttext(result)));
										ov_memstack_unlock();
									}
								}
								pVtblChannel->m_CloseConnection(pChannel);
								ksbase_free_KSDATAPACKET(&(pChannel->v_inData));
								ksbase_free_KSDATAPACKET(&(pChannel->v_outData));
								thisMngCom->v_PmReqSent = TRUE;
							}else{
								thisMngCom->v_PmRegister = FALSE;
							}
						}
					}
				}
				return;
			} else {
				servername.value.vartype = OV_VT_VOID;
				servername.value.valueunion.val_string = NULL;
				ov_vendortree_getservername(&servername, NULL);
				/*	find the first xdrClient in containment to request register	*/
				xdrClient = Ov_StaticPtrCast(ksxdr_xdrClient, Ov_GetFirstChild(ov_containment, this));
				if(xdrClient) {
					while(xdrClient && (Ov_GetClassPtr(xdrClient) != pclass_ksxdr_xdrClient))
						xdrClient = Ov_StaticPtrCast(ksxdr_xdrClient, Ov_GetNextChild(ov_containment, xdrClient));
				}

				if(!xdrClient) {/*	no xdrClient in containment --> create one	*/
					result = Ov_CreateObject(ksxdr_xdrClient, xdrClient, this, "xdrClient");
					if(Ov_Fail(result)) {
						ov_memstack_lock();
						KS_logfile_error(("%s: typemethod: could not create xdrClient. reason: %s", this->v_identifier, ov_result_getresulttext(result)));
						ov_memstack_unlock();
						this->v_actimode = 0;
						thisMngCom->v_RegisterState = XDR_MNGRCOM_REGISTERSTATE_ERROR;
						return;
					}
					xdrClient->v_holdConnection = TRUE;
				}
				result = ov_string_setvalue(&xdrClient->v_ManagerPort, thisMngCom->v_ManagerPort);
				if(Ov_Fail(result)) {
					ov_memstack_lock();
					KS_logfile_error(("%s: typemethod: could not set ManagerPort at xdrClient. reason: %s", this->v_identifier, ov_result_getresulttext(result)));
					ov_memstack_unlock();
					thisMngCom->v_Tries++;
					return;
				}
				ksxdr_xdrClient_requestRegister(Ov_StaticPtrCast(ksbase_ClientBase, xdrClient), NULL, servername.value.valueunion.val_string, 2, strtol(thisMngCom->v_OwnPort, NULL, 10), 30, Ov_StaticPtrCast(ov_domain, this), &ksxdr_xdrManagercom_Callback);
				thisMngCom->v_RegisterState = XDR_MNGRCOM_REGISTERSTATE_WAITING;	/*	set state to waiting for answer	*/
				thisMngCom->v_cycInterval = 5000;	/*	used as a timeout: 5 seconds should be enough for a local connection (assuming rootcomtask runs at 1 msec)	*/
				thisMngCom->v_Tries++;
				return;
			}
		} else {
			KS_logfile_error(("%s: registering at Manager failed 3 times. retry in 2 minutes.", thisMngCom->v_identifier));
			this->v_cycInterval = 120000;	/*	retry in 2 minutes	*/
			thisMngCom->v_Tries = 0;
			/*	reset client	*/
			/*	find the first xdrClient in containment to issue reset	*/
			xdrClient = Ov_StaticPtrCast(ksxdr_xdrClient, Ov_GetFirstChild(ov_containment, this));
			if(xdrClient) {
				while(xdrClient && (Ov_GetClassPtr(xdrClient) != pclass_ksxdr_xdrClient))
					xdrClient = Ov_StaticPtrCast(ksxdr_xdrClient, Ov_GetNextChild(ov_containment, xdrClient));
			}
			ksxdr_xdrClient_reset(Ov_StaticPtrCast(ksbase_ClientBase, xdrClient));
			return;
		}
		break;

	case XDR_MNGRCOM_REGISTERSTATE_WAITING: 	/*	waiting for answer	*/
		/*	the callbackfunction sets state to registered or to error. if typemethod is run in this state a timeout occurred	*/
		/*	increment Tries, reset Client, set state to error and be called again in 5 seconds	*/
		thisMngCom->v_Tries++;

		xdrClient = Ov_StaticPtrCast(ksxdr_xdrClient, Ov_GetFirstChild(ov_containment, this));
		if(xdrClient) {
			while(xdrClient && (Ov_GetClassPtr(xdrClient) != pclass_ksxdr_xdrClient))
				xdrClient = Ov_StaticPtrCast(ksxdr_xdrClient, Ov_GetNextChild(ov_containment, xdrClient));
		}
		ksxdr_xdrClient_reset(Ov_StaticPtrCast(ksbase_ClientBase, xdrClient));

		thisMngCom->v_RegisterState = XDR_MNGRCOM_REGISTERSTATE_ERROR;
		thisMngCom->v_cycInterval = 5000;
	break;

	}
	return;
}

void ksxdr_xdrManagercom_Callback(OV_INSTPTR_ov_domain instanceCalled, OV_INSTPTR_ov_domain instanceCalling)
{
	OV_INSTPTR_ksbase_ClientBase	xdrClient = Ov_StaticPtrCast(ksbase_ClientBase, instanceCalling);
	OV_INSTPTR_ksxdr_xdrManagerCom	this = Ov_StaticPtrCast(ksxdr_xdrManagerCom, instanceCalled);
	OV_RESULT						ksResult;
	OV_RESULT						result;

	result = ksxdr_xdrClient_processRegister(xdrClient, NULL, &ksResult);
	if(Ov_Fail(result))
	{	ov_memstack_lock();
		KS_logfile_error(("%s: callback: error processing answer: %s", this->v_identifier, ov_result_getresulttext(result)));
		ov_memstack_unlock();
		this->v_RegisterState = XDR_MNGRCOM_REGISTERSTATE_ERROR;	/*	error	*/
		return;
	}

	ksbase_free_KSDATAPACKET(&(Ov_StaticPtrCast(ksxdr_xdrClient, xdrClient)->v_dataReceived));
	KS_logfile_debug(("%s: callback: register processed: result: %lu", this->v_identifier, result));
	if(ksResult == KS_ERR_OK)
	{
		this->v_RegisterState = XDR_MNGRCOM_REGISTERSTATE_REGISTERED;	/*	registered	*/
		this->v_Tries = 0;
		this->v_cycInterval = 30000;	/*	reregister in 30 seconds	*/
		this->v_NextExecTime.secs += 30;
		return;
	}
	else
	{
		this->v_RegisterState = XDR_MNGRCOM_REGISTERSTATE_ERROR;	/*	error	*/
		this->v_ErrCode = ksResult;
		return;
	}

	return;
}
