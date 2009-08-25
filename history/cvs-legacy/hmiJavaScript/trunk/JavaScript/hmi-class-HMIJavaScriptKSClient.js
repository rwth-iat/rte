/*
*	Copyright (C) 2009
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
*	$Revision: 1.70 $
*	$Date: 2009-08-25 12:11:41 $
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
	this.KSServer;
	this.TCLKSGateway;
	this.TCLKSHandle = null;
	this.HMIMANAGER_PATH;
	
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
	getEP: function(Handle, path, cbfnc) {
		HMI.hmi_log_trace("HMIJavaScriptKSClient.prototype.getEP - Start: "+path+" Handle: "+Handle);
		if (Handle === null){
			Handle = this.TCLKSHandle;
		}
		var urlparameter;
		if (HMI.GatewayTypeTCL === true){
			path = path + "%20-output%20$::TKS::OP_NAME";
			urlparameter = 'obj='+Handle + '&args=getep%20' +path;
		}else if (HMI.GatewayTypePHP === true){
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
			if (req.responseText.length < 250){
				HMI.hmi_log_onwebsite('Could not initialize TCLKSGateway. Server responded: ' + req.responseText);
			}else{
				HMI.hmi_log_onwebsite('Could not initialize TCLKSGateway. Server responded (first 250 characters): ' + req.responseText.substr(0,250));
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
			this.getEP(null, '/servers%20*', this._cbGetServers);
		} else {
			HMI.hmi_log_trace("HMIJavaScriptKSClient.prototype.getServers - End - No TCLKSHandle");
			return null;
		}
		HMI.hmi_log_trace("HMIJavaScriptKSClient.prototype.getServers - End");
	},
	
	/*********************************
		_cbGetServers
	*********************************/
	_cbGetServers: function(Client, req) {
		HMI.hmi_log_trace("HMIJavaScriptKSClient.prototype._cbGetServers - Start");
		
		//responseText should be something like: {fb_hmi1} {fb_hmi2} {fb_hmi3} {MANAGER} {fb_hmi5} {fb_hmi4}
		var Response = req.responseText;
		var Server = new Array();
		
		var i = 0;
		
		//build an Array of potential servers
		while (	Response !== null
				&&	Response.indexOf('}') != -1)
		{
			//cut the servername out of the surrounding { }
			Server[i] = Response.substring(1, Response.indexOf('}'));
			//preserve the rest for another servertest
			Response = Response.substring(Response.indexOf('}') + 2, Response.length);
			i = i + 1;
			
			if (Response.indexOf('}') == -1)
				Response = null;
		};
		Server = Server.sort();
		HMI.hmi_log_trace("HMIJavaScriptKSClient.prototype._cbGetServers - number of potential servers: "+Server.length);
		
		if (Server.length > 0){
			//put first select option with a description
			HMI.PossServers.options[0] = new Option('- select server -', 'no server');
			
			var OptionNameLength = 0;
			for (i = 0; i < Server.length; i++)
			{
				//test all potential servers if they are HMI Servers
				if (HMIJavaScriptKSClient.prototype.pingServer(Server[i]) === true)
				{
					//put server into the dropdown box
					HMI.PossServers.options[HMI.PossServers.options.length] = new Option(Server[i], Server[i]);
					//keep longest name for expanding the width in Internet Explorer
					if (OptionNameLength < Server[i].length){
						OptionNameLength = Server[i].length;
					}
				};
			};
			//IE does not show a long elementname in dropdown => make the dropdown wider
			if (BrowserDetect && "Explorer" == BrowserDetect.browser){
				var OptimumWidth = OptionNameLength * 7.5 ;
				if (OptimumWidth > parseInt(HMI.PossServers.style.width, 10)){
					HMI.PossSheets.style.width = OptimumWidth + "px";
					HMI.PossServers.style.width = OptimumWidth + "px";
				}
			}
			HMI.hmi_log_trace("HMIJavaScriptKSClient.prototype._cbGetServers - number of valid servers: "+(HMI.PossServers.length-1));
			if (HMI.PossServers.length == 1){
				HMI.PossServers.options[0] = new Option('- no server available -', 'no server');
			}else if (HMI.PossServers.length == 2){
				//selecting the option does not trigger the EventListener
				//it is allways the second/last <option>...
				HMI.PossServers.selectedIndex = 1;
				HMI.showSheets(HMI.PossServers.lastChild.value);
			}else{
				HMI.PossSheets.options[0] = new Option('please select Server first', 'no sheet');
			}
			HMI.PossServers.disabled = false;
		} else {
			HMI.PossServers.options[0] = new Option('- no MANAGER available-', 'no server');
		};
		HMI.hmi_log_trace("HMIJavaScriptKSClient.prototype._cbGetServers - End");
	},
	
	/*********************************
		pingServer
	*********************************/
	pingServer: function(Server) {
		HMI.hmi_log_trace("HMIJavaScriptKSClient.prototype.pingServer - Start: "+Server);
		
		var TCLKSHandle = null;
		var ManagerResponse = null;
		
		try {
			//we need a new handle since we talk to another OV server
			TCLKSHandle = this.getHandle(HMI.KSClient.KSServer.substring(0, HMI.KSClient.KSServer.indexOf('/')) + '/' + Server, null);
			
			if (/KS_ERR/.exec(TCLKSHandle)){
				//the server is not available. Could be the case if there is an active KS-Bridge and its destination is not available
				HMI.hmi_log_trace("HMIJavaScriptKSClient.prototype.pingServer - Endf");
				return false;
			}
			//Try to get the Name of HMI Manager to test the existence
			ManagerResponse = this.getVar(TCLKSHandle, "/Libraries/hmi/Manager.instance", null);
			
			this.delHandle(TCLKSHandle);
			
			HMI.hmi_log_trace("HMIJavaScriptKSClient.prototype.pingServer - got "+ManagerResponse);
			
			if (ManagerResponse.length === 0){
				// Opera bis exklusive version 9.5 liefert einen leeren responseText bei HTTP-Status 503
				HMI.hmi_log_trace("HMIJavaScriptKSClient.prototype.pingServer - End1f+Operabug");
				return false;
			}else if (ManagerResponse == "{{}}"){
				HMI.hmi_log_trace("HMIJavaScriptKSClient.prototype.pingServer - End2f");
				return false;
			}else if (/KS_ERR/.exec(ManagerResponse)){
				//error could be: TksS-0174::KS_ERR_BADPATH {{/Libraries/hmi/Manager.instance KS_ERR_BADPATH}}
				HMI.hmi_log_trace("HMIJavaScriptKSClient.prototype.pingServer - End3f");
				return false;
			} else {
				HMI.hmi_log_trace("HMIJavaScriptKSClient.prototype.pingServer - End1t");
				return true;
			}
		} catch (e) {
			HMI.hmi_log_trace("HMIJavaScriptKSClient.prototype.pingServer - End2");
			return false;
	   };
	   
	   HMI.hmi_log_trace("HMIJavaScriptKSClient.prototype.pingServer - End3");
	},
	
	/*********************************
		getSheets
	*********************************/
	getSheets: function() {
		HMI.hmi_log_trace("HMIJavaScriptKSClient.prototype.getSheets - Start");
		
		//the path of the HMI Manager could be different in every OV Server
		this.HMIMANAGER_PATH = this.getVar(null, "/Libraries/hmi/Manager.instance", null);
		var PointOfSpace = this.HMIMANAGER_PATH.indexOf(' ');
		if (PointOfSpace == -1){
			this.HMIMANAGER_PATH = this.HMIMANAGER_PATH.replace(/{/g, "").replace(/}/g, "");
		}else{
			HMI.hmi_log_info_onwebsite("Warning: More than one HMIManagers available ("+this.HMIMANAGER_PATH.replace(/{/g, "").replace(/}/g, "")+"). Using first Manager.");
			this.HMIMANAGER_PATH = this.HMIMANAGER_PATH.substring(0,PointOfSpace).replace(/{/g, "").replace(/}/g, "");
		}
		delete PointOfSpace;
		
		var Command = null;
		if ($("idShowcomponents") && $("idShowcomponents").checked){
			Command = '{' + HMI.KSClient.getMessageID() + '}%20' +
				'{010}%20' +
				'{' + this.HMIMANAGER_PATH + '}%20' + 
				'{SHOWCOMPONENTS}';
			this.setVar(null, this.HMIMANAGER_PATH
					+ '.Command%20',
					Command,
					null);
		}else{
			Command = '{' + HMI.KSClient.getMessageID() + '}%20' +
				'{010}%20' +
				'{' + this.HMIMANAGER_PATH + '}%20' + 
				'{SHOWSHEETS}';
			this.setVar(null, this.HMIMANAGER_PATH
					+ '.Command%20',
					Command,
					null);
		}
		this.getVar(null, this.HMIMANAGER_PATH + '.CommandReturn', this._cbGetSheets);
		delete Command;
		
		HMI.hmi_log_trace("HMIJavaScriptKSClient.prototype.getSheets - End");
	},
	
	/*********************************
		_cbGetSheets
	*********************************/
	_cbGetSheets: function(Client, req) {
		HMI.hmi_log_trace("HMIJavaScriptKSClient.prototype._cbGetSheets - Start");
		
		var Response = req.responseText;
		var Sheet = new Array();
		
		var i = 0;
		
		//cut the sheetlist out of brackets
		if (Response.indexOf('{{') == -1)
		{
			//could be {/TechUnits/Sheet1}
			Response = Response.substring(Response.indexOf('{') + 1, Response.indexOf('}'));
		} else {
			//could be {{/TechUnits/Sheet1 /TechUnits/Sheet2 /TechUnits/Sheet3}}
			Response = Response.substring(Response.indexOf('{{') + 2, Response.indexOf('}}'));
		};
		
		while (Response !== null && Response !== "")
		{
			if (Response.indexOf(' ') == -1)
			{
				//only one Sheet
				Sheet[i] = Response;
				Response = null;
			} else {
				//multiple Sheets
				Sheet[i] = Response.substring(0, Response.indexOf(' '));
				Response = Response.substring(Response.indexOf(' ') + 1, Response.length);
			};
			i = i + 1;
		};
		Sheet = Sheet.sort();
		HMI.hmi_log_trace("HMIJavaScriptKSClient.prototype._cbGetSheets - number of sheets: "+Sheet.length);
		if (Sheet.length > 0){
			HMI.PossSheets.options[HMI.PossSheets.options.length] = new Option('- select sheet -', 'no sheet');
			var OptionNameLength = 0;
			for (i = 0; i < Sheet.length; i++){
				HMI.PossSheets.options[HMI.PossSheets.options.length] = new Option(Sheet[i], Sheet[i]);
				if (OptionNameLength < Sheet[i].length){
					OptionNameLength = Sheet[i].length;
				}
			}
			//IE does not show a long elementname in dropdown => make the dropdown wider
			if (BrowserDetect && "Explorer" == BrowserDetect.browser){
				var OptimumWidth = OptionNameLength * 7.5 ;
				if (OptimumWidth > parseInt(HMI.PossSheets.style.width, 10)){
					HMI.PossSheets.style.width = OptimumWidth + "px";
					HMI.PossServers.style.width = OptimumWidth + "px";
				}
			}
			if (Sheet.length == 1){
				//selecting the option does not trigger the EventListener
				HMI.PossSheets.selectedIndex = 1;
				HMI.showSheet(Sheet[0]);
			}
			HMI.PossSheets.disabled = false;
		} else {
			HMI.PossSheets.options[0] = new Option('- no sheets available -', 'no sheet');
			HMI.hmi_log_trace("HMIJavaScriptKSClient.prototype._cbGetSheets - number of sheets: "+Sheet.length);
		};
		
		HMI.hmi_log_trace("HMIJavaScriptKSClient.prototype._cbGetSheets - End");
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
		var DatePreventsCaching = new Date();
		
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
				+ urlparameter
				+ '&preventCaching='
				+DatePreventsCaching.getTime(), async);
				
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
			if (cbfnc !== null){
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
		
		//[StyleDescription] adjust this line if no ACPLT/HMI Server has a StyleDescription anymore
		var ReturnText = new Array(2);
		
		//	ComponentText should look like:
		//		{{GraphicDescription}} {{StyleDescription}}
		//	TksS-XXXX indicates error
		//
		if (/\bTksS-\b/.exec(ComponentText) && /KS_ERR/.exec(ComponentText)){
			HMI.hmi_log_error('HMIJavaScriptKSClient.prototype.prepareComponentText: ' + ComponentText);
			if (ComponentText.length < 250){
				HMI.hmi_log_onwebsite('Server lost. Gateway reply: ' + ComponentText);
			}else{
				HMI.hmi_log_onwebsite('Server lost. First 250 characters of reply: ' + ComponentText.substr(0,250));
			}
			window.clearInterval(HMI.RefreshTimeoutID);
			HMI.RefreshTimeoutID = null;
			ReturnText = null;
		}else if ("{{" != ComponentText.substr(0,2)){
			HMI.hmi_log_error('HMIJavaScriptKSClient.prototype.prepareComponentText: ' + ComponentText);
			if (ComponentText.length < 250){
				HMI.hmi_log_onwebsite('Server did not responded valid. Gateway reply: ' + ComponentText);
			}else{
				HMI.hmi_log_onwebsite('Server did not responded valid. First 250 characters: ' + ComponentText.substr(0,250));
			}
			window.clearInterval(HMI.RefreshTimeoutID);
			HMI.RefreshTimeoutID = null;
			ReturnText = null;
		} else {
			//cut the GraphicDescription and StyleDescription out of the response
			ReturnText[0] = ComponentText.substring(ComponentText.indexOf('{{') + 2, ComponentText.indexOf('}}'));
			//StyleDescription requested?
			if (ComponentText.indexOf('}} {{') != -1 ){
				ReturnText[1] = ComponentText.substring(ComponentText.indexOf('}} {{') + 5, ComponentText.length - 2);
			}else{
				//put StyleVariable to defined state
				ReturnText[1] = "";
			}
			if (ReturnText[0] === ""){
				ReturnText = null;
				HMI.hmi_log_onwebsite('Gateway reply was empty.');
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
		
		var StyleResponse = this.getVar(null, '{' + ComponentPath + '.StyleDescription' + '}', null);
		
		if (/KS_ERR_BADPATH/.exec(StyleResponse)){
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
var filedate = "$Date: 2009-08-25 12:11:41 $";
filedate = filedate.substring(7, filedate.length-2);
if ("undefined" == typeof HMIdate){
	HMIdate = filedate;
}else if (HMIdate < filedate){
	HMIdate = filedate;
}
