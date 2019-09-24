
/******************************************************************************
*
*   FILE
*   ----
*   xdrIdentificator.c
*
*   History
*   -------
*   2013-01-28   File created
*
*******************************************************************************
*
*   This file is generated by the 'acplt_builder' command
*
******************************************************************************/


#ifndef OV_COMPILE_LIBRARY_ksxdr
#define OV_COMPILE_LIBRARY_ksxdr
#endif

#include "ksbase.h"
#include "ks_logfile.h"
#include "ksxdr.h"
#include "ov_macros.h"
#include "ov_result.h"
#include "ksxdr_config.h"
#include "ksbase_helper.h"

OV_DLLFNCEXPORT OV_RESULT ksxdr_xdrIdentificator_constructor(
	OV_INSTPTR_ov_object 	pobj
) {

	OV_RESULT result;
	OV_INSTPTR_ksxdr_xdrIdentificator this = Ov_StaticPtrCast(ksxdr_xdrIdentificator, pobj);

	/* do what the base class does first */
	result = ov_object_constructor(pobj);
	if(Ov_Fail(result))
		return result;

	/*	set protoclID	*/
	return ov_string_setvalue(&(this->v_protocolID), KSXDR_IDENTIFIER);
}

OV_DLLFNCEXPORT OV_BOOL ksxdr_xdrIdentificator_identify (
	OV_INSTPTR_ksbase_ProtocolIdentificator this,
	OV_INSTPTR_ksbase_Channel pchannel
) {
    /*    
    *   local variables
    */
	OV_INSTPTR_ksxdr_xdrIdentificator thisId = Ov_StaticPtrCast(ksxdr_xdrIdentificator, this);
	char tempstr[4];
	OV_UINT* pprognr = (OV_UINT*) tempstr;
	unsigned int i = 0;
	unsigned int offset;

	KS_logfile_debug(("%s: identify called", thisId->v_identifier));
	/*
	 * check if first byte is valid for an xdr stream (0x80 for last fragment or 0x00 for other fragments
	 */
	if(pchannel->v_usesStreamProtocol == FALSE
			|| ((*(pchannel->v_inData.readPT) == (OV_BYTE) 0x00)
					|| (*(pchannel->v_inData.readPT) == (OV_BYTE) 0x80)))
	{
		/*
		 * Check if rpc ID is the right one
		 */
		//for strem protocols like tcp the xdr program nummer has an offset of 16 in the request otherwise the offset is 12
		if(pchannel->v_usesStreamProtocol == TRUE){
			offset = 16;
		} else {
			offset = 12;
		}
		//Endian conversion (we do NOT use an XDR-function here since we do NOT want to modify the read pointer)
		for(i=0; i<4; i++)		//offset is offset of program number in xdr
			tempstr[3-i] = *(pchannel->v_inData.readPT + offset + i);


		if(*pprognr == thisId->v_ksProgramnumber)
		{
			KS_logfile_debug(("%s: first byte and program number (%x) valid. XDR-ks identified", thisId->v_identifier, *pprognr));
			return TRUE;
		}
		else
			return FALSE;
	}
    return FALSE;
}

OV_DLLFNCEXPORT OV_RESULT ksxdr_xdrIdentificator_createClientHandler (
	OV_INSTPTR_ksbase_ProtocolIdentificator this,
	OV_INSTPTR_ksbase_Channel pchannel
) {
    /*    
    *   local variables
    */
	OV_INSTPTR_ksxdr_xdrClientHandler pClientHandler = NULL;
	OV_RESULT result = OV_ERR_OK;

	result = Ov_CreateIDedObject(ksxdr_xdrClientHandler, pClientHandler, this, "XDRClientHandler");
	if(Ov_OK(result))
	{
		KS_logfile_debug(("%s: ClientHandler created: %s", this->v_identifier, pClientHandler->v_identifier));
		result = Ov_Link(ksbase_AssocChannelClientHandler, pchannel, pClientHandler);
		if(Ov_Fail(result))
		{
			KS_logfile_error(("%s: could not link ClientHandler %s to Channel %s", this->v_identifier, pClientHandler->v_identifier, pchannel->v_identifier));
			return result;
		}
		else
		{
			result = ov_string_setvalue(&(pClientHandler->v_sourceAdr), pchannel->v_address);
			if(Ov_Fail(result))
			{
				KS_logfile_error(("%s: could not set ClientHandler %s's sourceAdr. reason: %s", this->v_identifier, pClientHandler->v_identifier, ov_result_getresulttext(result)));
			}
			return OV_ERR_OK;
		}
	}
	else
	{
		KS_logfile_error(("%s: could not create ClientHandler: %s", this->v_identifier, ov_result_getresulttext(result)));
		return result;
	}
}

