// const int STOK; //0
// const int STPMSOCKCR; //2
// const int STPMRQSND; //3
// const int STPMRPLYRCV; //4
// const int STMNGRQSND; //5

//cyccount of managercom obj, thus intervall of REGISTER sending
#define MANAGER_REGISTER_INTERVALL 10000

//timeouts for TCP/UDP connections with manager
#define MANAGER_TIMEOUT_SEC 0
#define MANAGER_TIMEOUT_USEC 500000

//timeout for a vaild portmap unregister answer
#define UNREGISTER_TIMEOUT 0.5

#define STATUS_STOK          0
#define STATUS_STPMSOCKCR    2
#define STATUS_STPMRQSND     3
#define STATUS_STPMRPLYRCV   4
#define STATUS_STMNGRQSND    5

//tcpconnection status
#define STATUS_TCPCON_OK          0
#define STATUS_TCPCON_SOCKOPENFAILED 1
#define STATUS_TCPCON_SOCKNONBLOCKFAILED 2
#define STATUS_TCPCON_SOCKBINDFAILED 3
#define STATUS_TCPCON_SOCKLISTENFAILED 4
#define STATUS_TCPCON_SOCKACCEPTFAILED 5
#define STATUS_TCPCON_SOCKNONBLOCKCLIENTFAILED 6

//manager status
#define STATUS_MANAGER_OK          0

//managersendrecv status
#define STATUS_MANAGERSENDRECV_UNUSED                             0
#define STATUS_MANAGERSENDRECV_WAITINGFORSENDING          1
#define STATUS_MANAGERSENDRECV_WAITINGFORANSWER           2


#if PLT_SYSTEM_NT
 #define CLOSE_SOCKET closesocket
 #define IOCTL_SOCKET ioctlsocket
#else
 #define CLOSE_SOCKET close
 #define IOCTL_SOCKET ioctl
#endif

#ifndef EWOULDBLOCK
#define EWOULDBLOCK WSAEWOULDBLOCK
#endif
