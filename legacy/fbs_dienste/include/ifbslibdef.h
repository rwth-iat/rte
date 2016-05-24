/*****************************************************************************
*                                                                            *
*    i F B S p r o                                                           *
*    #############                                                           *
*                                                                            *
*   L T S o f t                                                              *
*   Agentur für Leittechnik Software GmbH                                    *
*   Heinrich-Hertz-Straße 10                                                 *
*   50170 Kerpen                                                             *
*   Tel      : 02273/9893-0                                                  *
*   Fax      : 02273/9893-33                                                 *
*   e-Mail   : ltsoft@ltsoft.de                                              *
*   Internet : http://www.ltsoft.de                                          *
*                                                                            *
******************************************************************************
*                                                                            *
*   iFBSpro - Client-Bibliothek                                              *
*   IFBSpro/Client - iFBSpro  ACPLT/KS Dienste-Schnittstelle (C++)           *
*   ==============================================================           *
*                                                                            *
*   Datei                                                                    *
*   -----                                                                    *
*   ifbslibdef.h                                                             *
*                                                                            *
*   Historie                                                                 *
*   --------                                                                 *
*   2000-01-03  Alexander Neugebauer: Erstellung, LTSoft, Kerpen             *
*                                                                            *
*                                                                            *
*   Beschreibung                                                             *
*   ------------                                                             *
*   Funktions-Definitionen                                                   *
*                                                                            *
*****************************************************************************/

#ifndef _IFBLIBDEF_INCLUDE_
#define _IFBLIBDEF_INCLUDE_

/*
*	C-Includes
*	----------
*/

#include "plt/config.h"

#if PLT_USE_DEPRECIATED_HEADER
#include <iostream.h>
#else
#include <iostream>
#endif

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

/*
*	KS-Includes
*	-----------
*/

#include "ks/package.h"
#include "ks/string.h"
#include "ks/objmgrparams.h"
#include "ks/client.h"
#include "ks/mask.h"
#include "plt/list.h"
#include "plt/debug.h"
#include "ks/avsimplemodule.h"

#if PLT_COMPILER_BORLAND
#include "ks\array_builtins.h"
#include "ks/array.h"
#include "ks/array_builtins.h"
#include "ks/handle.h"
#include "ks/list.h"
#endif


//#include "libov/ov_ov.h"
//cherry pick the few missing defines
#define OV_ERR_FILEALREADYEXISTS	0x00010000
#define OV_ERR_CANTCREATEFILE		0x00010001
#define OV_ERR_CANTOPENFILE		0x00010002
#define OV_ERR_CANTLOCKFILE		0x00010003
#define OV_ERR_CANTREADFROMFILE		0x00010004
#define OV_ERR_CANTWRITETOFILE		0x00010005
#define OV_ERR_CANTMAPFILE		0x00010006
#define OV_ERR_BADDATABASE		0x00010007

#define OV_ERR_CANTOPENLIBRARY		0x00010010

#define OV_ERR_LIBDEFMISMATCH		0x00010020
#define OV_ERR_STRUCTDEFMISMATCH	0x00010021
#define OV_ERR_CLASSDEFMISMATCH		0x00010022
#define OV_ERR_ASSOCDEFMISMATCH		0x00010023
#define OV_ERR_VARDEFMISMATCH		0x00010024
#define OV_ERR_PARTDEFMISMATCH		0x00010025
#define OV_ERR_OPDEFMISMATCH		0x00010026
#define OV_ERR_UNKNOWNSTRUCTDEF		0x00010027
#define OV_ERR_UNKNOWNCLASSDEF		0x00010028
#define OV_ERR_UNKNOWNASSOCDEF		0x00010029

#define OV_ERR_DBOUTOFMEMORY		0x00010030
#define OV_ERR_HEAPOUTOFMEMORY		0x00010031

ENUMDEF(OV_VAR_TYPE)
#define OV_VT_BYTE           ENUMVAL(OV_VAR_TYPE, 0x01)	/*not in KS */

#define OV_VT_CTYPE          ENUMVAL(OV_VAR_TYPE, 0x41)	/*not in KS */
#define OV_VT_POINTER        ENUMVAL(OV_VAR_TYPE, 0x42)	/*not in KS */
#define OV_VT_ISVECTOR       ENUMVAL(OV_VAR_TYPE, 0x80)	/*not in KS; OV_VT_type_VEC = (OV_VT_type | OV_VT_ISVECTOR) */

#define OV_VT_STRUCT_VEC     ENUMVAL(OV_VAR_TYPE, 0x000000C0)	/*not in KS */

#define OV_VT_HAS_STATE      ENUMVAL(OV_VAR_TYPE, 0x00010000)	/*not in KS */
#define OV_VT_HAS_TIMESTAMP  ENUMVAL(OV_VAR_TYPE, 0x00020000)	/*not in KS */
#define OV_VT_KSMASK         ENUMVAL(OV_VAR_TYPE, 0x0000ffff)	/*not in KS */



#define IFBS_INC_NAMES_ONLY
#include "fb_namedef.h"

#include "fb_macros.h"
#include "ifbslib_params.h"
#include "blockparam.h"
#include "par_param.h"

/*
*   Definitionen
*   ------------
*/
#define IFB_PV_PORTIDENT      "  (PV)"
#define IFB_DYNAMIC_PORTIDENT "  (dynamic)"

#define FB_LIB_PATH           "/fb/"
#define CONTAINER_CLASS_PATH  "/acplt/ov/domain"
#define VARIABLE_CLASS_PATH   "/acplt/ov/variable"
#define CONNECTION_CLASS_PATH "/fb/connection"
#define TASK_CLASS_PATH       "/fb/task"
#define CLASS_CLASS_PATH      "/acplt/ov/class"

#define LIBRARY_FACTORY_PATH  "/acplt/ov/library"
/*
*   Funtions-Prototypen
*/


/*  Holt Zeiger auf KscServer-Objekt. */
KscServerBase* GetServerByName(                      /*  |> Funktionsrueckmeldung         */
                                PltString   &hs      /* >|  Host und Server : Host/Server */
                                ,KS_RESULT  &res     /*  |> Dienst-Ergebnis               */
                                );
/*  Klartext-Ausgabe des KS-Fehlers */
char *GetErrorCode (
                                            KS_RESULT fehler    /* >|  Errorcode                     */
                                            );
PltString iFBS_SetLastError(int set, KS_RESULT &err, PltString& Str);
PltString IFBS_GetLastLogError();
PltString IFBS_GetParserError();
PltString iFBS_PrintParserError(int set, PltString& Err);
/*
* Hilfsfunktion.
* Rueckgabe der Value-Laenge als String in Form "[Laenge]", falls
* Value ein Vektor ist, bzw. "" bei Skalar-Variablen
*/
char *ifb_getValueLength(                           /*  |> Funktionsrueckmeldung      */
                         const KsVarCurrProps* cp   /* >|  Eigenschaften der Variable */
                         );
/*
* Hilfsfunktion.
* Rueckgabe der Value-Laenge als numerischen Wert
*/
size_t ifb_getValueLengthAsNum(                     /*  |> Funktionsrueckmeldung      */
                        const KsVarCurrProps* cp    /* >|  Eigenschaften der Variable */
                        );
/*
* Hilfsfunktion.
* Rueckgabe Value-Type als String : KS_VT_INT als "INT", KS_VT_INT_VEC als "INT_VEC"
*/
char const *ifb_getPortValueType(                         /*  |> Funktionsrueckmeldung      */
                         KS_VAR_TYPE typ            /* >|  Typ der Variable */
                         );
/*
* Hilfsfunktion.
* Rueckgabe Value-Type als String fuer FB-Sicherung der Datenbasis
* zB. KS_VT_INT oder KS_VT_INT_VEC als "INT"
*/
char const *ifb_getOvValueType(                          /*  |> Funktionsrueckmeldung      */
                        KS_VAR_TYPE typ            /* >|  Typ der Variable */
                        );
/*
* Hilfsfunktion.
* Rueckgabe Value-Type und Laenge
*/
KS_RESULT ifb_getValueTypeAndLen(                           /*  |> Funktionsrueckmeldung                */
                            PltString            &Path      /* >|  Pfad der Variable                    */
                            ,KS_VAR_TYPE         &Typ       /*  |> Value-Type                           */
                            ,size_t              &Len       /*  |> Laenge, falls Variable ein Vektor    */
                            );
/*
* Hilfsfunktion.
* Gibt die Merkmahle des Ports als String :
* beim gesetztem Flag 'd' "(dynamic)" und
* beim gesetztem Flag 's' "(PV)". Sonst ""
*/
char const *ifb_getPortCharacteristic(                    /*  |> Funktionsrueckmeldung      */
                        KS_SEMANTIC_FLAGS flag      /* >|  Flags-Maske                */
                        );
/*
* Hilfsfunktion :
* Aendert ->,<- in ->.<-  im Float-Wert als String
*/
KS_RESULT fixFloatValue(
                        KsString &Val
                        );
/*
* Hilfsfunktion.
* Wert der Variable als String
*/
KS_RESULT ifb_getValueOnly(                                              /*  |> Funktionsrueckmeldung      */
                           const KsVarCurrProps* cp                      /* >|  Eigenschaften der Variable */
                           ,PltString            &Out                    /* >|> Value als String           */
                           ,unsigned long        MaxElem = ULONG_MAX     /* Max. Anzahl Elementen in Array */
                           ,bool                 ShortTime = FALSE       /* Merker, ob Time-Ausgabe "... hh:mm:ss" */
                           ,bool                 BoolAsNum = FALSE       /* Merker, ob TRUE als "1" und FALSE als "0" */
                           ,bool                 MakeList = TRUE         /* Merker, ob Array als Liste "{...}"        */
                           ,char*                ELEMENT_TRENNER = " ; " /* Trenner der Array-Elementen               */
                           ,bool                 Str = TRUE              /* Merker, ob string in Hochkommas   */
                           ,bool                 FixStr = TRUE           /* TRUE : '"'->'\"'    */
                           );
/*
* Hilfsfunktion.
* Rueckgabe : Daten einer Verbindung
*/
KS_RESULT get_comcon_data(
                                            KscServerBase*     Server,
                                            PltString          &Vname,
                                            ConData            &Data
                                            );

/*
*  Hilfsfunktion.
*  Rueckgabe : Daten aller Verbindungen zum Baustein
*/
KS_RESULT IFBS_GETCONS(
                                            KscServerBase*      Server
                                            ,FbDirParams        &Pars
                                            ,PltList<ConData>   &Res
                                            );
KS_RESULT IFBS_SETVAR(
                                            KscServerBase*      Server
                                            ,FbSetVarParams     &Pars
                                            );
/*
*  Hilfsfunktion.
*  Rueckgabe : Daten eines Ports
*/
KS_RESULT IFBS_GETPORTDATA(
                                                 KscServerBase*      Server
                                                 ,PltString          &port
                                                 ,ObjProps           &res
                                                 ,unsigned long      MaxElem = ULONG_MAX     /* Max. Anzahl Elementen in Array */
                                                 );

KS_RESULT getDomainProps(KscServerBase* 	   Server,
                         PltString             &inst_name,
                         KsEngPropsHandle      &hpo);
KS_RESULT ReadBlockPorts(KscServerBase* 	   Server,
                         PltString             &inst_name,
                         PltList<InstVarProps> &hpp);

void InsertAllComCon(const KsVarCurrProps &cp, PltList<PltString> &ConList);

PltString iFBS_SetLastError(int set, KS_RESULT &err, PltString& Str);

void GetPortProps(
                   KsEngPropsHandle      &hep
                   ,const KsVarCurrProps *cp
                   ,ObjProps             &item
                   ,unsigned long        MaxElem = ULONG_MAX     /* Max. Anzahl Elementen in Array */
                  );
                  
KscAvSimpleModule * GetClientAV();
KS_RESULT SetClientAV( PltString &AV );
KS_RESULT SetServerAV(KscServerBase  *Server,
                                                 PltString      &AV );
KS_RESULT IFBS_CREATE_COMCON(
                    KscServerBase* Server,
                    ConData        &pars,      /* Daten der Verbindungsobjektes */
                    Variables      *pvars = 0  /* Extra Variablen, (if not from fb/connection) */
                    );

void GenerateComConName(PltString &VerbName);

KS_RESULT ifb_SeparateValueAndSet(KscServerBase*    Server
                                 ,PltString         &VarPath
                                 ,PltString         &Value
                                 ,KS_VAR_TYPE       Typ
                                 ,KS_STATE          Status = 0
                                 ,bool              prepareString = TRUE);
KS_RESULT ifb_setVar(KscServerBase*         Server
                     ,PltString             &VarPath
                     ,PltList<PltString>    &ValList
                     ,KS_VAR_TYPE           Typ
                     ,KS_STATE              Status = 0
                     ,bool                  prepareString = TRUE);
KS_RESULT SeparateString(PltString& wert, KS_VAR_TYPE typ, PltList<PltString>& Liste);

KsSetVarItem* obj_CreateObjectVar( Variables* pvar);
KsValue* ifb_CrNewKsValue(
                         KS_RESULT              &err
                         ,PltList<PltString>    &ValList
                         ,KS_VAR_TYPE           Typ
                         ,bool                  prepareString = TRUE);
KS_RESULT PrepareStringValue(PltString& wert);

KS_RESULT Get_getEP_ErrOnly(KscServerBase* Server,
                            KsGetEPParams& params);
                            
KS_RESULT IFBS_DELETE_OBJ(KscServerBase* Server,
                          PltString      &path);

KS_RESULT SearchErrorCreateObject(KscServerBase*     Server,
                                  KS_RESULT          &res,
                                  FbCreateInstParams &pars);
KS_RESULT IFBS_CREATE_INST(KscServerBase*     Server,
                                                      FbCreateInstParams &Pars);
KS_RESULT IFBS_LINK_TO_TASK(KscServerBase*     Server,
                                                       FbLinkParams       &Pars);
KS_RESULT IFBS_UNLINK_FROM_TASK(KscServerBase* Server,
                                                           FbLinkParams   &Pars);
KS_RESULT IFBS_RENAME(KscServerBase* Server,
                                                 PltString      &old_path,
                                                 PltString      &new_path);
KS_RESULT IFBS_DELETE_FULLCONT(
                                                 KscServerBase*  Server,
                                                 PltString       &tu
                                                 );
KS_RESULT del_inst_childs(KscServerBase*  Server,
                          PltString       &inst,
                          PltString*      Logging);
KS_RESULT del_cont_recurs(KscServerBase*  Server,
                          PltString       &tu,
                          PltString*      Logging = 0);
KS_RESULT delAllXlinks(KscServerBase* Server, PltString &Str);

//KS_RESULT del_InstAndConsFormInstList(KscServerBase*      Server,
//                                      PltList<PltString>  &InstListe,
//                                      PltString           *Logging = 0);

KS_RESULT GetVerbFromList(KscServerBase*        Server,
                          PltList<PltString>    &InstListe,
                          PltList<PltString>    &VerbListe);
KS_RESULT TurnComConOff(KscServerBase*      Server,
                        PltList<PltString>  &VerbListe);
KS_RESULT DelObjsFromList(KscServerBase*      Server,
                          PltList<PltString>  &Liste,
                          PltString*          Logging = 0);
KS_RESULT TurnInstOff(KscServerBase*      Server,
                      PltList<PltString>  &InstListe);
                      
KS_RESULT get_libs(KscServerBase* Server, PltString& Out);
//KS_RESULT get_class(KscServerBase* Server, KsGetEPParams& params, PltString& Out, FILE *fout);
//KS_RESULT get_task_list(KscServerBase* Server,KsGetEPParams& params,PltString& Out, FILE *fout);
//KS_RESULT get_con_list(KscServerBase*,KsGetEPParams&,PltString&, FILE *fout);
//KS_RESULT get_xlink_list(KscServerBase*,KsGetEPParams&,PltString&, FILE *fout, bool parentOnly=TRUE);

KS_RESULT IFBS_DBSAVE(KscServerBase* 	Server,
                                      PltString        &datei);
void memfre(Dienst_param* pars);
KS_RESULT import_eval(KscServerBase*  Server
                      ,Dienst_param*  Params
                      ,PltString&     out
                      ,bool           DEL_INST=TRUE /* Merker, ob die angelegte Instanzen beim Fehler loeschen */
                      );
KS_RESULT IFBS_DBLOAD(KscServerBase* Server,
                      PltString&     inpfile,
                      PltString&     err_outfile);
KS_RESULT FB_CreateNewInstance(KscServerBase* Server,
                               InstanceItems* pinst,
                               PltString&     out);
KS_RESULT GetCreateObjectVar( Variables* pvar, KsArray<KsSetVarItem> &pars);
KS_RESULT aufraeumen(KscServerBase*  Server,
                     Dienst_param*   Params,
                     PltString&      out);
int IsNotInList(PltString& Value, PltList<PltString> &Liste);

KS_RESULT IFBS_DBCOMPARE(PltString& olddat,
                         PltString& newdat,
                         PltString& proto);

KS_RESULT compare_eval(Dienst_param* newpar,
                       Dienst_param* oldpar,
                       PltString& out);
void compare_libraries(Dienst_param* newpar,Dienst_param* oldpar);
KS_RESULT compare_any_inst(Dienst_param*,Dienst_param*,Dienst_param*,PltString&);
int  value_compare(Variables*, Variables*);
int  param_compare(InstanceItems*, InstanceItems*, PltString&);
void removeComConnLinks(Dienst_param *param, InstanceItems* ComConn);
int compareComConnLinks( Dienst_param *Newpar
                        ,Dienst_param *Oldpar
                        ,const char   *VerbName);
void put_variable(Variables* hpv, PltString& Out);

void get_any_links(Dienst_param*, Dienst_param*);


KS_RESULT  IFBS_DBUPDATE(KscServerBase *Server,
                         PltString     &datei,
                         PltString     &err_outfile);

KS_RESULT  update_eval(KscServerBase* Server,
                        Dienst_param*  Params,
                        PltString&     out);
int unlink_old_link(KscServerBase*, Dienst_param*, PltString&);
int delete_old_instance(KscServerBase*,Dienst_param*,PltString& );
int set_new_value(KscServerBase*,Dienst_param*,PltString&);

void AddValueToList(PltString& ,  PltList<PltString>&);

KS_RESULT IFBS_DIR(KscServerBase* 	Server,
                   FbDirParams        &Pars,
                   PltList<ObjProps>  &Res);
                   

void writeHeadSelectSave(KscServerBase* Server, PltString &out, FILE* fout=0);
                        
KS_RESULT write_inst_from_class(KscServerBase*, FbDirParams&,
                          PltString&, FILE *);

void write_help_message_from_select_save(PltString &out);

KS_RESULT select_save(KscServerBase* 	Server,
                                            FbDirParams     &Pars,
                                            PltString       &datei);
KS_RESULT IFBS_SELECT_SAVE(KscServerBase   *Server,
                                                      FbDirParams     &Pars,
                                                      PltString       &out);
                                                      
KS_RESULT IFBS_DUPLICATE(KscServerBase*  Server,
                                         PltString       &tu,
                                         PltString       &new_name);
KS_RESULT LoadDupl(KscServerBase* 	Server,
                   PltString        &Inp_String);
KS_RESULT getChildObjectsList(
                            KscServerBase 	   *Server,
						    KsGetEPParams      &params,
						    PltList<PltString> &erg
						    );
KS_RESULT IFBS_CLEANDB(KscServerBase*  Server, PltString*  Logging = 0);

KS_RESULT ifb_deleteLink(KscServerBase*     Server,
                         FbLinkParams       &Pars);
KS_RESULT ifb_createLink(KscServerBase*     Server,
                         FbLinkParams       &Pars);
                         
void ifb_SortList(PltList<PltString> &Liste);

KS_RESULT IFBS_UPLOAD_GETPATH(KscServerBase*  Server,
                              PltString &path);

KS_RESULT IFBS_UPLOAD(
                        KscServerBase*  Server,
                        PltString       cmd,        // "c" fuer 'Copy', "r" fuer 'Replace'
                        PltString       inFile,     // Pfad der zu kopierenden Datei
                        PltString       upPath,     // Pfad der Upload-Instanz auf Remote-Server
                        PltString       pwd,        // Passwort fuer Replace-Cmd
                        PltString       outFil,     // Name der Datei fuer Remote-Server
                        PltString      &Out         // Fehlermeldung im Klartext
                        );
    
KS_RESULT IFBS_UPLOAD_LIBRARY(
                        KscServerBase*  Server,
                        PltString       inFile,     // Pfad der zu kopierenden Datei
                        PltString       pwd,        // Passwort fuer Replace-Cmd
                        PltString      &Out         // Fehlermeldung im Klartext
                        );
    
    
KS_RESULT IFBS_DBSAVEXML(KscServerBase*   Server,
                         PltString        &path,
                         PltString        &datei,
                         PltTime          &wTim);

KS_RESULT ifb_writeInstData(
    KscServerBase *Server,
    KsGetEPParams &params,
    PltString     &Out,
    bool           recurs,
    bool           saveConLinks,
    bool           linkParentOnly,
    FILE          *fout
);

void ifb_writeLibItem(KsString libname, PltString& Out);
KS_RESULT saveInstances(KscServerBase* Server, FbDirParams& Pars,
                        PltList<PltString> &libList, PltList<PltString> &conList,
                        PltString& out, int& Recurs, FILE *fout=0);
KS_RESULT saveConsFromList(KscServerBase* Server, PltList<PltString> &conList,
                          PltString& out, FILE *fout=0);
// Holt Server-Version
float get_serverVersion(KscServerBase* Server);
                          
PltString log_getErrMsg(
    KS_RESULT   err,
    const char* msg1=NULL,
    const char* msg2=NULL,
    const char* msg3=NULL,
    const char* msg4=NULL,
    const char* msg5=NULL,
    const char* msg6=NULL,
    const char* msg7=NULL
);

PltString log_getOkMsg(
    const char* msg1=NULL,
    const char* msg2=NULL,
    const char* msg3=NULL,
    const char* msg4=NULL,
    const char* msg5=NULL,
    const char* msg6=NULL,
    const char* msg7=NULL
);

/* EH - Wrappers */
KS_RESULT clean_all_comcons(KscServerBase*  Server, PltString *Log = 0);
KS_RESULT clean_all_inst(KscServerBase*  Server, PltString *Log = 0);
KS_RESULT clean_all_libs(KscServerBase*  Server, PltString *Log = 0);
KS_RESULT clean_all_tasks(KscServerBase*  Server, PltString *Log = 0);
int verb_suchen(KscServerBase*, ConData &, PltString&);

#endif
