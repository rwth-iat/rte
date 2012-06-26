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

/**
 * Main cshmi constructor
 * @constructor
 */
function cshmi() {
	//every Action, Condition and Event could be requested multiple times
	//to build the visualisation. It should only transfered once,
	//so we have to remember the request client side.
	this.ResourceList = Object();
	this.ResourceList.onloadCallStack = Array();
	this.ResourceList.globalvarChangedCallStack = Array();
	
	this.ResourceList.Elements = Object();
	this.ResourceList.Actions = Object();
	this.ResourceList.Conditions = Object();
	this.ResourceList.Events = Object();
	this.ResourceList.baseKsPath = Object();
	this.ResourceList.ChildList = Object();
	this.ResourceList.ChildrenIterator = Object();
	this.ResourceList.EventInfos = Object();
	this.ResourceList.GlobalVar = Object();
	
	//holds the information if the visualisation is filled with content right now
	this.initStage = false;
	
	//we want to add all elements to a class to find it later
	this.cshmiGroupClass = "cshmi-group";
	this.cshmiTemplateClass = "cshmi-template";
	this.cshmiTemplateActionClass = "cshmi-fromTemplateAction";
	this.cshmiTemplateHideableClass = "cshmi-hideabletemplate";
	
	this.cshmiOperatorClickClass = "cshmi-click";
	this.cshmiOperatorDoubleclickClass = "cshmi-doubleclick";
	this.cshmiOperatorRightclickClass = "cshmi-rightclick";
	this.cshmiOperatorAftermoveClass = "cshmi-aftermove";
};





/*#########################################################################################################################
TODO: 

JavaScript:
- check return value of gethandleid

- hover für polylines

- Alle xmlhttprequests sollten async sein (bessere performance bei den meisten browsern)
- Nur einige wenige cycTimes (enum?) erlauben
- ks befehle konsolidieren bei zyklischer abarbeitung
#########################################################################################################################*/

/***********************************************************************
	prototype
***********************************************************************/
cshmi.prototype = {
	/**
	 * starts iteration of visualisation and displays the result
	 * @this main cshmi object
	 * @param {String} ObjectPath Path to this cshmi object containing the event/action/visualisation
	 * @return nothing
	 */
	instanciateCshmi: function(ObjectPath) {
		//we are in the init stage, so the DOM Tree is not populated
		this.initStage = true;
		
		//build the selected sheet aka group. This includes all containing elements
		var VisualObject = this.BuildDomain(null, ObjectPath, "/cshmi/Group");
		
		if(HMI.PlaygroundContainerNode){
			//the displayed size is calculated from the Container-Node in the html, so we correct the dimension of it
			HMI.PlaygroundContainerNode.setAttribute('height', VisualObject.getAttribute('height'));
			HMI.PlaygroundContainerNode.setAttribute('width', VisualObject.getAttribute('width'));
		}
		
		if (VisualObject !== null){
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
			VisualObject.appendChild(svgDefs);
			
			svgLinGrad = null;
			svgDefs = null;
			
			//push the svg VisualObject to the screen
			HMI.Playground.appendChild(VisualObject);
			
			//calculate all offset parameter to be able to display visual feedback
			//this is only possible now, as the orientation of the parents are not defined when they are not appended
			var maxPosition = HMI._setLayerPosition(VisualObject, true);
			//we want to have offset parameter on all visual elements
			var ComponentChilds = VisualObject.getElementsByTagNameNS(HMI.HMI_Constants.NAMESPACE_SVG, '*');
			for(var i = 0;i < ComponentChilds.length;i++){
				var Position = HMI._setLayerPosition(ComponentChilds[i], true);
				if (ComponentChilds[i].tagName !== "svg" && Position[0] > maxPosition[0]){
					maxPosition[0] = Position[0];
				}
				if (ComponentChilds[i].tagName !== "svg" && Position[1] > maxPosition[1]){
					maxPosition[1] = Position[1];
				}
			}
			Position = null;
			ComponentChilds = null;
			
			//interprete onload Actions in the order of occurrence
			while(this.ResourceList.onloadCallStack.length !== 0){
				var EventObjItem = this.ResourceList.onloadCallStack.shift();
				this._interpreteAction(EventObjItem["VisualObject"], EventObjItem["ObjectPath"]);
			}
			EventObjItem = null;
			
			if (maxPosition[0] > VisualObject.getAttribute('width') || maxPosition[1] > VisualObject.getAttribute('height')){
				HMI.hmi_log_info_onwebsite("Warning: There is a chance, that there is content outside your view.");
			}
			maxPosition = null;
			
			//the DOM Tree is populated now
			this.initStage = false;
		}
	},
	
	/**
	 * Main iteration loop for visualisation, finds and arms Actions as well
	 * @param {SVGElement} VisualParentObject visual Object which is parent to active Object
	 * @param {String} ObjectPath Path to this cshmi object containing the event/action/visualisation
	 * @return {SVGElement} VisualObject the new constructed element or null
	 */
	BuildDomain: function(VisualParentObject, ObjectPath, ObjectType) {
		var VisualObject = null;
		var Result = true;
		if (ObjectType.indexOf("/cshmi/Group") !== -1){
			VisualObject = this._buildSvgContainer(VisualParentObject, ObjectPath);
		}else if (ObjectType.indexOf("/cshmi/Template") !== -1){
			VisualObject = this._buildFromTemplate(VisualParentObject, ObjectPath);
		}else if (ObjectType.indexOf("/cshmi/Path") !== -1){
			VisualObject = this._buildSvgPath(VisualParentObject, ObjectPath);
		}else if (ObjectType.indexOf("/cshmi/Line") !== -1){
			VisualObject = this._buildSvgLine(VisualParentObject, ObjectPath);
		}else if (ObjectType.indexOf("/cshmi/Polyline") !== -1){
			VisualObject = this._buildSvgPolyline(VisualParentObject, ObjectPath);
		}else if (ObjectType.indexOf("/cshmi/Polygon") !== -1){
			VisualObject = this._buildSvgPolygon(VisualParentObject, ObjectPath);
		}else if (ObjectType.indexOf("/cshmi/Text") !== -1){
			VisualObject = this._buildSvgText(VisualParentObject, ObjectPath);
		}else if (ObjectType.indexOf("/cshmi/Circle") !== -1){
			VisualObject = this._buildSvgCircle(VisualParentObject, ObjectPath);
		}else if (ObjectType.indexOf("/cshmi/Ellipse") !== -1){
			VisualObject = this._buildSvgEllipse(VisualParentObject, ObjectPath);
		}else if (ObjectType.indexOf("/cshmi/Rectangle") !== -1){
			VisualObject = this._buildSvgRect(VisualParentObject, ObjectPath);
		}else if (ObjectType.indexOf("/cshmi/Image") !== -1){
			VisualObject = this._buildSvgImage(VisualParentObject, ObjectPath);
		}else if (ObjectType.indexOf("/cshmi/ClientEvent") !== -1){
			Result = this._interpreteClientEvent(VisualParentObject, ObjectPath);
		}else if (ObjectType.indexOf("/cshmi/TimeEvent") !== -1){
			Result = this._interpreteTimeEvent(VisualParentObject, ObjectPath);
		}else if (ObjectType.indexOf("/cshmi/OperatorEvent") !== -1){
			Result = this._interpreteOperatorEvent(VisualParentObject, ObjectPath);
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
		if (VisualObject === undefined || VisualObject === true || VisualObject === false){
			HMI.hmi_log_error("VisualObject "+ObjectPath+" gave a wrong returnvalue type.");
		}
		
		//get and prepare Children in an recursive call
		if (VisualObject !== null){
			var responseArray = HMI.KSClient.getChildObjArray(ObjectPath, this);
			for (var i=0; i < responseArray.length; i++) {
				var varName = responseArray[i].split(" ");
				if(VisualObject.tagName === "g" && VisualObject.id === "" && VisualObject.firstChild && VisualObject.firstChild.id !== ""){
					//nested rotation template
					var realComponent = VisualObject.firstChild;
				}else{
					realComponent = VisualObject;
				}
				var ChildComponent = this.BuildDomain(realComponent, realComponent.id+"/"+varName[0], varName[1]);
				if (ChildComponent !== null){
					realComponent.appendChild(ChildComponent);
				}
			}
		}
		
		return VisualObject;
	},
	
	/**
	 * calling Actions if supported ClientEvent is triggered
	 * @param {SVGElement} VisualObject Object to manipulate the visualisation
	 * @param {String} ObjectPath Path to this cshmi object containing the event/action/visualisation
	 * @return {Boolean} true
	 */
	_interpreteClientEvent: function(VisualObject, ObjectPath){
		var command = ObjectPath.split("/");
		if (command[command.length-1] === "onload"){
			//interprete Action later, so remember this
			var EventObjItem = Object();
			EventObjItem["VisualObject"] = VisualObject;
			EventObjItem["ObjectPath"] = ObjectPath;
			this.ResourceList.onloadCallStack.push(EventObjItem);
		}else if (command[command.length-1] === "globalvarchanged"){
			//remember Action to be called after a globalVar is changed
			var EventObjItem = Object();
			EventObjItem["VisualObject"] = VisualObject;
			EventObjItem["ObjectPath"] = ObjectPath;
			this.ResourceList.globalvarChangedCallStack.push(EventObjItem);
		}else{
			HMI.hmi_log_info_onwebsite("ClientEvent ("+command[command.length-1]+") "+ObjectPath+" not supported");
		}
		return true;
	},
	
	/**
	 * detect all OperatorEvents and register them
	 * @param {SVGElement} VisualObject Object to manipulate the visualisation
	 * @param {String} ObjectPath Path to this cshmi object containing the event/action/visualisation
	 * @return {Boolean} true
	 */
	_interpreteOperatorEvent: function(VisualObject, ObjectPath){
		var command = ObjectPath.split("/");
		if (command[command.length-1] === "click"){
			VisualObject.setAttribute("cursor", "pointer");
			HMI.addClass(VisualObject, this.cshmiOperatorClickClass);
			VisualObject.setAttribute("data-clickpath", ObjectPath);
			var preserveThis = this;	//grabbed from http://jsbin.com/etise/7/edit
			VisualObject.addEventListener("click", function(evt){
				if(HMI.instanceOf(VisualObject, preserveThis.cshmiOperatorAftermoveClass)){
					//we have an movegesture on the same VisualObject, so this will handle all action
					return;
				}
				
				preserveThis.ResourceList.EventInfos.EventObj = evt;
				//mark changed VisualObject for quick visual feedback (hidden after a second)
				HMI.displaygestureReactionMarker(VisualObject);
				
				//toggle visibility of hideable childtemplates
				preserveThis.toggleChildTemplates(VisualObject);
				
				//get and execute all actions
				preserveThis._interpreteAction(VisualObject, ObjectPath);
				
				if (evt.stopPropagation) evt.stopPropagation();
			}, false);
		}else if (command[command.length-1] === "doubleclick"){
			VisualObject.setAttribute("cursor", "pointer");
			HMI.addClass(VisualObject, this.cshmiOperatorDoubleclickClass);
			VisualObject.setAttribute("data-doubleclickpath", ObjectPath);
			var preserveThis = this;	//grabbed from http://jsbin.com/etise/7/edit
			VisualObject.addEventListener("dblclick", function(evt){
				preserveThis.ResourceList.EventInfos.EventObj = evt;
				//mark changed VisualObject for quick visual feedback (hidden after a second)
				HMI.displaygestureReactionMarker(VisualObject);
				
				//get and execute all actions
				preserveThis._interpreteAction(VisualObject, ObjectPath);
				if (evt.stopPropagation) evt.stopPropagation();
			}, false);
		}else if (command[command.length-1] === "rightclick"){
			HMI.addClass(VisualObject, this.cshmiOperatorRightclickClass);
			VisualObject.setAttribute("data-rightclickpath", ObjectPath);
			var preserveThis = this;	//grabbed from http://jsbin.com/etise/7/edit
			VisualObject.addEventListener("contextmenu", function(evt){
				preserveThis.ResourceList.EventInfos.EventObj = evt;
				//mark changed VisualObject for quick visual feedback (hidden after a second)
				HMI.displaygestureReactionMarker(VisualObject);
				
				//get and execute all actions
				preserveThis._interpreteAction(VisualObject, ObjectPath);
				if (evt.stopPropagation) evt.stopPropagation();
				if (evt.preventDefault) evt.preventDefault();  //default is a context menu, so disable it
			}, false);
		}else if (command[command.length-1] === "aftermove"){
			VisualObject.setAttribute("cursor", "move");
			HMI.addClass(VisualObject, this.cshmiOperatorAftermoveClass);
			VisualObject.setAttribute("data-aftermovepath", ObjectPath);
			
			var preserveThis = this;	//grabbed from http://jsbin.com/etise/7/edit
			
			//make an dummy wrapper function so we can use VisualObject and ObjectPath in it, 
			//addEventListener only provides the event object
			VisualObject._moveStartDragThunk = function(evt){
				preserveThis._moveStartDrag(VisualObject, ObjectPath, evt);
			}
			VisualObject._moveMouseMoveThunk = function(evt){
				preserveThis._moveMouseMove(VisualObject, ObjectPath, evt);
			}
			VisualObject._moveStopDragThunk = function(evt){
				//stop with interpreting the actions
				preserveThis._moveStopDrag(VisualObject, ObjectPath, evt, false);
			}
			VisualObject._moveCancelDragThunk = function(evt){
				//stop without interpreting the actions
				preserveThis._moveStopDrag(VisualObject, ObjectPath, evt, true);
			}
			VisualObject._moveHandleClickThunk = function(evt){
				//stop the propagation
				preserveThis._moveHandleClick(VisualObject, ObjectPath, evt, true);
			}
			
			//todo: try to implement via HTML5 drag&drop
			//todo http://blogs.msdn.com/b/ie/archive/2011/10/19/handling-multi-touch-and-mouse-input-in-all-browsers.aspx
			
			//try both, mousedown and mousetouch. mousetouch will fire first, there we will kill mousedown
			VisualObject.addEventListener("touchstart", VisualObject._moveStartDragThunk, false);
			VisualObject.addEventListener("mousedown", VisualObject._moveStartDragThunk, false);
		}else{
			HMI.hmi_log_info_onwebsite("OperatorEvent ("+command[command.length-1]+") "+ObjectPath+" not supported");
		}
		return true;
	},
	
	/**
	 * prepares the drag and drop handling via mousemove etc
	 * @param {SVGElement} VisualObject Object to manipulate the visualisation
	 * @param {String} ObjectPath Path to this cshmi object containing the event/action/visualisation
	 * @param {DOM Event} evt event object
	 * @return nothing
	 */
	_moveStartDrag : function(VisualObject, ObjectPath, evt){
		if (evt.button == 2){
			//right click
			HMI.hmi_log_trace("moveStartDrag - hit right mouse button on "+VisualObject.id);
			if (evt.stopPropagation) evt.stopPropagation();
			return;
		}
		HMI.hmi_log_trace("moveStartDrag - Start with object: "+VisualObject.id);
		this.ResourceList.EventInfos.EventObj = evt;
		//memorize Startposition
		var mouseposition = HMI.getClickPosition(evt, null);
		this.ResourceList.EventInfos.startXMouse = mouseposition[0];
		this.ResourceList.EventInfos.startYMouse = mouseposition[1];
		this.ResourceList.EventInfos.startXObj = parseInt(VisualObject.getAttribute("x"), 10);
		this.ResourceList.EventInfos.startYObj = parseInt(VisualObject.getAttribute("y"), 10);
		
		if(evt.type === 'touchstart'){
			HMI.hmi_log_trace("moveStartDrag - touch (x:"+mouseposition[0]+",y:"+mouseposition[1]+") detected, killing legacy events");
			//we have touch gestures, so kill legacy mousedown
			VisualObject.removeEventListener("mousedown", VisualObject._moveStartDragThunk, false);
			
			HMI.svgDocument.addEventListener("touchmove", VisualObject._moveMouseMoveThunk, false);
			HMI.svgDocument.addEventListener("touchend", VisualObject._moveStopDragThunk, false);
			HMI.svgDocument.addEventListener("touchcancel", VisualObject._moveCancelDragThunk, false);
		}else{
			HMI.hmi_log_trace("moveStartDrag - legacy click (x:"+mouseposition[0]+",y:"+mouseposition[1]+") detected");
			HMI.svgDocument.addEventListener("mousemove", VisualObject._moveMouseMoveThunk, false);
			HMI.svgDocument.addEventListener("mouseup", VisualObject._moveStopDragThunk, false);
		}
		VisualObject.addEventListener("click", VisualObject._moveHandleClickThunk, false);
		
		if (evt.stopPropagation) evt.stopPropagation();
	},
	
	/**
	 * stops propagation of event
	 * @param {SVGElement} VisualObject Object to manipulate the visualisation
	 * @param {String} ObjectPath Path to this cshmi object containing the event/action/visualisation
	 * @param {DOM Event} evt event object
	 * @return nothing
	 */
	_moveHandleClick : function(VisualObject, ObjectPath, evt){
		//no more grabbing of click needed
		VisualObject.removeEventListener("click", VisualObject._moveHandleClickThunk, false);
		
		//we do not want to propagate a click to the parents
		if (evt.stopPropagation) evt.stopPropagation();
	},
	
	/**
	 * handles the movement during a drag
	 * @param {SVGElement} VisualObject Object to manipulate the visualisation
	 * @param {String} ObjectPath Path to this cshmi object containing the event/action/visualisation
	 * @param {DOM Event} evt event object
	 * @return nothing
	 */
	_moveMouseMove : function(VisualObject, ObjectPath, evt){
		if (this.ResourceList.EventInfos.startXObj === null){
			return;
		}
		var mouseposition = HMI.getClickPosition(evt, null);
		var mouseX = mouseposition[0];
		var mouseY = mouseposition[1];
		var newx = this.ResourceList.EventInfos.startXObj+mouseX-this.ResourceList.EventInfos.startXMouse;
		var newy = this.ResourceList.EventInfos.startYObj+mouseY-this.ResourceList.EventInfos.startYMouse;
		if (!isNaN(newx) && !isNaN(newy)){
			VisualObject.setAttribute("x", newx);
			VisualObject.setAttribute("y", newy);
			
			HMI._setLayerPosition(VisualObject);
			//we want to have offset parameter on all visual elements
			var ComponentChilds = VisualObject.getElementsByTagNameNS(HMI.HMI_Constants.NAMESPACE_SVG, '*');
			for(var i = 0;i < ComponentChilds.length;i++){
				HMI._setLayerPosition(ComponentChilds[i]);
			}
			//save event for use in an action
			this.ResourceList.EventInfos.EventObj = evt;
		}
		if (evt.stopPropagation) evt.stopPropagation();
		if (evt.preventDefault) evt.preventDefault();  //default is scrolling, so disable it
	},
	
	/**
	 * stops the drag and call needed actions
	 * @param {SVGElement} VisualObject Object to manipulate the visualisation
	 * @param {String} ObjectPath Path to this cshmi object containing the event/action/visualisation
	 * @param {DOM Event} evt event object
	 * @return nothing
	 */
	_moveStopDrag : function(VisualObject, ObjectPath, evt, canceled){
		HMI.hmi_log_trace("moveStopDrag - Stop with object: "+VisualObject.id);
		if(evt.type === 'touchend'){
			HMI.hmi_log_trace("moveStartDrag - touch up detected");
			HMI.svgDocument.removeEventListener("touchmove", VisualObject._moveMouseMoveThunk, false);
			HMI.svgDocument.removeEventListener("touchend", VisualObject._moveStopDragThunk, false);
			HMI.svgDocument.removeEventListener("touchcancel", VisualObject._moveCancelDragThunk, false);
			//the touchend has no xy position (since the fingers left the device!), so an action should work on the last move eventobj
		}else{
			HMI.hmi_log_trace("moveStartDrag - legacy mouse up detected");
			HMI.svgDocument.removeEventListener("mousemove", VisualObject._moveMouseMoveThunk, false);
			HMI.svgDocument.removeEventListener("mouseup", VisualObject._moveStopDragThunk, false);
			//the mouseup event has xy position, so remember for use in an action
			this.ResourceList.EventInfos.EventObj = evt;
		}
		
		var mouseposition = HMI.getClickPosition(this.ResourceList.EventInfos.EventObj, null);
		if(HMI.instanceOf(VisualObject, this.cshmiOperatorClickClass)
			&& (Math.abs(mouseposition[0] - VisualObject.getAttribute("x")) < 5)
			&& (Math.abs(mouseposition[1] - VisualObject.getAttribute("y")) < 5)){
			
			//no movement detected, so interprete the click
			var interpreteEvent = "click";
		}
		
		//restore old position
		VisualObject.setAttribute("x", this.ResourceList.EventInfos.startXObj);
		VisualObject.setAttribute("y", this.ResourceList.EventInfos.startYObj);
		HMI._setLayerPosition(VisualObject);
		//we want to have offset parameter on all visual elements
		var ComponentChilds = VisualObject.getElementsByTagNameNS(HMI.HMI_Constants.NAMESPACE_SVG, '*');
		for(var i = 0;i < ComponentChilds.length;i++){
			HMI._setLayerPosition(ComponentChilds[i]);
		}
		
		if (canceled === true){
			//an action should not interprete this event
			this.ResourceList.EventInfos.EventObj = null;
		}else if (interpreteEvent === "click"){
			this._interpreteAction(VisualObject, VisualObject.getAttribute("data-clickpath"));
		}else{
			//get and execute all actions
			this._interpreteAction(VisualObject, ObjectPath);
		}
		
		if (evt.stopPropagation) evt.stopPropagation();
	},
	
	/**
	 * calling Actions if supported TimeEvent is triggered
	 * @param {SVGElement} VisualObject Object to manipulate the visualisation
	 * @param {String} ObjectPath Path to this cshmi object containing the event/action/visualisation
	 * @return {Boolean} true
	 */
	_interpreteTimeEvent: function(VisualObject, ObjectPath){
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
			EventObjItem["VisualObject"] = VisualObject;
			EventObjItem["ObjectPath"] = ObjectPath;
			this.ResourceList.onloadCallStack.push(EventObjItem);
		}else if(skipEvent === false){
			this._interpreteAction(VisualObject, ObjectPath);
		}
		
		var cyctime;
		//if the Object was scanned earlier, get the cached information (could be the case with templates or repeated/cyclic calls to the same object)
		if (!(this.ResourceList.Events && this.ResourceList.Events[ObjectPath] !== undefined)){
			var requestList = new Object();
			requestList[ObjectPath] = new Object();
			requestList[ObjectPath]["cyctime"] = null;
			
			var successCode = this._requestVariablesArray(requestList);
			if (successCode == false){
				return null;
			}
			
			cyctime = requestList[ObjectPath]["cyctime"];
			
			//feeding garbage collector early
			requestList = null;
			
			//we have asked the object successful, so remember the result
			this.ResourceList.Events[ObjectPath] = new Object();
			this.ResourceList.Events[ObjectPath].useCount = 1;
			this.ResourceList.Events[ObjectPath].TimeEventParameterCyctime = cyctime;
			HMI.hmi_log_trace("cshmi._interpreteTimeEvent: remembering config of "+ObjectPath+" ");
		}else{
			//the object was asked this session, so reuse the config to save communication requests
			cyctime = this.ResourceList.Events[ObjectPath].TimeEventParameterCyctime;
			this.ResourceList.Events[ObjectPath].useCount++;
			//HMI.hmi_log_trace("cshmi._interpreteTimeEvent: using remembered config of "+ObjectPath+" (#"+this.ResourceList.Events[ObjectPath].useCount+")");
		}
		
		//call us again for cyclic interpretation of the Actions
		//only if we are in the initialisation or normal stage
		//and the active cshmi display is "our" one
		if ((this.initStage === true || HMI.Playground.firstChild !== null ) && HMI.cshmi === this){
			var preserveThis = this;	//grabbed from http://jsbin.com/etise/7/edit
			window.setTimeout(function(){
				preserveThis._interpreteTimeEvent(VisualObject, ObjectPath);
			}, cyctime*1000);
		}
		return true;
	},
	
	/**
	 * detect all Actions and triggers them
	 * @param {SVGElement} VisualObject Object to manipulate the visualisation
	 * @param {String} ObjectPath Path to this cshmi object containing the event/action/visualisation
	 * @return returnValue returnValue from the last Action
	 */
	_interpreteAction: function(VisualObject, ObjectPath){
		var returnValue = true;
		var responseArray = HMI.KSClient.getChildObjArray(ObjectPath, this);
		
		//a server could be not available, or we request a not existing (xpos) variable. This should not cancel the other actions
		for (var i=0; i < responseArray.length; i++) {
			var varName = responseArray[i].split(" ");
			if (varName[1].indexOf("/cshmi/SetValue") !== -1){
				returnValue = this._setValue(VisualObject, ObjectPath+"/"+varName[0], false);
			}else if (varName[1].indexOf("/cshmi/SetConcatValue") !== -1){
				returnValue = this._setValue(VisualObject, ObjectPath+"/"+varName[0], true);
			}else if (varName[1].indexOf("/cshmi/GetValue") !== -1){
				HMI.hmi_log_info_onwebsite("GetValue Action ("+varName[1]+")"+ObjectPath+" not useful at this position");
			}else if (varName[1].indexOf("/cshmi/IfThenElse") !== -1){
				returnValue = this._interpreteIfThenElse(VisualObject, ObjectPath+"/"+varName[0]);
			}else if (varName[1].indexOf("/cshmi/ChildrenIterator") !== -1){
				returnValue = this._interpreteChildrenIterator(VisualObject, ObjectPath+"/"+varName[0]);
			}else if (varName[1].indexOf("/cshmi/InstantiateTemplate") !== -1){
				returnValue = this._interpreteInstantiateTemplate(VisualObject, ObjectPath+"/"+varName[0]);
			}else if (varName[1].indexOf("/cshmi/RoutePolyline") !== -1){
				returnValue = this._interpreteRoutePolyline(VisualObject, ObjectPath+"/"+varName[0]);
			}else if (varName[1].indexOf("/cshmi/debugger") !== -1){
				//breakpoint requested
				returnValue = true;
				debugger;
			}else{
				HMI.hmi_log_info_onwebsite("Action "+varName[0]+" (Type: "+varName[1]+") under "+ObjectPath+" not supported");
			}
			//check type of returnvalue
			if (!(returnValue === true || returnValue === false)){
				HMI.hmi_log_error("Action ("+varName[1]+") of "+ObjectPath+" given a non boolean return value.");
			}
		}
		return returnValue;
	},
	
	/**
	 * get a Value from multiple Sources
	 * @param {SVGElement} VisualObject Object to manipulate the visualisation
	 * @param {String} ObjectPath Path to this cshmi object containing the event/action/visualisation
	 * @return false if error, null if intentionally no value, "" if no entry found
	 */
	_getValue: function(VisualObject, ObjectPath){
		var ParameterName;
		var ParameterValue;
		//if the Object was scanned earlier, get the cached information (could be the case with templates or repeated/cyclic calls to the same object)
		if (!(this.ResourceList.Actions && this.ResourceList.Actions[ObjectPath] !== undefined)){
			var requestList = new Object();
			requestList[ObjectPath] = new Object();
			requestList[ObjectPath]["ksVar"] = null;
			requestList[ObjectPath]["elemVar"] = null;
			requestList[ObjectPath]["globalVar"] = null;
			requestList[ObjectPath]["persistentGlobalVar"] = null;
			requestList[ObjectPath]["OperatorInput"] = null;
			requestList[ObjectPath]["TemplateFBReferenceVariable"] = null;
			requestList[ObjectPath]["TemplateFBVariableReferenceName"] = null;
			requestList[ObjectPath]["TemplateConfigValues"] = null;
			requestList[ObjectPath]["value"] = null;
			
			var successCode = this._requestVariablesArray(requestList);
			if (successCode == false){
				return false;
			}
			
			//search the configuration of the action
			for (ParameterName in requestList[ObjectPath]){
				ParameterValue = requestList[ObjectPath][ParameterName];
				if (ParameterValue !== ""){
					//we have found the parameter
					break;
				}
			}
			if (ParameterValue === ""){
				//the action was not configured
				ParameterName = "";
			}
			//feeding garbage collector early
			requestList = null;
			
			//we have asked the object successful, so remember the result
			this.ResourceList.Actions[ObjectPath] = new Object();
			this.ResourceList.Actions[ObjectPath].ParameterName = ParameterName;
			this.ResourceList.Actions[ObjectPath].ParameterValue = ParameterValue;
			this.ResourceList.Actions[ObjectPath].useCount = 1;
			HMI.hmi_log_trace("cshmi._getValue: remembering config of "+ObjectPath+" ");
		}else{
			//the object is asked this session, so reuse the config to save communication requests
			ParameterName = this.ResourceList.Actions[ObjectPath].ParameterName;
			ParameterValue = this.ResourceList.Actions[ObjectPath].ParameterValue;
			this.ResourceList.Actions[ObjectPath].useCount++;
			//HMI.hmi_log_trace("cshmi._getValue: using remembered config of "+ObjectPath+" (#"+this.ResourceList.Actions[ObjectPath].useCount+")");
		}
		
		var iterationObject = VisualObject;
		var preventNetworkRequest = false;
		do{
			//skip eventhandling if the object is not visible and the initstage is active
			if(iterationObject.getAttribute("display") === "none" && this.initStage === false){
				preventNetworkRequest = true;
				
				//doku unsichtbare objekte koennen nicht per ks getriggert sichtbar werden
				
				break;
			}
		}while( (iterationObject = iterationObject.parentNode) && iterationObject !== null && iterationObject.namespaceURI == HMI.HMI_Constants.NAMESPACE_SVG);
		iterationObject = null;
		
		if (ParameterName === "ksVar" && preventNetworkRequest === true){
			//intentionally no value
			return null;
		}else if (ParameterName === "ksVar" && preventNetworkRequest === false){
			var response;
			if (ParameterValue.charAt(0) == "/"){
				//we have an absolute path
				response = HMI.KSClient.getVar(null, '{'+ParameterValue+'}', null);
			}else{
				//get baseKsPath
				var baseKsPath = this._getBaseKsPath(VisualObject, ObjectPath);
				response = HMI.KSClient.getVar(null, '{'+baseKsPath+"/"+ParameterValue+'}', null);
			}
			var responseArray = HMI.KSClient.splitKsResponse(response);
			if (responseArray.length === 0){
				return false;
			}else{
				return responseArray[0];
			}
		}else if (ParameterName === "elemVar"){
			//todo interprete elemVarPath
			if (ParameterValue == "content"){
				//content is special, as it is different in OVM and SVG
				if (typeof VisualObject.textContent != "undefined"){
					return VisualObject.textContent;
				}else if (typeof VisualObject.innerText != "undefined"){
					return VisualObject.innerText;
				}else{
					//todo asv compatibility
					return "";
				}
			}else if (ParameterValue == "title"){
				for (var i = 0;i < VisualObject.childNodes.length;i++){
					if (VisualObject.childNodes.item(i).tagName === "title"){
						return VisualObject.childNodes.item(i).firstChild.textContent;
					}
				} 
				return "";
			}else if (ParameterValue == "visible"){
				//display is special, as it is different in OVM and SVG
				var displayVar = VisualObject.getAttribute("display");
				if (displayVar == "none"){
					return "FALSE";
				}else{
					return "TRUE";
				}
			}else if (ParameterValue == "rotate"){
				//rotate is special, as it is different in OVM and SVG
				if (VisualObject.tagName == "svg" && VisualObject.parentNode.tagName == "g" && VisualObject.parentNode.id === ""){
					//svg are not transformable, so the rotation is in the objects parent
					var TransformString = VisualObject.parentNode.getAttribute("transform");
				}else if (VisualObject.tagName == "svg"){
					//it is an template, with no rotation
					return "0";
				}else{
					TransformString = VisualObject.getAttribute("transform");
				}
				//"rotate(45,21.000000,50.000000)" or "rotate(45)"
				
				//remove rotate()
				TransformString = TransformString.replace(")", "").replace("rotate(", "");
				//get first number if there are 3, separated via comma
				return TransformString.split(",")[0];
			}else if (VisualObject.hasAttribute(ParameterValue)){
				return VisualObject.getAttribute(ParameterValue);
			}else{
				//unknown element variable
				return "";
			}
		}else if (ParameterName === "globalVar"){
			if (this.ResourceList.GlobalVar[ParameterValue] !== undefined){
				return this.ResourceList.GlobalVar[ParameterValue];
			}else {
				return "";
			}
		}else if (ParameterName === "persistentGlobalVar"){
			if (localStorage !== undefined){
				if (localStorage.getItem(ParameterValue) !== null){
					return localStorage.getItem(ParameterValue);
				}
			}else if(this.ResourceList.GlobalVar[ParameterValue] !== undefined){
				//fall back to globalVar if no persistent storage is available
				return this.ResourceList.GlobalVar[ParameterValue];
			}
			return "";
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
						var defaultValue = this._getValue(VisualObject, "tempPath")
						if (defaultValue !== false && defaultValue !== null){
							input = window.prompt(textinputHint, defaultValue);
						}else{
							input = window.prompt(textinputHint, "");
						}
						defaultValue = null;
						delete this.ResourceList.Actions["tempPath"];
					//e.g. "textinput:Some textinputHint:fixValue"
					}else if (splittedValueParameter.length > 2){
						input = window.prompt(textinputHint, splittedValueParameter[2]);
					}else{
						//e.g. "textinput:Some textinputHint"
						input = window.prompt(textinputHint, "");
					}
				}else{
					textinputHint = 'Please input a new value';
					input = window.prompt(textinputHint, "");
				}
				if (input !== null){
					return input;
				}else{
					//intentionally no value
					return null;
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
			return false;
		}else if (ParameterName === "TemplateFBReferenceVariable" && preventNetworkRequest === true){
			//intentionally no value
			return null;
		}else if (ParameterName === "TemplateFBReferenceVariable" && preventNetworkRequest === false){
			var TemplateObject;
			var FBRef;
			
			//doku OP_NAME im Iterator nicht, sonst schon
			if (this.ResourceList.ChildrenIterator.currentChild !== undefined && this.ResourceList.ChildrenIterator.currentChild["OP_NAME"] !== undefined ){
				//we are in an iterator and want to read out a value from the currentchild
				TemplateObject = VisualObject;
				//search FBReference of root Object
				do{
					//FBReference found
					if(TemplateObject.FBReference && TemplateObject.FBReference["default"] !== undefined){
						FBRef = TemplateObject.FBReference["default"];
						
						if(ParameterValue == "identifier"){
							//if the identifier is requested calculate it to avoid network request
							return this.ResourceList.ChildrenIterator.currentChild["OP_NAME"];
						}
						
						if (this.ResourceList.ChildrenIterator.currentChild["OP_ACCESS"] !== undefined && this.ResourceList.ChildrenIterator.currentChild["OP_ACCESS"].indexOf("KS_AC_PART") !== -1){
							//we have an OV-PART, so the separator is a dot
							var result = HMI.KSClient.getVar(null, '{'+ FBRef+"."+this.ResourceList.ChildrenIterator.currentChild["OP_NAME"]+"."+ParameterValue + '}', null);
						}else{
							//we have no OV-PART, so the separator is a slash
							result = HMI.KSClient.getVar(null, '{'+ FBRef+"/"+this.ResourceList.ChildrenIterator.currentChild["OP_NAME"]+"."+ParameterValue + '}', null);
						}
						var returnValue = HMI.KSClient.splitKsResponse(result);
						if (returnValue.length > 0){
							//valid response
							return returnValue[0];
						}
						//error
						return false;
					}
				//loop upwards to find the Template object
				}while( (TemplateObject = TemplateObject.parentNode) && TemplateObject !== null && TemplateObject.namespaceURI == HMI.HMI_Constants.NAMESPACE_SVG);  //the = is no typo here!
				return "";
			}else if (this.ResourceList.ChildrenIterator.currentChild !== undefined && this.ResourceList.ChildrenIterator.currentChild["OP_VALUE"] !== undefined ){
				//we are in an iterator and want to read out a value from the currentchild
				TemplateObject = VisualObject;
				//search FBReference of root Object
				do{
					//FBReference found
					if(TemplateObject.FBReference && TemplateObject.FBReference["default"] !== undefined){
						FBRef = TemplateObject.FBReference["default"];
						break;
					}
				//loop upwards to find the Template object
				}while( (TemplateObject = TemplateObject.parentNode) && TemplateObject !== null && TemplateObject.namespaceURI == HMI.HMI_Constants.NAMESPACE_SVG);  //the = is no typo here!
				
				// check if FBref beginn with "//" because we need the server Info as prefix when using getElementById
				// e.g "//dev/ov_hmidemo7/TechUnits/TU10/h_bkqwmtbbhpf"" --> use prefix "//dev/ov_hmidemo7"
				var prefix = "";
				if (FBRef !== null && FBRef.charAt(0) === "/" && FBRef.charAt(1) === "/"){
					//find the 3rd "/"
					var slashIndex = FBRef.indexOf("/", 2);
					//find the 4th "/"
					slashIndex = FBRef.indexOf("/", slashIndex+1);
					//only keep the String before 4th "/"
					var prefix = FBRef.slice(0, slashIndex);
				}
				
				var result = HMI.KSClient.getVar(null, '{'+ prefix+"."+this.ResourceList.ChildrenIterator.currentChild["OP_VALUE"]+"."+ParameterValue + '}', null);
				
				var returnValue = HMI.KSClient.splitKsResponse(result);
				if (returnValue.length > 0){
					//valid response
					return returnValue[0];
				}
				//error
				return false;
			}
			
			//no active iterator, so plain FBReference
			TemplateObject = VisualObject;
			do{
				if(TemplateObject.FBReference && TemplateObject.FBReference[ParameterValue] !== undefined){
					//##### Deprecated: change this we have TemplateFBVariableReferenceName now
					
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
						return false;
					}else{
						//a normal relative path
						HMI.hmi_log_info_onwebsite('GetValue '+ObjectPath+' wrong configured. No relative path allowed');
						return false;
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
						return false;
					}else{
						//a normal relative path
						HMI.hmi_log_info_onwebsite('GetValue '+ObjectPath+' wrong configured. No relative path allowed');
						return false;
					}
				}
			//loop upwards to find the Template object
			}while( (TemplateObject = TemplateObject.parentNode) && TemplateObject !== null && TemplateObject.namespaceURI == HMI.HMI_Constants.NAMESPACE_SVG);  //the = is no typo here!
			return "";
		}else if (ParameterName === "TemplateFBVariableReferenceName" && preventNetworkRequest !== true){
			var TemplateObject = VisualObject;
			do{
				if(TemplateObject.FBVariableReference && TemplateObject.FBVariableReference[ParameterValue] !== undefined){
					//a FBVariableReferenceName was requested
					if (TemplateObject.FBVariableReference[ParameterValue].charAt(0) === "/"){
						//String begins with / so it is a fullpath
						var result = HMI.KSClient.getVar(null, TemplateObject.FBVariableReference[ParameterValue], null);
						var returnValue = HMI.KSClient.splitKsResponse(result);
						if (returnValue.length > 0){
							//valid response
							return returnValue[0];
						}
						//error
						return false;
					}else{
						//a normal relative path
						HMI.hmi_log_info_onwebsite('GetValue '+ObjectPath+' wrong configured. No relative path allowed');
						return false;
					}
				}
			//loop upwards to find the Template object
			}while( (TemplateObject = TemplateObject.parentNode) && TemplateObject !== null && TemplateObject.namespaceURI == HMI.HMI_Constants.NAMESPACE_SVG);  //the = is no typo here!
			return "";
		}else if (ParameterName === "TemplateConfigValues"){
			var TemplateObject = VisualObject;
			do{
				if(TemplateObject.ConfigValues && TemplateObject.ConfigValues[ParameterValue] !== undefined){
					//this is a ConfigValue and has valid data for us
					return TemplateObject.ConfigValues[ParameterValue];
				}
			//loop upwards to find the Template object
			}while( (TemplateObject = TemplateObject.parentNode) && TemplateObject !== null && TemplateObject.namespaceURI == HMI.HMI_Constants.NAMESPACE_SVG);  //the = is no typo here!
			return "";
		}else if (ParameterName === "value"){
			return ParameterValue;
		}
		return ""; //fallback
	},
	
	/**
	 * set a Value to multiple Targets
	 * @param {SVGElement} VisualObject Object to manipulate the visualisation
	 * @param {String} ObjectPath Path to this cshmi object containing the event/action/visualisation
	 * @param {Boolean} Concat Concat multiple getValues or have we one static OV_PART?
	 * @return false on error, true on success
	 * @todo configvalue unterstützen...
	 */
	_setValue: function(VisualObject, ObjectPath, Concat){
		var NewValue = "";
		//get Value to set
		if (Concat === false){
			//via getValue-part of setValue object
			NewValue = this._getValue(VisualObject, ObjectPath+".value");
			
			if (NewValue === false){
				//_getValue had an error
				HMI.hmi_log_info("cshmi._setValue on "+ObjectPath+" (baseobject: "+VisualObject.id+") failed because of an error in getValue.");
				//NewValue = this._getValue(VisualObject, ObjectPath+".value");
				return false;
			}else if (NewValue === null){
				//_getValue had intentionally no value, abort
				return true;
			}else if (NewValue === undefined){
				//should not happen
				HMI.hmi_log_error("cshmi._setValue on "+ObjectPath+" (baseobject: "+VisualObject.id+") failed because of an NewValue of undefined.");
				return false;
			}
		}else{
			//via multiple getValues under the setValue object
			var responseArray = HMI.KSClient.getChildObjArray(ObjectPath, this);
			for (var i=0; i < responseArray.length; i++) {
				var varName = responseArray[i].split(" ");
				if (varName[1].indexOf("/cshmi/GetValue") !== -1){
					var TempNewValue = this._getValue(VisualObject, ObjectPath+"/"+varName[0]);
					if (TempNewValue !== false && TempNewValue !== null && TempNewValue !== undefined){
						NewValue = NewValue + TempNewValue;
					}else if (TempNewValue !== false){
						HMI.hmi_log_info("cshmi._getValue of "+ObjectPath+"/"+varName[0]+" (baseobject: "+VisualObject.id+") was had an error. Skipping.");
					}
				}
			}
		}
		
		//get info where to set the NewValue
		
		var ParameterName;
		var ParameterValue;
		//if the Object was scanned earlier, get the cached information (could be the case with templates or repeated/cyclic calls to the same object)
		if (!(this.ResourceList.Actions && this.ResourceList.Actions[ObjectPath] !== undefined)){
			var requestList = new Object();
			requestList[ObjectPath] = new Object();
			requestList[ObjectPath]["ksVar"] = null;
			requestList[ObjectPath]["elemVar"] = null;
			requestList[ObjectPath]["globalVar"] = null;
			requestList[ObjectPath]["persistentGlobalVar"] = null;
			requestList[ObjectPath]["TemplateFBReferenceVariable"] = null;
			requestList[ObjectPath]["TemplateFBVariableReferenceName"] = null;
			requestList[ObjectPath]["TemplateConfigValues"] = null;
			
			var successCode = this._requestVariablesArray(requestList);
			if (successCode == false){
				return false;
			}
			
			//search the configuration of the action
			for (ParameterName in requestList[ObjectPath]){
				ParameterValue = requestList[ObjectPath][ParameterName];
				if (ParameterValue !== ""){
					//we have found the parameter
					break;
				}
			}
			if (ParameterValue === ""){
				//the action was not configured
				ParameterName = "";
			}
			//feeding garbage collector early
			requestList = null;
			
			//we have asked the object successful, so remember the result
			this.ResourceList.Actions[ObjectPath] = new Object();
			this.ResourceList.Actions[ObjectPath].useCount = 1;
			this.ResourceList.Actions[ObjectPath].ParameterName = ParameterName;
			this.ResourceList.Actions[ObjectPath].ParameterValue = ParameterValue;
			HMI.hmi_log_trace("cshmi._setValue: remembering config of "+ObjectPath+" ");
		}else{
			//the object is asked this session, so reuse the config to save communication requests
			ParameterName = this.ResourceList.Actions[ObjectPath].ParameterName;
			ParameterValue = this.ResourceList.Actions[ObjectPath].ParameterValue;
			this.ResourceList.Actions[ObjectPath].useCount++;
			//HMI.hmi_log_trace("cshmi._setValue: using remembered config of "+ObjectPath+" (#"+this.ResourceList.Actions[ObjectPath].useCount+")");
		}
		
		//set the new Value
		if (ParameterName === "ksVar"){
			var response;
			if (ParameterValue.charAt(0) == "/"){
				//we have an absolute path
				response = HMI.KSClient.setVar(null, '{'+ParameterValue+'}', NewValue, null);
			}else{
				//get baseKsPath
				var baseKsPath = this._getBaseKsPath(VisualObject, ObjectPath);
				response = HMI.KSClient.setVar(null, '{'+baseKsPath+ParameterValue+'}', NewValue, null);
			}
			if (response.indexOf("KS_ERR_BADPARAM") !== -1){
				HMI.hmi_log_onwebsite('Setting "'+NewValue+'" at '+baseKsPath+ParameterValue+' not successfull: Bad Parameter ');
			}else if (response.indexOf("KS_ERR") !== -1){
				HMI.hmi_log_info('Setting '+NewValue+' at variable '+baseKsPath+ParameterValue+' not successfull: '+response+' (configured here: '+ObjectPath+').');
			}
			return true;
		}else if (ParameterName === "elemVar"){
			//todo interprete elemVarPath
			if (ParameterValue == "content"){
				//content is special, as it is different in OVM and SVG
				
				var tspans = VisualObject.getElementsByTagNameNS(HMI.HMI_Constants.NAMESPACE_SVG, 'tspan');
				if (tspans.length == 0){
					return false;
				}
				//if trimToLength is set in parent TextFB and perform trimming if needed
				var trimLength = parseInt(this.ResourceList.Elements[VisualObject.id].ElementParameters.trimToLength, 10);
				var contentLength = parseInt(NewValue.length, 10);
				var trimmedContent;
				if(trimLength > 0 && isNumeric(NewValue)){
					//we have a numeric NewValue
					if(NewValue.indexOf(".") === -1){
						//INT or UINT
						tspans[0].replaceChild(HMI.svgDocument.createTextNode(NewValue), tspans[0].firstChild);
					}else{
						//other values
						var power = Math.pow(10, trimLength || 0);
						trimmedContent = String(Math.round(NewValue * power) / power);
						
						tspans[0].replaceChild(HMI.svgDocument.createTextNode(trimmedContent), tspans[0].firstChild);
						this._setTitle(VisualObject, NewValue);
					}
				}else if((trimLength > 0) && (contentLength > trimLength)){
					trimmedContent = NewValue.substr(0, trimLength) + String.fromCharCode(8230);
					tspans[0].replaceChild(HMI.svgDocument.createTextNode(trimmedContent), tspans[0].firstChild);
					this._setTitle(VisualObject, NewValue);
				}else if((trimLength < 0) && (contentLength > -trimLength)){
					trimmedContent =  String.fromCharCode(8230) + NewValue.substr(contentLength + trimLength);
					tspans[0].replaceChild(HMI.svgDocument.createTextNode(trimmedContent), tspans[0].firstChild);
					this._setTitle(VisualObject, NewValue);
				}else{
					tspans[0].replaceChild(HMI.svgDocument.createTextNode(NewValue), tspans[0].firstChild);
				}
			}else if (ParameterValue == "title"){
				this._setTitle(VisualObject, NewValue);
/*
			}else if (ParameterValue.indexOf("hover") !== -1){
				//todo, https://developer.mozilla.org/en/DOM/CSSStyleSheet/insertRule
				
				//VisualObject, NewValue;
*/
			}else if (ParameterValue == "visible"){
				//visible is special, as it is different in OVM and SVG
				if (NewValue == "FALSE"){
					VisualObject.setAttribute("display", "none");
				}else{
					VisualObject.setAttribute("display", "block");
				}
			}else if (ParameterValue == "rotate"){
				//rotate is special, as it is different in OVM and SVG
				
				//svg are not transformable, so the rotation is in the objects parent
				if (VisualObject.tagName === "svg" && VisualObject.parentNode.tagName === "g" && VisualObject.parentNode.id === ""){
					//object has already an g parent
					var rotationObject = VisualObject.parentNode;
				}else if (VisualObject.tagName === "svg" && VisualObject.parentNode.tagName !== "g"){
					//element has to be shifted into an g element
					var rotationObject = HMI.svgDocument.createElementNS(HMI.HMI_Constants.NAMESPACE_SVG, 'g');
					rotationObject.setAttribute("overflow", "visible");
					VisualObject.parentNode.replaceChild(rotationObject, VisualObject);
					rotationObject.appendChild(VisualObject);
				}else{
					//normal visual element
					rotationObject = VisualObject;
				}
				
				if(VisualObject.getAttribute("x") !== null){
					rotationObject.setAttribute("transform", "rotate("+NewValue+","+VisualObject.getAttribute("x")+","+VisualObject.getAttribute("y")+")");
				}else if(VisualObject.getAttribute("cx") !== null){
					VisualObject.setAttribute("transform", "rotate("+NewValue+","+VisualObject.getAttribute("cx")+","+VisualObject.getAttribute("cy")+")");
				}else{
					VisualObject.setAttribute("transform", "rotate("+NewValue+")");
				}
			}else{
				VisualObject.setAttribute(ParameterValue, NewValue);
				//reposition Layer if x, y, width or height is changed
				if (ParameterValue === "x" || ParameterValue === "y" || ParameterValue === "width" || ParameterValue === "height"){
					HMI._setLayerPosition(VisualObject);
					//we want to have offset parameter on all visual elements
					var ComponentChilds = VisualObject.getElementsByTagNameNS(HMI.HMI_Constants.NAMESPACE_SVG, '*');
					for(var i = 0;i < ComponentChilds.length;i++){
						HMI._setLayerPosition(ComponentChilds[i]);
					}
				}
			}
			return true;
		}else if (ParameterName === "globalVar"){
			//globalVar
			this.ResourceList.GlobalVar[ParameterValue] = NewValue;
			for (var i = 0; i < this.ResourceList.globalvarChangedCallStack.length;i++){
				var EventObjItem = this.ResourceList.globalvarChangedCallStack[i];
				this._interpreteAction(EventObjItem["VisualObject"], EventObjItem["ObjectPath"]);
			}
			return true;
		}else if (ParameterName === "persistentGlobalVar"){
			//persistentGlobalVar
			if (localStorage !== undefined){
				localStorage.setItem(ParameterValue, NewValue);
			}else{
				//fall back to globalVar if no persistent storage is available
				this.ResourceList.GlobalVar[ParameterValue] = NewValue;
			}
			for (var i = 0; i < this.ResourceList.globalvarChangedCallStack.length;i++){
				var EventObjItem = this.ResourceList.globalvarChangedCallStack[i];
				this._interpreteAction(EventObjItem["VisualObject"], EventObjItem["ObjectPath"]);
			}
			return true;
		}else if (ParameterName === "TemplateFBReferenceVariable"){
			var TemplateObject;
			if (this.ResourceList.ChildrenIterator.currentChild !== undefined && this.ResourceList.ChildrenIterator.currentChild["OP_NAME"] !== undefined ){
				//we are in an iterator and want to read out a value from the currentchild
				var TemplateObject = VisualObject;
				var FBRef;
				//search FBReference of root Object
				do{
					//FBReference found
					if(TemplateObject.FBReference && TemplateObject.FBReference["default"] !== undefined){
						FBRef = TemplateObject.FBReference["default"];
						
						if (this.ResourceList.ChildrenIterator.currentChild["OP_ACCESS"] !== undefined && this.ResourceList.ChildrenIterator.currentChild["OP_ACCESS"].indexOf("KS_AC_PART") !== -1){
							//we have an OV-PART, so the separator is a dot
							var result = HMI.KSClient.setVar(null, '{'+ FBRef+"."+this.ResourceList.ChildrenIterator.currentChild["OP_NAME"]+"."+ParameterValue + '}', NewValue, null);
						}else{
							//we have no OV-PART, so the separator is a slash
							result = HMI.KSClient.setVar(null, '{'+ FBRef+"/"+this.ResourceList.ChildrenIterator.currentChild["OP_NAME"]+"."+ParameterValue + '}', NewValue, null);
						}
						if (result.indexOf("KS_ERR_BADPARAM") !== -1){
							HMI.hmi_log_onwebsite('Setting "'+NewValue+'" at '+TemplateObject.FBReference[ParameterValue]+' not successfull: Bad Parameter ');
						}else if (result.indexOf("KS_ERR") !== -1){
							HMI.hmi_log_info('Setting '+NewValue+' not successfull: '+result+' (configured here: '+ObjectPath+').');
						}
						return true;
					}
				//loop upwards to find the Template object
				}while( (TemplateObject = TemplateObject.parentNode) && TemplateObject !== null && TemplateObject.namespaceURI == HMI.HMI_Constants.NAMESPACE_SVG);  //the = is no typo here!
				return false;
			}else if (this.ResourceList.ChildrenIterator.currentChild !== undefined && this.ResourceList.ChildrenIterator.currentChild["OP_VALUE"] !== undefined ){
				//we are in an iterator and want to read out a value from the currentchild
				var TemplateObject = VisualObject;
				var FBRef;
				//search FBReference of root Object
				do{
					//FBReference found
					if(TemplateObject.FBReference && TemplateObject.FBReference["default"] !== undefined){
						FBRef = TemplateObject.FBReference["default"];
						break;
					}
				//loop upwards to find the Template object
				}while( (TemplateObject = TemplateObject.parentNode) && TemplateObject !== null && TemplateObject.namespaceURI == HMI.HMI_Constants.NAMESPACE_SVG);  //the = is no typo here!
				
				// check if FBref beginn with "//" because we need the server Info as prefix when using getElementById
				// e.g "//dev/ov_hmidemo7/TechUnits/TU10/h_bkqwmtbbhpf"" --> use prefix "//dev/ov_hmidemo7"
				var prefix = "";
				if (FBRef !== null && FBRef.charAt(0) === "/" && FBRef.charAt(1) === "/"){
					//find the 3rd "/"
					var slashIndex = FBRef.indexOf("/", 2);
					//find the 4th "/"
					slashIndex = FBRef.indexOf("/", slashIndex+1);
					//only keep the String before 4th "/"
					var prefix = FBRef.slice(0, slashIndex);
				}
				
				var result = HMI.KSClient.setVar(null, '{'+ prefix + this.ResourceList.ChildrenIterator.currentChild["OP_VALUE"]+"."+ParameterValue + '}', NewValue, null);
				if (result.indexOf("KS_ERR_BADPARAM") !== -1){
					HMI.hmi_log_onwebsite('Setting "'+NewValue+'" at '+this.ResourceList.ChildrenIterator.currentChild["OP_VALUE"]+"."+ParameterValue+' not successfull: Bad Parameter ');
				}else if (result.indexOf("KS_ERR") !== -1){
					HMI.hmi_log_info('Setting '+NewValue+' not successfull: '+result+' (configured here: '+ObjectPath+').');
				}
				return true;
			}
			
			//no active iterator, so plain FBReference
			TemplateObject = VisualObject;
			do{
				if(TemplateObject.FBReference && TemplateObject.FBReference[ParameterValue] !== undefined){
					//##### Deprecated: change this we have TemplateFBVariableReferenceName now
					
					//a named variable of a FBReference was requested, naming was done in instantiateTemplate
					if (TemplateObject.FBReference[ParameterValue].charAt(0) === "/"){
						//String begins with / so it is a fullpath
						var response = HMI.KSClient.setVar(null, TemplateObject.FBReference[ParameterValue], NewValue, null);
						if (response.indexOf("KS_ERR_BADPARAM") !== -1){
							HMI.hmi_log_onwebsite('Setting "'+NewValue+'" at '+TemplateObject.FBReference[ParameterValue]+' not successfull: Bad Parameter ');
						}else if (response.indexOf("KS_ERR") !== -1){
							HMI.hmi_log_info('Setting '+NewValue+' not successfull: '+response+' (configured here: '+ObjectPath+').');
						}
						return true;
					}else{
						//a normal relative path
						HMI.hmi_log_info_onwebsite('SetValue '+ObjectPath+' wrong configured. No relative path allowed');
						return false;
					}
				}else if(TemplateObject.FBReference && TemplateObject.FBReference["default"] !== undefined){
					//a variable from a Template was requested
					if (TemplateObject.FBReference["default"].charAt(0) === "/"){
						//String begins with / so it is a fullpath
						response = HMI.KSClient.setVar(null, TemplateObject.FBReference["default"]+'.'+ParameterValue, NewValue, null);
						if (response.indexOf("KS_ERR_BADPARAM") !== -1){
							HMI.hmi_log_onwebsite('Setting "'+NewValue+'" at '+TemplateObject.FBReference["default"]+'.'+ParameterValue+' not successfull: Bad Parameter ');
						}else if (response.indexOf("KS_ERR") !== -1){
							HMI.hmi_log_info('Setting '+NewValue+' at '+TemplateObject.FBReference["default"]+'.'+ParameterValue+' not successfull: '+response+' (configured here: '+ObjectPath+').');
						}
						return true;
					}else{
						//a normal relative path
						HMI.hmi_log_info_onwebsite('SetValue '+ObjectPath+' wrong configured. No relative path allowed');
						return false;
					}
				}
			//loop upwards to find the Template object
			}while( (TemplateObject = TemplateObject.parentNode) && TemplateObject !== null && TemplateObject.namespaceURI == HMI.HMI_Constants.NAMESPACE_SVG);  //the = is no typo here!
			return false;
		}else if (ParameterName === "TemplateFBVariableReferenceName"){
			//TemplateFBReferenceVariable
			var TemplateObject = VisualObject;
			do{
				if(TemplateObject.FBVariableReference && TemplateObject.FBVariableReference[ParameterValue] !== undefined){
					//a FBVariableReferenceName was requested
					if (TemplateObject.FBVariableReference[ParameterValue].charAt(0) === "/"){
						//String begins with / so it is a fullpath
						var response = HMI.KSClient.setVar(null, TemplateObject.FBVariableReference[ParameterValue], NewValue, null);
						if (response.indexOf("KS_ERR_BADPARAM") !== -1){
							HMI.hmi_log_onwebsite('Setting "'+NewValue+'" at '+TemplateObject.FBVariableReference[ParameterValue]+' not successfull: Bad Parameter ');
						}else if (response.indexOf("KS_ERR") !== -1){
							HMI.hmi_log_info('Setting '+NewValue+' not successfull: '+response+' (configured here: '+ObjectPath+').');
						}
						return true;
					}else{
						//a normal relative path
						HMI.hmi_log_info_onwebsite('SetValue '+ObjectPath+' wrong configured. No relative path allowed');
						return false;
					}
				}
			//loop upwards to find the Template object
			}while( (TemplateObject = TemplateObject.parentNode) && TemplateObject !== null && TemplateObject.namespaceURI == HMI.HMI_Constants.NAMESPACE_SVG);  //the = is no typo here!
			return false;
		}else if (ParameterName === "TemplateConfigValues"){
			TemplateObject = VisualObject;
			do{
				if(TemplateObject.FBReference && TemplateObject.FBReference["default"] !== undefined){
					break;
				}
			//loop upwards to find the Template object
			}while( (TemplateObject = TemplateObject.parentNode) && TemplateObject !== null && TemplateObject.namespaceURI == HMI.HMI_Constants.NAMESPACE_SVG);  //the = is no typo here!
			
			if(TemplateObject !== null && TemplateObject.ConfigValues){
				TemplateObject.ConfigValues[ParameterValue] = NewValue;
				return true;
			}
			//fixme in diesem Fall muss die info auch irgendwo hinterlegt sein. Nur wohin?
			return false;
		}
		HMI.hmi_log_info_onwebsite('SetValue '+ObjectPath+' not configured.');
		return false;
	},
	
	/**
	 * search baseKsPath by iterating objects
	 * @param {SVGElement} VisualObject Object to manipulate the visualisation
	 * @param {String} ObjectPath Path to this cshmi object containing the event/action/visualisation
	 * @return {String} returnValue String of the baseKSPath as seen from the ObjectPath; could be ""
	 */
	_getBaseKsPath: function(VisualObject, ObjectPath){
		var ObjectPathArray = ObjectPath.split("/");
		
		var returnValue = "";
		do{
			var currentPath = ObjectPathArray.join("/");
			var responseArray;
			if (this.ResourceList.baseKsPath[currentPath] === undefined){
				var response = HMI.KSClient.getVar(null, '{'+currentPath+'.baseKsPath}', null);
				responseArray = HMI.KSClient.splitKsResponse(response);
				
				this.ResourceList.baseKsPath[currentPath] = responseArray;
				this.ResourceList.baseKsPath[ObjectPath].useCount = 1;
				HMI.hmi_log_trace("cshmi._BaseKsPath: remembering config of "+currentPath+" ");
			}else{
				responseArray = this.ResourceList.baseKsPath[currentPath];
				this.ResourceList.baseKsPath[ObjectPath].useCount++;
				//HMI.hmi_log_trace("cshmi._BaseKsPath: reusing remembered config of "+currentPath+" (#"+this.ResourceList.baseKsPath[ObjectPath].useCount+")");
			}
			
			if (responseArray.length === 0){
				//an object in tree is no cshmi object or we have an error
				return returnValue;
			}else if (responseArray[0] !== ""){
				if (responseArray[0].charAt(0) === "/"){
					//String begins with / so it is a fullpath
					returnValue = responseArray[0]+returnValue;
					//full path => stop searching for other path
					break;
				}else{
					//a normal relative path
					returnValue = responseArray[0]+returnValue;
				}
			}else{
				// no action in this loop
			}
			ObjectPathArray.pop();
		}while(ObjectPathArray.length > 1);
		return returnValue;
	},
	
	/**
	 * calls conditions below the if PART and triggers actions in then or else PART
	 * @param {SVGElement} VisualObject Object to manipulate the visualisation
	 * @param {String} ObjectPath Path to this cshmi object containing the event/action/visualisation
	 * @return {Boolean} false if an error occured, returnValue of the called actions
	 */
	_interpreteIfThenElse: function(VisualObject, ObjectPath){
		var anyCond;
		//if the Object was scanned earlier, get the cached information (could be the case with templates or repeated/cyclic calls to the same object)
		if (!(this.ResourceList.Actions && this.ResourceList.Actions[ObjectPath] !== undefined)){
			var requestList = new Object();
			requestList[ObjectPath] = new Object();
			requestList[ObjectPath]["anycond"] = null;
			
			var successCode = this._requestVariablesArray(requestList);
			if (successCode == false){
				return null;
			}
			
			anyCond = requestList[ObjectPath]["anycond"];
			
			//feeding garbage collector early
			requestList = null;
			
			//we have asked the object successful, so remember the result
			this.ResourceList.Actions[ObjectPath] = new Object();
			this.ResourceList.Actions[ObjectPath].IfThenElseParameterAnycond = anyCond;
			this.ResourceList.Actions[ObjectPath].useCount = 1;
			HMI.hmi_log_trace("cshmi._interpreteIfThenElse: remembering config of "+ObjectPath+" ");
		}else{
			//the object is asked this session, so reuse the config to save communication requests
			anyCond = this.ResourceList.Actions[ObjectPath].IfThenElseParameterAnycond;
			this.ResourceList.Actions[ObjectPath].useCount++;
			//HMI.hmi_log_trace("cshmi._interpreteIfThenElse: using remembered config of "+ObjectPath+" (#"+this.ResourceList.Actions[ObjectPath].useCount+")");
		}
		var ConditionMatched = false;
		var responseArray = HMI.KSClient.getChildObjArray(ObjectPath+".if", this);
		var i = 0;
		if (anyCond == "TRUE"){
			//logical OR
			while(i < responseArray.length && ConditionMatched !== true){
				var varName = responseArray[i].split(" ");
				if (varName[1].indexOf("/cshmi/CompareIteratedChild") !== -1){
					ConditionMatched = this._checkConditionIterator(VisualObject, ObjectPath+".if/"+varName[0], ObjectPath);
				}
				else if (varName[1].indexOf("/cshmi/Compare") !== -1){
					ConditionMatched = this._checkCondition(VisualObject, ObjectPath+".if/"+varName[0], ObjectPath);
				}
				i++;
			}
		}else{
			ConditionMatched = true;
			//logical AND
			while(i < responseArray.length && ConditionMatched !== false){
				var varName = responseArray[i].split(" ");
				if (varName[1].indexOf("/cshmi/CompareIteratedChild") !== -1){
					ConditionMatched = this._checkConditionIterator(VisualObject, ObjectPath+".if/"+varName[0], ObjectPath);
				}
				else if (varName[1].indexOf("/cshmi/Compare") !== -1){
					ConditionMatched = this._checkCondition(VisualObject, ObjectPath+".if/"+varName[0], ObjectPath);
				}
				i++;
			}
		}
		if (ConditionMatched === true){
			return this._interpreteAction(VisualObject, ObjectPath+".then");
		}else if (ConditionMatched === false){
			return this._interpreteAction(VisualObject, ObjectPath+".else");
		}else{
			//this Action produced an error
			return false;
		}
		
		return false;
	},
	
	/**
	 * iterates over an association (ov_containment or other) or an variable vector
	 * @param {SVGElement} VisualObject Object to manipulate the visualisation
	 * @param {String} ObjectPath Path to this cshmi object containing the event/action/visualisation
	 * @return {Boolean} false if an error occured, returnValue of the called actions
	 */
	_interpreteChildrenIterator: function(VisualObject, ObjectPath){
		var rootObject = VisualObject;
		var FBRef;
		//search FBReference of root Object
		while (rootObject !== null && rootObject.namespaceURI == HMI.HMI_Constants.NAMESPACE_SVG){
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
		
		var ChildrenType;
		//if the Object was scanned earlier, get the cached information (could be the case with templates or repeated/cyclic calls to the same object)
		if (!(this.ResourceList.Actions && this.ResourceList.Actions[ObjectPath] !== undefined)){
			var requestList = new Object();
			requestList[ObjectPath] = new Object();
			requestList[ObjectPath]["ChildrenType"] = null;
			
			var successCode = this._requestVariablesArray(requestList);
			if (successCode == false){
				return null;
			}
			
			ChildrenType = requestList[ObjectPath]["ChildrenType"];
			
			//feeding garbage collector early
			requestList = null;
			
			//we have asked the object successful, so remember the result
			this.ResourceList.Actions[ObjectPath] = new Object();
			this.ResourceList.Actions[ObjectPath].ChildrenIteratorParameterChildrenType = ChildrenType;
			this.ResourceList.Actions[ObjectPath].useCount = 1;
			HMI.hmi_log_trace("cshmi._interpreteChildrenIterator: remembering config of "+ObjectPath+" ");
		}else{
			//the object is asked this session, so reuse the config to save communication requests
			ChildrenType = this.ResourceList.Actions[ObjectPath].ChildrenIteratorParameterChildrenType;
			this.ResourceList.Actions[ObjectPath].useCount++;
			//HMI.hmi_log_trace("cshmi._interpreteChildrenIterator: using remembered config of "+ObjectPath+" (#"+this.ResourceList.Actions[ObjectPath].useCount+")");
		}
		
		//this will be increased in a successful instantiateTemplate
		this.ResourceList.ChildrenIterator.currentCount = 0;
		
		//remember an old currentChild (for cascaded iterators)
		var savedCurrentChild = this.ResourceList.ChildrenIterator.currentChild;
		delete this.ResourceList.ChildrenIterator.currentChild;
		
		var returnValue = true;
		if (ChildrenType.indexOf("OT_") !== -1){
			var response = HMI.KSClient.getEP(null, encodeURI(FBRef)+'%20*', "%20-type%20$::TKS::" + ChildrenType + "%20-output%20[expr%20$::TKS::OP_ANY]", null);
			response = HMI.KSClient.splitKsResponse(response, 1);
			for (var i=0; i<response.length; i++){
				var responseDictionary = Array();
				//Variables were requested or ANY and we got a Variable right now
				if (ChildrenType === "OT_VARIABLE" || response[i][1] === "KS_OT_VARIABLE"){
					responseDictionary["OP_NAME"] = response[i][0];
					responseDictionary["OP_TYPE"] = response[i][1];
					responseDictionary["OP_COMMENT"] = response[i][2];
					responseDictionary["OP_ACCESS"] = response[i][3];
					responseDictionary["OP_SEMANTICS"] = response[i][4];
					responseDictionary["OP_CREATIONTIME"] = response[i][5];
					responseDictionary["OP_CLASS"] = response[i][6];
					responseDictionary["OP_TECHUNIT"] = response[i][7];
				}
				//Domains were requested or ANY and we got a Domain right now
				else if (ChildrenType === "OT_DOMAIN" || response[i][1] === "KS_OT_DOMAIN"){
					responseDictionary["OP_NAME"] = response[i][0];
					responseDictionary["OP_TYPE"] = response[i][1];
					responseDictionary["OP_COMMENT"] = response[i][2];
					responseDictionary["OP_ACCESS"] = response[i][3];
					responseDictionary["OP_SEMANTICS"] = response[i][4];
					responseDictionary["OP_CREATIONTIME"] = response[i][5];
					responseDictionary["OP_CLASS"] = response[i][6];
					responseDictionary["OP_TECHUNIT"] = response[i][7];
				}
				//Links were requested or ANY and we got a Link right now
				else if (ChildrenType === "OT_LINK" || response[i][1] === "KS_OT_LINK"){
					responseDictionary["OP_NAME"] = response[i][0];
					responseDictionary["OP_TYPE"] = response[i][1];
					responseDictionary["OP_COMMENT"] = response[i][2];
					responseDictionary["OP_ACCESS"] = response[i][3];
					responseDictionary["OP_SEMANTICS"] = response[i][4];
					responseDictionary["OP_CREATIONTIME"] = response[i][5];
					responseDictionary["OP_CLASS"] = response[i][6];
					responseDictionary["OP_ASSOCIDENT"] = response[i][7];
					responseDictionary["OP_ROLEIDENT"] = response[i][8];
				}
				//Historys were requested or ANY and we got a History right now
				else if (ChildrenType === "OT_HISTORY" || response[i][1] === "KS_OT_HISTORY"){
					responseDictionary["OP_NAME"] = response[i][0];
					responseDictionary["OP_TYPE"] = response[i][1];
					responseDictionary["OP_COMMENT"] = response[i][2];
					responseDictionary["OP_ACCESS"] = response[i][3];
					responseDictionary["OP_SEMANTICS"] = response[i][4];
					responseDictionary["OP_CREATIONTIME"] = response[i][5];
					responseDictionary["OP_CLASS"] = response[i][6];
					responseDictionary["OP_DEFAULTINTERP"] = response[i][7];
					responseDictionary["OP_SUPPORTEDINTERP"] = response[i][8];
					responseDictionary["OP_TYPEIDENT"] = response[i][9];
				}
				//doku
				this.ResourceList.ChildrenIterator.currentChild = responseDictionary;
				
				returnValue = this._interpreteAction(VisualObject, ObjectPath + ".forEachChild");
			}
		}else if (ChildrenType === ""){
			HMI.hmi_log_info_onwebsite("ChildrenIterator "+ObjectPath+" is not configured.");
			return false;
		}else{
			//doku multiple values possible
			
			//allow a list of variables as ChildrenTypes
			var ChildrenTypeList = ChildrenType.split(" ");
			var response;
			for (var i=0; i < ChildrenTypeList.length; i++) {
				response = HMI.KSClient.getVar(null, '{'+ FBRef +'.' + ChildrenTypeList[i] + '}', null);
				if (response === false){
					continue;
				}
				//get a rid of external brackets 
				response = response.replace(/{/g, "");
				response = response.replace(/}/g, "");
				var responseArray = HMI.KSClient.splitKsResponse(response, 1);
				
				for (var j=0; j<responseArray.length; j++){
					var responseDictionary = Array();
					responseDictionary["OP_VALUE"] = responseArray[j];
					
					//doku
					this.ResourceList.ChildrenIterator.currentChild = responseDictionary;
					
					returnValue = this._interpreteAction(VisualObject, ObjectPath + ".forEachChild");
				}
			}
		}
		//reset Objects, after iteration we want to have the same (or non) currentChild as before (cascaded iterators)
		this.ResourceList.ChildrenIterator.currentChild = savedCurrentChild;
		this.ResourceList.ChildrenIterator.currentCount = 0;
		savedCurrentChild = null;
		
		return returnValue;
	},
	
	/**
	 * Action which calls _buildFromTemplate to build a template
	 * @param {SVGElement} VisualParentObject visual Object which is parent to active Object
	 * @param {String} ObjectPath Path to this cshmi object containing the event/action/visualisation
	 * @return {Boolean} true on success, false if an error occured
	 */
	_interpreteInstantiateTemplate: function(VisualParentObject, ObjectPath){
		var VisualObject = this._buildFromTemplate(VisualParentObject, ObjectPath, true);
		if (VisualObject !== null){
			VisualParentObject.appendChild(VisualObject);
			//calculate all offset parameter to be able to display visual feedback
			//needed now, because we append new components
			HMI._setLayerPosition(VisualObject);
			//we want to have offset parameter on all visual elements
			var ComponentChilds = VisualObject.getElementsByTagNameNS(HMI.HMI_Constants.NAMESPACE_SVG, '*');
			for(var i = 0;i < ComponentChilds.length;i++){
				HMI._setLayerPosition(ComponentChilds[i]);
			}
			
			var savedCurrentChild = this.ResourceList.ChildrenIterator.currentChild;
			delete this.ResourceList.ChildrenIterator.currentChild;
			
			if (this.initStage === false){
				//interprete onload Actions if we are allready loaded
				
				//for this objects, the init stage should be set (needed for getValue and timeevent)
				this.initStage = true;
				
				while(this.ResourceList.onloadCallStack.length !== 0){
					var EventObjItem = this.ResourceList.onloadCallStack.shift();
					this._interpreteAction(EventObjItem["VisualObject"], EventObjItem["ObjectPath"]);
				}
				//reset stage to real state
				this.initStage = false;
			}
			this.ResourceList.ChildrenIterator.currentChild = savedCurrentChild;
			savedCurrentChild = null;
			
			return true;
		}
		return false;
	},
	
	/**
	 * routes a polyline to connect two points
	 * @param {SVGElement} VisualObject Object to manipulate the visualisation
	 * @param {String} ObjectPath Path to this cshmi object containing the event/action/visualisation
	 * @return {Boolean} true on success, false if an error occured
	 */
	_interpreteRoutePolyline: function(VisualObject, ObjectPath){
		if(VisualObject.parentNode === null){
			//not visualised right now
			return false;
		}
		if(VisualObject.tagName.indexOf("polyline") === -1 ){
			HMI.hmi_log_info_onwebsite("RoutePolyline not supported with: "+VisualObject.tagName+"-Objects (path: "+ObjectPath+")");
			return false;
		}
		var FBRef;
		//if the Object was routed earlier, get the cached information (could be the case with templates or repeated/cyclic calls to the same object)
		
		
		//Not the config is cached here, but the result! So caching VisualObject specific, not ObjectPath
		if (!(VisualObject.ResourceList && VisualObject.ResourceList.Actions && VisualObject.ResourceList.Actions[VisualObject.id] !== undefined)){
			//get Values (via getValue-parts)
			var SourceBasename = this._getValue(VisualObject, ObjectPath+".SourceBasename");
			var SourceVariablename = this._getValue(VisualObject, ObjectPath+".SourceVariablename");
			var TargetBasename = this._getValue(VisualObject, ObjectPath+".TargetBasename");
			var TargetVariablename = this._getValue(VisualObject, ObjectPath+".TargetVariablename");
			
			if (SourceBasename === false || SourceVariablename === false || TargetBasename === false || TargetVariablename === false){
				//an error occured
				return false;
			}else if (SourceBasename === null){
				SourceBasename = "";
			}else if (SourceVariablename === null){
				SourceVariablename = "";
			}else if (TargetBasename === null){
				TargetBasename = "";
			}else if (TargetVariablename === null){
				TargetVariablename = "";
			}
			
			var rootObject = VisualObject;
			//search FBReference of root Object
			while (rootObject !== null && rootObject.namespaceURI == HMI.HMI_Constants.NAMESPACE_SVG){
				//FBReference found
				if(rootObject.FBReference && rootObject.FBReference["default"] !== undefined){
					FBRef = rootObject.FBReference["default"];
					//FBRef found, we can stop search
					rootObject = null;
				}
				else {
					//loop upwards to find the Template object
					rootObject = rootObject.parentNode;
					if (rootObject === null || rootObject.namespaceURI !== HMI.HMI_Constants.NAMESPACE_SVG){
						//if rootObject is null, we use Source and Target to cache
						FBRef = SourceBasename+SourceVariablename+TargetBasename+TargetVariablename;
						}
					}
			}
			rootObject = null;
			
			//we have asked the object successful, so remember the result
			VisualObject.ResourceList = Object();
			VisualObject.ResourceList.Actions = Object();
			VisualObject.ResourceList.Actions[VisualObject.id] = new Object();
			VisualObject.ResourceList.Actions[VisualObject.id].FBRef = FBRef;
			VisualObject.ResourceList.Actions[VisualObject.id].useCount = 1;
			HMI.hmi_log_trace("cshmi._interpreteRoutePolyline: remembering config of "+ObjectPath+" ");
		}else{
			//the object is asked this session, so reuse the config to save communication requests
			FBRef = VisualObject.ResourceList.Actions[VisualObject.id].FBRef;
			VisualObject.ResourceList.Actions[VisualObject.id].useCount++;
			//HMI.hmi_log_trace("cshmi._getValue: using remembered config of "+ObjectPath+" (#"+this.ResourceList.Actions[ObjectPath].useCount+")");
		}
		
		var SourceConnectionPoint = null;
		var SourceConnectionPointdirection = null;
		var TargetConnectionPoint = null;
		var TargetConnectionPointdirection = null;
		var ConnectionOffset = 3; //# pixel offset between connections from same FB
		var OffsetSource = null;
		var OffsetTarget = null;
		
		//todo merge caching!!
		
		
		//if the Polyline was routed earlier, get the cached information (could be the case with cyclic calls)
		if(FBRef !== null && this.ResourceList.Actions && this.ResourceList.Actions[FBRef] !== undefined){
			//the object is asked this session, so reuse the config to save communication requests
			SourceConnectionPoint = this.ResourceList.Actions[FBRef].SourceConnectionPoint;
			SourceConnectionPointdirection = this.ResourceList.Actions[FBRef].SourceConnectionPointdirection;
			TargetConnectionPoint = this.ResourceList.Actions[FBRef].TargetConnectionPoint;
			TargetConnectionPointdirection = this.ResourceList.Actions[FBRef].TargetConnectionPointdirection;
			OffsetSource = this.ResourceList.Actions[FBRef].OffsetSource;
			OffsetTarget = this.ResourceList.Actions[FBRef].OffsetTarget;
			this.ResourceList.Actions[FBRef].useCount++;
			HMI.hmi_log_trace("cshmi._interpreteRoutePolyline: using cached information of "+FBRef+" (#"+this.ResourceList.Actions[FBRef].useCount+")");
		}else{
			//Move Polyline-Container before every Functionblocks
			//needs only to be done once
			if (VisualObject.parentNode.parentNode.tagName === "svg"){
				VisualObject.parentNode.parentNode.insertBefore(VisualObject.parentNode, VisualObject.parentNode.parentNode.firstChild);
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
				//in the svg separators is always /, since it is named via ConfigValue "Name"
				Source = prefix + SourceBasename + "/" + SourceVariablename;
			}else{
				Source = prefix + SourceBasename;
			}
			if (TargetVariablename !== ""){
				//in the svg separators is always /, since it is named via ConfigValue "Name"
				Target = prefix + TargetBasename + "/" + TargetVariablename;
			}else{
				Target = prefix + TargetBasename;
			}
			if (HMI.svgDocument.getElementById(Source) !== null){
				Source = HMI.svgDocument.getElementById(Source);
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
					var domainSVG = VisualObject.parentNode;
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

			if (HMI.svgDocument.getElementById(Target) !== null){
				Target = HMI.svgDocument.getElementById(Target);
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
					var domainSVG = VisualObject.parentNode;
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
			
			//get connection offset 
			if (this.ResourceList.Actions["Connection_from_" + SourceBasename] !== undefined){
				//there is already an incomming connection for this Object
				this.ResourceList.Actions["Connection_from_" + SourceBasename].count += 1;
				OffsetSource = this.ResourceList.Actions["Connection_from_" + SourceBasename].count * ConnectionOffset;
			}
			else{ 
				//1st incomming connection for this Object
				this.ResourceList.Actions["Connection_from_" + SourceBasename] = new Object();
				this.ResourceList.Actions["Connection_from_" + SourceBasename].count = 0;
				OffsetSource = 0;
			}
			if (this.ResourceList.Actions["Connection_to_" + TargetBasename] !== undefined){
				//there is already an outgoing connection for this Object
				this.ResourceList.Actions["Connection_to_" + TargetBasename].count += 1;
				OffsetTarget = this.ResourceList.Actions["Connection_to_" + TargetBasename].count * ConnectionOffset;
			}
			else{
				//1st outgoing connection for this Object
				this.ResourceList.Actions["Connection_to_" + TargetBasename] = new Object();
				this.ResourceList.Actions["Connection_to_" + TargetBasename].count = 0;
				OffsetTarget = 0;
			}

			//remember the result
			this.ResourceList.Actions[FBRef] = new Object();
			this.ResourceList.Actions[FBRef].SourceConnectionPoint = SourceConnectionPoint;
			this.ResourceList.Actions[FBRef].SourceConnectionPointdirection = SourceConnectionPointdirection;
			this.ResourceList.Actions[FBRef].TargetConnectionPoint = TargetConnectionPoint;
			this.ResourceList.Actions[FBRef].TargetConnectionPointdirection = TargetConnectionPointdirection;
			this.ResourceList.Actions[FBRef].OffsetSource = OffsetSource;
			this.ResourceList.Actions[FBRef].OffsetTarget = OffsetTarget;
			this.ResourceList.Actions[FBRef].useCount = 1;
			HMI.hmi_log_trace("cshmi._interpreteRoutePolyline: remembering results for "+FBRef+" ");
		}
		
		if (SourceConnectionPoint === null){
			HMI.hmi_log_info_onwebsite("Routepolyline "+ObjectPath+" could not find the SourceConnectionPoint. Routepolyline or connected objects are wrong configured.");
			return false;
		}else if (TargetConnectionPoint === null){
			HMI.hmi_log_info_onwebsite("Routepolyline "+ObjectPath+" could not find the TargetConnectionPoint. Routepolyline or connected objects are wrong configured.");
			return false;
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
			// add minimum #pixel distance from Object till 1st direction change of connection
			OffsetSource += 40;
			OffsetTarget += 40;
			if (SourceConnectionPointdirection === "Right" && TargetConnectionPointdirection === "Left"){
				//OUTPUT --> INPUT
				points = xStart + "," + yStart + " ";
				xStart = xStart + OffsetSource;
				points = points + xStart + "," + yStart + " ";
				if (xStart >= xEnd) {
					yStart = (yEnd+yStart)/2;
					points = points + xStart + "," + yStart + " ";
					
					xStart = xEnd - OffsetTarget;
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
				xStart = xStart + OffsetTarget;
				points = points + xStart + "," + yStart + " ";
				if (xStart >= xEnd) {
					yStart = (yEnd+yStart)/2;
					points = points + xStart + "," + yStart + " ";
					
					xStart = xEnd - OffsetSource;
					points = points + xStart + "," + yStart + " ";
				}
				
				points = points + xStart + "," + yEnd + " ";
				points = points + xEnd + "," + yEnd;
			}else if (SourceConnectionPointdirection === "Left" && TargetConnectionPointdirection === "Left"){
				//INPUT --> INPUT
				points = xStart + "," + yStart + " ";
				xStart = xStart - OffsetSource;
				points = points + xStart + "," + yStart + " ";
				if (xStart >= xEnd) {
					xStart = xEnd - OffsetTarget;
					points = points + xStart + "," + yStart + " ";
				}
				
				points = points + xStart + "," + yEnd + " ";
				points = points + xEnd + "," + yEnd;
			}else if (SourceConnectionPointdirection === "Right" && TargetConnectionPointdirection === "Right"){
				//OUTPUT --> OUTPUT
				points = xStart + "," + yStart + " ";
				xStart = xStart + OffsetSource;
				points = points + xStart + "," + yStart + " ";
				if (xStart <= xEnd) {
					xStart = xEnd + OffsetTarget;
					points = points + xStart + "," + yStart + " ";
				}
				
				points = points + xStart + "," + yEnd + " ";
				points = points + xEnd + "," + yEnd;
			}else if (SourceConnectionPointdirection === "Up" && TargetConnectionPointdirection === "Down"){
				points = xStart + "," + yStart + " ";
				yStart = yStart - OffsetSource;
				points = points + xStart + "," + yStart + " ";
				if (yStart <= yEnd) {
					xStart = (xEnd+xStart)/2;
					points = points + xStart + "," + yStart + " ";
					
					yStart = yEnd + OffsetTarget;
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
				yStart = yStart - OffsetTarget;
				points = points + xStart + "," + yStart + " ";
				if (yStart <= yEnd) {
					xStart = (xEnd+xStart)/2;
					points = points + xStart + "," + yStart + " ";
					
					yStart = yEnd + OffsetSource;
					points = points + xStart + "," + yStart + " ";
				}
				
				points = points + xEnd + "," + yStart + " ";
				points = points + xEnd + "," + yEnd;
			}else if (SourceConnectionPointdirection === "Down" && TargetConnectionPointdirection === "Down"){
				points = xStart + "," + yStart + " ";
				yStart = yStart + OffsetSource;
				points = points + xStart + "," + yStart + " ";
				if (yStart <= yEnd) {
					yStart = yEnd + OffsetTarget;
					points = points + xStart + "," + yStart + " ";
				}
				
				points = points + xEnd + "," + yStart + " ";
				points = points + xEnd + "," + yEnd;
			}else if (SourceConnectionPointdirection === "Up" && TargetConnectionPointdirection === "Up"){
				points = xStart + "," + yStart + " ";
				yStart = yStart - OffsetSource;
				points = points + xStart + "," + yStart + " ";
				if (yStart >= yEnd) {
					yStart = yEnd - OffsetTarget;
					points = points + xStart + "," + yStart + " ";
				}
				
				points = points + xEnd + "," + yStart + " ";
				points = points + xEnd + "," + yEnd;
			}else if (SourceConnectionPointdirection === "Right" && TargetConnectionPointdirection === "Up"){
				points = xStart + "," + yStart + " ";
				xStart = xStart + OffsetSource;
				points = points + xStart + "," + yStart + " ";
				if (!(yStart < yEnd && xStart < xEnd)){
					yStart = yEnd - OffsetTarget;
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
				xStart = xStart + OffsetTarget;
				points = points + xStart + "," + yStart + " ";
				if (!(yStart < yEnd && xStart < xEnd)){
					yStart = yEnd - OffsetSource;
					points = points + xStart + "," + yStart + " ";
				}
				
				points = points + xEnd + "," + yStart + " ";
				points = points + xEnd + "," + yEnd;
			}else if (SourceConnectionPointdirection === "Right" && TargetConnectionPointdirection === "Down"){
				points = xStart + "," + yStart + " ";
				xStart = xStart + OffsetSource;
				points = points + xStart + "," + yStart + " ";
				if (!(yStart > yEnd && xStart < xEnd)){
					yStart = yEnd + OffsetTarget;
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
				xStart = xStart + OffsetTarget;
				points = points + xStart + "," + yStart + " ";
				if (!(yStart > yEnd && xStart < xEnd)){
					yStart = yEnd + OffsetSource;
					points = points + xStart + "," + yStart + " ";
				}
				points = points + xEnd + "," + yStart + " ";
				points = points + xEnd + "," + yEnd;
			}else if (SourceConnectionPointdirection === "Left" && TargetConnectionPointdirection === "Up"){
				points = xStart + "," + yStart + " ";
				xStart = xStart - OffsetSource;
				points = points + xStart + "," + yStart + " ";
				if (!(yStart < yEnd && xStart > xEnd)){
					yStart = yEnd - OffsetTarget;
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
				xStart = xStart - OffsetTarget;
				points = points + xStart + "," + yStart + " ";
				if (!(yStart < yEnd && xStart > xEnd)){
					yStart = yEnd - OffsetSource;
					points = points + xStart + "," + yStart + " ";
				}
				
				points = points + xEnd + "," + yStart + " ";
				points = points + xEnd + "," + yEnd;
			}else if (SourceConnectionPointdirection === "Left" && TargetConnectionPointdirection === "Down"){
				points = xStart + "," + yStart + " ";
				xStart = xStart - OffsetSource;
				points = points + xStart + "," + yStart + " ";
				if (!(yStart > yEnd && xStart > xEnd)){
					yStart = yEnd + OffsetTarget;
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
				xStart = xStart - OffsetTarget;
				points = points + xStart + "," + yStart + " ";
				if (!(yStart > yEnd && xStart > xEnd)){
					yStart = yEnd + OffsetSource;
					points = points + xStart + "," + yStart + " ";
				}
				points = points + xEnd + "," + yStart + " ";
				points = points + xEnd + "," + yEnd;
			}
			
			VisualObject.setAttribute("points", points);
			
		}else{
			//do nothing because the polyline was routed correctly last time
		}
		return true;
	},
	
	/**
	 * checks Condition
	 * @param {SVGElement} VisualObject Object to manipulate the visualisation
	 * @param {String} ObjectPath Path to this cshmi object containing the event/action/visualisation
	 * @return {Boolean} true if condition matched, false if not matched, null on error
	 */
	_checkCondition: function(VisualObject, ObjectPath, ConditionPath){
		//get Values
		var ignoreError;
		var comptype;
		//if the Object was scanned earlier, get the cached information (could be the case with templates or repeated/cyclic calls to the same object)
		if (!(this.ResourceList.Conditions && this.ResourceList.Conditions[ObjectPath] !== undefined)){
			var requestList = new Object();
			requestList[ObjectPath] = new Object();
			requestList[ObjectPath]["ignoreError"] = null;
			requestList[ObjectPath]["comptype"] = null;
			
			var successCode = this._requestVariablesArray(requestList);
			if (successCode == false){
				return null;
			}
			
			ignoreError = requestList[ObjectPath]["ignoreError"];
			comptype = requestList[ObjectPath]["comptype"];
			
			//feeding garbage collector early
			requestList = null;
			
			//we have asked the object successful, so remember the result
			this.ResourceList.Conditions[ObjectPath] = new Object();
			this.ResourceList.Conditions[ObjectPath].checkConditionIgnoreError = ignoreError;
			this.ResourceList.Conditions[ObjectPath].checkConditionCompType = comptype;
			this.ResourceList.Conditions[ObjectPath].useCount = 1;
			HMI.hmi_log_trace("cshmi._checkCondition: remembering config of "+ObjectPath+" ");
		}else{
			//the object is asked this session, so reuse the config to save communication requests
			comptype = this.ResourceList.Conditions[ObjectPath].checkConditionCompType;
			ignoreError = this.ResourceList.Conditions[ObjectPath].checkConditionIgnoreError;
			this.ResourceList.Conditions[ObjectPath].useCount++;
			//HMI.hmi_log_trace("cshmi._checkCondition: using remembered config of "+ObjectPath+" (#"+this.ResourceList.Conditions[ObjectPath].useCount+")");
		}
		
		
		var Value1 = this._getValue(VisualObject, ObjectPath+".value1");
		var Value2 = this._getValue(VisualObject, ObjectPath+".value2");
		
		if (Value1 === null){
			//getValue had intentionally no value, abort
			return null;
		}else if (Value1 === false && ignoreError === "FALSE"){
				HMI.hmi_log_info("cshmi._checkCondition on "+ObjectPath+" (baseobject: "+VisualObject.id+") failed because Value1 had an error.");
				//error state, so no boolean
				return null;
		}else if (Value1 === false && ignoreError === "TRUE"){
			Value1 = "";
		}
		if (Value2 === null){
			//getValue had intentionally no value, abort
			return null;
		}else if (Value2 === false && ignoreError === "FALSE"){
			HMI.hmi_log_info("cshmi._checkCondition on "+ObjectPath+" (baseobject: "+VisualObject.id+") failed because Value2 had an error.");
			//error state, so no boolean
			return null;
		}else if (Value2 === false && ignoreError === "TRUE"){
			Value2 = "";
		}
		
		//force proper numerical comparision for numbers, since "" < "0" is true in EcmaScript
		if(isNumeric(Value1)){
			Value1 = parseFloat(Value1);
		}
		if(isNumeric(Value2)){
			Value2 = parseFloat(Value2);
		}
		
		if (comptype === "<"){
			return (Value1 < Value2);
		}else if (comptype === "<="){
			return (Value1 <= Value2);
		}else if (comptype === "=="){
			return (Value1 === Value2);
		}else if (comptype === "!="){
			return (Value1 !== Value2);
		}else if (comptype === ">="){
			return (Value1 >= Value2);
		}else if (comptype === ">"){
			return (Value1 > Value2);
		}else{
			HMI.hmi_log_error("cshmi._checkCondition Comparingtype "+comptype+" unknown");
			//error state, so no boolean
			return null;
		}
	},
	
	/**
	 * checks Condition within ChildrenIterator
	 * @param {SVGElement} VisualObject Object to manipulate the visualisation
	 * @param {String} ObjectPath Path to this cshmi object containing the event/action/visualisation
	 * @return {Boolean} true if condition matched, false if not matched, null on error
	 */
	_checkConditionIterator: function(VisualObject, ObjectPath, ConditionPath){
		if (this.ResourceList.ChildrenIterator.currentChild === undefined){
			HMI.hmi_log_info_onwebsite("CompareIteratedChild "+ObjectPath+" is not placed under a Iterator");
			//error state, so no boolean
			return null;
		}
		//get Values
		var ignoreError;
		var comptype;
		var childValue;
		//if the Object was scanned earlier, get the cached information (could be the case with templates or repeated/cyclic calls to the same object)
		if (!(this.ResourceList.Conditions && this.ResourceList.Conditions[ObjectPath] !== undefined)){
			var requestList = new Object();
			requestList[ObjectPath] = new Object();
			requestList[ObjectPath]["ignoreError"] = null;
			requestList[ObjectPath]["comptype"] = null;
			requestList[ObjectPath]["childValue"] = null;
			
			var successCode = this._requestVariablesArray(requestList);
			if (successCode == false){
				return null;
			}
			
			ignoreError = requestList[ObjectPath]["ignoreError"];
			comptype = requestList[ObjectPath]["comptype"];
			childValue = HMI.KSClient.splitKsResponse(requestList[ObjectPath]["childValue"], 0);
			
			//feeding garbage collector early
			requestList = null;
			
			//we have asked the object successful, so remember the result
			this.ResourceList.Conditions[ObjectPath] = new Object();
			this.ResourceList.Conditions[ObjectPath].checkConditionIteratorIgnoreError = ignoreError;
			this.ResourceList.Conditions[ObjectPath].checkConditionIteratorCompType = comptype;
			this.ResourceList.Conditions[ObjectPath].checkConditionIteratorChildValue = childValue;
			this.ResourceList.Conditions[ObjectPath].useCount = 1;
			HMI.hmi_log_trace("cshmi._checkConditionIterator: remembering config of "+ObjectPath+" ");
		}else{
			//the object is asked this session, so reuse the config to save communication requests
			ignoreError = this.ResourceList.Conditions[ObjectPath].checkConditionIteratorIgnoreError;
			comptype = this.ResourceList.Conditions[ObjectPath].checkConditionIteratorCompType;
			childValue = this.ResourceList.Conditions[ObjectPath].checkConditionIteratorChildValue;
			this.ResourceList.Conditions[ObjectPath].useCount++;
			//HMI.hmi_log_trace("cshmi._checkConditionIterator: using remembered config of "+ObjectPath+" (#"+this.ResourceList.Conditions[ObjectPath].useCount+")");
		}
		
		var Value1;
		
		if (childValue[0] === ""){
			HMI.hmi_log_info_onwebsite("CompareIteratedChild "+ObjectPath+" is not configured");
			//error state, so no boolean
			return null;
		}else
		
		//check if we want to get a Value from the iteratedChild
		if (childValue[0].indexOf(".") !== -1){
			HMI.hmi_log_warning("Deprecated use of compareIteratedChild. Compare is now able to do the same! This function will be removed in summer 2012.")
			//##### Deprecated
			
			//found something like childValue : INPUT  STRING = "OP_NAME.flags";
			var rootObject = VisualObject;
			var FBRef;
			//search FBReference of root Object
			while (rootObject !== null && rootObject.namespaceURI == HMI.HMI_Constants.NAMESPACE_SVG){
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
			
			if (this.ResourceList.ChildrenIterator.currentChild["OP_ACCESS"] !== undefined && this.ResourceList.ChildrenIterator.currentChild["OP_ACCESS"].indexOf("KS_AC_PART") !== -1){
				//we have an OV-PART, so the separator is a dot
				Value1 = HMI.KSClient.getVar(null, '{'+ FBRef+"."+this.ResourceList.ChildrenIterator.currentChild[splittedValue[0]]+"."+splittedValue[1] + '}', null);
			}else{
				//we have no OV-PART, so the separator is a slash
				Value1 = HMI.KSClient.getVar(null, '{'+ FBRef+"/"+this.ResourceList.ChildrenIterator.currentChild[splittedValue[0]]+"."+splittedValue[1] + '}', null);
			}
			if ((Value1 === false || Value1 === null) && ignoreError === "FALSE"){
				//communication problem
				HMI.hmi_log_info("cshmi._checkConditionIterator on "+ObjectPath+" (baseobject: "+VisualObject.id+") failed because Value1 is null.");
				//error state, so no boolean
				return null;
			}
			Value1 = Value1.replace(/{/g, "");
			Value1 = Value1.replace(/}/g, "");
			
			//##### end change this code
		}else{
			Value1 = this.ResourceList.ChildrenIterator.currentChild[childValue];
		}
		var Value2 = this._getValue(VisualObject, ObjectPath+".withValue");
		
		if (Value1 === null && ignoreError === "FALSE"){
			//intentionally no value, abort
			return null;
		}else if (Value1 === false && ignoreError === "FALSE"){
			HMI.hmi_log_info("cshmi._checkConditionIterator on "+ObjectPath+" (baseobject: "+VisualObject.id+") failed because Value1 is null.");
			//error state, so no boolean
			return null;
		}else if ((Value1 === false || Value1 === null) && ignoreError === "TRUE"){
			Value1 = "";
		}
		if (Value2 === null){
			//intentionally no value, abort
			return null;
		}
		if (Value2 === null && ignoreError === "FALSE"){
			//getValue had intentionally no value, abort
			return null;
		}else if (Value2 === false && ignoreError === "FALSE"){
			HMI.hmi_log_info("cshmi._checkConditionIterator on "+ObjectPath+" (baseobject: "+VisualObject.id+") failed because Value2 is null.");
			//error state, so no boolean
			return null;
		}else if (Value2 === false && ignoreError === "TRUE"){
			Value2 = "";
		}
		
		Value2 = HMI.KSClient.splitKsResponse(Value2, 0);
		
		//force proper numerical comparision for numbers, since "" < "0" is true in EcmaScript
		if(isNumeric(Value2)){
			Value2 = parseFloat(Value2);
		}
		
		if (comptype === "<"){
			for (var i=0; i<Value2.length; i++){
				if (!(Value1 < Value2[i])){
					return false;
				}
			}
			return true;
		}else if (comptype === "<="){
			for (var i=0; i<Value2.length; i++){
				if (!(Value1 <= Value2[i])){
					return false;
				}
			}
			return true;
		}else if (comptype === "=="){
			//check if one entry of Value2 == Value1
			for (var i=0; i<Value2.length; i++){
				if (Value1 === Value2[i]){
					return true;
				}
			}
			return false;
		}else if (comptype === "!="){
			for (var i=0; i<Value2.length; i++){
				if (!(Value1 !== Value2[i])){
					return false;
				}
			}
			return true;
		}else if (comptype === ">="){
			for (var i=0; i<Value2.length; i++){
				if (!(Value1 >= Value2[i])){
					return false;
				}
			}
			return true;
		}else if (comptype === ">"){
			for (var i=0; i<Value2.length; i++){
				if (!(Value1 > Value2[i])){
					return false;
				}
			}
			return true;
		}else{
			HMI.hmi_log_error("cshmi._checkConditionIterator Comparingtype "+comptype+" unknown");
			//error state, so no boolean
			return null;
		}
	},
	
	
	/**
	 * builds template, gets the parameter via KS
	 * @param {SVGElement} VisualParentObject visual Object which is parent to active Object
	 * @param {String} ObjectPath Path to this cshmi object containing the event/action/visualisation
	 * @param {Boolean} calledFromInstantiateTemplate true if called from an action
	 * @return {SVGElement} VisualObject the new constructed element or null
	 */
	_buildFromTemplate: function(VisualParentObject, ObjectPath, calledFromInstantiateTemplate){
		var requestList;
		//if the Object was scanned earlier, get the cached information (could be the case with templates or repeated/cyclic calls to the same object)
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
				requestList[ObjectPath]["maxTemplatesPerDirection"] = null;
			}
			requestList[ObjectPath]["TemplateDefinition"] = null;
			requestList[ObjectPath]["FBReference"] = null;
			requestList[ObjectPath]["FBVariableReference"] = null;
			requestList[ObjectPath]["ConfigValues"] = null;
			
			var successCode = this._requestVariablesArray(requestList);
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
			//HMI.hmi_log_trace("cshmi._buildFromTemplate: using remembered config of "+ObjectPath+" (#"+this.ResourceList.Elements[ObjectPath].useCount+")");
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
		//if the Object was scanned earlier, get the cached information (could be the case with templates or repeated/cyclic calls to the same object)
		if (!(this.ResourceList.Elements && this.ResourceList.Elements[PathOfTemplateDefinition] !== undefined)){
			requestListTemplate = new Object();
			requestListTemplate[PathOfTemplateDefinition] = new Object();
			requestListTemplate[PathOfTemplateDefinition]["width"] = null;
			requestListTemplate[PathOfTemplateDefinition]["height"] = null;
			requestListTemplate[PathOfTemplateDefinition]["hideable"] = null;
			
			successCode = this._requestVariablesArray(requestListTemplate);
			if (successCode == false){
				HMI.hmi_log_info_onwebsite("Template "+ObjectPath+" is wrong configured. TemplateDefinition '"+TemplateLocation+requestList[ObjectPath]["TemplateDefinition"]+"' is not available.");
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
			//HMI.hmi_log_trace("cshmi._buildFromTemplate: using remembered config of "+PathOfTemplateDefinition+" (#"+this.ResourceList.Elements[PathOfTemplateDefinition].useCount+")");
		}
		
		//make svg Element
		var VisualObject = HMI.svgDocument.createElementNS(HMI.HMI_Constants.NAMESPACE_SVG, 'svg');
		VisualObject.id = ObjectPath;
		VisualObject.setAttribute("TemplateDescription", PathOfTemplateDefinition);
		VisualObject.setAttribute("data-NameOrigin", "TemplateName");
		
		HMI.addClass(VisualObject, this.cshmiTemplateClass);
		
		if (calledFromInstantiateTemplate === true){
			HMI.addClass(VisualObject, this.cshmiTemplateActionClass);
		}
		if (requestListTemplate[PathOfTemplateDefinition]["hideable"] === "TRUE"){
			HMI.addClass(VisualObject, this.cshmiTemplateHideableClass);
			VisualParentObject.setAttribute("cursor", "pointer");
		}
		
		//###########################################################################
		//parametrise templateDefinition with the config
		VisualObject.FBReference = Object();
		VisualObject.FBVariableReference = Object();
		VisualObject.ConfigValues = Object();
		
		
		////////////////////////////////////////////////////////////////////////////
		//FBReference
		
		var FBReferenceList = requestList[ObjectPath]["FBReference"].split(" ");
		var FBReferenceEntry = null;
		for (var i=0; i < FBReferenceList.length; i++) {
			FBReferenceEntry = FBReferenceList[i].split(":");
			if (FBReferenceList[i].charAt(0) === "/"){
				//entry could be an fullpath with an port, so ":" is no separator
				VisualObject.FBReference["default"] = FBReferenceList[i];
				VisualObject.id = VisualObject.FBReference["default"];
				VisualObject.setAttribute("data-NameOrigin", "FBReference");
			}else if (FBReferenceEntry.length >= 2){
				
				//extract the varname and remerge the rest into one arrayentry (a servername can contain : as a port separator)
				var VarName = FBReferenceEntry.shift();
				FBReferenceEntry = [VarName, FBReferenceEntry.join(":")];
				
				HMI.hmi_log_warning("Deprecated use of FBReference. Please use FBVariableReference! This function will be removed in summer 2012.")
				
				//##### Deprecated: change this code (named objects should use FBVarRef!), this should be named FBrefs
				//check if we want to get values from the current child (e.g. OP_NAME)
				//if instantiateTemplate is not called within a childreniterator, the currentChild is undefined
				if (calledFromInstantiateTemplate && this.ResourceList.ChildrenIterator.currentChild !== undefined && this.ResourceList.ChildrenIterator.currentChild[FBReferenceEntry[1]] !== undefined){
					var rootObject = VisualParentObject;
					var FBRef;
					//search FBReference of root Object
					while (rootObject !== null && rootObject.namespaceURI == HMI.HMI_Constants.NAMESPACE_SVG){
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
					VisualObject.FBReference[FBReferenceEntry[0]] = FBRef + "." + this.ResourceList.ChildrenIterator.currentChild[FBReferenceEntry[1]];
				}
				else{
					VisualObject.FBReference[FBReferenceEntry[0]] = FBReferenceEntry[1];
				}
				//##### end change this code (named objects should use FBVarRef!)
			}else if (FBReferenceEntry.length === 1 && FBReferenceEntry[0] != ""){
				//only one info was requested, so we can save it to the default position. 
				
				if (calledFromInstantiateTemplate && this.ResourceList.ChildrenIterator.currentChild !== undefined && this.ResourceList.ChildrenIterator.currentChild[FBReferenceEntry[0]] !== undefined){
					//something like OP_NAME was requested, so we have to find the real info from the iterator
					var rootObject = VisualParentObject;
					var FBRef;
					//search FBReference of root Object
					while (rootObject !== null && rootObject.namespaceURI == HMI.HMI_Constants.NAMESPACE_SVG){
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
					if (this.ResourceList.ChildrenIterator.currentChild[FBReferenceEntry[0]].charAt(0) === "/"){
						// the iterated string begins with / so it is a fullpath (likely from a GetVar on an assoziation)
						
						// if FBref begins with "//" then keep the server information
						// e.g "//dev/ov_hmidemo7/TechUnits/Add" --> keep "//dev/ov_hmidemo7"
						if (FBRef.charAt(0) === "/" && FBRef.charAt(1) === "/"){
							//find the 3rd "/"
							var slashIndex = FBRef.indexOf("/", 2);
							//find the 4th "/"
							slashIndex = FBRef.indexOf("/", slashIndex+1);
							//only keep the String before 4th "/"
							var ServerName = FBRef.slice(0, slashIndex);
							
							// it's a path to another server, so we must add the serverName
							VisualObject.FBReference["default"] = ServerName + this.ResourceList.ChildrenIterator.currentChild[FBReferenceEntry[0]];
						}else{
							// it's not a path to another server, so we must replace the full FBRef path
							VisualObject.FBReference["default"] = this.ResourceList.ChildrenIterator.currentChild[FBReferenceEntry[0]];
						}
						VisualObject.id = VisualObject.FBReference["default"]
						VisualObject.setAttribute("data-NameOrigin", "CurrentChildfullpath");
					}else{
						//In OP_NAME is a relative path (likely from a GetEP request). We have to use the correct separator
						if (this.ResourceList.ChildrenIterator.currentChild["OP_ACCESS"].indexOf("KS_AC_PART") !== -1){
							//we have an OV-PART, so the separator is a dot
							VisualObject.FBReference["default"] = FBRef + "." + this.ResourceList.ChildrenIterator.currentChild[FBReferenceEntry[0]];
						}else{
							//we have no OV-PART, so the separator is a slash
							VisualObject.FBReference["default"] = FBRef + "/" + this.ResourceList.ChildrenIterator.currentChild[FBReferenceEntry[0]];
						}
						VisualObject.id = FBRef + "/" + this.ResourceList.ChildrenIterator.currentChild[FBReferenceEntry[0]];
						VisualObject.setAttribute("data-NameOrigin", "FBReference/OP_NAME");
					}
				}else{
					//We have straightforward a full name of one FB Object, so save it with the default name
					if (FBReferenceEntry[0].charAt(0) === "/"){
						//full path is given
						VisualObject.FBReference["default"] = FBReferenceEntry[0];
						VisualObject.setAttribute("data-NameOrigin", "FBReference");
					}else{
						//relative path is given, so complete the path with the BaseKsPath
						VisualObject.FBReference["default"] = this._getBaseKsPath(VisualObject, ObjectPath) + FBReferenceEntry[0];
						VisualObject.setAttribute("data-NameOrigin", "FBReference+BaseKsPath");
					}
					VisualObject.id = VisualObject.FBReference["default"];
				}
			}
		}
		
		
		////////////////////////////////////////////////////////////////////////////
		//FBVariableReference
		
		var FBVariableReferenceList = requestList[ObjectPath]["FBVariableReference"].split(" ");
		var FBVariableReferenceEntry = null;
		for (var i=0; i < FBVariableReferenceList.length; i++) {
			FBVariableReferenceEntry = FBVariableReferenceList[i].split(":");
			if (FBVariableReferenceEntry.length <= 1){
				//a FBVariable must be always be named, so this is no valid entry
				continue;
			}
			if (FBVariableReferenceEntry.length >= 2){
				//extract the varname and remerge the rest into one arrayentry (a servername can contain : as a port separator)
				var VarName = FBVariableReferenceEntry.shift();
				FBVariableReferenceEntry = [VarName, FBVariableReferenceEntry.join(":")];
			}
			
			//if instantiateTemplate is not called within a childreniterator, the currentChild is undefined
			if (calledFromInstantiateTemplate && this.ResourceList.ChildrenIterator.currentChild !== undefined && this.ResourceList.ChildrenIterator.currentChild[FBVariableReferenceEntry[1]] !== undefined){
				//we want to get values from the current child (e.g. myFavoriteVar:OP_NAME)
				
				if (this.ResourceList.ChildrenIterator.currentChild[FBVariableReferenceEntry[1]].charAt(0) !== "/"){
					//Value from currentChild is not a full path, so search for it in the parents
					
					var rootObject = VisualParentObject;
					var FBRef = "";
					//search FBReference of root Object
					while (rootObject !== null && rootObject.namespaceURI == HMI.HMI_Constants.NAMESPACE_SVG){
						//FBVariableReference found
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
					VisualObject.FBVariableReference[FBVariableReferenceEntry[0]] = FBRef + "." + this.ResourceList.ChildrenIterator.currentChild[FBVariableReferenceEntry[1]];
				}else{
					//currentChild set a full path
					VisualObject.FBVariableReference[FBVariableReferenceEntry[0]] = this.ResourceList.ChildrenIterator.currentChild[FBVariableReferenceEntry[1]];
				}
			}else{
				//direct setting of a FBVariable
				if (FBVariableReferenceEntry[1].charAt(0) === "/"){
					//full path is given
					VisualObject.FBVariableReference[FBVariableReferenceEntry[0]] = FBVariableReferenceEntry[1];
				}else{
					//relative path is given, so complete the path with the BaseKsPath
					VisualObject.FBVariableReference[FBVariableReferenceEntry[0]] = this._getBaseKsPath(VisualObject, ObjectPath) + FBVariableReferenceEntry[1];
				}
			}
		}
		
		////////////////////////////////////////////////////////////////////////////
		//ConfigValue
		
		//ConfigValue is something like "pumpcolor:yellow pumpname:N18"
		var ConfigValueList = requestList[ObjectPath]["ConfigValues"].split(" ");
		var ConfigValueEntry = null;
		var lastEntry = null;
		for (var i=0; i < ConfigValueList.length; i++) {
			ConfigValueEntry = ConfigValueList[i].split(":");
			if (ConfigValueEntry.length === 2){
				//check if we want to get values from the current child (e.g. OP_NAME)
				//if instantiateTemplate is not called within a childreniterator, the currentChild is undefined
				if (calledFromInstantiateTemplate && this.ResourceList.ChildrenIterator.currentChild !== undefined && this.ResourceList.ChildrenIterator.currentChild[ConfigValueEntry[1]] !== undefined){
					VisualObject.ConfigValues[ConfigValueEntry[0]] = this.ResourceList.ChildrenIterator.currentChild[ConfigValueEntry[1]];
				}
				else{
					VisualObject.ConfigValues[ConfigValueEntry[0]] = ConfigValueEntry[1];
				}
				//doku
				if(ConfigValueEntry[0] === "Name"){
					//if an template has the configValue Name:Dieter, this should be the object id
					if (VisualParentObject.getAttribute("data-NameOrigin") !== "TemplateName"){
						//if the parent was named, append our name
						VisualObject.id = VisualParentObject.id + "/" + VisualObject.ConfigValues[ConfigValueEntry[0]];
						VisualObject.setAttribute("data-NameOrigin", "Parent+ConfigValue");
					}else{
						//our parent was named with an TemplateName, append our Name
						VisualObject.id = VisualObject.ConfigValues[ConfigValueEntry[0]];
						VisualObject.setAttribute("data-NameOrigin", "ConfigValue");
					}
				}
				lastEntry = ConfigValueEntry[0];
			}else if (ConfigValueEntry.length === 1 && lastEntry !== null){
				//we had something like "pumpcolor:yellow pumpname:N 18", so need to add the " 18" to the last entry
				VisualObject.ConfigValues[lastEntry] = VisualObject.ConfigValues[lastEntry]+" "+ConfigValueEntry[0];
			}
		}
		
		
		/////////////////////////////////////////////////////////////////////////////
		//Iterator position changing
		
		//save Template configuration for caching
		var xTemplate = requestList[ObjectPath]["x"];
		var yTemplate = requestList[ObjectPath]["y"];
		
		if (calledFromInstantiateTemplate && this.ResourceList.ChildrenIterator.currentChild !== undefined){
			//the offsetCount must be global for all InstantiateTemplate below an iterator
			var offsetCount = this.ResourceList.ChildrenIterator.currentCount;
			//the next InstantiateTemplate should go to an other position
			this.ResourceList.ChildrenIterator.currentCount++;
			var offsetCountX;
			var offsetCountY;
			var direction = requestList[ObjectPath]["maxTemplatesPerDirection"].split(":");
			if(direction.length == 2 && direction[0] === "x"){
				// we have an maximum in a line
				offsetCountX = offsetCount % parseInt(direction[1]);
				offsetCountY = Math.floor(offsetCount / parseInt(direction[1]));
			}else if(direction.length == 2 && direction[0] === "y"){
				// we have an maximum in a row
				offsetCountX = Math.floor(offsetCount / parseInt(direction[1]));
				offsetCountY = offsetCount % parseInt(direction[1]);
			}else{
				//use both offsets
				offsetCountX = offsetCount;
				offsetCountY = offsetCount;
			}
			var x = parseFloat(requestList[ObjectPath]["x"]) + (offsetCountX * parseFloat(requestList[ObjectPath]["xOffset"]));
			var y = parseFloat(requestList[ObjectPath]["y"]) + (offsetCountY * parseFloat(requestList[ObjectPath]["yOffset"]));
			
			requestList[ObjectPath]["x"] = x.toString();
			requestList[ObjectPath]["y"] = y.toString();
		}
		
		//////////////////////////////////////////////////////////////////////////
		//adjust visual appearance
		
		//setting the basic Element Variables like .visible .stroke .fill .opacity .rotate
		this._processBasicVariables(VisualObject, requestList[ObjectPath]);
		if (requestList[ObjectPath]["rotate"] !== "0"){
			//rotate is not specified with a svg-Element, so encapsule in a G-Element
			//http://www.w3.org/Graphics/SVG/WG/track/issues/2252
			var VisualChildObject = HMI.svgDocument.createElementNS(HMI.HMI_Constants.NAMESPACE_SVG, 'g');
			VisualChildObject.setAttribute("overflow", "visible");
			VisualChildObject.setAttribute("transform", "rotate("+requestList[ObjectPath]["rotate"]+","+requestList[ObjectPath]["x"]+","+requestList[ObjectPath]["y"]+")");
			VisualChildObject.appendChild(VisualObject);
		}
		
		//restore Template config
		requestList[ObjectPath]["x"] = xTemplate;
		requestList[ObjectPath]["y"] = yTemplate;
		
		//width and height comes from the TemplateDefinition
		VisualObject.setAttribute("width", requestListTemplate[PathOfTemplateDefinition]["width"]);
		VisualObject.setAttribute("height", requestListTemplate[PathOfTemplateDefinition]["height"]);
		VisualObject.setAttribute("overflow", "visible");
		
		
		//////////////////////////////////////////////////////////////////////////
		//get childs (grafics and actions) from the TemplateDefinition
		//our child will be fetched later
		var responseArrayChild = HMI.KSClient.getChildObjArray(PathOfTemplateDefinition, this);
		for (var i=0; i < responseArrayChild.length; i++) {
			var varName = responseArrayChild[i].split(" ");
			var ChildComponent = this.BuildDomain(VisualObject, PathOfTemplateDefinition+"/"+varName[0], varName[1]);
			if (ChildComponent !== null){
				VisualObject.appendChild(ChildComponent);
			}
		}
		var preserveThis = this;	//grabbed from http://jsbin.com/etise/7/edit
		//toggle visibility of hideable childtemplates onclick
		VisualParentObject.addEventListener("click", function(evt){
			preserveThis.toggleChildTemplates(VisualParentObject);
			
			//quit propagation of event in any case. We do not want the parent template to handle the click
			if (evt.stopPropagation) evt.stopPropagation();
		}, false);
		
		if (VisualChildObject){
			//transformed Template for rotation
			return VisualChildObject;
		}else{
			return VisualObject;
		}
	},
	
	/**
	 * toggles visibility of child Templates
	 * @param {Node} VisualParentObject Node which childs are toggled
	 * @return void
	 */
	toggleChildTemplates: function(VisualParentObject){
		var childTemplates = VisualParentObject.childNodes;
		
		for (var i=0; i < childTemplates.length; i++) {
			var Classes = childTemplates[i].getAttribute("class");
			if (Classes === null){
				continue;
			}else if (Classes.indexOf(this.cshmiTemplateHideableClass) === -1){
				continue;
			}else if (childTemplates[i].getAttribute("display") == "block"){
				childTemplates[i].setAttribute("display", "none");
			}else{
				childTemplates[i].setAttribute("display", "block");
				
				
				//doku depth of moving to top
				
				//Move Faceplate-Container after every other, so it is fully visible
				if (HMI.instanceOf(VisualParentObject, this.cshmiTemplateActionClass) === false){
					//hideable childtemplate in a normal template
					VisualParentObject.parentNode.appendChild(VisualParentObject);
				}else{
					//hideable childtemplate in an action instantiated template (one level more)
					VisualParentObject.parentNode.parentNode.appendChild(VisualParentObject.parentNode);
				}
			}
		}
	},

	/***************************************************************************************************************
	 * builds SVG container, gets the parameter via KS
	 * @param {SVGElement} VisualParentObject visual Object which is parent to active Object
	 * @param {String} ObjectPath Path to this cshmi object containing the event/action/visualisation
	 * @return {SVGElement} VisualObject the new constructed element or null
	 */
	_buildSvgContainer: function(VisualParentObject, ObjectPath){
		var requestList;
		requestList = new Object();
		requestList[ObjectPath] = new Object();
		requestList[ObjectPath]["x"] = null;
		requestList[ObjectPath]["y"] = null;
		requestList[ObjectPath]["width"] = null;
		requestList[ObjectPath]["height"] = null;
		
		var successCode = this._requestVariablesArray(requestList);
		if (successCode == false){
			return null;
		}
		
		var VisualObject = HMI.svgDocument.createElementNS(HMI.HMI_Constants.NAMESPACE_SVG, 'svg');
		VisualObject.id = ObjectPath;
		
		HMI.addClass(VisualObject, this.cshmiGroupClass);
		
		//set dimension of container
		VisualObject.setAttribute("x", requestList[ObjectPath]["x"]);
		VisualObject.setAttribute("y", requestList[ObjectPath]["y"]);
		VisualObject.setAttribute("width", requestList[ObjectPath]["width"]);
		VisualObject.setAttribute("height", requestList[ObjectPath]["height"]);
		
		VisualObject.setAttribute("overflow", "visible");
		
		return VisualObject;
	},
	
	/**
	 * builds SVG line object, gets the parameter via KS
	 * @param {SVGElement} VisualParentObject visual Object which is parent to active Object
	 * @param {String} ObjectPath Path to this cshmi object containing the event/action/visualisation
	 * @return {SVGElement} VisualObject the new constructed element or null
	 */
	_buildSvgLine: function(VisualParentObject, ObjectPath){
		var requestList;
		//if the Object was scanned earlier, get the cached information (could be the case with templates or repeated/cyclic calls to the same object)
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
			requestList[ObjectPath]["strokeWidth"] = null;
			
			var successCode = this._requestVariablesArray(requestList);
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
			//HMI.hmi_log_trace("cshmi._buildSvgLine: using remembered config of "+ObjectPath+" (#"+this.ResourceList.Elements[ObjectPath].useCount+")");
		}
		
		var VisualObject = HMI.svgDocument.createElementNS(HMI.HMI_Constants.NAMESPACE_SVG, 'line');
		VisualObject.id = ObjectPath;
		
		//setting the basic Element Variables like .visible .stroke .fill .opacity .rotate
		this._processBasicVariables(VisualObject, requestList[ObjectPath]);
		
		VisualObject.setAttribute("x1", requestList[ObjectPath]["x1"]);
		VisualObject.setAttribute("y1", requestList[ObjectPath]["y1"]);
		VisualObject.setAttribute("x2", requestList[ObjectPath]["x2"]);
		VisualObject.setAttribute("y2", requestList[ObjectPath]["y2"]);
		
		VisualObject.setAttribute("shape-rendering", "crispEdges");
		
		return VisualObject;
	},
	
	/**
	 * builds SVG polyline object, gets the parameter via KS
	 * @param {SVGElement} VisualParentObject visual Object which is parent to active Object
	 * @param {String} ObjectPath Path to this cshmi object containing the event/action/visualisation
	 * @return {SVGElement} VisualObject the new constructed element or null
	 */
	_buildSvgPolyline: function(VisualParentObject, ObjectPath){
		var requestList;
		//if the Object was scanned earlier, get the cached information (could be the case with templates or repeated/cyclic calls to the same object)
		if (!(this.ResourceList.Elements && this.ResourceList.Elements[ObjectPath] !== undefined)){
			requestList = new Object();
			requestList[ObjectPath] = new Object();
			requestList[ObjectPath]["visible"] = null;
			requestList[ObjectPath]["stroke"] = null;
			requestList[ObjectPath]["fill"] = null;
			requestList[ObjectPath]["opacity"] = null;
			requestList[ObjectPath]["rotate"] = null;
			requestList[ObjectPath]["points"] = null;
			requestList[ObjectPath]["strokeWidth"] = null;
			
			var successCode = this._requestVariablesArray(requestList);
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
			//HMI.hmi_log_trace("cshmi._buildSvgPolyline: using remembered config of "+ObjectPath+" (#"+this.ResourceList.Elements[ObjectPath].useCount+")");
		}
		
		var VisualObject = HMI.svgDocument.createElementNS(HMI.HMI_Constants.NAMESPACE_SVG, 'polyline');
		VisualObject.id = ObjectPath;
		
		//setting the basic Element Variables like .visible .stroke .fill .opacity .rotate
		this._processBasicVariables(VisualObject, requestList[ObjectPath]);
		
		VisualObject.setAttribute("points", requestList[ObjectPath]["points"]);
		
		VisualObject.setAttribute("shape-rendering", "crispEdges");

		return VisualObject;
	},
	
	/**
	 * builds SVG polygon object, gets the parameter via KS
	 * @param {SVGElement} VisualParentObject visual Object which is parent to active Object
	 * @param {String} ObjectPath Path to this cshmi object containing the event/action/visualisation
	 * @return {SVGElement} VisualObject the new constructed element or null
	 */
	_buildSvgPolygon: function(VisualParentObject, ObjectPath){
		var requestList;
		//if the Object was scanned earlier, get the cached information (could be the case with templates or repeated/cyclic calls to the same object)
		if (!(this.ResourceList.Elements && this.ResourceList.Elements[ObjectPath] !== undefined)){
			requestList = new Object();
			requestList[ObjectPath] = new Object();
			requestList[ObjectPath]["visible"] = null;
			requestList[ObjectPath]["stroke"] = null;
			requestList[ObjectPath]["fill"] = null;
			requestList[ObjectPath]["opacity"] = null;
			requestList[ObjectPath]["rotate"] = null;
			requestList[ObjectPath]["points"] = null;
			requestList[ObjectPath]["strokeWidth"] = null;
			
			var successCode = this._requestVariablesArray(requestList);
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
			//HMI.hmi_log_trace("cshmi._buildSvgPolygon: using remembered config of "+ObjectPath+" (#"+this.ResourceList.Elements[ObjectPath].useCount+")");
		}
		
		var VisualObject = HMI.svgDocument.createElementNS(HMI.HMI_Constants.NAMESPACE_SVG, 'polygon');
		VisualObject.id = ObjectPath;
		
		//setting the basic Element Variables like .visible .stroke .fill .opacity .rotate
		this._processBasicVariables(VisualObject, requestList[ObjectPath]);
		
		VisualObject.setAttribute("points", requestList[ObjectPath]["points"]);
		
		VisualObject.setAttribute("shape-rendering", "crispEdges");
		
		return VisualObject;
	},
	
	/**
	 * builds SVG path object, gets the parameter via KS
	 * @param {SVGElement} VisualParentObject visual Object which is parent to active Object
	 * @param {String} ObjectPath Path to this cshmi object containing the event/action/visualisation
	 * @return {SVGElement} VisualObject the new constructed element or null
	 */
	_buildSvgPath: function(VisualParentObject, ObjectPath){
		var requestList;
		//if the Object was scanned earlier, get the cached information (could be the case with templates or repeated/cyclic calls to the same object)
		if (!(this.ResourceList.Elements && this.ResourceList.Elements[ObjectPath] !== undefined)){
			requestList = new Object();
			requestList[ObjectPath] = new Object();
			requestList[ObjectPath]["visible"] = null;
			requestList[ObjectPath]["stroke"] = null;
			requestList[ObjectPath]["fill"] = null;
			requestList[ObjectPath]["opacity"] = null;
			requestList[ObjectPath]["rotate"] = null;
			requestList[ObjectPath]["d"] = null;
			requestList[ObjectPath]["strokeWidth"] = null;
			
			var successCode = this._requestVariablesArray(requestList);
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
			//HMI.hmi_log_trace("cshmi._buildSvgPath: using remembered config of "+ObjectPath+" (#"+this.ResourceList.Elements[ObjectPath].useCount+")");
		}
		
		var VisualObject = HMI.svgDocument.createElementNS(HMI.HMI_Constants.NAMESPACE_SVG, 'path');
		VisualObject.id = ObjectPath;
		
		//setting the basic Element Variables like .visible .stroke .fill .opacity .rotate
		this._processBasicVariables(VisualObject, requestList[ObjectPath]);
		
		VisualObject.setAttribute("d", requestList[ObjectPath]["d"]);
		
		VisualObject.setAttribute("shape-rendering", "crispEdges");
		
		return VisualObject;
		
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
	
	/**
	 * builds SVG Text object, gets the parameter via KS
	 * @param {SVGElement} VisualParentObject visual Object which is parent to active Object
	 * @param {String} ObjectPath Path to this cshmi object containing the event/action/visualisation
	 * @return {SVGElement} VisualObject the new constructed element or null
	 */
	_buildSvgText: function(VisualParentObject, ObjectPath){
		var requestList;
		//if the Object was scanned earlier, get the cached information (could be the case with templates or repeated/cyclic calls to the same object)
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
			
			var successCode = this._requestVariablesArray(requestList);
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
			//HMI.hmi_log_trace("cshmi._buildSvgText: using remembered config of "+ObjectPath+" (#"+this.ResourceList.Elements[ObjectPath].useCount+")");
		}
		
		var VisualObject = HMI.svgDocument.createElementNS(HMI.HMI_Constants.NAMESPACE_SVG, 'text');
		VisualObject.id = ObjectPath;
		
		//setting the basic Element Variables like .visible .stroke .fill .opacity .rotate
		this._processBasicVariables(VisualObject, requestList[ObjectPath]);
		VisualObject.setAttribute("font-size", requestList[ObjectPath]["fontSize"]);
		VisualObject.setAttribute("font-style", requestList[ObjectPath]["fontStyle"]);
		VisualObject.setAttribute("font-weight", requestList[ObjectPath]["fontWeight"]);
		VisualObject.setAttribute("font-family", requestList[ObjectPath]["fontFamily"]);
		VisualObject.setAttribute("text-anchor", requestList[ObjectPath]["horAlignment"]);
		
		var svgTspan = HMI.svgDocument.createElementNS(HMI.HMI_Constants.NAMESPACE_SVG, 'tspan');
		//perform trimming if needed
		var trimLength = parseInt(requestList[ObjectPath]["trimToLength"], 10);
		var contentLength = parseInt(requestList[ObjectPath]["content"].length, 10);
		var trimmedContent;
		
		if(trimLength > 0 && isNumeric(requestList[ObjectPath]["content"])){
			//we have a numeric NewValue
			if(requestList[ObjectPath]["content"].indexOf(".") === -1){
				//INT or UINT
				svgTspan.appendChild(HMI.svgDocument.createTextNode(requestList[ObjectPath]["content"]));
			}else{
				//other values
				var power = Math.pow(10, trimLength || 0);
				trimmedContent = String(Math.round(NewValue * power) / power);
				
				svgTspan.appendChild(HMI.svgDocument.createTextNode(trimmedContent));
				this._setTitle(VisualObject, NewValue);
			}
		}else if((trimLength > 0) && (contentLength > trimLength)){
			trimmedContent = requestList[ObjectPath]["content"].substr(0, trimLength) + String.fromCharCode(8230);
			svgTspan.appendChild(HMI.svgDocument.createTextNode(trimmedContent));
			this._setTitle(VisualObject, requestList[ObjectPath]["content"]);
		}else if((trimLength < 0) && (contentLength > -trimLength)){
			trimmedContent =  String.fromCharCode(8230) + requestList[ObjectPath]["content"].substr(contentLength + trimLength);
			svgTspan.appendChild(HMI.svgDocument.createTextNode(trimmedContent));
			this._setTitle(VisualObject, requestList[ObjectPath]["content"]);
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
		
		VisualObject.appendChild(svgTspan);
		
		return VisualObject;
	},
	
	/**
	 * builds SVG circle object, gets the parameter via KS
	 * @param {SVGElement} VisualParentObject visual Object which is parent to active Object
	 * @param {String} ObjectPath Path to this cshmi object containing the event/action/visualisation
	 * @return {SVGElement} VisualObject the new constructed element or null
	 */
	_buildSvgCircle: function(VisualParentObject, ObjectPath){
		var requestList;
		//if the Object was scanned earlier, get the cached information (could be the case with templates or repeated/cyclic calls to the same object)
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
			requestList[ObjectPath]["strokeWidth"] = null;
			
			var successCode = this._requestVariablesArray(requestList);
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
			//HMI.hmi_log_trace("cshmi._buildSvgCircle: using remembered config of "+ObjectPath+" (#"+this.ResourceList.Elements[ObjectPath].useCount+")");
		}
		
		var VisualObject = HMI.svgDocument.createElementNS(HMI.HMI_Constants.NAMESPACE_SVG, 'circle');
		VisualObject.id = ObjectPath;
		
		//setting the basic Element Variables like .visible .stroke .fill .opacity .rotate
		this._processBasicVariables(VisualObject, requestList[ObjectPath]);
		VisualObject.setAttribute("cx", requestList[ObjectPath]["cx"]);
		VisualObject.setAttribute("cy", requestList[ObjectPath]["cy"]);
		VisualObject.setAttribute("r",requestList[ObjectPath]["r"]);
		
		return VisualObject;
	},
	
	/**
	 * builds SVG ellipse object, gets the parameter via KS
	 * @param {SVGElement} VisualParentObject visual Object which is parent to active Object
	 * @param {String} ObjectPath Path to this cshmi object containing the event/action/visualisation
	 * @return {SVGElement} VisualObject the new constructed element or null
	 */
	_buildSvgEllipse: function(VisualParentObject, ObjectPath){
		var requestList;
		//if the Object was scanned earlier, get the cached information (could be the case with templates or repeated/cyclic calls to the same object)
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
			requestList[ObjectPath]["strokeWidth"] = null;
			
			var successCode = this._requestVariablesArray(requestList);
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
			//HMI.hmi_log_trace("cshmi._buildSvgEllipse: using remembered config of "+ObjectPath+" (#"+this.ResourceList.Elements[ObjectPath].useCount+")");
		}
		
		var VisualObject = HMI.svgDocument.createElementNS(HMI.HMI_Constants.NAMESPACE_SVG, 'ellipse');
		VisualObject.id = ObjectPath;
		
		//setting the basic Element Variables like .visible .stroke .fill .opacity .rotate
		this._processBasicVariables(VisualObject, requestList[ObjectPath]);
		VisualObject.setAttribute("cx", requestList[ObjectPath]["cx"]);
		VisualObject.setAttribute("cy", requestList[ObjectPath]["cy"]);
		VisualObject.setAttribute("rx",requestList[ObjectPath]["rx"]);
		VisualObject.setAttribute("ry",requestList[ObjectPath]["ry"]);
		
		//rotation should be around cx and cy
		VisualObject.setAttribute("transform", "rotate("+requestList[ObjectPath]["rotate"]+","+requestList[ObjectPath]["cx"]+","+requestList[ObjectPath]["cy"]+")");
		
		return VisualObject;
	},
	
	/**
	 * builds SVG rect object, gets the parameter via KS
	 * @param {SVGElement} VisualParentObject visual Object which is parent to active Object
	 * @param {String} ObjectPath Path to this cshmi object containing the event/action/visualisation
	 * @return {SVGElement} VisualObject the new constructed element or null
	 */
	_buildSvgRect: function(VisualParentObject, ObjectPath){
		var requestList;
		//if the Object was scanned earlier, get the cached information (could be the case with templates or repeated/cyclic calls to the same object)
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
			requestList[ObjectPath]["strokeWidth"] = null;
			
			var successCode = this._requestVariablesArray(requestList);
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
			//HMI.hmi_log_trace("cshmi._buildSvgRect: using remembered config of "+ObjectPath+" (#"+this.ResourceList.Elements[ObjectPath].useCount+")");
		}
		
		var VisualObject = HMI.svgDocument.createElementNS(HMI.HMI_Constants.NAMESPACE_SVG, 'rect');
		VisualObject.id = ObjectPath;
		
		//setting the basic Element Variables like .visible .stroke .fill .opacity .rotate
		this._processBasicVariables(VisualObject, requestList[ObjectPath]);
		
		VisualObject.setAttribute("shape-rendering", "crispEdges");
		
		return VisualObject;
	},
	
	/**
	 * builds SVG image object, gets the parameter via KS
	 * @param {SVGElement} VisualParentObject visual Object which is parent to active Object
	 * @param {String} ObjectPath Path to this cshmi object containing the event/action/visualisation
	 * @return {SVGElement} VisualObject the new constructed element or null
	 */
	_buildSvgImage: function(VisualParentObject, ObjectPath){
		var requestList;
		//if the Object was scanned earlier, get the cached information (could be the case with templates or repeated/cyclic calls to the same object)
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
			
			var successCode = this._requestVariablesArray(requestList);
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
			//HMI.hmi_log_trace("cshmi._buildSvgRect: using remembered config of "+ObjectPath+" (#"+this.ResourceList.Elements[ObjectPath].useCount+")");
		}
		
		var VisualObject;
		if(requestList[ObjectPath]["SVGcontent"] !== ""){
			//we have SVG Content to visualise
			//
			//the code could use an xlink, svg or no XML prefix at all.
			var svgContent =	"<svg:svg xmlns:svg=\""+HMI.HMI_Constants.NAMESPACE_SVG+"\" xmlns=\""+HMI.HMI_Constants.NAMESPACE_SVG+"\" "
				+"xmlns:xlink='http://www.w3.org/1999/xlink'>"
				+requestList[ObjectPath]["SVGcontent"]
				+"</svg:svg>";
			VisualObject = HMI.HMIDOMParser.parse(svgContent, null);
			VisualObject.setAttribute("overflow", "visible");
		}else if(requestList[ObjectPath]["Bitmapcontent"] !== ""){
			//we have an Bitmap Content to visualise
			//
			
			var bitmapContent =	"<svg:image xmlns:svg=\""+HMI.HMI_Constants.NAMESPACE_SVG+"\" "
				+"xmlns:xlink='http://www.w3.org/1999/xlink' xlink:href='"
				+requestList[ObjectPath]["Bitmapcontent"]
				+"'></svg:image>";
			VisualObject = HMI.HMIDOMParser.parse(bitmapContent, null);
			
			/* better but not working approach
			var VisualObject = HMI.svgDocument.createElementNS(HMI.HMI_Constants.NAMESPACE_SVG, 'image');
			
			VisualParentObject.setAttribute("xmlns:xlink", HMI.HMI_Constants.NAMESPACE_XLINK);
			//this causes an error, so the image is not displayed
			//VisualParentObject.setAttribute("xmlns", HMI.HMI_Constants.NAMESPACE_SVG);
			VisualObject.setAttribute("xlink:href", requestList[ObjectPath]["Bitmapcontent"]);
			VisualObject.setAttribute("width", requestList[ObjectPath]["width"]);
			VisualObject.setAttribute("height", requestList[ObjectPath]["height"]);
			*/
		}else{
			HMI.hmi_log_info_onwebsite("Image "+ObjectPath+" is not configured");
			return null;
		}
		VisualObject.id = ObjectPath;
		//setting the basic Element Variables like .visible .stroke .fill .opacity .rotate
		this._processBasicVariables(VisualObject, requestList[ObjectPath]);
		
		return VisualObject;
	},
	
	/**
	 * sets svg attributes from an Array
	 * @param {SVGElement} VisualObject Object to manipulate the visualisation
	 * @param {Array} configArray array with a list of thing to set
	 * @return {Boolean} true
	 */
	_processBasicVariables: function(VisualObject, configArray){
		if (configArray["visible"] && configArray["visible"] == "FALSE"){
			VisualObject.setAttribute("display", "none");
		}else{
			VisualObject.setAttribute("display", "block");
		}
		if(configArray["x"] && configArray["y"]){
			//the attribute should be "rotate(deg, x, y)"
			VisualObject.setAttribute("transform", "rotate("+configArray["rotate"]+","+configArray["x"]+","+configArray["y"]+")");
			VisualObject.setAttribute("x", configArray["x"]);
			VisualObject.setAttribute("y", configArray["y"]);
		}else{
			VisualObject.setAttribute("transform", "rotate("+configArray["rotate"]+")");
		}
		if(configArray["width"] && configArray["width"] !== ""){
			VisualObject.setAttribute("width", configArray["width"]);
		}
		if(configArray["height"] && configArray["height"] !== ""){
			VisualObject.setAttribute("height", configArray["height"]);
		}
		if(configArray["stroke"] && configArray["stroke"] !== ""){
			VisualObject.setAttribute("stroke", configArray["stroke"]);
		}
		if(configArray["strokeWidth"] && configArray["strokeWidth"] !== ""){
			VisualObject.setAttribute("stroke-width", configArray["strokeWidth"]);
		}
		if(configArray["fill"] && configArray["fill"] !== ""){
			VisualObject.setAttribute("fill", configArray["fill"]);
			if (configArray["fill"].indexOf("url(") !== -1){
				//opera has a Bug in May 2012 (v11+v12 beta), so the pointer-event is not correct in this case (buttons from processcontrol)
				VisualObject.setAttribute('pointer-events', 'visible');
			}
		}
		if(configArray["opacity"] && configArray["opacity"] !== ""){
			VisualObject.setAttribute("opacity", configArray["opacity"]);
		}
		return true;
	},
	
	/**
	 * Requests a list of OV-Variables from multiple OV-Objects
	 * @this main cshmi object
	 * @param {Array} requestList List of multiple Variables to fetch
	 * @param {Boolean} reportError Should an error be reported on screen?
	 * @return {Boolean} true on success, false if an error occured
	 */
	_requestVariablesArray: function(requestList, reportError){
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
		}else if (response.indexOf("KS_ERR_BADPATH") !== -1 && reportError === true){
			HMI.hmi_log_onwebsite("Sorry, your cshmi server is not supported, because the base model was changed. Please upgrade to the newest cshmi library. Don't forget to export your server.");
			HMI.hmi_log_error("cshmi._requestVariablesArray of "+requestString+" failed: "+response);
			return false;
		}else if (response.indexOf("KS_ERR") !== -1){
			HMI.hmi_log_error("cshmi._requestVariablesArray of "+requestString+" failed: "+response);
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
	
	/**
	 * sets an SVG title on an object
	 * @param {SVGElement} VisualObject Object to manipulate the visualisation
	 * @param {String} newText Text for the title
	 * @return {Boolean} true
	 */
	_setTitle: function(VisualObject, newText){
		var titles = VisualObject.getElementsByTagNameNS(HMI.HMI_Constants.NAMESPACE_SVG, 'title');
		if (titles.length >0){
			titles[0].replaceChild(HMI.svgDocument.createTextNode(newText), titles[0].firstChild);
		}else{
			var svgTitle = HMI.svgDocument.createElementNS(HMI.HMI_Constants.NAMESPACE_SVG, 'title');
			svgTitle.appendChild(HMI.svgDocument.createTextNode(newText));
			VisualObject.appendChild(svgTitle);
		}
		return true;
	},
	
	/**
	 * returns a recursive list of all elements with a requested class
	 * @param node node to start the search
	 * @param {String} newText a string representing the list of class names to match; class names are separated by whitespace
	 * @return {NodeList} returnElements a live NodeList (but see the note below) of found elements in the order they appear in the tree.
	 */
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
