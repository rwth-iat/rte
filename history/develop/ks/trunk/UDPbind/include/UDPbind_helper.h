#ifndef UDPbind_helper_INCLUDED
#define UDPbind_helper_INCLUDED

#if OV_SYSTEM_NT
 #define UDPBIND_INVALID_SOCKET INVALID_SOCKET
 #define UDPBIND_SOCKET_ERROR SOCKET_ERROR
 #define UDPBIND_SOCKET SOCKET
 #define UDPBIND_CLOSE_SOCKET closesocket
#else
 #define UDPBIND_INVALID_SOCKET -1
 #define UDPBIND_SOCKET_ERROR -1
 #define UDPBIND_SOCKET int
 #define UDPBIND_CLOSE_SOCKET close
#endif

#define OV_UDPBIND_SETSOCKET2INT(varsocket, varint)\
	if(varsocket == UDPBIND_INVALID_SOCKET)	\
		varint = -1;	\
	else	\
		varint = varsocket

#define OV_UDPBIND_SETINT2SOCKET(varint, varsocket)\
	if(varint == -1)	\
		varsocket = UDPBIND_INVALID_SOCKET;	\
	else	\
		varsocket = varint

#endif
