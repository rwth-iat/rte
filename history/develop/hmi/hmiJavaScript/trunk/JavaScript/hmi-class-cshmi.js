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
	this.ResourceList.baseKsPath = Object();
	
	this.cshmiComponentClass = "cshmi-component";
	this.cshmiGroupClass = "cshmi-group";
	this.cshmiTemplateClass = "cshmi-template";
	this.cshmiTemplateHideableClass = "cshmi-hideabletemplate";
};





//#########################################################################################################################
//fixme: check return value of gethandleid
//#########################################################################################################################

/***********************************************************************
	prototype
***********************************************************************/
cshmi.prototype = {
	/*********************************
		instanciateCshmi
	*********************************/
	instanciateCshmi: function (ObjectPath) {
		
		var Component = this.BuildDomain(null, ObjectPath, "/cshmi/Group");
		
		if(HMI.PlaygroundContainerNode){
			//the displayed size is calculated from the Container-Node in the html, so we correct the dimension of it
			HMI.PlaygroundContainerNode.setAttribute('height', Component.getAttribute('height'));
			HMI.PlaygroundContainerNode.setAttribute('width', Component.getAttribute('width'));
		}
		
		if (Component !== null){
			HMI.Playground.appendChild(Component);
			HMI._setLayerPosition(Component);
			var ComponentChilds = Component.getElementsByTagName('*');
			for(var i = 0;i < ComponentChilds.length;i++){
				HMI._setLayerPosition(ComponentChilds[i]);
			}
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
		if (ObjectType.indexOf("/cshmi/Group") !== -1){
			Component = this._buildSvgContainer(ObjectParent, ObjectPath);
		}else if (ObjectType.indexOf("/cshmi/Template") !== -1){
			Component = this._buildFromTemplate(ObjectParent, ObjectPath);
		}else if (ObjectType.indexOf("/cshmi/Path") !== -1){
			Component = this._buildSvgPath(ObjectParent, ObjectPath);
		}else if (ObjectType.indexOf("/cshmi/Line") !== -1){
			Component = this._buildSvgLine(ObjectParent, ObjectPath);
		}else if (ObjectType.indexOf("/cshmi/Polyline") !== -1){
			Component = this._buildSvgPolyline(ObjectParent, ObjectPath);
		}else if (ObjectType.indexOf("/cshmi/Polygon") !== -1){
			Component = this._buildSvgPolygon(ObjectParent, ObjectPath);
		}else if (ObjectType.indexOf("/cshmi/Text") !== -1){
			Component = this._buildSvgText(ObjectParent, ObjectPath);
		}else if (ObjectType.indexOf("/cshmi/Circle") !== -1){
			Component = this._buildSvgCircle(ObjectParent, ObjectPath);
		}else if (ObjectType.indexOf("/cshmi/Ellipse") !== -1){
			Component = this._buildSvgEllipse(ObjectParent, ObjectPath);
		}else if (ObjectType.indexOf("/cshmi/Rectangle") !== -1){
			Component = this._buildSvgRect(ObjectParent, ObjectPath);
		}else if (ObjectType.indexOf("/cshmi/ClientEvent") !== -1){
			Result = this._interpreteClientEvent(ObjectParent, ObjectPath);
		}else if (ObjectType.indexOf("/cshmi/TimeEvent") !== -1){
			Result = this._interpreteTimeEvent(ObjectParent, ObjectPath);
		}else if (ObjectType.indexOf("/cshmi/OperatorEvent") !== -1){
			Result = this._interpreteOperatorEvent(ObjectParent, ObjectPath);
		}else{
			if (	ObjectType.indexOf("/cshmi/SetValue") !== -1 ||
					ObjectType.indexOf("/cshmi/IfThenElse") !== -1){
				HMI.hmi_log_info("Actions not supported at this position: (Typ: "+ObjectType+"): "+ObjectPath+" nicht unterstützt");
			}else{
				HMI.hmi_log_info("Objekt (Typ: "+ObjectType+"): "+ObjectPath+" nicht unterstützt");
			}
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
			//interprete Action "now", but we want to have the full DOM tree ready
			var preserveThis = this;	//grabbed from http://jsbin.com/etise/7/edit
			window.setTimeout(function(){
				preserveThis._interpreteAction(ObjectParent, ObjectPath);
			}, 10);
		}else{
			HMI.hmi_log_info_onwebsite("ClientEvent ("+command[command.length-1]+") "+ObjectPath+" nicht unterstützt");
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
			ObjectParent.addEventListener(command[command.length-1], function(evt){
				//mark changed Component
				HMI.displaygestureReactionMarker(ObjectParent);
				
				preserveThis._interpreteAction(ObjectParent, ObjectPath);
			}, false);
		}else if (command[command.length-1] === "doubleclick"){
			ObjectParent.setAttribute("cursor", "pointer");
			var preserveThis = this;	//grabbed from http://jsbin.com/etise/7/edit
			//fixme make double click ASV compatible
			/*ObjectParent.addEventListener("click", function(evt){
				if (!(evt.button === 0 && evt.detail ==2)){
				return;
			}*/
			ObjectParent.addEventListener("dblclick", function(evt){
				//mark changed Component
				HMI.displaygestureReactionMarker(ObjectParent);
				
				preserveThis._interpreteAction(ObjectParent, ObjectPath);
			}, false);
		}else if (command[command.length-1] === "rightclick"){
			ObjectParent.setAttribute("cursor", "pointer");
			var preserveThis = this;	//grabbed from http://jsbin.com/etise/7/edit
			ObjectParent.addEventListener("contextmenu", function(evt){
				//mark changed Component
				HMI.displaygestureReactionMarker(ObjectParent);
				
				preserveThis._interpreteAction(ObjectParent, ObjectPath);
			}, false);
		}else{
			HMI.hmi_log_info_onwebsite("OperatorEvent ("+command[command.length-1]+") "+ObjectPath+" nicht unterstützt");
		}
	},
	/*********************************
		_interpreteTimeEvent
		-	calling Actions if supported TimeEvent is triggered
	*********************************/
	_interpreteTimeEvent: function(ObjectParent, ObjectPath){
		//fixme prevent update if not visible
		
		//todo könnte sein, dass interpreteAction und der spätere settimeout ungünstig zusammen fallen
		
		//interprete Action "now", but we want to have the full DOM tree ready
		var preserveThis = this;	//grabbed from http://jsbin.com/etise/7/edit
		window.setTimeout(function(){
			preserveThis._interpreteAction(ObjectParent, ObjectPath);
		}, 10);
		
		var response = HMI.KSClient.getVar(null, '{'+ObjectPath+'.cyctime}', null);
		if (response === false){
			//communication error
			return false;
		}else if (response.indexOf("KS_ERR") !== -1){
			HMI.hmi_log_error("cshmi._interpreteTimeEvent of "+ObjectPath+" failed: "+response);
			
			return false;
		}
		var responseArray = HMI.KSClient.splitKsResponse(response);
		
		if (responseArray.length !== 0){
			var preserveThis = this;	//grabbed from http://jsbin.com/etise/7/edit
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
			if (varName[1].indexOf("/cshmi/SetValue") !== -1){
				returnValue = this._setValue(ObjectParent, ObjectPath+"/"+varName[0]);
			}else if (varName[1].indexOf("/cshmi/IfThenElse") !== -1){
				returnValue = this._interpreteIfThenElse(ObjectParent, ObjectPath+"/"+varName[0]);
			}else{
				HMI.hmi_log_info_onwebsite("Action ("+varName[1]+")"+ObjectPath+" nicht unterstützt");
			}
		}
		return returnValue;
	},
	/*********************************
		_getValue
		-	get a Value from multiple Sources
	*********************************/
	_getValue: function(ObjectParent, ObjectPath){
		var responseArray;
		if (!(this.ResourceList.Actions && this.ResourceList.Actions[ObjectPath] !== undefined)){
			
			var response = HMI.KSClient.getVar(null, '{'+ObjectPath+'.ksVar .elemVar .elemVarPath .globalVar .TemplateFBReferenceVariable .TemplateConfigValues .value}', null);
			if (response === false){
				//communication error
				return null;
			}else if (response.indexOf("KS_ERR") !== -1){
				HMI.hmi_log_error("cshmi._getValue of "+ObjectPath+" failed: "+response);
				
				return null;
			}
			responseArray = HMI.KSClient.splitKsResponse(response);
			
			//prepare main js-object to remember config of getValue OV-Object
			this.ResourceList.Actions[ObjectPath] = new Object();
			this.ResourceList.Actions[ObjectPath].getVarParameters = responseArray;
			HMI.hmi_log_trace("cshmi._getValue: remembering config of "+ObjectPath+" ");
		}else{
			HMI.hmi_log_trace("cshmi._getValue: using remembered config of "+ObjectPath+" ");
			responseArray = this.ResourceList.Actions[ObjectPath].getVarParameters;
		}
		
		for (var i=0; i < responseArray.length; i++) {
			if (responseArray[i] !== ""){
				if (i === 0){
					//ksVar
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
					//todo interprete elemVarPath
					
					//elemVar
					if (responseArray[i] == "content"){
						//content is special, as it is different in OVM and SVG
						if (typeof ObjectParent.textContent != "undefined"){
							return ObjectParent.textContent;
						}else if (typeof ObjectParent.innerText != "undefined"){
							return ObjectParent.innerText;
						}else{
							//fixme asv compatibility
							return null;
						}
					}else if (ObjectParent.hasAttribute(responseArray[i])){
						return ObjectParent.getAttribute(responseArray[i]);
					}else{
						//unknown element variable
						return null;
					}
				}else if (i === 3){
					//globalVar
					HMI.hmi_log_info_onwebsite('GetValue globalVar not implemented. Object: '+ObjectPath);
					
					//todo
					return null;
				}else if (i === 4){
					//TemplateFBReferenceVariable
					var TemplateObject = ObjectParent;
					do{
						if(TemplateObject.FBReference && TemplateObject.FBReference[responseArray[i]] !== undefined){
							//this is a TemplateObject itself
							return TemplateObject.FBReference[responseArray[i]];
						}else if(TemplateObject.FBReference && TemplateObject.FBReference["default"] !== undefined){
							//this is a TemplateObject itself, but only one reference given
							
							if(responseArray[i] == "identifier"){
								//if the identifier is requested calculate this to avoid network request
								var Objectname = TemplateObject.FBReference["default"].split("/");
								return Objectname[Objectname.length - 1];
							}
							
							if (TemplateObject.FBReference["default"].charAt(0) === "/" && TemplateObject.FBReference["default"].charAt(1) === "/"){
								//String begins with // so it is a fullpath with Host and servername
								var servername = TemplateObject.FBReference["default"].split("/")[2]+"/"+TemplateObject.FBReference["default"].split("/")[3];
								var serverhandle = HMI.KSClient.getHandleID(servername);
								var serverpath = TemplateObject.FBReference["default"].substring(servername.length+2);
								var result = HMI.KSClient.getVar(serverhandle, '{'+serverpath+'.'+responseArray[i]+'}', null);
								var returnValue = HMI.KSClient.splitKsResponse(result);
								if (returnValue.length > 0){
									return returnValue[0];
								}
								return null;
							}else if (TemplateObject.FBReference["default"].charAt(0) === "/"){
								//String begins with / so it is a fullpath in this server
								var baseKsPath = this._getBaseKsPath(ObjectParent, ObjectPath);
								var result = HMI.KSClient.getVar(baseKsPath.serverhandle, '{'+TemplateObject.FBReference["default"]+'.'+responseArray[i]+'}', null);
								var returnValue = HMI.KSClient.splitKsResponse(result);
								if (returnValue.length > 0){
									return returnValue[0];
								}
								return null;
							}else{
								//a normal relativ path
								HMI.hmi_log_info_onwebsite('GetValue '+ObjectPath+' wrong configured. No relative path allowed');
								return null;
							}
						}
					//loop upwards to find the Template object
					}while( (TemplateObject = TemplateObject.parentNode) && TemplateObject !== null);  //the = is no typo here!
					return null;
				}else if (i === 5){
					//TemplateConfigValues
					var TemplateObject = ObjectParent;
					do{
						if(TemplateObject.ConfigValues && TemplateObject.ConfigValues[responseArray[i]] !== undefined){
							//this is a TemplateObject itself and have valid data for us
							return TemplateObject.ConfigValues[responseArray[i]];
						}
					//loop upwards to find the Template object
					}while( (TemplateObject = TemplateObject.parentNode) && TemplateObject !== null);  //the = is no typo here!
					return null;
				}else if (i === 6){
					//value
					return responseArray[i];
				}
			}//end if empty
		}//end for loop
		HMI.hmi_log_info_onwebsite('GetValue '+ObjectPath+' not configured.');
		return null; //unconfigured
	},
	/*********************************
		_setValue
		-	sets a Value to multiple Sources
	*********************************/
	_setValue: function(ObjectParent, ObjectPath){
		//get Value to set (via getValue-part of setValue Object)
		var NewValue = this._getValue(ObjectParent, ObjectPath+".value");
		
		if (NewValue === null){
			HMI.hmi_log_info("cshmi._setValue on "+ObjectPath+" (baseobject: "+ObjectPath+") failed because of an NewValue of null.");
			return false;
		}else if (NewValue === undefined){
			//should not happen
			HMI.hmi_log_error("cshmi._setValue on "+ObjectPath+" (baseobject: "+ObjectPath+") failed because of an NewValue of undefined.");
			return false;
		}
		//get info where to set the NewValue
		var response = HMI.KSClient.getVar(null, '{'+ObjectPath+'.ksVar .elemVar .globalVar .TemplateFBReferenceVariable}', null);
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
					//ksVar
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
					//elemVar
					if (responseArray[i] == "content"){
						//content is special, as it is different in OVM and SVG
						ObjectParent.firstChild.replaceChild(HMI.svgDocument.createTextNode(NewValue), ObjectParent.firstChild.firstChild);
					}else{
						ObjectParent.setAttribute(responseArray[i], NewValue);
					}
					return true;
				}else if (i === 2){
					//globalVar
					HMI.hmi_log_info_onwebsite('SetValue globalVar not implemented. Object: '+ObjectPath);
					
					return false;
				}else if (i === 3){
					//TemplateFBReferenceVariable
					var TemplateObject = ObjectParent;
					do{
						if(TemplateObject.FBReference && TemplateObject.FBReference[responseArray[i]] !== undefined){
							//this is a TemplateObject itself
							return TemplateObject.FBReference[responseArray[i]];
						}else if(TemplateObject.FBReference && TemplateObject.FBReference["default"] !== undefined){
							//this is a TemplateObject itself, but only one reference given
							
							if (TemplateObject.FBReference["default"].charAt(0) === "/" && TemplateObject.FBReference["default"].charAt(1) === "/"){
								//String begins with // so it is a fullpath with Host and servername
								var servername = TemplateObject.FBReference["default"].split("/")[2]+"/"+TemplateObject.FBReference["default"].split("/")[3];
								var serverhandle = HMI.KSClient.getHandleID(servername);
								var serverpath = TemplateObject.FBReference["default"].substring(servername.length+2);
								response = HMI.KSClient.setVar(serverhandle, '{'+serverpath+'.'+responseArray[i]+'}', NewValue, null);
								if (response.indexOf("KS_ERR") !== -1){
									HMI.hmi_log_info_onwebsite('Setting '+TemplateObject.FBReference["default"]+'.'+responseArray[i]+' not successfull: '+response+' (configured here: '+ObjectPath+').');
								}
								return true;
							}else if (TemplateObject.FBReference["default"].charAt(0) === "/"){
								//String begins with / so it is a fullpath in this server
								var baseKsPath = this._getBaseKsPath(ObjectParent, ObjectPath);
								response = HMI.KSClient.setVar(baseKsPath.serverhandle, '{'+TemplateObject.FBReference["default"]+'.'+responseArray[i]+'}', NewValue, null);
								if (response.indexOf("KS_ERR") !== -1){
									HMI.hmi_log_info_onwebsite('Setting '+TemplateObject.FBReference["default"]+'.'+responseArray[i]+' not successfull: '+response+' (configured here: '+ObjectPath+').');
								}
								return true;
							}else{
								//a normal relativ path
								HMI.hmi_log_info_onwebsite('SetValue '+ObjectPath+' wrong configured. No relative path allowed');
								return false;
							}
						}
					//loop upwards to find the Template object
					}while( (TemplateObject = TemplateObject.parentNode) && TemplateObject !== null);  //the = is no typo here!
					return null;
				}
			}//end: if empty
		}//end: for loop
		HMI.hmi_log_info_onwebsite('SetValue '+ObjectPath+' not configured.');
		return false;
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
			var currentPath = ObjectPathArray.join("/");
			var response;
			if (this.ResourceList.baseKsPath[currentPath] === undefined){
				response = HMI.KSClient.getVar(null, '{'+currentPath+'.baseKsPath}', null);
				this.ResourceList.baseKsPath[currentPath] = response;
				HMI.hmi_log_trace("cshmi._BaseKsPath: remembering config of "+currentPath+" ");
			}else{
				response = this.ResourceList.baseKsPath[currentPath];
				HMI.hmi_log_trace("cshmi._BaseKsPath: reusing remembered config of "+currentPath+" ");
			}
			if (response === false){
				//communication error
				return returnValue;
			}else if (response.indexOf("KS_ERR_BADPATH") !== -1){
				//an object in tree is no cshmi object
				return returnValue;
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
		var i = 0;
		if (andCond == "TRUE"){
			//logical OR
			while(i < responseArray.length && ConditionMatched !== false){
				var varName = responseArray[i].split(" ");
				if (varName[1].indexOf("/cshmi/Compare") !== -1){
					ConditionMatched = this._checkCondition(ObjectParent, ObjectPath+".if/"+varName[0], ObjectPath);
				}
				i++;
			}
		}else{
			//logical AND
			while(i < responseArray.length && ConditionMatched !== true){
				var varName = responseArray[i].split(" ");
				if (varName[1].indexOf("/cshmi/Compare") !== -1){
					ConditionMatched = this._checkCondition(ObjectParent, ObjectPath+".if/"+varName[0], ObjectPath);
				}
				i++;
			}
		}
		if (ConditionMatched === true){
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
		
		if (Value1 === null){
			HMI.hmi_log_info("cshmi._checkCondition on "+ObjectPath+" (baseobject: "+ObjectPath+") failed because Value1 is null.");
			return false;
		}
		if (Value2 === null){
			HMI.hmi_log_info("cshmi._checkCondition on "+ObjectPath+" (baseobject: "+ObjectPath+") failed because Value2 is null.");
			return false;
		}
		
		if (comptype === "{<}"){
			return (Value1 < Value2);
		}else if (comptype === "{<=}"){
			return (Value1 <= Value2);
		}else if (comptype === "{==}"){
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
		
		this._addClass(svgElement, this.cshmiGroupClass);
		this._addClass(svgElement, this.cshmiComponentClass);
		
		//svgElement.setAttribute("xmlns:svg", HMI.HMI_Constants.NAMESPACE_SVG);
		
		//set dimension of container
		svgElement.setAttribute("x", responseArray[0]);
		svgElement.setAttribute("y", responseArray[1]);
		svgElement.setAttribute("width", responseArray[2]);
		svgElement.setAttribute("height", responseArray[3]);
		
		return svgElement;
	},
	/*********************************
		_buildFromTemplate
	*********************************/
	_buildFromTemplate: function(ObjectParent, ObjectPath){
		var response = HMI.KSClient.getVar(null, '{'+ObjectPath+'.TemplateDefinition .x .y .FBReference .ConfigValues .visible .opacity}', null);
		if (response === false){
			//communication error
			return null;
		}else if (response.indexOf("KS_ERR") !== -1){
			HMI.hmi_log_error("cshmi._buildFromTemplate of "+ObjectPath+" failed: "+response);
			
			return null;
		}
		
		//transform "...{{/TechUnits/Pump1}} {{pumpcolor:yellow pumpname:N18}}..." 
		//to [..., "/TechUnits/Pump1", "pumpcolor:yellow pumpname:N18", ...]
		var responseArray = HMI.KSClient.splitKsResponse(response);
		
		if (responseArray[0] === "" && responseArray.length > 3){
			HMI.hmi_log_info_onwebsite("Template "+ObjectPath+" is not configured");
			return null;
		}else if (responseArray[0] === ""){
			return null;
		}
		
		var TemplateLocation = "/TechUnits/cshmi/Templates/";
		
		var response = HMI.KSClient.getVar(null, '{'+TemplateLocation+responseArray[0]+'.width .height .hideable}', null);
		if (response === false){
			//communication error
			return null;
		}else if (response.indexOf("KS_ERR") !== -1){
			HMI.hmi_log_info_onwebsite("Template "+ObjectPath+" is wrong configured. TemplateDefinition '"+responseArray[0]+"' is not available.");
			HMI.hmi_log_error("cshmi._buildSvgContainer of "+ObjectPath+" failed: "+response);
			return null;
		}
		var responseArrayTemplate = HMI.KSClient.splitKsResponse(response);
		
		var svgElement = HMI.svgDocument.createElementNS(HMI.HMI_Constants.NAMESPACE_SVG, 'svg');
		svgElement.id = ObjectPath;
		svgElement.setAttribute("TemplateDescription", TemplateLocation+responseArray[0]);
		
		this._addClass(svgElement, this.cshmiTemplateClass);
		this._addClass(svgElement, this.cshmiComponentClass);
		if (responseArrayTemplate[2] === "TRUE"){
			this._addClass(svgElement, this.cshmiTemplateHideableClass);
		}
		
		//set dimension of container
		svgElement.setAttribute("x", responseArray[1]);
		svgElement.setAttribute("y", responseArray[2]);
		svgElement.setAttribute("width", responseArrayTemplate[0]);
		svgElement.setAttribute("height", responseArrayTemplate[1]);
		if (responseArray[5] == "TRUE"){
			svgElement.setAttribute("display", "block");
		}else{
			svgElement.setAttribute("display", "none");
		}
		svgElement.setAttribute("opacity", responseArray[6]);
		svgElement.style.overflow = "visible";
		
		//parametrise templateDefinition with the config
		svgElement.FBReference = Object();
		svgElement.ConfigValues = Object();
		var ConfigEntry = null;
		var ConfigList = responseArray[3].split(" ");
		for (var i=0; i < ConfigList.length; i++) {
			ConfigEntry = ConfigList[i].split(":");
			if (ConfigEntry.length === 2){
				svgElement.FBReference[ConfigEntry[0]] = ConfigEntry[1];
			}else if (ConfigEntry.length === 1 && ConfigEntry[0] != ""){
				svgElement.FBReference["default"] = ConfigEntry[0];
			}
		}
		
		//responseArray[4] is "pumpcolor:yellow pumpname:N18"
		//problemfall: "pumpcolor:yellow {pumpname:N 18}"
		//und noch schöner: "{{{pumpcolor:y ellow} {pumpname:N 18}}}" => ["{{pumpcolor:y ellow", "pumpname:N 18}}"]
		ConfigList = responseArray[4].split(" ");
		var lastEntry = null;
		for (var i=0; i < ConfigList.length; i++) {
			ConfigEntry = ConfigList[i].split(":");
			if (ConfigEntry.length === 2){
				svgElement.ConfigValues[ConfigEntry[0]] = ConfigEntry[1];
				lastEntry = ConfigEntry[0];
			}else if (ConfigEntry.length === 1 && lastEntry !== null){
				svgElement.ConfigValues[lastEntry] = svgElement.ConfigValues[lastEntry]+" "+ConfigEntry[0];
			}
		}
		
		//get childs (grafics and actions) from the TemplateDefinition
		//our child will be fetched later
		var responseArrayChild = HMI.KSClient.getChildObjArray(TemplateLocation+responseArray[0]);
		for (var i=0; i < responseArrayChild.length; i++) {
			var varName = responseArrayChild[i].split(" ");
			var ChildComponent = this.BuildDomain(svgElement, TemplateLocation+responseArray[0]+"/"+varName[0], varName[1]);
			if (ChildComponent !== null){
				svgElement.appendChild(ChildComponent);
				
				if (ChildComponent.tagName == "svg"){
					HMI._setLayerPosition(ChildComponent);
				}
			}
		}
		var preserveThis = this;	//grabbed from http://jsbin.com/etise/7/edit
		ObjectParent.setAttribute("cursor", "pointer");
		//fixme make double click ASV compatible
		/*ObjectParent.addEventListener("click", function(evt){
			if (!(evt.button === 0 && evt.detail ==2)){
			return;
		}*/
		ObjectParent.addEventListener("click", function(evt){
			var childTemplates = preserveThis._getElementsByClassName(ObjectParent, preserveThis.cshmiTemplateHideableClass);
			for (var i=0; i < childTemplates.length; i++) {
				if (childTemplates[i].getAttribute("display") == "block"){
					childTemplates[i].setAttribute("display", "none");
				}else{
					childTemplates[i].setAttribute("display", "block");
				}
			}
			if (evt.stopPropagation) evt.stopPropagation();
		}, false);
		
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
		var response = HMI.KSClient.getVar(null, '{'+ObjectPath+'.visible .x .y .fontSize .fontStyle .fontWeight .fontFamily .horAlignment .verAlignment .stroke .fill .opacity .content}', null);
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
		svgElement.setAttribute("stroke", responseArray[9]);
		svgElement.setAttribute("fill", responseArray[10]);
		svgElement.setAttribute("opacity", responseArray[11]);
		
		var svgTspan = HMI.svgDocument.createElementNS(HMI.HMI_Constants.NAMESPACE_SVG, 'tspan');
		svgTspan.appendChild(HMI.svgDocument.createTextNode(responseArray[12]));
		
		if (responseArray[8] == "auto"){
		}else if (responseArray[8] == "middle"){
			svgTspan.setAttribute("dy", "0.7ex");
		}else if (responseArray[8] == "hanging"){
			if (svgTspan.style.baselineShift !== undefined){
				svgTspan.style.baselineShift = "-100%";
			}else if (svgTspan.style.dominantBaseline !== undefined){
				svgTspan.style.dominantBaseline = "hanging";
			}else{
				svgTspan.setAttribute("dy", "1ex");
			}
		}
		
		svgElement.appendChild(svgTspan);
		
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
	},
	_getElementsByClassName: function(node, className){
		if (node.getElementsByClassName){
			return node.getElementsByClassName(className);
		} else {
			var testClass = new RegExp("(^|\\s)" + className + "(\\s|$)");
			var node = node || HMI.svgDocument;
			var elements = node.getElementsByTagName("*");
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
};

var filedate = "$Date$";
filedate = filedate.substring(7, filedate.length-2);
if ("undefined" == typeof HMIdate){
	HMIdate = filedate;
}else if (HMIdate < filedate){
	HMIdate = filedate;
}
