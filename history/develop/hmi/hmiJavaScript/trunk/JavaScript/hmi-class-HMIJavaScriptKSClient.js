/*
*	Copyright (C) 2012
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
*/
/***********************************************************************
*
*	File:
*	------
*	hmi-class-HMIJavaScriptKSClient.js
*
*	Editors:
*	--------
*	St							Stefan Schmitz <StefanS@plt.rwth-aachen.de>
*
*	Je							Holger Jeromin <Holger.Jeromin@plt.rwth-aachen.de>
*
*	CVS:
*	----
*	$Revision$
*	$Date$
*
*	History:
*	--------
*	21-June-2007			St
*		-	General Revision
*
*	22-November-2007		St
*		-	added more information if MANAGER, server or sheet is not
*			available
*
*	04-April-2008			Je
*		-	auto select if only one server and Sheet
*
*	22-September-2008			Je
*		-	HMI Requests changed to getEP, getVar, setVar, getHandle, deleteCommunicationPoint
*
*	25-February-2009			Je
*		-	General Revision and full commented
*
***********************************************************************/

/***********************************************************************
	constructor
***********************************************************************/

function HMIJavaScriptKSClient() {
	/** Public **********************/
	
	this.KSCONSTANTS = Object();
	this.KSCONSTANTS.KS_VT_VOID           = "0x00000000";
	this.KSCONSTANTS.KS_VT_BOOL           = "0x00000002";
	this.KSCONSTANTS.KS_VT_INT            = "0x00000010";
	this.KSCONSTANTS.KS_VT_UINT           = "0x00000011";
	this.KSCONSTANTS.KS_VT_SINGLE         = "0x00000020";
	this.KSCONSTANTS.KS_VT_DOUBLE         = "0x00000021";
	this.KSCONSTANTS.KS_VT_STRING         = "0x00000030";
	this.KSCONSTANTS.KS_VT_TIME           = "0x00000031";
	this.KSCONSTANTS.KS_VT_TIME_SPAN      = "0x00000032";
	this.KSCONSTANTS.KS_VT_STATE          = "0x00000038";
	this.KSCONSTANTS.KS_VT_STRUCT         = "0x00000040";
	this.KSCONSTANTS.KS_VT_BYTE_VEC       = "0x00000081";
	this.KSCONSTANTS.KS_VT_BOOL_VEC       = "0x00000082";
	this.KSCONSTANTS.KS_VT_INT_VEC        = "0x00000090";
	this.KSCONSTANTS.KS_VT_UINT_VEC       = "0x00000091";
	this.KSCONSTANTS.KS_VT_SINGLE_VEC     = "0x000000a0";
	this.KSCONSTANTS.KS_VT_DOUBLE_VEC     = "0x000000a1";
	this.KSCONSTANTS.KS_VT_STRING_VEC     = "0x000000b0";
	this.KSCONSTANTS.KS_VT_TIME_VEC       = "0x000000b1";
	this.KSCONSTANTS.KS_VT_TIME_SPAN_VEC  = "0x000000b2";
	this.KSCONSTANTS.KS_VT_TIME_SERIES    = "0x000000b3";
	this.KSCONSTANTS.KS_VT_STATE_VEC      = "0x000000b8";
	
	//object to cache communication details
	this.ResourceList = Object();
	
	//needed in the gestures and SHOWSHEETS
	this.HMIMANAGER_PATH = Object();
	this.hmilib_path = Object();
	this.cshmilib_path = Object();
	
	//needed for absolute ks path on the same ksserver
	this.ResourceList.ModelHost = null;
	this.ResourceList.ModelServer = null;
	this.ResourceList.Servers = Object();
	
	/** Private *********************/
	var MessageID = 0;	
	
	/*********************************
		getMessageID
	*********************************/
	this.getMessageID = function () {
		MessageID += 1;
		return MessageID;
	};
};

/***********************************************************************
	prototype
***********************************************************************/

HMIJavaScriptKSClient.prototype = {
	/**
	 * usage example:
	 *		this.getEP(null, '/servers', 'OT_DOMAIN', 'OP_NAME', this._cbGetServers);
	 *		this.getEP(null, '/servers', 'OT_DOMAIN', ['OP_NAME', 'OP_CLASS'], this._cbGetServers);
	 * @param path of object to query
	 * @param requestType = OT_DOMAIN type of KS Object to query ("OT_DOMAIN", "OT_VARIABLE", "OT_LINK" or "OT_ANY")
	 * @param requestOutput Array of interesting objects properties ("OP_NAME", "OP_TYPE", "OP_COMMENT", "OP_ACCESS", "OP_SEMANTIC", "OP_CREATIONTIME", "OP_CLASS" or "OT_ANY")
	 * @param cbfnc callback function
	 * @param async request async communication
	 * @param responseFormat Mime-Type of requested response (probably "text/tcl", "text/ksx", "text/plain" used)
	 * @return "{fb_hmi1} {fb_hmi2} {fb_hmi3} {MANAGER} {fb_hmi4} {fb_hmi5}" or null or true (if callback used)
	 */
	getEP: function(path, requestType, requestOutput, cbfnc, async, responseFormat) {
		if(requestType === undefined || requestType === null){
			requestType = "OT_DOMAIN";
		}
		if(!requestOutput){
			requestOutput = "OP_NAME";
		}
		HMI.hmi_log_trace("HMIJavaScriptKSClient.prototype.getEP - Start: "+path);
		if(!path || path.length === 0){
			HMI.hmi_log_error("HMIJavaScriptKSClient.prototype.getEP - no path found");
			return null;
		}
		if(path.charAt(0) !== "/"){
			HMI.hmi_log_error("HMIJavaScriptKSClient.prototype.getEP - no valid path found, path was: "+path);
			return null;
		}
		if (!requestType.indexOf || requestType.indexOf("OT_") !== 0){
			HMI.hmi_log_error("HMIJavaScriptKSClient.prototype.getEP - no valid requestType found, requestType was: "+requestType);
			return null;
		}
		
		var ServerAndPath = this._splitKSPath(path);
		
		var method = "GET";
		var Handle;
		var urlparameter;
		var optionalurlparameter = "";
		if(responseFormat === undefined){
			responseFormat = "text/tcl";
		}
		if ("tcl" === HMI.HMI_Constants.ServerType){
			Handle = this.getCommunicationPoint(ServerAndPath[0]);
			if(Handle === null){
				return null;
			}
			/*
			 * possible optionalurlparameters:
			 * "%20-type%20$::TKS::OT_DOMAIN%20-output%20[expr%20$::TKS::OP_NAME%20|%20$::TKS::OP_CLASS]"
			 * multiple	-output%20[expr%20$::TKS::OP_NAME%20|%20$::TKS::OP_CLASS]
			 * one		-output%20$::TKS::OP_NAME
			 */
			optionalurlparameter = "%20-type%20$::TKS::"+requestType;
			if(Object.prototype.toString.call(requestOutput) !== "[object Array]"){
				//fail save array detection
				optionalurlparameter += "%20-output%20$::TKS::"+requestOutput;
			}else if(requestOutput.length === 1){
				//the expr syntax works for one entry, too, but we save a few bytes :-)
				optionalurlparameter += "%20-output%20$::TKS::"+requestOutput[0];
			}else if(requestOutput.length > 1){
				optionalurlparameter += "%20-output%20[expr%20$::TKS::";
				for (var i = 0; i < requestOutput.length;i++){
					optionalurlparameter += requestOutput[i];
					if(i+1 < requestOutput.length){
						//we will do another run
						optionalurlparameter += "|$::TKS::";
					}
				}
				optionalurlparameter += "]";
			}
			urlparameter = HMI.KSGateway_Path+"?obj="+Handle + "&args=getep%20" +ServerAndPath[1]+"%20*" + optionalurlparameter;
		}else if ("kshttp" === HMI.HMI_Constants.ServerType){
			Handle = this.getCommunicationPoint(ServerAndPath[0]);
			if(Handle === null){
				return null;
			}
			optionalurlparameter = "&requestType="+requestType;
			if(Object.prototype.toString.call(requestOutput) !== "[object Array]"){
				//fail save array detection
				optionalurlparameter += "&requestOutput="+requestOutput;
			}else if(requestOutput.length === 1){
				//the expr syntax works for one entry, too, but we save a few bytes :-)
				optionalurlparameter += "&requestOutput="+requestOutput[0];
			}else if(requestOutput.length > 1){
				for (var i = 0; i < requestOutput.length;i++){
					optionalurlparameter += "&requestOutput["+i+"]="+requestOutput[i];
				}
			}
			;
			urlparameter = "http://"+Handle+"/getEP?path=" +ServerAndPath[1]+optionalurlparameter;
		}else if ("php" === HMI.HMI_Constants.ServerType){
			//requestType, requestOutput not implemented, but any known tks.php is lacking them, too
			Handle = this.getCommunicationPoint(ServerAndPath[0]);
			if(Handle === null){
				return null;
			}
			urlparameter = HMI.KSGateway_Path+"?obj="+Handle + "&cmd=getep&path=" +ServerAndPath[1]+"%20*";
		}else{
			//communication type not implemented
			return null;
		}
		if (async === true && cbfnc !== null && cbfnc !== undefined){
			this._sendRequest(this, method, true, urlparameter, cbfnc, responseFormat);
		}else if (cbfnc !== null && cbfnc !== undefined){
			this._sendRequest(this, method, false, urlparameter, cbfnc, responseFormat);
		}else{
			var ReturnText = this._sendRequest(this, method, false, urlparameter, cbfnc, responseFormat);
			HMI.hmi_log_trace("HMIJavaScriptKSClient.prototype.getEP - End");
			return ReturnText;
		}
		
		HMI.hmi_log_trace("HMIJavaScriptKSClient.prototype.getEP - End");
		return true;
	},
	
	/**
	 * usage example:
	 *		ManagerResponse = this.getVar(TCLKSHandle, "/Libraries/hmi/Manager.instance", null)
	 *		this.getVar(null, '/TechUnits/HMIManager.CommandReturn', this._cbGetSheets);
	 * 
	 * @param path of the variable to fetch, multiple path possible via an Array (used in GraphicDescription+StyleDescription)
	 * @param requestOutput Array of interesting objects properties ("OP_NAME", "OP_TYPE", "OP_VALUE", "OP_TIMESTAMP" or "OP_STATE")
	 * @param cbfnc callback function
	 * @param async request async communication
	 * @param responseFormat Mime-Type of requested response (probably "text/tcl", "text/ksx", "text/plain" used)
	 * @return "{{/TechUnits/HMIManager}}", response: "{/TechUnits/Sheet1}" or "TksS-0042::KS_ERR_BADPATH {{/Libraries/hmi/Manager.instance KS_ERR_BADPATH}}"
	 */
	getVar: function(path, requestOutput, cbfnc, async, responseFormat) {
		if(!requestOutput){
			requestOutput = "OP_VALUE";
		}
		HMI.hmi_log_trace("HMIJavaScriptKSClient.prototype.getVar - Start: "+path);
		if(!path || path.length === 0){
			HMI.hmi_log_error("HMIJavaScriptKSClient.prototype.getVar - no path found");
			return null;
		}
		if (!requestOutput.indexOf || requestOutput.indexOf("OP_") !== 0){
			HMI.hmi_log_error("HMIJavaScriptKSClient.prototype.getVar - no valid requestOutput found, requestOutput was: "+requestOutput);
			return null;
		}
		
		var ServerAndPath = this._splitKSPath(path);
		var Handle;
		var urlparameter;
		var method = "GET";
		var optionalurlparameter = "";
		if(responseFormat === undefined){
			responseFormat = "text/tcl";
		}
		if ("tcl" === HMI.HMI_Constants.ServerType){
			Handle = this.getCommunicationPoint(ServerAndPath[0]);
			if(Handle === null){
				return null;
			}
			
			if(Object.prototype.toString.call(requestOutput) !== "[object Array]"){
				//fail save array detection
				optionalurlparameter = "%20-output%20$::TKS::"+requestOutput;
			}else if(requestOutput.length === 1){
				//the expr syntax works for one entry, too, but we save a few bytes :-)
				optionalurlparameter = "%20-output%20$::TKS::"+requestOutput[0];
			}else if(requestOutput.length > 1){
				optionalurlparameter = "%20-output%20[expr%20$::TKS::";
				for (var i = 0; i < requestOutput.length;i++){
					optionalurlparameter += requestOutput[i];
					if(i+1 < requestOutput.length){
						//we will do another run
						optionalurlparameter += "|$::TKS::";
					}
				}
				optionalurlparameter += "]";
			}
			
			urlparameter = HMI.KSGateway_Path+"?obj="+Handle + "&args=getvar%20" +
				"{"+ServerAndPath.slice(1).join(" ")+"}" + optionalurlparameter;
		}else if ("kshttp" === HMI.HMI_Constants.ServerType){
			Handle = this.getCommunicationPoint(ServerAndPath[0]);
			if(Handle === null){
				return null;
			}
			
			urlparameter = "http://"+Handle+"/getVar?";
			for(var i=1; i < ServerAndPath.length;i++){
				if(i>1){
					urlparameter += "&";
				}
				urlparameter += "path["+i+"]=" +ServerAndPath[i];
			}
		}else if ("php" === HMI.HMI_Constants.ServerType){
			Handle = this.getCommunicationPoint(ServerAndPath[0]);
			if(Handle === null){
				return null;
			}
			urlparameter = HMI.KSGateway_Path+"?obj="+Handle + "&cmd=getvar&path=" +
				"{"+ServerAndPath.slice(1).join(" ")+"}";
		}else{
			//communication type not implemented
			return null;
		}
		if (async === true && cbfnc !== null && cbfnc !== undefined){
			this._sendRequest(this, method, true, urlparameter, cbfnc, responseFormat);
		}else if (cbfnc !== null && cbfnc !== undefined){
			this._sendRequest(this, method, false, urlparameter, cbfnc, responseFormat);
		}else{
			var ReturnText = this._sendRequest(this, method, false, urlparameter, cbfnc, responseFormat);
			HMI.hmi_log_trace("HMIJavaScriptKSClient.prototype.getVar - End");
			return ReturnText;
		}
		
		HMI.hmi_log_trace("HMIJavaScriptKSClient.prototype.getVar - End");
		return true;
	},
	
	
	//fixme return value "" in true wandeln
	/**
	 * usage example:
	 *		this.setVar('/TechUnits/HMIManager.Command ', ["1", "010", "/TechUnits/HMIManager", "SHOWSHEETS"], null);
	 * 
	 * @param path of the variable to set
	 * @param {String} value to set (StringVec are Arrays)
	 * @param {String} type variable type (for example "KS_VT_STRING") to set, null if no change
	 * @param cbfnc callback function
	 * @param async request async communication
	 * @param responseFormat Mime-Type of requested response (probably "text/tcl", "text/ksx", "text/plain" used)
	 * @return "" or null
	 */
	setVar: function(path, value, type, cbfnc, async, responseFormat) {
		HMI.hmi_log_trace("HMIJavaScriptKSClient.prototype.setVar - Start: "+path);
		if(!path || path.length === 0){
			HMI.hmi_log_error("HMIJavaScriptKSClient.prototype.setVar - no path found");
			return null;
		}
		if(path.charAt(0) !== "/"){
			HMI.hmi_log_error("HMIJavaScriptKSClient.prototype.setVar - no valid path found, path was: "+path);
			return null;
		}
		
		var ServerAndPath = this._splitKSPath(path);
		var method = "GET";
		var Handle;
		var urlparameter;
		var optionalurlparameter = "";
		if(responseFormat === undefined){
			responseFormat = "text/tcl";
		}
		if ("tcl" === HMI.HMI_Constants.ServerType){
			if(Object.prototype.toString.call(value) === "[object Array]"){
				value = "{"+value.join("}%20{")+"}";
			}
			
			Handle = this.getCommunicationPoint(ServerAndPath[0]);
			if(Handle === null){
				return null;
			}
			if(type !== null && type !== undefined){
				//TCL is not able to interprete the type as text
				optionalurlparameter = "%20-type%20"+this.KSCONSTANTS[type];
			}
			
			urlparameter = HMI.KSGateway_Path+"?obj="+Handle + "&args=setvar%20"+
			"{"+ServerAndPath[1]+"%20{"+value+"}"+optionalurlparameter+"}";
		}else if ("kshttp" === HMI.HMI_Constants.ServerType){
			Handle = this.getCommunicationPoint(ServerAndPath[0]);
			if(Handle === null){
				return null;
			}
			if(Object.prototype.toString.call(value) === "[object Array]"){
				var newvalue = "{";
				for(var i=0;i < value.length;i++){
					if(i!=0){
						newvalue += "}%20{";
					}
					if(escape){
						//escape is evil, but we need latin1 support
						newvalue += escape(value[i]);
					}else{
						newvalue += encodeURIComponent(value[i]);
					}
				}
				value = newvalue + "}";
				newvalue = null;
			}else{
				if(escape){
					//escape is evil, but we need latin1 support
					value = escape(value);
				}else{
					value = encodeURIComponent(value);
				}
			}
			if(type !== null && type !== undefined){
				optionalurlparameter = "&vartype="+type;
			}
			urlparameter = "http://"+Handle+"/setVar?path=" +ServerAndPath[1]+"&newvalue="+value+optionalurlparameter;
		}else if ("php" === HMI.HMI_Constants.ServerType){
			if(Object.prototype.toString.call(value) === "[object Array]"){
				value = "{"+value.join("}%20{")+"}";
			}
			
			Handle = this.getCommunicationPoint(ServerAndPath[0]);
			if(Handle === null){
				return null;
			}
			urlparameter = HMI.KSGateway_Path+"?obj="+ Handle + "&cmd=setvar"+
				"&path=" + ServerAndPath[1] +
				"&val=" + value;
		}else{
			//communication type not implemented
			return null;
		}
		if (async === true && cbfnc !== null && cbfnc !== undefined){
			this._sendRequest(this, method, true, urlparameter, cbfnc, responseFormat);
		}else if (cbfnc !== null && cbfnc !== undefined){
			this._sendRequest(this, method, false, urlparameter, cbfnc, responseFormat);
		}else{
			var ReturnText = this._sendRequest(this, method, false, urlparameter, cbfnc, responseFormat);
			HMI.hmi_log_trace("HMIJavaScriptKSClient.prototype.setVar - End");
			return ReturnText;
		}
		
		HMI.hmi_log_trace("HMIJavaScriptKSClient.prototype.setVar - End");
		return true;
	},
	
	/**
	 * rename an object
	 * 
	 * @param path of the object to rename
	 * @param newname (optional with full path) of the object
	 * @param cbfnc callback function
	 * @param async request async communication
	 * @param responseFormat Mime-Type of requested response (probably "text/tcl", "text/ksx", "text/plain" used)
	 * @return true, "" or null
	 */
	renameObject: function(oldname, newname, cbfnc, async, responseFormat) {
		HMI.hmi_log_trace("HMIJavaScriptKSClient.prototype.renameObject - Start: "+oldname);
		if(!oldname || oldname.length === 0){
			HMI.hmi_log_error("HMIJavaScriptKSClient.prototype.renameObject - no oldname found");
			return null;
		}
		if(!newname || newname.length === 0){
			HMI.hmi_log_error("HMIJavaScriptKSClient.prototype.renameObject - no newname found");
			return null;
		}
		if(oldname.charAt(0) !== "/"){
			HMI.hmi_log_error("HMIJavaScriptKSClient.prototype.renameObject - no valid oldname found, oldname was: "+oldname);
			return null;
		}
		
		var ServerAndPath = this._splitKSPath(oldname);
		var ServerAndPathNewname = this._splitKSPath(newname);
		if(responseFormat === undefined){
			responseFormat = "text/tcl";
		}
		if(newname.charAt(0) !== "/"){
			//we have a plain name, add path of the oldname
			var PathArray = ServerAndPath[1].split("/");
			newname = ServerAndPath[1].replace( PathArray[PathArray.length - 1], newname);
		}else if (ServerAndPath[0] === ServerAndPathNewname[0]){
			//both names are within the same server
			newname = ServerAndPathNewname[1];
		}else if (ServerAndPath[0] !== ServerAndPathNewname[0]){
			//two full path names given , but on different server not possible
			return "KS_ERR_BADPATH";
		}
		
		var Handle;
		var urlparameter;
		var method = "GET";
		if ("tcl" === HMI.HMI_Constants.ServerType){
			Handle = this.getCommunicationPoint(ServerAndPath[0]);
			if(Handle === null){
				return null;
			}
			urlparameter = HMI.KSGateway_Path+"?obj="+Handle + "&args=rename%20"+
				ServerAndPath[1]+"%20"+newname;
		}else if ("kshttp" === HMI.HMI_Constants.ServerType){
			Handle = this.getCommunicationPoint(ServerAndPath[0]);
			if(Handle === null){
				return null;
			}
			
			urlparameter = "http://"+Handle+"/renameObject?path="+ServerAndPath[1]+"&newname="+newname;
		}else if ("php" === HMI.HMI_Constants.ServerType){
			//no known tks.php implement them!
			Handle = this.getCommunicationPoint(ServerAndPath[0]);
			if(Handle === null){
				return null;
			}
			urlparameter = HMI.KSGateway_Path+"?obj="+ Handle + "&cmd=rename"+
				"&path=" + ServerAndPath[1]+
				"&newname=" + newname;
		}else{
			//communication type not implemented
			return null;
		}
		if (async === true && cbfnc !== null && cbfnc !== undefined){
			this._sendRequest(this, method, true, urlparameter, cbfnc, responseFormat);
		}else if (cbfnc !== null && cbfnc !== undefined){
			this._sendRequest(this, method, false, urlparameter, cbfnc, responseFormat);
		}else{
			var ReturnText = this._sendRequest(this, method, false, urlparameter, cbfnc, responseFormat);
			HMI.hmi_log_trace("HMIJavaScriptKSClient.prototype.renameObject - End");
			return ReturnText;
		}
		
		HMI.hmi_log_trace("HMIJavaScriptKSClient.prototype.renameObject - End");
		return true;
	},
	
	/**
	 * creates an object
	 * 
	 * @param path of the object to create
	 * @param classname full class name of the new object
	 * @param cbfnc callback function
	 * @param async request async communication
	 * @param responseFormat Mime-Type of requested response (probably "text/tcl", "text/ksx", "text/plain" used)
	 * @return true, "" or null
	 */
	createObject: function(path, classname, cbfnc, async, responseFormat) {
		HMI.hmi_log_trace("HMIJavaScriptKSClient.prototype.createObject - Start: "+path);
		if(!path || path.length === 0){
			HMI.hmi_log_error("HMIJavaScriptKSClient.prototype.createObject - no path found");
			return null;
		}
		if(!classname || classname.length === 0){
			HMI.hmi_log_error("HMIJavaScriptKSClient.prototype.createObject - no classname found");
			return null;
		}
		if(path.charAt(0) !== "/"){
			HMI.hmi_log_error("HMIJavaScriptKSClient.prototype.createObject - no valid path found, path was: "+path);
			return null;
		}
		
		var ServerAndPath = this._splitKSPath(path);
		
		//prevent an hostname in this path
		var classname = this._splitKSPath(classname)[1];
		var Handle;
		var urlparameter;
		var method = "GET";
		if(responseFormat === undefined){
			responseFormat = "text/tcl";
		}
		if ("tcl" === HMI.HMI_Constants.ServerType){
			Handle = this.getCommunicationPoint(ServerAndPath[0]);
			if(Handle === null){
				return null;
			}
			urlparameter = HMI.KSGateway_Path+"?obj="+Handle + "&args=create%20{"+
			ServerAndPath[1]+"%20"+classname+"}";
		}else if ("kshttp" === HMI.HMI_Constants.ServerType){
			Handle = this.getCommunicationPoint(ServerAndPath[0]);
			if(Handle === null){
				return null;
			}
			
			urlparameter = "http://"+Handle+"/createObject?path="+ServerAndPath[1]+"&factory="+classname;
		}else if ("php" === HMI.HMI_Constants.ServerType){
			//no known tks.php implement them!
			Handle = this.getCommunicationPoint(ServerAndPath[0]);
			if(Handle === null){
				return null;
			}
			urlparameter = HMI.KSGateway_Path+"?obj="+ Handle + "&cmd=create"+
				"&path=" + ServerAndPath[1]+
				"&class=" + classname;
		}else{
			//communication type not implemented
			return null;
		}
		if (async === true && cbfnc !== null && cbfnc !== undefined){
			this._sendRequest(this, method, true, urlparameter, cbfnc, responseFormat);
		}else if (cbfnc !== null && cbfnc !== undefined){
			this._sendRequest(this, method, false, urlparameter, cbfnc, responseFormat);
		}else{
			var ReturnText = this._sendRequest(this, method, false, urlparameter, cbfnc, responseFormat);
			HMI.hmi_log_trace("HMIJavaScriptKSClient.prototype.createObject - End");
			return ReturnText;
		}
		
		HMI.hmi_log_trace("HMIJavaScriptKSClient.prototype.createObject - End");
		return true;
	},
	
	/**
	 * deletes an object
	 * 
	 * @param path ob the object to delete
	 * @param cbfnc callback function
	 * @param async request async communication
	 * @param responseFormat Mime-Type of requested response (probably "text/tcl", "text/ksx", "text/plain" used)
	 * @return true, "" or null
	 */
	deleteObject: function(path, cbfnc, async, responseFormat) {
		HMI.hmi_log_trace("HMIJavaScriptKSClient.prototype.deleteObject - Start: "+path);
		if(!path || path.length === 0){
			HMI.hmi_log_error("HMIJavaScriptKSClient.prototype.deleteObject - no path found");
			return null;
		}
		if(path.charAt(0) !== "/"){
			HMI.hmi_log_error("HMIJavaScriptKSClient.prototype.deleteObject - no valid path found, path was: "+path);
			return null;
		}
		
		var ServerAndPath = this._splitKSPath(path);
		var Handle;
		var urlparameter;
		var method = "GET";
		if(responseFormat === undefined){
			responseFormat = "text/tcl";
		}
		if ("tcl" === HMI.HMI_Constants.ServerType){
			Handle = this.getCommunicationPoint(ServerAndPath[0]);
			if(Handle === null){
				return null;
			}
			urlparameter = HMI.KSGateway_Path+"?obj="+Handle + "&args=delete%20"+
			ServerAndPath[1];
		}else if ("kshttp" === HMI.HMI_Constants.ServerType){
			Handle = this.getCommunicationPoint(ServerAndPath[0]);
			if(Handle === null){
				return null;
			}
			
			urlparameter = "http://"+Handle+"/deleteObject?path="+ServerAndPath[1];
		}else if ("php" === HMI.HMI_Constants.ServerType){
			//no known tks.php implement them!
			Handle = this.getCommunicationPoint(ServerAndPath[0]);
			if(Handle === null){
				return null;
			}
			urlparameter = HMI.KSGateway_Path+"?obj="+ Handle + "&cmd=delete"+
				"&path=" + ServerAndPath[1];
		}else{
			//communication type not implemented
			return null;
		}
		if (async === true && cbfnc !== null && cbfnc !== undefined){
			this._sendRequest(this, method, true, urlparameter, cbfnc, responseFormat);
		}else if (cbfnc !== null && cbfnc !== undefined){
			this._sendRequest(this, method, false, urlparameter, cbfnc, responseFormat);
		}else{
			var ReturnText = this._sendRequest(this, method, false, urlparameter, cbfnc, responseFormat);
			HMI.hmi_log_trace("HMIJavaScriptKSClient.prototype.deleteObject - End");
			return ReturnText;
		}
		
		HMI.hmi_log_trace("HMIJavaScriptKSClient.prototype.deleteObject - End");
		return true;
	},
	
	/**
	 * links two objects
	 * 
	 * @param path of the object to create
	 * @param classname full class name of the new object
	 * @param cbfnc callback function
	 * @param async request async communication
	 * @param responseFormat Mime-Type of requested response (probably "text/tcl", "text/ksx", "text/plain" used)
	 * @return true, "" or null
	 */
	linkObjects: function(pathA, pathB, portnameA, cbfnc, async, responseFormat) {
		HMI.hmi_log_trace("HMIJavaScriptKSClient.prototype.linkObjects");
		if(!pathA || pathA.length === 0){
			HMI.hmi_log_error("HMIJavaScriptKSClient.prototype.linkObjects - no pathA found");
			return null;
		}
		if(!pathB || pathB.length === 0){
			HMI.hmi_log_error("HMIJavaScriptKSClient.prototype.linkObjects - no pathB found");
			return null;
		}
		if(!portnameA || portnameA.length === 0){
			HMI.hmi_log_error("HMIJavaScriptKSClient.prototype.linkObjects - no portnameA found");
			return null;
		}
		if(pathA.charAt(0) !== "/"){
			HMI.hmi_log_error("HMIJavaScriptKSClient.prototype.linkObjects - no valid pathA found, pathA was: "+pathA);
			return null;
		}
		if(pathB.charAt(0) !== "/"){
			HMI.hmi_log_error("HMIJavaScriptKSClient.prototype.linkObjects - no valid pathB found, pathB was: "+pathB);
			return null;
		}
		
		var ServerAndPath = this._splitKSPath(pathA);
		var ServerAndPathB = this._splitKSPath(pathB);
		var Handle;
		var urlparameter;
		var method = "GET";
		if(responseFormat === undefined){
			responseFormat = "text/tcl";
		}
		if ("tcl" === HMI.HMI_Constants.ServerType){
			Handle = this.getCommunicationPoint(ServerAndPath[0]);
			if(Handle === null){
				return null;
			}
			urlparameter = HMI.KSGateway_Path+"?obj="+Handle + "&args=link%20"+
			ServerAndPath[1]+"."+portnameA+"%20"+ServerAndPathB[1];
		}else if ("kshttp" === HMI.HMI_Constants.ServerType){
			Handle = this.getCommunicationPoint(ServerAndPath[0]);
			if(Handle === null){
				return null;
			}
			
			urlparameter = "http://"+Handle+"/link?path="+ServerAndPath[1]+"."+portnameA+"&element="+ServerAndPathB[1];
		}else if ("php" === HMI.HMI_Constants.ServerType){
			//no known tks.php implement them!
			Handle = this.getCommunicationPoint(ServerAndPath[0]);
			if(Handle === null){
				return null;
			}
			urlparameter = HMI.KSGateway_Path+"?obj="+ Handle + "&cmd=link"+
				"&path=" + ServerAndPath[1]+
				"&pathb=" + ServerAndPathB[1];
		}else{
			//communication type not implemented
			return null;
		}
		if (async === true && cbfnc !== null && cbfnc !== undefined){
			this._sendRequest(this, method, true, urlparameter, cbfnc, responseFormat);
		}else if (cbfnc !== null && cbfnc !== undefined){
			this._sendRequest(this, method, false, urlparameter, cbfnc, responseFormat);
		}else{
			var ReturnText = this._sendRequest(this, method, false, urlparameter, cbfnc, responseFormat);
			HMI.hmi_log_trace("HMIJavaScriptKSClient.prototype.linkObjects - End");
			return ReturnText;
		}
		
		HMI.hmi_log_trace("HMIJavaScriptKSClient.prototype.linkObjects - End");
		return true;
	},
	
	/**
	 * unlinks two objects
	 * 
	 * @param path of the object to create
	 * @param classname full class name of the new object
	 * @param cbfnc callback function
	 * @param async request async communication
	 * @param responseFormat Mime-Type of requested response (probably "text/tcl", "text/ksx", "text/plain" used)
	 * @return true, "" or null
	 */
	unlinkObjects: function(pathA, pathB, portnameA, cbfnc, async, responseFormat) {
		HMI.hmi_log_trace("HMIJavaScriptKSClient.prototype.unlinkObjects");
		if(!pathA || pathA.length === 0){
			HMI.hmi_log_error("HMIJavaScriptKSClient.prototype.unlinkObjects - no pathA found");
			return null;
		}
		if(!pathB || pathB.length === 0){
			HMI.hmi_log_error("HMIJavaScriptKSClient.prototype.unlinkObjects - no pathB found");
			return null;
		}
		if(!portnameA || portnameA.length === 0){
			HMI.hmi_log_error("HMIJavaScriptKSClient.prototype.unlinkObjects - no portnameA found");
			return null;
		}
		if(pathA.charAt(0) !== "/"){
			HMI.hmi_log_error("HMIJavaScriptKSClient.prototype.unlinkObjects - no valid pathA found, pathA was: "+pathA);
			return null;
		}
		if(pathB.charAt(0) !== "/"){
			HMI.hmi_log_error("HMIJavaScriptKSClient.prototype.unlinkObjects - no valid pathB found, pathB was: "+pathB);
			return null;
		}
		
		var ServerAndPath = this._splitKSPath(pathA);
		var ServerAndPathB = this._splitKSPath(pathB);
		var Handle;
		var urlparameter;
		var method = "GET";
		if(responseFormat === undefined){
			responseFormat = "text/tcl";
		}
		if ("tcl" === HMI.HMI_Constants.ServerType){
			Handle = this.getCommunicationPoint(ServerAndPath[0]);
			if(Handle === null){
				return null;
			}
			urlparameter = HMI.KSGateway_Path+"?obj="+Handle + "&args=unlink%20"+
			ServerAndPath[1]+"."+portnameA+"%20"+ServerAndPathB[1];
		}else if ("kshttp" === HMI.HMI_Constants.ServerType){
			Handle = this.getCommunicationPoint(ServerAndPath[0]);
			if(Handle === null){
				return null;
			}
			
			urlparameter = "http://"+Handle+"/unlink?path="+ServerAndPath[1]+"."+portnameA+"&element="+ServerAndPathB[1];
		}else if ("php" === HMI.HMI_Constants.ServerType){
			//no known tks.php implement them!
			Handle = this.getCommunicationPoint(ServerAndPath[0]);
			if(Handle === null){
				return null;
			}
			urlparameter = HMI.KSGateway_Path+"?obj="+ Handle + "&cmd=unlink"+
				"&path=" + ServerAndPath[1]+
				"&pathb=" + ServerAndPathB[1];
		}else{
			//communication type not implemented
			return null;
		}
		if (async === true && cbfnc !== null && cbfnc !== undefined){
			this._sendRequest(this, method, async, urlparameter, cbfnc, responseFormat);
		}else if (cbfnc !== null && cbfnc !== undefined){
			this._sendRequest(this, method, false, urlparameter, cbfnc, responseFormat);
		}else{
			var ReturnText = this._sendRequest(this, method, false, urlparameter, cbfnc, responseFormat);
			HMI.hmi_log_trace("HMIJavaScriptKSClient.prototype.unlinkObjects - End");
			return ReturnText;
		}
		
		HMI.hmi_log_trace("HMIJavaScriptKSClient.prototype.unlinkObjects - End");
		return true;
	},
	
	/**
	 * @param {String} HostAndServername Host and Servername concat with a slash
	 * @return {String} TKS Handle or "Server:port" for kshttp of the requested Server or null
	 * @todo add timeout für unused deleteCommunicationPoint
	 */
	getCommunicationPoint: function(HostAndServername) {
		if(HostAndServername === null){
			return null;
		}
		if (this.ResourceList.Servers[HostAndServername] && this.ResourceList.Servers[HostAndServername].HandleString !== undefined){
			return this.ResourceList.Servers[HostAndServername].HandleString;
		}else{
			var HandleString = this._findCommunicationPoint(HostAndServername, null);
			if (HandleString === null || HandleString === false){
				//occures in shutdown
				return null;
			}else if (HandleString.indexOf("KS_ERR_SERVERUNKNOWN") !== -1){
				//the Manager sometimes reject connection to a valid server, so retry once
				//HMI.hmi_log_trace("HMIJavaScriptKSClient.prototype.getCommunicationPoint - got KS_ERR_SERVERUNKNOWN but do not trust. Retrying");
				
				//retry disabled for performance reasons!
				//HandleString = this._findCommunicationPoint(HostAndServername, null);
			}
			if (HandleString.indexOf("KS_ERR") !== -1){
				//the server is really not available. Could be the case if there is an active KS-Bridge and its destination is not available
				
				//caching the result of the failure
				HandleString = null;
			}else if ("tcl" === HMI.HMI_Constants.ServerType && !/^TksS-\b/.exec(HandleString)){
				//The handle must have the right format aka start with "TksS-"
				if (HandleString.length < 250){
					HMI.hmi_log_onwebsite('Could not initialize TCLKSGateway. Server responded: ' + HandleString);
				}else if (HandleString){
					HMI.hmi_log_onwebsite('Could not initialize TCLKSGateway. Server responded (first 250 characters): ' + HandleString.substr(0,250));
				}else{
					HMI.hmi_log_onwebsite('Could not analyse server response.');
				}
				return null;
			}
			this.ResourceList.Servers[HostAndServername] = Object();
			this.ResourceList.Servers[HostAndServername].HandleString = HandleString;
			return HandleString;
		}
	},
	
	/**
	 * finds the TCL handle or TCP Port of kshttp server
	 * usage example:
	 *		_findCommunicationPoint("localhost/MANAGER", this._cbInit);
	 *		_findCommunicationPoint("localhost/fb_hmi1", null);
	 * @param {String} HostAndServername Host and Servername concat with a slash
	 * @param cbfnc callback function
	 * @param async request async communication
	 * @return {String} TKS Handle or "Server:port" for kshttpof the requested Server or null
	 */
	_findCommunicationPoint: function(HostAndServer, cbfnc, async) {
		HMI.hmi_log_trace("HMIJavaScriptKSClient.prototype._findCommunicationPoint - Start on "+HostAndServer);
		
		//fixme auf async umstellen
		var urlparameter;
		var method = "GET";
		var tempArray = HostAndServer.split("/");
		var responseFormat = "text/tcl";
		if (tempArray.length !== 2){
			return null;
		}
		var Host = tempArray[0];
		var Server = tempArray[1];
		var ManagerPort = "7509";
		var Serverport = "";
		if(Host.indexOf(":") !== -1){
			tempArray = Host.split(":");
			Host = tempArray[0];
			ManagerPort = tempArray[1];
			var ManagerPortGiven = true;
		}
		if(Server.indexOf(":") !== -1){
			tempArray = Server.split(":");
			Server = tempArray[0];
			Serverport = tempArray[1];
		}
		if ("kshttp" === HMI.HMI_Constants.ServerType){
			if(Serverport !== ""){
				return Host+":"+Serverport;
			}else if(Server === "MANAGER"){
				//manager port is known if set
				//or on port 7509 with kshttp
				return Host+":"+ManagerPort;
			}else{
				urlparameter = "http://"+Host+":"+ManagerPort+"/getServer?servername="+Server;
				responseFormat = "text/plain";
			}
		}else{
			//tcl and php gateway
			if ("tcl" === HMI.HMI_Constants.ServerType){
				urlparameter = HMI.KSGateway_Path+"?obj=tks-server&args="+Host+ ':' + ManagerPort+"/"+Server;
			}else if("php" === HMI.HMI_Constants.ServerType){
				urlparameter = HMI.KSGateway_Path+"?cmd=tks-server&args="+Host+ ':' + ManagerPort+"/"+Server;
			}
		}
		var ReturnText = this._sendRequest(this, method, false, urlparameter, null, responseFormat);
		
		if (ReturnText === false){
			//communication error
			return null;
		}else if(ReturnText === true){
			//no async communication. oops
			return null;
		}else if (ReturnText.indexOf("KS_ERR") !== -1 && ManagerPortGiven !== true){
			//on error retest without default acplt port
			if ("tcl" === HMI.HMI_Constants.ServerType){
				urlparameter = HMI.KSGateway_Path+"?obj=tks-server&args="+Host+"/"+Server;
			}else if("php" === HMI.HMI_Constants.ServerType){
				urlparameter = HMI.KSGateway_Path+"?cmd=tks-server&args="+Host+"/"+Server;
			}else{
				return ReturnText;
			}
			ReturnText = this._sendRequest(this, method, false, urlparameter, null, responseFormat);
		}else if("kshttp" === HMI.HMI_Constants.ServerType){
			//prefix hostname if kshttp
			ReturnText = Host+":"+parseInt(ReturnText, 10);
		}
		HMI.hmi_log_trace("HMIJavaScriptKSClient.prototype._findCommunicationPoint - End");
		return ReturnText;
	},
	
	/**
	 * usage example:
	 * 		this.deleteCommunicationPoint("TksS-0042");
	 * @param Handle Handle to be destroyed
	 * @return none
	 */
	deleteCommunicationPoint: function(Handle) {
		var urlparameter;
		var method = "GET";
		if ("tcl" === HMI.HMI_Constants.ServerType){
			urlparameter = HMI.KSGateway_Path+"?obj="+Handle + "&args=destroy";
		}else if ("php" === HMI.HMI_Constants.ServerType){
			urlparameter = HMI.KSGateway_Path+"?obj="+Handle + "&cmd=destroy";
		}else if ("kshttp" === HMI.HMI_Constants.ServerType){
			//ksserv does not need this
			return;
		}else{
			//communication type not implemented
			return;
		}
		var responseFormat = "text/tcl";
		if (Handle !== null){
			//the result is not interesting, so can be ignored (async, no callback)
			this._sendRequest(this, method, true, urlparameter, null, responseFormat);
		}
	},
	
	/*********************************
		getServers
	*********************************/
	getServers: function(Host) {
		HMI.hmi_log_trace("HMIJavaScriptKSClient.prototype.getServers - Start");
		
		//fixme async
		
		//we want a domain listing and only the names are interesting
		var result = this.getEP("//"+Host+"/"+'MANAGER'+'/servers', "OT_DOMAIN", "OP_NAME", this._cbGetServers, false);
		if (result === null){
			HMI.PossServers.setAttribute("title", "No MANAGER available");
			HMI.hmi_log_info_onwebsite("Requested Host has no MANAGER available.");
		}
		HMI.hmi_log_trace("HMIJavaScriptKSClient.prototype.getServers - End");
		return true;
	},
	
	/*********************************
		_cbGetServers
	*********************************/
	_cbGetServers: function(Client, req) {
		HMI.hmi_log_trace("HMIJavaScriptKSClient.prototype._cbGetServers - Start");
		
		//responseText should be something like: {fb_hmi1} {fb_hmi2} {fb_hmi3} {MANAGER} {fb_hmi5} {fb_hmi4}
		var Server = HMI.KSClient.splitKsResponse(req.responseText, 0);
		
		var i = 0;
		
		Server = Server.sort();
		HMI.hmi_log_trace("HMIJavaScriptKSClient.prototype._cbGetServers - number of potential servers: "+Server.length);
		
		HMI.PossServers.setAttribute("title", "available OV-Servers: "+Server.join(", "));
		
		if (Server.length === 0){
			HMI.PossServers.options[0] = new Option('- no MANAGER available-', '');
		}else{
			//put first select option with a description
			HMI.PossServers.options[0] = new Option('loading...', '');
			
			var Host = HMI.getHostname();
			for (i = 0; i < Server.length; i++){
				//test all potential servers if they are HMI Servers
				if (HMI.KSClient.pingServer(Host, Server[i]) === true){
					//put server into the dropdown box
					HMI.PossServers.options[HMI.PossServers.options.length] = new Option(Server[i], Server[i]);
				};
			};
			if (HMI.PossServers.length === 1){
				HMI.PossServers.options[0] = new Option('- no HMI server available -', '');
			}else if (HMI.PossServers.length == 2){
				HMI.PossServers.options[0] = new Option('- select server -', '');
				//selecting the option does not trigger the EventListener
				//it is allways the second/last <option>...
				HMI.PossServers.selectedIndex = 1;
				HMI.showSheets(HMI.getHostname(), HMI.PossServers.options[1].value);
			}else{
				HMI.PossServers.options[0] = new Option('- select server -', '');
				//replacing option[0] drops us into the first server
				HMI.PossServers.selectedIndex = 0;
				HMI.PossServers.disabled = false;
				HMI.PossSheets.options[0] = new Option('please select Server first', '');
			}
		}
		HMI.hmi_log_trace("HMIJavaScriptKSClient.prototype._cbGetServers - End");
	},
	
	/**
	 * Test if a given server on a host is a cshmi or hmi server
	 * @param Host Hostname on wich the server runs
	 * @param Server Servername on the Host
	 */
	pingServer: function(Host, Server) {
		HMI.hmi_log_trace("HMIJavaScriptKSClient.prototype.pingServer - Start: "+Server);
		//get a list of all loaded ov libraries of this server
		this.hmilib_path[Host+Server] = null;
		this.cshmilib_path[Host+Server] = null;
		var Response = this.getVar("//"+Host+"/"+Server+"/acplt/ov/library.instance", "OP_VALUE", null);
		if (!Response){
			HMI.hmi_log_trace("HMIJavaScriptKSClient.prototype.pingServer - communication problem, so no hmi and no cshmi server");
			return false;
		}else if (Response.indexOf("KS_ERR") !== -1){
			HMI.hmi_log_trace("HMIJavaScriptKSClient.prototype.pingServer - no OV server (perhaps MANAGER), so no hmi and no cshmi server");
			return false;
		}else if (	Response.indexOf("/hmi") !== -1
				||	Response.indexOf("/cshmi") !== -1){
			var ServerLibraryList = this.splitKsResponse(Response);
			if (ServerLibraryList.length > 0 && ServerLibraryList[0] !== ""){
				ServerLibraryList = ServerLibraryList[0].split(" ");
			}
			var index, value;
			for (index = 0; index < ServerLibraryList.length; index++) {
				value = ServerLibraryList[index];
				if (value.search(".*/hmi$") !== -1) {
					this.hmilib_path[Host+Server] = value;
				}else if (value.search(".*/cshmi$") !== -1) {
					this.cshmilib_path[Host+Server] = value;
				}
			}
			//an hmi or cshmi library is loaded
			HMI.hmi_log_trace("HMIJavaScriptKSClient.prototype.pingServer - hmi and/or cshmi server");
			return true;
		}else{
			HMI.hmi_log_trace("HMIJavaScriptKSClient.prototype.pingServer - no hmi and no cshmi server");
			return false;
		}
	},
	
	/**
	 * remember the path of the HMI Manger
	 * @param Host Hostname on wich the server runs
	 * @param Server Servername on the Host
	 */
	getHMIManagerPointer: function(Host, Server) {
		//the path of the HMI Manager could be different in every OV Server
		var ManagerResponse = this.getVar("//"+Host+"/"+Server+this.hmilib_path[Host+Server]+"/Manager.instance", "OP_VALUE", null);
		
		var ManagerResponseArray = this.splitKsResponse(ManagerResponse);
		if (ManagerResponseArray.length === 0){
			HMI.hmi_log_trace("HMIJavaScriptKSClient.prototype.getHMIManagerPointer - Response invalid");
			this.HMIMANAGER_PATH[Host+Server] = null;
			return false;
		}else if (ManagerResponse.length === 0){
			// Opera bis exklusive version 9.5 liefert einen leeren responseText bei HTTP-Status 503
			HMI.hmi_log_error("HMIJavaScriptKSClient.prototype.getHMIManagerPointer - Empty Response");
			this.HMIMANAGER_PATH[Host+Server] = null;
			return false;
		}else if (ManagerResponseArray[0] === ""){
			HMI.hmi_log_error("HMIJavaScriptKSClient.prototype.getHMIManagerPointer - no instance found");
			this.HMIMANAGER_PATH[Host+Server] = null;
			return false;
		}
		
		var ManagerArray = ManagerResponseArray[0].split(' ');
		this.HMIMANAGER_PATH[Host+Server] = encodeURI(ManagerArray[0]);
		ManagerResponse = null;
		ManagerResponseArray = null;
		return true;
	},
	
	/*********************************
		getSheets
	*********************************/
	getSheets: function(Host, Server) {
		HMI.hmi_log_trace("HMIJavaScriptKSClient.prototype.getSheets - Start");
		
		var SheetList = new Array();
		
		if(this.cshmilib_path[Host+Server] === undefined && this.hmilib_path[Host+Server] === undefined){
			//we have no cached information about this server
			this.pingServer(Host, Server);
		}
		
		//get Sheets from cshmi library
		if(this.cshmilib_path[Host+Server] !== null){
			var responseArray;
			var lastEntry;
			var sortedList = Array();
			var cshmiString = this.getVar("//"+Host+"/"+Server+this.cshmilib_path[Host+Server]+'/Group.instance', "OP_VALUE", null);
			if (!(cshmiString && cshmiString.indexOf("KS_ERR") !== -1)){
				responseArray = this.splitKsResponse(cshmiString);
				//the array could be [""]
				if (responseArray.length > 0 && responseArray[0] !== ""){
					sortedList = responseArray[0].split(" ").sort();
				}
			}
			//detect all relevant groups
			for(var i = 0; i < sortedList.length;){
				if (sortedList[i].indexOf("/TechUnits/cshmi/Templates/") === 0){
					//hide all Groups in Templates
					sortedList.splice(i, 1);
				}else
				if (sortedList[i].indexOf(lastEntry) === 0){
					//this sheet is a child from the last sheet
					//so remove it from list
					//the next entry will move down, so index i is still valid
					sortedList.splice(i, 1);
				}else{
					//save Sheetname, but append a / to find only children not similar named sheets
					lastEntry = sortedList[i]+"/";
					i++;
				}
			}
			SheetList = SheetList.concat(sortedList);
			lastEntry = null;
			sortedList = Array();
		}
		
		
		//get Sheets from hmi library
		if(this.hmilib_path[Host+Server] !== null){
			//the path of the HMI Manager could be different in every OV Server
			if(!this.HMIMANAGER_PATH[Host+Server]){
				//hmimanager path is unknown right now
				this.getHMIManagerPointer(Host, Server);
			}
			if (this.HMIMANAGER_PATH[Host+Server] === null){
				HMI.hmi_log_trace("HMIJavaScriptKSClient.prototype.getSheets - No HMI Manager found on this Server.");
				return SheetList;
			}
			
			var Command = null;
			if (document.getElementById("idShowcomponents") && document.getElementById("idShowcomponents").checked){
				Command = [HMI.KSClient.getMessageID(),
				           '010',
				           this.HMIMANAGER_PATH[Host+Server],
				           'SHOWCOMPONENTS'];
			}else{
				Command = [HMI.KSClient.getMessageID(),
				           '010',
				           this.HMIMANAGER_PATH[Host+Server],
				           'SHOWSHEETS'];
			}
			
			this.setVar("//"+Host+"/"+Server+this.HMIMANAGER_PATH[Host+Server] + '.Command', Command, null);
			var Sheetstring = this.getVar("//"+Host+"/"+Server+this.HMIMANAGER_PATH[Host+Server] + '.CommandReturn', "OP_VALUE", null);
			Command = null;
			
			var responseArray = this.splitKsResponse(Sheetstring);
			//the array could be [""]
			if (responseArray.length > 0 && responseArray[0] !== ""){
				SheetList = SheetList.concat(responseArray[0].split(" ").sort());
			}
		}
		
		HMI.hmi_log_trace("HMIJavaScriptKSClient.prototype.getSheets - End");
		return SheetList;
	},
	
	/**
	 * calls callback function if loaded
	 * @param req XmlHttpRequest object
	 * @param cbfnc callback function
	 */
	_handleStateChange: function (req, cbfnc) {
		HMI.hmi_log_trace("HMIJavaScriptKSClient.prototype._handleStateChange - Start");
		
		switch (req.readyState) {
			case 0 : // UNINITIALIZED
			case 1 : // LOADING
			case 2 : // LOADED
			case 3 : // INTERACTIVE
				break;
			case 4 : // COMPLETED
				if (cbfnc !== null && cbfnc !== undefined)
				{
					cbfnc(this, req);
				};
				break;
			default : ; // FAILURE
		}
		
		HMI.hmi_log_trace("HMIJavaScriptKSClient.prototype._handleStateChange - End");
	},
	
	/**
	 * @param Client Instance of HMI Object
	 * @param method often "GET"
	 * @param async request async communication
	 * @param urlparameter the URL of the request
	 * @param cbfnc Name of the callack function or null
	 * @param responseFormat Mime-Type of requested response (probably "text/tcl", "text/ksx", "text/plain" used)
	 * @return if no cbfnc submitted returnvalue of request or false on an communication error
	 */
	_sendRequest: function(Client, method, async, urlparameter, cbfnc, responseFormat) {
		HMI.hmi_log_trace("HMIJavaScriptKSClient.prototype._sendRequest - Start, Async:"+async+" Meth:"+method+", requested: "+urlparameter);
		
		var req = new XMLHttpRequest();
		
		try {
			//FireFox 3 sends in a POST a content-encoding header killing the TCL Webserver
			//http://wiki.tcl.tk/2085 entry "nb Dec 18, 2008"
			//patch accepted but commented out in our
			// TCL-HTTPD/tcllib-1.6/modules/ncgi/ncgi.tcl
			
			//only send POST to non tcl servers!
			
			/*
			if(async){
				urlparameter +="&async=true";
			}else{
				urlparameter +="&async=false";
			}
			*/
			req.open(method, urlparameter, async);
			
			if ("kshttp" !== HMI.HMI_Constants.ServerType){
				//tcl webserver has buggy Cache-Control headers, so
				//prevent caching of request in all browsers (ie was the problem - as usual)
				req.setRequestHeader("If-Modified-Since", "Wed, 15 Nov 1995 04:58:08 GMT");
			}
			if(responseFormat){
				req.setRequestHeader("Accept", responseFormat);
			}
			
			if (req.timeout !== undefined && async === true){
				//timeout is not allowed with sync requests
				req.timeout = 5000;
			}
			
			if (async === true && cbfnc !== null && cbfnc !== undefined)
			{
				HMI.hmi_log_trace("HMIJavaScriptKSClient.prototype._sendRequest - entering async communication");
				//	Asynchron Communication
				//
				req.onreadystatechange = function () {
					Client._handleStateChange(req, cbfnc);
				};
			}
			
			try {
				if (method == 'POST') 
				{
					req.setRequestHeader('Content-Type', 'application/x-www-form-urlencoded');
					req.send('empty');
				} else {
					req.send(null);
				}
			} catch (e) {
				if ("kshttp" === HMI.HMI_Constants.ServerType){
					HMI.hmi_log_error('HMIJavaScriptKSClient._sendRequest: Request could not be sent.');
					HMI.hmi_log_onwebsite('Request could not be sent. Is the Server running?');
				}else{
					HMI.hmi_log_error('HMIJavaScriptKSClient._sendRequest: Request could not be sent. Is the gateway started?');
					HMI.hmi_log_onwebsite('Request could not be sent. Is the gateway started?');
				}
				window.clearInterval(HMI.RefreshTimeoutID);
				HMI.RefreshTimeoutID = null;
				return false;
			}
		} catch (e) {
			HMI.hmi_log_error("HMIJavaScriptKSClient._sendRequest: Error during request. "+e);
			HMI.hmi_log_onwebsite("Error during request.");
			return false;
		}
		if (async === false)
		{
			//	Synchron Communication
			//
			if (cbfnc !== null && cbfnc !== undefined){
				cbfnc(this, req);
			}else{
				HMI.hmi_log_trace("HMIJavaScriptKSClient.prototype._sendRequest - End");
				
				//FIXME req.status auswerten!
				return this.unescapeString(req.responseText);
			}
		}
		HMI.hmi_log_trace("HMIJavaScriptKSClient.prototype._sendRequest - End");
		return true;
	},
	
	/*********************************
		unescape server request
	*********************************/
	unescapeString: function(responseText) {
		var backslash = String.fromCharCode(92);
		var newline = String.fromCharCode(10);
		var tabstop = String.fromCharCode(9);
		var newString = responseText;
		newString = responseText
			.split(backslash+" ").join(" ")
			.split(backslash+"t").join(tabstop)
			.split(backslash+"n").join(newline)
			.split(backslash+'"').join('"')
			.split(backslash+"$").join("$")
			.split(backslash+";").join(";")
			.split(backslash+"{").join("{")
			.split(backslash+"}").join("}")
			.split(backslash+"[").join("[")
			.split(backslash+"]").join("]")
			;
		return newString;
	},
	
	/*********************************
		prepareComponentText
	*********************************/
	prepareComponentText: function(ComponentText) {
		HMI.hmi_log_trace("HMIJavaScriptKSClient.prototype.prepareComponentText - Start");
		
		if (ComponentText === null || ComponentText === ""){
			HMI.hmi_log_error("HMIJavaScriptKSClient.prototype.prepareComponentText - parameter was null or empty");
			return null;
		}
		var ReturnText;
		if ("kshttp" === HMI.HMI_Constants.ServerType){
			//remove ";" of text/plain encoding
			ReturnText = Array(ComponentText.slice(0, -1));
		}else{
			ReturnText = this.splitKsResponse(ComponentText);
		}
		
		//	ComponentText should look like:
		//		{{GraphicDescription}} {{StyleDescription}}
		//	TksS-XXXX indicates error
		//
		if (ReturnText.length === 0 ){
			var ErrorMessage;
			
			HMI.hmi_log_error('HMIJavaScriptKSClient.prototype.prepareComponentText: ' + ComponentText);
			if (ComponentText.indexOf("TksS-") !== -1){
				ErrorMessage = "Server lost.";
			}else{
				ErrorMessage = "Server did not responded valid.";
			}
			if (ComponentText.length < 250){
				HMI.hmi_log_onwebsite(ErrorMessage+' Gateway reply: ' + ComponentText);
			}else{
				HMI.hmi_log_onwebsite(ErrorMessage+' First 250 characters of reply: ' + ComponentText.substr(0,250));
			}
			window.clearInterval(HMI.RefreshTimeoutID);
			HMI.RefreshTimeoutID = null;
			return null;
		}else if (ReturnText[0] === ""){
				HMI.hmi_log_onwebsite('Gateway reply was empty.');
				return null;
		}
		
		HMI.hmi_log_trace("HMIJavaScriptKSClient.prototype.prepareComponentText - End");
		
		return ReturnText[0];
	},
	
	/*********************************
		splitKsResponse
			returns the KS Response as an Array, or an empty Array
			if the optional argument recursionDepth is > 0,  
	*********************************/
	splitKsResponse: function (response, recursionDepth, ignoreError) {
		//Checking if optional argument is set, else set to default: 0
		if (recursionDepth === undefined){
			recursionDepth = 0;
		}
		//check input
		if (response === null){
			return Array();
		}else if (response === undefined){
			return Array();
		}else if (response === ""){
			return Array();
		}else if (response === false){
			//communication error
			return Array();
		}else if (response === "{{}}"){
			return Array("");
		}else if (ignoreError !== true && response.indexOf("KS_ERR") !== -1){
			return Array();
		}

		var indexOfCurrentOpenBracket = 0;
		var indexOfNextOpenBracket = 0;
		var indexOfCurrentClosedBracket = 0;
		var indexOfNextClosedBracket = 0;
		var startIndexOfCurrentString = 0;
		
		var responseArray = [];
		var currentArrayElement = "";

		//search 1st open bracket
		indexOfCurrentOpenBracket = response.indexOf("{", 0);
		
		//if there are no brackets at all
		if (indexOfCurrentOpenBracket === -1){
			return response.split(" ");
		}

		//add array elements if they are listed before the 1st open bracket
		//e.g. "maxcalctime KS_OT_VARIABLE {maximum calculation time}"
		if (indexOfCurrentOpenBracket > 0) {
			//e.g. startIndexOfCurrentString = 0, indexOfCurrentOpenBracket = 27 --> currentArrayElement = "maxcalctime KS_OT_VARIABLE"
			currentArrayElement = response.slice(startIndexOfCurrentString, indexOfCurrentOpenBracket-1);
			//if there are space characters, we need to split currentArrayElement and append each of the elements to the responseArray
			currentArrayElement = currentArrayElement.split(" ");
			for (var i=0; i < currentArrayElement.length; i++) {
				responseArray.push(currentArrayElement[i]);
			}
		}
		startIndexOfCurrentString = indexOfCurrentOpenBracket;

		while(true){		
			
			indexOfNextOpenBracket = (response.indexOf("{", indexOfCurrentOpenBracket+1));
			indexOfNextClosedBracket = (response.indexOf("}", indexOfCurrentClosedBracket+1));
			
			// we have reached the closed bracket of an element
			if (indexOfNextClosedBracket < indexOfNextOpenBracket  || indexOfNextOpenBracket === -1) {
				//e.g. "{{/TechUnits/Pump1}} {{pumpcolor:yellow pumpname:N18}}"
				//indexOfNextClosedBracket = 19, indexOfNextOpenBracket = 21, startIndexOfCurrentString = 0 --> currentArrayElement = {/TechUnits/Pump1}
				currentArrayElement = response.slice(startIndexOfCurrentString+1, indexOfNextClosedBracket);

				if (recursionDepth > 0){
					currentArrayElement = this.splitKsResponse(currentArrayElement, recursionDepth-1, ignoreError);
				}
				//get a rid of the remaining brackets because we are done with this element 
				//e.g "{/TechUnits/Pump1}"
				else{
					currentArrayElement = currentArrayElement.replace(/{/g, "");
					currentArrayElement = currentArrayElement.replace(/}/g, "");
				}
				responseArray.push(currentArrayElement);
				
				//append elements if they are not between brackets
				//e.g. "{KS_AC_READ KS_AC_WRITE KS_AC_PART} hp {2011-12-13 12:14:57.767}"
				// there is more then just a space character between "{KS_AC_READ KS_AC_WRITE KS_AC_PART}" and "{2011-12-13 12:14:57.767}"
				if (indexOfNextClosedBracket+1 !== indexOfNextOpenBracket-1 && indexOfNextOpenBracket !== -1){
					currentArrayElement = response.slice(indexOfNextClosedBracket+2, indexOfNextOpenBracket-1);
					//if there are space characters, we need to split currentArrayElement and append each of the elements to the responseArray
					currentArrayElement = currentArrayElement.split(" ");
					for (var i=0; i < currentArrayElement.length; i++) {
						responseArray.push(currentArrayElement[i]);
					}
				}
				
				//check if we have reached the last closed bracket
				if (indexOfNextOpenBracket === -1){
					//append elements if they come after the last closed bracket
					//e.g. {2011-12-29 12:01:44.981} KS_VT_TIME_SPAN sec
					if (response.length > indexOfNextClosedBracket+1){
						currentArrayElement = response.slice(indexOfNextClosedBracket+2);
						//if there are space characters, we need to split currentArrayElement and append each of the elements to the responseArray
						currentArrayElement = currentArrayElement.split(" ");
						for (var i=0; i < currentArrayElement.length; i++) {
							responseArray.push(currentArrayElement[i]);
						}
					}
					
					//we are done with the split
					return responseArray;
				}
				
				//prepare to search next element that is beetween brackets
				indexOfCurrentOpenBracket = indexOfNextOpenBracket;
				indexOfCurrentClosedBracket = indexOfNextClosedBracket;
				startIndexOfCurrentString = indexOfNextOpenBracket;
			}

			//close bracket of an element not reached yet
			else {
				indexOfCurrentOpenBracket = indexOfNextOpenBracket;
				indexOfCurrentClosedBracket = indexOfNextClosedBracket;
			}
		}
	},
	
	/**
	 * returns the childs of an Object as an Array, or an empty Array
	 */
	getChildObjArray: function (ObjectPath, cachingTarget, cbfnc) {
		var responseArray;
		if (!(cachingTarget.ResourceList.ChildList && cachingTarget.ResourceList.ChildList[ObjectPath] !== undefined)){
			
			if(cbfnc === undefined){
				var response = this.getEP(encodeURI(ObjectPath), "OT_DOMAIN", ["OP_NAME", "OP_CLASS"], null);
			}else{
				response = this.getEP(encodeURI(ObjectPath), "OT_DOMAIN", ["OP_NAME", "OP_CLASS"], cbfnc, true);
				return;
			}
			
			//no caching with an communication error
			if (response === false){
				//communication error
				return Array();
			}else if (response === null){
				HMI.hmi_log_error("HMIJavaScriptKSClient.getChildObjArray of "+ObjectPath+" failed: response was null");
				return Array();
			}else if (response.indexOf("KS_ERR") !== -1){
				HMI.hmi_log_error("HMIJavaScriptKSClient.getChildObjArray of "+ObjectPath+" failed: "+response);
				return Array();
			}
			
			responseArray = this.splitKsResponse(response);
			
			//prepare main js-object to remember config of getValue OV-Object
			cachingTarget.ResourceList.ChildList[ObjectPath] = new Object();
			cachingTarget.ResourceList.ChildList[ObjectPath].ChildListParameters = responseArray;
		}else{
			responseArray = cachingTarget.ResourceList.ChildList[ObjectPath].ChildListParameters;
		}
		return responseArray;
	},
	
	/**
	 * returns a valid HostAndServer, Path Array
	 * @return Array with HostAndServer and Path as String or null (in Error) and an infostring ("emptystring", "fullpath", "relativepath" or "genericerror") in Array
	 */
	_splitKSPath: function(FullKSpath){
		if (FullKSpath.length === 0){
			return Array(null, "");
		}
		var resultArray = new Array(null);
		if(Object.prototype.toString.call(FullKSpath) !== "[object Array]"){
			//force a string input in Array
			FullKSpath = Array(FullKSpath);
		}
		for(var i=0;i<FullKSpath.length;i++){
			if(!FullKSpath[i]){
				//no valid input
				resultArray.push("");
			}else if(FullKSpath[i].charAt(0) === "/" && FullKSpath[i].charAt(1) === "/"){
				//full path
				
				//find the 3rd "/"
				var slashIndexAfterHost = FullKSpath[i].indexOf("/", 2);
				//find the 4th "/"
				var slashIndexAfterServer = FullKSpath[i].indexOf("/", slashIndexAfterHost+1);
				//only keep the String before 4th "/"
				//var Host = FullKSpath.slice(2, slashIndexAfterHost);
				//var Server = FullKSpath.slice(slashIndexAfterHost+1, slashIndexAfterServer);
				var HostAndServer = FullKSpath[i].slice(2, slashIndexAfterServer);
				var KSPath = FullKSpath[i].slice(slashIndexAfterServer);
				
				if(resultArray[0] === null){
					resultArray[0] = HostAndServer;
				}
				resultArray.push(KSPath);
			}else if(FullKSpath[i].charAt(0) === "/"){
				//no Host and Server found, so replace with Model location
				if(resultArray[0] === null){
					if(this.ResourceList.ModelHost !== null && this.ResourceList.ModelServer !== null){
						resultArray[0] = this.ResourceList.ModelHost+"/"+this.ResourceList.ModelServer;
					}else{
						//not configured. Fall back to same server
						resultArray[0] = "";
					}
				}
				resultArray.push(FullKSpath[i]);
			}else if(i>0){
				//relativ path allowed, if not the first entry
				resultArray.push(FullKSpath[i]);
			}else{
				//no valid input
				resultArray.push("");
			}
		}
		return resultArray;
	},
	
	/*********************************
		destroy
	*********************************/
	destroy: function () {
		this.HMIMANAGER_PATH = Object();
		this.hmilib_path = Object();
		this.cshmilib_path = Object();
		
		//destroy all handles
		for (var i in this.ResourceList.Servers){
			if(this.ResourceList.Servers[i].HandleString !== undefined){
				this.deleteCommunicationPoint(this.ResourceList.Servers[i].HandleString);
				delete this.ResourceList.Servers[i].HandleString;
			}
			delete this.ResourceList.Servers[i];
		}
	}
};
var filedate = "$Date$";
filedate = filedate.substring(7, filedate.length-2);
if ("undefined" == typeof HMIdate){
	HMIdate = filedate;
}else if (HMIdate < filedate){
	HMIdate = filedate;
}
