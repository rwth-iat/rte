

#define STATUS_STOK          0
#define STATUS_STPMSOCKCR    2
#define STATUS_STPMRQSND     3
#define STATUS_STPMRPLYRCV   4
#define STATUS_STMNGRQSND    5

//Channel status
#define CONSTATE_CHANNEL_OK    0

//KSCommon status
#define STATUS_KSCOMMON_OK    0
#define STATUS_KSCOMMON_BUSY    1
#define STATUS_KSCOMMON_NOCHANNEL    2
#define STATUS_KSCOMMON_CHANNELERROR    3


#if PLT_SYSTEM_NT
 #define CLOSE_SOCKET closesocket
 #define IOCTL_SOCKET ioctlsocket
#else
 #define CLOSE_SOCKET close
 #define IOCTL_SOCKET ioctl
#endif

