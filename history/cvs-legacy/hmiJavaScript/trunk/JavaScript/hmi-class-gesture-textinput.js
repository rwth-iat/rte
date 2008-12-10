/*
*	Copyright (C) 2008
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
*	hmi-class-gesture-textinput.js
*
*	Editors:
*	--------
*	St							Stefan Schmitz <StefanS@plt.rwth-aachen.de>
*
*	Je							Holger Jeromin <Holger.Jeromin@plt.rwth-aachen.de>
*
*	CVS:
*	----
*	$Revision: 1.15 $
*	$Date: 2008-12-10 14:01:40 $
*
*	History:
*	--------
*	26-June-2007			St
*		-	File created
*
***********************************************************************/

/***********************************************************************
	constructor
***********************************************************************/

function TextInput (Component, Controller) {
	this.arm(Component);
};

/***********************************************************************
	prototype
***********************************************************************/

TextInput.prototype = {
	/*********************************
		arm
	*********************************/
	arm : function (Component) {
		this._registerOnClick(Component, false, this);
		Component.setAttribute("cursor", "text");
	},
	
	/*********************************
		_registerOnClick
	*********************************/
	_registerOnClick: function(Component, capture, listener) {
		this._onMouseDownThunk = function (evt) { listener.onMouseDown(evt); };
		this._onClickThunk = function (evt) { listener.onClick(evt); };
		this._onMouseUpThunk = function (evt) { listener.onMouseUp(evt); };
		Component.addEventListener("mousedown", this._onMouseDownThunk, capture);
		Component.addEventListener("click", this._onClickThunk, capture);
		Component.addEventListener("mouseup", this._onMouseUpThunk, capture);
	},
	
	/*********************************
		onMouseDown
	*********************************/
	_onMouseDownThunk: null,
	onMouseDown: function (evt) {
		if (HMI.RefreshTimeoutID != null){
			clearTimeout(HMI.RefreshTimeoutID);
			HMI.RefreshTimeoutID = null;
		}
	},
	
	/*********************************
		onMouseUp
	*********************************/
	_onMouseUpThunk: null,
	onMouseUp: function (evt) {
		HMI.RefreshTimeoutID = setInterval('HMI.refreshSheet()', HMI.RefreshTime);
	},
	
	/*********************************
		onClick
	*********************************/
	_onClickThunk: null,
	onClick: function (evt) {		
		if (evt.detail == 2)
		{
			//	doubleclick => STOP
			return;
		};
		if (evt.target.textContent != undefined){
			var text = evt.target.textContent;
		}else{  //Adobe Plugin
			var text = evt.target.childNodes.item(0).nodeValue;
		}
		var input = prompt('Geben Sie bitte einen neuen Wert ein', text);
		if(	input	!= null
			&&	input	!= ""
			&&	input != text)
		{
			this._sendCommand(evt, HMI.getComponent(evt, 'hmi-component-gesture-textinput'), input);
		};
	},
	
	/*********************************
		_sendCommand
	*********************************/
	_sendCommand : function (evt, Component, input) {
		var Command = null;
		
		if (Component != null)
		{
			Command = '{' + HMI.KSClient.getMessageID() + '} ' +
				'{010} ' +
				'{' + Component.getAttribute('id') + '} ' + 
				'{TEXTINPUT} ' +
				'{' + input + '}';
			HMI.KSClient.setVar(null, HMI.KSClient.HMIMANAGER_PATH + '.Command', Command, HMI.cbrefreshSheet);
		};
		
		delete Command;
	}
};
var filedate = "$Date: 2008-12-10 14:01:40 $";
filedate = filedate.substring(7, filedate.length-2);
if ("undefined" == typeof HMIdate){
	HMIdate = filedate;
}else if (HMIdate < filedate){
	HMIdate = filedate;
}
