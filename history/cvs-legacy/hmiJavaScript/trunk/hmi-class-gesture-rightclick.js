/***********************************************************************
	constructor
***********************************************************************/

function RightClick (Component, Controller) {
	this.arm(Component);
};

/***********************************************************************
	prototype
***********************************************************************/

RightClick.prototype = {
	/*********************************
		arm
	*********************************/
	arm : function (Component) {
		this._registerOnRightClick(Component, false, this);
		Component.setAttribute("cursor", "pointer");
	},
	
	/*********************************
		_registerOnClick
	*********************************/
	_registerOnRightClick: function(Component, capture, listener) {
		this._onRightClickThunk = function (evt) { listener.onRightClick(evt); };
		Component.addEventListener("mouseup", this._onRightClickThunk, capture);
	},
	
	/*********************************
		onRightClick
	*********************************/
	_onRightClickThunk: null,
	onRightClick: function (evt) {
		if (evt.button == 2)
		{
			this._sendCommand(evt, HMI.getComponent(evt, 'hmi-component-gesture-rightclick'));
		};
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
				'{RIGHTCLICK} ' +
				'{' + evt.layerX + '} ' +
				'{' + evt.layerY + '}' +
				'}';
			HMI.KSClient.sendRequest('setvar', '{' + '/TechUnits/HMIManager' + '.Command ' + Command + '}');
		};
		
		delete Command;
	},
};