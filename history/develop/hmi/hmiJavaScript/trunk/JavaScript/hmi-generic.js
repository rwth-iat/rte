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
*	$Revision$
*	$Date$
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

/**
 * deletes all childs of a Node
 * @param ParentNode
 * @returns void
 */
function deleteChilds(ParentNode) {
	//HMI.hmi_log_trace("deleteChilds with id: "+ParentNode.id+" - Start");
	
	while (ParentNode.hasChildNodes() === true){
		ParentNode.removeChild(ParentNode.lastChild);
	}
	
	//HMI.hmi_log_trace("deleteChilds - End");
}

/**
 * checks if a variable is Numeric
 * http://stackoverflow.com/questions/18082
 * http://dl.dropbox.com/u/35146/js/tests/isNumber.html
 * @returns boolean
 */
function isNumeric(Value){
	if(!isNaN(parseFloat(Value)) && isFinite(Value)){
		return true;
	}
	return false;
}

/**
 * Feature - emulate XMLHttpRequest of old IEs with new API
 */
if( !window.XMLHttpRequest ) XMLHttpRequest = function(){
	try{ return new ActiveXObject("Msxml2.XMLHTTP.6.0"); }catch(e){}
	try{ return new ActiveXObject("Msxml2.XMLHTTP.3.0"); }catch(e){}
	try{ return new ActiveXObject("Msxml2.XMLHTTP"); }catch(e){}
	try{ return new ActiveXObject("Microsoft.XMLHTTP"); }catch(e){}
	throw new Error("Could not find an XMLHttpRequest alternative.");
};

/**
 * Feature - emulate String trim() in old browsers
 * trim is native on: Firefox 3.5, Internet Explorer 9, Webkit since Okt 2009, Opera since 2010
 */
if (!String.prototype.trim) {
	String.prototype.trim = function(){
		//performance test:
		//http://blog.stevenlevithan.com/archives/faster-trim-javascript
		return this.replace(/^\s\s*/, '').replace(/\s\s*$/, '');
	};
}

/**
 * Crossbrowser Eventhandling
 * http://www.quirksmode.org/js/eventSimple.html
 * used for the Select Menus
 * Adobe SVG Viewer supports addEventListener, so the gestures did not need a change
 */
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

/**
 * returns a recursive list of all elements with a requested class
 * @param node node to start the search
 * @param {String} newText a string representing the list of class names to match; class names are separated by whitespace
 * @return {NodeList} returnElements a live NodeList (but see the note below) of found elements in the order they appear in the tree.
 */

function csHMIgetElementsByClassName(node, className){
	node = node || HMI.svgDocument;
	if (node.getElementsByClassName){
		return node.getElementsByClassName(className);
	} else {
		var testClass = new RegExp("(^|\\s)" + className + "(\\s|$)");
		var elements = node.getElementsByTagNameNS(HMI.HMI_Constants.NAMESPACE_SVG, "*");
		var returnElements = [];
		var current;
		var length = elements.length;
		for(var i=0; i<length; i++){
			current = elements.item(i);
			if(testClass.test(current.getAttribute('class'))){
				returnElements.push(current);
			}
		}
		return returnElements;
	}
}

function getRotationFromObject(VisualObject){
	if (VisualObject.tagName === "g" && VisualObject.id === ""){
		//rotation should be logical on the child svg
		return 0;
	}else if (VisualObject.tagName === "svg" && VisualObject.parentNode && VisualObject.parentNode.tagName === "g" && VisualObject.parentNode.id === ""){
		//svg are not transformable, so the rotation is in the objects parent
		var TransformString = VisualObject.parentNode.getAttribute("transform");
	}else if (VisualObject.tagName === "svg"){
		//it is an template, with no rotation
		return 0;
	}else{
		TransformString = VisualObject.getAttribute("transform");
	}
	if(TransformString === "" || TransformString === null){
		return 0;
	}
	//"rotate(45,21.000000,50.000000)" or "rotate(45)"
	
	//remove rotate()
	TransformString = TransformString.replace(")", "").replace("rotate(", "");
	//get first number if there are 3, separated via comma
	return TransformString.split(",")[0];
}

var filedate = "$Date$";
filedate = filedate.substring(7, filedate.length-2);
if ("undefined" == typeof HMIdate){
	HMIdate = filedate;
}else if (HMIdate < filedate){
	HMIdate = filedate;
}
