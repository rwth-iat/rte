/**********************************************************************************
***                                                                             ***
***             F u n k t i o n s b a u s t e i n s y s t e m                   ***
***   ###################################################################       ***
***                                                                             ***
***   Implementierung des Funktionsbausteinsystems mit Kommandostrukturen       ***
***   des Lehrstuhls fuer Prozessleittechnik, RWTH Aachen                       ***
***                                                                             ***
**********************************************************************************/
/**********************************************************************************
*                                                                                 
* Datei                                                                           
* -----                                                                           
* processcontrol.c Implementierung von Kommandostrukturen                         
*
* CVS Revision:  $Revision: 1.1.1.1 $                                                       
* CVS Date: $Date: 2002-08-16 13:55:23 $
*                                                                                 
*                                                                                 
* Historie                                                                        
* --------                                                                        
* 10-Aug-1999 Murat Fedai: Erstellung,                                            
*             Lehrstuhl fuer Prozessleittechnik, RWTH Aachen                      
* 18-Apr-2000 Umbenennung des Makros COM zu CMD,                                  
*             Der Operator kann mit SETCMDR einen externen KG eintragen,          
*             Kommandos duerfen vom KG ueberschrieben werden                      
* 16.08.2002 Michael Heeg: Einchecken der cmdlib in CVS 
*                          (manueller Versionsstand: 0.20)
*                                                                                 
**********************************************************************************/

#include "cmdlib.h"
#include "libov/ov_macros.h"
#include "libov/ov_string.h"
#include "cmd_lib.h"
#include <string.h>


OV_STRING OV_DLLFNCEXPORT cmdlib_processcontrol_univ_comands(void);


/*********************************************************************************/
OV_RESULT OV_DLLFNCEXPORT cmdlib_processcontrol_order_set(
	OV_INSTPTR_cmdlib_processcontrol	pinst,
	const OV_STRING				comstr
) {
/*********************************************************************************/

OV_STRING          sender;         /* Kommandosender  */
OV_STRING          order;          /* Kommandoname    */
OV_STRING          value;          /* Kommandowert    */
OV_STRING          str;            /* Hilfsvariable   */
OV_STRING          pt1,pt2;        /* Hilfszeiger     */
OV_INT             i;              /* Hilfsvariable   */

OV_VTBLPTR_cmdlib_processcontrol       pctable;
OV_INSTPTR_cmdlib_processcontrol       pfb = Ov_StaticPtrCast(cmdlib_processcontrol,pinst);


 /*----------------------------------------------------------------------*/
 /* Kein Kommandostring vorhanden  (für Rückdoumentation erforderlich)   */
 /*----------------------------------------------------------------------*/

 if(ov_string_compare(comstr,NULL)==0) {
   return OV_ERR_OK;
 }

 /*----------------------------------------------------------------------*/
 /* Anzahl der Uebergebenen Kommando-Parameter prüfen                    */
 /*----------------------------------------------------------------------*/

 if(!pinst || !comstr) {
   return OV_ERR_BADPARAM;
 }

 for(i=0,pt1 = comstr;(*pt1 != '\0'); pt1++) {
   if (*pt1 == ';') i++;		             /* Trennzeichen : ';' */
 }
 if(i != 2) {             /* Nicht genügend Kommandoparameter vorhanden */
   return OV_ERR_BADPARAM;
 }

/*----------------------------------------------------------------------*/
/* Auftragspruefungen fuer den Kommandoeingang (order)                  */
/*----------------------------------------------------------------------*/

/* Speicherplätze fuer Variablen reservieren */

 str    = (OV_STRING) Ov_HeapMalloc(strlen(comstr)+1);
 if(!str ) {
   return OV_ERR_GENERIC;
 }

 sender = (OV_STRING) Ov_HeapMalloc(strlen(comstr)+1);
 if(!sender ) {
   Ov_HeapFree(str);
   return OV_ERR_GENERIC;
 }

 order  = (OV_STRING) Ov_HeapMalloc(strlen(comstr)+1);
 if(!order) {
   Ov_HeapFree(str);
   Ov_HeapFree(sender);
   return OV_ERR_GENERIC;
 }

 value  = (OV_STRING) Ov_HeapMalloc(strlen(comstr)+1);
  if(!value) {
   Ov_HeapFree(str);
   Ov_HeapFree(sender);
   Ov_HeapFree(order);
   return OV_ERR_GENERIC;
 }

 /*----------------------------------------------------------------------*/
 /* Extrahieren der Kommandoparameter                                    */
 /*----------------------------------------------------------------------*/

 pt1 = strchr(comstr,';');                           /* Zeiger auf erstes Trennzeichen ';' */
 strncpy(sender,comstr,strlen(comstr)-strlen(pt1));  /* Sendernamen zwischenspeichern */
 sender[strlen(comstr)-strlen(pt1)] = '\0';          /* Terminierende Null einfügen */

 pt1++;
 pt2 = strchr(pt1,';');                              /* Zeiger auf erstes Trennzeichen ';' */
 strncpy(order,pt1,strlen(pt1)-strlen(pt2));         /* Kommando zwischenspeichern */
 order[strlen(pt1)-strlen(pt2)] = '\0';              /* Terminierende Null einfügen */

 pt2++;
 strcpy(value,pt2);                                  /* Kommando-Wert zwischenspeichern */
 Ov_HeapFree(str);


   if(ov_string_compare(sender,pfb->v_commander)==0) {
   /*----------------------------------------------------------------------------------*/
   /* Einheit ist durch den Kommandogeber belegt  UND Kommandoeingang 'order' ist frei */
   /*----------------------------------------------------------------------------------*/

   /*----------------------------------------------------------------------*/
   /* Bearbeitung von fest-definierten (universellen) Kommandos            */
   /*----------------------------------------------------------------------*/

   str = (OV_STRING) Ov_HeapMalloc(strlen(cmdlib_processcontrol_univ_comands())+1);
   if(!str) {
     Ov_HeapFree(sender);
     Ov_HeapFree(order);
     Ov_HeapFree(value);
     return OV_ERR_GENERIC;
   }
   strcpy(str,cmdlib_processcontrol_univ_comands());

   if(strstr(str,order)) {                   /* Kommando ist ein univ. Kommando */
     ov_string_setvalue(&pfb->v_sender,sender);
     ov_string_setvalue(&pfb->v_order,order);
     ov_string_setvalue(&pfb->v_value,value);
     Ov_HeapFree(str);
     Ov_HeapFree(sender);
     Ov_HeapFree(order);
     Ov_HeapFree(value);
     return OV_ERR_OK;
   }
   Ov_HeapFree(str);

   /*----------------------------------------------------------------------*/
   /* Bearbeitung von Benutzer-definierten (Typabhängigen) Kommandos       */
   /*----------------------------------------------------------------------*/

   Ov_GetVTablePtr(cmdlib_processcontrol, pctable, pfb);
   str = (OV_STRING) Ov_HeapMalloc(strlen(pctable->m_typecommand())+1);
   strcpy(str,pctable->m_typecommand());

   if(strstr(str,order)) {                    /* Kommando ist ein typ. Kommando */
     ov_string_setvalue(&pfb->v_sender,sender);
     ov_string_setvalue(&pfb->v_order,order);
     ov_string_setvalue(&pfb->v_value,value);
     Ov_HeapFree(str);
     Ov_HeapFree(sender);
     Ov_HeapFree(order);
     Ov_HeapFree(value);
     return OV_ERR_OK;
   }
   Ov_HeapFree(str);

   /*----------------------------------------------------------------------*/
   /* Bearbeitung von Belegungs Kommandos                                  */
   /*----------------------------------------------------------------------*/

   if(ov_string_compare(order,FB_FREE)==0) {
     ov_string_setvalue(&pfb->v_commander,NULL);
     ov_string_setvalue(&pfb->v_sender,NULL);
     ov_string_setvalue(&pfb->v_order,NULL);
     ov_string_setvalue(&pfb->v_value,NULL);
     Ov_HeapFree(sender);
     Ov_HeapFree(order);
     Ov_HeapFree(value);
     return OV_ERR_OK;
   }
   else {                      /* Kein Auftrag erkannt */
     Ov_HeapFree(sender);
     Ov_HeapFree(order);
     Ov_HeapFree(value);
     return OV_ERR_BADPARAM;
   }
 } /* Sender iat als KG eingetragen */

 else if(ov_string_compare(sender,FB_OPERATOR)==0)  {
   /*----------------------------------------------------------------------*/
   /* Der Auftraggeber ist ein Operator (KG:OP)                            */
   /*----------------------------------------------------------------------*/

   /*----------------------------------------------------------------------*/
   /* Bearbeitung von Belegungs Kommandos                                  */
   /*----------------------------------------------------------------------*/

   if(ov_string_compare(order,FB_OCCUPY)==0) {
     ov_string_setvalue(&pfb->v_commander,FB_OPERATOR);
     ov_string_setvalue(&pfb->v_sender,NULL);
     ov_string_setvalue(&pfb->v_order,NULL);
     ov_string_setvalue(&pfb->v_value,NULL);
     Ov_HeapFree(sender);
     Ov_HeapFree(order);
     Ov_HeapFree(value);
     return OV_ERR_OK;
   }
   else if(ov_string_compare(order,"SETCMDR")==0) { /* externe Kommandovergabe durch den Operator */
     ov_string_setvalue(&pfb->v_commander,value);
     ov_string_setvalue(&pfb->v_sender,NULL);
     ov_string_setvalue(&pfb->v_order,NULL);
     ov_string_setvalue(&pfb->v_value,NULL);
     Ov_HeapFree(sender);
     Ov_HeapFree(order);
     Ov_HeapFree(value);
     return OV_ERR_OK;
   }
   else {
     Ov_HeapFree(sender); Ov_HeapFree(order); Ov_HeapFree(value);
     return OV_ERR_BADPARAM;
   }
 }
 else if((ov_string_compare(pfb->v_commander,"")==0) && (ov_string_compare(pfb->v_order,"")==0)) {
   /*----------------------------------------------------------------------*/
   /* Einheit ist noch nicht belegt UND Kommandoeingang 'order' ist frei   */
   /*----------------------------------------------------------------------*/

   /*----------------------------------------------------------------------*/
   /* Bearbeitung von Belegungs Kommandos                                  */
   /*----------------------------------------------------------------------*/

   if(ov_string_compare(order,FB_OCCUPY)==0) {
     ov_string_setvalue(&pfb->v_commander,sender);
     ov_string_setvalue(&pfb->v_sender,NULL);
     ov_string_setvalue(&pfb->v_order,NULL);
     ov_string_setvalue(&pfb->v_value,NULL);
     Ov_HeapFree(sender);
     Ov_HeapFree(order);
     Ov_HeapFree(value);
     return OV_ERR_OK;
   }
   else {                       /* Kein Auftrag erkannt */
     Ov_HeapFree(sender);
     Ov_HeapFree(order);
     Ov_HeapFree(value);
     return OV_ERR_BADPARAM;
   }
 }                                 /* Einheit ist nicht belegt  */
 else {                            /* Unbekannter Auftraggeber  */

 /*----------------------------------------------------------------------*/
 /* Sender ist als Auftraggeber nicht zugelassen                         */
 /*----------------------------------------------------------------------*/

   Ov_HeapFree(sender);
   Ov_HeapFree(order);
   Ov_HeapFree(value);
   return OV_ERR_BADPARAM;
 }
}

/*----------------------------------------------------------------------*/
OV_STRING OV_DLLFNCEXPORT cmdlib_processcontrol_univ_comands()
/*----------------------------------------------------------------------*/
{
  return "TIOP;TOOP;RES;STOP";
}

