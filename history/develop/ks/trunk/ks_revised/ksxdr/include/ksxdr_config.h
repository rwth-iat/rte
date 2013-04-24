

/*
 * KS_XDR identifier
 */

#define KSXDR_IDENTIFIER		"KSXDR"

/*
 * RPC-specific defines
 */

#define KSXDR_PROGRAMNUMBER		(0x049678)
#define KSXDR_PROGRAMVERSION	(0x02)
#define KSXDR_RPCVERSION		(0x02)
#define KSXDR_MSGTYPE_CALL		(0x00)
#define KSXDR_MSGTYPE_REPLY		(0x01)

/*
 * XDR Acceptance codes
 */

#define XDR_MSG_ACCEPTED		(0x00)
#define XDR_MSG_DENIED			(0x01)

/*
 * XDR Message Status codes	(for accepted messages)
 */
#define XDR_MSGST_SUCCESS       (0x00)		/* RPC executed successfully             */
#define XDR_MSGST_PROG_UNAVAIL  (0x01)		/* remote hasn't exported program        */
#define XDR_MSGST_PROG_MISMATCH (0x02)		/* remote can't support version #        */
#define XDR_MSGST_PROC_UNAVAIL	(0x03)		/* program can't support procedure       */
#define XDR_MSGST_GARBAGE_ARGS  (0x04)		/* procedure can't decode params         */
#define	XDR_MSGST_SYSTEM_ERR    (0x05)		/* errors like memory allocation failure */

/*
 * Reasons why a call message was rejected:
 */

#define XDR_DEN_RPC_MISMATCH 	(0x00)		/* RPC version number != 2          */
#define	XDR_DEN_AUTH_ERROR 		(0x01)		/* remote can't authenticate caller */

#define XDR_AUTH_FAILED			(0x07)		/* we only know generic here since we do not use xdr authentication */

/*
 * XDR AUTH types
 */

#define XDR_AUTH_NONE			(0x00)
#define XDR_AUTH_SYS			(0x01)
#define XDR_AUTH_SHORT			(0x02)

/*
 * KSXDR specific client states
 */

#define XDRCL_WAITINGFORSENDING	(1<<18)
#define	XDRCL_WAITINGFORPORT	(1<<19)
#define XDRCL_SERVERNOTFOUND	(1<<24)
#define XDRCL_MANAGERNOTFOUND	(1<<25)
#define XDRCL_TIMEOUT			(1<<26)

#define XDRCL_PROCANS_OK			0
#define XDRCL_PROCANS_INCOMPLETE	(1<<0)
#define XDRCL_PROCANS_XIDMISSMATCH	(1<<7)




