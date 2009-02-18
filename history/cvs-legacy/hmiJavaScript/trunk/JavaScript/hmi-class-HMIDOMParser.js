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
*	$Revision: 1.15 $
*	$Date: 2009-02-18 12:35:12 $
*
*	History:
*	--------
*	21-June-2007			St
*		-	General Revision
*
***********************************************************************/

/***********************************************************************
	constructor
***********************************************************************/

function HMIDOMParser() {
};

/***********************************************************************
	prototype
***********************************************************************/

HMIDOMParser.prototype = {
	
	/*********************************
		parse
	*********************************/
	parse: function(GraphicDescription, StyleDescription, BehaviourDescription) {
		HMI.hmi_log_trace("HMIDOMParser.parse - Start");
		var GraphicElement;
		var StyleElement;
		var StyleElementNode;
		var Return;
		if (typeof DOMParser != "undefined"){
			//Mozilla has the DOMParser Object
			var Parser = new DOMParser();
			try {
				GraphicElement = Parser.parseFromString(GraphicDescription, "text/xml");
			} catch (e) {
				HMI.hmi_log_error('HMIDOMParser.prototype.parse: Could not parse GraphicDescription');
				HMI.hmi_log_onwebsite('Could not parse GraphicDescription');
				return null;
			};
			if (GraphicElement.documentElement.namespaceURI == "http://www.mozilla.org/newlayout/xml/parsererror.xml"){
				HMI.hmi_log_error('HMIDOMParser.prototype.parse: ParseError on GraphicDescription');
				HMI.hmi_log_onwebsite('ParseError on GraphicDescription');
				return null;
			};
			if (StyleDescription.length != 0){
				try {
					StyleElement = Parser.parseFromString(StyleDescription, "text/xml");
				} catch (e) {
					HMI.hmi_log_error('HMIDOMParser.prototype.parse: Could not parse StyleDescription');
					HMI.hmi_log_onwebsite('Could not parse StyleDescription');
					return null;
				};
				if (StyleElement.documentElement.namespaceURI == "http://www.mozilla.org/newlayout/xml/parsererror.xml"){
					HMI.hmi_log_error('HMIDOMParser.prototype.parse: ParseError on StyleDescription');
					HMI.hmi_log_onwebsite('ParseError on StyleDescription');
				}
				//GraphicElement has another DOM ownerDocument
				StyleElementNode = GraphicElement.importNode(StyleElement.firstChild, true);
			}
			delete Parser;
		}else if(HMI.svgWindow.parseXML && HMI.PlaygroundEmbedNode){
			//building an XML Tree works a bit different in Adobe SVG Viewer
			GraphicElement = HMI.svgWindow.parseXML(GraphicDescription,HMI.svgDocument);
			if(GraphicElement !== null){
				//Renesis' reaction to an iso-8859-1
				//http://tickets.examotion.com/public/view.php?id=32
				HMI.PlaygroundEmbedNode.setAttribute('height', GraphicElement.firstChild.getAttribute('height'));
				HMI.PlaygroundEmbedNode.setAttribute('width', GraphicElement.firstChild.getAttribute('width'));
				if (StyleDescription.length != 0){
					StyleElement = HMI.svgWindow.parseXML(StyleDescription,HMI.svgDocument);
					StyleElementNode = StyleElement.firstChild;
				}
			}else{
				HMI.hmi_log_error('HMIDOMParser.prototype.parse: ParseError on GraphicDescription');
				HMI.hmi_log_onwebsite('ParseError on GraphicDescription or umlaut problem');
				return null;
			}
		}else{
			//building an XML Tree works a bit different in IE
			//inline SVG with AdobePlugin, not capable of proper events! http://schepers.cc/inlinesvg.html
			var GraphicElement = new ActiveXObject("Microsoft.XMLDOM");
			var loadXMLresult;
			loadXMLresult = GraphicElement.loadXML(GraphicDescription);
			if (loadXMLresult == false){
				HMI.hmi_log_error('HMIDOMParser.prototype.parse: ParseError on GraphicDescription');
				HMI.hmi_log_onwebsite('ParseError on GraphicDescription');
				return null;
			};
			if (StyleDescription.length != 0){
				var StyleElement = new ActiveXObject("Microsoft.XMLDOM");
				loadXMLresult = StyleElement.loadXML(StyleDescription);
				if (loadXMLresult == false){
					HMI.hmi_log_error('HMIDOMParser.prototype.parse: ParseError on StyleDescription');
					HMI.hmi_log_onwebsite('ParseError on StyleDescription');
				};
				//IE doesnot provide importNode and does not need it for the appendChild
				StyleElementNode = StyleElement.firstChild;
			}
			delete loadXMLresult;
		}
		if (StyleDescription.length != 0){
			GraphicElement.firstChild.appendChild(StyleElementNode);
		}
		
		Return = GraphicElement.firstChild;
		
		delete StyleDescription;
		delete GraphicDescription;
		delete StyleElementNode;
		delete GraphicElement;
		delete StyleElement;
		HMI.hmi_log_trace("HMIDOMParser.parse - End");
		
		return Return;
	}
};
var filedate = "$Date: 2009-02-18 12:35:12 $";
filedate = filedate.substring(7, filedate.length-2);
if ("undefined" == typeof HMIdate){
	HMIdate = filedate;
}else if (HMIdate < filedate){
	HMIdate = filedate;
}
