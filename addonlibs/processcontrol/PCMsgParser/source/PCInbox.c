
/******************************************************************************
*
*   FILE
*   ----
*   PCInbox.c
*
*   History
*   -------
*   2013-05-07   File created
*
*******************************************************************************
*
*   This file is generated by the 'acplt_builder' command
*
******************************************************************************/


#ifndef OV_COMPILE_LIBRARY_PCMsgParser
#define OV_COMPILE_LIBRARY_PCMsgParser
#endif


#include "PCMsgParser.h"
#include "libov/ov_macros.h"
#include "libov/ov_logfile.h"
#include <string.h>


OV_DLLFNCEXPORT OV_RESULT PCMsgParser_PCInbox_constructor(
	OV_INSTPTR_ov_object 	pobj
) {
    /*    
    *   local variables
    */
    OV_RESULT    result;

    /* do what the base class does first */
    result = ov_object_constructor(pobj);
    if(Ov_Fail(result))
         return result;

    /* do what */


    return OV_ERR_OK;
}

OV_RESULT PCMsgParser_findElementBegin(char const* xml, const OV_STRING elemName, OV_STRING* pStart)
{
	if(!xml || !elemName)
		return OV_ERR_BADPARAM;

	*pStart = strstr(xml, elemName);
	if(!pStart)
		return OV_ERR_BADVALUE;

	while(pStart && (**(pStart-1) != '<'))
	{/*	go over the string and check if this is an element name	*/
		(*pStart)++;
		*pStart = strstr(*pStart, elemName);
	}

	if(!pStart)
		return OV_ERR_BADVALUE;

	pStart--;
	return OV_ERR_OK;
}

/*
 * return OV_ERR_BADVALUE if element is not found (or not valid)
 */

OV_RESULT PCMsgParser_getElementData(char const* xml, const OV_STRING elemName, OV_STRING* pData)
{
	OV_UINT dataLength;
	OV_STRING beginElement = NULL;
	char const* tempPtr =  xml;

	if(!xml || !elemName)
		return OV_ERR_BADPARAM;

	beginElement = strstr(xml, elemName);
	if(!beginElement)
		return OV_ERR_BADVALUE;

	while(beginElement && (*(beginElement-1) != '<'))
	{/*	go over the string and check if this is an element name	*/
		beginElement++;
		beginElement = strstr(beginElement, elemName);
	}

	/*	jump to end of tag	*/
	tempPtr = strchr(beginElement, '>');
	if(!tempPtr)
		return OV_ERR_BADVALUE;
	if(*(tempPtr-1) == '/')
	{	/*	no Data	*/
		*pData = NULL;
		return OV_ERR_OK;
	}

	tempPtr++;
	/*	now it points to the begin of the data	*/
	for(dataLength = 0; tempPtr[dataLength] != '<'; dataLength++)
	{
		if(!tempPtr[dataLength])
			return OV_ERR_BADVALUE;	/*	file ends before end of element --> invalid	*/
	}

	*pData = ov_memstack_alloc(dataLength+1);
	if(!*pData)
		return OV_ERR_HEAPOUTOFMEMORY;

	strncpy(*pData, tempPtr, dataLength);
	(*pData)[dataLength] = '\0';


	return OV_ERR_OK;
}

OV_RESULT PCMsgParser_getAttributeData(char const* xml, const OV_STRING elemName, const OV_STRING attributeName, OV_STRING* pData)
{
	OV_UINT dataLength;
	char const* beginElement = NULL;
	char const* endPtr =  NULL;
	char const* tempptr = NULL;
	OV_UINT i;
	OV_BOOL found = FALSE;
	char const* valueEnd = NULL;

	if(!xml || !elemName || !attributeName)
		return OV_ERR_BADPARAM;

	beginElement = strstr(xml, elemName);
	if(!beginElement)
		return OV_ERR_BADVALUE;

	while(beginElement && (*(beginElement-1) != '<'))
	{/*	go over the string and check if this is an element name	*/
		beginElement++;
		beginElement = strstr(beginElement, elemName);
	}

	/*	find to end of tag	*/
	endPtr = strchr(beginElement, '>');
	if(!endPtr)
		return OV_ERR_BADVALUE;

	tempptr = beginElement;
	for(; tempptr < endPtr; tempptr++)
	{
		if(*tempptr == *attributeName)
		{
			for(i=0; (&(tempptr[i]) < endPtr) && (attributeName[i]); i++)
			{
				if(tempptr[i] == attributeName[i])
					found = TRUE;
				else
				{
					found = FALSE;
					break;
				}
			}
			if(found)
				break;
		}
	}

	if(!found)
	{
		pData = NULL;
		return OV_ERR_OK;
	}
	else
	{
		if(tempptr[i] == '=')
		{
			/*	set to begin of data	*/
			beginElement = &(tempptr[i+1]);
			if(*beginElement == '"')
			{
				beginElement++;
				tempptr = beginElement;
				for(; valueEnd && (valueEnd < endPtr); tempptr = valueEnd)
				{
					valueEnd = strchr(beginElement, '"');
					if(valueEnd && (*(valueEnd-1) == '\\'))
					{
						found = TRUE;
						continue;
					}
					else
					{
						if(!valueEnd)
							found = FALSE;
						break;
					}
				}
				if(found)
				{
					dataLength = endPtr - beginElement;
					*pData = ov_memstack_alloc(dataLength+1);
					if(!*pData)
						return OV_ERR_HEAPOUTOFMEMORY;

					strncpy(*pData, beginElement, dataLength);
					(*pData)[dataLength] = '\0';
					return OV_ERR_OK;
				}
				else
					return OV_ERR_BADVALUE;
			}
			else
			{
				valueEnd = strchr(beginElement, ' ');
				if(valueEnd)
					dataLength = valueEnd - beginElement;
				else
					dataLength = endPtr - beginElement;
				*pData = ov_memstack_alloc(dataLength+1);
				if(!*pData)
					return OV_ERR_HEAPOUTOFMEMORY;

				strncpy(*pData, beginElement, dataLength);
				(*pData)[dataLength] = '\0';
				return OV_ERR_OK;
			}
		}
		else
		{
			pData = NULL;
			return OV_ERR_OK;
		}
	}

}

OV_STRING PCMsgParser_skipWhiteSpace(OV_STRING input)
{
	OV_STRING temp;
	for(temp = input; *temp &&(*temp == ' ' || *temp == '\t' || *temp == '\r' || *temp == '\n'); temp++)
		;
	return temp;
}

void PCMsgParser_rStrip(OV_STRING input)
{
	OV_STRING temp;
	for(temp = input; *temp; temp++)
		;	/*	seek to end	*/
	while(temp>input && (*temp == ' ' || *temp == '\t' || *temp == '\r' || *temp == '\n'))
	{
		*temp = '\0';
		temp--;
	}
	return;
}

OV_DLLFNCEXPORT void PCMsgParser_PCInbox_typemethod(
		OV_INSTPTR_fb_functionblock	pfb,
		OV_TIME						*pltc
) {
	OV_INSTPTR_PCMsgParser_PCInbox this = Ov_StaticPtrCast(PCMsgParser_PCInbox, pfb);
	OV_INSTPTR_MessageSys_Message pMsg = NULL;
	OV_INSTPTR_ov_object	pMsgObj = NULL;
	OV_INSTPTR_ov_object	pNextMsgObj = NULL;
	OV_INSTPTR_ov_domain	pParentDomain = NULL;
	OV_INSTPTR_cmdlib_processcontrol pProcessControl = NULL;
	OV_UINT waitingMsgs = 0;
	OV_RESULT result;

	OV_STRING MsgBody = NULL;
	OV_STRING startPtr = NULL;
	OV_STRING commander = NULL;
	OV_STRING command = NULL;
	OV_STRING value = NULL;
	OV_STRING order = NULL;
	OV_STRING pData = NULL;


	/*	check if we are in the containment of a process control object	*/
	pParentDomain = Ov_GetParent(ov_containment, this);
	if(Ov_CanCastTo(cmdlib_processcontrol, pParentDomain))
		pProcessControl = Ov_StaticPtrCast(cmdlib_processcontrol, pParentDomain);
	else
	{
		ov_logfile_error("PCInbox %s not in the containment of a process control unit. deactivating.", this->v_identifier);
		this->v_actimode = 0;
		return;
	}


	pMsgObj = Ov_GetFirstChild(ov_containment, this);
	while(pMsgObj)
	{
		if(Ov_CanCastTo(MessageSys_Message, pMsgObj))
			waitingMsgs++;

		pMsgObj = Ov_GetNextChild(ov_containment, pMsgObj);
	}

	if(this->v_fiFoQueue)
	{	/*	starting working with the oldest --> delete the newer ones	*/
		pMsgObj = Ov_GetLastChild(ov_containment, this);
	}
	else
	{	/*	starting working with the newest --> delete the older ones	*/
		pMsgObj = Ov_GetFirstChild(ov_containment, this);
	}

	if(waitingMsgs > this->v_queueLength)
	{	/*	too many messages	*/
		while((waitingMsgs > this->v_queueLength) && pMsgObj)	/*	iterate over messages if there are too many. delete the object if it is a message	*/
		{
			if(this->v_fiFoQueue)
			{	/*	starting with the oldest --> delete the newer ones	*/
				pNextMsgObj = Ov_GetPreviousChild(ov_containment, pMsgObj);
				if(Ov_CanCastTo(MessageSys_Message, pMsgObj))
				{
					Ov_DeleteObject(pMsgObj);
					pMsgObj = pNextMsgObj;
					waitingMsgs--;
				}
				else
					pMsgObj = pNextMsgObj;
			}
			else
			{	/*	starting with the newest --> delete the older ones	*/
				pNextMsgObj = Ov_GetNextChild(ov_containment, pMsgObj);
				if(Ov_CanCastTo(MessageSys_Message, pMsgObj))
				{
					Ov_DeleteObject(pMsgObj);
					pMsgObj = pNextMsgObj;
					waitingMsgs--;
				}
				else
					pMsgObj = pNextMsgObj;
			}
		}
	}

	if(waitingMsgs)
	{	/*	there are messages waiting for processing	*/
		/*	get the Message to process (sort out other objects	*/
		while(pMsgObj && (!Ov_CanCastTo(MessageSys_Message, pMsgObj)))
		{
			if(this->v_fiFoQueue)
				pMsgObj = Ov_GetPreviousChild(ov_containment, pMsgObj);
			else
				pMsgObj = Ov_GetNextChild(ov_containment, pMsgObj);

		}
		if(pMsgObj)
		{
			pMsg = Ov_StaticPtrCast(MessageSys_Message, pMsgObj);
			/*	from here on we surely know
			 * 1. pMsg is of type Message or derived
			 * 2. pMsg is the message to work with basing on the queue type	*/

			MsgBody = MessageSys_Message_msgBody_get(pMsg);
			ov_memstack_lock();
			/*	DEBUG: print MSG	*/
			ov_logfile_debug("Msg-Body:\n\n%s\n\n", MsgBody);


			/*	check Message	*/
			/*	check for name	*/
			if(Ov_Fail(PCMsgParser_findElementBegin(MsgBody, "msg", &startPtr)))
			{
				ov_logfile_info("%s: no msg-tag --> deleting message", this->v_identifier);
				Ov_DeleteObject(pMsg);	/*	Message is corrupted --> delete it	*/
				ov_memstack_unlock();
				return;
			}
			/*	check for hdr	*/
			if(Ov_Fail(PCMsgParser_findElementBegin(startPtr, "hdr", &startPtr)))
			{
				ov_logfile_info("%s: no hdr-tag --> deleting message", this->v_identifier);
				Ov_DeleteObject(pMsg);	/*	Message is corrupted --> delete it	*/
				ov_memstack_unlock();
				return;
			}
			/*	check for rcvSysAdr	*/
			if(Ov_Fail(PCMsgParser_findElementBegin(startPtr, "rcvSysAdr", &startPtr)))
			{
				ov_logfile_info("%s: no rcvSysAdr-tag --> deleting message", this->v_identifier);
				Ov_DeleteObject(pMsg);	/*	Message is corrupted --> delete it	*/
				ov_memstack_unlock();
				return;
			}
			/*	check for rcvLocAdr	*/
			if(Ov_Fail(PCMsgParser_findElementBegin(startPtr, "rcvLocAdr", &startPtr)))
			{
				ov_logfile_info("%s: no rcvLocAdr-tag --> deleting message", this->v_identifier);
				Ov_DeleteObject(pMsg);	/*	Message is corrupted --> delete it	*/
				ov_memstack_unlock();
				return;
			}
			/*	check for msgId	*/
			if(Ov_Fail(PCMsgParser_findElementBegin(startPtr, "msgId", &startPtr)))
			{
				ov_logfile_info("%s: no msgId --> deleting message", this->v_identifier);
				Ov_DeleteObject(pMsg);	/*	Message is corrupted --> delete it	*/
				ov_memstack_unlock();
				return;
			}
			/*	check for bdy	*/
			if(Ov_Fail(PCMsgParser_findElementBegin(startPtr, "bdy", &MsgBody)))
			{
				ov_logfile_info("%s: no bdy --> deleting message", this->v_identifier);
				Ov_DeleteObject(pMsg);	/*	Message is corrupted --> delete it	*/
				ov_memstack_unlock();
				return;
			}

			/*	now MsgBody points to the bdy-tag in the message	*/
			/*	get the necessary information from the message	*/

			/*	get service	and Operation	*/
			/*	check for first val element	*/
			if(Ov_Fail(PCMsgParser_findElementBegin(MsgBody, "val", &startPtr)))
			{
				ov_logfile_info("%s: no val --> deleting message", this->v_identifier);
				Ov_DeleteObject(pMsg);	/*	Message is corrupted --> delete it	*/
				ov_memstack_unlock();
				return;
			}

			if(Ov_OK(PCMsgParser_getAttributeData(startPtr, "val", "id", &pData)))
			{
				if(!pData)
				{
					ov_logfile_info("%s %d: invalid Message --> deleting", this->v_identifier, __LINE__);
					Ov_DeleteObject(pMsg);	/*	Message is corrupted --> delete it	*/
					ov_memstack_unlock();
					return;
				}
				if(ov_string_compare(pData, "svc") == OV_STRCMP_EQUAL)
				{
					if(Ov_OK(PCMsgParser_getElementData(startPtr, "val", &pData)))
					{
						if(!pData)
						{
							ov_logfile_info("%s %d: invalid Message --> deleting", this->v_identifier, __LINE__);
							Ov_DeleteObject(pMsg);	/*	Message is corrupted --> delete it	*/
							ov_memstack_unlock();
							return;
						}
						if(ov_string_compare(pData, "ProcessControl") != OV_STRCMP_EQUAL)
						{
							ov_logfile_info("%s: unknown service requested --> deleting message", this->v_identifier);
							Ov_DeleteObject(pMsg);	/*	Message is corrupted --> delete it	*/
							ov_memstack_unlock();
							return;
						}
					}
				}
				else
				{
					ov_logfile_info("%s %d: invalid Message --> deleting", this->v_identifier, __LINE__);
					Ov_DeleteObject(pMsg);	/*	Message is corrupted --> delete it	*/
					ov_memstack_unlock();
					return;
				}

				/*	check for second val element	*/
				startPtr++;
				if(Ov_Fail(PCMsgParser_findElementBegin(startPtr, "val", &startPtr)))
				{
					ov_logfile_info("%s: no val --> deleting message", this->v_identifier);
					Ov_DeleteObject(pMsg);	/*	Message is corrupted --> delete it	*/
					ov_memstack_unlock();
					return;
				}
				result = PCMsgParser_getAttributeData(startPtr, "val", "id", &pData);
				if(Ov_Fail(result))
				{
					if(result != OV_ERR_HEAPOUTOFMEMORY)
						Ov_DeleteObject(pMsg);	/*	Message is corrupted --> delete it	*/
					ov_memstack_unlock();
					return;
				}
				if(!pData)
				{
					ov_logfile_info("%s %d: invalid Message --> deleting", this->v_identifier, __LINE__);
					Ov_DeleteObject(pMsg);	/*	Message is corrupted --> delete it	*/
					ov_memstack_unlock();
					return;
				}

				if(ov_string_compare(pData, "op") == OV_STRCMP_EQUAL)
				{
					result = PCMsgParser_getElementData(startPtr, "val", &command);
					if(Ov_Fail(result))
					{
						if(result != OV_ERR_HEAPOUTOFMEMORY)
							Ov_DeleteObject(pMsg);	/*	Message is corrupted --> delete it	*/

						ov_memstack_unlock();
						return;
					}
					if(!command)
					{
						ov_logfile_info("%s %d: invalid Message --> deleting", this->v_identifier, __LINE__);
						Ov_DeleteObject(pMsg);	/*	Message is corrupted --> delete it	*/
						ov_memstack_unlock();
						return;
					}
				}
			}
			else
			{
				ov_logfile_info("%s: no val --> deleting message", this->v_identifier);
				Ov_DeleteObject(pMsg);	/*	Message is corrupted --> delete it	*/
				ov_memstack_unlock();
				return;
			}


			if(Ov_Fail(PCMsgParser_findElementBegin(startPtr, "sd", &startPtr)))
			{
				ov_logfile_info("%s: no sd --> deleting message", this->v_identifier);
				Ov_DeleteObject(pMsg);	/*	Message is corrupted --> delete it	*/
				ov_memstack_unlock();
				return;
			}

			if(Ov_OK(PCMsgParser_getAttributeData(startPtr, "val", "id", &pData)))
			{
				if(!pData)
				{
					ov_logfile_info("%s %d: invalid Message --> deleting", this->v_identifier, __LINE__);
					Ov_DeleteObject(pMsg);	/*	Message is corrupted --> delete it	*/
					ov_memstack_unlock();
					return;
				}
				if(ov_string_compare(pData, "cmdr") == OV_STRCMP_EQUAL)
				{
					if(Ov_OK(PCMsgParser_getElementData(startPtr, "val", &commander)))
					{
						if(!commander)
						{
							ov_logfile_info("%s %d: invalid Message --> deleting", this->v_identifier, __LINE__);
							Ov_DeleteObject(pMsg);	/*	Message is corrupted --> delete it	*/
							ov_memstack_unlock();
							return;
						}
					}
				}
				else
				{
					ov_logfile_info("%s %d: invalid Message --> deleting", this->v_identifier, __LINE__);
					Ov_DeleteObject(pMsg);	/*	Message is corrupted --> delete it	*/
					ov_memstack_unlock();
					return;
				}

				/*	check for second val element	*/
				startPtr++;
				if(Ov_Fail(PCMsgParser_findElementBegin(startPtr, "val", &startPtr)))
				{
					ov_logfile_info("%s: no val --> deleting message", this->v_identifier);
					Ov_DeleteObject(pMsg);	/*	Message is corrupted --> delete it	*/
					ov_memstack_unlock();
					return;
				}
				result = PCMsgParser_getAttributeData(startPtr, "val", "id", &pData);
				if(Ov_Fail(result))
				{
					if(result != OV_ERR_HEAPOUTOFMEMORY)
						Ov_DeleteObject(pMsg);	/*	Message is corrupted --> delete it	*/
					ov_memstack_unlock();
					return;
				}
				if(!pData)
				{
					ov_logfile_info("%s %d: invalid Message --> deleting", this->v_identifier, __LINE__);
					Ov_DeleteObject(pMsg);	/*	Message is corrupted --> delete it	*/
					ov_memstack_unlock();
					return;
				}

				if(ov_string_compare(pData, "value") == OV_STRCMP_EQUAL)
				{
					result = PCMsgParser_getElementData(startPtr, "val", &value);
					if(Ov_Fail(result))
					{
						if(result != OV_ERR_HEAPOUTOFMEMORY)
							Ov_DeleteObject(pMsg);	/*	Message is corrupted --> delete it	*/

						ov_memstack_unlock();
						return;
					}
					if(!value)
					{
						ov_logfile_info("%s %d: invalid Message --> deleting", this->v_identifier, __LINE__);
						Ov_DeleteObject(pMsg);	/*	Message is corrupted --> delete it	*/
						ov_memstack_unlock();
						return;
					}
				}
			}
			else
			{
				ov_logfile_info("%s: no val in sd --> deleting message", this->v_identifier);
				Ov_DeleteObject(pMsg);	/*	Message is corrupted --> delete it	*/
				ov_memstack_unlock();
				return;
			}

			/*	concatenate to order and set it	*/

			order = ov_memstack_alloc(strlen(commander)+strlen(command)+strlen(value)+3);	/*	commander;command;value\0	*/
			if(!order)
			{
				ov_memstack_unlock();
				return;
			}
			sprintf(order, "%s;%s;%s", commander, command, value);
			ov_logfile_debug("order:\n\n\t%s\n\n", order);
			cmdlib_processcontrol_order_set(pProcessControl, order);
			/*	delete parsed message	*/
			Ov_DeleteObject(pMsg);
			ov_memstack_unlock();
		}

	}


	return;
}

