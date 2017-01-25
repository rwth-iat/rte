/*
*	Copyright (C) 2004
*	Chair of Process Control Engineering,
*	Aachen University of Technology.
*	All rights reserved.
*
*	Redistribution and use in source and binary forms, with or without
*	modification, are permitted provided that the following conditions
*	are met:
*	1. Redistributions of source code must retain the above copyright
*	   notice, this list of conditions and the following disclaimer.
*	2. Redistributions in binary form must print or display the above
*	   copyright notice either during startup or must have a means for
*	   the user to view the copyright notice.
*	3. Redistributions in binary form must reproduce the above copyright
*	   notice, this list of conditions and the following disclaimer in
*		the documentation and/or other materials provided with the
*		distribution.
*	4. Neither the name of the Chair of Process Control Engineering nor
*		the name of the Aachen University of Technology may be used to
*		endorse or promote products derived from this software without
*		specific prior written permission.
*
*	THIS SOFTWARE IS PROVIDED BY THE CHAIR OF PROCESS CONTROL ENGINEERING
*	``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
*	LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
*	A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE CHAIR OF
*	PROCESS CONTROL ENGINEERING BE LIABLE FOR ANY DIRECT, INDIRECT,
*	INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
*	BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
*	OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
*	AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
*	LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY
*	WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
*	POSSIBILITY OF SUCH DAMAGE.
*/
/***********************************************************************
*
*	File
*	-----
*	processcontrol.c
*
*	Autoren:
*	--------
*	MF							Murat Fedai <Murat@plt.rwth-aachen.de>
*	MH							Michal Heeg <MichaelH@plt.rwth.aachen.de>
*	St							Stefan Schmitz <StefanS@plt.rwth-aachen.de>
*
*	CVS:
*	----
*	$Revision: 1.3 $
*	$Date: 2011-01-18 15:22:13 $
*
*	Historie:
*	--------
*	10-August-1999			MF
*		- 	Erstellung, Lehrstuhl fuer Prozessleittechnik, RWTH Aachen
*
*	18-April-2000			MF
*		-	Umbenennung des Makros COM zu CMD
*		-	Der Operator kann mit SETCMDR einen externen KG eintragen,
*		-	Kommandos duerfen vom KG ueberschrieben werden
*
* 	16-August-2002 		MH		manueller Versionsstand: 0.20
*		-	Einchecken der cmdlib in CVS
*
*	16-Februar-2004		St
*		-	Einführung der Methode setOrder (vgl. auch cmdlib.ovm)
*
*	26-April-2004			St
*		-	Methode setOrder so angepasst, dass nur das genaue Kommando
*			akzeptiert wird. Bis dato wurden ein Teilvergleich gemacht,
*			was zur Folge hat, dass 'abc' angenommen wurde, wenn der
*			typspezifische Befehl 'abcdef' war
*
*	03-Mai-2004				St
*		-	Methode setOrder ebenso wie in V0.93 angepasst, allerdings für
*			die generischen Kommandos
*		-	generische und die typspezifischen Kommandos werden jetzt
*			gleichzeitig verarbeitet
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
#include "libov/ov_macros.h"

/***********************************************************************
	setAccessor der order-Variable
***********************************************************************/

OV_RESULT OV_DLLFNCEXPORT cmdlib_processcontrol_order_set(
	OV_INSTPTR_cmdlib_processcontrol	pinst,
	const OV_STRING						comstr
) {
	//	Local Method Tables
	//
	OV_VTBLPTR_cmdlib_processcontrol	ppcVTable;
	
	//	get Method Table
	//
	Ov_GetVTablePtr(cmdlib_processcontrol, ppcVTable, pinst);
	
	return ppcVTable->m_setOrder(pinst, comstr);
};

/***********************************************************************
	Setzen und Kontrollieren der order-Variable
***********************************************************************/

OV_RESULT OV_DLLFNCEXPORT cmdlib_processcontrol_setOrder(
	OV_INSTPTR_cmdlib_processcontrol	ppc,
	const OV_STRING						comstr
)	{
	//	Lokale Variablen
	//
	OV_STRING          sender;         /* Kommandosender  */
	OV_STRING          order;          /* Kommandoname    */
	OV_STRING          value;          /* Kommandowert    */
	OV_STRING          str, Cmd;       /* Hilfsvariable   */
	OV_STRING          pt1,pt2;        /* Hilfszeiger     */
	OV_INT             i;              /* Hilfsvariable   */
	
	//	Lokale Methodentabellen
	//
	OV_VTBLPTR_cmdlib_processcontrol       pctable;
	
	/*-----------------------------------------------------------------*/
	/* Kein Kommandostring vorhanden                                   */
	/*	(für Rückdokumentation erforderlich)                            */
	/*-----------------------------------------------------------------*/

	if (ov_string_compare(comstr, NULL) == 0)
		return OV_ERR_OK;

	/*-----------------------------------------------------------------*/
	/* Anzahl der Uebergebenen Kommando-Parameter prüfen               */
	/*-----------------------------------------------------------------*/

	if (!ppc || !comstr)
		return OV_ERR_BADPARAM;
	
	for (i = 0, pt1 = comstr; (*pt1 != '\0') ; pt1++)
		if (*pt1 == ';') i++;		               /* Trennzeichen : ';' */
		
	if (i != 2)          /* Nicht genügend Kommandoparameter vorhanden */
		return OV_ERR_BADPARAM;

	/*-----------------------------------------------------------------*/
	/* Auftragspruefungen fuer den Kommandoeingang (order)             */
	/*-----------------------------------------------------------------*/

	/* Speicherplätze fuer Variablen reservieren */

 	str = (OV_STRING) Ov_HeapMalloc(strlen(comstr) + 1);
	if (!str)
   	return OV_ERR_GENERIC;

	sender = (OV_STRING) Ov_HeapMalloc(strlen(comstr) + 1);
	if (!sender) 
	{
		Ov_HeapFree(str);
		return OV_ERR_GENERIC;
	};

	order  = (OV_STRING) Ov_HeapMalloc(strlen(comstr) + 1);
	if (!order)
	{
		Ov_HeapFree(str);
		Ov_HeapFree(sender);
		return OV_ERR_GENERIC;
	};

	value  = (OV_STRING) Ov_HeapMalloc(strlen(comstr) + 1);
	if (!value)
	{
		Ov_HeapFree(str);
		Ov_HeapFree(sender);
		Ov_HeapFree(order);
		return OV_ERR_GENERIC;
	};

	/*-----------------------------------------------------------------*/
	/* Extrahieren der Kommandoparameter                               */
	/*-----------------------------------------------------------------*/

	/* Zeiger auf erstes Trennzeichen ';' */
	pt1 = strchr(comstr, ';');
	/* Sendernamen zwischenspeichern */ 
	strncpy(sender, comstr, strlen(comstr) - strlen(pt1)); 
	/* Terminierende Null einfügen */
	sender[strlen(comstr) - strlen(pt1)] = '\0';

	pt1++;
	/* Zeiger auf erstes Trennzeichen ';' */
	pt2 = strchr(pt1, ';');
	/* Kommando zwischenspeichern */
	strncpy(order, pt1, strlen(pt1) - strlen(pt2));
	/* Terminierende Null einfügen */
	order[strlen(pt1) - strlen(pt2)] = '\0';
	
	pt2++;
	/* Kommando-Wert zwischenspeichern */
	strcpy(value, pt2);
	Ov_HeapFree(str);


	if (ov_string_compare(sender, ppc->v_commander) == 0) 
	{
		/*--------------------------------------------------------------*/
		/* Einheit ist durch den Kommandogeber belegt                   */
		/*--------------------------------------------------------------*/
	
		/*--------------------------------------------------------------*/
		/* Bearbeitung von Kommandos												 */
		/*		- fest-definierte (generisch)                             */
		/*		- type-spezifisch                                         */
		/*--------------------------------------------------------------*/
	
	   Ov_GetVTablePtr(cmdlib_processcontrol, pctable, ppc);
	
		str = NULL;		/* initialisieren */
		if (Ov_OK(pctable->m_typecommand(ppc, &str)))
		{
			if (str)
			{
				str = ov_realloc(str, strlen(str) + strlen(cmdlib_processcontrol_generic_commands()) + 1 + 1);
				strcat(str, ",");
			} else
				str = (OV_STRING) Ov_HeapMalloc(strlen(cmdlib_processcontrol_generic_commands()) + 1);
					
			if (str)
			{				
				//	generische Kommandos hinzufügen
				//
				strcat(str, cmdlib_processcontrol_generic_commands());
				
				Cmd = (OV_STRING) Ov_HeapMalloc(strlen(str) + 1);
				if (!Cmd)
				{
				  Ov_HeapFree(sender);
				  Ov_HeapFree(order);
				  Ov_HeapFree(value);
				  Ov_HeapFree(str);
				  return OV_ERR_GENERIC;
				};
			
				for (i = 0, pt1 = str; (*pt1 != '\0') ; pt1++)
					if (*pt1 == ',') i++;		         /* Trennzeichen : ',' */
				
				/* Zeiger auf erstes Trennzeichen ',' */
				pt1 = strchr(str, ',');
				/* Sendernamen zwischenspeichern */ 
				strncpy(Cmd, str, strlen(str) - strlen(pt1)); 
				/* Terminierende Null einfügen */
				Cmd[strlen(str) - strlen(pt1)] = '\0';
				/* Zähler initialisieren */
				pt1++;
				i--;
				
				for (; i > 0; i--)
				{					
					/* Kommando überprüfen */
					if (strcmp(order, Cmd) == 0)
					{									/* Kommando ist ein typ. Kommando */
						ov_string_setvalue(&ppc->v_sender, sender);
						ov_string_setvalue(&ppc->v_order, order);
						ov_string_setvalue(&ppc->v_value, value);
						Ov_HeapFree(str);
						Ov_HeapFree(Cmd);
						Ov_HeapFree(sender);
						Ov_HeapFree(order);
						Ov_HeapFree(value);
						return OV_ERR_OK;
					};
					
					/* Zeiger auf nächstes Trennzeichen ',' */
					pt2 = strchr(pt1, ',');
					/* Kommando zwischenspeichern */
					strncpy(Cmd, pt1, strlen(pt1) - strlen(pt2));
					/* Terminierende Null einfügen */
					Cmd[strlen(pt1) - strlen(pt2)] = '\0';
					pt1 = pt2 + 1;
				};
				
				/* Letzten beiden Kommandos überprüfen */
				if (	strcmp(order, Cmd) == 0
					||	strcmp(order, pt1) == 0)
				{									/* Kommando ist ein typ. Kommando */
					ov_string_setvalue(&ppc->v_sender, sender);
					ov_string_setvalue(&ppc->v_order, order);
					ov_string_setvalue(&ppc->v_value, value);
					Ov_HeapFree(str);
					Ov_HeapFree(Cmd);
					Ov_HeapFree(sender);
					Ov_HeapFree(order);
					Ov_HeapFree(value);
					return OV_ERR_OK;
				};
				
				Ov_HeapFree(Cmd);
			};
		};
		Ov_HeapFree(str);
	
		/*--------------------------------------------------------------*/
		/* Bearbeitung von Belegungs Kommandos                          */
		/*--------------------------------------------------------------*/
			
		if (ov_string_compare(order, FB_FREE) == 0) 
		{
			if(ov_string_compare(ppc->v_OldCommander, NULL) != 0)		//if there is an OldCommander reestablish and delete it, otherwise free unit
			{
				if(ov_string_compare(ppc->v_OldCommander, ppc->v_commander) == 0)	//if FREE-ing Commander is OldCommander, OldCommander is deleted
					ov_string_setvalue(&ppc->v_OldCommander, NULL);
					
				ov_string_setvalue(&ppc->v_commander, ppc->v_OldCommander);
				ov_string_setvalue(&ppc->v_OldCommander, NULL);
			}
			else
				ov_string_setvalue(&ppc->v_commander, NULL);
				
			ov_string_setvalue(&ppc->v_sender, NULL);
			ov_string_setvalue(&ppc->v_order, NULL);
			ov_string_setvalue(&ppc->v_value, NULL);
			Ov_HeapFree(sender);
			Ov_HeapFree(order);
			Ov_HeapFree(value);
			return OV_ERR_OK;
		} else {                                /* Kein Auftrag erkannt */
			Ov_HeapFree(sender);
			Ov_HeapFree(order);
			Ov_HeapFree(value);
			return OV_ERR_BADPARAM;
		};										 /* Sender iat als KG eingetragen */
	
	} else if (ov_string_compare(sender, FB_OPERATOR) == 0)  {
		
		/*--------------------------------------------------------------*/
		/* Der Auftraggeber ist ein Operator (KG:OP)                    */
		/*--------------------------------------------------------------*/
		
		/*--------------------------------------------------------------*/
		/* Bearbeitung von Belegungs Kommandos                          */
		/*--------------------------------------------------------------*/

	   if (ov_string_compare(order, FB_OCCUPY) == 0) 
	   {
			if(ov_string_compare(ppc->v_commander, FB_OPERATOR) != 0
				&& ov_string_compare(ppc->v_OldCommander, NULL) == 0)	//if Unit is not already Occupied by FB_OPERATOR and there is no OldCommander save last commander to OldCommander
				ov_string_setvalue(&ppc->v_OldCommander, ppc->v_commander);
				
			ov_string_setvalue(&ppc->v_commander, FB_OPERATOR);
			ov_string_setvalue(&ppc->v_sender, NULL);
			ov_string_setvalue(&ppc->v_order, NULL);
			ov_string_setvalue(&ppc->v_value, NULL);
			Ov_HeapFree(sender);
			Ov_HeapFree(order);
			Ov_HeapFree(value);
			return OV_ERR_OK;
	   
	   } else if (ov_string_compare(order, "SETCMDR") == 0) {
	   
			/* externe Kommandovergabe durch den Operator */   
			if(ov_string_compare(ppc->v_commander, FB_OPERATOR) != 0
				&& ov_string_compare(ppc->v_OldCommander, NULL) == 0)							//if Unit is not already Occupied by FB_OPERATOR and there is no OldCommander save last commander to OldCommander
				ov_string_setvalue(&ppc->v_OldCommander, ppc->v_commander);
			
			ov_string_setvalue(&ppc->v_commander, value);
			ov_string_setvalue(&ppc->v_sender, NULL);
			ov_string_setvalue(&ppc->v_order, NULL);
			ov_string_setvalue(&ppc->v_value, NULL);
			Ov_HeapFree(sender);
			Ov_HeapFree(order);
			Ov_HeapFree(value);
			return OV_ERR_OK;
	   
		} else {
		
			Ov_HeapFree(sender); Ov_HeapFree(order); Ov_HeapFree(value);
			return OV_ERR_BADPARAM;
		};
	
	} else 	if (	(ov_string_compare(ppc->v_commander, "") == 0) 
					&& (ov_string_compare(ppc->v_order, "") == 0))
	{	
		/*--------------------------------------------------------------*/
		/* Einheit ist noch nicht belegt                                */
		/*--------------------------------------------------------------*/
	
		/*--------------------------------------------------------------*/
		/* Bearbeitung von Belegungs Kommandos                          */
		/*--------------------------------------------------------------*/
	
		if (ov_string_compare(order, FB_OCCUPY) == 0)
		{
			ov_string_setvalue(&ppc->v_commander, sender);
			ov_string_setvalue(&ppc->v_sender, NULL);
			ov_string_setvalue(&ppc->v_order, NULL);
			ov_string_setvalue(&ppc->v_value, NULL);
			Ov_HeapFree(sender);
			Ov_HeapFree(order);
			Ov_HeapFree(value);
			return OV_ERR_OK;
		
		} else {                                /* Kein Auftrag erkannt */
		
	     Ov_HeapFree(sender);
	     Ov_HeapFree(order);
	     Ov_HeapFree(value);
	     return OV_ERR_BADPARAM;
	   };
	
	} else {     /* Einheit ist nicht belegt, Unbekannter Auftraggeber */

		/*--------------------------------------------------------------*/
		/* Sender ist als Auftraggeber nicht zugelassen                 */
		/*--------------------------------------------------------------*/
		
		Ov_HeapFree(sender);
		Ov_HeapFree(order);
		Ov_HeapFree(value);
		return OV_ERR_BADPARAM;
	};
};

/***********************************************************************
	universelle / generische Kommandos
***********************************************************************/

OV_STRING OV_DLLFNCEXPORT cmdlib_processcontrol_generic_commands()
{
  return "STOP,RES";				//TIOP and TOOP are not implemented anymore
};
