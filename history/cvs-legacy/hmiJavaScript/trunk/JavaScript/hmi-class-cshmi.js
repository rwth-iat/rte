/*
*	Copyright (C) 2011
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
*	hmi-class-cshmi.js
*
*	Autoren:
*	--------
*	St							Stefan Schmitz <StefanS@plt.rwth-aachen.de>
*	Je							Holger Jeromin <Holger.Jeromin@plt.rwth-aachen.de>
*
*	CVS:
*	----
*	$Revision: 1.26 $
*	$Date: 2011-03-11 15:20:21 $
*
*	History:
*	--------
*	11-August-2011			Je
*		-	file created
*
*	15-September-2011			Je
*		-	rebuild for cshmi
*
***********************************************************************/

/***********************************************************************
	constructor
***********************************************************************/

function cshmi() {
	this.TksGetChildInfo = "%20-type%20$::TKS::OT_DOMAIN%20-output%20[expr%20$::TKS::OP_NAME%20|%20$::TKS::OP_CLASS]";
//	this.TksGetVariableInfo = "%20-type%20$::TKS::OT_VARIABLE%20-output%20[expr%20$::TKS::OP_NAME%20|%20$::TKS::OP_CLASS]";
};





//#########################################################################################################################
//fixme: 
//#########################################################################################################################

/***********************************************************************
	prototype
***********************************************************************/
cshmi.prototype = {
	/*********************************
		instanciateCshmi
	*********************************/
	instanciateCshmi: function (ObjectPath) {
		
		var Component = this.BuildDomain(null, ObjectPath, "/Libraries/cshmi/Group");
		
		if (Component !== null){
			HMI.Playground.appendChild(Component);
		}
	},
	/*********************************
		BuildDomain
		-	Main interation loop for visualisation
		-	Finds and arms Actions as well
	*********************************/
	BuildDomain: function (ObjectParent, ObjectPath, ObjectType) {
		var ChildrenNeeded = false;
		var Component = null;
		
		if (ObjectType == "/Libraries/cshmi/Group"){
			Component = this._buildSvgContainer(ObjectParent, ObjectPath);
			ChildrenNeeded = true;
		}else if (ObjectType == "/Libraries/cshmi/Line"){
			Component = this._buildSvgLine(ObjectParent, ObjectPath);
			ChildrenNeeded = true;
		}else if (ObjectType == "/Libraries/cshmi/Polyline"){
			Component = this._buildSvgPolyline(ObjectParent, ObjectPath);
			ChildrenNeeded = true;
		}else if (ObjectType == "/Libraries/cshmi/Text"){
			Component = this._buildSvgText(ObjectParent, ObjectPath);
			ChildrenNeeded = true;
		}else if (ObjectType == "/Libraries/cshmi/Circle"){
			Component = this._buildSvgCircle(ObjectParent, ObjectPath);
			ChildrenNeeded = true;
		}else if (ObjectType == "/Libraries/cshmi/Rectangle"){
			Component = this._buildSvgRect(ObjectParent, ObjectPath);
			ChildrenNeeded = true;
		}else if (ObjectType == "/Libraries/cshmi/ClientEvent"){
			Component = this._interpreteClientEvent(ObjectParent, ObjectPath);
		}else if (ObjectType == "/Libraries/cshmi/TimeEvent"){
			HMI.hmi_log_info("TimeEvent "+ObjectPath+" noch nicht unterstützt");
		}else if (ObjectType == "/Libraries/cshmi/Gesture"){
			HMI.hmi_log_info("Gesture "+ObjectPath+" noch nicht unterstützt");
		}else{
			HMI.hmi_log_info("Objekt(Typ: "+ObjectType+"): "+ObjectPath+" nicht unterstützt");
		}
		
		//break if no recursion is needed
		if (ChildrenNeeded === false){
			return Component;
		}
		
		var responseArray = HMI.KSClient.getChildObjArray(ObjectPath);
		for (var i=0; i < responseArray.length; i++) {
			var varName = responseArray[i].split(" ");
			var ChildComponent = this.BuildDomain(Component, Component.id+"/"+varName[0], varName[1]);
			if (ChildComponent !== null){
				Component.appendChild(ChildComponent);
			}
		}
		return Component;
	},
	/*********************************
		_interpreteClientEvent
		-	
	*********************************/
	_interpreteClientEvent: function(ObjectParent, ObjectPath){
		var command = ObjectPath.split("/");
		if (command[command.length-1] === "onload"){
			//interprete Action now (todo perhaps as a callback after load)
			this._interpreteAction(ObjectParent, ObjectPath);
			
			return null;
		}else{
			HMI.hmi_log_info("ClientEvent "+ObjectPath+" nicht unterstützt");
			return null;
		}
	},
	_interpreteAction: function(ObjectParent, ObjectPath){
			var responseArray = HMI.KSClient.getChildObjArray(ObjectPath);
			for (var i=0; i < responseArray.length; i++) {
				var varName = responseArray[i].split(" ");
				if (varName[1] == "/Libraries/cshmi/SetVar"){
					this._setVar(ObjectParent, ObjectPath+"/"+varName[0]);
				}else{
					HMI.hmi_log_info("Action "+ObjectPath+" nicht unterstützt");
				}
			}
	},
	_getValue: function(ObjectParent, ObjectPath){
		var response = HMI.KSClient.getVar(null, '{'+ObjectPath+'.ksvar .elemvar .globalvar}', null);
		if (response.indexOf("KS_ERR") !== -1){
			HMI.hmi_log_error("cshmi._buildSvgContainer of "+ObjectPath+" failed: "+response);
			HMI.hmi_log_onwebsite("Visualising the sheet failed.");
			
			return null;
		}
		var responseArray = HMI.KSClient.splitKsResponse(response);
		
		for (var i=0; i < responseArray.length; i++) {
			if (responseArray[i] !== ""){
				if (i == 0){
					//ksvar
					response = HMI.KSClient.getVar(null, '{'+responseArray[i]+'}', null);
					responseArray = HMI.KSClient.splitKsResponse(response);
					return responseArray[0];
				}else if (i == 1){
					//elemvar
					debugger;
					//todo
					return null;
				}else if (i == 2){
					//globalvar
					debugger;
					
					//todo
					return null;
				}
			}
		}
	},
	_setVar: function(ObjectParent, ObjectPath){
		//get Value to set (via getValue-part of setVar Object)
		var NewValue = this._getValue(ObjectParent, ObjectPath+".value");
		
		//get info where to set the NewValue
		var response = HMI.KSClient.getVar(null, '{'+ObjectPath+'.ksvar .elemvar .globalvar}', null);
		if (response.indexOf("KS_ERR") !== -1){
			HMI.hmi_log_error("cshmi._buildSvgContainer of "+ObjectPath+" failed: "+response);
			HMI.hmi_log_onwebsite("Visualising the sheet failed.");
			
			return null;
		}
		var responseArray = HMI.KSClient.splitKsResponse(response);
		
		//set the new Value
		for (var i=0; i < responseArray.length; i++) {
			if (responseArray[i] !== ""){
				if (i == 0){
					//ksvar
					HMI.KSClient.setVar(null, '{'+responseArray[i]+'}', NewValue, null);
					return null;
				}else if (i == 1){
					//elemvar
					if (ObjectParent.hasAttribute(responseArray[i])){
						ObjectParent.setAttribute(responseArray[i], NewValue);
					}else if (responseArray[i] == "content"){
						//content is special, as it is different in OVM and SVG
						ObjectParent.replaceChild(HMI.svgDocument.createTextNode(NewValue), ObjectParent.firstChild);
					}
					return null;
				}else if (i == 2){
					//globalvar
					debugger;
					
					//todo
					return null;
				}
			}
		}
	},
	_buildSvgContainer: function(ObjectParent, ObjectPath){
		var response = HMI.KSClient.getVar(null, '{'+ObjectPath+'.visible .x .y .width .height}', null);
		if (response.indexOf("KS_ERR") !== -1){
			HMI.hmi_log_error("cshmi._buildSvgContainer of "+ObjectPath+" failed: "+response);
			HMI.hmi_log_onwebsite("Visualising the sheet failed.");
			
			return null;
		}
		var responseArray = HMI.KSClient.splitKsResponse(response);
		
		if (responseArray[0] != "FALSE"){
			var svgElement = HMI.svgDocument.createElementNS(HMI.HMI_Constants.NAMESPACE_SVG, 'svg');
			svgElement.id = ObjectPath;
			
			this._addClass(svgElement, "cshmi-group");
			//svgElement.setAttribute("xmlns:svg", HMI.HMI_Constants.NAMESPACE_SVG);
			
			//set dimension of container
			svgElement.setAttribute("x", responseArray[1]);
			svgElement.setAttribute("y", responseArray[2]);
			svgElement.setAttribute("width", responseArray[3]);
			svgElement.setAttribute("height", responseArray[4]);
			
			return svgElement;
		}
		return null;
	},
	_buildSvgLine: function(ObjectParent, ObjectPath){
		var response = HMI.KSClient.getVar(null, '{'+ObjectPath+'.visible .x1 .y1 .x2 .y2 .stroke}', null);
		if (response.indexOf("KS_ERR") !== -1){
			HMI.hmi_log_error("cshmi._buildSvgLine of "+ObjectPath+" failed: "+response);
			return null;
		}
		var responseArray = HMI.KSClient.splitKsResponse(response);
		
		if (responseArray[0] == "FALSE"){
			return null;
		}
		var svgElement = HMI.svgDocument.createElementNS(HMI.HMI_Constants.NAMESPACE_SVG, 'line');
		svgElement.id = ObjectPath;
		
		//set dimension of container
		svgElement.setAttribute("x1", responseArray[1]);
		svgElement.setAttribute("y1", responseArray[2]);
		svgElement.setAttribute("x2", responseArray[3]);
		svgElement.setAttribute("y2", responseArray[4]);
		svgElement.setAttribute("stroke", responseArray[5]);
		
		return svgElement;
	},
	_buildSvgPolyline: function(ObjectParent, ObjectPath){
		var response = HMI.KSClient.getVar(null, '{'+ObjectPath+'.visible .points .stroke .fill}', null);
		if (response.indexOf("KS_ERR") !== -1){
			HMI.hmi_log_error("cshmi._buildSvgPolyline of "+ObjectPath+" failed: "+response);
			return null;
		}
		var responseArray = HMI.KSClient.splitKsResponse(response);
		
		if (responseArray[0] == "FALSE"){
			return null;
		}
		var svgElement = HMI.svgDocument.createElementNS(HMI.HMI_Constants.NAMESPACE_SVG, 'polyline');
		svgElement.id = ObjectPath;
		
		//set dimension of container
		svgElement.setAttribute("points", responseArray[1]);
		svgElement.setAttribute("stroke", responseArray[2]);
		svgElement.setAttribute("fill", responseArray[3]);
		
		return svgElement;
	},
	_buildSvgPolygon: function(ObjectParent, ObjectPath){
		var response = HMI.KSClient.getVar(null, '{'+ObjectPath+'.visible .points .stroke .fill}', null);
		if (response.indexOf("KS_ERR") !== -1){
			HMI.hmi_log_error("cshmi._buildSvgPolygon of "+ObjectPath+" failed: "+response);
			return null;
		}
		
		var responseArray = HMI.KSClient.splitKsResponse(response);
		
		if (responseArray[0] == "FALSE"){
			return null;
		}
		var svgElement = HMI.svgDocument.createElementNS(HMI.HMI_Constants.NAMESPACE_SVG, 'polygon');
		svgElement.id = ObjectPath;
		
		//set dimension of container
		svgElement.setAttribute("points", responseArray[1]);
		svgElement.setAttribute("stroke", responseArray[2]);
		svgElement.setAttribute("fill", responseArray[3]);
		
		return svgElement;
	},
	_buildSvgPath: function(ObjectParent, ObjectPath){
		var response = HMI.KSClient.getVar(null, '{'+ObjectPath+'.visible .d .stroke .fill}', null);
		if (response.indexOf("KS_ERR") !== -1){
			HMI.hmi_log_error("cshmi._buildSvgPath of "+ObjectPath+" failed: "+response);
			return null;
		}
		
		var responseArray = HMI.KSClient.splitKsResponse(response);
		
		if (responseArray[0] == "FALSE"){
			return null;
		}
		var svgElement = HMI.svgDocument.createElementNS(HMI.HMI_Constants.NAMESPACE_SVG, 'path');
		svgElement.id = ObjectPath;
		
		//set dimension of container
		svgElement.setAttribute("d", responseArray[1]);
		svgElement.setAttribute("stroke", responseArray[2]);
		svgElement.setAttribute("fill", responseArray[3]);
		
		return svgElement;
	},
	_buildSvgText: function(ObjectParent, ObjectPath){
		var response = HMI.KSClient.getVar(null, '{'+ObjectPath+'.visible .x .y .fontSize .fontStyle .fontWeight .fontFamily .horAlignment .verAlignment .stroke .fill .content}', null);
		if (response.indexOf("KS_ERR") !== -1){
			HMI.hmi_log_error("cshmi._buildSvgText of "+ObjectPath+" failed: "+response);
			return null;
		}
		
		var responseArray = HMI.KSClient.splitKsResponse(response);
		
		if (responseArray[0] == "FALSE"){
			return null;
		}
		var svgElement = HMI.svgDocument.createElementNS(HMI.HMI_Constants.NAMESPACE_SVG, 'text');
		svgElement.id = ObjectPath;
		
		//set dimension of container
		svgElement.setAttribute("x", responseArray[1]);
		svgElement.setAttribute("y", responseArray[2]);
		svgElement.setAttribute("font-size", responseArray[3]);
		svgElement.setAttribute("font-style", responseArray[4]);
		svgElement.setAttribute("font-weight", responseArray[5]);
		svgElement.setAttribute("font-family", responseArray[6]);
		svgElement.setAttribute("text-anchor", responseArray[7]);
		svgElement.setAttribute("dominant-baseline", responseArray[8]);
		//todo rebuild for <tspan>
		svgElement.setAttribute("stroke", responseArray[9]);
		svgElement.setAttribute("fill", responseArray[10]);
		
		svgElement.appendChild(HMI.svgDocument.createTextNode(responseArray[11]));
		
		return svgElement;
	},
	_buildSvgCircle: function(ObjectParent, ObjectPath){
		var response = HMI.KSClient.getVar(null, '{'+ObjectPath+'.visible .cx .cy .r .stroke .fill}', null);
		if (response.indexOf("KS_ERR") !== -1){
			HMI.hmi_log_error("cshmi._buildSvgCircle of "+ObjectPath+" failed: "+response);
			return null;
		}
		var responseArray = HMI.KSClient.splitKsResponse(response);
		
		if (responseArray[0] == "FALSE"){
			return null;
		}
		var svgElement = HMI.svgDocument.createElementNS(HMI.HMI_Constants.NAMESPACE_SVG, 'circle');
		svgElement.id = ObjectPath;
		
		//set dimension of container
		svgElement.setAttribute("cx", responseArray[1]);
		svgElement.setAttribute("cy", responseArray[2]);
		svgElement.setAttribute("r", responseArray[3]);
		svgElement.setAttribute("stroke", responseArray[4]);
		svgElement.setAttribute("fill", responseArray[5]);
		
		return svgElement;
	},
	_buildSvgEllipse: function(ObjectParent, ObjectPath){
		var response = HMI.KSClient.getVar(null, '{'+ObjectPath+'.visible .cx .cy .rx .ry .stroke .fill}', null);
		if (response.indexOf("KS_ERR") !== -1){
			HMI.hmi_log_error("cshmi._buildSvgEllipse of "+ObjectPath+" failed: "+response);
			return null;
		}
		var responseArray = HMI.KSClient.splitKsResponse(response);
		
		if (responseArray[0] == "FALSE"){
			return null;
		}
		var svgElement = HMI.svgDocument.createElementNS(HMI.HMI_Constants.NAMESPACE_SVG, 'ellipse');
		svgElement.id = ObjectPath;
		
		//set dimension of container
		svgElement.setAttribute("cx", responseArray[1]);
		svgElement.setAttribute("cy", responseArray[2]);
		svgElement.setAttribute("rx", responseArray[3]);
		svgElement.setAttribute("ry", responseArray[4]);
		svgElement.setAttribute("stroke", responseArray[5]);
		svgElement.setAttribute("fill", responseArray[6]);
		
		return svgElement;
	},
	_buildSvgRect: function(ObjectParent, ObjectPath){
		var response = HMI.KSClient.getVar(null, '{'+ObjectPath+'.visible .x .y .width .height .stroke .fill}', null);
		if (response.indexOf("KS_ERR") !== -1){
			HMI.hmi_log_error("cshmi._buildSvgRect of "+ObjectPath+" failed: "+response);
			return null;
		}
		var responseArray = HMI.KSClient.splitKsResponse(response);
		
		if (responseArray[0] == "FALSE"){
			return null;
		}
		var svgElement = HMI.svgDocument.createElementNS(HMI.HMI_Constants.NAMESPACE_SVG, 'rect');
		svgElement.id = ObjectPath;
		
		//set dimension of container
		svgElement.setAttribute("x", responseArray[1]);
		svgElement.setAttribute("y", responseArray[2]);
		svgElement.setAttribute("width", responseArray[3]);
		svgElement.setAttribute("height", responseArray[4]);
		svgElement.setAttribute("stroke", responseArray[5]);
		svgElement.setAttribute("fill", responseArray[6]);
		
		return svgElement;
	},
/*
	_buildSvgTest: function(ObjectParent, ObjectPath){
		var response = HMI.KSClient.getVar(null, '{'+ObjectPath+'.visible .x .y .width .height}', null);
		if (response.indexOf("KS_ERR") !== -1){
			HMI.hmi_log_error("cshmi._buildSvgTest of "+ObjectPath+" failed: "+response);
			return null;
		}
		var responseArray = HMI.KSClient.splitKsResponse(response);
		
		if (responseArray[0] != "FALSE"){
			var response = HMI.KSClient.getVar(null, ObjectPath+'.SVGcontent', null);
			if (response.indexOf("KS_ERR") !== -1){
				HMI.hmi_log_error("cshmi._buildSvgTest of "+ObjectPath+" failed: "+response);
				return null;
			}
			response =	"<svg:svg xmlns:svg=\"http://www.w3.org/2000/svg\">"
							+response.substring(2, response.length-2)
							+"</svg:svg>";
			var svgElement = HMI.HMIDOMParser.parse(response, null);
			svgElement.id = ObjectPath;
			svgElement.setAttribute("x", responseArray[1]);
			svgElement.setAttribute("y", responseArray[2]);
			svgElement.setAttribute("width", responseArray[3]);
			svgElement.setAttribute("height", responseArray[4]);
			
			return svgElement;
		}
		return true;
	},
	_buildSvgCheckGesture: function(ObjectParent, ObjectPath){
		var response = HMI.KSClient.getVar(null, ObjectPath+'.GestureType', null);
		if (response.indexOf("KS_ERR") !== -1){
			HMI.hmi_log_error("cshmi._buildSvgTest of "+ObjectPath+" failed: "+response);
			return null;
		}
		
		this._addClass(ObjectParent, response.substring(1, response.length-1));
	},
*/
	_addClass: function(svgElement, additionalClass){
		if (svgElement.classList && svgElement.classList.add){
			svgElement.classList.add(additionalClass);
		}else if (svgElement.className !== undefined){
			svgElement.className.baseVal = (svgElement.className.baseVal+" "+additionalClass).trim();
		}else{
			svgElement.setAttribute('class', (svgElement.getAttribute('class')+ " "+additionalClass).trim());
		}
	}
};
var filedate = "$Date: 2011-03-11 15:20:21 $";
filedate = filedate.substring(7, filedate.length-2);
if ("undefined" == typeof HMIdate){
	HMIdate = filedate;
}else if (HMIdate < filedate){
	HMIdate = filedate;
}
