
/******************************************************************************
*
*   FILE
*   ----
*   ProtocolIdentificator.c
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


OV_DLLFNCEXPORT OV_BOOL ksbase_ProtocolIdentificator_identify(
	OV_INSTPTR_ksbase_ProtocolIdentificator this,
	OV_INSTPTR_ksbase_Channel pchannel
) {
    ks_logfile_warning("This protocol identificator has no functionality: %s", this->v_identifier);
	return OV_ERR_OK;
}

