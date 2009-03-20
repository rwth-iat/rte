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
*	hmi-class-gesture-doubleclick.js
*
*	Editors:
*	--------
*	St							Stefan Schmitz <StefanS@plt.rwth-aachen.de>
*
*	Je							Holger Jeromin <Holger.Jeromin@plt.rwth-aachen.de>
*
*	NH							Nikolas Hansen <Nikolas.Hansen@plt.rwth-aachen.de>
*
*	CVS:
*	----
*	$Revision: 1.17 $
*	$Date: 2009-03-20 13:42:12 $
*
*	History:
*	--------
*	21-June-2007			St
*		-	General Revision
*
*	31-August-2008			NH
*		-	Sheet refresh after click, event bubbling deactivated
*
*	25-February-2009			Je
*		-	General Revision and full commented
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
		this._onMouseDownThunk = function (evt) { listener.onMouseDown(evt); };
		this._onDoubleClickThunk = function (evt) { listener.onDoubleClick(evt); };
		this._onMouseUpThunk = function (evt) { listener.onMouseUp(evt); };
		if("unknown" == typeof Component.addEventListener || Component.addEventListener){
			//Adobe Plugin, Renesis, Firefox, Safari, Opera...
			Component.addEventListener("mousedown", this._onMouseDownThunk, capture);
			Component.addEventListener("click", this._onDoubleClickThunk, capture);
			Component.addEventListener("mouseup", this._onMouseUpThunk, capture);
		}else if (Component.attachEvent){
			//Native IE
			Component.attachEvent("onmousedown", this._onMouseDownThunk);
			Component.attachEvent("onclick", this._onDoubleClickThunk);
			Component.attachEvent("onmouseup", this._onMouseUpThunk);
		}
	},
	
	/*********************************
		onMouseDown
	*********************************/
	_onMouseDownThunk: null,
	onMouseDown: function (evt) {
		if (HMI.RefreshTimeoutID != null){
			//deactivate the Refresh
			//if there is a Screen-Refresh between mouse-down and mouse-up the click would be lost
			clearTimeout(HMI.RefreshTimeoutID);
			HMI.RefreshTimeoutID = null;
		}
	},
	
	/*********************************
		onMouseUp
	*********************************/
	_onMouseUpThunk: null,
	onMouseUp: function (evt) {
		if (HMI.RefreshTimeoutID == null){
			//reactivate the Refresh
			HMI.RefreshTimeoutID = setInterval('HMI.refreshSheet()', HMI.RefreshTime);
		}
	},
	
	/*********************************
		onDoubleClick
	*********************************/
	_onDoubleClickThunk: null,
	onDoubleClick: function (evt) {
		//possible that a screen refresh between two clicks doesnot trigger a double click
		if (evt.button == 0 && evt.detail ==2){
			evt.stopPropagation();
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
			//detect the mouse position relative to the component
			if (evt.layerX != undefined){
				//Firefox, Webkit
				var xpos = evt.layerX;
				var ypos = evt.layerY;
				//this is a mouse offset provided by the mouse event
				//not related with the HMI specific SVG-Attribute LayerX!
			}else if (evt.offsetX != undefined){
				//Opera
				var xpos = evt.offsetX;
				var ypos = evt.offsetY;
			}else{
				//IE Adobe SVG Viewer
				var xpos = 1;
				var ypos = 1;
			}
			Command = '{' + HMI.KSClient.getMessageID() + '}%20' +
				'{010}%20' +
				'{' + Component.getAttribute('id') + '}%20' + 
				'{DOUBLECLICK}%20' +
				'{' + xpos + '}%20' +
				'{' + ypos + '}';
			HMI.KSClient.setVar(null, HMI.KSClient.HMIMANAGER_PATH + '.Command', Command, HMI.cbrefreshSheet);
		};
		
		delete Command;
	}
};
var filedate = "$Date: 2009-03-20 13:42:12 $";
filedate = filedate.substring(7, filedate.length-2);
if ("undefined" == typeof HMIdate){
	HMIdate = filedate;
}else if (HMIdate < filedate){
	HMIdate = filedate;
}
