#ifndef OV_COMPILE_LIBRARY_ksapi
#define OV_COMPILE_LIBRARY_ksapi
#endif


#include "ksapi.h"
#include "libov/ov_macros.h"
#include "libov/ov_path.h"
#include "libov/ov_logfile.h"
#include "xdrhandling.h"



OV_DLLFNCEXPORT OV_BOOL ksapi_setPkg_dosetPkg_get(
    OV_INSTPTR_ksapi_setPkg          pobj
) {
    return pobj->v_dosetPkg;
}

OV_DLLFNCEXPORT OV_RESULT ksapi_setPkg_dosetPkg_set(
    OV_INSTPTR_ksapi_setPkg          pobj,
    const OV_BOOL  value
) {
    pobj->v_dosetPkg = value;

	 if(value == TRUE)
	 {
		ksapi_setPkg_state_set(pobj, "start sending package");
		ksapi_setPkg_submit(pobj);
		pobj->v_dosetPkg = FALSE;
	 }		

    return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_STRING ksapi_setPkg_state_get(
	OV_INSTPTR_ksapi_setPkg			pobj
) {
	return pobj->v_state;
}

OV_DLLFNCEXPORT OV_RESULT ksapi_setPkg_state_set(
	OV_INSTPTR_ksapi_setPkg			pobj,
	const OV_STRING           		value
) {
	return ov_string_setvalue(&pobj->v_state, value);
}

OV_DLLFNCEXPORT void ksapi_setPkg_startup(
	OV_INSTPTR_ov_object 	pobj
) {
    ksapi_KSCommon_startup(pobj);
    return;
}

OV_DLLFNCEXPORT void ksapi_setPkg_submit(
		OV_INSTPTR_ksapi_setPkg	 		pobj
) {
	OV_INSTPTR_ov_object pchild = NULL;
	OV_INSTPTR_ksapi_KSCommon pchildksc = NULL;
	OV_INSTPTR_ksapi_setBool pchildsb = NULL;
	OV_INSTPTR_ksapi_setBoolVec pchildsbv = NULL;
	OV_INSTPTR_ksapi_setDouble pchildsd = NULL;
	OV_INSTPTR_ksapi_setDoubleVec pchildsdv = NULL;
	OV_INSTPTR_ksapi_setInt pchildsi = NULL;
	OV_INSTPTR_ksapi_setIntVec pchildsiv = NULL;
	OV_INSTPTR_ksapi_setSingle pchildssgl = NULL;
	OV_INSTPTR_ksapi_setSingleVec pchildssglv = NULL;
	OV_INSTPTR_ksapi_setString pchildsstr = NULL;
	OV_INSTPTR_ksapi_setStringVec pchildsstrv = NULL;
	OV_INSTPTR_ksapi_setUInt pchildsui = NULL;
	OV_INSTPTR_ksapi_setUIntVec pchildsuiv = NULL;
	OV_UINT childcount = 0;
	char *xdr=NULL;
   int xdrlength;
	int errorcode = 0;
	int c;
	int check = 1;

	OV_INSTPTR_ksapi_KSCommon pksc = Ov_StaticPtrCast(ksapi_KSCommon, pobj);
	OV_INSTPTR_ksapi_Channel channel = (OV_INSTPTR_ksapi_Channel)Ov_SearchChild(ov_containment, pobj, "channel");
	if(channel == NULL) {
	    ov_logfile_error("SETPKG object without Channel to use: %s", pobj->v_identifier);
	    return;
	}

	//get host and server
	pchildksc = (OV_INSTPTR_ksapi_KSCommon)Ov_GetFirstChild(ov_containment, (OV_INSTPTR_ov_domain)pobj);
	while(check == 1)
	{
		check = 0;
		if((Ov_CanCastTo(ksapi_Channel, pchildksc)))
		{
			pchildksc = (OV_INSTPTR_ksapi_KSCommon)Ov_GetNextChild(ov_containment, (OV_INSTPTR_ov_domain)pchildksc);
			check = 1;
		}
	}
	if(pchildksc && pchildksc->v_host && pchildksc->v_server)
	{
		ov_string_setvalue(&pobj->v_host, pchildksc->v_host);
		ov_string_setvalue(&pobj->v_server, pchildksc->v_server);
	}
	else
	{
		ksapi_setPkg_state_set(pobj, "ERROR: no host or server");
		return;
	}

	//make header
	generateheader(KS_SETVAR, &xdr, &xdrlength);

	//make body
	Ov_ForEachChild(ov_containment, (OV_INSTPTR_ov_domain)pobj, pchild)
	{
		if(Ov_CanCastTo(ksapi_setBool, pchild))
		{
			childcount++;
			pchildsb = Ov_StaticPtrCast(ksapi_setBool, pchild);
			if(pchildsb->v_path)
				generatesetbody(OV_VT_BOOL, &xdr, &xdrlength, pchildsb->v_path, &(pchildsb->v_sendbool),
						pchildsb->v_varTimeStamp.secs, pchildsb->v_varTimeStamp.usecs, pchildsb->v_varQState);
			else
				errorcode = -1;
		}
		else if(Ov_CanCastTo(ksapi_setBoolVec, pchild))
		{
			childcount++;
			pchildsbv = Ov_StaticPtrCast(ksapi_setBoolVec, pchild);
			if(pchildsbv->v_path)
				generatesetvecbody(OV_VT_BOOL_VEC, &xdr, &xdrlength, pchildsbv->v_path, &(pchildsbv->v_sendboolvec.value), pchildsbv->v_sendboolvec.veclen,
						pchildsbv->v_varTimeStamp.secs, pchildsbv->v_varTimeStamp.usecs, pchildsbv->v_varQState);
			else
				errorcode = -1;
		}
		else if(Ov_CanCastTo(ksapi_setDouble, pchild))
		{
			childcount++;
			pchildsd = Ov_StaticPtrCast(ksapi_setDouble, pchild);
			if(pchildsd->v_path)
				generatesetbody(OV_VT_DOUBLE, &xdr, &xdrlength, pchildsd->v_path, &(pchildsd->v_senddouble),
						pchildsd->v_varTimeStamp.secs, pchildsd->v_varTimeStamp.usecs, pchildsd->v_varQState);
			else
				errorcode = -1;
		}
		else if(Ov_CanCastTo(ksapi_setDoubleVec, pchild))
		{
			childcount++;
			pchildsdv = Ov_StaticPtrCast(ksapi_setDoubleVec, pchild);
			if(pchildsdv->v_path)
				generatesetvecbody(OV_VT_DOUBLE_VEC, &xdr, &xdrlength, pchildsdv->v_path, &(pchildsdv->v_senddoublevec.value), pchildsdv->v_senddoublevec.veclen,
						pchildsdv->v_varTimeStamp.secs, pchildsdv->v_varTimeStamp.usecs, pchildsdv->v_varQState);
			else
				errorcode = -1;
		}
		else if(Ov_CanCastTo(ksapi_setInt, pchild))
		{
			childcount++;
			pchildsi = Ov_DynamicPtrCast(ksapi_setInt, pchild);
			if(pchildsi->v_path)
				generatesetbody(OV_VT_INT, &xdr, &xdrlength, pchildsi->v_path, &(pchildsi->v_sendint),
						pchildsi->v_varTimeStamp.secs, pchildsi->v_varTimeStamp.usecs, pchildsi->v_varQState);
			else
				errorcode = -1;
		}
		else if(Ov_CanCastTo(ksapi_setIntVec, pchild))
		{
			childcount++;
			pchildsiv = Ov_StaticPtrCast(ksapi_setIntVec, pchild);
			if(pchildsiv->v_path)
				generatesetvecbody(OV_VT_INT_VEC, &xdr, &xdrlength, pchildsiv->v_path, (int*)&(pchildsiv->v_sendintvec.value), pchildsiv->v_sendintvec.veclen,
						pchildsiv->v_varTimeStamp.secs, pchildsiv->v_varTimeStamp.usecs, pchildsiv->v_varQState);
			else
				errorcode = -1;
		}
		else if(Ov_CanCastTo(ksapi_setSingle, pchild))
		{
			childcount++;
			pchildssgl = Ov_DynamicPtrCast(ksapi_setSingle, pchild);
			if(pchildssgl->v_path)
				generatesetbody(OV_VT_SINGLE, &xdr, &xdrlength, pchildssgl->v_path, &(pchildssgl->v_sendsingle),
						pchildssgl->v_varTimeStamp.secs, pchildssgl->v_varTimeStamp.usecs, pchildssgl->v_varQState);
			else
				errorcode = -1;
		}
		else if(Ov_CanCastTo(ksapi_setSingleVec, pchild))
		{
			childcount++;
			pchildssglv = Ov_StaticPtrCast(ksapi_setSingleVec, pchild);
			if(pchildssglv->v_path)
				generatesetvecbody(OV_VT_SINGLE_VEC, &xdr, &xdrlength, pchildssglv->v_path, &(pchildssglv->v_sendsinglevec.value), pchildssglv->v_sendsinglevec.veclen,
						pchildssglv->v_varTimeStamp.secs, pchildssglv->v_varTimeStamp.usecs, pchildssglv->v_varQState);
			else
				errorcode = -1;
		}
		else if(Ov_CanCastTo(ksapi_setString, pchild))
		{
			childcount++;
			pchildsstr = Ov_DynamicPtrCast(ksapi_setString, pchild);
			if(pchildsstr->v_path)
				generatesetbody(OV_VT_STRING, &xdr, &xdrlength, pchildsstr->v_path, &(pchildsstr->v_sendstring),
						pchildsstr->v_varTimeStamp.secs, pchildsstr->v_varTimeStamp.usecs, pchildsstr->v_varQState);
			else
				errorcode = -1;
		}
		else if(Ov_CanCastTo(ksapi_setStringVec, pchild))
		{
			childcount++;
			pchildsstrv = Ov_StaticPtrCast(ksapi_setStringVec, pchild);
			if(pchildsstrv->v_path)
				generatesetvecbody(OV_VT_STRING_VEC, &xdr, &xdrlength, pchildsstrv->v_path, &(pchildsstrv->v_sendstringvec.value), pchildsstrv->v_sendstringvec.veclen,
						pchildsstrv->v_varTimeStamp.secs, pchildsstrv->v_varTimeStamp.usecs, pchildsstrv->v_varQState);
			else
				errorcode = -1;
		}
		else if(Ov_CanCastTo(ksapi_setUInt, pchild))
		{
			childcount++;
			pchildsui = Ov_DynamicPtrCast(ksapi_setUInt, pchild);
			if(pchildsui->v_path)
				generatesetbody(OV_VT_UINT, &xdr, &xdrlength, pchildsui->v_path, &(pchildsui->v_senduint),
						pchildsui->v_varTimeStamp.secs, pchildsui->v_varTimeStamp.usecs, pchildsui->v_varQState);
			else
				errorcode = -1;
		}
		else if(Ov_CanCastTo(ksapi_setUIntVec, pchild))
		{
			childcount++;
			pchildsuiv = Ov_StaticPtrCast(ksapi_setUIntVec, pchild);
			if(pchildsuiv->v_path)
				generatesetvecbody(OV_VT_UINT, &xdr, &xdrlength, pchildsuiv->v_path, (unsigned int*)&(pchildsuiv->v_senduintvec.value), pchildsuiv->v_senduintvec.veclen,
						pchildsuiv->v_varTimeStamp.secs, pchildsuiv->v_varTimeStamp.usecs, pchildsuiv->v_varQState);
			else
				errorcode = -1;
		}
		else if(Ov_CanCastTo(ksapi_Channel, pchild))
		{
		}
		else
		{
			errorcode = -1;
		}
	}
	if(errorcode >= 0)
	{
		//set package length
		char *tmp = (char*)&childcount;
		for (c=0; c<4; c++)
			xdr[51-c] = tmp[c];

		//add rpcheader
		addrpcheader(&xdr, &xdrlength);

		//send
		ksapi_Channel_sendxdr(channel, pksc, xdr, xdrlength);
	}
	else
	{
		ksapi_setPkg_state_set(pobj, "ERROR: path or variable not set");
		return;
	}

	//print xdr_received
	//~ int j;
	//~ printf("\nxdr:\nlength: %d\n", xdrlength);
	//~ for (j = 0; j < xdrlength; j=j+4)
		//~ printf("%X %X %X %X     ", xdr[j], xdr[j+1], xdr[j+2], xdr[j+3]);
	//~ printf("\n\n");
	//~ for (j = 0; j < xdrlength; j=j+4)
		//~ printf("%c %c %c %c     ", xdr[j], xdr[j+1], xdr[j+2], xdr[j+3]);
	//~ printf("\n\n");

	//printxdr(xdr, xdrlength);

	free(xdr);

    return;
}

OV_DLLFNCEXPORT void ksapi_setPkg_returnMethodxdr(
		OV_INSTPTR_ksapi_KSCommon			 	kscommon,
		OV_STRING									xdr,
		OV_INT										xdrlength
) {
    /*    
    *   local variables
    */
    OV_STRING errorstring = "";
	 int *errorcode;
    int finalerrorcode=0;
    int number;
    char temp[4];
    int j;
    
   // printxdr(xdr, xdrlength);

	//~ printf("\nreceivedxdr:\nlength: %d\n", xdrlength);
	//~ for (j = 0; j < xdrlength; j=j+4)
		//~ printf("%X %X %X %X     ", xdr[j], xdr[j+1], xdr[j+2], xdr[j+3]);
	//~ printf("\n\n");
	//~ for (j = 0; j < xdrlength; j=j+4)
		//~ printf("%c %c %c %c     ", xdr[j], xdr[j+1], xdr[j+2], xdr[j+3]);
	//~ printf("\n\n");

	for(j=0; j<4; j++) {
		temp[j] = xdr[35-j];
	}

	memcpy(&number, temp, 4);

	errorcode = (int*)malloc(number*sizeof(int));

	for(j=0;j<number;j++)
	{
		errorcode[j] = xdr[39+4*j];
		finalerrorcode = finalerrorcode+errorcode[j];
		switch (errorcode[j])
		{
			case KS_ERR_OK:
				ov_string_append(&errorstring, "OK; ");
				break;
			case KS_ERR_BADTYPE:
				ov_string_append(&errorstring, "ERR: BADTYPE; ");
				break;
			case KS_ERR_BADPATH:
				ov_string_append(&errorstring, "ERR: BADPATH; ");
				break;
			default:
				ov_string_append(&errorstring, "ERR: unknown; ");
		}
	}

	if(finalerrorcode == 0)
		ksapi_setPkg_state_set((OV_INSTPTR_ksapi_setPkg)kscommon, "setPkg OK");
	else
		ksapi_setPkg_state_set((OV_INSTPTR_ksapi_setPkg)kscommon, errorstring);

    return;
}

