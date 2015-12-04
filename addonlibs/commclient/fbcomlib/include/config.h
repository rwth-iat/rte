//0: processing
//1: everything ok
//2: wrong input
//3: No return method set
//4: OV-error
//5: connection error
//6: service error
//7: wrong variable type
//8: client busy
//9: no memory to store answer

#define FBCOMLIB_STATE_INIT				0
#define FBCOMLIB_STATESTR_INIT			"initial state"

#define FBCOMLIB_STATE_WAITING			1
#define FBCOMLIB_STATESTR_WAITING		"request sent, awaiting answer"

#define FBCOMLIB_STATE_OK				2
#define FBCOMLIB_STATESTR_OK			"OK"

#define FBCOMLIB_STATE_WRONGINPUT		4
#define FBCOMLIB_STATESTR_WRONGINPUT	"invalid input variables"

#define FBCOMLIB_STATE_INTERNAL_ERROR	8
#define FBCOMLIB_STATESTR_INTERNAL_ERROR	"internal error"

#define FBCOMLIB_STATE_KSAPI_ERROR		16
#define FBCOMLIB_STATESTR_KSAPI_ERROR	"ksapi error"

#define FBCOMLIB_STATE_EXTERNAL_ERROR	32
#define FBCOMLIB_STATESTR_EXTERNAL_ERROR	"external error"

#define FBCOMLIB_STATE_TIMEOUT			64
#define FBCOMLIB_STATESTR_TIMEOUT		"request timed out"

#define FBCOMLIB_STATE_UNKNOWN_ERROR	128
#define FBCOMLIB_STATESTR_UNKNOWN_ERROR	"unknown error"



