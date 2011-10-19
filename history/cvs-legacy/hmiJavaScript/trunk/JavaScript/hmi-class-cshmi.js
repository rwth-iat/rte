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
*	$Revision$
*	$Date$
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
	this.ResourceList = Object();
	this.ResourceList.Actions = Object();
	this.ResourceList.Conditions = Object();
	this.ResourceList.Events = Object();
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
		var Component = null;
		var Result = true;
		
		if (ObjectType == "/Libraries/cshmi/Group"){
			Component = this._buildSvgContainer(ObjectParent, ObjectPath);
		}else if (ObjectType == "/Libraries/cshmi/Line"){
			Component = this._buildSvgLine(ObjectParent, ObjectPath);
		}else if (ObjectType == "/Libraries/cshmi/Polyline"){
			Component = this._buildSvgPolyline(ObjectParent, ObjectPath);
		}else if (ObjectType == "/Libraries/cshmi/Text"){
			Component = this._buildSvgText(ObjectParent, ObjectPath);
		}else if (ObjectType == "/Libraries/cshmi/Circle"){
			Component = this._buildSvgCircle(ObjectParent, ObjectPath);
		}else if (ObjectType == "/Libraries/cshmi/Rectangle"){
			Component = this._buildSvgRect(ObjectParent, ObjectPath);
		}else if (ObjectType == "/Libraries/cshmi/ClientEvent"){
			Result = this._interpreteClientEvent(ObjectParent, ObjectPath);
		}else if (ObjectType == "/Libraries/cshmi/TimeEvent"){
			Result = this._interpreteTimeEvent(ObjectParent, ObjectPath);
		}else if (ObjectType == "/Libraries/cshmi/OperatorEvent"){
			Result = this._interpreteOperatorEvent(ObjectParent, ObjectPath);
		}else{
			//todo
			HMI.hmi_log_info("Objekt(Typ: "+ObjectType+"): "+ObjectPath+" nicht unterstützt");
		}
		
		//get Children if needed
		if (Component !== null){
			var responseArray = HMI.KSClient.getChildObjArray(ObjectPath);
			for (var i=0; i < responseArray.length; i++) {
				var varName = responseArray[i].split(" ");
				var ChildComponent = this.BuildDomain(Component, Component.id+"/"+varName[0], varName[1]);
				if (ChildComponent !== null){
					Component.appendChild(ChildComponent);
				}
			}
		}
		
		return Component;
	},
	/*********************************
		_interpreteClientEvent
		-	calling Actions if supported ClientEvent is triggered
	*********************************/
	_interpreteClientEvent: function(ObjectParent, ObjectPath){
		var command = ObjectPath.split("/");
		if (command[command.length-1] === "onload"){
			//interprete Action now (todo perhaps as a callback after load)
			this._interpreteAction(ObjectParent, ObjectPath);
		}else{
			//todo
			HMI.hmi_log_info("ClientEvent ("+command[command.length-1]+") "+ObjectPath+" nicht unterstützt");
		}
	},
	/*********************************
		_interpreteOperatorEvent
		-	detect all OperatorEvents and register them
	*********************************/
	_interpreteOperatorEvent: function(ObjectParent, ObjectPath){
		var command = ObjectPath.split("/");
		if (command[command.length-1] === "click"){
			ObjectParent.setAttribute("cursor", "pointer");
			var preserveThis = this;	//grabbed from http://jsbin.com/etise/7/edit
			ObjectParent.addEventListener(command[command.length-1], function(){
				preserveThis._interpreteAction(ObjectParent, ObjectPath);
			}, false);
		}else if (command[command.length-1] === "doubleclick"){
			ObjectParent.setAttribute("cursor", "pointer");
			var preserveThis = this;	//grabbed from http://jsbin.com/etise/7/edit
			//fixme make double click ASV compatible
			ObjectParent.addEventListener("dblclick", function(){
				preserveThis._interpreteAction(ObjectParent, ObjectPath);
			}, false);
		}else if (command[command.length-1] === "rightclick"){
			ObjectParent.setAttribute("cursor", "pointer");
			var preserveThis = this;	//grabbed from http://jsbin.com/etise/7/edit
			ObjectParent.addEventListener("contextmenu", function(){
				preserveThis._interpreteAction(ObjectParent, ObjectPath);
			}, false);
		}else{
			//todo
			HMI.hmi_log_info("OperatorEvent ("+command[command.length-1]+") "+ObjectPath+" nicht unterstützt");
		}
	},
	/*********************************
		_interpreteTimeEvent
		-	calling Actions if supported TimeEvent is triggered
	*********************************/
	_interpreteTimeEvent: function(ObjectParent, ObjectPath){
		//interprete Action now to initialise
		this._interpreteAction(ObjectParent, ObjectPath);
		
		var response = HMI.KSClient.getVar(null, '{'+ObjectPath+'.cyctime}', null);
		if (response === false){
			//communication error
			return false;
		}else if (response.indexOf("KS_ERR") !== -1){
			HMI.hmi_log_error("cshmi._interpreteTimeEvent of "+ObjectPath+" failed: "+response);
			
			return false;
		}
		var responseArray = HMI.KSClient.splitKsResponse(response);
		
		var preserveThis = this;	//grabbed from http://jsbin.com/etise/7/edit
		if (responseArray.length !== 0){
			//HMI.hmi_log_trace("cshmi._interpreteTimeEvent calling myself again in "+responseArray[0]+" seconds");
			window.setTimeout(function(){
				preserveThis._interpreteTimeEvent(ObjectParent, ObjectPath);
			}, responseArray[0]*1000);
		}
		return true;
	},
	/*********************************
		_interpreteAction
		-	detect all Actions and triggers them
	*********************************/
	_interpreteAction: function(ObjectParent, ObjectPath){
		var returnValue = true;
		var responseArray = HMI.KSClient.getChildObjArray(ObjectPath);
		for (var i=0; i < responseArray.length && returnValue == true; i++) {
			var varName = responseArray[i].split(" ");
			if (varName[1] == "/Libraries/cshmi/SetValue"){
				returnValue = this._setValue(ObjectParent, ObjectPath+"/"+varName[0]);
			}else if (varName[1] == "/Libraries/cshmi/IfThenElse"){
				returnValue = this._interpreteIfThenElse(ObjectParent, ObjectPath+"/"+varName[0]);
			}else{
				//todo
				HMI.hmi_log_info("Action ("+varName[1]+")"+ObjectPath+" nicht unterstützt");
			}
		}
		return returnValue;
	},
	/*********************************
		_getValue
		-	get a Value from multiple Sources
	*********************************/
	_getValue: function(ObjectParent, ObjectPath){
		if(!(ObjectParent[ObjectPath] && ObjectParent[ObjectPath].action)){
			ObjectParent[ObjectPath] = new CsGetValue();
		}
		
		var response = HMI.KSClient.getVar(null, '{'+ObjectPath+'.ksvar .elemvar .globalvar .value}', null);
		if (response === false){
			//communication error
			return null;
		}else if (response.indexOf("KS_ERR") !== -1){
			HMI.hmi_log_error("cshmi._getValue of "+ObjectPath+" failed: "+response);
			
			return null;
		}
		var responseArray = HMI.KSClient.splitKsResponse(response);
		
		for (var i=0; i < responseArray.length; i++) {
			if (responseArray[i] !== ""){
				if (i === 0){
					//ksvar
					
					if (responseArray[i].charAt(0) == "/" && responseArray[i].charAt(1) == "/"){
						//we have an absolute path on another server
						var RequestServer = Object();
						RequestServer.servername = responseArray[i].split("/")[2]+"/"+responseArray[i].split("/")[3];
						RequestServer.serverhandle = HMI.KSClient.getHandleID(RequestServer.servername);
						RequestServer.path = responseArray[0].substring(RequestServer.servername.length+2);
						response = HMI.KSClient.getVar(RequestServer.serverhandle, '{'+RequestServer.path+'}', null);
					}else if (responseArray[i].charAt(0) == "/"){
						//we have an absolute path on this server
						response = HMI.KSClient.getVar(null, '{'+responseArray[i]+'}', null);
					}else{
						//get baseKsPath
						var baseKsPath = this._getBaseKsPath(ObjectParent, ObjectPath);
						response = HMI.KSClient.getVar(baseKsPath.serverhandle, '{'+baseKsPath.path+"/"+responseArray[i]+'}', null);
					}
					responseArray = HMI.KSClient.splitKsResponse(response);
					if (responseArray.length === 0){
						return null;
					}else{
						return responseArray[0];
					}
				}else if (i === 1){
					//elemvar
					if (ObjectParent.hasAttribute(responseArray[i])){
						ObjectParent[ObjectPath].elemvar = ObjectParent.getAttribute(responseArray[i]);
						return ObjectParent[ObjectPath].elemvar;
					}else if (responseArray[i] == "content"){
						//content is special, as it is different in OVM and SVG
						if (typeof ObjectParent.textContent != "undefined"){
							ObjectParent[ObjectPath].elemvar = ObjectParent.textContent;
							return ObjectParent[ObjectPath].elemvar;
						}else if (typeof ObjectParent.innerText != "undefined"){
							ObjectParent[ObjectPath].elemvar = ObjectParent.innerText;
							return ObjectParent[ObjectPath].elemvar;
						}else{
							//fixme asv compatibility
							ObjectParent[ObjectPath].elemvar = null;
							return ObjectParent[ObjectPath].elemvar;
						}
					}else{
						//unknown elment variable
						return null;
					}
				}else if (i === 2){
					//globalvar
					debugger;
					
					//todo
					return null;
				}else if (i === 3){
					//value
					ObjectParent[ObjectPath].value = responseArray[i]
					return ObjectParent[ObjectPath].value;
				}else{
					//unknown
					debugger;
					return null;
				}
			}
		}
	},
	/*********************************
		_setValue
		-	sets a Value to multiple Sources
	*********************************/
	_setValue: function(ObjectParent, ObjectPath){
		//get Value to set (via getValue-part of setValue Object)
		var NewValue = this._getValue(ObjectParent, ObjectPath+".value");
		
		//get info where to set the NewValue
		var response = HMI.KSClient.getVar(null, '{'+ObjectPath+'.ksvar .elemvar .globalvar}', null);
		if (response === false){
			//communication error
			return false;
		}else if (response.indexOf("KS_ERR") !== -1){
			HMI.hmi_log_error("cshmi._setValue of "+ObjectPath+" failed: "+response);
			
			return false;
		}
		var responseArray = HMI.KSClient.splitKsResponse(response);
		
		//set the new Value
		for (var i=0; i < responseArray.length; i++) {
			if (responseArray[i] !== ""){
				if (i === 0){
					//ksvar
					
					if (responseArray[i].charAt(0) == "/" && responseArray[i].charAt(1) == "/"){
						//we have an absolute path on another server
						var RequestServer = Object();
						RequestServer.servername = responseArray[i].split("/")[2]+"/"+responseArray[i].split("/")[3];
						RequestServer.serverhandle = HMI.KSClient.getHandleID(RequestServer.servername);
						RequestServer.path = responseArray[0].substring(RequestServer.servername.length+2);
						HMI.KSClient.setVar(RequestServer.serverhandle, '{'+RequestServer.path+'}', NewValue, null);
					}else if (responseArray[i].charAt(0) == "/"){
						//we have an absolute path on this server
						HMI.KSClient.setVar(null, '{'+responseArray[i]+'}', NewValue, null);
					}else{
						//get baseKsPath
						var baseKsPath = this._getBaseKsPath(ObjectParent, ObjectPath);
						HMI.KSClient.setVar(baseKsPath.serverhandle, '{'+baseKsPath.path+responseArray[i]+'}', NewValue, null);
					}
					return true;
				}else if (i === 1){
					//elemvar
					if (ObjectParent.hasAttribute(responseArray[i])){
						ObjectParent.setAttribute(responseArray[i], NewValue);
					}else if (responseArray[i] == "content"){
						//content is special, as it is different in OVM and SVG
						ObjectParent.replaceChild(HMI.svgDocument.createTextNode(NewValue), ObjectParent.firstChild);
					}
					return true;
				}else if (i === 2){
					//globalvar
					debugger;
					
					//todo
					return false;
				}
			}
		}
	},
	/*********************************
		_getBaseKsPath
		-	search baseKsPath by iterating objects
	*********************************/
	_getBaseKsPath: function(ObjectParent, ObjectPath){
		var ObjectPathArray = ObjectPath.split("/");
		
		var returnValue = Object();
		returnValue.servername = null;
		returnValue.serverhandle = null;
		returnValue.path = "";
		do{
			var response = HMI.KSClient.getVar(null, '{'+ObjectPathArray.join("/")+'.baseKsPath}', null);
			if (response === false){
				//communication error
				return returnValue;
			}else if (response.indexOf("KS_ERR_BADPATH") !== -1){
				//an object in tree is no cshmi object => try parent
			}else if (response !== "{{}}" && response.length > 2){
				var responseArray = HMI.KSClient.splitKsResponse(response);
				if (responseArray[0].charAt(0) === "/" && responseArray[0].charAt(1) === "/"){
					//String begins with // so it is a fullpath with Host and servername
					returnValue.servername = responseArray[0].split("/")[2]+"/"+responseArray[0].split("/")[3];
					returnValue.serverhandle = HMI.KSClient.getHandleID(returnValue.servername);
					returnValue.path = responseArray[0].substring(returnValue.servername.length+2)+returnValue.path;
				}else if (responseArray[0].charAt(0) === "/"){
					//String begins with / so it is a fullpath in this server
					returnValue.path = responseArray[0]+returnValue.path;
					//full path => stop searching for other path
					break;
				}else{
					//a normal relativ path
					returnValue.path = responseArray[0]+returnValue.path;
				}
			}else{
				// no action in this loop
			}
			ObjectPathArray.pop();
		}while(ObjectPathArray.length > 1);
		return returnValue;
	},
	/*********************************
		_interpreteIfThenElse
		-	sets a Value to multiple Sources
	*********************************/
	_interpreteIfThenElse: function(ObjectParent, ObjectPath){
		var response = HMI.KSClient.getVar(null, '{'+ObjectPath+'.anycond}', null);
		if (response === false){
			//communication error
			return false;
		}else if (response.indexOf("KS_ERR") !== -1){
			HMI.hmi_log_error("cshmi._interpreteIfThenElse of "+ObjectPath+" failed: "+response);
			
			return false;
		}
		var responseArray = HMI.KSClient.splitKsResponse(response);
		
		var ConditionMatched = false;
		var andCond = responseArray[0];
		var responseArray = HMI.KSClient.getChildObjArray(ObjectPath+".if");
		var i=0
		if (andCond == "TRUE"){
			//logical OR
			while(i < responseArray.length && ConditionMatched !== false){
				var varName = responseArray[i].split(" ");
				if (varName[1] == "/Libraries/cshmi/Compare"){
					ConditionMatched = this._checkCondition(ObjectParent, ObjectPath+".if/"+varName[0], ObjectPath);
				}
				i++;
			}
		}else{
			//logical AND
			while(i < responseArray.length && ConditionMatched !== true){
				var varName = responseArray[i].split(" ");
				if (varName[1] == "/Libraries/cshmi/Compare"){
					ConditionMatched = this._checkCondition(ObjectParent, ObjectPath+".if/"+varName[0], ObjectPath);
				}
				i++;
			}
		}
		if (ConditionMatched == true){
			this._interpreteAction(ObjectParent, ObjectPath+".then");
		}else{
			this._interpreteAction(ObjectParent, ObjectPath+".else");
		}
		
		return true;
	},
	/*********************************
		_checkCondition
		-	checks Condition
	*********************************/
	_checkCondition: function(ObjectParent, ObjectPath, ConditionPath){
		//get Values
		var comptype = HMI.KSClient.getVar(null, '{'+ObjectPath+'.comptype}', null);
		var Value1 = this._getValue(ObjectParent, ObjectPath+".value1");
		var Value2 = this._getValue(ObjectParent, ObjectPath+".value2");
		
		if (comptype === "{<}"){
			return (Value1 < Value2);
		}else if (comptype === "{<=}"){
			return (Value1 <= Value2);
		}else if (comptype === "{=}"){
			return (Value1 === Value2);
		}else if (comptype === "{!=}"){
			return (Value1 !== Value2);
		}else if (comptype === "{>=}"){
			return (Value1 >= Value2);
		}else if (comptype === "{>}"){
			return (Value1 > Value2);
		}else{
			HMI.hmi_log_error("cshmi._checkCondition Comparingtype "+comptype+" unknown");
			return false;
		}
	},
	
	
	
	
	
	
	/*********************************
		_buildSvg*
		-	builds SVG Object
		-	gets parameter via KS
		-	returns DOM Object or null to caller
	*********************************/
	_buildSvgContainer: function(ObjectParent, ObjectPath){
		var response = HMI.KSClient.getVar(null, '{'+ObjectPath+'.x .y .width .height}', null);
		if (response === false){
			//communication error
			return null;
		}else if (response.indexOf("KS_ERR") !== -1){
			HMI.hmi_log_error("cshmi._buildSvgContainer of "+ObjectPath+" failed: "+response);
			HMI.hmi_log_onwebsite("Visualising the sheet failed.");
			
			return null;
		}
		var responseArray = HMI.KSClient.splitKsResponse(response);
		
		var svgElement = HMI.svgDocument.createElementNS(HMI.HMI_Constants.NAMESPACE_SVG, 'svg');
		svgElement.id = ObjectPath;
		
		this._addClass(svgElement, "cshmi-group");
		//svgElement.setAttribute("xmlns:svg", HMI.HMI_Constants.NAMESPACE_SVG);
		
		//set dimension of container
		svgElement.setAttribute("x", responseArray[0]);
		svgElement.setAttribute("y", responseArray[1]);
		svgElement.setAttribute("width", responseArray[2]);
		svgElement.setAttribute("height", responseArray[3]);
		
		return svgElement;
	},
	_buildSvgLine: function(ObjectParent, ObjectPath){
		var response = HMI.KSClient.getVar(null, '{'+ObjectPath+'.visible .x1 .y1 .x2 .y2 .stroke .opacity}', null);
		if (response === false){
			//communication error
			return null;
		}else if (response.indexOf("KS_ERR") !== -1){
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
		svgElement.setAttribute("opacity", responseArray[6]);
		
		return svgElement;
	},
	_buildSvgPolyline: function(ObjectParent, ObjectPath){
		var response = HMI.KSClient.getVar(null, '{'+ObjectPath+'.visible .points .stroke .fill .opacity}', null);
		if (response === false){
			//communication error
			return null;
		}else if (response.indexOf("KS_ERR") !== -1){
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
		svgElement.setAttribute("opacity", responseArray[4]);
		
		return svgElement;
	},
	_buildSvgPolygon: function(ObjectParent, ObjectPath){
		var response = HMI.KSClient.getVar(null, '{'+ObjectPath+'.visible .points .stroke .fill .opacity}', null);
		if (response === false){
			//communication error
			return null;
		}else if (response.indexOf("KS_ERR") !== -1){
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
		svgElement.setAttribute("opacity", responseArray[4]);
		
		return svgElement;
	},
	_buildSvgPath: function(ObjectParent, ObjectPath){
		var response = HMI.KSClient.getVar(null, '{'+ObjectPath+'.visible .d .stroke .fill .opacity}', null);
		if (response === false){
			//communication error
			return null;
		}else if (response.indexOf("KS_ERR") !== -1){
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
		svgElement.setAttribute("opacity", responseArray[4]);
		
		return svgElement;
	},
	_buildSvgText: function(ObjectParent, ObjectPath){
		var response = HMI.KSClient.getVar(null, '{'+ObjectPath+'.visible .x .y .fontSize .fontStyle .fontWeight .fontFamily .horAlignment .verAlignment .stroke .fill .content .opacity}', null);
		if (response === false){
			//communication error
			return null;
		}else if (response.indexOf("KS_ERR") !== -1){
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
		svgElement.setAttribute("opacity", responseArray[12]);
		
		return svgElement;
	},
	_buildSvgCircle: function(ObjectParent, ObjectPath){
		var response = HMI.KSClient.getVar(null, '{'+ObjectPath+'.visible .cx .cy .r .stroke .fill .opacity}', null);
		if (response === false){
			//communication error
			return null;
		}else if (response.indexOf("KS_ERR") !== -1){
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
		svgElement.setAttribute("opacity", responseArray[6]);
		
		return svgElement;
	},
	_buildSvgEllipse: function(ObjectParent, ObjectPath){
		var response = HMI.KSClient.getVar(null, '{'+ObjectPath+'.visible .cx .cy .rx .ry .stroke .fill .opacity}', null);
		if (response === false){
			//communication error
			return null;
		}else if (response.indexOf("KS_ERR") !== -1){
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
		svgElement.setAttribute("opacity", responseArray[7]);
		
		return svgElement;
	},
	_buildSvgRect: function(ObjectParent, ObjectPath){
		var response = HMI.KSClient.getVar(null, '{'+ObjectPath+'.visible .x .y .width .height .stroke .fill .opacity}', null);
		if (response === false){
			//communication error
			return null;
		}else if (response.indexOf("KS_ERR") !== -1){
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
		svgElement.setAttribute("opacity", responseArray[7]);
		
		return svgElement;
	},
/*
	_buildSvgTest: function(ObjectParent, ObjectPath){
		var response = HMI.KSClient.getVar(null, '{'+ObjectPath+'.visible .x .y .width .height}', null);
		if (response === false){
			//communication error
			return null;
		}else if (response.indexOf("KS_ERR") !== -1){
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


function CsGetValue(){
	this.ObjectType = "CsGetValue";
	this.ksvar = null;
	this.elemvar = null;
	this.globalvar = null;
	this.eventvar = null;
	this.value = null;
}


var filedate = "$Date$";
filedate = filedate.substring(7, filedate.length-2);
if ("undefined" == typeof HMIdate){
	HMIdate = filedate;
}else if (HMIdate < filedate){
	HMIdate = filedate;
}
