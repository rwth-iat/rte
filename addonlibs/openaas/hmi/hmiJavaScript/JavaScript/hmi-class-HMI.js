/*
*	Copyright (C) 2015
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
	//used to mark a visual feedback after a click for example
	this.HMI_Constants.NODE_NAME_REACTIONMARKER = "HMI_REACTIONMARKER";
	//used to mark an object while moving (old hmi!)
	this.HMI_Constants.NODE_NAME_CLONE = "HMI_CLONE";
	
	this.HMI_Constants.HMIdate = null;
	this.HMI_Constants.ServerType = null;
	this.HMI_Constants.UrlParameterList = null;
	
	this.debug = debug;
	//log critical errors to console
	this.error = error;
	this.warning = warning;
	this.info = info;
	//log trace info to console
	this.trace = trace;
	this.cyclicRequestNeeded = false;
	
	this.ButShowServers = null;
	this.InputRefreshTime = null;
	this.InputHost = null;
	this.HideableHeader = null;
	this.PossServers = null;
	this.PossSheets = null;
	this.Playground = null;
	this.ErrorOutput = null;
	this.InfoOutput = null;
	this.Throbber = null;
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
	
	this.RefreshTime = null;
	this.ServerProperty = {SheetHasStyleDescription:null};
	
	this.smoothHeaderTimeID = null;
	this.autoKeepHeader = false;
	this.HeaderIsVisible = true;
	
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
		if(null == document.getElementById('idPlayground')){
			//this is a too early call, the DOM is not ready
			//old presto parses the JS faster than the HTML, so try again later
			document.addEventListener("DOMContentLoaded", function(event) {
				HMI.init();
			});
			return false;
		}
		this.hmi_log_trace("HMI.prototype.init - Start");
		
		var ErrorDetail = "";
		if (typeof Click != "function"){
			ErrorDetail += "hmi-class-gesture-click.js not loaded. ";
		}
		if (typeof DoubleClick != "function"){
			ErrorDetail += "hmi-class-gesture-doubleclick.js not loaded. ";
		}
		if (typeof Dragger != "function"){
			ErrorDetail += "hmi-class-gesture-move.js not loaded. ";
		}
		if (typeof RightClick != "function"){
			ErrorDetail += "hmi-class-gesture-rightclick.js not loaded. ";
		}
		if (typeof TextInput != "function"){
			ErrorDetail += "hmi-class-gesture-textinput.js not loaded. ";
		}
		if (typeof HMIDOMParser != "function"){
			ErrorDetail += "hmi-class-HMIDOMParser.js not loaded. ";
		}
		if (typeof deleteChilds != "function"){
			ErrorDetail += "hmi-generic.js not loaded. ";
		}
		if (typeof HMIJavaScriptKSClient != "function"){
			ErrorDetail += "hmi-class-HMIJavaScriptKSClient.js not loaded. ";
		}
		
		if (typeof cshmi != "function"){
			ErrorDetail += "hmi-class-cshmi.js not loaded. ";
		}
		
		if(ErrorDetail === ""){
			//all js files loaded, continue testing of the website
			
			//Object of ShowServer-Button
			if ((this.ButShowServers = document.getElementById('idShowServers'))){
				addEventSimple(HMI.ButShowServers,'click',function(){HMI.showServers();});
			}else{
				ErrorDetail += "HTML Button with the ID: idShowServers not found. ";
			}
			//Object of ErrorOutput
			if (!(this.ErrorOutput = document.getElementById('idErrorOutput'))){
				ErrorDetail += "HTML Tag with the ID: idErrorOutput not found. ";
			}
			//Object of Server-Selectbox
			if (!(this.PossServers = document.getElementById('idServers'))){
				ErrorDetail += "HTML Select with the ID: idShowServers not found. ";
			}else{
				addEventSimple(HMI.PossServers, "change", 
					function () {
						if(HMI.PossServers.selectedIndex >= 0){
							HMI.showSheets(HMI.getHostname(), HMI.PossServers.options[HMI.PossServers.selectedIndex].value);
						}
					}
				);
			}
			//Object of Sheet-Selectbox
			if (!(this.PossSheets = document.getElementById('idSheets'))){
				ErrorDetail += "HTML Select with the ID: idShowServers not found. ";
			}else{
				addEventSimple(HMI.PossSheets, "change", 
					function () {
						if(HMI.PossSheets.selectedIndex >= 0){
							HMI.showOneSheet(HMI.getHostname(), HMI.PossServers.options[HMI.PossServers.selectedIndex].value, HMI.PossSheets.options[HMI.PossSheets.selectedIndex].value);
						}
					}
				);
			}
			//Element of SVG insertion (div for firefox, container for IE < v9)
			if (!(this.Playground = document.getElementById('idPlayground'))){
				ErrorDetail += "HTML Container-Element with the ID: idPlayground not found. ";
			}else{
				//prevent right click in supported Browsers (no Opera < 10.5, no IE6-8)
				addEventSimple(HMI.Playground, "contextmenu", function (evt) {if (evt.preventDefault) evt.preventDefault();});
			}
			//Object of RefreshTime
			if ((this.InputRefreshTime = document.getElementById('idRefreshTime'))){
				addEventSimple(HMI.InputRefreshTime,'change',function(){HMI.ChangeRefreshTime();});
			}else{
				ErrorDetail += "HTML Input with the ID: idRefreshTime not found. ";
			}
			//Object of Server-Hostname
			if ((this.InputHost = document.getElementById('idHost'))){
				//change is wrong here, since a click on the "reload"-button is a change here and a click there => duplicate action
				addEventSimple(HMI.InputHost, 'keyup',function(evt){
						if (evt.keyCode === 13){	// return key
							HMI.showServers();
						}
					});
			}else{
				ErrorDetail += "HTML Input with the ID: idHost not found. ";
			}
			//Object of the Deep Link
			if (!(document.getElementById('idBookmark'))){
				ErrorDetail += "HTML Input with the ID: idBookmark not found. ";
			}
			//Object of the hideable header
			if (!(this.HideableHeader = document.getElementById('idHideableHeader'))){
				ErrorDetail += "HTML Div with the ID: idHideableHeader not found. ";
			}
			
			//init the plain HTML website with events
			if (document.getElementById('idHeaderRow')){
				addEventSimple(document.getElementById('idHeaderRow'),'click',function(){HMI.hideHeader(null);});
			}else{
				ErrorDetail += "HTML object with the ID: idHeaderRow not found. ";
			}
			if (document.getElementById('idKeepHeader')){
				addEventSimple(document.getElementById('idKeepHeader'),'click',function(){HMI.updateKeepHeader();});
			}else{
				ErrorDetail += "HTML Checkbox with the ID: idKeepHeader not found. ";
			}
			
			//html5 Session history management
			//reload sheet on history navigation event
			window.onpopstate = function(){
				if(window.history.state !== undefined && window.history.state !== null){
					if(	window.history.state.Host !== HMI.InputHost.value ||
							(HMI.PossServers.selectedIndex >= 0 && window.history.state.Server !== HMI.PossServers.options[HMI.PossServers.selectedIndex].value)||
							(HMI.PossSheets.selectedIndex >= 0 && window.history.state.Sheet !== HMI.PossSheets.options[HMI.PossSheets.selectedIndex].value)){
						HMI.interpreteUrlParameter(); 
					}
				}
			};
		}
		
		this.Throbber = document.getElementById('idThrobber');
		
		if(ErrorDetail !== ""){
			if (this.Throbber !== null){
				if(this.Throbber.style){
					this.Throbber.style.display = "none";
				}else if(this.Throbber.setAttribute){
					this.Throbber.setAttribute("display", "none");
				}
			}
			window.alert ("Error initialising HMI Website: "+ErrorDetail);
			return false;
		}
		
		//init the optional Buttons
		if (document.getElementById('idRefresh')){
			addEventSimple(document.getElementById('idRefresh'),'click',function(){HMI.cbrefreshSheet();});
		}
		if (document.getElementById('idStopRefresh')){
			addEventSimple(document.getElementById('idStopRefresh'),'click',function(){
					window.clearTimeout(HMI.RefreshTimeoutID);
					HMI.RefreshTimeoutID = null;
				}
			);
		}
		
		if (document.getElementById('idStartRefresh')){
			addEventSimple(document.getElementById('idStartRefresh'),'click',function(){
				HMI.RefreshTimeoutID = window.setTimeout(function () {HMI.cbrefreshSheet();}, HMI.InputRefreshTime.value);});
		}
		
		//Object of InfoOutput, optional, not necessary
		if ((this.InfoOutput = document.getElementById('idInfoOutput'))){
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
		
		if(window.addEventListener){
			//we need a modern browser inside, so no code for ie < 9
			window.addEventListener("resize", HMI.optimizeSheetSize, false);
			//the feature is not important enough to be supported on browsers with a prefix on the event
			HMI.HideableHeader.addEventListener("transitionend", HMI.optimizeSheetSize, false);
		}
		
		var allowLocalRequests = true;
		//detect if the file is called from http or https, but not from filesystem
		if (window.location.protocol === "file:"){
			HMI.HMI_Constants.ServerType = "kshttp";
			allowLocalRequests = false;
			HMI.InputHost.placeholder = "hostname to contact";
			this.hmi_log_trace("HMI.prototype.init - detected direct file, only kshttp server are available");
		}else if (window.location.search.length !== 0 && -1 !== window.location.search.search(/ServerType=/)){
			if ( -1 !== window.location.search.search(/ServerType=php/)){
				HMI.HMI_Constants.ServerType = "php";
			}else if ( -1 !== window.location.search.search(/ServerType=tcl/)){
				HMI.HMI_Constants.ServerType = "tcl";
			}else if ( -1 !== window.location.search.search(/ServerType=kshttp/)){
				HMI.HMI_Constants.ServerType = "kshttp";
			}
		}else{
			//Try to detect the Servertype (TCL or PHP capable)
			//make a request to sniff the HTTP-Serverstring
			
			//IE sometimes uses a cached version, without server Header, so prevent caching
			var DatePreventsCaching = new Date();
			try{
				var req = new XMLHttpRequest();
				req.open("HEAD", window.location.pathname+'?preventCaching='+DatePreventsCaching.getTime(), false);
				req.send(null);
				var ResponseServerString = req.getResponseHeader('server');
				if (req.readyState != 4){
					//some old mobile browser does not support syncron requests
					this.hmi_log_error("HMI.prototype.init - Communication to Server failed. This website has to be transfered via HTTP. ");
					this.hmi_log_onwebsite("This Browser does not support synchronous XMLHttpRequest, which is required for ACPLT/HMI. Please use another Browser");
					if (this.Throbber !== null){
						if(this.Throbber.style){
							this.Throbber.style.display = "none";
						}else if(this.Throbber.setAttribute){
							this.Throbber.setAttribute("display", "none");
						}
					}
					return false;
				}else if (ResponseServerString && -1 !== ResponseServerString.indexOf('Tcl-Webserver')){
					HMI.HMI_Constants.ServerType = "tcl";
					this.hmi_log_trace("HMI.prototype.init - detected TCL Gateway");
				}else if (ResponseServerString &&  -1 !== ResponseServerString.indexOf('ACPLT/OV HTTP Server')){
					HMI.HMI_Constants.ServerType = "kshttp";
					HMI.InputHost.placeholder = "hostname to contact";
					this.hmi_log_trace("HMI.prototype.init - detected kshttp server");
				}else if (ResponseServerString &&  -1 !== ResponseServerString.indexOf('PHP')){
					HMI.HMI_Constants.ServerType = "php";
					this.hmi_log_trace("HMI.prototype.init - detected PHP Gateway");
				}else if (ResponseServerString &&  -1 !== ResponseServerString.indexOf('Apache')){
					HMI.HMI_Constants.ServerType = "php";
					this.hmi_log_trace("HMI.prototype.init - detected Apache Webserver, so probably a PHP Gateway");
				}
			}catch(e){
				this.hmi_log_error("HMI.prototype.init - Gatewaydetection failed: "+e.message);
				this.hmi_log_onwebsite('Could not detect type of HTTP/KS-Gateway. Please configure in hmi-class-HMI.js');
				if (this.Throbber !== null){
					if(this.Throbber.style){
						this.Throbber.style.display = "none";
					}else if(this.Throbber.setAttribute){
						this.Throbber.setAttribute("display", "none");
					}
				}
				return false;
			}
			req = null;
			ResponseServerString = null;
			DatePreventsCaching = null;
		}
		//the guessed servertype is used for communication
		if ("php" === HMI.HMI_Constants.ServerType){
			//tks.php is always in the same subdir as the html files
			this.KSGateway_Path = window.location.pathname.substring(0, window.location.pathname.lastIndexOf("/")+1)+ "tks.php";
		}else if("tcl" === HMI.HMI_Constants.ServerType){
			//tcl gateway is hardcoded in the Gateway via TCL-HTTPD\tclhttpd3.5.1\custom\tkshttpserver.tcl
			this.KSGateway_Path = "/tks";
		}else if("kshttp" === HMI.HMI_Constants.ServerType){
			//not required with kshttp
			this.KSGateway_Path = null;
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
			if (this.Throbber !== null){
				if(this.Throbber.style){
					this.Throbber.style.display = "none";
				}else if(this.Throbber.setAttribute){
					this.Throbber.setAttribute("display", "none");
				}
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
		if (this.Playground.nodeName.toLowerCase() == "embed" || this.Playground.nodeName.toLowerCase() == "object"){
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
				if (-1 !== this.PluginVendor.indexOf('Adobe')){
					this.PluginVendor = 'Adobe';
				}else if (-1 !== this.PluginVendor.indexOf('examotion')){
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
		
		/*
		//HMIdate was populated in every js-file with the date of CVS commit, not working with git
		//publish this date on website
		if ("undefined" != typeof HMIdate){
			HMI.HMI_Constants.HMIdate = HMIdate;
			var dateTextNode = document.createTextNode("Version: 2.5 ("+HMI.HMI_Constants.HMIdate.substr(0, 10)+")");
			
			var DateOutput;
			if ((DateOutput = document.getElementById("idDateOutput"))){
				DateOutput.appendChild(dateTextNode);
				DateOutput.setAttribute("title", "last changed: "+HMI.HMI_Constants.HMIdate+" UTC");
			}
			DateOutput = null;
			HMIdate = null;
			dateTextNode = null;
		}
		*/
		
		//make deep links work
		this.interpreteUrlParameter();
		
		this.hmi_log_trace("HMI.prototype.init - End");
		return true;
	},
	
	/**
	 * parse url Parameter and loads the requested sheets
	 */
	interpreteUrlParameter: function (){
		this.hmi_log_trace("HMI.prototype.interpreteUrlParameter");
		
		//analyse a "deep link" of a sheet
		var UrlParameterList = null;
		if (window.location.search.length !== 0){
			//collect parameter given by the deep link
			UrlParameterList = new Object();
			var wertestring = unescape(window.location.search);
			wertestring = wertestring.slice(1);
			var paare = wertestring.split("&");
			for (var i=0; i < paare.length; i++) {
				var name = paare[i].substring(0, paare[i].indexOf("="));
				var wert = paare[i].substring(paare[i].indexOf("=")+1, paare[i].length);
				UrlParameterList[name] = wert;
			}
			paare = null;
			wertestring = null;
		}
		//jump to a "deep link" of a sheet
		if (	UrlParameterList !== null &&
			 (	UrlParameterList.Host !== undefined
			||	UrlParameterList.RefreshTime !== undefined
			||	UrlParameterList.ShowComp !== undefined
			||	UrlParameterList.Server !== undefined
			||	UrlParameterList.Sheet !== undefined)
			){
			//correct host in website with user wish
			if (UrlParameterList.Host && UrlParameterList.Host.length !== 0){
				HMI.InputHost.value = UrlParameterList.Host;
			}
			//request sanitised name
			var Host = this.getHostname();
			
			//correct RefreshTime in website with user wish
			if (UrlParameterList.RefreshTime && UrlParameterList.RefreshTime.length !== 0){
				HMI.InputRefreshTime.value = UrlParameterList.RefreshTime;
			}
			
			//correct ShowComponents Status in website with user wish
			if (UrlParameterList.ShowComp && UrlParameterList.ShowComp.length !== 0){
				if (UrlParameterList.ShowComp == "true" && document.getElementById("idShowcomponents")){
					document.getElementById("idShowcomponents").checked = true;
				}
			}
			HMI.ChangeRefreshTime();
			
			//a server and sheet is specified in "deep link" (host is always there)
			if (	UrlParameterList.Server
				&&	UrlParameterList.Server.length !== 0
				&&	UrlParameterList.Sheet
				&&	UrlParameterList.Sheet.length !== 0)
			{
				HMI.ButShowServers.value = "Loading Sheet...";
				
				//clean old Server and Sheet entrys, an old SVG display and displayed errors in website
				deleteChilds(this.PossServers);
				deleteChilds(this.PossSheets);
				deleteChilds(this.Playground);
				deleteChilds(this.ErrorOutput);
				
				HMI.PossServers.options[0] = new Option('- list not loaded -', UrlParameterList.Server);
				HMI.PossSheets.options[0] = new Option('- list not loaded -', UrlParameterList.Sheet);
				
				//deactivate the Select-Boxes, because there is no usefull content
				HMI.PossServers.disabled = true;
				HMI.PossSheets.disabled = true;
				
				this.HMI_Constants.UrlParameterList = UrlParameterList;
				
				//spaces in objectname are encoded as %20 within OV
				HMI.showOneSheet(Host, UrlParameterList.Server, encodeURI(UrlParameterList.Sheet));
				
				HMI.ButShowServers.value = "Reload Serverlist";
			}else if (	UrlParameterList.Server
				&&	UrlParameterList.Server.length !== 0){
				//only a server is specified in "deep link" => showSheets
				
				HMI.ButShowServers.value = "Loading Sheetlist...";
				HMI.PossServers.options[0] = new Option('- list not loaded -', UrlParameterList.Server);
				HMI.PossSheets.options[0] = new Option('- list not loaded -', '');
				
				HMI.showSheets(Host, UrlParameterList.Server);
				
				HMI.ButShowServers.value = "Reload Serverlist";
			}else if (UrlParameterList.Host && UrlParameterList.Host.length !== 0){
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
		}catch(e){/* nothing to do*/}
		
		//append html5 datalist if supported and provided
		if(this.InputHost.list !== undefined){
			this.InputHost.setAttribute("list", "InputHost");
			var datalistNode = document.createElement("datalist");
			datalistNode.setAttribute("id", "InputHost");
			if(UrlParameterList !== null && UrlParameterList.hostlist !== undefined){
				var hostlist = UrlParameterList.hostlist.split(",");
			}else if(	window.location.hostname.toLowerCase() === "localhost"
					||	window.location.hostname.toLowerCase() === "127.0.0.1"
					||	window.location.hostname.toLowerCase() === "::1"
					||	window.location.protocol === "file:"
					||	window.location.hostname === "" // should not be the case...
					){
				hostlist = new Array("localhost");
			}else{
				hostlist = new Array("localhost", window.location.hostname.toLowerCase());
			}
			for (var i=0;i < hostlist.length;i++){
				var optionNode = document.createElement("option");
				optionNode.setAttribute("value", hostlist[i]);
				datalistNode.appendChild(optionNode);
			}
			this.InputHost.parentNode.appendChild(datalistNode);
		}
		
		if (this.Throbber !== null){
			if(this.Throbber.style){
				this.Throbber.style.display = "none";
			}else if(this.Throbber.setAttribute){
				this.Throbber.setAttribute("display", "none");
			}
		}
		
		this.hmi_log_trace("HMI.prototype.interpreteUrlParameter - End");
		return;
	},
	
	/**
	 * update deep link and HTML5 history
	 */
	updateDeepLink: function (optionalParameter){
		var newDeepLink = window.location.pathname.substring(0, window.location.pathname.lastIndexOf("/")+1);
		var SheetInfo = {"Host":"", "Server":"", "Sheet":""};
		newDeepLink += "?";
		if(this.cshmi === null && this.Playground.firstChild !== null){
			//only the old HMI has a refreshtime
			newDeepLink += "RefreshTime="+HMI.RefreshTime;
		}
		if(HMI.InputHost.value !== ""){
			if(newDeepLink.slice(-1) !== "?"){
				newDeepLink += "&";
			}
			newDeepLink += "Host="+HMI.InputHost.value;
			SheetInfo.Host = HMI.InputHost.value;
		}
		if(HMI.PossServers.selectedIndex >= 0 && HMI.PossServers.options[HMI.PossServers.selectedIndex].value !== ""){
			newDeepLink += "&Server="+HMI.PossServers.options[HMI.PossServers.selectedIndex].value;
			SheetInfo.Server = HMI.PossServers.options[HMI.PossServers.selectedIndex].value;
		}
		if(HMI.PossSheets.selectedIndex >= 0 && HMI.PossSheets.options[HMI.PossSheets.selectedIndex].value !== ""){
			newDeepLink += "&Sheet="+HMI.PossSheets.options[HMI.PossSheets.selectedIndex].value;
			SheetInfo.Sheet = HMI.PossSheets.options[HMI.PossSheets.selectedIndex].value;
		}
		if (HMI.trace === true){
			newDeepLink += "&trace=true";
		}
		if (document.getElementById("idShowcomponents") && document.getElementById("idShowcomponents").checked){
			newDeepLink += "&ShowComp=true";
		}
		if (optionalParameter !== undefined){
			newDeepLink += optionalParameter;
		}
		document.getElementById("idBookmark").href = newDeepLink;
		
		//add sheet to html5 Session history management, if new
		if (window.history.pushState && window.location.protocol !== "file:"){
			if (newDeepLink != window.location.pathname+window.location.search){
				window.history.pushState(SheetInfo, "", newDeepLink);
			}
		}else{
			//show deeplink if history management is not available
			document.getElementById("idBookmark").style.display = "inline";
		}
	},
	
	/**
	 * shows a rectangle for 800ms to provide visual feedback
	 * called after sending a gesture request
	 */
	displaygestureReactionMarker: function (Component){
		this.hmi_log_trace("HMI.prototype.displaygestureReactionMarker");
		
		if (	Component.getAttribute("width") === null ||
				Component.getAttribute("height") === null ||
				Component.getAttribute("absolutex") === 0 ||
				Component.getAttribute("absolutey") === 0){
			return;
		}
		//build info rect around affected component
		
		//warning: This function is called after the refresh, so this Component is NOT displayed anymore! (but probably another similar Component)
		
		var gestureReactionMarker = null;
		
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
			gestureReactionMarker.setAttribute('fill', '#D1E1FA');
			gestureReactionMarker.setAttribute('opacity', '0.6');
			
			//disable pointer Events
			gestureReactionMarker.setAttributeNS(null, 'pointer-events', 'none');
		}
		gestureReactionMarker.setAttribute("x", Component.getAttribute("absolutex"));
		gestureReactionMarker.setAttribute("y", Component.getAttribute("absolutey"));
		gestureReactionMarker.setAttribute("width", Component.getAttribute("width"));
		gestureReactionMarker.setAttribute("height", Component.getAttribute("height"));
		
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
		var arrowdown1 = document.getElementById("arrowdown1");
		if (hide === true || (hide === null && HMI.HeaderIsVisible === true)){
			//hide menu
			
			//if there is support for CSS transition use it
			//transition from an height="auto" is not supported, so we change maxHeight
			//internet explorer never used this with a prefix
			if(HMI.HideableHeader.style.transitionDuration !== undefined){
				HMI.HideableHeader.style.transitionProperty = "max-height";
				HMI.HideableHeader.style.transitionTimingFunction = "ease-in-out";
				HMI.HideableHeader.style.transitionDuration = "0.6s";
				HMI.HideableHeader.style.maxHeight = '0px';
				HMI.HeaderIsVisible = false;
			}else if(HMI.HideableHeader.style.MozTransitionDuration !== undefined){
				HMI.HideableHeader.style.MozTransitionProperty = "max-height";
				HMI.HideableHeader.style.MozTransitionTimingFunction = "ease-in-out";
				HMI.HideableHeader.style.MozTransitionDuration = "0.6s";
				HMI.HideableHeader.style.maxHeight = '0px';
				HMI.HeaderIsVisible = false;
			}else if(HMI.HideableHeader.style.WebkitTransitionDuration !== undefined){
				HMI.HideableHeader.style.WebkitTransitionProperty = "max-height";
				HMI.HideableHeader.style.WebkitTransitionTimingFunction = "ease-in-out";
				HMI.HideableHeader.style.WebkitTransitionDuration = "0.6s";
				HMI.HideableHeader.style.maxHeight = '0px';
				HMI.HeaderIsVisible = false;
			}else if(HMI.HideableHeader.style.OTransitionDuration !== undefined){
				HMI.HideableHeader.style.OTransitionProperty = "max-height";
				HMI.HideableHeader.style.OTransitionTimingFunction = "ease-in-out";
				HMI.HideableHeader.style.OTransitionDuration = "0.6s";
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
			if (arrowdown1 && arrowdown1.style){
				document.getElementById("arrowdown1").style.visibility="visible";
				document.getElementById("arrowdown2").style.visibility="visible";
				document.getElementById("arrowup1").style.visibility="hidden";
				document.getElementById("arrowup2").style.visibility="hidden";
			}
		} else {
			//show menu
			HMI.HeaderIsVisible = true;
			
			HMI.optimizeSheetSize();
			
			//repaint header
			HMI.HideableHeader.style.maxHeight = "300px";
			
			//switch arrow orientation
			//
			if (arrowdown1 && arrowdown1.style){
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

			HMI.optimizeSheetSize();
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
		if (HMI.InputRefreshTime.type === "number" && HMI.InputRefreshTime.valueAsNumber !== undefined){
			//https://bugzilla.mozilla.org/show_bug.cgi?id=636737
			//is a problem, so deactivated
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
			if(window.location.hostname === ""){
				//transported via file://
				HMI.InputHost.value = "localhost";
			}else{
				HMI.InputHost.value = window.location.hostname;
			}
		}
		
		var Host;
		if (("tcl" === HMI.HMI_Constants.ServerType || "php" === HMI.HMI_Constants.ServerType) 
				&& HMI.InputHost.value == window.location.hostname){
			//if we have a gateway and the hostname is identical to this, translate into "localhost"
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
		
		//flush cache of tcl handles
		this.KSClient.destroy();
		
		//disable double click by user
		this.ButShowServers.disabled = true;
		this.ButShowServers.value = "Please wait...";
		if (this.Throbber !== null){
			if(this.Throbber.style){
				this.Throbber.style.display = "inline";
			}else if(this.Throbber.setAttribute){
				this.Throbber.setAttribute("display", "inline");
			}
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
		
		//clear old info from urlparameter (at least FBReference must be cleared) 
		this.HMI_Constants.UrlParameterList = null;
		
		//prevent further interpretion of timeevents
		this.cshmi = null;
		
		//deactivate the Select-Boxes, because there is no usefull content
		this.PossServers.disabled = true;
		this.PossSheets.disabled = true;
		
		//Set a neutral title
		document.title = "Startcenter - ACPLT/HMI";
		
		this.KSClient.getServers(this.getHostname());
		
		
		//todo move this to call back
		
		//reenable click by user
		this.ButShowServers.disabled = false;
		this.ButShowServers.value = "Reload Serverlist";
		if (this.Throbber !== null){
			if(this.Throbber.style){
				this.Throbber.style.display = "none";
			}else if(this.Throbber.setAttribute){
				this.Throbber.setAttribute("display", "none");
			}
		}
		
		//present a "deep link" to the state
		this.updateDeepLink();
		
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
		
		//clear old info from urlparameter (at least FBReference must be cleared)
		this.HMI_Constants.UrlParameterList = null;
		
		//prevent further interpretion of timeevents
		this.cshmi = null;
		
		HMI.PossSheets.disabled = true;
		document.getElementById("idBookmark").style.display = "none";
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
			for (var i = 0; i < SheetList.length; i++){
				//spaces in objectname are encoded as %20 within OV
				HMI.PossSheets.options[HMI.PossSheets.options.length] = new Option(decodeURI(SheetList[i]), SheetList[i]);
			}
			if (SheetList.length === 1){
				//selecting the option does not trigger the EventListener
				HMI.PossSheets.selectedIndex = 1;
				HMI.showOneSheet(Host, Server, SheetList[0]);
			}
			HMI.PossSheets.disabled = false;
			if(HMI.PossServers.length > 1){
				HMI.PossServers.disabled = false;
			}
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
	showOneSheet: function (Host, Server, Sheet) {
		this.hmi_log_trace("HMI.prototype.showOneSheet - Start with Sheet: "+Sheet);
		
		//present a "deep link" to the state
		this.updateDeepLink();
		//show the refreshInfo
		this.hideRefreshInfo(false);
		
		//clean an old SVG display and displayed errors in website
		deleteChilds(this.Playground);
		deleteChilds(this.ErrorOutput);
		deleteChilds(this.InfoOutput);
		document.getElementById("idBookmark").style.display = "none";
		
		//prevent further interpretion of timeevents
		this.cshmi = null;
		
		window.clearTimeout(HMI.RefreshTimeoutID);
		HMI.RefreshTimeoutID = null;
		
		//nothing selected
		if (Sheet === ''){
			return;
		}
		
		this.refreshSheet(Host, Server, Sheet);
		
		//show fb-server name when known
		//spaces in objectname are encoded as %20 within OV
		
		var SheetArray = Sheet.split("/");
		document.title = SheetArray[SheetArray.length-1] + " - //" + Host+"/"+Server+decodeURI(Sheet)+" - ACPLT/HMI";
		SheetArray = null;
		if (HMI.autoKeepHeader === false && !HMI.ErrorOutput.firstChild){
			if (!HMI.InfoOutput){
				//no info output available => hide
				HMI.hideHeader(true);
			}else if (HMI.InfoOutput && !HMI.InfoOutput.firstChild){
				//info output with no content => hide
				HMI.hideHeader(true);
			}
		}
		//we are ready for now, so maximize the dimension of the main sheet
		this.optimizeSheetSize();
		
		//update the "deep link" to the state
		if(HMI.Playground.firstElementChild && HMI.Playground.firstElementChild.FBReference && HMI.Playground.firstElementChild.FBReference["default"]){
			//if we have an cshmi with a fbref on the first sheet, link to this ref
			this.updateDeepLink("&FBReference="+HMI.Playground.firstElementChild.FBReference["default"]);
		}else{
			this.updateDeepLink();
		}
		
		//blur the buttons for convenience with keyboard interaction
		HMI.PossSheets.blur();
		HMI.PossServers.blur();
		
		HMI.ButShowServers.value = "Reload Serverlist";
		if (this.Throbber !== null){
			if(this.Throbber.style){
				this.Throbber.style.display = "none";
			}else if(this.Throbber.setAttribute){
				this.Throbber.setAttribute("display", "none");
			}
		}
		
		this.hmi_log_trace("HMI.prototype.showOneSheet - End");
	},
	
	/**
	 * 
	 */
	hideRefreshInfo: function(hide){
		var cssvalue = hide?"none":"";
		this.InputRefreshTime.style.display = cssvalue;
		if(this.InputRefreshTime.labels){
			var labellist = this.InputRefreshTime.labels;
		}else if(HMI.svgDocument.querySelectorAll){
			labellist = HMI.svgDocument.querySelectorAll("label[for=idRefreshTime]");
		}else{
			//the feature is not imported enough to shim
			labellist = {length:0};
		}
		for (var i = 0; i < labellist.length; ++i) {
			labellist[i].style.display = cssvalue;
		}
		labellist = null;
		return;
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
		//never used with ms or o prefix
		if(	document.hidden === true ||
			document.mozHidden === true||
			document.webkitHidden === true
		){
			skipRefresh = true;
		}
		//load sheet if playground is empty or with empty view (load in background)
		if (skipRefresh === false || HMI.Playground.childNodes.length === 0){
			var ComponentText = null;
			var Component = null;
			HMI.cyclicRequestNeeded = false;
			
			if(this.KSClient.cshmilib_path[Host+Server] === undefined && this.KSClient.hmilib_path[Host+Server] === undefined){
				//we have no cached library information about this server
				this.KSClient.pingServer(Host, Server);
			}
			
			//test loading old HMI, if hmilib is loaded
			if(this.KSClient.hmilib_path[Host+Server] !== null){
				if(!this.KSClient.HMIMANAGER_PATH[Host+Server]){
					//hmimanager path is unknown right now
					this.KSClient.getHMIManagerPointer(Host, Server);
				}
				
				//check if this sheet is a old hmi url by requesting it
				
				//spaces in objectname are encoded as %20 within OV
				var SVGRequestURI = "//"+Host+"/"+Server+encodeURI(Sheet) + ".GraphicDescription";
				
				if (SVGRequestURI !== null){
					//	get GraphicDescription
					//
					if ("kshttp" === HMI.HMI_Constants.ServerType){
						ComponentText = this.KSClient.getVar(SVGRequestURI, "OP_VALUE", null, false, "text/plain");
					}else{
						ComponentText = this.KSClient.getVar(SVGRequestURI, "OP_VALUE", null);
					}
				}
				SVGRequestURI = null;
				
				if (ComponentText && ComponentText.length > 0 && ComponentText.indexOf("KS_ERR") === -1){
					//hmi target
					var SplitComponent = this.KSClient.prepareComponentText(ComponentText);
					if (SplitComponent === null){
						//logging not required, already done by prepareComponentText
						return;
					}
					
					//build a DOM fragment with the SVG String
					Component = this.HMIDOMParser.parse(SplitComponent, null);
					
					if (Component != null){
						this._showComponent(Host, Server, Component);
						HMI.cyclicRequestNeeded = true;
						//maximize the dimension of the new main sheet
						this.optimizeSheetSize();
					}
					
					//reload scroll setting
					//wheelsupport is not supported by the HMI Team and probably firefox only
					if(this.scrollComponent)
					{
						var scrollComponent = null;
						if(document.getElementById(this.scrollComponent) !== null){ //opera, ff
							scrollComponent = document.getElementById(this.scrollComponent);
						} else if(this.Playground.getElementById(this.scrollComponent) !== null){ //ie
							scrollComponent = this.Playground.getElementById(this.scrollComponent);
						}
						scrollComponent.setAttribute("y", this.currY);
						scrollComponent.setAttribute("x", this.currX);
					}
				}
				ComponentText = null;
			}
			
			//check if we have an cshmi target, if hmi was not successful till now
			if (Component === null && this.KSClient.cshmilib_path[Host+Server] !== null){
				//save for later use
				this.KSClient.ResourceList.ModelHost = Host;
				this.KSClient.ResourceList.ModelServer = Server;
				
				this.hideRefreshInfo(true);
				
				//startup cshmi
				this.cshmi = new cshmi();
				this.cshmi.instanciateCshmi(Host, Server, Sheet);
			}
			
		}
		if(this.Playground.firstChild !== null){
			//set x, y position to zero. A component could be out of view (especially when in embed-Node in IE)
			this.Playground.firstChild.setAttribute('x', 0);
			this.Playground.firstChild.setAttribute('y', 0);
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
	 * resizes the sheet to the maximum size
	 */
	optimizeSheetSize: function () {
		if(window.getComputedStyle && HMI.Playground.firstChild && HMI.Playground.firstChild.namespaceURI == HMI.HMI_Constants.NAMESPACE_SVG){
			if(!HMI.Playground.firstChild.getAttribute('data-originalModelWidth')){
				//remember original size from model
				HMI.Playground.firstChild.setAttribute('data-originalModelWidth', HMI.Playground.firstChild.getAttribute('width'));
				HMI.Playground.firstChild.setAttribute('data-originalModelHeight', HMI.Playground.firstChild.getAttribute('height'));
			}
			var originalModelWidth = parseFloat(HMI.Playground.firstChild.getAttribute('data-originalModelWidth'));
			var originalModelHeight = parseFloat(HMI.Playground.firstChild.getAttribute('data-originalModelHeight'));
			var windowHeight = window.innerHeight;
			var windowWidth = window.innerWidth;
			
			var playgroundStyle = window.getComputedStyle(HMI.Playground);
			var playgroundMarginRight = parseFloat(playgroundStyle.marginRight);
			var playgroundMarginBottom = parseFloat(playgroundStyle.marginBottom);
			
			//calculate the maximum edge which should be visible from the childrens
			var maxchildRightEdge = 0;
			var maxchildBottomEdge = 0;
			var AllChilds = HMI.Playground.firstChild.getElementsByTagNameNS(HMI.HMI_Constants.NAMESPACE_SVG, "*");
			for(var i = 0;i < AllChilds.length;i++){
				var elementWidth = parseInt(AllChilds[i].getAttribute("width"), 10);
				var elementHeight = parseInt(AllChilds[i].getAttribute("height"), 10);
				if (isNaN(elementWidth)){	elementWidth = 0;	}
				if (isNaN(elementHeight)){	elementHeight = 0;	}
				var iteratedchildRightEdge = parseInt(AllChilds[i].getAttribute("absolutex"), 10) + elementWidth;
				var iteratedchildBottomEdge = parseInt(AllChilds[i].getAttribute("absolutey"), 10) + elementHeight;
				if(maxchildRightEdge < iteratedchildRightEdge){
					maxchildRightEdge = iteratedchildRightEdge;
				}
				if(maxchildBottomEdge < iteratedchildBottomEdge){
					maxchildBottomEdge = iteratedchildBottomEdge;
				}
			}
			var candidateWidth = originalModelWidth < maxchildRightEdge?maxchildRightEdge:originalModelWidth;
			var candidateHeight = originalModelHeight < maxchildBottomEdge?maxchildBottomEdge:originalModelHeight;
			
			if(windowWidth < candidateWidth + HMI.Playground.offsetLeft + playgroundMarginRight){
				//we want to have scroll bars to reach everything
				HMI.Playground.firstChild.setAttribute('width', candidateWidth - HMI.Playground.offsetLeft - playgroundMarginRight);
			}else if(windowWidth > originalModelWidth + HMI.Playground.offsetLeft + playgroundMarginRight){
				//blow up the viewport to be able to see even dynamic moved objects
				HMI.Playground.firstChild.setAttribute('width', windowWidth - HMI.Playground.offsetLeft - playgroundMarginRight);
			}
			if(windowHeight < candidateHeight + HMI.Playground.offsetTop + playgroundMarginBottom){
				//we want to have scroll bars to reach everything
				HMI.Playground.firstChild.setAttribute('height', candidateHeight - HMI.Playground.offsetTop - playgroundMarginBottom);
			}else if(windowHeight > originalModelHeight + HMI.Playground.offsetTop + playgroundMarginBottom){
				//blow up the viewport to be able to see even dynamic moved objects
				HMI.Playground.firstChild.setAttribute('height', windowHeight - HMI.Playground.offsetTop - playgroundMarginBottom);
			}
			
			if(HMI.PlaygroundContainerNode){
				//fix the dimension of an embed or object node (as in deprecated Adobe SVG Plugin!)
				//the displayed size is calculated from the Container-Node in the html, so we correct the dimension of it
				HMI.PlaygroundContainerNode.setAttribute('height', HMI.Playground.firstChild.getAttribute('height'));
				HMI.PlaygroundContainerNode.setAttribute('width', HMI.Playground.firstChild.getAttribute('width'));
			}
		}
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
		this.hmi_log_trace("HMI.prototype.getComponent - Start - Target:"+(evt.target ? evt.target.id : evt.srcElement?evt.srcElement.id:" no event"));
		
		var Component = null;
		
		//get the Component itself which contained the triggered gesture
		if (evt.target){
			Component = evt.target;
		}else if(evt.srcElement){
			//native IE code
			Component = evt.srcElement;
		}else{
			//if called direct with an SVG Object
			Component = evt;
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
			this.saveAbsolutePosition(Element, false);
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
				if (Fragment.childNodes.item(idx).nodeName == "svg:svg"){
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
		//html5 classList saves us much work
		if (Node.classList && Node.classList.contains){
			return Node.classList.contains(ClassName);
		}else if(!Node.getAttribute){
			//this object can be for example a text node
			return false;
		}
		
		var idx;
		var Classes = Node.getAttribute("class");
		//conform to the standard would be "", but browser return null. not tested both because of speed and split can live without it
		if (Classes === null){
			return false;
		}
		Classes = Classes.split(" ");
		for (idx = Classes.length -1; idx >= 0; idx--){
			if (ClassName === Classes[idx]){
				return true;
			}
		}
		
		return false;
	},
	
	/**
	 * adds a class to a svg element
	 */
	addClass: function(Node, additionalClass){
		if (Node.classList && Node.classList.add){
			//html5 classList saves us some work
			Node.classList.add(additionalClass);
		}else{
			Node.setAttribute('class', (Node.getAttribute('class')+ " "+additionalClass).trim());
		}
	},
	
	/**
	 * removes a class from a svg element
	 */
	removeClass: function(Node, oldClass){
		if (Node.classList && Node.classList.remove){
			//html5 classList saves us some work
			Node.classList.remove(oldClass);
		}else{
			Node.setAttribute('class', (Node.getAttribute('class').replace(oldClass, "").trim()));
		}
	},
	
	/**
	 * calculates the absolute position and saves it into the custom absolutex and absolutey attributes
	 * @param Element Element to work on
	 * @param {bool} recursive: handle childrens, too.
	 */
	saveAbsolutePosition: function (Element, recursive) {
		if(!Element.getAttribute){
			return;
		}
		
		//absolutex and localy are HMI specific DOM Attributes!
		//They are ignored by the SVG Renderer but used for position calculation in the move gesture
		//and the function displaygestureReactionMarker
		if(Element.hasAttribute("x")){
			var localx = parseInt(Element.getAttribute("x"), 10);
			var localy = parseInt(Element.getAttribute("y"), 10);
		}else if(Element.hasAttribute("cx")){
			localx = parseInt(Element.getAttribute("cx"), 10);
			localy = parseInt(Element.getAttribute("cy"), 10);
		}else if(Element.hasAttribute("x1")){
			localx = parseInt(Element.getAttribute("x1"), 10);
			localy = parseInt(Element.getAttribute("y1"), 10);
		}
		var localrotate = parseInt(getRotationFromObject(Element), 10);
		var absolutex, absolutey, absoluterotate;
		
		if (isNaN(localx)){
			localx = 0;
		}
		if (isNaN(localy)){
			localy = 0;
		}
		if (isNaN(localrotate)){
			localrotate = 0;
		}
		
		if(Element.parentNode !== null && Element.parentNode.namespaceURI == HMI.HMI_Constants.NAMESPACE_SVG){
			var parentabsolutex = parseInt(Element.parentNode.getAttribute("absolutex"), 10);
			var parentabsolutey = parseInt(Element.parentNode.getAttribute("absolutey"), 10);
			var parentabsoluterotate = parseInt(Element.parentNode.getAttribute("absoluterotate"), 10);
			if(parentabsoluterotate == 0){
				absolutex = parentabsolutex + localx;
				absolutey = parentabsolutey + localy;
			}else{
				var cosRotate = Math.cos(parentabsoluterotate*Math.PI/180);
				var sinRotate = Math.sin(parentabsoluterotate*Math.PI/180);
				absolutex = parentabsolutex + cosRotate * localx - sinRotate * localy;
				absolutey = parentabsolutey + sinRotate * localx + cosRotate * localy;
			}
			absoluterotate = localrotate + parentabsoluterotate;
		}else{
			absolutex = 0;
			absolutey = 0;
			absoluterotate = 0;
		}
		
		if (isNaN(absolutex) || isNaN(absolutey) || isNaN(absoluterotate)){
			this.hmi_log_warning("SVG-ERROR - parentNode of "+Element.id+" has no localx or localy. Some features will not work.");
			Element.setAttribute("absolutex", 0);
			Element.setAttribute("absolutey", 0);
			Element.setAttribute("absoluterotate", 0);
		}else{
			Element.setAttribute("absolutex", absolutex);
			Element.setAttribute("absolutey", absolutey);
			Element.setAttribute("absoluterotate", absoluterotate);
		}
		
		var elementWidth = parseInt(Element.getAttribute("width"), 10);
		var elementHeight = parseInt(Element.getAttribute("height"), 10);
		
		if (isNaN(elementWidth)){
			elementWidth = 0;
		}
		if (isNaN(elementHeight)){
			elementHeight = 0;
		}
		if(recursive !== false){
			//performance problems?
			//var ComponentChilds = Element.getElementsByTagNameNS(HMI.HMI_Constants.NAMESPACE_SVG, "*");
			var ComponentChilds = Element.childNodes;
			for(var i = 0;i < ComponentChilds.length;i++){
				HMI.saveAbsolutePosition(ComponentChilds[i], true);
			}
			ComponentChilds = null;
		}
		return new Array(absolutex+elementWidth, absolutey+elementHeight);
	},
	
	/**
	 * mouse position from an event object
	 * @param evt
	 * @param Component if not null detect the mouse position relative to this component
	 * @return an array with x and y position
	 */
	getClickPosition: function (evt, Component) {
		this.hmi_log_trace("HMI.prototype.getClickPosition - Start");
		
		var clickPosition = new Array(Number.NaN, Number.NaN);
		if (!evt){
			this.hmi_log_error("HMI.prototype.getClickPosition - called without an event");
			return clickPosition;
		}else if (Component === undefined){
			this.hmi_log_error("HMI.prototype.getClickPosition - called without an Component");
			return clickPosition;
		}
		var mousePosX;
		var mousePosY;
		
		//see technology paper "Human Model Interface - JavaScript" for full details of crossbrowser problems
		if (evt.touches && evt.touches.length !== 0) {
			// iPhone (but no other touch devices)
			mousePosX = evt.touches[0].pageX;
			mousePosY = evt.touches[0].pageY;
		}else if (evt.changedTouches && evt.changedTouches.length) {
			// IE10 Touch. Not perfekt clean, as we should extract evt.changedTouches[x].identifier
			mousePosX = evt.changedTouches[0].pageX;
			mousePosY = evt.changedTouches[0].pageY;
		}else if (evt.pageX || evt.pageY) {
			//code for native SVG. pageX based on the full XHTML Document
			mousePosX = evt.pageX;
			mousePosY = evt.pageY;
		}else if (evt.clientX || evt.clientY) {
			//code for svg-plugin. clientX is based on the Plugin area, without browser scrolling sideeffects
			mousePosX = evt.clientX;
			mousePosY = evt.clientY;
		}else{
			mousePosX = Number.NaN;
			mousePosY = Number.NaN;
		}
		
		if (Component !== null){
			//detect the mouse position relative to the component
			
			//we have to find the offset of the svg-element in the XHTML
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
			obj = null;
			
			//the searched position is pageX/clientX minus Position of the HMI Component minus Position of the SVG
			clickPosition[0] = mousePosX - parseInt(Component.getAttribute("absolutex"), 10) - svgOffsetX;
			clickPosition[1] = mousePosY - parseInt(Component.getAttribute("absolutey"), 10) - svgOffsetY;
			
			svgOffsetX = null;
			svgOffsetY = null;
			
			this.hmi_log_trace("HMI.prototype.getClickPosition relative - End x:"+clickPosition[0]+" y:"+clickPosition[1]);
		}else{
			clickPosition[0] = mousePosX;
			clickPosition[1] = mousePosY;
			
			this.hmi_log_trace("HMI.prototype.getClickPosition absolute - End x:"+clickPosition[0]+" y:"+clickPosition[1]);
		}
		mousePosX = null;
		mousePosY = null;
		
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
		if (this.InfoOutput){
			//if we have an error, the warning is not interesting anymore
			deleteChilds(HMI.InfoOutput);
		}
		if(HMI.ErrorOutput.hasAttribute("title")){
			HMI.ErrorOutput.removeAttribute("title");
		}
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
if (window.location.search && -1 !== unescape(window.location.search).indexOf("trace=true")){
	HMI = new HMI(true, true, true, true, true);
}else{
	HMI = new HMI(true, true, true, true, false);
}
