#ifndef TCPbind_helper_INCLUDED
#define TCPbind_helper_INCLUDED

#if OV_SYSTEM_NT
 #define TCPBIND_INVALID_SOCKET INVALID_SOCKET
 #define TCPBIND_SOCKET_ERROR SOCKET_ERROR
 #define TCPBIND_SOCKET SOCKET
 #define TCPBIND_CLOSE_SOCKET closesocket
#else
 #define TCPBIND_INVALID_SOCKET -1
 #define TCPBIND_SOCKET_ERROR -1
 #define TCPBIND_SOCKET int
 #define TCPBIND_CLOSE_SOCKET close
#endif

#define TCPBIND_SETSOCKET2INT(varsocket, varint)\
	if(varsocket == TCPBIND_INVALID_SOCKET)	\
		varint = -1;	\
	else	\
		varint = varsocket

#define OV_TCPBIND_SETINT2SOCKET(varint, varsocket)\
	if(varint == -1)	\
		varsocket = TCPBIND_INVALID_SOCKET;	\
	else	\
		varsocket = varint

#endif
