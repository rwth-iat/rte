/*
*   Copyright (C) 2004
*   Lehrstuhl fuer Prozessleittechnik,
*   D-52056 Aachen, Germany.
*   All rights reserved.
*
*
*   This file is part of the ACPLT/OV Package 
*   Licensed under the Apache License, Version 2.0 (the "License");
*   you may not use this file except in compliance with the License.
*   You may obtain a copy of the License at
*
*       http://www.apache.org/licenses/LICENSE-2.0
*
*   Unless required by applicable law or agreed to in writing, software
*   distributed under the License is distributed on an "AS IS" BASIS,
*   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*   See the License for the specific language governing permissions and
*   limitations under the License.
*/
/***********************************************************************
*
*	File
*	-----
*	send_command.c
*
*	Autoren:
*	--------
*	MF							Murat Fedai <Murat@plt.rwth-aachen.de>
*	MH							Michal Heeg <MichaelH@plt.rwth.aachen.de>
*	St							Stefan Schmitz <StefanS@plt.rwth-aachen.de>
*
*	CVS:
*	----
*	$Revision: 1.2 $
*	$Date: 2005-08-30 14:06:27 $
*
*	Historie:
*	--------
*	10-August-1999			MF
*		-	Erstellung, Lehrstuhl fuer Prozessleittechnik, RWTH Aachen
*
*	10-September-1999		MF
*		-	Erstellung von Klientenstrukturen
*
*	16-August-2002			MH		manueller Versionsstand: 0.20
*		-	Einchecken der cmdlib in CVS
*
*	04-März-2004 			St
*		-	Überprüfen des targets, ob dieses bereits
*			/'FB_INSTANZ_CONTAINER/' enthält und entsprechende Reaktion
*
*	22-März-2004			St
*		-	Versenden von Kommandos auch Serverübergreifend (Remote)
*
*	30-August-2005			St
*		-	CVS-CheckIn
*
***********************************************************************/

#ifndef OV_COMPILE_LIBRARY_cmdlib
#define OV_COMPILE_LIBRARY_cmdlib
#endif

/***********************************************************************
	Includes
***********************************************************************/

#include "cmdlib.h"
#include "cmd.h"
#include "libov/ov_logfile.h"
#include "libov/ov_macros.h"

/***********************************************************************
	send_command
***********************************************************************/

OV_RESULT OV_DLLFNCEXPORT send_command(
	OV_INSTPTR_fb_functionblock	pCommander,
	OV_INSTPTR_cmdlib_CmdSender	pSender,
	const char  *target, 	           		/* >|  Zielinstanz	        */
	const char  *command,						/* >|  Kommandoname          */
	const char  *value							/* >|  Kommandowert          */
)	{
	OV_RESULT								fr;
	OV_INSTPTR_ov_object					pobjt;
	OV_PATH									path;
	OV_INT									strlaenge;
	OV_STRING								pcomand;
	OV_STRING								localinst;
	OV_STRING								Mask;
	OV_INSTPTR_cmdlib_processcontrol	pfbt;
	
	/*----------------------------------------------------------------------*/
	/*               Pruefen der Uebergebenen Parameter                     */
	/*----------------------------------------------------------------------*/
	
	if(!pCommander || !target || !value)
		return OV_ERR_GENERIC;
		
	/*----------------------------------------------------------------------*/
	/*        Prüfen ob Kommando local oder Remote versendet wird           */
	/*----------------------------------------------------------------------*/

	if(strncmp(target, "//", 2) == 0) 
	{	/* Kommandoversand soll über einen Com-Manager (Remote) erfolgen */
		
		ov_logfile_error("remote sending not implemented ritght now");
	
	} else 
	{	/* Kommandoversand innerhalb des lokalen Systems  */

		/*----------------------------------------------------------------------*/
		/*                 Zeiger auf Zielinstanz suchen                        */
		/*----------------------------------------------------------------------*/
		ov_memstack_lock();
		
		Mask = (OV_STRING) Ov_HeapMalloc(strlen(FB_INSTANZ_CONTAINER)+2+2);
		if(!Mask)
			return OV_ERR_GENERIC;
		
		/* Testen, ob das target bereits '/FB_INSTANZ_CONTAINER/' enthält */
		strcpy(Mask,"/");
		strcat(Mask,FB_INSTANZ_CONTAINER);
		strcat(Mask,"/");
		strcat(Mask,"*");
		Mask[strlen(Mask)]='\0';
		if (ov_string_match((char *) target, Mask) == TRUE)
		{	
			/* target enthält '/FB_INSTANZ_CONTAINER/' */
			localinst = (OV_STRING) Ov_HeapMalloc (strlen(target)+1);
			if(!localinst)
			{
				Ov_HeapFree(Mask);
				ov_memstack_unlock();
				return OV_ERR_GENERIC;
			};		
			strcpy(localinst, (char *) target);
			localinst[strlen(target)]='\0';
		} else {
			/* target enthält nicht '/FB_INSTANZ_CONTAINER/' */
			localinst = (OV_STRING) Ov_HeapMalloc (strlen(FB_INSTANZ_CONTAINER)+2+strlen(target)+1);
			if(!localinst)
			{
				Ov_HeapFree(Mask);
				ov_memstack_unlock();
				return OV_ERR_GENERIC;
			};
			strcpy(localinst,"/");
			strcat(localinst,FB_INSTANZ_CONTAINER);
			strcat(localinst,"/");
			strcat(localinst, (char *) target);
			localinst[strlen(FB_INSTANZ_CONTAINER)+2+strlen(target)]='\0';
		};
		Ov_HeapFree(Mask);
	
		/* Zeiger auf Instanz suchen */
		fr = ov_path_resolve(&path,NULL,localinst, 2);
		if(Ov_Fail(fr)) 
		{	/* Pfad nicht gefunden */
			Ov_HeapFree(localinst);
			ov_memstack_unlock();
			return OV_ERR_GENERIC;
		};
		if(path.elements->elemtype != OV_ET_OBJECT)
		{	/* Element ist kein Object */
			Ov_HeapFree(localinst);
			ov_memstack_unlock();
			return OV_ERR_GENERIC;
		};
		pobjt = path.elements[path.size-1].pobj;
		Ov_HeapFree(localinst);
	
		/*----------------------------------------------------------------------*/
		/*               Zienstanz gefunden, Auftrag senden                     */
		/*----------------------------------------------------------------------*/	
		
		strlaenge = strlen(ov_path_getcanonicalpath(Ov_PtrUpCast(ov_object,pCommander),2))+strlen(command)+strlen(value)+3;                          /* 3 =';'+';'+'\0' */
		
		pcomand = (OV_STRING) Ov_HeapMalloc(strlaenge);      /* Speicherplatz anlegen */
		if(!pcomand)
			return OV_ERR_GENERIC;
			
		strcpy(pcomand,ov_path_getcanonicalpath(Ov_PtrUpCast(ov_object,pCommander), 2));
		strcat(pcomand,";");
		strcat(pcomand,command);
		strcat(pcomand,";");
		strcat(pcomand,value);
		
		pfbt = Ov_StaticPtrCast(cmdlib_processcontrol, pobjt);
		if(pfbt)
			cmdlib_processcontrol_order_set(pfbt,pcomand);
		Ov_HeapFree(pcomand);
		ov_memstack_unlock();
	};
	
	return OV_ERR_OK;
};
