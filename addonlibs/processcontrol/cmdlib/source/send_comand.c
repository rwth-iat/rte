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
* send_comand.c Implementierung von Kommandostrukturen                            
*
* CVS Revision:  $Revision: 1.1.1.1 $                                                       
* CVS Date: $Date: 2002-08-16 13:55:23 $
*                                                                                 
*                                                                                 
* Historie                                                                        
* --------                                                                        
* 10-Aug-1999 Murat Fedai: Erstellung,                                            
*             Lehrstuhl fuer Prozessleittechnik, RWTH Aachen                      
* 10-Sep-1999 Erstellung von Klientenstrukturen                                   
* 16.08.2002 Michael Heeg: Einchecken der cmdlib in CVS 
*                          (manueller Versionsstand: 0.20)
*                                                                                 
***************************************************************************************/

#include "fb.h"
#include "fb_namedef.h"
#include "cmdlib.h"
#include "cmd_lib.h"
#include "libov/ov_path.h"
#include "libov/ov_memstack.h"
#include "libov/ov_string.h"
#include <string.h>
#include "libov/ov_time.h"
#include "libov/ov_macros.h"

/************************************************************************/
void OV_DLLFNCEXPORT send_comand(OV_INSTPTR_fb_functionblock  pinst,
     const char  *target, 	           /* >|  Zielinstanz	        */
     const char  *command,                 /* >|  Kommandoname          */
     const char  *value                    /* >|  Kommandowert          */
){
/************************************************************************/

  OV_RESULT                        	fr;
  OV_INSTPTR_ov_object   	        pobjt;
  OV_PATH				path;
  OV_INT       			        strlaenge;
  OV_STRING				pcomand;
  OV_STRING				localinst;
  OV_INSTPTR_cmdlib_processcontrol    	pfbt;

  /*----------------------------------------------------------------------*/
  /*               Pruefen der Uebergebenen Parameter                     */
  /*----------------------------------------------------------------------*/

  if(!pinst || !target || !value) {
    return;
  }


    /*----------------------------------------------------------------------*/
    /*                 Zeiger auf Zielinstanz suchen                        */
    /*----------------------------------------------------------------------*/
    ov_memstack_lock();
    localinst  = (OV_STRING) Ov_HeapMalloc (strlen(FB_INSTANZ_CONTAINER)+2+strlen(target)+1);
    if(!localinst) {
      return;
    }

    /* Name fuer Lokale Instanz kopieren */
    strcpy(localinst,"/");
    strcat(localinst,FB_INSTANZ_CONTAINER);
    strcat(localinst,"/");
    strcat(localinst,(char*)target);
    localinst[strlen(FB_INSTANZ_CONTAINER)+2+strlen(target)]='\0'; /* Abschliesende Null einfuegen */

    /* Zeiger auf Instanz suchen */
    fr = ov_path_resolve(&path,NULL,localinst, 2);
    if(Ov_Fail(fr)) {  /* Pfad nicht gefunden */
      Ov_HeapFree(localinst);
      ov_memstack_unlock();
      return;
    }
    if(path.elements->elemtype != OV_ET_OBJECT) {  /* Element ist kein Object */
      Ov_HeapFree(localinst);
      ov_memstack_unlock();
      return;
    }
    pobjt = path.elements[path.size-1].pobj;
    Ov_HeapFree(localinst);

    /*----------------------------------------------------------------------*/
    /*               Zienstanz gefunden, Auftrag senden                     */
    /*----------------------------------------------------------------------*/


    strlaenge = strlen(ov_path_getcanonicalpath(Ov_PtrUpCast(ov_object,pinst),2))+strlen(command)+strlen(value)+3;                          /* 3 =';'+';'+'\0' */

    pcomand = (OV_STRING) Ov_HeapMalloc(strlaenge);      /* Speicherplatz anlegen */
    if(!pcomand) {
      return;
    }
    strcpy(pcomand,ov_path_getcanonicalpath(Ov_PtrUpCast(ov_object,pinst),2));
    strcat(pcomand,";");
    strcat(pcomand,command);
    strcat(pcomand,";");
    strcat(pcomand,value);

    pfbt = Ov_StaticPtrCast(cmdlib_processcontrol,pobjt);
    if(pfbt) {
      cmdlib_processcontrol_order_set(pfbt,pcomand);
    }
    Ov_HeapFree(pcomand);
    ov_memstack_unlock();
    return;

}


