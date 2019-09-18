#ifndef TCPbind_confih_INCLUDED
#define TCPbind_confih_INCLUDED

#include "ksbase_helper.h"

//Size of data received per chunk
#define TCPbind_CHUNKSIZE			1460	/*	maximum size of tcp segment over ethernet IP 	*/

//timeouts for destroying inactive Channels
#define TCPbind_TTL_AFTER_ASSOC		1200
#define TCPbind_TTL_BEFORE_ASSOC	60

//timeout after receiving an incomplete package (erase of data)
#define TCPbind_TIMEOUT_PARTIALPACKAGE	300

//constates  0 - closed; 1 - opening; 2 - open; 4 - closing; 8 - sending; 16 - receiving; 32 - awaiting response; 128 - indicates error; 255 generic error
#define TCPbind_CONNSTATE_CLOSED			KSBASE_CONNSTATE_CLOSED
#define TCPbind_CONNSTATE_OPENING			KSBASE_CONNSTATE_OPENING
#define TCPbind_CONNSTATE_OPEN				KSBASE_CONNSTATE_OPEN
#define TCPbind_CONNSTATE_CLOSING			KSBASE_CONNSTATE_CLOSING
#define TCPbind_CONNSTATE_SENDING			KSBASE_CONNSTATE_SENDING
#define TCPbind_CONNSTATE_RECEIVING			KSBASE_CONNSTATE_RECEIVING
#define TCPbind_CONNSTATE_AWAITINGRESPONSE	KSBASE_CONNSTATE_AWAITINGRESPONSE
#define TCPbind_CONNSTATE_GENERICERROR		KSBASE_CONNSTATE_GENERICERROR
#define TCPbind_CONNSTATE_ERRORINDICATOR	KSBASE_CONNSTATE_ERRORINDICATOR
#define TCPbind_CONNSTATE_COULDNOTOPEN		KSBASE_CONNSTATE_COULDNOTOPEN
#define TCPbind_CONNSTATE_SENDERROR			KSBASE_CONNSTATE_SENDERROR
#define TCPbind_CONNSTATE_RECEIVEERROR		KSBASE_CONNSTATE_RECEIVEERROR

#endif
