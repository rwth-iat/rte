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
* cmd_lib.h - Headerdatei mit Definitionsinformationen                            
*
* CVS Revision:  $Revision: 1.1.1.1 $                                                       
* CVS Date: $Date: 2002-08-16 13:55:23 $
*                                                                                 
*                                                                                 
* Historie                                                                        
* --------                                                                        
* 10-Aug-1999 Murat Fedai: Erstellung,                                            
*             Lehrstuhl fuer Prozessleittechnik, RWTH Aachen                      
* 18-Apr-2000 Umbenennung auf cmd_lib.h und Umbennung des Makros COM zu CMD       
*           
* 16.08.2002 Michael Heeg: Einchecken der cmdlib in CVS (manueller Versionsstand: 0.20                                                                      *
****************************************************************************************/

#ifndef _CMD_LIB_H
#define _CMD_LIB_H


#include "libov/ov_macros.h"

/* Belegungskommandos */
#define FB_OPERATOR  "OP"
#define FB_OCCUPY    "OCCUPY"
#define FB_FREE      "FREE"


/*
*  Der Modul sendet ein Kommando von einer Quellinstanz (pinst) an eine
*  Zielinstanz (zinstanz). Vor der Versendung wird die Belegung der Einheit
*  und des Kommandoeinganges überprüft.
*/

void send_comand(OV_INSTPTR_fb_functionblock pinst
	,const char  *target	        /* >|  Zielinstanz	      */
	,const char  *command           /* >|  Kommandoname           */
        ,const char  *value             /* >|  Kommandowert           */
	);

#define CMD(target,command,value) {\
        send_comand((OV_INSTPTR_fb_functionblock)pfb\
	,target\
        ,command\
        ,value);}

/*
*   End of file 'cmd_lib.h'
*/
#endif
