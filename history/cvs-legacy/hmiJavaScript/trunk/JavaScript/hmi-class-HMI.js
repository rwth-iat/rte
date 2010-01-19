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
*	$Revision: 1.134 $
*	$Date: 2010-01-19 16:12:05 $
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
	this.HMI_Constants.NAMESPACE_SVG = "http://www.w3.org/2000/svg";
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
	this.PossServers = null;
	this.PossSheets = null;
	this.Playground = null;
	this.ErrorOutput = null;
	this.InfoOutput = null;
	
	this.RefreshTimeoutID = null;
	this._currentDragger = null;
	this.KSClient = null;
	this.Path = null;
	this.KSGateway_Path = null;
	
	this.svgWindow = null;
	this.svgDocument = null;
	this.EmbedAdobePlugin= null;
	this.GatewayTypeTCL = null;
	this.GatewayTypePHP = null;
	
	this.RefreshTime = null;
	this.ServerProperty = {SheetHasStyleDescription:null};
	
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
			//Element of SVG insertion (p for firefox, embed for IE)
			if (!(this.Playground = $('idPlayground'))){
				ErrorDetail += "HTML Container-Element with the ID: idPlayground not found.\n";
			}else{
				//prevent right click in supported Browsers (no Opera, no IE6-8)
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
				addEventSimple(HMI.InputHost,'change',function(){HMI.showServers();});
			}else{
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
		}
		if(ErrorDetail !== ""){
			window.alert ("Error initialising HMI Website:\n"+ErrorDetail);
			return;
		}
		
		//init the optional Buttons
		if ($('idRefresh')){
			addEventSimple($('idRefresh'),'click',function(){HMI.refreshSheet();});
		}
		if ($('idStopRefresh')){
			addEventSimple($('idStopRefresh'),'click',function(){
				window.clearInterval(HMI.RefreshTimeoutID);
				HMI.RefreshTimeoutID = null;
				}
			);
		}
		
		if ($('idStartRefresh')){
			addEventSimple($('idStartRefresh'),'click',function(){HMI.RefreshTimeoutID = window.setInterval(function () {HMI.refreshSheet();}, HMI.InputRefreshTime.value);});
		}
		
		//Object of InfoOutput, optional, not necessary
		if ((this.InfoOutput = $('idInfoOutput'))){
			deleteChilds(HMI.InfoOutput);
		}
		
		//detect if the file is called from http or https, but not from filesystem
		if (-1 == window.location.protocol.indexOf('http')){
			this.hmi_log_error("HMI.prototype.init - Communication to Server failed. This website has to be transfered via HTTP. ");
			this.hmi_log_onwebsite("Communication to Server failed. This website has to be transfered via HTTP.");
			return false;
		}
		//Try to detect the Servertype (TCL or PHP capable)
		//make a request to sniff the HTTP-Serverstring
		
		//IE sometimes uses a cached version, without server Header, so prevent caching
		var DatePreventsCaching = new Date();
		try{
			var req = new XMLHttpRequest();
			req.open("GET", window.location.pathname+'?preventCaching='+DatePreventsCaching.getTime(), false);
			req.send(null);
			var ResponseServerString = req.getResponseHeader('server');
			if (ResponseServerString && -1 != ResponseServerString.indexOf('Tcl-Webserver')){
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
			this.hmi_log_onwebsite("Gatewaydetection failed. ");
			return false;
		}
		delete req;
		delete ResponseServerString;
		delete DatePreventsCaching;
		
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
			HMI.hmi_log_onwebsite('Could not detect type of HTTP/KS-Gateway. Please configure in hmi-class-HMI.js');
			
			var ErrorNode = document.createElement('br');
			HMI.ErrorOutput.appendChild(ErrorNode);
			
			ErrorNode = document.createElement("a");
			ErrorNode.setAttribute('href', 'httpservertest.html');
			ErrorNode.appendChild(document.createTextNode('Servertest available'));
			HMI.ErrorOutput.appendChild(ErrorNode);
			
			delete ErrorNode;
//			alert('This website has to be transfered via HTTP. Could not detect type of HTTP/KS-Gateway. Please configure in hmi-class-HMI.js');
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
		
		//Init KSClient
		this.KSClient = new HMIJavaScriptKSClient();
		
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
			var titlenode = document.createAttribute("title");
			titlenode.nodeValue = "last changed: "+HMI.HMI_Constants.HMIdate+" UTC";
			
			var DateOutput;
			if ((DateOutput = document.getElementById("idDateOutput"))){
				DateOutput.appendChild(dateTextNode);
				DateOutput.parentNode.setAttributeNode(titlenode);
			}
			delete DateOutput;
			delete HMIdate;
			delete dateTextNode;
			delete titlenode;
		}
		
		//jump to a "deep link" of a sheet
		if (window.location.search.length !== 0){
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
			delete paare;
			delete wertestring;
			
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
			
			//a server and sheet is specified in "deep link" (host is allways there)
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
				this.KSClient.init(HMI_Parameter_Liste.Host + '/' + HMI_Parameter_Liste.Server, window.location.host + HMI.KSGateway_Path);
				if (this.KSClient.TCLKSHandle === null){
						HMI.hmi_log_onwebsite('Requested Host or FB-Server on Host not available.');
				}else{
					//the path of the HMI Manager could be different in every OV Server (manager needed for all gestures)
					this.KSClient.getHMIManagerPointer();
					if (HMI.KSClient.HMIMANAGER_PATH === null){
						HMI.hmi_log_onwebsite('Requested FB-Server is no HMI-Server.');
					}else{
						HMI.showSheet(HMI_Parameter_Liste.Sheet);
					}
				}
			}else if (	HMI_Parameter_Liste.Server
				&&	HMI_Parameter_Liste.Server.length !== 0){
				//only a server is specified in "deep link" => showSheets
				
				HMI.PossServers.options[0] = new Option('- list not loaded -', 'no server');
				HMI.PossSheets.options[0] = new Option('- list not loaded -', 'no sheet');
				
				HMI.ButShowServers.value = "load Serverlist";
				
				//an init generates a new Handle, needed cause we communicate to the Server the first time
				this.KSClient.init(HMI_Parameter_Liste.Host + '/' + HMI_Parameter_Liste.Server, window.location.host + HMI.KSGateway_Path);
				if (this.KSClient.TCLKSHandle === null){
						HMI.hmi_log_onwebsite('Requested Host or FB-Server on Host not available.');
				}else{
					//the path of the HMI Manager could be different in every OV Server (manager needed for all gestures)
					this.KSClient.getHMIManagerPointer();
					if (HMI.KSClient.HMIMANAGER_PATH === null){
						HMI.hmi_log_onwebsite('Requested FB-Server is no HMI-Server.');
					}else{
						HMI.showSheets(HMI_Parameter_Liste.Server);
					}
				}
			}else if (HMI_Parameter_Liste.Host && HMI_Parameter_Liste.Host.length !== 0){
				//no server and sheet specified, but a host => load serverlist
				HMI.showServers();
			}
			delete HMI_Parameter_Liste;
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
		this.hmi_log_trace("HMI.prototype.init - End");
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
	hideHeader: function(){
		if (HMI.showHeader){
			//hide menu
			HMI.showHeader = false;
			document.getElementById("idHideableHeader").style.display = "none";
			if (document.getElementById("arrowdown1") !== null){
				document.getElementById("arrowdown1").style.visibility="visible";
				document.getElementById("arrowdown2").style.visibility="visible";
				document.getElementById("arrowup1").style.visibility="hidden";
				document.getElementById("arrowup2").style.visibility="hidden";
			}
		} else {
			//show menu
			HMI.showHeader = true;
			document.getElementById("idHideableHeader").style.display = "block";
			if (document.getElementById("arrowdown1") !== null){
				document.getElementById("arrowdown1").style.visibility="hidden";
				document.getElementById("arrowdown2").style.visibility="hidden";
				document.getElementById("arrowup1").style.visibility="visible";
				document.getElementById("arrowup2").style.visibility="visible";
			}
		}
	},
	/*********************************
		Functions - ChangeRefreshTime
	*********************************/
	ChangeRefreshTime: function(){
		
		if (HMI.InputRefreshTime.value < 100){
			HMI.InputRefreshTime.value = 100;
			this.RefreshTime = 100;
		}else{
			this.RefreshTime = parseInt(HMI.InputRefreshTime.value,10);
			//make input variable number-only
			HMI.InputRefreshTime.value = this.RefreshTime;
		}
		$("idBookmark").setAttribute("href", window.location.protocol+"//"+
			window.location.host+window.location.pathname.substring(0, window.location.pathname.lastIndexOf("/")+1)+
			"?Host="+HMI.InputHost.value+
			"&RefreshTime="+HMI.RefreshTime+
			"&Server="+(HMI.KSClient.KSServer ? HMI.KSClient.KSServer.substr(HMI.KSClient.KSServer.indexOf('/')+1) : "")+
			"&Sheet="+(HMI.Path !== null ? HMI.Path : "")+
			(HMI.trace===true?"&trace=true":"")+
			(($("idShowcomponents") && $("idShowcomponents").checked)?"&ShowComp=true":"")
			);
		
		//if an auto refresh is active, reset to new value
		if (HMI.RefreshTimeoutID !== null){
			window.clearInterval(HMI.RefreshTimeoutID);
			HMI.refreshSheet();
			HMI.RefreshTimeoutID = window.setInterval(function () {HMI.refreshSheet();}, HMI.RefreshTime);
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
		
		window.clearInterval(HMI.RefreshTimeoutID);
		HMI.RefreshTimeoutID = null;
		
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
		
		//deactivate the Select-Boxes, because there is no usefull content
		HMI.PossServers.disabled = true;
		HMI.PossSheets.disabled = true;
		
		//Set a neutral title
		document.title = "Startcenter - ACPLT/HMI";
		
		//Gateway could not be another host without violating Same Origin Policy
		var KSGateway = window.location.host;
		
		//present a deep link to the Host setting
		$("idBookmark").style.cssText = "display:inline;";
		$("idBookmark").setAttribute("href", window.location.protocol+"//"+
			window.location.host+window.location.pathname.substring(0, window.location.pathname.lastIndexOf("/")+1)+
			"?Host="+HMI.InputHost.value+
			"&RefreshTime="+HMI.RefreshTime+
			(HMI.trace===true?"&trace=true":"")+
			(($("idShowcomponents") && $("idShowcomponents").checked)?"&ShowComp=true":"")
			);
		
		//an init generates a new Handle, needed cause we communicate to the Manager the first time
		this.KSClient.init(KSServer + '/MANAGER', KSGateway + HMI.KSGateway_Path);
		if (this.KSClient.TCLKSHandle !== null){
			this.KSClient.getServers();
		}
		delete KSServer;
		
		//reenable click by user
		HMI.ButShowServers.disabled = false;
		HMI.ButShowServers.value = "Reload Serverlist";
		if (document.getElementById("idThrobbler") !== null){
			document.getElementById("idThrobbler").style.display = "none";
		}
		
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
		
		HMI.PossSheets.disabled = true;
		$("idBookmark").style.cssText = "display:none;";
		document.title = "Startcenter - ACPLT/HMI";
		
		window.clearInterval(HMI.RefreshTimeoutID);
		HMI.RefreshTimeoutID = null;
		
		//nothing selected
		if (Server == 'no server'){
			HMI.PossSheets.options[0] = new Option('please select Server first', 'no sheet');
			return;
		}
		
		//an init generates a new Handle, needed cause we communicate to this server the first time
		this.KSClient.init(this.KSClient.KSServer.substring(0, this.KSClient.KSServer.indexOf('/')) + '/' + Server, this.KSClient.TCLKSGateway);
		if (this.KSClient.TCLKSHandle !== null){
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
		window.clearInterval(HMI.RefreshTimeoutID);
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
			
			this._getAndImportComponent(HMI.Path, HMI.Playground, true);
		};
		document.title = "//"+this.KSClient.KSServer+Sheet+" - ACPLT/HMI";
		if (HMI.autoKeepHeader === false && !HMI.ErrorOutput.firstChild){
			//no info output available
			if (!HMI.InfoOutput){
				HMI.hideHeader();
			//info output with no content
			}else if (HMI.InfoOutput && !HMI.InfoOutput.firstChild){
				HMI.hideHeader();
			}
		}
		
		//blur the buttons for convenience with keyboard interaction
		HMI.PossSheets.blur();
		HMI.PossServers.blur();
		//present a "deep link" to the sheet
		$("idBookmark").style.cssText = "display:inline;";
		$("idBookmark").setAttribute("href", window.location.protocol+"//"+
			window.location.host+window.location.pathname.substring(0, window.location.pathname.lastIndexOf("/")+1)+
			"?Host="+HMI.InputHost.value+
			"&RefreshTime="+HMI.RefreshTime+
			"&Server="+(HMI.KSClient.KSServer ? HMI.KSClient.KSServer.substr(HMI.KSClient.KSServer.indexOf('/')+1) : "")+
			"&Sheet="+(HMI.Path !== null ? HMI.Path : "")+
			(HMI.trace===true?"&trace=true":"")+
			(($("idShowcomponents") && $("idShowcomponents").checked)?"&ShowComp=true":"")
			);
		
		this.hmi_log_trace("HMI.prototype.showSheet - End");
	},
	
	/*********************************
		refreshSheet
	*********************************/
	refreshSheet: function () {
		this.hmi_log_trace("HMI.prototype.refreshSheet - Start");
		
		if (this.InfoOutput){
			deleteChilds(this.InfoOutput);
		}
		if (HMI.KSClient.TCLKSHandle !== null)
			this._getAndImportComponent(HMI.Path, HMI.Playground, false);
		
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
			if (HMI.autoKeepHeader === false && HMI.showHeader === true){
				HMI.hideHeader();
			}
		}
		if (HMI.RefreshTimeoutID === null){
			//reactivate the Refresh
			HMI.RefreshTimeoutID = window.setInterval(function () {HMI.refreshSheet();}, HMI.RefreshTime);
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
	*********************************/
	getComponent: function (evt, cssclass) {
		//This event could be called from nativ IE, so evt.target is not available
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
				&&	Component !== document
				&&	HMI.instanceOf(Component, cssclass) === false)
		{
			if (Component.ownerSVGElement !== undefined){
				//firefox and co
				Component = Component.ownerSVGElement;
			}else if (Component.parentNode.namespaceURI == HMI.HMI_Constants.NAMESPACE_SVG ){
				//Adobe+Renesis
				Component = Component.parentNode;
			}else if (Component.parentNode.tagUrn == HMI.HMI_Constants.NAMESPACE_SVG ){
				//Inline IE Code inline
				Component = Component.parentNode;
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
		this.hmi_log_trace("HMI.prototype.switchGround - Start, Evt: "+evt.type+", Evt.id: "+(evt.target ? evt.target.id : evt.srcElement.id)+", Evt.nodeName: "+(evt.target ? evt.target.nodeName : evt.srcElement.nodeName)+", Ground: "+ground._node.id);
		
		//is there a move gesture in action?
		if (this._currentDragger !== null)
			this._currentDragger.switchGround(evt, ground);
		
		this.hmi_log_trace("HMI.prototype.switchGround - End");
	},
	
	/*********************************
		_getAndImportComponent
	*********************************/
	_getAndImportComponent: function (ComponentPath, ParentNode, Insert) {
		this.hmi_log_trace("HMI.prototype._getAndImportComponent - Start");
		
		var SVGDescription;
		
		//[StyleDescription] remove this if no ACPLT/HMI Server has a StyleDescription anymore
		if (HMI.ServerProperty.SheetHasStyleDescription){
			SVGDescription = '{' + ComponentPath + '.GraphicDescription' + '%20' + ComponentPath + '.StyleDescription' + '}';
		}else{
			SVGDescription = '{' + ComponentPath + '.GraphicDescription' + '}';
		}
		
		if (Insert === true){
		//	import and append
		//
			this.KSClient.getVar(null, SVGDescription, this._cbGetAndAddComponent);
		} else {
			//	import and replace
			//
			this.KSClient.getVar(null, SVGDescription, this._cbGetAndReplaceComponent);
		}
		delete SVGDescription;
		
		this.hmi_log_trace("HMI.prototype._getAndImportComponent - End");
		
		return;
	},
	
	/*********************************
		_GetAndShowComponent
	*********************************/	
	_GetAndShowComponent: function (Client, req, replace) {
		HMI.hmi_log_trace("HMI.prototype._GetAndShowComponent - Start");
		
		//[StyleDescription] adjust this line if no ACPLT/HMI Server has a StyleDescription anymore
		var ComponentText = new Array(2);
		var Component;
		
		if (req.responseText !== "")
		{
			ComponentText = HMI.KSClient.prepareComponentText(req.responseText);
			if (ComponentText === null){
				//logging not required, allready done by prepareComponentText
				return;
			}
			
			Component = HMI._importComponent(ComponentText);
			if (Component === null){
				//logging not required, allready done by _importComponent
				return;
			}
			if(!HMI.EmbedAdobePlugin){
				var template = Component;
				Component = document.importNode(template, true);
			}
			HMI.initGestures(Component);
			//Adobe does not fire mousemove event if there is no rect around the mouse. Build a invisible rect around everything 
			if (HMI.AdobeMoveFixNeeded && HMI.PluginVendor == 'Adobe'){
				var dummyRect = HMI.svgDocument.createElementNS(HMI.HMI_Constants.NAMESPACE_SVG, 'rect');
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
			if(replace === true){
				HMI.Playground.replaceChild(Component, HMI.Playground.firstChild);
			}else{
				HMI.Playground.appendChild(Component);
				
				//	set TimeoutID
				if (HMI.RefreshTimeoutID === null){
					HMI.RefreshTimeoutID = window.setInterval(function () {HMI.refreshSheet();}, HMI.RefreshTime);
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
		
		//[StyleDescription] adjust this line if no ACPLT/HMI Server has a StyleDescription anymore
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
			if (null === Element.getAttribute('pointer-events') || "" === Element.getAttribute('pointer-events')){
				Element.setAttribute('pointer-events', 'all');
			}
			
			//calculate and save absolute offset of the Components
			this._setLayerPosition(Element);
		};
		
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
			if (HMI.EmbedAdobePlugin)
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
		
		if (HMI.EmbedAdobePlugin){
			//getElementsByTagNameNS in Adobe is often not complete
			var ChildNodesLength = Fragment.childNodes.length;
			for (idx = 0; idx < ChildNodesLength; ++idx){
				if (Fragment.childNodes.item(idx).tagName == "svg:svg"){
					//recursive init necessary
					HMI.initGestures(Fragment.childNodes.item(idx));
				}
			}
			delete ChildNodesLength;
		}else if (Fragment.getElementsByTagNameNS){   //gecko, opera, webkit
			Elements = Fragment.getElementsByTagNameNS(HMI.HMI_Constants.NAMESPACE_SVG, 'svg');
			ElementLength = Elements.length;
			for (idx = 0; idx < ElementLength; ++idx){
				//trace log deactivated, causes performanceproblem in a production system
				//HMI.hmi_log_trace("HMI.initGestures - idx: "+ idx +" Element.length: "+ ElementLength);
				
				//recursive init not necessary, getElementsByTagNameNS is recursive by itself
				HMI._initGestures(Elements[idx]);
			}
		}else{   // IE if svg inline, useless since svg inline does not work with gestures
			Elements = Fragment.getElementsByTagName('svg');
			ElementLength = Elements.length;
			for (idx = 0; idx < ElementLength; ++idx){
				//trace log deactivated, causes performanceproblem in a production system
				//HMI.hmi_log_trace("HMI.initGestures - idx: "+ idx +" Element.length: "+ ElementLength);
				
				if (Elements[idx].tagUrn == HMI.HMI_Constants.NAMESPACE_SVG){
					HMI._initGestures(Elements[idx]);
				}
			}
		}
		delete ElementLength;
		delete Elements;
		delete idx;
		
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
		
		//conform to the standard would be "", but browser return null. not tested both because of speed
		if (Node.getAttribute("class") != null)
		{
			//	Space Delimiter
			//
			if (this._instanceOf(Node, ClassName, " ") === true)
			{
//				this.hmi_log_trace("HMI.prototype.instanceOf - Endt1");
				return true;
			} else {
				// COMMA delimeter
				//
				if (this._instanceOf(Node, ClassName, ",") === true)
				{
//					this.hmi_log_trace("HMI.prototype.instanceOf - Endt2");
					return true;
				} else {
					// SEMICOLON delimeter
					//
					if (this._instanceOf(Node, ClassName, ";") === true)
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
		var ClassesLength = Classes.length;
		for (idx = 0; idx < ClassesLength; idx++)
		{
			if (ClassName == Classes[idx])
			{
//				this.hmi_log_trace("HMI.prototype._instanceOf - Endt");
				
				return true;
			};
		};
		delete ClassesLength;
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
		
		if (isNaN(LayerX)){
			LayerX = 0;
		}
		if (isNaN(LayerY)){
			LayerY = 0;
		}
		
		//Firefox, Safari and Opera
		if (Element.ownerSVGElement !== undefined){
			if ( Element.ownerSVGElement !== null && Element.ownerSVGElement != document){
				LayerX += parseInt(Element.ownerSVGElement.getAttribute("layerX"), 10);
				LayerY += parseInt(Element.ownerSVGElement.getAttribute("layerY"), 10);
			}else if (Element.parentNode !== null && Element.parentNode.attributes !== null){
				LayerX += parseInt(Element.parentNode.getAttribute("layerX"), 10);
				LayerY += parseInt(Element.parentNode.getAttribute("layerY"), 10);
			}
		}else if(HMI.EmbedAdobePlugin && Element.parentNode !== null && Element.parentNode.attributes !== null && Element.parentNode.attributes.length !== 0){
			LayerX += parseInt(Element.parentNode.getAttribute("layerX"), 10);
			LayerY += parseInt(Element.parentNode.getAttribute("layerY"), 10);
		}
		
		if (isNaN(LayerX) || isNaN(LayerY)){
			this.hmi_log_warning("SVG-ERROR - ownerSVGElement/parentNode of\n"+Element.id+"\n is no hmi-component (has no layerX or layerY). The move-Gesture will not work on child elements!");
			Element.setAttribute("layerX", 0);
			Element.setAttribute("layerY", 0);
		}else{
			Element.setAttribute("layerX", LayerX);
			Element.setAttribute("layerY", LayerY);
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
			LayerX._xxx = null; delete LayerX._xxx;
			LayerY._xxx = null; delete LayerY._xxx;
		} catch (e) {   //IE does not like this hack
		}
		delete LayerX;
		delete LayerY;
		
		this.hmi_log_trace("HMI.prototype._setLayerPosition - End");
	},	
	
	/*********************************
		getClickPosition
	*********************************/
	getClickPosition: function (evt, Component) {
		this.hmi_log_trace("HMI.prototype.getClickPosition - Start");
		
		var clickPosition = new Array(2);
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
		
		var mousePosX;
		var mousePosY;
		if (evt.pageX || evt.pageY) {
			//code for native SVG. pageX based on the full XHTML Document
			mousePosX = evt.pageX;
			mousePosY = evt.pageY;
		}else{
			//code for plugin. clientX is based on the Plugin area, without browser scrolling sideeffects
			mousePosX = evt.clientX;
			mousePosY = evt.clientY;
		}
		
		//the searched position is pageX/clientX minus Position of the HMI Component minus Position of the SVG
		clickPosition[0] = mousePosX - parseInt(Component.getAttribute("layerX"), 10) - svgOffsetX;
		clickPosition[1] = mousePosY - parseInt(Component.getAttribute("layerY"), 10) - svgOffsetY;
		
		delete mousePosX;
		delete mousePosY;
		delete svgOffsetX;
		delete svgOffsetY;
		delete obj;
		
		this.hmi_log_trace("HMI.prototype.getClickPosition - End");
		
		return clickPosition;
	},
	
	/*********************************
		unload
	*********************************/
	unload: function () {
		this.hmi_log_trace("HMI.prototype.unload - Start");
		
		if (HMI){
			HMI.PossServers = null;
			HMI.PossSheets = null;
			
			if (HMI.KSClient && HMI.KSClient.TCLKSHandle !== null){
				HMI.KSClient.destroy();
			}
		}
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
		if (window.console && this.debug === true){
			window.console.debug("HMI_DEBUG: %s", text);
		}else if(window.opera){
			if (opera.postError !== undefined && this.debug === true){
				opera.postError("HMI_DEBUG: "+ text);
			}
		}
	},
	
	/*********************************
		hmi_log_error
	*********************************/
	hmi_log_error: function (text) {
		if (window.console && this.error === true){
			window.console.error("HMI_ERROR: %s", text);
		}else if(window.opera){
			if (opera.postError !== undefined && this.error === true){
				opera.postError("HMI_ERROR: "+ text);
			}
		}
	},
	
	/*********************************
		hmi_log_warning
	*********************************/
	hmi_log_warning: function (text) {
		if (window.console && this.warning === true){
			window.console.warn("HMI_WARNING: %s", text);
		}else if(window.opera){
			if (opera.postError !== undefined && this.warning === true){
				opera.postError("HMI_WARNING: "+ text);
			}
		}
	},
	
	/*********************************
		hmi_log_info
	*********************************/
	hmi_log_info: function (text) {
		if (window.console && this.info === true){
			window.console.info("HMI_INFO: %s", text);
		}else if(window.opera){
			if (opera.postError !== undefined && this.info === true){
				opera.postError("HMI_INFO: "+ text);
			}
		}
	},
	
	/*********************************
		hmi_log_trace
	*********************************/
	hmi_log_trace: function (text) {
		if (window.console && this.trace === true ){
			window.console.debug("HMI_TRACE: %s", text);
		}else if(window.opera){
			if (opera.postError !== undefined && this.trace === true){
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
		if (HMI.showHeader === false){
			HMI.hideHeader();
		}
	},
	
	/*********************************
		hmi_log_info_onwebsite
	*********************************/
	hmi_log_info_onwebsite: function (text) {
		if (this.InfoOutput){
			var InfoTextNode = document.createTextNode(text);
			deleteChilds(HMI.InfoOutput);
			HMI.InfoOutput.appendChild(InfoTextNode);
			//if header in not visible: show it
			if (HMI.showHeader === false){
				HMI.hideHeader();
			}
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
	//Internet Explorer is a special case as usual
	window.attachEvent('onload',function(){HMI.init();});
}

//fallback for stupid browsers (mostly Google Chrome) which fires onload to early, so our init code is never called
//
window.setTimeout(function(){HMI.init();}, 1000);
window.setTimeout(function(){HMI.init();}, 5000);

var filedate = "$Date: 2010-01-19 16:12:05 $";
filedate = filedate.substring(7, filedate.length-2);
if ("undefined" == typeof HMIdate){
	HMIdate = filedate;
}else if (HMIdate < filedate){
	HMIdate = filedate;
}
