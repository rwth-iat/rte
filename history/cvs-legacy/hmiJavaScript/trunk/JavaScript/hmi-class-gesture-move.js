/*
*	Copyright (C) 2009
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
*	$Revision: 1.29 $
*	$Date: 2009-07-07 09:18:45 $
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
	this._totalDX			= null;
	this._totalDY			= null;
	
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
		//	DOUBLECLICK
		//
		if (	HMI.instanceOf(this._node, 'hmi-component-gesture-doubleclick') == true
			&&	evt.detail == 2
			&&	evt.button == 0)
		{
			DoubleClick.prototype.onDoubleClick(evt);
			return;
		};
		
		//	RIGHTCLICK
		//
		if (	HMI.instanceOf(this._node, 'hmi-component-gesture-rightclick') == true
			&&	evt.button == 2)
		{
			RightClick.prototype.onRightClick(evt);
			return;
		};	
		
		//	MOVE
		//
		if (evt.button == 0)
		{
			this.startDrag(evt);
			evt.stopPropagation();
		}
	},
	
	/*********************************
		registerOnMouseUp
	*********************************/
	registerOnMouseUp: function(element, capture, listener) {
		this.onMouseUpThunk = function (evt) { listener.onMouseUp(evt); };
		if("unknown" == typeof element.addEventListener || element.addEventListener){
			//Adobe Plugin, Renesis, Firefox, Safari, Opera...
			element.addEventListener("mouseup", this.onMouseUpThunk, capture);
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
			this.stopDrag(evt, false);
		}else{
			this.stopDrag(evt, true);
		}
	},
	
	/*********************************
		registerOnMouseMove
	*********************************/
	registerOnMouseMove: function(element, capture, listener) {
		this.onMouseMoveThunk = function (evt) { listener.onMouseMove(evt); };
		if("unknown" == typeof element.addEventListener || element.addEventListener){
			//Adobe Plugin, Renesis, Firefox, Safari, Opera...
			element.addEventListener("mousemove", this.onMouseMoveThunk, capture);
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
		//clientX is for the plugin, where clientX is based on the Plugin area, without browser scrolling sideeffects
		var dx = ((evt.pageX || evt.clientX) - this._lastX);
		var dy = ((evt.pageY || evt.clientY) - this._lastY);
		
		this._totalDX += dx;
		this._totalDY += dy;

		this._moveRelative(dx, dy);
		this._lastX = parseInt((evt.pageX || evt.clientX), 10);
		this._lastY = parseInt((evt.pageY || evt.clientY), 10);
		
		delete dx;
		delete dy;
	},
	
	/*********************************
		startDrag
	*********************************/
	startDrag: function (evt) {
		HMI.hmi_log_trace("Dragger.prototype.startDrag - Start");
		
		if (HMI.RefreshTimeoutID != null){
			window.clearInterval(HMI.RefreshTimeoutID);
			HMI.RefreshTimeoutID = null;
		}
		//mark that there is an active drag for HMI.switchGround
		this._controller._currentDragger = this;
		
		//initialize mouse starting position
		//clientX is for the plugin, where clientX is based on the Plugin area, without browser scrolling sideeffects
		this._lastX = parseInt((evt.pageX || evt.clientX), 10);
		this._lastY = parseInt((evt.pageY || evt.clientY), 10);
		
		this._totalDX = 0;
		this._totalDY = 0;
		
		//move dragNode to the back of SVG
		if (this._node != this._node.parentNode.firstChild)
		{
			this._node.parentNode.insertBefore(this._node, this._node.parentNode.firstChild);
		};
		
		//make a Clone and paint it with opacity
		var Node = this._node.cloneNode(true);
		Node.setAttribute('id', HMI.HMI_Constants.NODE_NAME_CLONE);
		Node.setAttribute('class', '');
		Node.setAttribute('fill-opacity', '0.25');
		Node.setAttribute('stroke-opacity', '0.25');
		Node.setAttribute('clonedID', this._node.getAttribute('id'));
		this._node.parentNode.appendChild(Node);
		
		//the dragged Node must not receive events while dragged
		this._node.setAttribute("pointer-events", "none");
		
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
		
		try{
			/**
			* Gecko does not garbage collect things correct in any cases.
			* The hack here is to reassign the additional properties attached to the
			* JS wrapper object in order to ensure it becomes dirty. Well,
			* considering that it becomes dirty from getting it from itself ...
			* I think this source code can't be exported to the US anymore
			* because of undecent language and probably thoughts.
			*/
			Node._xxx = null; delete Node._xxx;
		} catch (e) {   //IE does not like this hack
		}
		delete Node;
		
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
		if (validMove && (this._totalDX != 0
			||	this._totalDY != 0))
		{
			//every move has to have a ground as a target
			if (this._ground != null)
			{
				//	MOVE / DRAG'N'DROP
				//
				var xvalue = this._node.getAttribute("x");
				var yvalue = this._node.getAttribute("y");
				
				Command = '{' + HMI.KSClient.getMessageID() + '}%20' +
					'{010}%20' +
					'{' + this._node.id + '}%20' +
					'{MOVE}%20' +
					'{' + this._ground._node.id +  '}%20' +
					'{' + xvalue + '}%20' +
					'{' + yvalue + '}';
				HMI.KSClient.setVar(null, HMI.KSClient.HMIMANAGER_PATH + '.Command', Command, null);
				delete xvalue;
				delete yvalue;
			};
		} else if(!validMove){
			HMI.hmi_log_trace("Dragger.prototype.stopDrag - move aborted");
		} else {
			HMI.hmi_log_trace("Dragger.prototype.stopDrag - no movement => click");
			//	CLICK
			//
			if (HMI.instanceOf(this._node, 'hmi-component-gesture-click') == true)
			{
				Click.prototype.onClick(evt);
			};
		};
		
		// Renesis 1.1.1.0 does not clean Display, so this could be called with a empty Clone
		// http://tickets.examotion.com/public/view.php?id=83
		if (Clone && Clone.parentNode){
			Clone.parentNode.replaceChild(this._node, Clone);
			this._node.setAttribute('x', Clone.getAttribute('x'));
			this._node.setAttribute('y', Clone.getAttribute('y'));
		}
		
		if (HMI.RefreshTimeoutID == null){
			HMI.RefreshTimeoutID = window.setInterval(function () {HMI.refreshSheet();}, HMI.RefreshTime);
		}
		try{
			/**
			* Gecko does not garbage collect things correct in any cases.
			* The hack here is to reassign the additional properties attached to the
			* JS wrapper object in order to ensure it becomes dirty. Well,
			* considering that it becomes dirty from getting it from itself ...
			* I think this source code can't be exported to the US anymore
			* because of undecent language and probably thoughts.
			*/
			Clone._xxx = null; delete Clone._xxx;
		} catch (e) {   //IE does not like this hack
		}
		delete Clone;
		
		HMI.hmi_log_trace("Dragger.prototype.stopDrag - End");
	},
	
	/*********************************
		switchGround
	*********************************/
	switchGround: function (evt, ground) {
		HMI.hmi_log_trace("Dragger.prototype.switchGround - Start, Evt: "+evt.type+", Evt.id: "+evt.target.id+", Evt.nodeName: "+evt.target.nodeName+", Ground: "+ground._node.id);
		
		//detect the ground under the mouse
		
		//	impossible to be own ground
		//
		if (this._node == ground._node)
		{
			HMI.hmi_log_trace("Dragger.prototype.switchGround - own ground: "+ground._node.id);
			if (ground._node.ownerSVGElement != undefined){
				ground._node = ground._node.ownerSVGElement;
			}else if (ground._node.parentNode.namespaceURI == HMI.HMI_Constants.NAMESPACE_SVG ){
				ground._node = ground._node.parentNode;
			}
			while (	ground._node != null
					&&	ground._node != document
					&&	HMI.instanceOf(ground._node, 'hmi-component-ground') == false)
			{
				if (ground._node.ownerSVGElement != undefined){
					//firefox
					ground._node = ground._node.ownerSVGElement;
				}else if (ground._node.parentNode.namespaceURI == HMI.HMI_Constants.NAMESPACE_SVG ){
					//adobe + renesis
					ground._node = ground._node.parentNode;
				}
				if (	ground._node != null
					&&	ground._node.id == HMI.HMI_Constants.NODE_NAME_CLONE)
				{
					ground._node = HMI.svgDocument.getElementById(ground._node.getAttribute('clonedID'));
				};
			};
		};
		
		//	first ground
		//
		if (this._ground == null)
		{
			HMI.hmi_log_trace("Dragger.prototype.switchGround - first ground: "+ground._node.id);
			this._ground = ground;
			
			//LayerX and LayerY are HMI specific SVG Attributes!
			//They are ignored by the SVG Renderer but used for position calculation in the move gesture
			
			//new position is old coordinate + position of old parent - position of new parent (=ground)
			SVGx = parseInt(this._node.getAttribute("x"),10) + parseInt(this._node.parentNode.getAttribute("layerX"),10) - parseInt(ground._node.getAttribute("layerX"),10);
			SVGy = parseInt(this._node.getAttribute("y"),10) + parseInt(this._node.parentNode.getAttribute("layerY"),10) - parseInt(ground._node.getAttribute("layerY"),10);
			
			//move dragged Node into the new ground
			var node = this._node.parentNode.removeChild(this._node);
			ground._node.insertBefore(node, ground._node.firstChild);
			
			//set new position inside the ground
			if (SVGx != null){
				node.setAttribute("x", SVGx);
				node.setAttribute("y", SVGy);
			}
			delete SVGx;
			delete SVGy;
			
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
				//LayerX and LayerY are HMI specific SVG Attributes!
				//They are ignored by the SVG Renderer but used for position calculation in the move gesture
				
				//new position is old coordinate + position of old parent(=ground) - position of new parent (=ground)
				SVGx = parseInt(this._node.getAttribute("x"),10) + parseInt(this._ground._node.firstChild.getAttribute("layerX"),10) - parseInt(ground._node.getAttribute("layerX"),10);
				SVGy = parseInt(this._node.getAttribute("y"),10) + parseInt(this._ground._node.firstChild.getAttribute("layerY"),10) - parseInt(ground._node.getAttribute("layerY"),10);
				
				var node = this._ground._node.firstChild.removeChild(this._node);
				ground._node.firstChild.insertBefore(node, ground._node.firstChild.firstChild);
			} else {
				//new position is old coordinate + position of old parent(=ground) - position of new parent (=ground)
				SVGx = parseInt(this._node.getAttribute("x"),10) + parseInt(this._ground._node.getAttribute("layerX"),10) - parseInt(ground._node.getAttribute("layerX"),10);
				SVGy = parseInt(this._node.getAttribute("y"),10) + parseInt(this._ground._node.getAttribute("layerY"),10) - parseInt(ground._node.getAttribute("layerY"),10);
				
				var node = this._ground._node.removeChild(this._node);
				ground._node.insertBefore(node, ground._node.firstChild);
			};
			
			//set new position inside the ground
			node.setAttribute("x", SVGx);
			node.setAttribute("y", SVGy);
			delete SVGx;
			delete SVGy;
			
			this._ground = ground;
			
			delete node;
		};
		
		HMI.hmi_log_trace("Dragger.prototype.switchGround - End");
	},
	
	/*********************************
		_moveRelative
	*********************************/
	_moveRelative: function (dx, dy) {
		var x = parseInt(this._node.getAttribute("x"), 10);
		var y = parseInt(this._node.getAttribute("y"), 10);
		
		x += dx;
		y += dy;
		
		this._node.setAttribute("x", x)
		this._node.setAttribute("y", y)
		
		delete x;
		delete y;
	}
};
var filedate = "$Date: 2009-07-07 09:18:45 $";
filedate = filedate.substring(7, filedate.length-2);
if ("undefined" == typeof HMIdate){
	HMIdate = filedate;
}else if (HMIdate < filedate){
	HMIdate = filedate;
}
