/*
*   Copyright (C) 2013
*   Lehrstuhl fuer Prozessleittechnik,
*   D-52056 Aachen, Germany.
*   All rights reserved.
*
*
*   This file is part of the ACPLT/OV Package 
*   Licensed under the Apache License, Version 2.0 (the "License");
*   you may not use this file except in compliance with the License.
*   You may obtain a copy of the License at
*
*       http://www.apache.org/licenses/LICENSE-2.0
*
*   Unless required by applicable law or agreed to in writing, software
*   distributed under the License is distributed on an "AS IS" BASIS,
*   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*   See the License for the specific language governing permissions and
*   limitations under the License.
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
