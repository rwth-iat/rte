/*
*	Copyright (C) 2013
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
*	hmi-class-gesture-rightclick.js
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
*	25-February-2009			Je
*		-	General Revision and full commented
*
*	27-March-2009			Je
*		-	correct click position transfered to the server
*
***********************************************************************/

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
		_registerOnRightClick
	*********************************/
	_registerOnRightClick: function(Component, capture, listener) {
		this._onRightClickThunk = function (evt) { listener.onRightClick(evt); };
		//mouseup is to different in browsers (especially safari@mac)
		if("unknown" == typeof Component.addEventListener || Component.addEventListener){
			//Adobe Plugin, Renesis, Firefox, Safari, Opera...
			Component.addEventListener("mousedown", this._onRightClickThunk, capture);
		}else if (Component.attachEvent){
			//Native IE
			Component.attachEvent("onmousedown", this._onRightClickThunk);
		}
	},
	
	/*********************************
		onRightClick
	*********************************/
	_onRightClickThunk: null,
	onRightClick: function (evt) {
		if (evt.button == 2)
		{
			HMI.hmi_log_trace("RightClick.prototype.onRightClick - Start");
			this._sendCommand(evt, HMI.getComponent(evt, 'hmi-component-gesture-rightclick'));
			HMI.hmi_log_trace("RightClick.prototype.onRightClick - End");
			//kill handling of this event for gesture at a parentNode
			//cancelBubble for IE Bubbling with native IE handling (not used)
			//stopPropagation for W3C Bubbling
			evt.cancelBubble = true;
			if (evt.stopPropagation) evt.stopPropagation();
		};
	},
	
	/*********************************
		_sendCommand
	*********************************/
	_sendCommand : function (evt, Component) {
		HMI.hmi_log_trace("RightClick.prototype._sendCommand - Component: "+(Component===null?"null":Component.id));
		if (Component !== null)
		{
			var Command = null;
			var clickPosition;
			//detect the mouse position relative to the component
			clickPosition = HMI.getClickPosition(evt, Component);
			
			Command = [HMI.KSClient.getMessageID(),
			           '010',
			           encodeURI(Component.id),
			           'RIGHTCLICK',
			           clickPosition[0],
			           clickPosition[1]];
			clickPosition = null;
			
			HMI.KSClient.setVar(HMI.KSClient.HMIMANAGER_PATH + '.Command', Command, null, HMI.cbrefreshSheet);
			Command = null;
			
			//mark changed Component
			HMI.displaygestureReactionMarker(Component);
		};
	}
};
var filedate = "$Date$";
filedate = filedate.substring(7, filedate.length-2);
if ("undefined" == typeof HMIdate){
	HMIdate = filedate;
}else if (HMIdate < filedate){
	HMIdate = filedate;
}
