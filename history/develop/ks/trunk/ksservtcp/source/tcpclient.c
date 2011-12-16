/** @file 
 * This class takes over the whole communication with a connected client.
 * Objects of this class are created by the tcpconnection-object if a
 * connection-request is comming on the at ks_manager registered port.
 ******************************************************************************/
#ifndef OV_COMPILE_LIBRARY_ksservtcp
#define OV_COMPILE_LIBRARY_ksservtcp
#endif

#include <time.h>

#include "ksservtcp.h"
#include "ksserv.h"
#include "ksserv_logfile.h"
#include "config.h"
#include "libov/ov_scheduler.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <rpc/xdr.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>

#if !OV_SYSTEM_NT
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>
#else
#include <winsock2.h>
#endif

#include "libov/ov_macros.h"
#include "xdrexec.h"
#include "ov_ks_missing_symbols.h"
#include "libov/ov_path.h"

#define BUFFER_CHUNK_SIZE 128

/**
 * Returns the socket on which the object is receiving.
 */
OV_DLLFNCEXPORT OV_INT ksservtcp_tcpclient_receivesocket_get(
             OV_INSTPTR_ksservtcp_tcpclient          pobj
) {
             return pobj->v_receivesocket;
}

/**
 * Sets the socket on which the object is receiving.
 */
OV_DLLFNCEXPORT OV_RESULT ksservtcp_tcpclient_receivesocket_set(
             OV_INSTPTR_ksservtcp_tcpclient          pobj,
             const OV_INT           value
) {
    struct sockaddr_in m_addr;
#if OV_SYSTEM_NT
    int len;
#else
    socklen_t len;
#endif
    pobj->v_receivesocket = value;

    len = sizeof m_addr;
	if (getpeername(value, (struct sockaddr*)&m_addr, &len) == -1) {
		ksserv_logfile_error("Error while determining source IP of tcpclient -- getpeername() failed");
		 return OV_ERR_OK;
	 }
	 ov_string_print(&pobj->v_sourceAdr, "%s:%d", inet_ntoa(m_addr.sin_addr), (int) ntohs(m_addr.sin_port));
	 ksserv_logfile_debug("tcpclient/receivesocket/set: new TCPSocket %d from %s", value, pobj->v_sourceAdr);
     return OV_ERR_OK;
}


/**
 * This method is called on startup.
 */
OV_DLLFNCEXPORT void ksservtcp_tcpclient_startup(
	OV_INSTPTR_ov_object 	pobj
) {
	//ksserv_logfile_info("tcpclient/startup ###########");
	ov_object_startup(pobj);
    return;
}

/**
 * Procedure called on object shutdown.
 * It closes open socket.
 */
OV_DLLFNCEXPORT void ksservtcp_tcpclient_shutdown(
	OV_INSTPTR_ov_object 	pobj
) {
	OV_INSTPTR_ksservtcp_tcpclient this = Ov_StaticPtrCast(ksservtcp_tcpclient, pobj);
   
	int receivesocket;
	ksserv_logfile_error("tcpclient/shutdown: %s", pobj->v_identifier);
	if(!this->v_deleted) {// v_deleted cares that socket closing happens just once, not twiche while DeleteObj
		receivesocket = ksservtcp_tcpclient_receivesocket_get(this);
		if(receivesocket < 0) {
			ksserv_logfile_error("tcpclient/shutdown: %s has socket<0 - cant shutdown!?!", pobj->v_identifier);
			return;
		}
		//ksserv_logfile_debug("tcpclient/shutdown: %s closes socket %d", pobj->v_identifier, receivesocket);
		CLOSE_SOCKET(receivesocket);
		this->v_deleted = TRUE;
		Ov_DeleteObject(this); //calls shutdown again. Since it is deleted->TRUE, no recursion
		//ov_object_shutdown(pobj);
		return;
	}
	ov_object_shutdown(pobj);
	return;
}

/**
 * Procedure periodically called by ComTask // ov_scheduler. * 
 * It takes over the sending/receiving of data to/from the
 * connected client.
 *  * If the connection is closed by the client this object
 * delets itself.
 */
void ksservtcp_tcpclient_typemethod(
	//COmTask OV_INSTPTR_ov_object		pfb
	OV_INSTPTR_ksserv_ComTask	cTask
) {
	OV_INSTPTR_ksservtcp_tcpclient this = Ov_StaticPtrCast(ksservtcp_tcpclient, cTask);
    OV_INSTPTR_ov_domain managerdomain = (OV_INSTPTR_ov_domain)ov_path_getobjectpointer("/communication/ksservtcp/manager", 2);
    OV_INSTPTR_ksservtcp_register preg = (OV_INSTPTR_ksservtcp_register)Ov_SearchChild(ov_containment, managerdomain, "register");
    OV_INSTPTR_ksservtcp_unregister pureg = (OV_INSTPTR_ksservtcp_unregister)Ov_SearchChild(ov_containment, managerdomain, "unregister");
    OV_INSTPTR_ksservtcp_getserverdata pgsd = (OV_INSTPTR_ksservtcp_getserverdata)Ov_SearchChild(ov_containment, managerdomain, "getserverdata");
	XDR xdrdec, xdrs, resultXDRs;
    int i;
    int c;
    int bytes = -1; //-1 means nothing received
    int size_received, size_return, xdrlret;
    int receivesocket = ksservtcp_tcpclient_receivesocket_get(this);
    char *xdr_received, *xdr_return;
    char xid2[4];

    char *buffer = 0;
    int recvBytes = 0;
    int buffer_size = 0; //factor of buffer size * BUFFER_CHUNK_SIZE
    char *buffer_location = 0; //pointer into the buffer 

    char rpcheader[4];
    char *temp;
	char *tmpfragment;
    int j;
    char *receivedname;
  	 int receivednamelength, receivednamelengthxdr;
  	 int receivedversion;
  	 int receivedport;
  	 int receivedttl;
  	 int secs, usecs;
  	 char *buf;
  	 char tmp[4];
 	u_long xid, messageType, rpcVersion, progID, progVersion, procedure;
 	
 	MemoryStreamFragment* currFragment = NULL;
 	char* placeInBuffer = NULL;
 	int sentChunkSize = 0;
 	int sentBytes = 0;
        
  	//ksserv_logfile_debug("tcpclient typemethod called ");
	if (receivesocket < 0) { // check if the socket might be OK.
		ksserv_logfile_error("%s/typemethod: no receive socket set (receivesocket==-1), thus deleting myself",cTask->v_identifier);
		ksservtcp_tcpclient_shutdown((OV_INSTPTR_ov_object)cTask);
		return;
	}
	do {//read KS command from socket in chunks until nothing more appears
		buffer_size++;
		buffer = (char*)realloc(buffer, BUFFER_CHUNK_SIZE*buffer_size); 
		if(buffer == 0) {
			ov_logfile_error("tcpclient/typemethod: recv error, no memory for buffer");
			return;
		}
		buffer_location = buffer + BUFFER_CHUNK_SIZE * (buffer_size - 1);
		memset(buffer_location, 0, BUFFER_CHUNK_SIZE);
		recvBytes = recv(receivesocket, buffer_location, BUFFER_CHUNK_SIZE,0);
		if(recvBytes != -1 && bytes == -1) bytes = 0; //1st iteration and we have sth (prevents from missing 1 byte)
		if(recvBytes != -1) bytes += recvBytes;//if soth was received, calc overall size of received bytes
		//if(recvBytes > 0) ksserv_logfile_error("tcpclient/typemethod: ks cmd chunk no %d recv %d bytes, pBuffStart %p, pBuffInside %p",buffer_size, recvBytes, buffer, buffer_location);
	} while(recvBytes == BUFFER_CHUNK_SIZE); // stop if less than maximum bytes was read by recv

	//if(bytes != -1) ksserv_logfile_error("tcpclient/typemethod: ks cmd w/ size %d received ",bytes);
	

#if OV_SYSTEM_NT
	errno = WSAGetLastError();
#endif
	if(bytes == -1 && ( errno != EAGAIN || errno != EWOULDBLOCK ) ) {//ERROR during read - shutdown!
		ksserv_logfile_error("tcpclient/typemethod: recv error, shutdown TCPClient");
		ksservtcp_tcpclient_shutdown((OV_INSTPTR_ov_object)cTask);
		//else: bytes = -1 && errno == EAGAIN || EWOULDBLOCK
	} else if(bytes == 0) {//normal shutdown by client
	    ksserv_logfile_debug("tcpclient/typemethod: read 0 bytes - shutdown - %s", ((OV_INSTPTR_ksserv_Client)this)->v_sourceAdr);
		ksservtcp_tcpclient_shutdown((OV_INSTPTR_ov_object)cTask);
	//} else if (bytes == -1) {  // no current KS command this turn
	} else if(bytes > 0) {
			ksserv_logfile_error("tcpclient/typemethod: got ks cmd w/ %d bytes",bytes);
			this->v_receivedCalls++; //count number of calls
			ksserv_Client_setThisAsCurrent((OV_INSTPTR_ksserv_Client)this); //set this as current one
			//get size_received
			size_received = bytes-4;		//with rpc-header
			//set size of xdr_received
			xdr_received = (char*)malloc(size_received);
			memset(xdr_received, 0, size_received);
			for (i=0; i<size_received; i++)
				xdr_received[i] = buffer[i+4];		//with rpc-header
				
			xdrmem_create(&xdrdec, xdr_received , size_received, XDR_DECODE);
			if ( !xdr_u_long(&xdrdec, &xid)  ||
				     !xdr_u_long(&xdrdec, &messageType)  ||
				     !xdr_u_long(&xdrdec, &rpcVersion) ||
				     !xdr_u_long(&xdrdec, &progID) ||
				     !xdr_u_long(&xdrdec, &progVersion) ||
				     !xdr_u_long(&xdrdec, &procedure)) { //ooops
				  ksserv_logfile_info("tcpclient/typemethod: decode header didnt work out :-/");
				  free(buffer);
				  return;
			}
			//clear xdr?
			//MemStreamDestroy(&xdrs);

			//KSDEVEL int j;
			 //KSDEVEL printf("\nxdr_received:\nlength: %d\n", size_received);
			 //KSDEVEL for (j = 0; j < size_received; j=j+4)
				 //KSDEVEL printf("%X %X %X %X     ", xdr_received[j], xdr_received[j+1], xdr_received[j+2], xdr_received[j+3]);
			 //KSDEVEL printf("\n\n");
			
			//get xid
			for (i=0; i<4; i++)
				xid2[i] = xdr_received[i];
					
			//get procedure
		 	//for(j=0;j<4;j++)
		 	//	tmp[3-j] = xdr_received[16+j];
		 	//memcpy(&procedure, tmp, 4);
		 	ksserv_logfile_info("%s/typemethod (source %s) -- allHeaderData: xid:%d msgType:%d rpcVer:%d progID:%d progVersion %d procedure:%d ",
		 			cTask->v_identifier, this->v_sourceAdr, xid, messageType, rpcVersion, progID, progVersion, procedure);
			
		switch (procedure)	{
				 case 65281:	//register
				 	//get name
			 		for(j=0;j<4;j++)
		 				tmp[3-j] = xdr_received[44+j];//martin40
			 		memcpy(&receivednamelength, tmp, 4);
			 		receivednamelengthxdr = receivednamelength;
			 		while ((receivednamelengthxdr%4)!=0)
			 			receivednamelengthxdr++;
			 		receivedname = (char*)malloc(receivednamelength+1);
			 		memset(receivedname, 0, receivednamelength+1);
			 		for(j=0;j<receivednamelength;j++)
			 			receivedname[j]= xdr_received[48+j];//martin44
		 			//get version
			 		for(j=0;j<4;j++)
			 			tmp[3-j] = xdr_received[48+receivednamelengthxdr+j];//martin44
		 			memcpy(&receivedversion, tmp, 4);
			 		//get port
			 		for(j=0;j<4;j++)
		 				tmp[3-j] = xdr_received[52+receivednamelengthxdr+j];//martin48
		 			memcpy(&receivedport, tmp, 4);
			 		//get ttl
		 			for(j=0;j<4;j++)
		 				tmp[3-j] = xdr_received[56+receivednamelengthxdr+j];//martin52
		 			memcpy(&receivedttl, tmp, 4);
			 		//set register object
		 			ksserv_logfile_info("TCPClient: REGISTER cmd w/ name: %s, version %d, port: %d, ttl: %d",
							receivedname, receivedversion, receivedport,receivedttl);
					if(preg) {
						ksservtcp_register_name_set(preg, receivedname);
						ksservtcp_register_version_set(preg, receivedversion);
						ksservtcp_register_port_set(preg, receivedport);
						ksservtcp_register_timetolife_set(preg, receivedttl);
						ksservtcp_register_process_set(preg, 1);
					} else {
						ksserv_logfile_error("NO REGISTER-OBJECT");
						free(xdr_received);
						free(buffer);
						free(receivedname);
						return;
					}
				 	//generate answer
				 	size_return = 32;
				 	xdr_return = (char*)malloc(size_return);
				 	memset(xdr_return, 0, size_return);
				 	for(j=0;j<4;j++)
			 			xdr_return[j] = xid2[j];

			 		xdr_return[7] = 1;
					free(receivedname);
			 		break;
				 case 65282:	//unregister
			 		//get name
				 	for(j=0;j<4;j++)
		 				tmp[3-j] = xdr_received[44+j];
		 			memcpy(&receivednamelength, tmp, 4);
		 			receivednamelengthxdr = receivednamelength;
		 			while ((receivednamelengthxdr%4)!=0)
		 				receivednamelengthxdr++;
		 			receivedname = (char*)malloc(receivednamelength+1);
		 			memset(receivedname, 0, receivednamelength+1);
		 			for(j=0;j<receivednamelength;j++)
		 				receivedname[j]= xdr_received[48+j];
		 			//get version
		 			for(j=0;j<4;j++)
		 				tmp[3-j] = xdr_received[48+receivednamelengthxdr+j];
		 			memcpy(&receivedversion, tmp, 4);
		 			//set unregister object
		 			ksserv_logfile_info("TCPClient: UNREGISTER cmd w/ name: %s, version %d", receivedname, receivedversion);
		 			if(pureg) {
						ksservtcp_unregister_name_set(pureg, receivedname);
						ksservtcp_unregister_version_set(pureg, receivedversion);
						ksservtcp_unregister_process_set(pureg, 1);
					} else {
						ksserv_logfile_error("NO UNREGISTER-OBJECT");
						free(xdr_received);
						free(buffer);
						free(receivedname);
						return;
					}
				 	//generate answer
				 	size_return = 32;
				 	xdr_return = (char*)malloc(size_return);
				 	memset(xdr_return, 0, size_return);
				 	for(j=0;j<4;j++)
		 				xdr_return[j] = xid2[j];
		 			xdr_return[7] = 1;
		 			if(pureg->v_res == -2)
		 				xdr_return[31] = 0x21;
					free(receivedname);
			 		break;
				 case 65283:	//getserver
			 		//get name
				 	for(j=0;j<4;j++)
		 				tmp[3-j] = xdr_received[44+j];
		 			memcpy(&receivednamelength, tmp, 4);
		 			receivednamelengthxdr = receivednamelength;
		 			while ((receivednamelengthxdr%4)!=0)
		 				receivednamelengthxdr++;
		 			receivedname = (char*)malloc(receivednamelength+1);
		 			memset(receivedname, 0, receivednamelength+1);
		 			for(j=0;j<receivednamelength;j++)
		 				receivedname[j]= xdr_received[48+j];
				 	//get version
		 			for(j=0;j<4;j++)
		 				tmp[3-j] = xdr_received[48+receivednamelengthxdr+j];
		 			memcpy(&receivedversion, tmp, 4);
		 			//set getserverdata object
		 			ksserv_logfile_info("TCPClient: %d GETSERVER cmd w/ name: %s, version %d", receivednamelengthxdr, receivedname, receivedversion);
					if(pgsd) {
						ksservtcp_getserverdata_name_set(pgsd, receivedname);
						ksservtcp_getserverdata_version_set(pgsd, receivedversion);
						ksservtcp_getserverdata_process_set(pgsd, 1);
					} else {
						ksserv_logfile_error("NO GETSERVERDATA-OBJECT");
						free(xdr_received);
						free(buffer);
						free(receivedname);
						return;
					}
		 			//generate answer
		 			if(pgsd->v_res == 1){
						receivedversion = pgsd->v_version;
						receivedport = pgsd->v_port;
		 				secs = pgsd->v_expirationtime.secs;
		 				usecs = pgsd->v_expirationtime.usecs;
		 				ksserv_logfile_info("TCPClient: ANSWER GETSERVERtcp w/ name: %s, version: %d, port: %d, secs: %d, usecs: %d",
								receivedname, receivedversion, receivedport, secs, usecs);
						size_return = 60+receivednamelengthxdr; //martin 56
			 			xdr_return = (char*)malloc(size_return);
					 	memset(xdr_return, 0, size_return);

						//add tcp-fragment-rpcheader
						tmpfragment = (char*)&(size_return);
						for (c=0; c<4; c++)
								xdr_return[3-c] = tmpfragment[c];
						xdr_return[0] = 0x80;
						//xid
			 			for(j=0;j<4;j++)
		 					xdr_return[4+j] = xid2[j];//martin 0+
			 			//set type to reply
		 				xdr_return[11] = 1;
		 				//length of servername
			 			buf = (char*)&receivednamelength;
			 			memcpy(tmp, buf, 4);
			 			for (j=0; j<4; j++)
			 				xdr_return[39-j] = tmp[j];//martin 35
			 			//name itself
			 			for(j=0;j<receivednamelength;j++)
			 				xdr_return[40+j] = receivedname[j];//martin 36
			 			//version
			 			buf = (char*)&receivedversion;
			 			memcpy(tmp, buf, 4);
			 			for (j=0; j<4; j++)
			 				xdr_return[43+receivednamelengthxdr-j] = tmp[j];//martin39
			 			//port
			 			buf = (char*)&receivedport;
			 			memcpy(tmp, buf, 4);
			 			for (j=0; j<4; j++)
			 				xdr_return[47+receivednamelengthxdr-j] = tmp[j];//martin43
			 			//secs to live
			 			buf = (char*)&secs;
			 			memcpy(tmp, buf, 4);
			 			for (j=0; j<4; j++)
			 				xdr_return[51+receivednamelengthxdr-j] = tmp[j];//martin47
			 			//usecs to live
				 		buf = (char*)&usecs;
			 			memcpy(tmp, buf, 4);
			 			for (j=0; j<4; j++)
			 				xdr_return[55+receivednamelengthxdr-j] = tmp[j];//martin51
			 			//whats that for?
			 			xdr_return[size_return-1] = 1;
					} else { // no entry for server name found
						size_return = 36;
			 			xdr_return = (char*)malloc(size_return);
				 		memset(xdr_return, 0, size_return);
						//add tcp-fragment-rpcheader
						tmpfragment = (char*)&(size_return);
						for (c=0; c<4; c++)
								xdr_return[3-c] = tmpfragment[c];
						xdr_return[0] = 0x80;
						//xid
				 		for(j=0;j<4;j++)
		 					xdr_return[4+j] = xid2[j];
				 		//set type to reply
		 				xdr_return[11] = 1;
					}
		 			xdr_return[size_return-1] = 0x21;
		 			//print xdr
		 			//KSDEVEL printf("\nxdr (reply tcpclient):\nlength: %d\n", size_return);
		 			//KSDEVEL for (j = 0; j < size_return; j=j+4)
		 				//KSDEVEL printf("%X %X %X %X     ", xdr_return[j], xdr_return[j+1], xdr_return[j+2], xdr_return[j+3]);
		 			//KSDEVEL printf("\n\n");
					free(receivedname);
			 		break;
				 default:	//no manager command

	 				ksserv_logfile_info("TCPClient/typemethod: normal KS Command execution --> xdrexec");

	 				xdrmem_create(&xdrs, xdr_received , size_received, XDR_DECODE);
					xdrmemstream_create(&resultXDRs);
	 				ksserv_xdrexec(&xdrs, &resultXDRs, (OV_INSTPTR_ksserv_Client)this);
					//ksserv_xdrexec(&xdrs, &resultXDRs);
					xdrmemstream_rewind(&resultXDRs, XDR_DECODE);

{
	xdrmemstream_fragment_description descr;
	unsigned long count = 0;
	unsigned long total_size = 0;
	int len = 0;
	
	if(xdrmemstream_get_length(&resultXDRs, &len))
		ksserv_logfile_debug("resultXDR-length: %d", len);
	else
		ksserv_logfile_debug("xdrmemstream_get_length failed, len: %d", len);
		
	xdrmemstream_get_fragments(&resultXDRs, &descr, &count, &total_size);
		ksserv_logfile_info("result_XDRs fragment count: %d,\ntotal size: %d\n fragment description length: %d",
			count, total_size, descr.length);
}

				//copy xdr-data
				//set size_return to size of memstream + 28
//					size_return = (&resultXDRs)->x_handy+28;
					xdrmemstream_get_length(&resultXDRs, &size_return);
					size_return+=28;
		ksserv_logfile_debug("size_return set: %d", size_return);
				//allocate memory	
					xdr_return = (char*)malloc(size_return);
					buffer = (char*)realloc(buffer, size_return); //reuse buffer for returning answer
		ksserv_logfile_debug("buffer allocated: %p", buffer);		
				//initialize pointer for later iterations	
					placeInBuffer = buffer;
				//set currFragment to first Fragments address
					currFragment = ((MemoryStreamInfo*)((&resultXDRs)->x_base))->first;
				//iterate over fragments
					do
					{
					//copy fragment to buffer
						memcpy(placeInBuffer, &(currFragment->dummy), currFragment->used);
	ksserv_logfile_debug("fragment copied, currFragment->used: %d", currFragment->used);											
						//increment Bufferpointer	
						placeInBuffer = &(placeInBuffer[currFragment->used]);
					//set currFragment to the next one
						currFragment = currFragment->next;
	ksserv_logfile_debug("next fragment: %p", currFragment);
					} while(currFragment);
					
				
				//reset xdr_return
					memset(xdr_return, 0, size_return);
					//clear xdr?
					MemStreamDestroy(&resultXDRs);

					xdrlret = size_return-4;
					temp = (char*)&(xdrlret);
					for (i=0; i<4; i++)
						rpcheader[3-i] = temp[i];
		
					memcpy(xdr_return, rpcheader, 4);
					xdr_return[0] = 0x80;
					for (i=0; i<4; i++)
						xdr_return[i+4] = xid2[i];
					xdr_return[11] = 0x1;
					for (i=0; i<(size_return-32); i++)
						xdr_return[i+32] = buffer[i+4];
				break;
		} //close swtich REGISTER UNREGISTER GETSERVER KScmd
		
			//print xdr_return
			//KSDEVEL printf("\nsize_return: %d\n", size_return+28);
			//KSDEVEL int j=0;
			//KSDEVEL printf("\n\nxdr_return:\n");
			//KSDEVEL for (j = 0; j < (size_return+28); j=j+4)
				//KSDEVEL printf("%X %X %X %X     ", xdr_return[j], xdr_return[j+1], xdr_return[j+2], xdr_return[j+3]);
			//KSDEVEL printf("\n\n");
			//KSDEVEL for (j = 0; j < (size_return+28); j=j+4)
				//KSDEVEL printf("%c %c %c %c     ", xdr_return[j], xdr_return[j+1], xdr_return[j+2], xdr_return[j+3]);
			//KSDEVEL printf("\n\n");
			//KSDEVEL printf("%s\n", xdr_return);
		
			//send xdr to client, iterate over buffer if it cant be sent atomically
	ksserv_logfile_debug("tcpclient: sending answer: %d bytes", (size_return));
			//initialize send pointer with buffer
			placeInBuffer = xdr_return;
			do
			{
				if((size_return - sentBytes) > 4096)
				{
					sentChunkSize = send(receivesocket, placeInBuffer, 4096, 0);
					if (sentChunkSize == -1) 
					{
						ksserv_logfile_error("send() failed");
						break;
					}
				}
				else
				{
					sentChunkSize = send(receivesocket, placeInBuffer, (size_return - sentBytes), 0);
					if (sentChunkSize == -1) 
					{
						ksserv_logfile_error("send() failed");
						break;
					}
				}
				sentBytes += sentChunkSize;
	ksserv_logfile_debug("tcpclient: answer sent, sentChunkSize: %d\nsentBytes: %d", sentChunkSize, sentBytes);
				//move pointer to next chunk
				placeInBuffer = &(xdr_return[sentBytes]);
				
				if(sentBytes < size_return)
				{
				//sleep to give time for sending (sleep values are trial and error based for the development PC (Intel C600))
#if !OV_SYSTEM_NT				
				//on linux usleep counts in usecs
					usleep(1000);
#else
				//on windows Sleep counts in msecs 
					Sleep(1);
#endif					
				}
			}while(sentBytes < size_return);

			//client isnt any more the sender
			ksserv_Client_unsetThisAsCurrent((OV_INSTPTR_ksserv_Client)this); //unset this as current one
			free(xdr_return);
			free(xdr_received);
	} // else recv got bytes - closed
	//ksserv_logfile_debug("free xdrereturn");
	//free(xdr_return);
	//ksserv_logfile_debug("free xdrreceived");
	//free(xdr_received);
	//ksserv_logfile_debug("free receivedname");
	//free(receivedname);
	free(buffer);
	return;
}
