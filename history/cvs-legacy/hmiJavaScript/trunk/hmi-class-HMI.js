/***********************************************************************
	constructor
***********************************************************************/

function HMI() {
	this.RefreshTimeoutID = null;
	this.KSClient = null;
	
	this._currentDragger = null;
	
	this.CONSTANTS = new HMI_Constants();
	
	this.KSClient = new HMIJavaScriptKSClient();
	
	//this.registerDebug(document, false, this);
};

function HMI_Constants() {
	this.NAMESPACE_SVG = "http://www.w3.org/2000/svg";
		
	this.NODE_NAME_CLONE = "HMI_CLONE";
};

/***********************************************************************
	prototype
***********************************************************************/

HMI.prototype = {	
	/*********************************
		instanceOf
	*********************************/
	instanceOf: function (Node, ClassName) {
		
		if (Node.getAttribute("class") != null)
		{
			//	Space Delimiter
			//
			if (this._instanceOf(Node, ClassName, " ") == true)
			{
				return true;
			} else {
				// COMMA delimeter
				//
				if (this._instanceOf(Node, ClassName, ",") == true)
				{
					return true;
				} else {
					// SEMICOLON delimeter
					//
					if (this._instanceOf(Node, ClassName, ";") == true)
					{
						return true;
					};
				};
			};
		};
	
		return false;
	},
	
	/*********************************
		initGestures
	*********************************/
	initGestures: function () {
		var Elements;
		
		Elements = document.getElementsByTagNameNS(HMI.CONSTANTS.NAMESPACE_SVG, 'svg');
		for (var idx = 0; idx < Elements.length; ++idx)
		{
			//	HMI-COMPONENT
			//
			if (this.instanceOf(Elements[idx], "hmi-component"))
			{
				Elements[idx].setAttribute('pointer-events', 'all');
				this._setLayerPosition(Elements[idx]);
			};
			
			//	GROUND
			//
			if (this.instanceOf(Elements[idx], "hmi-component-ground"))
			{
				var ground = new Ground(Elements[idx], this);
			};
			
			//	MOVE
			//
			if (this.instanceOf(Elements[idx], "hmi-component-gesture-move"))
			{
  				var dragger = new Dragger(Elements[idx], this);
			} else {			
				//	RIGHTCLICK
				//
				if (this.instanceOf(Elements[idx], 'hmi-component-gesture-rightclick'))
				{
					var rightclick = new RightClick(Elements[idx], this);
				};
				
				//	(LEFT)CLICK
				//
				if (this.instanceOf(Elements[idx], 'hmi-component-gesture-click'))
				{
					var click = new Click(Elements[idx], this);
				};
				
				//	DOUBLECLICK
				//
				if (this.instanceOf(Elements[idx], 'hmi-component-gesture-doubleclick'))
				{
					var doubleclick = new DoubleClick(Elements[idx], this);
				};				
			};
		};
		
		delete Elements;
	},
	
	/*********************************
		getComponent
	*********************************/
	getComponent : function (evt, class) {
		var Component = null;
		
		Component = evt.target;
		while (	Component != null
				&&	Component != document
				&&	HMI.instanceOf(Component, class) == false)
		{
			Component = Component.ownerSVGElement;
			if (	Component != null
				&&	Component.id == HMI.CONSTANTS.NODE_NAME_CLONE)
			{
				Component = document.getElementById(Component.getAttribute('clonedID'));
			};
		};
		
		return Component;
	},
	
	/*********************************
		addComponent
	*********************************/
	addComponent: function (ParentID, ComponentPath) {
		//	Variables
		//
		var Descriptions = new Array(2);
		
		//	get Descriptions
	   //
	   Descriptions = this._getDescriptions(ComponentPath);
	   
	   //	add Descriptions
	   //
	   this._addDescriptions(ParentID, Descriptions);
	   
	   delete Descriptions;
	},
	
	/*********************************
		replaceComponent
	*********************************/
	replaceComponent: function (ParentID, ComponentPath) {
		//	Variables
		//
		var Descriptions = new Array(2);
		
		//	get Descriptions
	   //
	   Descriptions = this._getDescriptions(ComponentPath);
	   
	   //	replace Descriptions
	   //
	   this._replaceDescriptions(ParentID, Descriptions);
	   
	   delete Descriptions;
	},
	
	/*********************************
		_instanceOf
	*********************************/
	_instanceOf: function (Node, ClassName, Delimiter) {
		var Classes;
		var idx;
		
		Classes = Node.getAttribute("class").split(Delimiter);
		for (idx = 0; idx < Classes.length; idx++)
		{
			if (ClassName == Classes[idx])
				return true;
		};
		
		delete Classes;
		delete idx;
		
		return false		
	},
	
	/*********************************
		_setLayerPosition
	*********************************/
	_setLayerPosition: function (Element) {
		var LayerX = Element.x.animVal.value;
		var LayerY = Element.y.animVal.value;
		
		if (	Element.ownerSVGElement != null
			&&	Element.ownerSVGElement != document)
		{
			LayerX += parseInt(Element.ownerSVGElement.getAttribute("layerX"));
			LayerY += parseInt(Element.ownerSVGElement.getAttribute("layerY"));
		};
		
		Element.setAttribute("layerX", LayerX);
		Element.setAttribute("layerY", LayerY);
		
		delete LayerX;
		delete LayerY;
	},
	
	/*********************************
		_getDescriptions
	*********************************/
	_getDescriptions: function (Path) {
		var Descriptions = new Array(2);
		return HMI.KSClient.sendRequest('getvar', '{' + Path + '.GraphicDescription' + ' ' + Path + '.StyleDescription' + '}' + ' ' + '-output $::TKS::OP_VALUE');
	},
	
	/*********************************
		_addDescriptions
	*********************************/
	_addDescriptions: function (ParentID, Descriptions) {
		var Component = null;
		var Parser = new HMIDOMParser();
		
	  	 Component = Parser.parse(Descriptions[0], Descriptions[1], null);
	   document.getElementById(ParentID).appendChild(Component);
	   
	   delete Component;
	   delete Parser;
	},
	
	/*********************************
		_replaceDescriptions
	*********************************/
	_replaceDescriptions: function (ParentID, Descriptions) {
		var Component = null;
		var Parser = new HMIDOMParser();
		
	   Component = Parser.parse(Descriptions[0], Descriptions[1], null);
	   document.getElementById(ParentID).replaceChild(Component, document.getElementById(Component.getAttribute('id')));
	   
	   delete Component;
	   delete Parser;
	},
	
	/*********************************
		unload
	*********************************/
	unload: function () {
		if (HMI.KSClient.TCLKSHandle != null)
		{
			HMI.KSClient.destroy();
			delete HMI.KSClient;
		};
		delete HMI;		
	},
	
	/*********************************
		switchGround
	*********************************/
	switchGround: function (evt, ground) {
		if (this._currentDragger != null)
		{
			this._currentDragger.switchGround(evt, ground);
		};
	},
	
	/*********************************
		armDebug
	*********************************/
	armDebug: function () {
		this.registerDebug(document, false, this);
	},
	
	/*********************************
		registerDebug
	*********************************/
	registerDebug: function(element, capture, listener) {
		this.onDebugThunk = function (evt) {
			listener.onDebug(evt);
		};
		element.addEventListener("mousemove", this.onDebugThunk, capture);
	},

	/*********************************
		onDebug
	*********************************/
	onDebugThunk: null,
	onDebug: function (evt) {
		document.getElementById('idDebug').innerHTML = evt.target.nodeName
			+ ' - client: ' + evt.clientX
			+ ' ' + evt.clientY
			+ ' - layer: ' + evt.layerX
			+ ' ' + evt.layerY
			+ ' - page: ' + evt.pageX
			+ ' ' + evt.pageY
			+ ' - screen:' + evt.screenX
			+ ' ' + evt.screenY;
	},
};