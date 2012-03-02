#ifndef OV_COMPILE_LIBRARY_ksservhttp
#define OV_COMPILE_LIBRARY_ksservhttp
#endif


#include "ksservhttp.h"
#include "ksserv.h"
#include "config.h"


#define REALM "Top secret! User: root Pass: pass"

//needed for md5
#include "md5.h"

//needed for random numbers
#include <stdlib.h>

#define EXTRACT_RETURN ov_string_freelist(plist);\
					   ov_string_setvalue(&temp, "");\
					   return

OV_RESULT extract(OV_STRING search, OV_STRING start, OV_STRING end, OV_STRING* result){
	OV_STRING* plist=NULL;
	OV_STRING temp=NULL;
	OV_UINT len;
	if(!strstr(search, start)){
		EXTRACT_RETURN OV_ERR_GENERIC;
	}

	plist = ov_string_split(search, start, &len);
	if(len<2){
		EXTRACT_RETURN OV_ERR_GENERIC;
	}
	ov_string_setvalue(&temp, plist[1]);

	if(!strstr(temp, end)){
		EXTRACT_RETURN OV_ERR_GENERIC;
	}
	ov_string_freelist(plist);
	plist = ov_string_split(temp, end, &len);
	if(len<1){
		EXTRACT_RETURN OV_ERR_GENERIC;
	}

	ov_string_setvalue(result, plist[0]);

	EXTRACT_RETURN OV_ERR_OK;
}

#define AUTHORIZE_RETURN ov_string_setvalue(&digestline, NULL);\
						 ov_string_setvalue(&username, NULL);\
						 ov_string_setvalue(&realm, NULL);\
						 ov_string_setvalue(&nonce, NULL);\
						 ov_string_setvalue(&qop, NULL);\
						 ov_string_setvalue(&nc, NULL);\
						 ov_string_setvalue(&cnonce, NULL);\
						 ov_string_setvalue(&response, NULL);\
						 ov_string_setvalue(&ha1, NULL);\
						 ov_string_setvalue(&ha2, NULL);\
						 ov_string_setvalue(&own_response, NULL);\
						 ov_string_setvalue(&temp, NULL);\
						 return
OV_RESULT authorize(int level, OV_INSTPTR_ksservhttp_httpclienthandler this, OV_STRING request_header, OV_STRING* reply_header, OV_STRING request_type, OV_STRING cmd){
	OV_STRING random_number=NULL;
	md5_hash_return hash;
	OV_RESULT res;

	//authorization header line
	OV_STRING  digestline = NULL;

	//strings needed for authorization
	OV_STRING  username = NULL;
	OV_STRING  realm = NULL;
	OV_STRING  nonce = NULL;
	OV_STRING  qop = NULL;
	OV_STRING  nc = NULL;
	OV_STRING  cnonce = NULL;
	OV_STRING  response = NULL;

	//temp vars
	OV_STRING  ha1 = NULL;
	OV_STRING  ha2 = NULL;
	OV_STRING  own_response = NULL;
	OV_STRING  temp = NULL;


	if(this->v_securitylevel >= level){
		AUTHORIZE_RETURN OV_ERR_OK;
	}else if(strstr(request_header, "\r\nAuthorization: Digest ")){
		//verify authorization
	    res = extract(request_header, "\r\nAuthorization: Digest ", "\r\n", &digestline);
	    if(Ov_Fail(res)){
	    	AUTHORIZE_RETURN OV_ERR_BADPARAM; //400
	    }

	    //now extract all needed information
	    res = extract(digestline, "username=\"", "\"", &username);
	    if(Ov_Fail(res)){
	    	AUTHORIZE_RETURN OV_ERR_BADPARAM; //400
	    }
	    res = extract(digestline, "realm=\"", "\"", &realm);
	    if(Ov_Fail(res)){
	    	AUTHORIZE_RETURN OV_ERR_BADPARAM; //400
	    }
	    res = extract(digestline, "nonce=\"", "\"", &nonce);
	    if(Ov_Fail(res)){
	    	AUTHORIZE_RETURN OV_ERR_BADPARAM; //400
	    }
	    res = extract(digestline, "qop=", ",", &qop);
	    if(Ov_Fail(res)){
		    res = extract(digestline, "qop=", "\r\n", &qop);
		    //try end of line since no end delimiter
		    if(Ov_Fail(res)){
		    	AUTHORIZE_RETURN OV_ERR_BADPARAM; //400
		    }
	    }
	    res = extract(digestline, "nc=", ",", &nc);
	    if(Ov_Fail(res)){
		    res = extract(digestline, "nc=", "\r\n", &nc);
		    //try end of line since no end delimiter
		    if(Ov_Fail(res)){
		    	AUTHORIZE_RETURN OV_ERR_BADPARAM; //400
		    }
	    }
	    res = extract(digestline, "cnonce=\"", "\"", &cnonce);
	    if(Ov_Fail(res)){
	    	AUTHORIZE_RETURN OV_ERR_BADPARAM; //400
	    }
	    res = extract(digestline, "response=\"", "\"", &response);
	    if(Ov_Fail(res)){
	    	AUTHORIZE_RETURN OV_ERR_BADPARAM; //400
	    }

	    if(ov_string_compare(nonce, this->v_nonce) == OV_STRCMP_EQUAL && ov_string_compare(qop, "auth") == OV_STRCMP_EQUAL){
			//lets start hashing
			// GOT info look at http://en.wikipedia.org/wiki/Digest_access_authentication
			//HA1
			//TODO: the send username is ignored atm, since only one user root exists
			ov_string_print(&temp, "%s:%s:%s", "root", REALM, "pass");
			md5_string(&hash, temp, ov_string_getlength(temp));
			ov_string_setvalue(&ha1, hash);
			//HA2
			ov_string_print(&temp, "%s:%s", request_type, cmd);
			md5_string(&hash, temp, ov_string_getlength(temp));
			ov_string_setvalue(&ha2, hash);
			//compute own response
			ov_string_print(&temp, "%s:%s:%s:%s:%s:%s", ha1, this->v_nonce, nc, cnonce, qop, ha2);
			md5_string(&hash, temp, ov_string_getlength(temp));
			ov_string_setvalue(&own_response, hash);
			//verify match
			if(ov_string_compare(own_response, response) == OV_STRCMP_EQUAL){
				this->v_securitylevel = level;
				AUTHORIZE_RETURN OV_ERR_OK;
			}
	    }
	}

	//require authorization

	//set nounce

	//srand has been called by the time of ksservhttp object creation
	ov_string_print(&random_number, "%i", rand()*rand()*rand());
	md5_string(&hash, random_number, ov_string_getlength(random_number));
	ov_string_setvalue(&(this->v_nonce), hash);
	ov_string_print(reply_header, "WWW-Authenticate: Digest realm=\"%s\",\
												    qop=\"auth\",\
													opaque=\"\",\
													nonce=\"%s\"\r\n", REALM, this->v_nonce);
	return OV_ERR_BADAUTH; //401

}
