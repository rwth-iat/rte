/*
*	Copyright (C) 2007
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
*	St							Stefan Schmitz <StefanS@plt.rwth-aachen.de>
*
*	CVS:
*	$Revision: 1.4 $
*	$Date: 2008-03-27 16:52:42 $
*
*	History:
*	01-March-2005			HA
*		-	File created
*
*	03-July-2007			St
*		-	General Revision
*
***********************************************************************/


/***********************************************************************
	Temporary workaround for Mozilla bug #276037
***********************************************************************/

function SCRIPT_HUB_SEARCH(node, elementName, result) {
	if (node.ELEMENT_NODE == undefined){
//		alert("kein element node attribut");
		node.ELEMENT_NODE = 1;
	}
	if (	(node.nodeType == node.ELEMENT_NODE)
		&&	(node.nodeName == elementName))
	{
		result.push(node);
	};
	
	var children = node.childNodes;
	for (var idx = 0, len = children.length; idx < len; ++idx )
	{
		SCRIPT_HUB_SEARCH(children[idx], elementName, result);
	};
}

/***********************************************************************
	Loads the specified list of script files by creating the necessary
	<script> elements in the current document's DOM tree.
	Supported XML-based languages are HTML, XHTML, SVG.
	
	@param hubLoaderName
	@param hubFilePattern
	@param hubFilelist
***********************************************************************/

function SCRIPT_HUB(hubFilePattern, hubFilelist) {
	
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
			- <svg:script type="" xlink:href="">, where the svg
				namespace is "http://www.w3.org/2000/svg".
	********************************************************************/
	var scripts = new Array();
	SCRIPT_HUB_SEARCH(document, "script", scripts);
	
	var p = new RegExp(hubFilePattern);
	var base = null;
	var scriptAnchor = null;
	
	for ( var idx = 0; idx < scripts.length; ++idx )
	{
		var scriptNode = scripts[idx];
		var match = null;
	
		switch (scriptNode.namespaceURI)
		{
			//	Support for HTML and SVG <script> elements.
			//
			case null:
			case "http://www.w3.org/1999/xhtml":
				var match = p.exec(scriptNode.getAttribute("src"));
				break;
	
			//	Support for SVG <script> elements is slightly different from
			//	HTML and XHTML: no "src" attribute in the empty namespace but
			//	instead an xlink:href attribute. Sigh.
			//		
			case "http://www.w3.org/2000/svg":
				var match = p.exec(scriptNode.getAttributeNS(
					"http://www.w3.org/1999/xlink", "href"));
		};
		
		if (match)
		{
			base = match[1];
			scriptAnchor = scriptNode.parentNode;
			break;
		};
	};
	
	if (base == null)
	{
		alert("Fatal error: script hub loader unable to locate base in document");
		throw new Error("hub loader base location error");
	};
	
	/********************************************************************
		Now decide on the proper way of creating a script element.
	********************************************************************/
	var createScriptNode = null;
	switch ( scriptAnchor.namespaceURI )
	{
		//	Support for HTML and SVG <script> elements.
		//
		case null:
		case "http://www.w3.org/1999/xhtml":
			createScriptNode = function(document, source)
			{
				var node = document.createElementNS(
					scriptAnchor.namespaceURI, "script");
				node.setAttributeNS(null, "src", source);
				return node;
			};
			break;
			
		//	Support for SVG <script> elements is slightly different from
		//	HTML and XHTML: no "src" attribute in the empty namespace but
		//	instead an xlink:href attribute. Sigh.
		//
		case "http://www.w3.org/2000/svg":
			createScriptNode = function(document, source)
			{
				var node = document.createElementNS(
					scriptAnchor.namespaceURI, "script");
				node.setAttributeNS("http://www.w3.org/1999/xlink", "href",
					source);
				return node;
			};
			break;
	};
	
	/********************************************************************
   	Now load all script files that have been specified in the second
   	parameter (that is, the list of file names relative to the base
   	URL of this hub script).
   	
   	When it comes to the question of where to place the new scripting
   	elements within the DOM: just append them to the same parent
   	where we found our script element.
	********************************************************************/
	for ( var idx in hubFilelist )
	{
		scriptAnchor.appendChild(
			createScriptNode(document, base + hubFilelist[idx]));
	};
};

/***********************************************************************
	Finally let the whole strange magic commence.
***********************************************************************/

SCRIPT_HUB(
	//	You need to specify a regular pattern for detecting this script
	//
	"(.*/)hmi-hub-loader.js",
	
	//	List of script files to load
	[	
		"./hmi-class-HMIJavaScriptKSClient.js",
		"./hmi-class-HMIDOMParser.js",
		"./hmi-class-HMI.js",
		"./hmi-class-gesture-click.js",
		"./hmi-class-gesture-doubleclick.js",
		"./hmi-class-gesture-rightclick.js",
		"./hmi-class-gesture-move.js",
		"./hmi-class-gesture-textinput.js",
		"./hmi-generic.js",
	]
);