// const int STOK; //0
// const int STPMSOCKCR; //2
// const int STPMRQSND; //3
// const int STPMRPLYRCV; //4
// const int STMNGRQSND; //5

//cyccount of managercom obj, thus intervall of REGISTER sending
#define MANAGER_REGISTER_INTERVALL 10000

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

#define HTTP_COMMON_HEADER "Access-Control-Allow-Origin:*\nConnection: close\n"
#define HTTP_404_ERROR "HTTP/1.1 404 Not Found\n" HTTP_COMMON_HEADER "\nerror 404: path not found\n"
#define HTTP_400_ERROR "HTTP/1.1 400 OK\n" HTTP_COMMON_HEADER "\nerror 400: parameter are not valid\n"
#define HTTP_418_ERROR "HTTP/1.1 418 OK\n" HTTP_COMMON_HEADER "\nI am a teapot (see RFC 2324)\n"
#define HTTP_500_ERROR "HTTP/1.1 500 OK\n" HTTP_COMMON_HEADER "\nerror 500: internal server error\n"
#define HTTP_503_ERROR "HTTP/1.1 503 OK\n" HTTP_COMMON_HEADER "\nerror 503: service not available\n"
#define HTTP_200_HEADER "HTTP/1.1 200 OK\n" HTTP_COMMON_HEADER //content has to follow

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

OV_RESULT getvar_to_string(OV_INSTPTR_ov_object pObj, OV_STRING* varname, OV_STRING* message);
OV_RESULT setvar_at_object(OV_INSTPTR_ov_object pObj, OV_STRING* varname, OV_STRING* newcontent, OV_STRING* message);
OV_RESULT parse_http_request(OV_STRING buffer, OV_STRING* cmd, OV_STRING_VEC* args);
OV_RESULT find_arguments(OV_STRING_VEC* args, const OV_STRING varname, OV_STRING_VEC* re);
