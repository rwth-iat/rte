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
*	$Revision: 1.22 $
*	$Date: 2009-03-23 09:30:50 $
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
		for (var idx = 0; idx < Component.childNodes.length; ++idx){
			if (Component.childNodes.item(idx).nodeName == "svg:text" && !/\S+/.exec(Component.childNodes.item(idx).firstChild.nodeValue)){
				//build transparent dummy rect to allow input of new text
				var dummyRect = HMI.svgDocument.createElementNS('http://www.w3.org/2000/svg', 'rect');
				dummyRect.setAttributeNS(null, 'x', Component.childNodes.item(idx).getAttribute("x"));
				dummyRect.setAttributeNS(null, 'y', Component.childNodes.item(idx).getAttribute("y")-15);
				dummyRect.setAttributeNS(null, 'width', '15');
				dummyRect.setAttributeNS(null, 'height', '20');
				dummyRect.setAttributeNS(null, 'style', 'opacity:0;');
				dummyRect.setAttributeNS(null, 'class', 'dummyTextinputRect');
				Component.appendChild(dummyRect);
				delete dummyRect;
			}
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
			HMI.RefreshTimeoutID = setInterval(function () {HMI.refreshSheet();}, HMI.RefreshTime);
		}
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
		if (evt.target && evt.target.textContent){
			//TextContent is the summ of Text
			var text = evt.target.textContent;
		}else if (evt.target && evt.target.firstChild.nodeValue){  
			//Adobe Plugin
			var text = evt.target.firstChild.nodeValue;
		}else if (evt.target && evt.target.getAttribute("class") == "dummyTextinputRect"){
			//the TextInput was empty, so the user clicked on the dummyRect
			var text = "";
		}else if (evt.srcElement && evt.srcElement.firstChild.nodeValue){  //Native IE Code
			//native IE eventhandling
			var text = evt.srcElement.firstChild.nodeValue;
		}else if (evt.srcElement && evt.srcElement.getAttribute("class") == "dummyTextinputRect"){
			//native IE eventhandling
			var text = "";
		}else{
			var text = "";
			HMI.hmi_log_error('HMI.textinput.onclick Error: old Textinfo not found.');
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
			Command = '{' + HMI.KSClient.getMessageID() + '}%20' +
				'{010}%20' +
				'{' + Component.getAttribute('id') + '}%20' + 
				'{TEXTINPUT}%20' +
				//urlencode the UTF8 String in ISO-8859-1 Format
				//change the %u20AC of the &euro; Symbol to windows-1252 (accepted by all Browsers, not Adobe SVG)
				'{' + escape(input).replace(/%u20AC/g, "%80") + '}';
			HMI.KSClient.setVar(null, HMI.KSClient.HMIMANAGER_PATH + '.Command', Command, HMI.cbrefreshSheet);
		};
		
		delete Command;
	}
};
var filedate = "$Date: 2009-03-23 09:30:50 $";
filedate = filedate.substring(7, filedate.length-2);
if ("undefined" == typeof HMIdate){
	HMIdate = filedate;
}else if (HMIdate < filedate){
	HMIdate = filedate;
}
