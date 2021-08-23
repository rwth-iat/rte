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
*	$Revision$
*	$Date$
*
*	History:
*	--------
*	26-June-2007			St
*		-	File created
*
*	25-February-2009			Je
*		-	General Revision and full commented
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
		
		//not clickable if the String is only white spaces
		var buildDummyRect = false;
		//IE9, Gecko, Opera and Webkit are able to use textContent to find all Text
		if (typeof Component.textContent != "undefined" && Component.textContent.trim){
			if (Component.textContent.trim() === "") {
				buildDummyRect = true;
			}
		}else if (typeof Component.innerText != "undefined" ){
			if (!/\S+/.exec(Component.innerText)) {
				buildDummyRect = true;
			}
		}else{
			//other browsers has to iterate over all elements
			var Text;
			for (var idx = 0; idx < Component.childNodes.length; ++idx){
				if (Component.childNodes.item(idx).tagName == "svg:text" && Component.childNodes.item(idx).firstChild && Component.childNodes.item(idx).firstChild.nodeValue){
					Text += Component.childNodes.item(idx).firstChild.nodeValue;
				}else if (Component.childNodes.item(idx).tagName == "svg:tspan" && Component.childNodes.item(idx).firstChild && Component.childNodes.item(idx).firstChild.nodeValue){
					Text += Component.childNodes.item(idx).firstChild.nodeValue;
				}
			}
			if (!/\S+/.Text){
				buildDummyRect = true;
			}
		}
		if (buildDummyRect === true){
			//build transparent dummy rect to allow input of new text
			var dummyRect = HMI.svgDocument.createElementNS(HMI.HMI_Constants.NAMESPACE_SVG, 'rect');
			dummyRect.setAttributeNS(null, 'x', Component.getElementsByTagName("svg:text").length !== 0?(Component.getElementsByTagName("svg:text").item(0).getAttribute("x")):0);
			dummyRect.setAttributeNS(null, 'y', Component.getElementsByTagName("svg:text").length !== 0?(Component.getElementsByTagName("svg:text").item(0).getAttribute("y")-15):0);
			dummyRect.setAttributeNS(null, 'width', '15');
			dummyRect.setAttributeNS(null, 'height', '20');
			dummyRect.setAttributeNS(null, 'style', 'opacity:0;');
			dummyRect.setAttributeNS(null, 'class', 'dummyTextinputRect');
			Component.appendChild(dummyRect);
			dummyRect = null;
		}
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
		onClick
	*********************************/
	_onClickThunk: null,
	onClick: function (evt) {
		HMI.hmi_log_trace("TextInput.prototype.onClick - Start");
		if (evt.detail == 2)
		{
			HMI.hmi_log_trace("TextInput.prototype.onClick - End - DoubleClick detected");
			//	doubleclick => STOP
			//
			return;
		};
		//kill handling of this event for gesture at a parentNode
		//cancelBubble for IE Bubbling with native IE handling (not used)
		//stopPropagation for W3C Bubbling
		evt.cancelBubble = true;
		if (evt.stopPropagation) evt.stopPropagation();
		
		var input;
		input = this.getTextContent(evt);
		
		if(	input	!== null
			&&	input	!== undefined)
		{
			this._sendCommand(evt, HMI.getComponent(evt, 'hmi-component-gesture-textinput'), input);
		};
		//kill handling of this event for gesture at a parentNode
		//cancelBubble for IE Bubbling with native IE handling (not used)
		//stopPropagation for W3C Bubbling
		evt.cancelBubble = true;
		if (evt.stopPropagation) evt.stopPropagation();
		HMI.hmi_log_trace("TextInput.prototype.onClick - End");
	},
	
	/*********************************
		getTextContent
	*********************************/
	getTextContent : function (evt) {
		var text;
		//if we are called from the move gesture evt.target and evt.currentTarget could be document or the sheet
		if (evt.target && evt.target.getAttribute("class") == "dummyTextinputRect"){
			//the TextInput was empty, so the user clicked on the dummyRect
			text = "";
		}else if (evt.srcElement && evt.srcElement.getAttribute("class") == "dummyTextinputRect"){
			//native IE eventhandling
			text = "";
		}else if (evt.currentTarget && evt.currentTarget != HMI.svgDocument &&
			//currentTarget is the Component
			evt.currentTarget.getElementsByTagNameNS &&
			evt.currentTarget.getElementsByTagNameNS(HMI.HMI_Constants.NAMESPACE_SVG, "text").item(0) &&
			evt.currentTarget.getElementsByTagNameNS(HMI.HMI_Constants.NAMESPACE_SVG, "text").item(0).firstChild){
			//catch first text element, ignore the rest
			text = evt.currentTarget.getElementsByTagNameNS(HMI.HMI_Constants.NAMESPACE_SVG, "text").item(0).firstChild.nodeValue;
		}else if (evt.currentTarget && evt.currentTarget != HMI.svgDocument && typeof evt.currentTarget.textContent != "undefined"){
			//currentTarget is the Component
			//TextContent is the sum of Text
			text = evt.currentTarget.textContent;
		}else if (evt.currentTarget && evt.currentTarget.firstChild && evt.currentTarget.firstChild.firstChild && evt.currentTarget.firstChild.firstChild.nodeValue ){
			//currentTarget is the Component
			//Adobe Plugin
			text = evt.currentTarget.firstChild.firstChild.nodeValue;
		}else if (evt.target && evt.target != HMI.Playground.firstChild && typeof evt.target.textContent != "undefined"){
			//TextContent is the sum of Text
			text = evt.target.textContent;
		}else if (evt.target && evt.target != HMI.Playground.firstChild && evt.target.firstChild && evt.target.firstChild.nodeValue){
			//Adobe Plugin
			text = evt.target.firstChild.nodeValue;
		}else if (evt.srcElement && evt.srcElement.firstChild && evt.srcElement.firstChild.nodeValue){  //Native IE Code
			//native IE eventhandling
			text = evt.srcElement.firstChild.nodeValue;
		}else if (evt.target && evt.target != HMI.Playground.firstChild && evt.target.parentNode && evt.target.parentNode.getElementsByTagName && evt.target.parentNode.getElementsByTagName("svg:text").item(0)){
			//we clicked a svg object and need the first Textcontent
			//Gecko and co should be catched via textContent
			text = evt.target.parentNode.getElementsByTagName("svg:text").item(0).nodeValue;
		}else{
			text = "";
		}
		if (text === null || text === undefined){
			text = "";
		}
		var input;
		
		if(!window.prompt){
			HMI.hmi_log_info_onwebsite("Sorry, your browser does not support textinput. If this is IE10 Metro mode, try desktop mode.");
			input = null;
		}else if (evt.currentTarget && ("unknown" == typeof evt.currentTarget.hasAttributeNS || evt.currentTarget.hasAttributeNS) && evt.currentTarget.hasAttributeNS("http://www.acplt.de/hmi", "textinputcaption")) {
			//	supported in Firefox 2+3, Opera 10, Webkit, Adobe SVG Viewer 3, Renesis 1.1
			input = window.prompt(evt.currentTarget.getAttributeNS("http://www.acplt.de/hmi", "textinputcaption"), text);
		}else{
			input = window.prompt('Please input a new value', text);
		}
		
		text = null;
		
		return input;
	},
	
	/*********************************
		_sendCommand
	*********************************/
	_sendCommand : function (evt, Component, input) {
		HMI.hmi_log_trace("TextInput.prototype._sendCommand - Component: "+(Component===null?"null":Component.id));
		if (Component !== null)
		{
			var Command = null;
			
			Command = [HMI.KSClient.getMessageID(),
			           '010',
			           encodeURI(Component.id),
			           'TEXTINPUT',
			           //urlencode the UTF8 String in ISO-8859-1 Format
			           //change the %u20AC of the &euro; Symbol to windows-1252 (accepted by all Browsers, not Adobe SVG)
			           escape(input).replace(/%u20AC/g, "%80")];
			
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
