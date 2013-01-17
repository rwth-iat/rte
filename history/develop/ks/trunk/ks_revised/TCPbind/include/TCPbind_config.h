#ifndef TCPbind_confih_INCLUDED
#define TCPbind_confih_INCLUDED

//Size of data received per chunk
#define TCPbind_CHUNKSIZE			256

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

//association of ClientHandler
#define TCPbind_CH_NOTNEEDED			0
#define TCPbind_CH_ASSOCIATED			1
#define TCPbind_CH_NOTASSOCATIED		2
#define TCPbind_CH_NOTFOUND			252


#endif
