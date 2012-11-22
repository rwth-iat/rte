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
*		-	HMI Requests changed to getEP, getVar, setVar, getHandle, delHandle
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
	
	//object to cache communication details
	this.ResourceList = Object();
	
	//needed in the gestures and SHOWSHEETS
	this.HMIMANAGER_PATH = null;
	
	//needed for absolute ks path on the same ksserver
	this.ResourceList.ModelHost = null;
	this.ResourceList.ModelServer = null;
	this.ResourceList.Handles = Object();
	
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
	 * @param [requestType = OT_DOMAIN] type of KS Object to query (OT_DOMAIN, OT_VARIABLE, OT_LINK or OT_ANY)
	 * @param requestOutput Array of interesting objects properties (OP_NAME, OP_TYPE, OP_COMMENT, OP_ACCESS, OP_SEMANTIC, OP_CREATIONTIME, OP_CLASS or OT_ANY)
	 * @param cbfnc callback function
	 * @param async request async communication
	 * @return "{fb_hmi1} {fb_hmi2} {fb_hmi3} {MANAGER} {fb_hmi4} {fb_hmi5}" or null or true (if callback used)
	 */
	getEP: function(path, requestType, requestOutput, cbfnc, async) {
		if(requestType === undefined){
			requestType = "OT_DOMAIN";
		}
		if(requestOutput === undefined){
			requestOutput = "OP_NAME";
		}
		HMI.hmi_log_trace("HMIJavaScriptKSClient.prototype.getEP - Start: "+path);
		if(!path || path.length === 0){
			HMI.hmi_log_error("HMIJavaScriptKSClient.prototype.getEP - no path found");
			return null;
		}
		//if (path.indexof("http:") === 0){}else		//ksservhttp handling here
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
		if ("tcl" === HMI.HMI_Constants.ServerType){
			Handle = this.getHandleID(ServerAndPath[0]);
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
				optionalurlparameter += "%20-output%20[expr%20$::TKS::"
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
			if (ServerAndPath[0].indexOf(window.location.hostname) === -1){
				HMI.hmi_log_error("HMIJavaScriptKSClient.prototype.getEP - "+ServerAndPath[0]+" is not on the same host as our source. GetEP on a remote host is not supported right now. ");
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
			urlparameter = "/getEP?path=" +ServerAndPath[1]+optionalurlparameter;
		}else if ("php" === HMI.HMI_Constants.ServerType){
			//todo implement requestType, requestOutput
			Handle = this.getHandleID(ServerAndPath[0]);
			if(Handle === null){
				return null;
			}
			urlparameter = HMI.KSGateway_Path+"?obj="+Handle + "&cmd=getep&path=" +ServerAndPath[1]+"%20*";
		}else{
			//communication type not implemented
			return null;
		}
		if (async === true && cbfnc !== null && cbfnc !== undefined){
			this._sendRequest(this, method, true, urlparameter, cbfnc);
		}else if (cbfnc !== null && cbfnc !== undefined){
			this._sendRequest(this, method, false, urlparameter, cbfnc);
		}else{
			var ReturnText = this._sendRequest(this, method, false, urlparameter, cbfnc);
			HMI.hmi_log_trace("HMIJavaScriptKSClient.prototype.getEP - End");
			return ReturnText;
		}
		
		HMI.hmi_log_trace("HMIJavaScriptKSClient.prototype.getEP - End");
		return true;
	},
	
	/**
	 * usage example:
	 *		getHandle("localhost/MANAGER", this._cbInit);
	 *		getHandle("localhost/fb_hmi1", null);
	 * @param {String} HostAndServername Host and Servername concat with a slash
	 * @param cbfnc callback function
	 * @param async request async communication
	 * @return {String} TKS Handle of the requested Server or null
	 */
	_getHandle: function(HostAndServer, cbfnc, async) {
		HMI.hmi_log_trace("HMIJavaScriptKSClient.prototype._getHandle - Start on "+HostAndServer);
		
		//fixme auf async umstellen
		var urlparameter;
		var newhost;
		var method = "GET";
		var hostArray = HostAndServer.split("/");
		if (hostArray.length !== 2){
			return null;
		}
		if (hostArray[0].indexOf(":") === -1){
			//add default port if the request did not set it
			newhost = hostArray[0]+ ':7509/' + hostArray[1];
		}else{
			newhost = HostAndServer;
		}
		
		if ("tcl" === HMI.HMI_Constants.ServerType){
			urlparameter = HMI.KSGateway_Path+"?obj=tks-server&args="+newhost;
		}else if("php" === HMI.HMI_Constants.ServerType){
			urlparameter = HMI.KSGateway_Path+"?cmd=tks-server&args="+newhost;
		}
		var ReturnText = this._sendRequest(this, method, false, urlparameter, null);
		
		if (ReturnText === false){
			//communication error
			return null;
		}else if(ReturnText === true){
			//no async communication. oops
			return null;
		}else if (ReturnText.indexOf("KS_ERR") !== -1 && HostAndServer !== newhost){
			//on error retest without default acplt port
			if ("tcl" === HMI.HMI_Constants.ServerType){
				urlparameter = 'obj=tks-server&args='+HostAndServer;
			}else if("php" === HMI.HMI_Constants.ServerType){
				urlparameter = 'cmd=tks-server&args='+HostAndServer;
			}
			ReturnText = this._sendRequest(this, method, false, urlparameter, null);
		}
		HMI.hmi_log_trace("HMIJavaScriptKSClient.prototype._getHandle - End");
		return ReturnText;
	},
	
	/**
	 * usage example:
	 *		ManagerResponse = this.getVar(TCLKSHandle, "/Libraries/hmi/Manager.instance", null)
	 *		this.getVar(null, '/TechUnits/HMIManager.CommandReturn', this._cbGetSheets);
	 * 
	 * @param path of the variable to fetch, multiple path are {part1} {part1} coded (used in GraphicDescription+StyleDescription)
	 * @param requestOutput Array of interesting objects properties (OP_NAME, OP_TYPE, OP_VALUE, OP_TIMESTAMP or OP_STATE)
	 * @param cbfnc callback function
	 * @param async request async communication
	 * @return "{{/TechUnits/HMIManager}}", response: "{/TechUnits/Sheet1}" or "TksS-0042::KS_ERR_BADPATH {{/Libraries/hmi/Manager.instance KS_ERR_BADPATH}}"
	 */
	getVar: function(path, requestOutput, cbfnc, async) {
		if(requestOutput === undefined){
			requestOutput = "OP_VALUE";
		}
		HMI.hmi_log_trace("HMIJavaScriptKSClient.prototype.getVar - Start: "+path);
		if(!path || path.length === 0){
			HMI.hmi_log_error("HMIJavaScriptKSClient.prototype.getVar - no path found");
			return null;
		}
		//if (path.indexof("http:") === 0){}else		//ksservhttp handling here
		if(path.charAt(0) !== "/"){
			HMI.hmi_log_error("HMIJavaScriptKSClient.prototype.getVar - no valid path found, path was: "+path);
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
		if ("tcl" === HMI.HMI_Constants.ServerType){
			Handle = this.getHandleID(ServerAndPath[0]);
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
				optionalurlparameter = "%20-output%20[expr%20$::TKS::"
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
				"{"+ServerAndPath[1]+"}" + optionalurlparameter;
		}else if ("kshttp" === HMI.HMI_Constants.ServerType){
			if (ServerAndPath[0].indexOf(window.location.hostname) === -1){
				HMI.hmi_log_error("HMIJavaScriptKSClient.prototype.getVar - "+ServerAndPath[0]+" is not on the same host as our source. GetVar on a remote host is not supported right now. ");
				return null;
			}
			
			urlparameter = "/getVar?path=" +ServerAndPath[1];
		}else if ("php" === HMI.HMI_Constants.ServerType){
			Handle = this.getHandleID(ServerAndPath[0]);
			if(Handle === null){
				return null;
			}
			urlparameter = HMI.KSGateway_Path+"?obj="+Handle + "&cmd=getvar&path=" +
				"{"+ServerAndPath[1]+"}";
		}else{
			//communication type not implemented
			return null;
		}
		if (async === true && cbfnc !== null && cbfnc !== undefined){
			this._sendRequest(this, method, true, urlparameter, cbfnc);
		}else if (cbfnc !== null && cbfnc !== undefined){
			this._sendRequest(this, method, false, urlparameter, cbfnc);
		}else{
			var ReturnText = this._sendRequest(this, method, false, urlparameter, cbfnc);
			HMI.hmi_log_trace("HMIJavaScriptKSClient.prototype.getVar - End");
			return ReturnText;
		}
		
		HMI.hmi_log_trace("HMIJavaScriptKSClient.prototype.getVar - End");
		return true;
	},
	
	
	//fixme return value "" in true wandeln
	/**
	 * usage example:
	 *		this.setVar(null, '/TechUnits/HMIManager.Command ', "{1} {010} {/TechUnits/HMIManager} {SHOWSHEETS}", null);
	 * 
	 * @param path of the variable to set
	 * @param {String} value to set (StringVec are {part1} {part2} {part3} coded
	 * @param {String} type variable type to set, null if no change
	 * @param cbfnc callback function
	 * @param async request async communication
	 * @return "" or null
	 */
	setVar: function(Handle, path, value, cbfnc, async) {
		//wrapper function for old hmi gestures
		return this.setVar_NG(path, value, null, cbfnc, async)
	},
	setVar_NG: function(path, value, type, cbfnc, async) {
		HMI.hmi_log_trace("HMIJavaScriptKSClient.prototype.setVar - Start: "+path);
		if(!path || path.length === 0){
			HMI.hmi_log_error("HMIJavaScriptKSClient.prototype.setVar - no path found");
			return null;
		}
		//if (path.indexof("http:") === 0){}else		//ksservhttp handling here
		if(path.charAt(0) !== "/"){
			HMI.hmi_log_error("HMIJavaScriptKSClient.prototype.setVar - no valid path found, path was: "+path);
			return null;
		}
		
		var ServerAndPath = this._splitKSPath(path);
		var method = "GET";
		var Handle;
		var urlparameter;
		var optionalurlparameter = "";
		if ("tcl" === HMI.HMI_Constants.ServerType){
			Handle = this.getHandleID(ServerAndPath[0]);
			if(Handle === null){
				return null;
			}
			if(type !== null && type !== undefined){
				optionalurlparameter = "%20-type%20"+type;
			}
			
			urlparameter = HMI.KSGateway_Path+"?obj="+Handle + "&args=setvar%20"+
			"{"+ServerAndPath[1]+"%20{"+value+"}}"+optionalurlparameter;
		}else if ("kshttp" === HMI.HMI_Constants.ServerType){
			if (ServerAndPath[0].indexOf(window.location.hostname) === -1){
				HMI.hmi_log_error("HMIJavaScriptKSClient.prototype.setVar_NG - "+ServerAndPath[0]+" is not on the same host as our source. SetVar on a remote host is not supported right now. ");
				return null;
			}
			
			//method = "PUT"; //REST conform
			urlparameter = "/setVar?path=" +ServerAndPath[1]+"&newvalue="+value;
		}else if ("php" === HMI.HMI_Constants.ServerType){
			Handle = this.getHandleID(ServerAndPath[0]);
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
			this._sendRequest(this, method, true, urlparameter, cbfnc);
		}else if (cbfnc !== null && cbfnc !== undefined){
			this._sendRequest(this, method, false, urlparameter, cbfnc);
		}else{
			var ReturnText = this._sendRequest(this, method, false, urlparameter, cbfnc);
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
	 * @return true, "" or null
	 */
	renameObject: function(oldname, newname, cbfnc, async) {
		HMI.hmi_log_trace("HMIJavaScriptKSClient.prototype.renameObject - Start: "+oldname);
		if(!oldname || oldname.length === 0){
			HMI.hmi_log_error("HMIJavaScriptKSClient.prototype.renameObject - no oldname found");
			return null;
		}
		if(!newname || newname.length === 0){
			HMI.hmi_log_error("HMIJavaScriptKSClient.prototype.renameObject - no newname found");
			return null;
		}
		//if (oldname.indexof("http:") === 0){}else		//ksservhttp handling here
		if(oldname.charAt(0) !== "/"){
			HMI.hmi_log_error("HMIJavaScriptKSClient.prototype.renameObject - no valid oldname found, oldname was: "+oldname);
			return null;
		}
		
		var ServerAndPath = this._splitKSPath(oldname);
		var ServerAndPathNewname = this._splitKSPath(newname);
		if(newname.charAt(0) !== "/"){
			//we have a plain name, add path of the oldname
			var PathArray = ServerAndPath[1].split("/");
			newname = ServerAndPath[1].replace( PathArray[PathArray.length - 1], newname);
		}else if (ServerAndPath[0] === ServerAndPathNewname[0]){
			//both names are within the same server
			newname = ServerAndPathNewname[1];
		}else if (ServerAndPath[0] !== ServerAndPathNewname[0] &&
				ServerAndPath[2] === "fullpath" && ServerAndPathNewname[2] === "fullpath"){
			//two full path names given , but on different server not possible
			return "KS_ERR_BADPATH";
		}
		
		var Handle;
		var urlparameter;
		var method = "GET";
		if ("tcl" === HMI.HMI_Constants.ServerType){
			Handle = this.getHandleID(ServerAndPath[0]);
			if(Handle === null){
				return null;
			}
			urlparameter = HMI.KSGateway_Path+"?obj="+Handle + "&args=rename%20"+
				ServerAndPath[1]+"%20"+newname;
		}else if ("kshttp" === HMI.HMI_Constants.ServerType){
			if (ServerAndPath[0].indexOf(window.location.hostname) === -1){
				HMI.hmi_log_error("HMIJavaScriptKSClient.prototype.renameObject - "+ServerAndPath[0]+" is not on the same host as our source. rename on a remote host is not supported right now. ");
				return null;
			}
			
			//urlparameter = "";
			//not implemented
			return null;
		}else if ("php" === HMI.HMI_Constants.ServerType){
			//not implemented!
			Handle = this.getHandleID(ServerAndPath[0]);
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
			this._sendRequest(this, method, true, urlparameter, cbfnc);
		}else if (cbfnc !== null && cbfnc !== undefined){
			this._sendRequest(this, method, false, urlparameter, cbfnc);
		}else{
			var ReturnText = this._sendRequest(this, method, false, urlparameter, cbfnc);
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
	 * @return true, "" or null
	 */
	createObject: function(path, classname, cbfnc, async) {
		HMI.hmi_log_trace("HMIJavaScriptKSClient.prototype.createObject - Start: "+path);
		if(!path || path.length === 0){
			HMI.hmi_log_error("HMIJavaScriptKSClient.prototype.createObject - no path found");
			return null;
		}
		if(!classname || classname.length === 0){
			HMI.hmi_log_error("HMIJavaScriptKSClient.prototype.createObject - no classname found");
			return null;
		}
		//if (path.indexof("http:") === 0){}else		//ksservhttp handling here
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
		if ("tcl" === HMI.HMI_Constants.ServerType){
			Handle = this.getHandleID(ServerAndPath[0]);
			if(Handle === null){
				return null;
			}
			urlparameter = HMI.KSGateway_Path+"?obj="+Handle + "&args=create%20{"+
			ServerAndPath[1]+"%20"+classname+"}";
		}else if ("kshttp" === HMI.HMI_Constants.ServerType){
			if (ServerAndPath[0].indexOf(window.location.hostname) === -1){
				HMI.hmi_log_error("HMIJavaScriptKSClient.prototype.creatObject - "+ServerAndPath[0]+" is not on the same host as our source. create on a remote host is not supported right now. ");
				return null;
			}
			
			//urlparameter = "";
			//not implemented
			return null;
		}else if ("php" === HMI.HMI_Constants.ServerType){
			//not implemented!
			Handle = this.getHandleID(ServerAndPath[0]);
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
			this._sendRequest(this, method, true, urlparameter, cbfnc);
		}else if (cbfnc !== null && cbfnc !== undefined){
			this._sendRequest(this, method, false, urlparameter, cbfnc);
		}else{
			var ReturnText = this._sendRequest(this, method, false, urlparameter, cbfnc);
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
	 * @return true, "" or null
	 */
	deleteObject: function(path, cbfnc, async) {
		HMI.hmi_log_trace("HMIJavaScriptKSClient.prototype.deleteObject - Start: "+path);
		if(!path || path.length === 0){
			HMI.hmi_log_error("HMIJavaScriptKSClient.prototype.deleteObject - no path found");
			return null;
		}
		//if (path.indexof("http:") === 0){}else		//ksservhttp handling here
		if(path.charAt(0) !== "/"){
			HMI.hmi_log_error("HMIJavaScriptKSClient.prototype.deleteObject - no valid path found, path was: "+path);
			return null;
		}
		
		var ServerAndPath = this._splitKSPath(path);
		var Handle;
		var urlparameter;
		var method = "GET";
		if ("tcl" === HMI.HMI_Constants.ServerType){
			Handle = this.getHandleID(ServerAndPath[0]);
			if(Handle === null){
				return null;
			}
			urlparameter = HMI.KSGateway_Path+"?obj="+Handle + "&args=delete%20"+
			ServerAndPath[1];
		}else if ("kshttp" === HMI.HMI_Constants.ServerType){
			if (ServerAndPath[0].indexOf(window.location.hostname) === -1){
				HMI.hmi_log_error("HMIJavaScriptKSClient.prototype.deleteObject - "+ServerAndPath[0]+" is not on the same host as our source. delete on a remote host is not supported right now. ");
				return null;
			}
			
			//urlparameter = "";
			//not implemented
			return null;
		}else if ("php" === HMI.HMI_Constants.ServerType){
			Handle = this.getHandleID(ServerAndPath[0]);
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
			this._sendRequest(this, method, true, urlparameter, cbfnc);
		}else if (cbfnc !== null && cbfnc !== undefined){
			this._sendRequest(this, method, false, urlparameter, cbfnc);
		}else{
			var ReturnText = this._sendRequest(this, method, false, urlparameter, cbfnc);
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
	 * @return true, "" or null
	 */
	linkObjects: function(pathA, pathB, portnameA, cbfnc, async) {
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
		//if (path.indexof("http:") === 0){}else		//ksservhttp handling here
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
		if ("tcl" === HMI.HMI_Constants.ServerType){
			Handle = this.getHandleID(ServerAndPath[0]);
			if(Handle === null){
				return null;
			}
			urlparameter = HMI.KSGateway_Path+"?obj="+Handle + "&args=link%20"+
			ServerAndPath[1]+"."+portnameA+"%20"+ServerAndPathB[1];
		}else if ("kshttp" === HMI.HMI_Constants.ServerType){
			if (ServerAndPath[0].indexOf(window.location.hostname) === -1){
				HMI.hmi_log_error("HMIJavaScriptKSClient.prototype.linkObject - "+ServerAndPath[0]+" is not on the same host as our source. Link on a remote host is not supported right now. ");
				return null;
			}
			
			//urlparameter = "";
			//not implemented
			return null;
		}else if ("php" === HMI.HMI_Constants.ServerType){
			//not implemented!
			Handle = this.getHandleID(ServerAndPath[0]);
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
			this._sendRequest(this, method, true, urlparameter, cbfnc);
		}else if (cbfnc !== null && cbfnc !== undefined){
			this._sendRequest(this, method, false, urlparameter, cbfnc);
		}else{
			var ReturnText = this._sendRequest(this, method, false, urlparameter, cbfnc);
			HMI.hmi_log_trace("HMIJavaScriptKSClient.prototype.linkObjects - End");
			return ReturnText;
		}
		
		HMI.hmi_log_trace("HMIJavaScriptKSClient.prototype.linkObjects - End");
		return true;
	},
	
	/**
	 * links two objects
	 * 
	 * @param path of the object to create
	 * @param classname full class name of the new object
	 * @param cbfnc callback function
	 * @param async request async communication
	 * @return true, "" or null
	 */
	unlinkObjects: function(pathA, pathB, portnameA, cbfnc, async) {
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
		//if (path.indexof("http:") === 0){}else		//ksservhttp handling here
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
		if ("tcl" === HMI.HMI_Constants.ServerType){
			Handle = this.getHandleID(ServerAndPath[0]);
			if(Handle === null){
				return null;
			}
			urlparameter = HMI.KSGateway_Path+"?obj="+Handle + "&args=unlink%20"+
			ServerAndPath[1]+"."+portnameA+"%20"+ServerAndPathB[1];
		}else if ("kshttp" === HMI.HMI_Constants.ServerType){
			if (ServerAndPath[0].indexOf(window.location.hostname) === -1){
				HMI.hmi_log_error("HMIJavaScriptKSClient.prototype.unlinkObject - "+ServerAndPath[0]+" is not on the same host as our source. unlink on a remote host is not supported right now. ");
				return null;
			}
			
			//urlparameter = "";
			//not implemented
			return null;
		}else if ("php" === HMI.HMI_Constants.ServerType){
			//not implemented!
			Handle = this.getHandleID(ServerAndPath[0]);
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
			this._sendRequest(this, method, true, urlparameter, cbfnc);
		}else if (cbfnc !== null && cbfnc !== undefined){
			this._sendRequest(this, method, false, urlparameter, cbfnc);
		}else{
			var ReturnText = this._sendRequest(this, method, false, urlparameter, cbfnc);
			HMI.hmi_log_trace("HMIJavaScriptKSClient.prototype.unlinkObjects - End");
			return ReturnText;
		}
		
		HMI.hmi_log_trace("HMIJavaScriptKSClient.prototype.unlinkObjects - End");
		return true;
	},
	
	/**
	 * usage example:
	 * 		this.delHandle("TksS-0042");
	 * @param Handle Handle to be destroyed
	 * @return none
	 */
	delHandle: function(Handle) {
		var urlparameter;
		var method = "GET";
		if ("tcl" === HMI.HMI_Constants.ServerType){
			urlparameter = HMI.KSGateway_Path+"?obj="+Handle + "&args=destroy";
		}else if ("php" === HMI.HMI_Constants.ServerType){
			urlparameter = HMI.KSGateway_Path+"?obj="+Handle + "&cmd=destroy";
		}else{
			//communication type not implemented
			return;
		}
		if (Handle !== null){
			//the result is not interesting, so can be ignored (async, no callback)
			this._sendRequest(this, method, true, urlparameter, null);
		}
	},
	
	/**
	 * @param {String} HostAndServername Host and Servername concat with a slash
	 * @return {String} TKS Handle of the requested Server or null
	 * @todo add timeout für unused delHandle
	 */
	getHandleID: function(HostAndServername) {
		if (this.ResourceList.Handles[HostAndServername] && this.ResourceList.Handles[HostAndServername].HandleString !== undefined){
			return this.ResourceList.Handles[HostAndServername].HandleString;
		}else{
			var HandleString = this._getHandle(HostAndServername, null);
			if (HandleString === null || HandleString === false){
				//occures in shutdown
				return null;
			}else if (HandleString.indexOf("KS_ERR_SERVERUNKNOWN") !== -1){
				//the Manager sometimes reject connection to a valid server, so retry once
				//HMI.hmi_log_trace("HMIJavaScriptKSClient.prototype.getHandleID - got KS_ERR_SERVERUNKNOWN but do not trust. Retrying");
				
				//retry disabled for performance reasons!
				//HandleString = this._getHandle(HostAndServername, null);
			}
			if (HandleString.indexOf("KS_ERR") !== -1){
				//the server is really not available. Could be the case if there is an active KS-Bridge and its destination is not available
				
				//caching the result of the failure
				HandleString = null;
			}else if (!/^TksS-\b/.exec(HandleString)){
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
			this.ResourceList.Handles[HostAndServername] = Object();
			this.ResourceList.Handles[HostAndServername].HandleString = HandleString;
			return HandleString;
		}
	},
	
	_cbSetDefaultHandle: function(Client, req) {
		//fixme work in progress
		Client.ResourceList.Handles[HostAndServername] = Object();
		Client.ResourceList.Handles[HostAndServername].HandleString = HandleString;
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
				HMI.PossServers.disabled = false;
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
		var Response = this.getVar("//"+Host+"/"+Server+"/acplt/ov/library.instance", "OP_VALUE", null);
		if (!Response){
			HMI.hmi_log_trace("HMIJavaScriptKSClient.prototype.pingServer - communication problem, so no hmi and no cshmi server");
			return false;
		}else if (Response.indexOf("KS_ERR") !== -1){
			HMI.hmi_log_trace("HMIJavaScriptKSClient.prototype.pingServer - no OV server (perhaps MANAGER), so no hmi and no cshmi server");
			return false;
		}else if (	Response.indexOf("/hmi") !== -1
				||	Response.indexOf("/cshmi") !== -1){
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
		var ManagerResponse = this.getVar("//"+Host+"/"+Server+"/Libraries/hmi/Manager.instance", "OP_VALUE", null);
		
		var ManagerResponseArray = this.splitKsResponse(ManagerResponse);
		if (ManagerResponseArray.length === 0){
			HMI.hmi_log_trace("HMIJavaScriptKSClient.prototype.getHMIManagerPointer - Response invalid");
			this.HMIMANAGER_PATH = null;
			return false;
		}else if (ManagerResponse.length === 0){
			// Opera bis exklusive version 9.5 liefert einen leeren responseText bei HTTP-Status 503
			HMI.hmi_log_error("HMIJavaScriptKSClient.prototype.getHMIManagerPointer - Empty Response");
			this.HMIMANAGER_PATH = null;
			return false;
		}else if (ManagerResponseArray[0] === ""){
			HMI.hmi_log_error("HMIJavaScriptKSClient.prototype.getHMIManagerPointer - no instance found");
			this.HMIMANAGER_PATH = null;
			return false;
		}
		
		var ManagerArray = ManagerResponseArray[0].split(' ');
		this.HMIMANAGER_PATH = encodeURI(ManagerArray[0]);
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
		
		//get Sheets from cshmi library
		var responseArray;
		var lastEntry;
		var sortedList = Array();
		var cshmiString = this.getVar("//"+Host+"/"+Server+'/Libraries/cshmi/Group.instance', "OP_VALUE", null);
		if (!(cshmiString && cshmiString.indexOf("KS_ERR") !== -1)){
			responseArray = this.splitKsResponse(cshmiString);
			//the array could be [""]
			if (responseArray.length > 0 && responseArray[0] !== ""){
				sortedList = responseArray[0].split(" ").sort();
			}
		}else{
			cshmiString = this.getVar("//"+Host+"/"+Server+'/acplt/cshmi/Group.instance', "OP_VALUE", null);
			if (!(cshmiString && cshmiString.indexOf("KS_ERR") !== -1)){
				responseArray = this.splitKsResponse(cshmiString);
				//the array could be [""]
				if (responseArray.length > 0 && responseArray[0] !== ""){
					sortedList = responseArray[0].split(" ").sort();
				}
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
		
		var responseArray = null;
		var lastEntry = null;
		var sortedList = null;
		
		//get Sheets from hmi library
		
		//the path of the HMI Manager could be different in every OV Server
		this.getHMIManagerPointer(Host, Server);
		if (this.HMIMANAGER_PATH === null){
			HMI.hmi_log_trace("HMIJavaScriptKSClient.prototype.getSheets - No HMI Manager found on this Server.");
			return SheetList;
		}
		
		var Command = null;
		if ($("idShowcomponents") && $("idShowcomponents").checked){
			Command = '{' + HMI.KSClient.getMessageID() + '}%20' +
				'{010}%20' +
				'{' + this.HMIMANAGER_PATH + '}%20' + 
				'{SHOWCOMPONENTS}';
		}else{
			Command = '{' + HMI.KSClient.getMessageID() + '}%20' +
				'{010}%20' +
				'{' + this.HMIMANAGER_PATH + '}%20' + 
				'{SHOWSHEETS}';
		}
		this.setVar_NG("//"+Host+"/"+Server+this.HMIMANAGER_PATH
				+ '.Command',
				Command,
				null);
		var Sheetstring = this.getVar("//"+Host+"/"+Server+this.HMIMANAGER_PATH + '.CommandReturn', "OP_VALUE", null);
		Command = null;
		
		var responseArray = this.splitKsResponse(Sheetstring);
		
		HMI.hmi_log_trace("HMIJavaScriptKSClient.prototype.getSheets - End");
		//the array could be [""]
		if (responseArray.length > 0 && responseArray[0] !== ""){
			return SheetList.concat(responseArray[0].split(" ").sort());
		}else{
			return SheetList;
		}
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
	 * @return if no cbfnc submitted returnvalue of request or false on an communication error
	 */
	_sendRequest: function(Client, method, async, urlparameter, cbfnc) {
		HMI.hmi_log_trace("HMIJavaScriptKSClient.prototype._sendRequest - Start, Async:"+async+" Meth:"+method+", requested: "+urlparameter);
		
		var req = new XMLHttpRequest();
		
		try {
			//FireFox 3 sends in a POST a content-encoding header killing the TCL Webserver
			//http://wiki.tcl.tk/2085 entry "nb Dec 18, 2008"
			//patch accepted but commented out in our
			// TCL-HTTPD\tcllib-1.6\modules\ncgi\ncgi.tcl
			
			//only send POST to non tcl servers!
			
			req.open(method, urlparameter, async);
			
			//prevent caching of request in all browsers (ie was the problem - as usual)
			req.setRequestHeader("If-Modified-Since", "Wed, 15 Nov 1995 04:58:08 GMT");
			
			if (req.timeout !== undefined && async === true){
				//timeout is not allowed with sync requests
				req.timeout = 1000;
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
				HMI.hmi_log_error('HMIJavaScriptKSClient._sendRequest: Request could not be sent. Is the gateway started?');
				HMI.hmi_log_onwebsite('Request could not be sent. Is the gateway started?');
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
				return req.responseText;
			}
		}
		HMI.hmi_log_trace("HMIJavaScriptKSClient.prototype._sendRequest - End");
		return true;
	},
	
	/*********************************
		prepareComponentText
	*********************************/
	prepareComponentText: function(ComponentText) {
		HMI.hmi_log_trace("HMIJavaScriptKSClient.prototype.prepareComponentText - Start");
		
		//[StyleDescription] adjust this code if no ACPLT/HMI Server has a StyleDescription anymore
		
		if (ComponentText === null || ComponentText === ""){
			HMI.hmi_log_error("HMIJavaScriptKSClient.prototype.prepareComponentText - parameter was null or empty");
			return null;
		}
		var ReturnText = this.splitKsResponse(ComponentText);
		
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
			ReturnText = null;
		}else if (ReturnText[0] === ""){
				HMI.hmi_log_onwebsite('Gateway reply was empty.');
				return null;
		} else {
			//return the GraphicDescription and StyleDescription
			
			//put StyleVariable to defined state
			if (ReturnText.length !== 2 ){
				ReturnText[1] = null;
			}
		};
		
		HMI.hmi_log_trace("HMIJavaScriptKSClient.prototype.prepareComponentText - End");
		
		return ReturnText;
	},
	
	/*********************************
		checkSheetProperty
	*********************************/
	checkSheetProperty: function(Host, Server, ComponentPath) {
		HMI.hmi_log_trace("HMIJavaScriptKSClient.prototype.checkSheetProperty - Start");
		
		if (this.HMIMANAGER_PATH === null){
			//we have no HMIMANAGER, so there could be no StyleDescription
			HMI.ServerProperty.SheetHasStyleDescription = false;
			HMI.hmi_log_trace("HMIJavaScriptKSClient.prototype.checkSheetProperty - Endn");
			return;
		}
		//spaces in objectname are encoded as %20 within OV
		var StyleResponse = this.getVar("//"+Host+"/"+Server+encodeURI(ComponentPath) + '.StyleDescription', "OP_VALUE", null);
		
		if (StyleResponse.indexOf("KS_ERR") !== -1){
			//error could be: TksS-0015::KS_ERR_BADPATH {{/TechUnits/SchneemannImSchnee.StyleDescription KS_ERR_BADPATH}}
			HMI.ServerProperty.SheetHasStyleDescription = false;
			HMI.hmi_log_trace("HMIJavaScriptKSClient.prototype.checkSheetProperty - Endf");
		}else{
			HMI.ServerProperty.SheetHasStyleDescription = true;
			HMI.hmi_log_trace("HMIJavaScriptKSClient.prototype.checkSheetProperty - Endt");
		}
		return;
	},	
	/*********************************
		splitKsResponse
			returns the KS Response as an Array, or an empty Array
			if the optional argument recursionDepth is > 0,  
	*********************************/
	splitKsResponse: function (response, recursionDepth) {
		//Checking if optional argument is set, else set to default: 0
		if (recursionDepth === undefined){
			recursionDepth = 0;
		}
		//check input
		if (response === null){
			return Array();
		}else if (response === ""){
			return Array();
		}else if (response === false){
			//communication error
			return Array();
		}else if (response === "{{}}"){
			return Array("");
		}else if (response.indexOf("KS_ERR") !== -1){
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
			for (i=0; i < currentArrayElement.length; i++) {
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
					currentArrayElement = this.splitKsResponse(currentArrayElement, recursionDepth-1);
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
					for (i=0; i < currentArrayElement.length; i++) {
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
						for (i=0; i < currentArrayElement.length; i++) {
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
	getChildObjArray: function (ObjectPath, cachingTarget) {
		var responseArray;
		if (!(cachingTarget.ResourceList.ChildList && cachingTarget.ResourceList.ChildList[ObjectPath] !== undefined)){
			
			var response = this.getEP(encodeURI(ObjectPath), "OT_DOMAIN", ["OP_NAME", "OP_CLASS"], null);
			
			//no caching with an communication error
			if (response === false){
				//communication error
				return Array();
			}else if (response === null){
				HMI.hmi_log_error("_interpreteClientEvent of "+ObjectPath+" failed: response was null");
				return Array();
			}else if (response.indexOf("KS_ERR") !== -1){
				HMI.hmi_log_error("_interpreteClientEvent of "+ObjectPath+" failed: "+response);
				return Array();
			}
			
			responseArray = this.splitKsResponse(response);
			
			//prepare main js-object to remember config of getValue OV-Object
			cachingTarget.ResourceList.ChildList[ObjectPath] = new Object();
			cachingTarget.ResourceList.ChildList[ObjectPath].ChildListParameters = responseArray;
			cachingTarget.ResourceList.ChildList[ObjectPath].useCount = 1;
		}else{
			responseArray = cachingTarget.ResourceList.ChildList[ObjectPath].ChildListParameters;
			cachingTarget.ResourceList.ChildList[ObjectPath].useCount++;
		}
		return responseArray;
	},
	
	/**
	 * returns a valid HostAndServer, Path Array
	 * @return Array with HostAndServer and Path as String or null (in Error) and an infostring ("emptystring", "fullpath", "relativepath" or "genericerror") in Array
	 */
	_splitKSPath: function(FullKSpath){
		if (typeof FullKSpath !== "string" || FullKSpath.length === 0){
			return Array(null, "", "emptystring");
		}
		if (FullKSpath.charAt(0) === "/" && FullKSpath.charAt(1) === "/"){
			//find the 3rd "/"
			var slashIndexAfterHost = FullKSpath.indexOf("/", 2);
			//find the 4th "/"
			var slashIndexAfterServer = FullKSpath.indexOf("/", slashIndexAfterHost+1);
			//only keep the String before 4th "/"
			//var Host = FullKSpath.slice(2, slashIndexAfterHost);
			//var Server = FullKSpath.slice(slashIndexAfterHost+1, slashIndexAfterServer);
			var HostAndServer = FullKSpath.slice(2, slashIndexAfterServer);
			var KSPath = FullKSpath.slice(slashIndexAfterServer);
			
			return Array(HostAndServer, KSPath, "fullpath");
		}else if (FullKSpath.charAt(0) === "/"){
			//no Host and Server found, so replace with Model location
			return Array(this.ResourceList.ModelHost+"/"+this.ResourceList.ModelServer, FullKSpath, "relativepath");
		}else{
			//ups kaputt
			return Array(null, "", "genericerror");
		}
	},
	
	/*********************************
		destroy
	*********************************/
	destroy: function () {
		//destroy all handles
		for (var i in this.ResourceList.Handles){
			this.delHandle(this.ResourceList.Handles[i].HandleString);
			delete this.ResourceList.Handles[i].HandleString;
			delete this.ResourceList.Handles[i];
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
