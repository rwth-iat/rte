/*
*	Copyright (C) 2011
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
	this.KSServer = null;
	this.TCLKSGateway = null;
	this.TCLKSHandle = null;
	this.HMIMANAGER_PATH = null;
	
	this.TksGetChildInfo = "%20-type%20$::TKS::OT_DOMAIN%20-output%20[expr%20$::TKS::OP_NAME%20|%20$::TKS::OP_CLASS]";
	
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
	
	/*********************************
		init
	*********************************/
	init: function(HostAndServer, TCLKSGateway) {
		HMI.hmi_log_trace("HMIJavaScriptKSClient.prototype.init - Start");
		
		window.clearInterval(HMI.RefreshTimeoutID);
		HMI.RefreshTimeoutID = null;
		
		if (this.TCLKSHandle !== null)
			this.destroy();
		
		this.KSServer		= HostAndServer;
		this.TCLKSGateway	= TCLKSGateway;
		this.TCLKSHandle	= null;
		this.HMIMANAGER_PATH	= null;
		
		this.getHandle(this.KSServer, this._cbInit);
		
		HMI.hmi_log_trace("HMIJavaScriptKSClient.prototype.init - End");
	},
	
	/*********************************
		getEP
		Handle:	requires own Handle, null if uses the normal global Handle
		path:	command to process of the command
		cbfnc: callback function
		usage example:
			this.getEP(null, '/servers%20*', this._cbGetServers);
			response: "{fb_hmi1} {fb_hmi2} {fb_hmi3} {MANAGER} {fb_hmi4} {fb_hmi5}"
	*********************************/
	getEP: function(Handle, path, tksparameter, cbfnc) {
		HMI.hmi_log_trace("HMIJavaScriptKSClient.prototype.getEP - Start: "+path+" Handle: "+Handle);
		if (Handle === null){
			Handle = this.TCLKSHandle;
		}
		var urlparameter;
		if (HMI.GatewayTypeTCL === true){
			path = path + tksparameter;
			urlparameter = 'obj='+Handle + '&args=getep%20' +path;
		}else if (HMI.GatewayTypePHP === true){
			//todo php should get similar interface
			path = path;
			urlparameter = 'obj='+Handle + '&cmd=getep&path=' +path;
		}
		if (cbfnc !== null){
			this._sendRequest(this, 'GET', false, urlparameter, cbfnc);
		}else{
			var ReturnText = this._sendRequest(this, 'GET', false, urlparameter, cbfnc);
			HMI.hmi_log_trace("HMIJavaScriptKSClient.prototype.getEP - End");
			return ReturnText;
		}
		
		HMI.hmi_log_trace("HMIJavaScriptKSClient.prototype.getEP - End");
	},
	
	/*********************************
		getHandle
		cbfnc: callback function
		usage example:
			getHandle("localhost/MANAGER", this._cbInit);
			response: "TksS-0000"
			getHandle("localhost/fb_hmi1", null);
			response: "TksS-0042"
	*********************************/
	getHandle: function(host, cbfnc) {
		var urlparameter;
		HMI.hmi_log_trace("HMIJavaScriptKSClient.prototype.getHandle - Start");
		if (HMI.GatewayTypeTCL === true){
			urlparameter = 'obj=tks-server&args='+host;
		}else if(HMI.GatewayTypePHP === true){
			urlparameter = 'cmd=tks-server&args='+host;
		}
		if (cbfnc !== null){
			this._sendRequest(this, 'GET', false, urlparameter, cbfnc);
		}else{
			var ReturnText = this._sendRequest(this, 'GET', false, urlparameter, cbfnc);
			HMI.hmi_log_trace("HMIJavaScriptKSClient.prototype.getHandle - End");
			return ReturnText;
		}
		HMI.hmi_log_trace("HMIJavaScriptKSClient.prototype.getHandle - End");
	},
	
	/*********************************
		getVar
		Handle:	requires own Handle, null if uses the normal global Handle
		path:	command to process of the command, multiple commands are {part1} {part1} coded (used in GraphicDescription+StyleDescription)
		cbfnc: callback function
		usage example:
			ManagerResponse = this.getVar(TCLKSHandle, "/Libraries/hmi/Manager.instance", null)
			response: "{{/TechUnits/HMIManager}}"
			this.getVar(null, '/TechUnits/HMIManager.CommandReturn', this._cbGetSheets);
			response: "{/TechUnits/Sheet1}"
			
			error response: "TksS-0042::KS_ERR_BADPATH {{/Libraries/hmi/Manager.instance KS_ERR_BADPATH}}"
	*********************************/
	getVar: function(Handle, path, cbfnc) {
		HMI.hmi_log_trace("HMIJavaScriptKSClient.prototype.getVar - Start: "+path+" Handle: "+Handle);
		if (Handle === null){
			Handle = this.TCLKSHandle;
		}
		var urlparameter;
		if (HMI.GatewayTypeTCL === true){
			path = path + "%20-output%20$::TKS::OP_VALUE";
			urlparameter = 'obj='+Handle + '&args=getvar%20' +path;
		}else if (HMI.GatewayTypePHP === true){
			path = path;
			urlparameter = 'obj='+Handle + '&cmd=getvar&path=' + path;
		}
		if (cbfnc !== null){
			this._sendRequest(this, 'GET', false, urlparameter, cbfnc);
		}else{
			var ReturnText = this._sendRequest(this, 'GET', false, urlparameter, cbfnc);
			HMI.hmi_log_trace("HMIJavaScriptKSClient.prototype.getVar - End");
			return ReturnText;
		}
		
		HMI.hmi_log_trace("HMIJavaScriptKSClient.prototype.getVar - End");
	},
	
	/*********************************
		setVar
		Handle:	requires own Handle, null if uses the normal global Handle
		path:	command to process of the command
		value:	value to set (StringVec are {part1} {part2} {part3} coded
		cbfnc: callback function
		usage example:
			this.setVar(null, '/TechUnits/HMIManager.Command ', "{1} {010} {/TechUnits/HMIManager} {SHOWSHEETS}", null);
			response: ""
	*********************************/
	setVar: function(Handle, path, value, cbfnc) {
		HMI.hmi_log_trace("HMIJavaScriptKSClient.prototype.setVar - Start: "+path+" Handle: "+Handle);
		
		if (Handle === null){
			Handle = this.TCLKSHandle;
		}
		var urlparameter;
		if (HMI.GatewayTypeTCL === true){
			path = '{'+path+'%20{'+value+'}}';
			urlparameter = 'obj='+Handle + '&args=setvar%20' +path;
		}else if (HMI.GatewayTypePHP === true){
			urlparameter = 'obj='+Handle + '&cmd=setvar&path=' + path + "&val=" + value;
		}
		if (cbfnc !== null){
			this._sendRequest(this, 'GET', false, urlparameter, cbfnc);
		}else{
			var ReturnText = this._sendRequest(this, 'GET', false, urlparameter, cbfnc);
			HMI.hmi_log_trace("HMIJavaScriptKSClient.prototype.setVar - End");
			return ReturnText;
		}
		
		HMI.hmi_log_trace("HMIJavaScriptKSClient.prototype.setVar - End");
	},
	
	/*********************************
		delHandle
		Handle:	Handle to be destroyed
		usage example:
			this.delHandle("TksS-0042");
			response: ""
	*********************************/
	delHandle: function(Handle) {
		HMI.hmi_log_trace("HMIJavaScriptKSClient.prototype.delHandle - Start Handle: "+Handle);
		var urlparameter;
		if (HMI.GatewayTypeTCL === true){
			urlparameter = 'obj='+Handle + '&args=destroy';
		}else if (HMI.GatewayTypePHP === true){
			urlparameter = 'obj='+Handle + '&cmd=destroy';
		}
		this._sendRequest(this, 'GET', false, urlparameter, null);
		HMI.hmi_log_trace("HMIJavaScriptKSClient.prototype.delHandle - End");
	},
	
	/*********************************
		_cbinit
		
		got a Handle of the KS-Gateway
	*********************************/
	_cbInit: function(Client, req) {
		HMI.hmi_log_trace("HMIJavaScriptKSClient.prototype._cbinit - Start");
		
		//The handle must have the right format aka start with "TksS-"
		if (/^TksS-\b/.exec(req.responseText)){
			Client.TCLKSHandle = req.responseText;
		} else {
			Client.TCLKSHandle = null;
			if (HMI.PossServers.length === 0){
				HMI.PossServers.options[HMI.PossServers.options.length] = new Option('- no valid server response -', 'no server');
			}
			HMI.hmi_log_error('HMIJavaScriptKSClient._cbinit: Could not initialize TCLKSGateway. '
				+ 'Gateway responded:'
				+ '\n'
				+ req.responseText);
			if (req.readyState != 4){
				HMI.hmi_log_onwebsite('This Browser does not support synchronous XMLHttpRequest.');
			}else if (req.responseText.length < 250){
				HMI.hmi_log_onwebsite('Could not initialize TCLKSGateway. Server responded: ' + req.responseText);
			}else if (req.responseText){
				HMI.hmi_log_onwebsite('Could not initialize TCLKSGateway. Server responded (first 250 characters): ' + req.responseText.substr(0,250));
			}else{
				HMI.hmi_log_onwebsite('Could not analyse server response.');
			}
		}
		
		HMI.hmi_log_trace("HMIJavaScriptKSClient.prototype._cbinit - End");
	},
	
	/*********************************
		getServers
	*********************************/
	getServers: function() {
		HMI.hmi_log_trace("HMIJavaScriptKSClient.prototype.getServers - Start");
		
		if (this.TCLKSHandle !== null){
			//The Handle points to the Manager wich can provide us with a list of OV servers (detection of HMI Servers are made in the callback)
			this.getEP(null, '/servers%20*', "%20-output%20$::TKS::OP_NAME", this._cbGetServers);
		} else {
			HMI.hmi_log_trace("HMIJavaScriptKSClient.prototype.getServers - End - No TCLKSHandle");
			return false;
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
		var Server = HMI.KSClient.splitKsResponse(req.responseText);
		
		var i = 0;
		
		Server = Server.sort();
		HMI.hmi_log_trace("HMIJavaScriptKSClient.prototype._cbGetServers - number of potential servers: "+Server.length);
		
		HMI.PossServers.setAttribute("title", "available OV-Servers: "+Server.join(", "));
		
		if (Server.length === 0){
			HMI.PossServers.options[0] = new Option('- no MANAGER available-', 'no server');
		}else if (Server.length === 1){
			HMI.PossServers.options[0] = new Option('- no server available -', 'no server');
		}else{
			//put first select option with a description
			HMI.PossServers.options[0] = new Option('loading...', 'no server');
			
			for (i = 0; i < Server.length; i++)
			{
				//test all potential servers if they are HMI Servers
				if (HMI.KSClient.pingServer(Server[i]) === true)
				{
					//put server into the dropdown box
					HMI.PossServers.options[HMI.PossServers.options.length] = new Option(Server[i], Server[i]);
				};
			};
			if (HMI.PossServers.length === 1){
				HMI.PossServers.options[0] = new Option('- no HMI server available -', 'no server');
			}else if (HMI.PossServers.length == 2){
				HMI.PossServers.options[0] = new Option('- select server -', 'no server');
				//selecting the option does not trigger the EventListener
				//it is allways the second/last <option>...
				HMI.PossServers.selectedIndex = 1;
				HMI.PossServers.disabled = false;
				HMI.showSheets(HMI.PossServers.options[1].value);
			}else{
				HMI.PossServers.options[0] = new Option('- select server -', 'no server');
				//replacing option[0] drops us into the first server
				HMI.PossServers.selectedIndex = 0;
				HMI.PossServers.disabled = false;
				HMI.PossSheets.options[0] = new Option('please select Server first', 'no sheet');
			}
		}
		HMI.hmi_log_trace("HMIJavaScriptKSClient.prototype._cbGetServers - End");
	},
	
	/*********************************
		pingServer
	*********************************/
	pingServer: function(Server) {
		HMI.hmi_log_trace("HMIJavaScriptKSClient.prototype.pingServer - Start: "+Server);
		
		//we need a new handle since we talk to another OV server
		var TCLKSHandle = this.getHandle(HMI.KSClient.KSServer.substring(0, HMI.KSClient.KSServer.indexOf('/')) + '/' + Server, null);
		
		if (TCLKSHandle.indexOf("KS_ERR_SERVERUNKNOWN") !== -1){
			//the Manager sometimes reject connection to a valid server, so retry once
			HMI.hmi_log_trace("HMIJavaScriptKSClient.prototype.pingServer - got KS_ERR_SERVERUNKNOWN but do not trust");
			TCLKSHandle = this.getHandle(HMI.KSClient.KSServer.substring(0, HMI.KSClient.KSServer.indexOf('/')) + '/' + Server, null);
			if (TCLKSHandle.indexOf("KS_ERR") !== -1){
				//the server is really not available. Could be the case if there is an active KS-Bridge and its destination is not available
				HMI.hmi_log_trace("HMIJavaScriptKSClient.prototype.pingServer - Server really not there Handlemessage: "+TCLKSHandle);
				return false;
			}
		}else if (TCLKSHandle.indexOf("KS_ERR") !== -1){
			//generic error
			HMI.hmi_log_trace("HMIJavaScriptKSClient.prototype.pingServer - End Handlemessage: "+TCLKSHandle);
			return false;
		}
		//Try to get the Name of HMI Manager to test the existence
		var ManagerResponse = this.getVar(TCLKSHandle, "/Libraries/hmi/Manager.instance", null); 
		var ManagerResponseArray = this.splitKsResponse(ManagerResponse);
		
		if (ManagerResponseArray.length === 0){
			//Try to get the Name of cshmi Group to test the existence
			ManagerResponse = this.getVar(TCLKSHandle, "/Libraries/cshmi/Group.instance", null);
			ManagerResponseArray = this.splitKsResponse(ManagerResponse);
			this.delHandle(TCLKSHandle);
			if (ManagerResponseArray.length === 0){
				//no hmi, no cshmi server
				HMI.hmi_log_trace("HMIJavaScriptKSClient.prototype.pingServer - no hmi, no cshmi server");
				return false;
			}
		}
		HMI.hmi_log_trace("HMIJavaScriptKSClient.prototype.pingServer - hmi or cshmi server");
		return true;
	},
	
	/*********************************
		getHMIManagerPointer
	*********************************/
	getHMIManagerPointer: function() {
		//the path of the HMI Manager could be different in every OV Server
		var ManagerResponse = this.getVar(null, "/Libraries/hmi/Manager.instance", null);
		
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
		if (ManagerArray.length !== 1){
			HMI.hmi_log_info("Warning: More than one HMIManagers available ("+ManagerArray.join(", ")+"). Using first Manager. It is perfectly safe to delete all but one instances.");
		}
		this.HMIMANAGER_PATH = encodeURI(ManagerArray[0]);
		ManagerResponse = null;
		ManagerResponseArray = null;
		PointOfSpace = null;
		return true;
	},
	
	/*********************************
		getSheets
	*********************************/
	getSheets: function(Server) {
		HMI.hmi_log_trace("HMIJavaScriptKSClient.prototype.getSheets - Start");
		
		var SheetList = new Array();
		var i = 0;
		
		//an init generates a new Handle, needed cause we communicate to this server the first time
		this.init(this.KSServer.substring(0, this.KSServer.indexOf('/')) + '/' + Server, this.TCLKSGateway);
		if (this.TCLKSHandle === null){
			return false;
		}
		
		//get Sheets from cshmi library
		
		var cshmiString = this.getVar(null, '/Libraries/cshmi/Group.instance', null);
		if (!(cshmiString.indexOf("KS_ERR") !== -1)){
			var responseArray = this.splitKsResponse(cshmiString);
			SheetList = SheetList.concat(responseArray[0].split(" ").sort());
		}
		
		//get Sheets from hmi library
		
		//the path of the HMI Manager could be different in every OV Server
		this.getHMIManagerPointer();
		if (HMI.KSClient.HMIMANAGER_PATH === null){
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
		this.setVar(null, this.HMIMANAGER_PATH
				+ '.Command%20',
				Command,
				null);
		var Sheetstring = this.getVar(null, this.HMIMANAGER_PATH + '.CommandReturn', null);
		Command = null;
		
		var responseArray = this.splitKsResponse(Sheetstring);
		
		HMI.hmi_log_trace("HMIJavaScriptKSClient.prototype.getSheets - End");
		if (responseArray.length === 0){
			return SheetList;
		}else{
			return SheetList.concat(responseArray[0].split(" ").sort());
		}
	},
	
	/*********************************
		_handleStateChange
	*********************************/
	_handleStateChange: function (req, cbfnc) {
		HMI.hmi_log_trace("HMIJavaScriptKSClient.prototype._handleStateChange - Start");
		
		switch (req.readyState) {
			case 0 : // UNINITIALIZED
			case 1 : // LOADING
			case 2 : // LOADED
			case 3 : // INTERACTIVE
				break;
			case 4 : // COMPLETED
				if (cbfnc !== null)
				{
					cbfnc(this, req);
				};
				break;
			default : ; // FAILURE
		}
		
		HMI.hmi_log_trace("HMIJavaScriptKSClient.prototype._handleStateChange - End");
	},
	
	/*********************************
		_sendRequest
	*********************************/
	_sendRequest: function(Client, method, async, urlparameter, cbfnc) {
		HMI.hmi_log_trace("HMIJavaScriptKSClient.prototype._sendRequest - Start, Async:"+async+" Meth:"+method+", requested: "+window.location.protocol+'//'+ HMI.KSClient.TCLKSGateway + '?' + urlparameter);
		
		var req = new XMLHttpRequest();
		
		//netscape.security.PrivilegeManager.enablePrivilege("UniversalBrowserRead");
		//does not work in other browsers than mozillabased
		try {
			//FireFox 3 sends in a POST a content-encoding header killing the TCL Webserver
			//http://wiki.tcl.tk/2085 entry "nb Dec 18, 2008"
			//patch accepted but commented out in 
			// TCL-HTTPD\tcllib-1.6\modules\ncgi\ncgi.tcl
			
			//real POST is not nessessary, therefor GET is forced
			req.open('GET',
				window.location.protocol+'//'
				+ HMI.KSClient.TCLKSGateway
				+ '?'
				+ urlparameter, async);
			
			//prevent caching of request in all browsers (ie was the problem - as usual
			req.setRequestHeader("If-Modified-Since", "Wed, 15 Nov 1995 04:58:08 GMT");
			
			if (async === true)
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
	checkSheetProperty: function(ComponentPath) {
		HMI.hmi_log_trace("HMIJavaScriptKSClient.prototype.checkSheetProperty - Start");
		
		//spaces in objectname are encoded as %20 within OV
		var StyleResponse = this.getVar(null, '{' + encodeURI(ComponentPath) + '.StyleDescription' + '}', null);
		
		if (StyleResponse.indexOf("KS_ERR_BADPATH") !== -1){
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
	*********************************/
	splitKsResponse: function (response) {
		//check input
		if (response === null){
			return Array();
		}else if (response === "{{}}"){
			return Array();
		}else if (response.indexOf("KS_ERR") !== -1){
			return Array();
		}else if (response.charAt(0) !== "{" && response.charAt(response.length -1) !== "}"){
			return Array();
		}
		//get rid of the outmost pair of {}
		response = response.substring(1, response.length-1);
		var responseArray = response.split("} {");
		response = null;
		
		for (var i = responseArray.length - 1; i >= 0 ;i--){
			if (responseArray[i].charAt(0) == "{"){
				// "{hello world}"
				responseArray[i] = responseArray[i].substring(1, responseArray[i].length-1);
			}else{
				// "helloWorld"
			}
		}
		return responseArray;
	},
	/*********************************
		getChildObjArray
			returns the childs of an Object as an Array, or an empty Array
	*********************************/
	getChildObjArray: function (ObjectPath) {
			var response = this.getEP(null, encodeURI(ObjectPath)+'%20*', this.TksGetChildInfo, null);
			if (response.indexOf("KS_ERR") !== -1){
				HMI.hmi_log_error("_interpreteClientEvent of "+ObjectPath+" failed: "+response);
				return null;
			}
			return this.splitKsResponse(response);
	},
	/*********************************
		destroy
	*********************************/
	destroy: function () {
		HMI.hmi_log_trace("HMIJavaScriptKSClient.prototype.destroy - Start");
		
		if (this.TCLKSHandle !== null)
		{
			this.delHandle(this.TCLKSHandle);
		};
		this.KSServer = null;
		this.TCLKSHandle = null;
		
		HMI.hmi_log_trace("HMIJavaScriptKSClient.prototype.destroy - End");
	}
};
var filedate = "$Date$";
filedate = filedate.substring(7, filedate.length-2);
if ("undefined" == typeof HMIdate){
	HMIdate = filedate;
}else if (HMIdate < filedate){
	HMIdate = filedate;
}
