
#include "ksbase_helper.h"
#include <string.h>
#include "libov/ov_macros.h"
#include "ksbase.h"
#include "ks_logfile.h"

/**
 * splits up a path string into its components
 * a path string may contain:
 *	[//host[:MANAGERPort]/servername[:serverport]/]instancePath
 *	examples are
 *	//dev:7509/MANAGER:7509/vendor/server_description
 *	//dev/MANAGER:7509/vendor/server_description
 *	//dev:7509/MANAGER/vendor/server_description
 *	//dev/MANAGER/vendor/server_description
 *	call ov_memstack_lock() / _unlock() around it
 *	when the result pointers are NULL they are not filled, no error is returned
 *	however, managerport and serverport are only set, if the pointers host and serverName,
 *	respectively, are set
 */
OV_DLLFNCEXPORT OV_RESULT ks_splitOneStringPath(
		const OV_STRING path,
		OV_STRING* host,
		OV_STRING* managerPort,
		OV_STRING* serverName,
		OV_STRING* serverPort,
		OV_STRING* instancePath
){
	const char* temp1	=	NULL;	/*	points into original path-string	*/
	OV_STRING temp2		=	NULL;	/*	points into copy of path string	*/
	OV_UINT copylength;
	if(!path || !(*path)){
		return OV_ERR_BADPATH;
	}
	if(path[0] != '/' || path[1] != '/'){
		/*	only the instance path given	*/
		if(host){
			*host = NULL;
		}
		if(managerPort){
			*managerPort = NULL;
		}
		if(serverName){
			*serverName = NULL;
		}
		if(serverPort){
			*serverPort = NULL;
		}
		temp1 = path;
	} else {
		temp1 = &(path[2]);
		temp2 = strchr(temp1, '/');
		if(!temp2){	/*	only host and maybe MANAGERPort	*/
			if(host){
				copylength = strlen(temp1);
				*host = ov_memstack_alloc((copylength + 1) * sizeof(char));
				if(!(*host)){
					return OV_ERR_HEAPOUTOFMEMORY;
				}
				memcpy(*host, temp1, copylength);
				(*host)[copylength] = '\0';
				temp2 = strchr(*host, ':');
				if(temp2){
					*temp2 = '\0';
				}
				if(managerPort){
					*managerPort = temp2;
					(*managerPort)++;
				}
				if(serverName){
					*serverName = NULL;
				}
				if(serverPort){
					*serverPort = NULL;
				}
				if(instancePath){
					*instancePath = NULL;
				}
			}
			return OV_ERR_OK;
		} else {
			if(host){
				copylength = temp2 - temp1;
				*host = ov_memstack_alloc((copylength + 1) * sizeof(char));
				if(!(*host)){
					return OV_ERR_HEAPOUTOFMEMORY;
				}

				memcpy(*host, temp1, copylength);
				(*host)[copylength] = '\0';
				temp1 = temp2;
				temp2 = strchr(*host, ':');
				if(temp2){
					*temp2 = '\0';
				}
				if(managerPort){
					*managerPort = temp2;
					(*managerPort)++;
				}
			}
			temp1++;
			temp2 = strchr(temp1, '/');
			if(!temp2){	/*	only servername and maybe serverPort following	*/
				if(serverName){
					copylength = strlen(temp1);
					*serverName = ov_memstack_alloc((copylength + 1) * sizeof(char));
					if(!(*serverName)){
						return OV_ERR_HEAPOUTOFMEMORY;
					}
					memcpy(*serverName, temp1, copylength);
					(*serverName)[copylength] = '\0';
					temp2 = strchr(*serverName, ':');
					if(temp2){
						*temp2 = '\0';
					}
					if(serverPort){
						*serverPort = temp2;
						(*serverPort)++;
					}
					if(instancePath){
						*instancePath = NULL;
					}
				}
				return OV_ERR_OK;
			} else {
				if(serverName){
					copylength = temp2 - temp1;
					*serverName = ov_memstack_alloc((copylength + 1) * sizeof(char));
					if(!(*serverName)){
						return OV_ERR_HEAPOUTOFMEMORY;
					}

					memcpy(*serverName, temp1, copylength);
					(*serverName)[copylength] = '\0';
					temp1 = temp2;
					temp2 = strchr(*serverName, ':');
					if(temp2){
						*temp2 = '\0';
					}
					if(serverPort){
						*serverPort = temp2;
						(*serverPort)++;
					}
				}
			}
		}
	}
	/*	copy instancePath	*/
	if(instancePath){
		copylength = strlen(temp1);
		*instancePath = ov_memstack_alloc((copylength + 1) * sizeof(char));
		if(!(*instancePath)){
			return OV_ERR_HEAPOUTOFMEMORY;
		}
		memcpy(*instancePath, temp1, copylength);
		(*instancePath)[copylength] = '\0';
	}
	return OV_ERR_OK;
}


OV_DLLFNCEXPORT OV_BOOL ks_isvalidname(OV_STRING name)
{

	OV_UINT i = 0;
	if(!name || !(*name))
		return FALSE;

	while(name[i] && i < KS_NAME_MAXLEN)
	{
		if(!((name[i] >= 65 && name[i] <= 90)			/*	A-Z	*/
				|| (name[i] >= 97 && name[i] <= 122)	/*	a-z	*/
				|| (name[i] == 95)						/*	'_'	*/
				|| (name[i] >= 48 && name[i] <= 57)))	/*	0-9	*/
			return FALSE;
		i++;
	}
	if(i < KS_NAME_MAXLEN)
		return TRUE;
	else
		return FALSE;
}

/**
 * frees the memory of a datapacket
 */
OV_DLLFNCEXPORT void ksbase_free_KSDATAPACKET(KS_DATAPACKET* packet)
{
	if(!packet)
		return;

	if(packet->length)
	{
		if(packet->length > 65536){
			KS_logfile_debug(("freeing large (>64k)  ks-datapacket, length is: %i", packet->length));
		}
		Ov_HeapFree(packet->data);
		packet->data = NULL;
		packet->length = 0;
		packet->writePT = NULL;
		packet->readPT = NULL;
	}
	return;
}
/**
 * appends an amount of data to a datapacket via copying the data into the heap
 */
OV_DLLFNCEXPORT OV_RESULT ksbase_KSDATAPACKET_append(KS_DATAPACKET* packet, OV_BYTE* data, OV_UINT addlength)
{
	OV_BYTE* tempdata;

	if(!packet)
		return OV_ERR_BADPARAM;

	if(packet->length)
	{
		if(packet->length > 65536){
			KS_logfile_debug(("appending to large (>64k)  ks-datapacket, length is: %i", packet->length));
		}
		tempdata = Ov_HeapRealloc(packet->data, packet->length + addlength);
		if(!tempdata)
		{
			Ov_HeapFree(packet->data);
			packet->data = NULL;
			packet->length = 0;
			packet->readPT = NULL;
			packet->writePT = NULL;
			return OV_ERR_HEAPOUTOFMEMORY;
		}
		//set write pointer
		if(!packet->writePT)
			packet->writePT = tempdata;
		else
			packet->writePT = tempdata + (packet->writePT - packet->data);
		// set read pointer
		if(!packet->readPT)
			packet->readPT = tempdata;
		else
			packet->readPT = tempdata + (packet->readPT - packet->data);

		memcpy(packet->writePT, data, addlength);
		packet->writePT += addlength;
		packet->data = tempdata;
		packet->length += addlength;
	}
	else
	{
		packet->data = Ov_HeapMalloc(addlength);
		if(!packet->data)
		{
			packet->length = 0;
			packet->readPT = NULL;
			packet->writePT = NULL;
			return OV_ERR_HEAPOUTOFMEMORY;
		}
		packet->length = addlength;
		memcpy(packet->data, data, addlength);
		packet->readPT = packet->data;
		packet->writePT = packet->data + addlength;
	}

	return OV_ERR_OK;
}

/**
 * sets an amount of data to a datapacket via copying the data into the heap
 * frees the datapacket if the new size is zero
 */
OV_DLLFNCEXPORT OV_RESULT ksbase_KSDATAPACKET_set(KS_DATAPACKET* packet, OV_BYTE* data, OV_UINT addlength)
{
	if(!packet)
		return OV_ERR_BADPARAM;
	if(packet->length)
	{
		ksbase_free_KSDATAPACKET(packet);
	}
	if(addlength == 0){
		return OV_ERR_OK;
	}
	if(!data){
		return OV_ERR_BADPARAM;
	}

	packet->data = Ov_HeapMalloc(addlength);
	if(!packet->data)
	{
		packet->length = 0;
		packet->readPT = NULL;
		packet->writePT = NULL;
		return OV_ERR_HEAPOUTOFMEMORY;
	}
	packet->length = addlength;
	memcpy(packet->data, data, addlength);
	packet->readPT = packet->data;
	packet->writePT = packet->data + addlength;


	return OV_ERR_OK;
}
