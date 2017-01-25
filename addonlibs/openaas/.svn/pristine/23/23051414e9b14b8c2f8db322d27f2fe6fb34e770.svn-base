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
*	hmi-class-gesture-click.js
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
*	$Revision$
*	$Date$
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
*	27-March-2009			Je
*		-	correct click position transfered to the server
*
***********************************************************************/

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
		this._onMouseDownThunk = function (evt) { listener.onMouseDown(evt); };
		this._onClickThunk = function (evt) { listener.onClick(evt); };
		this._onMouseUpThunk = function (evt) { listener.onMouseUp(evt); };
		if("unknown" == typeof Component.addEventListener || Component.addEventListener){
			//Adobe Plugin, Renesis, Firefox, Safari, Opera...
			Component.addEventListener("mousedown", this._onMouseDownThunk, capture);
			Component.addEventListener("click", this._onClickThunk, capture);
			Component.addEventListener("mouseup", this._onMouseUpThunk, capture);
		}else if (Component.attachEvent){
			//Native IE
			Component.attachEvent("onmousedown", this._onMouseDownThunk);
			Component.attachEvent("onclick", this._onClickThunk);
			Component.attachEvent("onmouseup", this._onMouseUpThunk);
		}
	},
	
	/*********************************
		onMouseDown
	*********************************/
	_onMouseDownThunk: null,
	onMouseDown: function (evt) {
		HMI.hmi_log_trace("Click.prototype.onMouseDown: "+(evt.target ? evt.target.id : evt.srcElement.id));
		if (HMI.RefreshTimeoutID !== null){
			//deactivate the Refresh
			//if there is a Screen-Refresh between mouse-down and mouse-up the click would be lost
			window.clearTimeout(HMI.RefreshTimeoutID);
			HMI.RefreshTimeoutID = null;
			
			//provide a fallback for the case the mouseup do not fire
			if(HMI.svgDocument.addEventListener){
				//Firefox, Safari, Opera...
				HMI.svgDocument.addEventListener("mousemove", HMI.reactivateRefreshInterval, false);
			}else if("unknown" == typeof HMI.svgDocument.documentElement.addEventListener){
				//Adobe Plugin
				HMI.svgDocument.documentElement.addEventListener("mousemove", HMI.reactivateRefreshInterval, false);
			}else if (HMI.svgDocument.attachEvent){
				//Native IE
				HMI.svgDocument.attachEvent("mousemove", HMI.reactivateRefreshInterval);
			}else if(HMI.svgDocument.documentElement.addEventListener){
				//Renesis Plugin
				HMI.svgDocument.documentElement.addEventListener("mousemove", HMI.reactivateRefreshInterval, false);
			}
		}
		//kill handling of this event for gesture at a parentNode
		//cancelBubble for IE Bubbling with native IE handling (not used)
		//stopPropagation for W3C Bubbling
		evt.cancelBubble = true;
		if (evt.stopPropagation) evt.stopPropagation();
	},
	
	/*********************************
		onMouseUp
	*********************************/
	_onMouseUpThunk: null,
	onMouseUp: function (evt) {
		HMI.hmi_log_trace("Click.prototype.onMouseUp");
		HMI.reactivateRefreshInterval(evt);
	},
	
	/*********************************
		onClick
	*********************************/
	_onClickThunk: null,
	onClick: function (evt) {
		HMI.hmi_log_trace("Click.prototype.onClick - Start");
		if (evt.detail == 2)
		{
			HMI.hmi_log_trace("Click.prototype.onClick - End - DoubleClick detected");
			//	doubleclick => STOP
			//
			return;
		};
		
		this._sendCommand(evt, HMI.getComponent(evt, 'hmi-component-gesture-click'));
		//kill handling of this event for gesture at a parentNode
		//cancelBubble for IE Bubbling with native IE handling (not used)
		//stopPropagation for W3C Bubbling
		evt.cancelBubble = true;
		if (evt.stopPropagation) evt.stopPropagation();
		HMI.hmi_log_trace("Click.prototype.onClick - End");
	},
	
	/*********************************
		_sendCommand
	*********************************/
	_sendCommand : function (evt, Component) {
		HMI.hmi_log_trace("Click.prototype._sendCommand - Component: "+(Component===null?"null":Component.id));
		if (Component !== null)
		{
			var Command = null;
			var clickPosition;
			//detect the mouse position relative to the component
			clickPosition = HMI.getClickPosition(evt, Component);
			
			Command = [HMI.KSClient.getMessageID(),
			           '010',
			           encodeURI(Component.id),
			           'CLICK',
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
