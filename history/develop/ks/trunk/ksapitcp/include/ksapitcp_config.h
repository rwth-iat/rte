#include "ksapi.h"
#include "../../ksapi/include/config.h"
#include "../../ksapi/include/ksapi_logfile.h"

//TCPChannel status
//busy states are >10
#define CONSTATE_TCPCHANNEL_SENDING    11
#define CONSTATE_TCPCHANNEL_WAITINGRESPOND 12

//permanent error are >20
#define CONSTATE_TCPCHANNEL_MNGCOMINUSE    21 //Not yet used
#define CONSTATE_TCPCHANNEL_SOCKETCREATEFAILED 22
#define CONSTATE_TCPCHANNEL_SOCKETCONNECTFAILED 23
#define CONSTATE_TCPCHANNEL_SOCKETSENDFAILED 24
#define CONSTATE_TCPCHANNEL_NOSOCKET 25 //within typemethod no socket?!
#define CONSTATE_TCPCHANNEL_RESPONSETIMEDOUT 26	//response to xdr time out
#define CONSTATE_TCPCHANNEL_MNGTIMEDOUT 27		//response from Manager (getserver) times out
#define CONSTATE_TCPCHANNEL_GETSERVERFAILED 28
