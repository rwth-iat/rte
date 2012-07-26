#ifndef OV_COMPILE_LIBRARY_ksapi
#define OV_COMPILE_LIBRARY_ksapi
#endif


#include "ksapi.h"
#include "libov/ov_macros.h"
#include "libov/ov_path.h"
#include "libov/ov_logfile.h"
#include "libov/ov_result.h"
#include "xdrhandling.h"
#include "config.h"


void getPkgElementInspector(OV_INSTPTR_ksapi_getPkg this, char* xdr, OV_UINT* xdrposition, OV_INSTPTR_ksapi_KSCommon pchild)
{
	int errorcode;
	int vartype;
	int i, j;
	int veclength, stringlength, currstringlength;
	int* genvec =0;

	for(i=0;i<4; i++)
		((char*)(&errorcode))[i] = xdr[*xdrposition+3-i];		//read errorcode of current getvar
	//printf("\n\nvartype: %d\n\n", errorcode);
	//xdrposition +7 -i not used right now; purpose unknown
	if(!errorcode)	//0 is OK
	{
		for(i=0;i<4; i++)										//read type of current variable
			((char*)(&vartype))[i] = xdr[*xdrposition+11-i];
		switch(vartype)
		{
		case OV_VT_BOOL:
			if(Ov_CanCastTo(ksapi_getBool, pchild))				//compare type to ksapi get*block type
			{
				for(i=0; i<4; i++)								//read value
					((char*)&(Ov_StaticPtrCast(ksapi_getBool, pchild)->v_receivebool))[i] = xdr[*xdrposition+15-i];

				pchild->v_status = STATUS_KSCOMMON_OK;
				//decode Timestamp Seconds
				for(i=0;i<4; i++)
					((char*)&(Ov_StaticPtrCast(ksapi_getBool, pchild)->v_varTimeStamp.secs))[i] = xdr[*xdrposition+19-i];
				//decode Timestamp USeconds
				for(i=0;i<4; i++)
					((char*)&(Ov_StaticPtrCast(ksapi_getBool, pchild)->v_varTimeStamp.usecs))[i] = xdr[*xdrposition+23-i];
				//decode QState
				for(i=0;i<4; i++)
					((char*)&(Ov_StaticPtrCast(ksapi_getBool, pchild)->v_varQState))[i] = xdr[*xdrposition+27-i];

			}
			else												//wrong vartype
			{
				ksapi_getPkg_state_set(this, "wrong vartype");
				pchild->v_status = -1;
			}

			*xdrposition = *xdrposition+28;						//point to position of next variable in xdr
			break;

		case OV_VT_BOOL_VEC:
			if(Ov_CanCastTo(ksapi_getBoolVec, pchild))				//compare type to ksapi get*block type
			{
				for(i=0; i<4; i++)								//read veclength
					((char*)&veclength)[i] = xdr[*xdrposition+15-i];

				genvec = malloc(veclength*4);
				for (j = 0; j < veclength; j++)
				{
					for (i =0; i<4; i++)
						((char*)(&genvec[j]))[i] = xdr[*xdrposition+19+j*4-i];
				}
				ksapi_getBoolVec_receiveboolvec_set(Ov_StaticPtrCast(ksapi_getBoolVec, pchild), (OV_BOOL*) genvec, veclength);
				free(genvec);
				//decode Timestamp Seconds
				for(i=0;i<4; i++)
					((char*)&(Ov_StaticPtrCast(ksapi_getBoolVec, pchild)->v_varTimeStamp.secs))[i] = xdr[*xdrposition+veclength*4+19-i];
				//decode Timestamp USeconds
				for(i=0;i<4; i++)
					((char*)&(Ov_StaticPtrCast(ksapi_getBoolVec, pchild)->v_varTimeStamp.usecs))[i] = xdr[*xdrposition+veclength*4+23-i];
				//decode QState
				for(i=0;i<4; i++)
					((char*)&(Ov_StaticPtrCast(ksapi_getBoolVec, pchild)->v_varQState))[i] = xdr[*xdrposition+veclength*4+27-i];
			}
			else												//wrong vartype
			{
				ksapi_getPkg_state_set(this, "wrong vartype");
				pchild->v_status = -1;
			}

			*xdrposition = *xdrposition+28+veclength*4;						//point to position of next variable in xdr
			break;

		case OV_VT_INT:
			if(Ov_CanCastTo(ksapi_getInt, pchild))				//compare type to ksapi get*block type
			{
				for(i=0; i<4; i++)								//read value
					((char*)&(Ov_StaticPtrCast(ksapi_getInt, pchild)->v_receiveint))[i] = xdr[*xdrposition+15-i];
				//decode Timestamp Seconds
				for(i=0;i<4; i++)
					((char*)&(Ov_StaticPtrCast(ksapi_getInt, pchild)->v_varTimeStamp.secs))[i] = xdr[*xdrposition+19-i];
				//decode Timestamp USeconds
				for(i=0;i<4; i++)
					((char*)&(Ov_StaticPtrCast(ksapi_getInt, pchild)->v_varTimeStamp.usecs))[i] = xdr[*xdrposition+23-i];
				//decode QState
				for(i=0;i<4; i++)
					((char*)&(Ov_StaticPtrCast(ksapi_getInt, pchild)->v_varQState))[i] = xdr[*xdrposition+27-i];
			}
			else												//wrong vartype
			{
				ksapi_getPkg_state_set(this, "wrong vartype");
				pchild->v_status = -1;
			}

			*xdrposition = *xdrposition+28;						//point to position of next variable in xdr
			break;

		case OV_VT_INT_VEC:
			if(Ov_CanCastTo(ksapi_getIntVec, pchild))				//compare type to ksapi get*block type
			{
				for(i=0; i<4; i++)								//read veclength
					((char*)&veclength)[i] = xdr[*xdrposition+15-i];

				genvec = malloc(veclength*4);
				for (j = 0; j < veclength; j++)
				{
					for (i =0; i<4; i++)
						((char*)(&genvec[j]))[i] = xdr[*xdrposition+19+j*4-i];
				}
				ksapi_getIntVec_receiveintvec_set(Ov_StaticPtrCast(ksapi_getIntVec, pchild), (OV_INT*) genvec, veclength);
				free(genvec);
				//decode Timestamp Seconds
				for(i=0;i<4; i++)
					((char*)&(Ov_StaticPtrCast(ksapi_getIntVec, pchild)->v_varTimeStamp.secs))[i] = xdr[*xdrposition+veclength*4+19-i];
				//decode Timestamp USeconds
				for(i=0;i<4; i++)
					((char*)&(Ov_StaticPtrCast(ksapi_getIntVec, pchild)->v_varTimeStamp.usecs))[i] = xdr[*xdrposition+veclength*4+23-i];
				//decode QState
				for(i=0;i<4; i++)
					((char*)&(Ov_StaticPtrCast(ksapi_getIntVec, pchild)->v_varQState))[i] = xdr[*xdrposition+veclength*4+27-i];
			}
			else												//wrong vartype
			{
				ksapi_getPkg_state_set(this, "wrong vartype");
				pchild->v_status = -1;
			}

			*xdrposition = *xdrposition+28+veclength*4;						//point to position of next variable in xdr
			break;

		case OV_VT_UINT:
			if(Ov_CanCastTo(ksapi_getUInt, pchild))				//compare type to ksapi get*block type
			{
				for(i=0; i<4; i++)								//read value
					((char*)&(Ov_StaticPtrCast(ksapi_getUInt, pchild)->v_receiveuint))[i] = xdr[*xdrposition+15-i];
				//decode Timestamp Seconds
				for(i=0;i<4; i++)
					((char*)&(Ov_StaticPtrCast(ksapi_getUInt, pchild)->v_varTimeStamp.secs))[i] = xdr[*xdrposition+19-i];
				//decode Timestamp USeconds
				for(i=0;i<4; i++)
					((char*)&(Ov_StaticPtrCast(ksapi_getUInt, pchild)->v_varTimeStamp.usecs))[i] = xdr[*xdrposition+23-i];
				//decode QState
				for(i=0;i<4; i++)
					((char*)&(Ov_StaticPtrCast(ksapi_getUInt, pchild)->v_varQState))[i] = xdr[*xdrposition+27-i];
			}
			else												//wrong vartype
			{
				ksapi_getPkg_state_set(this, "wrong vartype");
				pchild->v_status = -1;
			}

			*xdrposition = *xdrposition+28;						//point to position of next variable in xdr
			break;

		case OV_VT_UINT_VEC:
			if(Ov_CanCastTo(ksapi_getUIntVec, pchild))				//compare type to ksapi get*block type
			{
				for(i=0; i<4; i++)								//read veclength
					((char*)&veclength)[i] = xdr[*xdrposition+15-i];

				genvec = malloc(veclength*4);
				for (j = 0; j < veclength; j++)
				{
					for (i =0; i<4; i++)
						((char*)(&genvec[j]))[i] = xdr[*xdrposition+19+j*4-i];
				}
				ksapi_getUIntVec_receiveuintvec_set(Ov_StaticPtrCast(ksapi_getUIntVec, pchild), (OV_UINT*) genvec, veclength);
				free(genvec);
				//decode Timestamp Seconds
				for(i=0;i<4; i++)
					((char*)&(Ov_StaticPtrCast(ksapi_getUIntVec, pchild)->v_varTimeStamp.secs))[i] = xdr[*xdrposition+veclength*4+19-i];
				//decode Timestamp USeconds
				for(i=0;i<4; i++)
					((char*)&(Ov_StaticPtrCast(ksapi_getUIntVec, pchild)->v_varTimeStamp.usecs))[i] = xdr[*xdrposition+veclength*4+23-i];
				//decode QState
				for(i=0;i<4; i++)
					((char*)&(Ov_StaticPtrCast(ksapi_getUIntVec, pchild)->v_varQState))[i] = xdr[*xdrposition+veclength*4+27-i];
			}
			else												//wrong vartype
			{
				ksapi_getPkg_state_set(this, "wrong vartype");
				pchild->v_status = -1;
			}
			*xdrposition = *xdrposition+28+veclength*4;						//point to position of next variable in xdr
			break;

		case OV_VT_DOUBLE:
			if(Ov_CanCastTo(ksapi_getDouble, pchild))				//compare type to ksapi get*block type
			{
				for(i=0; i<8; i++)								//read value
					((char*)&(Ov_StaticPtrCast(ksapi_getDouble, pchild)->v_receivedouble))[i] = xdr[*xdrposition+19-i];
				//decode Timestamp Seconds
				for(i=0;i<4; i++)
					((char*)&(Ov_StaticPtrCast(ksapi_getDouble, pchild)->v_varTimeStamp.secs))[i] = xdr[*xdrposition+23-i];
				//decode Timestamp USeconds
				for(i=0;i<4; i++)
					((char*)&(Ov_StaticPtrCast(ksapi_getDouble, pchild)->v_varTimeStamp.usecs))[i] = xdr[*xdrposition+27-i];
				//decode QState
				for(i=0;i<4; i++)
					((char*)&(Ov_StaticPtrCast(ksapi_getDouble, pchild)->v_varQState))[i] = xdr[*xdrposition+31-i];
			}
			else												//wrong vartype
			{
				ksapi_getPkg_state_set(this, "wrong vartype");
				pchild->v_status = -1;
			}

			*xdrposition = *xdrposition+32;						//point to position of next variable in xdr
			break;

		case OV_VT_DOUBLE_VEC:
			if(Ov_CanCastTo(ksapi_getDoubleVec, pchild))				//compare type to ksapi get*block type
			{
				for(i=0; i<4; i++)								//read veclength
					((char*)&veclength)[i] = xdr[*xdrposition+15-i];

				genvec = malloc(veclength*8);
				for (j = 0; j < veclength; j++)
				{
					for (i =0; i<8; i++)
						((char*)(&genvec[j]))[i] = xdr[*xdrposition+19+j*8-i];
				}
				ksapi_getDoubleVec_receivedoublevec_set(Ov_StaticPtrCast(ksapi_getDoubleVec, pchild), (OV_DOUBLE*) genvec, veclength);
				free(genvec);
				//decode Timestamp Seconds
				for(i=0;i<4; i++)
					((char*)&(Ov_StaticPtrCast(ksapi_getDoubleVec, pchild)->v_varTimeStamp.secs))[i] = xdr[*xdrposition+veclength*8+19-i];
				//decode Timestamp USeconds
				for(i=0;i<4; i++)
					((char*)&(Ov_StaticPtrCast(ksapi_getDoubleVec, pchild)->v_varTimeStamp.usecs))[i] = xdr[*xdrposition+veclength*8+23-i];
				//decode QState
				for(i=0;i<4; i++)
					((char*)&(Ov_StaticPtrCast(ksapi_getDoubleVec, pchild)->v_varQState))[i] = xdr[*xdrposition+veclength*8+27-i];
			}
			else												//wrong vartype
			{
				ksapi_getPkg_state_set(this, "wrong vartype");
				pchild->v_status = -1;
			}
			*xdrposition = *xdrposition+28+veclength*8;						//point to position of next variable in xdr
			break;

		case OV_VT_SINGLE:
			if(Ov_CanCastTo(ksapi_getSingle, pchild))				//compare type to ksapi get*block type
			{
				for(i=0; i<4; i++)								//read value
					((char*)&(Ov_StaticPtrCast(ksapi_getSingle, pchild)->v_receivesingle))[i] = xdr[*xdrposition+15-i];
				//decode Timestamp Seconds
				for(i=0;i<4; i++)
					((char*)&(Ov_StaticPtrCast(ksapi_getSingle, pchild)->v_varTimeStamp.secs))[i] = xdr[*xdrposition+19-i];
				//decode Timestamp USeconds
				for(i=0;i<4; i++)
					((char*)&(Ov_StaticPtrCast(ksapi_getSingle, pchild)->v_varTimeStamp.usecs))[i] = xdr[*xdrposition+23-i];
				//decode QState
				for(i=0;i<4; i++)
					((char*)&(Ov_StaticPtrCast(ksapi_getSingle, pchild)->v_varQState))[i] = xdr[*xdrposition+27-i];
			}
			else												//wrong vartype
			{
				ksapi_getPkg_state_set(this, "wrong vartype");
				pchild->v_status = -1;
			}

			*xdrposition = *xdrposition+28;						//point to position of next variable in xdr
			break;

		case OV_VT_SINGLE_VEC:
			if(Ov_CanCastTo(ksapi_getSingleVec, pchild))				//compare type to ksapi get*block type
			{
				for(i=0; i<4; i++)								//read veclength
					((char*)&veclength)[i] = xdr[*xdrposition+15-i];

				genvec = malloc(veclength*4);
				for (j = 0; j < veclength; j++)
				{
					for (i =0; i<4; i++)
						((char*)(&genvec[j]))[i] = xdr[*xdrposition+19+j*4-i];
				}
				ksapi_getSingleVec_receivesinglevec_set(Ov_StaticPtrCast(ksapi_getSingleVec, pchild), (OV_SINGLE*) genvec, veclength);
				free(genvec);
				//decode Timestamp Seconds
				for(i=0;i<4; i++)
					((char*)&(Ov_StaticPtrCast(ksapi_getSingleVec, pchild)->v_varTimeStamp.secs))[i] = xdr[*xdrposition+veclength*4+19-i];
				//decode Timestamp USeconds
				for(i=0;i<4; i++)
					((char*)&(Ov_StaticPtrCast(ksapi_getSingleVec, pchild)->v_varTimeStamp.usecs))[i] = xdr[*xdrposition+veclength*4+23-i];
				//decode QState
				for(i=0;i<4; i++)
					((char*)&(Ov_StaticPtrCast(ksapi_getSingleVec, pchild)->v_varQState))[i] = xdr[*xdrposition+veclength*4+27-i];
			}
			else												//wrong vartype
			{
				ksapi_getPkg_state_set(this, "wrong vartype");
				pchild->v_status = -1;
			}

			*xdrposition = *xdrposition+28+veclength*4;						//point to position of next variable in xdr
			break;

		case OV_VT_STRING:
			if(Ov_CanCastTo(ksapi_getString, pchild))				//compare type to ksapi get*block type
			{
				for(i=0; i<4; i++)								//read stringlength
					((char*)&stringlength)[i] = xdr[*xdrposition+15-i];

				genvec = malloc(stringlength+1);
				for (i =0; i<stringlength; i++)
					((char*)genvec)[i] = xdr[*xdrposition+16+i];
				((char*)genvec)[stringlength] = '\0';
				ksapi_getString_receivestring_set(Ov_StaticPtrCast(ksapi_getString, pchild), (OV_STRING) genvec);
				free(genvec);
				while(stringlength%4)
					stringlength++;
				//decode Timestamp Seconds
				for(i=0;i<4; i++)
					((char*)&(Ov_StaticPtrCast(ksapi_getString, pchild)->v_varTimeStamp.secs))[i] = xdr[*xdrposition+stringlength+19-i];
				//decode Timestamp USeconds
				for(i=0;i<4; i++)
					((char*)&(Ov_StaticPtrCast(ksapi_getString, pchild)->v_varTimeStamp.usecs))[i] = xdr[*xdrposition+stringlength+23-i];
				//decode QState
				for(i=0;i<4; i++)
					((char*)&(Ov_StaticPtrCast(ksapi_getString, pchild)->v_varQState))[i] = xdr[*xdrposition+stringlength+27-i];
			}
			else												//wrong vartype
			{
				ksapi_getPkg_state_set(this, "wrong vartype");
				pchild->v_status = -1;
			}

			*xdrposition = *xdrposition+28+stringlength;						//point to position of next variable in xdr
			while((*xdrposition)%4)
				(*xdrposition)++;
			break;

		case OV_VT_STRING_VEC:
			if(Ov_CanCastTo(ksapi_getStringVec, pchild))				//compare type to ksapi get*block type
			{
				for(i=0; i<4; i++)								//read veclength
					((char*)&veclength)[i] = xdr[*xdrposition+15-i];
				//printf("veclength: %d\n", veclength);
				stringlength = 0;
				genvec = malloc(veclength*sizeof(char*));
				for (i=0; i<veclength; i++)
				{
					for(j=0; j<4; j++)
						((char*)&currstringlength)[j] = xdr[(*xdrposition)+19+i*4+stringlength-j];
					((char**)genvec)[i] = malloc(currstringlength + 1);
					memcpy((char**)genvec[i], &(xdr[*xdrposition+20+i*4+stringlength]), currstringlength);
					((char**)genvec)[i][currstringlength] = '\0';
					stringlength+=currstringlength;
					while(stringlength%4)
						stringlength++;
					//printf("currstringlength: %d\n%s\n", currstringlength, ((char**)genvec)[i]);

				}

				ksapi_getStringVec_receivestringvec_set(Ov_StaticPtrCast(ksapi_getStringVec, pchild), (OV_STRING*) genvec, veclength);
				for (i=0; i<veclength; i++)
					free((char**)genvec[i]);
				free(genvec);
				//decode Timestamp Seconds
				for(i=0;i<4; i++)
					((char*)&(Ov_StaticPtrCast(ksapi_getStringVec, pchild)->v_varTimeStamp.secs))[i] = xdr[*xdrposition+veclength*4+stringlength+19-i];
				//decode Timestamp USeconds
				for(i=0;i<4; i++)
					((char*)&(Ov_StaticPtrCast(ksapi_getStringVec, pchild)->v_varTimeStamp.usecs))[i] = xdr[*xdrposition+veclength*4+stringlength+23-i];
				//decode QState
				for(i=0;i<4; i++)
					((char*)&(Ov_StaticPtrCast(ksapi_getStringVec, pchild)->v_varQState))[i] = xdr[*xdrposition+veclength*4+stringlength+27-i];
			}
			else												//wrong vartype
			{
				ksapi_getPkg_state_set(this, "wrong vartype");
				pchild->v_status = -1;
			}

			*xdrposition = *xdrposition+28+stringlength+4*veclength;						//point to position of next variable in xdr

			break;

		}



	}
	else
	{
		ksapi_getPkg_state_set(this, ov_result_getresulttext(errorcode));
		pchild->v_status = -1;
		*xdrposition = *xdrposition+4;		//ks error has only one field in xdr

	}

	return;
}


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

	OV_UINT childcount = 0;
	char *xdr=NULL;
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
	generateheader(KS_GETVAR, &xdr, &xdrlength);

	//make body
	Ov_ForEachChild(ov_containment, (OV_INSTPTR_ov_domain)pobj, pchild)
	{

		if(Ov_CanCastTo(ksapi_getBool, pchild)
				|| Ov_CanCastTo(ksapi_getBoolVec, pchild)
				|| Ov_CanCastTo(ksapi_getInt, pchild)
				|| Ov_CanCastTo(ksapi_getIntVec, pchild)
				|| Ov_CanCastTo(ksapi_getUInt, pchild)
				|| Ov_CanCastTo(ksapi_getUIntVec, pchild)
				|| Ov_CanCastTo(ksapi_getString, pchild)
				|| Ov_CanCastTo(ksapi_getStringVec, pchild)
				|| Ov_CanCastTo(ksapi_getSingle, pchild)
				|| Ov_CanCastTo(ksapi_getSingleVec, pchild)
				|| Ov_CanCastTo(ksapi_getDouble, pchild)
				|| Ov_CanCastTo(ksapi_getDoubleVec, pchild))
		{
			childcount++;
			pchildksc = Ov_StaticPtrCast(ksapi_KSCommon, pchild);
			pchildksc->v_status = STATUS_KSCOMMON_OK; //reset status, will provide detailed channel status afterwards
			if(pchildksc->v_path && *pchildksc->v_path)
				generategetbody(&xdr, &xdrlength, pchildksc->v_path);
			else
				errorcode = -1;
		}
		else if(Ov_CanCastTo(ksapi_Channel, pchild))	//ignore channel
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
	OV_UINT xdrposition;


	ksapi_getPkg_state_set(this, "sending completed");	//will be overwritten on error on one get*


	//printxdr(xdr, xdrlength);


	xdrposition = 36;			//begin of first variable (error code)

	Ov_ForEachChild(ov_containment, (OV_INSTPTR_ov_domain)kscommon, pchild)
	{

		if(Ov_CanCastTo(ksapi_KSCommon, pchild))
		{
			getPkgElementInspector(this, xdr, &xdrposition, Ov_StaticPtrCast(ksapi_KSCommon, pchild));
		}

		else if(Ov_CanCastTo(ksapi_Channel, pchild))
		{
		}

	}



	return;
}
