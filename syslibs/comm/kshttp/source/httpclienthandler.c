/*
 *	Copyright (C) 2015
 *	Chair of Process Control Engineering,
 *	Aachen University of Technology.
 *	All rights reserved.
 *
 *	Redistribution and use in source and binary forms, with or without
 *	modification, are permitted provided that the following conditions
 *	are met:
 *	1. Redistributions of source code must retain the above copyright
 *	   notice, this list of conditions and the following disclaimer.
 *	2. Redistributions in binary form must print or display the above
 *	   copyright notice either during startup or must have a means for
 *	   the user to view the copyright notice.
 *	3. Redistributions in binary form must reproduce the above copyright
 *	   notice, this list of conditions and the following disclaimer in
 *		the documentation and/or other materials provided with the
 *		distribution.
 *	4. Neither the name of the Chair of Process Control Engineering nor
 *		the name of the Aachen University of Technology may be used to
 *		endorse or promote products derived from this software without
 *		specific prior written permission.
 *
 *	THIS SOFTWARE IS PROVIDED BY THE CHAIR OF PROCESS CONTROL ENGINEERING
 *	``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *	LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *	A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE CHAIR OF
 *	PROCESS CONTROL ENGINEERING BE LIABLE FOR ANY DIRECT, INDIRECT,
 *	INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 *	BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 *	OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *	AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 *	LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY
 *	WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *	POSSIBILITY OF SUCH DAMAGE.
 *
 ***********************************************************************/

#ifndef OV_COMPILE_LIBRARY_kshttp
#define OV_COMPILE_LIBRARY_kshttp
#endif

#include "config.h"

/**
 * This function handles requests received over a channel. It reads data as an http-stream and triggers the appropriate internal functions
 */
OV_DLLFNCEXPORT OV_RESULT kshttp_httpclienthandler_HandleRequest(
	OV_INSTPTR_ksbase_ClientHandler baseClientHandler,
	OV_INSTPTR_ksbase_Channel pChannel,
	KS_DATAPACKET* dataReceived,
	KS_DATAPACKET* answer
) {
	OV_INSTPTR_kshttp_httpclienthandler this = Ov_StaticPtrCast(kshttp_httpclienthandler, baseClientHandler);
	OV_RESULT result = OV_ERR_OK;

	switch (this->v_CommunicationStatus) {
		case HTTP_CS_INITIAL:
			result = kshttp_httpclienthandler_analyzeRequestHeader(&this->v_ClientRequest, &this->v_ServerResponse, &this->v_CommunicationStatus, pChannel->v_inData.data, pChannel->v_inData.length);
			if(this->v_CommunicationStatus != HTTP_CS_REQUESTHEADERPARSED){
				//wait for more data
				return OV_ERR_OK;
			}
			if(Ov_Fail(result)) return result;
			//no break wanted
		case HTTP_CS_REQUESTHEADERPARSED :
			result = kshttp_httpclienthandler_analyzeRequestBody(&this->v_ClientRequest, &this->v_ServerResponse, &this->v_CommunicationStatus, pChannel->v_inData.data, pChannel->v_inData.length);
			if(this->v_CommunicationStatus != HTTP_CS_REQUESTPARSED){
				//wait for more data
				return OV_ERR_OK;
			}
			if(Ov_Fail(result)) return result;
			//empty the buffers
			ksbase_free_KSDATAPACKET(&pChannel->v_inData);
			// the http-part of the message is valid --> we received a service-request --> increment receivedCalls
			this->v_receivedCalls++;
			//no break wanted
		case HTTP_CS_REQUESTPARSED :
			result = kshttp_httpclienthandler_generateHttpBody(this->v_ClientRequest, &this->v_ServerResponse, &this->v_CommunicationStatus, pChannel, this);
			if(this->v_CommunicationStatus == HTTP_CS_CHANNELRESPONSIBILITYRELEASED){
				//we reject every further work on this channel
				return OV_ERR_OK;
			}
			//no break wanted
		case HTTP_CS_RESPONSEBODYGENERATED :
			result = kshttp_httpclienthandler_generateHttpHeader(this->v_ClientRequest, &this->v_ServerResponse, &this->v_CommunicationStatus, result, NULL);

			if(Ov_Fail(result)) return result;
			//no break wanted
		case HTTP_CS_RESPONSEHEADERGENERATED :
			result = kshttp_httpclienthandler_sendHttpHeader(this->v_ClientRequest, &this->v_ServerResponse, &this->v_CommunicationStatus, pChannel);
			if(Ov_Fail(result)) return result;
			//no break wanted
		case HTTP_CS_RESPONSEHEADERSEND :
			result = kshttp_httpclienthandler_sendHttpBody(this->v_ClientRequest, &this->v_ServerResponse, &this->v_CommunicationStatus, pChannel);
			Ov_HeapFree(this->v_ClientRequest.messageBody);
			this->v_ClientRequest.messageBody = NULL;

			Ov_DbFree(this->v_ServerResponse.contentBinary);
			this->v_ServerResponse.contentBinary = NULL;
			return result;

		case HTTP_CS_CHANNELRESPONSIBILITYRELEASED :
			KS_logfile_error(("%s: HandleRequest: we have no responsibility for this channel, but got called! 8-(", this->v_identifier));
			return OV_ERR_NOTIMPLEMENTED;
		case HTTP_CS_SHUTDOWN :
			Ov_DeleteObject(this);
			return OV_ERR_OK;
		default:
			KS_logfile_error(("%s: HandleRequest: no matching communication status found: %d. aborting.", this->v_identifier, this->v_CommunicationStatus));
			break;
	}
	return result;
}

/**
 * builds up the body with the KS commands, Extention or static files
 */
DLLFNCEXPORT OV_RESULT kshttp_httpclienthandler_generateHttpBody(
		const HTTP_REQUEST request,
		HTTP_RESPONSE *response,
		HTTP_COMMUNICATIONSTATUS *communicationStatus,
		OV_INSTPTR_ksbase_Channel pChannel,
		OV_INSTPTR_kshttp_httpclienthandler this
) {
	OV_RESULT result = OV_ERR_OK;
	OV_INSTPTR_kshttp_httpIdentificator pIdentificator = Ov_StaticPtrCast(kshttp_httpIdentificator, Ov_GetFirstChild(ov_instantiation, pclass_kshttp_httpIdentificator));
	OV_INSTPTR_kshttp_httpClientHandlerExtension pExtension = NULL;
	OV_VTBLPTR_kshttp_httpClientHandlerExtension pVtblClientHandlerExtension = NULL;
	OV_INSTPTR_ov_class pExtensionClass = NULL;
	OV_INSTPTR_kshttp_getvarpushhandler pPushhandler = NULL;
	OV_BOOL extclientfound = FALSE;
	OV_UINT iterator;
	OV_STRING_VEC match = {0,NULL};

	OV_INSTPTR_kshttp_staticfile pStaticfile;


	response->requestHandledBy = KSHTTP_RGB_NONE;

	if(KSX == request.response_format){
		ov_string_setvalue(&response->contentType, "text/xml");
	}else if(KSJSON == request.response_format){
		ov_string_setvalue(&response->contentType, "application/json");
	}else{
		ov_string_setvalue(&response->contentType, "text/plain");
	}
	ov_string_setvalue(&response->contentEncoding, "windows-1252");

	//allow javascript connection from any source (CORS)
	ov_string_setvalue(&response->header, "Access-Control-Allow-Origin:*\r\n");

	if(Ov_OK(result) && ov_string_compare(request.requestMethod, "OPTIONS") == OV_STRCMP_EQUAL){
		//some "fishy" requests are not send to us, but the browser ask us with a preflight of Cross-Origin Resource Sharing (CORS)

		//CORS is not easy with non standard headers, but we have not problem with other custom headers:
		//hmi uses if-modified-since
		//POST has content-type
		ov_string_append(&response->header, "Access-Control-Allow-Headers: if-modified-since, content-type\r\nAccess-Control-Max-Age: 60\r\n");

		//we must allow all Methods to make some browsers happy
		if(pIdentificator != NULL && pIdentificator->v_AllowedMethods.veclen > 0){
			//quite silly check, but better be safe

			ov_string_append(&response->header, "Access-Control-Allow-Methods: ");
			for(iterator = 0;iterator < pIdentificator->v_AllowedMethods.veclen;iterator++){
				ov_string_append(&response->header, pIdentificator->v_AllowedMethods.value[iterator]);
				if(iterator != pIdentificator->v_AllowedMethods.veclen -1 && pIdentificator->v_AllowedMethods.value[iterator] != NULL){
					//there are more methods in the Vector and this value was not NULL
					ov_string_append(&response->header, ", ");
				}
			}
			ov_string_append(&response->header, "\r\n");
		}

		//" "
		//only an 200 is required, so abort request handling
		response->requestHandledBy = KSHTTP_RGB_CORSOPTION;
	}

	//BEGIN command routine
	if(Ov_OK(result) && response->requestHandledBy == KSHTTP_RGB_NONE){
		if(ov_string_compare(request.urlPath, "/getServer") == OV_STRCMP_EQUAL){
			//http GET
			kshttp_printresponseheader(&response->contentString, request.response_format, "getserver");
			result = kshttp_exec_getserver(request, response);
			kshttp_printresponsefooter(&response->contentString, request.response_format, "getserver");
			response->requestHandledBy = KSHTTP_RGB_GETSERVER;
		}else if(ov_string_compare(request.urlPath, "/register") == OV_STRCMP_EQUAL){
			kshttp_printresponseheader(&response->contentString, request.response_format, "register");
			if(!pChannel->v_isLocal){
				result = KS_ERR_NOREMOTE;
				kshttp_print_result_array(&response->contentString, request.response_format, &result, 1, ": Registering remote is not allowed/useful");
			}else{
				//name, port, ksversion
				result = kshttp_exec_register(request, response);
			}
			kshttp_printresponsefooter(&response->contentString, request.response_format, "register");
			response->requestHandledBy = KSHTTP_RGB_REGISTER;
		}else if(ov_string_compare(request.urlPath, "/unregister") == OV_STRCMP_EQUAL){
			//name, port, ksversion
			kshttp_printresponseheader(&response->contentString, request.response_format, "unregister");
			if(!pChannel->v_isLocal){
				result = KS_ERR_NOREMOTE;
				kshttp_print_result_array(&response->contentString, request.response_format, &result, 1, ": Registering remote is not allowed/useful");
			}else{
				result = kshttp_exec_unregister(request, response);
			}
			kshttp_printresponsefooter(&response->contentString, request.response_format, "unregister");
			response->requestHandledBy = KSHTTP_RGB_UNREGISTER;
		}else if(ov_string_compare(request.urlPath, "/getVar") == OV_STRCMP_EQUAL){
			//http GET
			//server push requested?
			kshttp_find_arguments(&request.urlQuery, "stream", &match);
			if(match.veclen == 0){
				kshttp_printresponseheader(&response->contentString, request.response_format, "getvar");
				result = kshttp_exec_getvar(request, response);
				kshttp_printresponsefooter(&response->contentString, request.response_format, "getvar");

				response->requestHandledBy = KSHTTP_RGB_GETVAR;
			}else{
				//make a new push handler. This handler will send the header and hand the further connection over
				result = Ov_CreateIDedObject(kshttp_getvarpushhandler, pPushhandler, pChannel, "kshttpEvent");
				if(Ov_Fail(result)){
					result = OV_ERR_TARGETGENERIC;
				}
				//call every second
				ksbase_ComTask_cycInterval_set(Ov_PtrUpCast(ksbase_ComTask, pPushhandler), 1000);
				ksbase_ComTask_actimode_set(Ov_PtrUpCast(ksbase_ComTask, pPushhandler), 1);

				//configure the pushhandler
				pPushhandler->v_responseformat = request.response_format;
				Ov_SetDynamicVectorValue(&pPushhandler->v_urlQuery, request.urlQuery.value, request.urlQuery.veclen, STRING);

				//move assoc to the new object
				Ov_Unlink(ksbase_AssocChannelClientHandler, pChannel, Ov_GetFirstChild(ksbase_AssocChannelClientHandler, pChannel));
				result = Ov_Link(ksbase_AssocChannelClientHandler, pChannel, pPushhandler);
				if(Ov_Fail(result)){
					KS_logfile_error(("%s: could not link ClientHandler %s to Channel %s", this->v_identifier, pPushhandler->v_identifier, pChannel->v_identifier));
					return result;
				}

				//configure our handler to send the header and delete itself after that
				ov_string_setvalue(&response->contentType, "text/event-stream");
				//send a comment that we will deliver results soon
				ov_string_setvalue(&response->contentString, ": getvar push started\r\n");
				response->requestHandledBy = KSHTTP_RGB_GETVARPUSH;

				//deactivate compression if push is used
				response->compressionGzip = FALSE;
				//prevent closing the connection
				response->keepAlive = TRUE;
				//The spec of Server-Sent Events says: "Event streams are always decoded as UTF-8. There is no way to specify another character encoding." So we have to lie here 8-/
				ov_string_setvalue(&response->contentEncoding, "utf-8");
			}
			Ov_SetDynamicVectorLength(&match,0,STRING);
		}else if(ov_string_compare(request.urlPath, "/setVar") == OV_STRCMP_EQUAL){
			//http PUT, used in OData or PROPPATCH, used in WebDAV
			//add Access-Control-Allow-Methods to OPTIONS if implement other Methods
			kshttp_printresponseheader(&response->contentString, request.response_format, "setvar");
			result = kshttp_exec_setvar(request, response);
			kshttp_printresponsefooter(&response->contentString, request.response_format, "setvar");
			response->requestHandledBy = KSHTTP_RGB_SETVAR;
		}else if(ov_string_compare(request.urlPath, "/getEP") == OV_STRCMP_EQUAL){
			//http PROPFIND, used in WebDAV
			kshttp_printresponseheader(&response->contentString, request.response_format, "getep");
			result = kshttp_exec_getep(request, response);
			kshttp_printresponsefooter(&response->contentString, request.response_format, "getep");
			response->requestHandledBy = KSHTTP_RGB_GETEP;
		}else if(ov_string_compare(request.urlPath, "/createObject") == OV_STRCMP_EQUAL){
			//http PUT, used in WebDAV
			kshttp_printresponseheader(&response->contentString, request.response_format, "createobject");
			result = kshttp_exec_createObject(request, response);
			kshttp_printresponsefooter(&response->contentString, request.response_format, "createobject");
			response->requestHandledBy = KSHTTP_RGB_CREATEOBJECT;
		}else if(ov_string_compare(request.urlPath, "/deleteObject") == OV_STRCMP_EQUAL){
			//http DELETE, used in WebDAV
			kshttp_printresponseheader(&response->contentString, request.response_format, "deleteobject");
			result = kshttp_exec_deleteObject(request, response);
			kshttp_printresponsefooter(&response->contentString, request.response_format, "deleteobject");
			response->requestHandledBy = KSHTTP_RGB_DELETEOBJECT;
		}else if(ov_string_compare(request.urlPath, "/renameObject") == OV_STRCMP_EQUAL){
			//http MOVE, used in WebDAV
			kshttp_printresponseheader(&response->contentString, request.response_format, "renameobject");
			result = kshttp_exec_renameObject(request, response);
			kshttp_printresponsefooter(&response->contentString, request.response_format, "renameobject");
			response->requestHandledBy = KSHTTP_RGB_RENAMEOBJECT;
		}else if(ov_string_compare(request.urlPath, "/link") == OV_STRCMP_EQUAL){
			//http LINK
			kshttp_printresponseheader(&response->contentString, request.response_format, "link");
			result = kshttp_exec_link(request, response);
			kshttp_printresponsefooter(&response->contentString, request.response_format, "link");
			response->requestHandledBy = KSHTTP_RGB_LINK;
		}else if(ov_string_compare(request.urlPath, "/unlink") == OV_STRCMP_EQUAL){
			//http UNLINK
			kshttp_printresponseheader(&response->contentString, request.response_format, "unlink");
			result = kshttp_exec_unlink(request, response);
			kshttp_printresponsefooter(&response->contentString, request.response_format, "unlink");
			response->requestHandledBy = KSHTTP_RGB_UNLINK;
		}else if(ov_string_compare(request.urlPath, "/getLogfile") == OV_STRCMP_EQUAL){
			kshttp_printresponseheader(&response->contentString, request.response_format, "logfile");
			result = kshttp_exec_getlogfile(request, response);
			kshttp_printresponsefooter(&response->contentString, request.response_format, "logfile");
			response->requestHandledBy = KSHTTP_RGB_GETLOGFILE;
		}else if(ov_string_compare(request.urlPath, "/auth") == OV_STRCMP_EQUAL){
			result = kshttp_authorize(1, request.requestHeader, &response->header, request.requestMethod, request.urlPath);
			if(!Ov_Fail(result)){
				ov_string_append(&response->contentString, "Secret area");
				result = OV_ERR_OK;
			}
			response->requestHandledBy = KSHTTP_RGB_HTTPAUTH;
		}else{
			/*	iterate over all httpClientHandlerExtension	*/
			Ov_ForEachChildEx(ov_inheritance, pclass_kshttp_httpClientHandlerExtension, pExtensionClass, ov_class){
				Ov_ForEachChildEx(ov_instantiation, pExtensionClass, pExtension, kshttp_httpClientHandlerExtension){
					for(iterator = 0; iterator < pExtension->v_commandList.veclen; iterator++){
						if(ov_string_match(request.urlPath, pExtension->v_commandList.value[iterator]) == TRUE){
							extclientfound = TRUE;
							break;
						}
					}
					if(extclientfound == TRUE){
						break;
					}
				}
				if(extclientfound == TRUE){
					break;
				}
			}
			if(extclientfound == TRUE){
				KS_logfile_debug(("%s: HandleRequest: processing extension (%s) by protocolExtension %s", this->v_identifier, request.urlPath, pExtension->v_identifier));
				Ov_GetVTablePtr(kshttp_httpClientHandlerExtension, pVtblClientHandlerExtension, pExtension);
				if(!pVtblClientHandlerExtension){
					KS_logfile_error(("%s: HandleRequest: vtable pointer of %s not found. aborting. %s", this->v_identifier, pExtension->v_identifier));
				}else{
					result = pVtblClientHandlerExtension->m_HandleExtendedRequest(
							pExtension,
							this,
							pChannel,
							request,
							response);

					response->requestHandledBy = KSHTTP_RGB_CLIENTHANDLEREXT;
					if(Ov_Fail(result)){
						KS_logfile_error(("%s: HandleRequest: processing service %s by %s failed with error %s", this->v_identifier, request.urlPath, pExtension->v_identifier, ov_result_getresulttext(result)));
					}else{
						if(*communicationStatus == HTTP_CS_CHANNELRESPONSIBILITYRELEASED){
							//we reject every further work on this channel
							return OV_ERR_OK;
						}
					}
				}
			} else {
				//perhaps we have more luck with an static file
			}
		}
	}
	//END command routine

	//BEGIN static file routine
	//no command matched yet... Is it a static file?
	if(!Ov_Fail(result) && response->requestHandledBy == KSHTTP_RGB_NONE){
		OV_STRING filename = NULL;
		OV_STRING filepath = NULL;
		//assume index.html as a root file
		if(ov_string_compare("/", request.urlPath) == OV_STRCMP_EQUAL){
			ov_string_setvalue(&filename, "index.html");
		}else if(request.urlPath[ov_string_getlength(request.urlPath)-1] == '/'){
			//remove leading /
			ov_string_print(&filename, "%s%s", request.urlPath+1, "index.html");
		}else{
			//remove leading /
			ov_string_setvalue(&filename, request.urlPath + 1);
		}

		ov_memstack_lock();
		//a dot in a filename is represented via a percent notation in an identifier, so we need
		//to change the parameter. A directory should be possible, so we need to skip / in conversion
		ov_string_print(&filepath, "/data/kshttp/%s", kshttp_ov_path_topercent_noslash(filename));
		ov_memstack_unlock();
		pStaticfile = Ov_DynamicPtrCast(kshttp_staticfile, ov_path_getobjectpointer(filepath, 2));
		ov_string_setvalue(&filepath, NULL);
		ov_string_setvalue(&filename, NULL);	//usage is save here, as we had no memory in the database or heap

		if(pStaticfile != NULL){
			ov_string_setvalue(&response->contentType, pStaticfile->v_mimetype);
			ov_string_setvalue(&response->contentEncoding, pStaticfile->v_encoding);
			ov_string_setvalue(&response->contentString, pStaticfile->v_content);
			response->requestHandledBy = KSHTTP_RGB_STATICFILE;
			result = OV_ERR_OK;
		}
	}
	//END static file routine

	//no method has found a hit
	if (response->requestHandledBy == KSHTTP_RGB_NONE){
		result = OV_ERR_BADPATH; //404
		kshttp_printresponseheader(&response->contentString, request.response_format, "notimplemented");
		kshttp_print_result_array(&response->contentString, request.response_format, &result, 1, ": KS command not supported or static file not found");
		kshttp_printresponsefooter(&response->contentString, request.response_format, "notimplemented");
	}

	*communicationStatus = HTTP_CS_RESPONSEBODYGENERATED;

	return result;
}

/**
 * Sends the header from response->header via the channel
 */
DLLFNCEXPORT OV_RESULT kshttp_httpclienthandler_sendHttpHeader(
	const HTTP_REQUEST request,
	HTTP_RESPONSE *response,
	HTTP_COMMUNICATIONSTATUS *communicationStatus,
	OV_INSTPTR_ksbase_Channel pChannel
){
	if(response->header == NULL){
		return OV_ERR_OK;
	}

	KS_logfile_debug(("httpclienthandler: sending header: %" OV_PRINT_INT " bytes", ov_string_getlength(response->header)));
	ksbase_KSDATAPACKET_append(&pChannel->v_outData, (OV_BYTE*)response->header, ov_string_getlength(response->header));
	//note: this does not send the request

	ov_string_setvalue(&response->header, NULL);

	*communicationStatus = HTTP_CS_RESPONSEHEADERSEND;
	return OV_ERR_OK;
}

/**
 * Sends the content from response->contentBinary via the channel
 */
DLLFNCEXPORT OV_RESULT kshttp_httpclienthandler_sendHttpBody(
	const HTTP_REQUEST request,
	HTTP_RESPONSE *response,
	HTTP_COMMUNICATIONSTATUS *communicationStatus,
	OV_INSTPTR_ksbase_Channel pChannel
){
	//in case of a HEAD request there is no need to send the body
	if(response->contentLength > 0){
		KS_logfile_debug(("httpclienthandler: sending body: %" OV_PRINT_INT " bytes", response->contentLength));
		//this does not send the request
		ksbase_KSDATAPACKET_append(&pChannel->v_outData, response->contentBinary, response->contentLength);
	}
	*communicationStatus = HTTP_CS_RESPONSEBODYSEND;

	if(response->requestHandledBy == KSHTTP_RGB_GETVARPUSH){
		//further communication is moved to the push object
		pChannel->v_CloseAfterSend = FALSE;
		*communicationStatus = HTTP_CS_SHUTDOWN;
	}else if (response->keepAlive == TRUE) {
		pChannel->v_CloseAfterSend = FALSE;
		*communicationStatus = HTTP_CS_INITIAL;
	}else{
		//shutdown tcp connection if no keep_alive was set
		pChannel->v_CloseAfterSend = TRUE;
		*communicationStatus = HTTP_CS_SHUTDOWN;
	}

	return OV_ERR_OK;
}

/**
 * inits the pointers
 */
OV_DLLFNCEXPORT void kshttp_httpclienthandler_startup(
	OV_INSTPTR_ov_object 	pobj
) {
	/*
	*   local variables
	*/
	OV_INSTPTR_kshttp_httpclienthandler thisCl = Ov_StaticPtrCast(kshttp_httpclienthandler, pobj);

	/* do what the base class does first */
	ksbase_ClientHandler_startup(pobj);

	thisCl->v_CommunicationStatus = HTTP_CS_INITIAL;

	// initial strings
	thisCl->v_ClientRequest.urlQuery.veclen = 0;
	thisCl->v_ClientRequest.urlQuery.value = NULL;
	thisCl->v_ClientRequest.urlPath = NULL;
	thisCl->v_ClientRequest.host = NULL;
	thisCl->v_ClientRequest.requestHeader = NULL;
	thisCl->v_ClientRequest.requestMethod = NULL;
	thisCl->v_ClientRequest.httpVersion = NULL;
	thisCl->v_ClientRequest.Accept = NULL;

	thisCl->v_ClientRequest.contentLength = 0;

	thisCl->v_ClientRequest.response_format = FORMATUNDEFINED;

	thisCl->v_ServerResponse.httpVersion = NULL;
	thisCl->v_ServerResponse.statusCode = 0;
	thisCl->v_ServerResponse.headerLength = 0;
	thisCl->v_ServerResponse.header = NULL;
	thisCl->v_ServerResponse.contentString = NULL;
	thisCl->v_ServerResponse.contentBinary = NULL;
	thisCl->v_ServerResponse.contentLength = 0;
	thisCl->v_ServerResponse.contentType = NULL;
	thisCl->v_ServerResponse.contentEncoding = NULL;
	thisCl->v_ServerResponse.transferEncodingChunked = FALSE;
	thisCl->v_ServerResponse.compressionGzip = FALSE;

	return;
}

/**
 * frees all memory used by the structs
 */
OV_DLLFNCEXPORT void kshttp_httpclienthandler_shutdown(
	OV_INSTPTR_ov_object 	pobj
) {
	/*
	*   local variables
	*/
	OV_INSTPTR_kshttp_httpclienthandler thisCl = Ov_StaticPtrCast(kshttp_httpclienthandler, pobj);

	thisCl->v_CommunicationStatus = HTTP_CS_SHUTDOWN;

	//free memory
	Ov_SetDynamicVectorLength(&thisCl->v_ClientRequest.urlQuery, 0, STRING);
	ov_string_setvalue(&thisCl->v_ClientRequest.urlPath, NULL);
	ov_string_setvalue(&thisCl->v_ClientRequest.host, NULL);
	ov_string_setvalue(&thisCl->v_ClientRequest.requestHeader, NULL);
	ov_string_setvalue(&thisCl->v_ClientRequest.requestMethod, NULL);
	ov_string_setvalue(&thisCl->v_ClientRequest.httpVersion, NULL);
	ov_string_setvalue(&thisCl->v_ClientRequest.Accept, NULL);

	Ov_HeapFree(thisCl->v_ClientRequest.messageBody);
	thisCl->v_ClientRequest.messageBody = NULL;

	ov_string_setvalue(&thisCl->v_ServerResponse.header, NULL);
	ov_string_setvalue(&thisCl->v_ServerResponse.httpVersion, NULL);
	ov_string_setvalue(&thisCl->v_ServerResponse.contentString, NULL);
	Ov_DbFree(thisCl->v_ServerResponse.contentBinary);
	thisCl->v_ServerResponse.contentBinary = NULL;
	thisCl->v_ServerResponse.contentLength = 0;
	ov_string_setvalue(&thisCl->v_ServerResponse.contentType, NULL);

	/* set the object's state to "shut down" */
	ksbase_ClientHandler_shutdown(pobj);

	return;
}
