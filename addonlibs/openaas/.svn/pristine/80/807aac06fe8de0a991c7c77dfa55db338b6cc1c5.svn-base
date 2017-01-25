/*
*	Copyright (C) 2015
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
	this.ResourceList.TimeeventCallStack = Object();
	
	this.ResourceList.Elements = Object();
	this.ResourceList.Actions = Object();
	this.ResourceList.baseKsPath = Object();
	this.ResourceList.ChildList = Object();
	this.ResourceList.GlobalVar = Object();
	this.ResourceList.EventInfos = Object();
	this.ResourceList.EventInfos.EventObj = null;
	this.ResourceList.EventInfos.mouseRelativePosition = null;
	this.ResourceList.EventInfos.startXMouse = null;
	this.ResourceList.EventInfos.startYMouse = null;
	this.ResourceList.EventInfos.startXObj = null;
	this.ResourceList.EventInfos.startYObj = null;
	this.ResourceList.newRebuildObject = Object();
	this.ResourceList.cyclicEventList = Object();
	
	this.trashDocument = null;
	if(HMI.svgDocument.implementation && HMI.svgDocument.implementation.createDocument){
		//todo this magic prevents garbage collection!?
		
		this.trashDocument = HMI.svgDocument.implementation.createDocument (HMI.HMI_Constants.NAMESPACE_SVG, "html", null);
	}
	
	//newwrite
	//this.ResourceList.ModellVariables = Object();
	//this.ResourceList.ModellVariables.getValue = ["ksVar", "elemVar", "globalVar", "persistentGlobalVar", "OperatorInput", "TemplateFBReferenceVariable", "TemplateFBVariableReferenceName", "TemplateConfigValues", "value"];
	
	//holds the information if the visualisation is filled with content right now
	this.initStage = false;
	
	//we want to add all elements to a class to find it later
	this.cshmiBlackboxClass = "cshmi-blackbox";
	this.cshmiTemplateClass = "cshmi-template";
	this.cshmiTemplateActionClass = "cshmi-fromTemplateAction";
	this.cshmiGroupHideableClass = "cshmi-hideablegroup";
	this.cshmiGroupHasHideableChildrenClass = "cshmi-GroupHasHideableChildren";
	this.cshmiGroupVisibleChildrenNotLoadedClass = "cshmi-VisibleChildrenNotLoaded";
	
	this.cshmiOperatorClickClass = "cshmi-click";
	this.cshmiOperatorDoubleclickClass = "cshmi-doubleclick";
	this.cshmiOperatorRightclickClass = "cshmi-rightclick";
	this.cshmiOperatorAftermoveClass = "cshmi-aftermove";
	
	//disable foreignObject for now. See comment below
	this.useforeignObject = false;
};





/*#########################################################################################################################
TODO:
setvar type erlauben
ueberall asyncrone requests nutzen
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
		if (typeof JSON === 'object' && typeof JSON.parse === 'function'){
			var response = HMI.KSClient.getVar("//"+Host+"/"+Server+"/TechUnits/cshmi/turbo.asJSON");
			if (response && response !== "" && response !== "{}" && response !== "{{}}" && response !== "{{{}}}" && response.indexOf("KS_ERR") === -1){
				if(response.charAt(1) == "{"){
					//tcl wraps two braces
					response = response.slice(2,-2);
				}else{
					//kshttp wraps one brace
					response = response.slice(1,-1);
				}
				try {
					var plainJSON = decodeURI(response);
					var responseJSON = JSON.parse(plainJSON);
				} catch (e) {
					HMI.hmi_log_info("Parsing Cache was not successful. Skipping: "+e.message);
					//something went wrong, ignore the cache 8-/
					responseJSON = {};
				}
				plainJSON = null;
				if(responseJSON.Elements){
					this.ResourceList.Elements = responseJSON.Elements;
				}
				if(responseJSON.Actions){
					this.ResourceList.Actions = responseJSON.Actions;
				}
				if(responseJSON.baseKsPath){
					this.ResourceList.baseKsPath = responseJSON.baseKsPath;
				}
				if(responseJSON.ChildList){
					this.ResourceList.ChildList = responseJSON.ChildList;
				}
				responseJSON = null;
			}else if(response.indexOf("KS_ERR") === -1){
				HMI.hmi_log_info("Parsing Cache was empty. Skipping. See http://"+HMI.KSClient.getCommunicationPoint(Host+"/"+Server)+"/getLogfile?format=ksx&maxentries=5 for details.");
			}
		}
		
		//we are in the init stage, so the DOM Tree is not populated
		this.initStage = true;
		
		//build the selected sheet aka group. This includes all containing elements
		var VisualObject = this._interpreteElementOrEventRecursive(null, ObjectPath, "/cshmi/Group", false);
		
		if (VisualObject === null){
			if(HMI.InfoOutput.firstChild === null){
				HMI.hmi_log_onwebsite('No sheet with name  "'+ObjectPath+'" available.');
			}
			return;
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
		HMI.saveAbsolutePosition(VisualObject);
		
		this._interpreteOnloadCallStack();
		
		if(false === this.useforeignObject){
			var blackboxes = csHMIgetElementsByClassName(null, this.cshmiBlackboxClass);
			for (var i = 0; i < blackboxes.length; ++i){
				var HTMLcontentNode = document.getElementById(blackboxes[i].id+"*Div");
				if(HTMLcontentNode){
					//push the blackbox thing to the end
					HMI.Playground.appendChild(HTMLcontentNode);
					//correct position of the blackbox. absolutey are not known before
					HTMLcontentNode.style.top = blackboxes[i].getAttribute("absolutey")+"px";
					HTMLcontentNode.style.left = blackboxes[i].getAttribute("absolutex")+"px";
				}
			}
		}
		
		//the DOM Tree is populated now
		this.initStage = false;
	},
	
	/**
	 * Iterprete one Element or Event for visualisation. This is recursive for Elements
	 * @param {SVGElement} VisualParentObject visual Object which is parent to active Object
	 * @param {String} ObjectPath Path to this cshmi object containing the event/action/visualisation
	 * @param {bool} preventNetworkRequest the function should prevent network requests if possible
	 * @return {SVGElement} VisualObject the new constructed element or null
	 */
	_interpreteElementOrEventRecursive: function(VisualParentObject, ObjectPath, ObjectType, preventNetworkRequest){
		var VisualObject = null;
		var Result = true;
		if (ObjectType.indexOf("/cshmi/Group") !== -1 || ObjectType.indexOf("/cshmi/Template") !== -1){
			VisualObject = this._buildSvgGroup(VisualParentObject, ObjectPath, false, null, preventNetworkRequest);
		
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
		
		}else if (ObjectType.indexOf("/cshmi/Blackbox") !== -1){
			VisualObject = this._buildBlackbox(VisualParentObject, ObjectPath, preventNetworkRequest);
		
		}else if (ObjectType.indexOf("/cshmi/ClientEvent") !== -1){
			Result = this._interpreteClientEvent(VisualParentObject, ObjectPath, preventNetworkRequest);
		}else if (ObjectType.indexOf("/cshmi/OperatorEvent") !== -1){
			Result = this._interpreteOperatorEvent(VisualParentObject, ObjectPath, preventNetworkRequest);
		}else if (ObjectType.indexOf("/cshmi/TimeEvent") !== -1){
			Result = this._interpreteTimeEvent(VisualParentObject, ObjectPath, preventNetworkRequest);
		
		}else if (ObjectType.indexOf("/cshmi/SetValue") !== -1 && HMI.instanceOf(VisualParentObject, this.cshmiBlackboxClass)){
			// SetValue is ok to be Child of Blackbox
			return null;
		}else if (ObjectType.indexOf("/cshmi/TranslationSource") !== -1){
			// TranslationSource is placeable at any place
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
		var containerObject = null;
		if (VisualObject !== null){
			if (VisualObject.tagName === "g" && VisualObject.id === ""){
				containerObject = VisualObject;
				VisualObject = VisualObject.firstChild;
			}
			
			//remember the ObjectType on every object (needed for reloading via action)
			VisualObject.setAttribute("data-ObjectType", ObjectType);
			
			// special handling for invisible objects
			if (VisualObject.getAttribute("display") === "none"){
				//we are not visible, so we should not contact network
				this._interpreteChildrensRecursive(VisualObject, ObjectPath, true);
				
				// mark objects incomplete AFTER initialisation
				// _interpreteHiddenChildrenElements has to know if it has to load childrens
				HMI.addClass(VisualObject, this.cshmiGroupVisibleChildrenNotLoadedClass);
			}else{
				//load the childrens
				this._interpreteChildrensRecursive(VisualObject, ObjectPath, preventNetworkRequest);
			}
		}
		
		if (containerObject !== null){
			//container "g"
			return containerObject;
		}else{
			return VisualObject;
		}
	},
	
	/**
	 * calling Actions if supported ClientEvent is triggered
	 * @param {SVGElement} VisualObject Object to manipulate the visualisation
	 * @param {String} ObjectPath Path to this cshmi object containing the event/action/visualisation
	 * @param {bool} preventNetworkRequest the function should prevent network requests if possible
	 * @return {Boolean} true
	 */
	_interpreteClientEvent: function(VisualObject, ObjectPath, preventNetworkRequest){
		var command = ObjectPath.split("/");
		command = command[command.length-1];

		if(VisualObject.ResourceList === undefined){
			VisualObject.ResourceList = new Object();
		}
		if (command === "onload"){
			if(VisualObject.ResourceList.onloadDone === true){
				return true;
			}
			VisualObject.ResourceList.onloadDone = true;
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
		}else if (command === "globalvarchanged"){
			if(VisualObject.ResourceList.globalvarchangedDone === true){
				return true;
			}
			VisualObject.ResourceList.globalvarchangedDone = true;
			
			//remember Action to be called after a globalVar is changed
			var EventObjItem = Object();
			EventObjItem["VisualObject"] = VisualObject;
			EventObjItem["ObjectPath"] = ObjectPath;
			this.ResourceList.globalvarChangedCallStack.push(EventObjItem);
		}else if (command === "unconfigured"){
			//ignore an unconfigured Event
		}else{
			HMI.hmi_log_info_onwebsite("ClientEvent ("+command+") "+ObjectPath+" not supported");
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
		if(VisualObject.ResourceList === undefined){
			VisualObject.ResourceList = new Object();
		}
		if(VisualObject.ResourceList.Times === undefined){
			VisualObject.ResourceList.Times = new Object();
		}
		
		var requestList = new Object();
		//get the information if the config of the object is known through the turbo
		if (this.ResourceList.Actions[ObjectPath] !== undefined){
			requestList[ObjectPath] = this.ResourceList.Actions[ObjectPath].Parameters;
		}else{
			requestList[ObjectPath] = new Object();
			requestList[ObjectPath]["cyctime"] = null;
			
			var successCode = this._requestVariablesArray(requestList);
			if (successCode === false){
				return null;
			}
			
			//we have asked the object successful, so remember the result
			this.ResourceList.Actions[ObjectPath] = new Object();
			this.ResourceList.Actions[ObjectPath].Parameters = requestList[ObjectPath];
		}
		var cyctime = parseFloat(requestList[ObjectPath]["cyctime"]);
		
		var command = ObjectPath.split("/");
		command = command[command.length-1];
		if(VisualObject.ResourceList.Times[command] === true){
			return true;
		}
		VisualObject.ResourceList.Times[command] = true;
		
		if(this.ResourceList.TimeeventCallStack[cyctime] === undefined){
			if (cyctime <= 0){
				//negative values are not valid, zero disables the event
				return true;
			}else if (this.initStage === true && cyctime > 1 ){
				//we dont want to wait too long for the first action
				var nextcyctime = 1;
			}else{
				nextcyctime = cyctime;
			}
			var preserveThis = this;
			this.ResourceList.TimeeventCallStack[cyctime] = new Object();
			this.ResourceList.TimeeventCallStack[cyctime].triggeredObjectList = Array();
			this.ResourceList.TimeeventCallStack[cyctime].getVarCollection = Object();
			this.ResourceList.TimeeventCallStack[cyctime].timeoutID = window.setTimeout(function() {
				if (HMI.cshmi !== preserveThis){
					//the active cshmi display is not "our" one, cancel Timeout
					return true;
				}
				//the function will be executed in the context (this) of the HMI.cshmi object
				HMI.cshmi._handleTimeEvent(cyctime);
			}, nextcyctime*1000);
		}
		
		var EventObjItem = Object();
		EventObjItem["VisualObject"] = VisualObject;
		EventObjItem["ObjectPath"] = ObjectPath;
		this.ResourceList.TimeeventCallStack[cyctime].triggeredObjectList.push(EventObjItem);
		
		return true;
	},
	_handleTimeEvent: function(cyctime){
		var skipEvent = false;
		//check if the page is visible at all?
		//http://www.w3.org/TR/page-visibility/
		//never used with ms or o prefix
		if(	document.hidden === true ||
			document.mozHidden === true||
			document.webkitHidden === true
		){
			skipEvent = true;
		}else if (this.initStage === true){
			//we are in the init state of the sheet so interprete Action later onload, remembering this
			skipEvent = true;
		}
		if(skipEvent === false){
			var thisTime = this.ResourceList.TimeeventCallStack[cyctime];
			for (var objectname in thisTime.getVarCollection) {
				//delete old values
				for (var variablename in thisTime.getVarCollection[objectname]) {
					thisTime.getVarCollection[objectname][variablename] = null;
				}
			}
			var successCode = this._requestVariablesArray(thisTime.getVarCollection, true);
			if (successCode === false){
				//we have no good values, so clear the Collection
				thisTime.getVarCollection = Object();
			}
			
			for (var i = 0; i < thisTime.triggeredObjectList.length;){
				var EventObjItem = thisTime.triggeredObjectList[i];
				if(EventObjItem["VisualObject"].ownerDocument === this.trashDocument){
					//this Object was replaced with rebuildObject, so remove from callstack
					
					thisTime.getVarCollection = Object();
					
					thisTime.triggeredObjectList.splice(i, 1);
				}else{
					this._interpreteAction(EventObjItem["VisualObject"], EventObjItem["ObjectPath"], thisTime.getVarCollection, null);
					i++;
				}
			}
		}
		var preserveThis = this;
		this.ResourceList.TimeeventCallStack[cyctime].timeoutID = window.setTimeout(function() {
			if (HMI.cshmi !== preserveThis){
				//the active cshmi display is not "our" one, cancel Timeout
				return true;
			}
			//the function will be executed in the context (this) of the HMI.cshmi object
			HMI.cshmi._handleTimeEvent(cyctime);
		}, cyctime*1000);
	},
	
	/**
	 * detect all OperatorEvents and register them
	 * @param {SVGElement} VisualObject Object to manipulate the visualisation
	 * @param {String} ObjectPath Path to this cshmi object containing the event/action/visualisation
	 * @param {bool} preventNetworkRequest the function should prevent network requests if possible
	 * @return {Boolean} true
	 */
	_interpreteOperatorEvent: function(VisualObject, ObjectPath, preventNetworkRequest){
		if(VisualObject.ResourceList === undefined){
			VisualObject.ResourceList = new Object();
		}
		
		var command = ObjectPath.split("/");
		command = command[command.length-1];
		if (command === "click"){
			if(VisualObject.ResourceList.OperatorClickeventDone === true){
				return true;
			}
			VisualObject.ResourceList.OperatorClickeventDone = true;
			
			VisualObject.setAttribute("cursor", "pointer");
			HMI.addClass(VisualObject, this.cshmiOperatorClickClass);
			VisualObject.setAttribute("data-clickpath", ObjectPath);
			VisualObject.addEventListener("click", function(evt){
				//quit propagation of event in any case. We do not want the parent object to handle the event
				if (evt.stopPropagation) evt.stopPropagation();
				
				if(HMI.getComponent(VisualObject, HMI.cshmi.cshmiOperatorAftermoveClass)){
					//fixme handle toggle
					
					//we have an movegesture on the same VisualObject, so this handled all action in mouse up
					return;
				}
				
				//"this" is here the event target object, so work on HMI.cshmi
				HMI.cshmi.ResourceList.EventInfos.EventObj = evt;
				//mark changed VisualObject for quick visual feedback (hidden after a second)
				HMI.displaygestureReactionMarker(VisualObject);
				
				//toggle visibility of hideable childtemplates
				HMI.cshmi.toggleChildGroups(VisualObject);
				
				//get and execute all actions
				HMI.cshmi._interpreteAction(VisualObject, ObjectPath, null, null);
				
				//an later action should not interprete this event
				HMI.cshmi.ResourceList.EventInfos.EventObj = null;
			}, false);
		}else if (command === "doubleclick"){
			if(VisualObject.ResourceList.OperatorDoubleClickeventDone === true){
				return true;
			}
			VisualObject.ResourceList.OperatorDoubleClickeventDone = true;
			
			VisualObject.setAttribute("cursor", "pointer");
			HMI.addClass(VisualObject, this.cshmiOperatorDoubleclickClass);
			VisualObject.setAttribute("data-doubleclickpath", ObjectPath);
			VisualObject.addEventListener("dblclick", function(evt){
				//quit propagation of event in any case. We do not want the parent object to handle the event
				if (evt.stopPropagation) evt.stopPropagation();
				
				HMI.cshmi.ResourceList.EventInfos.EventObj = evt;
				//mark changed VisualObject for quick visual feedback (hidden after a second)
				HMI.displaygestureReactionMarker(VisualObject);
				
				//get and execute all actions
				HMI.cshmi._interpreteAction(VisualObject, ObjectPath, null, null);
				
				//an later action should not interprete this event
				HMI.cshmi.ResourceList.EventInfos.EventObj = null;
			}, false);
		}else if (command === "rightclick"){
			if(VisualObject.ResourceList.OperatorRightClickeventDone === true){
				return true;
			}
			VisualObject.ResourceList.OperatorRightClickeventDone = true;
			
			VisualObject.setAttribute("cursor", "pointer");
			HMI.addClass(VisualObject, this.cshmiOperatorRightclickClass);
			VisualObject.setAttribute("data-rightclickpath", ObjectPath);
			VisualObject.addEventListener("contextmenu", function(evt){
				//quit propagation of event in any case. We do not want the parent object to handle the event
				if (evt.stopPropagation) evt.stopPropagation();
				if (evt.preventDefault) evt.preventDefault();  //default is a context menu, so disable it
				
				HMI.cshmi.ResourceList.EventInfos.EventObj = evt;
				//mark changed VisualObject for quick visual feedback (hidden after a second)
				HMI.displaygestureReactionMarker(VisualObject);
				
				//get and execute all actions
				HMI.cshmi._interpreteAction(VisualObject, ObjectPath, null, null);
				
				//an later action should not interprete this event
				HMI.cshmi.ResourceList.EventInfos.EventObj = null;
			}, false);
		}else if (command === "mouseover"){
			if(VisualObject.ResourceList.OperatorMouseOvereventDone === true){
				return true;
			}
			VisualObject.ResourceList.OperatorMouseOvereventDone = true;
			
			var Eventname = "mouseover";
			if ('onmouseenter' in HMI.svgDocument.documentElement){
				//mouseenter is far better, than mouseover
				Eventname = "mouseenter";
			}
			
			VisualObject.addEventListener(Eventname, function(evt){
				//quit propagation of event in any case. We do not want the parent object to handle the event
				if (evt.stopPropagation) evt.stopPropagation();
				
				HMI.cshmi.ResourceList.EventInfos.EventObj = evt;
				
				//get and execute all actions
				HMI.cshmi._interpreteAction(VisualObject, ObjectPath, null, null);
				
				//an later action should not interprete this event
				HMI.cshmi.ResourceList.EventInfos.EventObj = null;
			}, false);
		}else if (command === "mouseout"){
			if(VisualObject.ResourceList.OperatorMouseouteventDone === true){
				return true;
			}
			VisualObject.ResourceList.OperatorMouseouteventDone = true;
			
			//mouseleave is far better, than mouseout
			var Eventname = "mouseout";
			if ('onmouseleave' in HMI.svgDocument.documentElement){
				//mouseleave is far better, than mouseout
				Eventname = "mouseleave";
			}
			
			VisualObject.addEventListener(Eventname, function(evt){
				//quit propagation of event in any case. We do not want the parent object to handle the event
				if (evt.stopPropagation) evt.stopPropagation();
				
				HMI.cshmi.ResourceList.EventInfos.EventObj = evt;
				
				//get and execute all actions
				HMI.cshmi._interpreteAction(VisualObject, ObjectPath, null, null);
				
				//an later action should not interprete this event
				HMI.cshmi.ResourceList.EventInfos.EventObj = null;
			}, false);
		}else if (command === "aftermove"){
			if(VisualObject.ResourceList.OperatorAftermoveeventDone === true){
				return true;
			}
			VisualObject.ResourceList.OperatorAftermoveeventDone = true;
			
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
				HMI.cshmi._moveHandleClick(VisualObject, ObjectPath, evt);
			};
			
			//try both, mousedown and mousetouch. mousetouch will fire first, there we will kill mousedown
			VisualObject.addEventListener("touchstart", VisualObject._moveStartDragThunk, false);
			VisualObject.addEventListener("mousedown", VisualObject._moveStartDragThunk, false);
			VisualObject.addEventListener("MSPointerDown", VisualObject._moveStartDragThunk, false);
		}else if (command === "unconfigured"){
			//ignore an unconfigured Event
		}else{
			HMI.hmi_log_info_onwebsite("OperatorEvent ("+command+") "+ObjectPath+" not supported");
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
		if(VisualObject.hasAttribute("x")){
			this.ResourceList.EventInfos.startXObj = parseInt(VisualObject.getAttribute("x"), 10);
			this.ResourceList.EventInfos.startYObj = parseInt(VisualObject.getAttribute("y"), 10);
		}else if(VisualObject.hasAttribute("cx")){
			this.ResourceList.EventInfos.startXObj = parseInt(VisualObject.getAttribute("cx"), 10);
			this.ResourceList.EventInfos.startYObj = parseInt(VisualObject.getAttribute("cy"), 10);
		}else if(VisualObject.hasAttribute("x1")){
			this.ResourceList.EventInfos.startXObj = parseInt(VisualObject.getAttribute("x1"), 10);
			this.ResourceList.EventInfos.startYObj = parseInt(VisualObject.getAttribute("y1"), 10);
		}
		
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
	 * stops propagation of event. Used in aftermove and route polyline
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
			if(typeof VisualObject.adjustingPolylinePoints === "function"){
				VisualObject.adjustingPolylinePoints(newx, newy);
			}else{
				this._setXYRotate(VisualObject, newx, newy, null);
			} 
			
			//save event for use in an action
			this.ResourceList.EventInfos.EventObj = evt;
		}
		if (evt.stopPropagation) evt.stopPropagation();
		if (evt.preventDefault) evt.preventDefault();  //default is scrolling with touch, so disable it
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
		if (evt.stopPropagation) evt.stopPropagation();
		
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
		/*	if we have an movegesture and an click on one object, and a child with an click this feature will fire both events
		 *	it seems to be ok, if this code is not used (august 2013) TODO remove?
		 */
		var x;
		var y;
		if(VisualObject.hasAttribute("x")){	//svg, rect
			x = parseFloat(VisualObject.getAttribute("x"));
			y = parseFloat(VisualObject.getAttribute("y"));
		}else if(VisualObject.hasAttribute("x1")){	//lines
			x = parseFloat(VisualObject.getAttribute("x1"));
			y = parseFloat(VisualObject.getAttribute("y1"));
		}else if(VisualObject.hasAttribute("cx")){	//circles
			x = parseFloat(VisualObject.getAttribute("cx"));
			y = parseFloat(VisualObject.getAttribute("cy"));
		}
		
		var movementX = Math.abs(this.ResourceList.EventInfos.startXObj - x);
		var movementY = Math.abs(this.ResourceList.EventInfos.startYObj - y);
		
		if(typeof VisualObject.savingPolylinePoints === "function"){
			//we manipulating a routed polyline
			VisualObject.savingPolylinePoints();
			return;
		}
		
		//restore old position
		this._setXYRotate(VisualObject, this.ResourceList.EventInfos.startXObj, this.ResourceList.EventInfos.startYObj, VisualObject.getAttribute("data-rotate"));
		
		if (canceled === true){
			//no action
		}else if(isNumeric(movementX) && isNumeric(movementY) && movementX < 5 && movementY < 5){
			//no movement detected, so interprete the click on the right Target
			//search the firstchild which has a click event
			var ClickTarget = HMI.getComponent(evt, this.cshmiOperatorClickClass);
			
			if(ClickTarget !== null){
				this.ResourceList.EventInfos.mouseRelativePosition = null;
				this._interpreteAction(ClickTarget, ClickTarget.getAttribute("data-clickpath"), null, null);
			}
		}else{
			//get and execute all actions
			this._interpreteAction(VisualObject, ObjectPath, null, null);
		}
		
		//an later action should not interprete this event
		this.ResourceList.EventInfos.EventObj = null;
		this.ResourceList.EventInfos.mouseRelativePosition = null;
	},
	
	/**
	 * detect all Actions and triggers them
	 * @param {SVGElement} VisualObject Object to manipulate the visualisation
	 * @param {String} ObjectPath Path to this cshmi object containing the event/action/visualisation
	 * @param CyctimeObject object for fetching values as a bunch or null
	 * @param ChildrenIterator of the iterator or null
	 * @return returnValue returnValue from the last Action
	 */
	_interpreteAction: function(VisualObject, ObjectPath, CyctimeObject, ChildrenIterator){
		var returnValue = true;
		var responseArray = HMI.KSClient.getChildObjArray(ObjectPath, this);
		
		//newwrite
		//fetch config from all childrens via this.ResourceList.ModellVariables.*
		
		//a server could be not available, or we request a not existing (xpos) variable. This should not cancel the other actions
		for (var i=0; i < responseArray.length; i++) {
			var varName = responseArray[i].split(" ");
			if (varName[1].indexOf("/cshmi/SetValue") !== -1){
				returnValue = this._setValue(VisualObject, ObjectPath+"/"+varName[0], "static", CyctimeObject, ChildrenIterator);
			}else if (varName[1].indexOf("/cshmi/SetConcatValue") !== -1){
				returnValue = this._setValue(VisualObject, ObjectPath+"/"+varName[0], "concat", CyctimeObject, ChildrenIterator);
			}else if (varName[1].indexOf("/cshmi/SetMathValue") !== -1){
				returnValue = this._setValue(VisualObject, ObjectPath+"/"+varName[0], "math", CyctimeObject, ChildrenIterator);
			}else if (varName[1].indexOf("/cshmi/GetValue") !== -1){
				HMI.hmi_log_info_onwebsite("GetValue Action ("+varName[1]+")"+ObjectPath+" not useful at this position");
			}else if (varName[1].indexOf("/cshmi/RenameObject") !== -1){
				returnValue = this._interpreteRenameObject(VisualObject, ObjectPath+"/"+varName[0], ChildrenIterator);
			}else if (varName[1].indexOf("/cshmi/CreateObject") !== -1){
				returnValue = this._interpreteCreateObject(VisualObject, ObjectPath+"/"+varName[0], ChildrenIterator);
			}else if (varName[1].indexOf("/cshmi/DeleteObject") !== -1){
				returnValue = this._interpreteDeleteObject(VisualObject, ObjectPath+"/"+varName[0], ChildrenIterator);
			}else if (varName[1].indexOf("/cshmi/LinkObjects") !== -1){
				returnValue = this._interpreteLinkObjects(VisualObject, ObjectPath+"/"+varName[0], ChildrenIterator);
			}else if (varName[1].indexOf("/cshmi/UnlinkObjects") !== -1){
				returnValue = this._interpreteUnlinkObjects(VisualObject, ObjectPath+"/"+varName[0], ChildrenIterator);
			}else if (varName[1].indexOf("/cshmi/IfThenElse") !== -1){
				returnValue = this._interpreteIfThenElse(VisualObject, ObjectPath+"/"+varName[0], CyctimeObject, ChildrenIterator);
			}else if (varName[1].indexOf("/cshmi/ChildrenIterator") !== -1){
				returnValue = this._interpreteChildrenIterator(VisualObject, ObjectPath+"/"+varName[0], ChildrenIterator);
			}else if (varName[1].indexOf("/cshmi/InstantiateTemplate") !== -1){
				returnValue = this._interpreteInstantiateTemplate(VisualObject, ObjectPath+"/"+varName[0], ChildrenIterator);
			}else if (varName[1].indexOf("/cshmi/RoutePolyline") !== -1){
				returnValue = this._interpreteRoutePolyline(VisualObject, ObjectPath+"/"+varName[0]);
			}else if (varName[1].indexOf("/cshmi/RebuildObject") !== -1){
				returnValue = this._interpreteRebuildObject(VisualObject, ObjectPath+"/"+varName[0]);
			}else if (varName[1].indexOf("/cshmi/Vibrate") !== -1){
				returnValue = this._interpreteVibrate(VisualObject, ObjectPath+"/"+varName[0]);
			}else if (varName[1].indexOf("/cshmi/debuggingbreakpoint") !== -1){
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
	 * @param callerObserver Object which hold info for the callback or null
	 * @param CyctimeObject object for fetching values as a bunch or null
	 * @param ChildrenIterator of the iterator or null
	 * @param forceNetworkrequest if true a value is requested from the network even if the object is not visible right now
	 * @return {bool} false if error, null if intentionally no value, "" if no entry found, true if the request is handled by a callback
	 */
	_getValue: function(VisualObject, ObjectPath, callerObserver, CyctimeObject, ChildrenIterator, forceNetworkrequest){
		var ParameterName = "";
		var ParameterValue = "";
		//get the information if the config of the object is known through the turbo
		if (this.ResourceList.Actions[ObjectPath] !== undefined){
			ParameterName = this.ResourceList.Actions[ObjectPath].ParameterName;
			ParameterValue = this.ResourceList.Actions[ObjectPath].ParameterValue;
		}else{
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
		}
		
		var preventNetworkRequest = false;
		if(forceNetworkrequest !== true){
			var iterationObject = VisualObject;
			do{
				//skip eventhandling if the object is not visible and the initstage is active
				if(iterationObject.getAttribute("display") === "none" && this.initStage === false){
					preventNetworkRequest = true;
					
					//doku unsichtbare objekte koennen nicht per ks getriggert sichtbar werden
					
					break;
				}
			}while( (iterationObject = iterationObject.parentNode) && iterationObject !== null && iterationObject.namespaceURI == HMI.HMI_Constants.NAMESPACE_SVG);
			iterationObject = null;
		}
		
		if(this.initStage === true || ChildrenIterator){
			// force sync request in the init stage.
			// the order of actions have to be fixed in the loading
			// for example a value can be precached and reused later
			// force sync request with an active Iterator as the currentChild is lost at a async call
			callerObserver = null;
			var GetVarCbfnc = null;
		}else{
			//after load, we can use async requests
			
			//the callback fills the callerObserver with data and call the checkAndTrigger()
			GetVarCbfnc = function(Client, req){
				if(req.status !== 200){
					var newValue = false;
				}else{
					var response = HMI.KSClient.unescapeString(req.responseText);
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
			//prevent cyclic fetching
			if(CyctimeObject !== undefined && CyctimeObject !== null){
				var path = this._generateFullKsPath(VisualObject, ObjectPath, ParameterValue);
				var pathArray = path.split(".");
				var variablename = pathArray.pop();
				var objectname = pathArray.join(".");
				if(path.indexOf("vendor/") !== -1){
					//vendor is special as usual...
					objectname = "";
					variablename = path;
				}
				//prevent cyclic fetching
				if(CyctimeObject[objectname] === undefined && CyctimeObject[objectname][variablename] !== undefined || CyctimeObject[objectname][variablename] !== undefined){
					//problem: if we remove this entry, a doublicate request (which has no preventNetworkRequest) is left without a value
					//delete CyctimeObject[objectname][variablename];
				}
			}
			//intentionally no value
			return null;
		}else if (ParameterName === "ksVar" && preventNetworkRequest === false){
			var response;
			var path = this._generateFullKsPath(VisualObject, ObjectPath, ParameterValue);
			if(CyctimeObject !== undefined && CyctimeObject !== null){
				var pathArray = path.split(".");
				var variablename = pathArray.pop();
				var objectname = pathArray.join(".");
				if(path.indexOf("vendor/") !== -1){
					//vendor is special as usual...
					objectname = "";
					variablename = path;
				}
				if(CyctimeObject[objectname] === undefined){
					CyctimeObject[objectname] = Object();
				}
				if(CyctimeObject[objectname][variablename] === undefined || CyctimeObject[objectname][variablename] === null){
					//this path needs a cyclic request
					CyctimeObject[objectname][variablename] = null;
				}else{
					//the value was requested in this cycle
					return CyctimeObject[objectname][variablename];
				}
			}
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
			//some elemVars are special, as they are different in Model and SVG
			if (ParameterValue === "strokeWidth"){
				return VisualObject.getAttribute("stroke-width");
			}else if (ParameterValue === "content"){
				if (typeof VisualObject.textContent != "undefined"){
					return VisualObject.textContent;
				}else if (typeof VisualObject.innerText != "undefined"){
					return VisualObject.innerText;
				}else{
					//asv compatibility broken
					return "";
				}
			}else if (ParameterValue === "title"){
				for (var i = 0;i < VisualObject.childNodes.length;i++){
					if (VisualObject.childNodes.item(i).tagName === "title"){
						return VisualObject.childNodes.item(i).firstChild.textContent;
					}
				} 
				return "";
			}else if (ParameterValue === "trimToLength"){
				if(VisualObject.hasAttribute("data-trimToLength")){
					return VisualObject.getAttribute("data-trimToLength");
				}else{
					return "0";
				}
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
				}else if (VisualObject.getAttribute("dy") === "0.6ex"){
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
				return getRotationFromObject(VisualObject).toString();
			}else if (ParameterValue === "previousTemplateCount"){
				if (HMI.instanceOf(VisualObject, this.cshmiTemplateClass) === false){
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
			}else if (ParameterValue === "getPolylineTotalLength" ||
					ParameterValue.indexOf("getPolylinePointXAtFractionLength") !== -1||
					ParameterValue.indexOf("getPolylinePointYAtFractionLength") !== -1){

				//find a polyline with Coords in a parent group
				var IteratorObj = VisualObject;
				var Coords = null;
				while( (IteratorObj = IteratorObj.parentNode) && IteratorObj !== null && IteratorObj.namespaceURI == HMI.HMI_Constants.NAMESPACE_SVG){
					for(var i = 0; i < IteratorObj.childNodes.length;i++){
						if (IteratorObj.childNodes[i].tagName === "polyline" && IteratorObj.childNodes[i].ResourceList && IteratorObj.childNodes[i].ResourceList.RoutePolyline && IteratorObj.childNodes[i].ResourceList.RoutePolyline.Coords !== undefined){
							Coords = IteratorObj.childNodes[i].ResourceList.RoutePolyline.Coords;
							break;
						}
					}
					if(Coords !== null){
						break;
					}
				}
				if(Coords === null){
					return "";
				}
				if (ParameterValue === "getPolylineTotalLength"){
					return getPolylineTotalLength(Coords).toString();
				}
				
				//e.g. "getPolylinePointXAtFractionLength:TemplateFBReferenceVariable:fraction"
				var splittedValueParameter = ParameterValue.split(":");
				var fraction = 0;
				if (splittedValueParameter.length > 2){
					this.ResourceList.Actions["tempPath"] = new Object();
					this.ResourceList.Actions["tempPath"].ParameterName = splittedValueParameter[1];
					this.ResourceList.Actions["tempPath"].ParameterValue = splittedValueParameter[2];
					var tempfraction = this._getValue(VisualObject, "tempPath", null, null, null, true);
					if(tempfraction){
						fraction = parseFloat(tempfraction);
					}
				}
				var Point = getPolylinePointFromFraction(Coords, fraction);
				if(Point.x !== null && ParameterValue.indexOf("getPolylinePointXAtFractionLength") !== -1){
					return Point.x.toString();
				}else if(Point.y !== null && ParameterValue.indexOf("getPolylinePointYAtFractionLength") !== -1){
					return Point.y.toString();
				}
				return "";
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
						var defaultValue = this._getValue(VisualObject, "tempPath", null, null, null, true);
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
			var FBRef = this._getFBReference(VisualObject, ChildrenIterator);
			if (FBRef.path === ""){
				return false;
			}else if (ParameterValue === "CSHMIHostServer"){
				// should be //dev:7509/server1
				var PathArray = FBRef.path.split("/");
				if(PathArray.length < 2){
					return "";
				}
				return "//"+PathArray[2]+"/"+PathArray[3];
			}else if (ParameterValue === "fullqualifiedparentname" || ParameterValue === "CSHMIfullqualifiedparentname"){
				// should be //dev:7509/server1/TechUnits
				var PathArray = FBRef.path.split("/");
				PathArray.pop();
				return PathArray.join("/");
			}else if (ParameterValue === "fullqualifiedname" || ParameterValue === "CSHMIfullqualifiedname"){
				// should be //dev:7509/server1/TechUnits/add
				return FBRef.path;
			}else if (ParameterValue === "absoluteparentpathname" || ParameterValue === "CSHMIabsoluteparentpathname"){
				// should be /TechUnits
				var PathArray = FBRef.path.split("/");
				PathArray.pop();
				return HMI.KSClient._splitKSPath(PathArray.join("/"))[1];
			}else if (ParameterValue === "absolutepathname" || ParameterValue === "CSHMIabsolutepathname"){
				// should be /TechUnits/add
				return HMI.KSClient._splitKSPath(FBRef.path)[1];
			}else if (ParameterValue === "identifier"){
				//if the identifier is requested calculate this to avoid network request
				var Objectname = FBRef.path.split("/");
				var identifier = Objectname[Objectname.length - 1];
				//if we refer to an variable return this name only
				identifier = identifier.split(".");
				return identifier[identifier.length - 1];
			}
			if(ParameterValue.charAt(0) !== "/"){
				var path = FBRef.path+"."+ParameterValue;
			}else{
				//todo doku
				path = FBRef.path+ParameterValue;
			}
			if(CyctimeObject !== undefined && CyctimeObject !== null){
				var pathArray = path.split(".");
				var variablename = pathArray.pop();
				var objectname = pathArray.join(".");
				if(path.indexOf("vendor/") !== -1){
					//vendor is special as usual...
					objectname = "";
					variablename = path;
				}
				if(CyctimeObject[objectname] === undefined){
					CyctimeObject[objectname] = Object();
				}
				if(preventNetworkRequest === true){
					//prevent cyclic fetching
					if(CyctimeObject[objectname] === undefined && CyctimeObject[objectname][variablename] !== undefined || CyctimeObject[objectname][variablename] !== undefined){
						//problem: if we remove this entry, a doublicate request (which has no preventNetworkRequest) is left without a value
						//delete CyctimeObject[objectname][variablename];
					}
					//intentionally no value
					return null;
				}else if(CyctimeObject[objectname][variablename] === undefined || CyctimeObject[objectname][variablename] === null){
					//this path needs a cyclic request
					CyctimeObject[objectname][variablename] = null;
				}else{
					//the value was requested in this cycle
					return CyctimeObject[objectname][variablename];
				}
			}else if(preventNetworkRequest === true){
				//intentionally no value
				return null;
			}
			
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
			var TemplateObject = VisualObject;
			do{
				if(TemplateObject.ResourceList && TemplateObject.ResourceList.FBVariableReference && TemplateObject.ResourceList.FBVariableReference[ParameterValue] !== undefined){
					//a FBVariableReferenceName was requested
					if (TemplateObject.ResourceList.FBVariableReference[ParameterValue].charAt(0) === "/"){
						//String begins with / so it is a fullpath
						var path = TemplateObject.ResourceList.FBVariableReference[ParameterValue];
						if(CyctimeObject !== undefined && CyctimeObject !== null){
							var pathArray = path.split(".");
							var variablename = pathArray.pop();
							var objectname = pathArray.join(".");
							if(path.indexOf("vendor/") !== -1){
								//vendor is special as usual...
								objectname = "";
								variablename = path;
							}
							if(CyctimeObject[objectname] === undefined){
								CyctimeObject[objectname] = Object();
							}
							if(preventNetworkRequest === true){
								//prevent cyclic fetching
								if(CyctimeObject[objectname] === undefined && CyctimeObject[objectname][variablename] !== undefined || CyctimeObject[objectname][variablename] !== undefined){
									//problem: if we remove this entry, a doublicate request (which has no preventNetworkRequest) is left without a value
									//delete CyctimeObject[objectname][variablename];
								}
								//intentionally no value
								return null;
							}else if(CyctimeObject[objectname][variablename] === undefined || CyctimeObject[objectname][variablename] === null){
								//this path needs a cyclic request
								CyctimeObject[objectname][variablename] = null;
							}else{
								//the value was requested in this cycle
								return CyctimeObject[objectname][variablename];
							}
						}else if(preventNetworkRequest === true){
							//intentionally no value
							return null;
						}
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
				if(TemplateObject.ResourceList && TemplateObject.ResourceList.ConfigValues && TemplateObject.ResourceList.ConfigValues[ParameterValue] !== undefined){
					//this is a ConfigValue and has valid data for us
					return TemplateObject.ResourceList.ConfigValues[ParameterValue];
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
	 * calls (multiple) getValue and after that calls _setValueExecute on success
	 * @param {SVGElement} VisualObject Object to manipulate the visualisation
	 * @param {String} ObjectPath Path to this cshmi object containing the event/action/visualisation
	 * @param {String} GetType "static" one static OV_PART, "concat" concat multiple getValues, "math" mathematics operation
	 * @param CyctimeObject object for fetching values as a bunch or null
	 * @param ChildrenIterator of the iterator or null
	 * @return false on error, true on success
	 */
	_setValue: function(VisualObject, ObjectPath, GetType, CyctimeObject, ChildrenIterator){
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
						HMI.hmi_log_info("setStaticValue on "+thisObserverEntry.ObjectName+" (baseobject: "+this.VisualObject.id+") failed because of an error in getValue: "+ObjectPath+".value");
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
				this.cshmiObject._setValueExecute(this.VisualObject, this.ObjectPath, GetType, NewValue, ChildrenIterator, false);
				return true;
			};
			var thisObserverEntry = new ObserverEntry("value", ".");
			setValueObserver.ObserverEntryArray[0] = thisObserverEntry;
			//via getValue-part of setValue object
			var NewValue = this._getValue(VisualObject, ObjectPath+".value", setValueObserver, CyctimeObject, ChildrenIterator);
			
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
						HMI.hmi_log_info("setConCatValue on "+thisObserverEntry.ObjectName+" (baseobject: "+this.VisualObject.id+") failed because of an error in a getValue from: "+ObjectPath);
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
				this.cshmiObject._setValueExecute(this.VisualObject, this.ObjectPath, GetType, NewValue, ChildrenIterator, false);
				return true;
			};
			
			for (var i=0; i < responseArray.length; i++) {
				var varName = responseArray[i].split(" ");
				if (varName[1].indexOf("/cshmi/GetValue") !== -1){
					var thisObserverEntry = new ObserverEntry(varName[0], "/");
					setValueObserver.ObserverEntryArray[i] = thisObserverEntry;
					
					//via getValue instance under setValue object
					var NewValuePart = this._getValue(VisualObject, ObjectPath+"/"+varName[0], setValueObserver, CyctimeObject, ChildrenIterator);
					
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
						HMI.hmi_log_info("setMathValue on "+thisObserverEntry.ObjectName+" (baseobject: "+this.VisualObject.id+") failed because of an error in a getValue from: "+ObjectPath);
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
					}else if (thisObserverEntry.ObjectName.indexOf("mod") === 0){
						NewValue = NewValue%NewValuePart;
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
					}else if (thisObserverEntry.ObjectName.indexOf("ran") === 0){
						NewValue = NewValue + Math.random()*NewValuePart;
					}else if (thisObserverEntry.ObjectName.indexOf("pow") === 0){
						NewValue = Math.pow(NewValue, parseFloat(NewValuePart));
					}
					
					if (!isFinite(NewValue)){
						NewValue = 0;
					}
				}
				//force string format
				NewValue = NewValue.toString();
				
				this.cshmiObject._setValueExecute(this.VisualObject, this.ObjectPath, GetType, NewValue, ChildrenIterator, false);
				return true;
			};
			
			for (var i=0; i < responseArray.length; i++) {
				var varName = responseArray[i].split(" ");
				if (varName[1].indexOf("/cshmi/GetValue") !== -1){
					var thisObserverEntry = new ObserverEntry(varName[0], "/");
					setValueObserver.ObserverEntryArray[i] = thisObserverEntry;
					
					//via getValue instance under setValue object
					var NewValuePart = this._getValue(VisualObject, ObjectPath+"/"+varName[0], setValueObserver, CyctimeObject, ChildrenIterator);
					
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
	 * @param ChildrenIterator of the iterator or null
	 * @param {Boolean} ignoreError a unconfigured setvar should not trigger a warning
	 * @return false on error, true on success
	 */
	_setValueExecute: function(VisualObject, ObjectPath, GetType, NewValue, ChildrenIterator, ignoreError){
		//get info where to set the NewValue
		
		var ParameterName = "";
		var ParameterValue = "";
		var TranslationSourcePath = "";
		//get the information if the config of the object is known through the turbo
		if (this.ResourceList.Actions[ObjectPath] !== undefined){
			ParameterName = this.ResourceList.Actions[ObjectPath].ParameterName;
			ParameterValue = this.ResourceList.Actions[ObjectPath].ParameterValue;
			if (GetType === "static"){
				TranslationSourcePath = this.ResourceList.Actions[ObjectPath].translationSource;
			}
		}else{
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
				requestList[ObjectPath]["translationSource"] = null;
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
			if (GetType === "static"){
				TranslationSourcePath = requestList[ObjectPath]["translationSource"];
			}
			
			//feeding garbage collector early
			requestList = null;
			
			//we have asked the object successful, so remember the result
			this.ResourceList.Actions[ObjectPath] = new Object();
			this.ResourceList.Actions[ObjectPath].ParameterName = ParameterName;
			this.ResourceList.Actions[ObjectPath].ParameterValue = ParameterValue;
			this.ResourceList.Actions[ObjectPath].translationSource = TranslationSourcePath;
		}
		//translate if needed
		if (TranslationSourcePath !== ""){
			var TranslationMapping = new Object();
			if (this.ResourceList.Actions && this.ResourceList.Actions[TranslationSourcePath] !== undefined){
				//get the information if the config of the object is known through the turbo
				if(this.ResourceList.Actions[TranslationSourcePath].Parameters.TranslationMappingRaw){
					var TranslationListArray = this.ResourceList.Actions[TranslationSourcePath].Parameters.TranslationMappingRaw;
					var KeyValueEntry = null;
					for (var i=0; i < TranslationListArray.length; i++) {
						KeyValueEntry = TranslationListArray[i].split(":");
						if (KeyValueEntry.length >= 2){
							if (KeyValueEntry.length > 2){
								var Value = KeyValueEntry.slice(1).join(":");
							}else{
								Value = KeyValueEntry[1];
							}
							TranslationMapping[KeyValueEntry[0]] = Value;
						}
					}
					delete this.ResourceList.Actions[TranslationSourcePath].Parameters.TranslationMappingRaw;
					this.ResourceList.Actions[TranslationSourcePath].Parameters.TranslationMapping = TranslationMapping;
				}else if(this.ResourceList.Actions[TranslationSourcePath].Parameters.TranslationMapping){
					TranslationMapping = this.ResourceList.Actions[TranslationSourcePath].Parameters.TranslationMapping;
				}
			}else{
				var requestTranslationList = new Object();
				requestTranslationList[TranslationSourcePath] = new Object();
				requestTranslationList[TranslationSourcePath].Parameters = new Object();
				requestTranslationList[TranslationSourcePath].Parameters["translationMapping"] = null;
				
				var successCode = this._requestVariablesArray(requestTranslationList, true);
				if (successCode === false){
					return null;
				}
				
				var TranslationListArray = requestTranslationList[TranslationSourcePath]["translationMapping"].split(" ");
				var KeyValueEntry = null;
				var lastEntry = null;
				for (var i=0; i < TranslationListArray.length; i++) {
					KeyValueEntry = TranslationListArray[i].split(":");
					if (KeyValueEntry.length >= 2){
						if (KeyValueEntry.length > 2){
							var Value = KeyValueEntry.slice(1).join(":");
						}else{
							Value = KeyValueEntry[1];
						}
						TranslationMapping[KeyValueEntry[0]] = Value;
						lastEntry = KeyValueEntry[0];
					}else if (KeyValueEntry.length === 1 && lastEntry !== null){
						//we had something like "pumpcolor:yellow pumpname:N 18", so need to add the " 18" to the last entry
						TranslationMapping[lastEntry] = TranslationMapping[lastEntry]+" "+KeyValueEntry[0];
					}
				}
				
				//we have asked the object successful, so remember the result
				this.ResourceList.Actions[TranslationSourcePath] = new Object();
				this.ResourceList.Actions[TranslationSourcePath].Parameters.TranslationMapping = TranslationMapping;
			}
			if(TranslationMapping[NewValue] !== undefined){
				//translate
				NewValue = TranslationMapping[NewValue];
			}
		}
		
		var SetVarCbfnc = function(Client, req){
			if(req.status !== 200){
				var response = HMI.KSClient.unescapeString(req.responseText);
				if (response.indexOf("KS_ERR_BADPARAM") !== -1){
					HMI.hmi_log_onwebsite('Setting "'+NewValue+'" at '+path+' not successfull: Bad Parameter ');
				}else if (response.indexOf("KS_VT_VOID") !== -1 || req.status === 412){
					//we are setting a value for a void variable:
					
					//tcl response: "TksS-0146::TKS_NOTIMPLEMENTED variable type KS_VT_VOID not implemented"
					//kshttp response: http status 412
					
					//guessing the correct Vartype
					if(NewValue.toUpperCase() == "TRUE" || NewValue.toUpperCase() == "FALSE"){
						HMI.KSClient.setVar(path, NewValue, "KS_VT_BOOL", null, false);
					}else if(!isNumeric(NewValue)){
						HMI.KSClient.setVar(path, NewValue, "KS_VT_STRING", null, false);
					}else if(NewValue.indexOf(".") !== -1){
						HMI.KSClient.setVar(path, NewValue, "KS_VT_DOUBLE", null, false);
					}else{
						HMI.KSClient.setVar(path, NewValue, "KS_VT_INT", null, false);
					}
				}else if (response.indexOf("KS_ERR") !== -1){
					HMI.hmi_log_info('Setting "'+NewValue+'" at variable '+path+' not successfull: '+response+' (configured here: '+ObjectPath+').');
				}else{
					HMI.hmi_log_info('Setting a variable failed.');
				}
			}
		};
		
		//set the new Value
		if (ParameterName === "ksVar"){
			var result;
			var path = this._generateFullKsPath(VisualObject, ObjectPath, ParameterValue);
			
			result = HMI.KSClient.setVar(path, NewValue, null, SetVarCbfnc, true);
			
			if (result !== true){
				return false;
			}
			return true;
		}else if (ParameterName === "elemVar"){
			//some elemVars are special, as they are different in Model and SVG
			if (ParameterValue === "strokeWidth"){
				VisualObject.setAttribute("stroke-width", NewValue);
			}else if (ParameterValue === "content"){
				this._setSvgText(VisualObject, NewValue, VisualObject.getAttribute("data-trimToLength"));
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
					this._interpreteHiddenChildrenElements(VisualObject);
				}
			}else if (ParameterValue === "rotate"){
				if(!isNumeric(NewValue)){
					//ignore writing wrong value
						return false;
					}
				this._setXYRotate(VisualObject, null, null, NewValue);
			}else if (ParameterValue === "transform"){
				//this is a best efford approach. Not garanteed to work, as a rotation will overwrite this! "scale(0.2)" as an example
				
				//svg are not transformable, so the rotation/transform is in the objects parent
				if (VisualObject.tagName === "svg" && VisualObject.parentNode.tagName === "g" && VisualObject.parentNode.id === ""){
					//object has already an g parent
					var rotationObject = VisualObject.parentNode;
				}else if (VisualObject.tagName === "svg" && VisualObject.parentNode.tagName !== "g"){
					//element has to be shifted into an g element
					rotationObject = HMI.svgDocument.createElementNS(HMI.HMI_Constants.NAMESPACE_SVG, 'g');
					rotationObject.setAttribute("overflow", "visible");
					rotationObject.setAttribute("x", "0");
					rotationObject.setAttribute("y", "0");
					VisualObject.parentNode.replaceChild(rotationObject, VisualObject);
					rotationObject.appendChild(VisualObject);
				}else{
					//normal visual element
					rotationObject = VisualObject;
				}
				rotationObject.setAttribute("transform", NewValue);
			}else if (ParameterValue === "absolutex"){
				var relativeX = 0;
				if (this.ResourceList.EventInfos.mouseRelativePosition !== null){
					//if we are after an move, we want to set a different x
					relativeX = this.ResourceList.EventInfos.mouseRelativePosition[0];
				}
				if(VisualObject.parentNode !== null && VisualObject.parentNode.namespaceURI == HMI.HMI_Constants.NAMESPACE_SVG){
					//absolutex is calculated from the offset of the parentNode
					this._setXYRotate(VisualObject, NewValue - parseFloat(VisualObject.parentNode.getAttribute("absolutex")) - relativeX, null, null);
					VisualObject.setAttribute("absolutex", NewValue - relativeX);
				}
				//we want to have offset parameter on all visual elements
				HMI.saveAbsolutePosition(VisualObject);
			}else if (ParameterValue === "absolutey"){
				var relativeY = 0;
				if (this.ResourceList.EventInfos.mouseRelativePosition !== null){
					//if we are after an move, we want to set a different y
					relativeY = this.ResourceList.EventInfos.mouseRelativePosition[1];
				}
				if(VisualObject.parentNode !== null && VisualObject.parentNode.namespaceURI == HMI.HMI_Constants.NAMESPACE_SVG){
					//absolutey is calculated from the offset of the parentNode
					this._setXYRotate(VisualObject, null, NewValue - parseFloat(VisualObject.parentNode.getAttribute("absolutey")) - relativeY, null);
					VisualObject.setAttribute("absolutey", NewValue - relativeY);
				}
				//we want to have offset parameter on all visual elements
				HMI.saveAbsolutePosition(VisualObject);
			}else if (ParameterValue === "absoluterotate"){
				if(VisualObject.parentNode !== null && VisualObject.parentNode.namespaceURI == HMI.HMI_Constants.NAMESPACE_SVG){
					//absoluterotate is calculated from the offset of the parentNode

					if (VisualObject.tagName === "svg" && VisualObject.parentNode.tagName === "g" && VisualObject.parentNode.id === ""){
						//object has already an g parent
						var rotationObject = VisualObject.parentNode;
					}else{
						rotationObject = VisualObject;
					}
					var newrotate = NewValue - getRotationFromObject(rotationObject.parentNode);
					this._setXYRotate(VisualObject, null, null, newrotate);
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
					if (HMI.instanceOf(VisualObject, this.cshmiBlackboxClass)) {
						var secondchild = VisualObject.firstChild.firstChild;
						secondchild.setAttribute(ParameterValue, NewValue);
					}
				}
				//reposition absolutex/y if x, y, x1, y1, cx or cy was changed
				if (ParameterValue === "x" || ParameterValue === "y" || ParameterValue === "cx" || ParameterValue === "cy" || ParameterValue === "x1" || ParameterValue === "y1"){
					HMI.saveAbsolutePosition(VisualObject);
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
					this._interpreteAction(EventObjItem["VisualObject"], EventObjItem["ObjectPath"], null, null);
					i++;
				}
			}
			var blackboxes = csHMIgetElementsByClassName(null, this.cshmiBlackboxClass);
			
			for (var i = 0; i < blackboxes.length; ++i){	
				//get string of "jsOnglobalvarchanged"
				var blackboxObjectpath = blackboxes[i].getAttribute("data-ModelSource");
				var requestList = new Object();
				
				requestList[blackboxObjectpath] = this.ResourceList.Elements[blackboxObjectpath].Parameters;
				if(requestList[blackboxObjectpath]["jsOnglobalvarchanged"] !== ""){
					this._executeScript(VisualObject, blackboxObjectpath, requestList[blackboxObjectpath]["jsOnglobalvarchanged"], "globalVar");
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
					this._interpreteAction(EventObjItem["VisualObject"], EventObjItem["ObjectPath"], null, null);
					i++;
				}
			}
			var blackboxes = csHMIgetElementsByClassName(null, this.cshmiBlackboxClass);
			
			for (var i = 0; i < blackboxes.length; ++i){	
				//get string of "jsOnglobalvarchanged"
				var blackboxObjectpath = blackboxes[i].getAttribute("data-ModelSource");
				var requestList = new Object();
				
				requestList[blackboxObjectpath] = this.ResourceList.Elements[blackboxObjectpath].Parameters;
				if(requestList[blackboxObjectpath]["jsOnglobalvarchanged"] !== ""){
					this._executeScript(VisualObject, blackboxObjectpath, requestList[blackboxObjectpath]["jsOnglobalvarchanged"], "persistentGlobalVar");
				}
			}
			return true;
		}else if (ParameterName === "TemplateFBReferenceVariable"){
			var result;
			var FBRef = this._getFBReference(VisualObject, ChildrenIterator);
			if (FBRef.path === ""){
				return false;
			}else if (ParameterValue === "fullqualifiedname" || ParameterValue === "CSHMIfullqualifiedname"){
				var TemplateObject = FBRef.object;
				if (TemplateObject === null){
					return false;
				}
				if(TemplateObject.ResourceList && TemplateObject.ResourceList.FBReference){
					if(NewValue === ""){
						HMI.hmi_log_info_onwebsite(ObjectPath+": Tried to set FBReference to the empty string. Aborted.");
						return false;
					}
					TemplateObject.ResourceList.FBReference = NewValue;
					TemplateObject.id = NewValue;
					if(TemplateObject.parentNode === HMI.Playground){
						HMI.updateDeepLink("&FBReference="+NewValue);
					}
					return true;
				}
				return false;
			}else if(ParameterValue === "identifier"){
				result = HMI.KSClient.renameObject(FBRef.path, NewValue, null, SetVarCbfnc, true);
			}else{
				var path;
				if(ParameterValue.charAt(0) !== "/"){
					path = FBRef.path+"."+ParameterValue;
				}else{
					path = FBRef.path+ParameterValue;
				}
				
				result = HMI.KSClient.setVar(path, NewValue, null, SetVarCbfnc, true);
			}
			if (result !== true){
				return false;
			}
			return true;
		}else if (ParameterName === "TemplateFBVariableReferenceName"){
			//TemplateFBReferenceVariable
			var TemplateObject = VisualObject;
			do{
				if(TemplateObject.ResourceList && TemplateObject.ResourceList.FBVariableReference && TemplateObject.ResourceList.FBVariableReference[ParameterValue] !== undefined){
					//a FBVariableReferenceName was requested
					if (TemplateObject.ResourceList.FBVariableReference[ParameterValue].charAt(0) === "/"){
						//String begins with / so it is a fullpath
						var path = TemplateObject.ResourceList.FBVariableReference[ParameterValue];
						result = HMI.KSClient.setVar(path, NewValue, null, SetVarCbfnc, true);
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
			//todo document setting ConfigValue
			
			var TemplateObject = VisualObject;
			
			//a target to use if no config value is found
			var BackupTarget = null;
			do{
				if(TemplateObject.ResourceList && TemplateObject.ResourceList.ConfigValues && TemplateObject.ResourceList.ConfigValues[ParameterValue] !== undefined){
					//we found a configValue in a parent object with the same name => overwrite it
					TemplateObject.ResourceList.ConfigValues[ParameterValue] = NewValue;
					return true;
				}
				
				if(TemplateObject.ResourceList && TemplateObject.ResourceList.FBReference){
					if(BackupTarget === null){
						//we have the first hit, remember
						BackupTarget = TemplateObject;
					}
				}
				if(TemplateObject.ResourceList && TemplateObject.ResourceList.FBVariableReference){
					for (var item in TemplateObject.ResourceList.FBVariableReference){
						if(typeof item == "string" && typeof TemplateObject.ResourceList.FBVariableReference[item] == "string"){
							if(BackupTarget === null){
								//we have the first hit, remember
								BackupTarget = TemplateObject;
							}
						}
					};
				}
			//loop upwards to find the Template object
			}while( (TemplateObject = TemplateObject.parentNode) && TemplateObject !== null && TemplateObject.namespaceURI == HMI.HMI_Constants.NAMESPACE_SVG);  //the = is no typo here!
			
			//we did not found a configValue with this name => save on first template with FBRef or FBVarRef
			if(BackupTarget !== null && BackupTarget.ResourceList && BackupTarget.ResourceList.ConfigValues){
				BackupTarget.ResourceList.ConfigValues[ParameterValue] = NewValue;
				return true;
			}else if(HMI.Playground.firstChild && HMI.Playground.firstChild.ResourceList && HMI.Playground.firstChild.ResourceList.ConfigValues){
				HMI.Playground.firstChild.ResourceList.ConfigValues[ParameterValue] = NewValue;
				return true;
			}
			
			//should not happen, since the firstChild of Playground has an empty ConfigValue
			return false;
		}else if (ignoreError === true){
			return true;
		}
		HMI.hmi_log_info_onwebsite('SetValue '+ObjectPath+' not configured.');
		return false;
	},
	
	/**
	 * rename an object
	 * @param {SVGElement} VisualObject Object to manipulate the visualisation
	 * @param {String} ObjectPath Path to this cshmi object containing the event/action/visualisation
	 * @param ChildrenIterator of the iterator or null
	 * @return false on error, true on success
	 */
	_interpreteRenameObject: function(VisualObject, ObjectPath, ChildrenIterator){
		//via getValue-part of RenameObject object
		var OldName = this._getValue(VisualObject, ObjectPath+".OldName", null, null, ChildrenIterator, true);
		var NewName = this._getValue(VisualObject, ObjectPath+".NewName", null, null, ChildrenIterator, true);
		
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
		
		/* changes in the structure should be blocking or at least no other communication should be performed
		var RenameCbfnc = function(Client, req){
			if(req.status !== 200){
				var response = HMI.KSClient.unescapeString(req.responseText);
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
		*/
		
		var result = HMI.KSClient.renameObject(OldName, NewName);
		
		if (result === false){
			return false;
		}
		return true;
	},
	
	/**
	 * creates an object
	 * @param {SVGElement} VisualObject Object to manipulate the visualisation
	 * @param {String} ObjectPath Path to this cshmi object containing the event/action/visualisation
	 * @param ChildrenIterator of the iterator or null
	 * @return false on error, true on success
	 */
	_interpreteCreateObject: function(VisualObject, ObjectPath, ChildrenIterator){
		var requestList = new Object();
		if (this.ResourceList.Actions[ObjectPath] !== undefined){
			requestList[ObjectPath] = this.ResourceList.Actions[ObjectPath].Parameters;
		}else{
			requestList[ObjectPath] = new Object();
			requestList[ObjectPath]["autoRenameIfExists"] = null;
			var successCode = this._requestVariablesArray(requestList);
			if (successCode === false){
				return null;
			}
			
			//we have asked the object successful, so remember the result
			this.ResourceList.Actions[ObjectPath] = new Object();
			this.ResourceList.Actions[ObjectPath].Parameters = requestList[ObjectPath];
		}
		
		var autoRenameIfExists = requestList[ObjectPath]["autoRenameIfExists"];
		
		//via getValue-part of CreateObject object
		var targetName		= this._getValue(VisualObject, ObjectPath+".Name", null, null, ChildrenIterator, true);
		var targetPlace		= this._getValue(VisualObject, ObjectPath+".Place", null, null, ChildrenIterator, true);
		var targetLibrary	= this._getValue(VisualObject, ObjectPath+".Library", null, null, ChildrenIterator, true);
		var targetClass		= this._getValue(VisualObject, ObjectPath+".Class", null, null, ChildrenIterator, true);
		
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
		
		/* changes in the structure should be blocking or at least no other communication should be performed
		var CreateObjectCbfnc = function(Client, req){
			if(req.status !== 200){
				var response = HMI.KSClient.unescapeString(req.responseText);
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
		*/
		
		var result = HMI.KSClient.createObject(targetPlace+"/"+targetName, targetLibrary+"/"+targetClass);
		
		var originalTargetName = targetName;
		var trycount = 1;
		while(autoRenameIfExists === "TRUE" && trycount < 10 && result.indexOf("KS_ERR_ALREADYEXISTS") !== -1){
			targetName = originalTargetName + Math.floor(Math.random() * 10000).toString();
			result = HMI.KSClient.createObject(targetPlace+"/"+targetName, targetLibrary+"/"+targetClass);
			trycount++;
		}
		if (result === false){
			return false;
		}
		return true;
	},
	
	/**
	 * deletes an object
	 * @param {SVGElement} VisualObject Object to manipulate the visualisation
	 * @param {String} ObjectPath Path to this cshmi object containing the event/action/visualisation
	 * @param ChildrenIterator of the iterator or null
	 * @return false on error, true on success
	 */
	_interpreteDeleteObject: function(VisualObject, ObjectPath, ChildrenIterator){
		//via getValue-part of DeleteObject object
		var targetName = this._getValue(VisualObject, ObjectPath+".Path", null, null, ChildrenIterator, true);
		
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
		
		/* changes in the structure should be blocking or at least no other communication should be performed
		var DeleteObjectCbfnc = function(Client, req){
			if(req.status !== 200){
				var response = HMI.KSClient.unescapeString(req.responseText);
				if (response.indexOf("KS_ERR_NOACCESS") !== -1){
					HMI.hmi_log_onwebsite('Deleting "'+targetName+'" not successfull. Operation not allowed.');
				}else if (response.indexOf("KS_ERR_BADPATH") !== -1){
					HMI.hmi_log_onwebsite('Deleting "'+targetName+'" not successfull. Object not found.');
				}else if (response.indexOf("KS_ERR") !== -1){
					HMI.hmi_log_info('Deleting '+targetName+' not successfull: '+response+' (configured here: '+ObjectPath+').');
				}
			}
		};
		*/
		
		var result = HMI.KSClient.deleteObject(targetName);
		
		if (result === false){
			return false;
		}
		return true;
	},
	
	/**
	 * links two objects
	 * @param {SVGElement} VisualObject Object to manipulate the visualisation
	 * @param {String} ObjectPath Path to this cshmi object containing the event/action/visualisation
	 * @param ChildrenIterator of the iterator or null
	 * @return false on error, true on success
	 */
	_interpreteLinkObjects: function(VisualObject, ObjectPath, ChildrenIterator){
		//via getValue-part of LinkObjects object
		var ObjectA		= this._getValue(VisualObject, ObjectPath+".ObjectA", null, null, ChildrenIterator, true);
		var ObjectB		= this._getValue(VisualObject, ObjectPath+".ObjectB", null, null, ChildrenIterator, true);
		var PortNameA	= this._getValue(VisualObject, ObjectPath+".Association", null, null, ChildrenIterator, true);
		
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
		
		/* changes in the structure should be blocking or at least no other communication should be performed
		var LinkObjectsCbfnc = function(Client, req){
			if(req.status !== 200){
				var response = HMI.KSClient.unescapeString(req.responseText);
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
		*/
		
		var result = HMI.KSClient.linkObjects(ObjectA, ObjectB, PortNameA);
		
		if (result === false){
			return false;
		}
		return true;
	},
	
	/**
	 * unlinks two objects
	 * @param {SVGElement} VisualObject Object to manipulate the visualisation
	 * @param {String} ObjectPath Path to this cshmi object containing the event/action/visualisation
	 * @param ChildrenIterator of the iterator or null
	 * @return false on error, true on success
	 */
	_interpreteUnlinkObjects: function(VisualObject, ObjectPath, ChildrenIterator){
		//via getValue-part of UnlinkObjects object
		var ObjectA		= this._getValue(VisualObject, ObjectPath+".ObjectA", null, null, ChildrenIterator, true);
		var ObjectB		= this._getValue(VisualObject, ObjectPath+".ObjectB", null, null, ChildrenIterator, true);
		var PortNameA	= this._getValue(VisualObject, ObjectPath+".Association", null, null, ChildrenIterator, true);
		
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
		
		/* changes in the structure should be blocking or at least no other communication should be performed
		var UnlinkObjectsCbfnc = function(Client, req){
			if(req.status !== 200){
				var response = HMI.KSClient.unescapeString(req.responseText);
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
		*/
		
		var result = HMI.KSClient.unlinkObjects(ObjectA, ObjectB, PortNameA);
		
		if (result === false){
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
	 */
	_generateFullKsPath: function(VisualObject, ObjectPath, PartialPath){
		var returnValue = "";
		if (PartialPath.charAt(0) !== "/"){
			//if the path is not a full path, search for the baseKsPath on the ObjectPath (perhaps in a TemplateDefinition?)
			returnValue = this._iterateBaseKsPathUp(ObjectPath)+PartialPath;
		}else{
			returnValue = PartialPath;
		}
		
		if (returnValue.indexOf("CSHMIModelHost") === 0){
			//CSHMIModelHost in the model should be replaced
			return returnValue.replace("CSHMIModelHost", "//"+HMI.KSClient.ResourceList.ModelHost);
		}else if (returnValue.charAt(0) === "/" && returnValue.charAt(1) === "/"){
			return returnValue;
		}else{
			//we want to have a full path with host and server
			
			//default to the ModelHost
			var serverName = "//" + HMI.KSClient.ResourceList.ModelHost + "/" + HMI.KSClient.ResourceList.ModelServer;
			
			//this Path has not host+server, so try to search for this via an parent Template
			if(VisualObject.parentNode !== null){
				//VisualObject is in DOM and configured
				var TemplateObject = VisualObject;
			}else if(VisualObject.VisualParentObject === null){
				//VisualObject is not in DOM and first object and we are configuring it right now (it has per definition no FBRef till now)
				TemplateObject = VisualObject;
			}else if(VisualObject.VisualParentObject !== undefined){
				//VisualObject is not in DOM and we are configuring it right now (it has per definition no FBRef till now)
				TemplateObject = VisualObject.VisualParentObject;
			}else{
				//VisualObject is not in DOM and no Template. make the do-while abort quick
				TemplateObject = VisualObject;
			}
			
			do{
				//test if we have reached a Template
				if (HMI.instanceOf(TemplateObject, this.cshmiTemplateClass)){
					//check if the Template can resolve us the path via its BaseKsPath
					baseKsPathTemplate = this._iterateBaseKsPathUp(TemplateObject.getAttribute("data-ModelSource"));
					if(baseKsPathTemplate !== ""){
						if(baseKsPathTemplate.charAt(0) === "/" && baseKsPathTemplate.charAt(1) === "/"){
							//we got an result, which gave us a full path
							return baseKsPathTemplate + returnValue;
						}else if(baseKsPathTemplate.charAt(0) === "/"){
							//we got an result, which gave us a full path without an servername, prefix ModelServer
							return serverName + baseKsPathTemplate + returnValue;
						}
					}
				}
				if(TemplateObject.ResourceList && TemplateObject.ResourceList.FBReference){
					//we want to find the host and server from the parent FBreference
					if (TemplateObject.ResourceList.FBReference.indexOf("//") === 0){
						var FBRef = TemplateObject.ResourceList.FBReference;
						
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
			
			if(returnValue.charAt(0) === "/"){
				returnValue = serverName + returnValue;
			}else{
				returnValue = serverName + "/" + returnValue;
			}
		}
		return returnValue;
	},
	_iterateBaseKsPathUp: function(ObjectPath){
		var returnValue = "";
		ObjectPathArray = ObjectPath.split("/");
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
		return returnValue;
	},
	
	/**
	 * calls conditions below the if PART and triggers actions in then or else PART
	 * @param {SVGElement} VisualObject Object to manipulate the visualisation
	 * @param {String} ObjectPath Path to this cshmi object containing the event/action/visualisation
	 * @param CyctimeObject object for fetching values as a bunch or null
	 * @param ChildrenIterator of the iterator or null
	 * @return {Boolean} false if an error occured, returnValue of the called actions
	 */
	_interpreteIfThenElse: function(VisualObject, ObjectPath, CyctimeObject, ChildrenIterator){
		var anyCond;
		//get the information if the config of the object is known through the turbo
		if (this.ResourceList.Actions[ObjectPath] !== undefined){
			anyCond = this.ResourceList.Actions[ObjectPath].IfThenElseParameterAnycond;
		}else{
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
		}
		var responseArray = HMI.KSClient.getChildObjArray(ObjectPath+".if", this);
		
		//newwrite
		//fetch config from all childrens via this.ResourceList.ModellVariables.*
		
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
				this.cshmiObject._interpreteAction(VisualObject, ObjectPath+".then", CyctimeObject, ChildrenIterator);
				return true;
			}else if (ConditionMatched === false){
				this.cshmiObject._interpreteAction(VisualObject, ObjectPath+".else", CyctimeObject, ChildrenIterator);
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
				ConditionMatched = this._checkCondition(VisualObject, ObjectPath+".if/"+varName[0], true, IfThenElseObserver, CyctimeObject, ChildrenIterator);
			}else if (varName[1].indexOf("/cshmi/Compare") !== -1){
				ConditionMatched = this._checkCondition(VisualObject, ObjectPath+".if/"+varName[0], false, IfThenElseObserver, CyctimeObject, ChildrenIterator);
			}else if (varName[1].indexOf("/cshmi/Confirm") !== -1){
				ConditionMatched = this._checkConfirm(VisualObject, ObjectPath+".if/"+varName[0], IfThenElseObserver);
			}
			if(ConditionMatched === undefined){
				//the checkCondition is handled via a callback or in an later loop
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
	 * @param IfThenElseObserver
	 * @param CyctimeObject object for fetching values as a bunch or null
	 * @param ChildrenIterator of the iterator or null
	 * @return {Boolean} true if condition matched, false if not matched, null on error, undefined if callback is in charge
	 */
	_checkCondition: function(VisualObject, ObjectPath, CompareIteratedChild, IfThenElseObserver, CyctimeObject, ChildrenIterator){
		if (CompareIteratedChild === true && !ChildrenIterator){
			HMI.hmi_log_info_onwebsite("CompareIteratedChild "+ObjectPath+" is not placed under a Iterator");
			//error state, so no boolean
			return null;
		}
		
		var requestList = new Object();
		if (this.ResourceList.Actions[ObjectPath] !== undefined){
			requestList[ObjectPath] = this.ResourceList.Actions[ObjectPath].Parameters;
		}else{
			requestList[ObjectPath] = new Object();
			requestList[ObjectPath]["ignoreError"] = null;
			requestList[ObjectPath]["comptype"] = null;
			requestList[ObjectPath]["ignoreCase"] = null;
			if (CompareIteratedChild === true){
				requestList[ObjectPath]["childValue"] = null;
			}
			var successCode = this._requestVariablesArray(requestList);
			if (successCode === false){
				return null;
			}
			
			//we have asked the object successful, so remember the result
			this.ResourceList.Actions[ObjectPath] = new Object();
			this.ResourceList.Actions[ObjectPath].Parameters = requestList[ObjectPath];
		}
		if(requestList[ObjectPath]["ignoreCase"] === undefined){
			//old cshmi servers with Turbo
			requestList[ObjectPath]["ignoreCase"] = "FALSE";
		}
		
		var ignoreError = requestList[ObjectPath]["ignoreError"];
		var comptype = requestList[ObjectPath]["comptype"];
		var ignoreCase = requestList[ObjectPath]["ignoreCase"];
		var childValue;
		var Value1;
		var Value2;
		
		var checkConditionObserver = new cshmiObserver(VisualObject, ObjectPath, 2, this);
		checkConditionObserver.triggerActivity = function(){
			this.cshmiObject._checkConditionResult(this.VisualObject, this.ObjectPath, IfThenElseObserver, checkConditionObserver);
		};
		checkConditionObserver.customInformation = {"ignoreError":ignoreError, "comptype":comptype, "ignoreCase":ignoreCase};
		
		var thisObserverEntry;
		if (CompareIteratedChild === true){
			childValue = HMI.KSClient.splitKsResponse(requestList[ObjectPath]["childValue"], 0);
			
			if (childValue[0] === ""){
				HMI.hmi_log_info_onwebsite("CompareIteratedChild "+ObjectPath+" is not configured");
				//error state, so no boolean
				return null;
			}else if(ChildrenIterator.currentChild[childValue[0]] === undefined){
				var errormessage = "CompareIteratedChild "+ObjectPath+".childValue is wrong configured. The requested iterator does not provide this information";
				if(childValue[0].indexOf("OP_") !== -1){
					errormessage += ": " + childValue[0]+" is not available on variables and associations.";
				}
				HMI.hmi_log_info_onwebsite(errormessage);
				//error state, so no boolean
				return null;
			}
			thisObserverEntry = new ObserverEntry("currentChild");
			//this is know right know, so fill it direct
			thisObserverEntry.value = ChildrenIterator.currentChild[childValue[0]];
			thisObserverEntry.requirementsFulfilled = true;
			checkConditionObserver.ObserverEntryArray[0] = thisObserverEntry;
			
			thisObserverEntry = new ObserverEntry("withValue", ".");
			checkConditionObserver.ObserverEntryArray[1] = thisObserverEntry;
			Value2 = this._getValue(VisualObject, ObjectPath+".withValue", checkConditionObserver, CyctimeObject, ChildrenIterator);
			if(Value2 === true){
				//the checkcondition is handled via a callback
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
			Value1 = this._getValue(VisualObject, ObjectPath+".value1", checkConditionObserver, CyctimeObject, ChildrenIterator);
			if(Value1 === true){
				//the checkcondition is handled via a callback
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
			Value2 = this._getValue(VisualObject, ObjectPath+".value2", checkConditionObserver, CyctimeObject, ChildrenIterator);
			if(Value2 === true){
				//the checkcondition is handled via a callback
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
		var ignoreCase = checkConditionObserver.customInformation.ignoreCase;
		
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
		}else if(ignoreCase === "TRUE"){
			Value1 = Value1.toLowerCase();
		}
		if (Value2 === ""){
			Value2 = [""];
		}else{
			//todo doku mehrfache Abfragen von value2
			Value2 = HMI.KSClient.splitKsResponse(Value2, 0);
			
			for (var i=0; i<Value2.length; i++){
				if(isNumeric(Value2[i])){
					Value2[i] = parseFloat(Value2[i]);
				}else if(ignoreCase === "TRUE"){
					Value2[i] = Value2[i].toLowerCase();
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
			IfThenElseObserver.updateValueInArray(ObjectPath, true);
			IfThenElseObserver.checkAndTrigger();
			return;
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
		question = this._getValue(VisualObject, ObjectPath+".question", checkConfirmObserver, null, null, null, true);
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
	_interpreteChildrenIterator: function(VisualObject, ObjectPath, ParentChildrenIterator){
		var FBRef = this._getFBReference(VisualObject, ParentChildrenIterator);
		if (FBRef.path === ""){
			HMI.hmi_log_info_onwebsite('ChildrenIterator '+ObjectPath+' could not work. Found no FBReference on a parent.');
			return false;
		}
		
		var ChildrenType;
		//get the information if the config of the object is known through the turbo
		if (this.ResourceList.Actions[ObjectPath] !== undefined){
			ChildrenType = this.ResourceList.Actions[ObjectPath].ChildrenIteratorParameterChildrenType;
		}else{
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
		}
		if (ChildrenType === ""){
			HMI.hmi_log_info_onwebsite("ChildrenIterator "+ObjectPath+" is not configured.");
			return false;
		}
		
		var ChildrenIterator = new Object();
		ChildrenIterator.currentChild = null;
		ChildrenIterator.currentCount = new Object();
		
		//the callbacks calls the actions of this iterator
		var GetEPIteratorCbfnc = function(Client, req){
			if(req.status === 200){
				//remember an old currentChild (for cascaded iterators)
				/*
				var savedCurrentChild = HMI.cshmi.ResourceList.ChildrenIterator.currentChild;
				delete HMI.cshmi.ResourceList.ChildrenIterator.currentChild;
				HMI.cshmi.ResourceList.ChildrenIterator.currentCount = new Object();
				*/
				
				var response = HMI.KSClient.unescapeString(req.responseText);
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
					//doku possible values
					ChildrenIterator.currentChild = responseDictionary;
					
					//get and execute all actions
					HMI.cshmi._interpreteAction(VisualObject, ObjectPath + ".forEachChild", null, ChildrenIterator);
				}
			}
		};
		var GetVarIteratorCbfnc = function(Client, req){
			if(req.status === 200){
				//remember an old currentChild (for cascaded iterators)
				/*
				var savedCurrentChild = HMI.cshmi.ResourceList.ChildrenIterator.currentChild;
				delete HMI.cshmi.ResourceList.ChildrenIterator.currentChild;
				*/
				
				//get a rid of external brackets 
				var response = req.responseText.replace(/{/g, "");
				response = response.replace(/}/g, "");
				var responseArray = HMI.KSClient.splitKsResponse(response, 1);
				for (var i=0; i<responseArray.length; i++){
					var responseDictionary = Array();
					responseDictionary["OP_VALUE"] = responseArray[i];
					responseDictionary["OP_NAME"] = responseArray[i];
					
					//doku value and name option
					ChildrenIterator.currentChild = responseDictionary;
					
					//get and execute all actions
					HMI.cshmi._interpreteAction(VisualObject, ObjectPath + ".forEachChild", null, ChildrenIterator);
				}
			}
		};
		
		var async = true;
		var returnValue = true;
		if (ChildrenType.indexOf("OT_") !== -1){
			//GetEP requested
			var response = HMI.KSClient.getEP(encodeURI(FBRef.path), ChildrenType, "OP_ANY", GetEPIteratorCbfnc, async);
		}else{
			//GetVar on a (vector?)-value requested
			//doku multiple values possible
			
			//allow a list of variables as ChildrenTypes
			var ChildrenTypeList = ChildrenType.split(" ");
			var response;
			for (var i=0; i < ChildrenTypeList.length; i++) {
				if(ChildrenTypeList[i].charAt(0) !== "/"){
					var path = FBRef.path + "." + ChildrenTypeList[i];
				}else{
					//todo doku
					path = FBRef.path + ChildrenTypeList[i];
				}
				response = HMI.KSClient.getVar(path, "OP_VALUE", GetVarIteratorCbfnc, async);
			}
		}
		
		return true;
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
		if(VisualObject.tagName !== "polyline" && VisualObject.tagName !== "path"){
			HMI.hmi_log_info_onwebsite("RoutePolyline not supported with: "+VisualObject.tagName+"-Objects (path: "+ObjectPath+")");
			return false;
		}
		if(VisualObject.getAttribute("display") === "none"){
			//we are not visible, so do not route at all
			return false;
		}
		
		var SourceBasename = null;
		var SourceVariablename = null;
		var TargetBasename		= null;
		var TargetVariablename	= null;

		var SourceConnectionPoint = null;
		var SourceConnectionPointdirection = 0;
		var TargetConnectionPoint = null;
		var TargetConnectionPointdirection = 180;
		var OffsetSource = 0;
		var OffsetTarget = 0;
		
		//if the Object was routed earlier, get the cached information (could be the case with templates or repeated/cyclic calls to the same object)
		//Not the config is cached here, but the result! So caching VisualObject specific, not ObjectPath
		var reuseConnection = false;
		if(VisualObject.ResourceList === undefined){
			reuseConnection = false;
		}else if (VisualObject.ResourceList && VisualObject.ResourceList.RoutePolyline !== undefined){
			SourceConnectionPoint = VisualObject.ResourceList.RoutePolyline.SourceConnectionPoint;
			TargetConnectionPoint = VisualObject.ResourceList.RoutePolyline.TargetConnectionPoint;
			if (SourceConnectionPoint && SourceConnectionPoint.ownerDocument === this.trashDocument){
				//we had a connection to a deleted point, so search for it again
				reuseConnection = false;
			}else if (TargetConnectionPoint && TargetConnectionPoint.ownerDocument === this.trashDocument){
				//we had a connection to a deleted point, so search for it again
				reuseConnection = false;
			}else if(!SourceConnectionPoint || !TargetConnectionPoint){
				//uncomplete caching
				reuseConnection = false;
				SourceBasename = VisualObject.ResourceList.RoutePolyline.SourceBasename;
				SourceVariablename = VisualObject.ResourceList.RoutePolyline.SourceVariablename;
				TargetBasename = VisualObject.ResourceList.RoutePolyline.TargetBasename;
				TargetVariablename = VisualObject.ResourceList.RoutePolyline.TargetVariablename;
				SourceConnectionPoint = null;
				TargetConnectionPoint = null;
			}else{
				reuseConnection = true;
			}
		}
		if(reuseConnection == true){
			SourceConnectionPoint = VisualObject.ResourceList.RoutePolyline.SourceConnectionPoint;
			SourceConnectionPointdirection = VisualObject.ResourceList.RoutePolyline.SourceConnectionPointdirection;
			TargetConnectionPoint = VisualObject.ResourceList.RoutePolyline.TargetConnectionPoint;
			TargetConnectionPointdirection = VisualObject.ResourceList.RoutePolyline.TargetConnectionPointdirection;
			OffsetSource = VisualObject.ResourceList.RoutePolyline.OffsetSource;
			OffsetTarget = VisualObject.ResourceList.RoutePolyline.OffsetTarget;
		}else{
			var requestList = new Object();
			//This caching is model config specific, not instance specific
			if (this.ResourceList.Actions[ObjectPath] !== undefined){
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
			
			if( SourceBasename == null || SourceVariablename == null || TargetBasename == null || TargetVariablename == null){
				//get Values (via getValue-parts)
				SourceBasename		= this._getValue(VisualObject, ObjectPath+".SourceBasename");
				SourceVariablename	= this._getValue(VisualObject, ObjectPath+".SourceVariablename");
				TargetBasename		= this._getValue(VisualObject, ObjectPath+".TargetBasename");
				TargetVariablename	= this._getValue(VisualObject, ObjectPath+".TargetVariablename");
				
				if (SourceBasename === false || SourceVariablename === false || TargetBasename === false || TargetVariablename === false){
					//this error will never be fixed, so prevent this routing forever. But how???
					return false;
				}
				if (SourceBasename === null || SourceVariablename === null || TargetBasename === null || TargetVariablename === null){
					//we are not visible right now... skip processing
					return false;
				}
			}
			
			var FBRef = this._getFBReference(VisualObject, null);
			
			//in DOM every object has a full server+Host name, so if the basename does not provide it, 
			//we have to add it
			// e.g "//dev/ov_hmidemo7/TechUnits/TU10/h_bkqwmtbbhpf"" --> use prefix "//dev/ov_hmidemo7"
			var prefix = "";
			if (FBRef.path !== ""){
				//find the 3rd "/"
				var slashIndexAfterHost = FBRef.path.indexOf("/", 2);
				//find the 4th "/"
				var slashIndexAfterServer = FBRef.path.indexOf("/", slashIndexAfterHost+1);
				//only keep the String before 4th "/"
				prefix = FBRef.path.slice(0, slashIndexAfterServer);
			}
			
			if(SourceBasename.charAt(0) === "/" && SourceBasename.charAt(1) !== "/"){
				//Basename has no prefix (perhaps from an association)
				SourceBasename = prefix + SourceBasename;
			}
			if(TargetBasename.charAt(0) === "/" && TargetBasename.charAt(1) !== "/"){
				//Basename has no prefix (perhaps from an association)
				TargetBasename = prefix + TargetBasename;
			}
			
			if (SourceBasename === ""){
				var SourceName = "";
			}else if (SourceVariablename !== ""){
				//in the svg DOM separators is always /
				SourceName = SourceBasename + "/" + SourceVariablename;
			}else{
				SourceName = SourceBasename;
			}
			if (TargetBasename === ""){
				var TargetName = "";
			}else if (TargetVariablename !== ""){
				//in the svg separators is always /
				TargetName = TargetBasename + "/" + TargetVariablename;
			}else{
				TargetName = TargetBasename;
			}
			var Source;
			var Target;
			
			if(SourceName === ""){
				//skip
			}else if (null !== (Source = HMI.svgDocument.getElementById(SourceName))){
				//todo doku moeglichkeit zur direkten SourceConnectionPoint angabe (inkl richtung und default)
				
				if (Source.tagName === "circle"){
					SourceConnectionPoint = Source;
				}else{
					for(var i = 0; i < Source.childNodes.length;i++){
						// search tagName "circle" with name containing ConnectionPoint
						if (Source.childNodes[i].tagName === "circle" && Source.childNodes[i].id.indexOf("/ConnectionPoint") !== -1){
							SourceConnectionPoint = Source.childNodes[i];
							break;
						}
					}
				}
				if(SourceConnectionPoint !== null){
					if(SourceConnectionPoint.id.toLowerCase().substr(-4) === "left"){
						SourceConnectionPointdirection = 180;
					}else if(SourceConnectionPoint.id.toLowerCase().substr(-2) === "up"){
						SourceConnectionPointdirection = 270;
					}else if(SourceConnectionPoint.id.toLowerCase().substr(-5) === "right"){
						SourceConnectionPointdirection = 0;
					}else if(SourceConnectionPoint.id.toLowerCase().substr(-4) === "down"){
						SourceConnectionPointdirection = 90;
					}
				}
				Source = null;
			}else{
				//the SourceConnectionPoint is outside the domain
				//search a connectionpoint recursively in all parent svgs for connectionsources that are outside the domain 	
				
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
							if (domainSVG.childNodes[i].tagName === "circle" && domainSVG.childNodes[i].id.indexOf("/SourceConnectionPointOutsideDomain") !== -1){
								SourceConnectionPoint = domainSVG.childNodes[i];
								SourceConnectionPointdirection = SourceConnectionPoint.id.slice(SourceConnectionPoint.id.indexOf("SourceConnectionPointOutsideDomain")+34);
								break;
							}
						}
						domainSVG = domainSVG.parentNode;
					}
					if(isNumeric(SourceConnectionPointdirection)){
						//nothing
					}else if(SourceConnectionPointdirection.toLowerCase() === "left"){
						SourceConnectionPointdirection = 180;
					}else if(SourceConnectionPointdirection.toLowerCase() === "up"){
						SourceConnectionPointdirection = 270;
					}else if(SourceConnectionPointdirection.toLowerCase() === "right"){
						SourceConnectionPointdirection = 0;
					}else if(SourceConnectionPointdirection.toLowerCase() === "down"){
						SourceConnectionPointdirection = 90;
					}else{
						SourceConnectionPointdirection = 0;
					}
					//remember the result
					this.ResourceList.Actions["SourceConnectionPointOutsideDomain"] = new Object();
					this.ResourceList.Actions["SourceConnectionPointOutsideDomain"].SourceConnectionPoint = SourceConnectionPoint;
					this.ResourceList.Actions["SourceConnectionPointOutsideDomain"].SourceConnectionPointdirection = SourceConnectionPointdirection;
				}
			}
			
			if(TargetName === ""){
				//skip
			}else if (null !== (Target = HMI.svgDocument.getElementById(TargetName))){
				if (Target.tagName === "circle"){
					TargetConnectionPoint = Target;
				}else{
					for(var i = 0; i < Target.childNodes.length;i++){
						// search tagName "circle" with name containing ConnectionPoint
						if (Target.childNodes[i].tagName === "circle" && Target.childNodes[i].id.indexOf("/ConnectionPoint") !== -1){
							TargetConnectionPoint = Target.childNodes[i];
							break;
						}
					}
				}
				if (TargetConnectionPoint !== null){
					if(TargetConnectionPoint.id.toLowerCase().substr(-4) === "left"){
						TargetConnectionPointdirection = 180;
					}else if(TargetConnectionPoint.id.toLowerCase().substr(-2) === "up"){
						TargetConnectionPointdirection = 270;
					}else if(TargetConnectionPoint.id.toLowerCase().substr(-5) === "right"){
						TargetConnectionPointdirection = 0;
					}else if(TargetConnectionPoint.id.toLowerCase().substr(-4) === "down"){
						TargetConnectionPointdirection = 90;
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
							if (domainSVG.childNodes[i].tagName === "circle" && domainSVG.childNodes[i].id.indexOf("/TargetConnectionPointOutsideDomain") !== -1){
								TargetConnectionPoint = domainSVG.childNodes[i];
								TargetConnectionPointdirection = TargetConnectionPoint.id.slice(TargetConnectionPoint.id.indexOf("TargetConnectionPointOutsideDomain")+34);
								break;
							}
						}
						domainSVG = domainSVG.parentNode;
					}
					if(isNumeric(TargetConnectionPointdirection)){
						//nothing
					}else if(TargetConnectionPointdirection.toLowerCase() === "left"){
						TargetConnectionPointdirection = 180;
					}else if(TargetConnectionPointdirection.toLowerCase() === "up"){
						TargetConnectionPointdirection = 270;
					}else if(TargetConnectionPointdirection.toLowerCase() === "right"){
						TargetConnectionPointdirection = 0;
					}else if(TargetConnectionPointdirection.toLowerCase() === "down"){
						TargetConnectionPointdirection = 90;
					}else{
						TargetConnectionPointdirection = 180;
					}
					//remember the result
					this.ResourceList.Actions["TargetConnectionPointOutsideDomain"] = new Object();
					this.ResourceList.Actions["TargetConnectionPointOutsideDomain"].TargetConnectionPoint = TargetConnectionPoint;
					this.ResourceList.Actions["TargetConnectionPointOutsideDomain"].TargetConnectionPointdirection = TargetConnectionPointdirection;
				}
			}
			
			//Basename should be the real basename. This could be changed if there is a / in the variablename
			var PathArray = SourceName.split("/");
			PathArray.pop();
			SourceBasename = PathArray.join("/");
			PathArray = TargetName.split("/");
			PathArray.pop();
			TargetBasename = PathArray.join("/");
			PathArray = null;
			
			var SourceBase = null;
			if(SourceBasename){
				SourceBase = HMI.svgDocument.getElementById(SourceBasename);
			}
			var TargetBase = null;
			if(TargetBasename){
				TargetBase = HMI.svgDocument.getElementById(TargetBasename);
			}
			
			//get connection offsets
			if (SourceBase === null){
				//skip
			}else if (SourceBase.ResourceList && SourceBase.ResourceList.ConnectionFromCount !== undefined && TargetConnectionPoint !== null){
				//there is already an incoming connection for this Object
				SourceBase.ResourceList.ConnectionFromCount += 1;
				OffsetSource = SourceBase.ResourceList.ConnectionFromCount * parseFloat(requestList[ObjectPath]["gridWidth"]);
			}else if(TargetConnectionPoint !== null){
				//remember the result
				if (SourceBase.ResourceList === undefined){
					SourceBase.ResourceList = new Object();
				}
				SourceBase.ResourceList.ConnectionFromCount = 0;
				OffsetSource = 0;
			}
			if (TargetBase === null){
				//skip
			}else if (TargetBase.ResourceList && TargetBase.ResourceList.ConnectionFromCount !== undefined && SourceConnectionPoint !== null){
				//there is already an incoming connection for this Object
				TargetBase.ResourceList.ConnectionFromCount += 1;
				OffsetTarget = TargetBase.ResourceList.ConnectionFromCount * parseFloat(requestList[ObjectPath]["gridWidth"]);
			}else if(SourceConnectionPoint !== null){
				//remember the result
				if (TargetBase.ResourceList === undefined){
					TargetBase.ResourceList = new Object();
				}
				TargetBase.ResourceList.ConnectionFromCount = 0;
				OffsetTarget = 0;
			}
			
			// add minimum #pixel distance from Object till 1st direction change of connection
			OffsetSource += parseFloat(requestList[ObjectPath]["offset"]);
			OffsetTarget += parseFloat(requestList[ObjectPath]["offset"]);
			
			VisualObject.ResourceList = new Object();
			VisualObject.ResourceList.RoutePolyline = new Object();
			VisualObject.ResourceList.RoutePolyline.Coords = new Object();
			if(SourceConnectionPoint === null || TargetConnectionPoint === null){
				//one or both connected object is not loaded yet.
				VisualObject.ResourceList.RoutePolyline.SourceBasename = SourceName;
				VisualObject.ResourceList.RoutePolyline.SourceVariablename = SourceVariablename;
				VisualObject.ResourceList.RoutePolyline.TargetBasename = TargetBasename;
				VisualObject.ResourceList.RoutePolyline.TargetVariablename = TargetVariablename;
			}else{
				//remember the result
				VisualObject.ResourceList.RoutePolyline.SourceConnectionPoint = SourceConnectionPoint;
				VisualObject.ResourceList.RoutePolyline.SourceConnectionPointdirection = SourceConnectionPointdirection;
				VisualObject.ResourceList.RoutePolyline.TargetConnectionPoint = TargetConnectionPoint;
				VisualObject.ResourceList.RoutePolyline.TargetConnectionPointdirection = TargetConnectionPointdirection;
				VisualObject.ResourceList.RoutePolyline.OffsetSource = OffsetSource;
				VisualObject.ResourceList.RoutePolyline.OffsetTarget = OffsetTarget;
			}
		}
		
		if (SourceConnectionPoint === null || TargetConnectionPoint === null){
			//could not paint something
			return false;
		}
		var IteratorObj = null;
		var absoluterotate = 0;
		var StartX = 0;
		var StartY = 0;
		var rotateStart = 0;
		var EndX = 0;
		var EndY = 0;
		var rotateEnd = 0;
		if (SourceConnectionPoint !== null){
			IteratorObj = SourceConnectionPoint;
			while( (IteratorObj = IteratorObj.parentNode) && IteratorObj !== null && IteratorObj.namespaceURI == HMI.HMI_Constants.NAMESPACE_SVG){
				if(IteratorObj.getAttribute("display") === "none"){
					//connection object is invisible, so hide us, too.
					if(VisualObject.tagName === "polyline"){
						VisualObject.setAttribute("points", "");
					}else if(VisualObject.tagName === "path"){
						VisualObject.setAttribute("d", "");
					}
					return true;
				}
			}
			//find source rotation
			absoluterotate = parseInt(SourceConnectionPoint.getAttribute("absoluterotate"), 10) % 360;
			if(absoluterotate < 44){
				//nothing
			}else if(absoluterotate < 134){
				SourceConnectionPointdirection = (SourceConnectionPointdirection + 90) % 360;
			}else if(absoluterotate < 226){
				SourceConnectionPointdirection = (SourceConnectionPointdirection + 180) % 360;
			}else if(absoluterotate < 316){
				SourceConnectionPointdirection = (SourceConnectionPointdirection + 270) % 360;
			}else{
				//nothing
			}
			StartX = parseInt(SourceConnectionPoint.getAttribute("absolutex"), 10);
			StartY = parseInt(SourceConnectionPoint.getAttribute("absolutey"), 10);
			rotateStart = parseInt(SourceConnectionPoint.getAttribute("absoluterotate"), 10);
			if(TargetConnectionPoint === null){
				//paint a dummy line into the correct direction
				/*	disabled to paint no line at the fb engineering 
				VisualObject.ResourceList.RoutePolyline.savingDisabled = true;
				if(SourceConnectionPointdirection == 0){
					EndX = StartX + OffsetSource/2;		EndY = StartY;					TargetConnectionPointdirection = 180;
				}else if(SourceConnectionPointdirection == 90){
					EndX = StartX;						EndY = StartY + OffsetSource/2;	TargetConnectionPointdirection = 270;
				}else if(SourceConnectionPointdirection == 180){
					EndX = StartX - OffsetSource/2;		EndY = StartY;					TargetConnectionPointdirection = 0;
				}else if(SourceConnectionPointdirection == 270){
					EndX = StartX;						EndY = StartY - OffsetSource/2;	TargetConnectionPointdirection = 90;
				}
				*/
			}
		}
		if(TargetConnectionPoint !== null){
			IteratorObj = TargetConnectionPoint;
			while( (IteratorObj = IteratorObj.parentNode) && IteratorObj !== null && IteratorObj.namespaceURI == HMI.HMI_Constants.NAMESPACE_SVG){
				if(IteratorObj.getAttribute("display") === "none"){
					//connection object is invisible, so hide us, too.
					if(VisualObject.tagName === "polyline"){
						VisualObject.setAttribute("points", "");
					}else if(VisualObject.tagName === "path"){
						VisualObject.setAttribute("d", "");
					}
					return true;
				}
			}
			//find target rotation
			absoluterotate = parseInt(TargetConnectionPoint.getAttribute("absoluterotate"), 10) % 360;
			if(absoluterotate < 44){
				//nothing
			}else if(absoluterotate < 134){
				TargetConnectionPointdirection = (TargetConnectionPointdirection + 90) % 360;
			}else if(absoluterotate < 226){
				TargetConnectionPointdirection = (TargetConnectionPointdirection + 180) % 360;
			}else if(absoluterotate < 316){
				TargetConnectionPointdirection = (TargetConnectionPointdirection + 270) % 360;
			}else{
				//nothing
			}
			EndX = parseInt(TargetConnectionPoint.getAttribute("absolutex"), 10);
			EndY = parseInt(TargetConnectionPoint.getAttribute("absolutey"), 10);
			rotateEnd = parseInt(TargetConnectionPoint.getAttribute("absoluterotate"), 10);
			if(SourceConnectionPoint === null){
				//paint a dummy line into the correct direction
				/*	disabled to paint no line at the fb engineering
				VisualObject.ResourceList.RoutePolyline.savingDisabled = true;
				if(TargetConnectionPointdirection == 0){
					StartX = EndX + OffsetTarget/2;		StartY = EndY;					SourceConnectionPointdirection = 180;
				}else if(TargetConnectionPointdirection == 90){
					StartX = EndX;						StartY = EndY - OffsetTarget/2;	SourceConnectionPointdirection = 270;
				}else if(TargetConnectionPointdirection == 180){
					StartX = EndX - OffsetTarget/2;		StartY = EndY;					SourceConnectionPointdirection = 0;
				}else if(TargetConnectionPointdirection == 270){
					StartX = EndX;						StartY = EndY + OffsetTarget/2;	SourceConnectionPointdirection = 90;
				}
				*/
			}
		}
		absoluterotate = null;
		
		//perhaps we are not at global coordinate 0,0
		StartX = StartX - parseFloat(VisualObject.parentNode.getAttribute("absolutex"));
		EndX = EndX - parseFloat(VisualObject.parentNode.getAttribute("absolutex"));
		StartY = StartY - parseFloat(VisualObject.parentNode.getAttribute("absolutey"));
		EndY = EndY - parseFloat(VisualObject.parentNode.getAttribute("absolutey"));
		
		if(VisualObject.ResourceList.linesArray === undefined){
			var linesArray = VisualObject.ResourceList.linesArray = 
				[HMI.svgDocument.createElementNS(HMI.HMI_Constants.NAMESPACE_SVG, 'line'),
				 HMI.svgDocument.createElementNS(HMI.HMI_Constants.NAMESPACE_SVG, 'line'),
				 HMI.svgDocument.createElementNS(HMI.HMI_Constants.NAMESPACE_SVG, 'line')];
			linesArray[0].setAttribute("data-lineid", "1");
			linesArray[1].setAttribute("data-lineid", "2");
			linesArray[2].setAttribute("data-lineid", "3");
			linesArray[0].id = VisualObject.id + "*" + "first";
			linesArray[1].id = VisualObject.id + "*" + "second";
			linesArray[2].id = VisualObject.id + "*" + "third";
			
			//this can not be handled in the for loop, as the scope will always be the last line
			linesArray[0]._moveStartDragThunk = function(evt){ HMI.cshmi._moveStartDrag(linesArray[0], ObjectPath, evt); };
			linesArray[0]._moveMouseMoveThunk = function(evt){ HMI.cshmi._moveMouseMove(linesArray[0], ObjectPath, evt); };
			linesArray[0]._moveStopDragThunk = function(evt){ HMI.cshmi._moveStopDrag(linesArray[0], ObjectPath, evt, null); };
			linesArray[0]._moveCancelDragThunk = function(evt){ HMI.cshmi._moveStopDrag(linesArray[0], ObjectPath, evt, true); };
			linesArray[0]._moveHandleClickThunk = function(evt){ HMI.cshmi._moveHandleClick(linesArray[0], ObjectPath, evt, true); };
			linesArray[1]._moveStartDragThunk = function(evt){ HMI.cshmi._moveStartDrag(linesArray[1], ObjectPath, evt); };
			linesArray[1]._moveMouseMoveThunk = function(evt){ HMI.cshmi._moveMouseMove(linesArray[1], ObjectPath, evt); };
			linesArray[1]._moveStopDragThunk = function(evt){ HMI.cshmi._moveStopDrag(linesArray[1], ObjectPath, evt, null); };
			linesArray[1]._moveCancelDragThunk = function(evt){ HMI.cshmi._moveStopDrag(linesArray[1], ObjectPath, evt, true); };
			linesArray[1]._moveHandleClickThunk = function(evt){ HMI.cshmi._moveHandleClick(linesArray[1], ObjectPath, evt, true); };
			linesArray[2]._moveStartDragThunk = function(evt){ HMI.cshmi._moveStartDrag(linesArray[2], ObjectPath, evt); };
			linesArray[2]._moveMouseMoveThunk = function(evt){ HMI.cshmi._moveMouseMove(linesArray[2], ObjectPath, evt); };
			linesArray[2]._moveStopDragThunk = function(evt){ HMI.cshmi._moveStopDrag(linesArray[2], ObjectPath, evt, null); };
			linesArray[2]._moveCancelDragThunk = function(evt){ HMI.cshmi._moveStopDrag(linesArray[2], ObjectPath, evt, true); };
			linesArray[2]._moveHandleClickThunk = function(evt){ HMI.cshmi._moveHandleClick(linesArray[2], ObjectPath, evt, true); };
			
			var adjustingPolylinePoints = function(newx, newy){
				//using the transparent line for visualisation the manipulation as a helperline
				this.setAttribute("stroke-width", "3px");
				this.setAttribute("opacity", "1");
				
				//fixme
				//limit position
				
				if(this.style.cursor === "col-resize"){
					this.setAttribute("x1", newx);
					this.setAttribute("x2", newx);
				}else if(this.style.cursor === "row-resize"){
					this.setAttribute("y1", newy);
					this.setAttribute("y2", newy);
				}
			};
			var savingPolylinePoints = function(){
				//reset the transparent helper line
				this.setAttribute("opacity", "0");
				this.setAttribute("stroke-width", "10px");
				
				if(this.style.cursor === "col-resize"){
					var newX = parseFloat(this.getAttribute("x1"));
					var oldX = 0;
					//detect old Coordinate
					if(this.getAttribute("data-lineid") === "1"){
						oldX = VisualObject.ResourceList.RoutePolyline.Coords.OffsetPointSourceX;
					}else if(this.getAttribute("data-lineid") === "2"){
						oldX = VisualObject.ResourceList.RoutePolyline.Coords.ContrlPointSourceX;
					}else if(this.getAttribute("data-lineid") === "3"){
						oldX = VisualObject.ResourceList.RoutePolyline.Coords.ContrlPointTargetX;
					}
					//all X coordinates which had this position has to be updated
					if(VisualObject.ResourceList.RoutePolyline.Coords.EndX !== VisualObject.ResourceList.RoutePolyline.Coords.StartX){
						//we do not start and end at the same position, so the OffsetPoints are candidates to be moved, too
						if(VisualObject.ResourceList.RoutePolyline.Coords.OffsetPointSourceX === oldX){ VisualObject.ResourceList.RoutePolyline.Coords.OffsetPointSourceX = newX;}
						if(VisualObject.ResourceList.RoutePolyline.Coords.OffsetPointTargetX === oldX){ VisualObject.ResourceList.RoutePolyline.Coords.OffsetPointTargetX = newX;}
					}
					if(VisualObject.ResourceList.RoutePolyline.Coords.ContrlPointSourceX === oldX){ VisualObject.ResourceList.RoutePolyline.Coords.ContrlPointSourceX = newX;}
					if(VisualObject.ResourceList.RoutePolyline.Coords.ContrlPointTargetX === oldX){ VisualObject.ResourceList.RoutePolyline.Coords.ContrlPointTargetX = newX;}
				}else if(this.style.cursor === "row-resize"){
					var newY = parseFloat(this.getAttribute("y1"));
					var oldY = 0;
					//detect old Coordinate
					if(this.getAttribute("data-lineid") === "1"){
						oldY = VisualObject.ResourceList.RoutePolyline.Coords.OffsetPointSourceY;
					}else if(this.getAttribute("data-lineid") === "2"){
						oldY = VisualObject.ResourceList.RoutePolyline.Coords.ContrlPointSourceY;
					}else if(this.getAttribute("data-lineid") === "3"){
						oldY = VisualObject.ResourceList.RoutePolyline.Coords.ContrlPointTargetY;
					}
					//all Y coordinates which had this position has to be updated
					if(VisualObject.ResourceList.RoutePolyline.Coords.EndY !== VisualObject.ResourceList.RoutePolyline.Coords.StartY){
						//we do not start and end at the same height, so the OffsetPoints are candidates to be moved, too
						if(VisualObject.ResourceList.RoutePolyline.Coords.OffsetPointSourceY === oldY){ VisualObject.ResourceList.RoutePolyline.Coords.OffsetPointSourceY = newY;}
						if(VisualObject.ResourceList.RoutePolyline.Coords.OffsetPointTargetY === oldY){ VisualObject.ResourceList.RoutePolyline.Coords.OffsetPointTargetY = newY;}
					}
					if(VisualObject.ResourceList.RoutePolyline.Coords.ContrlPointSourceY === oldY){ VisualObject.ResourceList.RoutePolyline.Coords.ContrlPointSourceY = newY;}
					if(VisualObject.ResourceList.RoutePolyline.Coords.ContrlPointTargetY === oldY){ VisualObject.ResourceList.RoutePolyline.Coords.ContrlPointTargetY = newY;}
				}
				VisualObject.correctAllLines(VisualObject);
				
				if(JSON && JSON.stringify && VisualObject.ResourceList.RoutePolyline.savingDisabled !== true){
					HMI.cshmi._setValueExecute(VisualObject, ObjectPath+".RoutingString", "static", JSON.stringify(VisualObject.ResourceList.RoutePolyline.Coords), null, true);
				}
				VisualObject.ResourceList.RoutePolyline.LineWasManipulated = true;
			};
			
			for(var i = 0; i<linesArray.length;i++){
				linesArray[i].setAttribute("stroke", VisualObject.getAttribute("stroke"));
				linesArray[i].setAttribute("stroke-width", "10px");
				linesArray[i].setAttribute("opacity", "0");
				
				linesArray[i].savingPolylinePoints = savingPolylinePoints;
				linesArray[i].adjustingPolylinePoints = adjustingPolylinePoints;
				
				//try both, mousedown and mousetouch. mousetouch will fire first, there we will kill mousedown
				linesArray[i].addEventListener("touchstart", linesArray[i]._moveStartDragThunk, false);
				linesArray[i].addEventListener("mousedown", linesArray[i]._moveStartDragThunk, false);
			}
			
			VisualObject.parentNode.insertBefore(linesArray[2], VisualObject);
			VisualObject.parentNode.insertBefore(linesArray[1], linesArray[2]);
			VisualObject.parentNode.insertBefore(linesArray[0], linesArray[1]);
			
			VisualObject.correctAllLines = function(VisualObject){
				if(VisualObject.tagName === "polyline"){
					var points = VisualObject.ResourceList.RoutePolyline.Coords.StartX + "," + VisualObject.ResourceList.RoutePolyline.Coords.StartY + " "
					+ VisualObject.ResourceList.RoutePolyline.Coords.OffsetPointSourceX + "," + VisualObject.ResourceList.RoutePolyline.Coords.OffsetPointSourceY + " "
					+ VisualObject.ResourceList.RoutePolyline.Coords.ContrlPointSourceX + "," + VisualObject.ResourceList.RoutePolyline.Coords.ContrlPointSourceY + " "
					+ VisualObject.ResourceList.RoutePolyline.Coords.ContrlPointTargetX + "," + VisualObject.ResourceList.RoutePolyline.Coords.ContrlPointTargetY + " "
					+ VisualObject.ResourceList.RoutePolyline.Coords.OffsetPointTargetX + "," + VisualObject.ResourceList.RoutePolyline.Coords.OffsetPointTargetY + " "
					+ VisualObject.ResourceList.RoutePolyline.Coords.EndX + "," + VisualObject.ResourceList.RoutePolyline.Coords.EndY;
					VisualObject.setAttribute("points", points);
				}else if(VisualObject.tagName === "path"){
					points = "M"+VisualObject.ResourceList.RoutePolyline.Coords.StartX + "," + VisualObject.ResourceList.RoutePolyline.Coords.StartY + " "
					+ "C"+ VisualObject.ResourceList.RoutePolyline.Coords.OffsetPointSourceX + "," + VisualObject.ResourceList.RoutePolyline.Coords.OffsetPointSourceY + " "
					+ VisualObject.ResourceList.RoutePolyline.Coords.ContrlPointSourceX + "," + VisualObject.ResourceList.RoutePolyline.Coords.ContrlPointSourceY + " "
					+ (VisualObject.ResourceList.RoutePolyline.Coords.ContrlPointSourceX+VisualObject.ResourceList.RoutePolyline.Coords.ContrlPointTargetX)/2 + ","
						+ (VisualObject.ResourceList.RoutePolyline.Coords.ContrlPointSourceY+VisualObject.ResourceList.RoutePolyline.Coords.ContrlPointTargetY)/2 + " "
					+ "S" + VisualObject.ResourceList.RoutePolyline.Coords.OffsetPointTargetX + "," + VisualObject.ResourceList.RoutePolyline.Coords.OffsetPointTargetY + " "
					+ VisualObject.ResourceList.RoutePolyline.Coords.EndX + "," + VisualObject.ResourceList.RoutePolyline.Coords.EndY;
					VisualObject.setAttribute("d", points);
				}
				
				var linesArray = VisualObject.ResourceList.linesArray;
				linesArray[0].setAttribute("x1", VisualObject.ResourceList.RoutePolyline.Coords.OffsetPointSourceX);
				linesArray[0].setAttribute("y1", VisualObject.ResourceList.RoutePolyline.Coords.OffsetPointSourceY);
				linesArray[0].setAttribute("x2", VisualObject.ResourceList.RoutePolyline.Coords.ContrlPointSourceX);
				linesArray[0].setAttribute("y2", VisualObject.ResourceList.RoutePolyline.Coords.ContrlPointSourceY);
				linesArray[0].style.cursor = VisualObject.ResourceList.RoutePolyline.Coords.OffsetPointSourceX===VisualObject.ResourceList.RoutePolyline.Coords.ContrlPointSourceX?"col-resize":"row-resize";
				linesArray[1].setAttribute("x1", VisualObject.ResourceList.RoutePolyline.Coords.ContrlPointSourceX);
				linesArray[1].setAttribute("y1", VisualObject.ResourceList.RoutePolyline.Coords.ContrlPointSourceY);
				linesArray[1].setAttribute("x2", VisualObject.ResourceList.RoutePolyline.Coords.ContrlPointTargetX);
				linesArray[1].setAttribute("y2", VisualObject.ResourceList.RoutePolyline.Coords.ContrlPointTargetY);
				linesArray[1].style.cursor = VisualObject.ResourceList.RoutePolyline.Coords.ContrlPointSourceX===VisualObject.ResourceList.RoutePolyline.Coords.ContrlPointTargetX?"col-resize":"row-resize";
				linesArray[2].setAttribute("x1", VisualObject.ResourceList.RoutePolyline.Coords.ContrlPointTargetX);
				linesArray[2].setAttribute("y1", VisualObject.ResourceList.RoutePolyline.Coords.ContrlPointTargetY);
				linesArray[2].setAttribute("x2", VisualObject.ResourceList.RoutePolyline.Coords.OffsetPointTargetX);
				linesArray[2].setAttribute("y2", VisualObject.ResourceList.RoutePolyline.Coords.OffsetPointTargetY);
				linesArray[2].style.cursor = VisualObject.ResourceList.RoutePolyline.Coords.ContrlPointTargetX===VisualObject.ResourceList.RoutePolyline.Coords.OffsetPointTargetX?"col-resize":"row-resize";
			};
		}
		
		var routingString = null;
		var savedRouting = false;
		if (VisualObject.ResourceList.RoutePolyline.Coords.StartX === undefined && typeof JSON === 'object' && typeof JSON.parse === 'function'){
			routingString = HMI.cshmi._getValue(VisualObject, ObjectPath+".RoutingString.value");
			if(routingString){
				if(routingString.charAt(0) !== "{"){
					routingString = "{"+routingString + "}";
				}
				try {
					var temp = JSON.parse(routingString);
				} catch (e) {
					temp = Object();
				}
				VisualObject.ResourceList.RoutePolyline.Coords = temp;
				temp = null;
				savedRouting = true;
			}else if(routingString === false){
				//the target has no possiblity to store this value (KS_ERR)
				VisualObject.ResourceList.RoutePolyline.savingDisabled = true;
			}
			routingString = null;
		}
		
		if(VisualObject.ResourceList.RoutePolyline.LineWasManipulated === undefined && savedRouting === true){
			//initial fill if we have a saved polyline
			VisualObject.correctAllLines(VisualObject);
			VisualObject.ResourceList.RoutePolyline.LineWasManipulated = true;
		}else if (StartX !== VisualObject.ResourceList.RoutePolyline.Coords.StartX ||
				//if start- and endPoints changed since last time, recompute polyline points
				StartY !== VisualObject.ResourceList.RoutePolyline.Coords.StartY ||
				rotateStart !== VisualObject.ResourceList.RoutePolyline.Coords.rotateStart ||
				EndX !== VisualObject.ResourceList.RoutePolyline.Coords.EndX ||
				EndY !== VisualObject.ResourceList.RoutePolyline.Coords.EndY ||
				rotateEnd !== VisualObject.ResourceList.RoutePolyline.Coords.rotateEnd ||
				(VisualObject.tagName === "polyline" && VisualObject.getAttribute("points") === "") || //we were hidden last time
				(VisualObject.tagName === "path" && VisualObject.getAttribute("d") === "") //we were hidden last time
			) {
			
			//the polyline routes in the worst case through those 6 points:
			//StartX
			//StartY
			var OffsetPointSourceX = 0;
			var OffsetPointSourceY = 0;
			var ContrlPointSourceX = 0;	//same as OffsetPointSource if not needed
			var ContrlPointSourceY = 0;
			var ContrlPointTargetX = 0;	//same as ContrlPointSource if not needed
			var ContrlPointTargetY = 0;
			var OffsetPointTargetX = 0;	//same as ContrlPointTarget if not needed
			var OffsetPointTargetY = 0;
			//EndX
			//EndY
			
			if (SourceConnectionPointdirection === 0 && TargetConnectionPointdirection === 180){
				//to right/OUTPUT --> from left/INPUT
				OffsetPointSourceX = StartX + OffsetSource;
				OffsetPointSourceY = StartY;
				if(OffsetPointSourceX >= EndX && StartX < EndX){
					OffsetPointSourceX = StartX + (EndX - StartX)/2;
					
					ContrlPointSourceX = OffsetPointSourceX;
					ContrlPointSourceY = (EndY+StartY)/2;
					
					ContrlPointTargetX = EndX - (EndX - StartX)/2;
					ContrlPointTargetY = ContrlPointSourceY;
					
					OffsetPointTargetX = ContrlPointTargetX;
					OffsetPointTargetY = EndY;
				}else if (OffsetPointSourceX >= EndX) {
					ContrlPointSourceX = OffsetPointSourceX;
					ContrlPointSourceY = (EndY+StartY)/2;
					
					ContrlPointTargetX = EndX - OffsetTarget;
					ContrlPointTargetY = ContrlPointSourceY;
					
					OffsetPointTargetX = ContrlPointTargetX;
					OffsetPointTargetY = EndY;
				}else{
					ContrlPointSourceX = OffsetPointSourceX;
					ContrlPointSourceY = OffsetPointSourceY;
					ContrlPointTargetX = ContrlPointSourceX;
					ContrlPointTargetY = ContrlPointSourceY;
					OffsetPointTargetX = OffsetPointSourceX;
					OffsetPointTargetY = EndY;
				}
			}else if (SourceConnectionPointdirection === 180 && TargetConnectionPointdirection === 0){
				//to left/INPUT --> from right/OUTPUT
				OffsetPointSourceX = StartX - OffsetSource;
				OffsetPointSourceY = StartY;
				if(OffsetPointSourceX <= EndX && StartX > EndX){
					OffsetPointSourceX = StartX + (EndX - StartX)/2;
					
					ContrlPointSourceX = OffsetPointSourceX;
					ContrlPointSourceY = (EndY+StartY)/2;
					
					ContrlPointTargetX = EndX - (EndX - StartX)/2;
					ContrlPointTargetY = ContrlPointSourceY;
					
					OffsetPointTargetX = ContrlPointTargetX;
					OffsetPointTargetY = EndY;
				}else if (OffsetPointSourceX <= EndX) {
					ContrlPointSourceX = OffsetPointSourceX;
					ContrlPointSourceY = (EndY+StartY)/2;
					
					ContrlPointTargetX = EndX + OffsetTarget;
					ContrlPointTargetY = ContrlPointSourceY;
					
					OffsetPointTargetX = ContrlPointTargetX;
					OffsetPointTargetY = EndY;
				}else{
					ContrlPointSourceX = OffsetPointSourceX;
					ContrlPointSourceY = OffsetPointSourceY;
					ContrlPointTargetX = ContrlPointSourceX;
					ContrlPointTargetY = ContrlPointSourceY;
					OffsetPointTargetX = OffsetPointSourceX;
					OffsetPointTargetY = EndY;
				}
			}else if (SourceConnectionPointdirection === 180 && TargetConnectionPointdirection === 180){
				//to left/INPUT --> from left/INPUT
				if (StartX - OffsetSource >= EndX) {
					OffsetPointSourceX = EndX - OffsetTarget;
					OffsetPointSourceY = StartY;
					ContrlPointSourceX = OffsetPointSourceX;
					ContrlPointSourceY = OffsetPointSourceY;
					ContrlPointTargetX = ContrlPointSourceX;
					ContrlPointTargetY = ContrlPointSourceY;
					OffsetPointTargetX = EndX - OffsetTarget;
					OffsetPointTargetY = EndY;
				}else{
					OffsetPointSourceX = StartX - OffsetSource;
					OffsetPointSourceY = StartY;
					ContrlPointSourceX = OffsetPointSourceX;
					ContrlPointSourceY = OffsetPointSourceY;
					ContrlPointTargetX = ContrlPointSourceX;
					ContrlPointTargetY = ContrlPointSourceY;
					OffsetPointTargetX = OffsetPointSourceX;
					OffsetPointTargetY = EndY;
				}
			}else if (SourceConnectionPointdirection === 0 && TargetConnectionPointdirection === 0){
				//toright/OUTPUT --> fromright/OUTPUT
				if (StartX + OffsetSource <= EndX) {
					OffsetPointSourceX = EndX + OffsetTarget;
					OffsetPointSourceY = StartY;
				}else{
					OffsetPointSourceX = StartX + OffsetSource;
					OffsetPointSourceY = StartY;
				}
				ContrlPointSourceX = OffsetPointSourceX;
				ContrlPointSourceY = OffsetPointSourceY;
				ContrlPointTargetX = ContrlPointSourceX;
				ContrlPointTargetY = ContrlPointSourceY;
				OffsetPointTargetX = ContrlPointTargetX;
				OffsetPointTargetY = EndY;
			}else if (SourceConnectionPointdirection === 270 && TargetConnectionPointdirection === 90){
				//to up --> from down
				if (StartY - OffsetSource < EndY){
					OffsetPointSourceX = StartX;
					OffsetPointSourceY = StartY - OffsetSource;
					ContrlPointSourceX = (EndX+StartX)/2;
					ContrlPointSourceY = OffsetPointSourceY;
					ContrlPointTargetX = ContrlPointSourceX;
					ContrlPointTargetY = EndY + OffsetTarget;
					OffsetPointTargetX = EndX;
					OffsetPointTargetY = EndY + OffsetTarget;
				}else{
					OffsetPointSourceX = StartX;
					OffsetPointSourceY = StartY - OffsetSource;
					ContrlPointSourceX = OffsetPointSourceX;
					ContrlPointSourceY = OffsetPointSourceY;
					ContrlPointTargetX = ContrlPointSourceX;
					ContrlPointTargetY = ContrlPointSourceY;
					OffsetPointTargetX = EndX;
					OffsetPointTargetY = ContrlPointTargetY;
				}
			}else if (SourceConnectionPointdirection === 90 && TargetConnectionPointdirection === 270){
				//to down --> from up
				if (StartY + OffsetSource < EndY){
					OffsetPointSourceX = StartX;
					OffsetPointSourceY = StartY + OffsetSource;
					ContrlPointSourceX = OffsetPointSourceX;
					ContrlPointSourceY = OffsetPointSourceY;
					ContrlPointTargetX = ContrlPointSourceX;
					ContrlPointTargetY = ContrlPointSourceY;
					OffsetPointTargetX = EndX;
					OffsetPointTargetY = StartY + OffsetSource;
				}else{
					OffsetPointSourceX = StartX;
					OffsetPointSourceY = StartY + OffsetSource;
					ContrlPointSourceX = (StartX + EndX)/2;
					ContrlPointSourceY = OffsetPointSourceY;
					ContrlPointTargetX = ContrlPointSourceX;
					ContrlPointTargetY = EndY - OffsetTarget;
					OffsetPointTargetX = EndX;
					OffsetPointTargetY = ContrlPointTargetY;
				}
			}else if (SourceConnectionPointdirection === 90 && TargetConnectionPointdirection === 90){
				//to down --> from down
				if (StartY + OffsetSource < EndY + OffsetTarget){
					OffsetPointSourceX = StartX;
					OffsetPointSourceY = EndY + OffsetTarget;
					ContrlPointSourceX = OffsetPointSourceX;
					ContrlPointSourceY = OffsetPointSourceY;
					ContrlPointTargetX = ContrlPointSourceX;
					ContrlPointTargetY = ContrlPointSourceY;
					OffsetPointTargetX = EndX;
					OffsetPointTargetY = EndY + OffsetTarget;
				}else{
					OffsetPointSourceX = StartX;
					OffsetPointSourceY = StartY + OffsetSource;
					ContrlPointSourceX = OffsetPointSourceX;
					ContrlPointSourceY = OffsetPointSourceY;
					ContrlPointTargetX = ContrlPointSourceX;
					ContrlPointTargetY = ContrlPointSourceY;
					OffsetPointTargetX = EndX;
					OffsetPointTargetY = StartY + OffsetSource;
				}
			}else if (SourceConnectionPointdirection === 270 && TargetConnectionPointdirection === 270){
				//to up --> from up
				if (StartY - OffsetSource < EndY - OffsetTarget) {
					OffsetPointSourceX = StartX;
					OffsetPointSourceY = StartY - OffsetSource;
					ContrlPointSourceX = OffsetPointSourceX;
					ContrlPointSourceY = OffsetPointSourceY;
					ContrlPointTargetX = ContrlPointSourceX;
					ContrlPointTargetY = ContrlPointSourceY;
					OffsetPointTargetX = EndX;
					OffsetPointTargetY = StartY - OffsetSource;
				}else{
					OffsetPointSourceX = StartX;
					OffsetPointSourceY = EndY - OffsetTarget;
					ContrlPointSourceX = OffsetPointSourceX;
					ContrlPointSourceY = OffsetPointSourceY;
					ContrlPointTargetX = ContrlPointSourceX;
					ContrlPointTargetY = ContrlPointSourceY;
					OffsetPointTargetX = EndX;
					OffsetPointTargetY = EndY - OffsetTarget;
				}
			}else if (SourceConnectionPointdirection === 0 && TargetConnectionPointdirection === 270){
				//to right --> from up
				if(StartY > EndY - OffsetTarget || StartX + OffsetSource > EndX){
					OffsetPointSourceX = StartX + OffsetSource;
					OffsetPointSourceY = StartY;
					ContrlPointSourceX = OffsetPointSourceX;
					ContrlPointSourceY = EndY - OffsetTarget;
					ContrlPointTargetX = ContrlPointSourceX;
					ContrlPointTargetY = ContrlPointSourceY;
					OffsetPointTargetX = EndX;
					OffsetPointTargetY = EndY - OffsetTarget;
				}else{
					OffsetPointSourceX = EndX;
					OffsetPointSourceY = StartY;
					ContrlPointSourceX = OffsetPointSourceX;
					ContrlPointSourceY = OffsetPointSourceY;
					ContrlPointTargetX = ContrlPointSourceX;
					ContrlPointTargetY = ContrlPointSourceY;
					OffsetPointTargetX = EndX;
					OffsetPointTargetY = EndY;
				}
			}else if (SourceConnectionPointdirection === 270 && TargetConnectionPointdirection === 0){
				//to up --> from right
				if (StartY - OffsetSource < EndY && StartX < EndX + OffsetTarget){
					OffsetPointSourceX = StartX;
					OffsetPointSourceY = StartY - OffsetSource;
					ContrlPointSourceX = EndX + OffsetTarget;
					ContrlPointSourceY = OffsetPointSourceY;
					ContrlPointTargetX = ContrlPointSourceX;
					ContrlPointTargetY = ContrlPointSourceY;
					OffsetPointTargetX = EndX + OffsetTarget;
					OffsetPointTargetY = EndY;
				}else{
					OffsetPointSourceX = StartX;
					OffsetPointSourceY = EndY;
					ContrlPointSourceX = OffsetPointSourceX;
					ContrlPointSourceY = OffsetPointSourceY;
					ContrlPointTargetX = ContrlPointSourceX;
					ContrlPointTargetY = ContrlPointSourceY;
					OffsetPointTargetX = EndX + OffsetTarget;
					OffsetPointTargetY = EndY;
				}
			}else if (SourceConnectionPointdirection === 0 && TargetConnectionPointdirection === 90){
				//to right --> from down
				if (StartX + OffsetSource < EndX && StartY > EndY + OffsetTarget){
					OffsetPointSourceX = StartX;
					OffsetPointSourceY = StartY;
					ContrlPointSourceX = OffsetPointSourceX;
					ContrlPointSourceY = OffsetPointSourceY;
					ContrlPointTargetX = ContrlPointSourceX;
					ContrlPointTargetY = ContrlPointSourceY;
					OffsetPointTargetX = EndX;
					OffsetPointTargetY = ContrlPointTargetY;
				}else{
					OffsetPointSourceX = StartX + OffsetSource;
					OffsetPointSourceY = StartY;
					ContrlPointSourceX = OffsetPointSourceX;
					ContrlPointSourceY = EndY + OffsetTarget;
					ContrlPointTargetX = ContrlPointSourceX;
					ContrlPointTargetY = ContrlPointSourceY;
					OffsetPointTargetX = EndX;
					OffsetPointTargetY = EndY + OffsetTarget;
				}
			}else if (SourceConnectionPointdirection === 90 && TargetConnectionPointdirection === 0){
				//to down --> from right
				if (StartX < EndX){
					OffsetPointSourceX = StartX;
					OffsetPointSourceY = StartY + OffsetSource;
					ContrlPointSourceX = EndX + OffsetTarget;
					ContrlPointSourceY = OffsetPointSourceY;
					ContrlPointTargetX = ContrlPointSourceX;
					ContrlPointTargetY = ContrlPointSourceY;
					OffsetPointTargetX = EndX + OffsetTarget;
					OffsetPointTargetY = EndY;
				}else{
					OffsetPointSourceX = StartX;
					OffsetPointSourceY = EndY;
					ContrlPointSourceX = OffsetPointSourceX;
					ContrlPointSourceY = OffsetPointSourceY;
					ContrlPointTargetX = ContrlPointSourceX;
					ContrlPointTargetY = ContrlPointSourceY;
					OffsetPointTargetX = ContrlPointTargetX;
					OffsetPointTargetY = ContrlPointTargetY;
				}
			}else if (SourceConnectionPointdirection === 180 && TargetConnectionPointdirection === 270){
				//to left --> from up
				if(StartX - OffsetSource < EndX){
					OffsetPointSourceX = StartX - OffsetSource;
					OffsetPointSourceY = StartY;
					ContrlPointSourceX = OffsetPointSourceX;
					ContrlPointSourceY = EndY - OffsetTarget;
					ContrlPointTargetX = ContrlPointSourceX;
					ContrlPointTargetY = ContrlPointSourceY;
					OffsetPointTargetX = EndX;
					OffsetPointTargetY = EndY - OffsetTarget;
				}else{
					OffsetPointSourceX = StartX - OffsetSource;
					OffsetPointSourceY = StartY;
					ContrlPointSourceX = OffsetPointSourceX;
					ContrlPointSourceY = OffsetPointSourceY;
					ContrlPointTargetX = ContrlPointSourceX;
					ContrlPointTargetY = ContrlPointSourceY;
					OffsetPointTargetX = EndX;
					OffsetPointTargetY = ContrlPointTargetY;
				}
			}else if (SourceConnectionPointdirection === 270 && TargetConnectionPointdirection === 180){
				//to up --> from left
				if (StartY - OffsetSource < EndY) {
					OffsetPointSourceX = StartX;
					OffsetPointSourceY = StartY - OffsetSource;
					ContrlPointSourceX = EndX - OffsetTarget;
					ContrlPointSourceY = OffsetPointSourceY;
					ContrlPointTargetX = ContrlPointSourceX;
					ContrlPointTargetY = ContrlPointSourceY;
					OffsetPointTargetX = ContrlPointTargetX;
					OffsetPointTargetY = EndY;
				}else{
					OffsetPointSourceX = StartX;
					OffsetPointSourceY = EndY;
					ContrlPointSourceX = OffsetPointSourceX;
					ContrlPointSourceY = OffsetPointSourceY;
					ContrlPointTargetX = ContrlPointSourceX;
					ContrlPointTargetY = ContrlPointSourceY;
					OffsetPointTargetX = ContrlPointTargetX;
					OffsetPointTargetY = EndY;
				}
			}else if (SourceConnectionPointdirection === 180 && TargetConnectionPointdirection === 90){
				//to left --> from down
				if (StartX - OffsetSource < EndX){
					OffsetPointSourceX = StartX - OffsetSource;
					OffsetPointSourceY = StartY;
					ContrlPointSourceX = OffsetPointSourceX;
					ContrlPointSourceY = EndY + OffsetTarget;
					ContrlPointTargetX = ContrlPointSourceX;
					ContrlPointTargetY = ContrlPointSourceY;
					OffsetPointTargetX = EndX;
					OffsetPointTargetY = EndY + OffsetTarget;
				}else{
					OffsetPointSourceX = EndX;
					OffsetPointSourceY = StartY;
					ContrlPointSourceX = OffsetPointSourceX;
					ContrlPointSourceY = OffsetPointSourceY;
					ContrlPointTargetX = ContrlPointSourceX;
					ContrlPointTargetY = ContrlPointSourceY;
					OffsetPointTargetX = EndX;
					OffsetPointTargetY = ContrlPointTargetY;
				}
			}else if (SourceConnectionPointdirection === 90 && TargetConnectionPointdirection === 180){
				//to down --> from left
				if (StartY + OffsetSource >= EndY) {
					OffsetPointSourceX = StartX;
					OffsetPointSourceY = StartY + OffsetSource;
					ContrlPointSourceX = EndX - OffsetTarget;
					ContrlPointSourceY = OffsetPointSourceY;
					ContrlPointTargetX = ContrlPointSourceX;
					ContrlPointTargetY = ContrlPointSourceY;
					OffsetPointTargetX = ContrlPointTargetX;
					OffsetPointTargetY = EndY;
				}else{
					OffsetPointSourceX = StartX;
					OffsetPointSourceY = EndY;
					ContrlPointSourceX = OffsetPointSourceX;
					ContrlPointSourceY = OffsetPointSourceY;
					ContrlPointTargetX = ContrlPointSourceX;
					ContrlPointTargetY = ContrlPointSourceY;
					OffsetPointTargetX = ContrlPointTargetX;
					OffsetPointTargetY = EndY;
				}
			}
			
			//remember manipulation result. Attention: these values are stored in many FBDs, so renaming them can destroy positioning
			VisualObject.ResourceList.RoutePolyline.Coords.StartX = StartX;
			VisualObject.ResourceList.RoutePolyline.Coords.StartY = StartY;
			VisualObject.ResourceList.RoutePolyline.Coords.rotateStart = rotateStart;
			VisualObject.ResourceList.RoutePolyline.Coords.OffsetPointSourceX = OffsetPointSourceX;
			VisualObject.ResourceList.RoutePolyline.Coords.OffsetPointSourceY = OffsetPointSourceY;
			VisualObject.ResourceList.RoutePolyline.Coords.ContrlPointSourceX = ContrlPointSourceX;
			VisualObject.ResourceList.RoutePolyline.Coords.ContrlPointSourceY = ContrlPointSourceY;
			VisualObject.ResourceList.RoutePolyline.Coords.ContrlPointTargetX = ContrlPointTargetX;
			VisualObject.ResourceList.RoutePolyline.Coords.ContrlPointTargetY = ContrlPointTargetY;
			VisualObject.ResourceList.RoutePolyline.Coords.OffsetPointTargetX = OffsetPointTargetX;
			VisualObject.ResourceList.RoutePolyline.Coords.OffsetPointTargetY = OffsetPointTargetY;
			VisualObject.ResourceList.RoutePolyline.Coords.EndX = EndX;
			VisualObject.ResourceList.RoutePolyline.Coords.EndY = EndY;
			VisualObject.ResourceList.RoutePolyline.Coords.rotateEnd = rotateEnd;
			
			VisualObject.correctAllLines(VisualObject);
			if(VisualObject.ResourceList.RoutePolyline.LineWasManipulated !== false){
				if(JSON && JSON.stringify && VisualObject.ResourceList.RoutePolyline.savingDisabled !== true){
					HMI.cshmi._setValueExecute(VisualObject, ObjectPath+".RoutingString", "static", "", null, true);
				}
			}
			VisualObject.ResourceList.RoutePolyline.LineWasManipulated = false;
			//end calculate new points
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
		if(VisualObject.ResourceList && VisualObject.ResourceList.FBReference){
			//save a (perhaps changed) FBref for later rebuilding of the template
			this.ResourceList.newRebuildObject.id = VisualObject.id;
			this.ResourceList.newRebuildObject.x = VisualObject.getAttribute("x");
			this.ResourceList.newRebuildObject.y = VisualObject.getAttribute("y");
		}
		var newVisualObject = this._interpreteElementOrEventRecursive(VisualParentObject, ObjectPath, ObjectType, false);
		this.ResourceList.newRebuildObject = Object();
		
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
				}else{
					i++;
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
				}else{
					i++;
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
		
		VisualObject.parentNode.replaceChild(newVisualObject, VisualObject);
		
		//we want to have offset parameter on all visual elements
		HMI.saveAbsolutePosition(newVisualObject);
		
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
	 * vibrate the display component
	 * @param {SVGElement} VisualObject Object to manipulate the visualisation
	 * @param {String} ObjectPath Path to this cshmi object containing the event/action/visualisation
	 * @return {Boolean} true on success, false if an error occured
	 */
	_interpreteVibrate: function(VisualObject, ObjectPath){
		var pattern = null;
		var requestList = new Object();
		if (this.ResourceList.Actions[ObjectPath] !== undefined){
			requestList[ObjectPath] = this.ResourceList.Actions[ObjectPath].Parameters;
			pattern = this.ResourceList.Actions[ObjectPath].Parameters["pattern"];
		}else{
			requestList[ObjectPath] = new Object();
			requestList[ObjectPath]["pattern"] = null;
			var successCode = this._requestVariablesArray(requestList);
			if (successCode === false){
				return null;
			}
			
			var temppattern = requestList[ObjectPath]["pattern"];
			
			if(temppattern.indexOf(" ") !== -1){
				temppattern = temppattern.split(" ");
			}else if(temppattern.indexOf("{") === -1){
				temppattern = HMI.KSClient.splitKsResponse(temppattern);
			}else{
				temppattern = [temppattern];
			}
			pattern = Array();
			for (var i = 0;i < temppattern.length;i++){
				pattern[i] = parseInt(temppattern[i], 10);
			}
			//we have asked the object successful, so remember the result
			this.ResourceList.Actions[ObjectPath] = new Object();
			this.ResourceList.Actions[ObjectPath].Parameters = requestList[ObjectPath];
			this.ResourceList.Actions[ObjectPath].Parameters["pattern"] = pattern;
		}
		if (navigator.vibrate){
			navigator.vibrate(pattern);
		}
		
		return true;
	},
	
	/**
	 * Action which calls _buildSvgGroup to build a template
	 * @param {SVGElement} VisualParentObject visual Object which is parent to active Object
	 * @param {String} ObjectPath Path to this cshmi object containing the event/action/visualisation
	 * @param ChildrenIterator of the iterator or null
	 * @return {Boolean} true on success, false if an error occured
	 */
	_interpreteInstantiateTemplate: function(VisualParentObject, ObjectPath, ChildrenIterator){
		if(VisualParentObject.getElementById === undefined){
			HMI.hmi_log_info_onwebsite("InstantiateTemplate is not allowed under an Element (configured here: "+ObjectPath+")");
			return false;
		}
		var VisualObject = this._buildSvgGroup(VisualParentObject, ObjectPath, true, ChildrenIterator, false);
		if (VisualObject !== null){
			//remember the ObjectType on every object (needed for reloading via action)
			VisualObject.setAttribute("data-ObjectType", "/cshmi/Template");
			
			VisualParentObject.appendChild(VisualObject);
			//calculate all offset parameter to be able to display visual feedback
			//needed now, because we append new components
			HMI.saveAbsolutePosition(VisualObject);
			
			//interprete onload Actions if we are already loaded
			if (this.initStage === false){
				this._interpreteOnloadCallStack();
			}
			
			return true;
		}
		return false;
	},
	
	/**
	 * builds groups and template actions, gets the parameter via KS
	 * @param {SVGElement} VisualParentObject visual Object which is parent to active Object
	 * @param {String} ObjectPath Path to this cshmi object containing the event/action/visualisation
	 * @param {Boolean} calledFromInstantiateTemplate true if called from an action
	 * @param ChildrenIterator of the iterator or null
	 * @param {bool} preventNetworkRequest the function should prevent network requests if possible
	 * @return {SVGElement} VisualObject the new constructed element or null
	 */
	_buildSvgGroup: function(VisualParentObject, ObjectPath, calledFromInstantiateTemplate, ChildrenIterator, preventNetworkRequest){
		var requestList = new Object();
		if(preventNetworkRequest === true && (calledFromInstantiateTemplate === true && ChildrenIterator && ChildrenIterator.currentChild !== undefined)){
			//not possible if called from action under a childrenIterator. There is the same ObjectPath, but different Objects under the same domain
			
			//skip this element
			return null;
		}else if (this.ResourceList.Elements[ObjectPath] !== undefined){
			//get the information if the config of the object is known through the turbo
			requestList[ObjectPath] = this.ResourceList.Elements[ObjectPath].Parameters;
		}else{
			requestList[ObjectPath] = new Object();
			requestList[ObjectPath]["x"] = null;
			requestList[ObjectPath]["y"] = null;
			requestList[ObjectPath]["visible"] = null;
			requestList[ObjectPath]["opacity"] = null;
			requestList[ObjectPath]["rotate"] = null;
			if (calledFromInstantiateTemplate === true){
				//an action does have some special properties
				requestList[ObjectPath]["xOffset"] = null;
				requestList[ObjectPath]["yOffset"] = null;
				requestList[ObjectPath]["maxTemplatesPerDirection"] = null;
				requestList[ObjectPath]["preventClone"] = null;
			}else{
				//a group has other
				requestList[ObjectPath]["width"] = null;
				requestList[ObjectPath]["height"] = null;
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
		
		var wasRebuildObject = false;
		var PathOfTemplateDefinition = null;
		if(requestList[ObjectPath]["TemplateDefinition"] !== undefined && requestList[ObjectPath]["TemplateDefinition"] !== ""){
			if(requestList[ObjectPath]["TemplateDefinition"][0] === "/"){
				//we have a template name with a full path
				PathOfTemplateDefinition = requestList[ObjectPath]["TemplateDefinition"];
			}else{
				PathOfTemplateDefinition = "/TechUnits/cshmi/Templates/"+requestList[ObjectPath]["TemplateDefinition"];
			}
			if (ObjectPath.indexOf(PathOfTemplateDefinition) === 0 && !ChildrenIterator){
				HMI.hmi_log_info_onwebsite("Template "+ObjectPath+" is calling itself outside an iterator");
				return null;
			}
			
			//we have an template to fill
			var requestListTemplate = new Object();
			//get the information if the config of the object is known through the turbo
			if (this.ResourceList.Elements && this.ResourceList.Elements[PathOfTemplateDefinition] !== undefined){
				requestListTemplate[PathOfTemplateDefinition] = this.ResourceList.Elements[PathOfTemplateDefinition].Parameters;
			}else{
				requestListTemplate[PathOfTemplateDefinition] = new Object();
				requestListTemplate[PathOfTemplateDefinition]["width"] = null;
				requestListTemplate[PathOfTemplateDefinition]["height"] = null;
				
				successCode = this._requestVariablesArray(requestListTemplate, true);
				if (successCode === false){
					HMI.hmi_log_info_onwebsite("Template "+ObjectPath+" is wrong configured. TemplateDefinition '"+PathOfTemplateDefinition+"' is not available.");
					return null;
				}
				
				//we have asked the object successful, so remember the result
				this.ResourceList.Elements[PathOfTemplateDefinition] = new Object();
				this.ResourceList.Elements[PathOfTemplateDefinition].Parameters = requestListTemplate[PathOfTemplateDefinition];
			}
		}
		
		var VisualObject = HMI.svgDocument.createElementNS(HMI.HMI_Constants.NAMESPACE_SVG, 'svg');
		
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
		VisualObject.setAttribute("data-NameOrigin", "TemplateName");
		if(PathOfTemplateDefinition){
			HMI.addClass(VisualObject, this.cshmiTemplateClass);
			VisualObject.setAttribute("data-TemplateModelSource", PathOfTemplateDefinition);
		}else{
			//some internal functions are happy if this attribute exists
			VisualObject.setAttribute("data-TemplateModelSource", "");
		}
		
		//the VPO is needed in generateFullKsPath
		VisualObject.VisualParentObject = VisualParentObject;
		
		//###########################################################################
		//parametrise templateDefinition with the config
		VisualObject.ResourceList = new Object();
		VisualObject.ResourceList.FBReference = null;
		VisualObject.ResourceList.FBVariableReference = Object();
		VisualObject.ResourceList.ConfigValues = Object();
		
		
		////////////////////////////////////////////////////////////////////////////
		//FBReference
		if(requestList[ObjectPath]["FBReference"] === undefined){
			//old Modells did not have template functionality
			requestList[ObjectPath]["FBReference"] = "";
			requestList[ObjectPath]["FBVariableReference"] = "";
			requestList[ObjectPath]["ConfigValues"] = "";
		}
		if (VisualParentObject === null && HMI.HMI_Constants.UrlParameterList && HMI.HMI_Constants.UrlParameterList.FBReference && HMI.HMI_Constants.UrlParameterList.FBReference.length > 0){
			//an url parameter is able to overwrite the FBReference
			var FBReferenceName = HMI.HMI_Constants.UrlParameterList.FBReference;
		}else if (requestList[ObjectPath]["FBReference"].indexOf(" ") !== -1){
			HMI.hmi_log_info_onwebsite("Only the first FBReference is valid. "+ObjectPath+" has more of them.");
			FBReferenceName = requestList[ObjectPath]["FBReference"].split(" ")[0];
		}else{
			FBReferenceName = requestList[ObjectPath]["FBReference"];
		}
		if (FBReferenceName !== ""){
			if(this.ResourceList.newRebuildObject.id !== undefined){
				//we should use a preconfigured FBref, instead of the configured one...
				VisualObject.ResourceList.FBReference = this.ResourceList.newRebuildObject.id;
				VisualObject.id = this.ResourceList.newRebuildObject.id;
				VisualObject.setAttribute("x", this.ResourceList.newRebuildObject.x);
				VisualObject.setAttribute("y", this.ResourceList.newRebuildObject.y);
				
				wasRebuildObject = true;
				//.. but only once/here
				this.ResourceList.newRebuildObject = Object();
			}else if (calledFromInstantiateTemplate === true && ChildrenIterator && ChildrenIterator.currentChild !== undefined && ChildrenIterator.currentChild[FBReferenceName] !== undefined){
				//something like OP_NAME or OP_VALUE was requested, so we have to find the real info from the iterator
				
				//getFBReference does all the magic with the iterator. The later checks are to detect the NameOrigin
				var FBRef = this._getFBReference(VisualParentObject, ChildrenIterator);
				if (FBRef.path === ""){
					HMI.hmi_log_info_onwebsite('Template '+ObjectPath+' is wrong configured. Found no FBReference on a parent.');
					return null;
				}
				if (ChildrenIterator.currentChild[FBReferenceName].charAt(0) === "/"){
					// the iterated string begins with / so it is a fullpath (likely from a GetVar on an assoziation)
					VisualObject.setAttribute("data-NameOrigin", "CurrentChildfullpath");
				}else{
					//In OP_NAME is a relative path (likely from a GetEP request).
					VisualObject.setAttribute("data-NameOrigin", "FBReference/OP_NAME");
				}
				VisualObject.ResourceList.FBReference = FBRef.path;
				VisualObject.id = FBRef.path;
			}else{
				if(FBReferenceName.indexOf("CSHMIfullqualifiedname") === 0){
					//CSHMIfullqualifiedname can be a prefix
					var FBrefName = this._getFBReference(VisualParentObject, null);
					VisualObject.ResourceList.FBReference = FBReferenceName.replace("CSHMIfullqualifiedname", FBrefName.path);
					VisualObject.setAttribute("data-NameOrigin", "fullqualifiedname+newPart");
				}else if(FBReferenceName.indexOf("CSHMIHostServer") === 0){
					//for example CSHMIHostServer/acplt/ov/library only adds the host and server
					var FBrefName = this._getFBReference(VisualParentObject, null);
					var HostServer = HMI.KSClient._splitKSPath(FBrefName.path)[0];
					VisualObject.ResourceList.FBReference = FBReferenceName.replace("CSHMIHostServer", "//"+HostServer);
					VisualObject.setAttribute("data-NameOrigin", "CSHMIHostServer+newPart");
					FBrefName = null;
					HostServer = null;
				}else{
					//We have straightforward a full name of one FB Object, so save it with the default name
					VisualObject.ResourceList.FBReference = FBReferenceName;
					if (FBReferenceName.charAt(0) === "/"){
						//full path is given
						VisualObject.setAttribute("data-NameOrigin", "FBReference");
					}else{
						//relative path is given, so complete the path with the BaseKsPath
						VisualObject.setAttribute("data-NameOrigin", "FBReference+BaseKsPath");
					}
				}
				//force a clean name and process CSHMIModelHost for example
				VisualObject.ResourceList.FBReference = this._generateFullKsPath(VisualObject, ObjectPath, VisualObject.ResourceList.FBReference);
				VisualObject.id = VisualObject.ResourceList.FBReference;
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
			if (calledFromInstantiateTemplate === true && ChildrenIterator && ChildrenIterator.currentChild !== undefined && ChildrenIterator.currentChild[FBVariableReferenceEntry[1]] !== undefined){
				//we want to get values from the current child (e.g. myFavoriteVar:OP_NAME)
				
				if (ChildrenIterator.currentChild[FBVariableReferenceEntry[1]].charAt(0) !== "/"){
					//Value from currentChild is not a full path, so search for it in the parents
					
					var FBRef = this._getFBReference(VisualParentObject, ChildrenIterator);
					if (FBRef.path === ""){
						HMI.hmi_log_info_onwebsite('Template '+ObjectPath+' is wrong configured. Found no FBReference on a parent.');
						return null;
					}else if (FBRef.path.indexOf("//localhost") === 0){
						//fixme change that to match fbreference
						//localhost in the model should not be the http-gateway
						FBRef.path = FBRef.path.replace(/localhost/, HMI.KSClient.ResourceList.ModelHost);
					}
					VisualObject.ResourceList.FBVariableReference[FBVariableReferenceEntry[0]] = FBRef.path;
				}else{
					//currentChild set a full path
					VisualObject.ResourceList.FBVariableReference[FBVariableReferenceEntry[0]] = ChildrenIterator.currentChild[FBVariableReferenceEntry[1]];
				}
			}else{
				//direct setting of a FBVariable
				FBVariableReferenceEntry[1] = this._generateFullKsPath(VisualObject, ObjectPath, FBVariableReferenceEntry[1]);
				VisualObject.ResourceList.FBVariableReference[FBVariableReferenceEntry[0]] = FBVariableReferenceEntry[1];
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
					var Value = KeyValueEntry.slice(1).join(":"); // KeyValueEntry could be "buttoncontent:hello:welt" so we want "hello:welt"
				}else{
					Value = KeyValueEntry[1];
				}
				//check if we want to get values from the current child (e.g. OP_NAME)
				//if instantiateTemplate is not called within a childreniterator, the currentChild is undefined
				if (calledFromInstantiateTemplate === true && ChildrenIterator && ChildrenIterator.currentChild !== undefined && ChildrenIterator.currentChild[Value] !== undefined){
					VisualObject.ResourceList.ConfigValues[KeyValueEntry[0]] = ChildrenIterator.currentChild[Value];
				}else{
					VisualObject.ResourceList.ConfigValues[KeyValueEntry[0]] = Value;
				}
				//doku
				if(KeyValueEntry[0].toLowerCase() === "name"){
					//if an template has the configValue Name:Dieter, this should be the local object id
					VisualObject.id = VisualParentObject.id + "/" + VisualObject.ResourceList.ConfigValues[KeyValueEntry[0]];
					VisualObject.setAttribute("data-NameOrigin", "Parent+ConfigValue");
				}
				if(KeyValueEntry[0].toLowerCase() === "fullqualifiedname"){
					//if an template has the configValue fullqualifiedname:Dieter, this should be the global object id
					VisualObject.id = VisualObject.ResourceList.ConfigValues[KeyValueEntry[0]];
					VisualObject.setAttribute("data-NameOrigin", "ConfigValueFQN");
				}
				lastEntry = KeyValueEntry[0];
			}else if (KeyValueEntry.length === 1 && lastEntry !== null){
				//we had something like "pumpcolor:yellow pumpname:N 18", so need to add the " 18" to the last entry
				VisualObject.ResourceList.ConfigValues[lastEntry] = VisualObject.ResourceList.ConfigValues[lastEntry]+" "+KeyValueEntry[0];
			}
		}
		
		VisualObject.setAttribute("overflow", "visible");
		
		if (calledFromInstantiateTemplate === true){
			HMI.addClass(VisualObject, this.cshmiTemplateActionClass);
		}
		
		/////////////////////////////////////////////////////////////////////////////
		//Iterator position changing
		
		//save Template configuration for caching
		var xTemplate = requestList[ObjectPath]["x"];
		var yTemplate = requestList[ObjectPath]["y"];
		
		if(wasRebuildObject === true){
			//xy is set already, prevent resetting to wrong value
			delete requestList[ObjectPath]["x"];
			delete requestList[ObjectPath]["y"];
		}else if (calledFromInstantiateTemplate === true && ChildrenIterator && requestList[ObjectPath]["maxTemplatesPerDirection"] !== undefined && requestList[ObjectPath]["xOffset"] !== undefined && requestList[ObjectPath]["yOffset"] !== undefined ){
			//adjust position with an offset. if there is no offset, do not change xy and do not increment the currentCount
			if(requestList[ObjectPath]["xOffset"] !== "0" || requestList[ObjectPath]["yOffset"] !== "0"){
				//the offsetCount must be global for all InstantiateTemplate with the same xy coordinates below an iterator
				//todo docu
				var iteratorID = "x"+parseInt(requestList[ObjectPath]["x"], 10) + "y" + parseInt(requestList[ObjectPath]["y"], 10);
				var offsetCount = 0;
				
				if(ChildrenIterator.currentCount[iteratorID]){
					//we have a iterator value
					offsetCount = ChildrenIterator.currentCount[iteratorID];
					//the next InstantiateTemplate should go to an other position
					ChildrenIterator.currentCount[iteratorID]++;
				}else{
					//we are the first instance for this iterator group
					ChildrenIterator.currentCount[iteratorID] = 1;
				}
				VisualObject.setAttribute("data-iteratorID", iteratorID);
				
				var offsetCountX;
				var offsetCountY;
				var direction = requestList[ObjectPath]["maxTemplatesPerDirection"].split(":");
				if(direction.length == 2 && direction[0] === "x"){
					// we have an maximum in a line
					offsetCountX = offsetCount % parseInt(direction[1], 10);
					offsetCountY = Math.floor(offsetCount / parseInt(direction[1]), 10);
				}else if(direction.length == 2 && direction[0] === "y"){
					// we have an maximum in a row
					offsetCountX = Math.floor(offsetCount / parseInt(direction[1], 10));
					offsetCountY = offsetCount % parseInt(direction[1],10);
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
		}
		
		//////////////////////////////////////////////////////////////////////////
		//adjust visual appearance
		
		//setting the basic Element Variables like .x .y .visible .stroke .fill .opacity .rotate
		this._processBasicVariables(VisualObject, requestList[ObjectPath]);
		if (requestList[ObjectPath]["rotate"] && requestList[ObjectPath]["rotate"] !== "0"){
			//rotate is not specified with a svg-Element, so encapsule in a G-Element
			//http://www.w3.org/Graphics/SVG/WG/track/issues/2252
			var VisualChildObject = HMI.svgDocument.createElementNS(HMI.HMI_Constants.NAMESPACE_SVG, 'g');
			VisualChildObject.setAttribute("overflow", "visible");
			VisualChildObject.setAttribute("x", "0");
			VisualChildObject.setAttribute("y", "0");
			var rotation = ((parseFloat(requestList[ObjectPath]["rotate"])%360)+360)%360;
			VisualChildObject.setAttribute("transform", "rotate("+rotation.toString()+","+requestList[ObjectPath]["x"]+","+requestList[ObjectPath]["y"]+")");
			VisualChildObject.appendChild(VisualObject);
			VisualObject.removeAttribute("transform");
		}else{
			VisualChildObject = null;
		}
		
		//restore Template config
		requestList[ObjectPath]["x"] = xTemplate;
		requestList[ObjectPath]["y"] = yTemplate;
		
		if(PathOfTemplateDefinition){
			//width and height comes from the TemplateDefinition
			VisualObject.setAttribute("width", requestListTemplate[PathOfTemplateDefinition]["width"]);
			VisualObject.setAttribute("height", requestListTemplate[PathOfTemplateDefinition]["height"]);
		}
		
		//mark object for preventClone. id has to be unique in the DOM (so searching for id results in the first object), name does not
		VisualObject.setAttribute("name", VisualObject.id);
		
		if(calledFromInstantiateTemplate && requestList[ObjectPath]["preventClone"] !== undefined){
			if(requestList[ObjectPath]["preventClone"] === "TRUE"){
				//search all object named the same
				if(HMI.svgDocument.querySelectorAll){
					var CloneCandidates = HMI.svgDocument.querySelectorAll('[name="'+VisualObject.id+'"]');
				}else{
					//shim the queryselector if not available
					CloneCandidates = Array();
					var ComponentChilds = HMI.Playground.firstChild.getElementsByTagName("svg");
					for(var i = 0;i < ComponentChilds.length;i++){
						if(ComponentChilds[i].getAttribute("name") == VisualObject.id){
							CloneCandidates.push();
						}
					}
				}
				//search and compare candidates
				for (var i = 0; i < CloneCandidates.length; ++i) {
					if(CloneCandidates[i].getAttribute("data-TemplateModelSource") === PathOfTemplateDefinition){
						//we should prevent a clone and we would produce one, abort
						return null;
					}
				}
			}
		}
		
		// test if the parent should get a click event and register for this
		// for magic with "hidable" attribute
		if (VisualParentObject !== null && requestList[ObjectPath]["hideable"] === "TRUE"){
			HMI.addClass(VisualObject, this.cshmiGroupHideableClass);
			VisualParentObject.setAttribute("cursor", "pointer");
			
			//make the parent clickable, if we can be hidden and no sibling has done this before
			if (HMI.instanceOf(VisualParentObject, this.cshmiGroupHasHideableChildrenClass) === false){
				//toggle visibility of hideable childtemplates onclick
				VisualParentObject.addEventListener("click", function(evt){
					//quit propagation of event in any case. We do not want the parent template to handle the click
					if (evt.stopPropagation) evt.stopPropagation();
					
					if(HMI.instanceOf(VisualParentObject, HMI.cshmi.cshmiOperatorClickClass)){
						//we have an clickgesture on the same VisualObject, so this handled all action in mouse up
						return;
					}
					
					HMI.cshmi.toggleChildGroups(VisualParentObject);
				}, false);
				
				//prevent multiple events on this
				HMI.addClass(VisualParentObject, this.cshmiGroupHasHideableChildrenClass);
			}
		}
		
		//instance order is lost on zindex manipulation (dom reordering)
		//so build a second childNodes-like collection for preserving ordering
		VisualObject.cshmiOriginalOrderList = new Object();

		if(VisualParentObject !== null){
			//add ourself to the parent zindex-list
			if(VisualParentObject.cshmiOriginalOrderList === undefined){
				VisualParentObject.cshmiOriginalOrderList = new Object();
			}
			if(VisualParentObject.cshmiOriginalOrderList[PathOfTemplateDefinition] === undefined){
				VisualParentObject.cshmiOriginalOrderList[PathOfTemplateDefinition] = new Array();
			}
			VisualParentObject.cshmiOriginalOrderList[PathOfTemplateDefinition].push(VisualObject);
		}
		
		//////////////////////////////////////////////////////////////////////////
		//get children (graphics and actions) from the TemplateDefinition
		//our children will be fetched later
		if(PathOfTemplateDefinition !== null){
			if(VisualObject.getAttribute("display") === "none"){
				//we are not visible, so we should not contact network
				
				//the marking of the class will be done in the caller of our function
				this._interpreteChildrensRecursive(VisualObject, PathOfTemplateDefinition, true);
				
				// mark objects incomplete AFTER initialisation
				// _interpreteHiddenChildrenElements has to know if it has to load childrens
				HMI.addClass(VisualObject, this.cshmiGroupVisibleChildrenNotLoadedClass);
			}else{
				this._interpreteChildrensRecursive(VisualObject, PathOfTemplateDefinition, preventNetworkRequest);
			}
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
		if(preventNetworkRequest === true){
			//skip this element
			return null;
		}else if (this.ResourceList.Elements[ObjectPath] !== undefined){
			//get the information if the config of the object is known through the turbo
			requestList[ObjectPath] = this.ResourceList.Elements[ObjectPath].Parameters;
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
			
			var successCode = this._requestVariablesArray(requestList);
			if (successCode === false){
				return null;
			}
			
			//get via KSClient because of problems getting '{}' caused by splitKsResponse
			//slice braces '{}' if any
			var HTMLcontent_braces = HMI.KSClient.getVar(ObjectPath+".HTMLcontent", "OP_VALUE", null);
			var j = 0;
			while(HTMLcontent_braces.charAt(0) === '{') {
				HTMLcontent_braces = HTMLcontent_braces.substr(1);
				j++;
			}
			HTMLcontent_braces = HTMLcontent_braces.substr(0, HTMLcontent_braces.length - j);
			requestList[ObjectPath]["HTMLcontent"] = HTMLcontent_braces;
			
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
			if (jsOnglobalvarchanged_braces.indexOf("KS_ERR_BADPATH") !== -1){
				var j = 0;
				while(jsOnglobalvarchanged_braces.charAt(0) === '{') {
					jsOnglobalvarchanged_braces = jsOnglobalvarchanged_braces.substr(1);
					j++;
				}
				jsOnglobalvarchanged_braces = jsOnglobalvarchanged_braces.substr(0, jsOnglobalvarchanged_braces.length - j);
				requestList[ObjectPath]["jsOnglobalvarchanged"] = jsOnglobalvarchanged_braces;
			}else{
				//we had an old version (pre svn 8224) of the cshmi where this variable was named jsOnglobalvarchange
				requestList[ObjectPath]["jsOnglobalvarchanged"] = "";
			}
			
			//we have asked the object successful, so remember the result
			this.ResourceList.Elements[ObjectPath] = new Object();
			this.ResourceList.Elements[ObjectPath].Parameters = requestList[ObjectPath];
		}
		
		var VisualObject = HMI.svgDocument.createElementNS(HMI.HMI_Constants.NAMESPACE_SVG, 'svg');
		
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
		
		var HTMLcontent = requestList[ObjectPath]["HTMLcontent"];
		var sourceList = requestList[ObjectPath]["sourceOfLibrary"];
		var jsOnload = requestList[ObjectPath]["jsOnload"];
		
		var HTMLcontentNode = null;
		if(HTMLcontent !== ""){
			//create foreignObject in <SVG>-Element
			var SVGWidth = VisualObject.getAttribute("width");
			var SVGHeight = VisualObject.getAttribute("height");
			var parentObject = null;
			if(this.useforeignObject){
				/* foreignObject has some problem right now:
				 * opera presto (outdated software): interaction with html forms are not possible
				 * chrome in spring 2015: https://code.google.com/p/chromium/issues/detail?id=463828 "canvas in positioned svg in html is drawn in 0,0 position"
				 * MS Edge in spring 2015: https://status.modern.ie/svgforeignobjectelement Feature not available, but Status: "In Development"
				 * Firefox in spring 2015: some small positioning glitches
				 * 
				 * So deactivating this for now
				 */
				var HTML = "<foreignObject x='0' y='0' width='"+SVGWidth+"' height='"+SVGHeight+"'>"+
					"<body xmlns='http://www.w3.org/1999/xhtml'>"+HTMLcontent+"</body></foreignObject>";
				
				var svgContent =	"<svg:svg xmlns:svg='"+HMI.HMI_Constants.NAMESPACE_SVG+"' xmlns='"+HMI.HMI_Constants.NAMESPACE_SVG+"' "
				+"xmlns:xlink='"+HMI.HMI_Constants.NAMESPACE_XLINK+"'>"
				+HTML
				+"</svg:svg>";
				HTMLcontentNode = HMI.HMIDOMParser.parse(svgContent);
				
				//append foreignObject to VisualObject
				parentObject = VisualObject;
				var classList = csHMIgetElementsByClassName(parentObject, 'autosize');
				var classListX = csHMIgetElementsByClassName(parentObject, 'autosizeX');
				var classListY = csHMIgetElementsByClassName(parentObject, 'autosizeY');
			}else{
				//build a new DIV in the pure html document
				HTMLcontentNode = document.createElement('div');
				HTMLcontentNode.id = VisualObject.id+"*Div";
				HTMLcontentNode.style.width = SVGWidth+"px";
				HTMLcontentNode.style.height = SVGHeight+"px";
				HTMLcontentNode.innerHTML = HTMLcontent;
				
				//the position will be later positioned to the Playground, so this is changed to "relative"
				HMI.Playground.style.position = "relative";
				HTMLcontentNode.style.position = "absolute";
				
				//append node to HTML document
				parentObject = HMI.Playground;
				classList = csHMIgetElementsByClassName(HTMLcontentNode, 'autosize');
				classListX = csHMIgetElementsByClassName(HTMLcontentNode, 'autosizeX');
				classListY = csHMIgetElementsByClassName(HTMLcontentNode, 'autosizeY');
			}
			
			//if Element with class "autosize/autosizeX/autosizeY" exists, adjust width&heigth/width/height taken from Client
			if (classList.length != 0) {
				for (var i = 0; i < classList.length; ++i) {
					//special adjustment for <canvas>-Element
					if (classList[i].tagName.toLowerCase() === "canvas") {
						classList[i].width = parseInt(SVGWidth, 10);
						classList[i].height = parseInt(SVGHeight, 10);
					}
					classList[i].style.width = SVGWidth+"px";
					classList[i].style.height = SVGHeight+"px";
				}		
			}
			if (classListX.length != 0) {
				for (var i = 0; i < classListX.length; ++i) {
					if (classListX[i].tagName.toLowerCase() === "canvas") {
						classListX[i].width = parsetInt(SVGWidth, 10);
					}
					classListX[0].style.width = SVGWidth+"px";
				}
			}
			if (classListY.length != 0) {
				for (var i = 0; i < classListY.length; ++i) {
					if (classListY[i].tagName.toLowerCase() === "canvas") {
						classListY[i].height = parsetInt(SVGHeight, 10);
					}
					classListY[0].style.height = SVGHeight+"px";
				}
			}
			parentObject.appendChild(HTMLcontentNode);
		}
		
		if(sourceList !== "" || jsOnload  !== "" || requestList[ObjectPath]["jsOnglobalvarchanged"] !== ""){
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
			
			function cshmimodelVariable(VisualObject, ObjectPath, varName) {
				this.varName = varName;
				this.getValue = function () {
					var getValueObjectPath = ObjectPath+"/"+varName+".value";
					return HMI.cshmi._getValue(VisualObject, getValueObjectPath);
				};
				this.setValue = function (newValue) {
					var setValueObjectPath = ObjectPath+"/"+varName;
					HMI.cshmi._setValueExecute(VisualObject, setValueObjectPath, newValue, null, false);
				};
			};
			
			/**
			 * js code in the blackbox should only use the object cshmimodel
			 */
			var cshmimodel = new Object();
			cshmimodel.SvgElement = VisualObject;
			cshmimodel.Modelpath = ObjectPath;
			cshmimodel.document = HMI.svgDocument;
			cshmimodel.window = window;
			cshmimodel.variables = new Object();
			
			if(HTMLcontentNode !== null){
				if(this.useforeignObject){
					//this is the html body node of the foreignObject
					cshmimodel.HtmlBody = HTMLcontentNode.firstChild.firstElementChild;
				}else{
					cshmimodel.HtmlBody = document.body;
					cshmimodel.document = document;
				}
			}else{
				cshmimodel.HtmlBody = null;
			}

			for(var i = 0; i < varNames.length; ++i){
				cshmimodel.variables[varNames[i]] = new cshmimodelVariable(VisualObject, ObjectPath, varNames[i]);
			}
			
			cshmimodel.instantiateTemplate = function(x, y, rotate, hideable, PathOfTemplateDefinition, FBReference, FBVariableReference, ConfigValues) {
				HMI.cshmi.ResourceList.Elements["tempPath"] = new Object();
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
				var VisualChildObject = HMI.cshmi._buildSvgGroup(VisualObject, "tempPath", false, null, false);
				VisualObject.appendChild(VisualChildObject);
				
				//calculate all offset parameter to be able to display visual feedback
				//needed now, because we append new components
				HMI.saveAbsolutePosition(VisualChildObject);
				
				//interprete onload Actions if we are already loaded
				if (HMI.cshmi.initStage === false){
					HMI.cshmi._interpreteOnloadCallStack();
				}
			};
			
			//KS functions
			
			/**
			 * @param path of object to query
			 * @param requestType = OT_DOMAIN type of KS Object to query ("OT_DOMAIN", "OT_VARIABLE", "OT_LINK" or "OT_ANY")
			 * @param requestOutput Array of interesting objects properties ("OP_NAME", "OP_TYPE", "OP_COMMENT", "OP_ACCESS", "OP_SEMANTIC", "OP_CREATIONTIME", "OP_CLASS" or "OT_ANY")
			 * @param cbfnc callback function for a async request
			 * @param responseFormat Mime-Type of requested response (probably "text/tcl", "text/ksx", "text/plain" used)
			 * @return "{fb_hmi1} {fb_hmi2} {fb_hmi3} {MANAGER} {fb_hmi4} {fb_hmi5}" or null or true (if callback used)
			 */
			cshmimodel.getEP = function(path, requestType, requestOutput, cbfnc, responseFormat) {
				return HMI.KSClient.getEP(path, requestType, requestOutput, cbfnc, !!cbfnc?true:false, responseFormat);
			};
			/**
			 * @param path of the variable to fetch, multiple path possible via an Array
			 * @param requestOutput Array of interesting objects properties ("OP_NAME", "OP_TYPE", "OP_VALUE", "OP_TIMESTAMP" or "OP_STATE")
			 * @param cbfnc callback function for a async request
			 * @param responseFormat Mime-Type of requested response (probably "text/tcl", "text/ksx", "text/plain" used)
			 * @return "{{/TechUnits/HMIManager}}", response: "{/TechUnits/Sheet1}" or "TksS-0042::KS_ERR_BADPATH {{/Libraries/hmi/Manager.instance KS_ERR_BADPATH}}"
			 */
			cshmimodel.getVar = function(path, requestOutput, cbfnc, responseFormat){
				return HMI.KSClient.getVar(path, requestOutput, cbfnc, !!cbfnc?true:false, responseFormat);
			};
			/**
			 * @param path of the variable to set
			 * @param {String} value to set (StringVec are Arrays)
			 * @param {String} type variable type (for example "KS_VT_STRING") to set, null if no change
			 * @param cbfnc callback function for a async request
			 * @param responseFormat Mime-Type of requested response (probably "text/tcl", "text/ksx", "text/plain" used)
			 * @return true, "" or null
			 */
			cshmimodel.setVar = function(path, value, type, cbfnc, responseFormat){
				return HMI.KSClient.setVar(path, value, type, cbfnc, !!cbfnc?true:false, responseFormat);
			};
			/**
			 * @param path of the object to rename
			 * @param newname (optional with full path) of the object
			 * @param cbfnc callback function for a async request
			 * @param responseFormat Mime-Type of requested response (probably "text/tcl", "text/ksx", "text/plain" used)
			 * @return true, "" or null
			 */
			cshmimodel.renameObjects = function(oldName, newName, cbfnc, responseFormat) {
				return HMI.KSClient.renameObject(oldName, newName, cbfnc, !!cbfnc?true:false, responseFormat);
			};
			/**
			 * @param path of the object to create
			 * @param classname full class name of the new object
			 * @@param cbfnc callback function for a async request
			 * @param responseFormat Mime-Type of requested response (probably "text/tcl", "text/ksx", "text/plain" used)
			 * @return true, "" or null
			 */
			cshmimodel.createObject = function(path, classname, cbfnc, responseFormat) {
				return HMI.KSClient.createObject(path, classname, cbfnc, !!cbfnc?true:false, responseFormat);
			};
			/**
			 * @param path ob the object to delete
			 * @@param cbfnc callback function for a async request
			 * @param responseFormat Mime-Type of requested response (probably "text/tcl", "text/ksx", "text/plain" used)
			 * @return true, "" or null
			 */
			cshmimodel.deleteObject = function(path, cbfnc, responseFormat) {
				return HMI.KSClient.deleteObject(path, cbfnc, !!cbfnc?true:false, responseFormat);
			};
			/**
			 * @param path of the object to create
			 * @param classname full class name of the new object
			 * @@param cbfnc callback function for a async request
			 * @param responseFormat Mime-Type of requested response (probably "text/tcl", "text/ksx", "text/plain" used)
			 * @return true, "" or null
			 */
			cshmimodel.linkObjects = function(pathA, pathB, portnameA, cbfnc, responseFormat) {
				return HMI.KSClient.linkObjects(pathA, pathB, portnameA, cbfnc, !!cbfnc?true:false, responseFormat);
			};
			/**
			 * @param path of the object to create
			 * @param classname full class name of the new object
			 * @@param cbfnc callback function for a async request
			 * @param responseFormat Mime-Type of requested response (probably "text/tcl", "text/ksx", "text/plain" used)
			 * @return true, "" or null
			 */
			cshmimodel.unlinkObjects = function(pathA, pathB, portnameA, cbfnc, responseFormat) {
				return HMI.KSClient.unlinkObjects(pathA, pathB, portnameA, cbfnc, !!cbfnc?true:false, responseFormat);
			};
			
			//hmi functions
			cshmimodel.log_info_onwebsite = function(text) {
				HMI.hmi_log_info_onwebsite(text);
			};
			cshmimodel.log_error_onwebsite = function(text) {
				HMI.hmi_log_onwebsite(text);
			};
			
			//helper functions
			/**
			 * returns the KS Response as an Array, or an empty Array
			 * if the optional argument recursionDepth is > 0,  
			 */
			cshmimodel.splitKsResponse = function(response, recursionDepth) {
				return HMI.KSClient.splitKsResponse(response, recursionDepth);
			};
			
			
			VisualObject.ResourceList = new Object();
			VisualObject.ResourceList.cshmimodel = cshmimodel;
		}
		
		var sourceListSplitted = sourceList.trim().split(" ");
		if(sourceListSplitted.length === 1 && sourceListSplitted[0] === ""){
			//remove the entry
			sourceListSplitted.pop();
		}
		for(var iterator = 0; iterator < sourceListSplitted.length; iterator++){
			if(sourceListSplitted[iterator].indexOf("CSHMIModelHost") !== -1){
				var hostnameWithPort = HMI.KSClient.getCommunicationPoint(HMI.KSClient.ResourceList.ModelHost + "/" + HMI.KSClient.ResourceList.ModelServer);
				if(hostnameWithPort){
					sourceListSplitted[iterator] = sourceListSplitted[iterator].replace("CSHMIModelHost", "http://"+hostnameWithPort);
				}
			}
		}
		
		//load scripts and call jsonload script on load (or direct, if no source was requested)
		var result = loadScriptUrls(
			sourceListSplitted, 
			function(){HMI.cshmi._executeScript(VisualObject, ObjectPath, jsOnload, "jsOnload")}, 
			false,
			null,
			null
		);
		
		return VisualObject;
	},

	/**
	 * executes JavaScript Code
	 * @param {SVGElement} VisualObject visual Object which is active Object
	 * @param {String} ObjectPath Path to this cshmi object containing the event/action/visualisation
	 * @param {String} evalcode JavaScript-Code to be executed via eval()
	 * @param {String} locationidentifier (could be jsOnload, or onglobalvarchanged)
	 */
	_executeScript: function(VisualObject, ObjectPath, evalcode, locationidentifier){
		//declare object 'cshmimodel' for further use [usage e.g.: 'cshmimodel.variables.<VARNAME>.getValue();']
		var cshmimodel = VisualObject.ResourceList.cshmimodel;
		cshmimodel.VisualObject = VisualObject;

		//evaluate JS-Code
		try {
			eval(evalcode);
		} catch (e) {
			HMI.hmi_log_onwebsite("JS Code in "+locationidentifier+" of "+ObjectPath+" throwed this error: " + e.message);
		}
		
		cshmimodel.VisualObject = null;
	},
	
	/***************************************************************************************************************
	 * builds SVG container, gets the parameter via KS
	 * @param {SVGElement} VisualParentObject visual Object which is parent to active Object
	 * @param {String} ObjectPath Path to this cshmi object containing the event/action/visualisation
	 * @param {bool} preventNetworkRequest the function should prevent network requests if possible
	 * @return {SVGElement} VisualObject the new constructed element or null
	 */
	
	//newwrite
	//alle buildSvg* in eine Funktion zusammenfassen, da sehr aehnlich. Mit this.ModellVariables...
	
	/**
	 * builds SVG line object, gets the parameter via KS
	 * @param {SVGElement} VisualParentObject visual Object which is parent to active Object
	 * @param {String} ObjectPath Path to this cshmi object containing the event/action/visualisation
	 * @param {bool} preventNetworkRequest the function should prevent network requests if possible
	 * @return {SVGElement} VisualObject the new constructed element or null
	 */
	_buildSvgLine: function(VisualParentObject, ObjectPath, preventNetworkRequest){
		var requestList = new Object();
		if(preventNetworkRequest === true){
			//skip this element
			return null;
		}else if (this.ResourceList.Elements[ObjectPath] !== undefined){
			//get the information if the config of the object is known through the turbo
			requestList[ObjectPath] = this.ResourceList.Elements[ObjectPath].Parameters;
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
		
		var VisualObject = HMI.svgDocument.createElementNS(HMI.HMI_Constants.NAMESPACE_SVG, 'line');
		
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
		if(preventNetworkRequest === true){
			//skip this element
			return null;
		}else if (this.ResourceList.Elements[ObjectPath] !== undefined){
			//get the information if the config of the object is known through the turbo
			requestList[ObjectPath] = this.ResourceList.Elements[ObjectPath].Parameters;
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
		
		var VisualObject = HMI.svgDocument.createElementNS(HMI.HMI_Constants.NAMESPACE_SVG, 'polyline');
		
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
		if(preventNetworkRequest === true){
			//skip this element
			return null;
		}else if (this.ResourceList.Elements[ObjectPath] !== undefined){
			//get the information if the config of the object is known through the turbo
			requestList[ObjectPath] = this.ResourceList.Elements[ObjectPath].Parameters;
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
		
		var VisualObject = HMI.svgDocument.createElementNS(HMI.HMI_Constants.NAMESPACE_SVG, 'polygon');
		
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
		if(preventNetworkRequest === true){
			//skip this element
			return null;
		}else if (this.ResourceList.Elements[ObjectPath] !== undefined){
			//get the information if the config of the object is known through the turbo
			requestList[ObjectPath] = this.ResourceList.Elements[ObjectPath].Parameters;
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
		
		var VisualObject = HMI.svgDocument.createElementNS(HMI.HMI_Constants.NAMESPACE_SVG, 'path');
		
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
		if(preventNetworkRequest === true){
			//skip this element
			return null;
		}else if (this.ResourceList.Elements[ObjectPath] !== undefined){
			//get the information if the config of the object is known through the turbo
			requestList[ObjectPath] = this.ResourceList.Elements[ObjectPath].Parameters;
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
		
		var VisualObject = HMI.svgDocument.createElementNS(HMI.HMI_Constants.NAMESPACE_SVG, 'text');
		
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
			VisualObject.setAttribute("dy", "0.6ex");
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
		if(preventNetworkRequest === true){
			//skip this element
			return null;
		}else if (this.ResourceList.Elements[ObjectPath] !== undefined){
			//get the information if the config of the object is known through the turbo
			requestList[ObjectPath] = this.ResourceList.Elements[ObjectPath].Parameters;
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
		
		var VisualObject = HMI.svgDocument.createElementNS(HMI.HMI_Constants.NAMESPACE_SVG, 'circle');
		
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
		if(preventNetworkRequest === true){
			//skip this element
			return null;
		}else if (this.ResourceList.Elements[ObjectPath] !== undefined){
			//get the information if the config of the object is known through the turbo
			requestList[ObjectPath] = this.ResourceList.Elements[ObjectPath].Parameters;
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
		
		var VisualObject = HMI.svgDocument.createElementNS(HMI.HMI_Constants.NAMESPACE_SVG, 'ellipse');
		
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
			var rotation = ((parseFloat(configArray["rotate"], 10)%360)+360)%360;
			VisualObject.setAttribute("transform", "rotate("+rotation.toString()+","+requestList[ObjectPath]["cx"]+","+requestList[ObjectPath]["cy"]+")");
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
		if(preventNetworkRequest === true){
			//skip this element
			return null;
		}else if (this.ResourceList.Elements[ObjectPath] !== undefined){
			//get the information if the config of the object is known through the turbo
			requestList[ObjectPath] = this.ResourceList.Elements[ObjectPath].Parameters;
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
		
		var VisualObject = HMI.svgDocument.createElementNS(HMI.HMI_Constants.NAMESPACE_SVG, 'rect');
		
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
		if(preventNetworkRequest === true){
			//skip this element
			return null;
		}else if (this.ResourceList.Elements[ObjectPath] !== undefined){
			//get the information if the config of the object is known through the turbo
			requestList[ObjectPath] = this.ResourceList.Elements[ObjectPath].Parameters;
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
			VisualObject = HMI.svgDocument.createElementNS(HMI.HMI_Constants.NAMESPACE_SVG, 'image');
			
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
		
		return VisualObject;
	},
	
	/**
	 * returns the FBReference
	 * @param {SVGElement} VisualObject Object to manipulate the visualisation
	 * @param ChildrenIterator of the iterator or null
	 * @return Path (or "") as .path and the Object as .object
	 */
	_getFBReference: function(VisualObject, ChildrenIterator){
		var TemplateObject = VisualObject;
		var result = {path:"", object:null};
		var FBRef = "";
		
		//a getVar currentChild has OP_VALUE and OP_NAME set
		if (ChildrenIterator && ChildrenIterator.currentChild !== undefined && ChildrenIterator.currentChild["OP_VALUE"] !== undefined ){
			//we are in an GetVar-iterator and want to read out a value from the currentchild
			//search FBReference of root Object
			do{
				//FBReference found
				if(TemplateObject.ResourceList && TemplateObject.ResourceList.FBReference){
					FBRef = TemplateObject.ResourceList.FBReference;
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
			if (ChildrenIterator.currentChild["OP_VALUE"].charAt(0) === "/"){
				// the iterated string begins with / so it is a fullpath (likely from a GetVar on an assoziation)
				
				// it could be a path to another server, so we must add the serverName
				result.path = ServerName+ChildrenIterator.currentChild["OP_VALUE"];
				result.object = TemplateObject;
			}
			
		//a getEP currentChild has OP_NAME set
		}else if (ChildrenIterator && ChildrenIterator.currentChild !== undefined && ChildrenIterator.currentChild["OP_NAME"] !== undefined ){
			//we are in an getEP-iterator and want to read out a value from the currentchild
			//search FBReference of root Object
			do{
				//FBReference found
				if(TemplateObject.ResourceList && TemplateObject.ResourceList.FBReference){
					FBRef = TemplateObject.ResourceList.FBReference;
					if (ChildrenIterator.currentChild["OP_ACCESS"] !== undefined && ChildrenIterator.currentChild["OP_ACCESS"].indexOf("KS_AC_PART") !== -1){
						//we have an OV-PART, so the separator is a dot
						result.path = FBRef+"."+ChildrenIterator.currentChild["OP_NAME"];
					}else{
						//we have no OV-PART, so the separator is a slash
						result.path = FBRef+"/"+ChildrenIterator.currentChild["OP_NAME"];
					}
					result.object = TemplateObject;
					break;
				}
			//loop upwards to find the Template object
			}while( (TemplateObject = TemplateObject.parentNode) && TemplateObject !== null && TemplateObject.namespaceURI == HMI.HMI_Constants.NAMESPACE_SVG);  //the = is no typo here!
		}else{
			//no active iterator, so plain FBReference
			do{
				if(TemplateObject.ResourceList && TemplateObject.ResourceList.FBReference){
					//the name of a Template was requested
					result.path = TemplateObject.ResourceList.FBReference;
					result.object = TemplateObject;
					break;
				}
			//loop upwards to find the Template object
			}while( (TemplateObject = TemplateObject.parentNode) && TemplateObject !== null && TemplateObject.namespaceURI == HMI.HMI_Constants.NAMESPACE_SVG);  //the = is no typo here!
		}
		return result;
	},
	
	/**
	 * sets svg attributes from an Array (visible, x, y, rotate, width, height, stroke, strokeWidth, fill, opacity possible)
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
		if(configArray["rotate"] !== undefined){
			VisualObject.setAttribute("data-rotate", configArray["rotate"]);
			if(configArray["rotate"] !== "0"){
				VisualObject.setAttribute("transform", "rotate("+configArray["rotate"]+")");
			}
		}else{
			VisualObject.setAttribute("data-rotate", "0");
		}
		if(configArray["x"] !== undefined && configArray["y"] !== undefined){
			//the attribute should be "rotate(deg, x, y)"
			VisualObject.setAttribute("x", configArray["x"]);
			VisualObject.setAttribute("y", configArray["y"]);
			if (configArray["rotate"] && configArray["rotate"] !== "0"){
				//rotation should be around x and y
				var rotation = ((parseFloat(configArray["rotate"], 10)%360)+360)%360;
				VisualObject.setAttribute("transform", "rotate("+rotation.toString()+","+configArray["x"]+","+configArray["y"]+")");
			}
		}
		if(configArray["width"] !== undefined && configArray["width"] !== ""){
			VisualObject.setAttribute("width", configArray["width"]);
		}
		if(configArray["height"] !== undefined && configArray["height"] !== ""){
			VisualObject.setAttribute("height", configArray["height"]);
		}
		if(configArray["stroke"] !== undefined && configArray["stroke"] !== ""){
			VisualObject.setAttribute("stroke", configArray["stroke"]);
		}
		if(configArray["strokeWidth"] !== undefined && configArray["strokeWidth"] !== ""){
			VisualObject.setAttribute("stroke-width", configArray["strokeWidth"]);
		}
		if(configArray["fill"] !== undefined && configArray["fill"] !== ""){
			VisualObject.setAttribute("fill", configArray["fill"]);
			if (configArray["fill"].indexOf("url(") !== -1){
				//opera has a Bug in May 2012 (v11+v12 beta), so the pointer-event is not correct in this case (buttons from processcontrol)
				VisualObject.setAttribute('pointer-events', 'visible');
			}
		}
		if(configArray["opacity"] !== undefined && configArray["opacity"] !== ""){
			VisualObject.setAttribute("opacity", configArray["opacity"]);
		}
		return true;
	},
	
	/** set x, y and/or rotate of an object. Parameters as null will be skipped
	 */
	_setXYRotate: function(VisualObject, x, y, rotate){
		//svg are not transformable, so the rotation/transform is in the objects parent
		if (VisualObject.tagName === "svg" && VisualObject.parentNode.tagName === "g" && VisualObject.parentNode.id === ""){
			//object has already an g parent
			var rotationObject = VisualObject.parentNode;
		}else if (rotate !== null && parseFloat(rotate) !== 0 //we have to rotate
				&& VisualObject.tagName === "svg" //svg are not rotatable itself
				&& VisualObject.parentNode.tagName !== "g" //we have no g element above
				&& VisualObject.parentNode.namespaceURI == HMI.HMI_Constants.NAMESPACE_SVG //but we are still in SVG
		){
			//element has to be shifted into an g element, as we change rotation
			rotationObject = HMI.svgDocument.createElementNS(HMI.HMI_Constants.NAMESPACE_SVG, 'g');
			rotationObject.setAttribute("overflow", "visible");
			rotationObject.setAttribute("x", "0");
			rotationObject.setAttribute("y", "0");
			VisualObject.parentNode.replaceChild(rotationObject, VisualObject);
			rotationObject.appendChild(VisualObject);
		}else{
			//normal visual element
			rotationObject = VisualObject;
		}
		if(x === null){
			if(VisualObject.hasAttribute("x")){
				x = VisualObject.getAttribute("x");
			}else if(VisualObject.hasAttribute("cx")){
				x = VisualObject.getAttribute("cx");
			}else{
				x = 0;
			}
		}else if(isNumeric(x)){
			if(VisualObject.hasAttribute("x")){
				VisualObject.setAttribute("x", x);
			}else if(VisualObject.hasAttribute("cx")){
				VisualObject.setAttribute("cx", x);
			}
		}else{
			x = 0;
		}
		if(y === null){
			if(VisualObject.hasAttribute("y")){
				y = VisualObject.getAttribute("y");
			}else if(VisualObject.hasAttribute("cy")){
				y = VisualObject.getAttribute("cy");
			}else{
				y = 0;
			}
		}else if(isNumeric(y)){
			if(VisualObject.hasAttribute("y")){
				VisualObject.setAttribute("y", y);
			}else if(VisualObject.hasAttribute("cy")){
				VisualObject.setAttribute("cy", y);
			}
		}else{
			y = 0;
		}
		if(rotate === null){
			rotate = getRotationFromObject(VisualObject);
		}else if(isNumeric(rotate)){
			//force numbers between 0 and 360
			rotate = ((rotate%360)+360)%360;
			VisualObject.setAttribute("data-rotate", rotate);
		}else{
			rotate = 0;
		}
		rotationObject.setAttribute("transform", "rotate("+rotate+","+x+","+y+")");
		
		//we want to have offset parameter on all visual elements
		HMI.saveAbsolutePosition(rotationObject);
	},
	
	/**
	 * Requests a list of OV-Variables from multiple OV-Objects
	 * @this main cshmi object
	 * @param {Array} requestList List of multiple Variables to fetch
	 * @param {Boolean} ignoreError true if an BADPATH should not trigger an warning
	 * @return {Boolean} true on success, false if an error occured
	 */
	_requestVariablesArray: function(requestList, ignoreError){
		var requestArray = new Array();
		var lastOvObjName = null;
		var VariableCount = 0;
		
		//collect all requested Variables
		for (var ovObjName in requestList) {
			for (var ksVarName in requestList[ovObjName]) {
				if(ovObjName === ""){
					//vendor is a special case as usual
					requestArray.push(ksVarName);
				}else if (lastOvObjName != ovObjName){
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
		if(VariableCount === 0){
			return true;
		}
		
		var response = HMI.KSClient.getVar(requestArray, "OP_VALUE", null);
		var responseClean;
		if (response === false || response === null){
			//communication error
			return false;
		}else if (response.indexOf("KS_ERR_BADPATH") !== -1 && ignoreError !== true){
			if(ignoreError !== false){
				HMI.hmi_log_onwebsite("Sorry, your cshmi server is not supported, because the base model was changed. Please upgrade to the newest cshmi library. Don't forget to backup your server into FBD.");
			}
			HMI.hmi_log_error("cshmi._requestVariablesArray of "+requestArray+" failed: "+response);
			return false;
		}else if (response.indexOf("KS_ERR") !== -1 && (response.match(/KS_ERR/g) || []).length - 1 === VariableCount){
			//TCL format gives one global KS_ERR and one for each error
			//"TksS-0335::KS_ERR_BADPATH {{/TechUnits/VesselCurrent.width KS_ERR_BADPATH} {.height KS_ERR_BADPATH} {.hideable KS_ERR_BADPATH}}"
			//"KS_ERR_BADPATH: error 404: path not found {0} {KS_ERR: bad path} {KS_ERR: bad path} {KS_ERR: bad path} {KS_ERR: bad path} {0} {0} {FALSE} {1}"
			
			//all our variables gave an error, nothing to recover
			HMI.hmi_log_error("cshmi._requestVariablesArray of "+requestArray+" failed: "+response);
			return false;
		}else if (response.indexOf("KS_ERR") !== -1){
			//only some requests had a problem. Try to extract the good values!
			var responseClean = response.substr(response.indexOf("{"));
		}else{
			responseClean = response;
		}
		var responseArray = HMI.KSClient.splitKsResponse(responseClean, 0, true);
		
		//fill request object with the result
		var i = 0;
		for (var ovObjName in requestList) {
			for (var ksVarName in requestList[ovObjName]) {
				if(responseArray[i].indexOf("KS_ERR") === -1){
					requestList[ovObjName][ksVarName] = responseArray[i];
				}
				i++;
			}
		}
		return true;
	},
	
	/**
	 * iterprets all children recursive and appends them to the parent
	 * @param {SVGElement} VisualParentObject visual Object which is parent to active Object
	 * @param {String} ObjectPath Path to this cshmi object containing the event/action/visualisation
	 * @param {bool} preventNetworkRequest the function should prevent network requests if possible
	 */
	_interpreteChildrensRecursive: function(VisualParentObject, ObjectPath, preventNetworkRequest){
		var responseArray = HMI.KSClient.getChildObjArray(ObjectPath, this);
		
		for (var i=0; i < responseArray.length; i++) {
			var varName = responseArray[i].split(" ");
			if(VisualParentObject.tagName === "g" && VisualParentObject.id === "" && VisualParentObject.firstChild && VisualParentObject.firstChild.id !== ""){
				//nested rotation template
				var realComponent = VisualParentObject.firstChild;
			}else{
				realComponent = VisualParentObject;
			}
			var ChildComponent = this._interpreteElementOrEventRecursive(realComponent, ObjectPath+"/"+varName[0], varName[1], preventNetworkRequest);
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
	_interpreteHiddenChildrenElements: function(VisualParentObject){
		if (HMI.instanceOf(VisualParentObject, this.cshmiGroupVisibleChildrenNotLoadedClass)){
			//dim the full sheet to show the progress
			HMI.Playground.firstChild.setAttribute("opacity", "0.6");
			
			//test if we have a template
			if (HMI.instanceOf(VisualParentObject, this.cshmiTemplateClass)){
				//load the elements of the template, allow all network request
				this._interpreteChildrensRecursive(VisualParentObject, VisualParentObject.getAttribute("data-TemplateModelSource"), false);
			}
			//load all graphical children of the object, allow all network request
			this._interpreteChildrensRecursive(VisualParentObject, VisualParentObject.getAttribute("data-ModelSource"), false);
			
			//we want to have offset parameter on all new visual elements
			HMI.saveAbsolutePosition(VisualParentObject);
			
			//interprete onload Actions if we are already loaded
			if (this.initStage === false){
				this._interpreteOnloadCallStack();
			}
			
			// mark the class as complete
			// the event registering requires the removing after loadChildren!
			HMI.removeClass(VisualParentObject, this.cshmiGroupVisibleChildrenNotLoadedClass);
			
			//deactivate the dim of the sheet, as we are ready
			HMI.Playground.firstChild.removeAttribute("opacity");
		}
	},
	
	/**
	 * interprete onloadCallStack
	 */
	_interpreteOnloadCallStack: function(){
		//for this objects, the init stage should be set (needed for getValue and timeevent)
		var oldStage = this.initStage;
		this.initStage = true;
		
		while(this.ResourceList.onloadCallStack.length !== 0){
			var EventObjItem = this.ResourceList.onloadCallStack.shift();
			if(EventObjItem["VisualObject"].ownerDocument === this.trashDocument){
				//the object was killed, so ignore
				continue;
			}
			this._interpreteAction(EventObjItem["VisualObject"], EventObjItem["ObjectPath"], null, null);
		}
		EventObjItem = null;
		
		this.initStage = oldStage;
		
		return;
	},
	
	/**
	 * toggles visibility of child Templates
	 * @param {Node} VisualParentObject Node which childs are toggled
	 * @return void
	 */
	toggleChildGroups: function(VisualParentObject){
		var childTemplates = VisualParentObject.childNodes;
		
		for (var i=0; i < childTemplates.length; i++) {
			var VisualObject = childTemplates[i];
			if (HMI.instanceOf(VisualObject, this.cshmiGroupHideableClass) === false){
				continue;
			}else if (VisualObject.getAttribute("display") == "block"){
				VisualObject.setAttribute("display", "none");
			}else{
				VisualObject.setAttribute("display", "block");
				
				//load hidden elements now
				this._interpreteHiddenChildrenElements(VisualObject);
				
				//doku depth of moving to top
				
				//Move Faceplate-Container after every other, so it is fully visible. This does not work in all cases...
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
				//INT or UINT, we have no way to show a shorter version... 8-/
				trimmedContent = NewText;
				this._setTitle(VisualObject, "");
			}else{
				//other values
				var power = Math.pow(10, trimToLength || 0);
				trimmedContent = String(Math.round(NewText * power) / power);
				
				this._setTitle(VisualObject, NewText);
			}
		}else if((trimToLength > 0) && (contentLength > trimToLength)){
			//shorten the string one char more, as we need space for the Ellipsis
			trimmedContent = NewText.substr(0, trimToLength-1);
			trimFromRight = true;
			this._setTitle(VisualObject, NewText);
		}else if((trimToLength < 0) && (contentLength > -trimToLength)){
			//shorten the string one char more, as we need space for the Ellipsis
			trimmedContent = NewText.substr(contentLength + trimToLength-1);
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
		VisualObject.setAttribute("data-trimToLength", trimToLength);
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
