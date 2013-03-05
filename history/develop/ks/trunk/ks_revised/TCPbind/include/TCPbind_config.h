#ifndef TCPbind_confih_INCLUDED
#define TCPbind_confih_INCLUDED

//Size of data received per chunk
#define TCPbind_CHUNKSIZE			1460	/*	maximum size of tcp segment over ethernet IP 	*/

//timeouts for destroying inactive Channels
#define TCPbind_TTL_AFTER_ASSOC		1200
#define TCPbind_TTL_BEFORE_ASSOC	60

//timeout after receiving an incomplete package (erase of data)
#define TCPbind_TIMEOUT_PARTIALPACKAGE	300

//constates  0 - closed; 1 - opening; 2 - open; 4 - closing; 8 - sending; 16 - receiving; 32 - awaiting response; 128 - indicates error; 255 generic error
#define TCPbind_CONNSTATE_CLOSED			0
#define TCPbind_CONNSTATE_OPENING			1
#define TCPbind_CONNSTATE_OPEN				2
#define TCPbind_CONNSTATE_CLOSING			4
#define TCPbind_CONNSTATE_SENDING			8
#define TCPbind_CONNSTATE_RECEIVING			16
#define TCPbind_CONNSTATE_AWAITINGRESPONSE	32
#define TCPbind_CONNSTATE_GENERICERROR		255
#define TCPbind_CONNSTATE_ERRORINDICATOR	128
#define TCPbind_CONNSTATE_COULDNOTOPEN		130	//0x80 | 0x02
#define TCPbind_CONNSTATE_SENDERROR			136	//0x80 | 0x08
#define TCPbind_CONNSTATE_RECEIVEERROR		144	//0x80 | 0x10


#endif
