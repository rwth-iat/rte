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
*	$Revision: 1.81 $
*	$Date: 2009-03-18 15:53:53 $
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
*	25-February-2009			Je
*		-	General Revision and full commented
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
	//log critical errors to console
	this.error = error;
	this.warning = warning;
	this.info = info;
	//log trace info to console
	this.trace = trace;
	
	this.PossServers = null;
	this.PossSheets = null;
	this.Playground = null;
	this.ErrorOutput = null;
	
	this.KSClient = new HMIJavaScriptKSClient(async);
	this.RefreshTime = null;
	
	this.showHeader = true;
	
	//the modern firebugconsole wants to be activated
	if (window.loadFirebugConsole){
		window.loadFirebugConsole();
	}
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
		
		var ErrorDetail = "";
		//Object of ShowServer-Button
		if (this.ButShowServers = $('idShowServers')){
			addEventSimple(HMI.ButShowServers,'click',function(){HMI.showServers();});
		}else{
			ErrorDetail += "HTML Button with the ID: idShowServers not found.\n";
		}
		//Object of ErrorOutput
		if (!(this.ErrorOutput = $('idErrorOutput'))){
			ErrorDetail += "HTML Tag with the ID: idErrorOutput not found.\n";
		}
		//Object of Server-Selectbox
		if (!(this.PossServers = $('idServers'))){
			ErrorDetail += "HTML Select with the ID: idShowServers not found.\n";
		}
		//Object of Sheet-Selectbox
		if (!(this.PossSheets = $('idSheets'))){
			ErrorDetail += "HTML Select with the ID: idShowServers not found.\n";
		}
		//Object of SVG insertion (p for firefox, embed for IE)
		if (!(this.Playground = $('idPlayground'))){
			ErrorDetail += "HTML Container-Element with the ID: idPlayground not found.\n";
		}
		//Object of RefreshTime
		if (!($('idRefreshTime'))){
			ErrorDetail += "HTML Input with the ID: idRefreshTime not found.\n";
		}
		//Object of Server-Hostname
		if (!($('idHost'))){
			ErrorDetail += "HTML Input with the ID: idHost not found.\n";
		}
		//Object of the Deep Link
		if (!($('idBookmark'))){
			ErrorDetail += "HTML Input with the ID: idBookmark not found.\n";
		}
		//Object of the hideable header
		if (!($('idHideableHeader'))){
			ErrorDetail += "HTML Div with the ID: idHideableHeader not found.\n";
		}
		
		//init the plain HTML website with events
		if ($('idHeaderRow')){
			addEventSimple($('idHeaderRow'),'click',function(){HMI.hideHeader();});
		}else{
			ErrorDetail += "HTML object with the ID: idHeaderRow not found.\n";
		}
		if ($('idKeepHeader')){
			addEventSimple($('idKeepHeader'),'click',function(){HMI.updateKeepHeader();});
		}else{
			ErrorDetail += "HTML Checkbox with the ID: idKeepHeader not found.\n";
		}
		
		if(ErrorDetail != ""){
			alert ("Error initialising HMI Website:\n"+ErrorDetail);
			return;
		}
		
		//init the optional Buttons
		if ($('idRefresh')){
			addEventSimple($('idRefresh'),'click',function(){HMI.refreshSheet();});
		}
		if ($('idStopRefresh')){
			addEventSimple($('idStopRefresh'),'click',function(){
				clearTimeout(HMI.RefreshTimeoutID);
				HMI.RefreshTimeoutID = null;
				}
			);
		}
		if ($('idStartRefresh')){
			addEventSimple($('idStartRefresh'),'click',function(){HMI.RefreshTimeoutID = setInterval('HMI.refreshSheet()', $('idRefreshTime').value)});
		}
		
		//Try to detect the Servertype (TCL or PHP capable)
		//make a request to sniff the HTTP-Serverstring
		//This fails in some cases, with Safari and Chrome, try manually to reload the page in that case
		
		//IE sometimes uses a cached version, without server Header, so prevent caching
		var DatePreventsCaching = new Date();
		var req = new XMLHttpRequest();
		req.open("GET", window.location.pathname+'?preventCaching='+DatePreventsCaching.getTime(), false);
		req.send(null);
		var ResponseServerString = req.getResponseHeader('server');
		if (-1 != ResponseServerString.indexOf('Tcl-Webserver')){
			HMI.HMI_Constants.ServerType = "tcl";
			this.hmi_log_trace("HMI.prototype.init - detected TCL Gateway");
		}else if (-1 != ResponseServerString.indexOf('PHP')){
			HMI.HMI_Constants.ServerType = "php";
			this.hmi_log_trace("HMI.prototype.init - detected PHP Gateway");
		}
		delete req;
		delete ResponseServerString;
		delete DatePreventsCaching;
		
		//detect type of SVG display
		if (this.Playground.tagName.toLowerCase() == "embed"){
			//via a embed plugin tag
			this.svgWindow = this.Playground.window;
			this.svgDocument = this.Playground.getSVGDocument();
			//switch the target Playground to the embed SVG-Plugin DOM
			this.PlaygroundEmbedNode= this.Playground;
			this.Playground=HMI.svgDocument.getElementById("svgcontainer");
			//sometimes feature detection is not possible
			this.EmbedAdobePlugin= true;
			this.PluginVendor = HMI.svgWindow.getSVGViewerVersion();
			if (-1 != this.PluginVendor.indexOf('Adobe')){
				this.PluginVendor = 'Adobe';
			}else if (-1 != this.PluginVendor.indexOf('examotion')){
				this.PluginVendor = 'Examotion';
			}
		}else{
			//inline SVG in XHTML DOM
			this.EmbedAdobePlugin= false;
			this.svgDocument = document;
			this.svgWindow = window;
		}
		
		//The state of the Checkbox is preserved at a reload from cache, so
		//we have to update the variable to reflect the userchoice
		HMI.updateKeepHeader();
		
		//deactivate the Select-Boxes, because there is no usefull content now
		HMI.PossServers.disabled = true;
		HMI.PossSheets.disabled = true;
		
		//reactivate the ShowServer button. It was disabled to prevent a click during initialisation of HMI
		HMI.ButShowServers.disabled = false;
		
		//focus the ShowServer button for convenience with keyboard interaction
		HMI.ButShowServers.focus();
		
		//HMIdate was populated in every js-file with the date of CVS commit
		//publish this date on website
		HMI.HMI_Constants.HMIdate = HMIdate;
		var dateTextNode = document.createTextNode("Version: 2.1 ("+HMI.HMI_Constants.HMIdate.substr(0, 10).replace(/\//g, "-")+")");
		var titlenode = document.createAttribute("title");
		titlenode.nodeValue = "last changed: "+HMI.HMI_Constants.HMIdate+" UTC";
		
		if (document.getElementById("idDateOutput")){
			document.getElementById("idDateOutput").appendChild(dateTextNode);
			document.getElementById("idDateOutput").parentNode.setAttributeNode(titlenode);
		}
		delete HMIdate;
		delete dateTextNode;
		delete titlenode;
		
		//jump to a "deep link" of a sheet
		if (window.location.search.length != 0){
			//collect parameter given by the deep link
			var HMI_Parameter_Liste = new Array();
			var wertestring = unescape(window.location.search);
			wertestring = wertestring.slice(1);
			var paare = wertestring.split("&");
			for (var i=0; i < paare.length; i++) {
				var name = paare[i].substring(0, paare[i].indexOf("="));
				var wert = paare[i].substring(paare[i].indexOf("=")+1, paare[i].length);
				HMI_Parameter_Liste[name] = wert;
			}
			
			//correct host in website with user wish
			if (HMI_Parameter_Liste.Host && HMI_Parameter_Liste.Host.length != 0){
				$('idHost').value = HMI_Parameter_Liste.Host;
			}
			//correct RefreshTime in website with user wish
			if (HMI_Parameter_Liste.RefreshTime && HMI_Parameter_Liste.RefreshTime.length != 0){
				$('idRefreshTime').value = HMI_Parameter_Liste.RefreshTime;
			}
			//a server is specified in "deep link"
			if (HMI_Parameter_Liste.Server && HMI_Parameter_Liste.Server.length != 0){
				//get list of servers
				HMI.showServers();
				//select server in drop-down box from deep link
				for (var i=0; i < HMI.PossServers.options.length; i++){
					if (HMI.PossServers.options[i].value == HMI_Parameter_Liste.Server){
						HMI.PossServers.options[i].selected = true;
					}
				}
				//if showServers encountered an error don't load the Sheet list
				if (!HMI.ErrorOutput.firstChild && HMI.PossServers.selectedIndex != 0){
					HMI.showSheets(HMI_Parameter_Liste.Server);
				}
			}
			
			if (HMI.PossServers && HMI.PossServers.selectedIndex != 0 && HMI_Parameter_Liste.Sheet && HMI_Parameter_Liste.Sheet.length != 0 && HMI_Parameter_Liste.Sheet){
				//no error and more than one sheet. If there is only one Sheet, showSheets has allready shown Sheet
				if (!HMI.ErrorOutput.firstChild && HMI.PossSheets.options[HMI.PossSheets.selectedIndex].value != HMI_Parameter_Liste.Sheet){
					for (var i=0; i < HMI.PossSheets.options.length; i++){
						if (HMI.PossSheets.options[i].value == HMI_Parameter_Liste.Sheet){
							HMI.PossSheets.options[i].selected = true;
						}
					}
					HMI.showSheet(HMI_Parameter_Liste.Sheet);
				}
			}
		}
		this.hmi_log_trace("HMI.prototype.init - End");
	},
	
	/*********************************
		Functions - updateKeepHeader
		
		called with a onclick handler of the checkbox and in HMI.init
	*********************************/
	updateKeepHeader: function (){
		this.hmi_log_trace("HMI.prototype.updateKeepHeader - change requested");
		if (document.getElementById("idKeepHeader").checked == true) {
			HMI.autoKeepHeader = true;
		}else{
			HMI.autoKeepHeader = false;
		}
	},
	/*********************************
		Functions - hideHeader
	*********************************/
	hideHeader: function(){
		if (HMI.showHeader){
			//hide menu
			HMI.showHeader = false;
			document.getElementById("idHideableHeader").style.display = "none";
			if (document.getElementById("arrowdown1") != null){
				document.getElementById("arrowdown1").style.visibility="visible";
				document.getElementById("arrowdown2").style.visibility="visible";
				document.getElementById("arrowup1").style.visibility="hidden";
				document.getElementById("arrowup2").style.visibility="hidden";
			}
		} else {
			//show menu
			HMI.showHeader = true;
			document.getElementById("idHideableHeader").style.display = "block";
			if (document.getElementById("arrowdown1") != null){
				document.getElementById("arrowdown1").style.visibility="hidden";
				document.getElementById("arrowdown2").style.visibility="hidden";
				document.getElementById("arrowup1").style.visibility="visible";
				document.getElementById("arrowup2").style.visibility="visible";
			}
		}
	},
	
	/*********************************
		showServers
	*********************************/
	showServers: function () {
		
		this.hmi_log_trace("HMI.prototype.showServers - Start");
		
		//disable double click by user
		HMI.ButShowServers.disabled = true;
		HMI.ButShowServers.value = "Please wait...";
		
		
		if ($('idRefreshTime').value < 100){
			$('idRefreshTime').value = 100;
			this.RefreshTime = 100;
		}else{
			this.RefreshTime = $('idRefreshTime').value;
		}
		if ($('idHost').value.length == 0){
			$('idHost').value = "localhost";
		}
		clearTimeout(HMI.RefreshTimeoutID);
		HMI.RefreshTimeoutID = null;
		
		//the guessed servertype is used for communication
		if ("php" == HMI.HMI_Constants.ServerType){
			//tks.php is always in the same subdir as the html files
			KSGateway_Path = window.location.pathname.substring(0, window.location.pathname.lastIndexOf("/")+1)+ "tks.php";
			this.GatewayTypeTCL = false;
			this.GatewayTypePHP = true;
		}else if("tcl" == HMI.HMI_Constants.ServerType){
			//tcl gateway is hardcoded in the Gateway via TCL-HTTPD\tclhttpd3.5.1\custom\tkshttpserver.tcl
			KSGateway_Path = "/tks";
			this.GatewayTypeTCL = true;
			this.GatewayTypePHP = false;
		}else{
			HMI.hmi_log_onwebsite('Could not detect type of KSGateway. Please configure in hmi-class-HMI.js. Fallback to PHP.');
			alert('Could not detect type of KSGateway. Please configure in hmi-class-HMI.js. Fallback to PHP.');
			//tks.php is always in the same subdir as the html files
			KSGateway_Path = window.location.pathname.substring(0, window.location.pathname.lastIndexOf("/")+1)+ "tks.php";
			this.GatewayTypeTCL = false;
			this.GatewayTypePHP = true;
		}
		
		//clean old Server and Sheet entrys, an old SVG display and displayed errors in website
		deleteChilds(this.PossServers);
		deleteChilds(this.PossSheets);
		deleteChilds(this.Playground);
		deleteChilds(this.ErrorOutput);
		
		//deactivate the Select-Boxes, because there is no usefull content
		HMI.PossServers.disabled = true;
		HMI.PossSheets.disabled = true;
		
		//Set a neutral title
		document.title = "Startcenter - ACPLT/HMI";
		
		$("idBookmark").style.cssText = "display:none;";
		
		//Gateway could not be another host without violating Same Origin Policy
		KSGateway = window.location.host;
		
		//an init generates a new Handle, needed cause we communicate to the Manager the first time
		this.KSClient.init($('idHost').value + '/MANAGER', KSGateway + KSGateway_Path);
		if (this.KSClient.TCLKSHandle != null){
			this.KSClient.getServers();
		}
		//present a deep link to the Host setting
		$("idBookmark").style.cssText = "display:inline;";
		$("idBookmark").setAttribute("href", window.location.protocol+"//"+window.location.host+window.location.pathname.substring(0, window.location.pathname.lastIndexOf("/")+1)+"?Host="+$('idHost').value+"&RefreshTime="+HMI.RefreshTime);
		
		//reenable click by user
		HMI.ButShowServers.disabled = false;
		HMI.ButShowServers.value = "Show Servers";
		
		this.hmi_log_trace("HMI.prototype.showServers - End");
	},
	
	/*********************************
		showSheets
	*********************************/
	showSheets: function (Server) {
		this.hmi_log_trace("HMI.prototype.showSheets - Start, requested Server: "+Server);
		
		var i = 0;
		
		//clean old Sheet entrys, an old SVG display and displayed errors in website
		deleteChilds(this.PossSheets);
		deleteChilds(this.Playground);
		deleteChilds(this.ErrorOutput);
		
		HMI.PossSheets.disabled = true;
		$("idBookmark").style.cssText = "display:none;";
		document.title = "Startcenter - ACPLT/HMI";
		
		clearTimeout(HMI.RefreshTimeoutID);
		HMI.RefreshTimeoutID = null;
		
		//nothing selected
		if (Server == 'no server'){
			HMI.PossSheets.options[0] = new Option('please select Server first', 'no sheet');
			return;
		}
		
		//an init generates a new Handle, needed cause we communicate to this server the first time
		this.KSClient.init(this.KSClient.KSServer.substring(0, this.KSClient.KSServer.indexOf('/')) + '/' + Server, this.KSClient.TCLKSGateway);
		if (this.KSClient.TCLKSHandle != null){
			this.KSClient.getSheets();
		}
		
		this.hmi_log_trace("HMI.prototype.showSheets - End");
	},
	
	/*********************************
		showSheet
	*********************************/
	showSheet: function (Sheet) {
		this.hmi_log_trace("HMI.prototype.showSheet - Start with Sheet: "+Sheet);
		
		//clean an old SVG display and displayed errors in website
		deleteChilds(this.Playground);
		deleteChilds(this.ErrorOutput);
		$("idBookmark").style.cssText = "display:none;";
		clearTimeout(HMI.RefreshTimeoutID);
		HMI.RefreshTimeoutID = null;
		
		//nothing selected
		if (Sheet == 'no sheet'){
			return;
		}
		
		if (HMI.KSClient.TCLKSHandle != null)
		{
			HMI.Path = Sheet;
			this._getAndImportComponent(HMI.Path, HMI.Playground, true);
		};
		document.title = "//"+this.KSClient.KSServer+Sheet+" - ACPLT/HMI";
		if (HMI.autoKeepHeader == false && !HMI.ErrorOutput.firstChild){
			HMI.hideHeader();
		}
		
		//blur the buttons for convenience with keyboard interaction
		HMI.PossSheets.blur();
		HMI.PossServers.blur();
		//present a "deep link" to the sheet
		$("idBookmark").style.cssText = "display:inline;";
		$("idBookmark").setAttribute("href", window.location.protocol+"//"+window.location.host+window.location.pathname.substring(0, window.location.pathname.lastIndexOf("/")+1)+"?Host="+$('idHost').value+"&RefreshTime="+HMI.RefreshTime+"&Server="+this.KSClient.KSServer.substr(this.KSClient.KSServer.indexOf('/')+1)+"&Sheet="+HMI.PossSheets.value);
		
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
		this.hmi_log_trace("HMI.prototype.getComponent - Start - Target:"+evt.target.id);
		
		var Component = null;
		
		//get the Component itself which contained the triggered gesture
		if (evt.target){
			Component = evt.target;
		}else if(evt.srcElement){
			//native IE code
			Component = evt.srcElement;
		}
		while (	Component != null
				&&	Component != document
				&&	HMI.instanceOf(Component, cssclass) == false)
		{
			if (Component.ownerSVGElement != undefined){
				//firefox and co
				Component = Component.ownerSVGElement;
			}else if (Component.parentNode.namespaceURI == HMI.HMI_Constants.NAMESPACE_SVG ){
				//Adobe+Renesis
				Component = Component.parentNode;
			}else if (Component.parentNode.tagUrn == HMI.HMI_Constants.NAMESPACE_SVG ){
				//Inline IE Code inline
				Component = Component.parentNode;
			}
			if ( Component != null && Component.id == HMI.HMI_Constants.NODE_NAME_CLONE){
				//in the move-gesture the original element has to be discovered
				Component = HMI.svgDocument.getElementById(Component.getAttribute('clonedID'));
			}
		}
		
		this.hmi_log_trace("HMI.prototype.getComponent - End");
		
		return Component;
	},
	
	/*********************************
		switchGround
		
		global function for finding a new ground with a mouse move into an element
	*********************************/
	switchGround: function (evt, ground) {
		this.hmi_log_trace("HMI.prototype.switchGround - Start, Evt: "+evt.type+", Evt.id: "+evt.target.id+", Evt.nodeName: "+evt.target.nodeName+", Ground: "+ground._node.id);
		
		//is there a move gesture in action?
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
			this.KSClient.getVar(null, '{' + ComponentPath + '.GraphicDescription' + '%20' + ComponentPath + '.StyleDescription' + '}', this._cbGetAndAddComponent);
		} else {			
			//	import and replace
			//
			this.KSClient.getVar(null, '{' + ComponentPath + '.GraphicDescription' + '%20' + ComponentPath + '.StyleDescription' + '}', this._cbGetAndReplaceComponent);
		}
		
		this.hmi_log_trace("HMI.prototype._getAndImportComponent - End");
		
		return;
	},
	
	/*********************************
		_GetAndShowComponent
	*********************************/	
	_GetAndShowComponent: function (Client, req, replace) {
		HMI.hmi_log_trace("HMI.prototype._GetAndShowComponent - Start");
		
		//only one entry in array if no styledescription is requested
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
			HMI.hmi_log_trace("HMI.prototype._GetAndShowComponent: now initGestures");
			HMI.initGestures(Component);
			//Adobe does not fire mousemove event if there is no rect around the mouse. Build a invisible rect around everything 
			if (HMI.AdobeMoveFixNeeded && HMI.PluginVendor == 'Adobe'){
				var dummyRect = HMI.svgDocument.createElementNS('http://www.w3.org/2000/svg', 'rect');
				dummyRect.setAttributeNS(null, 'x', '0');
				dummyRect.setAttributeNS(null, 'y', '0');
				dummyRect.setAttributeNS(null, 'width', Component.getAttribute('width'));
				dummyRect.setAttributeNS(null, 'height', Component.getAttribute('height'));
				dummyRect.setAttributeNS(null, 'style', 'opacity:0;');
				Component.insertBefore(dummyRect, Component.firstChild);
				delete dummyRect;
				HMI.hmi_log_trace("HMI.prototype._GetAndShowComponent - Fix for Adobe mousemove Bug enabled.");
			}
			HMI.hmi_log_trace("HMI.prototype._GetAndShowComponent: now Playground.append/replaceChild");
			if(replace == true){
				HMI.Playground.replaceChild(Component, HMI.Playground.firstChild);
			}else{
				HMI.Playground.appendChild(Component);
				
				//	set TimeoutID
				if (HMI.RefreshTimeoutID == null){
					HMI.RefreshTimeoutID = setInterval('HMI.refreshSheet()', HMI.RefreshTime);
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
				template._xxx = null; delete template._xxx;
				Component._xxx = null; delete Component._xxx;
				ComponentText._xxx = null; delete ComponentText._xxx;
			} catch (e) {   //IE does not like this hack
			}
			delete template;
			delete Component;
			delete ComponentText;
		};
		
		HMI.hmi_log_trace("HMI.prototype._GetAndShowComponent - End");
	},
	/*********************************
		_cbGetAndAddComponent
	*********************************/	
	_cbGetAndAddComponent: function (Client, req) {
		HMI.hmi_log_trace("HMI.prototype._cbGetAndAddComponent - Start");
		
		//call unique function with parameter to appendChild the SVG
		HMI._GetAndShowComponent(Client, req, false);
		
		HMI.hmi_log_trace("HMI.prototype._cbGetAndAddComponent - End");
	},
	
	/*********************************
		_cbGetAndReplaceComponent
	*********************************/	
	_cbGetAndReplaceComponent: function (Client, req) {
		HMI.hmi_log_trace("HMI.prototype._cbGetAndReplaceComponent - Start");
		
		//call unique function with parameter to replaceChild the SVG
		HMI._GetAndShowComponent(Client, req, true);
		
		HMI.hmi_log_trace("HMI.prototype._cbGetAndReplaceComponent - End");
	},
	
	/*********************************
		_importComponent
	*********************************/	
	_importComponent: function(ComponentText) {
		this.hmi_log_trace("HMI.prototype._importComponent - Start");
		
		//build a DOM fragment with the SVG String
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
			
			//calculate and save absolute offset of the Components
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
			//initialize drag&drop handling
			var dragger = new Dragger(Element, this);
			if (HMI.EmbedAdobePlugin)
			{
				//Adobe needs a hack for the move-gesture
				HMI.AdobeMoveFixNeeded = true;
			}
		//the move handler calls the Leftclick by himself if no movement was detected
		//so there is no further initialisation of move-gesture components needed
		} else {
			//	RIGHTCLICK
			//
			if (this.instanceOf(Element, 'hmi-component-gesture-rightclick'))
			{
				//initialize click handling
				var rightclick = new RightClick(Element, this);
			};
			
			//	(LEFT)CLICK
			//
			if (this.instanceOf(Element, 'hmi-component-gesture-click'))
			{
				//initialize click handling
				var click = new Click(Element, this);
			};
			
			//	DOUBLECLICK
			//
			if (this.instanceOf(Element, 'hmi-component-gesture-doubleclick'))
			{
				//initialize click handling
				var doubleclick = new DoubleClick(Element, this);
			};
			
			//	TEXTINPUT
			//
			if (this.instanceOf(Element, 'hmi-component-gesture-textinput'))
			{
				//initialize textinput handling
				var textinput = new TextInput(Element, this);
			};
			
			//	WHEEL-SCROLL
			//
			//wheelsupport is not supported by the HMI Team and probably firefox only
			if (this.instanceOf(Element, 'hmi-component-gesture-wheelscroll'))
			{
				//initialize wheel handling
				this.wheelscroll = new WheelScroll(Element, this);
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
		// _initGesture does no recursive init, therefor this is done here
		
		if (HMI.EmbedAdobePlugin){
			//getElementsByTagNameNS in Adobe is often not complete
			for (var idx = 0; idx < Fragment.childNodes.length; ++idx){
				if (Fragment.childNodes.item(idx).namespaceURI == HMI.HMI_Constants.NAMESPACE_SVG && Fragment.childNodes.item(idx).id != ""){
					//recursive init necessary
					HMI.initGestures(Fragment.childNodes.item(idx));
				}
			}
		}else if (Fragment.getElementsByTagNameNS){   //gecko, opera, webkit
			Elements = Fragment.getElementsByTagNameNS(HMI.HMI_Constants.NAMESPACE_SVG, 'svg');
			for (var idx = 0; idx < Elements.length; ++idx){
				HMI.hmi_log_trace("HMI.initGestures - idx: "+ idx +" Element.length: "+ Elements.length);
				//recursive init not necessary, getElementsByTagNameNS is recursive by itself
				HMI._initGestures(Elements[idx]);
			}
		}else{   // IE if svg inline, useless since svg inline does not work with gestures
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
		
		checks if a svg element is instanceOf a class
	*********************************/
	instanceOf: function (Node, ClassName) {
		
		//trace log deactivated, causes too much noise and performanceproblem in a production system
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
		//trace log deactivated, causes too much noise and performanceproblem in a production system
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
		delete Classes;
		delete idx;
		
//		this.hmi_log_trace("HMI.prototype._instanceOf - Endf");
		
		return false;
	},
	
	/*********************************
		_setLayerPosition
	*********************************/
	_setLayerPosition: function (Element) {
		this.hmi_log_trace("HMI.prototype._setLayerPosition - Start");
		
		//LayerX and LayerY are HMI specific SVG Attributes!
		//They are ignored by the SVG Renderer but used for position calculation in the move gesture
		var LayerX = parseInt(Element.getAttribute("x"), 10);
		var LayerY = parseInt(Element.getAttribute("y"), 10);
		
		//Firefox, Safari and Opera
		if (Element.ownerSVGElement != undefined){
			if ( Element.ownerSVGElement != null && Element.ownerSVGElement != document){
				LayerX += parseInt(Element.ownerSVGElement.getAttribute("layerX"), 10);
				LayerY += parseInt(Element.ownerSVGElement.getAttribute("layerY"), 10);
			}else if (Element.parentNode != null && Element.parentNode.attributes != null){
				LayerX += parseInt(Element.parentNode.getAttribute("layerX"), 10);
				LayerY += parseInt(Element.parentNode.getAttribute("layerY"), 10);
			}
		}else if(HMI.EmbedAdobePlugin && Element.parentNode != null && Element.parentNode.attributes != null && Element.parentNode.attributes.length != 0){
			LayerX += parseInt(Element.parentNode.getAttribute("layerX"), 10);
			LayerY += parseInt(Element.parentNode.getAttribute("layerY"), 10);
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
			requires console (Firebug, Opera, Safari...)
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
		deleteChilds(HMI.ErrorOutput);
		HMI.ErrorOutput.appendChild(ErrorTextNode);
		//if header in not visible: show it
		if (HMI.showHeader == false){
			HMI.hideHeader();
		}

	}
};
var filedate = "$Date: 2009-03-18 15:53:53 $";
filedate = filedate.substring(7, filedate.length-2);
if ("undefined" == typeof HMIdate){
	HMIdate = filedate;
}else if (HMIdate < filedate){
	HMIdate = filedate;
}
