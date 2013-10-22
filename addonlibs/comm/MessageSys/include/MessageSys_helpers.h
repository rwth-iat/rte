
#ifndef OV_COMPILE_LIBRARY_MessageSys
#define OV_COMPILE_LIBRARY_MessageSys
#endif


//MSGSYS: sending states of messages
#define MSGNEW 0
#define MSGREADYFORSENDING 1
#define MSGWAITING 2
#define MSGDONE 3
#define MSGRECEIVERERROR 4
#define MSGFATALERROR 5
//MSGSYS: receiving states of messages
#define MSGNEWARRIVED 10
#define MSGDONERECEIVE 11

#define COMPATH "/communication"
#define MSGDELIVERY "/communication/MessageSys"
#define SENDINGINSTANCE "/communication/MessageSys/sendingInstance"
#define REGISTEREDPATH "/communication/ServiceProvider/SystemServices"


OV_DLLFNCEXPORT OV_RESULT MessageSys_createAnonymousObject(OV_INSTPTR_ov_class pClass, OV_INSTPTR_ov_domain pParent, OV_STRING identifier, OV_INSTPTR_ov_object* pObj);

OV_DLLFNCEXPORT OV_RESULT MessageSys_createAnonymousMessage(OV_INSTPTR_ov_domain pParent, OV_STRING identifier, OV_INSTPTR_ov_object* pObj);

/*
 * splits a message by the delimiter "delimiter" returns the necht char after it in position and a copy of the split off string in token
 * you MUST call ov_memstack_lock() / ov_memstack_unlock()
 */
OV_BOOL MSG_split(OV_STRING victim, OV_STRING delimiter, OV_INT *position, OV_STRING *token);
