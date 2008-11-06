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
*	hmi-class-HMI.js
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
*	$Revision: 1.48 $
*	$Date: 2008-11-06 14:55:35 $
*
*	History:
*	--------
*	21-June-2007			St
*		-	General Revision
*
*	22-November-2007		St
*		-	test console-existing on logging
*	31-August-2008			NH
*		-	Sheet refresh after click
*
***********************************************************************/

/***********************************************************************
	constructor
***********************************************************************/

function HMI(async, debug, error, warning, info, trace) {
	this.HMI_Constants = Object();
	this.HMI_Constants.NAMESPACE_SVG = "http://www.w3.org/2000/svg";
	this.HMI_Constants.NODE_NAME_CLONE = "HMI_CLONE";
	
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
		init
	*********************************/
	init: function () {
		this.hmi_log_trace("HMI.prototype.init - Start");
		
		//Window-Reload does not reset the checkbox, so we update the javascript variable
		if (document.getElementById("checkbox_keepheader").checked == true) {
			autoKeepHeader = true;
		}else{
			autoKeepHeader = false;
		}
		document.getElementById("idShowServers").disabled = false;
		document.getElementById("idShowServers").focus();
		
		if (window.location.search.length != 0){
			var HMI_Parameter_Liste = new Array();
			var wertestring = unescape(window.location.search);
			wertestring = wertestring.slice(1);
			var paare = wertestring.split("&");
			for (var i=0; i < paare.length; i++) {
				var name = paare[i].substring(0, paare[i].indexOf("="));
				var wert = paare[i].substring(paare[i].indexOf("=")+1, paare[i].length);
				HMI_Parameter_Liste[name] = wert;
			}
			
			if (HMI_Parameter_Liste.Host && HMI_Parameter_Liste.Host.length != 0){
				$('idHost').value = HMI_Parameter_Liste.Host;
			}
			if (HMI_Parameter_Liste.RefreshTime && HMI_Parameter_Liste.RefreshTime.length != 0){
				$('idRefreshTime').value = HMI_Parameter_Liste.RefreshTime;
			}
			if (HMI_Parameter_Liste.Server && HMI_Parameter_Liste.Server.length != 0){
				HMI.showServers($('idHost').value, $('idRefreshTime').value, $('idServers'), $('idSheets'), $('idPlayground'));
				for (var i=0; i < HMI.PossServers.options.length; i++){
					if (HMI.PossServers.options[i].value == HMI_Parameter_Liste.Server){
						HMI.PossServers.options[i].selected = true;
					}
				}
				HMI.showSheets(HMI_Parameter_Liste.Server);
			}
			if (HMI_Parameter_Liste.Sheet && HMI_Parameter_Liste.Sheet.length != 0){
				if ($('idSheets').options[$('idSheets').selectedIndex].value != HMI_Parameter_Liste.Sheet){
					HMI.showSheet(HMI_Parameter_Liste.Sheet);
				}
			}
		}
		this.hmi_log_trace("HMI.prototype.init - End");
	},
	/*********************************
		showServers
	*********************************/
	showServers: function (Host, RefreshTime, PossServers, PossSheets, Playground) {
		
		if (window.loadFirebugConsole){
			window.loadFirebugConsole();
		}
		
		this.hmi_log_trace("HMI.prototype.showServers - Start");
		
		if (RefreshTime < 100){
			$('idRefreshTime').value = 100;
			this.RefreshTime = 100;
		}else{
			this.RefreshTime = RefreshTime;
		}
		clearTimeout(HMI.RefreshTimeoutID);
		HMI.RefreshTimeoutID = null;
		this.PossServers = PossServers;
		this.PossSheets = PossSheets;
		if (Playground.tagName.toLowerCase() == "embed"){
			this.svgWindow = Playground.getWindow();
			this.svgDocument = Playground.getSVGDocument();
			this.Playground=HMI.svgDocument.getElementById("svgcontainer");
			this.PlaygroundEmbedNode= Playground;
			this.EmbedAdobePlugin= true;
		}else{
			this.Playground = Playground;
			this.EmbedAdobePlugin= false;
			this.svgDocument = document;
		}
		KSGateway = window.location.host;
		
		if (false){
			//tks.php is always in the same subdir as the html files
			KSGateway_Path = window.location.pathname.substring(0, window.location.pathname.lastIndexOf("/")+1)+ "tks.php";
			this.GatewayTypeTCL = false;
			this.GatewayTypePHP = true;
		}else{
			KSGateway_Path = "/tks";
			this.GatewayTypeTCL = true;
			this.GatewayTypePHP = false;
		}
		
		deleteChilds(this.PossServers);
		deleteChilds(this.PossSheets);
		deleteChilds(this.Playground);
		deleteChilds(document.getElementById("ErrorOutput"));
		
		this.KSClient.init(Host + '/MANAGER', KSGateway + KSGateway_Path);
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
		$("idBookmark").style.cssText = "display:none;";
		clearTimeout(HMI.RefreshTimeoutID);
		HMI.RefreshTimeoutID = null;
		
		if (Server == 'no server'){
			return;
		}
		
		this.KSClient.init(this.KSClient.KSServer.substring(0, this.KSClient.KSServer.indexOf('/')) + '/' + Server, this.KSClient.TCLKSGateway);
		this.KSClient.getSheets();
		
		this.hmi_log_trace("HMI.prototype.showSheets - End");
	},
	
	/*********************************
		showSheet
	*********************************/
	showSheet: function (Sheet) {
		this.hmi_log_trace("HMI.prototype.showSheet - Start with Sheet: "+Sheet);
			
		deleteChilds(this.Playground);
		$("idBookmark").style.cssText = "display:none;";
		clearTimeout(HMI.RefreshTimeoutID);
		HMI.RefreshTimeoutID = null;
		
		if (Sheet == 'no sheet')
			return;
		
		if (HMI.KSClient.TCLKSHandle != null)
		{
			HMI.Path = Sheet;
			this._getAndImportComponent(HMI.Path, HMI.Playground, true);
		};
		document.title = "//"+this.KSClient.KSServer+Sheet+" - ACPLT/HMI";
		if (autoKeepHeader == false && document.getElementById("ErrorOutput").innerHTML.length == 0){
			hideHeader();
		}
		$("idSheets").blur();
		$("idServers").blur();
		$("idBookmark").style.cssText = "display:inline;";
		$("idBookmark").setAttribute("href", "http://"+window.location.host+window.location.pathname.substring(0, window.location.pathname.lastIndexOf("/")+1)+"?Host="+$('idHost').value+"&RefreshTime="+HMI.RefreshTime+"&Server="+this.KSClient.KSServer.replace(/localhost\//, "")+"&Sheet="+HMI.PossSheets.value);
		
		this.hmi_log_trace("HMI.prototype.showSheet - End");
	},
	
	/*********************************
		refreshSheet
	*********************************/
	refreshSheet: function () {
		this.hmi_log_trace("HMI.prototype.refreshSheet - Start");
		
		if (HMI.KSClient.TCLKSHandle != null)
			this._getAndImportComponent(HMI.Path, HMI.Playground, false);
		
		//reload scroll setting
		//wheelsupport is not supported by the HMI Team and probably firefox only
		if(this.scrollComponent)
		{
			if(document.getElementById(this.scrollComponent) != null){ //opera, ff
				var Component = document.getElementById(this.scrollComponent);
			} else if(this.Playground.getElementById(this.scrollComponent) != null){ //ie
				var Component = this.Playground.getElementById(this.scrollComponent);
			}
			Component.setAttribute("y", this.currY);
			Component.setAttribute("x", this.currX);
		}
		this.hmi_log_trace("HMI.prototype.refreshSheet - End");
	},
	
	/*********************************
		cbrefreshSheet
	*********************************/
	cbrefreshSheet: function (client, req) {
		HMI.hmi_log_trace("HMI.prototype.cbrefreshSheet - Start");
		
		HMI.refreshSheet();
		
		HMI.hmi_log_trace("HMI.prototype.refreshSheet - End");
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
			if (Component.ownerSVGElement != undefined){
				Component = Component.ownerSVGElement;
			}else if (Component.parentNode.namespaceURI == HMI.HMI_Constants.NAMESPACE_SVG ){
				Component = Component.parentNode;
			}
			if ( Component != null && Component.id == HMI.HMI_Constants.NODE_NAME_CLONE){
				Component = HMI.svgDocument.getElementById(Component.getAttribute('clonedID'));
			}
		}
		
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
		
		if (Insert == true){
		//	import and append
		//
			//this.KSClient.send2Request(null, 'GET', 'getvar', 
			//	'{' + ComponentPath + '.GraphicDescription' + ' ' + ComponentPath + '.StyleDescription' + '}',
			//	'', ' -output $::TKS::OP_VALUE', 
			//	this._cbGetAndAddComponent);
			this.KSClient.getVar(null, '{' + ComponentPath + '.GraphicDescription' + ' ' + ComponentPath + '.StyleDescription' + '}', this._cbGetAndAddComponent);
		} else {			
			//	import and replace
			//
			//this.KSClient.send2Request(null, 'GET', 'getvar', 
			//	'{' + ComponentPath + '.GraphicDescription' + ' ' + ComponentPath + '.StyleDescription' + '}',
			//	'', ' -output $::TKS::OP_VALUE', 
			//	this._cbGetAndReplaceComponent);
			this.KSClient.getVar(null, '{' + ComponentPath + '.GraphicDescription' + ' ' + ComponentPath + '.StyleDescription' + '}', this._cbGetAndReplaceComponent);
		}
/*
		if (Insert == true){
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
		}
*/
		
		this.hmi_log_trace("HMI.prototype._getAndImportComponent - End");
		
		return;
	},
	
	/*********************************
		_cbGetAndShowComponent
	*********************************/	
	_cbGetAndShowComponent: function (Client, req, replace) {
		HMI.hmi_log_trace("HMI.prototype._cbGetAndShowComponent - Start");
		
		var ComponentText = new Array(2);
		var Component;
		
		if (req.responseText != "")
		{
			ComponentText = HMI.KSClient.prepareComponentText(req.responseText);
			if (ComponentText == null){
				//logging not required, allready done by prepareComponentText
				return;
			}
			
			Component = HMI._importComponent(ComponentText);
			if (Component == null){
				//logging not required, allready done by _importComponent
				return;
			}
			if(!HMI.EmbedAdobePlugin){
				var template = Component;
				Component = document.importNode(template, true);
			}
			HMI.hmi_log_trace("HMI.prototype._cbGetAndShowComponent: now initGestures");
			HMI.initGestures(Component);
			HMI.hmi_log_trace("HMI.prototype._cbGetAndShowComponent: now Playground.append/replaceChild");
			if(replace == true){
				HMI.Playground.replaceChild(Component, HMI.Playground.firstChild);
			}else{
				HMI.Playground.appendChild(Component);
				
				//	set TimeoutID
				HMI.RefreshTimeoutID = setInterval('HMI.refreshSheet()', HMI.RefreshTime);
			}
			try{
				/**
				* Gecko does not garbage collect things correct in any cases.
				* The hack here is to reassign the additional properties attached to the
				* JS wrapper object in order to ensure it becomes dirty. Well,
				* considering that it becomes dirty from getting it from itself ...
				* I think this source code can't be exported to the US anymore
				* because of undecent language and probably thoughts.
				*/
				template._xxx = null; delete template._xxx;
				Component._xxx = null; delete Component._xxx;
				ComponentText._xxx = null; delete ComponentText._xxx;
			} catch (e) {   //IE does not like this hack
			}
			delete template;
			delete Component;
			delete ComponentText;
		};
		
		HMI.hmi_log_trace("HMI.prototype._cbGetAndShowComponent - End");
	},
	/*********************************
		_cbGetAndAddComponent
	*********************************/	
	_cbGetAndAddComponent: function (Client, req) {
		HMI.hmi_log_trace("HMI.prototype._cbGetAndAddComponent - Start");
		
		HMI._cbGetAndShowComponent(Client, req, false);
		
		HMI.hmi_log_trace("HMI.prototype._cbGetAndAddComponent - End");
	},
	
	/*********************************
		_cbGetAndReplaceComponent
	*********************************/	
	_cbGetAndReplaceComponent: function (Client, req) {
		HMI.hmi_log_trace("HMI.prototype._cbGetAndReplaceComponent - Start");
		
		HMI._cbGetAndShowComponent(Client, req, true);
		
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
		this.hmi_log_trace("HMI.prototype._initGestures - Start: "+Element.id);
		
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
			if (!HMI.EmbedAdobePlugin)
			{
				var dragger = new Dragger(Element, this);
			}else{
				//FIXME Adobe move gesture buggy
				HMI.hmi_log_onwebsite('Move-Gesture disabled!');
			}
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
			
			//	WHEEL-SCROLL
			//
			//wheelsupport is not supported by the HMI Team and probably firefox only
			if (this.instanceOf(Element, 'hmi-component-gesture-wheelscroll'))
			{
				this.wheelscroll = new WheelScroll(Element, this); //**diff
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
		
		if (HMI.EmbedAdobePlugin){
			//getElementsByTagNameNS in Adobe is often not complete
			for (var idx = 0; idx < Fragment.childNodes.length; ++idx){
				if (Fragment.childNodes.item(idx).namespaceURI == HMI.HMI_Constants.NAMESPACE_SVG && Fragment.childNodes.item(idx).id != ""){
					HMI.initGestures(Fragment.childNodes.item(idx)); //**diff NH recurse!
				}
			}
		}else if (Fragment.getElementsByTagNameNS){   //gecko
			Elements = Fragment.getElementsByTagNameNS(HMI.HMI_Constants.NAMESPACE_SVG, 'svg');
			for (var idx = 0; idx < Elements.length; ++idx){
				HMI.hmi_log_trace("HMI.initGestures - idx: "+ idx +" Element.length: "+ Elements.length);
				HMI._initGestures(Elements[idx]);
			}
		}else{   // IE if svg inline
			Elements = Fragment.getElementsByTagName('svg');
			for (var idx = 0; idx < Elements.length; ++idx){
				HMI.hmi_log_trace("HMI.initGestures - idx: "+ idx +" Element.length: "+ Elements.length);
				if (Elements[idx].tagUrn == HMI.HMI_Constants.NAMESPACE_SVG){
					HMI._initGestures(Elements[idx]);
				}
			}
		}
		
		try{
			/**
			* Gecko does not garbage collect things correct in any cases.
			* The hack here is to reassign the additional properties attached to the
			* JS wrapper object in order to ensure it becomes dirty. Well,
			* considering that it becomes dirty from getting it from itself ...
			* I think this source code can't be exported to the US anymore
			* because of undecent language and probably thoughts.
			*/
			Elements._xxx = null; delete Elements._xxx;
		} catch (e) {   //IE does not like this hack
		}
		delete Elements;
		
		this.hmi_log_trace("HMI.prototype.initGestures - End");
	},
	
	/*********************************
		instanceOf
	*********************************/
	instanceOf: function (Node, ClassName) {
		//logging deactivated, causes too much noise and performanceproblem in a production system
//		this.hmi_log_trace("HMI.prototype.instanceOf - Start");
		
		if (Node.getAttribute("class") != null)
		{
			//	Space Delimiter
			//
			if (this._instanceOf(Node, ClassName, " ") == true)
			{
//				this.hmi_log_trace("HMI.prototype.instanceOf - Endt1");
				return true;
			} else {
				// COMMA delimeter
				//
				if (this._instanceOf(Node, ClassName, ",") == true)
				{
//					this.hmi_log_trace("HMI.prototype.instanceOf - Endt2");
					return true;
				} else {
					// SEMICOLON delimeter
					//
					if (this._instanceOf(Node, ClassName, ";") == true)
					{
//						this.hmi_log_trace("HMI.prototype.instanceOf - Endt3");
						return true;
					};
				};
			};
		};
		
//		this.hmi_log_trace("HMI.prototype.instanceOf - Endf");
	
		return false;
	},
	
	/*********************************
		_instanceOf
	*********************************/
	_instanceOf: function (Node, ClassName, Delimiter) {
//		this.hmi_log_trace("HMI.prototype._instanceOf - Start");
		
		var Classes;
		var idx;
		
		Classes = Node.getAttribute("class").split(Delimiter);
		for (idx = 0; idx < Classes.length; idx++)
		{
			if (ClassName == Classes[idx])
			{
//				this.hmi_log_trace("HMI.prototype._instanceOf - Endt");
				
				return true;
			};
		};
		
		try{
			/**
			* Gecko does not garbage collect things correct in any cases.
			* The hack here is to reassign the additional properties attached to the
			* JS wrapper object in order to ensure it becomes dirty. Well,
			* considering that it becomes dirty from getting it from itself ...
			* I think this source code can't be exported to the US anymore
			* because of undecent language and probably thoughts.
			*/
			Classes._xxx = null; delete Classes._xxx;
			idx._xxx = null; delete idx._xxx;
		} catch (e) {   //IE does not like this hack
		}
		delete Classes;
		delete idx;
		
//		this.hmi_log_trace("HMI.prototype._instanceOf - Endf");
		
		return false		
	},
	
	/*********************************
		_setLayerPosition
	*********************************/
	_setLayerPosition: function (Element) {
		this.hmi_log_trace("HMI.prototype._setLayerPosition - Start");
		
		if (Element.x && Element.x.animVal){  //gecko
			var LayerX = Element.x.animVal.value;
			var LayerY = Element.y.animVal.value;
		}else if (Element.x){  //ie adobe inline
			var LayerX = Element.x;
			var LayerY = Element.y;
		}else{   //ie adobe embed
			var LayerX = Element.getAttribute("x");
			var LayerY = Element.getAttribute("y");
		}
		
		if ( Element.ownerSVGElement != null && Element.ownerSVGElement != document){
			LayerX += parseInt(Element.ownerSVGElement.getAttribute("layerX"));
			LayerY += parseInt(Element.ownerSVGElement.getAttribute("layerY"));
		}else if (Element.parentNode != null && Element.parentNode.attributes != null){
			LayerX += parseInt(Element.parentNode.getAttribute("layerX"));
			LayerY += parseInt(Element.parentNode.getAttribute("layerY"));
		}
		
		Element.setAttribute("layerX", LayerX);
		Element.setAttribute("layerY", LayerY);
		
		try{
			/**
			* Gecko does not garbage collect things correct in any cases.
			* The hack here is to reassign the additional properties attached to the
			* JS wrapper object in order to ensure it becomes dirty. Well,
			* considering that it becomes dirty from getting it from itself ...
			* I think this source code can't be exported to the US anymore
			* because of undecent language and probably thoughts.
			*/
			LayerX._xxx = null; delete LayerX._xxx;
			LayerY._xxx = null; delete LayerY._xxx;
		} catch (e) {   //IE does not like this hack
		}
		delete LayerX;
		delete LayerY;
		
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
		if (this.trace == true && window.console != null){
			window.console.debug("HMI_TRACE: %s", text);
		}else if(window.opera){
			if (opera.postError != null && this.trace == true){
				opera.postError("HMI_TRACE: "+ text);
			}
		}
	},
	
	/*********************************
		hmi_log_onwebsite
	*********************************/
	hmi_log_onwebsite: function (text) {
		var ErrorTextNode = document.createTextNode(text);
		deleteChilds(document.getElementById("ErrorOutput"));
		document.getElementById("ErrorOutput").appendChild(ErrorTextNode);
		//if header in not visible: show it
		if (showHeader == false){
			hideHeader();
		}

	}
};