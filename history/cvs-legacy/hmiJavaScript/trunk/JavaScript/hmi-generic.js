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
*	hmi-generic.js
*
*	Editors:
*	--------
*	St							Stefan Schmitz <StefanS@plt.rwth-aachen.de>
*
*	Je							Holger Jeromin <Holger.Jeromin@plt.rwth-aachen.de>
*
*	CVS:
*	----
*	$Revision: 1.36 $
*	$Date: 2010-08-26 09:50:14 $
*
*	History:
*	--------
*	21-June-2007			St
*		-	General Revision
*
*	26-March-2008			Je
*		-	Header hidable
*
*	25-February-2009			Je
*		-	General Revision and full commented
*
************************************************************************/


/***************************************************
				Attention
	The HMI is parametrised in hmi-class-HMI.js
****************************************************/


/*********************************
	Functions - $
*********************************/

function $(id) {
	return document.getElementById(id);
}

/*********************************
	Functions - deleteChilds
*********************************/

function deleteChilds(ParentNode) {
	HMI.hmi_log_trace("deleteChilds with id: "+ParentNode.id+" - Start");
	
	while (ParentNode.childNodes.length > 0)
		ParentNode.removeChild(ParentNode.lastChild);
	
	HMI.hmi_log_trace("deleteChilds - End");
}

/*********************************
	Feature - emulate XMLHttpRequest of old IEs with new API
*********************************/
if( !window.XMLHttpRequest ) XMLHttpRequest = function(){
	try{ return new ActiveXObject("Msxml2.XMLHTTP.6.0"); }catch(e){}
	try{ return new ActiveXObject("Msxml2.XMLHTTP.3.0"); }catch(e){}
	try{ return new ActiveXObject("Msxml2.XMLHTTP"); }catch(e){}
	try{ return new ActiveXObject("Microsoft.XMLHTTP"); }catch(e){}
	throw new Error("Could not find an XMLHttpRequest alternative.");
};

/*********************************
	Feature - emulate document.importNode if not available native, based on
	http://www.alistapart.com/articles/crossbrowserscripting
	
	used in the SVG inline code in IE9, with does importNode wrong from XMLDOM
*********************************/
if (!document.importNodeIE9) {
	document.importNodeIE9 = function(node, allChildren) {
		switch (node.nodeType) {
		case 1:  //document.ELEMENT_NODE
			/* create a new element */
			if (document.createElementNS){
				var newNode = document.createElementNS(node.namespaceURI, node.nodeName);
			}else{
				var newNode = document.createElement(node.nodeName);
			}
			/* does the node have any attributes to add? */
			if (node.attributes && node.attributes.length > 0){
				/* add all of the attributes */
				for (var i = 0, il = node.attributes.length; i < il;i++){
					if (node.attributes[i].nodeName == "style"){
						//IE has problems assigning the style by setAttribute
						newNode.style.cssText = node.getAttribute(node.attributes[i].nodeName);
					}else{
						newNode.setAttribute(node.attributes[i].nodeName, node.getAttribute(node.attributes[i].nodeName));
					}
				}
			}
			/* are we going after children too, and does the node have any? */
			if (allChildren && node.childNodes && node.childNodes.length > 0)
				/* recursively get all of the child nodes */
				for (var i = 0, il = node.childNodes.length; i < il;)
					newNode.appendChild(document.importNodeIE9(node.childNodes[i++], allChildren));
			return newNode;
		case 3: //document.TEXT_NODE
		case 4: //document.CDATA_SECTION_NODE
		case 8: //document.COMMENT_NODE
			return document.createTextNode(node.nodeValue);
		}
		return null;
	};
}

/*********************************
	Crossbrowser Eventhandling
	http://www.quirksmode.org/js/eventSimple.html
	used for the Select Menus
	Adobe SVG Viewer supports addEventListener, so the gestures did not need a change
*********************************/
function addEventSimple(obj,evt,fn) {
	if (obj.namespaceURI == HMI.HMI_Constants.NAMESPACE_SVG || obj.addEventListener)
		obj.addEventListener(evt,fn,false);
	else if (obj.attachEvent)
		obj.attachEvent('on'+evt,fn);
}
function removeEventSimple(obj,evt,fn) {
	if (obj.namespaceURI == HMI.HMI_Constants.NAMESPACE_SVG || obj.removeEventListener)
		obj.removeEventListener(evt,fn,false);
	else if (obj.detachEvent)
		obj.detachEvent('on'+evt,fn);
}

/*********************************
	BrowserDetect solution provides:
		BrowserDetect.browser
		BrowserDetect.version
		BrowserDetect.OS
	from www.quirksmode.org
*********************************/
var BrowserDetect = {
	init: function () {
		this.browser = this.searchString(this.dataBrowser) || "An unknown browser";
		this.version = this.searchVersion(navigator.userAgent)
			|| this.searchVersion(navigator.appVersion)
			|| "an unknown version";
		this.OS = this.searchString(this.dataOS) || "an unknown OS";
	},
	searchString: function (data) {
		for (var i=0;i<data.length;i++)	{
			var dataString = data[i].string;
			var dataProp = data[i].prop;
			this.versionSearchString = data[i].versionSearch || data[i].identity;
			if (dataString) {
				if (dataString.indexOf(data[i].subString) != -1)
					return data[i].identity;
			}
			else if (dataProp)
				return data[i].identity;
		}
	},
	searchVersion: function (dataString) {
		var index = dataString.indexOf(this.versionSearchString);
		if (index == -1) return;
		return parseFloat(dataString.substring(index+this.versionSearchString.length+1));
	},
	dataBrowser: [
		{
			string: navigator.userAgent,
			subString: "Chrome",
			identity: "Chrome"
		},
		{ 	string: navigator.userAgent,
			subString: "OmniWeb",
			versionSearch: "OmniWeb/",
			identity: "OmniWeb"
		},
		{
			string: navigator.vendor,
			subString: "Apple",
			identity: "Safari"
		},
		{
			prop: window.opera,
			identity: "Opera"
		},
		{
			string: navigator.vendor,
			subString: "iCab",
			identity: "iCab"
		},
		{
			string: navigator.vendor,
			subString: "KDE",
			identity: "Konqueror"
		},
		{
			string: navigator.userAgent,
			subString: "Firefox",
			identity: "Firefox"
		},
		{
			string: navigator.vendor,
			subString: "Camino",
			identity: "Camino"
		},
		{		// for newer Netscapes (6+)
			string: navigator.userAgent,
			subString: "Netscape",
			identity: "Netscape"
		},
		{
			string: navigator.userAgent,
			subString: "MSIE",
			identity: "Explorer",
			versionSearch: "MSIE"
		},
		{
			string: navigator.userAgent,
			subString: "Gecko",
			identity: "Mozilla",
			versionSearch: "rv"
		},
		{ 		// for older Netscapes (4-)
			string: navigator.userAgent,
			subString: "Mozilla",
			identity: "Netscape",
			versionSearch: "Mozilla"
		}
	],
	dataOS : [
		{
			string: navigator.platform,
			subString: "Win",
			identity: "Windows"
		},
		{
			string: navigator.platform,
			subString: "Mac",
			identity: "Mac"
		},
		{
			string: navigator.platform,
			subString: "Linux",
			identity: "Linux"
		}
	]

};
BrowserDetect.init();

var filedate = "$Date: 2010-08-26 09:50:14 $";
filedate = filedate.substring(7, filedate.length-2);
if ("undefined" == typeof HMIdate){
	HMIdate = filedate;
}else if (HMIdate < filedate){
	HMIdate = filedate;
}
