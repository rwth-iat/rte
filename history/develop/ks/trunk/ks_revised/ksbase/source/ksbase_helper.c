
#include "ksbase_helper.h"
#include <string.h>
#include "libov/ov_macros.h"
#include "ksbase.h"

OV_DLLFNCEXPORT OV_BOOL ks_isvalidname(OV_STRING name)
{

	OV_UINT i = 0;
	OV_UINT length;
	length = strlen(name);
	if(length <= KS_NAME_MAXLEN)
	{
		while(i<length)
		{
			if(!((name[i] >= 65 && name[i] <= 90)	//A-Z
					|| (name[i] >= 97 && name[i] <= 122)	//a-z
					|| name[i] == 95))		//'_'
					return FALSE;

		}
		return TRUE;
	}
	else
		return FALSE;
}


OV_DLLFNCEXPORT void ksbase_free_KSDATAPACKET(KS_DATAPACKET* packet)
{
	if(packet->length)
	{
		ov_free(packet->data);
		packet->data = NULL;
		packet->length = 0;
		packet->writePT = NULL;
		packet->readPT = NULL;
	}
	return;
}

OV_DLLFNCEXPORT OV_RESULT ksbase_KSDATAPACKET_append(KS_DATAPACKET* packet, OV_BYTE* data, OV_UINT addlength)
{
	OV_BYTE* tempdata;

	if(packet->length)
	{
		tempdata = ov_realloc(packet->data, packet->length + addlength);
		if(!tempdata)
			return OV_ERR_HEAPOUTOFMEMORY;
		memcpy(packet->writePT, data, addlength);
		packet->writePT += addlength;
	}
	else
	{
		packet->data = ov_malloc(addlength);
		if(!packet->data)
		{
			packet->length = 0;
			packet->readPT = NULL;
			packet->writePT = NULL;
			return OV_ERR_HEAPOUTOFMEMORY;
		}
		packet->length = addlength;
		packet->readPT = NULL;
		packet->writePT = packet->data + addlength;
	}

	return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_RESULT ksbase_KSDATAPACKET_set(KS_DATAPACKET* packet, OV_BYTE* data, OV_UINT addlength)
{
	if(packet->length)
	{
		ksbase_free_KSDATAPACKET(packet);
	}

	packet->data = ov_malloc(addlength);
	if(!packet->data)
	{
		packet->length = 0;
		packet->readPT = NULL;
		packet->writePT = NULL;
		return OV_ERR_HEAPOUTOFMEMORY;
	}
	packet->length = addlength;
	packet->readPT = NULL;
	packet->writePT = packet->data + addlength;


	return OV_ERR_OK;
}