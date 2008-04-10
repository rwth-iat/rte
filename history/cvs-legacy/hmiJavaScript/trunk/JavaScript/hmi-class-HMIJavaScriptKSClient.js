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
*	$Revision: 1.12 $
*	$Date: 2008-04-10 14:03:39 $
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
***********************************************************************/

/***********************************************************************
	constructor
***********************************************************************/

function HMIJavaScriptKSClient(async) {
	/** Public **********************/
	this.KSServer;
	this.TCLKSGateway;
	this.TCLKSHandle;
	
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
		
		if (this.TCLKSHandle != null)
			this.destroy();
		
		this.KSServer		= HostAndServer;
		this.TCLKSGateway	= TCLKSGateway;
		this.TCLKSHandle	= null;
		
		this._sendRequest(this, 'GET', false, 'tks-server', this.KSServer, this._cbInit);
		
		HMI.hmi_log_trace("HMIJavaScriptKSClient.prototype.init - End");
	},
	
	/*********************************
		sendRequest
	*********************************/
	sendRequest: function(service, method, args, cbfnc) {
		HMI.hmi_log_trace("HMIJavaScriptKSClient.prototype.sendRequest - Start");
		
		if (/\bTksS-\b/.exec(this.TCLKSHandle))
		{
			this._sendRequest(this, method, this.Asynchron, this.TCLKSHandle, service + ' ' + args, cbfnc);
		} else {
			HMI.hmi_log_error('HMIJavaScriptKSClient.prototype.sendRequest - TCLKSGateway not initialized.');
		};
		
		HMI.hmi_log_trace("HMIJavaScriptKSClient.prototype.sendRequest - End");
	},
	
	/*********************************
		_cbinit
	*********************************/
	_cbInit: function(Client, req) {
		HMI.hmi_log_trace("HMIJavaScriptKSClient.prototype._cbinit - Start");
		
		if (/\bTksS-\b/.exec(req.responseText))
		{
			Client.TCLKSHandle = req.responseText;
		} else {
			Client.KSServer = null;
			HMI.hmi_log_error('HMIJavaScriptKSClient._cbinit: Could not initialize TCLKSGateway.'
				+ '\n'
				+ 'Gateway responded:'
				+ '\n\n'
				+ req.responseText);
		};
		
		HMI.hmi_log_trace("HMIJavaScriptKSClient.prototype._cbinit - End");
	},
	
	/*********************************
		getServers
	*********************************/
	getServers: function() {
		HMI.hmi_log_trace("HMIJavaScriptKSClient.prototype.getServers - Start");
		
		if (this.TCLKSHandle != null)
		{
			this._sendRequest(this, 'GET', false, this.TCLKSHandle, 'getep /servers * -output $::TKS::OP_NAME', this._cbGetServers);
		} else
			return null;
		
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
			HMI.PossServers.appendChild(Node);
			
			for (i = 0; i < Server.length; i++)
			{
				if (HMIJavaScriptKSClient.prototype.pingServer(Server[i]) == true)
				{
					Node = document.createElement('option');
					Node.innerHTML = Server[i];
					HMI.PossServers.appendChild(Node);
					ValidServers ++;
					IdLastValidServer = i;
				};
			};
			HMI.hmi_log_trace("HMIJavaScriptKSClient.prototype._cbGetServers - number of valid servers: "+ValidServers);
			if (ValidServers == 0){
				Node.innerHTML = '- no server available -';
			}else{
				HMI.PossServers.addEventListener('change', function () {HMI.showSheets(this.options[this.selectedIndex].value)}, false);
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
			HMI.PossServers.appendChild(Node);
		};
		
		HMI.hmi_log_trace("HMIJavaScriptKSClient.prototype._cbGetServers - End");
	},
	
	/*********************************
		_simpleRequest
	*********************************/
	_simpleRequest: function(obj, args) {
//		HMI.hmi_log_trace("HMIJavaScriptKSClient.prototype._simpleRequest - Start");
		HMI.hmi_log_trace("HMIJavaScriptKSClient.prototype._simpleRequest - Start, requested: "+'http://'+ HMI.KSClient.TCLKSGateway + '/tks?obj=' + obj + '&args='+ args );
		
		var req = new XMLHttpRequest();
		
		req.open('GET',
			'http://'
			+ HMI.KSClient.TCLKSGateway
			+ '/tks?obj='
			+ obj
			+ '&args='
			+ args, false);
		req.setRequestHeader('Content-Type', 'application/x-www-form-urlencoded');
		req.send('empty');
		
		HMI.hmi_log_trace("HMIJavaScriptKSClient.prototype._simpleRequest - End");
		
		return req.responseText;
	},
	
	/*********************************
		_cbSimpleRequest
	*********************************/
	_cbSimpleRequest: function(Client, req) {
		HMI.hmi_log_trace("HMIJavaScriptKSClient.prototype._cbSimpleRequest - Start");
		
		return req.responseText;
		
		HMI.hmi_log_trace("HMIJavaScriptKSClient.prototype._cbSimpleRequest - End");
	},
	
	/*********************************
		pingServer
	*********************************/
	pingServer: function(Server) {
		HMI.hmi_log_trace("HMIJavaScriptKSClient.prototype.pingServer - Start");
		
		var TCLKSHandle = null;
		var ManagerResponse = null;
		
		try {
			TCLKSHandle = this._simpleRequest('tks-server',
				HMI.KSClient.KSServer.substring(0, HMI.KSClient.KSServer.indexOf('/')) + '/' + Server);
				
			ManagerResponse = this._simpleRequest(TCLKSHandle,
				'getep ' + HMI.HMI_Constants.HMIMANAGER_PATH + ' * -output $::TKS::OP_NAME');
				
			this._simpleRequest(TCLKSHandle,
				'destroy');

			// Opera bis exklusive version 9.5 liefert einen leeren responseText bei HTTP-Status 503
			if (ManagerResponse.length == 0){
				HMI.hmi_log_trace("HMIJavaScriptKSClient.prototype.pingServer - End1f+Operabug");
				return false;
			}else if (/\bTksS-\b/.exec(ManagerResponse))
			{
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
		
		var Command = null;
		Command = '{' + 
			'{' + HMI.KSClient.getMessageID() + '} ' +
			'{010} ' +
			'{' + HMI.HMI_Constants.HMIMANAGER_PATH + '} ' + 
			'{SHOWSHEETS}' +
			'}';
		
		this._sendRequest(this, 'POST', false, this.TCLKSHandle, 'setvar '
			+ '{'
			+ HMI.HMI_Constants.HMIMANAGER_PATH
			+ '.Command '
			+ Command
			+ '}' , null);
			
		this._sendRequest(this, 'GET', false, this.TCLKSHandle, 'getvar '
			+ '{'
			+ HMI.HMI_Constants.HMIMANAGER_PATH
			+ '.CommandReturn} '
			+ '-output $::TKS::OP_VALUE', this._cbGetSheets);
		
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
			HMI.PossSheets.appendChild(Node);
			
			for (i = 0; i < Sheet.length; i++){
				Node = document.createElement('option');
				Node.innerHTML = Sheet[i];
				HMI.PossSheets.appendChild(Node);
			};
			HMI.PossSheets.addEventListener('change', function () {HMI.showSheet(this.options[this.selectedIndex].value)}, false);
			if (Sheet.length == 1){
				$("idSheets").selectedIndex = 1;
				HMI.showSheet(Sheet[0]);
				if (autoKeepHeader == false){
					hideHeader();
				}
			}
		} else {
			Node = document.createElement('option');
			Node.innerHTML = '- no sheets available -';
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
		_sendRequest
	*********************************/
	_sendRequest: function(Client, method, async, obj, args, cbfnc) {
//		HMI.hmi_log_trace("HMIJavaScriptKSClient.prototype._sendRequest - Start");
		HMI.hmi_log_trace("HMIJavaScriptKSClient.prototype._sendRequest - Start, requested: "+'http://'+ this.TCLKSGateway + '/tks?obj=' + obj + '&args='+ args );

		
		var req = new XMLHttpRequest();
		
		try {
			req.open(method,
				'http://'
				+ this.TCLKSGateway
				+ '/tks?obj='
				+ obj
				+ '&args='
				+ args, async);
				
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
				
				if (async == false)
				{
					//	Synchron Communication
					//
					if (cbfnc != null)
						cbfnc(this, req);
				};
			} catch (e) {
				HMI.hmi_log_error('HMIJavaScriptKSClient._sendRequest: Request could not be sent. Is the gateway started?');
				return false;
			};
			HMI.hmi_log_trace("HMIJavaScriptKSClient.prototype._sendRequest - End");
			return true;
		} catch (e) {
			HMI.hmi_log_error("HMIJavaScriptKSClient._sendRequest: Error during request.");
			return false;
		};
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
			this._sendRequest(this, 'POST', false, this.TCLKSHandle, 'destroy', null);
		};
		this.KSServer = null;
		this.TCLKSHandle = null;
		
		HMI.hmi_log_trace("HMIJavaScriptKSClient.prototype.destroy - End");
	}
};