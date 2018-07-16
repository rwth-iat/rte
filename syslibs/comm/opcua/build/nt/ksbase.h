/*
*   This file was generated by the ACPLT/OV C-Code Generator.
*
*   C-Code Generator version: 1.6.4
*   ACPLT/OV library version: 2.0.0
*   Date of file generation:  12-Jul-2018 (15:40:52)
*
*   DO NOT EDIT THIS FILE!
*/

#ifndef OV_LIBRARY_ksbase_H_INCLUDED
#define OV_LIBRARY_ksbase_H_INCLUDED

#include "libov/ov_library.h"
#include "libov/ov_structure.h"
#include "libov/ov_class.h"
#include "libov/ov_association.h"
#include "libov/ov_variable.h"
#include "libov/ov_part.h"
#include "libov/ov_operation.h"
#include "ov.h"

#include "ksbase.ovt"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef OV_COMPILE_LIBRARY_ksbase
#if OV_COMPILER_BORLAND
#define OV_VAREXTERN extern
#else
#define OV_VAREXTERN OV_DLLVAREXPORT
#endif
#define OV_FNCEXTERN OV_DLLFNCEXPORT
#else
#define OV_VAREXTERN OV_DLLVARIMPORT
#define OV_FNCEXTERN OV_DLLVARIMPORT
#endif

#define OV_PPT_ksbase_AssocComTaskList struct OV_INST_ksbase_RootComTask*
#define OV_CPT_ksbase_AssocComTaskList struct OV_INST_ksbase_ComTask*
#define OV_PCI_ksbase_AssocComTaskList is_of_class_ksbase_RootComTask
#define OV_CCI_ksbase_AssocComTaskList is_of_class_ksbase_ComTask

#define OV_PPT_ksbase_AssocChannelClientHandler struct OV_INST_ksbase_Channel*
#define OV_CPT_ksbase_AssocChannelClientHandler struct OV_INST_ksbase_ClientHandler*
#define OV_PCI_ksbase_AssocChannelClientHandler is_of_class_ksbase_Channel
#define OV_CCI_ksbase_AssocChannelClientHandler is_of_class_ksbase_ClientHandler

#define OV_PPT_ksbase_AssocChannelDataHandler struct OV_INST_ksbase_Channel*
#define OV_CPT_ksbase_AssocChannelDataHandler struct OV_INST_ksbase_DataHandler*
#define OV_PCI_ksbase_AssocChannelDataHandler is_of_class_ksbase_Channel
#define OV_CCI_ksbase_AssocChannelDataHandler is_of_class_ksbase_DataHandler

#define OV_PPT_ksbase_AssocCurrentChannel struct OV_INST_ksbase_RootComTask*
#define OV_CPT_ksbase_AssocCurrentChannel struct OV_INST_ksbase_Channel*
#define OV_PCI_ksbase_AssocCurrentChannel is_of_class_ksbase_RootComTask
#define OV_CCI_ksbase_AssocCurrentChannel is_of_class_ksbase_Channel

#define OV_PPT_ksbase_AssocClientToTicketGenerator struct OV_INST_ksbase_ClientBase*
#define OV_CPT_ksbase_AssocClientToTicketGenerator struct OV_INST_ksbase_ClientTicketGenerator*
#define OV_PCI_ksbase_AssocClientToTicketGenerator is_of_class_ksbase_ClientBase
#define OV_CCI_ksbase_AssocClientToTicketGenerator is_of_class_ksbase_ClientTicketGenerator

#define OV_BIPT_ksbase_RootComTask OV_INSTPTR_ov_domain
#define OV_BVPT_ksbase_RootComTask OV_VTBLPTR_ov_domain

#define OV_INSTBODY_ksbase_RootComTask \
    OV_INSTBODY_ov_domain \
    OV_INT v_cycsecs; \
    OV_INT v_cycusecs;

#define OV_CIB_ksbase_RootComTask \
    OV_CIB_ov_domain \
    int is_of_class_ksbase_RootComTask:1;

#define OV_STATICINSTBODY_ksbase_RootComTask

#define OV_BIPT_ksbase_ComTask OV_INSTPTR_ov_domain
#define OV_BVPT_ksbase_ComTask OV_VTBLPTR_ov_domain

#define OV_INSTBODY_ksbase_ComTask \
    OV_INSTBODY_ov_domain \
    OV_INT v_actimode; \
    OV_INT v_cycInterval; \
    OV_TIME v_NextExecTime;

#define OV_CIB_ksbase_ComTask \
    OV_CIB_ov_domain \
    int is_of_class_ksbase_ComTask:1;

#define OV_STATICINSTBODY_ksbase_ComTask

#define OV_BIPT_ksbase_ClientHandler OV_INSTPTR_ksbase_ComTask
#define OV_BVPT_ksbase_ClientHandler OV_VTBLPTR_ksbase_ComTask

#define OV_INSTBODY_ksbase_ClientHandler \
    OV_INSTBODY_ksbase_ComTask \
    OV_STRING v_sourceAdr; \
    OV_INT v_receivedCalls;

#define OV_CIB_ksbase_ClientHandler \
    OV_CIB_ksbase_ComTask \
    int is_of_class_ksbase_ClientHandler:1;

#define OV_STATICINSTBODY_ksbase_ClientHandler

#define OV_BIPT_ksbase_baseClientHandlerExtension OV_INSTPTR_ksbase_ClientHandler
#define OV_BVPT_ksbase_baseClientHandlerExtension OV_VTBLPTR_ksbase_ClientHandler

#define OV_INSTBODY_ksbase_baseClientHandlerExtension \
    OV_INSTBODY_ksbase_ClientHandler

#define OV_CIB_ksbase_baseClientHandlerExtension \
    OV_CIB_ksbase_ClientHandler \
    int is_of_class_ksbase_baseClientHandlerExtension:1;

#define OV_STATICINSTBODY_ksbase_baseClientHandlerExtension

#define OV_BIPT_ksbase_DataHandler OV_INSTPTR_ksbase_ComTask
#define OV_BVPT_ksbase_DataHandler OV_VTBLPTR_ksbase_ComTask

#define OV_INSTBODY_ksbase_DataHandler \
    OV_INSTBODY_ksbase_ComTask

#define OV_CIB_ksbase_DataHandler \
    OV_CIB_ksbase_ComTask \
    int is_of_class_ksbase_DataHandler:1;

#define OV_STATICINSTBODY_ksbase_DataHandler

#define OV_BIPT_ksbase_ServerRep OV_INSTPTR_ksbase_ComTask
#define OV_BVPT_ksbase_ServerRep OV_VTBLPTR_ksbase_ComTask

#define OV_INSTBODY_ksbase_ServerRep \
    OV_INSTBODY_ksbase_ComTask \
    OV_STRING v_name; \
    OV_INT v_version; \
    OV_STRING_VEC v_protocols; \
    OV_STRING_VEC v_port; \
    OV_UINT v_timetolive; \
    OV_TIME v_expirationtime; \
    OV_TIME v_regtime; \
    OV_INT v_state;

#define OV_CIB_ksbase_ServerRep \
    OV_CIB_ksbase_ComTask \
    int is_of_class_ksbase_ServerRep:1;

#define OV_STATICINSTBODY_ksbase_ServerRep

#define OV_BIPT_ksbase_Manager OV_INSTPTR_ov_domain
#define OV_BVPT_ksbase_Manager OV_VTBLPTR_ov_domain

#define OV_INSTBODY_ksbase_Manager \
    OV_INSTBODY_ov_domain

#define OV_CIB_ksbase_Manager \
    OV_CIB_ov_domain \
    int is_of_class_ksbase_Manager:1;

#define OV_STATICINSTBODY_ksbase_Manager

#define OV_BIPT_ksbase_ProtocolIdentificator OV_INSTPTR_ov_domain
#define OV_BVPT_ksbase_ProtocolIdentificator OV_VTBLPTR_ov_domain

#define OV_INSTBODY_ksbase_ProtocolIdentificator \
    OV_INSTBODY_ov_domain \
    OV_STRING v_protocolID;

#define OV_CIB_ksbase_ProtocolIdentificator \
    OV_CIB_ov_domain \
    int is_of_class_ksbase_ProtocolIdentificator:1;

#define OV_STATICINSTBODY_ksbase_ProtocolIdentificator

#define OV_BIPT_ksbase_Channel OV_INSTPTR_ksbase_ComTask
#define OV_BVPT_ksbase_Channel OV_VTBLPTR_ksbase_ComTask

#define OV_INSTBODY_ksbase_Channel \
    OV_INSTBODY_ksbase_ComTask \
    OV_UINT v_ConnectionState; \
    OV_STRING v_address; \
    KS_DATAPACKET v_outData; \
    KS_DATAPACKET v_inData; \
    OV_UINT v_ClientHandlerAssociated; \
    OV_BOOL v_isLocal; \
    OV_BOOL v_usesStreamProtocol; \
    OV_TIME v_LastReceiveTime; \
    OV_UINT v_ConnectionTimeOut; \
    OV_UINT v_UnusedDataTimeOut; \
    OV_BOOL v_CloseAfterSend;

#define OV_CIB_ksbase_Channel \
    OV_CIB_ksbase_ComTask \
    int is_of_class_ksbase_Channel:1;

#define OV_STATICINSTBODY_ksbase_Channel

#define OV_BIPT_ksbase_TicketAuthenticator OV_INSTPTR_ov_domain
#define OV_BVPT_ksbase_TicketAuthenticator OV_VTBLPTR_ov_domain

#define OV_INSTBODY_ksbase_TicketAuthenticator \
    OV_INSTBODY_ov_domain \
    OV_TICKET_TYPE v_TicketType; \
    OV_TICKET v_ticket; \
    OV_ACCESS v_access; \
    OV_ACCESS v_TicketAccess;

#define OV_CIB_ksbase_TicketAuthenticator \
    OV_CIB_ov_domain \
    int is_of_class_ksbase_TicketAuthenticator:1;

#define OV_STATICINSTBODY_ksbase_TicketAuthenticator

#define OV_BIPT_ksbase_NoneTicketAuthenticator OV_INSTPTR_ksbase_TicketAuthenticator
#define OV_BVPT_ksbase_NoneTicketAuthenticator OV_VTBLPTR_ksbase_TicketAuthenticator

#define OV_INSTBODY_ksbase_NoneTicketAuthenticator \
    OV_INSTBODY_ksbase_TicketAuthenticator

#define OV_CIB_ksbase_NoneTicketAuthenticator \
    OV_CIB_ksbase_TicketAuthenticator \
    int is_of_class_ksbase_NoneTicketAuthenticator:1;

#define OV_STATICINSTBODY_ksbase_NoneTicketAuthenticator

#define OV_BIPT_ksbase_ClientBase OV_INSTPTR_ksbase_DataHandler
#define OV_BVPT_ksbase_ClientBase OV_VTBLPTR_ksbase_DataHandler

#define OV_INSTBODY_ksbase_ClientBase \
    OV_INSTBODY_ksbase_DataHandler \
    OV_STRING v_serverHost; \
    OV_STRING v_serverName; \
    OV_STRING v_serverPort; \
    OV_UINT v_state; \
    OV_UINT v_runningKSservice; \
    KSBASE_CALLBACKSTRUCT v_callback; \
    OV_BOOL v_holdConnection;

#define OV_CIB_ksbase_ClientBase \
    OV_CIB_ksbase_DataHandler \
    int is_of_class_ksbase_ClientBase:1;

#define OV_STATICINSTBODY_ksbase_ClientBase

#define OV_BIPT_ksbase_ClientTicketGenerator OV_INSTPTR_ov_domain
#define OV_BVPT_ksbase_ClientTicketGenerator OV_VTBLPTR_ov_domain

#define OV_INSTBODY_ksbase_ClientTicketGenerator \
    OV_INSTBODY_ov_domain \
    OV_TICKET_TYPE v_TicketType;

#define OV_CIB_ksbase_ClientTicketGenerator \
    OV_CIB_ov_domain \
    int is_of_class_ksbase_ClientTicketGenerator:1;

#define OV_STATICINSTBODY_ksbase_ClientTicketGenerator

#define OV_BIPT_ksbase_NoneTicketGenerator OV_INSTPTR_ksbase_ClientTicketGenerator
#define OV_BVPT_ksbase_NoneTicketGenerator OV_VTBLPTR_ksbase_ClientTicketGenerator

#define OV_INSTBODY_ksbase_NoneTicketGenerator \
    OV_INSTBODY_ksbase_ClientTicketGenerator

#define OV_CIB_ksbase_NoneTicketGenerator \
    OV_CIB_ksbase_ClientTicketGenerator \
    int is_of_class_ksbase_NoneTicketGenerator:1;

#define OV_STATICINSTBODY_ksbase_NoneTicketGenerator

OV_TYPEDEF_INSTANCE(ksbase_RootComTask);
OV_TYPEDEF_INSTANCE(ksbase_ComTask);
OV_TYPEDEF_INSTANCE(ksbase_ClientHandler);
OV_TYPEDEF_INSTANCE(ksbase_baseClientHandlerExtension);
OV_TYPEDEF_INSTANCE(ksbase_DataHandler);
OV_TYPEDEF_INSTANCE(ksbase_ServerRep);
OV_TYPEDEF_INSTANCE(ksbase_Manager);
OV_TYPEDEF_INSTANCE(ksbase_ProtocolIdentificator);
OV_TYPEDEF_INSTANCE(ksbase_Channel);
OV_TYPEDEF_INSTANCE(ksbase_TicketAuthenticator);
OV_TYPEDEF_INSTANCE(ksbase_NoneTicketAuthenticator);
OV_TYPEDEF_INSTANCE(ksbase_ClientBase);
OV_TYPEDEF_INSTANCE(ksbase_ClientTicketGenerator);
OV_TYPEDEF_INSTANCE(ksbase_NoneTicketGenerator);

#include "ksbase.ovf"

OV_FNCEXTERN OV_FNC_CONSTRUCTOR ksbase_RootComTask_constructor;
OV_FNCEXTERN OV_FNC_STARTUP ksbase_RootComTask_startup;
OV_FNCEXTERN OV_FNC_SHUTDOWN ksbase_RootComTask_shutdown;
OV_FNCEXTERN OV_FNC_GETACCESS ksbase_RootComTask_getaccess;

OV_FNCEXTERN OV_FNC_CONSTRUCTOR ksbase_ComTask_constructor;
OV_FNCEXTERN OV_FNC_DESTRUCTOR ksbase_ComTask_destructor;
OV_FNCEXTERN KSBASE_FNC_CALCEXEC ksbase_ComTask_calcExec;
OV_FNCEXTERN KSBASE_FNC_TYPEMETHOD ksbase_ComTask_typemethod;
OV_FNCEXTERN OV_FNC_GETACCESS ksbase_ComTask_getaccess;

OV_FNCEXTERN OV_FNC_STARTUP ksbase_ClientHandler_startup;
OV_FNCEXTERN OV_FNC_SHUTDOWN ksbase_ClientHandler_shutdown;
OV_FNCEXTERN KSBASE_FNC_HANDLEREQUEST ksbase_ClientHandler_HandleRequest;

OV_FNCEXTERN KSBASE_FNC_HANDLEDATA ksbase_DataHandler_HandleData;

OV_FNCEXTERN OV_FNC_CONSTRUCTOR ksbase_ServerRep_constructor;
OV_FNCEXTERN OV_FNC_STARTUP ksbase_ServerRep_startup;
OV_FNCEXTERN OV_FNC_SHUTDOWN ksbase_ServerRep_shutdown;
OV_FNCEXTERN KSBASE_FNC_TYPEMETHOD ksbase_ServerRep_typemethod;

OV_FNCEXTERN OV_FNC_SHUTDOWN ksbase_Manager_shutdown;
OV_FNCEXTERN KSBASE_FNC_REGISTER ksbase_Manager_register;
OV_FNCEXTERN KSBASE_FNC_UNREGISTER ksbase_Manager_unregister;
OV_FNCEXTERN KSBASE_FNC_GETSERVERDATA ksbase_Manager_getserverdata;
OV_FNCEXTERN OV_FNC_GETACCESS ksbase_Manager_getaccess;

OV_FNCEXTERN KSBASE_FNC_IDENTIFY ksbase_ProtocolIdentificator_identify;
OV_FNCEXTERN KSBASE_FNC_CREATECH ksbase_ProtocolIdentificator_createClientHandler;
OV_FNCEXTERN OV_FNC_GETACCESS ksbase_ProtocolIdentificator_getaccess;

OV_FNCEXTERN KSBASE_FNC_SENDDATA ksbase_Channel_SendData;
OV_FNCEXTERN KSBASE_FNC_OPENCONN ksbase_Channel_OpenConnection;
OV_FNCEXTERN KSBASE_FNC_OPENLOCAL ksbase_Channel_OpenLocalConn;
OV_FNCEXTERN KSBASE_FNC_CLOSECONN ksbase_Channel_CloseConnection;
OV_FNCEXTERN OV_FNC_STARTUP ksbase_Channel_startup;
OV_FNCEXTERN OV_FNC_SHUTDOWN ksbase_Channel_shutdown;

OV_FNCEXTERN OV_FNC_CONSTRUCTOR ksbase_TicketAuthenticator_constructor;
OV_FNCEXTERN OV_FNC_STARTUP ksbase_TicketAuthenticator_startup;
OV_FNCEXTERN OV_FNC_SHUTDOWN ksbase_TicketAuthenticator_shutdown;
OV_FNCEXTERN KSBASE_FNC_CREATETICKET ksbase_TicketAuthenticator_createticket;
OV_FNCEXTERN KSBASE_FNC_DELETETICKET ksbase_TicketAuthenticator_deleteticket;
OV_FNCEXTERN KSBASE_FNC_ENCODEREPLY ksbase_TicketAuthenticator_encodereply;
OV_FNCEXTERN KSBASE_FNC_GETACCESS ksbase_TicketAuthenticator_TicketGetaccess;

OV_FNCEXTERN OV_FNC_CONSTRUCTOR ksbase_NoneTicketAuthenticator_constructor;
OV_FNCEXTERN OV_FNC_STARTUP ksbase_NoneTicketAuthenticator_startup;
OV_FNCEXTERN KSBASE_FNC_CREATETICKET ksbase_NoneTicketAuthenticator_createticket;
OV_FNCEXTERN KSBASE_FNC_DELETETICKET ksbase_NoneTicketAuthenticator_deleteticket;
OV_FNCEXTERN KSBASE_FNC_ENCODEREPLY ksbase_NoneTicketAuthenticator_encodereply;
OV_FNCEXTERN KSBASE_FNC_GETACCESS ksbase_NoneTicketAuthenticator_TicketGetaccess;

OV_FNCEXTERN KSBASE_FNC_RESET ksbase_ClientBase_reset;
OV_FNCEXTERN KSBASE_FNC_REQREG ksbase_ClientBase_requestRegister;
OV_FNCEXTERN KSBASE_FNC_PROCREG ksbase_ClientBase_processRegister;
OV_FNCEXTERN KSBASE_FNC_REQUNREG ksbase_ClientBase_requestUnRegister;
OV_FNCEXTERN KSBASE_FNC_PROCUNREG ksbase_ClientBase_processUnRegister;
OV_FNCEXTERN KSBASE_FNC_REQGETSRV ksbase_ClientBase_requestGetServer;
OV_FNCEXTERN KSBASE_FNC_PROCGETSRV ksbase_ClientBase_processGetServer;
OV_FNCEXTERN KSBASE_FNC_REQGETPP ksbase_ClientBase_requestGetPP;
OV_FNCEXTERN KSBASE_FNC_PROCGETPP ksbase_ClientBase_processGetPP;
OV_FNCEXTERN KSBASE_FNC_REQGETEP ksbase_ClientBase_requestGetEP;
OV_FNCEXTERN KSBASE_FNC_PROCGETEP ksbase_ClientBase_processGetEP;
OV_FNCEXTERN KSBASE_FNC_REQGETVAR ksbase_ClientBase_requestGetVar;
OV_FNCEXTERN KSBASE_FNC_PROCGETVAR ksbase_ClientBase_processGetVar;
OV_FNCEXTERN KSBASE_FNC_REQSETVAR ksbase_ClientBase_requestSetVar;
OV_FNCEXTERN KSBASE_FNC_PROCSETVAR ksbase_ClientBase_processSetVar;
OV_FNCEXTERN KSBASE_FNC_REQGETCANONPATH ksbase_ClientBase_requestGetCanonicalPath;
OV_FNCEXTERN KSBASE_FNC_PROCGECANONPATH ksbase_ClientBase_processGetCanonicalPath;
OV_FNCEXTERN KSBASE_FNC_REQCREATEOBJ ksbase_ClientBase_requestCreateObject;
OV_FNCEXTERN KSBASE_FNC_PROCCREATEOBJ ksbase_ClientBase_processCreateObject;
OV_FNCEXTERN KSBASE_FNC_REQDELOBJ ksbase_ClientBase_requestDeleteObject;
OV_FNCEXTERN KSBASE_FNC_PROCDELOBJ ksbase_ClientBase_processDeleteObject;
OV_FNCEXTERN KSBASE_FNC_REQRENAMEOBJ ksbase_ClientBase_requestRenameObject;
OV_FNCEXTERN KSBASE_FNC_PROCRENAMEOBJ ksbase_ClientBase_processRenameObject;
OV_FNCEXTERN KSBASE_FNC_REQLINK ksbase_ClientBase_requestLink;
OV_FNCEXTERN KSBASE_FNC_PROCLINK ksbase_ClientBase_processLink;
OV_FNCEXTERN KSBASE_FNC_REQUNLINK ksbase_ClientBase_requestUnLink;
OV_FNCEXTERN KSBASE_FNC_PROCUNLINK ksbase_ClientBase_processUnLink;

OV_FNCEXTERN KSBASE_FNC_ENCODETICKET ksbase_ClientTicketGenerator_encodeTicket;
OV_FNCEXTERN KSBASE_FNC_DECODEREPLY ksbase_ClientTicketGenerator_decodeReply;

OV_FNCEXTERN OV_FNC_STARTUP ksbase_NoneTicketGenerator_startup;

#define OV_VTBLBODY_ksbase_RootComTask \
    OV_VTBLBODY_ov_domain

#define OV_VTBLBODY_ksbase_ComTask \
    OV_VTBLBODY_ov_domain \
    KSBASE_FNC_CALCEXEC* m_calcExec; \
    KSBASE_FNC_TYPEMETHOD* m_typemethod;

#define OV_VTBLBODY_ksbase_ClientHandler \
    OV_VTBLBODY_ksbase_ComTask \
    KSBASE_FNC_HANDLEREQUEST* m_HandleRequest;

#define OV_VTBLBODY_ksbase_baseClientHandlerExtension \
    OV_VTBLBODY_ksbase_ClientHandler

#define OV_VTBLBODY_ksbase_DataHandler \
    OV_VTBLBODY_ksbase_ComTask \
    KSBASE_FNC_HANDLEDATA* m_HandleData;

#define OV_VTBLBODY_ksbase_ServerRep \
    OV_VTBLBODY_ksbase_ComTask

#define OV_VTBLBODY_ksbase_Manager \
    OV_VTBLBODY_ov_domain \
    KSBASE_FNC_REGISTER* m_register; \
    KSBASE_FNC_UNREGISTER* m_unregister; \
    KSBASE_FNC_GETSERVERDATA* m_getserverdata;

#define OV_VTBLBODY_ksbase_ProtocolIdentificator \
    OV_VTBLBODY_ov_domain \
    KSBASE_FNC_IDENTIFY* m_identify; \
    KSBASE_FNC_CREATECH* m_createClientHandler;

#define OV_VTBLBODY_ksbase_Channel \
    OV_VTBLBODY_ksbase_ComTask \
    KSBASE_FNC_SENDDATA* m_SendData; \
    KSBASE_FNC_OPENCONN* m_OpenConnection; \
    KSBASE_FNC_OPENLOCAL* m_OpenLocalConn; \
    KSBASE_FNC_CLOSECONN* m_CloseConnection;

#define OV_VTBLBODY_ksbase_TicketAuthenticator \
    OV_VTBLBODY_ov_domain \
    KSBASE_FNC_CREATETICKET* m_createticket; \
    KSBASE_FNC_DELETETICKET* m_deleteticket; \
    KSBASE_FNC_ENCODEREPLY* m_encodereply; \
    KSBASE_FNC_GETACCESS* m_TicketGetaccess;

#define OV_VTBLBODY_ksbase_NoneTicketAuthenticator \
    OV_VTBLBODY_ksbase_TicketAuthenticator

#define OV_VTBLBODY_ksbase_ClientBase \
    OV_VTBLBODY_ksbase_DataHandler \
    KSBASE_FNC_RESET* m_reset; \
    KSBASE_FNC_REQREG* m_requestRegister; \
    KSBASE_FNC_PROCREG* m_processRegister; \
    KSBASE_FNC_REQUNREG* m_requestUnRegister; \
    KSBASE_FNC_PROCUNREG* m_processUnRegister; \
    KSBASE_FNC_REQGETSRV* m_requestGetServer; \
    KSBASE_FNC_PROCGETSRV* m_processGetServer; \
    KSBASE_FNC_REQGETPP* m_requestGetPP; \
    KSBASE_FNC_PROCGETPP* m_processGetPP; \
    KSBASE_FNC_REQGETEP* m_requestGetEP; \
    KSBASE_FNC_PROCGETEP* m_processGetEP; \
    KSBASE_FNC_REQGETVAR* m_requestGetVar; \
    KSBASE_FNC_PROCGETVAR* m_processGetVar; \
    KSBASE_FNC_REQSETVAR* m_requestSetVar; \
    KSBASE_FNC_PROCSETVAR* m_processSetVar; \
    KSBASE_FNC_REQGETCANONPATH* m_requestGetCanonicalPath; \
    KSBASE_FNC_PROCGECANONPATH* m_processGetCanonicalPath; \
    KSBASE_FNC_REQCREATEOBJ* m_requestCreateObject; \
    KSBASE_FNC_PROCCREATEOBJ* m_processCreateObject; \
    KSBASE_FNC_REQDELOBJ* m_requestDeleteObject; \
    KSBASE_FNC_PROCDELOBJ* m_processDeleteObject; \
    KSBASE_FNC_REQRENAMEOBJ* m_requestRenameObject; \
    KSBASE_FNC_PROCRENAMEOBJ* m_processRenameObject; \
    KSBASE_FNC_REQLINK* m_requestLink; \
    KSBASE_FNC_PROCLINK* m_processLink; \
    KSBASE_FNC_REQUNLINK* m_requestUnLink; \
    KSBASE_FNC_PROCUNLINK* m_processUnLink;

#define OV_VTBLBODY_ksbase_ClientTicketGenerator \
    OV_VTBLBODY_ov_domain \
    KSBASE_FNC_ENCODETICKET* m_encodeTicket; \
    KSBASE_FNC_DECODEREPLY* m_decodeReply;

#define OV_VTBLBODY_ksbase_NoneTicketGenerator \
    OV_VTBLBODY_ksbase_ClientTicketGenerator

OV_TYPEDEF_VTABLE(ksbase_RootComTask);
OV_TYPEDEF_VTABLE(ksbase_ComTask);
OV_TYPEDEF_VTABLE(ksbase_ClientHandler);
OV_TYPEDEF_VTABLE(ksbase_baseClientHandlerExtension);
OV_TYPEDEF_VTABLE(ksbase_DataHandler);
OV_TYPEDEF_VTABLE(ksbase_ServerRep);
OV_TYPEDEF_VTABLE(ksbase_Manager);
OV_TYPEDEF_VTABLE(ksbase_ProtocolIdentificator);
OV_TYPEDEF_VTABLE(ksbase_Channel);
OV_TYPEDEF_VTABLE(ksbase_TicketAuthenticator);
OV_TYPEDEF_VTABLE(ksbase_NoneTicketAuthenticator);
OV_TYPEDEF_VTABLE(ksbase_ClientBase);
OV_TYPEDEF_VTABLE(ksbase_ClientTicketGenerator);
OV_TYPEDEF_VTABLE(ksbase_NoneTicketGenerator);

OV_DLLFNCEXPORT OV_INT ksbase_RootComTask_cycsecs_get(
    OV_INSTPTR_ksbase_RootComTask pobj
);

OV_DLLFNCEXPORT OV_RESULT ksbase_RootComTask_cycsecs_set(
    OV_INSTPTR_ksbase_RootComTask pobj,
    const OV_INT value
);

OV_DLLFNCEXPORT OV_INT ksbase_RootComTask_cycusecs_get(
    OV_INSTPTR_ksbase_RootComTask pobj
);

OV_DLLFNCEXPORT OV_RESULT ksbase_RootComTask_cycusecs_set(
    OV_INSTPTR_ksbase_RootComTask pobj,
    const OV_INT value
);

OV_DLLFNCEXPORT OV_INT ksbase_ComTask_actimode_get(
    OV_INSTPTR_ksbase_ComTask pobj
);

OV_DLLFNCEXPORT OV_RESULT ksbase_ComTask_actimode_set(
    OV_INSTPTR_ksbase_ComTask pobj,
    const OV_INT value
);

OV_DLLFNCEXPORT OV_INT ksbase_ComTask_cycInterval_get(
    OV_INSTPTR_ksbase_ComTask pobj
);

OV_DLLFNCEXPORT OV_RESULT ksbase_ComTask_cycInterval_set(
    OV_INSTPTR_ksbase_ComTask pobj,
    const OV_INT value
);

OV_DLLFNCEXPORT OV_STRING ksbase_ServerRep_name_get(
    OV_INSTPTR_ksbase_ServerRep pobj
);

OV_DLLFNCEXPORT OV_RESULT ksbase_ServerRep_name_set(
    OV_INSTPTR_ksbase_ServerRep pobj,
    const OV_STRING value
);

OV_DLLFNCEXPORT OV_INT ksbase_ServerRep_version_get(
    OV_INSTPTR_ksbase_ServerRep pobj
);

OV_DLLFNCEXPORT OV_RESULT ksbase_ServerRep_version_set(
    OV_INSTPTR_ksbase_ServerRep pobj,
    const OV_INT value
);

OV_DLLFNCEXPORT OV_STRING *ksbase_ServerRep_protocols_get(
    OV_INSTPTR_ksbase_ServerRep pobj,
    OV_UINT *pveclen
);

OV_DLLFNCEXPORT OV_RESULT ksbase_ServerRep_protocols_set(
    OV_INSTPTR_ksbase_ServerRep pobj,
    const OV_STRING *pvalue,
    const OV_UINT veclen
);

OV_DLLFNCEXPORT OV_STRING *ksbase_ServerRep_port_get(
    OV_INSTPTR_ksbase_ServerRep pobj,
    OV_UINT *pveclen
);

OV_DLLFNCEXPORT OV_RESULT ksbase_ServerRep_port_set(
    OV_INSTPTR_ksbase_ServerRep pobj,
    const OV_STRING *pvalue,
    const OV_UINT veclen
);

OV_DLLFNCEXPORT OV_UINT ksbase_ServerRep_timetolive_get(
    OV_INSTPTR_ksbase_ServerRep pobj
);

OV_DLLFNCEXPORT OV_RESULT ksbase_ServerRep_timetolive_set(
    OV_INSTPTR_ksbase_ServerRep pobj,
    const OV_UINT value
);

OV_DLLFNCEXPORT OV_TIME *ksbase_ServerRep_expirationtime_get(
    OV_INSTPTR_ksbase_ServerRep pobj
);

OV_DLLFNCEXPORT OV_RESULT ksbase_ServerRep_expirationtime_set(
    OV_INSTPTR_ksbase_ServerRep pobj,
    const OV_TIME *pvalue
);

OV_DLLFNCEXPORT OV_TIME *ksbase_ServerRep_regtime_get(
    OV_INSTPTR_ksbase_ServerRep pobj
);

OV_DLLFNCEXPORT OV_RESULT ksbase_ServerRep_regtime_set(
    OV_INSTPTR_ksbase_ServerRep pobj,
    const OV_TIME *pvalue
);

OV_DLLFNCEXPORT OV_INT ksbase_ServerRep_state_get(
    OV_INSTPTR_ksbase_ServerRep pobj
);

OV_DLLFNCEXPORT OV_RESULT ksbase_ServerRep_state_set(
    OV_INSTPTR_ksbase_ServerRep pobj,
    const OV_INT value
);

OV_DLLFNCEXPORT OV_BOOL ksbase_TicketAuthenticator_ACRead_get(
    OV_INSTPTR_ksbase_TicketAuthenticator pobj
);

OV_DLLFNCEXPORT OV_RESULT ksbase_TicketAuthenticator_ACRead_set(
    OV_INSTPTR_ksbase_TicketAuthenticator pobj,
    const OV_BOOL value
);

OV_DLLFNCEXPORT OV_BOOL ksbase_TicketAuthenticator_ACWrite_get(
    OV_INSTPTR_ksbase_TicketAuthenticator pobj
);

OV_DLLFNCEXPORT OV_RESULT ksbase_TicketAuthenticator_ACWrite_set(
    OV_INSTPTR_ksbase_TicketAuthenticator pobj,
    const OV_BOOL value
);

OV_DLLFNCEXPORT OV_BOOL ksbase_TicketAuthenticator_ACInstantiate_get(
    OV_INSTPTR_ksbase_TicketAuthenticator pobj
);

OV_DLLFNCEXPORT OV_RESULT ksbase_TicketAuthenticator_ACInstantiate_set(
    OV_INSTPTR_ksbase_TicketAuthenticator pobj,
    const OV_BOOL value
);

OV_DLLFNCEXPORT OV_BOOL ksbase_TicketAuthenticator_ACDelete_get(
    OV_INSTPTR_ksbase_TicketAuthenticator pobj
);

OV_DLLFNCEXPORT OV_RESULT ksbase_TicketAuthenticator_ACDelete_set(
    OV_INSTPTR_ksbase_TicketAuthenticator pobj,
    const OV_BOOL value
);

OV_DLLFNCEXPORT OV_BOOL ksbase_TicketAuthenticator_ACRename_get(
    OV_INSTPTR_ksbase_TicketAuthenticator pobj
);

OV_DLLFNCEXPORT OV_RESULT ksbase_TicketAuthenticator_ACRename_set(
    OV_INSTPTR_ksbase_TicketAuthenticator pobj,
    const OV_BOOL value
);

OV_DLLFNCEXPORT OV_BOOL ksbase_TicketAuthenticator_ACLink_get(
    OV_INSTPTR_ksbase_TicketAuthenticator pobj
);

OV_DLLFNCEXPORT OV_RESULT ksbase_TicketAuthenticator_ACLink_set(
    OV_INSTPTR_ksbase_TicketAuthenticator pobj,
    const OV_BOOL value
);

OV_DLLFNCEXPORT OV_BOOL ksbase_TicketAuthenticator_ACUnlink_get(
    OV_INSTPTR_ksbase_TicketAuthenticator pobj
);

OV_DLLFNCEXPORT OV_RESULT ksbase_TicketAuthenticator_ACUnlink_set(
    OV_INSTPTR_ksbase_TicketAuthenticator pobj,
    const OV_BOOL value
);

OV_DLLFNCEXPORT OV_RESULT ksbase_ClientBase_serverHost_set(
    OV_INSTPTR_ksbase_ClientBase pobj,
    const OV_STRING value
);

OV_DLLFNCEXPORT OV_RESULT ksbase_ClientBase_serverName_set(
    OV_INSTPTR_ksbase_ClientBase pobj,
    const OV_STRING value
);

OV_DECL_LINK(ksbase_AssocComTaskList);
OV_DECL_UNLINK(ksbase_AssocComTaskList);
OV_DECL_GETACCESS(ksbase_AssocComTaskList);

OV_DECL_LINK(ksbase_AssocChannelClientHandler);
OV_DECL_UNLINK(ksbase_AssocChannelClientHandler);
OV_DECL_GETACCESS(ksbase_AssocChannelClientHandler);

OV_DECL_LINK(ksbase_AssocChannelDataHandler);
OV_DECL_UNLINK(ksbase_AssocChannelDataHandler);
OV_DECL_GETACCESS(ksbase_AssocChannelDataHandler);

OV_DECL_LINK(ksbase_AssocCurrentChannel);
OV_DECL_UNLINK(ksbase_AssocCurrentChannel);
OV_DECL_GETACCESS(ksbase_AssocCurrentChannel);

OV_DECL_LINK(ksbase_AssocClientToTicketGenerator);
OV_DECL_UNLINK(ksbase_AssocClientToTicketGenerator);
OV_DECL_GETACCESS(ksbase_AssocClientToTicketGenerator);

extern OV_CLASS_DEF OV_CLASS_DEF_ksbase_RootComTask;
OV_VAREXTERN OV_INSTPTR_ov_class pclass_ksbase_RootComTask;

extern OV_CLASS_DEF OV_CLASS_DEF_ksbase_ComTask;
OV_VAREXTERN OV_INSTPTR_ov_class pclass_ksbase_ComTask;

extern OV_CLASS_DEF OV_CLASS_DEF_ksbase_ClientHandler;
OV_VAREXTERN OV_INSTPTR_ov_class pclass_ksbase_ClientHandler;

extern OV_CLASS_DEF OV_CLASS_DEF_ksbase_baseClientHandlerExtension;
OV_VAREXTERN OV_INSTPTR_ov_class pclass_ksbase_baseClientHandlerExtension;

extern OV_CLASS_DEF OV_CLASS_DEF_ksbase_DataHandler;
OV_VAREXTERN OV_INSTPTR_ov_class pclass_ksbase_DataHandler;

extern OV_CLASS_DEF OV_CLASS_DEF_ksbase_ServerRep;
OV_VAREXTERN OV_INSTPTR_ov_class pclass_ksbase_ServerRep;

extern OV_CLASS_DEF OV_CLASS_DEF_ksbase_Manager;
OV_VAREXTERN OV_INSTPTR_ov_class pclass_ksbase_Manager;

extern OV_CLASS_DEF OV_CLASS_DEF_ksbase_ProtocolIdentificator;
OV_VAREXTERN OV_INSTPTR_ov_class pclass_ksbase_ProtocolIdentificator;

extern OV_CLASS_DEF OV_CLASS_DEF_ksbase_Channel;
OV_VAREXTERN OV_INSTPTR_ov_class pclass_ksbase_Channel;

extern OV_CLASS_DEF OV_CLASS_DEF_ksbase_TicketAuthenticator;
OV_VAREXTERN OV_INSTPTR_ov_class pclass_ksbase_TicketAuthenticator;

extern OV_CLASS_DEF OV_CLASS_DEF_ksbase_NoneTicketAuthenticator;
OV_VAREXTERN OV_INSTPTR_ov_class pclass_ksbase_NoneTicketAuthenticator;

extern OV_CLASS_DEF OV_CLASS_DEF_ksbase_ClientBase;
OV_VAREXTERN OV_INSTPTR_ov_class pclass_ksbase_ClientBase;

extern OV_CLASS_DEF OV_CLASS_DEF_ksbase_ClientTicketGenerator;
OV_VAREXTERN OV_INSTPTR_ov_class pclass_ksbase_ClientTicketGenerator;

extern OV_CLASS_DEF OV_CLASS_DEF_ksbase_NoneTicketGenerator;
OV_VAREXTERN OV_INSTPTR_ov_class pclass_ksbase_NoneTicketGenerator;

extern OV_ASSOCIATION_DEF OV_ASSOCIATION_DEF_ksbase_AssocComTaskList;
OV_VAREXTERN OV_INSTPTR_ov_association passoc_ksbase_AssocComTaskList;

extern OV_ASSOCIATION_DEF OV_ASSOCIATION_DEF_ksbase_AssocChannelClientHandler;
OV_VAREXTERN OV_INSTPTR_ov_association passoc_ksbase_AssocChannelClientHandler;

extern OV_ASSOCIATION_DEF OV_ASSOCIATION_DEF_ksbase_AssocChannelDataHandler;
OV_VAREXTERN OV_INSTPTR_ov_association passoc_ksbase_AssocChannelDataHandler;

extern OV_ASSOCIATION_DEF OV_ASSOCIATION_DEF_ksbase_AssocCurrentChannel;
OV_VAREXTERN OV_INSTPTR_ov_association passoc_ksbase_AssocCurrentChannel;

extern OV_ASSOCIATION_DEF OV_ASSOCIATION_DEF_ksbase_AssocClientToTicketGenerator;
OV_VAREXTERN OV_INSTPTR_ov_association passoc_ksbase_AssocClientToTicketGenerator;

extern OV_LIBRARY_DEF OV_LIBRARY_DEF_ksbase;

OV_RESULT ov_library_setglobalvars_ksbase(void);

OV_DLLFNCEXPORT OV_LIBRARY_DEF *ov_library_open_ksbase(void);

#undef OV_VAREXTERN
#undef OV_FNCEXTERN

#ifdef __cplusplus
}
#endif

#endif

/*
*   End of file
*/
