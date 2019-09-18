/*
*   Copyright (C) 2013
*   Lehrstuhl fuer Prozessleittechnik,
*   D-52056 Aachen, Germany.
*   All rights reserved.
*
*
*   This file is part of the ACPLT/OV Package 
*   Licensed under the Apache License, Version 2.0 (the "License");
*   you may not use this file except in compliance with the License.
*   You may obtain a copy of the License at
*
*       http://www.apache.org/licenses/LICENSE-2.0
*
*   Unless required by applicable law or agreed to in writing, software
*   distributed under the License is distributed on an "AS IS" BASIS,
*   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*   See the License for the specific language governing permissions and
*   limitations under the License.
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
