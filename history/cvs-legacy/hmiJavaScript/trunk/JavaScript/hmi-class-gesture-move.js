/*
*	Copyright (C) 2007
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
*	CVS:
*	----
*	$Revision: 1.3 $
*	$Date: 2008-04-11 15:12:38 $
*
*	History:
*	--------
*	21-June-2007			St
*		-	General Revision
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
		element.addEventListener("mouseover", this.onMouseInThunk, capture);
	},

	/*********************************
		unregisterOnMouseIn
	*********************************/
	unregisterOnMouseIn: function (element, capture, listener) {
		element.removeEventListener("mouseover", this.onMouseInThunk, capture);
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
	this._innerCursorX	= null;
	this._innerCursorY	= null;
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
		element.addEventListener("mousedown", this.onMouseDownThunk, capture);
	},
	
	/*********************************
		unregisterOnMouseDown
	*********************************/
	unregisterOnMouseDown: function (element, capture, listener) {
		element.removeEventListener("mousedown", this.onMouseDownThunk, capture);
	},

	/*********************************
		onMouseDown
	*********************************/
	onMouseDownThunk: null,
	onMouseDown: function (evt) {
		this.startDrag(evt);
		evt.stopPropagation();
	},
	
	/*********************************
		registerOnMouseUp
	*********************************/
	registerOnMouseUp: function(element, capture, listener) {
		this.onMouseUpThunk = function (evt) { listener.onMouseUp(evt); };
		element.addEventListener("mouseup", this.onMouseUpThunk, capture);
	},
	
	/*********************************
		deregisterOnMouseUp
	*********************************/
	deregisterOnMouseUp: function (element, capture, listener) {
		element.removeEventListener("mouseup", this.onMouseUpThunk, capture);
	},
	
	/*********************************
		onMouseUp
	*********************************/
	onMouseUpThunk: null,
	onMouseUp: function (evt) {
		this.stopDrag(evt);
	},
	
	/*********************************
		registerOnMouseMove
	*********************************/
	registerOnMouseMove: function(element, capture, listener) {
		this.onMouseMoveThunk = function (evt) { listener.onMouseMove(evt); };
		element.addEventListener("mousemove", this.onMouseMoveThunk, capture);
	},
	
	/*********************************
		deregisterOnMouseMove
	*********************************/
	deregisterOnMouseMove: function (element, capture, listener) {
		element.removeEventListener("mousemove", this.onMouseMoveThunk, capture);
	},

	/*********************************
		onMouseMove
	*********************************/
	onMouseMoveThunk: null,
	onMouseMove: function (evt) {
		var dx = (evt.clientX - this._lastX);
		var dy = (evt.clientY - this._lastY);
		
		this._totalDX += dx;
		this._totalDY += dy;

		this._moveRelative(dx, dy);
		this._lastX = parseInt(evt.clientX);
		this._lastY = parseInt(evt.clientY);
		
		delete dx;
		delete dy;
	},
	
	/*********************************
		startDrag
	*********************************/
	startDrag: function (evt) {
		HMI.hmi_log_trace("Dragger.prototype.startDrag - Start");
		
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
			var SVGComponent = evt.target;
			while (	!(SVGComponent instanceof SVGSVGElement)
					&&	SVGComponent != null
					&&	SVGComponent != document
					&&	/\bhmi-component-gesture-move\b/.exec(SVGComponent.className.animVal) == null)
			{
				SVGComponent = SVGComponent.ownerSVGElement;
			};
			//	Save innerCursor-Position inside actual SVGComponent
			//
			this._innerCursorX = (evt.layerX - SVGComponent.getAttribute("layerX"));
			this._innerCursorY = (evt.layerY - SVGComponent.getAttribute("layerY"));
			
			if (HMI.RefreshTimeoutID != null)
				clearTimeout(HMI.RefreshTimeoutID);
			
			this._controller._currentDragger = this;
	
			this._lastX = parseInt(evt.clientX);
			this._lastY = parseInt(evt.clientY);
	
			this._totalDX = 0;
			this._totalDY = 0;
			
			if (this._node != this._node.parentNode.firstChild)
			{
				this._node.parentNode.insertBefore(this._node, this._node.parentNode.firstChild);
			};	
			
			var Node = this._node.cloneNode(true);
			Node.setAttribute('id', HMI.HMI_Constants.NODE_NAME_CLONE);
			Node.setAttribute('class', '');
			Node.setAttribute('fill-opacity', '0.25');
			Node.setAttribute('stroke-opacity', '0.25');
			Node.setAttribute('clonedID', this._node.getAttribute('id'));
			this._node.parentNode.appendChild(Node);
	
			this._node.setAttribute("pointer-events", "none");
			
			this.registerOnMouseMove(document, true, this);
			this.registerOnMouseUp(document, true, this);
			
			Node._xxx = null; delete Node._xxx; delete Node;
			SVGComponent._xxx = null; delete SVGComponent._xxx; delete SVGComponent;
		};		
		
		HMI.hmi_log_trace("Dragger.prototype.startDrag - End");
	},
	
	/*********************************
		stopDrag
	*********************************/
	stopDragThunk: function() {
	},
	stopDrag: function (evt) {		
		HMI.hmi_log_trace("Dragger.prototype.stopDrag - Start");
		
		var Clone = document.getElementById(HMI.HMI_Constants.NODE_NAME_CLONE);
		
		this._controller._currentDragger = null;
		
		this._node.setAttribute("pointer-events", "all");
		
		this.deregisterOnMouseUp(document, true, this);
		this.deregisterOnMouseMove(document, true, this);
		
		this.stopDragThunk();
		
		if (	this._totalDX != 0
			||	this._totalDY != 0)
		{
			if (this._ground != null)
			{
				//	MOVE / DRAG'N'DROP
				//
				Command = '{' +
					'{' + HMI.KSClient.getMessageID() + '} ' +
					'{010} ' +
					'{' + this._node.id + '} ' +
					'{MOVE} ' +
					'{' + this._ground._node.id +  '} ' +
					'{' + this._node.x.baseVal.value + '} ' +
					'{' + this._node.y.baseVal.value + '}' + '}';
				HMI.KSClient.sendRequest('setvar', 'POST', '{' + '/TechUnits/HMIManager' + '.Command ' + Command + '}', null);
			};
		} else {
			//	CLICK
			//
			if (HMI.instanceOf(this._node, 'hmi-component-gesture-click') == true)
			{
				Click.prototype.onClick(evt);
			};
		};
		
		Clone.parentNode.replaceChild(this._node, Clone);
		this._node.setAttribute('x', Clone.getAttribute('x'));
		this._node.setAttribute('y', Clone.getAttribute('y'));
		
		if (HMI.RefreshTimeoutID != null)
		{
			HMI.RefreshTimeoutID = setInterval('HMI.refreshSheet()', HMI.RefreshTime);
		};
		
		Clone._xxx = null; delete Clone._xxx; delete Clone;
		
		HMI.hmi_log_trace("Dragger.prototype.stopDrag - End");
	},
	
	/*********************************
		switchGround
	*********************************/
	switchGround: function (evt, ground) {
		HMI.hmi_log_trace("Dragger.prototype.switchGround - Start");
		//	impossible to be own ground
		//
		if (this._node == ground._node)
		{
			ground._node = ground._node.ownerSVGElement
			while (	ground._node != null
					&&	ground._node != document
					&&	HMI.instanceOf(ground._node, 'hmi-component-ground') == false)
			{
				ground._node = ground._node.ownerSVGElement;
				if (	ground._node != null
					&&	ground._node.id == HMI.HMI_Constants.NODE_NAME_CLONE)
				{
					ground._node = document.getElementById(ground._node.getAttribute('clonedID'));
				};
			};
		};
		
		//	first ground
		//
		if (this._ground == null)
		{			
			this._ground = ground;
			
			var node = this._node.parentNode.removeChild(this._node);
			ground._node.insertBefore(node, ground._node.firstChild);
			
			var SVGx = evt.layerX - node.ownerSVGElement.getAttribute("layerX") + (-1)*this._innerCursorX;
			var SVGy = evt.layerY - node.ownerSVGElement.getAttribute("layerY") + (-1)*this._innerCursorY;
			node.setAttribute("x", SVGx);
			node.setAttribute("y", SVGy);
			delete SVGx;
			delete SVGy;
			
			return;
		};

		//	new ground
		//
		if (this._ground != ground)
		{			
			if (this._ground._node.firstChild.localName == "g")
			{
				// firstChild == <g></g>
				//
				var node = this._ground._node.firstChild.removeChild(this._node);
				ground._node.firstChild.insertBefore(node, ground._node.firstChild.firstChild);
			} else {
				var node = this._ground._node.removeChild(this._node);
				ground._node.insertBefore(node, ground._node.firstChild);
			};
			
			var SVGx = evt.layerX - node.ownerSVGElement.getAttribute("layerX") + (-1)*this._innerCursorX;
			var SVGy = evt.layerY - node.ownerSVGElement.getAttribute("layerY") + (-1)*this._innerCursorY;
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
		var x = parseInt(this._node.getAttribute("x"));
		var y = parseInt(this._node.getAttribute("y"));
		
		x += dx;
		y += dy;

		this._node.setAttribute("x", x)
		this._node.setAttribute("y", y)
		
		delete x;
		delete y;
	},
	
	/*********************************
		_getLayerPosition
	*********************************/
	_getLayerPosition: function () {
		var SVGElement = node.ownerSVGElement;
		var SVGx = 0;
		var SVGy = 0;
		while (	SVGElement != null
				&&	SVGElement != document)
		{
			SVGx += SVGElement.x.animVal.value;
			SVGy += SVGElement.y.animVal.value;
			
			SVGElement = SVGElement.ownerSVGElement;
		};
		SVGx = evt.layerX - SVGx;
		SVGy = evt.layerY - SVGy;
		node.setAttribute("x", SVGx);
		node.setAttribute("y", SVGy);
		
		delete SVGElement;
		delete SVGx;
		delete SVGy;
	}
};