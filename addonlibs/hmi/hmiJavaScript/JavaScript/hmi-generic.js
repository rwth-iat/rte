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
/**
	gets the total length of an Polyline Coords Object
	Coords.StartX; 
	Coords.StartY; 
	Coords.OffsetPointSourceX; 
	Coords.OffsetPointSourceY; 
	Coords.ContrlPointSourceX; 
	Coords.ContrlPointSourceY; 
	Coords.ContrlPointTargetX; 
	Coords.ContrlPointTargetY; 
	Coords.OffsetPointTargetX; 
	Coords.OffsetPointTargetY; 
	Coords.EndX; 
	Coords.EndY; 
*/
function getPolylineTotalLength(Coords){
	return getPolylinePointFromFraction(Coords, null);
}
/**
	gets the total length of an Polyline Coords Object

*/
function getPolylinePointFromFraction(Coords, Fraction){
	if(Coords === null || Coords === undefined){
		return 0;
	}
	sPosXY = new Object();
	sPosXY.x=null;
	sPosXY.y=null;
	
	var l1,l2,l3,l4,l5;
	var total_length, Position;
	l1= Math.sqrt( Math.pow(Coords.StartX - Coords.OffsetPointSourceX , 2)             + Math.pow(Coords.StartY - Coords.OffsetPointSourceY , 2)) ;
	l2= Math.sqrt( Math.pow(Coords.OffsetPointSourceX - Coords.ContrlPointSourceX , 2) + Math.pow(Coords.OffsetPointSourceY - Coords.ContrlPointSourceY , 2) ) ;
	l3= Math.sqrt( Math.pow(Coords.ContrlPointSourceX - Coords.ContrlPointTargetX , 2) + Math.pow(Coords.ContrlPointSourceY - Coords.ContrlPointTargetY , 2) ) ;
	l4= Math.sqrt( Math.pow(Coords.ContrlPointTargetX - Coords.OffsetPointTargetX , 2) + Math.pow(Coords.ContrlPointTargetY - Coords.OffsetPointTargetY , 2) ) ;
	l5= Math.sqrt( Math.pow(Coords.OffsetPointTargetX - Coords.EndX , 2)               + Math.pow(Coords.OffsetPointTargetY - Coords.EndY , 2) ) ;
	total_length=l1+l2+l3+l4+l5;
	
	if(Fraction === null || Fraction === undefined){
		return total_length;
	}
	
	Position=total_length * Fraction;
	if( Position<=l1 ){
		Fraction= (l1 + l2+l3+l4+l5 -(1-Fraction)*total_length)/l1;
		sPosXY.x= Coords.StartX+ Fraction* (Coords.OffsetPointSourceX-Coords.StartX);
		sPosXY.y= Coords.StartY+ Fraction* (Coords.OffsetPointSourceY-Coords.StartY);
		return sPosXY;
	}else if( Position<=(l1+l2) ){
		Fraction= (l2 + l3+l4+l5 -(1-Fraction)*total_length)/l2;
		sPosXY.x= Coords.OffsetPointSourceX+ Fraction* (Coords.ContrlPointSourceX-Coords.OffsetPointSourceX);
		sPosXY.y= Coords.OffsetPointSourceY+ Fraction* (Coords.ContrlPointSourceY-Coords.OffsetPointSourceY);
		return sPosXY;
	}else if ( Position<=(l1+l2+l3) ){
		Fraction= (l3 + l4+l5 -(1-Fraction)*total_length)/l3;
		sPosXY.x= Coords.ContrlPointSourceX+ Fraction* (Coords.ContrlPointTargetX-Coords.ContrlPointSourceX);
		sPosXY.y= Coords.ContrlPointSourceY+ Fraction* (Coords.ContrlPointTargetY-Coords.ContrlPointSourceY);
		return sPosXY;
	}else if ( Position<=(l1+l2+l3+l4) ){
		Fraction= (l4 + l5 -(1-Fraction)*total_length)/l4;
		sPosXY.x= Coords.ContrlPointTargetX+ Fraction* (Coords.OffsetPointTargetX-Coords.ContrlPointTargetX);
		sPosXY.y= Coords.ContrlPointTargetY+ Fraction* (Coords.OffsetPointTargetY-Coords.ContrlPointTargetY);
		return sPosXY;
	}else if ( Position<=(l1+l2+l3+l4+l5) ){
		Fraction= (l5 + 0 -(1-Fraction)*total_length)/l5;
		sPosXY.x= Coords.OffsetPointTargetX+ Fraction* (Coords.EndX-Coords.OffsetPointTargetX);
		sPosXY.y= Coords.OffsetPointTargetY+ Fraction* (Coords.EndY-Coords.OffsetPointTargetY);
		return sPosXY;
	}else{
		return sPosXY; 
	} 
}
