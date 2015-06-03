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
*	hmi-hub-loader.js
*
*	Editors:
*	HA							Harald Albrecht <harald@plt.rwth-aachen.de>
*
*	St							Stefan Schmitz <StefanS@plt.rwth-aachen.de>
*
*	Je							Holger Jeromin <Holger.Jeromin@plt.rwth-aachen.de>
*
*	CVS:
*	$Revision$
*	$Date$
*
*	History:
*	01-March-2005			HA
*		-	File created
*
*	03-July-2007			St
*		-	General Revision
*
*	25-February-2009			Je
*		-	General Revision and full commented
*
*	03-February-2010			Je
*		-	rewritten with object detection
*		-	dropped SVG DOM support
*
***********************************************************************/

/***********************************************************************
	Loads the specified list of script files by creating the necessary
	<script> elements in the current document's DOM tree.
	Supported XML-based languages are HTML, XHTML.
	
	@param loadScriptlist
	@param callFnc
	@param async
	@param hubFilePattern
	@param mimetype
	@return will return true, if the callFnc is executed, false if this will called async, null on error
***********************************************************************/

function loadScriptUrls(loadScriptlist, callFnc, async, hubFilePattern, mimetype) {
	
	/********************************************************************
		We first try to find from where this hub file has been loaded.
		Our strategy is comparable simple: we scan all script elements until
		we find the one that is referencing us -- but since we can't
		know our name (we don't now which script element referenced us),
		we require the caller to supply a suitable regular expression for
		finding this out.
		
		Possible script elements are:
			- <script type="" src="">, where script has no
				associated namespace. Yes, this is good old HTML.
			- <xhtml:script type="" src="">, where the xhtml
				namespace is "http://www.w3.org/1999/xhtml".
	********************************************************************/
	
	var scripts = document.getElementsByTagName("script");
	
	var p = new RegExp(hubFilePattern);
	var base = "";
	var scriptAnchor = document.head || document.getElementsByTagName('head')[0];
	var match = null;
	var existingScriptNode = null;
	var existingScriptNodeUrl = null;
	var idx = 0;
	
	/********************************************************************
		search our existingScriptNode for path of the JS-files and <head> element
	********************************************************************/
	for ( idx = 0; idx < scripts.length; ++idx ) {
		existingScriptNode = scripts[idx];
		match = null;
		if (existingScriptNode.src !== undefined){
			//defined in W3C DOM Level 2 HTML (HTML4 and XHTML1.0) so probable usable in XHTML 1.1
			//opera sometimes gets the Filename as "hmi%2fhub%2floader.js". Fixed with decodeURI
			//
			existingScriptNodeUrl = decodeURI(existingScriptNode.src);
		}else if (existingScriptNode.getAttribute !== undefined && (existingScriptNode.getAttribute("src") !== null || existingScriptNode.getAttribute("src") !== "") ){
			existingScriptNodeUrl = existingScriptNode.getAttribute("src");
		}else if(existingScriptNode.hasAttributeNS !== undefined && existingScriptNode.getAttributeNS !== undefined){
			if (existingScriptNode.hasAttributeNS("http://www.w3.org/1999/xlink", "href") ){
				existingScriptNodeUrl = existingScriptNode.getAttributeNS("http://www.w3.org/1999/xlink", "href");
			}
		}
		if(hubFilePattern){
			match = p.exec(existingScriptNodeUrl);
			if (match){
				base = match[1];
			}
		}
		
		//search if a requested url is already loaded in the browser
		for (var intidx = 0 ; intidx < loadScriptlist.length ; ){
			if (loadScriptlist[intidx] === existingScriptNodeUrl){
				//remove the url from the Filelist
				loadScriptlist.splice(intidx, 1);
			}else{
				intidx++
			}
		}
	}
	
	if(loadScriptlist.length === 0){
		var executeFncIfReady = function(){
			var scripts = document.getElementsByTagName("script");
			for ( var idx = 0; idx < scripts.length; ++idx ) {
				if(scripts[idx].hasAttribute("data-transferstatus")){
					//we are not ready yet, try again later
					window.setTimeout(executeFncIfReady, 200);
					return;
				}
			}
			callFnc();
		};
		// nothing to do, so execute the code after a few ms to wait for the dom load
		window.setTimeout(executeFncIfReady, 100);
		
		return true;
	}

	/********************************************************************
		Decide on the proper way of creating a script element.
	********************************************************************/
	
	var node = null;
	for ( idx = 0 ; idx < loadScriptlist.length ; idx++){
		if (document.createElementNS !== undefined){
			node = document.createElementNS("http://www.w3.org/1999/xhtml", "script");
		}else if (document.createElement !== undefined){
			node = document.createElement("script");
		}else{
			window.alert("Fatal error: script hub loader unable to create new script node in document");
			return null;
		}
		if (node.type !== undefined){
			node.type = "text/javascript";
		}else{
			node.setAttribute("type", "text/javascript");
		}
		if(mimetype){
			node.setAttribute("charset", mimetype);
		}
		if(		loadScriptlist[idx].indexOf("http://") === 0
				||	loadScriptlist[idx].indexOf("https://") === 0
				||	loadScriptlist[idx].indexOf("//") === 0
			||	!base){
			//this is a full path or base path is not known (so just try)
			var srcEntry = loadScriptlist[idx];
		}else{
			srcEntry = base+loadScriptlist[idx];
		}
		if (node.src !== undefined){
			//defined in W3C DOM Level 2 HTML (HTML4 and XHTML1.0) so probable usable in XHTML 1.1
			node.src = srcEntry;
		}else{
			//not supported by Firefox 3.6 and Chrome 5
			node.setAttribute("src", srcEntry);
		}
		//remember the original name for later. src is manipulated by the browsers
		node.setAttribute("data-src", loadScriptlist[idx]);
		node.setAttribute("data-transferstatus", "loading");
		
		//some code could get parsed async as soon as it is fetched, without blocking the browser
		if (node.async !== undefined && async === true){
			node.async = true;
		}else if (node.defer !== undefined && async === true){
			//the code should get parsed as soon the html parser is ready, without him
			//(only if async is not supported) 
			node.defer = true;
		}else if(node.async !== undefined && async !== true){
			//Firefox 4 no longer enforces execution in insertion order if async is unset
			node.async = false;
		}
		node.onload = node.onerror = function(evt){
			if(evt.type !== "load"){
				if(typeof HMI !== "undefined" && HMI && HMI.hmi_log_onwebsite){
					HMI.hmi_log_onwebsite("Loading "+evt.target.src+" failed.");
				}
			}else{
				if(typeof HMI !== "undefined" && HMI && HMI.hmi_log_info){
					HMI.hmi_log_info("Success in loading script: "+evt.target.src);
				}
			}
			if(loadScriptlist !== null){
				var idx = loadScriptlist.indexOf(evt.target.getAttribute("data-src"));
				if(idx >= 0){
					//we found our entry, so remove it from the loadlist
					loadScriptlist.splice(idx, 1);
					//mark this node as ready
					evt.target.removeAttribute("data-transferstatus")
				}
				
				if(loadScriptlist.length === 0){
					// we have everything, so execute the code
					callFnc();
				}
			}
		};

		if (scriptAnchor !== null && scriptAnchor.appendChild !== undefined){
			scriptAnchor.appendChild(node);
			//some blackberrys kill scriptAnchor, so HMI will not work after that
			if (scriptAnchor === undefined){
				window.setTimeout(function(){
					if (document.getElementById('idErrorOutput')){
						document.getElementById('idErrorOutput').appendChild(document.createTextNode('Sorry, Browser not supported.'));
					}
				}, 5000);
			}
		}else{
			window.alert("Fatal error: script hub loader unable to append new script node into document");
			return null;
		}
	}
	return false;
}


/***********************************************************************
	Finally let the whole strange magic commence.
***********************************************************************/

loadScriptUrls(
	//	List of script files to load
	[	
		"hmi-generic.js",
		"hmi-class-HMIJavaScriptKSClient.js",
		"hmi-class-HMIDOMParser.js",
		"hmi-class-gesture-click.js",
		"hmi-class-gesture-doubleclick.js",
		"hmi-class-gesture-rightclick.js",
		"hmi-class-gesture-move.js",
		"hmi-class-gesture-textinput.js",
		"hmi-class-cshmi.js",
		//wheelsupport is not supported by the HMI Team and probably firefox only
		"hmi-class-gesture-wheelscroll.js",
		"hmi-class-HMI.js"
	],
	function(){HMI.init();},
	true,
	//	a regular pattern for detecting this script the basepath
	//
	"(.*/)hmi-hub-loader.js",
	"windows-1252"
);

var HMIdate;	//this is the first file, so the var declaration is allowed

var filedate = "$Date$";
filedate = filedate.substring(7, filedate.length-2);
if ("undefined" == typeof HMIdate){
	HMIdate = filedate;
}else if (HMIdate < filedate){
	HMIdate = filedate;
}
