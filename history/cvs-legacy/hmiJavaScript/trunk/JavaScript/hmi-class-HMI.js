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
*	hmi-class-HMI.js
*
*	Editors:
*	--------
*	St							Stefan Schmitz <StefanS@plt.rwth-aachen.de>
*
*	CVS:
*	----
*	$Revision: 1.9 $
*	$Date: 2008-05-05 09:18:38 $
*
*	History:
*	--------
*	21-June-2007			St
*		-	General Revision
*
*	22-November-2007		St
*		-	test console-existing on logging
*
***********************************************************************/

/***********************************************************************
	constructor
***********************************************************************/

function HMI(async, debug, error, warning, info, trace) {
	this.HMI_Constants = Object();
	this.HMI_Constants.NAMESPACE_SVG = "http://www.w3.org/2000/svg";
	this.HMI_Constants.NODE_NAME_CLONE = "HMI_CLONE";
	this.HMI_Constants.HMIMANAGER_PATH = "/TechUnits/HMIManager";
	
	this.debug = debug;
	this.error = error;
	this.warning = warning;
	this.info = info;
	this.trace = trace;
	
	this.PossServers = null;
	this.PossSheets = null;
	this.Playground = null;
	
	//var _currentDragger = null;
	
	this.KSClient = new HMIJavaScriptKSClient(async);
	this.RefreshTime = null;
};

/***********************************************************************
	prototype
***********************************************************************/

HMI.prototype = {
	/*********************************
		showServers
	*********************************/
	showServers: function (Host, RefreshTime, PossServers, PossSheets, Playground) {
		this.hmi_log_trace("HMI.prototype.showServers - Start");
		
		this.RefreshTime = RefreshTime;
		this.PossServers = PossServers;
		this.PossSheets = PossSheets;
		this.Playground = Playground
		
		deleteChilds(this.PossServers);
		deleteChilds(this.PossSheets);
		deleteChilds(this.Playground);
		deleteChilds(document.getElementById("ErrorOutput"));
		
		this.KSClient.init(Host + '/MANAGER', location.host);
		this.KSClient.getServers();
		
		this.hmi_log_trace("HMI.prototype.showServers - End");
	},
	
	/*********************************
		showSheets
	*********************************/
	showSheets: function (Server) {
		this.hmi_log_trace("HMI.prototype.showSheets - Start, requested Server: "+Server);
		
		var i = 0;
		
		deleteChilds(this.PossSheets);
		deleteChilds(this.Playground);
		deleteChilds(document.getElementById("ErrorOutput"));
		clearTimeout(HMI.RefreshTimeoutID);
		HMI.RefreshTimeoutID = null;
		
		if (Server == '- select server -')
			return;
		
		this.KSClient.init(this.KSClient.KSServer.substring(0, this.KSClient.KSServer.indexOf('/')) + '/' + Server, location.host);
		this.KSClient.getSheets();
		
		this.hmi_log_trace("HMI.prototype.showSheets - End");
	},
	
	/*********************************
		showSheet
	*********************************/
	showSheet: function (Sheet) {
		this.hmi_log_trace("HMI.prototype.showSheet - Start");
			
		deleteChilds(this.Playground);
		clearTimeout(HMI.RefreshTimeoutID);
		HMI.RefreshTimeoutID = null;
		
		if (Sheet == '- select sheet -')
			return;
		
		if (HMI.KSClient.TCLKSHandle != null)
		{
			HMI.Path = Sheet;
			this._getAndImportComponent(HMI.Path, HMI.Playground, true);
		};
		
		this.hmi_log_trace("HMI.prototype.showSheet - End");
	},
	
	/*********************************
		refreshSheet
	*********************************/
	refreshSheet: function () {
		this.hmi_log_trace("HMI.prototype.refreshSheet - Start");
		
		if (HMI.KSClient.TCLKSHandle != null)
			this._getAndImportComponent(HMI.Path, HMI.Playground, false);
		
		this.hmi_log_trace("HMI.prototype.refreshSheet - End");
	},
	
	/*********************************
		getComponent
	*********************************/
	getComponent: function (evt, cssclass) {
		this.hmi_log_trace("HMI.prototype.getComponent - Start");
		
		var Component = null;
		
		Component = evt.target;
		while (	Component != null
				&&	Component != document
				&&	HMI.instanceOf(Component, cssclass) == false)
		{
			Component = Component.ownerSVGElement;
			if (	Component != null
				&&	Component.id == HMI.HMI_Constants.NODE_NAME_CLONE)
			{
				Component = $(Component.getAttribute('clonedID'));
			};
		};
		
		this.hmi_log_trace("HMI.prototype.getComponent - End");
		
		return Component;
	},
	
	/*********************************
		switchGround
	*********************************/
	switchGround: function (evt, ground) {
		this.hmi_log_trace("HMI.prototype.switchGround - Start");
		
		if (this._currentDragger != null)
			this._currentDragger.switchGround(evt, ground);
		
		this.hmi_log_trace("HMI.prototype.switchGround - End");
	},
	
	/*********************************
		_getAndImportComponent
	*********************************/
	_getAndImportComponent: function (ComponentPath, ParentNode, Insert) {
		this.hmi_log_trace("HMI.prototype._getAndImportComponent - Start");
		
		if (Insert == true)
		{
			//	import and append
			//
			this.KSClient.sendRequest('getvar', 'GET',
				'{' + ComponentPath + '.GraphicDescription' + ' ' + ComponentPath + '.StyleDescription' + '}' + ' ' + '-output $::TKS::OP_VALUE',
				this._cbGetAndAddComponent);
		} else {			
			//	import and replace
			//
			this.KSClient.sendRequest('getvar', 'GET',
				'{' + ComponentPath + '.GraphicDescription' + ' ' + ComponentPath + '.StyleDescription' + '}' + ' ' + '-output $::TKS::OP_VALUE',
				this._cbGetAndReplaceComponent);
		};
		
		this.hmi_log_trace("HMI.prototype._getAndImportComponent - End");
		
		return;
	},
	
	/*********************************
		_cbGetAndAddComponent
	*********************************/	
	_cbGetAndAddComponent: function (Client, req) {
		HMI.hmi_log_trace("HMI.prototype._cbGetAndAddComponent - Start");
		
		var ComponentText = new Array(2);
		var Component;
		
		if (req.responseText != "")
		{
			ComponentText = HMI.KSClient.prepareComponentText(req.responseText);
			Component = HMI._importComponent(ComponentText);
			
			if (Component == null)
			{
				HMI.hmi_log_error("HMI.prototype._cbGetAndImportComponent: Could not import component");
				return;
			};
			var template = Component;
			Component = document.importNode(template, true);
			HMI.hmi_log_trace("HMI.prototype._cbGetAndImportComponent: now initGestures");
			HMI.initGestures(Component);
			HMI.hmi_log_trace("HMI.prototype._cbGetAndImportComponent: now Playground.appendChild");
			HMI.Playground.appendChild(Component);
			
			//	set TimeoutID
			//
			HMI.RefreshTimeoutID = setInterval('HMI.refreshSheet()', HMI.RefreshTime);
		};
		
		HMI.hmi_log_trace("HMI.prototype._cbGetAndAddComponent - End");
	},
	
	/*********************************
		_cbGetAndReplaceComponent
	*********************************/	
	_cbGetAndReplaceComponent: function (Client, req) {
		HMI.hmi_log_trace("HMI.prototype._cbGetAndReplaceComponent - Start");
		
		var ComponentText = new Array(2);
		var Component;
		
		if (req.responseText != "")
		{
			ComponentText = HMI.KSClient.prepareComponentText(req.responseText);
			Component = HMI._importComponent(ComponentText);
			
			if (Component == null)
			{
				HMI.hmi_log_error("HMI.prototype._cbGetAndReplaceComponent: Could not import component");
				clearTimeout(HMI.RefreshTimeoutID);
				HMI.RefreshTimeoutID = null;
				return;
			};
			
			var template = Component;
			Component = document.importNode(template, true);
			HMI.initGestures(Component);
			HMI.Playground.replaceChild(Component, HMI.Playground.firstChild);
			//HMI.Playground.replaceChild(Component, $(HMI.Path));
			
			template._xxx = null; delete template._xxx; delete template;
			Component._xxx = null; delete Component._xxx; delete Component;
			ComponentText._xxx = null; delete ComponentText._xxx; delete ComponentText;
		};
		
		HMI.hmi_log_trace("HMI.prototype._cbGetAndReplaceComponent - End");
	},
	
	/*********************************
		_importComponent
	*********************************/	
	_importComponent: function(ComponentText) {
		this.hmi_log_trace("HMI.prototype._importComponent - Start");

		var parsedComponentText = new HMIDOMParser().parse(ComponentText[0], ComponentText[1], null);

		this.hmi_log_trace("HMI.prototype._importComponent - End");
		return (parsedComponentText);
	},
	
	/*********************************
		_initGestures
	*********************************/
	_initGestures: function (Element) {
		this.hmi_log_trace("HMI.prototype._initGestures - Start");
		
		//	HMI-COMPONENT
		//
		if (this.instanceOf(Element, "hmi-component"))
		{
			Element.setAttribute('pointer-events', 'all');
			this._setLayerPosition(Element);
		};
		
		//	GROUND
		//
		if (this.instanceOf(Element, "hmi-component-ground"))
		{
			var ground = new Ground(Element, this);
		};
		
		//	MOVE
		//
		if (this.instanceOf(Element, "hmi-component-gesture-move"))
		{
			var dragger = new Dragger(Element, this);
		} else {			
			//	RIGHTCLICK
			//
			if (this.instanceOf(Element, 'hmi-component-gesture-rightclick'))
			{
				var rightclick = new RightClick(Element, this);
			};
			
			//	(LEFT)CLICK
			//
			if (this.instanceOf(Element, 'hmi-component-gesture-click'))
			{
				var click = new Click(Element, this);
			};
			
			//	DOUBLECLICK
			//
			if (this.instanceOf(Element, 'hmi-component-gesture-doubleclick'))
			{
				var doubleclick = new DoubleClick(Element, this);
			};
			
			//	TEXTINPUT
			//
			if (this.instanceOf(Element, 'hmi-component-gesture-textinput'))
			{
				var textinput = new TextInput(Element, this);
			};
		};
		
		this.hmi_log_trace("HMI.prototype._initGestures - End");
	},
	
	/*********************************
		initGestures
	*********************************/
	initGestures: function (Fragment) {
		this.hmi_log_trace("HMI.prototype.initGestures - Start");
		
		var Elements;
		
		HMI._initGestures(Fragment);
//		this.hmi_log_trace("HMI.prototype.initGestures - done _initGestures(Fragment) ");
		
		Elements = Fragment.getElementsByTagNameNS(HMI.HMI_Constants.NAMESPACE_SVG, 'svg');
		for (var idx = 0; idx < Elements.length; ++idx)
		{
			HMI.hmi_log_trace("HMI.initGestures - idx: "+ idx +" Element.length: "+ Elements.length);
			HMI._initGestures(Elements[idx]);
		};
		
		Elements._xxx = null; delete Elements._xxx; delete Elements;
		
		this.hmi_log_trace("HMI.prototype.initGestures - End");
	},
	
	/*********************************
		instanceOf
	*********************************/
	instanceOf: function (Node, ClassName) {
		this.hmi_log_trace("HMI.prototype.instanceOf - Start");
		
		if (Node.getAttribute("class") != null)
		{
			//	Space Delimiter
			//
			if (this._instanceOf(Node, ClassName, " ") == true)
			{
				this.hmi_log_trace("HMI.prototype.instanceOf - Endt1");
				return true;
			} else {
				// COMMA delimeter
				//
				if (this._instanceOf(Node, ClassName, ",") == true)
				{
					this.hmi_log_trace("HMI.prototype.instanceOf - Endt2");
					return true;
				} else {
					// SEMICOLON delimeter
					//
					if (this._instanceOf(Node, ClassName, ";") == true)
					{
						this.hmi_log_trace("HMI.prototype.instanceOf - Endt3");
						return true;
					};
				};
			};
		};
		
		this.hmi_log_trace("HMI.prototype.instanceOf - Endf");
	
		return false;
	},
	
	/*********************************
		_instanceOf
	*********************************/
	_instanceOf: function (Node, ClassName, Delimiter) {
		this.hmi_log_trace("HMI.prototype._instanceOf - Start");
		
		var Classes;
		var idx;
		
		Classes = Node.getAttribute("class").split(Delimiter);
		for (idx = 0; idx < Classes.length; idx++)
		{
			if (ClassName == Classes[idx])
			{
				this.hmi_log_trace("HMI.prototype._instanceOf - Endt");
				
				return true;
			};
		};
		
		Classes._xxx = null; delete Classes._xxx; delete Classes;
		idx._xxx = null; delete idx._xxx; delete idx;
		
		this.hmi_log_trace("HMI.prototype._instanceOf - Endf");
		
		return false		
	},
	
	/*********************************
		_setLayerPosition
	*********************************/
	_setLayerPosition: function (Element) {
		this.hmi_log_trace("HMI.prototype._setLayerPosition - Start");
		
		var LayerX = Element.x.animVal.value;
		var LayerY = Element.y.animVal.value;
		
		if (	Element.ownerSVGElement != null
			&&	Element.ownerSVGElement != document)
		{
			LayerX += parseInt(Element.ownerSVGElement.getAttribute("layerX"));
			LayerY += parseInt(Element.ownerSVGElement.getAttribute("layerY"));
		};
		
		Element.setAttribute("layerX", LayerX);
		Element.setAttribute("layerY", LayerY);
		
		LayerX._xxx = null; delete LayerX._xxx; delete LayerX;
		LayerY._xxx = null; delete LayerY._xxx; delete LayerY;
		
		this.hmi_log_trace("HMI.prototype._setLayerPosition - End");
	},	
	
	/*********************************
		unload
	*********************************/
	unload: function () {
		this.hmi_log_trace("HMI.prototype.unload - Start");
		
		HMI.PossServers = null;
		HMI.PossSheets = null;
		
		if (HMI.KSClient.TCLKSHandle != null)
			HMI.KSClient.destroy();
			
		this.hmi_log_trace("HMI.prototype.unload - End");
	},
	
   /********************************************************************
		logging
			requires console of firebug or Opera
   ********************************************************************/
	
	/*********************************
		hmi_log_debug
	*********************************/
	hmi_log_debug: function (text) {
		if (window.console != null && this.debug == true){
			window.console.debug("HMI_DEBUG: %s", text);
		}else if(window.opera){
			if (opera.postError != null && this.debug == true){
				opera.postError("HMI_DEBUG: "+ text);
			}
		}
	},
	
	/*********************************
		hmi_log_error
	*********************************/
	hmi_log_error: function (text) {
		if (window.console != null && this.error == true){
			window.console.error("HMI_ERROR: %s", text);
		}else if(window.opera){
			if (opera.postError != null && this.error == true){
				opera.postError("HMI_ERROR: "+ text);
			}
		}
	},
	
	/*********************************
		hmi_log_warning
	*********************************/
	hmi_log_warning: function (text) {
		if (window.console != null && this.warning == true){
			window.console.warn("HMI_WARNING: %s", text);
		}else if(window.opera){
			if (opera.postError != null && this.warning == true){
				opera.postError("HMI_WARNING: "+ text);
			}
		}
	},
	
	/*********************************
		hmi_log_info
	*********************************/
	hmi_log_info: function (text) {
		if (window.console != null && this.info == true){
			window.console.info("HMI_INFO: %s", text);
		}else if(window.opera){
			if (opera.postError != null && this.info == true){
				opera.postError("HMI_INFO: "+ text);
			}
		}
	},
	
	/*********************************
		hmi_log_trace
	*********************************/
	hmi_log_trace: function (text) {
		if (window.console != null && this.trace == true){
			window.console.debug("HMI_TRACE: %s", text);
		}else if(window.opera){
			if (opera.postError != null && this.trace == true){
				opera.postError("HMI_TRACE: "+ text);
			}
		}
	}
};