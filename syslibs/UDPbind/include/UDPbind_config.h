#ifndef UDPbind_confih_INCLUDED
#define UDPbind_confih_INCLUDED

#include "ksbase_helper.h"

#if !OV_SYSTEM_NT
#include <unistd.h>
#include <sys/select.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#endif

//Size of data received per chunk
#define UDPbind_CHUNKSIZE			1460	/*	maximum size of tcp segment over ethernet IP 	*/

//timeouts for destroying inactive Channels
#define UDPbind_TTL_AFTER_ASSOC		1200
#define UDPbind_TTL_BEFORE_ASSOC	60

//timeout after receiving an incomplete package (erase of data)
#define UDPbind_TIMEOUT_PARTIALPACKAGE	300

//constates  0 - closed; 1 - opening; 2 - open; 4 - closing; 8 - sending; 16 - receiving; 32 - awaiting response; 128 - indicates error; 255 generic error
#define UDPbind_CONNSTATE_CLOSED			KSBASE_CONNSTATE_CLOSED
#define UDPbind_CONNSTATE_OPENING			KSBASE_CONNSTATE_OPENING
#define UDPbind_CONNSTATE_OPEN				KSBASE_CONNSTATE_OPEN
#define UDPbind_CONNSTATE_CLOSING			KSBASE_CONNSTATE_CLOSING
#define UDPbind_CONNSTATE_SENDING			KSBASE_CONNSTATE_SENDING
#define UDPbind_CONNSTATE_RECEIVING			KSBASE_CONNSTATE_RECEIVING
#define UDPbind_CONNSTATE_AWAITINGRESPONSE	KSBASE_CONNSTATE_AWAITINGRESPONSE
#define UDPbind_CONNSTATE_GENERICERROR		KSBASE_CONNSTATE_GENERICERROR
#define UDPbind_CONNSTATE_ERRORINDICATOR	KSBASE_CONNSTATE_ERRORINDICATOR
#define UDPbind_CONNSTATE_COULDNOTOPEN		KSBASE_CONNSTATE_COULDNOTOPEN
#define UDPbind_CONNSTATE_SENDERROR			KSBASE_CONNSTATE_SENDERROR
#define UDPbind_CONNSTATE_RECEIVEERROR		KSBASE_CONNSTATE_RECEIVEERROR

#endif
