#ifndef TCPbind_helper_INCLUDED
#define TCPbind_helper_INCLUDED

#if OV_SYSTEM_NT
 #define CLOSE_SOCKET closesocket
 #define IOCTL_SOCKET ioctlsocket
#else
 #define CLOSE_SOCKET close
 #define IOCTL_SOCKET ioctl
#endif


#endif
