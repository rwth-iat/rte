/*
 * KSDATAPACKET_xdrhandling.c
 *
 *  Created on: 29.01.2013
 *      Author: lars
 *
 *      This file contains routines to read data in xdr-represantation form KSDATAPACKETs and routines to write data in the same manner.
 */

#include "ksxdr.h"
#include "libov/ov_malloc.h"
#include "libov/ov_memstack.h"
#include "ksbase_helper.h"
#include <string.h>
#include "KSDATAPACKET_xdrhandling.h"
#include "ov_ksserver_backend.h"
/*
 * unsigned long (OV_UINT)
 */

OV_DLLFNCEXPORT OV_RESULT KS_DATAPACKET_read_xdr_u_long(KS_DATAPACKET* datapacket, OV_UINT* value)
{
	char temp[4];
	unsigned int i = 0;

	if(!datapacket
			|| !datapacket->data)
		return OV_ERR_GENERIC;
	if(!value)
		return OV_ERR_BADPARAM;
	if(datapacket->readPT
			&& (datapacket->readPT >= datapacket->data)
			&& (datapacket->readPT <= (datapacket->data + datapacket->length)))
	{
		for(i=0; i<4; i++)
			temp[3-i] = datapacket->readPT[i];
		memcpy(value, temp, 4);
		datapacket->readPT += 4;
		return OV_ERR_OK;
	}

	return OV_ERR_GENERIC;
}

OV_DLLFNCEXPORT OV_RESULT KS_DATAPACKET_write_xdr_u_long(KS_DATAPACKET* datapacket, const OV_UINT* value)
{
	unsigned char temp[4];
	unsigned int i = 0;

	if(value)
		for(i=0; i<4; i++)
			temp[3-i] = ((unsigned char*)value)[i];
	else
		memset(temp, 0, 4);
	return ksbase_KSDATAPACKET_append(datapacket, temp, 4);
}

/*
 * long (OV_INT)
 */
OV_DLLFNCEXPORT OV_RESULT KS_DATAPACKET_read_xdr_long(KS_DATAPACKET* datapacket, OV_INT* value)
{
	char temp[4];
	unsigned int i = 0;

	if(!datapacket
			|| !datapacket->data)
		return OV_ERR_GENERIC;
	if(!value)
		return OV_ERR_BADPARAM;

	if(datapacket->readPT
			&& (datapacket->readPT >= datapacket->data)
			&& (datapacket->readPT <= (datapacket->data + datapacket->length)))
	{
		for(i=0; i<4; i++)
			temp[3-i] = datapacket->readPT[i];
		memcpy(value, temp, 4);
		datapacket->readPT += 4;
		return OV_ERR_OK;
	}

	return OV_ERR_GENERIC;
}

OV_DLLFNCEXPORT OV_RESULT KS_DATAPACKET_write_xdr_long(KS_DATAPACKET* datapacket, const OV_INT* value)
{
	unsigned char temp[4];
	unsigned int i = 0;

	if(value)
		for(i=0; i<4; i++)
			temp[3-i] = ((unsigned char*)value)[i];
	else
		memset(temp, 0, 4);
	return ksbase_KSDATAPACKET_append(datapacket, temp, 4);
}

/*
 * Single (OV_SINGLE)
 */
OV_DLLFNCEXPORT OV_RESULT KS_DATAPACKET_read_xdr_single(KS_DATAPACKET* datapacket, OV_SINGLE* value)
{
	char temp[4];
	unsigned int i = 0;

	if(!datapacket
			|| !datapacket->data)
		return OV_ERR_GENERIC;
	if(!value)
		return OV_ERR_BADPARAM;

	if(datapacket->readPT
			&& (datapacket->readPT >= datapacket->data)
			&& (datapacket->readPT <= (datapacket->data + datapacket->length)))
	{
		for(i=0; i<4; i++)
			temp[3-i] = datapacket->readPT[i];
		memcpy(value, temp, 4);
		datapacket->readPT += 4;
		return OV_ERR_OK;
	}

	return OV_ERR_GENERIC;
}

OV_DLLFNCEXPORT OV_RESULT KS_DATAPACKET_write_xdr_single(KS_DATAPACKET* datapacket, const OV_SINGLE* value)
{
	unsigned char temp[4];
	unsigned int i = 0;

	if(value)
		for(i=0; i<4; i++)
			temp[3-i] = ((unsigned char*)value)[i];
	else
		memset(temp, 0, 4);
	return ksbase_KSDATAPACKET_append(datapacket, temp, 4);
}

/*
 * double (OV_DOUBLE)
 */

OV_DLLFNCEXPORT OV_RESULT KS_DATAPACKET_read_xdr_double(KS_DATAPACKET* datapacket, OV_DOUBLE* value)
{
	char temp[8];
	unsigned int i = 0;

	if(!datapacket
			|| !datapacket->data)
		return OV_ERR_GENERIC;
	if(!value)
		return OV_ERR_BADPARAM;

	if(datapacket->readPT
			&& (datapacket->readPT >= datapacket->data)
			&& (datapacket->readPT <= (datapacket->data + datapacket->length)))
	{
		for(i=0; i<8; i++)
			temp[7-i] = datapacket->readPT[i];
		memcpy(value, temp, 8);
		datapacket->readPT += 8;
		return OV_ERR_OK;
	}

	return OV_ERR_GENERIC;
}

OV_DLLFNCEXPORT OV_RESULT KS_DATAPACKET_write_xdr_double(KS_DATAPACKET* datapacket, const OV_DOUBLE* value)
{
	unsigned char temp[8];
	unsigned int i = 0;

	if(value)
		for(i=0; i<8; i++)
			temp[7-i] = ((unsigned char*)value)[i];
	else
		memset(temp, 0, 4);
	return ksbase_KSDATAPACKET_append(datapacket, temp, 8);
}

/*
 * string (OV_STRING)
 */
OV_DLLFNCEXPORT OV_RESULT KS_DATAPACKET_read_xdr_string(KS_DATAPACKET* datapacket, OV_STRING* value, OV_UINT maxlen)
{
	unsigned int i = 0;
	OV_UINT length = 0;

	if(!datapacket
			|| !datapacket->data
			|| !value)
		return OV_ERR_GENERIC;
	if(!value)
		return OV_ERR_BADPARAM;

	if(KS_DATAPACKET_read_xdr_u_long(datapacket, &length))
		return OV_ERR_GENERIC;

	//check if it fits into the buffer (do not forget terminating '\0')
	if(maxlen && (length >= (maxlen)))
	{
		datapacket->readPT -=4;		//reset readPT to initial state
		return OV_ERR_BADVALUE;
	}
	if(datapacket->readPT
			&& (datapacket->readPT >= datapacket->data)
			&& (datapacket->readPT <= (datapacket->data + datapacket->length)))
	{
		*value = ov_malloc(length+1);
		if(!(*value))
			return OV_ERR_HEAPOUTOFMEMORY;

		//copy it to the buffer
		for(i=0; i<length; i++)
			(*value)[i] = datapacket->readPT[i];
		(*value)[length] = '\0';

		//xdrs are aligned in 4byte blocks
		while(length%4)
			length++;
		datapacket->readPT += length;
		return OV_ERR_OK;
	}
	return OV_ERR_GENERIC;
}

OV_DLLFNCEXPORT OV_RESULT KS_DATAPACKET_read_xdr_string_wolength(KS_DATAPACKET* datapacket, OV_STRING* value)
{
	return KS_DATAPACKET_read_xdr_string(datapacket, value, ~0);
}

OV_DLLFNCEXPORT OV_RESULT KS_DATAPACKET_read_xdr_string_tomemstack(KS_DATAPACKET* datapacket, OV_STRING* value, OV_UINT maxlen)
{
	unsigned int i = 0;
	OV_UINT length = 0;

	if(!datapacket
			|| !datapacket->data
			|| !value)
		return OV_ERR_GENERIC;
	if(!value)
		return OV_ERR_BADPARAM;

	if(KS_DATAPACKET_read_xdr_u_long(datapacket, &length))
		return OV_ERR_GENERIC;

	//check if it fits into the buffer (do not forget terminating '\0')
	if(maxlen && (length >= (maxlen)))
	{
		datapacket->readPT -=4;		//reset readPT to initial state
		return OV_ERR_BADVALUE;
	}
	if(datapacket->readPT
			&& (datapacket->readPT >= datapacket->data)
			&& (datapacket->readPT <= (datapacket->data + datapacket->length)))
	{
		*value = ov_memstack_alloc(length+1);
		if(!(*value))
			return OV_ERR_HEAPOUTOFMEMORY;

		//copy it to the buffer
		for(i=0; i<length; i++)
			(*value)[i] = datapacket->readPT[i];
		(*value)[length] = '\0';

		//xdrs are aligned in 4byte blocks
		while(length%4)
			length++;
		datapacket->readPT += length;
		return OV_ERR_OK;
	}
	return OV_ERR_GENERIC;
}

OV_DLLFNCEXPORT OV_RESULT KS_DATAPACKET_read_xdr_string_tomemstack_wolength(KS_DATAPACKET* datapacket, OV_STRING* value)
{
	return KS_DATAPACKET_read_xdr_string_tomemstack(datapacket, value, ~0);
}

OV_DLLFNCEXPORT OV_RESULT KS_DATAPACKET_write_xdr_string(KS_DATAPACKET* datapacket, const OV_STRING* value)
{
	unsigned char* temp = NULL;
	unsigned int length;
	unsigned int xdradd;
	unsigned int i = 0;
	OV_RESULT result;

	/*	NULL-pointers are treated as empty strings because the xdr-stream structure is predefined	*/
	if(!value || !(*value))
		return KS_DATAPACKET_write_xdr_u_long(datapacket, (OV_UINT*) &i);	/*	at this point i contains 0	*/


	ov_memstack_lock();
	length = strlen(*value);
	xdradd = length;
	while(xdradd%4)	/*	care for alignment	*/
		xdradd++;
	temp = ov_memstack_alloc(xdradd + 4);		/*	reserve space for length (4bytes) and string	*/
	if(!temp)
		return OV_ERR_HEAPOUTOFMEMORY;
		/*	copy in length	*/
	for(i=0; i<4; i++)
		temp[3-i] = ((char*)&length)[i];
	/*	copy in data	*/
	memcpy((temp+4), *value, length);
	result = ksbase_KSDATAPACKET_append(datapacket, temp, xdradd+4);
	ov_memstack_unlock();
	return result;
}

/*
 * opaque data (byte string of dynamic length)
 */

OV_DLLFNCEXPORT OV_RESULT KS_DATAPACKET_read_xdr_opaque(KS_DATAPACKET* datapacket, char** value, OV_UINT* decLength, OV_UINT maxlen)
{
	unsigned int i = 0;
	OV_UINT length = 0;

	if(!datapacket
			|| !datapacket->data
			|| !value)
		return OV_ERR_GENERIC;
	if(!value)
		return OV_ERR_BADPARAM;

	if(KS_DATAPACKET_read_xdr_u_long(datapacket, &length))
		return OV_ERR_GENERIC;
	if(!length)
	{
		*decLength = 0;
		*value = NULL;
		return OV_ERR_OK;
	}
	//check if it fits into the buffer
	if(maxlen && (length >= (maxlen)))
	{
		datapacket->readPT -=4;		//reset readPT to initial state
		return OV_ERR_BADVALUE;
	}

	if(datapacket->readPT
			&& (datapacket->readPT >= datapacket->data)
			&& (datapacket->readPT <= (datapacket->data + datapacket->length)))
	{
		*value = ov_malloc(length);
		if(!(*value))
			return OV_ERR_HEAPOUTOFMEMORY;

		*decLength = length;
		//copy it to the buffer
		for(i=0; i<length; i++)
			(*value)[i] = datapacket->readPT[i];

		//xdrs are aligned in 4byte blocks
		while(length%4)
			length++;
		datapacket->readPT += length;
		return OV_ERR_OK;
	}
	return OV_ERR_GENERIC;
}

OV_DLLFNCEXPORT OV_RESULT KS_DATAPACKET_read_xdr_opaque_tomemstack(KS_DATAPACKET* datapacket, char** value, OV_UINT* decLength, OV_UINT maxlen)
{
	unsigned int i = 0;
	OV_UINT length = 0;

	if(!datapacket
			|| !datapacket->data
			|| !value)
		return OV_ERR_GENERIC;
	if(!value)
		return OV_ERR_BADPARAM;

	if(KS_DATAPACKET_read_xdr_u_long(datapacket, &length))
		return OV_ERR_GENERIC;
	if(!length)
	{
		*decLength = 0;
		*value = NULL;
		return OV_ERR_OK;
	}
	//check if it fits into the buffer
	if(maxlen && (length >= (maxlen)))
	{
		datapacket->readPT -=4;		//reset readPT to initial state
		return OV_ERR_BADVALUE;
	}

	if(datapacket->readPT
			&& (datapacket->readPT >= datapacket->data)
			&& (datapacket->readPT <= (datapacket->data + datapacket->length)))
	{
		*value = ov_memstack_alloc(length);
		if(!(*value))
			return OV_ERR_HEAPOUTOFMEMORY;

		*decLength = length;
		//copy it to the buffer
		for(i=0; i<length; i++)
			(*value)[i] = datapacket->readPT[i];

		//xdrs are aligned in 4byte blocks
		while(length%4)
			length++;
		datapacket->readPT += length;
		return OV_ERR_OK;
	}
	return OV_ERR_GENERIC;
}

OV_DLLFNCEXPORT OV_RESULT KS_DATAPACKET_write_xdr_opaque(KS_DATAPACKET* datapacket, const char* value, OV_UINT length)
{
	unsigned char* temp = 0;
	unsigned int xdradd;
	unsigned int i = 0;
	OV_RESULT result;


	/*	NULL-pointers are treated as empty values because the xdr-stream structure is predefined	*/
	if(!value)
		return KS_DATAPACKET_write_xdr_u_long(datapacket, (OV_UINT*) &i);	/*	at this point i contains 0	*/

	xdradd = length;
	while(xdradd%4)	/*	alignment!	*/
		xdradd++;

	ov_memstack_lock();
	temp = ov_memstack_alloc(xdradd + 4);	/*	reserver space for length (4bytes) and data	*/
	if(!temp)
		return OV_ERR_HEAPOUTOFMEMORY;
	for(i=0; i<4; i++)
		temp[3-i] = ((char*)&length)[i];
	memcpy((temp+4), value, length);
	result = ksbase_KSDATAPACKET_append(datapacket, temp, xdradd+4);
	ov_memstack_unlock();
	return result;
}


/*
 * opaque data with fixed length (byte string of fixed length)
 */
OV_DLLFNCEXPORT OV_RESULT KS_DATAPACKET_read_xdr_opaque_fixedlength(KS_DATAPACKET* datapacket, char** value, OV_UINT length)
{
	unsigned int i = 0;

	if(!datapacket
			|| !datapacket->data
			|| !value)
		return OV_ERR_GENERIC;
	if(!value)
		return OV_ERR_BADPARAM;

	if(datapacket->readPT
			&& (datapacket->readPT >= datapacket->data)
			&& (datapacket->readPT <= (datapacket->data + datapacket->length)))
	{
		//copy it to the buffer
		for(i=0; i<length; i++)
			(*value)[i] = datapacket->readPT[i];

		//xdrs are aligned in 4byte blocks
		while(length%4)
			length++;
		datapacket->readPT += length;
		return OV_ERR_OK;
	}
	return OV_ERR_GENERIC;
}

OV_DLLFNCEXPORT OV_RESULT KS_DATAPACKET_write_xdr_opaque_fixedlength(KS_DATAPACKET* datapacket, const char* value, OV_UINT length)
{
	OV_RESULT result;
	unsigned char* temp = NULL;
	unsigned int xdradd;


	if(value && !(length%4))
		return ksbase_KSDATAPACKET_append(datapacket, (unsigned char*) value, length);
	else
	{
		xdradd = length;
		while(xdradd%4)
			xdradd++;
		ov_memstack_lock();
		temp = ov_memstack_alloc(xdradd);	/*	reserve more space for alignment	*/
		if(!temp)
			return OV_ERR_HEAPOUTOFMEMORY;
		if(value)
			memcpy(temp, value, length);
		else
			memset(temp, 0, xdradd);
		result = ksbase_KSDATAPACKET_append(datapacket, temp, xdradd);
		ov_memstack_unlock();
		return result;
	}
}


/*
 * This function reads an array from an xdr-stream to the memstack. it uses readfnc for each element.
 * if you want to read a string you have to set the read_string_tomemstack_wolength function
 */
OV_DLLFNCEXPORT OV_RESULT KS_DATAPACKET_read_xdr_array_tomemstack(KS_DATAPACKET* datapacket, void** value, OV_UINT elementSize,
		OV_UINT* length, OV_RESULT (*readfnc)(KS_DATAPACKET*, void*))
{
	OV_RESULT result;
	OV_UINT i;
	OV_BYTE* temp;

	if(!value || ! length)
		return OV_ERR_BADPARAM;

	result = KS_DATAPACKET_read_xdr_u_long(datapacket, length);
	if(Ov_Fail(result))
		return result;

	temp = ov_memstack_alloc(*length * elementSize);
	if(!temp)
		return OV_ERR_HEAPOUTOFMEMORY;

	*value = temp;

	for(i=0; i<*length; i++)
	{
		result = (*readfnc)(datapacket, temp);
		if(Ov_Fail(result))
			return result;
		temp += elementSize;
	}

	return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_RESULT KS_DATAPACKET_write_xdr_array(KS_DATAPACKET* datapacket, void** value, OV_UINT elementSize,
		OV_UINT* length, OV_RESULT (*writefnc)(KS_DATAPACKET*, void*))
{
	OV_RESULT result;
	OV_UINT i;
	OV_BYTE* tempptr;

	if(!value || !length)
		return OV_ERR_BADPARAM;

	result = KS_DATAPACKET_write_xdr_u_long(datapacket, length);
	if(Ov_Fail(result))
		return result;

	tempptr = *value;
	for(i=0; i<*length; i++)
	{
		result = (*writefnc)(datapacket, tempptr);
		if(Ov_Fail(result))
			return result;
		tempptr += elementSize;
	}
	return OV_ERR_OK;
}


