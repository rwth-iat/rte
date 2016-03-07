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
*   log_error.cpp                                                            *
*                                                                            *
*   Historie                                                                 *
*   --------                                                                 *
*   2000-05-12  Alexander Neugebauer: Erstellung, LTSoft, Kerpen             *
*                                                                            *
*                                                                            *
*   Beschreibung                                                             *
*   ------------                                                             *
*   Hilfsfunktionen : Ausgabe der Dienst-Fehlers im Klartext                 *
*                                                                            *
*****************************************************************************/


#include "ifbslibdef.h"

/*****************************************************************************/
char *GetErrorCode (KS_RESULT fehler)
/*****************************************************************************/
{
 static char msg[32];

 switch (fehler)
  {
    case KS_ERR_OK:
        strcpy(msg,"KS_ERR_OK");
        break;
    case KS_ERR_GENERIC:
        strcpy(msg,"KS_ERR_GENERIC");
        break;
    case KS_ERR_TARGETGENERIC:
        strcpy(msg,"KS_ERR_TARGETGENERIC");
        break;
    case KS_ERR_BADAUTH:
        strcpy(msg,"KS_ERR_BADAUTH");
        break;
    case KS_ERR_UNKNOWNAUTH:
        strcpy(msg,"KS_ERR_UNKNOWNAUTH");
        break;
    case KS_ERR_NOTIMPLEMENTED:
        strcpy(msg,"KS_ERR_NOTIMPLEMENTED");
        break;
    case KS_ERR_BADPARAM:
        strcpy(msg,"KS_ERR_BADPARAM");
        break;
    case KS_ERR_BADOBJTYPE:
        strcpy(msg,"KS_ERR_BADOBJTYPE");
        break;
    case KS_ERR_BADNAME:
        strcpy(msg,"KS_ERR_BADNAME");
        break;
    case KS_ERR_BADPATH:
        strcpy(msg,"KS_ERR_BADPATH");
        break;
    case KS_ERR_BADMASK:
        strcpy(msg,"KS_ERR_BADMASK");
        break;
    case KS_ERR_NOACCESS:
        strcpy(msg,"KS_ERR_NOACCESS");
        break;
    case KS_ERR_BADTYPE:
        strcpy(msg,"KS_ERR_BADTYPE");
        break;
    case KS_ERR_CANTSYNC:
        strcpy(msg,"KS_ERR_CANTSYNC");
        break;
    case KS_ERR_BADSELECTOR:
        strcpy(msg,"KS_ERR_BADSELECTOR");
        break;
    case KS_ERR_BADVALUE:
        strcpy(msg,"KS_ERR_BADVALUE");
        break;
    case KS_ERR_NOREMOTE:
        strcpy(msg,"KS_ERR_NOREMOTE");
        break;
    case KS_ERR_SERVERUNKNOWN:
        strcpy(msg,"KS_ERR_SERVERUNKNOWN");
        break;
    case KS_ERR_BADFACTORY:
        strcpy(msg,"KS_ERR_BADFACTORY");
        break;
    case KS_ERR_ALREADYEXISTS:
        strcpy(msg,"KS_ERR_ALREADYEXISTS");
        break;
    case KS_ERR_BADINITPARAM:
        strcpy(msg,"KS_ERR_BADINITPARAM");
        break;
    case KS_ERR_BADPLACEMENT:
        strcpy(msg,"KS_ERR_BADPLACEMENT");
        break;
    case KS_ERR_CANTMOVE:
        strcpy(msg,"KS_ERR_CANTMOVE");
        break;
    case KS_ERR_MALFORMEDPATH:
        strcpy(msg,"KS_ERR_MALFORMEDPATH");
        break;
    case KS_ERR_NETWORKERROR:
        strcpy(msg,"KS_ERR_NETWORKERROR");
        break;
    case KS_ERR_TYPEMISMATCH:
        strcpy(msg,"KS_ERR_TYPEMISMATCH");
        break;
    case KS_ERR_HOSTUNKNOWN:
        strcpy(msg,"KS_ERR_HOSTUNKNOWN");
        break;
    case KS_ERR_CANTCONTACT:
        strcpy(msg,"KS_ERR_CANTCONTACT");
        break;
    case KS_ERR_TIMEOUT:
        strcpy(msg,"KS_ERR_TIMEOUT");
        break;
    case KS_ERR_NOMANAGER:
        strcpy(msg,"KS_ERR_NOMANAGER");
        break;


    case OV_ERR_FILEALREADYEXISTS:
        strcpy(msg,"OV_ERR_FILEALREADYEXISTS");
        break;
    case OV_ERR_CANTCREATEFILE:
        strcpy(msg,"OV_ERR_CANTCREATEFILE");
        break;
    case OV_ERR_CANTOPENFILE:
        strcpy(msg,"OV_ERR_CANTOPENFILE");
        break;
    case OV_ERR_CANTLOCKFILE:
        strcpy(msg,"OV_ERR_CANTLOCKFILE");
        break;
    case OV_ERR_CANTREADFROMFILE:
        strcpy(msg,"OV_ERR_CANTREADFROMFILE");
        break;
    case OV_ERR_CANTWRITETOFILE:
        strcpy(msg,"OV_ERR_CANTWRITETOFILE");
        break;
    case OV_ERR_CANTMAPFILE:
        strcpy(msg,"OV_ERR_CANTMAPFILE");
        break;
    case OV_ERR_BADDATABASE:
        strcpy(msg,"OV_ERR_BADDATABASE");
        break;
    case OV_ERR_CANTOPENLIBRARY:
        strcpy(msg,"OV_ERR_CANTOPENLIBRARY");
        break;
    case OV_ERR_LIBDEFMISMATCH:
        strcpy(msg,"OV_ERR_LIBDEFMISMATCH");
        break;
    case OV_ERR_STRUCTDEFMISMATCH:
        strcpy(msg,"OV_ERR_STRUCTDEFMISMATCH");
        break;
    case OV_ERR_CLASSDEFMISMATCH:
        strcpy(msg,"OV_ERR_CLASSDEFMISMATCH");
        break;
    case OV_ERR_ASSOCDEFMISMATCH:
        strcpy(msg,"OV_ERR_ASSOCDEFMISMATCH");
        break;
    case OV_ERR_VARDEFMISMATCH:
        strcpy(msg,"OV_ERR_VARDEFMISMATCH");
        break;
    case OV_ERR_PARTDEFMISMATCH:
        strcpy(msg,"OV_ERR_PARTDEFMISMATCH");
        break;
    case OV_ERR_OPDEFMISMATCH:
        strcpy(msg,"OV_ERR_OPDEFMISMATCH");
        break;
    case OV_ERR_UNKNOWNSTRUCTDEF:
        strcpy(msg,"OV_ERR_UNKNOWNSTRUCTDEF");
        break;
    case OV_ERR_UNKNOWNCLASSDEF:
        strcpy(msg,"OV_ERR_UNKNOWNCLASSDEF");
        break;
    case OV_ERR_UNKNOWNASSOCDEF:
        strcpy(msg,"OV_ERR_UNKNOWNASSOCDEF");
        break;
    case OV_ERR_DBOUTOFMEMORY:
        strcpy(msg,"OV_ERR_DBOUTOFMEMORY");
        break;
    case OV_ERR_HEAPOUTOFMEMORY:
        strcpy(msg,"OV_ERR_HEAPOUTOFMEMORY");
        break;

    case FB_ERR_ATPARSE:
        strcpy(msg,"FB_ERR_ATPARSE");
        break;

    default:
        sprintf(msg,"Unknown code 0x%x", fehler);
    }

  return msg;
}

/******************************************************************************/
PltString iFBS_SetLastError(int set, KS_RESULT &err, PltString& Str) {
/******************************************************************************/
  static PltString ErrStr("");
  char   msg[256];
  
  if(!set) {
    return ErrStr;
  }
  
  switch (err) {
    
     case KS_ERR_OK:
                    ErrStr = Str;
                    break;
     default :
                    sprintf(msg, "%i  \"%s\"  ", err, GetErrorCode(err) );
                    ErrStr = msg;
                    ErrStr += Str;
                    break;
  }

  return ErrStr;
}

/******************************************************************************/
PltString IFBS_GetLastLogError() {
/******************************************************************************/

  PltString Str;
  KS_RESULT err = 0;
  
  return iFBS_SetLastError(0, err, Str);
}

/******************************************************************************/
PltString iFBS_PrintParserError(int set, PltString& Err) {
/******************************************************************************/

 static PltString Str;

 if(set) {
    Str = Err;
 }
 
 return Str;

}

/******************************************************************************/
PltString IFBS_GetParserError() {
/******************************************************************************/
 PltString Str;

 return iFBS_PrintParserError(0, Str);
}

/******************************************************************************/
extern "C" void iFBS_SetParserError(int line, char* msg) {
/******************************************************************************/
 
 PltString Str;
 KS_RESULT err = 0;
 char ph[1024];
 
 sprintf(ph, "line : %d : ", line);
 
 Str = ph;
 Str += msg;

 iFBS_PrintParserError( 1, Str);
 
 sprintf(ph, "%d  \"%s\"  \"line %s\"  %d  \"%s\"",
              FB_ERR_ATPARSE,
                   "FB_ERR_ATPARSE",
                                "%d %s",
                                        line,
                                             msg);
 Str = ph;
 
 iFBS_SetLastError(1, err, Str);

 return;

}

/*****************************************************************************/
KS_RESULT Get_getEP_ErrOnly(KscServerBase* Server,
                            KsGetEPParams& params)    {
/*****************************************************************************/
    KsGetEPResult result;
    KS_RESULT     err;
    bool ok = Server->getEP(0, params, result);
    if ( !ok ) {
        err = Server->getLastResult();
        if(err == KS_ERR_OK) err = KS_ERR_GENERIC;
        return err;
    }

    if ( result.result != KS_ERR_OK ) {
           return result.result;
    }
    if(params.name_mask != "*") {
        if ( !(result.items.size()) ) {
            return 1;
        }
    }
    return KS_ERR_OK;
}

/*****************************************************************************/
PltString log_getErrMsg(
    KS_RESULT   err,
    const char* msg1,
    const char* msg2,
    const char* msg3,
    const char* msg4,
    const char* msg5,
    const char* msg6,
    const char* msg7
)    {
/*****************************************************************************/
    PltString  out;
    
    out = "ERR ";
    if(msg1) {
        out += msg1;
        out += " ";
    }
    if(msg2) {
        out += msg2;
        out += " ";
    }
    if(msg3) {
        out += msg3;
        out += " ";
    }
    if(msg4) {
        out += msg4;
        out += " ";
    }
    if(msg5) {
        out += msg5;
        out += " ";
    }
    if(msg6) {
        out += msg6;
        out += " ";
    }
    if(msg7) {
        out += msg7;
        out += " ";
    }
    if(err != KS_ERR_OK) {
        out += " Error : ";
        out += GetErrorCode(err);
    }
    out += "\n";

    return out;
}
/*****************************************************************************/
PltString log_getOkMsg(
    const char* msg1,
    const char* msg2,
    const char* msg3,
    const char* msg4,
    const char* msg5,
    const char* msg6,
    const char* msg7
)    {
/*****************************************************************************/
    PltString  out;
    
    out = "OK  ";
    if(msg1) {
        out += msg1;
        out += " ";
    }
    if(msg2) {
        out += msg2;
        out += " ";
    }
    if(msg3) {
        out += msg3;
        out += " ";
    }
    if(msg4) {
        out += msg4;
        out += " ";
    }
    if(msg5) {
        out += msg5;
        out += " ";
    }
    if(msg6) {
        out += msg6;
        out += " ";
    }
    if(msg7) {
        out += msg7;
        out += " ";
    }
    out += "\n";

    return out;
}

