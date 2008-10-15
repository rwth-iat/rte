/*
*	Copyright (C) 2007
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
*	$Revision: 1.34 $
*	$Date: 2008-10-15 15:44:46 $
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
***********************************************************************/

/***********************************************************************
	constructor
***********************************************************************/

function HMIJavaScriptKSClient(async) {
	/** Public **********************/
	this.KSServer;
	this.TCLKSGateway;
	this.TCLKSHandle;
	this.HMIMANAGER_PATH;
	
	this.Asynchron = async;
	
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
		
		clearTimeout(HMI.RefreshTimeoutID);
		HMI.RefreshTimeoutID = null;
		
		if (this.TCLKSHandle != null)
			this.destroy();
		
		this.KSServer		= HostAndServer;
		this.TCLKSGateway	= TCLKSGateway;
		this.TCLKSHandle	= null;
		this.HMIMANAGER_PATH	= null;
		
		document.title = "Startcenter - ACPLT/HMI";
		$("idBookmark").style.cssText = "display:none;";
		
//		this._sendRequest(this, 'GET', false, 'tks-server', this.KSServer, this._cbInit);
		//this.send2Request(null, 'newHandle', '', this.KSServer, '', '', this._cbInit);
		this.getHandle(this.KSServer, this._cbInit);
		
		HMI.hmi_log_trace("HMIJavaScriptKSClient.prototype.init - End");
	},
	
	/*********************************
		sendRequest replaced
	*********************************/
	sendRequest: function(service, method, args, cbfnc) {
		HMI.hmi_log_trace("HMIJavaScriptKSClient.prototype.sendRequest - Start: "+service+" Meth: "+method+" "+args);
		
		if (/\bTksS-\b/.exec(this.TCLKSHandle)){
			if (HMI.GatewayTypeTCL == true){
				this._sendRequest(this, method, this.Asynchron, this.TCLKSHandle, service + ' ' + args, cbfnc);
			}else if (HMI.GatewayTypePHP == true){
				this._sendRequest(this, method, this.Asynchron, this.TCLKSHandle, '&cmd=' + service + args, cbfnc);
			}
		} else {
			HMI.hmi_log_error('HMIJavaScriptKSClient.prototype.sendRequest - TCLKSGateway not initialized.');
			HMI.hmi_log_onwebsite('TCLKSGateway not initialized.');
		};
		
		HMI.hmi_log_trace("HMIJavaScriptKSClient.prototype.sendRequest - End");
	},
	
	/*********************************
		send2Request replaced
		Handle:	requires own Handle, null if uses the normal global Handle
		method:	GET, POST or 'newHandle' if a new handle is required (if no callbackfunction is given, 
			the	new handle is given as a return value, otherwise the new handle is set globally
		command:	command to process (often zero, than the args includes the command - requirement of the TCLgateway design)
		args:	command to process or parameter of the command
		path:	especially for the getep command
		suffix:	things like "* -output $::TKS::OP_NAME" not suitable for the PHP gateway
		cbfnc: callback function
	*********************************/
	send2Request: function(Handle, method, command, args, path, suffix, cbfnc) {
		HMI.hmi_log_trace("HMIJavaScriptKSClient.prototype.send2Request - Start: "+command+" Meth: "+method+" "+args);
		
		if (HMI.GatewayTypeTCL == true){
			if (Handle == null){
				Handle = this.TCLKSHandle
			}
			if (command != ''){
				command = '&args=' + command + ' ' + args;
				args = '';
			}else{
				if (args != ''){
					args = '&args=' + args;
				}
			}
			if (path != ''){
				path = ' ' + path;
			}
		}else if (HMI.GatewayTypePHP == true){
			if (command != ''){
				command = '&cmd=' + command;
			}
			if (args != ''){
				args = '&args=' + args;
			}
			if (path != ''){
				path = '&path=' + path;
			}
			suffix = '';
		}
		if (method == 'newHandle'){
			if (cbfnc != null){
				this._send2Request(this, 'GET', false, 'obj=tks-server'+args, cbfnc);
			}else{
				var Return = this._send2Request(this, 'GET', false, 'obj=tks-server'+args, cbfnc);
				return Return;
			}
		}else if (/\bTksS-\b/.exec(Handle)){
			var urlparameter = 'obj='+Handle + command + args + path+suffix;
			if (cbfnc != null){
				this._send2Request(this, method, this.Asynchron, urlparameter, cbfnc);
			}else{
				var Return = this._send2Request(this, method, false, urlparameter, null);
				return Return;
			}
		} else {
			HMI.hmi_log_error('HMIJavaScriptKSClient.prototype.send2Request - TCLKSGateway not initialized.');
			HMI.hmi_log_onwebsite('TCLKSGateway not initialized.');
		};
		
		HMI.hmi_log_trace("HMIJavaScriptKSClient.prototype.send2Request - End");
	},
	
	/*********************************
		getEP
		Handle:	requires own Handle, null if uses the normal global Handle
		path:	command to process of the command
		cbfnc: callback function
		usage example:
			this.getEP(null, '/servers *', this._cbGetServers);
			response: "{fb_hmi1} {fb_hmi2} {fb_hmi3} {MANAGER} {fb_hmi4} {fb_hmi5}"
	*********************************/
	getEP: function(Handle, path, cbfnc) {
		HMI.hmi_log_trace("HMIJavaScriptKSClient.prototype.getEP - Start: "+path+" Handle: "+Handle);
		if (Handle == null){
			Handle = this.TCLKSHandle
		}
		if (HMI.GatewayTypeTCL == true){
			path = path + " -output $::TKS::OP_NAME";
			var urlparameter = 'obj='+Handle + '&args=getep ' +path;
		}else if (HMI.GatewayTypePHP == true){
			path = path;
			var urlparameter = 'obj='+Handle + '&cmd=getep&path=' +path;
		}
		if (cbfnc != null){
			this._send2Request(this, 'GET', false, urlparameter, cbfnc);
		}else{
			var Return = this._send2Request(this, 'GET', false, urlparameter, cbfnc);
			HMI.hmi_log_trace("HMIJavaScriptKSClient.prototype.getEP - End");
			return Return;
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
		HMI.hmi_log_trace("HMIJavaScriptKSClient.prototype.getHandle - Start");
		if (HMI.GatewayTypeTCL == true){
			var urlparameter = 'obj=tks-server&args='+host;
		}else if (HMI.GatewayTypePHP == true){
			var urlparameter = 'obj=tks-server&args='+host;
		}
		if (cbfnc != null){
			this._send2Request(this, 'GET', false, urlparameter, cbfnc);
		}else{
			var Return = this._send2Request(this, 'GET', false, urlparameter, cbfnc);
			HMI.hmi_log_trace("HMIJavaScriptKSClient.prototype.getHandle - End");
			return Return;
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
		if (Handle == null){
			Handle = this.TCLKSHandle
		}
		if (HMI.GatewayTypeTCL == true){
			path = path + " -output $::TKS::OP_VALUE";
			var urlparameter = 'obj='+Handle + '&args=getvar ' +path;
		}else if (HMI.GatewayTypePHP == true){
			path = path;
			var urlparameter = 'obj='+Handle + '&args=getvar ' +path;
		}
		if (cbfnc != null){
			this._send2Request(this, 'GET', false, urlparameter, cbfnc);
		}else{
			var Return = this._send2Request(this, 'GET', false, urlparameter, cbfnc);
			HMI.hmi_log_trace("HMIJavaScriptKSClient.prototype.getVar - End");
			return Return;
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
		if (Handle == null){
			Handle = this.TCLKSHandle
		}
		if (HMI.GatewayTypeTCL == true){
			path = '{'+path+' {'+value+'}}';
			var urlparameter = 'obj='+Handle + '&args=setvar ' +path;
		}else if (HMI.GatewayTypePHP == true){
			path = path;
			var urlparameter = 'obj='+Handle + '&args=setvar ' +path;
		}
		if (cbfnc != null){
			this._send2Request(this, 'GET', false, urlparameter, cbfnc);
		}else{
			var Return = this._send2Request(this, 'GET', false, urlparameter, cbfnc);
			HMI.hmi_log_trace("HMIJavaScriptKSClient.prototype.setVar - End");
			return Return;
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
		if (HMI.GatewayTypeTCL == true){
			var urlparameter = 'obj='+Handle + '&args=destroy';
		}else if (HMI.GatewayTypePHP == true){
			var urlparameter = 'obj='+Handle + '&args=destroy';
		}
		this._send2Request(this, 'GET', false, urlparameter);
		HMI.hmi_log_trace("HMIJavaScriptKSClient.prototype.delHandle - End");
	},
	
	/*********************************
		simple2Request never used => delme soon
	*********************************/
	simple2Request: function(obj, args, suffix) {
		HMI.hmi_log_trace("HMIJavaScriptKSClient.prototype.simple2Request - Start, requested: "+'http://'+ HMI.KSClient.TCLKSGateway + '?obj=' + obj + '&args='+ args );
		
		if (HMI.GatewayTypeTCL == true){
			if (obj != ''){
				obj = 'obj=' + obj;
			}
			if (args != ''){
				args = '&args=' + args;
			}
		}else if (HMI.GatewayTypePHP == true){
			if (obj != ''){
				obj = 'obj=' + obj;
			}
			if (args != ''){
				args = '&args=' + args;
			}
			suffix = '';
		}
		
		var urlparameter = obj + args + suffix;
		this._send2Request(this, 'GET', false, urlparameter, null);
		
		HMI.hmi_log_trace("HMIJavaScriptKSClient.prototype.simple2Request - End");
		
		return req.responseText;
	},
	
	/*********************************
		_cbinit
	*********************************/
	_cbInit: function(Client, req) {
		HMI.hmi_log_trace("HMIJavaScriptKSClient.prototype._cbinit - Start");
		
		if (/\bTksS-\b/.exec(req.responseText)){
			Client.TCLKSHandle = req.responseText;
		} else {
			Client.KSServer = null;
			Node = document.createElement('option');
			Node.innerHTML = '- no valid server response -';
			Node.value = 'no server';
			HMI.PossServers.appendChild(Node);
			HMI.hmi_log_error('HMIJavaScriptKSClient._cbinit: Could not initialize TCLKSGateway.'
				+ '\n'
				+ 'Gateway responded:'
				+ '\n\n'
				+ req.responseText);
			HMI.hmi_log_onwebsite('Could not initialize TCLKSGateway. Server responded: ' + req.responseText);
		}
		
		HMI.hmi_log_trace("HMIJavaScriptKSClient.prototype._cbinit - End");
	},
	
	/*********************************
		getServers
	*********************************/
	getServers: function() {
		HMI.hmi_log_trace("HMIJavaScriptKSClient.prototype.getServers - Start");
		
		if (this.TCLKSHandle != null){
			//this.send2Request(null, 'GET', 'getep', '', '/servers', ' * -output $::TKS::OP_NAME', this._cbGetServers);
			this.getEP(null, '/servers *', this._cbGetServers);
/*
			if (HMI.GatewayTypeTCL == true){
				this._sendRequest(this, 'GET', false, this.TCLKSHandle, 'getep /servers * -output $::TKS::OP_NAME', this._cbGetServers);
			}else if (HMI.GatewayTypePHP == true){
				this._sendRequest(this, 'GET', false, this.TCLKSHandle, '&cmd=getep&path=/servers', this._cbGetServers);
			}
*/
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
		
		var Response = req.responseText;
		var Server = new Array();
		
		var Node = null;
		var i = 0;
		
		while (	Response != null
				&&	Response.indexOf('}') != -1)
		{
			Server[i] = Response.substring(1, Response.indexOf('}'));
			Response = Response.substring(Response.indexOf('}') + 2, Response.length);
			i = i + 1;
			
			if (Response.indexOf('}') == -1)
				Response = null;
		};
		HMI.hmi_log_trace("HMIJavaScriptKSClient.prototype._cbGetServers - number of potential servers: "+Server.length);
		var ValidServers = 0;
		var IdLastValidServer;
		if (Server.length > 0){
			Node = document.createElement('option');
			Node.innerHTML = '- select server -';
			Node.value = 'no server';
			HMI.PossServers.appendChild(Node);
			var OptionNameLength = 0;
			
			for (i = 0; i < Server.length; i++)
			{
				if (HMIJavaScriptKSClient.prototype.pingServer(Server[i]) == true)
				{
					Node = document.createElement('option');
					Node.innerHTML = Server[i];
					Node.value = Server[i];
					HMI.PossServers.appendChild(Node);
					ValidServers ++;
					IdLastValidServer = i;
					if (OptionNameLength < Server[i].length){
						OptionNameLength = Server[i].length;
					}
				};
			};
			if ("Explorer" == BrowserDetect.browser){
				var OptimumWidth = OptionNameLength * 7.5 ;
				if (OptimumWidth > parseInt(HMI.PossServers.style.width)){
					HMI.PossSheets.style.width = OptimumWidth + "px";
					HMI.PossServers.style.width = OptimumWidth + "px";
				}
			}
			HMI.hmi_log_trace("HMIJavaScriptKSClient.prototype._cbGetServers - number of valid servers: "+ValidServers);
			if (ValidServers == 0){
				Node.innerHTML = '- no server available -';
				Node.value = 'no server';
			}else{
				//'this' is not PossServers here, but refers to the window and is completely useless in Internet Explorer
				addEventSimple(HMI.PossServers, "change", function () {HMI.showSheets(HMI.PossServers.options[HMI.PossServers.selectedIndex].value)});
				if (ValidServers == 1){
					//selecting the option does not trigger the EventListener
					//it is allways the second <option>...
					$("idServers").selectedIndex = 1;
					//...but the right ServerID can vary.
					HMI.showSheets(Server[IdLastValidServer]);
				}
			}
		} else {
			Node = document.createElement('option');
			Node.innerHTML = '- no MANAGER available-';
			Node.value = 'no server';
			HMI.PossServers.appendChild(Node);
		};
		
		HMI.hmi_log_trace("HMIJavaScriptKSClient.prototype._cbGetServers - End");
	},
	
	/*********************************
		_simpleRequest replaced by send2Request
	*********************************/
	_simpleRequest: function(obj, args) {
		HMI.hmi_log_trace("HMIJavaScriptKSClient.prototype._simpleRequest - Start, requested: "+'http://'+ HMI.KSClient.TCLKSGateway + '/tks?obj=' + obj + '&args='+ args );
		
		var req = new XMLHttpRequest();
		var DatePreventsCaching = new Date();
		
		req.open('GET',
			'http://'
			+ HMI.KSClient.TCLKSGateway
			+ '?obj='
			+ obj
			+ '&args='
			+ args
			+ '&preventCaching='
			+DatePreventsCaching.getTime(), false);
		req.setRequestHeader('Content-Type', 'application/x-www-form-urlencoded');
		req.send('empty');
		
		HMI.hmi_log_trace("HMIJavaScriptKSClient.prototype._simpleRequest - End");
		
		return req.responseText;
	},
	
	/*********************************
		pingServer
	*********************************/
	pingServer: function(Server) {
		HMI.hmi_log_trace("HMIJavaScriptKSClient.prototype.pingServer - Start: "+Server);
		
		var TCLKSHandle = null;
		var ManagerResponse = null;
		
		try {
//			TCLKSHandle = this._simpleRequest('tks-server', HMI.KSClient.KSServer.substring(0, HMI.KSClient.KSServer.indexOf('/')) + '/' + Server);
			//TCLKSHandle = this.send2Request('', 'newHandle', '', HMI.KSClient.KSServer.substring(0, HMI.KSClient.KSServer.indexOf('/')) + '/' + Server, '', '', null);
			TCLKSHandle = this.getHandle(HMI.KSClient.KSServer.substring(0, HMI.KSClient.KSServer.indexOf('/')) + '/' + Server, null);
			
			//ManagerResponse = this.send2Request(TCLKSHandle, 'GET', 'getep', '', this.HMIMANAGER_PATH, ' * -output $::TKS::OP_NAME', null);
			//ManagerResponse = this.send2Request(TCLKSHandle, 'GET', 'getvar', '', "/Libraries/hmi/Manager.instance", ' -output $::TKS::OP_VALUE', null);
			ManagerResponse = this.getVar(TCLKSHandle, "/Libraries/hmi/Manager.instance", null)
			
/*			if (HMI.GatewayTypeTCL == true){
				ManagerResponse = this._simpleRequest(TCLKSHandle,
					'getep ' + this.HMIMANAGER_PATH + ' * -output $::TKS::OP_NAME');
			}else if (HMI.GatewayTypePHP == true){
				ManagerResponse = this._simpleRequest(TCLKSHandle,
					'&cmd=getep&path=' + this.HMIMANAGER_PATH);
			}
*/
			//this.send2Request(TCLKSHandle, 'GET', 'destroy', '', '', '', null);
			this.delHandle(TCLKSHandle);
//			this._simpleRequest(TCLKSHandle, 'destroy');
			
			HMI.hmi_log_trace("HMIJavaScriptKSClient.prototype.pingServer - got "+ManagerResponse);
			
			// Opera bis exklusive version 9.5 liefert einen leeren responseText bei HTTP-Status 503
			if (ManagerResponse.length == 0){
				HMI.hmi_log_trace("HMIJavaScriptKSClient.prototype.pingServer - End1f+Operabug");
				return false;
			}else if (/\bTksS-\b/.exec(ManagerResponse)){
				HMI.hmi_log_trace("HMIJavaScriptKSClient.prototype.pingServer - End1f");
				return false;
			} else {
				HMI.hmi_log_trace("HMIJavaScriptKSClient.prototype.pingServer - End1t");
				return true;
			};
						
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
		this.HMIMANAGER_PATH = this.getVar(null, "/Libraries/hmi/Manager.instance", null).replace(/{/g, "").replace(/}/g, "");
		
		var Command = null;
		if ($("checkbox_showcomponents") && $("checkbox_showcomponents").checked){
			//Command = '{' + 
			//	'{' + HMI.KSClient.getMessageID() + '} ' +
			//	'{010} ' +
			//	'{' + this.HMIMANAGER_PATH + '} ' + 
			//	'{SHOWCOMPONENTS}' +
			//	'}';
			//this.send2Request(null, 'POST', 'setvar', '{'
			//		+ this.HMIMANAGER_PATH
			//		+ '.Command '
			//		+ Command
			//		+ '}', '', '', null);
			Command = '{' + HMI.KSClient.getMessageID() + '} ' +
				'{010} ' +
				'{' + this.HMIMANAGER_PATH + '} ' + 
				'{SHOWCOMPONENTS}';
			this.setVar(null, this.HMIMANAGER_PATH
					+ '.Command ',
					Command,
					null);
		}else{
			//Command = '{' + 
			//	'{' + HMI.KSClient.getMessageID() + '} ' +
			//	'{010} ' +
			//	'{' + this.HMIMANAGER_PATH + '} ' + 
			//	'{SHOWSHEETS}' +
			//	'}';
			//this.send2Request(null, 'POST', 'setvar', '{'
			//		+ this.HMIMANAGER_PATH
			//		+ '.Command '
			//		+ Command
			//		+ '}', '', '', null);
			Command = '{' + HMI.KSClient.getMessageID() + '} ' +
				'{010} ' +
				'{' + this.HMIMANAGER_PATH + '} ' + 
				'{SHOWSHEETS}';
			this.setVar(null, this.HMIMANAGER_PATH
					+ '.Command ',
					Command,
					null);
		}
		//this.send2Request(null, 'GET', 'getvar', '{'
		//		+ this.HMIMANAGER_PATH
		//		+ '.CommandReturn} ', '', ' -output $::TKS::OP_VALUE', this._cbGetSheets);
		this.getVar(null, this.HMIMANAGER_PATH + '.CommandReturn', this._cbGetSheets);
/*
		this._sendRequest(this, 'POST', false, this.TCLKSHandle, 'setvar '
			+ '{'
			+ this.HMIMANAGER_PATH
			+ '.Command '
			+ Command
			+ '}' , null);
			
		this._sendRequest(this, 'GET', false, this.TCLKSHandle, 'getvar '
			+ '{'
			+ this.HMIMANAGER_PATH
			+ '.CommandReturn} '
			+ '-output $::TKS::OP_VALUE', this._cbGetSheets);
*/
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
		
		var Node = null;
		var i = 0;
		
		if (Response.indexOf('{{') == -1)
		{
			Response = Response.substring(Response.indexOf('{') + 1, Response.indexOf('}'));
		} else {
			Response = Response.substring(Response.indexOf('{{') + 2, Response.indexOf('}}'));
		};
		
		while (Response != null && Response != "")
		{
			if (Response.indexOf(' ') == -1)
			{
				Sheet[i] = Response;
				Response = null;
			} else {
				Sheet[i] = Response.substring(0, Response.indexOf(' '));
				Response = Response.substring(Response.indexOf(' ') + 1, Response.length);
			};
			i = i + 1;
		};
		HMI.hmi_log_trace("HMIJavaScriptKSClient.prototype._cbGetSheets - number of sheets: "+Sheet.length);
		if (Sheet.length > 0){
			Node = document.createElement('option');
			Node.innerHTML = '- select sheet -';
			Node.value = 'no sheet';
			HMI.PossSheets.appendChild(Node);
			var OptionNameLength = 0;
			for (i = 0; i < Sheet.length; i++){
				Node = document.createElement('option');
				Node.innerHTML = Sheet[i];
				Node.value = Sheet[i];
				HMI.PossSheets.appendChild(Node);
				if (OptionNameLength < Sheet[i].length){
					OptionNameLength = Sheet[i].length;
				}
			}
			if ("Explorer" == BrowserDetect.browser){
				var OptimumWidth = OptionNameLength * 7.5 ;
				if (OptimumWidth > parseInt(HMI.PossSheets.style.width)){
					HMI.PossSheets.style.width = OptimumWidth + "px";
					HMI.PossServers.style.width = OptimumWidth + "px";
				}
			}
			addEventSimple(HMI.PossSheets, "change", function () {HMI.showSheet(HMI.PossSheets.options[HMI.PossSheets.selectedIndex].value)});
			if (Sheet.length == 1){
				$("idSheets").selectedIndex = 1;
				HMI.showSheet(Sheet[0]);
			}
		} else {
			Node = document.createElement('option');
			Node.innerHTML = '- no sheets available -';
			Node.value = 'no sheet';
			HMI.PossSheets.appendChild(Node);
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
				if (cbfnc != null)
				{
					cbfnc(this, req);
				};
				break;
			default : ; // FAILURE
		}
		
		HMI.hmi_log_trace("HMIJavaScriptKSClient.prototype._handleStateChange - End");
	},
	
	/*********************************
		_sendRequest replaced by send2Request
	*********************************/
	_sendRequest: function(Client, method, async, obj, args, cbfnc) {
		HMI.hmi_log_trace("HMIJavaScriptKSClient.prototype._sendRequest - Start, Async:"+async+" Meth:"+method+", requested: "+'http://'+ this.TCLKSGateway + '?obj=' + obj + '&args='+ args );
		
		var req = new XMLHttpRequest();
		var DatePreventsCaching = new Date();
		
		//netscape.security.PrivilegeManager.enablePrivilege("UniversalBrowserRead");
		//does not work in other browsers than mozillabased
		try {
			req.open(method,
				'http://'
				+ this.TCLKSGateway
				+ '?obj='
				+ obj
				+ '&args='
				+ args
				+ '&preventCaching='
				+DatePreventsCaching.getTime(), async);
				
			if (async == true)
			{
				HMI.hmi_log_trace("HMIJavaScriptKSClient.prototype._sendRequest - entering async communication");
				//	Asynchron Communication
				//
				req.onreadystatechange = function () {
					Client._handleStateChange(req, cbfnc);
				};
			};
							
			try {
				if (method == 'POST') 
				{
					req.setRequestHeader('Content-Type', 'application/x-www-form-urlencoded');
					req.send('empty');
				} else {
					req.send(null);
				};

/*
HMI.hmi_log_info('http://'
				+ this.TCLKSGateway
				+ '/tks?obj='
				+ obj
				+ '&args='
				+ args
				+ '&preventCaching='
				+DatePreventsCaching.getTime() + "###########"+req.responseText);
*/
			} catch (e) {
				HMI.hmi_log_error('HMIJavaScriptKSClient._sendRequest: Request could not be sent. Is the gateway started?');
				HMI.hmi_log_onwebsite('Request could not be sent. Is the gateway started?');
				return false;
			};
		} catch (e) {
			HMI.hmi_log_error("HMIJavaScriptKSClient._sendRequest: Error during request. "+e);
			HMI.hmi_log_onwebsite("Error during request.");
			return false;
		};
		if (async == false)
		{
			//	Synchron Communication
			//
			if (cbfnc != null)
				cbfnc(this, req);
		};
		HMI.hmi_log_trace("HMIJavaScriptKSClient.prototype._sendRequest - End");
		return true;
	},
	
	/*********************************
		_send2Request
	*********************************/
	_send2Request: function(Client, method, async, urlparameter, cbfnc) {
		HMI.hmi_log_trace("HMIJavaScriptKSClient.prototype._send2Request - Start, Async:"+async+" Meth:"+method+", requested: "+'http://'+ HMI.KSClient.TCLKSGateway + '?' + urlparameter);
		
		var req = new XMLHttpRequest();
		var DatePreventsCaching = new Date();
		
		//netscape.security.PrivilegeManager.enablePrivilege("UniversalBrowserRead");
		//does not work in other browsers than mozillabased
		try {
			//FireFox 3 sends in a POST a content-encoding header killing the TCL Webserver
			//real POST is not nessessary, therefor GET is forced
			req.open('GET',
				'http://'
				+ HMI.KSClient.TCLKSGateway
				+ '?'
				+ urlparameter
				+ '&preventCaching2='
				+DatePreventsCaching.getTime(), async);
				
			if (async == true)
			{
				HMI.hmi_log_trace("HMIJavaScriptKSClient.prototype._send2Request - entering async communication");
				//	Asynchron Communication
				//
				req.onreadystatechange = function () {
					Client._handleStateChange(req, cbfnc);
				}
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
				HMI.hmi_log_error('HMIJavaScriptKSClient._send2Request: Request could not be sent. Is the gateway started?');
				HMI.hmi_log_onwebsite('Request could not be sent. Is the gateway started?');
				return false;
			}
		} catch (e) {
			HMI.hmi_log_error("HMIJavaScriptKSClient._send2Request: Error during request. "+e);
			HMI.hmi_log_onwebsite("Error during request.");
			return false;
		}
		if (async == false)
		{
			//	Synchron Communication
			//
			if (cbfnc != null){
				cbfnc(this, req);
			}else{
				HMI.hmi_log_trace("HMIJavaScriptKSClient.prototype._send2Request - End");
				return req.responseText;
			}
		}
		HMI.hmi_log_trace("HMIJavaScriptKSClient.prototype._send2Request - End");
		return true;
	},
	
	/*********************************
		prepareComponentText
	*********************************/
	prepareComponentText: function(ComponentText) {
		HMI.hmi_log_trace("HMIJavaScriptKSClient.prototype.prepareComponentText - Start");
		
		var Return = new Array(2);
		
		//	ComponentText should look like:
		//		{{GraphicDescription}} {{BehaviourDescription}}
		//	TksS-XXXX indicates error
		//
		if (/\bTksS-\b/.exec(ComponentText))
		{
			HMI.hmi_log_error('HMIJavaScriptKSClient.prototype.prepareComponentText: ' + ComponentText)
			HMI.hmi_log_onwebsite('Server lost: ' + ComponentText)
			clearTimeout(HMI.RefreshTimeoutID);
			HMI.RefreshTimeoutID = null;
			return null;
		} else {			
			Return[0] = ComponentText.substring(ComponentText.indexOf('{{') + 2, ComponentText.indexOf('}}'));
			Return[1] = ComponentText.substring(ComponentText.indexOf('}} {{') + 5, ComponentText.length - 2);
		};
		
		HMI.hmi_log_trace("HMIJavaScriptKSClient.prototype.prepareComponentText - End");
		
		return Return;
	},
	
	/*********************************
		destroy
	*********************************/
	destroy: function () {
		HMI.hmi_log_trace("HMIJavaScriptKSClient.prototype.destroy - Start");
		
		if (this.TCLKSHandle != null)
		{
//			this._sendRequest(this, 'POST', false, this.TCLKSHandle, 'destroy', null);
//			this.send2Request(null, 'GET', 'destroy', '', '', '', null);
			this.delHandle(this.TCLKSHandle);
		};
		this.KSServer = null;
		this.TCLKSHandle = null;
		
		HMI.hmi_log_trace("HMIJavaScriptKSClient.prototype.destroy - End");
	}
};