#include "ov_ksserver_stripped.h"

//#include "ks/ks.h"
#include <stdlib.h>
#include <stdio.h>
#include "ksserv.h"
#include "ksserv_logfile.h"

#if !OV_SYSTEM_NT
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#else
#include <rpc/xdr.h>
#endif

#include "libov/ov_macros.h"
#include "libov/ov_string.h"
#include "libov/ov_path.h"

#include "xdrexec.h"

OV_STRING ksserv_xdrinfo(XDR xdrs) {
	u_long xid, messageType, rpcVersion, progID, progVersion, procedure;

        char tmp[100] = "";
	OV_STRING result = "";

	//analyse XDR
	if ( !xdr_u_long(&xdrs, &xid)  ||
	     !xdr_u_long(&xdrs, &messageType)  ||
	     !xdr_u_long(&xdrs, &rpcVersion) ||
	     !xdr_u_long(&xdrs, &progID) ||
	     !xdr_u_long(&xdrs, &progVersion) ||
	     !xdr_u_long(&xdrs, &procedure)) { //ooops
	   ksserv_logfile_info("decode didnt work out :-/");
	   ov_string_setvalue(&result,"decode didnt work out :-/");
	} else {
          ksserv_logfile_info("Metainfo from XDR");
	  ov_string_setvalue(&result,"Ok - Metainfo from XDR");
	  ov_string_append(&result,"\n\n");

	  if(rpcVersion == 1) {//seems to be a RPC-REPLY we have to analyse!
	    //this is a ugly detection of type, but I dont klnow any other
	    //in normal RPC situations you know if its a CALL or REPLY.
	    ksserv_logfile_info("detected type: REPLY");
	    sprintf(tmp, "detected type: REPLY\n");
	    ov_string_append(&result,tmp);
	
	    //messageType of CALL seems to be xid in case of REPLY
	    ksserv_logfile_info("xid: %d", messageType);
	    sprintf(tmp, "xid: %ld \n",messageType);
	    ov_string_append(&result,tmp);

	    //rpcVersion of CALL seems to be messageType of REPLY
	    ksserv_logfile_info("messageType (call=0/reply=1): %d", rpcVersion);
	    sprintf(tmp, "messageType (call=0/reply=1): %ld \n",rpcVersion);
	    ov_string_append(&result,tmp);

	  } else { //seems to be sth...maybe a RPC-CALL!
	    ksserv_logfile_info("detected type: CALL");
	    sprintf(tmp, "detected type: CALL\n");
	    ov_string_append(&result,tmp);

	    ksserv_logfile_info("xid: %d", xid);
	    sprintf(tmp, "xid: %ld \n",xid);
	    ov_string_append(&result,tmp);

	    ksserv_logfile_info("messageType (call=0/reply=1): %d", messageType);
	    sprintf(tmp, "messageType (call=0/reply=1): %ld \n",messageType);
	    ov_string_append(&result,tmp);

	    ksserv_logfile_info("rpcVersion (=2): %d", rpcVersion);
	    sprintf(tmp, "rpcVersion (=2): %ld \n",rpcVersion);
	    ov_string_append(&result,tmp);

	    ksserv_logfile_info("progID (=300664): %d", progID);
	    sprintf(tmp, "progID (=300664): %ld \n",progID);
	    ov_string_append(&result,tmp);

	    ksserv_logfile_info("progVersion (=2): %d", progVersion);
	    sprintf(tmp, "progVersion (=2): %ld \n",progVersion);
	    ov_string_append(&result,tmp);

	    ksserv_logfile_info("procedure: %d", procedure);
	    sprintf(tmp, "procedure: %ld \n",procedure);
	    ov_string_append(&result,tmp);
	  }

	}

	//finally return the generated data
	return result;

}

/**
 * Handles the XDR Stream and returns the result as a XDR
 */
OV_DLLFNCEXPORT OV_STRING ksserv_xdrexec(XDR *in, XDR *out, OV_INSTPTR_ksserv_Client client) {
	//~ OV_INSTPTR_ov_domain communication = (OV_INSTPTR_ov_domain)ov_path_getobjectpointer("/communication", 2);
	//~ OV_INSTPTR_ksservtcp_tcpclient tcpclient;
	u_long xid, messageType, rpcVersion, progID, progVersion, procedure, trash, trash2, trash3, trash4, ticketindicator;
	char *ovticket;
	OV_TICKET *ticket;
    //int err = 0;
	
	//ksserv_logfile_info("xdrexec/################################################### ksserv xdrexec: Start of Exec!");



	//analyse XDR
	if ( !xdr_u_long(in, &xid)  ||
	     !xdr_u_long(in, &messageType)  ||
	     !xdr_u_long(in, &rpcVersion) ||
	     !xdr_u_long(in, &progID) ||
	     !xdr_u_long(in, &progVersion) ||
	     !xdr_u_long(in, &procedure)) { //ooops
	  ksserv_logfile_info("xdrexec/decode didnt work out :-/");
	  return "ERROR: decoding the RPC header didnt work out, thus dont executing";
	} 
	//ksserv_logfile_info("xdrexec/Ok - executing...after RCP header; handy: %d and private: ", (in)->x_handy, (in)->x_private);
	//ksserv_logfile_info("xdrexec/procedure: %d", procedure);
	//ksserv_logfile_info("xdrexec/progVersion: %d", progVersion);

	//ksserv_logfile_info("Ok - preparing: Create Ticket...");
	//ticket = ov_ksserver_ticket_create(in);
	ticket = ov_ksserver_ticket_defaultticket_createticket_wrap(in, OV_TT_NONE);

	//this is trash within the XDR - or better: I dont know what it is for!
	//this has to be skipped
	xdr_u_long(in, &trash);		/*	Lars: these are the rpc-authentication fields. the first contains the authentication credential's "flavor" (in pricipal a ticket indicator)	*/
	xdr_u_long(in, &trash2);	/*	Lars: this contains opaque authentication data. This means there could be more than 4 bates but as we do not use this stuff, there normally are only 4	*/
	xdr_u_long(in, &trash3);	/*	Lars: This onecontains the authentication verifiers's "flavor" (in pricipal a ticket indicator)	*/
	xdr_u_long(in, &trash4);	/*	Lars: this contains opaque authentication data. This means there could be more than 4 bates but as we do not use this stuff, there normally are only 4	*/
	xdr_u_long(in, &ticketindicator);
	//ksserv_logfile_info("trash: %d", trash);
	//ksserv_logfile_info("trash2: %d", trash2);
	//ksserv_logfile_info("trash3: %d", trash3);
	//ksserv_logfile_info("trash4: %d", trash4);
	//ksserv_logfile_info("ticketindicator: %d", ticketindicator);

	if(ticketindicator == 0){
		ksserv_logfile_info("xdrexec/OVTicket: detected None");
	} else if(ticketindicator == 1) {
		ovticket = (char*)malloc(256);
		xdr_string(in, &ovticket, 256);
		ksserv_logfile_info("xdrexec/OVTicket: detected Simple: '%s' (provided to tcpclient obj)",ovticket);
        ov_string_print(&client->v_ovticket, "%s", ovticket);
        ov_string_setvalue(&ovticket, NULL);
	} else {
		ksserv_logfile_info("OVTicket: unknown (complex?) type: %d - ignoring",ticketindicator);
		//if(! ksserv_decodeOVTicket(in, client)) { TODO
		  return "ERROR: OVTicket of unknown type detected, requires implementation!";
	    // }
	}

	//real dispatch of the KS XDR body
	ov_ksserver_dispatch_wo_answer(procedure,
		progVersion,
		ticket,	
		in,
		out);
	return "Done.";
}

