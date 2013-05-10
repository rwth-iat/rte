/*
 * MessageSys_ExportedFncs.c
 *
 *  Created on: 08.05.2013
 *      Author: lars
 */

#ifndef OV_COMPILE_LIBRARY_MessageSys
#define OV_COMPILE_LIBRARY_MessageSys
#endif

#include "MessageSys.h"
#include "libov/ov_macros.h"
#include <string.h>

/**
 * This is a function to create new objects, which have a default name, followed by '_x', where 'x' stands for an int
 * starting with 0. Works just like Ov_CreateObject, except for it needs an OV_RESULT as an additional parameter.
 */

OV_DLLFNCEXPORT OV_RESULT MessageSys_createAnonymousObject(OV_INSTPTR_ov_class pClass, OV_INSTPTR_ov_domain pParent, OV_STRING identifier, OV_INSTPTR_ov_object* pObj)
{
	OV_UINT i = 0;
	OV_STRING tmpName = NULL;
	OV_UINT nameLength;
	OV_RESULT result;

	nameLength= strlen(identifier);
	ov_memstack_lock();
	tmpName = ov_memstack_alloc(nameLength+12); /*	nameLength plus max langth of uint plus '_' and '\0'	*/
	if(!tmpName)
	{
		ov_memstack_unlock();
		return OV_ERR_HEAPOUTOFMEMORY;
	}

	do{
		sprintf(tmpName, "%s_%lu",identifier,i);
		result = ov_class_createobject(pClass, pParent, tmpName, OV_PMH_DEFAULT, NULL, NULL, NULL, pObj);
		if(Ov_OK(result))
			break;	/*	finished, leave the loop	*/
		else if(result == OV_ERR_ALREADYEXISTS)
		{	/*	ALREADYEXISTS is ok so increment i and continue. Otherwise there is an error and we leave the loop (while(0) will end)	*/
			i++;
		}
	}while(result == OV_ERR_ALREADYEXISTS);

	ov_memstack_unlock();
	return result;
}

/**
 * This is a function to create an anonymous message.
 */
OV_DLLFNCEXPORT OV_RESULT MessageSys_createAnonymousMessage(OV_INSTPTR_ov_domain pParent, OV_STRING identifier, OV_INSTPTR_ov_object* pObj)
{
	return MessageSys_createAnonymousObject(pclass_MessageSys_Message, pParent, identifier, pObj);
}

/*
 * Checks if the delimiter matches the string
 */
static OV_BOOL MSG_isDelimiter(OV_STRING delimiter,OV_STRING x)
{
  OV_UINT i;
  OV_UINT length;

  length = strlen(delimiter);
  ////ov_logfile_debug("===>>>> isDELIMITER: delimiter='%s' -- x='%s'",delimiter,x);
  for(i = 0; i < length;i++)
    {
      if(x[i] == '\0' || x[i] != delimiter[i] )
    	  return FALSE;
    }
  return TRUE;
}

OV_BOOL MSG_split(OV_STRING victim, OV_STRING delimiter, OV_INT *position, OV_STRING *token) {
	OV_UINT curPosition = 0;
	OV_UINT vLength;

	vLength = strlen(victim);
	if(victim[*position] == 0)
		return FALSE;

	for(curPosition = (*position); curPosition <= vLength; curPosition++)
	{
		if(victim[curPosition] == '\0' || MSG_isDelimiter(delimiter, victim+curPosition))
		{
			*token = ov_memstack_alloc(curPosition-(*position)+1);
			if(!(*token))
				return FALSE;
			memcpy(*token, victim+*position, curPosition-(*position) );
			(*token)[curPosition-(*position)] = '\0';

			if(victim[curPosition]=='\0')
				*position = curPosition;
			else
			  *position = curPosition+strlen(delimiter);
			return TRUE;
		}
	}
	return FALSE;//never reached
}
