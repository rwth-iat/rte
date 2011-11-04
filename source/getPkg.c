#ifndef OV_COMPILE_LIBRARY_ksapi
#define OV_COMPILE_LIBRARY_ksapi
#endif


#include "ksapi.h"
#include "libov/ov_macros.h"
#include "libov/ov_path.h"
#include "libov/ov_logfile.h"
#include "xdrhandling.h"



OV_DLLFNCEXPORT OV_BOOL ksapi_getPkg_dogetPkg_get(
    OV_INSTPTR_ksapi_getPkg          pobj
) {
    return pobj->v_dogetPkg;
}

OV_DLLFNCEXPORT OV_RESULT ksapi_getPkg_dogetPkg_set(
    OV_INSTPTR_ksapi_getPkg          pobj,
    const OV_BOOL  value
) {
    pobj->v_dogetPkg = value;

	 if(value == TRUE)
	 {
		ksapi_getPkg_state_set(pobj, "start sending package");
		ksapi_getPkg_submit(pobj);
		pobj->v_dogetPkg = FALSE;
	 }		

    return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_STRING ksapi_getPkg_state_get(
	OV_INSTPTR_ksapi_getPkg			pobj
) {
	return pobj->v_state;
}

OV_DLLFNCEXPORT OV_RESULT ksapi_getPkg_state_set(
	OV_INSTPTR_ksapi_getPkg			pobj,
	const OV_STRING           		value
) {
	return ov_string_setvalue(&pobj->v_state, value);
}

OV_DLLFNCEXPORT void ksapi_getPkg_startup(
	OV_INSTPTR_ov_object 	pobj
) {
    /*    
    *   local variables
    */

    /* do what the base class does first */
    ksapi_KSCommon_startup(pobj);

    /* do what */


    return;
}

OV_DLLFNCEXPORT void ksapi_getPkg_submit(
		OV_INSTPTR_ksapi_getPkg	 		pobj
) {
	OV_INSTPTR_ov_object pchild = NULL;
	OV_INSTPTR_ksapi_KSCommon pchildksc = NULL;
	OV_INSTPTR_ksapi_getBool pchildgb = NULL;
	OV_INSTPTR_ksapi_getBoolVec pchildgbv = NULL;
	OV_INSTPTR_ksapi_getDouble pchildgd = NULL;
	OV_INSTPTR_ksapi_getDoubleVec pchildgdv = NULL;
	OV_INSTPTR_ksapi_getInt pchildgi = NULL;
	OV_INSTPTR_ksapi_getIntVec pchildgiv = NULL;
	OV_INSTPTR_ksapi_getSingle pchildgsgl = NULL;
	OV_INSTPTR_ksapi_getSingleVec pchildgsglv = NULL;
	OV_INSTPTR_ksapi_getString pchildgstr = NULL;
	OV_INSTPTR_ksapi_getStringVec pchildgstrv = NULL;
	OV_INSTPTR_ksapi_getUInt pchildgui = NULL;
	OV_INSTPTR_ksapi_getUIntVec pchildguiv = NULL;
	OV_UINT childcount = 0;
	char *xdr;
   int xdrlength;
	int errorcode = 0;
	int c;
	int check = 1;

	OV_INSTPTR_ksapi_KSCommon pksc = Ov_StaticPtrCast(ksapi_KSCommon, pobj);
	OV_INSTPTR_ksapi_Channel channel = (OV_INSTPTR_ksapi_Channel)Ov_SearchChild(ov_containment, pobj, "channel");
	if(channel == NULL) {
	    ov_logfile_error("GETPKG object without Channel to use: %s", pobj->v_identifier);
	    ksapi_getPkg_state_set(pobj, "GETPKG object without Channel to use");
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
	//~ printf("\n\n\nname: %s\n\n\n", pchildksc->v_identifier);
	if(pchildksc && pchildksc->v_host && pchildksc->v_server)
	{
		ov_string_setvalue(&pobj->v_host, pchildksc->v_host);
		ov_string_setvalue(&pobj->v_server, pchildksc->v_server);
	}
	else
	{
		ksapi_getPkg_state_set(pobj, "ERROR: no host or server");
		return;
	}

	//make header
	generategetheader(&xdr, &xdrlength);

	//make body
	Ov_ForEachChild(ov_containment, (OV_INSTPTR_ov_domain)pobj, pchild)
	{
		if(Ov_CanCastTo(ksapi_getBool, pchild))
		{
			childcount++;
			pchildgb = Ov_StaticPtrCast(ksapi_getBool, pchild);
			if(pchildgb->v_path)
				generategetbody(&xdr, &xdrlength, pchildgb->v_path);
			else
				errorcode = -1;
		}
		else if(Ov_CanCastTo(ksapi_getBoolVec, pchild))
		{
			childcount++;
			pchildgbv = Ov_StaticPtrCast(ksapi_getBoolVec, pchild);
			if(pchildgbv->v_path)
				generategetbody(&xdr, &xdrlength, pchildgbv->v_path);
			else
				errorcode = -1;
		}
		else if(Ov_CanCastTo(ksapi_getDouble, pchild))
		{
			childcount++;
			pchildgd = Ov_StaticPtrCast(ksapi_getDouble, pchild);
			if(pchildgd->v_path)
				generategetbody(&xdr, &xdrlength, pchildgd->v_path);
			else
				errorcode = -1;
		}
		else if(Ov_CanCastTo(ksapi_getDoubleVec, pchild))
		{
			childcount++;
			pchildgdv = Ov_StaticPtrCast(ksapi_getDoubleVec, pchild);
			if(pchildgdv->v_path)
				generategetbody(&xdr, &xdrlength, pchildgdv->v_path);
			else
				errorcode = -1;
		}
		else if(Ov_CanCastTo(ksapi_getInt, pchild))
		{
			childcount++;
			pchildgi = Ov_DynamicPtrCast(ksapi_getInt, pchild);
			if(pchildgi->v_path)
				generategetbody(&xdr, &xdrlength, pchildgi->v_path);
			else
				errorcode = -1;
		}
		else if(Ov_CanCastTo(ksapi_getIntVec, pchild))
		{
			childcount++;
			pchildgiv = Ov_StaticPtrCast(ksapi_getIntVec, pchild);
			if(pchildgiv->v_path)
				generategetbody(&xdr, &xdrlength, pchildgiv->v_path);
			else
				errorcode = -1;
		}
		else if(Ov_CanCastTo(ksapi_getSingle, pchild))
		{
			childcount++;
			pchildgsgl = Ov_DynamicPtrCast(ksapi_getSingle, pchild);
			if(pchildgsgl->v_path)
				generategetbody(&xdr, &xdrlength, pchildgsgl->v_path);
			else
				errorcode = -1;
		}
		else if(Ov_CanCastTo(ksapi_getSingleVec, pchild))
		{
			childcount++;
			pchildgsglv = Ov_StaticPtrCast(ksapi_getSingleVec, pchild);
			if(pchildgsglv->v_path)
				generategetbody(&xdr, &xdrlength, pchildgsglv->v_path);
			else
				errorcode = -1;
		}
		else if(Ov_CanCastTo(ksapi_getString, pchild))
		{
			childcount++;
			pchildgstr = Ov_DynamicPtrCast(ksapi_getString, pchild);
			if(pchildgstr->v_path)
				generategetbody(&xdr, &xdrlength, pchildgstr->v_path);
			else
				errorcode = -1;
		}
		else if(Ov_CanCastTo(ksapi_getStringVec, pchild))
		{
			childcount++;
			pchildgstrv = Ov_StaticPtrCast(ksapi_getStringVec, pchild);
			if(pchildgstrv->v_path)
				generategetbody(&xdr, &xdrlength, pchildgstrv->v_path);
			else
				errorcode = -1;
		}
		else if(Ov_CanCastTo(ksapi_getUInt, pchild))
		{
			childcount++;
			pchildgui = Ov_DynamicPtrCast(ksapi_getUInt, pchild);
			if(pchildgui->v_path)
				generategetbody(&xdr, &xdrlength, pchildgui->v_path);
			else
				errorcode = -1;
		}
		else if(Ov_CanCastTo(ksapi_getUIntVec, pchild))
		{
			childcount++;
			pchildguiv = Ov_StaticPtrCast(ksapi_getUIntVec, pchild);
			if(pchildguiv->v_path)
				generategetbody(&xdr, &xdrlength, pchildguiv->v_path);
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
			xdr[47-c] = tmp[c];

		//add rpcheader
		addrpcheader(&xdr, &xdrlength);

		//send
		ksapi_Channel_sendxdr(channel, pksc, xdr, xdrlength);
	}
	else
	{
		ksapi_getPkg_state_set(pobj, "ERROR: path or variable not set");
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

	free(xdr);

    return;
}

OV_DLLFNCEXPORT void ksapi_getPkg_returnMethodxdr(
		OV_INSTPTR_ksapi_KSCommon			 	kscommon,
		OV_STRING									xdr,
		OV_INT										xdrlength
) {
    /*    
    *   local variables
    */
    OV_INSTPTR_ksapi_getPkg this = Ov_StaticPtrCast(ksapi_getPkg, kscommon);
	 OV_INSTPTR_ov_object pchild = NULL;
	 OV_INSTPTR_ksapi_getBool pchildgb = NULL;
	 OV_INSTPTR_ksapi_getBoolVec pchildgbv = NULL;
	 OV_INSTPTR_ksapi_getDouble pchildgd = NULL;
	 OV_INSTPTR_ksapi_getDoubleVec pchildgdv = NULL;
	 OV_INSTPTR_ksapi_getInt pchildgi = NULL;
	 OV_INSTPTR_ksapi_getIntVec pchildgiv = NULL;
	 OV_INSTPTR_ksapi_getSingle pchildgsgl = NULL;
	 OV_INSTPTR_ksapi_getSingleVec pchildgsglv = NULL;
	 OV_INSTPTR_ksapi_getString pchildgstr = NULL;
	 OV_INSTPTR_ksapi_getStringVec pchildgstrv = NULL;
	 OV_INSTPTR_ksapi_getUInt pchildgui = NULL;
	 OV_INSTPTR_ksapi_getUIntVec pchildguiv = NULL;
    OV_UINT childcount = 0;
    OV_UINT packagelength;
    OV_UINT xdrposition;
    OV_UINT veclength = 0;
    int vartype;
    int i,j,k;
    char temp[8];
    OV_BOOL *boolvec;
    OV_DOUBLE *doublevec;
    OV_INT *intvec;
    OV_SINGLE *singlevec;
    OV_UINT strlength = 0;
    OV_STRING str;
    OV_STRING *strvec;
    OV_UINT strlengthges = 0;
    OV_UINT *uintvec;

	 
	//~ printf("\nreceivedxdr:\nlength: %d\n", xdrlength);
	//~ for (j = 0; j < xdrlength; j=j+4)
		//~ printf("%X %X %X %X     ", xdr[j], xdr[j+1], xdr[j+2], xdr[j+3]);
	//~ printf("\n\n");
	//~ for (j = 0; j < xdrlength; j=j+4)
		//~ printf("%c %c %c %c     ", xdr[j], xdr[j+1], xdr[j+2], xdr[j+3]);
	//~ printf("\n\n");

	//get packagelength
	for (i = 3; i >= 0; i--)
		temp[3-i] = xdr[36+i];
	memcpy(&packagelength, temp, 4);
	//~ printf("\n\npackagelength: %d\n\n", packagelength);

	xdrposition = 48;

	Ov_ForEachChild(ov_containment, (OV_INSTPTR_ov_domain)kscommon, pchild)
	{
		if(Ov_CanCastTo(ksapi_getBool, pchild))
		{
			childcount++;
			pchildgb = Ov_StaticPtrCast(ksapi_getBool, pchild);
			vartype = xdr[xdrposition+3];
			printf("\n\nvartype: %d\n\n", vartype);
			if(vartype==2)
			{
				pchildgb->v_receivebool = xdr[xdrposition+7];
				xdrposition = xdrposition+28;
			}
			else if (vartype==0)
			{
				ksapi_getPkg_state_set(this, "wrong path");
				return;
			}
			else if (vartype==48)
			{
				ksapi_getPkg_state_set(this, "wrong vartype");
				return;
			}
			else
			{
				ksapi_getPkg_state_set(this, "error");
				return;
			}
		}
		else if(Ov_CanCastTo(ksapi_getBoolVec, pchild))
		{
			childcount++;
			pchildgbv = Ov_StaticPtrCast(ksapi_getBoolVec, pchild);
			vartype = xdr[xdrposition+3];
			if(vartype==-126){
				//get veclength
				for (i = 3; i >= 0; i--)
					temp[3-i] = xdr[xdrposition+4+i];
				memcpy(&veclength, temp, 4);

				k = xdrposition+8;
				boolvec = malloc(veclength*sizeof(OV_BOOL));
				for (j = 0; j < veclength; j++)
				{
					for (i = 3; i >= 0; i--)
						temp[3-i] = xdr[k+i];
					memcpy(&(boolvec[j]), temp, 4);
					k = k+4;
				}
				ksapi_getBoolVec_receiveboolvec_set(pchildgbv, boolvec, veclength);
				free(boolvec);
				xdrposition = xdrposition+28+veclength*4;
			}
			else if (vartype==0)
			{
				ksapi_getPkg_state_set(this, "wrong path");
				return;
			}
			else if (vartype==48)
			{
				ksapi_getPkg_state_set(this, "wrong vartype");
				return;
			}
			else
			{
				ksapi_getPkg_state_set(this, "error");
				return;
			}
		}
		else if(Ov_CanCastTo(ksapi_getDouble, pchild))
		{
			childcount++;
			pchildgd = Ov_StaticPtrCast(ksapi_getDouble, pchild);
			vartype = xdr[xdrposition+3];
			if(vartype==33){
				for (i = 7; i >= 0; i--)
					temp[7-i] = xdr[xdrposition+4+i];
				memcpy(&pchildgd->v_receivedouble, temp, 8);
				xdrposition = xdrposition+32;
			}
			else if (vartype==0)
			{
				ksapi_getPkg_state_set(this, "wrong path");
				return;
			}
			else if (vartype==48)
			{
				ksapi_getPkg_state_set(this, "wrong vartype");
				return;
			}
			else
			{
				ksapi_getPkg_state_set(this, "error");
				return;
			}
		}
		else if(Ov_CanCastTo(ksapi_getDoubleVec, pchild))
		{
			childcount++;
			pchildgdv = Ov_StaticPtrCast(ksapi_getDoubleVec, pchild);
			vartype = xdr[xdrposition+3];
			if(vartype==-95){
				//get veclength
				for (i = 3; i >= 0; i--)
					temp[3-i] = xdr[xdrposition+4+i];
				memcpy(&veclength, temp, 4);

				k = xdrposition+8;
				doublevec = malloc(veclength*sizeof(OV_DOUBLE));
				for (j = 0; j < veclength; j++)
				{
					for (i = 7; i >= 0; i--)
						temp[7-i] = xdr[k+i];
					memcpy(&(doublevec[j]), temp, 8);
					k = k+8;
				}
				ksapi_getDoubleVec_receivedoublevec_set(pchildgdv, doublevec, veclength);
				free(doublevec);
				xdrposition = xdrposition+28+veclength*8;
			}
			else if (vartype==0)
			{
				ksapi_getPkg_state_set(this, "wrong path");
				return;
			}
			else if (vartype==48)
			{
				ksapi_getPkg_state_set(this, "wrong vartype");
				return;
			}
			else
			{
				ksapi_getPkg_state_set(this, "error");
				return;
			}
		}
		else if(Ov_CanCastTo(ksapi_getInt, pchild))
		{
			childcount++;
			pchildgi = Ov_DynamicPtrCast(ksapi_getInt, pchild);
			vartype = xdr[xdrposition+3];
			if(vartype==16){
				for (i = 3; i >= 0; i--)
					temp[3-i] = xdr[xdrposition+4+i];
				memcpy(&pchildgi->v_receiveint, temp, 4);
				xdrposition = xdrposition+28;
			}
			else if (vartype==0)
			{
				ksapi_getPkg_state_set(this, "wrong path");
				return;
			}
			else if (vartype==48)
			{
				ksapi_getPkg_state_set(this, "wrong vartype");
				return;
			}
			else
			{
				ksapi_getPkg_state_set(this, "error");
				return;
			}
		}
		else if(Ov_CanCastTo(ksapi_getIntVec, pchild))
		{
			childcount++;
			pchildgiv = Ov_StaticPtrCast(ksapi_getIntVec, pchild);
			vartype = xdr[xdrposition+3];
			if(vartype==-112){
				//get veclength
				for (i = 3; i >= 0; i--)
					temp[3-i] = xdr[xdrposition+4+i];
				memcpy(&veclength, temp, 4);

				k = xdrposition+8;
				intvec = malloc(veclength*sizeof(OV_INT));
				for (j = 0; j < veclength; j++)
				{
					for (i = 3; i >= 0; i--)
						temp[3-i] = xdr[k+i];
					memcpy(&(intvec[j]), temp, 4);
					k = k+4;
				}
				ksapi_getIntVec_receiveintvec_set(pchildgiv, intvec, veclength);
				free(intvec);
				xdrposition = xdrposition+28+veclength*4;
			}
			else if (vartype==0)
			{
				ksapi_getPkg_state_set(this, "wrong path");
				return;
			}
			else if (vartype==48)
			{
				ksapi_getPkg_state_set(this, "wrong vartype");
				return;
			}
			else
			{
				ksapi_getPkg_state_set(this, "error");
				return;
			}
		}
		else if(Ov_CanCastTo(ksapi_getSingle, pchild))
		{
			childcount++;
			pchildgsgl = Ov_DynamicPtrCast(ksapi_getSingle, pchild);
			vartype = xdr[xdrposition+3];
			if(vartype==32){
				for (i = 3; i >= 0; i--)
					temp[3-i] = xdr[xdrposition+4+i];
				memcpy(&pchildgsgl->v_receivesingle, temp, 4);
				xdrposition = xdrposition+28;
			}
			else if (vartype==0)
			{
				ksapi_getPkg_state_set(this, "wrong path");
				return;
			}
			else if (vartype==48)
			{
				ksapi_getPkg_state_set(this, "wrong vartype");
				return;
			}
			else
			{
				ksapi_getPkg_state_set(this, "error");
				return;
			}
		}
		else if(Ov_CanCastTo(ksapi_getSingleVec, pchild))
		{
			childcount++;
			pchildgsglv = Ov_StaticPtrCast(ksapi_getSingleVec, pchild);
			vartype = xdr[xdrposition+3];
			if(vartype==-96){
				//get veclength
				for (i = 3; i >= 0; i--)
					temp[3-i] = xdr[xdrposition+4+i];
				memcpy(&veclength, temp, 4);

				k = xdrposition+8;
				singlevec = malloc(veclength*sizeof(OV_SINGLE));
				for (j = 0; j < veclength; j++)
				{
					for (i = 3; i >= 0; i--)
						temp[3-i] = xdr[k+i];
					memcpy(&(singlevec[j]), temp, 4);
					k = k+4;
				}
				ksapi_getSingleVec_receivesinglevec_set(pchildgsglv, singlevec, veclength);
				free(singlevec);
				xdrposition = xdrposition+28+veclength*4;
			}
			else if (vartype==0)
			{
				ksapi_getPkg_state_set(this, "wrong path");
				return;
			}
			else if (vartype==48)
			{
				ksapi_getPkg_state_set(this, "wrong vartype");
				return;
			}
			else
			{
				ksapi_getPkg_state_set(this, "error");
				return;
			}
		}
		else if(Ov_CanCastTo(ksapi_getString, pchild))
		{
			childcount++;
			pchildgstr = Ov_DynamicPtrCast(ksapi_getString, pchild);
			vartype = xdr[xdrposition+3];
			if(vartype==48){
				//get strlength
				for (i = 3; i >= 0; i--)
					temp[3-i] = xdr[xdrposition+4+i];
				memcpy(&strlength, temp, 4);

				str = (char*)malloc((strlength+1)*sizeof(char));
				memset(str, 0, strlength+1);
				for (i = 0; i < strlength; i++)
					str[i] = xdr[xdrposition+8+i];
				ksapi_getString_receivestring_set(pchildgstr, str);
				free(str);
				while ((strlength%4) != 0)
					strlength++;
				xdrposition = xdrposition+28+strlength;
			}
			else if (vartype==0)
			{
				ksapi_getPkg_state_set(this, "wrong path");
				return;
			}
			else if (vartype==48)
			{
				ksapi_getPkg_state_set(this, "wrong vartype");
				return;
			}
			else
			{
				ksapi_getPkg_state_set(this, "error");
				return;
			}
		}
		else if(Ov_CanCastTo(ksapi_getStringVec, pchild))
		{
			childcount++;
			pchildgstrv = Ov_StaticPtrCast(ksapi_getStringVec, pchild);
			vartype = xdr[xdrposition+3];
			if(vartype==-80){
				//get veclength
				for (i = 3; i >= 0; i--)
					temp[3-i] = xdr[xdrposition+4+i];
				memcpy(&veclength, temp, 4);
				strvec = (OV_STRING*)malloc(veclength*sizeof(char*));
				
				k = xdrposition+8;
				for (i = 0; i < veclength; i++)
				{
					//get strlength
					for (j = 3; j >= 0; j--)
						temp[3-j] = xdr[k+j];
					memcpy(&strlength, temp, 4);
					strlengthges = strlengthges+strlength;
					while ((strlengthges%4) != 0)
						strlengthges++;
					k = k+4;

					strvec[i] = (OV_STRING)malloc((strlength+1)*sizeof(char));
					memset(strvec[i], 0, strlength+1);
					for (j = 0; j < strlength; j++)
					{
						strvec[i][j] = xdr[k+j];
						strvec[i][j+1] = 0;
					}
					k = k+strlength;
					while (k%4 != 0)
						k++;
				}
				ksapi_getStringVec_receivestringvec_set(pchildgstrv, strvec, veclength);
				free(strvec);
				xdrposition = xdrposition+28+veclength*4+strlengthges;
			}
			else if (vartype==0)
			{
				ksapi_getPkg_state_set(this, "wrong path");
				return;
			}
			else if (vartype==48)
			{
				ksapi_getPkg_state_set(this, "wrong vartype");
				return;
			}
			else
			{
				ksapi_getPkg_state_set(this, "error");
				return;
			}
		}
		else if(Ov_CanCastTo(ksapi_getUInt, pchild))
		{
			childcount++;
			pchildgui = Ov_DynamicPtrCast(ksapi_getUInt, pchild);
			vartype = xdr[xdrposition+3];
			if(vartype==17){
				for (i = 3; i >= 0; i--)
					temp[3-i] = xdr[xdrposition+4+i];
				memcpy(&pchildgui->v_receiveuint, temp, 4);
				xdrposition = xdrposition+28;
			}
			else if (vartype==0)
			{
				ksapi_getPkg_state_set(this, "wrong path");
				return;
			}
			else if (vartype==48)
			{
				ksapi_getPkg_state_set(this, "wrong vartype");
				return;
			}
			else
			{
				ksapi_getPkg_state_set(this, "error");
				return;
			}
		}
		else if(Ov_CanCastTo(ksapi_getUIntVec, pchild))
		{
			childcount++;
			pchildguiv = Ov_StaticPtrCast(ksapi_getUIntVec, pchild);
			vartype = xdr[xdrposition+3];
			if(vartype==-111){
				//get veclength
				for (i = 3; i >= 0; i--)
					temp[3-i] = xdr[xdrposition+4+i];
				memcpy(&veclength, temp, 4);

				k = xdrposition+8;
				uintvec = malloc(veclength*sizeof(OV_UINT));
				for (j = 0; j < veclength; j++)
				{
					for (i = 3; i >= 0; i--)
						temp[3-i] = xdr[k+i];
					memcpy(&(uintvec[j]), temp, 4);
					k = k+4;
				}
				ksapi_getUIntVec_receiveuintvec_set(pchildguiv, uintvec, veclength);
				free(uintvec);
				xdrposition = xdrposition+28+veclength*4;
			}
			else if (vartype==0)
			{
				ksapi_getPkg_state_set(this, "wrong path");
				return;
			}
			else if (vartype==48)
			{
				ksapi_getPkg_state_set(this, "wrong vartype");
				return;
			}
			else
			{
				ksapi_getPkg_state_set(this, "error");
				return;
			}
		}
		else if(Ov_CanCastTo(ksapi_Channel, pchild))
		{
		}
	}

	ksapi_getPkg_state_set(this, "sending complete");

    return;
}
