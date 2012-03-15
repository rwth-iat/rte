/*
*	Copyright (C) 2011
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
*	$Revision$
*	$Date$
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
	
	called at construction time, has to be independent from other js files
***********************************************************************/

function HMI(debug, error, warning, info, trace) {
	this.HMI_Constants = Object();
	this.HMI_Constants.NAMESPACE_XHTML = "http://www.w3.org/1999/xhtml";
	this.HMI_Constants.NAMESPACE_XLINK = "http://www.w3.org/1999/xlink";
	this.HMI_Constants.NAMESPACE_SVG = "http://www.w3.org/2000/svg";
	this.HMI_Constants.NODE_NAME_REACTIONMARKER = "HMI_REACTIONMARKER";
	this.HMI_Constants.NODE_NAME_CLONE = "HMI_CLONE";
	
	this.HMI_Constants.HMIdate = null;
	this.HMI_Constants.ServerType = null;
	
	this.debug = debug;
	//log critical errors to console
	this.error = error;
	this.warning = warning;
	this.info = info;
	//log trace info to console
	this.trace = trace;
	this.initialised = false;
	
	this.ButShowServers = null;
	this.InputRefreshTime = null;
	this.InputHost = null;
	this.HideableHeader = null;
	this.PossServers = null;
	this.PossSheets = null;
	this.Playground = null;
	this.ErrorOutput = null;
	this.InfoOutput = null;
	this.gestureReactionMarkerTimeoutID = null;
	
	this.RefreshTimeoutID = null;
	this._currentDragger = null;
	this.KSClient = null;
	this.cshmi = null;
	this.Path = null;
	this.KSGateway_Path = null;
	this.HMIDOMParser = null;
	
	this.svgWindow = null;
	this.svgDocument = null;
	this.SVGPlugin = null;
	this.GatewayTypeTCL = null;
	this.GatewayTypePHP = null;
	
	this.RefreshTime = null;
	this.ServerProperty = {SheetHasStyleDescription:null};
	
	this.smoothHeaderTimeID = null;
	this.autoKeepHeader = false;
	this.HeaderIsVisible = true;
	this.WebmagellanPath = null;
	
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
		
		called from "load" event, all js files are available
	*********************************/
	init: function () {
		//garantee uniq initialisation
		if (this.initialised === true){
			return true;
		}else{
			this.initialised = true;
		}
		
		this.hmi_log_trace("HMI.prototype.init - Start");
		
		var ErrorDetail = "";
		if (typeof Click != "function"){
			ErrorDetail += "hmi-class-gesture-click.js not loaded.\n";
		}
		if (typeof DoubleClick != "function"){
			ErrorDetail += "hmi-class-gesture-doubleclick.js not loaded.\n";
		}
		if (typeof Dragger != "function"){
			ErrorDetail += "hmi-class-gesture-move.js not loaded.\n";
		}
		if (typeof RightClick != "function"){
			ErrorDetail += "hmi-class-gesture-rightclick.js not loaded.\n";
		}
		if (typeof TextInput != "function"){
			ErrorDetail += "hmi-class-gesture-textinput.js not loaded.\n";
		}
		if (typeof HMIDOMParser != "function"){
			ErrorDetail += "hmi-class-HMIDOMParser.js not loaded.\n";
		}
		if (typeof deleteChilds != "function"){
			ErrorDetail += "hmi-generic.js not loaded.\n";
		}
		if (typeof HMIJavaScriptKSClient != "function"){
			ErrorDetail += "hmi-class-HMIJavaScriptKSClient.js not loaded.\n";
		}
		
		if (typeof cshmi != "function"){
			ErrorDetail += "hmi-class-cshmi.js not loaded.\n";
		}
		
		if(ErrorDetail === ""){
			//all js files loaded, continue testing of the website
			
			//Object of ShowServer-Button
			if ((this.ButShowServers = $('idShowServers'))){
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
			}else{
				addEventSimple(HMI.PossServers, "change", function () {HMI.showSheets(HMI.PossServers.options[HMI.PossServers.selectedIndex].value);});
			}
			//Object of Sheet-Selectbox
			if (!(this.PossSheets = $('idSheets'))){
				ErrorDetail += "HTML Select with the ID: idShowServers not found.\n";
			}else{
				addEventSimple(HMI.PossSheets, "change", function () {HMI.showSheet(HMI.PossSheets.options[HMI.PossSheets.selectedIndex].value);});
			}
			//Element of SVG insertion (div for firefox, container for IE < v9)
			if (!(this.Playground = $('idPlayground'))){
				ErrorDetail += "HTML Container-Element with the ID: idPlayground not found.\n";
			}else{
				//prevent right click in supported Browsers (no Opera < 10.5, no IE6-8)
				addEventSimple(HMI.Playground, "contextmenu", function (evt) {if (evt.preventDefault) evt.preventDefault();});
			}
			//Object of RefreshTime
			if ((this.InputRefreshTime = $('idRefreshTime'))){
				addEventSimple(HMI.InputRefreshTime,'change',function(){HMI.ChangeRefreshTime();});
			}else{
				ErrorDetail += "HTML Input with the ID: idRefreshTime not found.\n";
			}
			//Object of Server-Hostname
			if ((this.InputHost = $('idHost'))){
				//change is wrong here, since a click on the "reload"-button is a change here and a click there => duplicate action
				addEventSimple(HMI.InputHost, 'keyup',function(evt){
						if (evt.keyCode === 13){	// return key
							HMI.showServers();
						}
					});
			}else{
				ErrorDetail += "HTML Input with the ID: idHost not found.\n";
			}
			//Object of the Deep Link
			if (!($('idBookmark'))){
				ErrorDetail += "HTML Input with the ID: idBookmark not found.\n";
			}
			//Object of the hideable header
			if (!(this.HideableHeader = $('idHideableHeader'))){
				ErrorDetail += "HTML Div with the ID: idHideableHeader not found.\n";
			}
			
			//init the plain HTML website with events
			if ($('idHeaderRow')){
				addEventSimple($('idHeaderRow'),'click',function(){HMI.hideHeader(null);});
			}else{
				ErrorDetail += "HTML object with the ID: idHeaderRow not found.\n";
			}
			if ($('idKeepHeader')){
				addEventSimple($('idKeepHeader'),'click',function(){HMI.updateKeepHeader();});
			}else{
				ErrorDetail += "HTML Checkbox with the ID: idKeepHeader not found.\n";
			}
			
			//html5 Session history management
			//reload sheet on history navigation event
			window.onpopstate = function(){ HMI.interpreteUrlParameter(); };
		}
		if(ErrorDetail !== ""){
			if (document.getElementById("idThrobbler") !== null){
				document.getElementById("idThrobbler").style.display = "none";
			}
			window.alert ("Error initialising HMI Website:\n"+ErrorDetail);
			return false;
		}
		
		//init the optional Buttons
		if ($('idRefresh')){
			addEventSimple($('idRefresh'),'click',function(){HMI.refreshSheet();});
		}
		if ($('idStopRefresh')){
			addEventSimple($('idStopRefresh'),'click',function(){
					window.clearTimeout(HMI.RefreshTimeoutID);
					HMI.RefreshTimeoutID = null;
				}
			);
		}
		
		if ($('idStartRefresh')){
			addEventSimple($('idStartRefresh'),'click',function(){
				HMI.RefreshTimeoutID = window.setTimeout(function () {HMI.refreshSheet();}, HMI.InputRefreshTime.value);});
		}
		
		//Object of InfoOutput, optional, not necessary
		if ((this.InfoOutput = $('idInfoOutput'))){
			deleteChilds(HMI.InfoOutput);
		}
		
		// prepare smooth hiding
		HMI.HideableHeader.style.overflow = "hidden";
		
		addEventSimple(document, 'keyup',function(evt){
				if (evt.keyCode === 27){	// escape key
					HMI.hideHeader(false);
				}
			});
		
		//detect if the file is called from http or https, but not from filesystem
		if (-1 == window.location.protocol.indexOf('http')){
			this.hmi_log_error("HMI.prototype.init - Communication to Server failed. This website has to be transfered via HTTP. ");
			this.hmi_log_onwebsite("Communication to Server failed. This website has to be transfered via HTTP.");
			if (document.getElementById("idThrobbler") !== null){
				document.getElementById("idThrobbler").style.display = "none";
			}
			return false;
		}else if (window.location.search.length !== 0 && -1 !== window.location.search.search(/ServerType=/)){
			if ( -1 !== window.location.search.search(/ServerType=php/)){
				HMI.HMI_Constants.ServerType = "php";
			}else if ( -1 !== window.location.search.search(/ServerType=tcl/)){
				HMI.HMI_Constants.ServerType = "tcl";
			}
		}else{
			//Try to detect the Servertype (TCL or PHP capable)
			//make a request to sniff the HTTP-Serverstring
			
			//IE sometimes uses a cached version, without server Header, so prevent caching
			var DatePreventsCaching = new Date();
			try{
				var req = new XMLHttpRequest();
				req.open("GET", window.location.pathname+'?preventCaching='+DatePreventsCaching.getTime(), false);
				req.send(null);
				var ResponseServerString = req.getResponseHeader('server');
				if (req.readyState != 4){
					//some old mobile browser does not support syncron requests
					this.hmi_log_error("HMI.prototype.init - Communication to Server failed. This website has to be transfered via HTTP. ");
					this.hmi_log_onwebsite("This Browser does not support synchronous XMLHttpRequest, which is required for ACPLT/HMI. Please use another Browser");
					if (document.getElementById("idThrobbler") !== null){
						document.getElementById("idThrobbler").style.display = "none";
					}
					return false;
				}else if (ResponseServerString && -1 != ResponseServerString.indexOf('Tcl-Webserver')){
					HMI.HMI_Constants.ServerType = "tcl";
					this.hmi_log_trace("HMI.prototype.init - detected TCL Gateway");
				}else if (ResponseServerString &&  -1 != ResponseServerString.indexOf('PHP')){
					HMI.HMI_Constants.ServerType = "php";
					this.hmi_log_trace("HMI.prototype.init - detected PHP Gateway");
				}else if (ResponseServerString &&  -1 != ResponseServerString.indexOf('Apache')){
					HMI.HMI_Constants.ServerType = "php";
					this.hmi_log_trace("HMI.prototype.init - detected Apache Webserver, so probably a PHP Gateway");
				}
			}catch(e){
				this.hmi_log_error("HMI.prototype.init - Gatewaydetection failed: "+e.message);
				this.hmi_log_onwebsite('Could not detect type of HTTP/KS-Gateway. Please configure in hmi-class-HMI.js');
				if (document.getElementById("idThrobbler") !== null){
					document.getElementById("idThrobbler").style.display = "none";
				}
				return false;
			}
			req = null;
			ResponseServerString = null;
			DatePreventsCaching = null;
		}
		//the guessed servertype is used for communication
		if ("php" == HMI.HMI_Constants.ServerType){
			//tks.php is always in the same subdir as the html files
			this.KSGateway_Path = window.location.pathname.substring(0, window.location.pathname.lastIndexOf("/")+1)+ "tks.php";
			this.GatewayTypeTCL = false;
			this.GatewayTypePHP = true;
		}else if("tcl" == HMI.HMI_Constants.ServerType){
			//tcl gateway is hardcoded in the Gateway via TCL-HTTPD\tclhttpd3.5.1\custom\tkshttpserver.tcl
			this.KSGateway_Path = "/tks";
			this.GatewayTypeTCL = true;
			this.GatewayTypePHP = false;
		}else{
			HMI.hmi_log_onwebsite('Could not detect type of HTTP/KS-Gateway (Is a manipulating Proxy like Opera Turbo active?). Please configure in hmi-class-HMI.js');
			
			var NewURLBasename = null;
			if (window.location.search === null || window.location.search.length === 0){
				NewURLBasename = window.location.href+"?ServerType=";
			}else if (-1 === window.location.search.search(/ServerType/)){
				NewURLBasename = window.location.href+"&ServerType=";
			}
			
			HMI.ErrorOutput.appendChild(document.createElement('br'));
			
			var ErrorNode = document.createElement("a");
			ErrorNode.href = 'httpservertest.html';
			ErrorNode.appendChild(document.createTextNode('Servertest available.'));
			
			HMI.ErrorOutput.appendChild(ErrorNode);
			
			if (NewURLBasename !== null){
				HMI.ErrorOutput.appendChild(document.createElement('br'));
				
				ErrorNode = document.createElement("a");
				ErrorNode.href = NewURLBasename + 'tcl';
				ErrorNode.appendChild(document.createTextNode('Force TCL.'));
				
				HMI.ErrorOutput.appendChild(ErrorNode);
				HMI.ErrorOutput.appendChild(document.createTextNode(' '));
				
				ErrorNode = document.createElement("a");
				ErrorNode.href = NewURLBasename+'php';
				ErrorNode.appendChild(document.createTextNode('Force PHP. '));
				
				HMI.ErrorOutput.appendChild(ErrorNode);
			}
			ErrorNode = null;
			if (document.getElementById("idThrobbler") !== null){
				document.getElementById("idThrobbler").style.display = "none";
			}
			return false;
		}
		
		//call function at unload to clean up
		if( window.addEventListener ) {
			window.addEventListener('unload',function(){HMI.unload();},false);
		} else if( document.addEventListener ) {
			document.addEventListener('unload',function(){HMI.unload();},false);
		} else if( window.attachEvent ) {
			window.attachEvent('onunload',function(){HMI.unload();});
		}
		
		//detect type of SVG display
		if (this.Playground.tagName.toLowerCase() == "embed" || this.Playground.tagName.toLowerCase() == "object"){
			//via a container plugin tag
			
			//remember ContainerNode for resizing
			this.PlaygroundContainerNode = this.Playground;
			if (this.Playground.window){
				//get inner window
				this.svgWindow = this.Playground.window;
			}else{
				this.svgWindow = window;
			}
			if (typeof this.Playground.getSVGDocument != "undefined"){
				//get inner document (Adobe SVG Viewer, Renesis Plugin
				this.svgDocument = this.Playground.getSVGDocument();
			}else if (this.Playground.documentElement){
				//get inner document (Ssrc svg Plugin)
				this.svgDocument = this.Playground.documentElement;
			}else{
				this.svgDocument = this.Playground.document;
			}
			//switch the target Playground to the container SVG-Plugin DOM
			if (this.svgDocument.getElementById("svgcontainer") !== null){
				//get inner document
				this.Playground = HMI.svgDocument.getElementById("svgcontainer");
			}
			//sometimes feature detection is not possible
			if (typeof HMI.svgWindow.getSVGViewerVersion != "undefined"){
				this.SVGPlugin = true;
				this.PluginVendor = HMI.svgWindow.getSVGViewerVersion();
				if (-1 != this.PluginVendor.indexOf('Adobe')){
					this.PluginVendor = 'Adobe';
				}else if (-1 != this.PluginVendor.indexOf('examotion')){
					this.PluginVendor = 'Examotion';
				}
			}
		}else{
			//inline SVG in XHTML DOM, we have only one document and window
			this.SVGPlugin = false;
			this.svgDocument = document;
			this.svgWindow = window;
		}
		
		//Init KSClient
		this.KSClient = new HMIJavaScriptKSClient();
		this.HMIDOMParser = new HMIDOMParser();
		
		//The state of the Checkbox is preserved at a reload from cache, so
		//we have to update the variable to reflect the userchoice
		HMI.updateKeepHeader();
		
		//deactivate the Select-Boxes, because there is no usefull content now
		HMI.PossServers.disabled = true;
		HMI.PossSheets.disabled = true;
		
		//HMIdate was populated in every js-file with the date of CVS commit
		//publish this date on website
		if ("undefined" != typeof HMIdate){
			HMI.HMI_Constants.HMIdate = HMIdate;
			var dateTextNode = document.createTextNode("Version: 2.1 ("+HMI.HMI_Constants.HMIdate.substr(0, 10).replace(/\//g, "-")+")");
			
			var DateOutput;
			if ((DateOutput = document.getElementById("idDateOutput"))){
				DateOutput.appendChild(dateTextNode);
				DateOutput.setAttribute("title", "last changed: "+HMI.HMI_Constants.HMIdate+" UTC");
			}
			DateOutput = null;
			HMIdate = null;
			dateTextNode = null;
		}
		
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		//try to search for webmagellan and provide a link
		var ksmagellanPath = new Array("/magellan", "/webmagellan", "/webksmagellan");
		var path = ksmagellanPath.shift();
		while(this.WebmagellanPath === null && path !== undefined){
			req = new XMLHttpRequest();
			req.open("GET", path, false);
			req.send(null);
			
			if (req.status == 200 && req.responseText && req.responseText.length > 300 && req.responseText.indexOf("Not Found") == -1){
				this.WebmagellanPath = path;
			}
			path = ksmagellanPath.shift();
		}
		req = null;
		path = null;
		
		if (this.WebmagellanPath !== null){
			var MagellanLink;
			if (document.createElementNS){
				MagellanLink = document.createElementNS(this.HMI_Constants.NAMESPACE_XHTML, 'a');
			}else{
				MagellanLink = document.createElement('a');
			}
			if (HMI.InputHost.value.length === 0){
				MagellanLink.href = this.WebmagellanPath;
				MagellanLink.style.display = 'none';
			}else{
				MagellanLink.href = this.WebmagellanPath + '?cmd=start&arg1='+HMI.InputHost.value;
			}
			MagellanLink.target = 'TargetWebmagellan';
			MagellanLink.id = 'idWebmagellan';
			MagellanLink.style.paddingRight = '10px';
			MagellanLink.appendChild(document.createTextNode('See in Webmagellan'));
			
			$("idBookmark").parentNode.insertBefore(MagellanLink, $("idBookmark").parentNode.firstChild);
			MagellanLink = null;
		}
		
		//make deep links work
		this.interpreteUrlParameter();
		
		/*	TODO reimplement event handling based on document with this code
		if (this.svgDocument.addEventListener) {  // Firefox, Google Chrome, Safari, Opera, ie9
			this.svgDocument.addEventListener ("mousedown", 
				function(evt){
					var myDragger = HMI.getComponent(evt, 'hmi-component-gesture-move');
					if (myDragger !== null){
						HMI.hmi_log_info("document ### found: "+myDragger.id);
					}
					//HMI.ProtoDragger.startDrag(evt, HMI.getComponent(evt, 'hmi-component-gesture-move'));
				}
			, true);
			this.svgDocument.addEventListener ("click", 
				function(evt){
					//HMI.ProtoClick._sendCommand(evt, HMI.getComponent(evt, 'hmi-component-gesture-click'));
				}
			, true);
			alert("events registered on document");
		}else if (typeof HMI.Playground.addEventListener == "unknown"){
			//adobe plugin
			HMI.Playground.addEventListener ("mousedown", 
				function(evt){
					var myDragger = HMI.getComponent(evt, 'hmi-component-gesture-move');
					if (myDragger !== null){
						HMI.hmi_log_info("document ### found: "+myDragger.id);
					}
					//HMI.ProtoDragger.startDrag(evt, HMI.getComponent(evt, 'hmi-component-gesture-move'));
				}
			, false);
			alert("event registered on playground");
		}
		*/
		
		if (document.getElementById("idThrobbler") !== null){
			document.getElementById("idThrobbler").style.display = "none";
		}
		
		this.hmi_log_trace("HMI.prototype.init - End");
		return true;
	},
	
	/*********************************
		Functions - interpreteUrlParameter
	*********************************/
	interpreteUrlParameter: function (){
		this.hmi_log_trace("HMI.prototype.interpreteUrlParameter");
		
		//analyse a "deep link" of a sheet
		var HMI_Parameter_Liste = null;
		if (window.location.search.length !== 0){
			//collect parameter given by the deep link
			HMI_Parameter_Liste = new Array();
			var wertestring = unescape(window.location.search);
			wertestring = wertestring.slice(1);
			var paare = wertestring.split("&");
			for (var i=0; i < paare.length; i++) {
				var name = paare[i].substring(0, paare[i].indexOf("="));
				var wert = paare[i].substring(paare[i].indexOf("=")+1, paare[i].length);
				HMI_Parameter_Liste[name] = wert;
			}
			paare = null;
			wertestring = null;
		}
		//jump to a "deep link" of a sheet
		if (	HMI_Parameter_Liste !== null &&
			 (	HMI_Parameter_Liste.Host !== undefined
			||	HMI_Parameter_Liste.RefreshTime !== undefined
			||	HMI_Parameter_Liste.ShowComp !== undefined
			||	HMI_Parameter_Liste.Server !== undefined
			||	HMI_Parameter_Liste.Sheet !== undefined)
			){
			//correct host in website with user wish
			if (HMI_Parameter_Liste.Host && HMI_Parameter_Liste.Host.length !== 0 && HMI_Parameter_Liste.Host == window.location.hostname){
				//we faked the host to the hostname in some cases (empty input field)
				HMI.InputHost.value = HMI_Parameter_Liste.Host;
				HMI_Parameter_Liste.Host = "localhost";
			}else if (HMI_Parameter_Liste.Host && HMI_Parameter_Liste.Host.length !== 0){
				HMI.InputHost.value = HMI_Parameter_Liste.Host;
			}else{
				//allow shorten of the deep link, fall back to localhost (server view)
				HMI_Parameter_Liste.Host = "localhost";
			}
			//correct RefreshTime in website with user wish
			if (HMI_Parameter_Liste.RefreshTime && HMI_Parameter_Liste.RefreshTime.length !== 0){
				HMI.InputRefreshTime.value = HMI_Parameter_Liste.RefreshTime;
			}
			HMI.ChangeRefreshTime();
			
			//correct ShowComponents Status in website with user wish
			if (HMI_Parameter_Liste.ShowComp && HMI_Parameter_Liste.ShowComp.length !== 0){
				if (HMI_Parameter_Liste.ShowComp == "true" && $("idShowcomponents")){
					$("idShowcomponents").checked = true;
				}
			}
			
			//a server and sheet is specified in "deep link" (host is always there)
			if (	HMI_Parameter_Liste.Server
				&&	HMI_Parameter_Liste.Server.length !== 0
				&&	HMI_Parameter_Liste.Sheet
				&&	HMI_Parameter_Liste.Sheet.length !== 0)
			{
				//clean old Server and Sheet entrys, an old SVG display and displayed errors in website
				deleteChilds(this.PossServers);
				deleteChilds(this.PossSheets);
				deleteChilds(this.Playground);
				deleteChilds(this.ErrorOutput);
				
				HMI.PossServers.options[0] = new Option('- list not loaded -', 'no server');
				HMI.PossSheets.options[0] = new Option('- list not loaded -', 'no sheet');
				
				//deactivate the Select-Boxes, because there is no usefull content
				HMI.PossServers.disabled = true;
				HMI.PossSheets.disabled = true;
				
				HMI.ButShowServers.value = "Reload Serverlist";
				
				//an init generates a new Handle, needed cause we communicate to the Server the first time
				if (HMI_Parameter_Liste.Host.indexOf(":") === -1){
					this.KSClient.init(HMI_Parameter_Liste.Host + ':7509/' + HMI_Parameter_Liste.Server, window.location.host + HMI.KSGateway_Path);
					if (this.KSClient.TCLKSHandle === null){
						//fall back for portmapper MANAGER detection
						this.KSClient.init(HMI_Parameter_Liste.Host + '/' + HMI_Parameter_Liste.Server, window.location.host + HMI.KSGateway_Path);
					}
				}else{
					this.KSClient.init(HMI_Parameter_Liste.Host + '/' + HMI_Parameter_Liste.Server, window.location.host + HMI.KSGateway_Path);
				}
				if (this.KSClient.TCLKSHandle === null){
						HMI.hmi_log_onwebsite('Requested Host or FB-Server on Host not available.');
				}else{
					//the path of the HMI Manager could be different in every OV Server (manager needed for all gestures)
					this.KSClient.getHMIManagerPointer();
					//spaces in objectname are encoded as %20 within OV
					HMI.showSheet(encodeURI(HMI_Parameter_Liste.Sheet));
				}
			}else if (	HMI_Parameter_Liste.Server
				&&	HMI_Parameter_Liste.Server.length !== 0){
				//only a server is specified in "deep link" => showSheets
				
				HMI.PossServers.options[0] = new Option('- list not loaded -', 'no server');
				HMI.PossSheets.options[0] = new Option('- list not loaded -', 'no sheet');
				
				HMI.ButShowServers.value = "Show Servers";
				
				//an init generates a new Handle, needed cause we communicate to the Server the first time
				if (HMI_Parameter_Liste.Host.indexOf(":") === -1){
					this.KSClient.init(HMI_Parameter_Liste.Host + ':7509/' + HMI_Parameter_Liste.Server, window.location.host + HMI.KSGateway_Path);
					if (this.KSClient.TCLKSHandle === null){
						//fall back for portmapper MANAGER detection
						this.KSClient.init(HMI_Parameter_Liste.Host + '/' + HMI_Parameter_Liste.Server, window.location.host + HMI.KSGateway_Path);
					}
				}else{
					this.KSClient.init(HMI_Parameter_Liste.Host + '/' + HMI_Parameter_Liste.Server, window.location.host + HMI.KSGateway_Path);
				}
				if (this.KSClient.TCLKSHandle === null){
						HMI.hmi_log_onwebsite('Requested Host or FB-Server on Host not available.');
				}else{
					HMI.showSheets(HMI_Parameter_Liste.Server);
				}
			}else if (HMI_Parameter_Liste.Host && HMI_Parameter_Liste.Host.length !== 0){
				//no server and sheet specified, but a host => load serverlist
				HMI.showServers();
			}
		}
		if (HMI.ButShowServers.value == "Initialising HMI..."){
			HMI.ButShowServers.value = "Show Servers";
		}
		//reenable click by user
		HMI.ButShowServers.disabled = false;
		
		//focus the ShowServer button for convenience with keyboard interaction
		//try because the button could be nonvisible
		try{
			HMI.ButShowServers.focus();
		}catch(e){ }
		
		if (document.getElementById("idThrobbler") !== null){
			document.getElementById("idThrobbler").style.display = "none";
		}
		
		//append html5 datalist if supported and provided
		if(this.InputHost.list !== undefined & HMI_Parameter_Liste !== null && HMI_Parameter_Liste.hostlist !== undefined){
			this.InputHost.setAttribute("list", "InputHost");
			var datalistNode = document.createElement("datalist");
			datalistNode.setAttribute("id", "InputHost");
			
			var hostlist = HMI_Parameter_Liste.hostlist.split(",");
			for (var i=0;i < hostlist.length;i++){
				var optionNode = document.createElement("option");
				optionNode.setAttribute("value", hostlist[i]);
				datalistNode.appendChild(optionNode);
			}
			this.InputHost.parentNode.appendChild(datalistNode);
		}
		HMI_Parameter_Liste = null;
		
		this.hmi_log_trace("HMI.prototype.interpreteUrlParameter - End");
		return;
	},
	
	/*********************************
		Functions - displaygestureReactionMarker
		
		called after sending a gesture request
	*********************************/
	displaygestureReactionMarker: function (Component){
		this.hmi_log_trace("HMI.prototype.displaygestureReactionMarker");
		
		//build info rect around affected component
		
		//warning: This function is called after the refresh, so this Component is NOT displayed anymore! (but probably another similar Component)
		
		var gestureReactionMarker;
		
		//find old marker and remove from dom
		if (HMI.Playground.firstChild.lastChild.id == HMI.HMI_Constants.NODE_NAME_REACTIONMARKER){
			gestureReactionMarker = HMI.Playground.firstChild.removeChild(HMI.Playground.firstChild.lastChild);
			if (HMI.gestureReactionMarkerTimeoutID !== null){
				window.clearTimeout(HMI.gestureReactionMarkerTimeoutID);
				HMI.gestureReactionMarkerTimeoutID = null;
			}
		}else{
			//build new marker
			gestureReactionMarker = HMI.svgDocument.createElementNS(HMI.HMI_Constants.NAMESPACE_SVG, 'rect');
			gestureReactionMarker.id = HMI.HMI_Constants.NODE_NAME_REACTIONMARKER;
			gestureReactionMarker.setAttributeNS(null, 'fill', '#D1E1FA');
			gestureReactionMarker.setAttributeNS(null, 'opacity', '0.6');
			
			//disable pointer Events
			gestureReactionMarker.setAttributeNS(null, 'pointer-events', 'none');
		}
		gestureReactionMarker.setAttributeNS(null, 'x', Component.getAttribute('layerX'));
		gestureReactionMarker.setAttributeNS(null, 'y', Component.getAttribute('layerY'));
		gestureReactionMarker.setAttributeNS(null, 'width', Component.getAttribute('width'));
		gestureReactionMarker.setAttributeNS(null, 'height', Component.getAttribute('height'));
		
		//append this rect into svg
		HMI.Playground.firstChild.appendChild(gestureReactionMarker);
		
		gestureReactionMarker = null;
		
		//clear display in a few milliseconds
		HMI.gestureReactionMarkerTimeoutID = window.setTimeout(
			function(){
				if (HMI.Playground.firstChild.lastChild.id == HMI.HMI_Constants.NODE_NAME_REACTIONMARKER){
					HMI.Playground.firstChild.removeChild(HMI.Playground.firstChild.lastChild);
					HMI.gestureReactionMarkerTimeoutID = null;
				}
			}, 800);
	},
	
	/*********************************
		Functions - updateKeepHeader
		
		called with a onclick handler of the checkbox and in HMI.init
	*********************************/
	updateKeepHeader: function (){
		this.hmi_log_trace("HMI.prototype.updateKeepHeader - change requested");
		if (document.getElementById("idKeepHeader").checked === true) {
			HMI.autoKeepHeader = true;
		}else{
			HMI.autoKeepHeader = false;
		}
	},
	/*********************************
		Functions - hideHeader
	*********************************/
	hideHeader: function(hide){
		if (hide === true || (hide === null && HMI.HeaderIsVisible === true)){
			//hide menu
			
			//	iterate hiding function
			//
			HMI.smoothHeaderTimeID = window.setTimeout(this.smoothHeaderHide, 30);
			
			//switch arrow orientation
			//
			if (document.getElementById("arrowdown1") !== null){
				document.getElementById("arrowdown1").style.visibility="visible";
				document.getElementById("arrowdown2").style.visibility="visible";
				document.getElementById("arrowup1").style.visibility="hidden";
				document.getElementById("arrowup2").style.visibility="hidden";
			}
		} else {
			//show menu
			HMI.HeaderIsVisible = true;
			
			//repaint header
			HMI.HideableHeader.style.height = "";
			
			//switch arrow orientation
			//
			if (document.getElementById("arrowdown1") !== null){
				document.getElementById("arrowdown1").style.visibility="hidden";
				document.getElementById("arrowdown2").style.visibility="hidden";
				document.getElementById("arrowup1").style.visibility="visible";
				document.getElementById("arrowup2").style.visibility="visible";
			}
		}
	},
	/*********************************
		Functions - smoothHeaderHide
	*********************************/
	smoothHeaderHide: function(){
		var HeaderActualheight = parseInt(HMI.HideableHeader.offsetHeight, 10);
		if (HeaderActualheight <= 30){
			// last iteration, complete hiding
			//
			HMI.HideableHeader.style.height = '1px';	// ie < v8 renders 0px like fully visible
			HMI.HeaderIsVisible = false;
		}else{
			//adjust header height and call us in a few milli seconds again
			HeaderActualheight -= 30;
			HMI.HideableHeader.style.height = HeaderActualheight + 'px';
			window.setTimeout(HMI.smoothHeaderHide, 30);
		}
	},
	/*********************************
		Functions - ChangeRefreshTime
	*********************************/
	ChangeRefreshTime: function(){
		
		var newInputRefreshTime;
		//html5 forms give us the value for free
		if (HMI.InputRefreshTime.valueAsNumber !== undefined){
			newInputRefreshTime = HMI.InputRefreshTime.valueAsNumber;
		}else{
			newInputRefreshTime = parseInt(HMI.InputRefreshTime.value,10);
		}
		
		if (isNaN(newInputRefreshTime)){
			//there was non number characters in the form
			HMI.InputRefreshTime.value = 1000;
			this.RefreshTime = 1000;
		}else if (newInputRefreshTime < 100){
			HMI.InputRefreshTime.value = 100;
			this.RefreshTime = 100;
		}else{
			this.RefreshTime = newInputRefreshTime;
			//make input variable number-only
			HMI.InputRefreshTime.value = newInputRefreshTime;
		}
		newInputRefreshTime = null;
		$("idBookmark").href = window.location.protocol+"//"+
			window.location.host+window.location.pathname.substring(0, window.location.pathname.lastIndexOf("/")+1)+
			"?Host="+HMI.InputHost.value+
			"&RefreshTime="+HMI.RefreshTime+
			"&Server="+(HMI.KSClient.KSServer ? HMI.KSClient.KSServer.substr(HMI.KSClient.KSServer.indexOf('/')+1) : "")+
			"&Sheet="+(HMI.Path !== null ? HMI.Path : "")+
			(HMI.trace===true?"&trace=true":"")+
			(($("idShowcomponents") && $("idShowcomponents").checked)?"&ShowComp=true":"");
		
		//if an auto refresh is active, reset to new value
		if (HMI.RefreshTimeoutID !== null){
			HMI.refreshSheet();
			HMI.hmi_log_info_onwebsite('Refreshtime set to '+(HMI.RefreshTime/1000)+'s.');
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
		if (document.getElementById("idThrobbler") !== null){
			document.getElementById("idThrobbler").style.display = "inline";
		}
		
		window.clearTimeout(HMI.RefreshTimeoutID);
		HMI.RefreshTimeoutID = null;
		
		//get rid of spaces in the textinput
		HMI.InputHost.value = HMI.InputHost.value.trim();
		if (HMI.InputHost.value.length === 0){
			HMI.InputHost.value = window.location.hostname;
		}
		
		//if the hostname is identical to the http server, translate into "localhost"
		var KSServer;
		if (HMI.InputHost.value == window.location.hostname){
			KSServer = "localhost";
		}else{
			KSServer = HMI.InputHost.value;
		}
		
		HMI.ChangeRefreshTime();
		
		//clean old Server and Sheet entrys, an old SVG display and displayed errors in website
		deleteChilds(this.PossServers);
		deleteChilds(this.PossSheets);
		deleteChilds(this.Playground);
		deleteChilds(this.ErrorOutput);
		deleteChilds(this.InfoOutput);
		
		//deactivate the Select-Boxes, because there is no usefull content
		HMI.PossServers.disabled = true;
		HMI.PossSheets.disabled = true;
		
		//Set a neutral title
		document.title = "Startcenter - ACPLT/HMI";
		
		//Gateway could not be another host without violating Same Origin Policy
		var KSGateway = window.location.host;
		
		//present a deep link to the Host setting
		$("idBookmark").style.display = "";
		$("idBookmark").href = window.location.protocol+"//"+
			window.location.host+window.location.pathname.substring(0, window.location.pathname.lastIndexOf("/")+1)+
			"?Host="+HMI.InputHost.value+
			"&RefreshTime="+HMI.RefreshTime+
			(HMI.trace===true?"&trace=true":"")+
			(($("idShowcomponents") && $("idShowcomponents").checked)?"&ShowComp=true":"");
		
		//an init generates a new Handle, needed cause we communicate to the Manager the first time
		if (KSServer.indexOf(":") === -1){
			this.KSClient.init(KSServer + ':7509/MANAGER', KSGateway + HMI.KSGateway_Path);
			if (this.KSClient.TCLKSHandle === null){
				//fall back for portmapper MANAGER detection
				this.KSClient.init(KSServer + '/MANAGER', KSGateway + HMI.KSGateway_Path);
			}
		}else{
			this.KSClient.init(KSServer + '/MANAGER', KSGateway + HMI.KSGateway_Path);
		}
		if (this.KSClient.TCLKSHandle !== null){
			this.KSClient.getServers();
		}else{
			HMI.PossServers.setAttribute("title", "No MANAGER available");
			HMI.hmi_log_info_onwebsite("Requested Host has no MANAGER available.");
		}
		
		//reenable click by user
		HMI.ButShowServers.disabled = false;
		HMI.ButShowServers.value = "Reload Serverlist";
		if (document.getElementById("idThrobbler") !== null){
			document.getElementById("idThrobbler").style.display = "none";
		}
		//correct magellan URL
		//
		if (this.WebmagellanPath !== null){
			$("idWebmagellan").style.display = '';
			//todo gefälscher ksserver dings
			$("idWebmagellan").href = HMI.WebmagellanPath + (HMI.InputHost.value.length !== 0 ? '?cmd=start&arg1='+KSServer : "");
		}
		KSServer = null;
		
		this.hmi_log_trace("HMI.prototype.showServers - End");
	},
	
	/*********************************
		showSheets
	*********************************/
	showSheets: function (Server) {
		this.hmi_log_trace("HMI.prototype.showSheets - Start, requested Server: "+Server);
		
		//clean old Sheet entrys, an old SVG display and displayed errors in website
		deleteChilds(this.PossSheets);
		deleteChilds(this.Playground);
		deleteChilds(this.ErrorOutput);
		deleteChilds(this.InfoOutput);
		
		HMI.PossSheets.disabled = true;
		$("idBookmark").style.display = "none";
		document.title = "Startcenter - ACPLT/HMI";
		
		window.clearTimeout(HMI.RefreshTimeoutID);
		HMI.RefreshTimeoutID = null;
		
		//nothing selected
		if (Server == 'no server'){
			HMI.PossSheets.options[0] = new Option('please select Server first', 'no sheet');
			return false;
		}
		
		var SheetList = this.KSClient.getSheets(Server);
		
		this.hmi_log_trace("HMI.prototype.showSheets - number of sheets: "+SheetList.length);
		if (SheetList.length === 0){
			this.PossSheets.options[0] = new Option('- no sheets available -', 'no sheet');
			return false;
		} else {
			HMI.PossSheets.options[HMI.PossSheets.options.length] = new Option('- select sheet -', 'no sheet');
			for (i = 0; i < SheetList.length; i++){
				//spaces in objectname are encoded as %20 within OV
				HMI.PossSheets.options[HMI.PossSheets.options.length] = new Option(decodeURI(SheetList[i]), SheetList[i]);
			}
			if (SheetList.length === 1){
				//selecting the option does not trigger the EventListener
				HMI.PossSheets.selectedIndex = 1;
				HMI.showSheet(SheetList[0]);
			}
			HMI.PossSheets.disabled = false;
		}
		SheetList = null;
		
		this.hmi_log_trace("HMI.prototype.showSheets - End");
		return true;
	},
	
	/*********************************
		showSheet
	*********************************/
	showSheet: function (Sheet) {
		this.hmi_log_trace("HMI.prototype.showSheet - Start with Sheet: "+Sheet);
		
		//clean an old SVG display and displayed errors in website
		deleteChilds(this.Playground);
		deleteChilds(this.ErrorOutput);
		deleteChilds(this.InfoOutput);
		$("idBookmark").style.display = "none";
		
		window.clearTimeout(HMI.RefreshTimeoutID);
		HMI.RefreshTimeoutID = null;
		
		//nothing selected
		if (Sheet == 'no sheet'){
			return;
		}
		
		if (HMI.KSClient.TCLKSHandle !== null)
		{
			HMI.Path = Sheet;
			
			//[StyleDescription] remove this line if no ACPLT/HMI Server has a StyleDescription anymore
			HMI.KSClient.checkSheetProperty(HMI.Path);
			
			this.refreshSheet();
		};
		//show fb-server name when known
		//spaces in objectname are encoded as %20 within OV
		document.title = ((HMI.PossServers.options.length !==1)?"//"+HMI.PossServers.options[HMI.PossServers.selectedIndex].value:"")+decodeURI(Sheet)+" - ACPLT/HMI";
		if (HMI.autoKeepHeader === false && !HMI.ErrorOutput.firstChild){
			if (!HMI.InfoOutput){
				//no info output available => hide
				HMI.hideHeader(true);
			}else if (HMI.InfoOutput && !HMI.InfoOutput.firstChild){
				//info output with no content => hide
				HMI.hideHeader(true);
			}
		}
		
		//blur the buttons for convenience with keyboard interaction
		HMI.PossSheets.blur();
		HMI.PossServers.blur();
		//present a "deep link" to the sheet
		$("idBookmark").href = window.location.protocol+"//"+
			window.location.host+window.location.pathname.substring(0, window.location.pathname.lastIndexOf("/")+1)+
			"?Host="+HMI.InputHost.value+
			"&RefreshTime="+HMI.RefreshTime+
			"&Server="+(HMI.KSClient.KSServer ? HMI.KSClient.KSServer.substr(HMI.KSClient.KSServer.indexOf('/')+1) : "")+
			"&Sheet="+(HMI.Path !== null ? HMI.Path : "")+
			(HMI.trace===true?"&trace=true":"")+
			(($("idShowcomponents") && $("idShowcomponents").checked)?"&ShowComp=true":"");
		
		//add sheet to html5 Session history management, if new
		if (window.history.pushState){
			if ($("idBookmark").href != window.location.href){
				window.history.pushState("", "", $("idBookmark").href);
			}
		}else{
			//show deeplink if history management is not available
			$("idBookmark").style.display = "inline";
		}
		
		this.hmi_log_trace("HMI.prototype.showSheet - End");
	},
	
	/*********************************
		refreshSheet
	*********************************/
	refreshSheet: function () {
		this.hmi_log_trace("HMI.prototype.refreshSheet - Start");
		
		window.clearTimeout(HMI.RefreshTimeoutID);
		HMI.RefreshTimeoutID = null;
		
		if (this.InfoOutput){
			deleteChilds(this.InfoOutput);
		}
		if (HMI.KSClient.TCLKSHandle !== null){
			var SVGRequestURI;
			
			//[StyleDescription] remove this if no ACPLT/HMI Server has a StyleDescription anymore
			if (HMI.ServerProperty.SheetHasStyleDescription){
				//spaces in objectname are encoded as %20 within OV
				SVGRequestURI = '{' + encodeURI(HMI.Path) + '.GraphicDescription' + '%20' + encodeURI(HMI.Path) + '.StyleDescription' + '}';
			}else{
				//spaces in objectname are encoded as %20 within OV
				SVGRequestURI = encodeURI(HMI.Path) + '.GraphicDescription';
			}
			
			//	get GraphicDescription
			//
			var ComponentText = this.KSClient.getVar(null, SVGRequestURI, null);
			
			//check if we have an cshmi target
			if (ComponentText && ComponentText.indexOf("KS_ERR_BADPATH") !== -1){
				this.cshmi = new cshmi();
				this.cshmi.instanciateCshmi(HMI.Path);
			}else{
				//hmi target
				var SplitComponent = this.KSClient.prepareComponentText(ComponentText);
				if (SplitComponent === null){
					//logging not required, allready done by prepareComponentText
					return;
				}
				
				//[StyleDescription] adjust this line if no ACPLT/HMI Server has a StyleDescription anymore
				//build a DOM fragment with the SVG String
				var Component = this.HMIDOMParser.parse(SplitComponent[0], SplitComponent[1]);
				
				if (Component != null){
					this._showComponent(Component);
					
					//	activate refresh of sheet later
					this.RefreshTimeoutID = window.setTimeout(function () {HMI.refreshSheet();}, this.RefreshTime);
				}
				
			}
			//reload scroll setting
			//wheelsupport is not supported by the HMI Team and probably firefox only
			if(this.scrollComponent)
			{
				var Component;
				if(document.getElementById(this.scrollComponent) !== null){ //opera, ff
					Component = document.getElementById(this.scrollComponent);
				} else if(this.Playground.getElementById(this.scrollComponent) !== null){ //ie
					Component = this.Playground.getElementById(this.scrollComponent);
				}
				Component.setAttribute("y", this.currY);
				Component.setAttribute("x", this.currX);
			}
			
			SVGRequestURI = null;
			ComponentText = null;
		}
		this.hmi_log_trace("HMI.prototype.refreshSheet - End");
	},
	
	/*********************************
		cbrefreshSheet
	*********************************/
	cbrefreshSheet: function (client, req) {
		HMI.hmi_log_trace("HMI.prototype.cbrefreshSheet - Start");
		
		HMI.refreshSheet();
		
		HMI.hmi_log_trace("HMI.prototype.cbrefreshSheet - End");
	},
	
	/*********************************
		reactivateRefreshInterval
	*********************************/
	reactivateRefreshInterval: function (evt) {
		HMI.hmi_log_trace("HMI.prototype.reactivateRefreshInterval- Start");
		
		//inform the user if screen update is reactivated after loss of server
		if (HMI.ErrorOutput.firstChild){
			deleteChilds(HMI.ErrorOutput);
			HMI.hmi_log_info_onwebsite("Screenrefresh reactivated.");
			HMI.hideHeader(false);
		}
		if (HMI.RefreshTimeoutID === null){
			//reactivate the Refresh
			HMI.RefreshTimeoutID = window.setTimeout(function () {HMI.refreshSheet();}, HMI.RefreshTime);
		}
		
		//this function is called by mouseup event or mousemove => cleanup
		if(HMI.svgDocument.removeEventListener){
			//Firefox, Safari, Opera...
			HMI.svgDocument.removeEventListener("mousemove", HMI.reactivateRefreshInterval, false);
		}else if("unknown" == typeof HMI.svgDocument.documentElement.removeEventListener){
			//Adobe Plugin
			HMI.svgDocument.documentElement.removeEventListener("mousemove", HMI.reactivateRefreshInterval, false);
		}else if (HMI.svgDocument.detachEvent){
			//Native IE
			HMI.svgDocument.detachEvent("mousemove", HMI.reactivateRefreshInterval);
		}else if(HMI.svgDocument.documentElement.removeEventListener){
			//Renesis Plugin
			HMI.svgDocument.documentElement.removeEventListener("mousemove", HMI.reactivateRefreshInterval, false);
		}
		
		HMI.hmi_log_trace("HMI.prototype.reactivateRefreshInterval - End");
	},
	
	/*********************************
		getComponent
		-	returns the Component which has the cssclass set
	*********************************/
	getComponent: function (evt, cssclass) {
		//This event could be called from native IE < 9, so evt.target is not available
		this.hmi_log_trace("HMI.prototype.getComponent - Start - Target:"+(evt.target ? evt.target.id : evt.srcElement.id));
		
		var Component = null;
		
		//get the Component itself which contained the triggered gesture
		if (evt.target){
			Component = evt.target;
		}else if(evt.srcElement){
			//native IE code
			Component = evt.srcElement;
		}
		while (	Component !== null
				&&	Component !== HMI.svgDocument
				&&	HMI.instanceOf(Component, cssclass) === false)
		{
			if (Component.parentNode.namespaceURI == HMI.HMI_Constants.NAMESPACE_SVG ){
				//W3C conforming Browsers, Adobe, Renesis
				Component = Component.parentNode;
			}else if (Component.parentNode.tagUrn == HMI.HMI_Constants.NAMESPACE_SVG ){
				//Inline IE6-8 Code inline
				Component = Component.parentNode;
			}else{
				//there is no svg parent and we checked this object last loop, break
				Component = null;
			}
			if ( Component !== null && Component.id == HMI.HMI_Constants.NODE_NAME_CLONE){
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
		//This event could be called from nativ IE, so evt.target is not available
		this.hmi_log_trace("HMI.prototype.switchGround - Start, Evt: "+evt.type+", Evt.id: "+(evt.target ? evt.target.id : evt.srcElement.id)+", Evt.name: "+(evt.target ? evt.target.name : evt.srcElement.name)+", Ground: "+ground._node.id);
		
		//is there a move gesture in action?
		if (this._currentDragger !== null)
			this._currentDragger.switchGround(evt, ground);
		
		this.hmi_log_trace("HMI.prototype.switchGround - End");
	},
	
	/*********************************
		_showComponent
	*********************************/	
	_showComponent: function (Component) {
		this.hmi_log_trace("HMI.prototype._showComponent - Start");
		
		if (Component === null){
			//logging not required, already done by refreshSheet
			return;
		}
		
		//if this will be the Sheet (first viewed sheet or comoponent is same as active view)
		if(this.Playground.firstChild === null || this.Playground.firstChild.id === Component.id){
			//set x, y position to zero. A component could be out of view (especially when in embed-Node in IE)
			Component.setAttribute('x', 0);
			Component.setAttribute('y', 0);
		}
		
		this.initGestures(Component);
		//Adobe does not fire mousemove event if there is no rect around the mouse. Build a invisible rect around everything 
		if (this.AdobeMoveFixNeeded){
			var dummyRect = this.svgDocument.createElementNS(this.HMI_Constants.NAMESPACE_SVG, 'rect');
			dummyRect.setAttributeNS(null, 'x', '0');
			dummyRect.setAttributeNS(null, 'y', '0');
			dummyRect.setAttributeNS(null, 'width', Component.getAttribute('width'));
			dummyRect.setAttributeNS(null, 'height', Component.getAttribute('height'));
			dummyRect.setAttributeNS(null, 'style', 'opacity:0;');
			Component.insertBefore(dummyRect, Component.firstChild);
			dummyRect = null;
			this.hmi_log_trace("HMI.prototype._showComponent - Fix for Adobe mousemove Bug enabled.");
		}
		this.hmi_log_trace("HMI.prototype._showComponent: now Playground.append/replaceChild");
		
		if(this.PlaygroundContainerNode){
			//the displayed size is calculated from the Container-Node in the html, so we correct the dimension of it
			this.PlaygroundContainerNode.setAttribute('height', Component.getAttribute('height'));
			this.PlaygroundContainerNode.setAttribute('width', Component.getAttribute('width'));
		}
		
		if(this.Playground.firstChild === null){
			//we have no display => append
			this.Playground.appendChild(Component);
		}else if(this.Playground.firstChild !== null){
			//we have a display => replace
			this.Playground.replaceChild(Component, this.Playground.firstChild);
		}
		Component = null;
		ComponentText = null;
		
		this.hmi_log_trace("HMI.prototype._showComponent - End");
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
			if (null === Element.getAttribute('pointer-events') || "" === Element.getAttribute('pointer-events')){
				Element.setAttribute('pointer-events', 'all');
			}
			
			//calculate and save absolute offset of the Components
			this._setLayerPosition(Element);
		}
		
		//	GROUND
		//
		if (this.instanceOf(Element, "hmi-component-ground"))
		{
			var ground = new Ground(Element, this);
			this.hmi_log_trace("HMI.prototype._initGestures - Ground initialised: "+Element.id);
		};
		
		//	MOVE
		//
		if (this.instanceOf(Element, "hmi-component-gesture-move"))
		{
			//initialize drag&drop handling
			var dragger = new Dragger(Element, this);
			if (HMI.PluginVendor == 'Adobe')
			{
				//Adobe needs a hack for the move-gesture
				HMI.AdobeMoveFixNeeded = true;
			}
			this.hmi_log_trace("HMI.prototype._initGestures - Move initialised: "+Element.id);
		//the move handler calls the other clicks by himself if no movement was detected
		//so there is no further initialisation of move-gesture components needed
		} else {
			//	RIGHTCLICK
			//
			if (this.instanceOf(Element, 'hmi-component-gesture-rightclick'))
			{
				//initialize click handling
				var rightclick = new RightClick(Element, this);
				this.hmi_log_trace("HMI.prototype._initGestures - RightClick initialised: "+Element.id);
			};
			
			//	(LEFT)CLICK
			//
			if (this.instanceOf(Element, 'hmi-component-gesture-click'))
			{
				//initialize click handling
				var click = new Click(Element, this);
				this.hmi_log_trace("HMI.prototype._initGestures - Click initialised: "+Element.id);
			};
			
			//	DOUBLECLICK
			//
			if (this.instanceOf(Element, 'hmi-component-gesture-doubleclick'))
			{
				//initialize click handling
				var doubleclick = new DoubleClick(Element, this);
				this.hmi_log_trace("HMI.prototype._initGestures - DoubleClick initialised: "+Element.id);
			};
			
			//	TEXTINPUT
			//
			if (this.instanceOf(Element, 'hmi-component-gesture-textinput'))
			{
				//initialize textinput handling
				var textinput = new TextInput(Element, this);
				this.hmi_log_trace("HMI.prototype._initGestures - TextInput initialised: "+Element.id);
			};
			
			//	WHEEL-SCROLL
			//
			//wheelsupport is not supported by the HMI Team and probably firefox only
			if (this.instanceOf(Element, 'hmi-component-gesture-wheelscroll'))
			{
				//initialize wheel handling
				this.wheelscroll = new WheelScroll(Element, this);
				this.hmi_log_trace("HMI.prototype._initGestures - WheelScroll initialised: "+Element.id);
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
		var ElementLength;
		var idx;
		
		HMI._initGestures(Fragment);
		// _initGesture does no recursive init, therefor this is done here
		
		if (HMI.SVGPlugin){
			//getElementsByTagNameNS in Adobe is often not complete
			var ChildNodesLength = Fragment.childNodes.length;
			for (idx = 0; idx < ChildNodesLength; ++idx){
				if (Fragment.childNodes.item(idx).tagName == "svg:svg"){
					//recursive init necessary
					HMI.initGestures(Fragment.childNodes.item(idx));
				}
			}
			ChildNodesLength = null;
		}else if (Fragment.getElementsByTagNameNS){   //gecko, opera, webkit, ie9
			Elements = Fragment.getElementsByTagNameNS(HMI.HMI_Constants.NAMESPACE_SVG, 'svg');
			ElementLength = Elements === null? 0 : Elements.length;
			for (idx = 0; idx < ElementLength; ++idx){
				//trace log deactivated, causes performanceproblem in a production system
				//HMI.hmi_log_trace("HMI.initGestures - idx: "+ idx +" Element.length: "+ ElementLength);
				
				//recursive init not necessary, getElementsByTagNameNS is recursive by itself
				HMI._initGestures(Elements[idx]);
			}
		}else{   // IE<9 if svg inline, useless since svg inline does not work with gestures
			Elements = Fragment.getElementsByTagName('svg');
			ElementLength = Elements === null? 0 : Elements.length;
			for (idx = 0; idx < ElementLength; ++idx){
				//trace log deactivated, causes performanceproblem in a production system
				//HMI.hmi_log_trace("HMI.initGestures - idx: "+ idx +" Element.length: "+ ElementLength);
				
				if (Elements[idx].tagUrn == HMI.HMI_Constants.NAMESPACE_SVG){
					HMI._initGestures(Elements[idx]);
				}
			}
		}
		ElementLength = null;
		Elements = null;
		idx = null;
		
		Elements = null;
		
		this.hmi_log_trace("HMI.prototype.initGestures - End");
	},
	
	/*********************************
		instanceOf
		
		checks if a svg element is instanceOf a class
	*********************************/
	instanceOf: function (Node, ClassName) {
		
		//trace log deactivated, causes too much noise and performanceproblem in a production system
//		this.hmi_log_trace("HMI.prototype.instanceOf - Start");
		
		//html5 classList saves us much work
		if (Node.classList && Node.classList.contains){
			return Node.classList.contains(ClassName);
		}
		
		var idx;
		var Classes = Node.getAttribute("class");
		//conform to the standard would be "", but browser return null. not tested both because of speed
		if (Classes === null){
			return false;
		}
		Classes = Classes.split(" ");
		for (idx = Classes.length -1; idx >= 0; idx--){
			if (ClassName === Classes[idx]){
//				this.hmi_log_trace("HMI.prototype._instanceOf - Endt");
				return true;
			}
		}
		
//		this.hmi_log_trace("HMI.prototype.instanceOf - Endf");
		
		return false;
	},
	
	/*********************************
		_setLayerPosition
	*********************************/
	_setLayerPosition: function (Element) {
		//this.hmi_log_trace("HMI.prototype._setLayerPosition - Start: "+Element.id);
		
		//LayerX and LayerY are HMI specific DOM Attributes!
		//They are ignored by the SVG Renderer but used for position calculation in the move gesture
		//and the function displaygestureReactionMarker
		var LayerX = parseInt(Element.getAttribute("x"), 10);
		var LayerY = parseInt(Element.getAttribute("y"), 10);
		
		if (isNaN(LayerX)){
			LayerX = 0;
		}
		if (isNaN(LayerY)){
			LayerY = 0;
		}
		
		if(Element.parentNode !== null && Element.parentNode.namespaceURI == HMI.HMI_Constants.NAMESPACE_SVG){
			newLayerX = parseInt(Element.parentNode.getAttribute("layerX"), 10);
			newLayerY = parseInt(Element.parentNode.getAttribute("layerY"), 10);
			LayerX += newLayerX;
			LayerY += newLayerY;
		}
		
		if (isNaN(LayerX) || isNaN(LayerY)){
			this.hmi_log_warning("SVG-ERROR - parentNode of\n"+Element.id+"\n is no hmi-component (has no layerX or layerY). The move-Gesture will not work on child elements!");
			Element.setAttribute("layerX", 0);
			Element.setAttribute("layerY", 0);
		}else{
			Element.setAttribute("layerX", LayerX);
			Element.setAttribute("layerY", LayerY);
		}
		LayerX = null;
		LayerY = null;
		
		//this.hmi_log_trace("HMI.prototype._setLayerPosition - End");
	},	
	
	/*********************************
		getClickPosition
	*********************************/
	getClickPosition: function (evt, Component) {
		this.hmi_log_trace("HMI.prototype.getClickPosition - Start");
		
		var clickPosition = new Array(2);
		if (!evt){
			clickPosition[0] = Number.NaN;
			clickPosition[1] = Number.NaN;
			this.hmi_log_error("HMI.prototype.getClickPosition - called without an event!");
			return clickPosition;
		}
		var mousePosX;
		var mousePosY;
		if (Component !== null)
		{
			//detect the mouse position relative to the component
			//see technology paper "Human Model Interface - JavaScript" for full details of crossbrowser problems
			
			//First we have to find the offset of the svg-element in the XHTML
			var obj = HMI.Playground;
			var svgOffsetX = 0;
			var svgOffsetY = 0;
			//The Plugin in IE has no offsetParent, but clientX is relative to its own scope, not HTML page
			if (obj.offsetParent) {
				//code for native SVG. Loop upwards till there is no parent
				do {
					svgOffsetX += obj.offsetLeft;
					svgOffsetY += obj.offsetTop;
				} while (obj = obj.offsetParent);
			}
			
			if (evt.pageX || evt.pageY) {
				//code for native SVG. pageX based on the full XHTML Document
				mousePosX = evt.pageX;
				mousePosY = evt.pageY;
			}else if (evt.clientX || evt.clientY) {
				//code for plugin. clientX is based on the Plugin area, without browser scrolling sideeffects
				mousePosX = evt.clientX;
				mousePosY = evt.clientY;
			}else{
				mousePosX = Number.NaN;
				mousePosY = Number.NaN;
			}
			
			//the searched position is pageX/clientX minus Position of the HMI Component minus Position of the SVG
			clickPosition[0] = mousePosX - parseInt(Component.getAttribute("layerX"), 10) - svgOffsetX;
			clickPosition[1] = mousePosY - parseInt(Component.getAttribute("layerY"), 10) - svgOffsetY;
			
			mousePosX = null;
			mousePosY = null;
			svgOffsetX = null;
			svgOffsetY = null;
			obj = null;
			this.hmi_log_trace("HMI.prototype.getClickPosition relative - End x:"+clickPosition[0]+" y:"+clickPosition[1]);
		}else{
			if (evt.touches && evt.touches.length) {
				// iPhone (but no other touch devices)
				clickPosition[0] = evt.touches[0].pageX;
				clickPosition[1] = evt.touches[0].pageY;
			}else if (evt.pageX || evt.pageY) {
				//code for native SVG. pageX based on the full XHTML Document
				clickPosition[0] = evt.pageX;
				clickPosition[1] = evt.pageY;
			}else if (evt.clientX || evt.clientY) {
				//code for plugin. clientX is based on the Plugin area, without browser scrolling sideeffects
				clickPosition[0] = evt.clientX;
				clickPosition[1] = evt.clientY;
			}else{
				clickPosition[0] = Number.NaN;
				clickPosition[1] = Number.NaN;
			}
			this.hmi_log_trace("HMI.prototype.getClickPosition absolute - End x:"+clickPosition[0]+" y:"+clickPosition[1]);
		}
		
		return clickPosition;
	},
	
	/*********************************
		unload
	*********************************/
	unload: function () {
		if (HMI && HMI.KSClient){
			HMI.KSClient.destroy();
		}
	},
	
	/********************************************************************
		logging
			requires console (Firebug, Opera, Safari...)
	********************************************************************/
	
	/*********************************
		hmi_log_debug
	*********************************/
	hmi_log_debug: function (text) {
		if (this.debug === true){
			if(window.console && window.console.debug){
				window.console.debug("HMI_DEBUG: "+text);
			}else if(window.opera && window.opera.postError){
				window.opera.postError("HMI_DEBUG: "+ text);
			}else if(window.console && window.console.info){
				//some consoles (like ie9) does not implement debug but info
				window.console.info("HMI_DEBUG: "+text);
			}
		}
	},
	
	/*********************************
		hmi_log_error
	*********************************/
	hmi_log_error: function (text) {
		if (this.error === true){
			if(window.console && window.console.error){
				window.console.error("HMI_ERROR: "+text);
			}else if(window.opera && window.opera.postError){
				window.opera.postError("HMI_ERROR: "+ text);
			}
		}
	},
	
	/*********************************
		hmi_log_warning
	*********************************/
	hmi_log_warning: function (text) {
		if (this.warning === true){
			if(window.console && window.console.warn){
				window.console.warn("HMI_WARNING: "+text);
			}else if(window.opera && window.opera.postError){
				opera.postError("HMI_WARNING: "+ text);
			}
		}
	},
	
	/*********************************
		hmi_log_info
	*********************************/
	hmi_log_info: function (text) {
		if (this.info === true){
			if(window.console && window.console.info){
				window.console.info("HMI_INFO: "+text);
			}else if(window.opera && window.opera.postError){
				opera.postError("HMI_INFO: "+ text);
			}
		}
	},
	
	/*********************************
		hmi_log_trace
	*********************************/
	hmi_log_trace: function (text) {
		if (this.trace === true){
			if(window.console && window.console.debug){
				window.console.debug("HMI_TRACE: "+text);
			}else if(window.opera && window.opera.postError){
				opera.postError("HMI_TRACE: "+ text);
			}else if(window.console && window.console.info){
				window.console.info("HMI_TRACE: "+text);
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
		//show header
		HMI.hideHeader(false);
	},
	
	/*********************************
		hmi_log_info_onwebsite
	*********************************/
	hmi_log_info_onwebsite: function (text) {
		if (this.InfoOutput){
			var target = this.InfoOutput;
			var targetText = "";
			//do we have an old output right now?
			if (target.childNodes.length !== 0){
				if (typeof target.textContent != "undefined"){
					targetText = target.textContent;
				}else if (typeof target.innerText != "undefined"){
					targetText = target.innerText;
				}
				if (targetText.indexOf(text) !== -1){
					//duplication of error message
					return;
				}
				//new message, build newline
				target.appendChild(document.createElement('br'));
			}
			target.appendChild(document.createTextNode(text));
			//show header
			HMI.hideHeader(false);
		}
	}
};

/*********************************
	Global Variables
	
	HMI(debug, error, warning, info, trace)
*********************************/
var HMI;
if (window.location.search && -1 != unescape(window.location.search).indexOf("trace=true")){
	HMI = new HMI(true, true, true, true, true);
}else{
	HMI = new HMI(true, true, true, true, false);
}

//init HMI after all js-files are loaded (not guaranteed at construction time)
if( window.addEventListener ) {
	//window is the wrong place for the eventlistener, but available at the most browsers
	//http://www.howtocreate.co.uk/tutorials/javascript/domevents
	window.addEventListener('load',function(){HMI.init();},false);
} else if( document.addEventListener ) {
	//document is the right place for the eventlistener
	//but not supported by mozilla https://bugzilla.mozilla.org/show_bug.cgi?id=99820
	//and Webkit
	document.addEventListener('load',function(){HMI.init();},false);
} else if( window.attachEvent ) {
	//Internet Explorer < 9 is a special case as usual
	window.attachEvent('onload',function(){HMI.init();});
}

//fallback for stupid browsers (mostly Google Chrome) which fires onload to early, so our init code is never called
//
window.setTimeout(function(){HMI.init();}, 1000);

var filedate = "$Date$";
filedate = filedate.substring(7, filedate.length-2);
if ("undefined" == typeof HMIdate){
	HMIdate = filedate;
}else if (HMIdate < filedate){
	HMIdate = filedate;
}
