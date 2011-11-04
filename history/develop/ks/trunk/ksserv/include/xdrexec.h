OV_DLLFNCEXPORT OV_STRING ksserv_xdrexec(XDR *in, XDR *out, OV_INSTPTR_ksserv_Client client);
OV_STRING ksserv_xdrinfo(XDR xdr);

#if PLT_SYSTEM_NT
 #define CLOSE_SOCKET closesocket
 #define IOCTL_SOCKET ioctlsocket
#else
 #define CLOSE_SOCKET close
 #define IOCTL_SOCKET ioctl
#endif
