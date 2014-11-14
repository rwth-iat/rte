
/******************************************************************************
*
*   FILE
*   ----
*   DataHandler.c
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
#include "ks_logfile.h"
#include <stdio.h>



/**
 * This function is called by a channel object when there is data for this DataHandler. It is used as some kind of callback function.
 * @param OV_INSTPTR_ksbase_DataHandler this: a pointer to the DataHandler itself
 * @param KS_DATAPACKET* dataReceived: the received data the ClientHandler has to work with. The calling Channel will set this to its inData variable
 * @param KS_DATAPACKET* answer: the answer generated by the ClientHandler. The calling Channel  will set this to its outData variable.
 */
OV_DLLFNCEXPORT OV_RESULT ksbase_DataHandler_HandleData(
	OV_INSTPTR_ksbase_DataHandler this,
	OV_INSTPTR_ksbase_Channel pChannel,
	KS_DATAPACKET* dataReceived,
	KS_DATAPACKET* answer
)	{
#if LOG_KS || LOG_KS_DEBUG
	OV_UINT i = 0;
	char temp [9];
#endif
	KS_logfile_warning(("Empty HandleData function. No functionality implemented for DataHandler %s of class %s", this->v_identifier, Ov_GetParent(ov_instantiation, this)->v_identifier));
#if LOG_KS || LOG_KS_DEBUG
	KS_logfile_debug(("DataHandler: %s dataReceived first 64 bytes:\n"));

	for(i=0; i < 8; i++)
	{
		snprintf(temp, 9, (char*)&(dataReceived->data[i*8]));
		fprintf(stdout, "\t%#08x %08x %s\n", (unsigned int) dataReceived->data[i*8], (unsigned int) dataReceived->data[i*8+4], temp);
	}
#endif
	return OV_ERR_OK;
}

