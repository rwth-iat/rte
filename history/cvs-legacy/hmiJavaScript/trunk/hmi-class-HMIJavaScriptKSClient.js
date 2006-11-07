/***********************************************************************
	constructor
***********************************************************************/

function HMIJavaScriptKSClient() {
	//	Public Variables
	//
	this.KSServer;
	this.TCLKSGateway;
	this.TCLKSHandle;
	this.Request = this._createXMLHttpRequest();
	
	//	Private Variables
	//
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
		this.KSServer = HostAndServer;
		this.TCLKSGateway = TCLKSGateway;
		this.TCLKSHandle = null;
		
		if (this._sendRequest('tks-server', this.KSServer) == true)
		{
			if (/\bTksS-\b/.exec(this.Request.responseText))
			{
				this.TCLKSHandle = this.Request.responseText;
			} else {
				this.KSServer = null;
				alert('HMIJavaScriptKSClient.init: Could not initialize TCLKSGateway.'
					+ '\n'
					+ 'Gateway responded:'
					+ '\n\n'
					+ this.Request.responseText);
			};
		};
		
		return this.TCLKSHandle;
	},
	
	/*********************************
		sendRequest
	*********************************/
	sendRequest: function(service, args) {
		document.getElementById('idStatus').innerHTML = service + ' ' + args + '<p/>';
		
		var Return = new Array(2);
		
		if (/\bTksS-\b/.exec(this.TCLKSHandle))
		{
			if (this._sendRequest(this.TCLKSHandle, service + ' ' + args) == true)
			{
				//	responseText should look like:
				//		{{GraphicDescription}} {{BehaviourDescription}}
				//	TksS-XXXX indicates error
				//
				if (/\bTksS-\b/.exec(this.Request.responseText))
				{
					alert('HMIJavaScriptKSClient.sendRequest: ' + this.Request.responseText)
					return null;
				} else {			
					Return[0] = this.Request.responseText.substring(this.Request.responseText.indexOf('{{') + 2, this.Request.responseText.indexOf('}}'));
					Return[1] = this.Request.responseText.substring(this.Request.responseText.indexOf('}} {{') + 5, this.Request.responseText.length - 2);
				};
			};
			
			return Return;
		} else {
			alert('HMIJavaScriptKSClient.sendRequest: TCLKSGateway not initialized.');
		};
	},
	
	/*********************************
		destroy
	*********************************/
	destroy: function () {
		if (this.TCLKSHandle != null)
		{
			this._sendRequest(this.TCLKSHandle, 'destroy');
		};
		this.KSServer = null;
		this.TCLKSHandle = null;
	},
	
	/*********************************
		_sendRequest
	*********************************/
	_sendRequest: function(obj, args) {
		try {
			netscape.security.PrivilegeManager.enablePrivilege("UniversalBrowserRead");
			
			this.Request.open('GET',
				'http://'
				+ this.TCLKSGateway
				+ '/tks?obj='
				+ obj
				+ '&args='
				+ args, false);
				
			try {
				this.Request.send(null);
			} catch (e) {
				alert('HMIJavaScriptKSClient._sendRequest: Request could not be sent. Is the gateway started?');
				netscape.security.PrivilegeManager.disablePrivilege("UniversalBrowserRead");
				
				return false;
			};
			
			netscape.security.PrivilegeManager.disablePrivilege("UniversalBrowserRead");

		} catch (e) {
			alert("HMIJavaScriptKSClient._sendRequest: Permission UniversalBrowserRead denied.");
			return false;
	   };
	
		return true;
	},
	
	/*********************************
		_createXMLHttpRequest
	*********************************/	
	_createXMLHttpRequest: function() {
		var req = null;
		req = new XMLHttpRequest();
		
		return req;
	},
};