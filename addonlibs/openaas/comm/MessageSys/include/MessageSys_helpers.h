
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
#define KSMSG_IDENTIFIER	"KSMSG"


OV_DLLFNCEXPORT OV_RESULT MessageSys_createAnonymousObject(OV_INSTPTR_ov_class pClass, OV_INSTPTR_ov_domain pParent, OV_STRING identifier, OV_INSTPTR_ov_object* pObj);

OV_DLLFNCEXPORT OV_RESULT MessageSys_createAnonymousMessage(OV_INSTPTR_ov_domain pParent, OV_STRING identifier, OV_INSTPTR_ov_object* pObj);

OV_DLLFNCEXPORT OV_RESULT MessageSys_parseAndDeliverMsg(const OV_STRING	value, OV_INSTPTR_MessageSys_Message* createdMsg, OV_INSTPTR_ov_domain* msgCreatedIn);

