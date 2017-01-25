/*
*	Copyright (C) 2013
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
*	hmi-class-HMIDOMParser.js
*
*	Editors:
*	--------
*	St							Stefan Schmitz <StefanS@plt.rwth-aachen.de>
*
*	Je							Holger Jeromin <Holger.Jeromin@plt.rwth-aachen.de>
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
*	25-February-2009			Je
*		-	General Revision and full commented
*
***********************************************************************/

/***********************************************************************
	constructor
***********************************************************************/

function HMIDOMParser() {
	//none
};

/***********************************************************************
	prototype
***********************************************************************/

HMIDOMParser.prototype = {
	
	/*********************************
		parse
	*********************************/
	//[StyleDescription] adjust this if no ACPLT/HMI Server has a StyleDescription anymore
	parse: function(GraphicDescription, StyleDescription) {
		HMI.hmi_log_trace("HMIDOMParser.parse - Start");
		var GraphicElement;
		var StyleElement;
		var StyleElementNode;
		var Return;
		if (typeof DOMParser != "undefined"){
			//Mozilla, Opera, Webkit and IE9 have the DOMParser Object
			var Parser = new DOMParser();
			try {
				GraphicElement = Parser.parseFromString(GraphicDescription, "text/xml");
			} catch (e) {
				//IE9 throws a SYNTAX_ERR DOM Exception: http://blogs.msdn.com/b/ie/archive/2010/10/15/domparser-and-xmlserializer-in-ie9-beta.aspx
				HMI.hmi_log_error('HMIDOMParser.prototype.parse: Could not parse GraphicDescription: '+e.message);
				HMI.hmi_log_onwebsite('Could not parse XML: '+e.message);
				return null;
			};
			//Mozilla has a very strange way of presenting an error
			//supported by Opera
			//not supported by Webkit https://bugs.webkit.org/show_bug.cgi?id=13057
			if (GraphicElement.documentElement.namespaceURI == "http://www.mozilla.org/newlayout/xml/parsererror.xml" && GraphicElement.documentElement.nodeName == "parsererror"){
				//opera/firefox has a highlighting of the error here:
				HMI.hmi_log_error('HMIDOMParser.prototype.parse: ParseError on first XML String: '+GraphicElement.documentElement.lastChild.textContent);
				//opera/firefox has a readable description here:
				HMI.hmi_log_onwebsite('Could not parse XML: '+GraphicElement.documentElement.firstChild.textContent);
				return null;
			}else if (GraphicElement.documentElement.firstChild && GraphicElement.documentElement.firstChild.nodeName == "parsererror"){
				if(GraphicElement.documentElement.firstChild && GraphicElement.documentElement.firstChild.children.length > 2 && GraphicElement.documentElement.firstChild.children[1].nodeName == "div"){
					HMI.hmi_log_error('HMIDOMParser.prototype.parse: ParseError on first XML String');
					//chrome Feb 2013 has the error info hidden in a div...
					HMI.hmi_log_onwebsite('Could not parse XML: '+GraphicElement.documentElement.firstChild.children[1].textContent);
				}else{
					HMI.hmi_log_error('HMIDOMParser.prototype.parse: ParseError on first XML String');
					HMI.hmi_log_onwebsite('Could not parse XML');
				}
				return null;
			}
			//StyleDescription is optional
			if (StyleDescription !== null && StyleDescription !== undefined){
				try {
					StyleElement = Parser.parseFromString(StyleDescription, "text/xml");
				} catch (e) {
					//IE9 throws a SYNTAX_ERR DOM Exception: http://blogs.msdn.com/b/ie/archive/2010/10/15/domparser-and-xmlserializer-in-ie9-beta.aspx
					HMI.hmi_log_error('HMIDOMParser.prototype.parse: Could not parse StyleDescription: '+e.message);
					HMI.hmi_log_onwebsite('Could not parse StyleDescription: '+e.message);
				};
				if (StyleElement.documentElement.namespaceURI == "http://www.mozilla.org/newlayout/xml/parsererror.xml"){
				//Mozilla has a very strange way of presenting an error
				//supported by Opera
				//not supported by Webkit https://bugs.webkit.org/show_bug.cgi?id=13057
					HMI.hmi_log_error('HMIDOMParser.prototype.parse: ParseError on StyleDescription');
					HMI.hmi_log_onwebsite('ParseError on StyleDescription');
				}
			}
			Parser = null;
		}else if(HMI.svgWindow.parseXML){
			//building an XML Tree works a bit different in Adobe SVG Viewer and Renesis
			GraphicElement = HMI.svgWindow.parseXML(GraphicDescription,HMI.svgDocument);
			if(GraphicElement !== null){
				//Renesis' reaction to an iso-8859-1
				//http://tickets.examotion.com/public/view.php?id=32
				
				//StyleDescription is optional
				if (StyleDescription !== null && StyleDescription !== undefined){
					StyleElement = HMI.svgWindow.parseXML(StyleDescription,HMI.svgDocument);
				}
			}else{
				HMI.hmi_log_error('HMIDOMParser.prototype.parse: ParseError on GraphicDescription');
				HMI.hmi_log_onwebsite('ParseError on GraphicDescription or umlaut problem');
				return null;
			}
		}else if(window.ActiveXObject){
			//building an XML Tree works a bit different in IE
			//inline SVG native or with AdobePlugin (not capable of proper events! http://schepers.cc/inlinesvg.html)
			
			GraphicElement = new ActiveXObject("Microsoft.XMLDOM");
			var loadXMLresult;
			loadXMLresult = GraphicElement.loadXML(GraphicDescription);
			if (loadXMLresult === false){
				HMI.hmi_log_error('HMIDOMParser.prototype.parse: ParseError on GraphicDescription');
				HMI.hmi_log_onwebsite('ParseError on GraphicDescription');
				return null;
			};
			//StyleDescription is optional
			if (StyleDescription !== null && StyleDescription !== undefined){
				StyleElement = new ActiveXObject("Microsoft.XMLDOM");
				loadXMLresult = StyleElement.loadXML(StyleDescription);
				if (loadXMLresult === false){
					HMI.hmi_log_error('HMIDOMParser.prototype.parse: ParseError on StyleDescription');
					HMI.hmi_log_onwebsite('ParseError on StyleDescription');
				};
			}
			loadXMLresult = null;
		}
		
		if (StyleDescription !== null && StyleDescription !== undefined){
			//GraphicElement has another DOM ownerDocument
			if("unknown" == typeof GraphicElement.importNode){
				//adobe plugin is buggy
				StyleElementNode = StyleElement.firstChild;
			}else if (GraphicElement.importNode){
				StyleElementNode = GraphicElement.importNode(StyleElement.firstChild, true);
			}
			GraphicElement.firstChild.appendChild(StyleElementNode);
		}
		
		if("unknown" == typeof HMI.svgDocument.importNode){
			//adobe plugin is buggy, but does not need importNode
			Return = GraphicElement.firstChild;
		}else if(HMI.svgDocument.importNode){
			Return = HMI.svgDocument.importNode(GraphicElement.firstChild, true);
		}else{
			Return = GraphicElement.firstChild;
		}
		
		StyleDescription = null;
		GraphicDescription = null;
		StyleElementNode = null;
		GraphicElement = null;
		StyleElement = null;
		HMI.hmi_log_trace("HMIDOMParser.parse - End");
		
		return Return;
	}
};
var filedate = "$Date$";
filedate = filedate.substring(7, filedate.length-2);
if ("undefined" == typeof HMIdate){
	HMIdate = filedate;
}else if (HMIdate < filedate){
	HMIdate = filedate;
}
