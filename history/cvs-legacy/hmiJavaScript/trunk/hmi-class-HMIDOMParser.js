/***********************************************************************
	constructor
***********************************************************************/

function HMIDOMParser() {
};

/***********************************************************************
	prototype
***********************************************************************/

HMIDOMParser.prototype = {
	
	/*********************************
		parse
	*********************************/
	parse: function(GraphicDescription, StyleDescription, BehaviourDescription) {
		var Parser = new DOMParser();
		var GraphicElement;
		var StyleElement;
		var Return;
		
		try {
			GraphicElement = Parser.parseFromString(GraphicDescription, "text/xml");	
		} catch (e) {
			alert('HMIDOMParser.parse: Could not parse GraphicDescription');
			return null;
		};
		
		if (GraphicElement.documentElement.namespaceURI == "http://www.mozilla.org/newlayout/xml/parsererror.xml")
		{
			return null;
		} else {
			StyleElement = document.createElementNS('http://www.w3.org/1999/xhtml', 'style');
	   	StyleElement.type = 'text/css';
	   	StyleElement.appendChild(document.createTextNode(StyleDescription));
	   	
			GraphicElement.firstChild.appendChild(StyleElement);
		};
		
		Return = GraphicElement.firstChild;
	   
	   delete Parser;
	   delete GraphicElement;
	   delete StyleElement;
		
		return Return;
	},
};