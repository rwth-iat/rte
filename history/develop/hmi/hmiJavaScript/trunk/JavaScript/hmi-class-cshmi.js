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
	//every Action, Condition and Event could be requested multiple times
	//to build the visualisation. It should only transfered once,
	//so we have to remember the request client side.
	this.ResourceList = Object();
	this.ResourceList.Elements = Object();
	this.ResourceList.Actions = Object();
	this.ResourceList.Conditions = Object();
	this.ResourceList.Events = Object();
	this.ResourceList.baseKsPath = Object();
	this.ResourceList.ChildList = Object();
	
	//holds the information if the visualisation is filled with content right now
	this.initStage = false;
	
	//we want to add all elements to a class to find it later
	this.cshmiComponentClass = "cshmi-component";
	this.cshmiGroupClass = "cshmi-group";
	this.cshmiTemplateClass = "cshmi-template";
	this.cshmiTemplateHideableClass = "cshmi-hideabletemplate";
};





//#########################################################################################################################
//TODO: check return value of gethandleid
//#########################################################################################################################

/***********************************************************************
	prototype
***********************************************************************/
cshmi.prototype = {
	/*********************************
		instanciateCshmi
		-	starts iteration of visualisation
		-	displays the visualisation
	*********************************/
	instanciateCshmi: function (ObjectPath) {
		//we are in the init stage, so the DOM Tree is not populated
		this.initStage = true;
		
		//build the selected sheet aka group. This includes all containing elements
		var Component = this.BuildDomain(null, ObjectPath, "/cshmi/Group");
		
		if(HMI.PlaygroundContainerNode){
			//the displayed size is calculated from the Container-Node in the html, so we correct the dimension of it
			HMI.PlaygroundContainerNode.setAttribute('height', Component.getAttribute('height'));
			HMI.PlaygroundContainerNode.setAttribute('width', Component.getAttribute('width'));
		}
		
		if (Component !== null){
			//build predefined gradients
			//they are used in the fill attributes like this
			//	url(#VertWhite2Black)
			var svgDefs = HMI.svgDocument.createElementNS(HMI.HMI_Constants.NAMESPACE_SVG, 'defs');
			var svgLinGrad = HMI.svgDocument.createElementNS(HMI.HMI_Constants.NAMESPACE_SVG, 'linearGradient');
				svgLinGrad.id = "VertWhite2Black";
				svgLinGrad.setAttribute("x1", "50%");
				svgLinGrad.setAttribute("y1", "0%");
				svgLinGrad.setAttribute("x2", "50%");
				svgLinGrad.setAttribute("y2", "140%");
				var svgStop = HMI.svgDocument.createElementNS(HMI.HMI_Constants.NAMESPACE_SVG, 'stop');
					svgStop.setAttribute("stop-color", "white");
					svgStop.setAttribute("offset", "0%");
					svgLinGrad.appendChild(svgStop);
				svgStop = HMI.svgDocument.createElementNS(HMI.HMI_Constants.NAMESPACE_SVG, 'stop');
					svgStop.setAttribute("stop-color", "black");
					svgStop.setAttribute("offset", "100%");
				svgLinGrad.appendChild(svgStop);
				svgDefs.appendChild(svgLinGrad);
			svgLinGrad = HMI.svgDocument.createElementNS(HMI.HMI_Constants.NAMESPACE_SVG, 'linearGradient');
				svgLinGrad.id = "HorWhite2Black";
				svgLinGrad.setAttribute("x1", "0%");
				svgLinGrad.setAttribute("y1", "50%");
				svgLinGrad.setAttribute("x2", "140%");
				svgLinGrad.setAttribute("y2", "50%");
				svgStop = HMI.svgDocument.createElementNS(HMI.HMI_Constants.NAMESPACE_SVG, 'stop');
					svgStop.setAttribute("stop-color", "white");
					svgStop.setAttribute("offset", "0%");
					svgLinGrad.appendChild(svgStop);
				svgStop = HMI.svgDocument.createElementNS(HMI.HMI_Constants.NAMESPACE_SVG, 'stop');
					svgStop.setAttribute("stop-color", "black");
					svgStop.setAttribute("offset", "100%");
				svgLinGrad.appendChild(svgStop);
				svgDefs.appendChild(svgLinGrad);
			Component.appendChild(svgDefs);
			
			//push the svg Component to the screen
			HMI.Playground.appendChild(Component);
			
			//calculate all offset parameter to be able to display visual feedback
			//this is only possible now, as the orientation of the parents are not defined when they are not appended
			HMI._setLayerPosition(Component);
			//we want to have offset parameter on all visual elements
			var ComponentChilds = Component.getElementsByTagName('*');
			for(var i = 0;i < ComponentChilds.length;i++){
				HMI._setLayerPosition(ComponentChilds[i]);
			}
			//the DOM Tree is populated now
			this.initStage = false;
		}
	},
	/*********************************
		BuildDomain
		-	Main iteration loop for visualisation
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
		}else if (ObjectType.indexOf("/cshmi/Image") !== -1){
			Component = this._buildSvgImage(ObjectParent, ObjectPath);
		}else if (ObjectType.indexOf("/cshmi/ClientEvent") !== -1){
			Result = this._interpreteClientEvent(ObjectParent, ObjectPath);
		}else if (ObjectType.indexOf("/cshmi/TimeEvent") !== -1){
			Result = this._interpreteTimeEvent(ObjectParent, ObjectPath);
		}else if (ObjectType.indexOf("/cshmi/OperatorEvent") !== -1){
			Result = this._interpreteOperatorEvent(ObjectParent, ObjectPath);
		}else{
			if (	ObjectType.indexOf("/cshmi/SetValue") !== -1 ||
					ObjectType.indexOf("/cshmi/IfThenElse") !== -1){
				HMI.hmi_log_info("Actions not supported at this position: (Typ: "+ObjectType+"): "+ObjectPath);
			}else{
				HMI.hmi_log_info("Object (Typ: "+ObjectType+"): "+ObjectPath+" not supported");
			}
		}
		
		//get and prepare Children in an recursive call
		if (Component !== null){
			var responseArray = HMI.KSClient.getChildObjArray(ObjectPath, this);
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
			HMI.hmi_log_info_onwebsite("ClientEvent ("+command[command.length-1]+") "+ObjectPath+" not supported");
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
				//mark changed Component for quick visual feedback (hidden after a second)
				HMI.displaygestureReactionMarker(ObjectParent);
				
				//get and execute all actions
				preserveThis._interpreteAction(ObjectParent, ObjectPath);
			}, false);
		}else if (command[command.length-1] === "doubleclick"){
			ObjectParent.setAttribute("cursor", "pointer");
			var preserveThis = this;	//grabbed from http://jsbin.com/etise/7/edit
			//todo make double click ASV compatible
			/*ObjectParent.addEventListener("click", function(evt){
				if (!(evt.button === 0 && evt.detail ==2)){
				return;
			}*/
			ObjectParent.addEventListener("dblclick", function(evt){
				//mark changed Component for quick visual feedback (hidden after a second)
				HMI.displaygestureReactionMarker(ObjectParent);
				
				//get and execute all actions
				preserveThis._interpreteAction(ObjectParent, ObjectPath);
			}, false);
		}else if (command[command.length-1] === "rightclick"){
			ObjectParent.setAttribute("cursor", "pointer");
			var preserveThis = this;	//grabbed from http://jsbin.com/etise/7/edit
			ObjectParent.addEventListener("contextmenu", function(evt){
				//mark changed Component for quick visual feedback (hidden after a second)
				HMI.displaygestureReactionMarker(ObjectParent);
				
				//get and execute all actions
				preserveThis._interpreteAction(ObjectParent, ObjectPath);
			}, false);
		}else{
			HMI.hmi_log_info_onwebsite("OperatorEvent ("+command[command.length-1]+") "+ObjectPath+" not supported");
		}
	},
	/*********************************
		_interpreteTimeEvent
		-	calling Actions if supported TimeEvent is triggered
	*********************************/
	_interpreteTimeEvent: function(ObjectParent, ObjectPath){
		var skipEvent = false;
		var iterationObject = ObjectParent;
		do{
			//skip eventhandling if the object is not visible
			if(iterationObject.getAttribute("display") === "none"){
				skipEvent = true;
				break;
			}
		}while( (iterationObject = iterationObject.parentNode) && iterationObject !== null && iterationObject.namespaceURI == HMI.HMI_Constants.NAMESPACE_SVG);
		iterationObject = null;
		
		if(skipEvent === false){
			//interprete Action "now", but we want to have the full DOM tree ready
			var preserveThis = this;	//grabbed from http://jsbin.com/etise/7/edit
			window.setTimeout(function(){
				//get and execute all actions
				preserveThis._interpreteAction(ObjectParent, ObjectPath);
			}, 10);
		}
		
		var responseArray;
		//if the Object is scanned earlier, get the cached information (could be the case with templates or repeated/cyclic calls to the same object)
		if (!(this.ResourceList.Events && this.ResourceList.Events[ObjectPath] !== undefined)){
			var response = HMI.KSClient.getVar(null, '{'+ObjectPath+'.cyctime}', null);
			if (response === false){
				//communication error
				return false;
			}else if (response.indexOf("KS_ERR") !== -1){
				HMI.hmi_log_error("cshmi._interpreteTimeEvent of "+ObjectPath+" failed: "+response);
				
				return false;
			}
			responseArray = HMI.KSClient.splitKsResponse(response);
			
			//we have asked the object successful, so remember the result
			this.ResourceList.Events[ObjectPath] = new Object();
			this.ResourceList.Events[ObjectPath].useCount = 1;
			this.ResourceList.Events[ObjectPath].TimeEventParameters = responseArray;
			HMI.hmi_log_trace("cshmi._interpreteTimeEvent: remembering config of "+ObjectPath+" ");
		}else{
			//the object is asked this session, so reuse the config to save communication requests
			responseArray = this.ResourceList.Events[ObjectPath].TimeEventParameters;
			this.ResourceList.Events[ObjectPath].useCount++;
			HMI.hmi_log_trace("cshmi._interpreteTimeEvent: using remembered config of "+ObjectPath+" ("+this.ResourceList.Events[ObjectPath].useCount+")");
		}
		
		//call us again for cyclic interpretation of the Actions
		//only if we are in the initialisation or normal stage
		//and the active cshmi display is "our" one
		if (responseArray.length !== 0 && (this.initStage === true || HMI.Playground.firstChild !== null ) && HMI.cshmi === this){
			var preserveThis = this;	//grabbed from http://jsbin.com/etise/7/edit
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
		var responseArray = HMI.KSClient.getChildObjArray(ObjectPath, this);
		for (var i=0; i < responseArray.length && returnValue == true; i++) {
			var varName = responseArray[i].split(" ");
			if (varName[1].indexOf("/cshmi/SetValue") !== -1){
				returnValue = this._setValue(ObjectParent, ObjectPath+"/"+varName[0]);
			}else if (varName[1].indexOf("/cshmi/IfThenElse") !== -1){
				returnValue = this._interpreteIfThenElse(ObjectParent, ObjectPath+"/"+varName[0]);
			}else if (varName[1].indexOf("/cshmi/GetValue") !== -1){
				HMI.hmi_log_info_onwebsite("GetValue Action ("+varName[1]+")"+ObjectPath+" not useful at this position");
			}else{
				HMI.hmi_log_info_onwebsite("Action ("+varName[1]+")"+ObjectPath+" not supported");
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
		//if the Object is scanned earlier, get the cached information (could be the case with templates or repeated/cyclic calls to the same object)
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
			
			//we have asked the object successful, so remember the result
			this.ResourceList.Actions[ObjectPath] = new Object();
			this.ResourceList.Actions[ObjectPath].useCount = 1;
			this.ResourceList.Actions[ObjectPath].getVarParameters = responseArray;
			HMI.hmi_log_trace("cshmi._getValue: remembering config of "+ObjectPath+" ");
		}else{
			//the object is asked this session, so reuse the config to save communication requests
			responseArray = this.ResourceList.Actions[ObjectPath].getVarParameters;
			this.ResourceList.Actions[ObjectPath].useCount++;
			HMI.hmi_log_trace("cshmi._getValue: using remembered config of "+ObjectPath+" ("+this.ResourceList.Actions[ObjectPath].useCount+")");
		}
		
		for (var i=0; i < responseArray.length; i++) {
			if (responseArray[i] !== ""){
				if (i === 0){
					//ksVar
					if (responseArray[i].charAt(0) == "/"){
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
							//todo asv compatibility
							return null;
						}
					}else if (responseArray[i] == "rotate"){
						//rotate is special, as it is different in OVM and SVG
						var TransformString = ObjectParent.getAttribute("transform");
						//"rotate(45,21.000000,50.000000)" or "rotate(45)"
						
						//remove rotate()
						TransformString = TransformString.replace(")", "").replace("rotate(", "");
						//get first number if there are 3, separated via comma
						return TransformString.split(",")[0];
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
							
							if (TemplateObject.FBReference["default"].charAt(0) === "/"){
								//String begins with / so it is a fullpath
								var result = HMI.KSClient.getVar(null, TemplateObject.FBReference["default"]+'.'+responseArray[i], null);
								var returnValue = HMI.KSClient.splitKsResponse(result);
								if (returnValue.length > 0){
									//valid response
									return returnValue[0];
								}
								//error
								return null;
							}else{
								//a normal relativ path
								HMI.hmi_log_info_onwebsite('GetValue '+ObjectPath+' wrong configured. No relative path allowed');
								return null;
							}
						}
					//loop upwards to find the Template object
					}while( (TemplateObject = TemplateObject.parentNode) && TemplateObject !== null && TemplateObject.namespaceURI == HMI.HMI_Constants.NAMESPACE_SVG);  //the = is no typo here!
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
					}while( (TemplateObject = TemplateObject.parentNode) && TemplateObject !== null && TemplateObject.namespaceURI == HMI.HMI_Constants.NAMESPACE_SVG);  //the = is no typo here!
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
			//debugger;
			//NewValue = this._getValue(ObjectParent, ObjectPath+".value");
			return false;
		}else if (NewValue === undefined){
			//should not happen
			HMI.hmi_log_error("cshmi._setValue on "+ObjectPath+" (baseobject: "+ObjectPath+") failed because of an NewValue of undefined.");
			return false;
		}
		
		//get info where to set the NewValue
		
		var responseArray;
		//if the Object is scanned earlier, get the cached information (could be the case with templates or repeated/cyclic calls to the same object)
		if (!(this.ResourceList.Actions && this.ResourceList.Actions[ObjectPath] !== undefined)){
			var response = HMI.KSClient.getVar(null, '{'+ObjectPath+'.ksVar .elemVar .globalVar .TemplateFBReferenceVariable}', null);
			if (response === false){
				//communication error
				return false;
			}else if (response.indexOf("KS_ERR") !== -1){
				HMI.hmi_log_error("cshmi._setValue of "+ObjectPath+" failed: "+response);
				
				return false;
			}
			responseArray = HMI.KSClient.splitKsResponse(response);
			
			//we have asked the object successful, so remember the result
			this.ResourceList.Actions[ObjectPath] = new Object();
			this.ResourceList.Actions[ObjectPath].useCount = 1;
			this.ResourceList.Actions[ObjectPath].setVarParameters = responseArray;
			HMI.hmi_log_trace("cshmi._setValue: remembering config of "+ObjectPath+" ");
		}else{
			//the object is asked this session, so reuse the config to save communication requests
			responseArray = this.ResourceList.Actions[ObjectPath].setVarParameters;
			this.ResourceList.Actions[ObjectPath].useCount++;
			HMI.hmi_log_trace("cshmi._setValue: using remembered config of "+ObjectPath+" ("+this.ResourceList.Actions[ObjectPath].useCount+")");
		}
		
		//set the new Value
		for (var i=0; i < responseArray.length; i++) {
			if (responseArray[i] !== ""){
				if (i === 0){
					//ksVar
					if (responseArray[i].charAt(0) == "/"){
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
					}else if (responseArray[i] == "rotate"){
						//rotate is special, as it is different in OVM and SVG
						if(ObjectParent.getAttribute("x") !== null){
							ObjectParent.setAttribute("transform", "rotate("+NewValue+","+ObjectParent.getAttribute("x")+","+ObjectParent.getAttribute("y")+")");
						}else if(ObjectParent.getAttribute("cx") !== null){
							ObjectParent.setAttribute("transform", "rotate("+NewValue+","+ObjectParent.getAttribute("cx")+","+ObjectParent.getAttribute("cy")+")");
						}else{
							ObjectParent.setAttribute("transform", "rotate("+NewValue+")");
						}
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
							if (TemplateObject.FBReference["default"].charAt(0) === "/"){
								//String begins with / so it is a fullpath
								response = HMI.KSClient.setVar(null, TemplateObject.FBReference["default"]+'.'+responseArray[i], NewValue, null);
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
					}while( (TemplateObject = TemplateObject.parentNode) && TemplateObject !== null && TemplateObject.namespaceURI == HMI.HMI_Constants.NAMESPACE_SVG);  //the = is no typo here!
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
				this.ResourceList.baseKsPath[ObjectPath].useCount = 1;
				HMI.hmi_log_trace("cshmi._BaseKsPath: remembering config of "+currentPath+" ");
			}else{
				response = this.ResourceList.baseKsPath[currentPath];
				this.ResourceList.baseKsPath[ObjectPath].useCount++;
				HMI.hmi_log_trace("cshmi._BaseKsPath: reusing remembered config of "+currentPath+" ("+this.ResourceList.baseKsPath[ObjectPath].useCount+")");
			}
			if (response === false){
				//communication error
				return returnValue;
			}else if (response.indexOf("KS_ERR_BADPATH") !== -1){
				//an object in tree is no cshmi object
				return returnValue;
			}else if (response !== "{{}}" && response.length > 2){
				var responseArray = HMI.KSClient.splitKsResponse(response);
				if (responseArray[0].charAt(0) === "/"){
					//String begins with / so it is a fullpath
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
		var anyCond;
		//if the Object is scanned earlier, get the cached information (could be the case with templates or repeated/cyclic calls to the same object)
		if (!(this.ResourceList.Actions && this.ResourceList.Actions[ObjectPath] !== undefined)){
			var response = HMI.KSClient.getVar(null, '{'+ObjectPath+'.anycond}', null);
			if (response === false){
				//communication error
				return false;
			}else if (response.indexOf("KS_ERR") !== -1){
				HMI.hmi_log_error("cshmi._interpreteIfThenElse of "+ObjectPath+" failed: "+response);
				
				return false;
			}
			anyCond = HMI.KSClient.splitKsResponse(response)[0];
			
			//we have asked the object successful, so remember the result
			this.ResourceList.Actions[ObjectPath] = new Object();
			this.ResourceList.Actions[ObjectPath].IfThenElseParameters = anyCond;
			this.ResourceList.Actions[ObjectPath].useCount = 1;
			HMI.hmi_log_trace("cshmi._interpreteIfThenElse: remembering config of "+ObjectPath+" ");
		}else{
			//the object is asked this session, so reuse the config to save communication requests
			anyCond = this.ResourceList.Actions[ObjectPath].IfThenElseParameters;
			this.ResourceList.Actions[ObjectPath].useCount++;
			HMI.hmi_log_trace("cshmi._interpreteIfThenElse: using remembered config of "+ObjectPath+" ("+this.ResourceList.Actions[ObjectPath].useCount+")");
		}
		
		var ConditionMatched = false;
		var responseArray = HMI.KSClient.getChildObjArray(ObjectPath+".if", this);
		var i = 0;
		if (anyCond == "TRUE"){
			//logical OR
			while(i < responseArray.length && ConditionMatched !== true){
				var varName = responseArray[i].split(" ");
				if (varName[1].indexOf("/cshmi/Compare") !== -1){
					ConditionMatched = this._checkCondition(ObjectParent, ObjectPath+".if/"+varName[0], ObjectPath);
				}
				i++;
			}
		}else{
			ConditionMatched = true;
			//logical AND
			while(i < responseArray.length && ConditionMatched !== false){
				var varName = responseArray[i].split(" ");
				if (varName[1].indexOf("/cshmi/Compare") !== -1){
					ConditionMatched = this._checkCondition(ObjectParent, ObjectPath+".if/"+varName[0], ObjectPath);
				}
				i++;
			}
		}
		if (ConditionMatched === true){
			this._interpreteAction(ObjectParent, ObjectPath+".then");
		}else if (ConditionMatched === false){
			this._interpreteAction(ObjectParent, ObjectPath+".else");
		}else{
			//this Action produced an error
			return false;
		}
		
		return true;
	},
	/*********************************
		_checkCondition
		-	checks Condition
	*********************************/
	_checkCondition: function(ObjectParent, ObjectPath, ConditionPath){
		//get Values
		var comptype;
		//if the Object is scanned earlier, get the cached information (could be the case with templates or repeated/cyclic calls to the same object)
		if (!(this.ResourceList.Conditions && this.ResourceList.Conditions[ObjectPath] !== undefined)){
			comptype = HMI.KSClient.getVar(null, '{'+ObjectPath+'.comptype}', null);
			
			//we have asked the object successful, so remember the result
			this.ResourceList.Conditions[ObjectPath] = new Object();
			this.ResourceList.Conditions[ObjectPath].checkConditionParameters = comptype;
			this.ResourceList.Conditions[ObjectPath].useCount = 1;
			HMI.hmi_log_trace("cshmi._checkCondition: remembering config of "+ObjectPath+" ");
		}else{
			//the object is asked this session, so reuse the config to save communication requests
			comptype = this.ResourceList.Conditions[ObjectPath].checkConditionParameters;
			this.ResourceList.Conditions[ObjectPath].useCount++;
			HMI.hmi_log_trace("cshmi._checkCondition: using remembered config of "+ObjectPath+" ("+this.ResourceList.Conditions[ObjectPath].useCount+")");
		}
		
		var Value1 = this._getValue(ObjectParent, ObjectPath+".value1");
		var Value2 = this._getValue(ObjectParent, ObjectPath+".value2");
		
		if (Value1 === null){
			HMI.hmi_log_info("cshmi._checkCondition on "+ObjectPath+" (baseobject: "+ObjectPath+") failed because Value1 is null.");
			return null;
		}
		if (Value2 === null){
			HMI.hmi_log_info("cshmi._checkCondition on "+ObjectPath+" (baseobject: "+ObjectPath+") failed because Value2 is null.");
			return null;
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
			return null;
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
		var responseArray;
		
		//if the Object is scanned earlier, get the cached information (could be the case with templates or repeated/cyclic calls to the same object)
		if (!(this.ResourceList.Elements && this.ResourceList.Elements[ObjectPath] !== undefined)){
			var response = HMI.KSClient.getVar(null, '{'+ObjectPath+'.visible .stroke .fill .opacity .rotate .x .y .TemplateDefinition .FBReference .ConfigValues}', null);
			if (response === false){
				//communication error
				return null;
			}else if (response.indexOf("KS_ERR") !== -1){
				HMI.hmi_log_error("cshmi._buildFromTemplate of "+ObjectPath+" failed: "+response);
				
				return null;
			}
			
			//transform "...{{/TechUnits/Pump1}} {{pumpcolor:yellow pumpname:N18}}..." 
			//to [..., "/TechUnits/Pump1", "pumpcolor:yellow pumpname:N18", ...]
			responseArray = HMI.KSClient.splitKsResponse(response);
			
			//we have asked the object successful, so remember the result
			this.ResourceList.Elements[ObjectPath] = new Object();
			this.ResourceList.Elements[ObjectPath].ElementParameters = responseArray;
			this.ResourceList.Elements[ObjectPath].useCount = 1;
			HMI.hmi_log_trace("cshmi._buildFromTemplate: remembering config of "+ObjectPath+" ");
		}else{
			//the object is asked this session, so reuse the config to save communication requests
			responseArray = this.ResourceList.Elements[ObjectPath].ElementParameters;
			this.ResourceList.Elements[ObjectPath].useCount++;
			HMI.hmi_log_trace("cshmi._buildFromTemplate: using remembered config of "+ObjectPath+" ("+this.ResourceList.Elements[ObjectPath].useCount+")");
		}
		
		var TemplateLocation = "/TechUnits/cshmi/Templates/";
		if (responseArray.length > 8 && responseArray[7] === ""){
			HMI.hmi_log_info_onwebsite("Template "+ObjectPath+" is not configured");
			return null;
		}else if (ObjectPath.indexOf(TemplateLocation+responseArray[7]) === 0){
			HMI.hmi_log_info_onwebsite("Template "+ObjectPath+" is calling itself");
			return null;
		}
		
		var responseArrayTemplate;
		//if the Object is scanned earlier, get the cached information (could be the case with templates or repeated/cyclic calls to the same object)
		if (!(this.ResourceList.Elements && this.ResourceList.Elements[TemplateLocation+responseArray[7]] !== undefined)){
			var response = HMI.KSClient.getVar(null, '{'+TemplateLocation+responseArray[7]+'.width .height .hideable}', null);
			if (response === false){
				//communication error
				return null;
			}else if (response.indexOf("KS_ERR") !== -1){
				HMI.hmi_log_info_onwebsite("Template "+ObjectPath+" is wrong configured. TemplateDefinition '"+responseArray[0]+"' is not available.");
				HMI.hmi_log_error("cshmi._buildSvgContainer of "+ObjectPath+" failed: "+response);
				return null;
			}
			responseArrayTemplate = HMI.KSClient.splitKsResponse(response);
			
			//we have asked the object successful, so remember the result
			this.ResourceList.Elements[TemplateLocation+responseArray[7]] = new Object();
			this.ResourceList.Elements[TemplateLocation+responseArray[7]].TemplateParameters = responseArrayTemplate;
			this.ResourceList.Elements[TemplateLocation+responseArray[7]].useCount = 1;
			HMI.hmi_log_trace("cshmi._buildFromTemplate: remembering config of "+ObjectPath+" ");
		}else{
			//the object is asked this session, so reuse the config to save communication requests
			responseArrayTemplate = this.ResourceList.Elements[TemplateLocation+responseArray[7]].TemplateParameters;
			this.ResourceList.Elements[TemplateLocation+responseArray[7]].useCount++;
			HMI.hmi_log_trace("cshmi._buildFromTemplate: using remembered config of "+TemplateLocation+responseArray[7]+" ("+this.ResourceList.Elements[TemplateLocation+responseArray[7]].useCount+")");
		}
		
		var svgElement = HMI.svgDocument.createElementNS(HMI.HMI_Constants.NAMESPACE_SVG, 'svg');
		svgElement.id = ObjectPath;
		svgElement.setAttribute("TemplateDescription", TemplateLocation+responseArray[7]);
		
		this._addClass(svgElement, this.cshmiTemplateClass);
		this._addClass(svgElement, this.cshmiComponentClass);
		if (responseArrayTemplate[2] === "TRUE"){
			this._addClass(svgElement, this.cshmiTemplateHideableClass);
		}
		
		//setting the basic Element Variables like .visible .stroke .fill .opacity .rotate
		//Template has x y coordinates
		this._processBasicVariables(svgElement, responseArray, true);
		
		svgElement.setAttribute("width", responseArrayTemplate[0]);
		svgElement.setAttribute("height", responseArrayTemplate[1]);
		svgElement.style.overflow = "visible";
		
		//parametrise templateDefinition with the config
		svgElement.FBReference = Object();
		svgElement.ConfigValues = Object();
		var ConfigEntry = null;
		var ConfigList = responseArray[8].split(" ");
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
		ConfigList = responseArray[9].split(" ");
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
		var responseArrayChild = HMI.KSClient.getChildObjArray(TemplateLocation+responseArray[7], this);
		for (var i=0; i < responseArrayChild.length; i++) {
			var varName = responseArrayChild[i].split(" ");
			var ChildComponent = this.BuildDomain(svgElement, TemplateLocation+responseArray[7]+"/"+varName[0], varName[1]);
			if (ChildComponent !== null){
				svgElement.appendChild(ChildComponent);
			}
		}
		var preserveThis = this;	//grabbed from http://jsbin.com/etise/7/edit
		ObjectParent.setAttribute("cursor", "pointer");
		//todo make double click ASV compatible
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
		var responseArray;
		//if the Object is scanned earlier, get the cached information (could be the case with templates or repeated/cyclic calls to the same object)
		if (!(this.ResourceList.Elements && this.ResourceList.Elements[ObjectPath] !== undefined)){
			var response = HMI.KSClient.getVar(null, '{'+ObjectPath+'.visible .stroke .fill .opacity .rotate .x1 .y1 .x2 .y2}', null);
			if (response === false){
				//communication error
				return null;
			}else if (response.indexOf("KS_ERR") !== -1){
				HMI.hmi_log_error("cshmi._buildSvgLine of "+ObjectPath+" failed: "+response);
				return null;
			}
			responseArray = HMI.KSClient.splitKsResponse(response);
			
			//we have asked the object successful, so remember the result
			this.ResourceList.Elements[ObjectPath] = new Object();
			this.ResourceList.Elements[ObjectPath].ElementParameters = responseArray;
			this.ResourceList.Elements[ObjectPath].useCount = 1;
			HMI.hmi_log_trace("cshmi._buildSvgLine: remembering config of "+ObjectPath+" ");
		}else{
			//the object is asked this session, so reuse the config to save communication requests
			responseArray = this.ResourceList.Elements[ObjectPath].ElementParameters;
			this.ResourceList.Elements[ObjectPath].useCount++;
			HMI.hmi_log_trace("cshmi._buildSvgLine: using remembered config of "+ObjectPath+" ("+this.ResourceList.Elements[ObjectPath].useCount+")");
		}
		
		var svgElement = HMI.svgDocument.createElementNS(HMI.HMI_Constants.NAMESPACE_SVG, 'line');
		svgElement.id = ObjectPath;
		
		//setting the basic Element Variables like .visible .stroke .fill .opacity .rotate
		//Line has no x y coordinates
		this._processBasicVariables(svgElement, responseArray, false);
		
		svgElement.setAttribute("x1", responseArray[5]);
		svgElement.setAttribute("y1", responseArray[6]);
		svgElement.setAttribute("x2", responseArray[7]);
		svgElement.setAttribute("y2", responseArray[8]);
		
		return svgElement;
	},
	_buildSvgPolyline: function(ObjectParent, ObjectPath){
		var responseArray;
		//if the Object is scanned earlier, get the cached information (could be the case with templates or repeated/cyclic calls to the same object)
		if (!(this.ResourceList.Elements && this.ResourceList.Elements[ObjectPath] !== undefined)){
			var response = HMI.KSClient.getVar(null, '{'+ObjectPath+'.visible .stroke .fill .opacity .rotate .points}', null);
			if (response === false){
				//communication error
				return null;
			}else if (response.indexOf("KS_ERR") !== -1){
				HMI.hmi_log_error("cshmi._buildSvgPolyline of "+ObjectPath+" failed: "+response);
				return null;
			}
			responseArray = HMI.KSClient.splitKsResponse(response);
			
			//we have asked the object successful, so remember the result
			this.ResourceList.Elements[ObjectPath] = new Object();
			this.ResourceList.Elements[ObjectPath].ElementParameters = responseArray;
			this.ResourceList.Elements[ObjectPath].useCount = 1;
			HMI.hmi_log_trace("cshmi._buildSvgPolyline: remembering config of "+ObjectPath+" ");
		}else{
			//the object is asked this session, so reuse the config to save communication requests
			responseArray = this.ResourceList.Elements[ObjectPath].ElementParameters;
			this.ResourceList.Elements[ObjectPath].useCount++;
			HMI.hmi_log_trace("cshmi._buildSvgPolyline: using remembered config of "+ObjectPath+" ("+this.ResourceList.Elements[ObjectPath].useCount+")");
		}
		
		var svgElement = HMI.svgDocument.createElementNS(HMI.HMI_Constants.NAMESPACE_SVG, 'polyline');
		svgElement.id = ObjectPath;
		
		//setting the basic Element Variables like .visible .stroke .fill .opacity .rotate
		//Polyline has no x y coordinates
		this._processBasicVariables(svgElement, responseArray, false);
		
		svgElement.setAttribute("points", responseArray[5]);
		
		return svgElement;
	},
	_buildSvgPolygon: function(ObjectParent, ObjectPath){
		var responseArray;
		//if the Object is scanned earlier, get the cached information (could be the case with templates or repeated/cyclic calls to the same object)
		if (!(this.ResourceList.Elements && this.ResourceList.Elements[ObjectPath] !== undefined)){
			var response = HMI.KSClient.getVar(null, '{'+ObjectPath+'.visible .stroke .fill .opacity .rotate .points}', null);
			if (response === false){
				//communication error
				return null;
			}else if (response.indexOf("KS_ERR") !== -1){
				HMI.hmi_log_error("cshmi._buildSvgPolygon of "+ObjectPath+" failed: "+response);
				return null;
			}
			responseArray = HMI.KSClient.splitKsResponse(response);
			
			//we have asked the object successful, so remember the result
			this.ResourceList.Elements[ObjectPath] = new Object();
			this.ResourceList.Elements[ObjectPath].ElementParameters = responseArray;
			this.ResourceList.Elements[ObjectPath].useCount = 1;
			HMI.hmi_log_trace("cshmi._buildSvgPolygon: remembering config of "+ObjectPath+" ");
		}else{
			//the object is asked this session, so reuse the config to save communication requests
			responseArray = this.ResourceList.Elements[ObjectPath].ElementParameters;
			this.ResourceList.Elements[ObjectPath].useCount++;
			HMI.hmi_log_trace("cshmi._buildSvgPolygon: using remembered config of "+ObjectPath+" ("+this.ResourceList.Elements[ObjectPath].useCount+")");
		}
		
		var svgElement = HMI.svgDocument.createElementNS(HMI.HMI_Constants.NAMESPACE_SVG, 'polygon');
		svgElement.id = ObjectPath;
		
		//setting the basic Element Variables like .visible .stroke .fill .opacity .rotate
		//Polygon has no x y coordinates
		this._processBasicVariables(svgElement, responseArray, false);
		
		svgElement.setAttribute("points", responseArray[5]);
		
		return svgElement;
	},
	_buildSvgPath: function(ObjectParent, ObjectPath){
		var responseArray;
		//if the Object is scanned earlier, get the cached information (could be the case with templates or repeated/cyclic calls to the same object)
		if (!(this.ResourceList.Elements && this.ResourceList.Elements[ObjectPath] !== undefined)){
			var response = HMI.KSClient.getVar(null, '{'+ObjectPath+'.visible .stroke .fill .opacity .rotate .d}', null);
			if (response === false){
				//communication error
				return null;
			}else if (response.indexOf("KS_ERR") !== -1){
				HMI.hmi_log_error("cshmi._buildSvgPath of "+ObjectPath+" failed: "+response);
				return null;
			}
			responseArray = HMI.KSClient.splitKsResponse(response);
			
			//we have asked the object successful, so remember the result
			this.ResourceList.Elements[ObjectPath] = new Object();
			this.ResourceList.Elements[ObjectPath].ElementParameters = responseArray;
			this.ResourceList.Elements[ObjectPath].useCount = 1;
			HMI.hmi_log_trace("cshmi._buildSvgPath: remembering config of "+ObjectPath+" ");
		}else{
			//the object is asked this session, so reuse the config to save communication requests
			responseArray = this.ResourceList.Elements[ObjectPath].ElementParameters;
			this.ResourceList.Elements[ObjectPath].useCount++;
			HMI.hmi_log_trace("cshmi._buildSvgPath: using remembered config of "+ObjectPath+" ("+this.ResourceList.Elements[ObjectPath].useCount+")");
		}
		
		var svgElement = HMI.svgDocument.createElementNS(HMI.HMI_Constants.NAMESPACE_SVG, 'path');
		svgElement.id = ObjectPath;
		
		//setting the basic Element Variables like .visible .stroke .fill .opacity .rotate
		//Path has no x y coordinates
		this._processBasicVariables(svgElement, responseArray, false);
		
		svgElement.setAttribute("d", responseArray[5]);
		
		return svgElement;
		
		/* Code from Yannick Bochatay http://ybochatay.fr
		autoSmooth = function() {
			var list = this.pathSegList,
			i,N = list.numberOfItems,
			seg,nm1,n0,np1,np2,x0,y0,x1,y1,x2,y2,x3,y3,
			tgx0,tgy0,tgx3,tgy3,dx,dy,d,dt0,dt3,ft0,ft3;

			if (N < 3) {return;}
			for (i=0;i<N-1;i++){
				nm1 = (i===0) ? list.getItem(i) : list.getItem(i-1);
				n0 = list.getItem(i);
				np1 = list.getItem(i+1);
				np2 = (i===N-2) ? list.getItem(i+1) : list.getItem(i+2);
				        
				x0 = n0.x;  y0 = n0.y;
				x3 = np1.x;    y3 = np1.y;
				
				tgx3 = x0 - np2.x;
				tgy3 = y0 - np2.y;
				tgx0 = nm1.x - np1.x;
				tgy0 = nm1.y - np1.y;
				dx  = Math.abs(x0 - x3);
				dy  = Math.abs(y0 - y3);
				d   = Math.sqrt(dx*dx + dy*dy);
				dt3 = Math.sqrt(tgx3*tgx3 + tgy3*tgy3);
				dt0 = Math.sqrt(tgx0*tgx0 + tgy0*tgy0);
				
				if (d !== 0){
					ft3 = (dt3 / d) * 3;
					ft0 = (dt0 / d) * 3;
					
					x1 = x0 - tgx0 / ft0;
					y1 = y0 - tgy0 / ft0;
					x2 = x3 + tgx3 / ft3;
					y2 = y3 + tgy3 / ft3;
					
					seg = this.createSVGPathSegCurvetoCubicAbs(np1.x,np1.y,x1,y1,x2,y2);
					list.replaceItem(seg,i+1);
				}
			}
		};
		*/
	},
	_buildSvgText: function(ObjectParent, ObjectPath){
		var responseArray;
		//if the Object is scanned earlier, get the cached information (could be the case with templates or repeated/cyclic calls to the same object)
		if (!(this.ResourceList.Elements && this.ResourceList.Elements[ObjectPath] !== undefined)){
			var response = HMI.KSClient.getVar(null, '{'+ObjectPath+'.visible .stroke .fill .opacity .rotate .x .y .fontSize .fontStyle .fontWeight .fontFamily .horAlignment .verAlignment .content}', null);
			if (response === false){
				//communication error
				return null;
			}else if (response.indexOf("KS_ERR") !== -1){
				HMI.hmi_log_error("cshmi._buildSvgText of "+ObjectPath+" failed: "+response);
				return null;
			}
			responseArray = HMI.KSClient.splitKsResponse(response);
			
			//we have asked the object successful, so remember the result
			this.ResourceList.Elements[ObjectPath] = new Object();
			this.ResourceList.Elements[ObjectPath].ElementParameters = responseArray;
			this.ResourceList.Elements[ObjectPath].useCount = 1;
			HMI.hmi_log_trace("cshmi._buildSvgText: remembering config of "+ObjectPath+" ");
		}else{
			//the object is asked this session, so reuse the config to save communication requests
			responseArray = this.ResourceList.Elements[ObjectPath].ElementParameters;
			this.ResourceList.Elements[ObjectPath].useCount++;
			HMI.hmi_log_trace("cshmi._buildSvgText: using remembered config of "+ObjectPath+" ("+this.ResourceList.Elements[ObjectPath].useCount+")");
		}
		
		var svgElement = HMI.svgDocument.createElementNS(HMI.HMI_Constants.NAMESPACE_SVG, 'text');
		svgElement.id = ObjectPath;
		
		//setting the basic Element Variables like .visible .stroke .fill .opacity .rotate
		//Text has x y coordinates
		this._processBasicVariables(svgElement, responseArray, true);
		svgElement.setAttribute("font-size", responseArray[7]);
		svgElement.setAttribute("font-style", responseArray[8]);
		svgElement.setAttribute("font-weight", responseArray[9]);
		svgElement.setAttribute("font-family", responseArray[10]);
		svgElement.setAttribute("text-anchor", responseArray[11]);
		
		var svgTspan = HMI.svgDocument.createElementNS(HMI.HMI_Constants.NAMESPACE_SVG, 'tspan');
		svgTspan.appendChild(HMI.svgDocument.createTextNode(responseArray[13]));
		
		if (responseArray[12] == "auto"){
		}else if (responseArray[12] == "middle"){
			svgTspan.setAttribute("dy", "0.7ex");
		}else if (responseArray[12] == "hanging"){
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
		var responseArray;
		//if the Object is scanned earlier, get the cached information (could be the case with templates or repeated/cyclic calls to the same object)
		if (!(this.ResourceList.Elements && this.ResourceList.Elements[ObjectPath] !== undefined)){
			var response = HMI.KSClient.getVar(null, '{'+ObjectPath+'.visible .stroke .fill .opacity .rotate .cx .cy .r}', null);
			if (response === false){
				//communication error
				return null;
			}else if (response.indexOf("KS_ERR") !== -1){
				HMI.hmi_log_error("cshmi._buildSvgCircle of "+ObjectPath+" failed: "+response);
				return null;
			}
			responseArray = HMI.KSClient.splitKsResponse(response);
			
			//we have asked the object successful, so remember the result
			this.ResourceList.Elements[ObjectPath] = new Object();
			this.ResourceList.Elements[ObjectPath].ElementParameters = responseArray;
			this.ResourceList.Elements[ObjectPath].useCount = 1;
			HMI.hmi_log_trace("cshmi._buildSvgCircle: remembering config of "+ObjectPath+" ");
		}else{
			//the object is asked this session, so reuse the config to save communication requests
			responseArray = this.ResourceList.Elements[ObjectPath].ElementParameters;
			this.ResourceList.Elements[ObjectPath].useCount++;
			HMI.hmi_log_trace("cshmi._buildSvgCircle: using remembered config of "+ObjectPath+" ("+this.ResourceList.Elements[ObjectPath].useCount+")");
		}
		
		var svgElement = HMI.svgDocument.createElementNS(HMI.HMI_Constants.NAMESPACE_SVG, 'circle');
		svgElement.id = ObjectPath;
		
		//setting the basic Element Variables like .visible .stroke .fill .opacity .rotate
		//Circle has no x y coordinates
		this._processBasicVariables(svgElement, responseArray, false);
		svgElement.setAttribute("cx", responseArray[5]);
		svgElement.setAttribute("cy", responseArray[6]);
		svgElement.setAttribute("r", responseArray[7]);
		
		return svgElement;
	},
	_buildSvgEllipse: function(ObjectParent, ObjectPath){
		var responseArray;
		//if the Object is scanned earlier, get the cached information (could be the case with templates or repeated/cyclic calls to the same object)
		if (!(this.ResourceList.Elements && this.ResourceList.Elements[ObjectPath] !== undefined)){
			var response = HMI.KSClient.getVar(null, '{'+ObjectPath+'.visible .stroke .fill .opacity .rotate .cx .cy .rx .ry}', null);
			if (response === false){
				//communication error
				return null;
			}else if (response.indexOf("KS_ERR") !== -1){
				HMI.hmi_log_error("cshmi._buildSvgEllipse of "+ObjectPath+" failed: "+response);
				return null;
			}
			responseArray = HMI.KSClient.splitKsResponse(response);
			
			//we have asked the object successful, so remember the result
			this.ResourceList.Elements[ObjectPath] = new Object();
			this.ResourceList.Elements[ObjectPath].ElementParameters = responseArray;
			this.ResourceList.Elements[ObjectPath].useCount = 1;
			HMI.hmi_log_trace("cshmi._buildSvgEllipse: remembering config of "+ObjectPath+" ");
		}else{
			//the object is asked this session, so reuse the config to save communication requests
			responseArray = this.ResourceList.Elements[ObjectPath].ElementParameters;
			this.ResourceList.Elements[ObjectPath].useCount++;
			HMI.hmi_log_trace("cshmi._buildSvgEllipse: using remembered config of "+ObjectPath+" ("+this.ResourceList.Elements[ObjectPath].useCount+")");
		}
		
		var svgElement = HMI.svgDocument.createElementNS(HMI.HMI_Constants.NAMESPACE_SVG, 'ellipse');
		svgElement.id = ObjectPath;
		
		//setting the basic Element Variables like .visible .stroke .fill .opacity .rotate
		//Ellipse has no x y coordinates
		this._processBasicVariables(svgElement, responseArray, false);
		svgElement.setAttribute("cx", responseArray[5]);
		svgElement.setAttribute("cy", responseArray[6]);
		svgElement.setAttribute("rx", responseArray[7]);
		svgElement.setAttribute("ry", responseArray[8]);
		
		//rotation should be around cx and cy
		svgElement.setAttribute("transform", "rotate("+responseArray[4]+","+responseArray[5]+","+responseArray[6]+")");
		
		return svgElement;
	},
	_buildSvgRect: function(ObjectParent, ObjectPath){
		var responseArray;
		//if the Object is scanned earlier, get the cached information (could be the case with templates or repeated/cyclic calls to the same object)
		if (!(this.ResourceList.Elements && this.ResourceList.Elements[ObjectPath] !== undefined)){
			var response = HMI.KSClient.getVar(null, '{'+ObjectPath+'.visible .stroke .fill .opacity .rotate .x .y .width .height}', null);
			if (response === false){
				//communication error
				return null;
			}else if (response.indexOf("KS_ERR") !== -1){
				HMI.hmi_log_error("cshmi._buildSvgRect of "+ObjectPath+" failed: "+response);
				return null;
			}
			responseArray = HMI.KSClient.splitKsResponse(response);
			
			//we have asked the object successful, so remember the result
			this.ResourceList.Elements[ObjectPath] = new Object();
			this.ResourceList.Elements[ObjectPath].ElementParameters = responseArray;
			this.ResourceList.Elements[ObjectPath].useCount = 1;
			HMI.hmi_log_trace("cshmi._buildSvgRect: remembering config of "+ObjectPath+" ");
		}else{
			//the object is asked this session, so reuse the config to save communication requests
			responseArray = this.ResourceList.Elements[ObjectPath].ElementParameters;
			this.ResourceList.Elements[ObjectPath].useCount++;
			HMI.hmi_log_trace("cshmi._buildSvgRect: using remembered config of "+ObjectPath+" ("+this.ResourceList.Elements[ObjectPath].useCount+")");
		}
		
		var svgElement = HMI.svgDocument.createElementNS(HMI.HMI_Constants.NAMESPACE_SVG, 'rect');
		svgElement.id = ObjectPath;
		
		//setting the basic Element Variables like .visible .stroke .fill .opacity .rotate
		//Rect has x y coordinates
		this._processBasicVariables(svgElement, responseArray, true);
		
		svgElement.setAttribute("width", responseArray[7]);
		svgElement.setAttribute("height", responseArray[8]);
		
		return svgElement;
	},
	_buildSvgImage: function(ObjectParent, ObjectPath){
		var responseArray;
		//if the Object is scanned earlier, get the cached information (could be the case with templates or repeated/cyclic calls to the same object)
		if (!(this.ResourceList.Elements && this.ResourceList.Elements[ObjectPath] !== undefined)){
			var response = HMI.KSClient.getVar(null, '{'+ObjectPath+'.visible .stroke .fill .opacity .rotate .x .y .SVGcontent .Bitmapcontent}', null);
			if (response === false){
				//communication error
				return null;
			}else if (response.indexOf("KS_ERR") !== -1){
				HMI.hmi_log_error("cshmi._buildSvgRect of "+ObjectPath+" failed: "+response);
				return null;
			}
			responseArray = HMI.KSClient.splitKsResponse(response);
			
			//we have asked the object successful, so remember the result
			this.ResourceList.Elements[ObjectPath] = new Object();
			this.ResourceList.Elements[ObjectPath].ElementParameters = responseArray;
			this.ResourceList.Elements[ObjectPath].useCount = 1;
			HMI.hmi_log_trace("cshmi._buildSvgRect: remembering config of "+ObjectPath+" ");
		}else{
			//the object is asked this session, so reuse the config to save communication requests
			responseArray = this.ResourceList.Elements[ObjectPath].ElementParameters;
			this.ResourceList.Elements[ObjectPath].useCount++;
			HMI.hmi_log_trace("cshmi._buildSvgRect: using remembered config of "+ObjectPath+" ("+this.ResourceList.Elements[ObjectPath].useCount+")");
		}
		
//		var svgElement = HMI.svgDocument.createElementNS(HMI.HMI_Constants.NAMESPACE_SVG, 'svg');
		
		
		var svgElement;
		if(responseArray[7] !== ""){
			responseArray[7] =	"<svg:svg xmlns:svg=\""+HMI.HMI_Constants.NAMESPACE_SVG+"\" xmlns=\""+HMI.HMI_Constants.NAMESPACE_SVG+"\">"
				+responseArray[7]
				+"</svg:svg>";
			svgElement = HMI.HMIDOMParser.parse(responseArray[7], null);
			svgElement.id = ObjectPath;
			//svgElement.style.overflow = "visible";
			
			//setting the basic Element Variables like .visible .stroke .fill .opacity .rotate
			//Image has x y coordinates
			this._processBasicVariables(svgElement, responseArray, true);
		}else if(responseArray[8] !== ""){
			return null;
		}else{
			HMI.hmi_log_info_onwebsite("Image "+ObjectPath+" is not configured");
			return null;
		}
		
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
	_processBasicVariables: function(svgElement, responseArray, processXY){
		//the order of settings is:
		//	.visible .stroke .fill .opacity .rotate
		//	if x and y is available, this are the next
		if (responseArray[0] == "FALSE"){
			svgElement.setAttribute("display", "none");
		}else{
			svgElement.setAttribute("display", "block");
		}
		//set dimension of container
		svgElement.setAttribute("stroke", responseArray[1]);
		svgElement.setAttribute("fill", responseArray[2]);
		svgElement.setAttribute("opacity", responseArray[3]);
		if(processXY === true){
			//the attribute should be "rotate(deg, x, y)"
			svgElement.setAttribute("transform", "rotate("+responseArray[4]+","+responseArray[5]+","+responseArray[6]+")");
			svgElement.setAttribute("x", responseArray[5]);
			svgElement.setAttribute("y", responseArray[6]);
		}else{
			svgElement.setAttribute("transform", "rotate("+responseArray[4]+")");
		}
	},
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
			node = node || HMI.svgDocument;
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
