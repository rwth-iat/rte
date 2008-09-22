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
*	hmi-class-gesture-doubleclick.js
*
*	Editors:
*	--------
*	St							Stefan Schmitz <StefanS@plt.rwth-aachen.de>
*
*	CVS:
*	----
*	$Revision: 1.4 $
*	$Date: 2008-09-22 08:20:51 $
*
*	History:
*	--------
*	21-June-2007			St
*		-	General Revision
*
***********************************************************************/

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
		Component.addEventListener("click", this._onDoubleClickThunk, capture);
	},
	
	/*********************************
		onDoubleClick
	*********************************/
	_onDoubleClickThunk: null,
	onDoubleClick: function (evt) {
		if (evt.button == 0 && evt.detail ==2){
			this._sendCommand(evt, HMI.getComponent(evt, 'hmi-component-gesture-doubleclick'));
		}
	},
	
	/*********************************
		_sendCommand
	*********************************/
	_sendCommand : function (evt, Component) {
		var Command = null;
		
		if (Component != null)
		{
			//Command = '{' + 
			//	'{' + HMI.KSClient.getMessageID() + '} ' +
			//	'{010} ' +
			//	'{' + Component.getAttribute('id') + '} ' + 
			//	'{DOUBLECLICK} ' +
			//	'{' + evt.layerX + '} ' +
			//	'{' + evt.layerY + '}' +
			//	'}';
			//HMI.KSClient.send2Request(null, 'POST', 'setvar', 
			//	'{' + '/TechUnits/HMIManager' + '.Command ' + Command + '}',
			//	'', '', null);
			Command = '{' + HMI.KSClient.getMessageID() + '} ' +
				'{010} ' +
				'{' + Component.getAttribute('id') + '} ' + 
				'{DOUBLECLICK} ' +
				'{' + evt.layerX + '} ' +
				'{' + evt.layerY + '}';
			HMI.KSClient.setVar(null, '/TechUnits/HMIManager' + '.Command', Command, null);
//			HMI.KSClient.sendRequest('setvar', 'POST', '{' + '/TechUnits/HMIManager' + '.Command ' + Command + '}');
		};
		
		delete Command;
	}
};