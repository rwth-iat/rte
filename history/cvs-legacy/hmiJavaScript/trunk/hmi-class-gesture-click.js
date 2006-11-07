/***********************************************************************
	constructor
***********************************************************************/

function Click (Component, Controller) {
	this.arm(Component);
};

/***********************************************************************
	prototype
***********************************************************************/

Click.prototype = {
	/*********************************
		arm
	*********************************/
	arm : function (Component) {
		this._registerOnClick(Component, false, this);
		Component.setAttribute("cursor", "pointer");
	},
	
	/*********************************
		_registerOnClick
	*********************************/
	_registerOnClick: function(Component, capture, listener) {
		this._onClickThunk = function (evt) { listener.onClick(evt); };
		Component.addEventListener("click", this._onClickThunk, capture);
	},
	
	/*********************************
		onClick
	*********************************/
	_onClickThunk: null,
	onClick: function (evt) {		
		if (evt.detail == 2)
		{
			//	doubleclick => STOP
			//
			return;
		};
		
		this._sendCommand(evt, HMI.getComponent(evt, 'hmi-component-gesture-click'));
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
				'{CLICK} ' +
				'{' + evt.layerX + '} ' +
				'{' + evt.layerY + '}' +
				'}';
			HMI.KSClient.sendRequest('setvar', '{' + '/TechUnits/HMIManager' + '.Command ' + Command + '}');
		};
		
		delete Command;
	},
};