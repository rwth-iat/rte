
#ifndef ksbase_helper_INCLUDED
#define ksbase_helper_INCLUDED

#include "libov/ov_ov.h"
#include "ksbase.h"

OV_DLLFNCEXPORT OV_BOOL ks_isvalidname(OV_STRING name);

OV_DLLFNCEXPORT void ksbase_free_KSDATAPACKET(KS_DATAPACKET* packet);

OV_DLLFNCEXPORT OV_RESULT ksbase_KSDATAPACKET_append(KS_DATAPACKET* packet, OV_BYTE* data, OV_UINT addlength);

OV_DLLFNCEXPORT OV_RESULT ksbase_KSDATAPACKET_set(KS_DATAPACKET* packet, OV_BYTE* data, OV_UINT addlength);

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
