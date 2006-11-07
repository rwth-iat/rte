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
		var SVGComponent = evt.target;
		
		while (	!(SVGComponent instanceof SVGSVGElement)
				&&	SVGComponent != null
				&&	SVGComponent != document
				&&	/\bhmi-component-gesture-move\b/.exec(SVGComponent.className.animVal) == null)
		{
			SVGComponent = SVGComponent.ownerSVGElement;
		};
		this._innerCursorX = (evt.layerX - SVGComponent.getAttribute("layerX"));
		this._innerCursorY = (evt.layerY - SVGComponent.getAttribute("layerY"));
		//this._innerCursorX = (evt.layerX - SVGComponent.x.animVal.value);
		//this._innerCursorY = (evt.layerY - SVGComponent.y.animVal.value);

		this.startDrag(evt);
		
		evt.stopPropagation();
		
		delete SVGComponent;
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
		
		if (evt.button == 0)
		{	
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
			Node.setAttribute('id', HMI.CONSTANTS.NODE_NAME_CLONE);
			Node.setAttribute('class', '');
			Node.setAttribute('fill-opacity', '0.25');
			Node.setAttribute('stroke-opacity', '0.25');
			Node.setAttribute('clonedID', this._node.getAttribute('id'));
			this._node.parentNode.appendChild(Node);
	
			this._node.setAttribute("pointer-events", "none");
			
			this.registerOnMouseMove(document, true, this);
			this.registerOnMouseUp(document, true, this);
			
			delete Node;
		};
	},
	
	/*********************************
		stopDrag
	*********************************/
	stopDragThunk: function() {
	},
	stopDrag: function (evt) {
		var Clone = document.getElementById(HMI.CONSTANTS.NODE_NAME_CLONE);
		
		this._controller._currentDragger = null;
		
		this._node.setAttribute("pointer-events", "all");
		
		this.deregisterOnMouseUp(document, true, this);
		this.deregisterOnMouseMove(document, true, this);
		
		this.stopDragThunk();
		
		if (	this._totalDX != 0
			||	this._totalDY != 0)
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
			HMI.KSClient.sendRequest('setvar', '{' + '/TechUnits/HMIManager' + '.Command ' + Command + '}');
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
			fncOnClickRefresh();
		};
		
		delete Clone;
	},
	
	/*********************************
		switchGround
	*********************************/
	switchGround: function (evt, ground) {		
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
					&&	ground._node.id == HMI.CONSTANTS.NODE_NAME_CLONE)
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
			return;
		};

		//	new ground
		//
		if (this._ground != ground)
		{
			
			// firstChild == <g></g>
			//
			var node = this._ground._node.firstChild.removeChild(this._node);
			ground._node.firstChild.insertBefore(node, ground._node.firstChild.firstChild);
			
			var SVGx = evt.layerX - node.ownerSVGElement.getAttribute("layerX") + (-1)*this._innerCursorX;
			var SVGy = evt.layerY - node.ownerSVGElement.getAttribute("layerY") + (-1)*this._innerCursorY;
			node.setAttribute("x", SVGx);
			node.setAttribute("y", SVGy);
			delete SVGx;
			delete SVGy;

			this._ground = ground;
			
			delete node;
		};
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
	},	
};