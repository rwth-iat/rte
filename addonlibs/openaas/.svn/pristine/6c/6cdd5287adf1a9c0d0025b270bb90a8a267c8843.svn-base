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
*	hmi-class-gesture-move.js
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
*	25-February-2009			Je
*		-	General Revision and full commented
*
***********************************************************************/

/***********************************************************************
	constructor
		node: POINTER on dom / svg node that is used as ground.
		controller: POINTER on js object that acts as library controller
***********************************************************************/

function Ground (node, controller) {
	this._node = node;
	this._controller = controller;
	
	this.dropcursorStyle = "url(data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAA4AAAAOBAMAAADtZjDiAAAAAXNSR0IArs4c6QAAAARnQU1BAACxjwv8YQUAAAAgY0hSTQAAeiYAAICEAAD6AAAAgOgAAHUwAADqYAAAOpgAABdwnLpRPAAAADBQTFRF////M2bMP2/PS3jSVoHUYorXbpPaepvdhqTgka3inLblqL/owNDuzNnx1+Lz4+v2CpBZCQAAAAF0Uk5TAEDm2GYAAABRSURBVAhbY2AAgZXzwdQTkQsgilexAMy9JAYk+Bn4De2B9FEGNsEDQLpxwkaQMIOhlKEciBYUFAwA0RsFBTeAaG5BIbBmBogyoIAUhGbYCyIAUTsNEExoQVIAAAAASUVORK5CYII=) 5 12, crosshair";
	
	this.arm();
};

/***********************************************************************
	prototype
***********************************************************************/

Ground.prototype = {
	
	/*********************************
		arm
	*********************************/
	arm: function () {
		this.registerOnMouseIn(this._node, false, this);
	},
	
	/*********************************
		disarm
	*********************************/
	disarm: function () {
		this.unregisterOnMouseIn(this._node, false, this);
	},
	
	/*********************************
		registerOnMouseIn
	*********************************/
	registerOnMouseIn: function(element, capture, listener) {
		this.onMouseInThunk = function (evt) {
			listener.onMouseIn(evt);
		};
		if("unknown" == typeof element.addEventListener || element.addEventListener){
			//Adobe Plugin, Renesis, Firefox, Safari, Opera...
			element.addEventListener("mouseover", this.onMouseInThunk, capture);
		}else if (element.attachEvent){
			//Native IE
			element.attachEvent("onmouseover", this.onMouseInThunk);
		}
	},

	/*********************************
		unregisterOnMouseIn
	*********************************/
	unregisterOnMouseIn: function (element, capture, listener) {
		if("unknown" == typeof element.removeEventListener || element.removeEventListener){
			//Adobe Plugin, Renesis, Firefox, Safari, Opera...
			element.removeEventListener("mouseover", this.onMouseInThunk, capture);
		}else if (element.attachEvent){
			//Native IE
			element.detachEvent("onmouseover", this.onMouseInThunk);
		}
	},

	/*********************************
		onMouseIn
	*********************************/
	onMouseInThunk: null,
	onMouseIn: function (evt) {
		this._node = HMI.getComponent(evt, 'hmi-component-ground');
		this._controller.switchGround(evt, this);
		//kill handling of this event for gesture at a parentNode
		//cancelBubble for IE Bubbling with native IE handling (not used)
		//stopPropagation for W3C Bubbling
		evt.cancelBubble = true;
		if (evt.stopPropagation) evt.stopPropagation();
	}
};

/***********************************************************************
	constructor
		node: Pointer on DOM node / SVG root-node of draggable object
		controller: Pointer in JS-Object / root controller
***********************************************************************/

function Dragger (node, controller) {
	//	Internal Variables
	//
	this._ground			= null;
	this._lastX				= null;
	this._lastY				= null;
	this._totalDX			= 0;
	this._totalDY			= 0;
	
	this._node = node;
	this._controller = controller;
	
	//	arm Dragger
	//
	this.arm();
};

/***********************************************************************
	prototype
***********************************************************************/

Dragger.prototype = {
	/*********************************
		arm
	*********************************/
	arm: function () {
		this.registerOnMouseDown(this._node, false, this);
		this._node.setAttribute("cursor", "move");
	},

	/*********************************
		registerOnMouseDown
	*********************************/
	registerOnMouseDown: function(element, capture, listener) {
		this.onMouseDownThunk = function (evt) {
			listener.onMouseDown(evt);
		};
		if("unknown" == typeof element.addEventListener || element.addEventListener){
			//Adobe Plugin, Renesis, Firefox, Safari, Opera...
			element.addEventListener("mousedown", this.onMouseDownThunk, capture);
			
			//	todo:touch device developing:
			//element.addEventListener("touchstart", this.onMouseDownThunk, capture);
		}else if (element.attachEvent){
			//Native IE
			element.attachEvent("onmousedown", this.onMouseDownThunk);
		}
	},
	
	/*********************************
		unregisterOnMouseDown
	*********************************/
	unregisterOnMouseDown: function (element, capture, listener) {
		if("unknown" == typeof element.removeEventListener || element.removeEventListener){
			//Adobe Plugin, Renesis, Firefox, Safari, Opera...
			element.removeEventListener("mousedown", this.onMouseDownThunk, capture);
		}else if (element.attachEvent){
			//Native IE
			element.detachEvent("onmousedown", this.onMouseDownThunk);
		}
	},

	/*********************************
		onMouseDown
	*********************************/
	onMouseDownThunk: null,
	onMouseDown: function (evt) {
		//	todo:touch device developing:
		/*
		if (evt.type == "touchstart"){
			HMI.hmi_log_trace("Touch event detected, legacy events are disabled.");
			this.unregisterOnMouseDown(this._node, false, this);
		}
		*/
		//	DOUBLECLICK
		//
		if (	evt.detail === 2
			&&	evt.button === 0
			&&	HMI.instanceOf(this._node, 'hmi-component-gesture-doubleclick') === true)
		{
			DoubleClick.prototype.onDoubleClick(evt);
			return;
		};
		
		//	RIGHTCLICK
		//
		if (	evt.button == 2
			&&	HMI.instanceOf(this._node, 'hmi-component-gesture-rightclick') === true)
		{
			RightClick.prototype.onRightClick(evt);
			return;
		};
		//	CLICK and TEXTINPUT is handled in StopDrag, when no movement is detected
		
		//	MOVE
		//
		if (evt.button === 0 || evt.button === undefined)	//	touch devices does not have a button
		{
			this.startDrag(evt);
			//kill handling of this event for gesture at a parentNode
			//cancelBubble for IE Bubbling with native IE handling (not used)
			//stopPropagation for W3C Bubbling
			evt.cancelBubble = true;
			if (evt.stopPropagation) evt.stopPropagation();
		}
		
		//	todo:touch device developing:
		// no scroll on iPhone
		//return false;
	},
	
	/*********************************
		registerOnMouseUp
	*********************************/
	registerOnMouseUp: function(element, capture, listener) {
		this.onMouseUpThunk = function (evt) { listener.onMouseUp(evt); };
		if("unknown" == typeof element.addEventListener || element.addEventListener){
			//Adobe Plugin, Renesis, Firefox, Safari, Opera...
			element.addEventListener("mouseup", this.onMouseUpThunk, capture);
			//	todo:touch device developing:
			//element.addEventListener("touchend", this.onMouseUpThunk, capture);
		}else if (element.attachEvent){
			//Native IE
			element.attachEvent("onmouseup", this.onMouseUpThunk);
		}
	},
	
	/*********************************
		deregisterOnMouseUp
	*********************************/
	deregisterOnMouseUp: function (element, capture, listener) {
		if("unknown" == typeof element.removeEventListener || element.removeEventListener){
			//Adobe Plugin, Renesis, Firefox, Safari, Opera...
			element.removeEventListener("mouseup", this.onMouseUpThunk, capture);
			//	todo:touch device developing:
			//element.removeEventListener("touchend", this.onMouseUpThunk, capture);
		}else if (element.attachEvent){
			//Native IE
			element.detachEvent("onmouseup", this.onMouseUpThunk);
		}
	},
	
	/*********************************
		onMouseUp
	*********************************/
	onMouseUpThunk: null,
	onMouseUp: function (evt) {
		if (evt.button == 2){
			//right mouse button cancels move
			this.stopDrag(evt, false);
		}else{
			//left mouse button => valid move
			this.stopDrag(evt, true);
		}
		//kill handling of this event for gesture at a parentNode
		//cancelBubble for IE Bubbling with native IE handling (not used)
		//stopPropagation for W3C Bubbling
		evt.cancelBubble = true;
		if (evt.stopPropagation) evt.stopPropagation();
		
		//	todo:touch device developing:
		// no scroll on iPhone
		//return false;
	},
	
	/*********************************
		registerOnMouseMove
	*********************************/
	registerOnMouseMove: function(element, capture, listener) {
		this.onMouseMoveThunk = function (evt) { listener.onMouseMove(evt); };
		if("unknown" == typeof element.addEventListener || element.addEventListener){
			//Adobe Plugin, Renesis, Firefox, Safari, Opera...
			element.addEventListener("mousemove", this.onMouseMoveThunk, capture);
			//	todo:touch device developing:
			//element.addEventListener("touchmove", this.onMouseMoveThunk, capture);
		}else if (element.attachEvent){
			//Native IE
			element.attachEvent("onmousemove", this.onMouseMoveThunk);
		}
	},
	
	/*********************************
		deregisterOnMouseMove
	*********************************/
	deregisterOnMouseMove: function (element, capture, listener) {
		if("unknown" == typeof element.removeEventListener || element.removeEventListener){
			//Adobe Plugin, Renesis, Firefox, Safari, Opera...
			element.removeEventListener("mousemove", this.onMouseMoveThunk, capture);
			//	todo:touch device developing:
			//element.removeEventListener("touchmove", this.onMouseMoveThunk, capture);
		}else if (element.detachEvent){
			//Native IE
			element.detachEvent("onmousemove", this.onMouseMoveThunk);
		}
	},

	/*********************************
		onMouseMove
	*********************************/
	onMouseMoveThunk: null,
	onMouseMove: function (evt) {
		HMI.hmi_log_trace("Dragger.prototype.onMouseMove active");
		
		var newX;
		var newY;
		//	todo:touch device developing:
		/*
		if (evt.touches && evt.touches.length) { 	// iPhone
			newX = evt.touches[0].clientX;
			newY = evt.touches[0].clientY;
		}else 
		*/
		if(evt.pageX !== undefined){
			newX = evt.pageX;
			newY = evt.pageY;
		}else{
			//clientX is for the plugin, where clientX is based on the Plugin area, without browser scrolling sideeffects
			newX = evt.clientX;
			newY = evt.clientY;
		}
		
		var dx = (newX - this._lastX);
		var dy = (newY - this._lastY);
		
		this._totalDX += dx;
		this._totalDY += dy;
		
		this._moveRelative(dx, dy);
		this._lastX = parseInt(newX, 10);
		this._lastY = parseInt(newY, 10);
		
		dx = null;
		dy = null;
	},
	
	/*********************************
		startDrag
	*********************************/
	startDrag: function (evt) {
		HMI.hmi_log_trace("Dragger.prototype.startDrag - Start");
		if (HMI.RefreshTimeoutID !== null){
			window.clearTimeout(HMI.RefreshTimeoutID);
			HMI.RefreshTimeoutID = null;
		}
		//mark that there is an active drag for HMI.switchGround
		this._controller._currentDragger = this;
		
		//initialize mouse starting position
		//clientX is for the plugin, where clientX is based on the Plugin area, without browser scrolling sideeffects
		
		var newX;
		var newY;
		//	todo:touch device developing:
		/*
		if (evt.touches && evt.touches.length) { 	// iPhone
			newX = evt.touches[0].clientX;
			newY = evt.touches[0].clientY;
		}else 
		*/
		if(evt.pageX !== undefined){
			newX = evt.pageX;
			newY = evt.pageY;
		}else{
			//clientX is for the plugin, where clientX is based on the Plugin area, without browser scrolling sideeffects
			newX = evt.clientX;
			newY = evt.clientY;
		}
		
		this._lastX = parseInt(newX, 10);
		this._lastY = parseInt(newY, 10);
		
		this._totalDX = 0;
		this._totalDY = 0;
		
		//the dragged Node and cloned node must not receive events while dragged
		this._node.setAttribute('pointer-events', 'none');
		
		//make a Clone, place it under on the position of original and paint it with opacity
		var Node = this._node.cloneNode(true);
		Node.id = HMI.HMI_Constants.NODE_NAME_CLONE;
		Node.setAttribute('class', '');
		Node.setAttribute('opacity', '0.25');
		Node.setAttribute('clonedID', this._node.id);
		this._node.parentNode.insertBefore(Node, this._node);
		
		//move dragNode to the front of SVG, so it is visible in all cases
		if (this._node != this._node.parentNode.lastChild)
		{
			this._node.parentNode.appendChild(this._node);
		};
		
		if (HMI.svgDocument.addEventListener){
			//Firefox and co
			this.registerOnMouseMove(HMI.svgDocument, true, this);
			this.registerOnMouseUp(HMI.svgDocument, true, this);
		}else if("unknown" == typeof HMI.svgDocument.documentElement.addEventListener){
			//adobe plugin
			this.registerOnMouseMove(HMI.svgDocument.documentElement, false, this);
			this.registerOnMouseUp(HMI.svgDocument.documentElement, false, this);
		}else if(HMI.svgDocument.documentElement.addEventListener){
			//Renesis Plugin
			this.registerOnMouseMove(HMI.svgDocument.documentElement, false, this);
			this.registerOnMouseUp(HMI.svgDocument.documentElement, false, this);
		}
		Node = null;
		
		HMI.hmi_log_trace("Dragger.prototype.startDrag - End");
	},
	
	/*********************************
		stopDrag
	*********************************/
	stopDrag: function (evt, validMove) {		
		HMI.hmi_log_trace("Dragger.prototype.stopDrag - Start");
		
		var Clone = HMI.svgDocument.getElementById(HMI.HMI_Constants.NODE_NAME_CLONE);
		//disable drag for HMI.switchGround
		this._controller._currentDragger = null;
		
		this._node.setAttribute("pointer-events", "all");
		
		if (HMI.svgDocument.addEventListener){
			//Firefox and co
			this.deregisterOnMouseUp(document, true, this);
			this.deregisterOnMouseMove(document, true, this);
		}else if("unknown" == typeof HMI.svgDocument.documentElement.removeEventListener){
			//adobe plugin
			this.deregisterOnMouseMove(HMI.svgDocument.documentElement, false, this);
			this.deregisterOnMouseUp(HMI.svgDocument.documentElement, false, this);
		}else if(HMI.svgDocument.documentElement.removeEventListener){
			//Renesis Plugin
			this.deregisterOnMouseMove(HMI.svgDocument.documentElement, false, this);
			this.deregisterOnMouseUp(HMI.svgDocument.documentElement, false, this);
		}
		
		//was there a move?
		if (validMove && (this._totalDX !== 0
			||	this._totalDY !== 0))
		{
			//every move has to have a ground as a target
			if (this._ground !== null)
			{
				//	MOVE / DRAG'N'DROP
				//
				var xvalue = this._node.getAttribute("x");
				var yvalue = this._node.getAttribute("y");
				
				Command = [HMI.KSClient.getMessageID(),
				           '010',
				           encodeURI(this._node.id),
				           'MOVE',
				           encodeURI(this._ground._node.id),
				           xvalue,
				           yvalue];
				xvalue = null;
				yvalue = null;
				
				HMI.KSClient.setVar(HMI.KSClient.HMIMANAGER_PATH + '.Command', Command, null, HMI.cbrefreshSheet);
			};
		} else if(!validMove){
			HMI.hmi_log_trace("Dragger.prototype.stopDrag - move aborted");
		} else {
			//	CLICK
			//
			if (HMI.instanceOf(this._node, 'hmi-component-gesture-click') === true)
			{
				HMI.hmi_log_trace("Dragger.prototype.stopDrag - no movement => click");
				//call internal function to use correct component (evt.target is likely to be wrong in this mouseup-event)
				//
				Click.prototype._sendCommand(evt, this._node);
			}else
			//	TEXTINPUT
			//
			if (HMI.instanceOf(this._node, 'hmi-component-gesture-textinput') === true)
			{
				HMI.hmi_log_trace("Dragger.prototype.stopDrag - no movement => textinput");
				//call internal function to use correct component (evt.target is likely to be wrong in this mouseup-event)
				//
				var input = TextInput.prototype.getTextContent(evt);
				if(	input	!== null
					&&	input	!== undefined)
				{
					TextInput.prototype._sendCommand(evt, this._node, input);
				}
				input = null;
			}else{
				HMI.hmi_log_trace("Dragger.prototype.stopDrag - no movement and no clickhandler on this element");
			}
		}
		
		//clear special cursor on ground
		if (this._ground !== null){
			this._ground._node.setAttribute("cursor", "");
		}
		
		if (Clone && Clone.parentNode){
			Clone.parentNode.replaceChild(this._node, Clone);
			this._node.setAttribute('x', Clone.getAttribute('x'));
			this._node.setAttribute('y', Clone.getAttribute('y'));
		}
		Clone = null;
		HMI.reactivateRefreshInterval(evt);
		
		HMI.hmi_log_trace("Dragger.prototype.stopDrag - End");
	},
	
	/*********************************
		switchGround
	*********************************/
	switchGround: function (evt, ground) {
		HMI.hmi_log_trace("Dragger.prototype.switchGround - Start, Evt: "+evt.type+", Evt.id: "+evt.target.id+", Evt.TagName: "+evt.target.TagName+", Ground: "+ground._node.id);
		
		var node;
		var SVGx = Number.NaN;
		var SVGy = Number.NaN;
		
		//detect the ground under the mouse
		
		//	impossible to be own ground
		//
		if (this._node == ground._node)
		{
			HMI.hmi_log_trace("Dragger.prototype.switchGround - own ground: "+ground._node.id);
			if (ground._node.ownerSVGElement !== undefined){
				ground._node = ground._node.ownerSVGElement;
			}else if (ground._node.parentNode.namespaceURI == HMI.HMI_Constants.NAMESPACE_SVG ){
				ground._node = ground._node.parentNode;
			}
			while (	ground._node !== null
					&&	ground._node != document
					&&	HMI.instanceOf(ground._node, 'hmi-component-ground') === false)
			{
				if (ground._node.ownerSVGElement !== undefined){
					//firefox
					ground._node = ground._node.ownerSVGElement;
				}else if (ground._node.parentNode.namespaceURI == HMI.HMI_Constants.NAMESPACE_SVG ){
					//adobe + renesis
					ground._node = ground._node.parentNode;
				}
				if (	ground._node !== null
					&&	ground._node.id == HMI.HMI_Constants.NODE_NAME_CLONE)
				{
					ground._node = HMI.svgDocument.getElementById(ground._node.getAttribute('clonedID'));
				};
			};
		};
		
		//	first ground
		//
		if (this._ground === null)
		{
			HMI.hmi_log_trace("Dragger.prototype.switchGround - first ground: "+ground._node.id);
			ground._node.style.cursor = ground.dropcursorStyle;
			if (ground._node.style.cursor.length < 50){
					ground._node.style.cursor = "crosshair";
			}
			this._ground = ground;
			
			//absolutex and absolutey are HMI specific DOM Attributes!
			//They are ignored by the SVG Renderer but used for position calculation in the move gesture
			
			//new position is old coordinate + position of old parent - position of new parent (=ground)
			SVGx = parseInt(this._node.getAttribute("x"),10) + parseInt(this._node.parentNode.getAttribute("absolutex"),10) - parseInt(ground._node.getAttribute("absolutex"),10);
			SVGy = parseInt(this._node.getAttribute("y"),10) + parseInt(this._node.parentNode.getAttribute("absolutey"),10) - parseInt(ground._node.getAttribute("absolutey"),10);
			
			//move dragged Node into the new ground
			node = this._node.parentNode.removeChild(this._node);
			ground._node.appendChild(node);
			
			//set new position inside the ground
			if (!isNaN(SVGx) && !isNaN(SVGy)){
				node.setAttribute("x", SVGx);
				node.setAttribute("y", SVGy);
			}
			SVGx = null;
			SVGy = null;
			
			HMI.hmi_log_trace("Dragger.prototype.switchGround - first ground - End");
			return;
		};

		//	new ground
		//
		if (this._ground != ground)
		{	
			HMI.hmi_log_trace("Dragger.prototype.switchGround - new ground: "+ground._node.id);
			if (this._ground._node.firstChild.localName == "g")
			{
				// firstChild == <g></g>
				//
				//absolutex and absolutey are HMI specific DOM Attributes!
				//They are ignored by the SVG Renderer but used for position calculation in the move gesture
				
				//new position is old coordinate + position of old parent(=ground) - position of new parent (=ground)
				SVGx = parseInt(this._node.getAttribute("x"),10) + parseInt(this._ground._node.firstChild.getAttribute("absolutex"),10) - parseInt(ground._node.getAttribute("absolutex"),10);
				SVGy = parseInt(this._node.getAttribute("y"),10) + parseInt(this._ground._node.firstChild.getAttribute("absolutey"),10) - parseInt(ground._node.getAttribute("absolutey"),10);
				
				node = this._ground._node.firstChild.removeChild(this._node);
				ground._node.firstChild.appendChild(node);
			} else {
				//new position is old coordinate + position of old parent(=ground) - position of new parent (=ground)
				SVGx = parseInt(this._node.getAttribute("x"),10) + parseInt(this._ground._node.getAttribute("absolutex"),10) - parseInt(ground._node.getAttribute("absolutex"),10);
				SVGy = parseInt(this._node.getAttribute("y"),10) + parseInt(this._ground._node.getAttribute("absolutey"),10) - parseInt(ground._node.getAttribute("absolutey"),10);
				
				try{
					node = this._ground._node.removeChild(this._node);
					ground._node.appendChild(node);
				}catch(e){
					HMI.hmi_log_warning("Dragger.prototype.switchGround - ground status confused.");
				}
			};
			
			//set new position inside the ground
			if (!isNaN(SVGx) && !isNaN(SVGy) && node !== undefined){
				node.setAttribute("x", SVGx);
				node.setAttribute("y", SVGy);
				
				SVGx = null;
				SVGy = null;
				
				this._ground._node.style.cursor = "";
				ground._node.style.cursor = ground.dropcursorStyle;
				if (ground._node.style.cursor.length < 50){
					ground._node.style.cursor = "crosshair";
				}
				this._ground = ground;
			}
			
			node = null;
		};
		
		HMI.hmi_log_trace("Dragger.prototype.switchGround - End");
	},
	
	/*********************************
		_moveRelative
	*********************************/
	_moveRelative: function (dx, dy) {
		var x = parseInt(this._node.getAttribute("x"), 10);
		var y = parseInt(this._node.getAttribute("y"), 10);
		
		if (isNaN(x)){
			x = 0;
		}
		if (isNaN(x)){
			y = 0;
		}
		
		x += dx;
		y += dy;
		
		this._node.setAttribute("x", x);
		this._node.setAttribute("y", y);
		
		x = null;
		y = null;
	}
};
var filedate = "$Date$";
filedate = filedate.substring(7, filedate.length-2);
if ("undefined" == typeof HMIdate){
	HMIdate = filedate;
}else if (HMIdate < filedate){
	HMIdate = filedate;
}
