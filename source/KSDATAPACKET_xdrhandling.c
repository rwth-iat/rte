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
#include <stdint.h>
#include "KSDATAPACKET_xdrhandling.h"
#include "ov_ksserver_backend.h"
/*
 * unsigned long (OV_UINT)
 */

OV_DLLFNCEXPORT OV_RESULT KS_DATAPACKET_read_xdr_uint(KS_DATAPACKET* datapacket, OV_UINT* value)
{
	if(!datapacket
			|| !datapacket->data)
		return OV_ERR_GENERIC;
	if(!value)
		return OV_ERR_BADPARAM;
	if(datapacket->readPT
			&& (datapacket->readPT >= datapacket->data)
			&& (datapacket->readPT <= (datapacket->data + datapacket->length)))
	{
		/*	shift left is defined als multiplication with powers of 2
		 * so it does the same independent of endianess of host system	*/
		*value = (OV_UINT)(((datapacket->readPT[0]) << 24)
		                 | ((datapacket->readPT[1]) << 16)
		                 | ((datapacket->readPT[2]) << 8)
		                 | ((datapacket->readPT[3])));
		datapacket->readPT += 4;
		return OV_ERR_OK;
	}

	return OV_ERR_GENERIC;
}

OV_DLLFNCEXPORT OV_RESULT KS_DATAPACKET_write_xdr_uint(KS_DATAPACKET* datapacket, const OV_UINT* value)
{
	unsigned char temp[4];

	if(value)
	{
		temp[0] = (unsigned char)((*value & 0xFF000000) >> 24);
		temp[1] = (unsigned char)((*value & 0x00FF0000) >> 16);
		temp[2] = (unsigned char)((*value & 0x0000FF00) >> 8);
		temp[3] = (unsigned char)((*value & 0x000000FF));
	}
	else
		memset(temp, 0, 4);
	return ksbase_KSDATAPACKET_append(datapacket, temp, 4);
}

/*
 * long (OV_INT)
 */
OV_DLLFNCEXPORT OV_RESULT KS_DATAPACKET_read_xdr_int(KS_DATAPACKET* datapacket, OV_INT* value)
{
	if(!datapacket
			|| !datapacket->data)
		return OV_ERR_GENERIC;
	if(!value)
		return OV_ERR_BADPARAM;

	if(datapacket->readPT
			&& (datapacket->readPT >= datapacket->data)
			&& (datapacket->readPT <= (datapacket->data + datapacket->length)))
	{
		*value = (OV_INT)(((datapacket->readPT[0]) << 24)
		                | ((datapacket->readPT[1]) << 16)
		                | ((datapacket->readPT[2]) << 8)
		                | ((datapacket->readPT[3])));
		datapacket->readPT += 4;
		return OV_ERR_OK;
	}

	return OV_ERR_GENERIC;
}

OV_DLLFNCEXPORT OV_RESULT KS_DATAPACKET_write_xdr_int(KS_DATAPACKET* datapacket, const OV_INT* value)
{
	unsigned char temp[4];

	if(value)
	{
		temp[0] = (unsigned char)((*value & 0xFF000000) >> 24);
		temp[1] = (unsigned char)((*value & 0x00FF0000) >> 16);
		temp[2] = (unsigned char)((*value & 0x0000FF00) >> 8);
		temp[3] = (unsigned char)((*value & 0x000000FF));
	}
	else
	{
		memset(temp, 0, 4);
	}
	return ksbase_KSDATAPACKET_append(datapacket, temp, 4);
}

/*
 * Single (OV_SINGLE)
 */
OV_DLLFNCEXPORT OV_RESULT KS_DATAPACKET_read_xdr_single(KS_DATAPACKET* datapacket, OV_SINGLE* value)
{
	OV_UINT temp;

	if(!datapacket
			|| !datapacket->data)
		return OV_ERR_GENERIC;
	if(!value)
		return OV_ERR_BADPARAM;

	if(datapacket->readPT
			&& (datapacket->readPT >= datapacket->data)
			&& (datapacket->readPT <= (datapacket->data + datapacket->length)))
	{
		temp = (OV_UINT)((OV_UINT)((datapacket->readPT[0]) << 24)
		               | (OV_UINT)((datapacket->readPT[1]) << 16)
		               | (OV_UINT)((datapacket->readPT[2]) << 8)
		               | (OV_UINT)((datapacket->readPT[3])));
		datapacket->readPT += 4;
		memcpy(value, &temp, sizeof(OV_UINT));
		return OV_ERR_OK;
	}

	return OV_ERR_GENERIC;
}

OV_DLLFNCEXPORT OV_RESULT KS_DATAPACKET_write_xdr_single(KS_DATAPACKET* datapacket, const OV_SINGLE* value)
{
	unsigned char temp[4];
	OV_UINT temp2;

	if(value)
	{
		memcpy(&temp2, value, sizeof(OV_UINT));
		temp[0] = (unsigned char)((temp2 & 0xFF000000) >> 24);
		temp[1] = (unsigned char)((temp2 & 0x00FF0000) >> 16);
		temp[2] = (unsigned char)((temp2 & 0x0000FF00) >> 8);
		temp[3] = (unsigned char)((temp2 & 0x000000FF));
	}
	else
	{
		memset(temp, 0, 4);
	}
	return ksbase_KSDATAPACKET_append(datapacket, temp, 4);
}

/*
 * double (OV_DOUBLE)
 */

OV_DLLFNCEXPORT OV_RESULT KS_DATAPACKET_read_xdr_double(KS_DATAPACKET* datapacket, OV_DOUBLE* value)
{
#ifndef ARCH_OLD_ARM
	uint64_t temp;
#else
	unsigned char temp[8];
#endif

	if(!datapacket
			|| !datapacket->data)
		return OV_ERR_GENERIC;
	if(!value)
		return OV_ERR_BADPARAM;

	if(datapacket->readPT
			&& (datapacket->readPT >= datapacket->data)
			&& (datapacket->readPT <= (datapacket->data + datapacket->length)))
	{
		/*	some "old" arm cores represent the double format in two little endian DWORDs with the most significant word first^^	*/
#ifndef ARCH_OLD_ARM
		temp = (((uint64_t)(datapacket->readPT[0]) << 56)
		      | ((uint64_t)(datapacket->readPT[1]) << 48)
		      | ((uint64_t)(datapacket->readPT[2]) << 40)
		      | ((uint64_t)(datapacket->readPT[3]) << 32)
		      | ((uint64_t)(datapacket->readPT[4]) << 24)
		      | ((uint64_t)(datapacket->readPT[5]) << 16)
		      | ((uint64_t)(datapacket->readPT[6]) << 8)
		      | ((uint64_t)(datapacket->readPT[7])));
		memcpy(value, &temp, sizeof(OV_DOUBLE));
#else
		temp[0] = datapacket->readPT[3];
		temp[1] = datapacket->readPT[2];
		temp[2] = datapacket->readPT[1];
		temp[3] = datapacket->readPT[0];
		temp[4] = datapacket->readPT[7];
		temp[5] = datapacket->readPT[6];
		temp[6] = datapacket->readPT[5];
		temp[7] = datapacket->readPT[4];
		memcpy(value, temp, sizeof(OV_DOUBLE));
#endif
		datapacket->readPT += 8;
		return OV_ERR_OK;
	}

	return OV_ERR_GENERIC;
}

OV_DLLFNCEXPORT OV_RESULT KS_DATAPACKET_write_xdr_double(KS_DATAPACKET* datapacket, const OV_DOUBLE* value)
{
	unsigned char temp[8];
#ifndef ARCH_OLD_ARM
	uint64_t temp2;
#endif

	if(value)
	{
#ifndef ARCH_OLD_ARM
		memcpy(&temp2, value, sizeof(OV_DOUBLE));
		temp[0] = (unsigned char)((temp2 & 0xFF00000000000000) >> 56);
		temp[1] = (unsigned char)((temp2 & 0x00FF000000000000) >> 48);
		temp[2] = (unsigned char)((temp2 & 0x0000FF0000000000) >> 40);
		temp[3] = (unsigned char)((temp2 & 0x000000FF00000000) >> 32);
		temp[4] = (unsigned char)((temp2 & 0x00000000FF000000) >> 24);
		temp[5] = (unsigned char)((temp2 & 0x0000000000FF0000) >> 16);
		temp[6] = (unsigned char)((temp2 & 0x000000000000FF00) >> 8);
		temp[7] = (unsigned char)((temp2 & 0x00000000000000FF));
#else
		temp[0] = ((unsigned char*)value)[3];
		temp[1] = ((unsigned char*)value)[2];
		temp[2] = ((unsigned char*)value)[1];
		temp[3] = ((unsigned char*)value)[0];
		temp[4] = ((unsigned char*)value)[7];
		temp[5] = ((unsigned char*)value)[6];
		temp[6] = ((unsigned char*)value)[5];
		temp[7] = ((unsigned char*)value)[4];
#endif
	}
	else
		memset(temp, 0, 8);
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

	if(Ov_Fail(KS_DATAPACKET_read_xdr_uint(datapacket, &length)))
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

	if(Ov_Fail(KS_DATAPACKET_read_xdr_uint(datapacket, &length)))
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
		return KS_DATAPACKET_write_xdr_uint(datapacket, (OV_UINT*) &i);	/*	at this point i contains 0	*/


	ov_memstack_lock();
	length = strlen(*value);
	xdradd = length;
	while(xdradd%4)	/*	care for alignment	*/
		xdradd++;
	temp = ov_memstack_alloc(xdradd + 4);		/*	reserve space for length (4bytes) and string	*/
	if(!temp)
		return OV_ERR_HEAPOUTOFMEMORY;
	/*	initialize	*/
	memset(temp, 0, xdradd+4);
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

	if(Ov_Fail(KS_DATAPACKET_read_xdr_uint(datapacket, &length)))
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

	if(Ov_Fail(KS_DATAPACKET_read_xdr_uint(datapacket, &length)))
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
		return KS_DATAPACKET_write_xdr_uint(datapacket, (OV_UINT*) &i);	/*	at this point i contains 0	*/

	xdradd = length;
	while(xdradd%4)	/*	alignment!	*/
		xdradd++;

	ov_memstack_lock();
	temp = ov_memstack_alloc(xdradd + 4);	/*	reserver space for length (4bytes) and data	*/
	if(!temp)
		return OV_ERR_HEAPOUTOFMEMORY;
	/*	initialize	*/
	memset(temp, 0, xdradd+4);
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
		/*	initialize	*/
		memset(temp, 0, xdradd);
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

	result = KS_DATAPACKET_read_xdr_uint(datapacket, length);
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

	result = KS_DATAPACKET_write_xdr_uint(datapacket, length);
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


