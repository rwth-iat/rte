
#ifndef ksbase_helper_INCLUDED
#define ksbase_helper_INCLUDED

#include "libov/ov_ov.h"
#include "ksbase.h"

/**
 * splits up a path string into its components
 * a path string may contain:
 *	[//host[:MANAGERPort]/servername[:serverport]/]instancePath
 *	examples are
 *	//dev:7509/MANAGER:7509/vendor/server_description
 *	//dev/MANAGER:7509/vendor/server_description
 *	//dev:7509/MANAGER/vendor/server_description
 *	//dev/MANAGER/vendor/server_description
 *	call ov_memstack_lock() / _unlock() around it
 *	when the result pointers are NULL they are not filled, no error is returned
 *	however, managerport and serverport are only set, if the pointers host and serverName,
 *	respectively, are set
 */
OV_DLLFNCEXPORT OV_RESULT ks_splitOneStringPath(
		const OV_STRING path,
		OV_STRING* host,
		OV_STRING* managerPort,
		OV_STRING* serverName,
		OV_STRING* serverPort,
		OV_STRING* instancePath
);

OV_DLLFNCEXPORT OV_BOOL ks_isvalidname(OV_STRING name);

OV_DLLFNCEXPORT void ksbase_free_KSDATAPACKET(KS_DATAPACKET* packet);

OV_DLLFNCEXPORT OV_RESULT ksbase_KSDATAPACKET_append(KS_DATAPACKET* packet, OV_BYTE* data, OV_UINT addlength);

OV_DLLFNCEXPORT OV_RESULT ksbase_KSDATAPACKET_set(KS_DATAPACKET* packet, OV_BYTE* data, OV_UINT addlength);

/* ----------------------------------------------------------------------------
 * ACPLT/KS error codes defined by the ACPLT/KS core protocol...
 */
#define KS_ERR_OK              OV_ERR_OK
#define KS_ERR_GENERIC         OV_ERR_GENERIC
#define KS_ERR_TARGETGENERIC   OV_ERR_TARGETGENERIC
#define KS_ERR_BADAUTH         OV_ERR_BADAUTH
#define KS_ERR_UNKNOWNAUTH     OV_ERR_UNKNOWNAUTH
#define KS_ERR_NOTIMPLEMENTED  OV_ERR_NOTIMPLEMENTED
#define KS_ERR_BADPARAM        OV_ERR_BADPARAM
#define KS_ERR_BADOBJTYPE      OV_ERR_BADOBJTYPE


#define KS_ERR_BADNAME         OV_ERR_BADNAME
#define KS_ERR_BADPATH         OV_ERR_BADPATH
#define KS_ERR_BADMASK         OV_ERR_BADMASK
#define KS_ERR_NOACCESS        OV_ERR_NOACCESS
#define KS_ERR_BADTYPE         OV_ERR_BADTYPE
#define KS_ERR_BADSELECTOR     OV_ERR_BADSELECTOR
#define KS_ERR_BADVALUE        OV_ERR_BADVALUE


#define KS_ERR_BADFACTORY      OV_ERR_BADFACTORY
#define KS_ERR_ALREADYEXISTS   OV_ERR_ALREADYEXISTS
#define KS_ERR_BADINITPARAM    OV_ERR_BADINITPARAM
#define KS_ERR_BADPLACEMENT    OV_ERR_BADPLACEMENT
#define KS_ERR_CANTMOVE        OV_ERR_CANTMOVE

#define KS_ERR_CANTSYNC                  ENUMVAL(OV_RESULT, 0x0015)
#define KS_ERR_NOREMOTE                  ENUMVAL(OV_RESULT, 0x0020)
#define KS_ERR_SERVERUNKNOWN             ENUMVAL(OV_RESULT, 0x0021)

/*
 * ... and errors which can occur in the client part of the
 * C++ Communication Library. These error codes have been put into
 * the range 0x1000-0x1FFF to distinguish them from core protocol
 * errors.
 */
#define KS_ERR_MALFORMEDPATH             ENUMVAL(OV_RESULT, 0x1001)
#define KS_ERR_NETWORKERROR              ENUMVAL(OV_RESULT, 0x1002)
#define KS_ERR_TYPEMISMATCH              ENUMVAL(OV_RESULT, 0x1003)
#define KS_ERR_HOSTUNKNOWN               ENUMVAL(OV_RESULT, 0x1004)
#define KS_ERR_CANTCONTACT               ENUMVAL(OV_RESULT, 0x1005)
#define KS_ERR_TIMEOUT                   ENUMVAL(OV_RESULT, 0x1006)
#define KS_ERR_NOMANAGER                 ENUMVAL(OV_RESULT, 0x1007)

/*
 * Please note that although an objects name (identifier) can't be longer than
 * 255 characters in length, there is no limit on the path to an object. Thus,
 * a full path and identifier name can be of arbitrary length.
 */
#define KS_NAME_MAXLEN OV_NAME_MAXLEN
#define KS_COMMENT_MAXLEN OV_COMMENT_MAXLEN
#define KS_TECHUNIT_MAXLEN OV_TECHUNIT_MAXLEN
#define KS_SIMPLETICKETID_MAXLEN OV_SIMPLETICKETID_MAXLEN

/* ----------------------------------------------------------------------------
 * States a value can have, so clients can decide whether they want to trust
 * a value.
 */
typedef OV_STATE KS_STATE;
#define KS_ST_NOTSUPPORTED  OV_ST_NOTSUPPORTED /* no state available          */
#define KS_ST_UNKNOWN       OV_ST_UNKNOWN      /* state unknown at this time  */
#define KS_ST_BAD           OV_ST_BAD          /* information is bad          */
#define KS_ST_QUESTIONABLE  OV_ST_QUESTIONABLE /* information is questionable */
#define KS_ST_GOOD          OV_ST_GOOD         /* information is good         */

//association of ClientHandler
#define KSBASE_CH_NOTNEEDED			0
#define KSBASE_CH_ASSOCIATED		1
#define KSBASE_CH_NOTASSOCATIED		2
#define KSBASE_CH_NOTFOUND			252

/*	ks client states	*/
#define KSBASE_CLST_INITIAL				0
#define KSBASE_CLST_AWAITINGCONNECTION	(1<<0)
#define KSBASE_CLST_AWAITINGANSWER		(1<<1)
#define KSBASE_CLST_BUSY				(1<<2)
#define KSBASE_CLST_COMPLETED			(1<<3)
#define KSBASE_CLST_ERROR				(1<<7)

//constates  0 - closed; 1 - opening; 2 - open; 4 - closing; 8 - sending; 16 - receiving; 32 - awaiting response; 128 - indicates error; 255 generic error
#define KSBASE_CONNSTATE_CLOSED				0
#define KSBASE_CONNSTATE_OPENING			1
#define KSBASE_CONNSTATE_OPEN				2
#define KSBASE_CONNSTATE_CLOSING			4
#define KSBASE_CONNSTATE_SENDING			8
#define KSBASE_CONNSTATE_RECEIVING			16
#define KSBASE_CONNSTATE_AWAITINGRESPONSE	32
#define KSBASE_CONNSTATE_GENERICERROR		255
#define KSBASE_CONNSTATE_ERRORINDICATOR	128
#define KSBASE_CONNSTATE_COULDNOTOPEN		130	//0x80 | 0x02
#define KSBASE_CONNSTATE_SENDERROR			136	//0x80 | 0x08
#define KSBASE_CONNSTATE_RECEIVEERROR		144	//0x80 | 0x10

//state of the server; 0: offline; 1: online; 2: inactive
#define KSBASE_SERVERREP_STATE_OFFLINE 0
#define KSBASE_SERVERREP_STATE_ONLINE 1
#define KSBASE_SERVERREP_STATE_INACTIVE 2

#endif
