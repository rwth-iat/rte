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
		HMI.reactivateRefreshInterval(evt);
	},
	
	/*********************************
		onDoubleClick
	*********************************/
	_onDoubleClickThunk: null,
	onDoubleClick: function (evt) {
		//possible that a screen refresh between two clicks doesnot trigger a double click
		HMI.hmi_log_trace("DoubleClick.prototype.onDoubleClick - Start");
		if (evt.button === 0 && evt.detail ==2){
			HMI.hmi_log_trace("DoubleClick.prototype.onDoubleClick - Doubleclick detected");
			this._sendCommand(evt, HMI.getComponent(evt, 'hmi-component-gesture-doubleclick'));
			//kill handling of this event for gesture at a parentNode
			//cancelBubble for IE Bubbling with native IE handling (not used)
			//stopPropagation for W3C Bubbling
			evt.cancelBubble = true;
			if (evt.stopPropagation) evt.stopPropagation();
		}else{
			HMI.hmi_log_trace("DoubleClick.prototype.onDoubleClick - no Doubleclick detected");
		}
	},
	
	/*********************************
		_sendCommand
	*********************************/
	_sendCommand : function (evt, Component) {
		HMI.hmi_log_trace("DoubleClick.prototype._sendCommand - Component: "+(Component===null?"null":Component.id));
		if (Component !== null)
		{
			var Command = null;
			var clickPosition;
			//detect the mouse position relative to the component
			clickPosition = HMI.getClickPosition(evt, Component);
			
			Command = [HMI.KSClient.getMessageID(),
			           '010',
			           encodeURI(Component.id),
			           'DOUBLECLICK',
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
