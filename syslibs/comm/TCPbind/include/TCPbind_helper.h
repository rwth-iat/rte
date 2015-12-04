#ifndef TCPbind_helper_INCLUDED
#define TCPbind_helper_INCLUDED

#define OV_TCPBIND_SETSOCKET2INT(varsocket, varint)\
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
