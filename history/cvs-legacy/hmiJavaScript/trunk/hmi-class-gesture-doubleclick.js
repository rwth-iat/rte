/***********************************************************************
	constructor
***********************************************************************/

function DoubleClick (Component, Controller) {
	this.arm(Component);
};

/***********************************************************************
	prototype
***********************************************************************/

DoubleClick.prototype = {
	/*********************************
		arm
	*********************************/
	arm : function (Component) {
		this._registerOnDoubleClick(Component, false, this);
		Component.setAttribute("cursor", "pointer");
	},
	
	/*********************************
		_registerOnDoubleClick
	*********************************/
	_registerOnDoubleClick: function(Component, capture, listener) {
		this._onDoubleClickThunk = function (evt) { listener.onDoubleClick(evt); };
		Component.addEventListener("dblclick", this._onDoubleClickThunk, capture);
	},
	
	/*********************************
		onDoubleClick
	*********************************/
	_onDoubleClickThunk: null,
	onDoubleClick: function (evt) {
		this._sendCommand(evt, HMI.getComponent(evt, 'hmi-component-gesture-doubleclick'));
	},
	
	/*********************************
		_sendCommand
	*********************************/
	_sendCommand : function (evt, Component) {
		var Command = null;
		
		if (Component != null)
		{
			Command = '{' + 
				'{' + HMI.KSClient.getMessageID() + '} ' +
				'{010} ' +
				'{' + Component.getAttribute('id') + '} ' + 
				'{DOUBLECLICK} ' +
				'{' + evt.layerX + '} ' +
				'{' + evt.layerY + '}' +
				'}';
			HMI.KSClient.sendRequest('setvar', '{' + '/TechUnits/HMIManager' + '.Command ' + Command + '}');
		};
		
		delete Command;
	},
};