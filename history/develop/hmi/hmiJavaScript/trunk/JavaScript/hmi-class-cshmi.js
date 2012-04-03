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
	this.ResourceList.onloadCallStack = Array();
	
	this.ResourceList.Elements = Object();
	this.ResourceList.Actions = Object();
	this.ResourceList.Conditions = Object();
	this.ResourceList.Events = Object();
	this.ResourceList.baseKsPath = Object();
	this.ResourceList.ChildList = Object();
	this.ResourceList.ChildrenIterator = Object();
	this.ResourceList.InstantiateTemplate = Object();
	this.ResourceList.EventInfos = Object();
	this.ResourceList.GlobalVar = Object();
	
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
			var ComponentChilds = Component.getElementsByTagNameNS(HMI.HMI_Constants.NAMESPACE_SVG, '*');
			for(var i = 0;i < ComponentChilds.length;i++){
				HMI._setLayerPosition(ComponentChilds[i]);
			}
			
			//interprete onload Actions in the order of occurrence
			while(this.ResourceList.onloadCallStack.length !== 0){
				var EventObjItem = this.ResourceList.onloadCallStack.shift();
				this._interpreteAction(EventObjItem["ObjectParent"], EventObjItem["ObjectPath"]);
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
					ObjectType.indexOf("/cshmi/IfThenElse") !== -1 ||
					ObjectType.indexOf("/cshmi/ChildrenIterator") !== -1 ){
				HMI.hmi_log_info("Actions not supported at this position: (Typ: "+ObjectType+"): "+ObjectPath);
			}else{
				HMI.hmi_log_info("Object (Typ: "+ObjectType+"): "+ObjectPath+" not supported");
			}
		}
		
		//check type of returnvalue
		if (!(Result === true || Result === false)){
			HMI.hmi_log_error("Action "+ObjectPath+" given a non boolean returnvalue.");
		}
		if (Component === undefined || Component === true || Component === false){
			HMI.hmi_log_error("Component "+ObjectPath+" given a wrong returnvalue type.");
		}
		
		//get and prepare Children in an recursive call
		if (Component !== null){
			var responseArray = HMI.KSClient.getChildObjArray(ObjectPath, this);
			for (var i=0; i < responseArray.length; i++) {
				var varName = responseArray[i].split(" ");
				if(Component.tagName === "g" && Component.id === "" && Component.firstChild && Component.firstChild.id !== ""){
					//nested rotation template
					var realComponent = Component.firstChild;
				}else{
					realComponent = Component;
				}
				var ChildComponent = this.BuildDomain(realComponent, realComponent.id+"/"+varName[0], varName[1]);
				if (ChildComponent !== null){
					realComponent.appendChild(ChildComponent);
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
			//interprete Action later, so remember this
			var EventObjItem = Object();
			EventObjItem["ObjectParent"] = ObjectParent;
			EventObjItem["ObjectPath"] = ObjectPath;
			this.ResourceList.onloadCallStack.push(EventObjItem)
		}else{
			HMI.hmi_log_info_onwebsite("ClientEvent ("+command[command.length-1]+") "+ObjectPath+" not supported");
		}
		return true;
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
			ObjectParent.addEventListener("click", function(evt){
				preserveThis.ResourceList.EventInfos.EventObj = evt;
				//mark changed Component for quick visual feedback (hidden after a second)
				HMI.displaygestureReactionMarker(ObjectParent);
				
				//get and execute all actions
				preserveThis._interpreteAction(ObjectParent, ObjectPath);
				if (evt.stopPropagation) evt.stopPropagation();
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
				preserveThis.ResourceList.EventInfos.EventObj = evt;
				//mark changed Component for quick visual feedback (hidden after a second)
				HMI.displaygestureReactionMarker(ObjectParent);
				
				//get and execute all actions
				preserveThis._interpreteAction(ObjectParent, ObjectPath);
				if (evt.stopPropagation) evt.stopPropagation();
			}, false);
		}else if (command[command.length-1] === "rightclick"){
			var preserveThis = this;	//grabbed from http://jsbin.com/etise/7/edit
			ObjectParent.addEventListener("contextmenu", function(evt){
				preserveThis.ResourceList.EventInfos.EventObj = evt;
				//mark changed Component for quick visual feedback (hidden after a second)
				HMI.displaygestureReactionMarker(ObjectParent);
				
				//get and execute all actions
				preserveThis._interpreteAction(ObjectParent, ObjectPath);
				if (evt.stopPropagation) evt.stopPropagation();
				if (evt.preventDefault) evt.preventDefault();  //default is a context menu, so disable it
			}, false);
		}else if (command[command.length-1] === "aftermove"){
			ObjectParent.setAttribute("cursor", "move");
			
			var preserveThis = this;	//grabbed from http://jsbin.com/etise/7/edit
			
			//make an dummy wrapper function so we can use ObjectParent and ObjectPath in it, 
			//addEventListener only provides the event object
			ObjectParent._moveStartDragThunk = function(evt){
				preserveThis._moveStartDrag(ObjectParent, ObjectPath, evt);
			}
			ObjectParent._moveMouseMoveThunk = function(evt){
				preserveThis._moveMouseMove(ObjectParent, ObjectPath, evt);
			}
			ObjectParent._moveStopDragThunk = function(evt){
				//stop with interpreting the actions
				preserveThis._moveStopDrag(ObjectParent, ObjectPath, evt, false);
			}
			ObjectParent._moveCancelDragThunk = function(evt){
				//stop without interpreting the actions
				preserveThis._moveStopDrag(ObjectParent, ObjectPath, evt, true);
			}
			//todo: try to implement via HTML5 drag&drop
			
			//try both, mousedown and mousetouch. mousetouch will fire first, there we will kill mousedown
			ObjectParent.addEventListener("touchstart", ObjectParent._moveStartDragThunk, false);
			ObjectParent.addEventListener("mousedown", ObjectParent._moveStartDragThunk, false);
		}else{
			HMI.hmi_log_info_onwebsite("OperatorEvent ("+command[command.length-1]+") "+ObjectPath+" not supported");
		}
		return true;
	},
	_moveStartDrag : function(ObjectParent, ObjectPath, evt){
		if (evt.button == 2){
			//right click
			HMI.hmi_log_trace("moveStartDrag - hit right mouse button on "+ObjectParent.id);
			if (evt.stopPropagation) evt.stopPropagation();
			return;
		}
		HMI.hmi_log_trace("moveStartDrag - Start with object: "+ObjectParent.id);
		this.ResourceList.EventInfos.EventObj = evt;
		//memorize Startposition
		var mouseposition = HMI.getClickPosition(evt, null);
		this.ResourceList.EventInfos.startXMouse = mouseposition[0];
		this.ResourceList.EventInfos.startYMouse = mouseposition[1];
		this.ResourceList.EventInfos.startXObj = parseInt(ObjectParent.getAttribute("x"), 10);
		this.ResourceList.EventInfos.startYObj = parseInt(ObjectParent.getAttribute("y"), 10);
		
		if(evt.type === 'touchstart'){
			HMI.hmi_log_trace("moveStartDrag - touch (x:"+mouseposition[0]+",y:"+mouseposition[1]+") detected, killing legacy events");
			//we have touch gestures, so kill legacy mousedown
			ObjectParent.removeEventListener("mousedown", ObjectParent._moveStartDragThunk, false);
			
			document.addEventListener("touchmove", ObjectParent._moveMouseMoveThunk, false);
			document.addEventListener("touchend", ObjectParent._moveStopDragThunk, false);
			document.addEventListener("touchcancel", ObjectParent._moveCancelDragThunk, false);
		}else{
			HMI.hmi_log_trace("moveStartDrag - legacy click (x:"+mouseposition[0]+",y:"+mouseposition[1]+") detected");
			document.addEventListener("mousemove", ObjectParent._moveMouseMoveThunk, false);
			document.addEventListener("mouseup", ObjectParent._moveStopDragThunk, false);
		}
		if (evt.stopPropagation) evt.stopPropagation();
	},
	_moveMouseMove : function(ObjectParent, ObjectPath, evt){
		if (this.ResourceList.EventInfos.startXObj === null){
			return;
		}
		var mouseposition = HMI.getClickPosition(evt, null);
		var mouseX = mouseposition[0];
		var mouseY = mouseposition[1];
		var newx = this.ResourceList.EventInfos.startXObj+mouseX-this.ResourceList.EventInfos.startXMouse;
		var newy = this.ResourceList.EventInfos.startYObj+mouseY-this.ResourceList.EventInfos.startYMouse;
		if (!isNaN(newx) && !isNaN(newy)){
			ObjectParent.setAttribute("x", newx);
			ObjectParent.setAttribute("y", newy);
			
			HMI._setLayerPosition(ObjectParent);
			//we want to have offset parameter on all visual elements
			var ComponentChilds = ObjectParent.getElementsByTagNameNS(HMI.HMI_Constants.NAMESPACE_SVG, '*');
			for(var i = 0;i < ComponentChilds.length;i++){
				HMI._setLayerPosition(ComponentChilds[i]);
			}
			//save event for use in an action
			this.ResourceList.EventInfos.EventObj = evt;
		}
		if (evt.stopPropagation) evt.stopPropagation();
		if (evt.preventDefault) evt.preventDefault();  //default is scrolling, so disable it
	},
	_moveStopDrag : function(ObjectParent, ObjectPath, evt, canceled){
		HMI.hmi_log_trace("moveStopDrag - Stop with object: "+ObjectParent.id);
		if(evt.type === 'touchend'){
			HMI.hmi_log_trace("moveStartDrag - touch up detected");
			document.removeEventListener("touchmove", ObjectParent._moveMouseMoveThunk, false);
			document.removeEventListener("touchend", ObjectParent._moveStopDragThunk, false);
			document.removeEventListener("touchcancel", ObjectParent._moveCancelDragThunk, false);
			//the touchend has no xy position (since the fingers left the device!), so an action should work on the last move eventobj
		}else{
			HMI.hmi_log_trace("moveStartDrag - legacy mouse up detected");
			document.removeEventListener("mousemove", ObjectParent._moveMouseMoveThunk, false);
			document.removeEventListener("mouseup", ObjectParent._moveStopDragThunk, false);
			//the mouseup event has xy position, so remember for use in an action
			this.ResourceList.EventInfos.EventObj = evt;
		}
		
		//restore old position
		ObjectParent.setAttribute("x", this.ResourceList.EventInfos.startXObj);
		ObjectParent.setAttribute("y", this.ResourceList.EventInfos.startYObj);
		HMI._setLayerPosition(ObjectParent);
		//we want to have offset parameter on all visual elements
		var ComponentChilds = ObjectParent.getElementsByTagNameNS(HMI.HMI_Constants.NAMESPACE_SVG, '*');
		for(var i = 0;i < ComponentChilds.length;i++){
			HMI._setLayerPosition(ComponentChilds[i]);
		}
		
		if (canceled === true){
			//an action should not interprete this event
			this.ResourceList.EventInfos.EventObj = null;
		}else{
			//get and execute all actions
			this._interpreteAction(ObjectParent, ObjectPath);
		}
		
		if (evt.stopPropagation) evt.stopPropagation();
	},
	/*********************************
		_interpreteTimeEvent
		-	calling Actions if supported TimeEvent is triggered
	*********************************/
	_interpreteTimeEvent: function(ObjectParent, ObjectPath){
		var skipEvent = false;
		//check if the page is visible at all?
		//http://www.w3.org/TR/page-visibility/
		if(	document.hidden === true ||
			document.mozHidden === true||
			document.webkitHidden === true||
			document.msHidden === true||
			document.oHidden === true
		){
			skipEvent = true;
		}
		if (this.initStage === true){
			//we are in the init state of the sheet so interprete Action later onload, remembering this
			var EventObjItem = Object();
			EventObjItem["ObjectParent"] = ObjectParent;
			EventObjItem["ObjectPath"] = ObjectPath;
			this.ResourceList.onloadCallStack.push(EventObjItem);
		}else if(skipEvent === false){
			this._interpreteAction(ObjectParent, ObjectPath);
		}
		
		var responseArray;
		//if the Object was scanned earlier, get the cached information (could be the case with templates or repeated/cyclic calls to the same object)
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
			//the object was asked this session, so reuse the config to save communication requests
			responseArray = this.ResourceList.Events[ObjectPath].TimeEventParameters;
			this.ResourceList.Events[ObjectPath].useCount++;
			HMI.hmi_log_trace("cshmi._interpreteTimeEvent: using remembered config of "+ObjectPath+" (#"+this.ResourceList.Events[ObjectPath].useCount+")");
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
		for (var i=0; i < responseArray.length && returnValue === true; i++) {
			var varName = responseArray[i].split(" ");
			if (varName[1].indexOf("/cshmi/SetValue") !== -1){
				returnValue = this._setValue(ObjectParent, ObjectPath+"/"+varName[0]);
			}else if (varName[1].indexOf("/cshmi/IfThenElse") !== -1){
				returnValue = this._interpreteIfThenElse(ObjectParent, ObjectPath+"/"+varName[0]);
			}else if (varName[1].indexOf("/cshmi/GetValue") !== -1){
				HMI.hmi_log_info_onwebsite("GetValue Action ("+varName[1]+")"+ObjectPath+" not useful at this position");
			}else if (varName[1].indexOf("/cshmi/ChildrenIterator") !== -1){
				returnValue = this._interpreteChildrenIterator(ObjectParent, ObjectPath+"/"+varName[0]);
			}else if (varName[1].indexOf("/cshmi/InstantiateTemplate") !== -1){
				returnValue = this._interpreteInstantiateTemplate(ObjectParent, ObjectPath+"/"+varName[0]);
			}else if (varName[1].indexOf("/cshmi/RoutePolyline") !== -1){
				returnValue = this._interpreteRoutePolyline(ObjectParent, ObjectPath+"/"+varName[0]);
			}else{
				HMI.hmi_log_info_onwebsite("Action ("+varName[1]+") "+ObjectPath+" not supported");
			}
			//check type of returnvalue
			if (!(returnValue === true || returnValue === false)){
				HMI.hmi_log_error("Action ("+varName[1]+") of "+ObjectPath+" given a non boolean return value.");
			}
		}
		return returnValue;
	},
	/*********************************
		_getValue
		-	get a Value from multiple Sources
	*********************************/
	_getValue: function(ObjectParent, ObjectPath){
		var requestList;
		var ParameterName;
		var ParameterValue;
		//if the Object is scanned earlier, get the cached information (could be the case with templates or repeated/cyclic calls to the same object)
		if (!(this.ResourceList.Actions && this.ResourceList.Actions[ObjectPath] !== undefined)){
			requestList = new Object();
			requestList[ObjectPath] = new Object();
			requestList[ObjectPath]["ksVar"] = null;
			requestList[ObjectPath]["elemVar"] = null;
			requestList[ObjectPath]["elemVarPath"] = null;
			requestList[ObjectPath]["globalVar"] = null;
			requestList[ObjectPath]["OperatorInput"] = null;
			requestList[ObjectPath]["TemplateFBReferenceVariable"] = null;
			requestList[ObjectPath]["TemplateConfigValues"] = null;
			requestList[ObjectPath]["value"] = null;
			
			var successCode = this._executeVariablesArray(requestList);
			if (successCode == false){
				return false;
			}
			
			for (ParameterName in requestList[ObjectPath]){
				ParameterValue = requestList[ObjectPath][ParameterName];
				if (ParameterValue !== ""){
					break;
				}
			}
			if (ParameterValue === ""){
				//the action was not configured
				ParameterName = "";
			}
			
			//we have asked the object successful, so remember the result
			this.ResourceList.Actions[ObjectPath] = new Object();
			this.ResourceList.Actions[ObjectPath].ParameterName = ParameterName;
			this.ResourceList.Actions[ObjectPath].ParameterValue = ParameterValue;
			this.ResourceList.Actions[ObjectPath].useCount = 1;
			HMI.hmi_log_trace("cshmi._getValue: remembering config of "+ObjectPath+" ");
		}else{
			//the object is asked this session, so reuse the config to save communication requests
			requestList = new Object();
			ParameterName = this.ResourceList.Actions[ObjectPath].ParameterName;
			ParameterValue = this.ResourceList.Actions[ObjectPath].ParameterValue;
			this.ResourceList.Actions[ObjectPath].useCount++;
			//HMI.hmi_log_trace("cshmi._getValue: using remembered config of "+ObjectPath+" (#"+this.ResourceList.Actions[ObjectPath].useCount+")");
		}
		
		var iterationObject = ObjectParent;
		do{
			//skip eventhandling if the object is not visible
			if(iterationObject.getAttribute("display") === "none"){
				var preventNetworkRequest = true;
				break;
			}
		}while( (iterationObject = iterationObject.parentNode) && iterationObject !== null && iterationObject.namespaceURI == HMI.HMI_Constants.NAMESPACE_SVG);
		iterationObject = null;
		
		if (ParameterName === "ksVar" && preventNetworkRequest !== true){
			var response;
			if (ParameterValue.charAt(0) == "/"){
				//we have an absolute path on this server
				response = HMI.KSClient.getVar(null, '{'+ParameterValue+'}', null);
			}else{
				//get baseKsPath
				var baseKsPath = this._getBaseKsPath(ObjectParent, ObjectPath);
				response = HMI.KSClient.getVar(baseKsPath.serverhandle, '{'+baseKsPath.path+"/"+ParameterValue+'}', null);
			}
			var responseArray = HMI.KSClient.splitKsResponse(response);
			if (responseArray.length === 0){
				return null;
			}else{
				return responseArray[0];
			}
		}else if (ParameterName === "elemVar"){
			//todo interprete elemVarPath
			if (ParameterValue == "content"){
				//content is special, as it is different in OVM and SVG
				if (typeof ObjectParent.textContent != "undefined"){
					return ObjectParent.textContent;
				}else if (typeof ObjectParent.innerText != "undefined"){
					return ObjectParent.innerText;
				}else{
					//todo asv compatibility
					return null;
				}
			}else if (ParameterValue == "title"){
				for (var i = 0;i < ObjectParent.childNodes.length;i++){
					if (ObjectParent.childNodes.item(i).tagName === "title"){
						return ObjectParent.childNodes.item(i).firstChild.textContent;
					}
				} 
				return null;
			}else if (ParameterValue == "visible"){
				//display is special, as it is different in OVM and SVG
				var displayVar = ObjectParent.getAttribute("display");
				if (displayVar == "none"){
					return "FALSE";
				}else{
					return "TRUE";
				}
			}else if (ParameterValue == "rotate"){
				//rotate is special, as it is different in OVM and SVG
				if (ObjectParent.tagName == "svg" && ObjectParent.parentNode.tagName == "g" && ObjectParent.parentNode.id === ""){
					//svg are not transformable, so the rotation is in the objects parent
					var TransformString = ObjectParent.parentNode.getAttribute("transform");
				}else if (ObjectParent.tagName == "svg"){
					//it is an template, with no rotation
					return "0";
				}else{
					TransformString = ObjectParent.getAttribute("transform");
				}
				//"rotate(45,21.000000,50.000000)" or "rotate(45)"
				
				//remove rotate()
				TransformString = TransformString.replace(")", "").replace("rotate(", "");
				//get first number if there are 3, separated via comma
				return TransformString.split(",")[0];
			}else if (ObjectParent.hasAttribute(ParameterValue)){
				return ObjectParent.getAttribute(ParameterValue);
			}else{
				//unknown element variable
				return null;
			}
		}else if (ParameterName === "globalVar"){
			if (this.ResourceList.GlobalVar[ParameterValue] !== undefined){
				return this.ResourceList.GlobalVar[ParameterValue];
			}else {
				return null;
			}
		}else if (ParameterName === "OperatorInput"){
			if(ParameterValue.indexOf("textinput") !== -1){
				var input;
				var textinputHint;
				var splittedValueParameter = ParameterValue.split(":");
				if (splittedValueParameter.length > 1){
					textinputHint = splittedValueParameter[1];
					//e.g. "textinput:Some textinputHint:TemplateFBReferenceVariable:InputVarPath"
					if (splittedValueParameter.length > 3){
						this.ResourceList.Actions["tempPath"] = new Object();
						this.ResourceList.Actions["tempPath"].ParameterName = splittedValueParameter[2];
						this.ResourceList.Actions["tempPath"].ParameterValue = splittedValueParameter[3];
						input = window.prompt(textinputHint, this._getValue(ObjectParent, "tempPath"));
						delete this.ResourceList.Actions["tempPath"];
					//e.g. "textinput:Some textinputHint:fixValue"
					}else if (splittedValueParameter.length > 2){
						input = window.prompt(textinputHint, splittedValueParameter[2]);
					}else{
						//e.g. "textinput:Some textinputHint"
						input = window.prompt(textinputHint);
					}
				}else{
					textinputHint = 'Please input a new value';
					input = window.prompt(textinputHint);
				}
				if (input !== null){
					return input;
				}
			}else if(ParameterValue === "mousex"){
				var newX = HMI.getClickPosition(this.ResourceList.EventInfos.EventObj, null)[0];
				if (!isNaN(newX)){
					return this.ResourceList.EventInfos.startXObj+newX-this.ResourceList.EventInfos.startXMouse;
				}
			}else if(ParameterValue === "mousey"){
				var newY = HMI.getClickPosition(this.ResourceList.EventInfos.EventObj, null)[1];
				if (!isNaN(newY)){
					return this.ResourceList.EventInfos.startYObj+newY-this.ResourceList.EventInfos.startYMouse;
				}
			}else{
				HMI.hmi_log_info_onwebsite('GetValue OperatorInput not implemented. command: '+ParameterValue);
			}
			return null;
		}else if (ParameterName === "TemplateFBReferenceVariable" && preventNetworkRequest !== true){
			var TemplateObject = ObjectParent;
			do{
				if(TemplateObject.FBReference && TemplateObject.FBReference[ParameterValue] !== undefined){
					//a named variable of a FBReference was requested, naming was done in instantiateTemplate
					if (TemplateObject.FBReference[ParameterValue].charAt(0) === "/"){
						//String begins with / so it is a fullpath
						var result = HMI.KSClient.getVar(null, TemplateObject.FBReference[ParameterValue], null);
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
				}else if(TemplateObject.FBReference && TemplateObject.FBReference["default"] !== undefined){
					//a variable from a Template was requested
					
					if(ParameterValue == "identifier"){
						//if the identifier is requested calculate this to avoid network request
						var Objectname = TemplateObject.FBReference["default"].split("/");
						return Objectname[Objectname.length - 1];
					}
					
					if (TemplateObject.FBReference["default"].charAt(0) === "/"){
						//String begins with / so it is a fullpath
						var result = HMI.KSClient.getVar(null, TemplateObject.FBReference["default"]+'.'+ParameterValue, null);
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
		}else if (ParameterName === "TemplateConfigValues"){
			var TemplateObject = ObjectParent;
			do{
				if(TemplateObject.ConfigValues && TemplateObject.ConfigValues[ParameterValue] !== undefined){
					//this is a ConfigValue and has valid data for us
					return TemplateObject.ConfigValues[ParameterValue];
				}
			//loop upwards to find the Template object
			}while( (TemplateObject = TemplateObject.parentNode) && TemplateObject !== null && TemplateObject.namespaceURI == HMI.HMI_Constants.NAMESPACE_SVG);  //the = is no typo here!
			return null;
		}else if (ParameterName === "value"){
			return ParameterValue;
		}
		return ""; //fallback
	},
	/*********************************
		_setValue
		-	sets a Value to multiple Targets
	*********************************/
	_setValue: function(ObjectParent, ObjectPath){
		//get Value to set (via getValue-part of setValue Object)
		var NewValue = this._getValue(ObjectParent, ObjectPath+".value");
		
		if (NewValue === null){
			HMI.hmi_log_info("cshmi._setValue on "+ObjectPath+" (baseobject: "+ObjectPath+") failed because of an NewValue of null.");
			//NewValue = this._getValue(ObjectParent, ObjectPath+".value");
			return false;
		}else if (NewValue === undefined){
			//should not happen
			HMI.hmi_log_error("cshmi._setValue on "+ObjectPath+" (baseobject: "+ObjectPath+") failed because of an NewValue of undefined.");
			return false;
		}
		
		//get info where to set the NewValue
		
		var requestList;
		var ParameterName;
		var ParameterValue;
		//if the Object is scanned earlier, get the cached information (could be the case with templates or repeated/cyclic calls to the same object)
		if (!(this.ResourceList.Actions && this.ResourceList.Actions[ObjectPath] !== undefined)){
			requestList = new Object();
			requestList[ObjectPath] = new Object();
			requestList[ObjectPath]["ksVar"] = null;
			requestList[ObjectPath]["elemVar"] = null;
			requestList[ObjectPath]["globalVar"] = null;
			requestList[ObjectPath]["TemplateFBReferenceVariable"] = null;
			
			var successCode = this._executeVariablesArray(requestList);
			if (successCode == false){
				return false;
			}
			
			for (ParameterName in requestList[ObjectPath]){
				ParameterValue = requestList[ObjectPath][ParameterName];
				if (ParameterValue !== ""){
					break;
				}
			}
			if (ParameterValue === ""){
				//the action was not configured
				ParameterName = "";
			}
			
			//we have asked the object successful, so remember the result
			this.ResourceList.Actions[ObjectPath] = new Object();
			this.ResourceList.Actions[ObjectPath].useCount = 1;
			this.ResourceList.Actions[ObjectPath].ParameterName = ParameterName;
			this.ResourceList.Actions[ObjectPath].ParameterValue = ParameterValue;
			HMI.hmi_log_trace("cshmi._setValue: remembering config of "+ObjectPath+" ");
		}else{
			//the object is asked this session, so reuse the config to save communication requests
			requestList = new Object();
			ParameterName = this.ResourceList.Actions[ObjectPath].ParameterName;
			ParameterValue = this.ResourceList.Actions[ObjectPath].ParameterValue;
			this.ResourceList.Actions[ObjectPath].useCount++;
			HMI.hmi_log_trace("cshmi._setValue: using remembered config of "+ObjectPath+" (#"+this.ResourceList.Actions[ObjectPath].useCount+")");
		}
		
		//set the new Value
		if (ParameterName === "ksVar"){
			//ksVar
			if (ParameterValue.charAt(0) == "/"){
				//we have an absolute path on this server
				HMI.KSClient.setVar(null, '{'+ParameterValue+'}', NewValue, null);
			}else{
				//get baseKsPath
				var baseKsPath = this._getBaseKsPath(ObjectParent, ObjectPath);
				HMI.KSClient.setVar(baseKsPath.serverhandle, '{'+baseKsPath.path+ParameterValue+'}', NewValue, null);
			}
			return true;
		}else if (ParameterName === "elemVar"){
			if (ParameterValue == "content"){
				//content is special, as it is different in OVM and SVG
				
				//if trimToLength is set in parent TextFB and perform trimming if needed
				var trimLength = parseInt(this.ResourceList.Elements[ObjectParent.id].ElementParameters.trimToLength, 10);
				var contentLength = parseInt(NewValue.length, 10);
				var trimmedContent;
				if((trimLength > 0) && (contentLength > trimLength)){
					trimmedContent = NewValue.substr(0, trimLength) + String.fromCharCode(8230);
					ObjectParent.firstChild.replaceChild(HMI.svgDocument.createTextNode(trimmedContent), ObjectParent.firstChild.firstChild);
					this._setTitle(ObjectParent, NewValue);
				}else if((trimLength < 0) && (contentLength > -trimLength)){
					trimmedContent =  String.fromCharCode(8230) + NewValue.substr(contentLength + trimLength);
					ObjectParent.firstChild.replaceChild(HMI.svgDocument.createTextNode(trimmedContent), ObjectParent.firstChild.firstChild);
					this._setTitle(ObjectParent, NewValue);
				}else{
					ObjectParent.firstChild.replaceChild(HMI.svgDocument.createTextNode(NewValue), ObjectParent.firstChild.firstChild);
				}
			}else if (ParameterValue == "title"){
				this._setTitle(ObjectParent, NewValue);
			}else if (ParameterValue == "visible"){
				//visible is special, as it is different in OVM and SVG
				if (NewValue == "FALSE"){
					ObjectParent.setAttribute("display", "none");
				}else{
					ObjectParent.setAttribute("display", "block");
				}
			}else if (ParameterValue == "rotate"){
				//rotate is special, as it is different in OVM and SVG
				
				//svg are not transformable, so the rotation is in the objects parent
				if (ObjectParent.tagName === "svg" && ObjectParent.parentNode.tagName === "g" && ObjectParent.parentNode.id === ""){
					//object has already an g parent
					var rotationObject = ObjectParent.parentNode;
				}else if (ObjectParent.tagName === "svg" && ObjectParent.parentNode.tagName !== "g"){
					//element has to be shifted into an g element
					var rotationObject = HMI.svgDocument.createElementNS(HMI.HMI_Constants.NAMESPACE_SVG, 'g');
					rotationObject.style.overflow = "visible";
					ObjectParent.parentNode.replaceChild(rotationObject, ObjectParent);
					rotationObject.appendChild(ObjectParent);
				}else{
					//normal visual element
					rotationObject = ObjectParent;
				}
				
				if(ObjectParent.getAttribute("x") !== null){
					rotationObject.setAttribute("transform", "rotate("+NewValue+","+ObjectParent.getAttribute("x")+","+ObjectParent.getAttribute("y")+")");
				}else if(ObjectParent.getAttribute("cx") !== null){
					ObjectParent.setAttribute("transform", "rotate("+NewValue+","+ObjectParent.getAttribute("cx")+","+ObjectParent.getAttribute("cy")+")");
				}else{
					ObjectParent.setAttribute("transform", "rotate("+NewValue+")");
				}
			}else{
				ObjectParent.setAttribute(ParameterValue, NewValue);
				//reposition Layer if x, y, width or height is changed
				if (ParameterValue === "x" || ParameterValue === "y" || ParameterValue === "width" || ParameterValue === "height"){
					HMI._setLayerPosition(ObjectParent);
					//we want to have offset parameter on all visual elements
					var ComponentChilds = ObjectParent.getElementsByTagNameNS(HMI.HMI_Constants.NAMESPACE_SVG, '*');
					for(var i = 0;i < ComponentChilds.length;i++){
						HMI._setLayerPosition(ComponentChilds[i]);
					}
				}
			}
			return true;
		}else if (ParameterName === "globalVar"){
			//globalVar
			this.ResourceList.GlobalVar[ParameterValue] = NewValue;
			return true;
		}else if (ParameterName === "TemplateFBReferenceVariable"){
			//TemplateFBReferenceVariable
			var TemplateObject = ObjectParent;
			do{
				if(TemplateObject.FBReference && TemplateObject.FBReference[ParameterValue] !== undefined){
					//a named variable of a FBReference was requested, naming was done in instantiateTemplate
					if (TemplateObject.FBReference[ParameterValue].charAt(0) === "/"){
						//String begins with / so it is a fullpath
						var response = HMI.KSClient.setVar(null, TemplateObject.FBReference[ParameterValue], NewValue, null);
						if (response.indexOf("KS_ERR") !== -1){
							HMI.hmi_log_info_onwebsite('Setting '+TemplateObject.FBReference[ParameterValue]+' not successfull: '+response+' (configured here: '+ObjectPath+').');
						}
						return true;
					}else{
						//a normal relativ path
						HMI.hmi_log_info_onwebsite('SetValue '+ObjectPath+' wrong configured. No relative path allowed');
						return false;
					}
				}else if(TemplateObject.FBReference && TemplateObject.FBReference["default"] !== undefined){
					//a variable from a Template was requested
					if (TemplateObject.FBReference["default"].charAt(0) === "/"){
						//String begins with / so it is a fullpath
						response = HMI.KSClient.setVar(null, TemplateObject.FBReference["default"]+'.'+ParameterValue, NewValue, null);
						if (response.indexOf("KS_ERR") !== -1){
							HMI.hmi_log_info_onwebsite('Setting '+TemplateObject.FBReference["default"]+'.'+ParameterValue+' not successfull: '+response+' (configured here: '+ObjectPath+').');
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
			return false;
		}
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
				HMI.hmi_log_trace("cshmi._BaseKsPath: reusing remembered config of "+currentPath+" (#"+this.ResourceList.baseKsPath[ObjectPath].useCount+")");
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
			HMI.hmi_log_trace("cshmi._interpreteIfThenElse: using remembered config of "+ObjectPath+" (#"+this.ResourceList.Actions[ObjectPath].useCount+")");
		}
		var ConditionMatched = false;
		var responseArray = HMI.KSClient.getChildObjArray(ObjectPath+".if", this);
		var i = 0;
		if (anyCond == "TRUE"){
			//logical OR
			while(i < responseArray.length && ConditionMatched !== true){
				var varName = responseArray[i].split(" ");
				if (varName[1].indexOf("/cshmi/CompareIteratedChild") !== -1){
					ConditionMatched = this._checkConditionIterator(ObjectParent, ObjectPath+".if/"+varName[0], ObjectPath);
				}
				else if (varName[1].indexOf("/cshmi/Compare") !== -1){
					ConditionMatched = this._checkCondition(ObjectParent, ObjectPath+".if/"+varName[0], ObjectPath);
				}
				i++;
			}
		}else{
			ConditionMatched = true;
			//logical AND
			while(i < responseArray.length && ConditionMatched !== false){
				var varName = responseArray[i].split(" ");
				if (varName[1].indexOf("/cshmi/CompareIteratedChild") !== -1){
					ConditionMatched = this._checkConditionIterator(ObjectParent, ObjectPath+".if/"+varName[0], ObjectPath);
				}
				else if (varName[1].indexOf("/cshmi/Compare") !== -1){
					ConditionMatched = this._checkCondition(ObjectParent, ObjectPath+".if/"+varName[0], ObjectPath);
				}
				i++;
			}
		}
		if (ConditionMatched === true){
			return this._interpreteAction(ObjectParent, ObjectPath+".then");
		}else if (ConditionMatched === false){
			return this._interpreteAction(ObjectParent, ObjectPath+".else");
		}else{
			//this Action produced an error
			return false;
		}
		
		return false;
	},
	/*********************************
	_interpreteChildrenIterator
	*********************************/
	_interpreteChildrenIterator: function(ObjectParent, ObjectPath){
		var rootObject = ObjectParent;
		var FBRef;
		//search FBReference of root Object
		while (rootObject !== null){
			//FBReference found
			if(rootObject.FBReference && rootObject.FBReference["default"] !== undefined){
				FBRef = rootObject.FBReference["default"];
				//FBRef found, we can stop search
				rootObject = null;
			}
			else {
				//loop upwards to find the Template object
				rootObject = rootObject.parentNode;
			}
		}
		//get Values
		var childrenType = HMI.KSClient.getVar(null, '{'+ObjectPath+'.ChildrenType}', null);
		if (childrenType === false){
			//communication error
			return false;
		}else if (childrenType.indexOf("KS_ERR") !== -1){
			HMI.hmi_log_error("cshmi._interpreteChildrenIterator of "+ObjectPath+" failed: "+response);
			return false;
		}
		childrenType = HMI.KSClient.splitKsResponse(childrenType, 0)[0];
		
		var returnValue = true;
		if (childrenType.indexOf("OT_") !== -1){
			var response = HMI.KSClient.getEP(null, encodeURI(FBRef)+'%20*', "%20-type%20$::TKS::" + childrenType + "%20-output%20[expr%20$::TKS::OP_ANY]", null);
			response = HMI.KSClient.splitKsResponse(response, 1);
			for (var i=0; i<response.length; i++){
				var responseDictionary = Array();
				if (childrenType === "OT_VARIABLE"){
					responseDictionary["OP_NAME"] = response[i][0];
					responseDictionary["OP_TYPE"] = response[i][1];
					responseDictionary["OP_COMMENT"] = response[i][2];
					responseDictionary["OP_ACCESS"] = response[i][3];
					responseDictionary["OP_SEMANTICS"] = response[i][4];
					responseDictionary["OP_CREATIONTIME"] = response[i][5];
					responseDictionary["OP_CLASS"] = response[i][6];
					responseDictionary["OP_TECHUNIT"] = response[i][7];
				}
				else if (childrenType === "OT_DOMAIN"){
					responseDictionary["OP_NAME"] = response[i][0];
					responseDictionary["OP_TYPE"] = response[i][1];
					responseDictionary["OP_COMMENT"] = response[i][2];
					responseDictionary["OP_ACCESS"] = response[i][3];
					responseDictionary["OP_SEMANTICS"] = response[i][4];
					responseDictionary["OP_CREATIONTIME"] = response[i][5];
					responseDictionary["OP_CLASS"] = response[i][6];
				}
				this.ResourceList.ChildrenIterator.currentChild = responseDictionary;
				
				returnValue = this._interpreteAction(ObjectParent, ObjectPath + ".forEachChild");
			}
		}
		else {
			//allow mutltiple childrenTypes
			var childrenTypeList = childrenType.split(" ");
			var response;
			for (var i=0; i < childrenTypeList.length; i++) {
				response = HMI.KSClient.getVar(null, '{'+ FBRef +'.' + childrenTypeList[i] + '}', null);
				//get a rid of external brackets 
				response = response.replace(/{/g, "");
				response = response.replace(/}/g, "");
				response = HMI.KSClient.splitKsResponse(response, 1);
				for (var j=0; j<response.length; j++){
					var responseDictionary = Array();
					responseDictionary["OP_VALUE"] = response[j];
					this.ResourceList.ChildrenIterator.currentChild = responseDictionary;
					
					returnValue = this._interpreteAction(ObjectParent, ObjectPath + ".forEachChild");
					}
			}
		}
		//reset Objects, after iteration is done we don't want to cache the last entries
		this.ResourceList.InstantiateTemplate = Object();
		this.ResourceList.ChildrenIterator = Object();
		return returnValue;
	},
	/*********************************
	_interpreteInstantiateTemplate
	*********************************/
	_interpreteInstantiateTemplate: function(ObjectParent, ObjectPath){
		if (this.ResourceList.InstantiateTemplate[ObjectPath] === undefined){
			this.ResourceList.InstantiateTemplate[ObjectPath] = new Object();
			this.ResourceList.InstantiateTemplate[ObjectPath].useCount = 0;
		}
		var Component = this._buildFromTemplate(ObjectParent, ObjectPath, true);
		this.ResourceList.InstantiateTemplate[ObjectPath].useCount ++;
		ObjectParent.appendChild(Component);
		//calculate all offset parameter to be able to display visual feedback
		//needed now, because we append new components
		HMI._setLayerPosition(Component);
		//we want to have offset parameter on all visual elements
		var ComponentChilds = Component.getElementsByTagNameNS(HMI.HMI_Constants.NAMESPACE_SVG, '*');
		for(var i = 0;i < ComponentChilds.length;i++){
			HMI._setLayerPosition(ComponentChilds[i]);
		}
		
		if (this.initStage === false){
			//interprete onload Actions if we are allready loaded
			while(this.ResourceList.onloadCallStack.length !== 0){
				var EventObjItem = this.ResourceList.onloadCallStack.shift();
				this._interpreteAction(EventObjItem["ObjectParent"], EventObjItem["ObjectPath"]);
			}
		}
		
		return true;
	},
	/*********************************
	_interpreteRoutePolyline
	*********************************/
	_interpreteRoutePolyline: function(ObjectParent, ObjectPath){
		if(ObjectParent.tagName.indexOf("polyline") === -1 ){
			HMI.hmi_log_info_onwebsite("RoutePolyline not supported with: "+ObjectParent.tagName+"-Objects (path: "+ObjectPath+")");
			return false;
		}
		//get Values (via getValue-parts)
		var SourceBasename = this._getValue(ObjectParent, ObjectPath+".SourceBasename");
		var SourceVariablename = this._getValue(ObjectParent, ObjectPath+".SourceVariablename");
		var TargetBasename = this._getValue(ObjectParent, ObjectPath+".TargetBasename");
		var TargetVariablename = this._getValue(ObjectParent, ObjectPath+".TargetVariablename");

		var rootObject = ObjectParent;
		var FBRef;
		//search FBReference of root Object
		while (rootObject !== null){
			//FBReference found
			if(rootObject.FBReference && rootObject.FBReference["default"] !== undefined){
				FBRef = rootObject.FBReference["default"];
				//FBRef found, we can stop search
				rootObject = null;
			}
			else {
				//loop upwards to find the Template object
				rootObject = rootObject.parentNode;
				if (rootObject === null){
					//if rootObject is null, we use Source and Target to cache
					FBRef = SourceBasename+SourceVariablename+TargetBasename+TargetVariablename;
					}
				}
		}


		var SourceConnectionPoint = null;
		var SourceConnectionPointdirection = null;
		var TargetConnectionPoint = null;
		var TargetConnectionPointdirection = null;
		//if the Polyline was routed earlier, get the cached information (could be the case with cyclic calls)
		if(FBRef !== null && this.ResourceList.Actions && this.ResourceList.Actions[FBRef] !== undefined){
			//the object is asked this session, so reuse the config to save communication requests
			SourceConnectionPoint = this.ResourceList.Actions[FBRef].SourceConnectionPoint;
			SourceConnectionPointdirection = this.ResourceList.Actions[FBRef].SourceConnectionPointdirection;
			TargetConnectionPoint = this.ResourceList.Actions[FBRef].TargetConnectionPoint;
			TargetConnectionPointdirection = this.ResourceList.Actions[FBRef].TargetConnectionPointdirection;
			this.ResourceList.Actions[FBRef].useCount++;
			HMI.hmi_log_trace("cshmi._interpreteRoutePolyline: using cached information of "+FBRef+" (#"+this.ResourceList.Actions[FBRef].useCount+")");
		}else{
			//Move Polyline-Container before every Functionblocks
			//needs only to be done once
			if (ObjectParent.parentNode.parentNode.tagName === "svg"){
				ObjectParent.parentNode.parentNode.insertBefore(ObjectParent.parentNode, ObjectParent.parentNode.parentNode.firstChild);
			}

			// check if FBref beginn with "//" because we need the server Info as prefix when using getElementById
			// e.g "//dev/ov_hmidemo7/TechUnits/TU10/h_bkqwmtbbhpf"" --> use prefix "//dev/ov_hmidemo7"
			var prefix;
			if (FBRef !== null && FBRef.charAt(0) === "/" && FBRef.charAt(1) === "/"){
				//find the 3rd "/"
				var slashIndex = FBRef.indexOf("/", 2);
				//find the 4th "/"
				slashIndex = FBRef.indexOf("/", slashIndex+1);
				//only keep the String before 4th "/"
				prefix = FBRef.slice(0, slashIndex);
			}else {
				prefix = "";
			}

			var Source;
			var Target;
			if (SourceVariablename !== ""){
				Source = prefix + SourceBasename + "." + SourceVariablename;
			}else{
				Source = prefix + SourceBasename;
			}
			if (TargetVariablename !== ""){
				Target = prefix + TargetBasename + "." + TargetVariablename;
			}else{
				Target = prefix + TargetBasename;
			}
			if (document.getElementById(Source) !== null){
				Source = document.getElementById(Source);
				for(var i = 0; i < Source.childNodes.length;i++){
					// search tagName "circle" with name containing ConnectionPoint
					if (Source.childNodes[i].tagName === "circle" && Source.childNodes[i].id.indexOf("ConnectionPoint") !== -1){
						SourceConnectionPoint = Source.childNodes[i];
						SourceConnectionPointdirection = SourceConnectionPoint.id.slice(SourceConnectionPoint.id.indexOf("ConnectionPoint")+15);
						break;
					}
				}
			//the SourceConnectionPoint is outside the domain
			//search a connectionpoint recursively in all parent svgs for connectionsources that are outside the domain 	
			}else{
				//there should only be one SourceConnectionPointOutsideDomain, so use the cached value if it has been searched before
				if (this.ResourceList.Actions && this.ResourceList.Actions["SourceConnectionPointOutsideDomain"] !== undefined){
					SourceConnectionPoint = this.ResourceList.Actions["SourceConnectionPointOutsideDomain"].SourceConnectionPoint;
					SourceConnectionPointdirection = this.ResourceList.Actions["SourceConnectionPointOutsideDomain"].SourceConnectionPointdirection;
				}
				//search for SourceConnectionPointOutsideDomain in all parent svgs
				else{
					var domainSVG = ObjectParent.parentNode;
					while (domainSVG.tagName === "svg" && SourceConnectionPoint === null){
						for(var i = 0; i < domainSVG.childNodes.length;i++){
							// search tagName "circle" with name containing SourceConnectionPointOutsideDomain
							if (domainSVG.childNodes[i].tagName === "circle" && domainSVG.childNodes[i].id.indexOf("SourceConnectionPointOutsideDomain") !== -1){
								SourceConnectionPoint = domainSVG.childNodes[i];
								SourceConnectionPointdirection = SourceConnectionPoint.id.slice(SourceConnectionPoint.id.indexOf("SourceConnectionPointOutsideDomain")+34);
								break;
							}
						}
						domainSVG = domainSVG.parentNode;
					}
					//remember the result
					this.ResourceList.Actions["SourceConnectionPointOutsideDomain"] = new Object();
					this.ResourceList.Actions["SourceConnectionPointOutsideDomain"].SourceConnectionPoint = SourceConnectionPoint;
					this.ResourceList.Actions["SourceConnectionPointOutsideDomain"].SourceConnectionPointdirection = SourceConnectionPointdirection;
				}
			}

			if (document.getElementById(Target) !== null){
				Target = document.getElementById(Target);
				var TargetConnectionPoint;
				var TargetConnectionPointdirection;
				for(var i = 0; i < Target.childNodes.length;i++){
					// search tagName "circle" with name containing ConnectionPoint
					if (Target.childNodes[i].tagName === "circle" && Target.childNodes[i].id.indexOf("ConnectionPoint") !== -1){
						TargetConnectionPoint = Target.childNodes[i];
						TargetConnectionPointdirection = TargetConnectionPoint.id.slice(TargetConnectionPoint.id.indexOf("ConnectionPoint")+15);
						break;
					}
				}
			//the TargetConnectionPoint is outside the domain
			//search a connectionpoint recursively in all parent svgs for connectiontargets that are outside the domain 	
			}else{
				//there should only be one TargetConnectionPointOutsideDomain, so use the cached value if it has been searched before
				if (this.ResourceList.Actions && this.ResourceList.Actions["TargetConnectionPointOutsideDomain"] !== undefined){
					TargetConnectionPoint = this.ResourceList.Actions["TargetConnectionPointOutsideDomain"].TargetConnectionPoint;
					TargetConnectionPointdirection = this.ResourceList.Actions["TargetConnectionPointOutsideDomain"].TargetConnectionPointdirection;
				}
				//search for TargetConnectionPointOutsideDomain in all parent svgs
				else{
					var domainSVG = ObjectParent.parentNode;
					while (domainSVG.tagName === "svg" && TargetConnectionPoint === null){
						for(var i = 0; i < domainSVG.childNodes.length;i++){
							// search tagName "circle" with name containing TargetConnectionPointOutsideDomain
							if (domainSVG.childNodes[i].tagName === "circle" && domainSVG.childNodes[i].id.indexOf("TargetConnectionPointOutsideDomain") !== -1){
								TargetConnectionPoint = domainSVG.childNodes[i];
								TargetConnectionPointdirection = TargetConnectionPoint.id.slice(TargetConnectionPoint.id.indexOf("TargetConnectionPointOutsideDomain")+34);
								break;
							}
						}
						domainSVG = domainSVG.parentNode;
					}
					//remember the result
					this.ResourceList.Actions["TargetConnectionPointOutsideDomain"] = new Object();
					this.ResourceList.Actions["TargetConnectionPointOutsideDomain"].TargetConnectionPoint = TargetConnectionPoint;
					this.ResourceList.Actions["TargetConnectionPointOutsideDomain"].TargetConnectionPointdirection = TargetConnectionPointdirection;
				}
			}

			//remember the result
			this.ResourceList.Actions[FBRef] = new Object();
			this.ResourceList.Actions[FBRef].SourceConnectionPoint = SourceConnectionPoint;
			this.ResourceList.Actions[FBRef].SourceConnectionPointdirection = SourceConnectionPointdirection;
			this.ResourceList.Actions[FBRef].TargetConnectionPoint = TargetConnectionPoint;
			this.ResourceList.Actions[FBRef].TargetConnectionPointdirection = TargetConnectionPointdirection;
			this.ResourceList.Actions[FBRef].useCount = 1;
			HMI.hmi_log_trace("cshmi._interpreteRoutePolyline: remembering results for "+FBRef+" ");
		}		

		var xStart = parseInt(SourceConnectionPoint.getAttribute("layerX"), 10);
		var cx = parseInt(SourceConnectionPoint.getAttribute("cx"), 10);
		xStart = xStart + cx;
		var cy = parseInt(SourceConnectionPoint.getAttribute("cy"), 10);
		var yStart = parseInt(SourceConnectionPoint.getAttribute("layerY"), 10);
		yStart = yStart +cy;

		var xEnd = parseInt(TargetConnectionPoint.getAttribute("layerX"), 10);
		var cx = parseInt(TargetConnectionPoint.getAttribute("cx"), 10);
		xEnd = xEnd + cx;
		var cy = parseInt(TargetConnectionPoint.getAttribute("cy"), 10);
		var yEnd = parseInt(TargetConnectionPoint.getAttribute("layerY"), 10);
		yEnd = yEnd +cy;

		//if start- and endPoints changed since last time, recompute polyline points
		if (	xStart !== this.ResourceList.Actions[FBRef].xStart ||
				yStart !== this.ResourceList.Actions[FBRef].yStart ||
				xEnd !== this.ResourceList.Actions[FBRef].xEnd ||
				yEnd !== this.ResourceList.Actions[FBRef].yEnd) {
			//cache new start- and endPoints
			this.ResourceList.Actions[FBRef].xStart = xStart;
			this.ResourceList.Actions[FBRef].yStart = yStart;
			this.ResourceList.Actions[FBRef].xEnd = xEnd;
			this.ResourceList.Actions[FBRef].yEnd = yEnd;

			var points;
			if (SourceConnectionPointdirection === "Right" && TargetConnectionPointdirection === "Left"){
				//OUTPUT --> INPUT
				points = xStart + "," + yStart + " ";
				xStart = xStart + 40;
				points = points + xStart + "," + yStart + " ";
				if (xStart >= xEnd) {
					yStart = (yEnd+yStart)/2;
					points = points + xStart + "," + yStart + " ";
					
					xStart = xEnd - 40;
					points = points + xStart + "," + yStart + " ";
				}

				points = points + xStart + "," + yEnd + " ";
				points = points + xEnd + "," + yEnd;
			}else if (SourceConnectionPointdirection === "Left" && TargetConnectionPointdirection === "Right"){
				//INPUT --> OUTPUT
				var xTemp = xStart;
				var yTemp = yStart;
				xStart = xEnd;
				yStart = yEnd;
				xEnd = xTemp;
				yEnd = yTemp;
				points = xStart + "," + yStart + " ";
				xStart = xStart + 40;
				points = points + xStart + "," + yStart + " ";
				if (xStart >= xEnd) {
					yStart = (yEnd+yStart)/2;
					points = points + xStart + "," + yStart + " ";
					
					xStart = xEnd - 40;
					points = points + xStart + "," + yStart + " ";
				}

				points = points + xStart + "," + yEnd + " ";
				points = points + xEnd + "," + yEnd;
			}else if (SourceConnectionPointdirection === "Left" && TargetConnectionPointdirection === "Left"){
				//INPUT --> INPUT
				points = xStart + "," + yStart + " ";
				xStart = xStart - 40;
				points = points + xStart + "," + yStart + " ";
				if (xStart >= xEnd) {
					xStart = xEnd - 40;
					points = points + xStart + "," + yStart + " ";
				}

				points = points + xStart + "," + yEnd + " ";
				points = points + xEnd + "," + yEnd;
			}else if (SourceConnectionPointdirection === "Right" && TargetConnectionPointdirection === "Right"){
				//OUTPUT --> OUTPUT
				points = xStart + "," + yStart + " ";
				xStart = xStart + 40;
				points = points + xStart + "," + yStart + " ";
				if (xStart <= xEnd) {
					xStart = xEnd + 40;
					points = points + xStart + "," + yStart + " ";
				}
		
				points = points + xStart + "," + yEnd + " ";
				points = points + xEnd + "," + yEnd;
			}else if (SourceConnectionPointdirection === "Up" && TargetConnectionPointdirection === "Down"){
				points = xStart + "," + yStart + " ";
				yStart = yStart - 40;
				points = points + xStart + "," + yStart + " ";
				if (yStart <= yEnd) {
					xStart = (xEnd+xStart)/2;
					points = points + xStart + "," + yStart + " ";
					
					yStart = yEnd + 40;
					points = points + xStart + "," + yStart + " ";
				}

				points = points + xEnd + "," + yStart + " ";
				points = points + xEnd + "," + yEnd;
			}else if (SourceConnectionPointdirection === "Down" && TargetConnectionPointdirection === "Up"){
				var xTemp = xStart;
				var yTemp = yStart;
				xStart = xEnd;
				yStart = yEnd;
				xEnd = xTemp;
				yEnd = yTemp;
				points = xStart + "," + yStart + " ";
				yStart = yStart - 40;
				points = points + xStart + "," + yStart + " ";
				if (yStart <= yEnd) {
					xStart = (xEnd+xStart)/2;
					points = points + xStart + "," + yStart + " ";
					
					yStart = yEnd + 40;
					points = points + xStart + "," + yStart + " ";
				}

				points = points + xEnd + "," + yStart + " ";
				points = points + xEnd + "," + yEnd;
			}else if (SourceConnectionPointdirection === "Down" && TargetConnectionPointdirection === "Down"){
				points = xStart + "," + yStart + " ";
				yStart = yStart + 40;
				points = points + xStart + "," + yStart + " ";
				if (yStart <= yEnd) {
					yStart = yEnd + 40;
					points = points + xStart + "," + yStart + " ";
				}

				points = points + xEnd + "," + yStart + " ";
				points = points + xEnd + "," + yEnd;
			}else if (SourceConnectionPointdirection === "Up" && TargetConnectionPointdirection === "Up"){
				points = xStart + "," + yStart + " ";
				yStart = yStart - 40;
				points = points + xStart + "," + yStart + " ";
				if (yStart >= yEnd) {
					yStart = yEnd - 40;
					points = points + xStart + "," + yStart + " ";
				}

				points = points + xEnd + "," + yStart + " ";
				points = points + xEnd + "," + yEnd;
			}else if (SourceConnectionPointdirection === "Right" && TargetConnectionPointdirection === "Up"){
				points = xStart + "," + yStart + " ";
				xStart = xStart + 40;
				points = points + xStart + "," + yStart + " ";
				if (!(yStart < yEnd && xStart < xEnd)){
					yStart = yEnd - 40;
					points = points + xStart + "," + yStart + " ";
				}

				points = points + xEnd + "," + yStart + " ";
				points = points + xEnd + "," + yEnd;
			}else if (SourceConnectionPointdirection === "Up" && TargetConnectionPointdirection === "Right"){
				var xTemp = xStart;
				var yTemp = yStart;
				xStart = xEnd;
				yStart = yEnd;
				xEnd = xTemp;
				yEnd = yTemp;
				points = xStart + "," + yStart + " ";
				xStart = xStart + 40;
				points = points + xStart + "," + yStart + " ";
				if (!(yStart < yEnd && xStart < xEnd)){
					yStart = yEnd - 40;
					points = points + xStart + "," + yStart + " ";
				}

				points = points + xEnd + "," + yStart + " ";
				points = points + xEnd + "," + yEnd;
			}else if (SourceConnectionPointdirection === "Right" && TargetConnectionPointdirection === "Down"){
				points = xStart + "," + yStart + " ";
				xStart = xStart + 40;
				points = points + xStart + "," + yStart + " ";
				if (!(yStart > yEnd && xStart < xEnd)){
					yStart = yEnd + 40;
					points = points + xStart + "," + yStart + " ";
				}
				points = points + xEnd + "," + yStart + " ";
				points = points + xEnd + "," + yEnd;
			}else if (SourceConnectionPointdirection === "Down" && TargetConnectionPointdirection === "Right"){
				var xTemp = xStart;
				var yTemp = yStart;
				xStart = xEnd;
				yStart = yEnd;
				xEnd = xTemp;
				yEnd = yTemp;
				points = xStart + "," + yStart + " ";
				xStart = xStart + 40;
				points = points + xStart + "," + yStart + " ";
				if (!(yStart > yEnd && xStart < xEnd)){
					yStart = yEnd + 40;
					points = points + xStart + "," + yStart + " ";
				}
				points = points + xEnd + "," + yStart + " ";
				points = points + xEnd + "," + yEnd;
			}else if (SourceConnectionPointdirection === "Left" && TargetConnectionPointdirection === "Up"){
				points = xStart + "," + yStart + " ";
				xStart = xStart - 40;
				points = points + xStart + "," + yStart + " ";
				if (!(yStart < yEnd && xStart > xEnd)){
					yStart = yEnd - 40;
					points = points + xStart + "," + yStart + " ";
				}

				points = points + xEnd + "," + yStart + " ";
				points = points + xEnd + "," + yEnd;
			}else if (SourceConnectionPointdirection === "Up" && TargetConnectionPointdirection === "Left"){
				var xTemp = xStart;
				var yTemp = yStart;
				xStart = xEnd;
				yStart = yEnd;
				xEnd = xTemp;
				yEnd = yTemp;
				points = xStart + "," + yStart + " ";
				xStart = xStart - 40;
				points = points + xStart + "," + yStart + " ";
				if (!(yStart < yEnd && xStart > xEnd)){
					yStart = yEnd - 40;
					points = points + xStart + "," + yStart + " ";
				}

				points = points + xEnd + "," + yStart + " ";
				points = points + xEnd + "," + yEnd;
			}else if (SourceConnectionPointdirection === "Left" && TargetConnectionPointdirection === "Down"){
				points = xStart + "," + yStart + " ";
				xStart = xStart - 40;
				points = points + xStart + "," + yStart + " ";
				if (!(yStart > yEnd && xStart > xEnd)){
					yStart = yEnd + 40;
					points = points + xStart + "," + yStart + " ";
				}
				points = points + xEnd + "," + yStart + " ";
				points = points + xEnd + "," + yEnd;
			}else if (SourceConnectionPointdirection === "Down" && TargetConnectionPointdirection === "Left"){
				var xTemp = xStart;
				var yTemp = yStart;
				xStart = xEnd;
				yStart = yEnd;
				xEnd = xTemp;
				yEnd = yTemp;
				points = xStart + "," + yStart + " ";
				xStart = xStart - 40;
				points = points + xStart + "," + yStart + " ";
				if (!(yStart > yEnd && xStart > xEnd)){
					yStart = yEnd + 40;
					points = points + xStart + "," + yStart + " ";
				}
				points = points + xEnd + "," + yStart + " ";
				points = points + xEnd + "," + yEnd;
			}

			ObjectParent.setAttribute("points", points);
			
		}else{
			//do nothing because the polyline was routed correctly last time
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
			HMI.hmi_log_trace("cshmi._checkCondition: using remembered config of "+ObjectPath+" (#"+this.ResourceList.Conditions[ObjectPath].useCount+")");
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
	_checkConditionIterator
	-	checks Condition within ChildrenIterator
*********************************/
_checkConditionIterator: function(ObjectParent, ObjectPath, ConditionPath){
	//get Values
	var comptype;
	var childValue;
	//if the Object is scanned earlier, get the cached information (could be the case with templates or repeated/cyclic calls to the same object)
	if (!(this.ResourceList.Conditions && this.ResourceList.Conditions[ObjectPath] !== undefined)){
		//fixme requests zusammenfassen!
		comptype = HMI.KSClient.getVar(null, '{'+ObjectPath+'.comptype}', null);
		childValue = HMI.KSClient.getVar(null, '{'+ObjectPath+'.childValue}', null);
		childValue = HMI.KSClient.splitKsResponse(childValue, 0);
		
		//we have asked the object successful, so remember the result
		this.ResourceList.Conditions[ObjectPath] = new Object();
		this.ResourceList.Conditions[ObjectPath].checkConditionIteratorCompType = comptype;
		this.ResourceList.Conditions[ObjectPath].checkConditionIteratorChildValue = childValue;
		this.ResourceList.Conditions[ObjectPath].useCount = 1;
		HMI.hmi_log_trace("cshmi._checkConditionIterator: remembering config of "+ObjectPath+" ");
	}else{
		//the object is asked this session, so reuse the config to save communication requests
		comptype = this.ResourceList.Conditions[ObjectPath].checkConditionIteratorCompType;
		childValue = this.ResourceList.Conditions[ObjectPath].checkConditionIteratorChildValue;
		this.ResourceList.Conditions[ObjectPath].useCount++;
		HMI.hmi_log_trace("cshmi._checkConditionIterator: using remembered config of "+ObjectPath+" (#"+this.ResourceList.Conditions[ObjectPath].useCount+")");
	}

	var Value1;
	//check if we want to get a Value from the iteratedChild
	if (childValue[0].indexOf(".") !== -1){
		var rootObject = ObjectParent;
		var FBRef;
		//search FBReference of root Object
		while (rootObject !== null){
			//FBReference found
			if(rootObject.FBReference && rootObject.FBReference["default"] !== undefined){
				FBRef = rootObject.FBReference["default"];
				//FBRef found, we can stop search
				rootObject = null;
			}
			else {
				//loop upwards to find the Template object
				rootObject = rootObject.parentNode;
			}
		}
		var splittedValue = childValue[0].split(".");
		Value1 = HMI.KSClient.getVar(null, '{'+ FBRef+"/"+this.ResourceList.ChildrenIterator.currentChild[splittedValue[0]]+"."+splittedValue[1] + '}', null);
		Value1 = Value1.replace(/{/g, "");
		Value1 = Value1.replace(/}/g, "");

		
	}else{
		Value1 = this.ResourceList.ChildrenIterator.currentChild[childValue];
	}
	var Value2 = this._getValue(ObjectParent, ObjectPath+".withValue");
	
	if (Value1 === null){
		HMI.hmi_log_info("cshmi._checkCondition on "+ObjectPath+" (baseobject: "+ObjectPath+") failed because Value1 is null.");
		return null;
	}
	if (Value2 === null){
		HMI.hmi_log_info("cshmi._checkCondition on "+ObjectPath+" (baseobject: "+ObjectPath+") failed because Value2 is null.");
		return null;
	}
	
	Value2 = HMI.KSClient.splitKsResponse(Value2, 0);


	
	if (comptype === "{<}"){
		for (var i=0; i<Value2.length; i++){
			if (!(Value1 < Value2[i])){
				return false;
			}
		}
		return true;
	}else if (comptype === "{<=}"){
		for (var i=0; i<Value2.length; i++){
			if (!(Value1 <= Value2[i])){
				return false;
			}
		}
		return true;
	}else if (comptype === "{==}"){
		//check if one entry of Value2 == Value1
		for (var i=0; i<Value2.length; i++){
			if (Value1 === Value2[i]){
				return true;
			}
		}
		return false;
	}else if (comptype === "{!=}"){
		for (var i=0; i<Value2.length; i++){
			if (!(Value1 !== Value2[i])){
				return false;
			}
		}
		return true;
	}else if (comptype === "{>=}"){
		for (var i=0; i<Value2.length; i++){
			if (!(Value1 >= Value2[i])){
				return false;
			}
		}
		return true;
	}else if (comptype === "{>}"){
		for (var i=0; i<Value2.length; i++){
			if (!(Value1 > Value2[i])){
				return false;
			}
		}
		return true;
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
		var requestList;
		requestList = new Object();
		requestList[ObjectPath] = new Object();
		requestList[ObjectPath]["x"] = null;
		requestList[ObjectPath]["y"] = null;
		requestList[ObjectPath]["width"] = null;
		requestList[ObjectPath]["height"] = null;
		
		var successCode = this._executeVariablesArray(requestList);
		if (successCode == false){
			return null;
		}
		
		var svgElement = HMI.svgDocument.createElementNS(HMI.HMI_Constants.NAMESPACE_SVG, 'svg');
		svgElement.id = ObjectPath;
		
		this._addClass(svgElement, this.cshmiGroupClass);
		this._addClass(svgElement, this.cshmiComponentClass);
		
		//set dimension of container
		svgElement.setAttribute("x", requestList[ObjectPath]["x"]);
		svgElement.setAttribute("y", requestList[ObjectPath]["y"]);
		svgElement.setAttribute("width", requestList[ObjectPath]["width"]);
		svgElement.setAttribute("height", requestList[ObjectPath]["height"]);
		
		return svgElement;
	},
	/*********************************
		_buildFromTemplate
	*********************************/
	_buildFromTemplate: function(ObjectParent, ObjectPath, calledFromInstantiateTemplate){
		var requestList;
		//if the Object is scanned earlier, get the cached information (could be the case with templates or repeated/cyclic calls to the same object)
		if (!(this.ResourceList.Elements && this.ResourceList.Elements[ObjectPath] !== undefined)){
			requestList = new Object();
			requestList[ObjectPath] = new Object();
			requestList[ObjectPath]["visible"] = null;
			requestList[ObjectPath]["stroke"] = null;
			requestList[ObjectPath]["fill"] = null;
			requestList[ObjectPath]["opacity"] = null;
			requestList[ObjectPath]["rotate"] = null;
			requestList[ObjectPath]["x"] = null;
			requestList[ObjectPath]["y"] = null;
			if (calledFromInstantiateTemplate){
				requestList[ObjectPath]["xOffset"] = null;
				requestList[ObjectPath]["yOffset"] = null;
			}
			requestList[ObjectPath]["TemplateDefinition"] = null;
			requestList[ObjectPath]["FBReference"] = null;
			requestList[ObjectPath]["ConfigValues"] = null;
			
			var successCode = this._executeVariablesArray(requestList);
			if (successCode == false){
				return null;
			}
			
			//we have asked the object successful, so remember the result
			this.ResourceList.Elements[ObjectPath] = new Object();
			this.ResourceList.Elements[ObjectPath].ElementParameters = requestList[ObjectPath];
			this.ResourceList.Elements[ObjectPath].useCount = 1;
			HMI.hmi_log_trace("cshmi._buildFromTemplate: remembering config of "+ObjectPath+" ");
		}else{
			//the object is asked this session, so reuse the config to save communication requests
			requestList = new Object();
			requestList[ObjectPath] = this.ResourceList.Elements[ObjectPath].ElementParameters;
			this.ResourceList.Elements[ObjectPath].useCount++;
			HMI.hmi_log_trace("cshmi._buildFromTemplate: using remembered config of "+ObjectPath+" (#"+this.ResourceList.Elements[ObjectPath].useCount+")");
		}
		
		var TemplateLocation = "/TechUnits/cshmi/Templates/";
		if (requestList[ObjectPath]["TemplateDefinition"] === ""){
			HMI.hmi_log_info_onwebsite("Template "+ObjectPath+" is not configured");
			return null;
		}else if (ObjectPath.indexOf(PathOfTemplateDefinition) === 0){
			HMI.hmi_log_info_onwebsite("Template "+ObjectPath+" is calling itself");
			return null;
		}
		
		var requestListTemplate;
		var PathOfTemplateDefinition = TemplateLocation+requestList[ObjectPath]["TemplateDefinition"];
		//if the Object is scanned earlier, get the cached information (could be the case with templates or repeated/cyclic calls to the same object)
		if (!(this.ResourceList.Elements && this.ResourceList.Elements[PathOfTemplateDefinition] !== undefined)){
			requestListTemplate = new Object();
			requestListTemplate[PathOfTemplateDefinition] = new Object();
			requestListTemplate[PathOfTemplateDefinition]["width"] = null;
			requestListTemplate[PathOfTemplateDefinition]["height"] = null;
			requestListTemplate[PathOfTemplateDefinition]["hideable"] = null;
			
			successCode = this._executeVariablesArray(requestListTemplate);
			if (successCode == false){
				HMI.hmi_log_info_onwebsite("Template "+ObjectPath+" is wrong configured. TemplateDefinition '"+requestList[ObjectPath]["TemplateDefinition"]+"' is not available.");
				return null;
			}
			
			//we have asked the object successful, so remember the result
			this.ResourceList.Elements[PathOfTemplateDefinition] = new Object();
			this.ResourceList.Elements[PathOfTemplateDefinition].TemplateParameters = requestListTemplate;
			this.ResourceList.Elements[PathOfTemplateDefinition].useCount = 1;
			HMI.hmi_log_trace("cshmi._buildFromTemplate: remembering config of "+ObjectPath+" ");
		}else{
			//the object is asked this session, so reuse the config to save communication requests
			requestListTemplate = this.ResourceList.Elements[PathOfTemplateDefinition].TemplateParameters;
			this.ResourceList.Elements[PathOfTemplateDefinition].useCount++;
			HMI.hmi_log_trace("cshmi._buildFromTemplate: using remembered config of "+PathOfTemplateDefinition+" (#"+this.ResourceList.Elements[PathOfTemplateDefinition].useCount+")");
		}
		
		var svgElement = HMI.svgDocument.createElementNS(HMI.HMI_Constants.NAMESPACE_SVG, 'svg');
		svgElement.id = ObjectPath;
		svgElement.setAttribute("TemplateDescription", PathOfTemplateDefinition);
		
		this._addClass(svgElement, this.cshmiTemplateClass);
		this._addClass(svgElement, this.cshmiComponentClass);
		if (requestListTemplate[PathOfTemplateDefinition]["hideable"] === "TRUE"){
			this._addClass(svgElement, this.cshmiTemplateHideableClass);
		}
		
		//parametrise templateDefinition with the config
		svgElement.FBReference = Object();
		svgElement.ConfigValues = Object();
		var realFBobjectID = null; 
		var ConfigEntry = null;
		var ConfigList = requestList[ObjectPath]["FBReference"].split(" ");
		for (var i=0; i < ConfigList.length; i++) {
			ConfigEntry = ConfigList[i].split(":");
			if (ConfigList[i].charAt(0) === "/"){
				//entry could be an fullpath with an port, so ":" is no separator
				svgElement.FBReference["default"] = ConfigList[i];
				realFBobjectID = svgElement.FBReference["default"];
			}else
			if (ConfigEntry.length === 2){
				//check if we want to get values from the current child (e.g. OP_NAME)
				//if instantiateTemplate is not called within a childreniterator, the currentChild is undefined
				if (calledFromInstantiateTemplate && this.ResourceList.ChildrenIterator.currentChild[ConfigEntry[1]] !== undefined){
					var rootObject = ObjectParent;
					var FBRef;
					//search FBReference of root Object
					while (rootObject !== null){
						//FBReference found
						if(rootObject.FBReference && rootObject.FBReference["default"] !== undefined){
							FBRef = rootObject.FBReference["default"];
							//FBRef found, we can stop search
							rootObject = null;
						}
						else {
							//loop upwards to find the Template object
							rootObject = rootObject.parentNode;
						}
					}
					svgElement.FBReference[ConfigEntry[0]] = FBRef + "." + this.ResourceList.ChildrenIterator.currentChild[ConfigEntry[1]];
					realFBobjectID = svgElement.FBReference[ConfigEntry[0]];
				}
				
				else{
					svgElement.FBReference[ConfigEntry[0]] = ConfigEntry[1];
					realFBobjectID = ConfigEntry[1];
				}
			}
			else if (ConfigEntry.length === 1 && ConfigEntry[0] != ""){
				if (calledFromInstantiateTemplate && this.ResourceList.ChildrenIterator.currentChild[ConfigEntry[0]] !== undefined){
					var rootObject = ObjectParent;
					var FBRef;
					//search FBReference of root Object
					while (rootObject !== null){
						//FBReference found
						if(rootObject.FBReference && rootObject.FBReference["default"] !== undefined){
							FBRef = rootObject.FBReference["default"];
							//FBRef found, we can stop search
							rootObject = null;
						}
						else {
							//loop upwards to find the Template object
							rootObject = rootObject.parentNode;
						}
					}


					if (this.ResourceList.ChildrenIterator.currentChild[ConfigEntry[0]].charAt(0) === "/"){
						//String begins with / so it is a fullpath
						// if FBref beginn with "//" then keep the server information
						// e.g "//dev/ov_hmidemo7/TechUnits/Add" --> keep "//dev/ov_hmidemo7"
						if (FBRef.charAt(0) === "/" && FBRef.charAt(1) === "/"){
							//find the 3rd "/"
							var slashIndex = FBRef.indexOf("/", 2);
							//find the 4th "/"
							slashIndex = FBRef.indexOf("/", slashIndex+1);
							//only keep the String before 4th "/"
							FBRef = FBRef.slice(0, slashIndex);
							
							svgElement.FBReference["default"] = FBRef + this.ResourceList.ChildrenIterator.currentChild[ConfigEntry[0]];
						}
						// FBRef not needed because it's not a path to another server
						else{
							svgElement.FBReference["default"] = this.ResourceList.ChildrenIterator.currentChild[ConfigEntry[0]];
						}
						
					}else{
						//a normal relativ path
						svgElement.FBReference["default"] = FBRef + "/" + this.ResourceList.ChildrenIterator.currentChild[ConfigEntry[0]];
					}
				}
				else{
					svgElement.FBReference["default"] = ConfigEntry[0];
				}
				realFBobjectID = svgElement.FBReference["default"];
			}
		}
		if (realFBobjectID !== null){
			svgElement.id = realFBobjectID;
		}
		
		var xTemplate = requestList[ObjectPath]["x"];
		var yTemplate = requestList[ObjectPath]["y"];
		
		if (calledFromInstantiateTemplate){
			var offsetCount = this.ResourceList.InstantiateTemplate[ObjectPath].useCount;
			var x = parseFloat(requestList[ObjectPath]["x"]) + (offsetCount * parseFloat(requestList[ObjectPath]["xOffset"]));
			requestList[ObjectPath]["x"] = x.toString();
			var y = parseFloat(requestList[ObjectPath]["y"]) + (offsetCount * parseFloat(requestList[ObjectPath]["yOffset"]));
			requestList[ObjectPath]["y"] = y.toString();
		}
		
		//setting the basic Element Variables like .visible .stroke .fill .opacity .rotate
		this._processBasicVariables(svgElement, requestList[ObjectPath]);
		if (requestList[ObjectPath]["rotate"] !== "0"){
			//rotate is not specified with a svg-Element, so encapsule in a G-Element
			//http://www.w3.org/Graphics/SVG/WG/track/issues/2252
			var svgGElement = HMI.svgDocument.createElementNS(HMI.HMI_Constants.NAMESPACE_SVG, 'g');
			svgGElement.style.overflow = "visible";
			svgGElement.setAttribute("transform", "rotate("+requestList[ObjectPath]["rotate"]+","+requestList[ObjectPath]["x"]+","+requestList[ObjectPath]["y"]+")");
			svgGElement.appendChild(svgElement);
		}
		
		requestList[ObjectPath]["x"] = xTemplate;
		requestList[ObjectPath]["y"] = yTemplate;
		
		//width and height comes from the TemplateDefinition
		svgElement.setAttribute("width", requestListTemplate[PathOfTemplateDefinition]["width"]);
		svgElement.setAttribute("height", requestListTemplate[PathOfTemplateDefinition]["height"]);
		svgElement.style.overflow = "visible";
		
		//ConfigValue is something like "pumpcolor:yellow pumpname:N18"
		//problem: "pumpcolor:yellow pumpname:N 18"
		//or "pumpcolor:yel low pumpname:N 18"
		ConfigList = requestList[ObjectPath]["ConfigValues"].split(" ");
		var lastEntry = null;
		for (var i=0; i < ConfigList.length; i++) {
			ConfigEntry = ConfigList[i].split(":");
			if (ConfigEntry.length === 2){
				//check if we want to get values from the current child (e.g. OP_NAME)
				//if instantiateTemplate is not called within a childreniterator, the currentChild is undefined
				if (calledFromInstantiateTemplate && this.ResourceList.ChildrenIterator.currentChild[ConfigEntry[1]] !== undefined){
					svgElement.ConfigValues[ConfigEntry[0]] = this.ResourceList.ChildrenIterator.currentChild[ConfigEntry[1]];
				}
				else{
					svgElement.ConfigValues[ConfigEntry[0]] = ConfigEntry[1];
				}
				if(ConfigEntry[0] === "Name" && realFBobjectID === null){
					svgElement.id = ConfigEntry[1];
				}
				lastEntry = ConfigEntry[0];
			}else if (ConfigEntry.length === 1 && lastEntry !== null){
				//we had something like "pumpcolor:yellow pumpname:N 18", so need to add the " 18" to the last entry
				svgElement.ConfigValues[lastEntry] = svgElement.ConfigValues[lastEntry]+" "+ConfigEntry[0];
			}
		}
		
		//get childs (grafics and actions) from the TemplateDefinition
		//our child will be fetched later
		var responseArrayChild = HMI.KSClient.getChildObjArray(PathOfTemplateDefinition, this);
		for (var i=0; i < responseArrayChild.length; i++) {
			var varName = responseArrayChild[i].split(" ");
			var ChildComponent = this.BuildDomain(svgElement, PathOfTemplateDefinition+"/"+varName[0], varName[1]);
			if (ChildComponent !== null){
				svgElement.appendChild(ChildComponent);
			}
		}
		var preserveThis = this;	//grabbed from http://jsbin.com/etise/7/edit
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
		
		if (svgGElement){
			//transformed Template
			return svgGElement;
		}else{
			return svgElement;
		}
	},
	_buildSvgLine: function(ObjectParent, ObjectPath){
		var requestList;
		//if the Object is scanned earlier, get the cached information (could be the case with templates or repeated/cyclic calls to the same object)
		if (!(this.ResourceList.Elements && this.ResourceList.Elements[ObjectPath] !== undefined)){
			requestList = new Object();
			requestList[ObjectPath] = new Object();
			requestList[ObjectPath]["visible"] = null;
			requestList[ObjectPath]["stroke"] = null;
			requestList[ObjectPath]["fill"] = null;
			requestList[ObjectPath]["opacity"] = null;
			requestList[ObjectPath]["rotate"] = null;
			requestList[ObjectPath]["x1"] = null;
			requestList[ObjectPath]["y1"] = null;
			requestList[ObjectPath]["x2"] = null;
			requestList[ObjectPath]["y2"] = null;
			
			var successCode = this._executeVariablesArray(requestList);
			if (successCode == false){
				return null;
			}
			
			//we have asked the object successful, so remember the result
			this.ResourceList.Elements[ObjectPath] = new Object();
			this.ResourceList.Elements[ObjectPath].ElementParameters = requestList[ObjectPath];
			this.ResourceList.Elements[ObjectPath].useCount = 1;
			HMI.hmi_log_trace("cshmi._buildSvgLine: remembering config of "+ObjectPath+" ");
		}else{
			//the object is asked this session, so reuse the config to save communication requests
			requestList = new Object();
			requestList[ObjectPath] = this.ResourceList.Elements[ObjectPath].ElementParameters;
			this.ResourceList.Elements[ObjectPath].useCount++;
			HMI.hmi_log_trace("cshmi._buildSvgLine: using remembered config of "+ObjectPath+" (#"+this.ResourceList.Elements[ObjectPath].useCount+")");
		}
		
		var svgElement = HMI.svgDocument.createElementNS(HMI.HMI_Constants.NAMESPACE_SVG, 'line');
		svgElement.id = ObjectPath;
		
		//setting the basic Element Variables like .visible .stroke .fill .opacity .rotate
		this._processBasicVariables(svgElement, requestList[ObjectPath]);
		
		svgElement.setAttribute("x1", requestList[ObjectPath]["x1"]);
		svgElement.setAttribute("y1", requestList[ObjectPath]["y1"]);
		svgElement.setAttribute("x2", requestList[ObjectPath]["x2"]);
		svgElement.setAttribute("y2", requestList[ObjectPath]["y2"]);
		
		svgElement.setAttribute("shape-rendering", "crispEdges");
		
		return svgElement;
	},
	_buildSvgPolyline: function(ObjectParent, ObjectPath){
		var requestList;
		//if the Object is scanned earlier, get the cached information (could be the case with templates or repeated/cyclic calls to the same object)
		if (!(this.ResourceList.Elements && this.ResourceList.Elements[ObjectPath] !== undefined)){
			requestList = new Object();
			requestList[ObjectPath] = new Object();
			requestList[ObjectPath]["visible"] = null;
			requestList[ObjectPath]["stroke"] = null;
			requestList[ObjectPath]["fill"] = null;
			requestList[ObjectPath]["opacity"] = null;
			requestList[ObjectPath]["rotate"] = null;
			requestList[ObjectPath]["points"] = null;
			
			var successCode = this._executeVariablesArray(requestList);
			if (successCode == false){
				return null;
			}
			
			//we have asked the object successful, so remember the result
			this.ResourceList.Elements[ObjectPath] = new Object();
			this.ResourceList.Elements[ObjectPath].ElementParameters = requestList[ObjectPath];
			this.ResourceList.Elements[ObjectPath].useCount = 1;
			HMI.hmi_log_trace("cshmi._buildSvgPolyline: remembering config of "+ObjectPath+" ");
		}else{
			//the object is asked this session, so reuse the config to save communication requests
			requestList = new Object();
			requestList[ObjectPath] = this.ResourceList.Elements[ObjectPath].ElementParameters;
			this.ResourceList.Elements[ObjectPath].useCount++;
			HMI.hmi_log_trace("cshmi._buildSvgPolyline: using remembered config of "+ObjectPath+" (#"+this.ResourceList.Elements[ObjectPath].useCount+")");
		}
		
		var svgElement = HMI.svgDocument.createElementNS(HMI.HMI_Constants.NAMESPACE_SVG, 'polyline');
		svgElement.id = ObjectPath;
		
		//setting the basic Element Variables like .visible .stroke .fill .opacity .rotate
		this._processBasicVariables(svgElement, requestList[ObjectPath]);
		
		svgElement.setAttribute("points", requestList[ObjectPath]["points"]);
		
		svgElement.setAttribute("shape-rendering", "crispEdges");

		return svgElement;
	},
	_buildSvgPolygon: function(ObjectParent, ObjectPath){
		var requestList;
		//if the Object is scanned earlier, get the cached information (could be the case with templates or repeated/cyclic calls to the same object)
		if (!(this.ResourceList.Elements && this.ResourceList.Elements[ObjectPath] !== undefined)){
			requestList = new Object();
			requestList[ObjectPath] = new Object();
			requestList[ObjectPath]["visible"] = null;
			requestList[ObjectPath]["stroke"] = null;
			requestList[ObjectPath]["fill"] = null;
			requestList[ObjectPath]["opacity"] = null;
			requestList[ObjectPath]["rotate"] = null;
			requestList[ObjectPath]["points"] = null;
			
			var successCode = this._executeVariablesArray(requestList);
			if (successCode == false){
				return null;
			}
			
			//we have asked the object successful, so remember the result
			this.ResourceList.Elements[ObjectPath] = new Object();
			this.ResourceList.Elements[ObjectPath].ElementParameters = requestList[ObjectPath];
			this.ResourceList.Elements[ObjectPath].useCount = 1;
			HMI.hmi_log_trace("cshmi._buildSvgPolygon: remembering config of "+ObjectPath+" ");
		}else{
			//the object is asked this session, so reuse the config to save communication requests
			requestList = new Object();
			requestList[ObjectPath] = this.ResourceList.Elements[ObjectPath].ElementParameters;
			this.ResourceList.Elements[ObjectPath].useCount++;
			HMI.hmi_log_trace("cshmi._buildSvgPolygon: using remembered config of "+ObjectPath+" (#"+this.ResourceList.Elements[ObjectPath].useCount+")");
		}
		
		var svgElement = HMI.svgDocument.createElementNS(HMI.HMI_Constants.NAMESPACE_SVG, 'polygon');
		svgElement.id = ObjectPath;
		
		//setting the basic Element Variables like .visible .stroke .fill .opacity .rotate
		this._processBasicVariables(svgElement, requestList[ObjectPath]);
		
		svgElement.setAttribute("points", requestList[ObjectPath]["points"]);
		
		svgElement.setAttribute("shape-rendering", "crispEdges");
		
		return svgElement;
	},
	_buildSvgPath: function(ObjectParent, ObjectPath){
		var requestList;
		//if the Object is scanned earlier, get the cached information (could be the case with templates or repeated/cyclic calls to the same object)
		if (!(this.ResourceList.Elements && this.ResourceList.Elements[ObjectPath] !== undefined)){
			requestList = new Object();
			requestList[ObjectPath] = new Object();
			requestList[ObjectPath]["visible"] = null;
			requestList[ObjectPath]["stroke"] = null;
			requestList[ObjectPath]["fill"] = null;
			requestList[ObjectPath]["opacity"] = null;
			requestList[ObjectPath]["rotate"] = null;
			requestList[ObjectPath]["d"] = null;
			
			var successCode = this._executeVariablesArray(requestList);
			if (successCode == false){
				return null;
			}
			
			//we have asked the object successful, so remember the result
			this.ResourceList.Elements[ObjectPath] = new Object();
			this.ResourceList.Elements[ObjectPath].ElementParameters = requestList[ObjectPath];
			this.ResourceList.Elements[ObjectPath].useCount = 1;
			HMI.hmi_log_trace("cshmi._buildSvgPath: remembering config of "+ObjectPath+" ");
		}else{
			//the object is asked this session, so reuse the config to save communication requests
			requestList = new Object();
			requestList[ObjectPath] = this.ResourceList.Elements[ObjectPath].ElementParameters;
			this.ResourceList.Elements[ObjectPath].useCount++;
			HMI.hmi_log_trace("cshmi._buildSvgPath: using remembered config of "+ObjectPath+" (#"+this.ResourceList.Elements[ObjectPath].useCount+")");
		}
		
		var svgElement = HMI.svgDocument.createElementNS(HMI.HMI_Constants.NAMESPACE_SVG, 'path');
		svgElement.id = ObjectPath;
		
		//setting the basic Element Variables like .visible .stroke .fill .opacity .rotate
		this._processBasicVariables(svgElement, requestList[ObjectPath]);
		
		svgElement.setAttribute("d", requestList[ObjectPath]["d"]);
		
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
		var requestList;
		//if the Object is scanned earlier, get the cached information (could be the case with templates or repeated/cyclic calls to the same object)
		if (!(this.ResourceList.Elements && this.ResourceList.Elements[ObjectPath] !== undefined)){
			requestList = new Object();
			requestList[ObjectPath] = new Object();
			requestList[ObjectPath]["visible"] = null;
			requestList[ObjectPath]["stroke"] = null;
			requestList[ObjectPath]["fill"] = null;
			requestList[ObjectPath]["opacity"] = null;
			requestList[ObjectPath]["rotate"] = null;
			requestList[ObjectPath]["x"] = null;
			requestList[ObjectPath]["y"] = null;
			requestList[ObjectPath]["fontSize"] = null;
			requestList[ObjectPath]["fontStyle"] = null;
			requestList[ObjectPath]["fontWeight"] = null;
			requestList[ObjectPath]["fontFamily"] = null;
			requestList[ObjectPath]["horAlignment"] = null;
			requestList[ObjectPath]["verAlignment"] = null;
			requestList[ObjectPath]["content"] = null;
			requestList[ObjectPath]["trimToLength"] = null;
			
			var successCode = this._executeVariablesArray(requestList);
			if (successCode == false){
				return null;
			}
			
			//we have asked the object successful, so remember the result
			this.ResourceList.Elements[ObjectPath] = new Object();
			this.ResourceList.Elements[ObjectPath].ElementParameters = requestList[ObjectPath];
			this.ResourceList.Elements[ObjectPath].useCount = 1;
			HMI.hmi_log_trace("cshmi._buildSvgText: remembering config of "+ObjectPath+" ");
		}else{
			//the object is asked this session, so reuse the config to save communication requests
			requestList = new Object();
			requestList[ObjectPath] = this.ResourceList.Elements[ObjectPath].ElementParameters;
			this.ResourceList.Elements[ObjectPath].useCount++;
			HMI.hmi_log_trace("cshmi._buildSvgText: using remembered config of "+ObjectPath+" (#"+this.ResourceList.Elements[ObjectPath].useCount+")");
		}
		
		var svgElement = HMI.svgDocument.createElementNS(HMI.HMI_Constants.NAMESPACE_SVG, 'text');
		svgElement.id = ObjectPath;
		
		//setting the basic Element Variables like .visible .stroke .fill .opacity .rotate
		this._processBasicVariables(svgElement, requestList[ObjectPath]);
		svgElement.setAttribute("font-size", requestList[ObjectPath]["fontSize"]);
		svgElement.setAttribute("font-style", requestList[ObjectPath]["fontStyle"]);
		svgElement.setAttribute("font-weight", requestList[ObjectPath]["fontWeight"]);
		svgElement.setAttribute("font-family", requestList[ObjectPath]["fontFamily"]);
		svgElement.setAttribute("text-anchor", requestList[ObjectPath]["horAlignment"]);
		
		var svgTspan = HMI.svgDocument.createElementNS(HMI.HMI_Constants.NAMESPACE_SVG, 'tspan');
		//perform trimming if needed
		var trimLength = parseInt(requestList[ObjectPath]["trimToLength"], 10);
		var contentLength = parseInt(requestList[ObjectPath]["content"].length, 10);
		var trimmedContent;
		
		if((trimLength > 0) && (contentLength > trimLength)){
			trimmedContent = requestList[ObjectPath]["content"].substr(0, trimLength) + String.fromCharCode(8230);
			svgTspan.appendChild(HMI.svgDocument.createTextNode(trimmedContent));
			this._setTitle(svgElement, requestList[ObjectPath]["content"]);
		}else if((trimLength < 0) && (contentLength > -trimLength)){
			trimmedContent =  String.fromCharCode(8230) + requestList[ObjectPath]["content"].substr(contentLength + trimLength);
			svgTspan.appendChild(HMI.svgDocument.createTextNode(trimmedContent));
			this._setTitle(svgElement, requestList[ObjectPath]["content"]);
		}else{
			svgTspan.appendChild(HMI.svgDocument.createTextNode(requestList[ObjectPath]["content"]));
		}
				
		if (requestList[ObjectPath]["verAlignment"] == "auto"){
		}else if (requestList[ObjectPath]["verAlignment"] == "middle"){
			svgTspan.setAttribute("dy", "0.7ex");
		}else if (requestList[ObjectPath]["verAlignment"] == "hanging"){
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
		var requestList;
		//if the Object is scanned earlier, get the cached information (could be the case with templates or repeated/cyclic calls to the same object)
		if (!(this.ResourceList.Elements && this.ResourceList.Elements[ObjectPath] !== undefined)){
			requestList = new Object();
			requestList[ObjectPath] = new Object();
			requestList[ObjectPath]["visible"] = null;
			requestList[ObjectPath]["stroke"] = null;
			requestList[ObjectPath]["fill"] = null;
			requestList[ObjectPath]["opacity"] = null;
			requestList[ObjectPath]["rotate"] = null;
			requestList[ObjectPath]["cx"] = null;
			requestList[ObjectPath]["cy"] = null;
			requestList[ObjectPath]["r"] = null;
			
			var successCode = this._executeVariablesArray(requestList);
			if (successCode == false){
				return null;
			}
			
			//we have asked the object successful, so remember the result
			this.ResourceList.Elements[ObjectPath] = new Object();
			this.ResourceList.Elements[ObjectPath].ElementParameters = requestList[ObjectPath];
			this.ResourceList.Elements[ObjectPath].useCount = 1;
			HMI.hmi_log_trace("cshmi._buildSvgCircle: remembering config of "+ObjectPath+" ");
		}else{
			//the object is asked this session, so reuse the config to save communication requests
			requestList = new Object();
			requestList[ObjectPath] = this.ResourceList.Elements[ObjectPath].ElementParameters;
			this.ResourceList.Elements[ObjectPath].useCount++;
			HMI.hmi_log_trace("cshmi._buildSvgCircle: using remembered config of "+ObjectPath+" (#"+this.ResourceList.Elements[ObjectPath].useCount+")");
		}
		
		var svgElement = HMI.svgDocument.createElementNS(HMI.HMI_Constants.NAMESPACE_SVG, 'circle');
		svgElement.id = ObjectPath;
		
		//setting the basic Element Variables like .visible .stroke .fill .opacity .rotate
		this._processBasicVariables(svgElement, requestList[ObjectPath]);
		svgElement.setAttribute("cx", requestList[ObjectPath]["cx"]);
		svgElement.setAttribute("cy", requestList[ObjectPath]["cy"]);
		svgElement.setAttribute("r",requestList[ObjectPath]["r"]);
		
		return svgElement;
	},
	_buildSvgEllipse: function(ObjectParent, ObjectPath){
		var requestList;
		//if the Object is scanned earlier, get the cached information (could be the case with templates or repeated/cyclic calls to the same object)
		if (!(this.ResourceList.Elements && this.ResourceList.Elements[ObjectPath] !== undefined)){
			requestList = new Object();
			requestList[ObjectPath] = new Object();
			requestList[ObjectPath]["visible"] = null;
			requestList[ObjectPath]["stroke"] = null;
			requestList[ObjectPath]["fill"] = null;
			requestList[ObjectPath]["opacity"] = null;
			requestList[ObjectPath]["rotate"] = null;
			requestList[ObjectPath]["cx"] = null;
			requestList[ObjectPath]["cy"] = null;
			requestList[ObjectPath]["rx"] = null;
			requestList[ObjectPath]["ry"] = null;
			
			var successCode = this._executeVariablesArray(requestList);
			if (successCode == false){
				return null;
			}
			
			//we have asked the object successful, so remember the result
			this.ResourceList.Elements[ObjectPath] = new Object();
			this.ResourceList.Elements[ObjectPath].ElementParameters = requestList[ObjectPath];
			this.ResourceList.Elements[ObjectPath].useCount = 1;
			HMI.hmi_log_trace("cshmi._buildSvgEllipse: remembering config of "+ObjectPath+" ");
		}else{
			//the object is asked this session, so reuse the config to save communication requests
			requestList = new Object();
			requestList[ObjectPath] = this.ResourceList.Elements[ObjectPath].ElementParameters;
			this.ResourceList.Elements[ObjectPath].useCount++;
			HMI.hmi_log_trace("cshmi._buildSvgEllipse: using remembered config of "+ObjectPath+" (#"+this.ResourceList.Elements[ObjectPath].useCount+")");
		}
		
		var svgElement = HMI.svgDocument.createElementNS(HMI.HMI_Constants.NAMESPACE_SVG, 'ellipse');
		svgElement.id = ObjectPath;
		
		//setting the basic Element Variables like .visible .stroke .fill .opacity .rotate
		this._processBasicVariables(svgElement, requestList[ObjectPath]);
		svgElement.setAttribute("cx", requestList[ObjectPath]["cx"]);
		svgElement.setAttribute("cy", requestList[ObjectPath]["cy"]);
		svgElement.setAttribute("rx",requestList[ObjectPath]["rx"]);
		svgElement.setAttribute("ry",requestList[ObjectPath]["ry"]);
		
		//rotation should be around cx and cy
		svgElement.setAttribute("transform", "rotate("+requestList[ObjectPath]["rotate"]+","+requestList[ObjectPath]["cx"]+","+requestList[ObjectPath]["cy"]+")");
		
		return svgElement;
	},
	_buildSvgRect: function(ObjectParent, ObjectPath){
		var requestList;
		//if the Object is scanned earlier, get the cached information (could be the case with templates or repeated/cyclic calls to the same object)
		if (!(this.ResourceList.Elements && this.ResourceList.Elements[ObjectPath] !== undefined)){
			requestList = new Object();
			requestList[ObjectPath] = new Object();
			requestList[ObjectPath]["visible"] = null;
			requestList[ObjectPath]["stroke"] = null;
			requestList[ObjectPath]["fill"] = null;
			requestList[ObjectPath]["opacity"] = null;
			requestList[ObjectPath]["rotate"] = null;
			requestList[ObjectPath]["x"] = null;
			requestList[ObjectPath]["y"] = null;
			requestList[ObjectPath]["width"] = null;
			requestList[ObjectPath]["height"] = null;
			
			var successCode = this._executeVariablesArray(requestList);
			if (successCode == false){
				return null;
			}
			
			//we have asked the object successful, so remember the result
			this.ResourceList.Elements[ObjectPath] = new Object();
			this.ResourceList.Elements[ObjectPath].ElementParameters = requestList[ObjectPath];
			this.ResourceList.Elements[ObjectPath].useCount = 1;
			HMI.hmi_log_trace("cshmi._buildSvgRect: remembering config of "+ObjectPath+" ");
		}else{
			//the object is asked this session, so reuse the config to save communication requests
			requestList = new Object();
			requestList[ObjectPath] = this.ResourceList.Elements[ObjectPath].ElementParameters;
			this.ResourceList.Elements[ObjectPath].useCount++;
			HMI.hmi_log_trace("cshmi._buildSvgRect: using remembered config of "+ObjectPath+" (#"+this.ResourceList.Elements[ObjectPath].useCount+")");
		}
		
		var svgElement = HMI.svgDocument.createElementNS(HMI.HMI_Constants.NAMESPACE_SVG, 'rect');
		svgElement.id = ObjectPath;
		
		//setting the basic Element Variables like .visible .stroke .fill .opacity .rotate
		this._processBasicVariables(svgElement, requestList[ObjectPath]);
		
		svgElement.setAttribute("shape-rendering", "crispEdges");
		
		return svgElement;
	},
	_buildSvgImage: function(ObjectParent, ObjectPath){
		var requestList;
		//if the Object is scanned earlier, get the cached information (could be the case with templates or repeated/cyclic calls to the same object)
		if (!(this.ResourceList.Elements && this.ResourceList.Elements[ObjectPath] !== undefined)){
			requestList = new Object();
			requestList[ObjectPath] = new Object();
			requestList[ObjectPath]["visible"] = null;
			requestList[ObjectPath]["stroke"] = null;
			requestList[ObjectPath]["fill"] = null;
			requestList[ObjectPath]["opacity"] = null;
			requestList[ObjectPath]["rotate"] = null;
			requestList[ObjectPath]["x"] = null;
			requestList[ObjectPath]["y"] = null;
			requestList[ObjectPath]["width"] = null;
			requestList[ObjectPath]["height"] = null;
			requestList[ObjectPath]["SVGcontent"] = null;
			requestList[ObjectPath]["Bitmapcontent"] = null;
			
			var successCode = this._executeVariablesArray(requestList);
			if (successCode == false){
				return null;
			}
			
			//we have asked the object successful, so remember the result
			this.ResourceList.Elements[ObjectPath] = new Object();
			this.ResourceList.Elements[ObjectPath].ElementParameters = requestList[ObjectPath];
			this.ResourceList.Elements[ObjectPath].useCount = 1;
			HMI.hmi_log_trace("cshmi._buildSvgRect: remembering config of "+ObjectPath+" ");
		}else{
			//the object is asked this session, so reuse the config to save communication requests
			requestList = new Object();
			requestList[ObjectPath] = this.ResourceList.Elements[ObjectPath].ElementParameters;
			this.ResourceList.Elements[ObjectPath].useCount++;
			HMI.hmi_log_trace("cshmi._buildSvgRect: using remembered config of "+ObjectPath+" (#"+this.ResourceList.Elements[ObjectPath].useCount+")");
		}
		
		var svgElement;
		if(requestList[ObjectPath]["SVGcontent"] !== ""){
			//we have SVG Content to visualise
			//
			//the code could use an xlink, svg or no XML prefix at all.
			var svgContent =	"<svg:svg xmlns:svg=\""+HMI.HMI_Constants.NAMESPACE_SVG+"\" xmlns=\""+HMI.HMI_Constants.NAMESPACE_SVG+"\" "
				+"xmlns:xlink='http://www.w3.org/1999/xlink'>"
				+requestList[ObjectPath]["SVGcontent"]
				+"</svg:svg>";
			svgElement = HMI.HMIDOMParser.parse(svgContent, null);
			svgElement.style.overflow = "visible";
		}else if(requestList[ObjectPath]["Bitmapcontent"] !== ""){
			//we have an Bitmap Content to visualise
			//
			
			var bitmapContent =	"<svg:image xmlns:svg=\""+HMI.HMI_Constants.NAMESPACE_SVG+"\" "
				+"xmlns:xlink='http://www.w3.org/1999/xlink' xlink:href='"
				+requestList[ObjectPath]["Bitmapcontent"]
				+"'></svg:image>";
			svgElement = HMI.HMIDOMParser.parse(bitmapContent, null);
			
			/* better but not working approach
			var svgElement = HMI.svgDocument.createElementNS(HMI.HMI_Constants.NAMESPACE_SVG, 'image');
			
			ObjectParent.setAttribute("xmlns:xlink", HMI.HMI_Constants.NAMESPACE_XLINK);
			//this causes an error, so the image is not displayed
			//ObjectParent.setAttribute("xmlns", HMI.HMI_Constants.NAMESPACE_SVG);
			svgElement.setAttribute("xlink:href", requestList[ObjectPath]["Bitmapcontent"]);
			svgElement.setAttribute("width", requestList[ObjectPath]["width"]);
			svgElement.setAttribute("height", requestList[ObjectPath]["height"]);
			*/
		}else{
			HMI.hmi_log_info_onwebsite("Image "+ObjectPath+" is not configured");
			return null;
		}
		svgElement.id = ObjectPath;
		//setting the basic Element Variables like .visible .stroke .fill .opacity .rotate
		this._processBasicVariables(svgElement, requestList[ObjectPath]);
		
		return svgElement;
	},
	/*********************************
	_processBasicVariables
		-	sets svg attributes from an Array
	*********************************/
	_processBasicVariables: function(svgElement, configArray){
		if (configArray["visible"] && configArray["visible"] == "FALSE"){
			svgElement.setAttribute("display", "none");
		}else{
			svgElement.setAttribute("display", "block");
		}
		if(configArray["x"] && configArray["y"]){
			//the attribute should be "rotate(deg, x, y)"
			svgElement.setAttribute("transform", "rotate("+configArray["rotate"]+","+configArray["x"]+","+configArray["y"]+")");
			svgElement.setAttribute("x", configArray["x"]);
			svgElement.setAttribute("y", configArray["y"]);
		}else{
			svgElement.setAttribute("transform", "rotate("+configArray["rotate"]+")");
		}
		if(configArray["width"] && configArray["width"] !== ""){
			svgElement.setAttribute("width", configArray["width"]);
		}
		if(configArray["height"] && configArray["height"] !== ""){
			svgElement.setAttribute("height", configArray["height"]);
		}
		if(configArray["stroke"] && configArray["stroke"] !== ""){
			svgElement.setAttribute("stroke", configArray["stroke"]);
		}
		if(configArray["fill"] && configArray["fill"] !== ""){
			svgElement.setAttribute("fill", configArray["fill"]);
		}
		if(configArray["opacity"] && configArray["opacity"] !== ""){
			svgElement.setAttribute("opacity", configArray["opacity"]);
		}
	},
	/*********************************
	_executeVariablesArray
		-	requests a list of OV-Variables from multiple OV-Objects
	*********************************/
	_executeVariablesArray: function(requestList){
		var requestString = "";
		var lastOvObjName = null;
		
		//collect all requested Variables
		for (var ovObjName in requestList) {
			for (var ksVarName in requestList[ovObjName]) {
				if (lastOvObjName != ovObjName){
					//variable from a new object requested
					requestString += ovObjName+"."+ksVarName+" ";
				}else{
					//variable from the same object requested
					requestString += "."+ksVarName+" ";
				}
				lastOvObjName = ovObjName;
			}
		}
		
		var response = HMI.KSClient.getVar(null, '{'+requestString+'}', null);
		if (response === false){
			//communication error
			return false;
		}else if (response.indexOf("KS_ERR") !== -1){
			HMI.hmi_log_error("cshmi._executeVariablesArray of "+requestString+" failed: "+response);
			return false;
		}
		var responseArray = HMI.KSClient.splitKsResponse(response);
		
		//fill request object with the result
		var i = 0;
		for (var ovObjName in requestList) {
			for (var ksVarName in requestList[ovObjName]) {
				requestList[ovObjName][ksVarName] = responseArray[i];
				i++;
			}
		}
		return true;
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
	_setTitle: function(svgElement, newText){
		var titles = svgElement.getElementsByTagNameNS(HMI.HMI_Constants.NAMESPACE_SVG, 'title');
		if (titles.length >0){
			titles[0].replaceChild(HMI.svgDocument.createTextNode(newText), titles[0].firstChild);
		}else{
			var svgTitle = HMI.svgDocument.createElementNS(HMI.HMI_Constants.NAMESPACE_SVG, 'title');
			svgTitle.appendChild(HMI.svgDocument.createTextNode(newText));
			svgElement.appendChild(svgTitle);
		}
	},
	_getElementsByClassName: function(node, className){
		if (node.getElementsByClassName){
			return node.getElementsByClassName(className);
		} else {
			var testClass = new RegExp("(^|\\s)" + className + "(\\s|$)");
			node = node || HMI.svgDocument;
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
};

var filedate = "$Date$";
filedate = filedate.substring(7, filedate.length-2);
if ("undefined" == typeof HMIdate){
	HMIdate = filedate;
}else if (HMIdate < filedate){
	HMIdate = filedate;
}
