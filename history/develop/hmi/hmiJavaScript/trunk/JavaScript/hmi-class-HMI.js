/*
*	Copyright (C) 2012
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

/**
 * initialises HMI object
 * @param {boolean} debug log debug output to the console
 * @param {boolean} error log errors to the console
 * @param {boolean} warning log warning to the console
 * @param {boolean} info log info output to the console
 * @param {boolean} trace log trace information to the console
 * @constructor
 */
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
	this.cyclicRequestNeeded = true;
	
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

/**
 * prototype
 */
HMI.prototype = {
	/**
	 * init
	 * called from "load" event, when all js files are available
	 */
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
				addEventSimple(HMI.PossServers, "change", 
					function () {
						HMI.showSheets(HMI.getHostname(), HMI.PossServers.options[HMI.PossServers.selectedIndex].value);
					}
				);
			}
			//Object of Sheet-Selectbox
			if (!(this.PossSheets = $('idSheets'))){
				ErrorDetail += "HTML Select with the ID: idShowServers not found.\n";
			}else{
				addEventSimple(HMI.PossSheets, "change", 
					function () {
						HMI.showSheet(HMI.getHostname(), HMI.PossServers.options[HMI.PossServers.selectedIndex].value, HMI.PossSheets.options[HMI.PossSheets.selectedIndex].value);
					}
				);
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
			addEventSimple($('idRefresh'),'click',function(){HMI.cbrefreshSheet();});
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
				HMI.RefreshTimeoutID = window.setTimeout(function () {HMI.cbrefreshSheet();}, HMI.InputRefreshTime.value);});
		}
		
		//Object of InfoOutput, optional, not necessary
		if ((this.InfoOutput = $('idInfoOutput'))){
			deleteChilds(HMI.InfoOutput);
		}
		
		// prepare smooth hiding
		HMI.HideableHeader.style.overflow = "hidden";
		HMI.HideableHeader.style.maxHeight = HMI.HideableHeader.offsetHeight+"px";
		
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
			var dateTextNode = document.createTextNode("Version: 2.5 ("+HMI.HMI_Constants.HMIdate.substr(0, 10).replace(/\//g, "-")+")");
			
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
	
	/**
	 * parse url Parameter and loads the requested sheets
	 */
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
			if (HMI_Parameter_Liste.Host && HMI_Parameter_Liste.Host.length !== 0){
				HMI.InputHost.value = HMI_Parameter_Liste.Host;
			}
			//request sanitised name
			var Host = this.getHostname();
			
			//correct RefreshTime in website with user wish
			if (HMI_Parameter_Liste.RefreshTime && HMI_Parameter_Liste.RefreshTime.length !== 0){
				HMI.InputRefreshTime.value = HMI_Parameter_Liste.RefreshTime;
			}
			
			//correct ShowComponents Status in website with user wish
			if (HMI_Parameter_Liste.ShowComp && HMI_Parameter_Liste.ShowComp.length !== 0){
				if (HMI_Parameter_Liste.ShowComp == "true" && $("idShowcomponents")){
					$("idShowcomponents").checked = true;
				}
			}
			HMI.ChangeRefreshTime();
			
			//a server and sheet is specified in "deep link" (host is always there)
			if (	HMI_Parameter_Liste.Server
				&&	HMI_Parameter_Liste.Server.length !== 0
				&&	HMI_Parameter_Liste.Sheet
				&&	HMI_Parameter_Liste.Sheet.length !== 0)
			{
				HMI.ButShowServers.value = "Loading Sheet...";
				
				//clean old Server and Sheet entrys, an old SVG display and displayed errors in website
				deleteChilds(this.PossServers);
				deleteChilds(this.PossSheets);
				deleteChilds(this.Playground);
				deleteChilds(this.ErrorOutput);
				
				HMI.PossServers.options[0] = new Option('- list not loaded -', HMI_Parameter_Liste.Server);
				HMI.PossSheets.options[0] = new Option('- list not loaded -', HMI_Parameter_Liste.Sheet);
				
				//deactivate the Select-Boxes, because there is no usefull content
				HMI.PossServers.disabled = true;
				HMI.PossSheets.disabled = true;
				
				//the path of the HMI Manager could be different in every OV Server (manager needed for all gestures)
				this.KSClient.getHMIManagerPointer(Host, HMI_Parameter_Liste.Server);
				//spaces in objectname are encoded as %20 within OV
				HMI.showSheet(Host, HMI_Parameter_Liste.Server, encodeURI(HMI_Parameter_Liste.Sheet));
				
				HMI.ButShowServers.value = "Reload Serverlist";
			}else if (	HMI_Parameter_Liste.Server
				&&	HMI_Parameter_Liste.Server.length !== 0){
				//only a server is specified in "deep link" => showSheets
				
				HMI.ButShowServers.value = "Loading Sheetlist...";
				HMI.PossServers.options[0] = new Option('- list not loaded -', HMI_Parameter_Liste.Server);
				HMI.PossSheets.options[0] = new Option('- list not loaded -', '');
				
				HMI.showSheets(Host, HMI_Parameter_Liste.Server);
				
				HMI.ButShowServers.value = "Reload Serverlist";
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
		
		if (document.getElementById("idThrobbler") !== null){
			document.getElementById("idThrobbler").style.display = "none";
		}
		
		this.hmi_log_trace("HMI.prototype.interpreteUrlParameter - End");
		return;
	},
	
	/**
	 * update deep link and HTML5 history
	 */
	updateDeepLink: function (){
		var newDeepLink = window.location.pathname.substring(0, window.location.pathname.lastIndexOf("/")+1);
		newDeepLink += "?RefreshTime="+HMI.RefreshTime;
		if(HMI.InputHost.value !== ""){
			newDeepLink += "&Host="+HMI.InputHost.value;
		}
		if(HMI.PossServers.selectedIndex >= 0 && HMI.PossServers.options[HMI.PossServers.selectedIndex].value !== ""){
			newDeepLink += "&Server="+HMI.PossServers.options[HMI.PossServers.selectedIndex].value;
		}
		if(HMI.PossSheets.selectedIndex >= 0 && HMI.PossSheets.options[HMI.PossSheets.selectedIndex].value !== ""){
			newDeepLink += "&Sheet="+HMI.PossSheets.options[HMI.PossSheets.selectedIndex].value;
		}
		if (HMI.trace === true){
			newDeepLink += "&trace=true";
		}
		if ($("idShowcomponents") && $("idShowcomponents").checked){
			newDeepLink += "&ShowComp=true";
		}
		$("idBookmark").href = newDeepLink;
		
		//add sheet to html5 Session history management, if new
		if (window.history.pushState){
			if (newDeepLink != window.location.href){
				window.history.pushState("", "", newDeepLink);
			}
		}else{
			//show deeplink if history management is not available
			$("idBookmark").style.display = "inline";
		}
	},
	
	/**
	 * shows a rectangle for 800ms to provide visual feedback
	 * called after sending a gesture request
	 */
	displaygestureReactionMarker: function (Component){
		this.hmi_log_trace("HMI.prototype.displaygestureReactionMarker");
		
		if (	Component.getAttribute('width') === null ||
				Component.getAttribute('height') === null ||
				Component.getAttribute('LayerX') === 0 ||
				Component.getAttribute('LayerY') === 0){
			return;
		}
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
	
	/**
	 * saves state of input checkbox in a js variable
	 * called with a onclick handler of the checkbox and in HMI.init
	 */
	updateKeepHeader: function (){
		this.hmi_log_trace("HMI.prototype.updateKeepHeader - change requested");
		if (document.getElementById("idKeepHeader").checked === true) {
			HMI.autoKeepHeader = true;
		}else{
			HMI.autoKeepHeader = false;
		}
	},
	
	/**
	 * hides or toggles header
	 * @param {boolean} show hide header if false, hide if false, toggle if null
	 */
	hideHeader: function(hide){
		if (hide === true || (hide === null && HMI.HeaderIsVisible === true)){
			//hide menu
			
			//if there is support for CSS transition use it
			//transition from an height="auto" is not supported, so we change maxHeight
			if(HMI.HideableHeader.style.MozTransitionDuration !== undefined){
				HMI.HideableHeader.style.MozTransitionProperty = "all";
				HMI.HideableHeader.style.MozTransitionTimingFunction = "ease-in-out";
				HMI.HideableHeader.style.MozTransitionDuration = "0.6s";
				HMI.HideableHeader.style.maxHeight = '0px';
				HMI.HeaderIsVisible = false;
			}else if(HMI.HideableHeader.style.WebkitTransitionDuration !== undefined){
				HMI.HideableHeader.style.WebkitTransitionProperty = "all";
				HMI.HideableHeader.style.WebkitTransitionTimingFunction = "ease-in-out";
				HMI.HideableHeader.style.WebkitTransitionDuration = "0.6s";
				HMI.HideableHeader.style.maxHeight = '0px';
				HMI.HeaderIsVisible = false;
			}else if(HMI.HideableHeader.style.OTransitionDuration !== undefined){
				HMI.HideableHeader.style.OTransitionProperty = "all";
				HMI.HideableHeader.style.OTransitionTimingFunction = "ease-in-out";
				HMI.HideableHeader.style.OTransitionDuration = "0.6s";
				HMI.HideableHeader.style.maxHeight = '0px';
				HMI.HeaderIsVisible = false;
			}else if(HMI.HideableHeader.style.MSTransitionDuration !== undefined){
				HMI.HideableHeader.style.MSTransitionProperty = "all";
				HMI.HideableHeader.style.MSTransitionTimingFunction = "ease-in-out";
				HMI.HideableHeader.style.MSTransitionDuration = "0.6s";
				HMI.HideableHeader.style.maxHeight = '0px';
				HMI.HeaderIsVisible = false;
			}else if(HMI.HideableHeader.style.transitionDuration !== undefined){
				HMI.HideableHeader.style.transitionProperty = "all";
				HMI.HideableHeader.style.transitionTimingFunction = "ease-in-out";
				HMI.HideableHeader.style.transitionDuration = "0.6s";
				HMI.HideableHeader.style.maxHeight = '0px';
				HMI.HeaderIsVisible = false;
			}else{
				//simulate transition with an own hiding function
				
				//	iterate hiding function
				//
				HMI.smoothHeaderTimeID = window.setTimeout(this.smoothHeaderHide, 30);
			}
			
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
			HMI.HideableHeader.style.maxHeight = "300px";
			
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
	
	/**
	 * reduces height of header
	 * recall itself if header is still visible
	 */
	smoothHeaderHide: function(){
		var HeaderActualheight = parseInt(HMI.HideableHeader.offsetHeight, 10);
		if (HeaderActualheight <= 30){
			// last iteration, complete hiding
			//
			HMI.HideableHeader.style.maxHeight = '1px';	// ie < v8 renders 0px like fully visible
			HMI.HeaderIsVisible = false;
		}else{
			//adjust header height and call us in a few milli seconds again
			HeaderActualheight -= 30;
			HMI.HideableHeader.style.maxHeight = HeaderActualheight + 'px';
			window.setTimeout(HMI.smoothHeaderHide, 30);
		}
	},
	
	/**
	 * handles a refreshtime input change
	 */
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
		
		//present a "deep link" to the state
		this.updateDeepLink();
		
		//if an auto refresh is active, reset to new value
		if (HMI.RefreshTimeoutID !== null){
			HMI.cbrefreshSheet();
			HMI.hmi_log_info_onwebsite('Refreshtime set to '+(HMI.RefreshTime/1000)+'s.');
		}
	},
	
	/**
	 * returns the correct hostname
	 */
	getHostname: function () {
		//get rid of spaces in the textinput
		HMI.InputHost.value = HMI.InputHost.value.trim();
		if (HMI.InputHost.value.length === 0){
			HMI.InputHost.value = window.location.hostname;
		}
		
		//if the hostname is identical to the http server, translate into "localhost"
		var Host;
		if (HMI.InputHost.value == window.location.hostname){
			Host = "localhost";
		}else{
			Host = HMI.InputHost.value;
		}
		return Host;
	},
	
	/**
	 * clears active display and loads a new serverlist from host
	 */
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
		
		this.KSClient.getServers(this.getHostname());
		
		
		//todo move this to call back
		
		//reenable click by user
		HMI.ButShowServers.disabled = false;
		HMI.ButShowServers.value = "Reload Serverlist";
		if (document.getElementById("idThrobbler") !== null){
			document.getElementById("idThrobbler").style.display = "none";
		}
		
		//present a "deep link" to the state
		this.updateDeepLink();
		
		//correct magellan URL
		//
		if (this.WebmagellanPath !== null){
			$("idWebmagellan").style.display = '';
			//todo gefälscher host dings
			$("idWebmagellan").href = HMI.WebmagellanPath + (HMI.InputHost.value.length !== 0 ? '?cmd=start&arg1='+this.getHostname() : "");
		}
		
		this.hmi_log_trace("HMI.prototype.showServers - End");
	},
	
	/**
	 * clears active display and loads a new sheetlist from server
	 * @param {String} Server Name of server to ask for sheets
	 */
	showSheets: function (Host, Server) {
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
		if (Server === ''){
			HMI.PossSheets.options[0] = new Option('please select Server first', '');
			return false;
		}
		
		var SheetList = this.KSClient.getSheets(Host, Server);
		
		this.hmi_log_trace("HMI.prototype.showSheets - number of sheets: "+SheetList.length);
		if (SheetList.length === 0){
			this.PossSheets.options[0] = new Option('- no sheets available -', '');
			return false;
		} else {
			HMI.PossSheets.options[HMI.PossSheets.options.length] = new Option('- select sheet -', '');
			for (i = 0; i < SheetList.length; i++){
				//spaces in objectname are encoded as %20 within OV
				HMI.PossSheets.options[HMI.PossSheets.options.length] = new Option(decodeURI(SheetList[i]), SheetList[i]);
			}
			if (SheetList.length === 1){
				//selecting the option does not trigger the EventListener
				HMI.PossSheets.selectedIndex = 1;
				HMI.showSheet(Host, Server, SheetList[0]);
			}
			HMI.PossSheets.disabled = false;
		}
		SheetList = null;
		
		//present a "deep link" to the state
		this.updateDeepLink();
		
		this.hmi_log_trace("HMI.prototype.showSheets - End");
		return true;
	},
	
	/**
	 * clears active display and loads a new sheet from server
	 * @param {String} Sheet Name of sheet to load
	 */
	showSheet: function (Host, Server, Sheet) {
		this.hmi_log_trace("HMI.prototype.showSheet - Start with Sheet: "+Sheet);
		
		//present a "deep link" to the state
		this.updateDeepLink();
		
		//clean an old SVG display and displayed errors in website
		deleteChilds(this.Playground);
		deleteChilds(this.ErrorOutput);
		deleteChilds(this.InfoOutput);
		$("idBookmark").style.display = "none";
		
		window.clearTimeout(HMI.RefreshTimeoutID);
		HMI.RefreshTimeoutID = null;
		
		//nothing selected
		if (Sheet === ''){
			return;
		}
		
		//[StyleDescription] remove this line if no ACPLT/HMI Server has a StyleDescription anymore
		HMI.KSClient.checkSheetProperty(Host, Server, Sheet);
		
		this.refreshSheet(Host, Server, Sheet);
		
		//show fb-server name when known
		//spaces in objectname are encoded as %20 within OV
		document.title = Server+decodeURI(Sheet)+" - ACPLT/HMI";
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
		
		HMI.ButShowServers.value = "Reload Serverlist";
		if (document.getElementById("idThrobbler") !== null){
			document.getElementById("idThrobbler").style.display = "none";
		}
		
		this.hmi_log_trace("HMI.prototype.showSheet - End");
	},
	
	/**
	 * gets a new sheet from a server and displays it
	 */
	refreshSheet: function (Host, Server, Sheet) {
		this.hmi_log_trace("HMI.prototype.refreshSheet - Start");
		
		window.clearTimeout(HMI.RefreshTimeoutID);
		HMI.RefreshTimeoutID = null;
		
		if (this.InfoOutput){
			deleteChilds(this.InfoOutput);
		}
		var skipRefresh = false;
		//check if the page is visible at all?
		//http://www.w3.org/TR/page-visibility/
		if(	document.hidden === true ||
			document.mozHidden === true||
			document.webkitHidden === true||
			document.msHidden === true||
			document.oHidden === true
		){
			var skipRefresh = true;
		}
		//load hmi if playground is empty or with empty view (load in background)
		if (skipRefresh === false || HMI.Playground.childNodes.length === 0){
			var SVGRequestURI = "";
			
			//[StyleDescription] remove this if no ACPLT/HMI Server has a StyleDescription anymore
			if (HMI.ServerProperty.SheetHasStyleDescription){
				//spaces in objectname are encoded as %20 within OV
				SVGRequestURI = '{' + encodeURI(Sheet) + '.GraphicDescription' + '%20' + encodeURI(Sheet) + '.StyleDescription' + '}';
			}else if (HMI.KSClient.HMIMANAGER_PATH !== null){
				//spaces in objectname are encoded as %20 within OV
				SVGRequestURI = encodeURI(Sheet) + '.GraphicDescription';
			}
			
			var ComponentText = null;
			if (SVGRequestURI !== ""){
				//	get GraphicDescription
				//
				ComponentText = this.KSClient.getVar_NG("//"+Host+"/"+Server+SVGRequestURI, null);
			}
			
			//check if we have an cshmi target, no hmimanager at all or an error
			if (ComponentText === null || (ComponentText && ComponentText.indexOf("KS_ERR_BADPATH") !== -1)){
				//save for later use
				this.KSClient.ResourceList.ModelHost = Host;
				this.KSClient.ResourceList.ModelServer = Server;
				
				this.cshmi = new cshmi();
				this.cshmi.instanciateCshmi(Host, Server, Sheet);
				HMI.cyclicRequestNeeded = false;
			}else{
				//hmi target
				var SplitComponent = this.KSClient.prepareComponentText(ComponentText);
				if (SplitComponent === null){
					//logging not required, already done by prepareComponentText
					return;
				}
				
				//[StyleDescription] adjust this line if no ACPLT/HMI Server has a StyleDescription anymore
				//build a DOM fragment with the SVG String
				var Component = this.HMIDOMParser.parse(SplitComponent[0], SplitComponent[1]);
				
				if (Component != null){
					this._showComponent(Host, Server, Component);
				}
				
				HMI.cyclicRequestNeeded = true;
				
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
			}
			
			SVGRequestURI = null;
			ComponentText = null;
		}
		if(HMI.cyclicRequestNeeded == true){
			//the Host, Server and Path has to be cached for use in an eventhandler
			HMI.KSClient.ResourceList.Host = Host;
			HMI.KSClient.ResourceList.Server = Server;
			HMI.KSClient.ResourceList.Sheet = Sheet;
			
			//	refresh the sheet again later
			this.RefreshTimeoutID = window.setTimeout(function () {HMI.refreshSheet(Host, Server, Sheet);}, this.RefreshTime);
		}
		
		this.hmi_log_trace("HMI.prototype.refreshSheet - End");
	},
	
	/**
	 * request a reload of the active sheet
	 * The caller does not know the Host, Server, Sheet so we detect it
	 */
	cbrefreshSheet: function (client, req) {
		HMI.hmi_log_trace("HMI.prototype.cbrefreshSheet - Start");
		
		HMI.refreshSheet(HMI.getHostname(), HMI.PossServers.options[HMI.PossServers.selectedIndex].value, HMI.PossSheets.options[HMI.PossSheets.selectedIndex].value);
		
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
			HMI.RefreshTimeoutID = window.setTimeout(function () {HMI.cbrefreshSheet();}, HMI.RefreshTime);
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
	_showComponent: function (Host, Server, Component) {
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
			//save for later use
			this.KSClient.ResourceList.ModelHost = Host;
			this.KSClient.ResourceList.ModelServer = Server;
			
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
	
	/**
	 * checks if a svg element is instanceOf a class
	 */
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
	
	/**
	 * adds a class to a svg element
	 */
	addClass: function(Node, additionalClass){
		if (Node.classList && Node.classList.add){
			//html5 classList saves us some work
			Node.classList.add(additionalClass);
		}else if (Node.className !== undefined){
			Node.className.baseVal = (Node.className.baseVal+" "+additionalClass).trim();
		}else{
			Node.setAttribute('class', (Node.getAttribute('class')+ " "+additionalClass).trim());
		}
	},
	
	/*********************************
		_setLayerPosition
	*********************************/
	_setLayerPosition: function (Element, reportSize) {
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
		
		if (reportSize === true){
			var LayerWidth = parseInt(Element.getAttribute("width"), 10);
			var LayerHeight = parseInt(Element.getAttribute("height"), 10);
			
			if (isNaN(LayerWidth)){
				LayerWidth = 0;
			}
			if (isNaN(LayerHeight)){
				LayerHeight = 0;
			}
			return new Array(LayerX+LayerWidth, LayerY+LayerHeight);
		}
		
		return;
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
	
	/**
	 * prints debug output on the console
	 */
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
	
	/**
	 * prints error output on the console
	 */
	hmi_log_error: function (text) {
		if (this.error === true){
			if(window.console && window.console.error){
				window.console.error("HMI_ERROR: "+text);
			}else if(window.opera && window.opera.postError){
				window.opera.postError("HMI_ERROR: "+ text);
			}
		}
	},
	
	/**
	 * prints warning output on the console
	 */
	hmi_log_warning: function (text) {
		if (this.warning === true){
			if(window.console && window.console.warn){
				window.console.warn("HMI_WARNING: "+text);
			}else if(window.opera && window.opera.postError){
				opera.postError("HMI_WARNING: "+ text);
			}
		}
	},
	
	/**
	 * prints info output on the console
	 */
	hmi_log_info: function (text) {
		if (this.info === true){
			if(window.console && window.console.info){
				window.console.info("HMI_INFO: "+text);
			}else if(window.opera && window.opera.postError){
				opera.postError("HMI_INFO: "+ text);
			}
		}
	},
	
	/**
	 * prints trace output on the console
	 */
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
	
	/**
	 * clears error section on the page and prints output on the page
	 */
	hmi_log_onwebsite: function (text) {
		var ErrorTextNode = document.createTextNode(text);
		deleteChilds(HMI.ErrorOutput);
		HMI.ErrorOutput.appendChild(ErrorTextNode);
		//show header
		HMI.hideHeader(false);
	},
	
	/**
	 * prints info output on the page, but prevents dupe message
	 */
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

/**
 * instanciates a new HMI object in the global scope
 * 
 * HMI(debug, error, warning, info, trace)
 */
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

//fallback for stupid browsers (aka Google Chrome v1-v8 or so) which fires onload to early, so our init code is never called
//
window.setTimeout(function(){HMI.init();}, 1000);

var filedate = "$Date$";
filedate = filedate.substring(7, filedate.length-2);
if ("undefined" == typeof HMIdate){
	HMIdate = filedate;
}else if (HMIdate < filedate){
	HMIdate = filedate;
}
