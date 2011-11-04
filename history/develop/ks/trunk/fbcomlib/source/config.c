/** @file 
* This file holds general definitions of strings for the fbconlib
*/

#include "libov/ov_string.h"

#include "config.h"

//0: processing
//1: everything ok
//2: wrong input
//3: No return method set
//4: OV-error
//5: connection error
//6: service error
//7: wrong variable type
//8: client busy
//9: no memory to store answer

const OV_STRING STATESTRING_INIT = "processing";
const OV_INT STATE_INIT = 0;

const OV_STRING STATESTRING_OK = "everything ok";
const OV_INT STATE_OK = 1;

const OV_STRING STATESTRING_WRONGINPUT = "wrong input - please set all values";
const OV_INT STATE_WRONGINPUT = 2;

const OV_STRING STATESTRING_NORETMETHOD = "No return method set";
const OV_INT STATE_NORETMETHOD = 3;

const OV_STRING STATESTRING_OVERROR = "OV-error (cant create ksapi obj))";
const OV_INT STATE_OVERROR = 4;

const OV_STRING STATESTRING_CONNECTIONERROR = "connection error";
const OV_INT STATE_CONNECTIONERROR = 5;

const OV_STRING STATESTRING_SERVICEERROR = "wrong variable type";
const OV_INT STATE_SERVICEERROR = 6;

const OV_STRING STATESTRING_WRONGVARONDESTINATION = "client busy";
const OV_INT STATE_WRONGVARONDESTINATION = 7;

const OV_STRING STATESTRING_CLIENTBUSY = "client busy";
const OV_INT STATE_CLIENTBUSY = 8;

const OV_STRING STATESTRING_NOMEMORY = "no memory to store answer";
const OV_INT STATE_NOMEMORY = 9;

const OV_STRING STATESTRING_UNKNOWNERROR = "Unknown state";
