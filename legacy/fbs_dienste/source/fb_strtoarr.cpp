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
*    iFBSpro  ACPLT/KS Dienste-Schnittstelle (C++)                           *
*   ===============================================                          *
*                                                                            *
*   Datei                                                                    *
*   -----                                                                    *
*   fb_strtoarr.cpp                                                          *
*                                                                            *
*   Historie                                                                 *
*   --------                                                                 *
*   1999-04-07  Alexander Neugebauer: Erstellung, LTSoft, Kerpen             *
*                                                                            *
*                                                                            *
*   Beschreibung                                                             *
*   ------------                                                             *
*   Setzt eine Variable einer Funktionblockinstanz                           *
*                                                                            *
*****************************************************************************/

#include "ifbslibdef.h"

/*****************************************************************************/
KS_RESULT BoolToArray(ObjProps        &port,
                                                 PltArray<bool>  &arr) {
/*****************************************************************************/

 PltList<PltString> Liste;
 unsigned int       j;
 PltString          Str("");
 KS_RESULT          err = 0;
 
 iFBS_SetLastError(1, err, Str);
 
 if(port.len == 0 ) {
    return KS_ERR_OK;
 }
 
 if(port.len != arr.size() ) {
    err = KS_ERR_BADINITPARAM;
    Str = "\"%s %s\"  \"";
    Str += port.identifier;
    Str += "\" \"";
    Str += port.value;
    Str += "\"";
    iFBS_SetLastError(1, err, Str);
    return err;
 }

 err = SeparateString(port.value, KS_VT_BOOL_VEC, Liste);
 if(err) {
    Str = "\"%s %s\"  \"";
    Str += port.identifier;
    Str += "\" \"";
    Str += port.value;
    Str += "\"";
    iFBS_SetLastError(1, err, Str);
    return err;
 }

 if(port.len != Liste.size() ) {
    err = KS_ERR_BADINITPARAM;
    Str = "\"%s %s\"  \"";
    Str += port.identifier;
    Str += "\" \"";
    Str += port.value;
    Str += "\"";
    iFBS_SetLastError(1, err, Str);
    return err;
 }

 for(j=0; j < arr.size() ; j++ ) {
    Str = Liste.removeFirst();
    if( Str == "FALSE") {
        arr[j] = false;
    } else {
        arr[j] = true;
    }
 }

 return KS_ERR_OK;

}


/*****************************************************************************/
KS_RESULT IntToArray(ObjProps   &port,
                                                PltArray<int>    &arr) {
/*****************************************************************************/

 PltList<PltString> Liste;
 unsigned int       j;
 PltString          Str("");
 KS_RESULT          err = 0;
 
 iFBS_SetLastError(1, err, Str);

 if(port.len == 0 ) {
    return KS_ERR_OK;
 }
 if(port.len != arr.size() ) {
    err = KS_ERR_BADINITPARAM;
    Str = "\"%s %s\"  \"";
    Str += port.identifier;
    Str += "\" \"";
    Str += port.value;
    Str += "\"";
    iFBS_SetLastError(1, err, Str);
    return err;
 }

 err = SeparateString(port.value, KS_VT_INT_VEC, Liste);
 if(err) {
    Str = "\"%s %s\"  \"";
    Str += port.identifier;
    Str += "\" \"";
    Str += port.value;
    Str += "\"";
    iFBS_SetLastError(1, err, Str);
    return err;
 }

 if(port.len != Liste.size() ) {
    err = KS_ERR_BADINITPARAM;
    Str = "\"%s %s\"  \"";
    Str += port.identifier;
    Str += "\" \"";
    Str += port.value;
    Str += "\"";
    iFBS_SetLastError(1, err, Str);
    return err;
 }

 for(j=0; j < arr.size() ; j++ ) {
    Str = Liste.removeFirst();
    arr[j] = (int)atoi( (const char*)Str);
 }

 return KS_ERR_OK;

}

/*****************************************************************************/
KS_RESULT UintToArray(ObjProps   &port,
                                                 PltArray<unsigned int> &arr) {
/*****************************************************************************/

 PltList<PltString> Liste;
 unsigned int       j;
 PltString          Str("");
 KS_RESULT          err = 0;
 
 iFBS_SetLastError(1, err, Str);

 if(port.len == 0 ) {
    return KS_ERR_OK;
 }
 if(port.len != arr.size() ) {
    err = KS_ERR_BADINITPARAM;
    Str = "\"%s %s\"  \"";
    Str += port.identifier;
    Str += "\" \"";
    Str += port.value;
    Str += "\"";
    iFBS_SetLastError(1, err, Str);
    return err;
 }

 err = SeparateString(port.value, KS_VT_UINT_VEC, Liste);
 if(err) {
    Str = "\"%s %s\"  \"";
    Str += port.identifier;
    Str += "\" \"";
    Str += port.value;
    Str += "\"";
    iFBS_SetLastError(1, err, Str);
    return err;
 }

 if(port.len != Liste.size() ) {
    err = KS_ERR_BADINITPARAM;
    Str = "\"%s %s\"  \"";
    Str += port.identifier;
    Str += "\" \"";
    Str += port.value;
    Str += "\"";
    iFBS_SetLastError(1, err, Str);
    return err;
 }

 for(j=0; j < arr.size() ; j++ ) {
    Str = Liste.removeFirst();
    arr[j] = (unsigned int)atoi( (const char*)Str);
 }

 return KS_ERR_OK;

}

/*****************************************************************************/
KS_RESULT SingleToArray(ObjProps   &port,
                                                   PltArray<float>  &arr) {
/*****************************************************************************/

 PltList<PltString> Liste;
 unsigned int       j;
 PltString          Str("");
 KS_RESULT          err = 0;
 
 iFBS_SetLastError(1, err, Str);

 if(port.len == 0 ) {
    return KS_ERR_OK;
 }

 if(port.len != arr.size() ) {
    err = KS_ERR_BADINITPARAM;
    Str = "\"%s %s\"  \"";
    Str += port.identifier;
    Str += "\" \"";
    Str += port.value;
    Str += "\"";
    iFBS_SetLastError(1, err, Str);
    return err;
 }

 err = SeparateString(port.value, KS_VT_SINGLE_VEC, Liste);
 if(err) {
    Str = "\"%s %s\"  \"";
    Str += port.identifier;
    Str += "\" \"";
    Str += port.value;
    Str += "\"";
    iFBS_SetLastError(1, err, Str);
    return err;
 }

 if(port.len != Liste.size() ) {
    err = KS_ERR_BADINITPARAM;
    Str = "\"%s %s\"  \"";
    Str += port.identifier;
    Str += "\" \"";
    Str += port.value;
    Str += "\"";
    iFBS_SetLastError(1, err, Str);
    return err;
 }

 for(j=0; j < arr.size() ; j++ ) {
    Str = Liste.removeFirst();
    arr[j] = (float)atof( (const char*)Str);
 }

 return KS_ERR_OK;

}

/*****************************************************************************/
KS_RESULT DoubleToArray(ObjProps   &port,
                                                   PltArray<double> &arr) {
/*****************************************************************************/

 PltList<PltString> Liste;
 unsigned int       j;
 PltString          Str("");
 KS_RESULT          err = 0;
 
 iFBS_SetLastError(1, err, Str);

 if(port.len == 0 ) {
    return KS_ERR_OK;
 }
 if(port.len != arr.size() ) {
    err = KS_ERR_BADINITPARAM;
    Str = "\"%s %s\"  \"";
    Str += port.identifier;
    Str += "\" \"";
    Str += port.value;
    Str += "\"";
    iFBS_SetLastError(1, err, Str);
    return err;
 }

 err = SeparateString(port.value, KS_VT_DOUBLE_VEC, Liste);
 if(err) {
    Str = "\"%s %s\"  \"";
    Str += port.identifier;
    Str += "\" \"";
    Str += port.value;
    Str += "\"";
    iFBS_SetLastError(1, err, Str);
    return err;
 }

 if(port.len != Liste.size() ) {
    err = KS_ERR_BADINITPARAM;
    Str = "\"%s %s\"  \"";
    Str += port.identifier;
    Str += "\" \"";
    Str += port.value;
    Str += "\"";
    iFBS_SetLastError(1, err, Str);
    return err;
 }

 for(j=0; j < arr.size() ; j++ ) {
    Str = Liste.removeFirst();
    arr[j] = (double)atof( (const char*)Str);
 }

 return KS_ERR_OK;

}

/*****************************************************************************/
KS_RESULT TimeSpanToArray(ObjProps   &port,
                                                     PltArray<float> &arr) {
/*****************************************************************************/

 PltList<PltString> Liste;
 unsigned int       j;
 PltString          Str("");
 KS_RESULT          err = 0;
 
 iFBS_SetLastError(1, err, Str);

 if(port.len == 0 ) {
    return KS_ERR_OK;
 }
 if(port.len != arr.size() ) {
    err = KS_ERR_BADINITPARAM;
    Str = "\"%s %s\"  \"";
    Str += port.identifier;
    Str += "\" \"";
    Str += port.value;
    Str += "\"";
    iFBS_SetLastError(1, err, Str);
    return err;
 }

 err = SeparateString(port.value, KS_VT_SINGLE_VEC, Liste);
 if(err) {
    Str = "\"%s %s\"  \"";
    Str += port.identifier;
    Str += "\" \"";
    Str += port.value;
    Str += "\"";
    iFBS_SetLastError(1, err, Str);
    return err;
 }

 if(port.len != Liste.size() ) {
    err = KS_ERR_BADINITPARAM;
    Str = "\"%s %s\"  \"";
    Str += port.identifier;
    Str += "\" \"";
    Str += port.value;
    Str += "\"";
    iFBS_SetLastError(1, err, Str);
    return err;
 }

 for(j=0; j < arr.size() ; j++ ) {
    Str = Liste.removeFirst();
    arr[j] = (float)atof( (const char*)Str);
 }

 return KS_ERR_OK;

}

/*****************************************************************************/
KS_RESULT StringToArray(ObjProps   &port,
                                                   PltArray<PltString> &arr) {
/*****************************************************************************/

 PltList<PltString> Liste;
 unsigned int       j;
 PltString          Str("");
 KS_RESULT          err = 0;
 
 iFBS_SetLastError(1, err, Str);

 if(port.len == 0 ) {
    return KS_ERR_OK;
 }
 if(port.len != arr.size() ) {
    err = KS_ERR_BADINITPARAM;
    Str = "\"%s\"  \"";
    Str += port.identifier;
    Str += "\"";
    iFBS_SetLastError(1, err, Str);
    return err;
 }

 err = SeparateString(port.value, KS_VT_STRING_VEC, Liste);
 if(err) {
    Str = "\"%s\"  \"";
    Str += port.identifier;
    Str += "\"";
    iFBS_SetLastError(1, err, Str);
    return err;
 }

 if(port.len != Liste.size() ) {
    err = KS_ERR_BADINITPARAM;
    Str = "\"%s\"  \"";
    Str += port.identifier;
    Str += "\"";
    iFBS_SetLastError(1, err, Str);
    return err;
 }

 for(j=0; j < arr.size() ; j++ ) {
    arr[j] = Liste.removeFirst();
 }

 return KS_ERR_OK;

}