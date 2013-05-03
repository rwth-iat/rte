/*
*	Copyright (C) 2013
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
	this.ResourceList.GlobalVar = Object();
	this.ResourceList.EventInfos = Object();
	this.ResourceList.EventInfos.EventObj = null;
	this.ResourceList.EventInfos.mouseRelativePosition = null;
	this.ResourceList.EventInfos.startXMouse = null;
	this.ResourceList.EventInfos.startYMouse = null;
	this.ResourceList.EventInfos.startXObj = null;
	this.ResourceList.EventInfos.startYObj = null;
	this.ResourceList.newRebuildObjectId = null;
	
	this.trashDocument = null;
	if(HMI.svgDocument.implementation && HMI.svgDocument.implementation.createDocument){
		this.trashDocument = HMI.svgDocument.implementation.createDocument (HMI.HMI_Constants.NAMESPACE_SVG, "html", null);
	}
	
	//newwrite
	//this.ResourceList.ModellVariables = Object();
	//this.ResourceList.ModellVariables.getValue = ["ksVar", "elemVar", "globalVar", "persistentGlobalVar", "OperatorInput", "TemplateFBReferenceVariable", "TemplateFBVariableReferenceName", "TemplateConfigValues", "value"];
	
	//holds the information if the visualisation is filled with content right now
	this.initStage = false;
	
	//we want to add all elements to a class to find it later
	this.cshmiGroupClass = "cshmi-group";
	this.cshmiBlackboxClass = "cshmi-blackbox";
	this.cshmiTemplateClass = "cshmi-template";
	this.cshmiTemplateActionClass = "cshmi-fromTemplateAction";
	this.cshmiTemplateHideableClass = "cshmi-hideabletemplate";
	this.cshmiObjectHasHideableChildren = "cshmi-ObjectHasHideableChildren";
	this.cshmiObjectVisibleChildrenNotLoaded = "cshmi-ObjectVisibleChildrenNotLoaded";
	
	this.cshmiOperatorClickClass = "cshmi-click";
	this.cshmiOperatorDoubleclickClass = "cshmi-doubleclick";
	this.cshmiOperatorRightclickClass = "cshmi-rightclick";
	this.cshmiOperatorAftermoveClass = "cshmi-aftermove";
};





/*#########################################################################################################################
TODO: 

JavaScript:
- check return value of gethandleid

var varName = responseArray[i].split(" ");
varName[1] evtl nicht verf�gbar!

- Nach dem Laden (nur gebrauchter Objecte), sollte ein Hintergrund thread die restliche Config nachladen
- Auch das laden sollte asyncrone requests nutzen. 
	Daf�r muss die Verarbeitungsreihenfolge innerhalb eines Events jedoch fest bleiben
- Nur einige wenige cycTimes (enum like?) erlauben
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
	instanciateCshmi: function(Host, Server, ObjectPath) {
		//fill cache if possible
		var response = HMI.KSClient.getVar("/TechUnits/cshmi/turbo.asJSON");
		if (response !== null && response.indexOf("KS_ERR") === -1){
			try {
				var responseJSON = JSON.parse(decodeURI(response.slice(2,-2)));
			} catch (e) {
				//something went wrong, ignore the cache 8-/
				responseJSON = {};
			}
			if(responseJSON.Elements){
				this.ResourceList.Elements = responseJSON.Elements;
			}
			if(responseJSON.Actions){
				this.ResourceList.Actions = responseJSON.Actions;
			}
			if(responseJSON.Conditions){
				this.ResourceList.Conditions = responseJSON.Conditions;
			}
			if(responseJSON.Events){
				this.ResourceList.Events = responseJSON.Events;
			}
			if(responseJSON.baseKsPath){
				this.ResourceList.baseKsPath = responseJSON.baseKsPath;
			}
			if(responseJSON.ChildList){
				this.ResourceList.ChildList = responseJSON.ChildList;
			}
		}
		
		//we are in the init stage, so the DOM Tree is not populated
		this.initStage = true;
		
		//build the selected sheet aka group. This includes all containing elements
		var VisualObject = this.BuildDomain(null, ObjectPath, "/cshmi/Group", false);
		
		if (VisualObject === null){
			HMI.hmi_log_onwebsite('No sheet with name  "'+ObjectPath+'" available.');
			return;
		}
		if(HMI.PlaygroundContainerNode){
			//the displayed size is calculated from the Container-Node in the html, so we correct the dimension of it
			HMI.PlaygroundContainerNode.setAttribute('height', VisualObject.getAttribute('height'));
			HMI.PlaygroundContainerNode.setAttribute('width', VisualObject.getAttribute('width'));
		}
		
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
		var maxPosition = HMI.saveAbsolutePosition(VisualObject, true);
		//we want to have offset parameter on all visual elements
		var ComponentChilds = VisualObject.getElementsByTagNameNS(HMI.HMI_Constants.NAMESPACE_SVG, "*");
		for(var i = 0;i < ComponentChilds.length;i++){
			var Position = HMI.saveAbsolutePosition(ComponentChilds[i], false);
		}
		Position = null;
		ComponentChilds = null;
		
		this._interpreteOnloadCallStack();
		
		/*
		var invisibleObjectName = "";
		var ComponentChilds = VisualObject.getElementsByTagName("svg");
		for(var i = 0;i < ComponentChilds.length;i++){
			var elementWidth = parseInt(ComponentChilds[i].getAttribute("width"), 10)+parseInt(ComponentChilds[i].getAttribute("absolutex"), 10);
			var elementHeight = parseInt(ComponentChilds[i].getAttribute("height"), 10)+parseInt(ComponentChilds[i].getAttribute("absolutey"), 10);
			
			if (!isNaN(elementWidth) && elementWidth > maxPosition[0]){
				//wider than the last
				maxPosition[0] = elementWidth;
				invisibleObjectName = ComponentChilds[i].id;
			}
			if (!isNaN(elementHeight) && elementHeight > maxPosition[1]){
				//higher than the last
				maxPosition[1] = elementHeight;
				invisibleObjectName = ComponentChilds[i].id;
			}
		}
		
		if (maxPosition[0] > VisualObject.getAttribute('width') || maxPosition[1] > VisualObject.getAttribute('height')){
			HMI.hmi_log_info_onwebsite("Warning: There is a chance, that there is content outside your view. Last known candidate is: "+invisibleObjectName);
		}
		maxPosition = null;
		invisibleObjectName = null;
		*/
		
		//the DOM Tree is populated now
		this.initStage = false;
	},
	
	/**
	 * Main iteration loop for visualisation, finds and arms Actions as well
	 * @param {SVGElement} VisualParentObject visual Object which is parent to active Object
	 * @param {String} ObjectPath Path to this cshmi object containing the event/action/visualisation
	 * @param {bool} preventNetworkRequest the function should prevent network requests if possible
	 * @return {SVGElement} VisualObject the new constructed element or null
	 */
	BuildDomain: function(VisualParentObject, ObjectPath, ObjectType, preventNetworkRequest){
		var VisualObject = null;
		var Result = true;
		if (ObjectType.indexOf("/cshmi/Group") !== -1){
			VisualObject = this._buildSvgGroup(VisualParentObject, ObjectPath, preventNetworkRequest);
		}else if (ObjectType.indexOf("/cshmi/Blackbox") !== -1){
			VisualObject = this._buildBlackbox(VisualParentObject, ObjectPath, preventNetworkRequest);
		}else if (ObjectType.indexOf("/cshmi/Template") !== -1){
			VisualObject = this._buildFromTemplate(VisualParentObject, ObjectPath, false, preventNetworkRequest);
		}else if (ObjectType.indexOf("/cshmi/Path") !== -1){
			VisualObject = this._buildSvgPath(VisualParentObject, ObjectPath, preventNetworkRequest);
		}else if (ObjectType.indexOf("/cshmi/Line") !== -1){
			VisualObject = this._buildSvgLine(VisualParentObject, ObjectPath, preventNetworkRequest);
		}else if (ObjectType.indexOf("/cshmi/Polyline") !== -1){
			VisualObject = this._buildSvgPolyline(VisualParentObject, ObjectPath, preventNetworkRequest);
		}else if (ObjectType.indexOf("/cshmi/Polygon") !== -1){
			VisualObject = this._buildSvgPolygon(VisualParentObject, ObjectPath, preventNetworkRequest);
		}else if (ObjectType.indexOf("/cshmi/Text") !== -1){
			VisualObject = this._buildSvgText(VisualParentObject, ObjectPath, preventNetworkRequest);
		}else if (ObjectType.indexOf("/cshmi/Circle") !== -1){
			VisualObject = this._buildSvgCircle(VisualParentObject, ObjectPath, preventNetworkRequest);
		}else if (ObjectType.indexOf("/cshmi/Ellipse") !== -1){
			VisualObject = this._buildSvgEllipse(VisualParentObject, ObjectPath, preventNetworkRequest);
		}else if (ObjectType.indexOf("/cshmi/Rectangle") !== -1){
			VisualObject = this._buildSvgRect(VisualParentObject, ObjectPath, preventNetworkRequest);
		}else if (ObjectType.indexOf("/cshmi/Image") !== -1){
			VisualObject = this._buildSvgImage(VisualParentObject, ObjectPath, preventNetworkRequest);
		}else if (ObjectType.indexOf("/cshmi/ClientEvent") !== -1){
			Result = this._interpreteClientEvent(VisualParentObject, ObjectPath, preventNetworkRequest);
		}else if (ObjectType.indexOf("/cshmi/TimeEvent") !== -1){
			Result = this._interpreteTimeEvent(VisualParentObject, ObjectPath, preventNetworkRequest);
		}else if (ObjectType.indexOf("/cshmi/OperatorEvent") !== -1){
			Result = this._interpreteOperatorEvent(VisualParentObject, ObjectPath, preventNetworkRequest);
		}else if (ObjectType.indexOf("/cshmi/SetValue") !== -1 && HMI.instanceOf(VisualParentObject, "cshmi-blackbox")){
			// SetValue is ok to be Child of Blackbox
			return null;
		}else{
			HMI.hmi_log_info_onwebsite("Object (Typ: "+ObjectType+"): "+ObjectPath+" not supported");
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
			//remember the ObjectType on every object (needed for reloading via action)
			VisualObject.setAttribute("data-ObjectType", ObjectType);
			
			// special handling for invisible objects
			if (VisualObject.getAttribute("display") === "none"){
				//we are not visible, so we should not contact network
				this._loadChildren(VisualObject, ObjectPath, true);
				
				// mark objects incomplete AFTER initialisation
				// the event registering require this to prevent duplicate registration
				HMI.addClass(VisualObject, this.cshmiObjectVisibleChildrenNotLoaded);
			}else if (VisualObject.hasAttribute("display") === true){
				this._loadChildren(VisualObject, ObjectPath, false);
			}
		}
		
		return VisualObject;
	},
	
	/**
	 * calling Actions if supported ClientEvent is triggered
	 * @param {SVGElement} VisualObject Object to manipulate the visualisation
	 * @param {String} ObjectPath Path to this cshmi object containing the event/action/visualisation
	 * @param {bool} preventNetworkRequest the function should prevent network requests if possible
	 * @return {Boolean} true
	 */
	_interpreteClientEvent: function(VisualObject, ObjectPath, preventNetworkRequest){
		if (VisualObject !== null && HMI.instanceOf(VisualObject, this.cshmiObjectVisibleChildrenNotLoaded)){
			//this is the run that fills the objects with content, the Events were already armed
			return true;
		}
		
		var command = ObjectPath.split("/");
		if (command[command.length-1] === "onload"){
			//interprete Action later, so remember this
			var EventObjItem = Object();
			EventObjItem["VisualObject"] = VisualObject;
			EventObjItem["ObjectPath"] = ObjectPath;
			this.ResourceList.onloadCallStack.push(EventObjItem);
			
			/* not necessary
			VisualObject.addEventListener("DOMNodeRemoved", function(evt){
				if(evt.target !== VisualObject){
					return;
				}
				//remove ourself out of the globalvarchangeStack
				for (var i = 0; i < HMI.cshmi.ResourceList.onloadCallStack.length;){
					var EventObjItem = HMI.cshmi.ResourceList.onloadCallStack[i];
					if(EventObjItem["VisualObject"] === VisualObject){
						//we found an old entry, remove
						HMI.cshmi.ResourceList.onloadCallStack.splice(i, 1);
					}else{
						i++;
					}
				}
			}, false);
			*/
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
	 * calling Actions if supported TimeEvent is triggered
	 * @param {SVGElement} VisualObject Object to manipulate the visualisation
	 * @param {String} ObjectPath Path to this cshmi object containing the event/action/visualisation
	 * @param {bool} preventNetworkRequest the function should prevent network requests if possible
	 * @return {Boolean} true
	 */
	_interpreteTimeEvent: function(VisualObject, ObjectPath, preventNetworkRequest){
		if (VisualObject.ownerDocument === this.trashDocument || HMI.cshmi !== this){
			//our object are no active anymore. Removed from DOM or 
			//the active cshmi display is not "our" one
			return true;
		}
		if (VisualObject !== null && HMI.instanceOf(VisualObject, this.cshmiObjectVisibleChildrenNotLoaded)){
			//this is the run that fills the objects with content, the Events were already armed
			return true;
		}
		
		var skipEvent = false;
		//check if the page is visible at all?
		//http://www.w3.org/TR/page-visibility/
		//never used with ms or o prefix
		if(	document.hidden === true ||
			document.mozHidden === true||
			document.webkitHidden === true
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
			if (successCode === false){
				return null;
			}
			
			cyctime = requestList[ObjectPath]["cyctime"];
			
			//feeding garbage collector early
			requestList = null;
			
			//we have asked the object successful, so remember the result
			this.ResourceList.Events[ObjectPath] = new Object();
			this.ResourceList.Events[ObjectPath].TimeEventParameterCyctime = cyctime;
		}else{
			//the object was asked this session, so reuse the config to save communication requests
			cyctime = this.ResourceList.Events[ObjectPath].TimeEventParameterCyctime;
		}
		
		//call us again for cyclic interpretation of the Actions
		//only if we are in the initialisation or normal stage
		if (this.initStage === true || HMI.Playground.firstChild !== null ){
			//call a function, which is manipulating this keyword
			window.setTimeout(function() {
				//the function will be executed in the context (this) of the HMI.cshmi object
				HMI.cshmi._interpreteTimeEvent(VisualObject, ObjectPath);
			}, cyctime*1000);
		}
		return true;
	},
	
	/**
	 * detect all OperatorEvents and register them
	 * @param {SVGElement} VisualObject Object to manipulate the visualisation
	 * @param {String} ObjectPath Path to this cshmi object containing the event/action/visualisation
	 * @param {bool} preventNetworkRequest the function should prevent network requests if possible
	 * @return {Boolean} true
	 */
	_interpreteOperatorEvent: function(VisualObject, ObjectPath, preventNetworkRequest){
		if (VisualObject !== null && HMI.instanceOf(VisualObject, this.cshmiObjectVisibleChildrenNotLoaded)){
			//this is the run that fills the objects with content, the Events were already armed
			return true;
		}
		
		var command = ObjectPath.split("/");
		if (command[command.length-1] === "click"){
			VisualObject.setAttribute("cursor", "pointer");
			HMI.addClass(VisualObject, this.cshmiOperatorClickClass);
			VisualObject.setAttribute("data-clickpath", ObjectPath);
			VisualObject.addEventListener("click", function(evt){
				if(HMI.instanceOf(VisualObject, HMI.cshmi.cshmiOperatorAftermoveClass)){
					//we have an movegesture on the same VisualObject, so this will handle all action
					return;
				}
				
				//"this" is here the event target object, so work on HMI.cshmi
				HMI.cshmi.ResourceList.EventInfos.EventObj = evt;
				//mark changed VisualObject for quick visual feedback (hidden after a second)
				HMI.displaygestureReactionMarker(VisualObject);
				
				//toggle visibility of hideable childtemplates
				HMI.cshmi.toggleChildTemplates(VisualObject);
				
				//get and execute all actions
				HMI.cshmi._interpreteAction(VisualObject, ObjectPath);
				
				//an later action should not interprete this event
				HMI.cshmi.ResourceList.EventInfos.EventObj = null;
				if (evt.stopPropagation) evt.stopPropagation();
			}, false);
		}else if (command[command.length-1] === "doubleclick"){
			VisualObject.setAttribute("cursor", "pointer");
			HMI.addClass(VisualObject, this.cshmiOperatorDoubleclickClass);
			VisualObject.setAttribute("data-doubleclickpath", ObjectPath);
			VisualObject.addEventListener("dblclick", function(evt){
				HMI.cshmi.ResourceList.EventInfos.EventObj = evt;
				//mark changed VisualObject for quick visual feedback (hidden after a second)
				HMI.displaygestureReactionMarker(VisualObject);
				
				//get and execute all actions
				HMI.cshmi._interpreteAction(VisualObject, ObjectPath);
				
				//an later action should not interprete this event
				HMI.cshmi.ResourceList.EventInfos.EventObj = null;
				if (evt.stopPropagation) evt.stopPropagation();
			}, false);
		}else if (command[command.length-1] === "rightclick"){
			VisualObject.setAttribute("cursor", "pointer");
			HMI.addClass(VisualObject, this.cshmiOperatorRightclickClass);
			VisualObject.setAttribute("data-rightclickpath", ObjectPath);
			VisualObject.addEventListener("contextmenu", function(evt){
				HMI.cshmi.ResourceList.EventInfos.EventObj = evt;
				//mark changed VisualObject for quick visual feedback (hidden after a second)
				HMI.displaygestureReactionMarker(VisualObject);
				
				//get and execute all actions
				HMI.cshmi._interpreteAction(VisualObject, ObjectPath);
				
				//an later action should not interprete this event
				HMI.cshmi.ResourceList.EventInfos.EventObj = null;
				
				if (evt.stopPropagation) evt.stopPropagation();
				if (evt.preventDefault) evt.preventDefault();  //default is a context menu, so disable it
			}, false);
		}else if (command[command.length-1] === "mouseover"){
			var Eventname = "mouseover";
			if ('onmouseenter' in HMI.svgDocument.documentElement){
				//mouseenter is far better, than mouseover
				Eventname = "mouseenter";
			}
			
			VisualObject.addEventListener(Eventname, function(evt){
				HMI.cshmi.ResourceList.EventInfos.EventObj = evt;
				
				//get and execute all actions
				HMI.cshmi._interpreteAction(VisualObject, ObjectPath);
				
				//an later action should not interprete this event
				HMI.cshmi.ResourceList.EventInfos.EventObj = null;
				
				if (evt.stopPropagation) evt.stopPropagation();
			}, false);
		}else if (command[command.length-1] === "mouseout"){
			//mouseleave is far better, than mouseout
			var Eventname = "mouseout";
			if ('onmouseleave' in HMI.svgDocument.documentElement){
				//mouseenter is far better, than mouseover
				Eventname = "mouseleave";
			}
			
			VisualObject.addEventListener(Eventname, function(evt){
				HMI.cshmi.ResourceList.EventInfos.EventObj = evt;
				
				//get and execute all actions
				HMI.cshmi._interpreteAction(VisualObject, ObjectPath);
				
				//an later action should not interprete this event
				HMI.cshmi.ResourceList.EventInfos.EventObj = null;
				
				if (evt.stopPropagation) evt.stopPropagation();
			}, false);
		}else if (command[command.length-1] === "aftermove"){
			if(VisualObject.getAttribute("x") === null || VisualObject.getAttribute("y") === null){
				HMI.hmi_log_info_onwebsite("OperatorEvent aftermove "+ObjectPath+" is not under an object with x,y coordinates. Aborting.");
				return false;
			}
			VisualObject.setAttribute("cursor", "move");
			HMI.addClass(VisualObject, this.cshmiOperatorAftermoveClass);
			VisualObject.setAttribute("data-aftermovepath", ObjectPath);
			
			//make an dummy wrapper function so we can use VisualObject and ObjectPath in it, 
			//addEventListener only provides the event object
			VisualObject._moveStartDragThunk = function(evt){
				HMI.cshmi._moveStartDrag(VisualObject, ObjectPath, evt);
			};
			VisualObject._moveMouseMoveThunk = function(evt){
				HMI.cshmi._moveMouseMove(VisualObject, ObjectPath, evt);
			};
			VisualObject._moveStopDragThunk = function(evt){
				//stop with interpreting the actions
				HMI.cshmi._moveStopDrag(VisualObject, ObjectPath, evt, false);
			};
			VisualObject._moveCancelDragThunk = function(evt){
				//stop without interpreting the actions
				HMI.cshmi._moveStopDrag(VisualObject, ObjectPath, evt, true);
			};
			VisualObject._moveHandleClickThunk = function(evt){
				//stop the propagation
				HMI.cshmi._moveHandleClick(VisualObject, ObjectPath, evt, true);
			};
			
			//todo: try to implement via HTML5 drag&drop
			//http://blogs.msdn.com/b/ie/archive/2011/10/19/handling-multi-touch-and-mouse-input-in-all-browsers.aspx
			
			//try both, mousedown and mousetouch. mousetouch will fire first, there we will kill mousedown
			VisualObject.addEventListener("touchstart", VisualObject._moveStartDragThunk, false);
			VisualObject.addEventListener("mousedown", VisualObject._moveStartDragThunk, false);
			VisualObject.addEventListener("MSPointerDown", VisualObject._moveStartDragThunk, false);
		}else{
			HMI.hmi_log_info_onwebsite("OperatorEvent ("+command[command.length-1]+") "+ObjectPath+" not supported");
		}
		
		if (this.ResourceList.Event && this.ResourceList.Event[ObjectPath] !== undefined){
			requestList[ObjectPath] = new Object();
			//we have registered the object successful, so remember the result
			this.ResourceList.Elements[ObjectPath] = new Object();
			this.ResourceList.Elements[ObjectPath].Parameters = requestList[ObjectPath];
		}
		return true;
	},
	
	/**
	 * prepares the drag and drop handling via mousemove etc
	 * @param {SVGElement} VisualObject Object to manipulate the visualisation
	 * @param {String} ObjectPath Path to this cshmi object containing the event/action/visualisation
	 * @param {DOMEvent} evt event object
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
		var mouseposition = HMI.getClickPosition(evt, HMI.Playground.firstChild);
		this.ResourceList.EventInfos.startXMouse = mouseposition[0];
		this.ResourceList.EventInfos.startYMouse = mouseposition[1];
		this.ResourceList.EventInfos.mouseRelativePosition = HMI.getClickPosition(evt, VisualObject);
		this.ResourceList.EventInfos.startXObj = parseInt(VisualObject.getAttribute("x"), 10);
		this.ResourceList.EventInfos.startYObj = parseInt(VisualObject.getAttribute("y"), 10);
		
		if(evt.type === 'touchstart'){
			HMI.hmi_log_trace("moveStartDrag - touch (x:"+mouseposition[0]+",y:"+mouseposition[1]+") detected, killing legacy events");
			//we have touch gestures, so kill legacy mousedown
			VisualObject.removeEventListener("mousedown", VisualObject._moveStartDragThunk, false);
			
			HMI.svgDocument.addEventListener("touchmove", VisualObject._moveMouseMoveThunk, false);
			HMI.svgDocument.addEventListener("touchend", VisualObject._moveStopDragThunk, false);
			HMI.svgDocument.addEventListener("touchcancel", VisualObject._moveCancelDragThunk, false);
		}else if(evt.type === 'MSPointerDown'){
			//we have touch gestures, so kill legacy mousedown (and w3c touchstart)
			VisualObject.removeEventListener("mousedown", VisualObject._moveStartDragThunk, false);
			VisualObject.removeEventListener("touchstart", VisualObject._moveStartDragThunk, false);
			
			HMI.svgDocument.addEventListener("MSPointerMove", VisualObject._moveMouseMoveThunk, false);
			HMI.svgDocument.addEventListener("MSPointerUp", VisualObject._moveStopDragThunk, false);
			HMI.svgDocument.addEventListener("MSPointerCancel", VisualObject._moveCancelDragThunk, false);
		}else{
			HMI.hmi_log_trace("moveStartDrag - legacy click (x:"+mouseposition[0]+",y:"+mouseposition[1]+") detected");
			HMI.svgDocument.addEventListener("mousemove", VisualObject._moveMouseMoveThunk, false);
			HMI.svgDocument.addEventListener("mouseup", VisualObject._moveStopDragThunk, false);
		}
		//we do not want to propagate a click to the parents
		VisualObject.addEventListener("click", VisualObject._moveHandleClickThunk, false);
		
		if (evt.stopPropagation) evt.stopPropagation();
	},
	
	/**
	 * stops propagation of event
	 * @param {SVGElement} VisualObject Object to manipulate the visualisation
	 * @param {String} ObjectPath Path to this cshmi object containing the event/action/visualisation
	 * @param {DOMEvent} evt event object
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
	 * @param {DOMEvent} evt event object
	 * @return nothing
	 */
	_moveMouseMove : function(VisualObject, ObjectPath, evt){
		if (this.ResourceList.EventInfos.startXObj === null){
			return;
		}
		var mouseposition = HMI.getClickPosition(evt, HMI.Playground.firstChild);
		var mouseX = mouseposition[0];
		var mouseY = mouseposition[1];
		var newx = this.ResourceList.EventInfos.startXObj+mouseX-this.ResourceList.EventInfos.startXMouse;
		var newy = this.ResourceList.EventInfos.startYObj+mouseY-this.ResourceList.EventInfos.startYMouse;
		if (!isNaN(newx) && !isNaN(newy)){
			VisualObject.setAttribute("x", newx);
			VisualObject.setAttribute("y", newy);
			
			HMI.saveAbsolutePosition(VisualObject);
			//we want to have offset parameter on all visual elements
			var ComponentChilds = VisualObject.getElementsByTagNameNS(HMI.HMI_Constants.NAMESPACE_SVG, '*');
			for(var i = 0;i < ComponentChilds.length;i++){
				HMI.saveAbsolutePosition(ComponentChilds[i]);
			}
			//save event for use in an action
			this.ResourceList.EventInfos.EventObj = evt;
		}
		if (evt.stopPropagation) evt.stopPropagation();
		if (evt.preventDefault) evt.preventDefault();  //default is scrolling, so disable it
		if (evt.preventManipulation) evt.preventManipulation(); //stop panning and zooming in ie10
	},
	
	/**
	 * stops the drag and call needed actions
	 * @param {SVGElement} VisualObject Object to manipulate the visualisation
	 * @param {String} ObjectPath Path to this cshmi object containing the event/action/visualisation
	 * @param {DOMEvent} evt event object
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
		}else if(evt.type === 'MSPointerUp'){
			HMI.hmi_log_trace("moveStartDrag - MS touch up detected");
			HMI.svgDocument.removeEventListener("MSPointerMove", VisualObject._moveMouseMoveThunk, false);
			HMI.svgDocument.removeEventListener("MSPointerUp", VisualObject._moveStopDragThunk, false);
			HMI.svgDocument.removeEventListener("MSPointerCancel", VisualObject._moveCancelDragThunk, false);
			//the touchend has no xy position (since the fingers left the device!), so an action should work on the last move eventobj
		}else{
			HMI.hmi_log_trace("moveStartDrag - legacy mouse up detected");
			HMI.svgDocument.removeEventListener("mousemove", VisualObject._moveMouseMoveThunk, false);
			HMI.svgDocument.removeEventListener("mouseup", VisualObject._moveStopDragThunk, false);
			//the mouseup event has xy position, so remember for use in an action
			this.ResourceList.EventInfos.EventObj = evt;
		}
		
		//check if object is clickable and the movement is marginal
		if(HMI.instanceOf(VisualObject, this.cshmiOperatorClickClass)
			&& (Math.abs(this.ResourceList.EventInfos.startXObj - VisualObject.getAttribute("x")) < 5)
			&& (Math.abs(this.ResourceList.EventInfos.startYObj - VisualObject.getAttribute("y")) < 5)){
			
			//no movement detected, so interprete the click
			var interpreteEvent = "click";
		}
		
		//restore old position
		VisualObject.setAttribute("x", this.ResourceList.EventInfos.startXObj);
		VisualObject.setAttribute("y", this.ResourceList.EventInfos.startYObj);
		HMI.saveAbsolutePosition(VisualObject);
		//we want to have offset parameter on all visual elements
		var ComponentChilds = VisualObject.getElementsByTagNameNS(HMI.HMI_Constants.NAMESPACE_SVG, '*');
		for(var i = 0;i < ComponentChilds.length;i++){
			HMI.saveAbsolutePosition(ComponentChilds[i]);
		}
		
		if (canceled === true){
			//no action
		}else if (interpreteEvent === "click"){
			this._interpreteAction(VisualObject, VisualObject.getAttribute("data-clickpath"));
		}else{
			//get and execute all actions
			this._interpreteAction(VisualObject, ObjectPath);
		}
		
		//an later action should not interprete this event
		this.ResourceList.EventInfos.EventObj = null;
		this.ResourceList.EventInfos.mouseRelativePosition = null;
		
		if (evt.stopPropagation) evt.stopPropagation();
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
		
		//newwrite
		//fetch config from all childrens via this.ResourceList.ModellVariables.*
		
		//a server could be not available, or we request a not existing (xpos) variable. This should not cancel the other actions
		for (var i=0; i < responseArray.length; i++) {
			var varName = responseArray[i].split(" ");
			if (varName[1].indexOf("/cshmi/SetValue") !== -1){
				returnValue = this._setValue(VisualObject, ObjectPath+"/"+varName[0], "static");
			}else if (varName[1].indexOf("/cshmi/SetConcatValue") !== -1){
				returnValue = this._setValue(VisualObject, ObjectPath+"/"+varName[0], "concat");
			}else if (varName[1].indexOf("/cshmi/SetMathValue") !== -1){
				returnValue = this._setValue(VisualObject, ObjectPath+"/"+varName[0], "math");
			}else if (varName[1].indexOf("/cshmi/GetValue") !== -1){
				HMI.hmi_log_info_onwebsite("GetValue Action ("+varName[1]+")"+ObjectPath+" not useful at this position");
			}else if (varName[1].indexOf("/cshmi/RenameObject") !== -1){
				returnValue = this._interpreteRenameObject(VisualObject, ObjectPath+"/"+varName[0]);
			}else if (varName[1].indexOf("/cshmi/CreateObject") !== -1){
				returnValue = this._interpreteCreateObject(VisualObject, ObjectPath+"/"+varName[0]);
			}else if (varName[1].indexOf("/cshmi/DeleteObject") !== -1){
				returnValue = this._interpreteDeleteObject(VisualObject, ObjectPath+"/"+varName[0]);
			}else if (varName[1].indexOf("/cshmi/LinkObjects") !== -1){
				returnValue = this._interpreteLinkObjects(VisualObject, ObjectPath+"/"+varName[0]);
			}else if (varName[1].indexOf("/cshmi/UnlinkObjects") !== -1){
				returnValue = this._interpreteUnlinkObjects(VisualObject, ObjectPath+"/"+varName[0]);
			}else if (varName[1].indexOf("/cshmi/IfThenElse") !== -1){
				returnValue = this._interpreteIfThenElse(VisualObject, ObjectPath+"/"+varName[0]);
			}else if (varName[1].indexOf("/cshmi/ChildrenIterator") !== -1){
				returnValue = this._interpreteChildrenIterator(VisualObject, ObjectPath+"/"+varName[0]);
			}else if (varName[1].indexOf("/cshmi/InstantiateTemplate") !== -1){
				returnValue = this._interpreteInstantiateTemplate(VisualObject, ObjectPath+"/"+varName[0]);
			}else if (varName[1].indexOf("/cshmi/RoutePolyline") !== -1){
				returnValue = this._interpreteRoutePolyline(VisualObject, ObjectPath+"/"+varName[0]);
			}else if (varName[1].indexOf("/cshmi/RebuildObject") !== -1){
				returnValue = this._interpreteRebuildObject(VisualObject, ObjectPath+"/"+varName[0]);
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
	 * @param callerObserver Object which hold info for the callback
	 * @return {bool} false if error, null if intentionally no value, "" if no entry found, true if the request is handled by a callback
	 */
	_getValue: function(VisualObject, ObjectPath, callerObserver){
		var ParameterName = "";
		var ParameterValue = "";
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
			if (successCode === false){
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
		}else{
			//the object is asked this session, so reuse the config to save communication requests
			ParameterName = this.ResourceList.Actions[ObjectPath].ParameterName;
			ParameterValue = this.ResourceList.Actions[ObjectPath].ParameterValue;
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
		
		if(this.initStage === true){
			//force sync request in the init stage. The order of actions have to be fixed in the loading
			callerObserver = null;
			var GetVarCbfnc = null;
		}else{
			//after load, we can use async requests
			
			//the callback fills the callerObserver with data and call the checkAndTrigger()
			GetVarCbfnc = function(Client, req){
				if(req.status !== 200){
					var newValue = false;
				}else{
					var response = req.responseText;
					var responseArray = HMI.KSClient.splitKsResponse(response);
					if (responseArray.length === 0){
						newValue = false;
					}else{
						newValue = responseArray[0];
					}
				}
				callerObserver.updateValueInArray(ObjectPath, newValue);
				callerObserver.checkAndTrigger();
			};
		}
		
		if (ParameterName === "ksVar" && preventNetworkRequest === true){
			//intentionally no value
			return null;
		}else if (ParameterName === "ksVar" && preventNetworkRequest === false){
			var response;
			if (ParameterValue.charAt(0) !== "/"){
				//we have no absolute path => get baseKsPath
			}
			var path = this._generateFullKsPath(VisualObject, ObjectPath, ParameterValue);
			if(callerObserver !== undefined && callerObserver !== null){
				response = HMI.KSClient.getVar(path, "OP_VALUE", GetVarCbfnc, true);
				return true;
			}else{
				response = HMI.KSClient.getVar(path, "OP_VALUE", null);
				var responseArray = HMI.KSClient.splitKsResponse(response);
				if (responseArray.length === 0){
					return false;
				}else{
					return responseArray[0];
				}
			}
		}else if (ParameterName === "elemVar"){
			//todo interprete elemVarPath
			
			//some elemVars are special, as they are different in OVM and SVG
			if (ParameterValue === "content"){
				if (typeof VisualObject.textContent != "undefined"){
					return VisualObject.textContent;
				}else if (typeof VisualObject.innerText != "undefined"){
					return VisualObject.innerText;
				}else{
					//todo asv compatibility
					return "";
				}
			}else if (ParameterValue === "title"){
				for (var i = 0;i < VisualObject.childNodes.length;i++){
					if (VisualObject.childNodes.item(i).tagName === "title"){
						return VisualObject.childNodes.item(i).firstChild.textContent;
					}
				} 
				return "";
			}else if (ParameterValue === "fontSize"){
				return VisualObject.getAttribute("font-size");
			}else if (ParameterValue === "fontStyle"){
				return VisualObject.getAttribute("font-style");
			}else if (ParameterValue === "fontWeight"){
				return VisualObject.getAttribute("font-weight");
			}else if (ParameterValue === "fontFamily"){
				return VisualObject.getAttribute("font-family");
			}else if (ParameterValue === "horAlignment"){
				return VisualObject.getAttribute("text-anchor");
			}else if (ParameterValue === "verAlignment"){
				if (VisualObject.tagName !== "text"){
					return "";
				}else if (VisualObject.getAttribute("dy") === "0.7ex"){
					return "middle";
				}else if (VisualObject.getAttribute("dy") === "1ex"){
					return "hanging";
				}
				return "auto";
			}else if (ParameterValue === "visible"){
				var displayVar = VisualObject.getAttribute("display");
				if (displayVar == "none"){
					return "FALSE";
				}else{
					return "TRUE";
				}
			}else if (ParameterValue === "rotate"){
				if (VisualObject.tagName === "svg" && VisualObject.parentNode.tagName === "g" && VisualObject.parentNode.id === ""){
					//svg are not transformable, so the rotation is in the objects parent
					var TransformString = VisualObject.parentNode.getAttribute("transform");
				}else if (VisualObject.tagName === "svg"){
					//it is an template, with no rotation
					return "0";
				}else{
					TransformString = VisualObject.getAttribute("transform");
				}
				if(TransformString === ""){
					return 0;
				}
				//"rotate(45,21.000000,50.000000)" or "rotate(45)"
				
				//remove rotate()
				TransformString = TransformString.replace(")", "").replace("rotate(", "");
				//get first number if there are 3, separated via comma
				return TransformString.split(",")[0];
			}else if (ParameterValue === "previousTemplateCount"){
				if (HMI.instanceOf(VisualObject, this.cshmiTemplateClass) !== true){
					return 0;
				}
				var previousTemplateCount = 0;
				var myTemplateClass = VisualObject.getAttribute("data-TemplateModelSource");
				var SiblingObj = VisualObject;
				
				/**
				 * helper Function for checking all childrens
				 */
				var checkChildrenTemplateMatch = function (currentNode, myTemplateClass){
					if(currentNode.cshmiOriginalOrderList[myTemplateClass] === undefined){
						return true;
					}
					for (var i = 0; i < currentNode.cshmiOriginalOrderList[myTemplateClass].length;i++){
						if (currentNode.cshmiOriginalOrderList[myTemplateClass][i].ownerDocument === HMI.cshmi.trashDocument){
							//this child was replaced with rebuildObject, so remove from the orderList
							//should not happen if we made no error in rebuildObject
							currentNode.cshmiOriginalOrderList[myTemplateClass].splice(i, 1);
							i--; //we need i at the same value in the next loop
						}else if (currentNode.cshmiOriginalOrderList[myTemplateClass][i] === VisualObject){
							//we found ourself, break
							return false;
						}else if (currentNode.cshmiOriginalOrderList[myTemplateClass][i].getAttribute("display") !== "none"){
							//we found a hit
							previousTemplateCount++;
							//iterate down and check childrens
							var result = checkChildrenTemplateMatch(currentNode.cshmiOriginalOrderList[myTemplateClass][i], myTemplateClass);
							if (false === result){
								//we are below this branch, break
								return false;
							}
						}
					}
					return true;
				};
				
				//search maximum parent from the same template
				while(SiblingObj.parentNode !== null){
					SiblingObj = SiblingObj.parentNode;
					if (SiblingObj.getAttribute("data-TemplateModelSource") !== myTemplateClass){
						//the object is from another same class, so break
						break;
					}
				}
				//check recursive the childrens
				checkChildrenTemplateMatch(SiblingObj, myTemplateClass);
				
				return previousTemplateCount;
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
			try {
				if (localStorage !== undefined){
					if (localStorage.getItem(ParameterValue) !== null){
						return localStorage.getItem(ParameterValue);
					}
				}else if(this.ResourceList.GlobalVar[ParameterValue] !== undefined){
					//fall back to globalVar if no persistent storage is available
					return this.ResourceList.GlobalVar[ParameterValue];
				}
			} catch (e) {
				if(this.ResourceList.GlobalVar[ParameterValue] !== undefined){
					//fall back to globalVar if no persistent storage is allowed
					return this.ResourceList.GlobalVar[ParameterValue];
				}
			}
			return "";
		}else if (ParameterName === "OperatorInput"){
			if(ParameterValue.indexOf("textinput") !== -1){
				//todo idea: rebuild as html div with textarea
				
				var input = null;
				var textinputHint;
				var splittedValueParameter = ParameterValue.split(":");
				if(!window.prompt){
					HMI.hmi_log_info_onwebsite("Sorry, your browser does not support textinput. If this is IE10 Metro mode, try desktop mode.");
				}else if (splittedValueParameter.length > 1){
					textinputHint = splittedValueParameter[1];
					//e.g. "textinput:Some textinputHint:TemplateFBReferenceVariable:InputVarPath"
					if (splittedValueParameter.length > 3){
						this.ResourceList.Actions["tempPath"] = new Object();
						this.ResourceList.Actions["tempPath"].ParameterName = splittedValueParameter[2];
						this.ResourceList.Actions["tempPath"].ParameterValue = splittedValueParameter[3];
						var defaultValue = this._getValue(VisualObject, "tempPath");
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
				var newX = HMI.getClickPosition(this.ResourceList.EventInfos.EventObj, HMI.Playground.firstChild)[0];
				if (!isNaN(newX)){
					return newX;
				}
			}else if(ParameterValue === "mousey"){
				var newY = HMI.getClickPosition(this.ResourceList.EventInfos.EventObj, HMI.Playground.firstChild)[1];
				if (!isNaN(newY)){
					return newY;
				}
			}else{
				HMI.hmi_log_info_onwebsite('GetValue OperatorInput not implemented. command: '+ParameterValue);
			}
			return false;
		}else if (ParameterName === "TemplateFBReferenceVariable"){
			var FBRef = this._getFBReference(VisualObject);
			if (FBRef === ""){
				return false;
			}else if (ParameterValue === "fullqualifiedname"){
				return FBRef;
			}else if (ParameterValue === "fullqualifiedparentname"){
				var PathArray = FBRef.split("/");
				PathArray.pop();
				return PathArray.join("/");
			}else if (ParameterValue === "absolutepathname"){
				return HMI.KSClient._splitKSPath(FBRef)[1];
			}else if (ParameterValue === "absoluteparentpathname"){
				var PathArray = FBRef.split("/");
				PathArray.pop();
				return HMI.KSClient._splitKSPath(PathArray.join("/"))[1];
			}else if (ParameterValue === "identifier"){
				//if the identifier is requested calculate this to avoid network request
				var Objectname = FBRef.split("/");
				var identifier = Objectname[Objectname.length - 1];
				//if we refer to an variable return this name only
				identifier = identifier.split(".");
				return identifier[identifier.length - 1];
			}else if(preventNetworkRequest === true){
				//intentionally no value
				return null;
			}
			var path = FBRef+"."+ParameterValue;
			
			if(callerObserver !== undefined && callerObserver !== null){
				response = HMI.KSClient.getVar(path, "OP_VALUE", GetVarCbfnc, true);
				return true;
			}else{
				response = HMI.KSClient.getVar(path, "OP_VALUE", null);
				var responseArray = HMI.KSClient.splitKsResponse(response);
				if (responseArray.length === 0){
					return false;
				}else{
					return responseArray[0];
				}
			}
		}else if (ParameterName === "TemplateFBVariableReferenceName"){
			if (preventNetworkRequest === true){
				//intentionally no value
				return null;
			}
			var TemplateObject = VisualObject;
			do{
				if(TemplateObject.FBVariableReference && TemplateObject.FBVariableReference[ParameterValue] !== undefined){
					//a FBVariableReferenceName was requested
					if (TemplateObject.FBVariableReference[ParameterValue].charAt(0) === "/"){
						//String begins with / so it is a fullpath
						var path = TemplateObject.FBVariableReference[ParameterValue];
						if(callerObserver !== undefined && callerObserver !== null){
							response = HMI.KSClient.getVar(path, "OP_VALUE", GetVarCbfnc, true);
							return true;
						}else{
							response = HMI.KSClient.getVar(path, "OP_VALUE", null);
							var responseArray = HMI.KSClient.splitKsResponse(response);
							if (responseArray.length === 0){
								return false;
							}else{
								return responseArray[0];
							}
						}
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
	 * calls (multiple) getValue and after that calls _setVarExecute on success
	 * @param {SVGElement} VisualObject Object to manipulate the visualisation
	 * @param {String} ObjectPath Path to this cshmi object containing the event/action/visualisation
	 * @param {String} GetType "static" one static OV_PART, "concat" concat multiple getValues, "math" mathematics operation
	 * @return false on error, true on success
	 */
	_setValue: function(VisualObject, ObjectPath, GetType){
		//todo get config for setvalue and getvalue combined in one request, ergebnis per requestList �bergeben...?
		
		//get Value to set
		if (GetType === "static"){
			var setValueObserver = new cshmiObserver(VisualObject, ObjectPath, 1, this);
			setValueObserver.triggerActivity = function(){
				//in a static GetType we are ready in the first call by definition
				var NewValue = "";
				for (var i=0; i < this.ObserverEntryArray.length; i++) {
					var thisObserverEntry = this.ObserverEntryArray[i];
					if(thisObserverEntry.value === false){
						//getValue had an error
						HMI.hmi_log_info("cshmi._setValue on "+this.ObjectPath+" (baseobject: "+this.VisualObject.id+") failed because of an error in getValue.");
						return false;
					}else if(thisObserverEntry.value === null){
						//getValue had intentionally no value, abort
						return true;
					}else if(thisObserverEntry.value === undefined){
						//getValue gave no result, should not happen in static case
						return false;
					}
					NewValue = thisObserverEntry.value;
				}
				this.cshmiObject._setVarExecute(this.VisualObject, this.ObjectPath, GetType, NewValue);
				return true;
			};
			var thisObserverEntry = new ObserverEntry("value", ".");
			setValueObserver.ObserverEntryArray[0] = thisObserverEntry;
			//via getValue-part of setValue object
			var NewValue = this._getValue(VisualObject, ObjectPath+".value", setValueObserver);
			
			if(NewValue === true){
				//the setVar is handled via a callback
				return true;
			}else{
				thisObserverEntry.requirementsFulfilled = true;
				thisObserverEntry.value = NewValue;
				var result = setValueObserver.checkAndTrigger();
				if (result === false){
					//in an known error state, skip processing
					//break;
				}
				return true;
			}
		}else if (GetType === "concat"){
			//via multiple getValues under the setValue object
			var responseArray = HMI.KSClient.getChildObjArray(ObjectPath, this);
			
			//newwrite
			//fetch config from all childrens via this.ResourceList.ModellVariables.*
			
			var setValueObserver = new cshmiObserver(VisualObject, ObjectPath, responseArray.length, this);
			setValueObserver.triggerActivity = function(){
				var NewValue = "";
				for (var i=0; i < this.ObserverEntryArray.length; i++) {
					var thisObserverEntry = this.ObserverEntryArray[i];
					if(thisObserverEntry.value === false){
						//getValue had an error
						HMI.hmi_log_info("cshmi._setValue on "+this.ObjectPath+" (baseobject: "+this.VisualObject.id+") failed because of an error in getValue.");
						return false;
					}else if(thisObserverEntry.value === null){
						//getValue had intentionally no value, abort
						return true;
					}else if(thisObserverEntry.value === undefined){
						//getValue gave no result, should not happen in static case
						return false;
					}
					//force string to clean append
					NewValue = NewValue + thisObserverEntry.value.toString();
				}
				this.cshmiObject._setVarExecute(this.VisualObject, this.ObjectPath, GetType, NewValue);
				return true;
			};
			
			for (var i=0; i < responseArray.length; i++) {
				var varName = responseArray[i].split(" ");
				if (varName[1].indexOf("/cshmi/GetValue") !== -1){
					var thisObserverEntry = new ObserverEntry(varName[0], "/");
					setValueObserver.ObserverEntryArray[i] = thisObserverEntry;
					
					//via getValue instance under setValue object
					var NewValuePart = this._getValue(VisualObject, ObjectPath+"/"+varName[0], setValueObserver);
					
					if(NewValuePart === true){
						//the setVar is handled via a callback
					}else{
						thisObserverEntry.requirementsFulfilled = true;
						thisObserverEntry.value = NewValuePart;
						var result = setValueObserver.checkAndTrigger();
						if (result === false){
							//in an known error state, skip processing
							break;
						}
					}
				}
			}
			return true;
		}else if (GetType === "math"){
			//via multiple getValues under the setValue object
			var responseArray = HMI.KSClient.getChildObjArray(ObjectPath, this);
			
			//newwrite
			//fetch config from all childrens via this.ResourceList.ModellVariables.*
			
			var setValueObserver = new cshmiObserver(VisualObject, ObjectPath, responseArray.length, this);
			setValueObserver.triggerActivity = function(){
				var NewValue = 0;
				for (var i=0; i < this.ObserverEntryArray.length; i++) {
					var thisObserverEntry = this.ObserverEntryArray[i];
					if(thisObserverEntry.value === false){
						//getValue had an error
						HMI.hmi_log_info("cshmi._setValue on "+this.ObjectPath+" (baseobject: "+this.VisualObject.id+") failed because of an error in getValue.");
						return false;
					}else if(thisObserverEntry.value === null){
						//getValue had intentionally no value, abort
						return true;
					}else if(thisObserverEntry.value === undefined){
						//getValue gave no result, should not happen in static case
						return false;
					}else if(!isNumeric(thisObserverEntry.value)){
						//clear an non numeric entry
						thisObserverEntry.value = 0;
					}
					var NewValuePart = thisObserverEntry.value;
					
					if (thisObserverEntry.ObjectName.indexOf("add") === 0){
						NewValue = NewValue + parseFloat(NewValuePart);
					}else if (thisObserverEntry.ObjectName.indexOf("sub") === 0){
						NewValue = NewValue - parseFloat(NewValuePart);
					}else if (thisObserverEntry.ObjectName.indexOf("mul") === 0){
						NewValue = NewValue * parseFloat(NewValuePart);
					}else if (thisObserverEntry.ObjectName.indexOf("div") === 0){
						NewValue = NewValue / parseFloat(NewValuePart);
					}else if (thisObserverEntry.ObjectName.indexOf("abs") === 0){
						NewValue = NewValue + Math.abs(parseFloat(NewValuePart));
					}else if (thisObserverEntry.ObjectName.indexOf("acos") === 0){
						NewValue = NewValue + Math.acos(parseFloat(NewValuePart));
					}else if (thisObserverEntry.ObjectName.indexOf("asin") === 0){
						NewValue = NewValue + Math.asin(parseFloat(NewValuePart));
					}else if (thisObserverEntry.ObjectName.indexOf("atan") === 0){
						NewValue = NewValue + Math.atan(parseFloat(NewValuePart));
					}else if (thisObserverEntry.ObjectName.indexOf("cos") === 0){
						NewValue = NewValue + Math.cos(parseFloat(NewValuePart));
					}else if (thisObserverEntry.ObjectName.indexOf("exp") === 0){
						NewValue = NewValue + Math.exp(parseFloat(NewValuePart));
					}else if (thisObserverEntry.ObjectName.indexOf("log") === 0){
						NewValue = NewValue + Math.log(parseFloat(NewValuePart));
					}else if (thisObserverEntry.ObjectName.indexOf("sin") === 0){
						NewValue = NewValue + Math.sin(parseFloat(NewValuePart));
					}else if (thisObserverEntry.ObjectName.indexOf("sqrt") === 0){
						NewValue = NewValue + Math.sqrt(parseFloat(NewValuePart));
					}else if (thisObserverEntry.ObjectName.indexOf("tan") === 0){
						NewValue = NewValue + Math.tan(parseFloat(NewValuePart));
					}else if (thisObserverEntry.ObjectName.indexOf("pow") === 0){
						NewValue = Math.pow(NewValue, parseFloat(NewValuePart));
					}
					
					if (!isFinite(NewValue)){
						NewValue = 0;
					}
				}
				//force string format
				NewValue = NewValue.toString();
				
				this.cshmiObject._setVarExecute(this.VisualObject, this.ObjectPath, GetType, NewValue);
				return true;
			};
			
			for (var i=0; i < responseArray.length; i++) {
				var varName = responseArray[i].split(" ");
				if (varName[1].indexOf("/cshmi/GetValue") !== -1){
					var thisObserverEntry = new ObserverEntry(varName[0], "/");
					setValueObserver.ObserverEntryArray[i] = thisObserverEntry;
					
					//via getValue instance under setValue object
					var NewValuePart = this._getValue(VisualObject, ObjectPath+"/"+varName[0], setValueObserver);
					
					if(NewValuePart === true){
						//the setVar is handled via a callback
					}else{
						thisObserverEntry.requirementsFulfilled = true;
						thisObserverEntry.value = NewValuePart;
						var result = setValueObserver.checkAndTrigger();
						if (result === false){
							//in an known error state, skip processing
							break;
						}
					}
				}
			}
			return true;
		}else{
			//setValue type not recognised!
			return false;
		}
	},
	
	/**
	 * sets a given value
	 * @param {SVGElement} VisualObject Object to manipulate the visualisation
	 * @param {String} ObjectPath Path to this cshmi object containing the event/action/visualisation
	 * @param {String} GetType "static" one static OV_PART, "concat" concat multiple getValues, "math" mathematics operation
	 * @param {String} NewValue Value to set
	 * @return false on error, true on success
	 */
	_setVarExecute: function(VisualObject, ObjectPath, GetType, NewValue){
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
			if (GetType === "static"){
				//requestList[ObjectPath]["translationSource"] = null;
			}
			
			var successCode = this._requestVariablesArray(requestList);
			if (successCode === false){
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
		}else{
			//the object is asked this session, so reuse the config to save communication requests
			ParameterName = this.ResourceList.Actions[ObjectPath].ParameterName;
			ParameterValue = this.ResourceList.Actions[ObjectPath].ParameterValue;
		}
		
		var SetVarCbfnc = function(Client, req){
			if(req.status !== 200){
				var response = req.responseText;
				if (response.indexOf("KS_ERR_BADPARAM") !== -1){
					HMI.hmi_log_onwebsite('Setting "'+NewValue+'" at '+path+' not successfull: Bad Parameter ');
				}else if (response.indexOf("KS_ERR") !== -1){
					HMI.hmi_log_info('Setting "'+NewValue+'" at variable '+path+' not successfull: '+req.responseText+' (configured here: '+ObjectPath+').');
				}else{
					HMI.hmi_log_info('Setting a variable failed.');
				}
			}
		};
		
		//set the new Value
		if (ParameterName === "ksVar"){
			var result;
			var path = this._generateFullKsPath(VisualObject, ObjectPath, ParameterValue);
			
			result = HMI.KSClient.setVar_NG(path, NewValue, null, SetVarCbfnc, true);
			
			if (result !== true){
				return false;
			}
			return true;
		}else if (ParameterName === "elemVar"){
			//todo interprete elemVarPath
			
			//some elemVars are special, as they are different in OVM and SVG
			if (ParameterValue === "content"){
				this._setSvgText(VisualObject, NewValue, VisualObject.getAttribute("trimToLength"));
			}else if (ParameterValue === "trimToLength"){
				this._setSvgText(VisualObject, null, NewValue);
			}else if (ParameterValue === "title"){
				this._setTitle(VisualObject, NewValue);
			}else if (ParameterValue === "fontSize"){
				VisualObject.setAttribute("font-size", NewValue);
			}else if (ParameterValue === "fontStyle"){
				VisualObject.setAttribute("font-style", NewValue);
			}else if (ParameterValue === "fontWeight"){
				VisualObject.setAttribute("font-weight", NewValue);
			}else if (ParameterValue === "fontFamily"){
				VisualObject.setAttribute("font-family", NewValue);
			}else if (ParameterValue === "horAlignment"){
				VisualObject.setAttribute("text-anchor", NewValue);
			}else if (ParameterValue === "verAlignment"){
				if (NewValue === "auto"){
					if(VisualObject.hasAttribute("dy")){
						VisualObject.removeAttribute("dy");
					}
				}else if (NewValue === "middle"){
					VisualObject.setAttribute("dy", "0.7ex");
				}else if (NewValue === "hanging"){
					VisualObject.setAttribute("dy", "1ex");
				}
			}else if (ParameterValue === "visible"){
				if (NewValue == "FALSE"){
					VisualObject.setAttribute("display", "none");
				}else{
					VisualObject.setAttribute("display", "block");
					
					//load hidden elements now
					this._loadHiddenChildrenElements(VisualObject);
				}
			}else if (ParameterValue === "rotate"){
				if(!isNumeric(NewValue)){
					//ignore writing wrong value
					return false;
				}
				//svg are not transformable, so the rotation is in the objects parent
				if (VisualObject.tagName === "svg" && VisualObject.parentNode.tagName === "g" && VisualObject.parentNode.id === ""){
					//object has already an g parent
					var rotationObject = VisualObject.parentNode;
				}else if (VisualObject.tagName === "svg" && VisualObject.parentNode.tagName !== "g"){
					//element has to be shifted into an g element
					rotationObject = HMI.svgDocument.createElementNS(HMI.HMI_Constants.NAMESPACE_SVG, 'g');
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
			}else if (ParameterValue === "absolutex"){
				var relativeX = 0;
				if (this.ResourceList.EventInfos.mouseRelativePosition !== null){
					//if we are after an move, we want to set a different x
					relativeX = this.ResourceList.EventInfos.mouseRelativePosition[0];
				}
				if(VisualObject.parentNode !== null && VisualObject.parentNode.namespaceURI == HMI.HMI_Constants.NAMESPACE_SVG){
					//absolutex is calculated from the offset of the parentNode
					VisualObject.setAttribute("x", NewValue - VisualObject.parentNode.getAttribute("absolutex") - relativeX);
					VisualObject.setAttribute("absolutex", NewValue - relativeX);
					//we want to have offset parameter on all visual elements
					var ComponentChilds = VisualObject.getElementsByTagNameNS(HMI.HMI_Constants.NAMESPACE_SVG, '*');
					for(var i = 0;i < ComponentChilds.length;i++){
						HMI.saveAbsolutePosition(ComponentChilds[i]);
					}
				}
			}else if (ParameterValue === "absolutey"){
				var relativeY = 0;
				if (this.ResourceList.EventInfos.mouseRelativePosition !== null){
					//if we are after an move, we want to set a different y
					relativeY = this.ResourceList.EventInfos.mouseRelativePosition[1];
				}
				if(VisualObject.parentNode !== null && VisualObject.parentNode.namespaceURI == HMI.HMI_Constants.NAMESPACE_SVG){
					//absolutey is calculated from the offset of the parentNode
					VisualObject.setAttribute("y", NewValue - VisualObject.parentNode.getAttribute("absolutey") - relativeY);
					VisualObject.setAttribute("absolutey", NewValue - relativeY);
					//we want to have offset parameter on all visual elements
					var ComponentChilds = VisualObject.getElementsByTagNameNS(HMI.HMI_Constants.NAMESPACE_SVG, '*');
					for(var i = 0;i < ComponentChilds.length;i++){
						HMI.saveAbsolutePosition(ComponentChilds[i]);
					}
				}
			}else{
				if (NewValue === "" && 
					(		ParameterValue === "x"||
							ParameterValue === "y"||
							ParameterValue === "width"||
							ParameterValue === "height"||
							ParameterValue === "x1"||
							ParameterValue === "y1"||
							ParameterValue === "x2"||
							ParameterValue === "y2"||
							ParameterValue === "rx"||
							ParameterValue === "ry"||
							ParameterValue === "cx"||
							ParameterValue === "cy"||
							ParameterValue === "r")
							){
					//do not allow setting of the basic length to the empty string
					return false;
				}
				VisualObject.setAttribute(ParameterValue, NewValue);
				if (ParameterValue === "width" || ParameterValue === "height"){
					if (HMI.instanceOf(VisualObject, "cshmi-blackbox")) {
						var secondchild = VisualObject.firstChild.firstChild;
						secondchild.setAttribute(ParameterValue, NewValue);
					}
				}
				//reposition absolutex/y if x, y, width or height was changed
				if (ParameterValue === "x" || ParameterValue === "y"){
					HMI.saveAbsolutePosition(VisualObject);
					//we want to have offset parameter on all visual elements
					var ComponentChilds = VisualObject.getElementsByTagNameNS(HMI.HMI_Constants.NAMESPACE_SVG, '*');
					for(var i = 0;i < ComponentChilds.length;i++){
						HMI.saveAbsolutePosition(ComponentChilds[i]);
					}
				}
			}
			return true;
		}else if (ParameterName === "globalVar"){
			//globalVar
			this.ResourceList.GlobalVar[ParameterValue] = NewValue;
			for (var i = 0; i < this.ResourceList.globalvarChangedCallStack.length;){
				var EventObjItem = this.ResourceList.globalvarChangedCallStack[i];
				if(EventObjItem["VisualObject"].ownerDocument === this.trashDocument){
					//this Object was replaced with rebuildObject, so remove from callstack
					this.ResourceList.globalvarChangedCallStack.splice(i, 1);
				}else{
					this._interpreteAction(EventObjItem["VisualObject"], EventObjItem["ObjectPath"]);
					i++;
				}
			}
			var blackboxes = csHMIgetElementsByClassName(null, "cshmi-blackbox");
			
			for (var i = 0; i < blackboxes.length; ++i){	
	
				//get string of "jsOnglobalvarchanged"
				//TODO iteration through all objects
				var blackboxObjectpath = blackboxes[i].getAttribute("data-ModelSource");
				var requestList = new Object();
				
				//if the Object was scanned earlier, get the cached information (could be the case with templates or repeated/cyclic calls to the same object)
				if (this.ResourceList.Elements && this.ResourceList.Elements[blackboxObjectpath] !== undefined){
					//the object is asked this session, so reuse the config to save communication requests
					requestList[blackboxObjectpath] = this.ResourceList.Elements[blackboxObjectpath].Parameters;
					this.executeScript(VisualObject, blackboxObjectpath, requestList[blackboxObjectpath]["jsOnglobalvarchanged"]);
				}else{
					requestList[blackboxObjectpath]["jsOnglobalvarchanged"] = null;
				}
			}
			return true;
		}else if (ParameterName === "persistentGlobalVar"){
			//persistentGlobalVar
			try {
				if (localStorage !== undefined){
						localStorage.setItem(ParameterValue, NewValue);
				}else{
					//fall back to globalVar if no persistent storage is available
					this.ResourceList.GlobalVar[ParameterValue] = NewValue;
				}
			} catch (e) {
				//fall back to globalVar if no persistent storage is allowed
				this.ResourceList.GlobalVar[ParameterValue] = NewValue;
			}
			for (var i = 0; i < this.ResourceList.globalvarChangedCallStack.length;){
				var EventObjItem = this.ResourceList.globalvarChangedCallStack[i];
				if(EventObjItem["VisualObject"].ownerDocument === this.trashDocument){
					//this Object was replaced with rebuildObject, so remove from callstack
					this.ResourceList.globalvarChangedCallStack.splice(i, 1);
				}else{
					this._interpreteAction(EventObjItem["VisualObject"], EventObjItem["ObjectPath"]);
					i++;
				}
			}
			var blackboxes = csHMIgetElementsByClassName(null, "cshmi-blackbox");
			
			for (var i = 0; i < blackboxes.length; ++i){	
	
				//get string of "jsOnglobalvarchanged"
				//TODO iteration through all objects
				var blackboxObjectpath = blackboxes[i].getAttribute("data-ModelSource");
				var requestList = new Object();
				
				//if the Object was scanned earlier, get the cached information (could be the case with templates or repeated/cyclic calls to the same object)
				if (this.ResourceList.Elements && this.ResourceList.Elements[blackboxObjectpath] !== undefined){
					//the object is asked this session, so reuse the config to save communication requests
					requestList[blackboxObjectpath] = this.ResourceList.Elements[blackboxObjectpath].Parameters;
					this.executeScript(VisualObject, blackboxObjectpath, requestList[blackboxObjectpath]["jsOnglobalvarchanged"]);
				}else{
					requestList[blackboxObjectpath]["jsOnglobalvarchanged"] = null;
				}
			}
			return true;
		}else if (ParameterName === "TemplateFBReferenceVariable"){
			var result;
			var path;
			var FBRef = this._getFBReference(VisualObject, true);
			if (FBRef[0] === ""){
				return false;
			}else if (ParameterValue === "fullqualifiedname"){
				var TemplateObject = FBRef[1];
				if (TemplateObject === null){
					return false;
				}
				if(TemplateObject.FBReference && TemplateObject.FBReference["default"] !== undefined){
					if(NewValue === ""){
						HMI.hmi_log_info_onwebsite(ObjectPath+": Tried to set FBReference to the empty string. Aborted.");
						return false;
					}
					TemplateObject.FBReference["default"] = NewValue;
					TemplateObject.id = NewValue;
					return true;
				}
				return false;
			}else if(ParameterValue === "identifier"){
				path = FBRef[0];
				result = HMI.KSClient.renameObject(path, NewValue, null, SetVarCbfnc, true);
			}else{
				path = FBRef[0]+"."+ParameterValue;
				result = HMI.KSClient.setVar_NG(path, NewValue, null, SetVarCbfnc, true);
			}
			if (result !== true){
				return false;
			}
			return true;
		}else if (ParameterName === "TemplateFBVariableReferenceName"){
			//TemplateFBReferenceVariable
			var TemplateObject = VisualObject;
			do{
				if(TemplateObject.FBVariableReference && TemplateObject.FBVariableReference[ParameterValue] !== undefined){
					//a FBVariableReferenceName was requested
					if (TemplateObject.FBVariableReference[ParameterValue].charAt(0) === "/"){
						//String begins with / so it is a fullpath
						var path = TemplateObject.FBVariableReference[ParameterValue];
						result = HMI.KSClient.setVar_NG(path, NewValue, null, SetVarCbfnc, true);
						if (result !== true){
							return false;
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
			var TemplateObject = VisualObject;
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
			//should not happen, since the first SVG has an empty ConfigValue
			return false;
		}
		HMI.hmi_log_info_onwebsite('SetValue '+ObjectPath+' not configured.');
		return false;
	},
	
	/**
	 * rename an object
	 * @param {SVGElement} VisualObject Object to manipulate the visualisation
	 * @param {String} ObjectPath Path to this cshmi object containing the event/action/visualisation
	 * @return false on error, true on success
	 */
	_interpreteRenameObject: function(VisualObject, ObjectPath){
		//via getValue-part of RenameObject object
		var OldName = this._getValue(VisualObject, ObjectPath+".OldName");
		var NewName = this._getValue(VisualObject, ObjectPath+".NewName");
		
		if (OldName === false || NewName === false){
			//getValue had an error
			return false;
		}else if (OldName === null || NewName === null){
			//intentionally no value, abort
			return null;
		}else if (OldName === "" || NewName === ""){
			//no value found
			return false;
		}
		
		var RenameCbfnc = function(Client, req){
			if(req.status !== 200){
				var response = req.responseText;
				if (response.indexOf("KS_ERR_NOACCESS") !== -1){
					HMI.hmi_log_onwebsite('Renaming "'+OldName+'" not successfull. Operation not allowed.');
				}else if (response.indexOf("KS_ERR_BADNAME") !== -1){
					HMI.hmi_log_onwebsite('Renaming "'+OldName+'" to "'+NewName+'" not successfull. Bad name supplied.');
				}else if (response.indexOf("KS_ERR_BADPATH") !== -1){
					HMI.hmi_log_onwebsite('Renaming "'+OldName+'" to "'+NewName+'" not successfull, new path was not correct:'+NewName);
				}else if (response.indexOf("KS_ERR") !== -1){
					HMI.hmi_log_info('Renaming "'+OldName+'" to "'+NewName+'" not successfull: '+response+' (configured here: '+ObjectPath+').');
				}
			}
		};
		
		var result = HMI.KSClient.renameObject(OldName, NewName, RenameCbfnc, true);
		
		if (result !== true){
			return false;
		}
		return true;
	},
	
	/**
	 * creates an object
	 * @param {SVGElement} VisualObject Object to manipulate the visualisation
	 * @param {String} ObjectPath Path to this cshmi object containing the event/action/visualisation
	 * @return false on error, true on success
	 */
	_interpreteCreateObject: function(VisualObject, ObjectPath){
		//todo: implement autoRenameIfExists
		
		//via getValue-part of CreateObject object
		var targetName = this._getValue(VisualObject, ObjectPath+".Name");
		var targetPlace = this._getValue(VisualObject, ObjectPath+".Place");
		var targetLibrary = this._getValue(VisualObject, ObjectPath+".Library");
		var targetClass = this._getValue(VisualObject, ObjectPath+".Class");
		
		if (targetName === false || targetPlace === false || targetLibrary === false || targetClass === false){
			//getValue had an error
			return false;
		}else if (targetName === null || targetPlace === null || targetLibrary === null || targetClass === null){
			//intentionally no value, abort
			return true;
		}else if (targetName === "" || targetPlace === "" || targetLibrary === "" || targetClass === ""){
			//no value found
			return false;
		}
		
		var CreateObjectCbfnc = function(Client, req){
			if(req.status !== 200){
				var response = req.responseText;
				if (response.indexOf("KS_ERR_NOACCESS") !== -1){
					HMI.hmi_log_onwebsite('Creation of "'+targetName+'" not successfull. Operation not allowed.');
				}else if (response.indexOf("KS_ERR_ALREADYEXISTS") !== -1){
					HMI.hmi_log_onwebsite('Creation of "'+targetName+'" not successfull. Object already exists.');
				}else if (response.indexOf("KS_ERR_BADPATH") !== -1){
					HMI.hmi_log_onwebsite('Creation of "'+targetName+'" not successfull. Could be a problem with the class name or the target domain.');
				}else if (response.indexOf("KS_ERR_BADNAME") !== -1){
					HMI.hmi_log_onwebsite('Creation of "'+targetName+'" not successfull. Bad name supplied.');
				}else if (response.indexOf("KS_ERR_GENERIC") !== -1){
					HMI.hmi_log_onwebsite('Creation of "'+targetName+'" not successfull. Generic error.');
				}else if (response.indexOf("KS_ERR") !== -1){
					HMI.hmi_log_onwebsite('Creation of "'+targetName+'" not successfull: '+response);
//					HMI.hmi_log_info('Creation of '+targetName+' not successfull: '+response+' (configured here: '+ObjectPath+').');
				}
			}
		};
		
		var result = HMI.KSClient.createObject(targetPlace+"/"+targetName, targetLibrary+"/"+targetClass, CreateObjectCbfnc, true);
		
		if (result !== true){
			return false;
		}
		return true;
	},
	
	/**
	 * deletes an object
	 * @param {SVGElement} VisualObject Object to manipulate the visualisation
	 * @param {String} ObjectPath Path to this cshmi object containing the event/action/visualisation
	 * @return false on error, true on success
	 */
	_interpreteDeleteObject: function(VisualObject, ObjectPath){
		//via getValue-part of DeleteObject object
		var targetName = this._getValue(VisualObject, ObjectPath+".Path");
		
		if (targetName === false){
			//getValue had an error
			return false;
		}else if (targetName === null){
			//intentionally no value, abort
			return null;
		}else if (targetName === ""){
			//no value found
			return false;
		}
		
		var DeleteObjectCbfnc = function(Client, req){
			if(req.status !== 200){
				var response = req.responseText;
				if (response.indexOf("KS_ERR_NOACCESS") !== -1){
					HMI.hmi_log_onwebsite('Deleting "'+targetName+'" not successfull. Operation not allowed.');
				}else if (response.indexOf("KS_ERR_BADPATH") !== -1){
					HMI.hmi_log_onwebsite('Deleting "'+targetName+'" not successfull. Object not found.');
				}else if (response.indexOf("KS_ERR") !== -1){
					HMI.hmi_log_info('Deleting '+targetName+' not successfull: '+response+' (configured here: '+ObjectPath+').');
				}
			}
		};

		
		var result = HMI.KSClient.deleteObject(targetName, DeleteObjectCbfnc, true);
		
		if (result !== true){
			return false;
		}
		return true;
	},
	
	/**
	 * links two objects
	 * @param {SVGElement} VisualObject Object to manipulate the visualisation
	 * @param {String} ObjectPath Path to this cshmi object containing the event/action/visualisation
	 * @return false on error, true on success
	 */
	_interpreteLinkObjects: function(VisualObject, ObjectPath){
		//via getValue-part of LinkObjects object
		var ObjectA = this._getValue(VisualObject, ObjectPath+".ObjectA");
		var ObjectB = this._getValue(VisualObject, ObjectPath+".ObjectB");
		var PortNameA = this._getValue(VisualObject, ObjectPath+".Association");
		
		if (ObjectA === false || ObjectB === false || PortNameA === false){
			//getValue had an error
			return false;
		}else if (ObjectA === null || ObjectB === null || PortNameA === null){
			//intentionally no value, abort
			return null;
		}else if (ObjectA === "" || ObjectB === "" || PortNameA === ""){
			//no value found
			return false;
		}
		
		var LinkObjectsCbfnc = function(Client, req){
			if(req.status !== 200){
				var response = req.responseText;
				if (response.indexOf("KS_ERR_NOACCESS") !== -1){
					HMI.hmi_log_onwebsite('Linking "'+ObjectA+'" and "'+ObjectB+'" not successfull. Operation not allowed.');
				}else if (response.indexOf("KS_ERR_ALREADYEXISTS") !== -1){
					HMI.hmi_log_onwebsite('Linking "'+ObjectA+'" and "'+ObjectB+'" not successfull. Link already exists.');
				}else if (response.indexOf("KS_ERR_BADPATH") !== -1){
					HMI.hmi_log_onwebsite('Linking "'+ObjectA+'" and "'+ObjectB+'" not successfull. Object not found.');
				}else if (response.indexOf("KS_ERR_BADPARAM") !== -1){
					HMI.hmi_log_onwebsite('Linking "'+ObjectA+'" and "'+ObjectB+'" not successfull. Bad Parameter.');
				}else if (response.indexOf("KS_ERR") !== -1){
					HMI.hmi_log_info('Linking "'+ObjectA+'" and "'+ObjectB+'" not successfull: '+response+' (configured here: '+ObjectPath+').');
				}
			}
		};
		
		var result = HMI.KSClient.linkObjects(ObjectA, ObjectB, PortNameA, LinkObjectsCbfnc, true);
		
		if (result !== true){
			return false;
		}
		return true;
	},
	
	/**
	 * unlinks two objects
	 * @param {SVGElement} VisualObject Object to manipulate the visualisation
	 * @param {String} ObjectPath Path to this cshmi object containing the event/action/visualisation
	 * @return false on error, true on success
	 */
	_interpreteUnlinkObjects: function(VisualObject, ObjectPath){
		//via getValue-part of UnlinkObjects object
		var ObjectA = this._getValue(VisualObject, ObjectPath+".ObjectA");
		var ObjectB = this._getValue(VisualObject, ObjectPath+".ObjectB");
		var PortNameA = this._getValue(VisualObject, ObjectPath+".Association");
		
		if (ObjectA === false || ObjectB === false || PortNameA === false){
			//getValue had an error
			return false;
		}else if (ObjectA === null || ObjectB === null || PortNameA === null){
			//intentionally no value, abort
			return null;
		}else if (ObjectA === "" || ObjectB === "" || PortNameA === ""){
			//no value found
			return false;
		}
		
		var UnlinkObjectsCbfnc = function(Client, req){
			if(req.status !== 200){
				var response = req.responseText;
				if (response.indexOf("KS_ERR_NOACCESS") !== -1){
					HMI.hmi_log_onwebsite('Unlinking "'+ObjectA+'" and "'+ObjectB+'" not successfull. Operation not allowed.');
				}else if (response.indexOf("KS_ERR_BADPATH") !== -1){
					HMI.hmi_log_onwebsite('Unlinking "'+ObjectA+'" and "'+ObjectB+'" not successfull. Object not found.');
				}else if (response.indexOf("KS_ERR_BADPARAM") !== -1){
					HMI.hmi_log_onwebsite('Unlinking "'+ObjectA+'" and "'+ObjectB+'" not successfull. Bad Parameter.');
				}else if (response.indexOf("KS_ERR") !== -1){
					HMI.hmi_log_info('Unlinking "'+ObjectA+'" and "'+ObjectB+'" not successfull: '+response+' (configured here: '+ObjectPath+').');
				}
			}
		};
		
		var result = HMI.KSClient.unlinkObjects(ObjectA, ObjectB, PortNameA, UnlinkObjectsCbfnc, true);
		
		if (result !== true){
			return false;
		}
		return true;
	},
	
	/**
	 * generate the full path (append baseKsPath by iterating objects if needed)
	 * @param {SVGElement} VisualObject Object to manipulate the visualisation
	 * @param {String} ObjectPath Path to this cshmi object containing the event/action/visualisation
	 * @param {String} PartialPath The path which should get potentially expanded via baseKsPath
	 * @return {String} returnValue String of the baseKSPath as seen from the ObjectPath; could be ""
	 * 
	 * 
	 * fixme: baseKsPath darf nicht ov_containment sondern instanziierungsbaum abgehen
	 * sollte den vollen pfad zwischen speichern
	 * sollte gef�llt werden von den containern, die sowieso abfragen machen
	 */
	_generateFullKsPath: function(VisualObject, ObjectPath, PartialPath){
		var returnValue = "";
		var ObjectPathArray = new Array();
		if (PartialPath.charAt(0) !== "/"){
			//if the path is not a full path, search for the baseKsPath
			ObjectPathArray = ObjectPath.split("/");
		}
		
		while(ObjectPathArray.length > 0){
			var currentPath = ObjectPathArray.join("/");
			var responseArray;
			if (this.ResourceList.baseKsPath[currentPath] === undefined){
				var response = HMI.KSClient.getVar(currentPath+'.baseKsPath', "OP_VALUE", null);
				responseArray = HMI.KSClient.splitKsResponse(response);
				
				this.ResourceList.baseKsPath[currentPath] = responseArray;
			}else{
				responseArray = this.ResourceList.baseKsPath[currentPath];
			}
			
			if (responseArray.length === 0){
				//an object in tree is no cshmi object or we have an error
				break;
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
		};
		
		//create the full path
		returnValue = returnValue+PartialPath;
		
		if (returnValue.indexOf("//localhost") === 0){
			//localhost in the model should not be the http-gateway
			return returnValue.replace(/localhost/, HMI.KSClient.ResourceList.ModelHost);
		}else if (! (returnValue.charAt(0) === "/" && returnValue.charAt(1) === "/")){
			//we want to have a full path with host and server
			
			//default to the ModelHost
			var serverName = "//" + HMI.KSClient.ResourceList.ModelHost + "/" + HMI.KSClient.ResourceList.ModelServer;
			
			//this Path has not host+server, so try to search for this via an parent Template
			if(VisualObject.parentNode !== null){
				//VisualObject is in DOM and configured
				var TemplateObject = VisualObject;
			}else if(VisualObject.VisualParentObject !== undefined){
				//VisualObject is not in DOM and we are configuring it right now (it has per definition no FBRef till now)
				TemplateObject = VisualObject.VisualParentObject;
			}else{
				//VisualObject is not in DOM and no Template. make the do-while abbort quick
				TemplateObject = VisualObject;
			}
			
			do{
				if(TemplateObject.FBReference && TemplateObject.FBReference["default"] !== undefined){
					//the name of a Template was requested
					if (TemplateObject.FBReference["default"].indexOf("//") === 0){
						var FBRef = TemplateObject.FBReference["default"];
						
						//find the 3rd "/"
						var slashIndex = FBRef.indexOf("/", 2);
						//find the 4th "/"
						slashIndex = FBRef.indexOf("/", slashIndex+1);
						//only keep the String before 4th "/"
						
						serverName = FBRef.slice(0, slashIndex);
						break;
					}
				}
			//loop upwards to find the Template object
			}while( (TemplateObject = TemplateObject.parentNode) && TemplateObject !== null && TemplateObject.namespaceURI == HMI.HMI_Constants.NAMESPACE_SVG);  //the = is no typo here!
			
			returnValue = serverName + returnValue;
		}
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
			if (successCode === false){
				return null;
			}
			
			anyCond = requestList[ObjectPath]["anycond"];
			
			//feeding garbage collector early
			requestList = null;
			
			//we have asked the object successful, so remember the result
			this.ResourceList.Actions[ObjectPath] = new Object();
			this.ResourceList.Actions[ObjectPath].IfThenElseParameterAnycond = anyCond;
		}else{
			//the object is asked this session, so reuse the config to save communication requests
			anyCond = this.ResourceList.Actions[ObjectPath].IfThenElseParameterAnycond;
		}
		var responseArray = HMI.KSClient.getChildObjArray(ObjectPath+".if", this);
		
		//newwrite
		//fetch config from all childrens via this.ResourceList.ModellVariables.*
		
		
		//fixme bei UND verkn�pfung kann man evtl fr�hzeitig eine l�sung haben
		var IfThenElseObserver = new cshmiObserver(VisualObject, ObjectPath, responseArray.length, this);
		IfThenElseObserver.triggerActivity = function(){
			var ConditionMatched = null;
			if(this.customInformation == true){
				//logical OR
				ConditionMatched = false;
				
				for (var i=0; i < this.ObserverEntryArray.length && ConditionMatched !== true; i++) {
					var thisObserverEntry = this.ObserverEntryArray[i];
					ConditionMatched = thisObserverEntry.value;
				}
			}else{
				//logical AND
				ConditionMatched = true;
				
				for (var i=0; i < this.ObserverEntryArray.length && ConditionMatched !== false; i++) {
					var thisObserverEntry = this.ObserverEntryArray[i];
					ConditionMatched = thisObserverEntry.value;
				}
			}
			if (ConditionMatched === true){
				this.cshmiObject._interpreteAction(VisualObject, ObjectPath+".then");
				return true;
			}else if (ConditionMatched === false){
				this.cshmiObject._interpreteAction(VisualObject, ObjectPath+".else");
				return true;
			}else{
				//this Action produced an error
				return false;
			}
		};
		
		if (anyCond == "TRUE"){
			IfThenElseObserver.customInformation = true;
		}else{
			IfThenElseObserver.customInformation = false;
		}
		
		var ConditionMatched;
		for(var i = 0;i < responseArray.length;i++){
			var varName = responseArray[i].split(" ");
			var thisObserverEntry = new ObserverEntry(varName[0], ".if/");
			IfThenElseObserver.ObserverEntryArray[i] = thisObserverEntry;
			
			if (varName[1].indexOf("/cshmi/CompareIteratedChild") !== -1){
				ConditionMatched = this._checkCondition(VisualObject, ObjectPath+".if/"+varName[0], true, IfThenElseObserver);
			}else if (varName[1].indexOf("/cshmi/Compare") !== -1){
				ConditionMatched = this._checkCondition(VisualObject, ObjectPath+".if/"+varName[0], false, IfThenElseObserver);
			}else if (varName[1].indexOf("/cshmi/Confirm") !== -1){
				ConditionMatched = this._checkConfirm(VisualObject, ObjectPath+".if/"+varName[0], IfThenElseObserver);
			}
			if(ConditionMatched === undefined){
				//the checkCondition is handled via a callback
			}else{
				thisObserverEntry.value = ConditionMatched;
				thisObserverEntry.requirementsFulfilled = true;
				var result = IfThenElseObserver.checkAndTrigger();
				if (result === false){
					//in an known error state, skip processing
					break;
				}
			}
		}
		return true;
	},
	
	/**
	 * checks Condition
	 * @param {SVGElement} VisualObject Object to manipulate the visualisation
	 * @param {String} ObjectPath Path to this cshmi object containing the event/action/visualisation
	 * @param {Boolean} CompareIteratedChild is the object a CompareIteratedChild?
	 * @return {Boolean} true if condition matched, false if not matched, null on error, undefined if callback is in charge
	 */
	_checkCondition: function(VisualObject, ObjectPath, CompareIteratedChild, IfThenElseObserver){
		if (CompareIteratedChild === true && this.ResourceList.ChildrenIterator.currentChild === undefined){
			HMI.hmi_log_info_onwebsite("CompareIteratedChild "+ObjectPath+" is not placed under a Iterator");
			//error state, so no boolean
			return null;
		}
		
		var requestList = new Object();
		if (this.ResourceList.Conditions && this.ResourceList.Conditions[ObjectPath] !== undefined){
			requestList[ObjectPath] = this.ResourceList.Conditions[ObjectPath].Parameters;
		}else{
			requestList[ObjectPath] = new Object();
			requestList[ObjectPath]["ignoreError"] = null;
			requestList[ObjectPath]["comptype"] = null;
			if (CompareIteratedChild === true){
				requestList[ObjectPath]["childValue"] = null;
			}
			var successCode = this._requestVariablesArray(requestList);
			if (successCode === false){
				return null;
			}
			
			//we have asked the object successful, so remember the result
			this.ResourceList.Conditions[ObjectPath] = new Object();
			this.ResourceList.Conditions[ObjectPath].Parameters = requestList[ObjectPath];
		}
		
		var ignoreError = requestList[ObjectPath]["ignoreError"];
		var comptype = requestList[ObjectPath]["comptype"];
		var childValue;
		var Value1;
		var Value2;
		
		var checkConditionObserver = new cshmiObserver(VisualObject, ObjectPath, 2, this);
		checkConditionObserver.triggerActivity = function(){
			this.cshmiObject._checkConditionResult(this.VisualObject, this.ObjectPath, IfThenElseObserver, checkConditionObserver);
		};
		checkConditionObserver.customInformation = {"ignoreError":ignoreError, "comptype":comptype};
		
		var thisObserverEntry;
		if (CompareIteratedChild === true){
			childValue = HMI.KSClient.splitKsResponse(requestList[ObjectPath]["childValue"], 0);
			
			if (childValue[0] === ""){
				HMI.hmi_log_info_onwebsite("CompareIteratedChild "+ObjectPath+" is not configured");
				//error state, so no boolean
				return null;
			}
			thisObserverEntry = new ObserverEntry("currentChild");
			//this is know right know, so fill it direct
			thisObserverEntry.value = this.ResourceList.ChildrenIterator.currentChild[childValue[0]];
			thisObserverEntry.requirementsFulfilled = true;
			checkConditionObserver.ObserverEntryArray[0] = thisObserverEntry;
			
			thisObserverEntry = new ObserverEntry("withValue", ".");
			checkConditionObserver.ObserverEntryArray[1] = thisObserverEntry;
			Value2 = this._getValue(VisualObject, ObjectPath+".withValue", checkConditionObserver);
			if(Value2 === true){
				//the setVar is handled via a callback
				return true;
			}else{
				thisObserverEntry.requirementsFulfilled = true;
				thisObserverEntry.value = Value2;
				var result = checkConditionObserver.checkAndTrigger();
				if (result === false){
					//in an known error state, skip processing
					//break;
				}
			}
		}else{
			thisObserverEntry = new ObserverEntry("value1", ".");
			checkConditionObserver.ObserverEntryArray[0] = thisObserverEntry;
			Value1 = this._getValue(VisualObject, ObjectPath+".value1", checkConditionObserver);
			if(Value1 === true){
				//the setVar is handled via a callback
			}else{
				thisObserverEntry.requirementsFulfilled = true;
				thisObserverEntry.value = Value1;
				var result = checkConditionObserver.checkAndTrigger();
				if (result === false){
					//in an known error state, skip processing
					//break;
				}
			}
			thisObserverEntry = new ObserverEntry("value2", ".");
			checkConditionObserver.ObserverEntryArray[1] = thisObserverEntry;
			Value2 = this._getValue(VisualObject, ObjectPath+".value2", checkConditionObserver);
			if(Value2 === true){
				//the setVar is handled via a callback
			}else{
				thisObserverEntry.requirementsFulfilled = true;
				thisObserverEntry.value = Value2;
				var result = checkConditionObserver.checkAndTrigger();
				if (result === false){
					//in an known error state, skip processing
					//break;
				}
			}
		}
		//feeding garbage collector early
		requestList = null;
		
		return;
	},
	
	/**
	 * callback function for comparing two values from the condition
	 * @param {SVGElement} VisualObject Object to manipulate the visualisation
	 * @param {String} ObjectPath Path to this cshmi object containing the event/action/visualisation
	 * @param {cshmiObserver} IfThenElseObserver Observer to save the result from the compare
	 * @param {cshmiObserver} checkConditionObserver Observer which holds the value
	 */
	_checkConditionResult: function(VisualObject, ObjectPath, IfThenElseObserver, checkConditionObserver){
		var ignoreError = checkConditionObserver.customInformation.ignoreError;
		var comptype = checkConditionObserver.customInformation.comptype;
		
		var Value1 = checkConditionObserver.ObserverEntryArray[0].value;
		var Value2 = checkConditionObserver.ObserverEntryArray[1].value;
		
		if (Value1 === null && ignoreError === "FALSE"){
			//intentionally no value, abort
			IfThenElseObserver.updateValueInArray(ObjectPath, null);
			IfThenElseObserver.checkAndTrigger();
			return;
		}else if (Value1 === false && ignoreError === "FALSE"){
			HMI.hmi_log_info("cshmi._checkCondition on "+ObjectPath+" (baseobject: "+VisualObject.id+") failed because Value1 had an error.");
			//error state, so no boolean
			IfThenElseObserver.updateValueInArray(ObjectPath, null);
			IfThenElseObserver.checkAndTrigger();
			return;
		}else if ((Value1 === false || Value1 === null) && ignoreError === "TRUE"){
			Value1 = "";
		}
		if (Value2 === null){
			//intentionally no value, abort
			IfThenElseObserver.updateValueInArray(ObjectPath, null);
			IfThenElseObserver.checkAndTrigger();
			return;
		}
		if (Value2 === null && ignoreError === "FALSE"){
			//getValue had intentionally no value, abort
			IfThenElseObserver.updateValueInArray(ObjectPath, null);
			IfThenElseObserver.checkAndTrigger();
			return;
		}else if (Value2 === false && ignoreError === "FALSE"){
			HMI.hmi_log_info("cshmi._checkCondition on "+ObjectPath+" (baseobject: "+VisualObject.id+") failed because Value2 had an error.");
			//error state, so no boolean
			IfThenElseObserver.updateValueInArray(ObjectPath, null);
			IfThenElseObserver.checkAndTrigger();
			return;
		}else if (Value2 === false && ignoreError === "TRUE"){
			Value2 = "";
		}
		
		//force proper numerical comparision for numbers, since "" < "0" is true in EcmaScript
		if(isNumeric(Value1)){
			Value1 = parseFloat(Value1);
		}
		if (Value2 === ""){
			Value2 = [""];
		}else{
			//todo doku mehrfache Abfragen von value2
			Value2 = HMI.KSClient.splitKsResponse(Value2, 0);
			
			for (var i=0; i<Value2.length; i++){
				if(isNumeric(Value2[i])){
					Value2[i] = parseFloat(Value2[i]);
				}
			}
		}
		
		if (comptype === "<"){
			for (var i=0; i<Value2.length; i++){
				if (!(Value1 < Value2[i])){
					IfThenElseObserver.updateValueInArray(ObjectPath, false);
					IfThenElseObserver.checkAndTrigger();
					return;
				}
			}
			return true;
		}else if (comptype === "<="){
			for (var i=0; i<Value2.length; i++){
				if (!(Value1 <= Value2[i])){
					IfThenElseObserver.updateValueInArray(ObjectPath, false);
					IfThenElseObserver.checkAndTrigger();
					return;
				}
			}
			IfThenElseObserver.updateValueInArray(ObjectPath, true);
			IfThenElseObserver.checkAndTrigger();
			return;
		}else if (comptype === "=="){
			//check if one entry of Value2 == Value1
			for (var i=0; i<Value2.length; i++){
				if (Value1 === Value2[i]){
					IfThenElseObserver.updateValueInArray(ObjectPath, true);
					IfThenElseObserver.checkAndTrigger();
					return;
				}
			}
			IfThenElseObserver.updateValueInArray(ObjectPath, false);
			IfThenElseObserver.checkAndTrigger();
			return;
		}else if (comptype === "!="){
			for (var i=0; i<Value2.length; i++){
				if (!(Value1 !== Value2[i])){
					IfThenElseObserver.updateValueInArray(ObjectPath, false);
					IfThenElseObserver.checkAndTrigger();
					return;
				}
			}
			IfThenElseObserver.updateValueInArray(ObjectPath, true);
			IfThenElseObserver.checkAndTrigger();
			return;
		}else if (comptype === ">="){
			for (var i=0; i<Value2.length; i++){
				if (!(Value1 >= Value2[i])){
					IfThenElseObserver.updateValueInArray(ObjectPath, false);
					IfThenElseObserver.checkAndTrigger();
					return;
				}
			}
			IfThenElseObserver.updateValueInArray(ObjectPath, true);
			IfThenElseObserver.checkAndTrigger();
			return;
		}else if (comptype === ">"){
			for (var i=0; i<Value2.length; i++){
				if (!(Value1 > Value2[i])){
					IfThenElseObserver.updateValueInArray(ObjectPath, false);
					IfThenElseObserver.checkAndTrigger();
					return;
				}
			}
			IfThenElseObserver.updateValueInArray(ObjectPath, true);
			IfThenElseObserver.checkAndTrigger();
			return;
		}else{
			HMI.hmi_log_error("cshmi._checkCondition Comparingtype "+comptype+" unknown");
			//error state, so no boolean
			IfThenElseObserver.updateValueInArray(ObjectPath, null);
			IfThenElseObserver.checkAndTrigger();
			return;
		}
	},
	
	/**
	 * checks Confirm
	 * @param {SVGElement} VisualObject Object to manipulate the visualisation
	 * @param {String} ObjectPath Path to this cshmi object containing the event/action/visualisation
	 * @return {Boolean} true if condition matched, false if not matched, null on error, undefined if callback is in charge
	 */
	_checkConfirm: function(VisualObject, ObjectPath, IfThenElseObserver){
		var checkConfirmObserver = new cshmiObserver(VisualObject, ObjectPath, 1, this);
		checkConfirmObserver.triggerActivity = function(){
			IfThenElseObserver.updateValueInArray(ObjectPath, window.confirm(checkConfirmObserver.ObserverEntryArray[0].value));
			IfThenElseObserver.checkAndTrigger();
		};
		
		var thisObserverEntry = new ObserverEntry("question", ".");
		checkConfirmObserver.ObserverEntryArray[0] = thisObserverEntry;
		question = this._getValue(VisualObject, ObjectPath+".question", checkConfirmObserver);
		if(question === true){
			//the setVar is handled via a callback
		}else{
			thisObserverEntry.requirementsFulfilled = true;
			thisObserverEntry.value = question;
			var result = checkConfirmObserver.checkAndTrigger();
			if (result === false){
				//in an known error state, skip processing
				//break;
			}
		}
		return;
	},
	
	/**
	 * iterates over an association (ov_containment or other) or an variable vector
	 * @param {SVGElement} VisualObject Object to manipulate the visualisation
	 * @param {String} ObjectPath Path to this cshmi object containing the event/action/visualisation
	 * @return {Boolean} false if an error occured, returnValue of the called actions
	 */
	_interpreteChildrenIterator: function(VisualObject, ObjectPath){
		var FBRef = this._getFBReference(VisualObject);
		if (FBRef === ""){
			HMI.hmi_log_info_onwebsite('ChildrenIterator '+ObjectPath+' could not work. Found no FBReference on a parent.');
			return false;
		}
		
		var ChildrenType;
		//if the Object was scanned earlier, get the cached information (could be the case with templates or repeated/cyclic calls to the same object)
		if (!(this.ResourceList.Actions && this.ResourceList.Actions[ObjectPath] !== undefined)){
			var requestList = new Object();
			requestList[ObjectPath] = new Object();
			requestList[ObjectPath]["ChildrenType"] = null;
			
			var successCode = this._requestVariablesArray(requestList);
			if (successCode === false){
				return null;
			}
			
			ChildrenType = requestList[ObjectPath]["ChildrenType"];
			
			//feeding garbage collector early
			requestList = null;
			
			//we have asked the object successful, so remember the result
			this.ResourceList.Actions[ObjectPath] = new Object();
			this.ResourceList.Actions[ObjectPath].ChildrenIteratorParameterChildrenType = ChildrenType;
		}else{
			//the object is asked this session, so reuse the config to save communication requests
			ChildrenType = this.ResourceList.Actions[ObjectPath].ChildrenIteratorParameterChildrenType;
		}
		
		//this will be increased in a successful instantiateTemplate
		this.ResourceList.ChildrenIterator.currentCount = 0;
		
		//remember an old currentChild (for cascaded iterators)
		var savedCurrentChild = this.ResourceList.ChildrenIterator.currentChild;
		delete this.ResourceList.ChildrenIterator.currentChild;
		
		var returnValue = true;
		if (ChildrenType.indexOf("OT_") !== -1){
			//GetEP requested
			var response = HMI.KSClient.getEP(encodeURI(FBRef), ChildrenType, "OP_ANY", null);
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
					//todo prefix path from active fbref
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
			//GetVar on a (vector?)-value requested
			//doku multiple values possible
			
			//allow a list of variables as ChildrenTypes
			var ChildrenTypeList = ChildrenType.split(" ");
			var response;
			for (var i=0; i < ChildrenTypeList.length; i++) {
				response = HMI.KSClient.getVar(FBRef +'.' + ChildrenTypeList[i], "OP_VALUE", null);
				if (response === false || response === null){
					continue;
				}
				//get a rid of external brackets 
				response = response.replace(/{/g, "");
				response = response.replace(/}/g, "");
				var responseArray = HMI.KSClient.splitKsResponse(response, 1);
				
				for (var j=0; j<responseArray.length; j++){
					var responseDictionary = Array();
					responseDictionary["OP_VALUE"] = responseArray[j];
					responseDictionary["OP_NAME"] = responseArray[j];
					
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
		if(VisualObject.tagName !== "polyline"){
			HMI.hmi_log_info_onwebsite("RoutePolyline not supported with: "+VisualObject.tagName+"-Objects (path: "+ObjectPath+")");
			return false;
		}
		
		var SourceConnectionPoint = null;
		var SourceConnectionPointdirection = "Right";
		var TargetConnectionPoint = null;
		var TargetConnectionPointdirection = "Left";
		var OffsetSource = 0;
		var OffsetTarget = 0;
		
		//if the Object was routed earlier, get the cached information (could be the case with templates or repeated/cyclic calls to the same object)
		//Not the config is cached here, but the result! So caching VisualObject specific, not ObjectPath
		if (VisualObject.ResourceList && VisualObject.ResourceList.RoutePolyline !== undefined){
			//the object is asked this session, so reuse the config to save communication requests
			SourceConnectionPoint = VisualObject.ResourceList.RoutePolyline.SourceConnectionPoint;
			SourceConnectionPointdirection = VisualObject.ResourceList.RoutePolyline.SourceConnectionPointdirection;
			TargetConnectionPoint = VisualObject.ResourceList.RoutePolyline.TargetConnectionPoint;
			TargetConnectionPointdirection = VisualObject.ResourceList.RoutePolyline.TargetConnectionPointdirection;
			OffsetSource = VisualObject.ResourceList.RoutePolyline.OffsetSource;
			OffsetTarget = VisualObject.ResourceList.RoutePolyline.OffsetTarget;
		}else{
			var requestList = new Object();
			//This caching is model config specific, not instance specific
			if (this.ResourceList.Actions && this.ResourceList.Actions[ObjectPath] !== undefined){
				//the object is asked this session, so reuse the config to save communication requests
				requestList[ObjectPath] = this.ResourceList.Actions[ObjectPath].Parameters;
			}else{
				requestList[ObjectPath] = new Object();
				requestList[ObjectPath]["offset"] = null;
				requestList[ObjectPath]["gridWidth"] = null;
				
				var successCode = this._requestVariablesArray(requestList);
				if (successCode === false){
					return null;
				}
				
				//we have asked the object successful, so remember the result
				this.ResourceList.Actions[ObjectPath] = new Object();
				this.ResourceList.Actions[ObjectPath].Parameters = requestList[ObjectPath];
			}
			
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
			
			var FBRef = this._getFBReference(VisualObject);
			
			//in DOM every object has a full server+Host name, so if the basename does not provide it, 
			//we have to add it
			// e.g "//dev/ov_hmidemo7/TechUnits/TU10/h_bkqwmtbbhpf"" --> use prefix "//dev/ov_hmidemo7"
			var prefix = "";
			if (FBRef !== ""){
				//find the 3rd "/"
				var slashIndexAfterHost = FBRef.indexOf("/", 2);
				//find the 4th "/"
				var slashIndexAfterServer = FBRef.indexOf("/", slashIndexAfterHost+1);
				//only keep the String before 4th "/"
				prefix = FBRef.slice(0, slashIndexAfterServer);
			}
			
			var Source;
			var Target;
			
			if(SourceBasename.charAt(0) === "/" && SourceBasename.charAt(1) !== "/"){
				//Basename has no prefix (perhaps from an association)
				SourceBasename = prefix + SourceBasename;
			}
			if(TargetBasename.charAt(0) === "/" && TargetBasename.charAt(1) !== "/"){
				//Basename has no prefix (perhaps from an association)
				TargetBasename = prefix + TargetBasename;
			}
			
			if (SourceBasename === ""){
				Source = "";
			}else if (SourceVariablename !== ""){
				//in the svg separators is always /, since it is named via ConfigValue "Name"
				Source = SourceBasename + "/" + SourceVariablename;
			}else{
				Source = SourceBasename;
			}
			if (TargetBasename === ""){
				Target = "";
			}else if (TargetVariablename !== ""){
				//in the svg separators is always /, since it is named via ConfigValue "Name"
				Target = TargetBasename + "/" + TargetVariablename;
			}else{
				Target = TargetBasename;
			}
			if(Source === ""){
				//skip
			}else if (null !== (Source = HMI.svgDocument.getElementById(Source))){
				//todo doku moeglichkeit zur direkten SourceConnectionPoint angabe (inkl richtung und default)
				
				
				//fixme multiple visualisations of the same object are not found correctly! could be the case in the tree and the cfcview
				if (Source.tagName === "circle"){
					SourceConnectionPoint = Source;
				}else{
					for(var i = 0; i < Source.childNodes.length;i++){
						// search tagName "circle" with name containing ConnectionPoint
						if (Source.childNodes[i].tagName === "circle" && Source.childNodes[i].id.indexOf("ConnectionPoint") !== -1){
							SourceConnectionPoint = Source.childNodes[i];
							break;
						}
					}
				}
				if(SourceConnectionPoint !== null){
					//asv problem due to ie bug for substr. Solved in ie9
					if(SourceConnectionPoint.id.toLowerCase().substr(-4) === "left"){
						SourceConnectionPointdirection = "Left";
					}else if(SourceConnectionPoint.id.toLowerCase().substr(-5) === "right"){
						SourceConnectionPointdirection = "Right";
					}else if(SourceConnectionPoint.id.toLowerCase().substr(-2) === "up"){
						SourceConnectionPointdirection = "Up";
					}else if(SourceConnectionPoint.id.toLowerCase().substr(-4) === "down"){
						SourceConnectionPointdirection = "Down";
					}
				}
				Source = null;
			}else{
				//the SourceConnectionPoint is outside the domain
				//search a connectionpoint recursively in all parent svgs for connectionsources that are outside the domain 	
				
				//there should only be one SourceConnectionPointOutsideDomain, so use the cached value if it has been searched before
				if (this.ResourceList.Actions && this.ResourceList.Actions["SourceConnectionPointOutsideDomain"] !== undefined){
					SourceConnectionPoint = this.ResourceList.Actions["SourceConnectionPointOutsideDomain"].SourceConnectionPoint;
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
			
			if(Target === ""){
				//skip
			}else if (null !== (Target = HMI.svgDocument.getElementById(Target))){
				if (Target.tagName === "circle"){
					TargetConnectionPoint = Target;
				}else{
					for(var i = 0; i < Target.childNodes.length;i++){
						// search tagName "circle" with name containing ConnectionPoint
						if (Target.childNodes[i].tagName === "circle" && Target.childNodes[i].id.indexOf("ConnectionPoint") !== -1){
							TargetConnectionPoint = Target.childNodes[i];
							break;
						}
					}
				}
				if (TargetConnectionPoint !== null){
					if(TargetConnectionPoint.id.toLowerCase().substr(-4) === "left"){
						TargetConnectionPointdirection = "Left";
					}else if(TargetConnectionPoint.id.toLowerCase().substr(-5) === "right"){
						TargetConnectionPointdirection = "Right";
					}else if(TargetConnectionPoint.id.toLowerCase().substr(-2) === "up"){
						TargetConnectionPointdirection = "Up";
					}else if(TargetConnectionPoint.id.toLowerCase().substr(-4) === "down"){
						TargetConnectionPointdirection = "Down";
					}
				}
				Target = null;
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
			if (SourceBasename === ""){
				//skip
			}else if (this.ResourceList.Actions["Connection_from_" + SourceBasename] !== undefined){
				//there is already an incomming connection for this Object
				this.ResourceList.Actions["Connection_from_" + SourceBasename].count += 1;
				OffsetSource = this.ResourceList.Actions["Connection_from_" + SourceBasename].count * parseFloat(requestList[ObjectPath]["gridWidth"]);
			}else{ 
				//1st incomming connection for this Object
				this.ResourceList.Actions["Connection_from_" + SourceBasename] = new Object();
				this.ResourceList.Actions["Connection_from_" + SourceBasename].count = 0;
				OffsetSource = 0;
			}
			
			if (TargetBasename === ""){
				//skip
			}else if (this.ResourceList.Actions["Connection_to_" + TargetBasename] !== undefined){
				//there is already an outgoing connection for this Object
				this.ResourceList.Actions["Connection_to_" + TargetBasename].count += 1;
				OffsetTarget = this.ResourceList.Actions["Connection_to_" + TargetBasename].count * parseFloat(requestList[ObjectPath]["gridWidth"]);
			}else{
				//1st outgoing connection for this Object
				this.ResourceList.Actions["Connection_to_" + TargetBasename] = new Object();
				this.ResourceList.Actions["Connection_to_" + TargetBasename].count = 0;
				OffsetTarget = 0;
			}
			
			// add minimum #pixel distance from Object till 1st direction change of connection
			OffsetSource += parseFloat(requestList[ObjectPath]["offset"]);
			OffsetTarget += parseFloat(requestList[ObjectPath]["offset"]);
			
			//remember the result
			VisualObject.ResourceList = new Object();
			VisualObject.ResourceList.RoutePolyline = new Object();
			VisualObject.ResourceList.RoutePolyline.SourceConnectionPoint = SourceConnectionPoint;
			VisualObject.ResourceList.RoutePolyline.SourceConnectionPointdirection = SourceConnectionPointdirection;
			VisualObject.ResourceList.RoutePolyline.TargetConnectionPoint = TargetConnectionPoint;
			VisualObject.ResourceList.RoutePolyline.TargetConnectionPointdirection = TargetConnectionPointdirection;
			VisualObject.ResourceList.RoutePolyline.OffsetSource = OffsetSource;
			VisualObject.ResourceList.RoutePolyline.OffsetTarget = OffsetTarget;
		}
		
		if (SourceConnectionPoint === null){
			//could not paint something
			return false;
		}else if(TargetConnectionPoint === null){
			//could not paint something
			return false;
		}
		var IteratorObj = SourceConnectionPoint;
		while( (IteratorObj = IteratorObj.parentNode) && IteratorObj !== null && IteratorObj.namespaceURI == HMI.HMI_Constants.NAMESPACE_SVG){
			if(IteratorObj.getAttribute("display") === "none"){
				//connection object is invisible, so hide us, too.
				VisualObject.setAttribute("points", "");
				return true;
			}
		}
		IteratorObj = TargetConnectionPoint;
		while( (IteratorObj = IteratorObj.parentNode) && IteratorObj !== null && IteratorObj.namespaceURI == HMI.HMI_Constants.NAMESPACE_SVG){
			if(IteratorObj.getAttribute("display") === "none"){
				//connection object is invisible, so hide us, too.
				VisualObject.setAttribute("points", "");
				return true;
			}
		}
		
		var xStart = parseInt(SourceConnectionPoint.getAttribute("absolutex"), 10) +
			parseInt(SourceConnectionPoint.getAttribute("cx"), 10);
		var yStart = parseInt(SourceConnectionPoint.getAttribute("absolutey"), 10) +
			parseInt(SourceConnectionPoint.getAttribute("cy"), 10);
		
		var xEnd = parseInt(TargetConnectionPoint.getAttribute("absolutex"), 10) +
			parseInt(TargetConnectionPoint.getAttribute("cx"), 10);
		var yEnd = parseInt(TargetConnectionPoint.getAttribute("absolutey"), 10) +
			parseInt(TargetConnectionPoint.getAttribute("cy"), 10);
		
		//if start- and endPoints changed since last time, recompute polyline points
		if (	xStart !== VisualObject.ResourceList.RoutePolyline.xStart ||
				yStart !== VisualObject.ResourceList.RoutePolyline.yStart ||
				xEnd !== VisualObject.ResourceList.RoutePolyline.xEnd ||
				yEnd !== VisualObject.ResourceList.RoutePolyline.yEnd) {
			//cache new start- and endPoints
			VisualObject.ResourceList.RoutePolyline.xStart = xStart;
			VisualObject.ResourceList.RoutePolyline.yStart = yStart;
			VisualObject.ResourceList.RoutePolyline.xEnd = xEnd;
			VisualObject.ResourceList.RoutePolyline.yEnd = yEnd;
			
			var points = "";
			
			//we are perhaps not at global coordinate 0,0
			xStart = xStart - VisualObject.parentNode.getAttribute("absolutex");
			xEnd = xEnd - VisualObject.parentNode.getAttribute("absolutex");
			yStart = yStart - VisualObject.parentNode.getAttribute("absolutey");
			yEnd = yEnd - VisualObject.parentNode.getAttribute("absolutey");
			
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
	 * rebuilds the VisualObject
	 * @param {SVGElement} VisualObject Object to manipulate the visualisation
	 * @param {String} ObjectPath Path to this cshmi object containing the event/action/visualisation
	 * @return {Boolean} true on success, false if an error occured
	 */
	_interpreteRebuildObject: function(VisualObject, ObjectPath){
		//get list of parameters for rebuilding
		if(VisualObject.parentNode === null){
			//could be a just replaced VisualObject, not in the DOM tree
			return false;
		}
		//dim object while recreation
		VisualObject.setAttribute("opacity", "0.6");
		
		var VisualParentObject = null;
		if (VisualObject.parentNode.namespaceURI === HMI.HMI_Constants.NAMESPACE_SVG){
			VisualParentObject = VisualObject.parentNode;
		}
		
		var ObjectPath = VisualObject.getAttribute("data-ModelSource");
		var ObjectType = VisualObject.getAttribute("data-ObjectType");
		if(VisualObject.FBReference && VisualObject.FBReference["default"] !== undefined && VisualObject.FBReference["default"] !== ""){
			//save a (perhaps changed) FBref for later rebuilding of the template
			this.ResourceList.newRebuildObjectId = VisualObject.id;
		}
		var newVisualObject = this.BuildDomain(VisualParentObject, ObjectPath, ObjectType, false);
		this.ResourceList.newRebuildObjectId = null;
		
		//find references to the old VisualObject in cshmiOriginalOrderList of the VisualParentObject and change to new
		if(VisualParentObject !== null && VisualParentObject.cshmiOriginalOrderList !== undefined){
			//place the new object to the same order position as the old object
			var OrderArray = VisualParentObject.cshmiOriginalOrderList[VisualObject.getAttribute("data-ModelSource")];
			for (var i = 0; OrderArray !== undefined && i < OrderArray.length;) {
				if (OrderArray[i] === VisualObject){
					//we found an old entry, change to new
					OrderArray[i] = newVisualObject;
					i++;
				}else if(OrderArray[i] === newVisualObject){
					//we found an new entry, inserted in the creation, remove
					OrderArray.splice(i, 1);
				}
			}
			//same for Template Objects
			OrderArray = VisualParentObject.cshmiOriginalOrderList[VisualObject.getAttribute("data-TemplateModelSource")];
			for (var i = 0; OrderArray !== undefined && i < OrderArray.length;) {
				if (OrderArray[i] === VisualObject){
					//we found an old entry, change to new
					OrderArray[i] = newVisualObject;
					i++;
				}else if(OrderArray[i] === newVisualObject){
					//we found an new entry, inserted in the creation, remove
					OrderArray.splice(i, 1);
				}
			}
		}
		
		//remove ourself out of the globalvarchangeStack
		//caveat: our children will not be removed from stack, therefor the whole subtree will be moved out of dom later and tested on execution
		for (var i = 0; i < HMI.cshmi.ResourceList.globalvarChangedCallStack.length;){
			var EventObjItem = HMI.cshmi.ResourceList.globalvarChangedCallStack[i];
			if(EventObjItem["VisualObject"] === VisualObject){
				//we found an old entry, remove
				HMI.cshmi.ResourceList.globalvarChangedCallStack.splice(i, 1);
			}else{
				i++;
			}
		}
		//we want to have offset parameter on all visual elements
		HMI.saveAbsolutePosition(newVisualObject, true);
		var ComponentChilds = newVisualObject.getElementsByTagNameNS(HMI.HMI_Constants.NAMESPACE_SVG, "*");
		for(var i = 0;i < ComponentChilds.length;i++){
			HMI.saveAbsolutePosition(ComponentChilds[i], false);
		}
		ComponentChilds = null;
		
		VisualObject.parentNode.replaceChild(newVisualObject, VisualObject);
		
		if(this.trashDocument && this.trashDocument.adoptNode){
			//move old node out of document, to make clear it is not used anymore (HACK)
			this.trashDocument.adoptNode(VisualObject);
		}
		//note the references in cshmiOriginalOrderList are only internal in the VisualObject tree.
		//So the garbage collection will not have a problem with the circular reference.
		
		//interprete onload Actions if we are already loaded
		if (this.initStage === false){
			this._interpreteOnloadCallStack();
		}
		
		return true;
	},
	
	/**
	 * Action which calls _buildFromTemplate to build a template
	 * @param {SVGElement} VisualParentObject visual Object which is parent to active Object
	 * @param {String} ObjectPath Path to this cshmi object containing the event/action/visualisation
	 * @return {Boolean} true on success, false if an error occured
	 */
	_interpreteInstantiateTemplate: function(VisualParentObject, ObjectPath){
		if(VisualParentObject.getElementById === undefined){
			HMI.hmi_log_info_onwebsite("InstantiateTemplate is not allowed under an Element (configured here: "+ObjectPath+")");
			return false;
		}
		var VisualObject = this._buildFromTemplate(VisualParentObject, ObjectPath, true, false);
		if (VisualObject !== null){
			//special handling for invisible objects
			if (VisualObject.getAttribute("display") === "none"){
				HMI.addClass(VisualObject, this.cshmiObjectVisibleChildrenNotLoaded);
			}
			
			VisualParentObject.appendChild(VisualObject);
			//calculate all offset parameter to be able to display visual feedback
			//needed now, because we append new components
			HMI.saveAbsolutePosition(VisualObject);
			//we want to have offset parameter on all visual elements
			var ComponentChilds = VisualObject.getElementsByTagNameNS(HMI.HMI_Constants.NAMESPACE_SVG, '*');
			for(var i = 0;i < ComponentChilds.length;i++){
				HMI.saveAbsolutePosition(ComponentChilds[i]);
			}
			
			//interprete onload Actions if we are already loaded
			if (this.initStage === false){
				this._interpreteOnloadCallStack();
			}
			
			return true;
		}
		return false;
	},
	
	/**
	 * builds template, gets the parameter via KS
	 * @param {SVGElement} VisualParentObject visual Object which is parent to active Object
	 * @param {String} ObjectPath Path to this cshmi object containing the event/action/visualisation
	 * @param {Boolean} calledFromInstantiateTemplate true if called from an action
	 * @param {bool} preventNetworkRequest the function should prevent network requests if possible
	 * @return {SVGElement} VisualObject the new constructed element or null
	 */
	_buildFromTemplate: function(VisualParentObject, ObjectPath, calledFromInstantiateTemplate, preventNetworkRequest){
		var requestList = new Object();
		//if the Object was scanned earlier, get the cached information (could be the case with templates or repeated/cyclic calls to the same object)
		if (this.ResourceList.Elements && this.ResourceList.Elements[ObjectPath] !== undefined){
			//the object is asked this session, so reuse the config to save communication requests
			requestList[ObjectPath] = this.ResourceList.Elements[ObjectPath].Parameters;
		}else if(preventNetworkRequest === true && (calledFromInstantiateTemplate === true && this.ResourceList.ChildrenIterator.currentChild !== undefined)){
			//not possible if called from action under a childrenIterator. There is the same ObjectPath, but different Objects under the same domain
			
			//build a skeleton to preserve zindex/sequence
			var VisualObject = HMI.svgDocument.createElementNS(HMI.HMI_Constants.NAMESPACE_SVG, 'svg');
			VisualObject.id = ObjectPath;
			return VisualObject;
		}else{
			requestList[ObjectPath] = new Object();
			requestList[ObjectPath]["visible"] = null;
			requestList[ObjectPath]["stroke"] = null;
			requestList[ObjectPath]["fill"] = null;
			requestList[ObjectPath]["opacity"] = null;
			requestList[ObjectPath]["rotate"] = null;
			requestList[ObjectPath]["x"] = null;
			requestList[ObjectPath]["y"] = null;
			if (calledFromInstantiateTemplate === true){
				requestList[ObjectPath]["xOffset"] = null;
				requestList[ObjectPath]["yOffset"] = null;
				requestList[ObjectPath]["maxTemplatesPerDirection"] = null;
			}
			requestList[ObjectPath]["TemplateDefinition"] = null;
			requestList[ObjectPath]["FBReference"] = null;
			requestList[ObjectPath]["FBVariableReference"] = null;
			requestList[ObjectPath]["ConfigValues"] = null;
			requestList[ObjectPath]["hideable"] = null;
			
			var successCode = this._requestVariablesArray(requestList);
			if (successCode === false){
				return null;
			}
			
			//we have asked the object successful, so remember the result
			this.ResourceList.Elements[ObjectPath] = new Object();
			this.ResourceList.Elements[ObjectPath].Parameters = requestList[ObjectPath];
		}
		
		var TemplateLocation = "/TechUnits/cshmi/Templates/";
		var PathOfTemplateDefinition = TemplateLocation+requestList[ObjectPath]["TemplateDefinition"];
		if (requestList[ObjectPath]["TemplateDefinition"] === ""){
			HMI.hmi_log_info_onwebsite("Template "+ObjectPath+" is not configured");
			return null;
		}else if (ObjectPath.indexOf(PathOfTemplateDefinition) === 0 && this.ResourceList.ChildrenIterator.currentChild === undefined){
			HMI.hmi_log_info_onwebsite("Template "+ObjectPath+" is calling itself outside an iterator");
			return null;
		}
		
		var requestListTemplate = new Object();
		//if the Object was scanned earlier, get the cached information (could be the case with templates or repeated/cyclic calls to the same object)
		if (this.ResourceList.Elements && this.ResourceList.Elements[PathOfTemplateDefinition] !== undefined){
			//the object is asked this session, so reuse the config to save communication requests
			requestListTemplate[PathOfTemplateDefinition] = this.ResourceList.Elements[PathOfTemplateDefinition].Parameters;
		}else{
			requestListTemplate[PathOfTemplateDefinition] = new Object();
			requestListTemplate[PathOfTemplateDefinition]["width"] = null;
			requestListTemplate[PathOfTemplateDefinition]["height"] = null;
			
			successCode = this._requestVariablesArray(requestListTemplate);
			if (successCode === false){
				HMI.hmi_log_info_onwebsite("Template "+ObjectPath+" is wrong configured. TemplateDefinition '"+TemplateLocation+requestList[ObjectPath]["TemplateDefinition"]+"' is not available.");
				return null;
			}
			
			//we have asked the object successful, so remember the result
			this.ResourceList.Elements[PathOfTemplateDefinition] = new Object();
			this.ResourceList.Elements[PathOfTemplateDefinition].Parameters = requestListTemplate[PathOfTemplateDefinition];
		}
		
		//search a predefined children
		var VisualObject = VisualParentObject.getElementById(ObjectPath);
		
		if (VisualObject === null || (calledFromInstantiateTemplate === true && this.ResourceList.ChildrenIterator.currentChild !== undefined)){
			//do not use a predefined VisualObject if called from action under a childrenIterator. There is the same ObjectPath, but different Objects under the same domain
			
			VisualObject = HMI.svgDocument.createElementNS(HMI.HMI_Constants.NAMESPACE_SVG, 'svg');
		}
		
		//id should be the name of the parent plus our identifier
		var NameList = PathOfTemplateDefinition.split("/");
		VisualObject.id = VisualParentObject.id + "/" + NameList[NameList.length-1];
		NameList = null;
		
		VisualObject.setAttribute("data-ModelSource", ObjectPath);
		VisualObject.setAttribute("data-TemplateModelSource", PathOfTemplateDefinition);
		VisualObject.setAttribute("data-NameOrigin", "TemplateName");
		
		HMI.addClass(VisualObject, this.cshmiTemplateClass);
		
		if (calledFromInstantiateTemplate === true){
			HMI.addClass(VisualObject, this.cshmiTemplateActionClass);
		}
		
		this._armToggleChildVisibility(VisualParentObject, VisualObject, ObjectPath, requestList);
		
		//the VPO is needed in generateFullKsPath
		VisualObject.VisualParentObject = VisualParentObject;
		
		//###########################################################################
		//parametrise templateDefinition with the config
		VisualObject.FBReference = Object();
		VisualObject.FBVariableReference = Object();
		VisualObject.ConfigValues = Object();
		
		
		////////////////////////////////////////////////////////////////////////////
		//FBReference
		
		var FBReferenceList = requestList[ObjectPath]["FBReference"].split(" ");
		for (var i=0; i < FBReferenceList.length; i++) {
			if (i > 1){
				HMI.hmi_log_info_onwebsite("Only one FBReference is valid. "+ObjectPath+" has more of them.");
				break;
			}
			if (FBReferenceList[i] !== ""){
				//save the info to the default position 
				
				if(this.ResourceList.newRebuildObjectId !== null){
					//we should use a preconfigured FBref, instead of the configured one...
					VisualObject.FBReference["default"] = this.ResourceList.newRebuildObjectId;
					VisualObject.id = this.ResourceList.newRebuildObjectId;
					
					//.. but only once/here
					this.ResourceList.newRebuildObjectId = null;
					break;
				}
				
				if (calledFromInstantiateTemplate === true && this.ResourceList.ChildrenIterator.currentChild !== undefined && this.ResourceList.ChildrenIterator.currentChild[FBReferenceList[i]] !== undefined){
					//something like OP_NAME or OP_VALUE was requested, so we have to find the real info from the iterator
					
					//getFBReference does all the magic. The later checks are to detect the NameOrigin
					var FBRef = this._getFBReference(VisualParentObject);
					if (FBRef === ""){
						HMI.hmi_log_info_onwebsite('Template '+ObjectPath+' is wrong configured. Found no FBReference on a parent.');
						return null;
					}else if (FBRef.indexOf("//localhost") === 0){
						//localhost in the model should not be the http-gateway
						FBRef = FBRef.replace(/localhost/, HMI.KSClient.ResourceList.ModelHost);
					}
					if (this.ResourceList.ChildrenIterator.currentChild[FBReferenceList[i]].charAt(0) === "/"){
						// the iterated string begins with / so it is a fullpath (likely from a GetVar on an assoziation)
						VisualObject.setAttribute("data-NameOrigin", "CurrentChildfullpath");
					}else{
						//In OP_NAME is a relative path (likely from a GetEP request).
						VisualObject.setAttribute("data-NameOrigin", "FBReference/OP_NAME");
					}
					VisualObject.FBReference["default"] = FBRef;
					VisualObject.id = FBRef;
				}else{
					//We have straightforward a full name of one FB Object, so save it with the default name
					VisualObject.FBReference["default"] = this._generateFullKsPath(VisualObject, ObjectPath, FBReferenceList[i]);
					if (FBReferenceList[i].charAt(0) === "/"){
						//full path is given
						VisualObject.setAttribute("data-NameOrigin", "FBReference");
					}else{
						//relative path is given, so complete the path with the BaseKsPath
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
			if (calledFromInstantiateTemplate === true && this.ResourceList.ChildrenIterator.currentChild !== undefined && this.ResourceList.ChildrenIterator.currentChild[FBVariableReferenceEntry[1]] !== undefined){
				//we want to get values from the current child (e.g. myFavoriteVar:OP_NAME)
				
				if (this.ResourceList.ChildrenIterator.currentChild[FBVariableReferenceEntry[1]].charAt(0) !== "/"){
					//Value from currentChild is not a full path, so search for it in the parents
					
					var FBRef = this._getFBReference(VisualParentObject);
					if (FBRef === ""){
						HMI.hmi_log_info_onwebsite('Template '+ObjectPath+' is wrong configured. Found no FBReference on a parent.');
						return null;
					}else if (FBRef.indexOf("//localhost") === 0){
						//localhost in the model should not be the http-gateway
						FBRef = FBRef.replace(/localhost/, HMI.KSClient.ResourceList.ModelHost);
					}
					VisualObject.FBVariableReference[FBVariableReferenceEntry[0]] = FBRef;
				}else{
					//currentChild set a full path
					VisualObject.FBVariableReference[FBVariableReferenceEntry[0]] = this.ResourceList.ChildrenIterator.currentChild[FBVariableReferenceEntry[1]];
				}
			}else{
				//direct setting of a FBVariable
				FBVariableReferenceEntry[1] = this._generateFullKsPath(VisualObject, ObjectPath, FBVariableReferenceEntry[1]);
				VisualObject.FBVariableReference[FBVariableReferenceEntry[0]] = FBVariableReferenceEntry[1];
			}
		}
		
		//the VPO was only needed in generateFullKsPath
		delete VisualObject.VisualParentObject;
		
		////////////////////////////////////////////////////////////////////////////
		//ConfigValue
		
		//ConfigValue is something like "pumpcolor:yellow pumpname:N18"
		var ConfigValueList = requestList[ObjectPath]["ConfigValues"].split(" ");
		var KeyValueEntry = null;
		var lastEntry = null;
		for (var i=0; i < ConfigValueList.length; i++) {
			KeyValueEntry = ConfigValueList[i].split(":");
			if (KeyValueEntry.length >= 2){
				if (KeyValueEntry.length > 2){
					var Value = KeyValueEntry.slice(1).join(":");
				}else{
					Value = KeyValueEntry[1];
				}
				//check if we want to get values from the current child (e.g. OP_NAME)
				//if instantiateTemplate is not called within a childreniterator, the currentChild is undefined
				if (calledFromInstantiateTemplate === true && this.ResourceList.ChildrenIterator.currentChild !== undefined && this.ResourceList.ChildrenIterator.currentChild[Value] !== undefined){
					VisualObject.ConfigValues[KeyValueEntry[0]] = this.ResourceList.ChildrenIterator.currentChild[Value];
				}
				else{
					VisualObject.ConfigValues[KeyValueEntry[0]] = Value;
				}
				//doku
				if(KeyValueEntry[0] === "Name"){
					//if an template has the configValue Name:Dieter, this should be the object id
					if (VisualParentObject.getAttribute("data-NameOrigin") !== "TemplateName"){
						//if the parent was named, append our name
						VisualObject.id = VisualParentObject.id + "/" + VisualObject.ConfigValues[KeyValueEntry[0]];
						VisualObject.setAttribute("data-NameOrigin", "Parent+ConfigValue");
					}else{
						//our parent was named with an TemplateName, append our Name
						VisualObject.id = VisualObject.ConfigValues[KeyValueEntry[0]];
						VisualObject.setAttribute("data-NameOrigin", "ConfigValue");
					}
				}
				lastEntry = KeyValueEntry[0];
			}else if (KeyValueEntry.length === 1 && lastEntry !== null){
				//we had something like "pumpcolor:yellow pumpname:N 18", so need to add the " 18" to the last entry
				VisualObject.ConfigValues[lastEntry] = VisualObject.ConfigValues[lastEntry]+" "+KeyValueEntry[0];
			}
		}
		
		
		/////////////////////////////////////////////////////////////////////////////
		//Iterator position changing
		
		//save Template configuration for caching
		var xTemplate = requestList[ObjectPath]["x"];
		var yTemplate = requestList[ObjectPath]["y"];
		
		if (calledFromInstantiateTemplate === true && this.ResourceList.ChildrenIterator.currentChild !== undefined){
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
		if (requestList[ObjectPath]["rotate"] && requestList[ObjectPath]["rotate"] !== "0"){
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
		
		//instance order is lost on zindex manipulation (dom reordering)
		//so build a second childNodes-like collection for preserving ordering
		VisualObject.cshmiOriginalOrderList = new Object();
		
		//add ourself to the parent zindex-list
		if(VisualParentObject.cshmiOriginalOrderList === undefined){
			VisualParentObject.cshmiOriginalOrderList = new Object();
		}
		if(VisualParentObject.cshmiOriginalOrderList[PathOfTemplateDefinition] === undefined){
			VisualParentObject.cshmiOriginalOrderList[PathOfTemplateDefinition] = new Array();
		}
		VisualParentObject.cshmiOriginalOrderList[PathOfTemplateDefinition].push(VisualObject);
		
		//////////////////////////////////////////////////////////////////////////
		//get childs (graphics and actions) from the TemplateDefinition
		//our child will be fetched later
		
		
		// special handling for invisible objects
		if (VisualObject !== null && VisualObject.getAttribute("display") === "none"){
			//we are not visible, so we should not contact network
			
			//the marking of the class will be done in the caller of our function
			this._loadChildren(VisualObject, PathOfTemplateDefinition, true);
		}else if (VisualObject !== null){
			this._loadChildren(VisualObject, PathOfTemplateDefinition, false);
		}
		
		if (VisualChildObject){
			//transformed Template for rotation
			return VisualChildObject;
		}else{
			return VisualObject;
		}
	},

	/**
	 * builds SVG container, gets the parameter via KS
	 * @param {SVGElement} VisualParentObject visual Object which is parent to active Object
	 * @param {String} ObjectPath Path to this cshmi object containing the event/action/visualisation
	 * @param {bool} preventNetworkRequest the function should prevent network requests if possible
	 * @return {SVGElement} VisualObject the new constructed element or null
	 */
	_buildBlackbox: function(VisualParentObject, ObjectPath, preventNetworkRequest){
		var requestList = new Object();
		//if the Object was scanned earlier, get the cached information (could be the case with templates or repeated/cyclic calls to the same object)
		if (this.ResourceList.Elements && this.ResourceList.Elements[ObjectPath] !== undefined){
			//the object is asked this session, so reuse the config to save communication requests
			requestList[ObjectPath] = this.ResourceList.Elements[ObjectPath].Parameters;
		}else if(preventNetworkRequest === true){
			//build a skeleton to preserve zindex/sequence
			var VisualObject = HMI.svgDocument.createElementNS(HMI.HMI_Constants.NAMESPACE_SVG, 'svg');
			VisualObject.id= ObjectPath;
			return VisualObject;
		}else{
			requestList[ObjectPath] = new Object();
			requestList[ObjectPath]["x"] = null;
			requestList[ObjectPath]["y"] = null;
			requestList[ObjectPath]["width"] = null;
			requestList[ObjectPath]["height"] = null;
			requestList[ObjectPath]["visible"] = null;
			requestList[ObjectPath]["opacity"] = null;
			requestList[ObjectPath]["rotate"] = null;
			requestList[ObjectPath]["sourceOfLibrary"] = null;
			requestList[ObjectPath]["HTMLcontent"] = null;
			
			
			var successCode = this._requestVariablesArray(requestList);
			if (successCode === false){
				return null;
			}
			
			//slice braces '{}' if any
			var HTMLcontent_braces = HMI.KSClient.getVar(ObjectPath+".HTMLcontent", "OP_VALUE", null);
			var j = 0;
			while(HTMLcontent_braces.charAt(0) === '{') {
				HTMLcontent_braces = HTMLcontent_braces.substr(1);
				j++;
			}
			HTMLcontent_braces = HTMLcontent_braces.substr(0, HTMLcontent_braces.length - j);
			requestList[ObjectPath]["HTMLcontent"] = HTMLcontent_braces;
			
			//get via KSClient because of problems getting '{}' caused by splitKsResponse
			//slice braces '{}' if any
			var jsOnload_braces = HMI.KSClient.getVar(ObjectPath+".jsOnload", "OP_VALUE", null);
			var j = 0;
			while(jsOnload_braces.charAt(0) === '{') {
				jsOnload_braces = jsOnload_braces.substr(1);
				j++;
			}
			jsOnload_braces = jsOnload_braces.substr(0, jsOnload_braces.length - j);
			requestList[ObjectPath]["jsOnload"] = jsOnload_braces;

			//slice braces '{}' if any
			var jsOnglobalvarchanged_braces = HMI.KSClient.getVar(ObjectPath+".jsOnglobalvarchanged", "OP_VALUE", null);
			var j = 0;
			while(jsOnglobalvarchanged_braces.charAt(0) === '{') {
				jsOnglobalvarchanged_braces = jsOnglobalvarchanged_braces.substr(1);
				j++;
			}
			jsOnglobalvarchanged_braces = jsOnglobalvarchanged_braces.substr(0, jsOnglobalvarchanged_braces.length - j);
			requestList[ObjectPath]["jsOnglobalvarchanged"] = jsOnglobalvarchanged_braces;
			
			//we have asked the object successful, so remember the result
			this.ResourceList.Elements[ObjectPath] = new Object();
			this.ResourceList.Elements[ObjectPath].Parameters = requestList[ObjectPath];
		}
		
		if (VisualParentObject !== null){
			//search a predefined children
			var VisualObject = VisualParentObject.getElementById(ObjectPath);
		}
		
		if (VisualObject === null || VisualObject === undefined){
			VisualObject = HMI.svgDocument.createElementNS(HMI.HMI_Constants.NAMESPACE_SVG, 'svg');
		}
		
		if (VisualParentObject !== null){
			//id should be the name of the parent plus our identifier
			var NameList = ObjectPath.split("/");
			VisualObject.id = VisualParentObject.id + "/" + NameList[NameList.length-1];
			NameList = null;
		}else{
			//we are the main sheet, so no parent available
			VisualObject.id = "//"+HMI.KSClient.ResourceList.ModelHost+"/"+HMI.KSClient.ResourceList.ModelServer+ObjectPath;
		}
		VisualObject.setAttribute("data-ModelSource", ObjectPath);
		
		HMI.addClass(VisualObject, this.cshmiBlackboxClass);
		
		//setting the basic Element Variables like .visible .stroke .fill .opacity .rotate
		this._processBasicVariables(VisualObject, requestList[ObjectPath]);

		VisualObject.setAttribute("overflow", "visible");
		
		var sourceList = requestList[ObjectPath]["sourceOfLibrary"];
		var jsOnload = requestList[ObjectPath]["jsOnload"];
		var HTMLcontent = requestList[ObjectPath]["HTMLcontent"];
				
		var sourceListSplitted = sourceList.split(" ");
		jsloadObserver = new cshmiObserver(VisualObject, ObjectPath, sourceListSplitted.length);
		

		loadLibrary:
		//externe (via http erreichbare) Bibliotheken in head anhaengen
		for(var i = 0; i < sourceListSplitted.length; ++i){
			var thisEntry = new ObserverEntry(sourceListSplitted[i]);
			jsloadObserver.ObserverEntryArray[i] = thisEntry;

			//append only if same JS-library is not already loaded
			var scripts = document.getElementsByTagName('script');
			
			for (var j = 0; j < scripts.length; ++j) {
				if (sourceListSplitted[i] === scripts[j].src){
					continue loadLibrary;
				}
			}
			
			var node = document.createElement("script");
			node.type = "text/javascript";
			node.src = sourceListSplitted[i];
			node.async = false;
//			node.onload = node.onerror = function(evt){
//				//todo hier sauber insertvalue oder so nutzen. parameter muss man rausfinden
//				thisEntry.requirementsFulfilled = true;
//				if(evt.type !== "load"){
//				};
//				HMI.hmi_log_info("loading script: event type: "+evt.type+": "+thisEntry.ObjectName);
//				jsloadObserver.checkAndTrigger();
//				//jsonload muss auch gestartet werden, wenn wir keine Bib laden!
//			};

			document.head.appendChild(node);
			jsloadObserver.checkAndTrigger();
		}

		//create foreignObject in <SVG>-Element
		var SVGWidth = VisualObject.getAttribute("width");
		var SVGLength = VisualObject.getAttribute("height");
		var HTML = "<foreignObject x='0' y='0' width='"+SVGWidth+"' height='"+SVGLength+"'><body xmlns='http://www.w3.org/1999/xhtml'>"+HTMLcontent+"</body></foreignObject>";
		var svgContent =	"<svg:svg xmlns:svg='"+HMI.HMI_Constants.NAMESPACE_SVG+"' xmlns='"+HMI.HMI_Constants.NAMESPACE_SVG+"' "
		+"xmlns:xlink='"+HMI.HMI_Constants.NAMESPACE_XLINK+"'>"
		+HTML
		+"</svg:svg>";
		
		//append foreignObject to VisualObject
		var newNode = HMI.HMIDOMParser.parse(svgContent);
		VisualObject.appendChild(newNode);
		
		
		//if Element with class "autosize/autosizeX/autosizeY" exists, adjust width&heigth/width/height taken from Client
		if (csHMIgetElementsByClassName(VisualObject, 'autosize').length != 0) {
			var classList = csHMIgetElementsByClassName(VisualObject, 'autosize');
			for (var i = 0; i < classList.length; ++i) {
				//special adjustment for <canvas>-Element
				if (classList[i].tagName.toLowerCase() === "canvas") {
					classList[i].width = VisualObject.getAttribute("width");
					classList[i].height = VisualObject.getAttribute("height");
				}
				classList[i].style.width = VisualObject.getAttribute("width")+"px";
				classList[i].style.height = VisualObject.getAttribute("height")+"px";
			}		
		}
		if (csHMIgetElementsByClassName(VisualObject, 'autosizeX').length != 0) {
			var classListX = csHMIgetElementsByClassName(VisualObject, 'autosizeX');
			for (var i = 0; i < classListX.length; ++i) {
				if (classListX[i].tagName.toLowerCase() === "canvas") {
					classListX[i].width = VisualObject.getAttribute("width");
				}
				classListX[0].style.width = VisualObject.getAttribute("width")+"px";
			}
		}
		if (csHMIgetElementsByClassName(VisualObject, 'autosizeY').length != 0) {
			var classListY = csHMIgetElementsByClassName(VisualObject, 'autosizeY');
			for (var i = 0; i < classListY.length; ++i) {
				if (classListY[i].tagName.toLowerCase() === "canvas") {
					classListY[i].height = VisualObject.getAttribute("height");
				}
				classListY[0].style.height = VisualObject.getAttribute("height")+"px";
			}
		}
		
		//create object 'cshmimodel'
		var responseArray = HMI.KSClient.getChildObjArray(ObjectPath, HMI.cshmi);
		var varNames = new Array();

		//find out at which position a SetValue stands
		for(var i = 0; i < responseArray.length; ++i){
			
			var objectType = responseArray[i].split(' ')[1];
			
			if(objectType.match(/SetValue/)){	
				//get names of created SetValues
				varNames.push(responseArray[i].split(' ')[0]);
			}
		}
		
		function myVariable(VisualObject, ObjectPath, varName) {
			this.VisualObject = VisualObject;
			this.ObjectPath = ObjectPath;
			this.varName = varName;
			this.getValue = function () {
				var getValueObjectPath = ObjectPath+"/"+varName+".value";
				return HMI.cshmi._getValue(VisualObject, getValueObjectPath);
			};
			this.setValue = function (newValue) {
				var setValueObjectPath = ObjectPath+"/"+varName;
				HMI.cshmi._setVarExecute(VisualObject, setValueObjectPath, newValue);
			};
		};
		
		var cshmimodel = new Object();
		cshmimodel.variables = new Object();

		for(var i = 0; i < varNames.length; ++i){
			cshmimodel.variables[varNames[i]] = new myVariable(VisualObject, ObjectPath, varNames[i]);
		}
		
		cshmimodel.instantiateTemplate = function(x, y, rotate, hideable, PathOfTemplateDefinition, FBReference, FBVariableReference, ConfigValues) {
			HMI.cshmi.ResourceList.Elements["tempPath"].Parameters = new Object();
			HMI.cshmi.ResourceList.Elements["tempPath"].Parameters["x"] = x;
			HMI.cshmi.ResourceList.Elements["tempPath"].Parameters["y"] = y;
			HMI.cshmi.ResourceList.Elements["tempPath"].Parameters["rotate"] = rotate;
			HMI.cshmi.ResourceList.Elements["tempPath"].Parameters["hideable"] = hideable;
			HMI.cshmi.ResourceList.Elements["tempPath"].Parameters["TemplateDefinition"] = PathOfTemplateDefinition;
			HMI.cshmi.ResourceList.Elements["tempPath"].Parameters["FBReference"] = FBReference;
			HMI.cshmi.ResourceList.Elements["tempPath"].Parameters["FBVariableReference"] = FBVariableReference;
			HMI.cshmi.ResourceList.Elements["tempPath"].Parameters["ConfigValues"] = ConfigValues;
			HMI.cshmi.ResourceList.Elements["tempPath"].Parameters["FBReference"] = FBReference;
			VisualObject.appendChild(HMI.cshmi._buildFromTemplate(VisualObject, "tempPath", false, false));
		};
		
		cshmimodel.getEP = function(path, requestType, requestOutput) {
			return HMI.KSClient.getEP(path, requestType, requestOutput);
		};

		cshmimodel.createObject = function(path, classname) {
			return HMI.KSClient.createObject(path, classname);
		};
		
		cshmimodel.deleteObject = function(path) {
			return HMI.KSClient.deleteObject(path);
		};
		
		cshmimodel.linkObjects = function(pathA, pathB, portnameA) {
			return HMI.KSClient.linkObjects(pathA, pathB, portnameA);
		};
		
		cshmimodel.unlinkObjects = function(pathA, pathB, portnameA) {
			return HMI.KSClient.unlinkObjects(pathA, pathB, portnameA);
		};
		
		cshmimodel.renameObjects = function(oldName, newName) {
			return HMI.KSClient.renameObject(oldName, newName);
		};
		
		//TODO: 
		//Error-log aufnehmen in API
		
		VisualObject.ResourceList = new Object();
		VisualObject.ResourceList.cshmimodel = cshmimodel;
		
		//"onload" - TODO: noch sporadisch
		window.setTimeout(function(){
			HMI.cshmi._executeScript(VisualObject, ObjectPath, jsOnload);
		}, 4000);
		
		return VisualObject;
	},

	/**
	 * executes JavaScript Code
	 * @param {SVGElement} VisualObject visual Object which is active Object
	 * @param {String} ObjectPath Path to this cshmi object containing the event/action/visualisation
	 * @param {String} jsOnload JavaScript-Code inserted in Blackbox and hereby executed via eval()
	 */

	_executeScript: function(VisualObject, ObjectPath, jsOnload){
		
		//declare object 'cshmimodel' for further use [usage e.g.: 'cshmimodel.variables.<VARNAME>.getValue();']
		var cshmimodel = VisualObject.ResourceList.cshmimodel;

		//evaluate JS-Code of Blackbox
		eval(jsOnload);
					
	},
	
	/***************************************************************************************************************
	 * builds SVG container, gets the parameter via KS
	 * @param {SVGElement} VisualParentObject visual Object which is parent to active Object
	 * @param {String} ObjectPath Path to this cshmi object containing the event/action/visualisation
	 * @param {bool} preventNetworkRequest the function should prevent network requests if possible
	 * @return {SVGElement} VisualObject the new constructed element or null
	 */
	
	//newwrite
	//alle buildSvg* in eine Funktion zusammenfassen, da sehr �hnlich. Mit this.ModellVariables...
	
	_buildSvgGroup: function(VisualParentObject, ObjectPath, preventNetworkRequest){
		var requestList = new Object();
		//if the Object was scanned earlier, get the cached information (could be the case with templates or repeated/cyclic calls to the same object)
		if (this.ResourceList.Elements && this.ResourceList.Elements[ObjectPath] !== undefined){
			//the object is asked this session, so reuse the config to save communication requests
			requestList[ObjectPath] = this.ResourceList.Elements[ObjectPath].Parameters;
		}else if(preventNetworkRequest === true){
			//build a skeleton to preserve zindex/sequence
			var VisualObject = HMI.svgDocument.createElementNS(HMI.HMI_Constants.NAMESPACE_SVG, 'svg');
			VisualObject.id = ObjectPath;
			return VisualObject;
		}else{
			requestList[ObjectPath] = new Object();
			if (VisualParentObject !== null){
				//x,y of the first svg should be 0,0 regardless of the config
				//with this "hack" we also can visualize a TemplateDefinition via deep link for testing
				requestList[ObjectPath]["x"] = null;
				requestList[ObjectPath]["y"] = null;
			}
			requestList[ObjectPath]["width"] = null;
			requestList[ObjectPath]["height"] = null;
			requestList[ObjectPath]["hideable"] = null;
			requestList[ObjectPath]["visible"] = null;
			
			var successCode = this._requestVariablesArray(requestList);
			if (successCode === false){
				return null;
			}
			//we have asked the object successful, so remember the result
			this.ResourceList.Elements[ObjectPath] = new Object();
			this.ResourceList.Elements[ObjectPath].Parameters = requestList[ObjectPath];
		}
		
		if (VisualParentObject !== null){
			//search a predefined children
			var VisualObject = VisualParentObject.getElementById(ObjectPath);
		}
		
		if (VisualObject === null || VisualObject === undefined){
			VisualObject = HMI.svgDocument.createElementNS(HMI.HMI_Constants.NAMESPACE_SVG, 'svg');
		}
		
		if (VisualParentObject !== null){
			//id should be the name of the parent plus our identifier
			var NameList = ObjectPath.split("/");
			VisualObject.id = VisualParentObject.id + "/" + NameList[NameList.length-1];
			NameList = null;
		}else{
			//we are the main sheet, so no parent available
			VisualObject.id = "//"+HMI.KSClient.ResourceList.ModelHost+"/"+HMI.KSClient.ResourceList.ModelServer+ObjectPath;
		}
		VisualObject.setAttribute("data-ModelSource", ObjectPath);
		
		HMI.addClass(VisualObject, this.cshmiGroupClass);
		
		//setting the basic Element Variables like .visible .stroke .fill .opacity .rotate
		this._processBasicVariables(VisualObject, requestList[ObjectPath]);
		
		VisualObject.setAttribute("overflow", "visible");
		
		if (VisualParentObject === null){
			//if this is the first svg, we need a default container for ConfigValues for saving them
			VisualObject.FBReference = Object();
			VisualObject.FBReference["default"] = "";
			VisualObject.ConfigValues = Object();
		}
		
		this._armToggleChildVisibility(VisualParentObject, VisualObject, ObjectPath, requestList);
		
		return VisualObject;
	},
	
	/**
	 * builds SVG line object, gets the parameter via KS
	 * @param {SVGElement} VisualParentObject visual Object which is parent to active Object
	 * @param {String} ObjectPath Path to this cshmi object containing the event/action/visualisation
	 * @param {bool} preventNetworkRequest the function should prevent network requests if possible
	 * @return {SVGElement} VisualObject the new constructed element or null
	 */
	_buildSvgLine: function(VisualParentObject, ObjectPath, preventNetworkRequest){
		var requestList = new Object();
		//if the Object was scanned earlier, get the cached information (could be the case with templates or repeated/cyclic calls to the same object)
		if (this.ResourceList.Elements && this.ResourceList.Elements[ObjectPath] !== undefined){
			//the object is asked this session, so reuse the config to save communication requests
			requestList[ObjectPath] = this.ResourceList.Elements[ObjectPath].Parameters;
		}else if(preventNetworkRequest === true){
			//build a skeleton to preserve zindex/sequence
			var VisualObject = HMI.svgDocument.createElementNS(HMI.HMI_Constants.NAMESPACE_SVG, 'line');
			VisualObject.id = ObjectPath;
			return VisualObject;
		}else{
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
			if (successCode === false){
				return null;
			}
			
			//we have asked the object successful, so remember the result
			this.ResourceList.Elements[ObjectPath] = new Object();
			this.ResourceList.Elements[ObjectPath].Parameters = requestList[ObjectPath];
		}
		
		//search a predefined children
		var VisualObject = VisualParentObject.getElementById(ObjectPath);
		
		if (VisualObject === null){
			VisualObject = HMI.svgDocument.createElementNS(HMI.HMI_Constants.NAMESPACE_SVG, 'line');
		}
		
		//id should be the name of the parent plus our identifier
		var NameList = ObjectPath.split("/");
		VisualObject.id = VisualParentObject.id + "/" + NameList[NameList.length-1];
		VisualObject.setAttribute("data-ModelSource", ObjectPath);
		NameList = null;
		
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
	 * @param {bool} preventNetworkRequest the function should prevent network requests if possible
	 * @return {SVGElement} VisualObject the new constructed element or null
	 */
	_buildSvgPolyline: function(VisualParentObject, ObjectPath, preventNetworkRequest){
		var requestList = new Object();
		//if the Object was scanned earlier, get the cached information (could be the case with templates or repeated/cyclic calls to the same object)
		if (this.ResourceList.Elements && this.ResourceList.Elements[ObjectPath] !== undefined){
			//the object is asked this session, so reuse the config to save communication requests
			requestList[ObjectPath] = this.ResourceList.Elements[ObjectPath].Parameters;
		}else if(preventNetworkRequest === true){
			//build a skeleton to preserve zindex/sequence
			var VisualObject = HMI.svgDocument.createElementNS(HMI.HMI_Constants.NAMESPACE_SVG, 'polyline');
			VisualObject.id = ObjectPath;
			return VisualObject;
		}else{
			requestList[ObjectPath] = new Object();
			requestList[ObjectPath]["visible"] = null;
			requestList[ObjectPath]["stroke"] = null;
			requestList[ObjectPath]["fill"] = null;
			requestList[ObjectPath]["opacity"] = null;
			requestList[ObjectPath]["rotate"] = null;
			requestList[ObjectPath]["points"] = null;
			requestList[ObjectPath]["strokeWidth"] = null;
			
			var successCode = this._requestVariablesArray(requestList);
			if (successCode === false){
				return null;
			}
			
			//we have asked the object successful, so remember the result
			this.ResourceList.Elements[ObjectPath] = new Object();
			this.ResourceList.Elements[ObjectPath].Parameters = requestList[ObjectPath];
		}
		
		//search a predefined children
		var VisualObject = VisualParentObject.getElementById(ObjectPath);
		
		if (VisualObject === null){
			VisualObject = HMI.svgDocument.createElementNS(HMI.HMI_Constants.NAMESPACE_SVG, 'polyline');
		}
		
		//id should be the name of the parent plus our identifier
		var NameList = ObjectPath.split("/");
		VisualObject.id = VisualParentObject.id + "/" + NameList[NameList.length-1];
		VisualObject.setAttribute("data-ModelSource", ObjectPath);
		NameList = null;
		
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
	 * @param {bool} preventNetworkRequest the function should prevent network requests if possible
	 * @return {SVGElement} VisualObject the new constructed element or null
	 */
	_buildSvgPolygon: function(VisualParentObject, ObjectPath, preventNetworkRequest){
		var requestList = new Object();
		//if the Object was scanned earlier, get the cached information (could be the case with templates or repeated/cyclic calls to the same object)
		if (this.ResourceList.Elements && this.ResourceList.Elements[ObjectPath] !== undefined){
			//the object is asked this session, so reuse the config to save communication requests
			requestList[ObjectPath] = this.ResourceList.Elements[ObjectPath].Parameters;
		}else if(preventNetworkRequest === true){
			//build a skeleton to preserve zindex/sequence
			var VisualObject = HMI.svgDocument.createElementNS(HMI.HMI_Constants.NAMESPACE_SVG, 'polygon');
			VisualObject.id = ObjectPath;
			return VisualObject;
		}else{
			requestList[ObjectPath] = new Object();
			requestList[ObjectPath]["visible"] = null;
			requestList[ObjectPath]["stroke"] = null;
			requestList[ObjectPath]["fill"] = null;
			requestList[ObjectPath]["opacity"] = null;
			requestList[ObjectPath]["rotate"] = null;
			requestList[ObjectPath]["points"] = null;
			requestList[ObjectPath]["strokeWidth"] = null;
			
			var successCode = this._requestVariablesArray(requestList);
			if (successCode === false){
				return null;
			}
			
			//we have asked the object successful, so remember the result
			this.ResourceList.Elements[ObjectPath] = new Object();
			this.ResourceList.Elements[ObjectPath].Parameters = requestList[ObjectPath];
		}
		
		//search a predefined children
		var VisualObject = VisualParentObject.getElementById(ObjectPath);
		
		if (VisualObject === null){
			VisualObject = HMI.svgDocument.createElementNS(HMI.HMI_Constants.NAMESPACE_SVG, 'polygon');
		}
		
		//id should be the name of the parent plus our identifier
		var NameList = ObjectPath.split("/");
		VisualObject.id = VisualParentObject.id + "/" + NameList[NameList.length-1];
		VisualObject.setAttribute("data-ModelSource", ObjectPath);
		NameList = null;
		
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
	 * @param {bool} preventNetworkRequest the function should prevent network requests if possible
	 * @return {SVGElement} VisualObject the new constructed element or null
	 */
	_buildSvgPath: function(VisualParentObject, ObjectPath, preventNetworkRequest){
		var requestList = new Object();
		//if the Object was scanned earlier, get the cached information (could be the case with templates or repeated/cyclic calls to the same object)
		if (this.ResourceList.Elements && this.ResourceList.Elements[ObjectPath] !== undefined){
			//the object is asked this session, so reuse the config to save communication requests
			requestList[ObjectPath] = this.ResourceList.Elements[ObjectPath].Parameters;
		}else if(preventNetworkRequest === true){
			//build a skeleton to preserve zindex/sequence
			var VisualObject = HMI.svgDocument.createElementNS(HMI.HMI_Constants.NAMESPACE_SVG, 'path');
			VisualObject.id = ObjectPath;
			return VisualObject;
		}else{
			requestList[ObjectPath] = new Object();
			requestList[ObjectPath]["visible"] = null;
			requestList[ObjectPath]["stroke"] = null;
			requestList[ObjectPath]["fill"] = null;
			requestList[ObjectPath]["opacity"] = null;
			requestList[ObjectPath]["rotate"] = null;
			requestList[ObjectPath]["d"] = null;
			requestList[ObjectPath]["strokeWidth"] = null;
			
			var successCode = this._requestVariablesArray(requestList);
			if (successCode === false){
				return null;
			}
			
			//we have asked the object successful, so remember the result
			this.ResourceList.Elements[ObjectPath] = new Object();
			this.ResourceList.Elements[ObjectPath].Parameters = requestList[ObjectPath];
		}
		
		//search a predefined children
		var VisualObject = VisualParentObject.getElementById(ObjectPath);
		
		if (VisualObject === null){
			VisualObject = HMI.svgDocument.createElementNS(HMI.HMI_Constants.NAMESPACE_SVG, 'path');
		}
		
		//id should be the name of the parent plus our identifier
		var NameList = ObjectPath.split("/");
		VisualObject.id = VisualParentObject.id + "/" + NameList[NameList.length-1];
		VisualObject.setAttribute("data-ModelSource", ObjectPath);
		NameList = null;
		
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
	 * @param {bool} preventNetworkRequest the function should prevent network requests if possible
	 * @return {SVGElement} VisualObject the new constructed element or null
	 */
	_buildSvgText: function(VisualParentObject, ObjectPath, preventNetworkRequest){
		var requestList = new Object();
		//if the Object was scanned earlier, get the cached information (could be the case with templates or repeated/cyclic calls to the same object)
		if (this.ResourceList.Elements && this.ResourceList.Elements[ObjectPath] !== undefined){
			//the object is asked this session, so reuse the config to save communication requests
			requestList[ObjectPath] = this.ResourceList.Elements[ObjectPath].Parameters;
		}else if(preventNetworkRequest === true){
			//build a skeleton to preserve zindex/sequence
			var VisualObject = HMI.svgDocument.createElementNS(HMI.HMI_Constants.NAMESPACE_SVG, 'text');
			VisualObject.id = ObjectPath;
			return VisualObject;
		}else{
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
			if (successCode === false){
				return null;
			}
			
			//we have asked the object successful, so remember the result
			this.ResourceList.Elements[ObjectPath] = new Object();
			this.ResourceList.Elements[ObjectPath].Parameters = requestList[ObjectPath];
		}
		
		//search a predefined children
		var VisualObject = VisualParentObject.getElementById(ObjectPath);
		
		if (VisualObject === null){
			VisualObject = HMI.svgDocument.createElementNS(HMI.HMI_Constants.NAMESPACE_SVG, 'text');
		}
		
		//id should be the name of the parent plus our identifier
		var NameList = ObjectPath.split("/");
		VisualObject.id = VisualParentObject.id + "/" + NameList[NameList.length-1];
		VisualObject.setAttribute("data-ModelSource", ObjectPath);
		NameList = null;
		
		//setting the basic Element Variables like .visible .stroke .fill .opacity .rotate
		this._processBasicVariables(VisualObject, requestList[ObjectPath]);
		VisualObject.setAttribute("font-size", requestList[ObjectPath]["fontSize"]);
		VisualObject.setAttribute("font-style", requestList[ObjectPath]["fontStyle"]);
		VisualObject.setAttribute("font-weight", requestList[ObjectPath]["fontWeight"]);
		VisualObject.setAttribute("font-family", requestList[ObjectPath]["fontFamily"]);
		VisualObject.setAttribute("text-anchor", requestList[ObjectPath]["horAlignment"]);
		
		this._setSvgText(VisualObject, requestList[ObjectPath]["content"], parseInt(requestList[ObjectPath]["trimToLength"], 10));
		
		//dominant-baseline:hanging not supported by Opera (v12) and IE9
		//baseline-shift:-100% only defined on a tspan, not on text (respected by Opera), not supported by IE9
		if (requestList[ObjectPath]["verAlignment"] == "auto"){
			//no change needed
		}else if (requestList[ObjectPath]["verAlignment"] == "middle"){
			VisualObject.setAttribute("dy", "0.7ex");
		}else if (requestList[ObjectPath]["verAlignment"] == "hanging"){
			VisualObject.setAttribute("dy", "1ex");
		}
		
		return VisualObject;
	},
	
	/**
	 * builds SVG circle object, gets the parameter via KS
	 * @param {SVGElement} VisualParentObject visual Object which is parent to active Object
	 * @param {String} ObjectPath Path to this cshmi object containing the event/action/visualisation
	 * @param {bool} preventNetworkRequest the function should prevent network requests if possible
	 * @return {SVGElement} VisualObject the new constructed element or null
	 */
	_buildSvgCircle: function(VisualParentObject, ObjectPath, preventNetworkRequest){
		var requestList = new Object();
		//if the Object was scanned earlier, get the cached information (could be the case with templates or repeated/cyclic calls to the same object)
		if (this.ResourceList.Elements && this.ResourceList.Elements[ObjectPath] !== undefined){
			//the object is asked this session, so reuse the config to save communication requests
			requestList[ObjectPath] = this.ResourceList.Elements[ObjectPath].Parameters;
		}else if(preventNetworkRequest === true){
			//build a skeleton to preserve zindex/sequence
			var VisualObject = HMI.svgDocument.createElementNS(HMI.HMI_Constants.NAMESPACE_SVG, 'circle');
			VisualObject.id = ObjectPath;
			return VisualObject;
		}else{
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
			if (successCode === false){
				return null;
			}
			
			//we have asked the object successful, so remember the result
			this.ResourceList.Elements[ObjectPath] = new Object();
			this.ResourceList.Elements[ObjectPath].Parameters = requestList[ObjectPath];
		}
		
		//search a predefined children
		var VisualObject = VisualParentObject.getElementById(ObjectPath);
		
		if (VisualObject === null){
			VisualObject = HMI.svgDocument.createElementNS(HMI.HMI_Constants.NAMESPACE_SVG, 'circle');
		}
		
		//id should be the name of the parent plus our identifier
		var NameList = ObjectPath.split("/");
		VisualObject.id = VisualParentObject.id + "/" + NameList[NameList.length-1];
		VisualObject.setAttribute("data-ModelSource", ObjectPath);
		NameList = null;
		
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
	 * @param {bool} preventNetworkRequest the function should prevent network requests if possible
	 * @return {SVGElement} VisualObject the new constructed element or null
	 */
	_buildSvgEllipse: function(VisualParentObject, ObjectPath, preventNetworkRequest){
		var requestList = new Object();
		//if the Object was scanned earlier, get the cached information (could be the case with templates or repeated/cyclic calls to the same object)
		if (this.ResourceList.Elements && this.ResourceList.Elements[ObjectPath] !== undefined){
			//the object is asked this session, so reuse the config to save communication requests
			requestList[ObjectPath] = this.ResourceList.Elements[ObjectPath].Parameters;
		}else if(preventNetworkRequest === true){
			//build a skeleton to preserve zindex/sequence
			var VisualObject = HMI.svgDocument.createElementNS(HMI.HMI_Constants.NAMESPACE_SVG, 'ellipse');
			VisualObject.id = ObjectPath;
			return VisualObject;
		}else{
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
			if (successCode === false){
				return null;
			}
			
			//we have asked the object successful, so remember the result
			this.ResourceList.Elements[ObjectPath] = new Object();
			this.ResourceList.Elements[ObjectPath].Parameters = requestList[ObjectPath];
		}
		
		//search a predefined children
		var VisualObject = VisualParentObject.getElementById(ObjectPath);
		
		if (VisualObject === null){
			VisualObject = HMI.svgDocument.createElementNS(HMI.HMI_Constants.NAMESPACE_SVG, 'ellipse');
		}
		
		//id should be the name of the parent plus our identifier
		var NameList = ObjectPath.split("/");
		VisualObject.id = VisualParentObject.id + "/" + NameList[NameList.length-1];
		VisualObject.setAttribute("data-ModelSource", ObjectPath);
		NameList = null;
		
		//setting the basic Element Variables like .visible .stroke .fill .opacity .rotate
		this._processBasicVariables(VisualObject, requestList[ObjectPath]);
		VisualObject.setAttribute("cx", requestList[ObjectPath]["cx"]);
		VisualObject.setAttribute("cy", requestList[ObjectPath]["cy"]);
		VisualObject.setAttribute("rx",requestList[ObjectPath]["rx"]);
		VisualObject.setAttribute("ry",requestList[ObjectPath]["ry"]);
		
		//rotation should be around cx and cy
		if (requestList[ObjectPath]["rotate"] && requestList[ObjectPath]["rotate"] !== "0"){
			VisualObject.setAttribute("transform", "rotate("+requestList[ObjectPath]["rotate"]+","+requestList[ObjectPath]["cx"]+","+requestList[ObjectPath]["cy"]+")");
		}
		
		return VisualObject;
	},
	
	/**
	 * builds SVG rect object, gets the parameter via KS
	 * @param {SVGElement} VisualParentObject visual Object which is parent to active Object
	 * @param {String} ObjectPath Path to this cshmi object containing the event/action/visualisation
	 * @param {bool} preventNetworkRequest the function should prevent network requests if possible
	 * @return {SVGElement} VisualObject the new constructed element or null
	 */
	_buildSvgRect: function(VisualParentObject, ObjectPath, preventNetworkRequest){
		var requestList = new Object();
		//if the Object was scanned earlier, get the cached information (could be the case with templates or repeated/cyclic calls to the same object)
		if (this.ResourceList.Elements && this.ResourceList.Elements[ObjectPath] !== undefined){
			//the object is asked this session, so reuse the config to save communication requests
			requestList[ObjectPath] = this.ResourceList.Elements[ObjectPath].Parameters;
		}else if(preventNetworkRequest === true){
			//build a skeleton to preserve zindex/sequence
			var VisualObject = HMI.svgDocument.createElementNS(HMI.HMI_Constants.NAMESPACE_SVG, 'rect');
			VisualObject.id = ObjectPath;
			return VisualObject;
		}else{
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
			if (successCode === false){
				return null;
			}
			
			//we have asked the object successful, so remember the result
			this.ResourceList.Elements[ObjectPath] = new Object();
			this.ResourceList.Elements[ObjectPath].Parameters = requestList[ObjectPath];
		}
		
		//search a predefined children
		var VisualObject = VisualParentObject.getElementById(ObjectPath);
		
		if (VisualObject === null){
			VisualObject = HMI.svgDocument.createElementNS(HMI.HMI_Constants.NAMESPACE_SVG, 'rect');
		}
		
		//id should be the name of the parent plus our identifier
		var NameList = ObjectPath.split("/");
		VisualObject.id = VisualParentObject.id + "/" + NameList[NameList.length-1];
		VisualObject.setAttribute("data-ModelSource", ObjectPath);
		NameList = null;
		
		//setting the basic Element Variables like .visible .stroke .fill .opacity .rotate
		this._processBasicVariables(VisualObject, requestList[ObjectPath]);
		
		VisualObject.setAttribute("shape-rendering", "crispEdges");
		
		return VisualObject;
	},
	
	/**
	 * builds SVG image object, gets the parameter via KS
	 * @param {SVGElement} VisualParentObject visual Object which is parent to active Object
	 * @param {String} ObjectPath Path to this cshmi object containing the event/action/visualisation
	 * @param {bool} preventNetworkRequest the function should prevent network requests if possible
	 * @return {SVGElement} VisualObject the new constructed element or null
	 */
	_buildSvgImage: function(VisualParentObject, ObjectPath, preventNetworkRequest){
		var requestList = new Object();
		//if the Object was scanned earlier, get the cached information (could be the case with templates or repeated/cyclic calls to the same object)
		if (this.ResourceList.Elements && this.ResourceList.Elements[ObjectPath] !== undefined){
			//the object is asked this session, so reuse the config to save communication requests
			requestList[ObjectPath] = this.ResourceList.Elements[ObjectPath].Parameters;
		}else if(preventNetworkRequest === true){
			//build a skeleton to preserve zindex/sequence
			var VisualObject = HMI.svgDocument.createElementNS(HMI.HMI_Constants.NAMESPACE_SVG, 'image');
			VisualObject.id = ObjectPath;
			return VisualObject;
		}else{
			requestList[ObjectPath] = new Object();
			requestList[ObjectPath]["visible"] = null;
			requestList[ObjectPath]["opacity"] = null;
			requestList[ObjectPath]["rotate"] = null;
			requestList[ObjectPath]["x"] = null;
			requestList[ObjectPath]["y"] = null;
			requestList[ObjectPath]["width"] = null;
			requestList[ObjectPath]["height"] = null;
			requestList[ObjectPath]["SVGcontent"] = null;
			requestList[ObjectPath]["Bitmapcontent"] = null;
			
			var successCode = this._requestVariablesArray(requestList);
			if (successCode === false){
				return null;
			}
			
			//we have asked the object successful, so remember the result
			this.ResourceList.Elements[ObjectPath] = new Object();
			this.ResourceList.Elements[ObjectPath].Parameters = requestList[ObjectPath];
		}
		
		//search a predefined children
		var PredefinedVisualObject = VisualParentObject.getElementById(ObjectPath);
		
		
		var VisualObject;
		if(requestList[ObjectPath]["SVGcontent"] !== ""){
			//we have SVG Content to visualise
			//
			//the code could use an xlink, svg or no XML prefix at all. Examples
			//"<rect stroke='red' x='0' y='0' width='10' height='20'></rect>"
			//"<svg:rect stroke='red' x='0' y='0' width='10' height='20'></svg:rect>"
			
			var svgContent =	"<svg:svg xmlns:svg='"+HMI.HMI_Constants.NAMESPACE_SVG+"' xmlns='"+HMI.HMI_Constants.NAMESPACE_SVG+"' "
				+"xmlns:xlink='"+HMI.HMI_Constants.NAMESPACE_XLINK+"'>"
				+requestList[ObjectPath]["SVGcontent"]
				+"</svg:svg>";
			VisualObject = HMI.HMIDOMParser.parse(svgContent);
			VisualObject.setAttribute("overflow", "visible");
		}else if(requestList[ObjectPath]["Bitmapcontent"] !== ""){
			//we have an Bitmap Content to visualise
			//
			
			if (requestList[ObjectPath]["Bitmapcontent"].indexOf("data:") !== 0){
				HMI.hmi_log_info_onwebsite("Bitmapcontent of Image "+ObjectPath+" must begin with the string 'data:'");
				return null;
			}
			var VisualObject = HMI.svgDocument.createElementNS(HMI.HMI_Constants.NAMESPACE_SVG, 'image');
			
			VisualObject.setAttributeNS(HMI.HMI_Constants.NAMESPACE_XLINK, "href", requestList[ObjectPath]["Bitmapcontent"]);
			VisualObject.setAttribute("width", requestList[ObjectPath]["width"]);
			VisualObject.setAttribute("height", requestList[ObjectPath]["height"]);
		}else{
			HMI.hmi_log_info_onwebsite("Image "+ObjectPath+" is not configured");
			return null;
		}
		
		//id should be the name of the parent plus our identifier
		var NameList = ObjectPath.split("/");
		VisualObject.id = VisualParentObject.id + "/" + NameList[NameList.length-1];
		VisualObject.setAttribute("data-ModelSource", ObjectPath);
		NameList = null;
		
		//setting the basic Element Variables like .visible .stroke .fill .opacity .rotate
		this._processBasicVariables(VisualObject, requestList[ObjectPath]);
		
		if (PredefinedVisualObject !== null && PredefinedVisualObject.parentNode !== null){
			PredefinedVisualObject.parentNode.replaceChild(VisualObject, PredefinedVisualObject);
		}
		
		return VisualObject;
	},
	
	/**
	 * returns the FBReference
	 * @param {SVGElement} VisualObject Object to manipulate the visualisation
	 * @param {BOOL} giveObject should we give the targetObject?
	 * @return {String} Path of the FBReference or ""
	 */
	_getFBReference: function(VisualObject, giveObject){
		var TemplateObject = VisualObject;
		var resultArray = ["", null];
		var FBRef;
		
		//a getVar currentChild has OP_VALUE and OP_NAME set
		if (this.ResourceList.ChildrenIterator.currentChild !== undefined && this.ResourceList.ChildrenIterator.currentChild["OP_VALUE"] !== undefined ){
			//we are in an GetVar-iterator and want to read out a value from the currentchild
			//search FBReference of root Object
			do{
				//FBReference found
				if(TemplateObject.FBReference && TemplateObject.FBReference["default"] !== undefined){
					FBRef = TemplateObject.FBReference["default"];
					break;
				}
			//loop upwards to find the Template object
			}while( (TemplateObject = TemplateObject.parentNode) && TemplateObject !== null && TemplateObject.namespaceURI == HMI.HMI_Constants.NAMESPACE_SVG);  //the = is no typo here!
			
			// check if FBref beginn with "//" because we need the server Info as prefix for our OP_VALUE
			// e.g "//dev/ov_hmidemo7/TechUnits/TU10/h_bkqwmtbbhpf"" --> use prefix "//dev/ov_hmidemo7"
			var ServerName = "";
			if (FBRef !== "" && FBRef.charAt(0) === "/" && FBRef.charAt(1) === "/"){
				//find the 3rd "/"
				var slashIndex = FBRef.indexOf("/", 2);
				//find the 4th "/"
				slashIndex = FBRef.indexOf("/", slashIndex+1);
				//only keep the String before 4th "/"
				ServerName = FBRef.slice(0, slashIndex);
			}
			if (this.ResourceList.ChildrenIterator.currentChild["OP_VALUE"].charAt(0) === "/"){
				// the iterated string begins with / so it is a fullpath (likely from a GetVar on an assoziation)
				
				// it could be a path to another server, so we must add the serverName
				resultArray[0] = ServerName+this.ResourceList.ChildrenIterator.currentChild["OP_VALUE"];
			}
			
		//a getEP currentChild has OP_NAME set
		}else if (this.ResourceList.ChildrenIterator.currentChild !== undefined && this.ResourceList.ChildrenIterator.currentChild["OP_NAME"] !== undefined ){
			//we are in an getEP-iterator and want to read out a value from the currentchild
			//search FBReference of root Object
			do{
				//FBReference found
				if(TemplateObject.FBReference && TemplateObject.FBReference["default"] !== undefined){
					FBRef = TemplateObject.FBReference["default"];
					if (this.ResourceList.ChildrenIterator.currentChild["OP_ACCESS"] !== undefined && this.ResourceList.ChildrenIterator.currentChild["OP_ACCESS"].indexOf("KS_AC_PART") !== -1){
						//we have an OV-PART, so the separator is a dot
						resultArray[0] = FBRef+"."+this.ResourceList.ChildrenIterator.currentChild["OP_NAME"];
					}else{
						//we have no OV-PART, so the separator is a slash
						resultArray[0] = FBRef+"/"+this.ResourceList.ChildrenIterator.currentChild["OP_NAME"];
					}
					break;
				}
			//loop upwards to find the Template object
			}while( (TemplateObject = TemplateObject.parentNode) && TemplateObject !== null && TemplateObject.namespaceURI == HMI.HMI_Constants.NAMESPACE_SVG);  //the = is no typo here!
		}else{
			//no active iterator, so plain FBReference
			do{
				if(TemplateObject.FBReference && TemplateObject.FBReference["default"] !== undefined){
					//the name of a Template was requested
					resultArray = [TemplateObject.FBReference["default"], TemplateObject];
					break;
				}
			//loop upwards to find the Template object
			}while( (TemplateObject = TemplateObject.parentNode) && TemplateObject !== null && TemplateObject.namespaceURI == HMI.HMI_Constants.NAMESPACE_SVG);  //the = is no typo here!
		}
		if(giveObject === true){
			return resultArray;
		}else{
			return resultArray[0];
		}
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
			VisualObject.setAttribute("x", configArray["x"]);
			VisualObject.setAttribute("y", configArray["y"]);
			if (configArray["rotate"] && configArray["rotate"] !== "0"){
				VisualObject.setAttribute("transform", "rotate("+configArray["rotate"]+","+configArray["x"]+","+configArray["y"]+")");
			}
		}else if (configArray["rotate"] && configArray["rotate"] !== "0"){
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
	 * @return {Boolean} true on success, false if an error occured
	 */
	_requestVariablesArray: function(requestList){
		var requestArray = new Array();
		var lastOvObjName = null;
		var VariableCount = 0;
		
		//collect all requested Variables
		for (var ovObjName in requestList) {
			for (var ksVarName in requestList[ovObjName]) {
				if (lastOvObjName != ovObjName){
					//variable from a new object requested
					requestArray.push(ovObjName+"."+ksVarName);
				}else{
					//variable from the same object requested
					requestArray.push("."+ksVarName);
				}
				lastOvObjName = ovObjName;
				VariableCount++;
			}
		}
		
		var response = HMI.KSClient.getVar(requestArray, "OP_VALUE", null);
		if (response === false || response === null){
			//communication error
			return false;
		}else if (response.indexOf("KS_ERR_BADPATH") !== -1 && (response.match(/KS_ERR_BADPATH/g)||[]).length - 1 === VariableCount){
			//TCL gives one global KS_ERR and one for each error
			//"TksS-0335::KS_ERR_BADPATH {{/TechUnits/VesselCurrent.width KS_ERR_BADPATH} {.height KS_ERR_BADPATH} {.hideable KS_ERR_BADPATH}}"
			
			//all our variables gave an error, so it is an server problem and no model problem, will be reported in caller function
			return false;
		}else if (response.indexOf("KS_ERR_BADPATH") !== -1){
			HMI.hmi_log_onwebsite("Sorry, your cshmi server is not supported, because the base model was changed. Please upgrade to the newest cshmi library. Don't forget to export your server.");
			HMI.hmi_log_error("cshmi._requestVariablesArray of "+requestArray+" failed: "+response);
			return false;
		}else if (response.indexOf("KS_ERR") !== -1){
			HMI.hmi_log_error("cshmi._requestVariablesArray of "+requestArray+" failed: "+response);
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
	 * loads all children and appends them to the Parent
	 * @param {SVGElement} VisualParentObject visual Object which is parent to active Object
	 * @param {String} ObjectPath Path to this cshmi object containing the event/action/visualisation
	 * @param {bool} preventNetworkRequest the function should prevent network requests if possible
	 */
	_loadChildren: function(VisualParentObject, ObjectPath, preventNetworkRequest){
		var responseArray = HMI.KSClient.getChildObjArray(ObjectPath, this);
		
		//newwrite
		//fetch config from all childrens via this.ResourceList.ModellVariables.*
		
		for (var i=0; i < responseArray.length; i++) {
			var varName = responseArray[i].split(" ");
			if(VisualParentObject.tagName === "g" && VisualParentObject.id === "" && VisualParentObject.firstChild && VisualParentObject.firstChild.id !== ""){
				//nested rotation template
				var realComponent = VisualParentObject.firstChild;
			}else{
				realComponent = VisualParentObject;
			}
			var ChildComponent = this.BuildDomain(realComponent, ObjectPath+"/"+varName[0], varName[1], preventNetworkRequest);
			//children could be already in DOM
			if (ChildComponent !== null && ChildComponent.parentNode === null){
				realComponent.appendChild(ChildComponent);
			}
		}
	},
	
	/**
	 * tests if we have to load children, dims the sheet and loads all graphical elements
	 * @param {SVGElement} VisualParentObject Object to manipulate the visualisation
	 */
	_loadHiddenChildrenElements: function(VisualParentObject){
		if (HMI.instanceOf(VisualParentObject, this.cshmiObjectVisibleChildrenNotLoaded)){
			//dim the full sheet to show the progress
			HMI.Playground.firstChild.setAttribute("opacity", "0.6");
			
			//test if we have an template
			if (HMI.instanceOf(VisualParentObject, this.cshmiTemplateClass)){
				//load the elements of the template, allow all network request
				this._loadChildren(VisualParentObject, VisualParentObject.getAttribute("data-TemplateModelSource"), false);
			}
			//load all graphical children of the object, allow all network request
			this._loadChildren(VisualParentObject, VisualParentObject.getAttribute("data-ModelSource"), false);
			
			//we want to have offset parameter on all new visual elements
			var ComponentChilds = VisualParentObject.getElementsByTagNameNS(HMI.HMI_Constants.NAMESPACE_SVG, '*');
			for(var i = 0;i < ComponentChilds.length;i++){
				HMI.saveAbsolutePosition(ComponentChilds[i]);
			}
			
			//interprete onload Actions if we are already loaded
			if (this.initStage === false){
				this._interpreteOnloadCallStack();
			}
			
			// mark the class as complete
			// the event registering requires the removing after loadChildren!
			HMI.removeClass(VisualParentObject, this.cshmiObjectVisibleChildrenNotLoaded);
			
			//deactivate the dim of the sheet, as we are ready
			HMI.Playground.firstChild.removeAttribute("opacity");
		}
	},
	
	/**
	 * interprete onloadCallStack
	 */
	_interpreteOnloadCallStack: function(){
		//onload code should not know, if we are in an iterator
		var savedCurrentChild = this.ResourceList.ChildrenIterator.currentChild;
		delete this.ResourceList.ChildrenIterator.currentChild;
		
		//for this objects, the init stage should be set (needed for getValue and timeevent)
		var oldStage = this.initStage;
		this.initStage= true;
		
		while(this.ResourceList.onloadCallStack.length !== 0){
			var EventObjItem = this.ResourceList.onloadCallStack.shift();
			if(EventObjItem["VisualObject"].ownerDocument === this.trashDocument){
				//the object was killed, so ignore
				continue;
			}
			this._interpreteAction(EventObjItem["VisualObject"], EventObjItem["ObjectPath"]);
		}
		EventObjItem = null;
		
		//restore currentChild status
		this.ResourceList.ChildrenIterator.currentChild = savedCurrentChild;
		savedCurrentChild = null;
		
		this.initStage = oldStage;
		
		return;
	},
	
	/**
	 * tests if the parent should get a click event and register for this
	 */
	_armToggleChildVisibility: function(VisualParentObject, VisualObject, ObjectPath, requestList){
		if(VisualParentObject === null){
			return;
		}
		if (requestList[ObjectPath]["hideable"] === "TRUE"){
			HMI.addClass(VisualObject, this.cshmiTemplateHideableClass);
			VisualParentObject.setAttribute("cursor", "pointer");
		}
		//make the parent clickable, if we can be hidden and no sibling has done this before
		if (requestList[ObjectPath]["hideable"] === "TRUE"
			&& HMI.instanceOf(VisualParentObject, this.cshmiObjectHasHideableChildren) === false){
			//toggle visibility of hideable childtemplates onclick
			VisualParentObject.addEventListener("click", function(evt){
				if(HMI.instanceOf(VisualParentObject, HMI.cshmi.cshmiOperatorClickClass)){
					//we have an clickgesture on the same VisualObject, so this will handle all action
					return;
				}
				
				HMI.cshmi.toggleChildTemplates(VisualParentObject);
				
				//quit propagation of event in any case. We do not want the parent template to handle the click
				if (evt.stopPropagation) evt.stopPropagation();
			}, false);
			
			//prevent multiple events on this
			HMI.addClass(VisualParentObject, this.cshmiObjectHasHideableChildren);
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
			var VisualObject = childTemplates[i];
			if(VisualObject.getAttribute === undefined){
				//this children can be for example a text node
				continue;
			}
			var Classes = VisualObject.getAttribute("class");
			if (Classes === null){
				continue;
			}else if (Classes.indexOf(this.cshmiTemplateHideableClass) === -1){
				continue;
			}else if (VisualObject.getAttribute("display") == "block"){
				VisualObject.setAttribute("display", "none");
			}else{
				VisualObject.setAttribute("display", "block");
				
				//load hidden elements now
				this._loadHiddenChildrenElements(VisualObject);
				
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
		VisualObject = null;
	},
	
	/**
	 * set Text, trim if needed and remember the length
	 * @param {SVGElement} VisualObject visual Object which is active Text Object
	 * @param {String} newText Text to set, null if unchanged.
	 * @param {trimLength} maximal length to set
	 * @return void
	 */
	_setSvgText: function(VisualObject, NewText, trimToLength){
		if (VisualObject.tagName !== "text"){
			return;
		}
		if (NewText === null){
			//no text change, but only trimToLength
			for (var i = 0;i < VisualObject.childNodes.length;i++){
				//search untrimmed text in the title
				if (VisualObject.childNodes.item(i).tagName === "title"){
					NewText = VisualObject.childNodes.item(i).firstChild.textContent;
					break;
				}
			}
			
			if (NewText === null && typeof VisualObject.textContent != "undefined"){
				NewText = VisualObject.textContent;
			}else if (NewText === null && typeof VisualObject.innerText != "undefined"){
				NewText = VisualObject.innerText;
			}
		}
		
		trimToLength = parseInt(trimToLength, 10);
		var contentLength = parseInt(NewText.length, 10);
		var trimmedContent;
		var trimFromRight = null;
		deleteChilds(VisualObject);
		if(trimToLength > 0 && isNumeric(NewText)){
			//we have a numeric NewText
			if(NewText.indexOf(".") === -1){
				//INT or UINT
				trimmedContent = NewText;
				this._setTitle(VisualObject, "");
			}else{
				//other values
				var power = Math.pow(10, trimToLength || 0);
				trimmedContent = String(Math.round(NewText * power) / power);
				
				this._setTitle(VisualObject, NewText);
			}
		}else if((trimToLength > 0) && (contentLength > trimToLength)){
			trimmedContent = NewText.substr(0, trimToLength);
			trimFromRight = true;
			this._setTitle(VisualObject, NewText);
		}else if((trimToLength < 0) && (contentLength > -trimToLength)){
			trimmedContent = NewText.substr(contentLength + trimToLength);
			trimFromRight = false;
			this._setTitle(VisualObject, NewText);
		}else{
			trimmedContent = NewText;
			this._setTitle(VisualObject, "");
		}
		
		trimmedContent = HMI.svgDocument.createTextNode(trimmedContent);
		VisualObject.appendChild(trimmedContent);
		
		var dots = HMI.svgDocument.createElementNS(HMI.HMI_Constants.NAMESPACE_SVG, 'tspan');
		dots.appendChild(HMI.svgDocument.createTextNode(String.fromCharCode(8230)));
		dots.setAttribute("font-size", "70%");
		if(trimFromRight === true){
			VisualObject.appendChild(dots);
		}else if(trimFromRight === false){
			VisualObject.insertBefore(dots, VisualObject.firstChild);
		}
		
		//remember config
		VisualObject.setAttribute("trimToLength", trimToLength);
	},
	
	/**
	 * sets an SVG title on an object
	 * @param {SVGElement} VisualObject Object to manipulate the visualisation
	 * @param {String} newText Text for the title
	 * @return {Boolean} true
	 */
	_setTitle: function(VisualObject, newText){
		var titles = VisualObject.getElementsByTagNameNS(HMI.HMI_Constants.NAMESPACE_SVG, 'title');
		if(newText === ""){
			if (titles.length !== 0){
				titles[0].parentNode.removeChild(titles[0]);
			}
		}else{
			if (titles.length >0){
				titles[0].replaceChild(HMI.svgDocument.createTextNode(newText), titles[0].firstChild);
			}else{
				var svgTitle = HMI.svgDocument.createElementNS(HMI.HMI_Constants.NAMESPACE_SVG, 'title');
				svgTitle.appendChild(HMI.svgDocument.createTextNode(newText));
				VisualObject.appendChild(svgTitle);
			}
		}
		return true;
	}
};

/**
 * Constructor for a generic observer for multiple tasks in cshmi
 * @param {SVGElement} VisualObject Object to manipulate the visualisation
 * @param {String} ObjectPath Path to this cshmi object containing the event/action/visualisation
 * @param length ammount of Activities to check
 */
function cshmiObserver(VisualObject, ObjectPath, length, cshmiObject){
	this.VisualObject = VisualObject;
	this.ObjectPath = ObjectPath;
	this.cshmiObject = cshmiObject;
	this.ObserverEntryArray = new Array(length);
	
	//will be overwritten with a specific function
	//should return false on error, true on success
	this.triggerActivity = null;
	
	//useful for own information stuff
	this.customInformation = null;
}
cshmiObserver.prototype = {
		/**
		 * Tests all entries if they are processed
		 * @return {bool} true if ready
		 */
		checkIfReady: function(){
			for (var i=0; i < this.ObserverEntryArray.length; i++) {
				var thisObserverEntry = this.ObserverEntryArray[i];
				
				if(thisObserverEntry === undefined){
					//not all checkConditions are build up till now, waiting for next call
					return false;
				}else if(thisObserverEntry.requirementsFulfilled === false){
					//not all entries are ready till now, waiting for next call
					return false;
				}
			}
			
			return true;
		},
		/**
		 * Calls checkIfReady and calls triggerActivity on success
		 * @return false on error (no triggerActivity defined), null if not ready
		 */
		checkAndTrigger: function(){
			if(this.checkIfReady() === true){
				if(this.triggerActivity !== null){
					return this.triggerActivity();
				}
				return false;
			}
			return null;
		},
		/**
		 * searches right place in the array, saves the value and marks this entry as complete
		 * @param {String} ObjectPath full path to the active Activity
		 * @param {String} value to set
		 */
		updateValueInArray: function(ObjectPath, value){
			for(var i = 0; i < this.ObserverEntryArray.length;i++){
				var thisObserverEntry = this.ObserverEntryArray[i];
				if(ObjectPath === this.ObjectPath+thisObserverEntry.delimiter+thisObserverEntry.ObjectName){
					//we found "our" callback
					thisObserverEntry.requirementsFulfilled = true;
					thisObserverEntry.value = value;
					break;
				}
			}
			return true;
		}
};
/**
 * Constructor for a generic ObserverEntry
 * @param ObjectName
 */
function ObserverEntry(ObjectName, delimiter){
	this.requirementsFulfilled = false;
	if(delimiter === undefined){
		this.delimiter = "/";
	}else{
		this.delimiter = delimiter;
	}
	this.ObjectName = ObjectName;
	this.value = null;
}

var filedate = "$Date$";
filedate = filedate.substring(7, filedate.length-2);
if ("undefined" == typeof HMIdate){
	HMIdate = filedate;
}else if (HMIdate < filedate){
	HMIdate = filedate;
}
